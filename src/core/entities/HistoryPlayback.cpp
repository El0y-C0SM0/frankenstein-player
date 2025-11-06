/**
 * @file HistoryPlaybackRepository.cpp
 * @brief Implementação do histórico de reproduções
 *
 * @ingroup entities
 * @author Eloy Maciel
 * @date 2025-11-05
 */

#include "core/entities/HistoryPlayback.hpp"
#include <string>

namespace core {
    HistoryPlayback::HistoryPlayback() :
        Entity(),
        _user(nullptr),
        _song(nullptr),
        _played_at(std::time(nullptr)) {}

    HistoryPlayback::HistoryPlayback(User& user, Song& song) :
        Entity(),
        _user(std::make_shared<User>(user)),
        _song(std::make_shared<Song>(song)),
        _played_at(std::time(nullptr)) {}

    HistoryPlayback::HistoryPlayback(unsigned id,
                                     User& user,
                                     Song& song) :
        Entity(id),
        _user(std::make_shared<User>(user)),
        _song(std::make_shared<Song>(song)),
        _played_at(std::time(nullptr)) {}

    HistoryPlayback::HistoryPlayback(unsigned id,
                                     User& user,
                                     Song& song,
                                     std::time_t played_at) :
        Entity(id),
        _user(std::make_shared<User>(user)),
        _song(std::make_shared<Song>(song)),
        _played_at(played_at) {}

    HistoryPlayback::HistoryPlayback(User& user,
                                     Song& song,
                                     std::time_t played_at) :
        Entity(),
        _user(std::make_shared<User>(user)),
        _song(std::make_shared<Song>(song)),
        _played_at(played_at) {}

    std::shared_ptr<const User> HistoryPlayback::getUser() const {
        return _user;
    }

    void HistoryPlayback::setUser(const User& user) {
        _user = std::make_shared<User>(user);
    }

    std::shared_ptr<const Song> HistoryPlayback::getSong() const {
        return _song;
    }

    void HistoryPlayback::setSong(const Song& song) {
        _song = std::make_shared<Song>(song);
    }

    std::time_t HistoryPlayback::getPlayedAt() const {
        return _played_at;
    }

    void HistoryPlayback::setPlayedAt(std::time_t played_at) {
        _played_at = played_at;
    }

    std::string HistoryPlayback::toString() const {
        return "HistoryPlayback{id=" + std::to_string(getId()) +
               ", user=" + (_user ? std::to_string(_user->getId()) : "null") +
               ", song=" + (_song ? std::to_string(_song->getId()) : "null") +
               ", played_at=" + std::to_string(_played_at) + "}";
    }

    bool HistoryPlayback::operator==(const Entity& other) const {
        const HistoryPlayback* other_history =
            dynamic_cast<const HistoryPlayback*>(&other);

        if (!other_history)
            return false;

        return getId() == other_history->getId() &&
               ((_user && other_history->getUser() &&
                 _user->getId() == other_history->_user->getId()) ||
                (!_user && !other_history->_user)) &&
               ((_song && other_history->_song &&
                 _song->getId() == other_history->_song->getId()) ||
                (!_song && !other_history->_song)) &&
               _played_at == other_history->_played_at;
    }

    bool HistoryPlayback::operator!=(const Entity& other) const {
        return !(*this == other);
    }
}
