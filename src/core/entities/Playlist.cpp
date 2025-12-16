#include "core/entities/Playlist.hpp"

#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"
#include <vector>


namespace core {

	Playlist::Playlist() : Entity(0), _title(""), _user(nullptr) {}

	Playlist::Playlist(const unsigned id, const std::string title)
		: Entity(id), _title(title), _user(nullptr), _songs(), _loader() {}

	Playlist::Playlist(const unsigned id,
                       const std::string title,
                       const User &user)
        : Entity(id),
          _title(title),
          _user(std::make_shared<User>(user)),
          _songs(),
          _loader() {}

	std::string Playlist::getTitle() const { return _title; }

	void Playlist::setTitle(const std::string &title) { _title = title; }

	bool Playlist::operator==(const Entity& other) const {
		const Playlist* otherPl = dynamic_cast<const Playlist*>(&other);
		if(otherPl == nullptr) return false;
		return (otherPl->getId() == this->getId());
	}

	bool Playlist::operator!=(const Entity& other) const {
		return !(*this == other);
	}

	bool Playlist::operator<(const Entity& other) const {
        const Playlist* otherPl = dynamic_cast<const Playlist*>(&other);
        if(otherPl == nullptr) {
            throw std::invalid_argument("Erro no casting: objeto não é do tipo Playlist");
        }
        return this->getTitle() < otherPl->getTitle();
    }

    bool Playlist::operator>(const Entity& other) const {
        const Playlist* otherPl = dynamic_cast<const Playlist*>(&other);
        if(otherPl == nullptr) {
            throw std::invalid_argument("Erro no casting: objeto não é do tipo Playlist");
        }
        return this->getTitle() > otherPl->getTitle();
    }

    bool Playlist::operator>=(const Entity& other) const {
        return *this > other || *this == other;
    }

    bool Playlist::operator<=(const Entity& other) const {
        return *this < other || *this == other;
    }

    std::vector<std::shared_ptr<Song>> Playlist::loadSongs() const {
        if (!_loader) {
            throw std::runtime_error("Songs loader nao foi definido");
        }

        if (!_songsLoaded) {
            _songs = _loader();
            _songsLoaded = true;
        }

        return _songs;
    }

	std::vector<std::shared_ptr<Song>> Playlist::getSongs() const {
        loadSongs();

        std::vector<std::shared_ptr<Song>> v;
        v.reserve(_songs.size() + 5);

        for (auto const &s : _songs)
            v.push_back(s);

        return v;
	}

	void Playlist::setSongsLoader(
		const std::function<std::vector<std::shared_ptr<Song>>()>& loader) {
		if (loader) {
			_loader = loader;
		}
	}

	void Playlist::addSong(const Song& song) {
	    loadSongs();
		_songs.push_back(std::make_shared<Song>(song));
	}

	bool Playlist::removeSong(unsigned id) {
	    loadSongs();
	    for (size_t i = 0; i < _songs.size(); i++) {
            if (_songs[i]->getId() == id) {
                _songs.erase(_songs.begin() + static_cast<int>(i));
                return true;
            }
        }
        return false;
	}

	std::shared_ptr<Song> Playlist::findSongById(unsigned songId) {
        loadSongs();
	    for (const auto &i : _songs) {
            if (i->getId() == songId) {
                return i;
            }
        }
        return nullptr;
	}

	std::vector<std::shared_ptr<Song>> Playlist::findSongByTitle(const std::string& title) {
	    loadSongs();
        std::vector<std::shared_ptr<Song>> result;
        for (auto const &s : _songs) {
            if (s->getTitle() == title) {
                result.push_back(s);
            }
        }
        return result;
	}

	size_t Playlist::getSongsCount() const {
        loadSongs();
        return _songs.size();
	}

	unsigned Playlist::calculateTotalDuration() {
	    loadSongs();
		int total = 0;
		for (const auto& s : _songs) {
			if (!s) continue;
			total += s->getDuration();
		}
		return total;
	}

	std::shared_ptr<Song> Playlist::getSongAt(int index) {
        loadSongs();

        try {
        return this->operator[](index);
        } catch (const std::out_of_range& e) {
            return nullptr;
        }
	}

    std::shared_ptr<Song> Playlist::operator[](int index) {
        loadSongs();
	    if (index < 0 || static_cast<size_t>(index) >= _songs.size()) {
               throw std::out_of_range("Índice fora dos limites: " + std::to_string(index));
           }
		return _songs.at(static_cast<size_t>(index));
    }

	std::vector<std::shared_ptr<IPlayableObject>> Playlist::getPlayableObjects() const {
	    loadSongs();
		std::vector<std::shared_ptr<IPlayableObject>> out;
		out.reserve(_songs.size());
		for (const auto& s : _songs) {
		    out.push_back(s);
		}
		return out;
	}

	std::shared_ptr<const User> Playlist::getUser() const {
	    return std::const_pointer_cast<const User>(_user);
	}

	void Playlist::setUser(const User& user) {
		_user = std::make_shared<User>(user);
	}

}
