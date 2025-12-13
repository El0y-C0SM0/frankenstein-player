#include "core/entities/Artist.hpp"
#include "core/entities/Album.hpp"
#include <cassert>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>

namespace core {

    Artist::Artist() {};

    Artist::Artist(unsigned id, std::string name, unsigned user_id) {
        if (id <= 0 || user_id <= 0) {
            throw std::invalid_argument("Id invalido");
        }

        _id = id;
        _name = name;
        _user_id = user_id;
    };

    Artist::Artist(const std::string &name, const std::string &genre)
        : _name(name), _genre(genre) {
    }

    std::vector<std::shared_ptr<Song>> Artist::loadSongs() const {
    	if (!songsLoader) {
            throw std::runtime_error("Songs loader nao foi definido");
     	}

        if (!_songsLoaded) {
            _songs = songsLoader();
            _songsLoaded = true;
        }

        return _songs;
    }

    std::vector<std::shared_ptr<Album>> Artist::loadAlbums() const {
        if (!albumsLoader) {
            throw std::runtime_error("Albums loader nao foi definido");
        }

        if (!_albumsLoaded) {
            _albums = albumsLoader();
            _albumsLoaded = true;
        }

        return _albums;
    };

    std::string Artist::getName() const {
        return _name;
    };

    std::string Artist::getGenre() const {
        return _genre;
    };

    std::vector<std::shared_ptr<Song>> Artist::getSongs() const {
        std::vector<std::shared_ptr<Song>> vector;

        loadSongs();

        for (auto const &s : _songs) {
            vector.push_back(s);
        }

        return vector;
    };

    std::vector<std::shared_ptr<Album>> Artist::getAlbums() const {
        return _albums;
    };

    size_t Artist::getSongsCount() const {
        if (!_songsLoaded)
            loadSongs();

        return getSongs().size();
    };

    size_t Artist::getAlbumsCount() const {
        if (!_albumsLoaded)
            loadAlbums();

        return getAlbums().size();
    };

    void Artist::setAlbumsLoader(
        const std::function<std::vector<std::shared_ptr<Album>>()> &loader) {

        if (!loader) {
            throw std::invalid_argument("Loader nao pode ser null");
        }
        albumsLoader = loader;
    };

    // Seters

    void Artist::setName(const std::string &name) {
        if (name.empty()) {
            throw std::invalid_argument("Name nao pode ser null");
            return;
        }
        _name = name;
    };

    void Artist::setGenre(const std::string &genre) {
        if (genre.empty()) {
            throw std::invalid_argument("Genre nao pode ser null");
            return;
        }
        _genre = genre;
    };

    void Artist::setUser(const User &user) {
        _user = std::make_shared<User>(user);
        _user_id = user.getId();
    };

    void Artist::addSong(const Song &song) {
        if (!_songsLoaded)
            loadSongs();

        _songs.push_back(std::make_shared<Song>(song));
    };

    void Artist::addAlbum(const Album &album) {
        if (!_albumsLoaded)
            loadAlbums();

        _albums.push_back(std::make_shared<Album>(album));
    };

    // bool Artist::switchSong(unsigned id, unsigned index) {
    //     int currentIndex = -1;
    //     for (size_t i = 0; i < _songs.size(); i++) {
    //         if (_songs[i]->getId() == id) {
    //             currentIndex = i;
    //             break;
    //         }
    //     }

    //     if (currentIndex == -1) {
    //         throw std::invalid_argument("Musica não encontrada com id passado");
    //     }

    //     if (index >= _songs.size()) {
    //         throw std::invalid_argument("Index invalido");
    //     }

    //     if (static_cast<unsigned>(currentIndex) == index) {
    //         return true;
    //     }

    //     auto songToMove = _songs[currentIndex];
    //     _songs.erase(_songs.begin() + currentIndex);

    //     if (index >= _songs.size()) {
    //         _songs.push_back(songToMove);
    //     } else {
    //         _songs.insert(_songs.begin() + static_cast<int>(index), songToMove);
    //     }

    //     return true;
    // };

    std::shared_ptr<Song>
    Artist::findSongByTitle(const std::string &title) {
        loadSongs();

        for (auto const &s : _songs) {
            if (s->getTitle() == title) {
                return s;
            }
        }
        return nullptr;
    };

    std::shared_ptr<Song> Artist::findSongById(unsigned songId) {
        loadSongs();

        for (auto const &s : _songs) {
            if (s->getId() == songId) {
                return s;
            }
        }
        return nullptr;
    };

    bool Artist::removeAlbum(unsigned albumId) {
        loadAlbums();

        for (size_t i = 0; i < _albums.size(); i++) {
            if (_albums[i]->getId() == albumId) {
                _albums.erase(_albums.begin() + static_cast<int>(i));
                return true;
            }
        }
        return false;
    };

    // std::shared_ptr<Album>
    // Artist::findAlbumByName(const std::string &albumName) const {
    //     for (auto const &a : _albums) {
    //         if (a->getName().compare(albumName)) {
    //             return a;
    //         }
    //     }
    //     return nullptr;
    // };

    // unsigned Artist::getTotalDuration() const {
    //     int totalSeconds = 0;
    //     for (auto const &s : _songs) {
    //         totalSeconds += s->getDuration();
    //     }
    //     return totalSeconds;
    // };

    // std::string Artist::getFormattedDuration() {
    //     int totalSeconds = getTotalDuration();

    //     int h = totalSeconds / 3600;
    //     int m = (totalSeconds % 3600) / 60;
    //     int s = totalSeconds % 60;

    //     std::string formatted;

    //     if (h > 0) {
    //         formatted = std::to_string(h) + ":" + (m < 10 ? "0" : "") + std::to_string(m) + ":" + (s < 10 ? "0" : "") + std::to_string(s);
    //     } else {
    //         formatted = (m < 10 ? "0" : "") + std::to_string(m) + ":" + (s < 10 ? "0" : "") + std::to_string(s);
    //     }
    //     return formatted;
    // };

    std::string Artist::toString() const {
        std::string info = "{Artist:Id:" + std::to_string(_id) + ", Nome:" + _name + ", Genre:" + _genre + "}";
        return info;
    };

    bool Artist::hasSong() const {
        auto songs = loadSongs();
        return (songs.empty() ? false : true);
    };

    bool Artist::hasAlbum() const {
        auto albums = loadAlbums();
        return (albums.empty() ? false : true);
    };

    bool Artist::isSongsLoaded() const {
        return _songsLoaded;
    };

    bool Artist::isAlbumsLoaded() const {
        return _albumsLoaded;
    };

    std::vector<std::shared_ptr<IPlayableObject>>
    Artist::getPlayableObjects() const {
        loadSongs();

        std::vector<std::shared_ptr<IPlayableObject>> v;
        v.reserve(_songs.size());

        for (auto const &s : _songs) {
            v.push_back(s);
        }
        return v;
    };

    bool Artist::operator==(const Entity &other) const {
        const Artist *otherArtist = dynamic_cast<const Artist *>(&other);
        if (otherArtist == nullptr) {
            throw std::invalid_argument("Erro no casting");
        }
        if (otherArtist->getId() == this->getId() && otherArtist->getName() == this->getName() && otherArtist->getSongsCount() == this->getSongsCount()) {
            return true;
        }
        return false;
    };

    bool Artist::operator!=(const Entity &other) const {
        const Artist *otherArtist = dynamic_cast<const Artist *>(&other);
        if (otherArtist == nullptr) {
            throw std::invalid_argument("Erro no casting");
        }
        if (otherArtist->getId() == this->getId() && otherArtist->getName().compare(this->getName()) && otherArtist->getSongsCount() == this->getSongsCount()) {
            return false;
        }
        return true;
    };

    bool Artist::operator<(const Entity &other) const {
		const Artist *otherArtist = dynamic_cast<const Artist *>(&other);
		if (otherArtist == nullptr) {
			throw std::invalid_argument("Erro no casting");
		}
		return this->getName() < otherArtist->getName();
	};

	bool Artist::operator<=(const Entity &other) const {
		const Artist *otherArtist = dynamic_cast<const Artist *>(&other);
		if (otherArtist == nullptr) {
			throw std::invalid_argument("Erro no casting");
		}
		return *this < *otherArtist || *this == *otherArtist;
	};

	bool Artist::operator>(const Entity &other) const {
		const Artist *otherArtist = dynamic_cast<const Artist *>(&other);
		if (otherArtist == nullptr) {
			throw std::invalid_argument("Erro no casting");
		}
		return this->getName() > otherArtist->getName();
	};

	bool Artist::operator>=(const Entity &other) const {
		const Artist *otherArtist = dynamic_cast<const Artist *>(&other);
		if (otherArtist == nullptr) {
			throw std::invalid_argument("Erro no casting");
		}
		return *this > *otherArtist || *this == *otherArtist;
	};

    bool Artist::removeSong(unsigned id) {
        loadSongs();
        for (size_t i = 0; i < _songs.size(); i++) {
            if (_songs[i]->getId() == id) {
                _songs.erase(_songs.begin() + static_cast<int>(i));
                return true;
            }
        }
        return false;
    };

    std::shared_ptr<Song> Artist::getSongAt(int index) {
        loadSongs();
        if (index < 0 || static_cast<size_t>(index) >= _songs.size()) {
            throw std::out_of_range("Índice fora dos limites: " + std::to_string(index));
        }

        return _songs.at(index);
    };

    std::shared_ptr<Song> Artist::operator[](int index) {
        return getSongAt(index);
    };

    std::shared_ptr<const User> Artist::getUser() const {
        return std::const_pointer_cast<const User>(_user);
    };

    unsigned Artist::calculateTotalDuration() {
        loadSongs();
        unsigned totalSeconds = 0;
        for (auto const &s : _songs) {
            totalSeconds += s->getDuration();
        }
        return totalSeconds;
    }

    void Artist::setSongsLoader(const std::function<std::vector<std::shared_ptr<Song>>()> &loader) {
        if (!loader) {
            throw std::invalid_argument("Loader não pode ser null");
        }
        songsLoader = loader;
    }
} // namespace core
