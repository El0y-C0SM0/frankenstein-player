#include "core/entities/Album.hpp"
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <cassert>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vector>

namespace core {
    Album::Album() {};

    // Album para mapRowToEntity
    Album::Album(unsigned id,
                 std::string title,
                 int year,
                 std::string genre,
                 unsigned user_id)
        : _id(id), _user_id(0), _genre(""), _year(0) {

        // Validações PRIMEIRO
        if (user_id == 0) {
            throw std::invalid_argument("ID do usuário não pode ser 0");
        }

        if (title.empty()) {
            throw std::invalid_argument("Título do álbum não pode estar vazio");
        }

        _name = std::move(title);
        _year = year;
        _genre = std::move(genre);
        _user_id = user_id;
    };

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
        if (!_artist) {
            throw std::invalid_argument("Artista desconhecido");
        }
        return _artist;
    };

    std::vector<std::shared_ptr<const Artist>>
    Album::getFeaturingArtists() const {
        // TODO
        return std::vector<std::shared_ptr<const Artist>>();
    };

    std::string Album::getGenre() const {
        return _genre;
    };

    int Album::getYear() const {
        return _year;
    };

    int Album::getSongCount() const {
        return static_cast<int>(_songs.size());
    };

    // Setters

    void Album::setFilePath(std::string &path) {
        if (path.empty()) {
            throw std::invalid_argument("Path nao pode ser vazio");
        }
        _file_path = path;
    }

    void Album::setArtist(const std::shared_ptr<Artist> &artist) {
        if (artist == nullptr) {
            throw std::invalid_argument("Artist nao pode ser null");
        }
        _artist = artist;
    };

    void Album::setFeaturingArtists(const std::vector<Artist> &artists) {
        if (!artists.empty()) {
            throw std::invalid_argument("Parametro vazio");
        }
        for (auto const &a : artists) {
            _featuring_artists_ids.push_back(a.getId());
        }
    };

    void Album::setGenre(const std::string &genre) {
        if (genre.empty()) {
            throw std::invalid_argument("Genre nao pode ser vazio");
        }
        _genre = genre;
    };

    void Album::setYear(int year) {
        _year = year;
    };

    void Album::setArtistLoader(
        const std::function<std::shared_ptr<Artist>()> &loader) {
        if (!loader) {
            throw std::invalid_argument("Loader nao pode ser null");
        }
        artistLoader = loader;
    };

    void Album::setFeaturingArtistsLoader(
        const std::function<std::vector<std::shared_ptr<Artist>>()> &loader) {
        if (!loader) {
            throw std::invalid_argument("Loader nao pode ser null");
        }
        featuringArtistsLoader = loader;
    };

    std::string Album::toString() const {
        std::string info = "{Album: " + _name + ", Artista: " + _artist->getName() + ", Ano: " + std::to_string(_year) + "}";

        return info;
    };
    // Entity

    bool Album::operator==(const Entity &other) const {
        const Album *otherAlbum = dynamic_cast<const Album *>(&other);
        // assert(otherAlbum != nullptr);
        if (otherAlbum == nullptr) {
            throw std::invalid_argument("Erro no casting");
        }
        if (otherAlbum->_name == this->_name && otherAlbum->getSongCount() == this->getSongCount()) {
            return true;
        }
        return false;
    };
    bool Album::operator!=(const Entity &other) const {
        const Album *otherAlbum = dynamic_cast<const Album *>(&other);
        // assert(otherAlbum != nullptr);
        if (otherAlbum == nullptr) {
            throw std::invalid_argument("Erro no casting");
        }

        if (otherAlbum->_name == this->_name && otherAlbum->getSongCount() == this->getSongCount()) {
            return false;
        }
        return true;
    };

    std::vector<std::shared_ptr<IPlayableObject>>
    Album::getPlayableObjects() const {
        std::vector<std::shared_ptr<IPlayableObject>> vector;

        for (auto const &s : _songs) {
            vector.push_back(s);
        }

        return vector;
    }
    std::string Album::getAudioFilePath() const {
        return _file_path;
    };

    std::vector<std::shared_ptr<Song>> Album::getSongs() const {
        std::vector<std::shared_ptr<Song>> vector;
        for (auto const &s : _songs) {
            vector.push_back(s);
        }
        return vector;
    }

    void Album::setSongsLoader(
        const std::function<std::vector<std::shared_ptr<Song>>()> &
            loader) {
        songsLoader = loader;
    };

    void Album::addSong(Song &song) {
        _songs.push_back(std::make_shared<Song>(song));
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
            throw std::invalid_argument("Musica não encontrada com id passado");
        }

        if (index >= _songs.size()) {
            throw std::invalid_argument("Index invalido");
        }

        if (static_cast<unsigned>(currentIndex) == index) {
            return true;
        }

        auto songToMove = _songs[currentIndex];
        _songs.erase(_songs.begin() + currentIndex); // reorganiza vetor apos tirar

        if (index >= _songs.size()) {
            _songs.push_back(songToMove); // talvez usar end() aponta para
                                          // posicao alem do vector
        } else {
            _songs.insert(_songs.begin() + static_cast<int>(index), songToMove);
        }

        return true;
    }

    bool Album::removeSong(unsigned id) {
        for (size_t i = 0; i < _songs.size(); i++) {
            if (_songs[i]->getId() == id) {
                _songs.erase(_songs.begin() + static_cast<int>(i));
                return true;
            }
        }
        return false;
    };

    std::shared_ptr<Song> Album::findSongById(unsigned songId) {
        for (const auto &song : _songs) {
            if (song && song->getId() == songId) {
                return song;
            }
        }
        return nullptr;
    }

    std::shared_ptr<Song>
    Album::findSongByTitle(const std::string &title) {
        for (auto const &s : _songs) {
            if (s && s->getTitle() == title) {
                return s;
            }
        }
        return nullptr;
    };

    int Album::calculateTotalDuration() {
        int totalSeconds = 0;
        for (auto const &s : _songs) {
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
            formatted = std::to_string(h) + ":" + (m < 10 ? "0" : "") + std::to_string(m) + ":" + (s < 10 ? "0" : "") + std::to_string(s);
        } else {
            formatted = std::to_string(m) + ":" + (s < 10 ? "0" : "") + std::to_string(s);
        }

        return formatted;
    };
    std::shared_ptr<Song>
    Album::getNextSong(Song &current) {
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
    }

    std::shared_ptr<Song>
    Album::getPreviousSong(Song &current) {
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

    std::shared_ptr<Song> Album::getSongAt(int index) {
        if (index < 0 || static_cast<size_t>(index) >= _songs.size()) {
            throw std::out_of_range("Índice fora dos limites: " + std::to_string(index));
        }
        return _songs.at(static_cast<size_t>(index));
    };

} // namespace core
