
#include "core/entities/Song.hpp"
#include "core/bd/ArtistRepository.hpp"
#include "core/bd/SongRepository.hpp"
#include "core/entities/Album.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/Entity.hpp"
#include "core/entities/User.hpp"
#include <cassert>
#include <memory>
#include <miniaudio.h>
#include <stdexcept>
#include <string>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>

namespace core {
    Song::Song() {
    }

    Song::Song(const std::string &title,
               std::shared_ptr<Artist> &artist,
               std::shared_ptr<Album> &album)
        : _title(title),
          _artist(artist),
          _album(album) {};

    Song::Song(unsigned id,
               std::string title,
               unsigned artist_id)
        : Entity(id),
          _title(std::move(title)),
          _artist_id(artist_id) {};

    Song::Song(unsigned id,
               const std::string &title,
               unsigned &artist_id,
               unsigned &album_id)
        : Entity(id),
          _title(title),
          _artist_id(artist_id),
          _album_id(album_id)
    // Construtor focado em IDs para integração com banco de dados
    {}

    Song::Song(const std::string &title, const Artist &artist, const Album &album, const User &user)
        : _title(title),
          _user(std::make_shared<User>(user)),
          _artist_id(artist.getId()),
          _artist(std::make_shared<Artist>(artist)),
          _album_id(album.getId()),
          _album(std::make_shared<Album>(album)) {}

    // Getters

    std::string Song::getTitle() const {
        return _title;
    };

    std::shared_ptr<const Artist> Song::getArtist() const {
        if (!_artist.expired())
            return std::const_pointer_cast<const Artist>(_artist.lock());

        if (!artistLoader)
            throw std::runtime_error("Artist loader nao foi definido");

        auto artist = artistLoader();
        _artist = artist;
        return std::const_pointer_cast<const Artist>(artist);
    };

    // std::vector<unsigned> Song::getFeaturingArtistsId() const {
    //     return _featuring_artists_ids;
    // };

    // std::vector<std::shared_ptr<const Artist>> Song::getFeaturingArtists() {
    //     if (featuringArtistsLoader) {
    //         auto allArtists = featuringArtistsLoader();
    //         std::vector<std::shared_ptr<const Artist>> featuring;

    //         if (allArtists.size() > 1) {
    //             for (auto it = allArtists.begin(); it != allArtists.end(); ++it) {
    //                 featuring.push_back(std::const_pointer_cast<const Artist>(*it));
    //             }
    //         }

    //         return featuring;
    //     }
    //     return {};
    // };
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

    std::shared_ptr<const Album> Song::getAlbum() const {
        if (!_album.expired())
            return std::const_pointer_cast<const Album>(_album.lock());

        if (!albumLoader)
            throw std::runtime_error("Album loader nao foi definido");

        auto album = albumLoader();
        _album = album;
        return std::const_pointer_cast<const Album>(album);
    };

    int Song::getDuration() const {
        return _duration;
    }

    std::string Song::getGenre() const {
        return _genre;
    };

    int Song::getYear() const {
        return _year;
    };

    unsigned Song::getTrackNumber() const {
        return _track_number;
    };

    std::shared_ptr<const User> Song::getUser() const {
        return std::const_pointer_cast<const User>(_user);
    };

    // Setters

    void Song::setUser(const User &user) {
        _user = std::make_shared<User>(user);
    };

    void Song::setTitle(const std::string &title) {
        assert(!title.empty());
        _title = title;
    };

    void Song::setArtist(std::shared_ptr<Artist> &artist) {
        this->_artist = artist;
    };

    void Song::setArtistLoader(
        const std::function<std::shared_ptr<Artist>()> &loader) {
        artistLoader = loader;
    };

    void Song::setFeaturingArtistsLoader(
        const std::function<std::vector<std::shared_ptr<Artist>>()> &loader) {
        featuringArtistsLoader = loader;
    }

    void Song::setFeaturingArtists(std::shared_ptr<Artist> &artist) {
        _featuring_artists_ids.push_back(artist->getId());
    };

    void Song::setFeaturingArtists(const std::vector<Artist> &artists) {

        for (auto const &a : artists) {
            _featuring_artists_ids.push_back(a.getId());
        }
    };

    void Song::setAlbumLoader(
        const std::function<std::shared_ptr<Album>()> &loader) {
        albumLoader = loader;
    };

    void Song::setAlbum(const Album &album) {
        _album = std::make_shared<Album>(album);
    };

    void Song::setGenre(const std::string &genre) {
        _genre = genre;
    };

    void Song::setYear(int year) {
        _year = year;
    };

    void Song::setTrackNumber(unsigned track_number) {
        _track_number = track_number;
    };

    void Song::setDuration(int sec) {
        _duration = sec;
    };

    std::string Song::toString() const {
        std::string info = "{Musica: " + _title + ", Artista: " + getArtist()->getName() + ", Duracao: " + std::to_string(getDuration()) + ", Ano: " + std::to_string(_year) + "}";
        return info;
    };

    std::vector<std::shared_ptr<IPlayableObject>>
    Song::getPlayableObjects() const {

        return {std::make_shared<Song>(*this)}; // {} para converer em vector;
    };

    bool Song::operator==(const Entity &other) const {
        const Song *otherSong = dynamic_cast<const Song *>(&other);

        if (otherSong == nullptr) {
            throw std::invalid_argument("Erro no casting: objeto não é do tipo Song");
        }

        if (otherSong->getId() == this->getId() && otherSong->_title == this->_title) {
            return true;
        }
        return false;
    };

    bool Song::operator!=(const Entity &other) const {
        const Song *otherSong = dynamic_cast<const Song *>(&other);

        if (otherSong == nullptr) {
            throw std::invalid_argument("Erro no casting: objeto não é do tipo Song");
        }

        if (otherSong->getId() == this->getId() && otherSong->_title == this->_title) {
            return false;
        }
        return true;
    };

    bool Song::operator<(const Entity &other) const {
        const Song *otherSong = dynamic_cast<const Song *>(&other);

        if (otherSong == nullptr) {
            throw std::invalid_argument("Erro no casting: objeto não é do tipo Song");
        }

        if (this->getArtist() == otherSong->getArtist() &&
            this->getAlbum() == otherSong->getAlbum())
            return this->getTrackNumber() < otherSong->getTrackNumber();

        return this->getTitle() < otherSong->getTitle();
    };

    bool Song::operator<=(const Entity &other) const {
        const Song *otherSong = dynamic_cast<const Song *>(&other);

        if (otherSong == nullptr) {
            throw std::invalid_argument("Erro no casting: objeto não é do tipo Song");
        }

        return *this < *otherSong || *this == *otherSong;
    };

    bool Song::operator>(const Entity &other) const {
        const Song *otherSong = dynamic_cast<const Song *>(&other);

        if (otherSong == nullptr) {
            throw std::invalid_argument("Erro no casting: objeto não é do tipo Song");
        }


        if (this->getArtist() == otherSong->getArtist() &&
            this->getAlbum() == otherSong->getAlbum())
            return this->getTrackNumber() > otherSong->getTrackNumber();

        return this->getTitle() > otherSong->getTitle();
    };

    bool Song::operator>=(const Entity &other) const {
        const Song *otherSong = dynamic_cast<const Song *>(&other);

        if (otherSong == nullptr) {
            throw std::invalid_argument("Erro no casting: objeto não é do tipo Song");
        }

        return *this > *otherSong || *this == *otherSong;
    };

    std::string Song::getAudioFilePath() const {
        std::string path = _user->getHomePath() + "/" + getArtist()->getName() + "/";
        if (getAlbum())
            path += getAlbum()->getTitle() + "/";
        else
            path += SINGLE_ALBUM;
        path += getTitle() + ".mp3";

        return path;
    };
} // namespace core
