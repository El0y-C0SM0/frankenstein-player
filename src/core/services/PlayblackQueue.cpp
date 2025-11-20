/**
 * @file PlaybackQueue.hpp
 * @brief Implementação da fila de reprodução
 *
 * @ingroup services
 * @author Eloy Maciel
 * @date 2025-11-03
 */

#include "core/entities/Song.hpp"
#include "core/services/PlaybackQueue.hpp"

#include <cassert>
#include <cstddef>
#include <algorithm>
#include <random>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace core {
    PlaybackQueue::PlaybackQueue()
        : _current(0),
        _max_size(MAX_SIZE_DEFAULT),
        _aleatory(false),
        _history_repo(nullptr),
        _current_user(nullptr) {}

    PlaybackQueue::PlaybackQueue(std::shared_ptr<User> current_user,
                                const IPlayable& playable,
                                std::shared_ptr<HistoryPlaybackRepository> history_repo,
                                size_t max_size)
        : _current(0),
        _max_size(max_size),
        _aleatory(false),
        _history_repo(history_repo),
        _current_user(current_user) {
        add(playable);
    }

    PlaybackQueue::~PlaybackQueue() = default;

    size_t PlaybackQueue::getCurrentIndex() const {
        return _aleatory ? _indices_aleatory[_current] : _current;
    }

    void PlaybackQueue::add(const IPlayable& tracks) {
        auto new_songs = tracks.getPlayableObjects();

        unsigned count {0};
        size_t initial_size = _queue.size();
        for (const auto& song : new_songs) {
            if (_queue.size() < _max_size) {
                _queue.push_back(std::dynamic_pointer_cast<Song>(song));
                count++;
            } else {
                throw std::length_error("PlaybackQueue reached its maximum size");
            }
        }

        std::vector<size_t> new_indices;
        for (size_t i = 0; i < count; ++i)
            new_indices.push_back(initial_size + i);

        if (_aleatory) {
            auto rng = std::default_random_engine{std::random_device{}()};
            std::shuffle(_indices_aleatory.begin(), _indices_aleatory.end(), rng);
        }

        _indices_aleatory.insert(_indices_aleatory.end(),
                                new_indices.begin(),
                                new_indices.end());
    }

    void PlaybackQueue::operator+=(const IPlayable& tracks) {
        add(tracks);
    }

    void PlaybackQueue::add(const PlaybackQueue& other_queue) {
        size_t initial_size = _queue.size(), size_other = other_queue.size();
        unsigned count {0};

        for (size_t i = 0; i < size_other; ++i) {
            if (_queue.size() < _max_size) {
                _queue.push_back(other_queue.at(i));
                count++;
            } else {
                throw std::length_error("PlaybackQueue reached its maximum size");
            }
        }

        std::vector<size_t> new_indices;
        for (size_t i = 0; i < count; ++i)
            new_indices.push_back(initial_size + i);

        if (_aleatory) {
            auto rng = std::default_random_engine{std::random_device{}()};
            std::shuffle(_indices_aleatory.begin(), _indices_aleatory.end(), rng);
        }

        _indices_aleatory.insert(_indices_aleatory.end(),
                                new_indices.begin(),
                                new_indices.end());
    }

    void PlaybackQueue::operator+=(const PlaybackQueue& other_queue) {
        add(other_queue);
    }

    bool PlaybackQueue::remove(size_t index) {
        if (index >= _queue.size()) {
            return false;
        }

        size_t actual_index = _aleatory ? _indices_aleatory[index] : index;

        _queue.erase(_queue.begin() + actual_index);

        if ((_current > actual_index && _current > 0) ||
            (_current == actual_index && _current == _queue.size() && _current > 0))
            (_current)--;

        return true;
    }

    int PlaybackQueue::findNextIndex(const Song& song) const {
        for (size_t i = _current + 1; i < _queue.size(); ++i) {
            if ((!_aleatory && *_queue[i] == song) ||
                (_aleatory && *_queue[_indices_aleatory[i]] == song))
                return i;
        }

        return -1;
    }

    int PlaybackQueue::findCurrentIndex() const {
        if (_queue.empty()) return -1;
        return static_cast<int>(getCurrentIndex());
    }

    int PlaybackQueue::findPreviousIndex() const {
        if (_queue.empty()) return -1;

        return static_cast<int>(getCurrentIndex() == 0 ? 0 : getCurrentIndex() - 1);
    }

    std::shared_ptr<Song> PlaybackQueue::at(size_t index) const {
        if (index >= _queue.size())
            return nullptr;

        size_t actual_index = _aleatory ? _indices_aleatory[index] : index;

        return _queue[actual_index];
    }

    std::shared_ptr<Song> PlaybackQueue::getNextSong() {
        if (_queue.empty() || _current >= _queue.size())
            return nullptr;

        if (_current + 1 == _current && _loop)
            return at(0);
        else if (_current + 1 >= _queue.size())
            return nullptr;

        return at(_current + 1);
    }

    std::shared_ptr<const Song> PlaybackQueue::getCurrentSong() const {
        if (_queue.empty() || _current >= _queue.size())
            return nullptr;

        return _queue[getCurrentIndex()];
    }

    std::shared_ptr<const Song> PlaybackQueue::getPreviousSong() const {
        if (_queue.empty() || (_current == 0 && !_loop))
            return nullptr;

        if (_current == 0 && _loop)
            return at(_queue.size() - 1);

        return at(_current - 1);
    }

    std::shared_ptr<const Song> PlaybackQueue::next() {
        if (_queue.empty() || _current >= _queue.size())
            return nullptr;

        if (_current + 1 == _queue.size() && _loop) {
            _current = 0;
        } else if (_current + 1 >= _queue.size()) {
            return nullptr;
        } else {
            _current++;
        }

        return _queue[getCurrentIndex()];
    }

    std::shared_ptr<const Song> PlaybackQueue::operator++() {
        return next();
    }

    std::shared_ptr<const Song> PlaybackQueue::previous() {
        if (_queue.empty() || (_current == 0 && !_loop))
            return nullptr;

        if (_current == 0 && _loop) {
            _current = _queue.size() - 1;
        } else {
            _current--;
        }

        return _queue[getCurrentIndex()];
    }

    std::shared_ptr<const Song> PlaybackQueue::operator--() {
        return previous();
    }

    std::vector<std::shared_ptr<const Song>> PlaybackQueue::getQueueView(size_t before,
                                                        size_t after) const {
        std::vector<std::shared_ptr<const Song>> view;

        if (_queue.empty())
            return view;

        size_t start = std::max(static_cast<int>(_current - before), 0);
        size_t end = std::min(_current + after, _queue.size() - 1);

        for (size_t i = start; i <= end; ++i)
            view.push_back(at(i));

        return view;
    }

    std::vector<std::shared_ptr<const Song>> PlaybackQueue::getQueueSegment(size_t start,
                                                           size_t count) const {
        std::vector<std::shared_ptr<const Song>> segment;

        if (_queue.empty() || start >= _queue.size())
            return segment;

        size_t end = std::min(start + count, _queue.size());

        for (size_t i = start; i < end; ++i)
            segment.push_back(at(i));

        return segment;
    }

    void PlaybackQueue::clear() {
        _queue.clear();
        _current = 0;
        _indices_aleatory.clear();
    }

    size_t PlaybackQueue::size() const {
        return _queue.size();
    }

    bool PlaybackQueue::empty() const {
        return _queue.empty();
    }

    void PlaybackQueue::setAleatory(bool aleatory) {
        _aleatory = aleatory;
        if (_aleatory) shuffle();
    }

    bool PlaybackQueue::toggleAleatory() {
        setAleatory(!_aleatory);
        return _aleatory;
    }

    bool PlaybackQueue::isAleatory() const {
        return _aleatory;
    }

    void PlaybackQueue::setLoop(bool loop) {
        _loop = loop;
    }

    bool PlaybackQueue::toggleLoop() {
        setLoop(!_loop);
        return _loop;
    }

    bool PlaybackQueue::isLoop() const {
        return _loop;
    }

    void PlaybackQueue::shuffle() {
        if (_queue.empty())
             return;

        auto rng = std::default_random_engine{std::random_device{}()};
        std::shuffle(_indices_aleatory.begin(), _indices_aleatory.end(), rng);
    }

    std::string PlaybackQueue::toString() const {
        std::string result = "PlaybackQueue ("
            + std::to_string(_queue.size()) + " songs) in "
            + (_aleatory ? "aleatory" : "sequential") + " mode, "
            + (_loop ? "looping" : "not looping") + ".\n";

        return result;
    }

    std::string PlaybackQueue::toStringDetailed() const {
        std::string result = "PlaybackQueue:\n";
        result += "Total Songs: " + std::to_string(_queue.size()) + "\n";
        result += "Current Index: " + std::to_string(_current) + "\n";
        result += "Mode: " + std::string(_aleatory ? "Aleatory" : "Sequential") + "\n";
        result += "Looping: " + std::string(_loop ? "Enabled" : "Disabled") + "\n";
        result += "Songs:\n";
        result += "[";

        for (size_t i = 0; i < _queue.size(); ++i) {
            result += " (" + std::to_string(i) + ", " + _queue[i]->getTitle() + ")";
            if (i < _queue.size() - 1) result += ",";
            result += " ";
        }
        result += "]\n";

        return result;
    }
}
