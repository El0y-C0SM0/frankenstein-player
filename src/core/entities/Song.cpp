
#include "core/entities/Song.hpp"
#include "core/bd/ArtistRepository.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/User.hpp"
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <cassert>
#include <memory>
#include <miniaudio.h>

namespace core {
    Song::Song() {};

    Song::Song(unsigned id,
               std::string file_path,
               std::string title,
               unsigned artist_id)
        : _id(id), _file_path(file_path), _title(title),
          _artist_id(artist_id) {};

    Song::Song(unsigned id,
               const std::string &title,
               unsigned &artist,
               unsigned &user_id)
        : _id(id), _title(title), _artist_id(artist), _user_id(user_id) {};

    Song::Song(const std::string &title,
               std::shared_ptr<Artist> &artist,
               std::shared_ptr<Album> &album)
        : _title(title), _artist(artist), _album(album) {};

    Song::~Song() = default;

    // Getters

    std::string Song::getFilePath() const {
        return _file_path;
    };

    std::string Song::getTitle() const {
        return _title;
    };

    std::shared_ptr<const Artist> Song::getArtist() const {
        return _artist;
    };

    std::vector<unsigned> Song::getFeaturingArtistsId() const {
        return _featuring_artists_ids;
    };

    std::vector<std::shared_ptr<const Artist>> getFeaturingArtists() {
        // temos apenas _featuring_artists_ids, precisa entao buscar no BD.
        std::vector<std::shared_ptr<const Artist>> _artist;
    };

    std::shared_ptr<const Album> Song::getAlbum() const {
        return _album;
    };

    int Song::getDuration() const {
    }

    std::string Song::getGenre() const {
        return _genre;
    };

    int Song::getYear() const {
        return _year;
    };

    // Setters

    void Song::setUser(const User &user) {
        // pensei em que musicar ter um vector de shared_ptr(user) para que uma
        // musica ser compartilhada
        // User seria por Usuarios do computador
        auto id = user.getId();
        _user_id = id;
    };

    void Song::setTitle(const std::string &title) {
        assert(!title.empty());
        _title = title;
    };

    void Song::setArtist(std::shared_ptr<Artist> &artist) {
        this->_artist = artist;
    };

    void Song::setFeaturingArtists(std::shared_ptr<Artist> &artist) {
        _featuring_artists_ids.push_back(artist->getId());
    };

    void Song::setArtistLoader(
        const std::function<std::shared_ptr<Artist>()> &loader) {
        artistLoader = loader;
    };

    void Song::setFeaturingArtistsLoader(
        const std::function<std::vector<std::shared_ptr<Artist>>()> &loader) {
        featuringArtistsLoader = loader;
    }

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

    std::string Song::getFormattedDuration() const {
        int totalSeconds = getDuration();
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
    }

    std::string Song::toString() const {
        std::string info = "{Musica: " + _title + ", Artista: " + _artist->getName() + ", Duracao: " + getFormattedDuration() + ", Ano: " + std::to_string(_year) + "}";
        return info;
    };

    std::vector<std::shared_ptr<IPlayableObject>>
    Song::getPlayableObjects() const {

        return {std::make_shared<Song>(*this)}; // {} para converer em vector;
    };

    bool Song::operator==(const Entity &other) const {
        const Song *otherSong = dynamic_cast<const Song *>(&other);
        assert(otherSong != nullptr); // TODO exception
        if (otherSong->getId() == this->getId() && otherSong->_title == this->_title) {
            return true;
        }
        return false;
    };

    bool Song::operator!=(const Entity &other) const {
        const Song *otherSong = dynamic_cast<const Song *>(&other);
        assert(otherSong != nullptr); // TODO exception
        if (otherSong->getId() == this->getId() && otherSong->_title == this->_title) {
            return false;
        }
        return true;
    };

    std::string Song::getAudioFilePath() const {
        return _file_path;
    };
} // namespace core
