#include "core/entities/Playlist.hpp"

#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>


namespace core {

	Playlist::Playlist() :
	    Entity(0),
		_title(""),
		_user(nullptr),
		_loader([]() {return std::vector<std::shared_ptr<Song>>();}) {}

	Playlist::Playlist(const unsigned id, const std::string title) :
	    Entity(id),
        _title(title),
        _user(nullptr),
        _songs(),
        _loader([]() {return std::vector<std::shared_ptr<Song>>();}) {}

	Playlist::Playlist(const unsigned id,
                       const std::string title,
                       const User &user)
        : Entity(id),
          _title(title),
          _user(std::make_shared<User>(user)),
          _songs(),
          _loader([]() {return std::vector<std::shared_ptr<Song>>();}) {}

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

        _song_ids.clear();
        for (const auto& song : _songs)
            if (song) _song_ids.insert(song->getId());

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

		if (_song_ids.find(song.getId()) != _song_ids.end())
            return;

		_songs.push_back(std::make_shared<Song>(song));
	}

	bool Playlist::removeSong(unsigned id) {
	    loadSongs();
	    for (size_t i = 0; i < _songs.size(); i++) {
            if (_songs[i]->getId() == id) {
                _songs.erase(_songs.begin() + static_cast<int>(i));
                _song_ids.erase(id);
                return true;
            }
        }
        return false;
	}

	std::shared_ptr<Song> Playlist::findSongById(unsigned songId) {
        loadSongs();

        if (_song_ids.find(songId) == _song_ids.end())
            return nullptr;

        auto it = std::find_if(_songs.begin(), _songs.end(),
                               [songId](const std::shared_ptr<Song>& song) {
                                   return song->getId() == songId;
                               });

        return (it != _songs.end()) ? *it : nullptr;
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

	bool Playlist::containsSong(unsigned songId) const {
        loadSongs();
        return _song_ids.find(songId) != _song_ids.end();
    }

    bool Playlist::containsSong(const Song& song) const {
        return containsSong(song.getId());
    }

	bool Playlist::insert(const Song &song, size_t pos) {
        loadSongs();

        if (containsSong(song.getId()))
            return false;

        if (pos > _songs.size())
            pos = _songs.size();

        _songs.insert(_songs.begin() + static_cast<int>(pos),
                      std::make_shared<Song>(song));
        return true;
	}

	bool Playlist::insert(const Album &album, size_t pos) {
        loadSongs();

        const auto& albumSongs = album.getSongs();
        bool inserted = false;


        if (pos > _songs.size())
            pos = _songs.size();
        auto it_pos = _songs.begin() + static_cast<int>(pos);

        for (const auto& song : albumSongs) {
            if (song && !containsSong(song->getId())) {
                it_pos = _songs.insert(it_pos, song);
                ++it_pos;
                inserted = true;
            }
        }

        return inserted;
    }

    bool Playlist::insert(const Artist &artist, size_t pos) {
        loadSongs();

        const auto& artistSongs = artist.getSongs();
        bool inserted = false;

        if (pos > _songs.size())
            pos = _songs.size();

        auto it_pos = _songs.begin() + static_cast<int>(pos);

        for (const auto& song : artistSongs) {
            if (song && !containsSong(song->getId())) {
                it_pos = _songs.insert(it_pos, song);
                ++it_pos;
                inserted = true;
            }
        }

        return inserted;
    }

    bool Playlist::insert(const std::vector<std::shared_ptr<Song>> &songs, size_t pos) {
        loadSongs();

        bool inserted = false;

        if (pos > _songs.size())
            pos = _songs.size();

        auto it_pos = _songs.begin() + static_cast<int>(pos);

        for (const auto& song : songs) {
            if (song && !containsSong(song->getId())) {
                it_pos = _songs.insert(it_pos, song);
                ++it_pos;
                inserted = true;
            }
        }

        return inserted;
    }

    bool Playlist::insert(const Playlist &playlist, size_t pos) {
        loadSongs();

        const auto& playlistSongs = playlist.getSongs();
        bool inserted = false;

        if (pos > _songs.size())
            pos = _songs.size();

        auto it_pos = _songs.begin() + static_cast<int>(pos);

        for (const auto& song : playlistSongs) {
            if (song && !containsSong(song->getId())) {
                _songs.insert(it_pos, song);
                it_pos++;
                inserted = true;
            }
        }

        return inserted;
    }

    bool Playlist::pushBack(const Song &song) {
        return insert(song, _songs.size());
    }

    bool Playlist::pushBack(const Album &album) {
        return insert(album, _songs.size());
    }

    bool Playlist::pushBack(const Artist &artist) {
        return insert(artist, _songs.size());
    }

    bool Playlist::pushBack(const std::vector<std::shared_ptr<Song>> &songs) {
        return insert(songs, _songs.size());
    }

    bool Playlist::pushBack(const Playlist &playlist) {
        return insert(playlist, _songs.size());
    }

    bool Playlist::pushFront(const Song &song) {
        return insert(song, 0);
    }

    bool Playlist::pushFront(const Album &album) {
        return insert(album, 0);
    }

    bool Playlist::pushFront(const Artist &artist) {
        return insert(artist, 0);
    }

    bool Playlist::pushFront(const std::vector<std::shared_ptr<Song>> &songs) {
        return insert(songs, 0);
    }

    bool Playlist::pushFront(const Playlist &playlist) {
        return insert(playlist, 0);
    }
}
