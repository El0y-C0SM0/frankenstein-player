#include "core/entities/Album.hpp"
#include "core/entities/Entity.hpp"
#include "core/bd/ArtistRepository.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/User.hpp"
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
                 User &user)
        : Entity(id), _title(title), _genre(genre), _year(year) {

        // Validações PRIMEIRO
        if (title.empty()) {
            throw std::invalid_argument("Título do álbum não pode estar vazio");
        }

        _user = std::make_shared<User>(user);
    };

    Album::Album(const std::string title,
                 std::shared_ptr<Artist> artist,
                 const std::string genre)
        : _title(title), _genre(genre), _artist_id(artist->getId()) {};

    Album::Album(unsigned id,
                 const std::string title,
                 std::shared_ptr<Artist> artist,
                 const std::string genre,
                 int year)
        : Entity(id), _title(title), _genre(genre), _year(year), _artist_id(artist->getId()) {};

    // Getters
    std::string Album::getTitle() const {
        return _title;
    };

    std::shared_ptr<const User> Album::getUser() const {
        return std::const_pointer_cast<const User>(_user);
    };

    std::shared_ptr<const Artist> Album::getArtist() const {
        if (!_artist.expired())
            return std::const_pointer_cast<const Artist>(_artist.lock());

        auto artist = artistLoader();
        _artist = artist;
        return std::const_pointer_cast<const Artist>(artist);
    };

    std::vector<std::shared_ptr<const Artist>>
    Album::getFeaturingArtists() const {
        if (!featuringArtistsLoader) {
            throw std::runtime_error("Featuring Artists Loader nao foi definido");
        }

        auto allArtists = featuringArtistsLoader();
        std::vector<std::shared_ptr<const Artist>> featuring;

        for (auto it : allArtists) {
            featuring.push_back(std::const_pointer_cast<const Artist>(it));
        }

        return featuring;
    };

    std::string Album::getGenre() const {
        return _genre;
    };

    int Album::getYear() const {
        return _year;
    };

    size_t Album::getSongCount() const {
        return static_cast<int>(loadSongs().size());
    };

    bool Album::isSongsLoaded() const {
        return _songsLoaded;
    };

    // Setters

    std::vector<std::shared_ptr<Song>> Album::loadSongs() const {
        if (!songsLoader) {
            throw std::runtime_error("Songs loader nao foi definido");
        }

        if (!_songsLoaded) {
            _songs = songsLoader();
            _songsLoaded = true;
        }

        return _songs;
    }

    void Album::setArtist(const Artist &artist) {
        if (artist.getId() == 0) {
            throw std::invalid_argument("Artist nao pode ser nulo");
        }
        _artist_id = artist.getId();
    };

    void Album::setFeaturingArtists(const std::vector<Artist> &artists) {
        if (!artists.empty()) {
            throw std::invalid_argument("Parametro vazio");
        }

        _featuring_artists_ids.clear();
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

    void Album::setUser(const User &user) {
        _user = std::make_shared<User>(user);
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

    void Album::setSongsLoader(
        const std::function<std::vector<std::shared_ptr<Song>>()> &
            loader) {
        songsLoader = loader;
    };

    std::string Album::toString() const {
        std::string info = "{Album: " + _title + ", Artista: " + getArtist()->getName() + ", Ano: " + std::to_string(_year) + "}";

        return info;
    };
    // Entity

    bool Album::operator==(const Entity &other) const {
        const Album *otherAlbum = dynamic_cast<const Album *>(&other);
        // assert(otherAlbum != nullptr);
        if (otherAlbum == nullptr) {
            throw std::invalid_argument("Erro no casting");
        }
        if (otherAlbum->_title == this->_title && otherAlbum->getSongCount() == this->getSongCount()) {
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

        if (otherAlbum->_title == this->_title && otherAlbum->getSongCount() == this->getSongCount()) {
            return false;
        }
        return true;
    };
    bool Album::operator<(const Entity &other) const {
        const Album *otherAlbum = dynamic_cast<const Album *>(&other);
        if (otherAlbum == nullptr) {
            throw std::invalid_argument("Erro no casting");
        }
        if (this->getArtist() == otherAlbum->getArtist())
            return this->getYear() < otherAlbum->getYear();
        return this->getTitle() < otherAlbum->getTitle();
    };
    bool Album::operator<=(const Entity &other) const {
        const Album *otherAlbum = dynamic_cast<const Album *>(&other);
        if (otherAlbum == nullptr) {
            throw std::invalid_argument("Erro no casting");
        }
        return *this < *otherAlbum || *this == *otherAlbum;
    };
    bool Album::operator>(const Entity &other) const {
        const Album *otherAlbum = dynamic_cast<const Album *>(&other);
        if (otherAlbum == nullptr) {
            throw std::invalid_argument("Erro no casting");
        }
        if (this->getArtist() == otherAlbum->getArtist())
            return this->getYear() > otherAlbum->getYear();
        return this->getTitle() > otherAlbum->getTitle();
    };
    bool Album::operator>=(const Entity &other) const {
        const Album *otherAlbum = dynamic_cast<const Album *>(&other);
        if (otherAlbum == nullptr) {
            throw std::invalid_argument("Erro no casting");
        }
        return *this > *otherAlbum || *this == *otherAlbum;
    };

    std::vector<std::shared_ptr<IPlayableObject>>
    Album::getPlayableObjects() const {
        loadSongs();
        std::vector<std::shared_ptr<IPlayableObject>> v;
        v.reserve(_songs.size());

        for (auto const &s : _songs) {
            v.push_back(s);
        }

        return v;
    }

    std::vector<std::shared_ptr<Song>> Album::getSongs() const {
        loadSongs();

        std::vector<std::shared_ptr<Song>> v;
        v.reserve(_songs.size() + 5);

        for (auto const &s : _songs) {
            v.push_back(s);
        }

        return v;
    }

    void Album::addSong(const Song &song) {
        loadSongs();
        _songs.push_back(std::make_shared<Song>(song));
    };

    bool Album::removeSong(unsigned id) {
        loadSongs();
        for (size_t i = 0; i < _songs.size(); i++) {
            if (_songs[i]->getId() == id) {
                _songs.erase(_songs.begin() + static_cast<int>(i));
                return true;
            }
        }
        return false;
    };

    std::shared_ptr<Song> Album::findSongById(unsigned songId) {
        loadSongs();
        for (const auto &song : _songs) {
            if (song && song->getId() == songId) {
                return song;
            }
        }
        return nullptr;
    }

    std::shared_ptr<Song>
    Album::findSongByTitle(const std::string &title) {
        loadSongs();
        for (auto const &s : _songs) {
            if (s && s->getTitle() == title) {
                return s;
            }
        }
        return nullptr;
    };

    unsigned Album::calculateTotalDuration() {
        loadSongs();
        int totalSeconds = 0;
        for (auto const &s : _songs) {
            totalSeconds += s->getDuration();
        }
        return totalSeconds;
    };

    std::shared_ptr<Song> Album::getSongAt(int index) {
        loadSongs();
        if (index < 0 || static_cast<size_t>(index) >= _songs.size()) {
            throw std::out_of_range("Índice fora dos limites: " + std::to_string(index));
        }
        return _songs.at(static_cast<size_t>(index));
    };

    std::shared_ptr<Song> Album::operator[](int index) {
        return getSongAt(index);
    }

} // namespace core
