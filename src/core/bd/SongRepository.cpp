#include "core/bd/SongRepository.hpp"
#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"
#include "core/bd/AlbumRepository.hpp"
#include "core/bd/ArtistRepository.hpp"
#include "core/bd/SQLiteRepositoryBase.hpp"
#include "core/bd/UserRepository.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/Song.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
/*
 * CREATE TABLE IF NOT EXISTS songs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL,
    duration INTEGER NOT NULL,
    track_number INTEGER,
    album_id INTEGER,
    artist_id INTEGER NOT NULL,
    release_year INTEGER,
    genre TEXT,
    file_size INTEGER,
    bitrate INTEGER,
    sample_rate INTEGER,
    play_count INTEGER DEFAULT 0,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    user_id INTEGER NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    FOREIGN KEY (album_id) REFERENCES albums(id) ON DELETE SET NULL,
    FOREIGN KEY (artist_id) REFERENCES artists(id) ON DELETE CASCADE
);
*/

namespace core {

    SongRepository::SongRepository(std::shared_ptr<SQLite::Database> db)
        : SQLiteRepositoryBase<Song>(db, "songs") {
    }

    bool SongRepository::insert(Song &entity) {
        std::string sql = "INSERT INTO " + _table_name + " (title, duration, track_number, artist_id, album_id, user_id, release_year) "
                                                    "VALUES (?, ?, ?, ?, ?, ?, ?);";



        SQLite::Statement query = prepare(sql);
        query.bind(1, entity.getTitle());
        query.bind(2, entity.getDuration());
        query.bind(3, entity.getTrackNumber());
        query.bind(4, entity.getArtist()->getId());
        query.bind(7, entity.getYear());
        std::shared_ptr<const Album> album = entity.getAlbum();
        if (album != nullptr)
            query.bind(5, album->getId());
        else
            query.bind(5);
        query.bind(6, entity.getUser()->getId());

        bool success = query.exec() > 0;

        if (success)
            entity.setId(static_cast<unsigned>(getLastInsertId()));

        return success;
    };

    bool SongRepository::update(const Song &entity) {
        std::string sql = "UPDATE " + _table_name + " SET title = ?, artist_id = ?, user_id = ? "
                                                    "WHERE id = ?;"; // nao faz sentido trocar duration

        SQLite::Statement query = prepare(sql);
        query.bind(1, entity.getTitle());
        query.bind(2, entity.getArtist()->getId());
        query.bind(3, entity.getUser()->getId());

        return query.exec() > 0;
    };

    std::shared_ptr<Song> SongRepository::mapRowToEntity(SQLite::Statement &query) const {
        unsigned id = query.getColumn("id").getInt();
        std::string title = query.getColumn("title").getString();
        unsigned duration = query.getColumn("duration").getInt();
        unsigned track_number = query.getColumn("track_number").getInt();
        unsigned artist_id = query.getColumn("artist_id").getInt();
        unsigned user_id = query.getColumn("user_id").getInt();
        int year = query.getColumn("release_year").getInt();

        auto song = std::make_shared<Song>(id, title, artist_id, user_id);
        song->setDuration(duration);
        song->setTrackNumber(track_number);
        song->setYear(year);

        auto artistLoader = [this, song]() -> std::shared_ptr<Artist> {
            return this->getArtist(*song);
        };

        auto featuringArtistsLoader = [this, id]() -> std::vector<std::shared_ptr<Artist>> {
            Song tempSong;
            tempSong.setId(id);
            return this->getFeaturingArtists(tempSong);
        };

        auto albumLoader = [this, song]() -> std::shared_ptr<Album> {
            return this->getAlbum(*song);
        };

        song->setArtistLoader(artistLoader);
        song->setFeaturingArtistsLoader(featuringArtistsLoader);
        song->setAlbumLoader(albumLoader);

        auto user_repo = UserRepository(_db);
        song->setUser(*user_repo.findById(user_id));

        return song;
    }
    bool SongRepository::save(Song &entity) {
        if (entity.getId() == 0) {
            return insert(entity);
        } else {
            return update(entity);
        }
    };

    bool SongRepository::remove(unsigned id) {
        std::string sql = "DELETE FROM " + _table_name + " WHERE id = ?;";

        SQLite::Statement query = prepare(sql);

        query.bind(1, id);

        return query.exec() > 0;
    };

    std::vector<std::shared_ptr<Song>>
    SongRepository::findByTitleAndUser(const std::string &title,
                                       const User &user) const {
        std::string sql = "SELECT * FROM " + _table_name + " WHERE title LIKE ? AND user_id = ? ORDER BY title;";

        SQLite::Statement query = prepare(sql);
        query.bind(1, "%" + title + "%"); // "%" nao considera char especial
        query.bind(2, user.getId());

        std::vector<std::shared_ptr<Song>> songs;
        while (query.executeStep()) {
            songs.push_back(mapRowToEntity(query));
        }

        return songs;
    }

    std::vector<std::shared_ptr<Song>>
    SongRepository::findByUser(const User &user) const {

        std::string sql = "SELECT * FROM " + _table_name + " WHERE user_id = ? ORDER BY title;";

        SQLite::Statement query = prepare(sql);

        query.bind(1, user.getId());

        std::vector<std::shared_ptr<Song>> songs;

        while (query.executeStep()) {
            songs.push_back(mapRowToEntity(query));
        }

        return songs;
    };

    std::vector<std::shared_ptr<Song>>
    SongRepository::findByArtist(const Artist &artist) const {

        std::string sql = "SELECT * FROM " + _table_name + " WHERE artist_id = ? ORDER BY title;";

        SQLite::Statement query = prepare(sql);

        query.bind(1, artist.getId());

        std::vector<std::shared_ptr<Song>> songs;

        while (query.executeStep()) {
            songs.push_back(mapRowToEntity(query));
        }

        return songs;
    };

    std::vector<std::shared_ptr<Song>>
    SongRepository::findByAlbum(const Album &album) const {
        std::string sql = "SELECT * FROM " + _table_name + " WHERE album_id = ? ORDER BY title;";

        SQLite::Statement query = prepare(sql);

        query.bind(1, album.getId());

        std::vector<std::shared_ptr<Song>> songs;

        while (query.executeStep()) {
            songs.push_back(mapRowToEntity(query));
        }

        return songs;
    };

    std::shared_ptr<Song> SongRepository::findById(unsigned id) const {
        std::string sql = "SELECT * FROM " + _table_name + " WHERE id = ?;";

        SQLite::Statement query = prepare(sql);
        query.bind(1, id);

        if (query.executeStep()) {
            return mapRowToEntity(query);
        }

        return nullptr;
    };

    std::shared_ptr<Album> SongRepository::getAlbum(const Song &song) const {
        std::string sql = "SELECT album_id FROM songs WHERE id = ?;";
        SQLite::Statement query = prepare(sql);
        query.bind(1, song.getId());

        if (query.executeStep()) {
            unsigned album_id = query.getColumn("album_id").getInt();
            if (album_id > 0) {
                AlbumRepository album_repo(_db);
                return album_repo.findById(album_id);
            }
        }
        return nullptr;
    };

    std::shared_ptr<Artist> SongRepository::getArtist(const Song &song) const {
        std::string sql = "SELECT artist_id FROM songs WHERE id = ?;";
        SQLite::Statement query = prepare(sql);
        query.bind(1, song.getId());

        if (query.executeStep()) {
            unsigned artist_id = query.getColumn("artist_id").getInt();
            if (artist_id > 0) {
                ArtistRepository artist_repo(_db);
                return artist_repo.findById(artist_id);
            }
        }
        return nullptr;
    };

    std::vector<std::shared_ptr<Artist>>
    SongRepository::getFeaturingArtists(const Song &song) const {
        std::string sql = "SELECT a.* FROM artists a "
                          "JOIN song_artists sa ON a.id = sa.artist_id "
                          "WHERE sa.song_id = ? AND sa.is_principal = 0;";

        SQLite::Statement query = prepare(sql);
        query.bind(1, song.getId());

        ArtistRepository artist_repo(_db);
        std::vector<std::shared_ptr<Artist>> artists;

        while (query.executeStep()) {
            unsigned id = query.getColumn("id").getInt();
            std::string name = query.getColumn("name").getString();
            unsigned user_id = query.getColumn("user_id").getInt();

            artists.push_back(std::make_shared<Artist>(id, name, user_id));
        }

        return artists;
    };

    size_t SongRepository::count() const {
        std::string sql = "SELECT COUNT(*) FROM " + _table_name + ";";

        SQLite::Statement query = prepare(sql);

        if (query.executeStep()) {
            return query.getColumn(0).getInt();
        }

        return 0;
    }

    bool SongRepository::addFeaturingArtist(const Song &song, const Artist &artist, const User &user) const {
        std::string sql = "INSERT OR IGNORE INTO song_artists (song_id, artist_id, user_id, is_principal) "
                          "VALUES (?, ?, ?, 0);";

        SQLite::Statement query = prepare(sql);
        query.bind(1, song.getId());
        query.bind(2, artist.getId());
        query.bind(3, user.getId());

        return query.exec() > 0;
    }

    bool SongRepository::removeFeaturingArtist(const Song &song, const Artist &artist) const {
        std::string sql = "DELETE FROM song_artists "
                          "WHERE song_id = ? AND artist_id = ? AND is_principal = 0;";

        SQLite::Statement query = prepare(sql);
        query.bind(1, song.getId());
        query.bind(2, artist.getId());

        return query.exec() > 0;
    }

    bool SongRepository::setPrincipalArtist(const Song &song, const Artist &artist, const User &user) const {

        std::string delete_sql = "DELETE FROM song_artists WHERE song_id = ? AND is_principal = 1;";
        SQLite::Statement delete_query = prepare(delete_sql);
        delete_query.bind(1, song.getId());
        delete_query.exec();

        std::string insert_sql = "INSERT OR REPLACE INTO song_artists (song_id, artist_id, user_id, is_principal) "
                                 "VALUES (?, ?, ?, 1);";
        SQLite::Statement insert_query = prepare(insert_sql);
        insert_query.bind(1, song.getId());
        insert_query.bind(2, artist.getId());
        insert_query.bind(3, user.getId());

        return insert_query.exec() > 0;
    }

} // namespace core
