#include "core/entities/Artist.hpp"
#include <cassert>
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
    std::string Artist::getName() const {
        return _name;
    };

    std::string Artist::getGenre() const {
        return _genre;
    };

    std::vector<std::shared_ptr<Song>> Artist::getSongs() const {
        return _songs;
    }

    std::vector<std::shared_ptr<Album>> Artist::getAlbums() const {
        return _albums;
    };

    int Artist::getSongsCount() const {
        return static_cast<int>(_songs.size());
    };

    int Artist::getAlbumsCount() const {

        return static_cast<int>(_albums.size());
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

    void Artist::addSong(const Song &song) {
        _songs.push_back(std::make_shared<Song>(song));
    };

    void Artist::addAlbum(const Album &album) {
        _albums.push_back(std::make_shared<Album>(album));
    };

    bool Artist::switchSong(unsigned id, unsigned index) {
        int currentIndex = -1;
        for (size_t i = 0; i < _songs.size(); i++) {
            if (_songs[i]->getId() == id) {
                currentIndex = i;
                break;
            }
        }

        if (currentIndex == -1) {
            throw std::invalid_argument("Musica não encontrada com id passado");
        }

        if (index >= _songs.size()) {
            throw std::invalid_argument("Index invalido");
        }

        if (static_cast<unsigned>(currentIndex) == index) {
            return true;
        }

        auto songToMove = _songs[currentIndex];
        _songs.erase(_songs.begin() + currentIndex);

        if (index >= _songs.size()) {
            _songs.push_back(songToMove);
        } else {
            _songs.insert(_songs.begin() + static_cast<int>(index), songToMove);
        }

        return true;
    };

    std::shared_ptr<Song>
    Artist::findSongByTitle(const std::string &title) {
        for (auto const &s : _songs) {
            if (s->getTitle().compare(title)) {
                return s;
            }
        }
        return nullptr;
    };

    std::shared_ptr<Song> Artist::findSongById(unsigned songId) {

        for (auto const &s : _songs) {
            if (s->getId() == songId) {
                return s;
            }
        }
        return nullptr;
    };

    bool Artist::removeAlbum(unsigned albumId) {
        for (size_t i = 0; i < _albums.size(); i++) {
            if (_albums[i]->getId() == albumId) {
                _albums.erase(_albums.begin() + static_cast<int>(i));
                return true;
            }
        }
        return false;
    };

    std::shared_ptr<Album>
    Artist::findAlbumByName(const std::string &albumName) const {
        for (auto const &a : _albums) {
            if (a->getName().compare(albumName)) {
                return a;
            }
        }
        return nullptr;
    };

    int Artist::getTotalDuration() const {
        int totalSeconds = 0;
        for (auto const &s : _songs) {
            totalSeconds += s->getDuration();
        }
        return totalSeconds;
    };

    std::string Artist::getFormattedDuration() {
        int totalSeconds = getTotalDuration();

        int h = totalSeconds / 3600;
        int m = (totalSeconds % 3600) / 60;
        int s = totalSeconds % 60;

        std::string formatted;

        if (h > 0) {
            formatted = std::to_string(h) + ":" + (m < 10 ? "0" : "") + std::to_string(m) + ":" + (s < 10 ? "0" : "") + std::to_string(s);
        } else {
            formatted = (m < 10 ? "0" : "") + std::to_string(m) + ":" + (s < 10 ? "0" : "") + std::to_string(s);
        }
        return formatted;
    };

    std::string Artist::toString() const {
        std::string info = "{Artist:Id:" + std::to_string(_id) + ", Nome:" + _name + ", Genre:" + _genre + "}";
        return info;
    };

    bool Artist::hasSong() const {
        return (_songs.empty() ? false : true);
    };

    bool Artist::hasAlbum() const {
        return (_albums.empty() ? false : true);
    };

    std::vector<std::shared_ptr<IPlayableObject>>
    Artist::getPlayableObjects() const {
        std::vector<std::shared_ptr<IPlayableObject>> vector;

        for (auto const &s : _songs) {
            vector.push_back(s);
        }
        return vector;
    };

    bool Artist::operator==(const Entity &other) const {
        const Artist *otherArtist = dynamic_cast<const Artist *>(&other);
        if (otherArtist == nullptr) {
            throw std::invalid_argument("Erro no casting");
        }
        if (otherArtist->_id == this->_id && otherArtist->_name.compare(this->_name) && otherArtist->_songs.size() == this->_songs.size()) {
            return true;
        }
        return false;
    };

    bool Artist::operator!=(const Entity &other) const {
        const Artist *otherArtist = dynamic_cast<const Artist *>(&other);
        if (otherArtist == nullptr) {
            throw std::invalid_argument("Erro no casting");
        }
        if (otherArtist->_id == this->_id && otherArtist->_name.compare(this->_name) && otherArtist->_songs.size() == this->_songs.size()) {
            return false;
        }
        return true;
    };

    std::vector<std::shared_ptr<IPlayable>> Artist::getSongs() {
        std::vector<std::shared_ptr<IPlayable>> vector;
        for (auto const &s : _songs) {
            vector.push_back(s);
        }
        return vector;
    };
    std::vector<std::shared_ptr<IPlayable>>
    Artist::getSongsAlbum(unsigned idAlbum) {
        std::vector<std::shared_ptr<IPlayable>> songs;

        for (auto const &album : _albums) {
            if (album->getId() == idAlbum) {

                auto albumSongs = album->getSongs();
                songs.insert(songs.end(), albumSongs.begin(), albumSongs.end());
                break;
            }
        }

        return songs;
    }

    void Artist::addSong(Song &song) {
        _songs.push_back(std::make_shared<Song>(song));
    };

    void Artist::addSongAlbum(Song &song,
                              unsigned idAlbum) {
        for (auto const &album : _albums) {
            if (album->getId() == idAlbum) {
                album->addSong(song);
                break;
            }
        }
        throw std::invalid_argument("Erro ao encontrar o album");
    };

    bool Artist::removeSong(unsigned id) {
        for (size_t i = 0; i < _songs.size(); i++) {
            if (_songs[i]->getId() == id) {
                _songs.erase(_songs.begin() + static_cast<int>(i));
                return true;
            }
        }
        return false;
    };

    bool Artist::removeSongAlbum(unsigned id, unsigned idAlbum) {
        for (auto const &album : _albums) {
            if (album->getId() == idAlbum) {
                return album->removeSong(id);
            }
        }
        return false;
    };

    std::shared_ptr<Song>
    Artist::getNextSong(Song &current) {
        for (size_t i = 0; i < _songs.size(); i++) {
            if (*_songs[i] == current) {

                if (i + 1 < _songs.size()) {
                    return _songs[i + 1];
                } else {
                    return nullptr;
                }
            }
        }
        return nullptr;
    };

    std::shared_ptr<Song>
    Artist::getPreviousSong(Song &current) {
        for (size_t i = 0; i < _songs.size(); i++) {
            if (*_songs[i] == current) {
                if (i > 0) {
                    return _songs[i - 1];
                } else {
                    return nullptr;
                }
            }
        }
        return nullptr;
    };

    std::shared_ptr<Song>
    Artist::getNextSongAlbum(Song &current,
                             unsigned idAlbum) {
        for (auto const &album : _albums) {
            if (album->getId() == idAlbum) {
                return album->getNextSong(current);
            }
        }
        return nullptr;
    }

    std::shared_ptr<Song>
    Artist::getPreviousSongAlbum(Song &current,
                                 unsigned idAlbum) {

        for (auto const &album : _albums) {
            if (album->getId() == idAlbum) {
                return album->getPreviousSong(current);
            }
        }
        return nullptr;
    };

    std::shared_ptr<Song> Artist::getSongAt(int index) {
        if (index < 0 || static_cast<size_t>(index) >= _songs.size()) {
            throw std::out_of_range("Índice fora dos limites: " + std::to_string(index));
        }

        return _songs.at(index);
    };

    std::shared_ptr<IPlayable> Artist::getSongAtAlbum(int index,
                                                      unsigned idAlbum) {
        for (auto const &album : _albums) {
            if (album->getId() == idAlbum) {
                return album->getSongAt(index);
            }
        }
        return nullptr;
    };

    std::shared_ptr<User> Artist::getUser() const {
        return user;
    };

} // namespace core
