#define MINIAUDIO_IMPLEMENTATION
#include "core/services/Player.hpp"
#include "miniaudio.h"
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <atomic>
#include <thread>
#include <chrono>

namespace core {

    void Player::onSoundEnd(void* pUserData, ma_sound* pSound) {
        Player* player = static_cast<Player*>(pUserData);
        if (player && !player->_isLooping) {
            player->_shouldAdvanceToNext.store(true, std::memory_order_release);

            std::thread([player]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                player->checkAndAdvanceIfNeeded();
            }).detach();
        }
    }

    Player::Player()
        : _currentQueueIndex(-1),
          _currentSongIndex(-1),
          _playerState(PlayerState::STOPPED),
          _isLooping(false),
          _volume(1.0f),
          _previousVolume(1.0f),
          _audioInitialized(false),
          _shouldAdvanceToNext(false) {
        ma_result result = ma_engine_init(NULL, &_audioEngine);
        if (result != MA_SUCCESS) {
            throw std::runtime_error("Falha ao inicializar Audio Engine: "
                                     + std::to_string(result));
        }

        _audioInitialized = true;
        memset(&_currentSound, 0, sizeof(_currentSound));

        std::cout << "Audio engine inicializado" << std::endl;

        _queue = std::make_shared<core::PlaybackQueue>();
    }

    Player::Player(const core::PlaybackQueue& tracks)
        : Player() {
        addPlaybackQueue(tracks);
    }

    Player::~Player() {
        cleanupCurrentSound();
        if (_audioInitialized) {
            ma_engine_uninit(&_audioEngine);
        }
    }

    std::shared_ptr<PlaybackQueue> Player::getCurrentQueue() const {
        if (_currentQueueIndex >= 0
            && static_cast<size_t>(_currentQueueIndex) < _queues.size()) {
            return _queues[_currentQueueIndex];
        }
        return nullptr;
    }

    ma_uint64 Player::getEngineTime() const {
        if (!_audioInitialized) {
            return 0;
        }
        return ma_engine_get_time(&_audioEngine);
    }

    void Player::cleanupCurrentSound() {
        if (_currentSound.pDataSource == nullptr) {
            return;
        }

        if (ma_sound_is_playing(&_currentSound)) {
            ma_sound_stop(&_currentSound);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        ma_sound_uninit(&_currentSound);
        memset(&_currentSound, 0, sizeof(_currentSound));
    }

    bool Player::loadCurrentSong() {
        if (!_audioInitialized) {
            throw std::runtime_error("Audio engine não inicializado");
        }

        if (!_queue) {
            throw std::runtime_error("Queue inválida");
        }

        if (_currentSongIndex < 0
            || static_cast<size_t>(_currentSongIndex) >= _queue->size()) {
            throw std::runtime_error("Índice inválido");
        }

        cleanupCurrentSound();

        _currentSong = _queue->getCurrentSong();
        if (!_currentSong) {
            throw std::runtime_error("Música nula");
        }

        std::string filePath = _currentSong->getAudioFilePath();
        if (filePath.empty()) {
            throw std::runtime_error("Caminho vazio");
        }

        ma_uint32 flags = MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC;

        ma_result result = ma_sound_init_from_file(
            &_audioEngine, filePath.c_str(), flags, NULL, NULL, &_currentSound);

        if (result != MA_SUCCESS) {
            std::cerr << "Erro ao carregar: " << result << std::endl;
            return false;
        }

        ma_sound_set_end_callback(&_currentSound, onSoundEnd, this);

        ma_sound_set_volume(&_currentSound, _volume);
        ma_sound_set_looping(&_currentSound, _isLooping ? MA_TRUE : MA_FALSE);
        ma_sound_seek_to_pcm_frame(&_currentSound, 0);

        return true;
    }

    void Player::addPlaybackQueue(const core::PlaybackQueue& tracks) {
        if (tracks.empty()) {
            throw std::invalid_argument("PlaybackQueue nao pode ser vazia");
        }

        *_queue += tracks;
    }

    void Player::checkAndAdvanceIfNeeded() {
        if (_shouldAdvanceToNext.load(std::memory_order_acquire)) {
            _shouldAdvanceToNext.store(false, std::memory_order_release);
            playNextSong();
        }
    }

    void Player::play() {
        if (!_audioInitialized) {
            throw std::runtime_error("Audio engine não inicializado");
        }

        if (_playerState == PlayerState::PAUSED) {
            resume();
            return;
        }

        if (_currentQueueIndex == -1 && !_queues.empty()) {
            _currentQueueIndex = 0;
        }

        if (!_queue || _queue->empty()) {
            return;
        }

        if (_currentSongIndex == -1) {
            _currentSongIndex = 0;
        }

        if (loadCurrentSong()) {
            ma_result result = ma_sound_start(&_currentSound);
            if (result == MA_SUCCESS) {
                _playerState = PlayerState::PLAYING;
            }
        }
    }

    void Player::pause() {
        if (_playerState == PlayerState::PLAYING
            && ma_sound_is_playing(&_currentSound)) {
            ma_sound_stop(&_currentSound);
            _playerState = PlayerState::PAUSED;
        }
    }

    void Player::resume() {
        if (_playerState == PlayerState::PAUSED
            && _currentSound.pDataSource != nullptr) {
            ma_sound_start(&_currentSound);
            _playerState = PlayerState::PLAYING;
        }
    }

    void Player::restart() {
        if (_currentSound.pDataSource != nullptr) {
            ma_sound_stop(&_currentSound);
            ma_sound_seek_to_pcm_frame(&_currentSound, 0);

            ma_sound_start(&_currentSound);
            _playerState = PlayerState::PLAYING;
        }
    }

    void Player::playNextSong() {
        if (!_queue || _queue->empty()) {
            _playerState = PlayerState::STOPPED;
            return;
        }

        auto nextSong = _queue->next();

        if (!nextSong) {
            _playerState = PlayerState::STOPPED;
            cleanupCurrentSound();
            return;
        }

        if (loadCurrentSong()) {
            ma_result result = ma_sound_start(&_currentSound);
            if (result == MA_SUCCESS) {
                _playerState = PlayerState::PLAYING;
            } else {
                std::cerr << "Erro ao iniciar som: " << result << std::endl;
            }
        }
    }

    void Player::next() {
        playNextSong();
    }

    void Player::previous() {
        if (!_queue || _queue->empty()) {
            throw std::runtime_error("Queue não inicializada");
        }

        auto prevSong = _queue->previous();

        if (!prevSong)
            return;

        if (loadCurrentSong()) {
            ma_sound_start(&_currentSound);
            _playerState = PlayerState::PLAYING;
        }
    }

    void Player::seek(int seconds) {
        if (!_currentSong || _currentSound.pDataSource == nullptr) {
            throw std::runtime_error("Música não carregada");
        }

        ma_uint64 currentFrame;
        ma_sound_get_cursor_in_pcm_frames(&_currentSound, &currentFrame);

        ma_uint64 sampleRate = ma_engine_get_sample_rate(&_audioEngine);
        ma_int64 framesToSeek =
            static_cast<ma_int64>(seconds) * static_cast<ma_int64>(sampleRate);
        ma_uint64 newFrame;

        if (framesToSeek < 0) {
            if (static_cast<ma_uint64>(-framesToSeek) > currentFrame) {
                newFrame = 0;
            } else {
                newFrame = currentFrame - static_cast<ma_uint64>(-framesToSeek);
            }
        } else {
            newFrame = currentFrame + static_cast<ma_uint64>(framesToSeek);
        }

        ma_sound_seek_to_pcm_frame(&_currentSound, newFrame);

    }
    void Player::rewind(unsigned int seconds) {
        seek(-static_cast<int>(seconds));
    }

    void Player::fastForward(unsigned int seconds) {
        seek(static_cast<int>(seconds));
    }

    void Player::setLooping() {
        _isLooping = true;
        if (_currentSound.pDataSource != nullptr) {
            ma_sound_set_looping(&_currentSound, MA_TRUE);
        }
    }

    void Player::unsetLooping() {
        _isLooping = false;
        if (_currentSound.pDataSource != nullptr) {
            ma_sound_set_looping(&_currentSound, MA_FALSE);
        }
    }

    bool Player::isLooping() const {
        return _isLooping;
    }

    void Player::setVolume(float volume) {
        _volume = std::max(0.0f, std::min(volume, 1.0f));
        if (_currentSound.pDataSource != nullptr) {
            ma_sound_set_volume(&_currentSound, _volume);
        }
    }

    float Player::getVolume() const {
        return _volume;
    }

    void Player::mute() {
        _previousVolume = _volume;
        setVolume(0.0f);
    }

    void Player::unmute() {
        setVolume(_previousVolume);
    }

    PlayerState Player::stateOfPlayer() const {
        const_cast<Player*>(this)->checkAndAdvanceIfNeeded();
        return _playerState;
    }

    bool Player::isMuted() const {
        return _volume == 0.0f;
    }

    bool Player::isPlaying() const {
        const_cast<Player*>(this)->checkAndAdvanceIfNeeded();

        return _playerState == PlayerState::PLAYING
               && _currentSound.pDataSource != nullptr
               && ma_sound_is_playing(&_currentSound);
    }

    bool Player::isPaused() const {
        return _playerState == PlayerState::PAUSED;
    }

    unsigned int Player::getElapsedTime() const {
        if (!_currentSong || _currentSound.pDataSource == nullptr) {
            return 0;
        }

        ma_uint64 currentFrame;
        ma_result result =
            ma_sound_get_cursor_in_pcm_frames(&_currentSound, &currentFrame);

        if (result != MA_SUCCESS) {
            return 0;
        }

        ma_uint64 sampleRate = ma_engine_get_sample_rate(&_audioEngine);
        return static_cast<unsigned int>(currentFrame / sampleRate);
    }
    float Player::getProgress() const {
        const_cast<Player*>(this)->checkAndAdvanceIfNeeded();

        if (!_currentSong || _currentSound.pDataSource == nullptr) {
            return 0.0f;
        }

        ma_uint64 lengthInFrames;
        ma_result lengthResult =
            ma_sound_get_length_in_pcm_frames(&_currentSound, &lengthInFrames);

        if (lengthResult != MA_SUCCESS || lengthInFrames == 0) {
            return 0.0f;
        }

        unsigned int elapsed = getElapsedTime();
        ma_uint64 sampleRate = ma_engine_get_sample_rate(&_audioEngine);
        ma_uint64 elapsedFrames = elapsed * sampleRate;

        return std::min(1.0f,
                        static_cast<float>(elapsedFrames)
                            / static_cast<float>(lengthInFrames));
    }

    int Player::getPlaylistSize() const {
        if (!_queue) {
            throw std::runtime_error("Queue não inicializada");
        }
        return _queue->size();
    }

    std::shared_ptr<PlaybackQueue> Player::getPlaybackQueue() const {
        return _queue;
    }

    void Player::clearPlaylist() {
        pause();
        cleanupCurrentSound();
        _queues.clear();
        _currentQueueIndex = -1;
        _currentSongIndex = -1;
        _currentSong.reset();
        _playerState = PlayerState::STOPPED;
    }

    bool Player::hasNext() const {
        return _queue->getNextSong() != nullptr;
    }

    bool Player::hasPrevious() const {
        return _queue->getPreviousSong() != nullptr;
    }

} // namespace core
