#include "core/bd/SongRepository.hpp"
#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"
#include "core/bd/AlbumRepository.hpp"
#include "core/bd/ArtistRepository.hpp"
#include "core/bd/SQLiteRepositoryBase.hpp"
#include "core/bd/UserRepository.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/Song.hpp"
#include <memory>
#include <string>
/*
 * CREATE TABLE IF NOT EXISTS songs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL,
    duration INTEGER NOT NULL,
    track_number INTEGER,
    album_id INTEGER,
    artist_id INTEGER NOT NULL,
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
    SongRepository::SongRepository()
        : SQLiteRepositoryBase<Song>(nullptr, "songs") {}

    SongRepository::SongRepository(std::shared_ptr<SQLite::Database> db)
        : SQLiteRepositoryBase<Song>(db, "songs") {}

    bool SongRepository::insert(const Song& entity) {
        std::string sql = "INSERT INTO " + _table_name
                          + " (title, duration, artist_id, user_id) "
                          + "VALUES(?, ?, ?, ?);";

        SQLite::Statement query = prepare(sql);
        query.bind(1, entity.getTitle());
        query.bind(2, entity.getDuration());
        query.bind(3, entity.getArtist()->getId());
        query.bind(4, entity.getUser()->getId());

        return query.exec() > 0;
    };

    bool SongRepository::update(const Song& entity) {
        std::string sql = "UPDATE " + _table_name
                          + " SET title = ?, artist_id = ?, user_id = ? "
                            "WHERE id = ?;";  // nao faz sentido trocar duration

        SQLite::Statement query = prepare(sql);
        query.bind(1, entity.getTitle());
        query.bind(2, entity.getArtist()->getId());
        query.bind(3, entity.getUser()->getId());

        return query.exec() > 0;
    };

    std::shared_ptr<Song>
    SongRepository::mapRowToEntity(SQLite::Statement& query) const {
        unsigned id = query.getColumn("id").getInt();
        std::string title = query.getColumn("title").getString();
        unsigned artist_id = query.getColumn("artist_id").getInt();
        unsigned user_id = query.getColumn("user_id").getInt();

        // TODO repensar implementação dos contrutores de Song

        return std::make_shared<Song>(id, title, artist_id, user_id);
    }
    bool SongRepository::save(Song& entity) {
        if (entity.getId() == 0) {
            return insert(entity);
        } else {
            return update(entity);
        }
    };

    bool SongRepository::remove(unsigned id) {
        // todo id exception caso id for negativo entre outros casos

        std::string sql = "DELETE FROM " + _table_name + " WHERE id = ?;";

        SQLite::Statement query = prepare(sql);

        query.bind(1, id);

        return query.exec() > 0;
    };

    std::vector<std::shared_ptr<Song>>
    SongRepository::findByTitleAndUser(const std::string& title,
                                       const User& user) const {
        std::string sql = "SELECT * FROM " + _table_name
                          + " WHERE title LIKE ? AND user_id = ?;";

        SQLite::Statement query = prepare(sql);
        query.bind(1, "%" + title + "%");  // "%" nao considera char especial
        query.bind(2, user.getId());

        std::vector<std::shared_ptr<Song>> songs;
        while (query.executeStep()) {
            songs.push_back(mapRowToEntity(query));
        }

        return songs;
    }

    std::vector<std::shared_ptr<Song>>
    SongRepository::findByUser(const User& user) const {

        std::string sql = "SELECT * FROM " + _table_name
                          + " WHERE user_id = ? ORDER BY title;";

        SQLite::Statement query = prepare(sql);

        query.bind(1, user.getId());

        std::vector<std::shared_ptr<Song>> songs;

        while (query.executeStep()) {
            songs.push_back(mapRowToEntity(query));
        }

        return songs;
    };

    std::vector<std::shared_ptr<Song>>
    SongRepository::findByArtist(const Artist& artist) const {

        std::string sql = "SELECT * FROM " + _table_name
                          + " WHERE artist_id = ? ORDER BY title;";

        SQLite::Statement query = prepare(sql);

        query.bind(1, artist.getId());

        std::vector<std::shared_ptr<Song>> songs;

        while (query.executeStep()) {
            songs.push_back(mapRowToEntity(query));
        }

        return songs;
    };

    std::vector<std::shared_ptr<Song>>
    SongRepository::findByAlbum(const Album& album) const {
        std::string sql = "SELECT * FROM " + _table_name
                          + " WHERE album_id = ? ORDER BY title;";

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

    std::shared_ptr<Album> SongRepository::getAlbum(const Song& song) const {
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

    std::shared_ptr<Artist> SongRepository::getArtist(const Song& song) const {
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
    SongRepository::getFeaturingArtists(const Song& song) const {
        // TODO
    };

    size_t SongRepository::count() const {
        std::string sql = "SELECT COUNT(*) FROM " + _table_name + ";";

        SQLite::Statement query = prepare(sql);

        if (query.executeStep()) {
            return query.getColumn(0).getInt();
        }

        return 0;
    }

}  // namespace core
