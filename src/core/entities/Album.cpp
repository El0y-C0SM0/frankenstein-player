#include "core/entities/Album.hpp"
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <cassert>
#include <cstddef>
#include <memory>
#include <vector>

namespace core {
    Album::Album() {};

    // Album para mapRowToEntity
    Album::Album(unsigned id,
                 std::string title,
                 int year,
                 std::string genre,
                 unsigned user_id)
        : _id(id), _name(title), _year(year), _genre(genre),
          _user_id(user_id) {};

    Album::Album(const std::string name,
                 std::shared_ptr<Artist> artist,
                 const std::string genre)
        : _name(name), _artist(artist), _genre(genre) {};

    Album::Album(unsigned id,
                 const std::string name,
                 std::shared_ptr<Artist> artist,
                 const std::string genre,
                 int year)
        : _id(id), _name(name), _artist(artist), _genre(genre), _year(year) {};

    Album::~Album() = default;

    // Getters
    std::string Album::getName() const {

        return _name;
    };

    std::shared_ptr<User> Album::getUser() const {
        return _user;
    };

    std::shared_ptr<Artist> Album::getArtist() const {
        assert(_artist != nullptr);
        return _artist;
    };

    std::vector<std::shared_ptr<const Artist>>
    Album::getFeaturingArtists() const {
        // TODO iterar _featuring_artists_ids e extrair os feat da musica pelo
        // BD;
    };

    std::string Album::getGenre() const {
        return _genre;
    };

    int Album::getYear() const {
        return _year;
    };

    int Album::getSongCount() const {
        int count = 0;
        for (auto const& s : _songs) {
            count++;
        }
        return count;
    };

    // Setters

    void Album::setFilePath(std::string& path) {
        _file_path = path;
    }

    void Album::setArtist(const std::shared_ptr<Artist>& artist) {
        _artist = artist;
    };

    void Album::setFeaturingArtists(const std::vector<Artist>& artists) {
        assert(!artists.empty());
        for (auto const& a : artists) {
            _featuring_artists_ids.push_back(a.getId());
        }
    };

    void Album::setGenre(const std::string& genre) {
        _genre = genre;
    };

    void Album::setYear(int year) {
        _year = year;
    };

    void Album::setArtistLoader(
        const std::function<std::shared_ptr<Artist>()>& loader) {
        artistLoader = loader;
    };

    void Album::setFeaturingArtistsLoader(
        const std::function<std::vector<std::shared_ptr<Artist>>()>& loader) {
        featuringArtistsLoader = loader;
    };

    std::string Album::toString() const {
        std::string info = "{Album: " + _name
                           + ", Artista: " + _artist->getName()
                           + ", Ano: " + std::to_string(_year) + "}";

        return info;
    };
    // Entity

    bool Album::operator==(const Entity& other) const {
        const Album* otherAlbum = dynamic_cast<const Album*>(&other);
        assert(otherAlbum != nullptr);
        if (otherAlbum->_name == this->_name
            && otherAlbum->getSongCount() == this->getSongCount()) {
            return true;
        }
        return false;
    };
    bool Album::operator!=(const Entity& other) const {
        const Album* otherAlbum = dynamic_cast<const Album*>(&other);
        assert(otherAlbum != nullptr);
        if (otherAlbum->_name == this->_name
            && otherAlbum->getSongCount() == this->getSongCount()) {
            return false;
        }
        return true;
    };

    std::vector<std::shared_ptr<IPlayableObject>>
    Album::getPlayableObjects() const {
        std::vector<std::shared_ptr<IPlayableObject>> vector;
        assert(!vector.empty());

        for (auto const& s : _songs) {
            vector.push_back(s);
        }

        return vector;
    }
    std::string Album::getAudioFilePath() const {
        return _file_path;
    };

    std::vector<std::shared_ptr<IPlayable>> Album::getSongs() {
        std::vector<std::shared_ptr<IPlayable>> vector;

        for (auto const& s : _songs) {
            vector.push_back(s);
        }
        return vector;
    }

    void Album::setSongsLoader(
        const std::function<std::vector<std::shared_ptr<IPlayable>>()>&
            loader) {
        songsLoader = loader;  // TODO alterei songsLoader para IPlayable para
                               // evitar esse cast que seria gambiarra
                               // se precisar faço a gambiarra
    };

    void Album::addSong(std::shared_ptr<IPlayable> song) {

        auto songPtr = std::dynamic_pointer_cast<Song>(song);
        assert(songPtr != nullptr);

        _songs.push_back(songPtr);
    };

    bool Album::switchSong(unsigned id, unsigned index) {

        int currentIndex = -1;
        for (size_t i = 0; i < _songs.size(); i++) {
            if (_songs[i]->getId() == id) {
                currentIndex = i;
                break;
            }
        }

        if (currentIndex == -1) {
            // TODO INVALID ARGUMENT EXCEPTON
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
        _songs.erase(_songs.begin()
                     + currentIndex);  // reorganiza vetor apos tirar

        if (index >= _songs.size()) {
            _songs.push_back(songToMove);  // talvez usar end() aponta para
                                           // posicao alem do vector
        } else {
            _songs.insert(_songs.begin() + index, songToMove);
        }

        return true;
    }

    bool Album::removeSong(unsigned id) {
        for (size_t i = 0; i < _songs.size(); i++) {
            if (_songs[i]->getId() == id) {
                _songs.erase(_songs.begin() + i);
                return true;
            }
        }
        return false;
    };

    std::shared_ptr<IPlayable> Album::findSongById(unsigned songId) {
        for (const auto& song : _songs) {
            if (song && song->getId() == songId) {
                return song;
            }
        }
        return nullptr;
    }

    std::shared_ptr<IPlayable>
    Album::findSongByTitle(const std::string& title) {
        for (auto const& s : _songs) {
            if (s->getTitle().compare(title)) {
                return s;
            }
        }
        return nullptr;
    };

    int Album::calculateTotalDuration() {
        int totalSeconds = 0;
        for (auto const& s : _songs) {
            totalSeconds += s->getDuration();
        }
        return totalSeconds;
    };

    std::string Album::getFormattedDuration() {
        int totalSeconds = calculateTotalDuration();

        int h = totalSeconds / 3600;
        int m = (totalSeconds % 3600) / 60;
        int s = totalSeconds % 60;

        std::string formatted;
        if (h > 0) {
            formatted = std::to_string(h) + ":" + (m < 10 ? "0" : "")
                        + std::to_string(m) + ":" + (s < 10 ? "0" : "")
                        + std::to_string(s);
        } else {
            formatted = std::to_string(m) + ":" + (s < 10 ? "0" : "")
                        + std::to_string(s);
        }

        return formatted;
    };
    std::shared_ptr<IPlayable>
    Album::getNextSong(std::shared_ptr<IPlayable> current) {
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
    }

    std::shared_ptr<IPlayable>
    Album::getPreviousSong(std::shared_ptr<IPlayable> current) {
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

    std::shared_ptr<IPlayable> Album::getSongAt(int index) {
        return _songs.at(index);  // It throws out_of_range if n is out of
                                  // bounds. entao n precisa de exception
    };

    std::vector<std::shared_ptr<Song>> Album::getSongs() const {
        // Implementação: converter _songs ou carregar via loader
        std::vector<std::shared_ptr<Song>> songs;
        for (auto& song : _songs) {
            songs.push_back(song);
        }
        return songs;
    }

    void Album::setSongsLoader(
        const std::function<std::vector<std::shared_ptr<Song>>()>& loader) {
        // TODO conflito no ICollection
    }

}  // namespace core
