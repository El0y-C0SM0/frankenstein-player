
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
               std::string file_path,
               std::string title,
               unsigned artist_id)
        : Entity(id),
          _file_path(std::move(file_path)),
          _title(std::move(title)),
          _artist_id(artist_id) {};

    Song::Song(unsigned id,
               const std::string &title,
               unsigned &artist,
               unsigned &user_id)
        : Entity(id),
          _title(title),
          _artist_id(artist),
          user_id(user_id)
    // Construtor focado em IDs para integração com banco de dados
    {};

    Song::Song(const std::string &title, Artist &artist, Album &album, User &user)
        : _title(title),
          _user(user),
          _artist(std::make_shared<Artist>(artist)),
          _album(std::make_shared<Album>(album)) {};

    Song::~Song() = default;

    // Getters

    std::string Song::getFilePath() const {
        return _file_path;
    };

    std::string Song::getTitle() const {
        return _title;
    };

    std::shared_ptr<const Artist> Song::getArtist() const {
        return artistLoader ? artistLoader() : _artist;
    };

    std::vector<unsigned> Song::getFeaturingArtistsId() const {
        return _featuring_artists_ids;
    };

    std::vector<std::shared_ptr<const Artist>> Song::getFeaturingArtists() {
        if (featuringArtistsLoader) {
            auto allArtists = featuringArtistsLoader();
            std::vector<std::shared_ptr<const Artist>> featuring;

            if (allArtists.size() > 1) {
                for (auto it = allArtists.begin(); it != allArtists.end(); ++it) {
                    featuring.push_back(std::const_pointer_cast<const Artist>(*it));
                }
            }

            return featuring;
        }
        return {};
    };

    std::shared_ptr<const Album> Song::getAlbum() const {
        return albumLoader ? albumLoader() : _album;
    };

    int Song::getDuration() const {
        if (_duration > 0) {
            return _duration;
        }

        if (getAudioFilePath().empty()) {
            throw std::runtime_error("Caminho do arquivo está vazio para a música: " + _title);
        }
        try {
            TagLib::FileRef file(_file_path.c_str());
            if (file.isNull()) {
                throw std::runtime_error("Não foi possível abrir o arquivo: " + _file_path);
            }
            if (!file.audioProperties()) {
                throw std::runtime_error("Arquivo não contém propriedades de áudio: " + _file_path);
            }

            int duration = file.audioProperties()->lengthInSeconds();
            const_cast<Song *>(this)->_duration = duration;

            return duration;
        } catch (const std::exception &e) {
            throw std::runtime_error(std::string("Falha ao obter duração de '") +
                                     _file_path + "': " + e.what());
        }
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

    std::shared_ptr<User> Song::getUser() const {
        return std::make_shared<User>(_user);
    };

    // Setters

    void Song::setUser(const User &user) {
        // pensei em que musicar ter um vector de shared_ptr(user) para que uma
        // musica ser compartilhada
        // User seria por Usuarios do computador
        // auto id = user.getId();
        // _user.setId(id);
        _user = user;
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

    void Song::setTrackNumber(unsigned track_number) {
        _track_number = track_number;
    };

    void Song::setDuration(int sec) {
        _duration = sec;
    }

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
        std::string path = _user.getHomePath() + "/" + getArtist()->getName() + "/";
        if (getAlbum())
            path += getAlbum()->getName() + "/";
        else
            path += "Singles/";
        path += getTitle() + ".mp3";

        return path;
    };
} // namespace core
