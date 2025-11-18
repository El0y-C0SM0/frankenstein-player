#include "core/services/Player.hpp"
#include <cassert>
#include <memory>

namespace core {

    Player::Player()
        : _currentQueueIndex(-1),
          _currentSongIndex(-1),
          _playerState(PlayerState::STOPPED),
          _isLooping(false),
          _volume(1.0f),
          _previousVolume(1.0f),
          _audioInitialized(false) {

        ma_result result = ma_engine_init(NULL, &_audioEngine);
        if (result != MA_SUCCESS) {
            std::cerr << "Falha ao iniciar Audio Engine: " << result << std::endl;
            _audioInitialized = false;
        } else {
            _audioInitialized = true;
            std::cout << "Audio engine inicializado" << std::endl;
        }
        // inicializando um som vazio.
        ma_sound_config soundConfig = ma_sound_config_init();
        soundConfig.pFilePath = "";
        ma_sound_init_ex(&_audioEngine, &soundConfig, &_currentSound);
    }

    Player::Player(const core::PlaybackQueue &tracks) : Player() {
        addPlaybackQueue(tracks);
    }

    Player::~Player() {
        cleanupCurrentSound();
        if (_audioInitialized) {
            ma_engine_uninit(&_audioEngine);
        }
    }

    std::shared_ptr<PlaybackQueue> Player::getCurrentQueue() const {
        if (_currentQueueIndex >= 0 && _currentQueueIndex < _queue.size()) {
            return _queue[_currentQueueIndex];
        }
        return nullptr;
    }

    void Player::sound_end_callback(void *pUserData, ma_sound *pSound) {
        // Manter pSound, pois é preciso para miniaudio
        Player *player = static_cast<Player *>(pUserData);
        if (player && !player->_isLooping) {
            player->playNextSong();
        }
    }

    bool Player::loadCurrentSong() {
        if (!_audioInitialized) {
            return false;
        }

        auto currentQueue = getCurrentQueue();
        if (!currentQueue || _currentSongIndex < 0 || _currentSongIndex >= currentQueue->size()) {
            return false;
        }

        cleanupCurrentSound();

        _currentSong = currentQueue->at(_currentSongIndex);
        if (!_currentSong) {
            return false;
        }
        // TODO logica e filepath no construtor de BD
        std::string filePath = _currentSong->getFilePath();

        ma_result result = ma_sound_init_from_file(&_audioEngine, filePath.c_str(), 0, NULL, NULL, &_currentSound);

        if (result != MA_SUCCESS) {
            std::cerr << "Failed to load sound: " << filePath << std::endl;
            return false;
        }

        // Configurar callback para quando o som terminar
        ma_sound_set_end_callback(&_currentSound, sound_end_callback, this);

        // Configurar volume
        ma_sound_set_volume(&_currentSound, _volume);

        // Configurar looping se necessário
        ma_sound_set_looping(&_currentSound, _isLooping);

        return true;
    }

    void Player::cleanupCurrentSound() {
        if (ma_sound_is_playing(&_currentSound)) {
            ma_sound_stop(&_currentSound);
        }
        ma_sound_uninit(&_currentSound);
    }
    void Player::addPlaybackQueue(const core::PlaybackQueue &tracks) {
        _queue.push_back(std::make_shared<core::PlaybackQueue>(tracks));

        if (_currentQueueIndex == -1 && !_queue.empty()) {
            _currentQueueIndex = 0;
            if (_queue[0]->size() > 0) {
                _currentSongIndex = 0;
            }
        }
    }

    void Player::play() {
        if (!_audioInitialized) {
            return;
        }

        if (_playerState == PlayerState::PAUSED) {
            resume();
            return;
        }

        if (_currentQueueIndex == -1 && !_queue.empty()) {
            _currentQueueIndex = 0;
        }

        auto currentQueue = getCurrentQueue();
        if (!currentQueue || currentQueue->empty())
            return;

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
        if (ma_sound_is_playing(&_currentSound)) {
            ma_sound_stop(&_currentSound);
            _playerState = PlayerState::PAUSED;
        }
    }

    void Player::playNextSong() {
        auto currentQueue = getCurrentQueue();
        if (!currentQueue || currentQueue->empty()) {
            return;
        }

        int nextIndex = _currentSongIndex + 1;

        if (nextIndex >= currentQueue->size()) {
            if (_isLooping) {
                nextIndex = 0;
            } else {
                int nextQueueIndex = _currentQueueIndex + 1;
                if (nextQueueIndex < _queue.size() && !_queue[nextQueueIndex]->empty()) {
                    _currentQueueIndex = nextQueueIndex;
                    _currentSongIndex = 0;
                } else {
                    _playerState = PlayerState::STOPPED;
                }
            }
        } else {
            _currentSongIndex = nextIndex;
        }

        if (loadCurrentSong()) {
            ma_sound_start(&_currentSound);
            _playerState = PlayerState::PLAYING;
        }
    }

    void Player::resume() {
        if (_playerState == PlayerState::PAUSED) {
            ma_sound_start(&_currentSound);
            _playerState = PlayerState::PLAYING;
        }
    }

    void Player::restart() {
        if (getCurrentQueue()) {
            ma_sound_stop(&_currentSound);
            ma_sound_seek_to_pcm_frame(&_currentSound, 0);
            ma_sound_start(&_currentSound);
            _playerState = PlayerState::PLAYING;
        }
    }

    void Player::rewind(unsigned int seconds) {
        seek(-static_cast<int>(seconds));
    }

    void Player::seek(int seconds) {

        if (!_currentSong) {
            return;
        }

        ma_uint64 currentFrame;
        ma_sound_get_cursor_in_pcm_frames(&_currentSound, &currentFrame);

        ma_uint64 sampleRate = ma_engine_get_sample_rate(&_audioEngine);
        ma_int64 framesToSeek = static_cast<ma_int64>(seconds) * static_cast<ma_int64>(sampleRate);
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

    void Player::fastForward(unsigned int seconds) {
        seek(static_cast<int>(seconds));
    }

    unsigned int Player::getElapsedTime() const {
        if (!_currentSong)
            return 0;

        ma_uint64 currentFrame;
        ma_sound_get_cursor_in_pcm_frames(&_currentSound, &currentFrame);

        ma_uint64 sampleRate = ma_engine_get_sample_rate(&_audioEngine);
        return static_cast<unsigned int>(currentFrame / sampleRate);
    }

    void Player::next() {
        playNextSong();
    }

    void Player::previous() {
        auto currentQueue = getCurrentQueue();

        if (!currentQueue || currentQueue->empty()) {
            return;
        }

        int prevIndex = _currentSongIndex - 1;

        if (prevIndex < 0) {
            if (_isLooping) {
                prevIndex = currentQueue->size() - 1;
            } else {
                int prevQueueIndex = _currentQueueIndex - 1;

                if (prevQueueIndex >= 0 && !_queue[prevQueueIndex]->empty()) {
                    _currentQueueIndex = prevQueueIndex;
                    _currentSongIndex = _queue[prevQueueIndex]->size() - 1;
                } else {
                    return;
                }
            }
        } else {
            _currentSongIndex = prevIndex;
        }

        if (loadCurrentSong()) {
            ma_sound_start(&_currentSound);
            _playerState = PlayerState::PLAYING;
        }
    }

    void Player::setLooping() {
        _isLooping = true;
        ma_sound_set_looping(&_currentSound, MA_TRUE);
    }

    void Player::unsetLooping() {
        _isLooping = false;
        ma_sound_set_looping(&_currentSound, MA_FALSE);
    }

    void Player::setVolume(float volume) {
        _volume = std::max(0.0f, std::min(volume, 1.0f));
        ma_sound_set_volume(&_currentSound, _volume);
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
        return _playerState;
    }

    bool Player::isPlaying() const {
        return _playerState == PlayerState::PLAYING && ma_sound_is_playing(&_currentSound);
    }

    bool Player::isPaused() const {
        return _playerState == PlayerState::PAUSED;
    }

    bool Player::isLooping() const {
        return _isLooping;
    }
    float Player::getProgress() const {
        if (!_currentSong)
            return 0.0f;

        ma_uint64 lengthInFrames;
        ma_uint64 currentFrame;

        // Obter o comprimento total do som (requer ponteiro para armazenar o resultado)
        ma_result lengthResult = ma_sound_get_length_in_pcm_frames(&_currentSound, &lengthInFrames);

        // Obter a posição atual
        ma_result cursorResult = ma_sound_get_cursor_in_pcm_frames(&_currentSound, &currentFrame);

        if (lengthResult != MA_SUCCESS || cursorResult != MA_SUCCESS || lengthInFrames == 0) {
            return 0.0f;
        }

        return static_cast<float>(currentFrame) / static_cast<float>(lengthInFrames);
    }

    int Player::getPlaylistSize() const {
        auto currentQueue = getCurrentQueue();
        if (!currentQueue)
            return 0;
        return currentQueue->size();
    }

    const std::shared_ptr<PlaybackQueue> Player::getPlaybackQueue() const {
        return getCurrentQueue();
    }

    void Player::clearPlaylist() {
        pause();
        cleanupCurrentSound();
        _queue.clear();
        _currentQueueIndex = -1;
        _currentSongIndex = -1;
        _currentSong.reset();
        _playerState = PlayerState::STOPPED;
    }

    bool Player::hasNext() const {
        auto currentQueue = getCurrentQueue();
        if (!currentQueue)
            return false;

        return (_currentSongIndex + 1 < currentQueue->size()) ||
               (_currentQueueIndex + 1 < _queue.size());
    }

    bool Player::hasPrevious() const {
        auto currentQueue = getCurrentQueue();
        if (!currentQueue)
            return false;

        return (_currentSongIndex > 0) ||
               (_currentQueueIndex > 0);
    }
}; // namespace core
