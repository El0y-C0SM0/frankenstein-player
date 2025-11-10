#include "core/entities/Artist.hpp"
#include <cassert>
#include <memory>
#include <string>

namespace core {

    Artist::Artist() {};

    Artist::Artist(unsigned id, std::string name, unsigned user_id)
        : _id(id), _name(name), _user_id(user_id) {};

    Artist::Artist(const std::string& name, const std::string& genre)

        : _name(name), _genre(genre) {}
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
        int numSongs = 0;
        for (auto const& s : _songs) {
            assert(s != nullptr);
            numSongs++;
        }
        for (auto const& a : _albums) {
            assert(a != nullptr);
            numSongs += a->getSongCount();
        }

        return numSongs;
    };

    int Artist::getAlbumsCount() const {
        int numAlbum = 0;
        for (auto const& a : _albums) {
            assert(a != nullptr);
            numAlbum++;
        }
        return numAlbum;
    };
    void Artist::setAlbumsLoader(
        const std::function<std::vector<std::shared_ptr<Album>>()>& loader) {

        if (!loader) {
            // throw std::invalid_argument("Loader function cannot be null");
            //  TODO quando escolher tratamento refatorar loader de Song e Album
            //  tb ;P
        }
        albumsLoader = loader;
    };

    // Seters

    void Artist::setName(const std::string& name) {
        if (name.empty()) {
            // TODO exception
            return;
        }
        _name = name;
    };

    void Artist::setGenre(const std::string& genre) {
        if (genre.empty()) {
            // TODO exception
            return;
        }
        _genre = genre;
    };

    void Artist::addSong(const Song& song) {
        _songs.push_back(std::make_shared<Song>(song));
    };

    void Artist::addAlbum(const Album& album) {
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
            return false;
        }

        if (index >= _songs.size()) {
            // TODO INVALID ....
            return false;
        }

        if (currentIndex == index) {
            return true;
        }

        auto songToMove = _songs[currentIndex];
        _songs.erase(_songs.begin() + currentIndex);

        if (index >= _songs.size()) {
            _songs.push_back(songToMove);
        } else {
            _songs.insert(_songs.begin() + index, songToMove);
        }

        return true;
    };

    std::shared_ptr<IPlayable>
    Artist::findSongByTitle(const std::string& title) {
        // todas musicas de um album devem estar em
        // _songs? iterar _songs : iterar _songs && _albums

        for (auto const& s : _songs) {
            if (s->getTitle().compare(title)) {
                return s;
            }
        }
        return nullptr;
    };

    std::shared_ptr<IPlayable> Artist::findSongById(unsigned songId) {
        // todas musicas de um album devem estar em
        // _songs? iterar _songs : iterar _songs && _albums

        for (auto const& s : _songs) {
            if (s->getId() == songId) {
                return s;
            }
        }
        return nullptr;
    };

    bool Artist::removeAlbum(unsigned albumId) {
        for (size_t i = 0; i < _albums.size(); i++) {
            if (_albums[i]->getId() == albumId) {
                _albums.erase(_albums.begin() + i);
                return true;
            }
        }
        return false;
    };

    std::shared_ptr<Album>
    Artist::findAlbumByName(const std::string& albumName) const {
        for (auto const& a : _albums) {
            if (a->getName().compare(albumName)) {
                return a;
            }
        }
        return nullptr;
    };

    int Artist::getTotalDuration() const {
        // todas musicas de um album devem estar em
        // _songs? iterar _songs : iterar _songs && _albums
        int totalSeconds = 0;
        for (auto const& s : _songs) {
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
            formatted = std::to_string(h) + ":" + (m < 10 ? "0" : "")
                        + std::to_string(m) + ":" + (s < 10 ? "0" : "")
                        + std::to_string(s);
        } else {
            formatted = (m < 10 ? "0" : "") + std::to_string(m) + ":"
                        + (s < 10 ? "0" : "") + std::to_string(s);
        }
        return formatted;
    };

    std::string Artist::toString() const {
        std::string info = "{Artist:Id:" + std::to_string(_id)
                           + ", Nome:" + _name + ", Genre:" + _genre + "}";
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
        // todas musicas de um album devem estar em
        // _songs? iterar _songs : iterar _songs && _albums
        std::vector<std::shared_ptr<IPlayableObject>> vector;

        for (auto const& s : _songs) {
            vector.push_back(s);
        }
        return vector;
    };

    bool Artist::operator==(const Entity& other) const {
        // TODO RULE OF THREE
        const Artist* otherArtist = dynamic_cast<const Artist*>(&other);
        if (otherArtist->_id == this->_id
            && otherArtist->_name.compare(this->_name)
            && otherArtist->_songs.size() == this->_songs.size()) {
            return true;
        }
        return false;
    };

    bool Artist::operator!=(const Entity& other) const {
        // TODO RULE OF THREE
        const Artist* otherArtist = dynamic_cast<const Artist*>(&other);
        if (otherArtist->_id == this->_id
            && otherArtist->_name.compare(this->_name)
            && otherArtist->_songs.size() == this->_songs.size()) {
            return false;
        }
        return true;
    };

    std::vector<std::shared_ptr<IPlayable>> Artist::getSongs() {
        std::vector<std::shared_ptr<IPlayable>> vector;
        assert(_songs.size() != 0);
        for (auto const& s : _songs) {
            vector.push_back(s);
        }
        return vector;
    };
    std::vector<std::shared_ptr<IPlayable>>
    Artist::getSongsAlbum(unsigned idAlbum) {
        std::vector<std::shared_ptr<IPlayable>> songs;

        for (auto const& album : _albums) {
            if (album->getId() == idAlbum) {

                auto albumSongs = album->getSongs();
                songs.insert(songs.end(), albumSongs.begin(), albumSongs.end());
                break;
            }
        }

        return songs;
    }

    void Artist::addSong(std::shared_ptr<IPlayable> song) {
        auto songPtr = std::dynamic_pointer_cast<Song>(song);
        assert(songPtr != nullptr);
        _songs.push_back(songPtr);
    };

    void Artist::addSongAlbum(std::shared_ptr<IPlayable> song,
                              unsigned idAlbum) {
        for (auto const& album : _albums) {
            if (album->getId() == idAlbum) {
                album->addSong(song);
                break;
            }
        }
        // TODO exception album n encontrado
    };

    bool Artist::removeSong(unsigned id) {
        for (size_t i = 0; i < _songs.size(); i++) {
            if (_songs[i]->getId() == id) {
                _songs.erase(_songs.begin() + i);
                return true;
            }
        }
        return false;
    };

    bool Artist::removeSongAlbum(unsigned id, unsigned idAlbum) {
        for (auto const& album : _albums) {
            if (album->getId() == idAlbum) {
                return album->removeSong(id);
            }
        }
        return false;
    };

    std::shared_ptr<IPlayable>
    Artist::getNextSong(std::shared_ptr<IPlayable> current) {
        for (size_t i = 0; i < _songs.size(); i++) {
            if (_songs[i].get() == current.get()) {

                if (i + 1 < _songs.size()) {
                    return _songs[i + 1];
                } else {
                    return nullptr;
                }
            }
        }
        return nullptr;
    };

    std::shared_ptr<IPlayable>
    Artist::getPreviousSong(std::shared_ptr<IPlayable> current) {
        for (size_t i = 0; i < _songs.size(); i++) {
            if (_songs[i].get() == current.get()) {
                if (i > 0) {
                    return _songs[i - 1];
                } else {
                    return nullptr;
                }
            }
        }
        return nullptr;
    };

    std::shared_ptr<IPlayable>
    Artist::getNextSongAlbum(std::shared_ptr<IPlayable> current,
                             unsigned idAlbum) {
        for (auto const& album : _albums) {
            if (album->getId() == idAlbum) {
                return album->getNextSong(current);
            }
        }
        return nullptr;
    }

    std::shared_ptr<IPlayable>
    Artist::getPreviousSongAlbum(std::shared_ptr<IPlayable> current,
                                 unsigned idAlbum) {

        for (auto const& album : _albums) {
            if (album->getId() == idAlbum) {
                return album->getPreviousSong(current);
            }
        }
        return nullptr;
    };

    std::shared_ptr<IPlayable> Artist::getSongAt(int index) {
        return _songs.at(index);  // It throws out_of_range if n is out of
                                  // bounds. entao n precisa de exception
    };

    std::shared_ptr<IPlayable> Artist::getSongAtAlbum(int index,
                                                      unsigned idAlbum) {
        for (auto const& album : _albums) {
            if (album->getId() == idAlbum) {
                return album->getSongAt(index);
            }
        }
        return nullptr;
    };

    std::shared_ptr<User> Artist::getUser() const {
        return user;
    };

}  // namespace core
