#include "core/bd/AlbumRepository.hpp"
#include "SQLiteCpp/Statement.h"
#include "core/bd/ArtistRepository.hpp"
#include "core/bd/SQLiteRepositoryBase.hpp"
#include "core/bd/SongRepository.hpp"
#include "core/entities/Song.hpp"
#include <memory>
#include <string>

/*--
 * Tabela de álbuns
    CREATE TABLE IF NOT EXISTS albums (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT UNIQUE NOT NULL,
    release_year INTEGER,
    genre TEXT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    user_id INTEGER NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE);
*/

namespace core {

    AlbumRepository::AlbumRepository(std::shared_ptr<SQLite::Database> db)
        : core::SQLiteRepositoryBase<Album>(db, "albums") {};

    bool AlbumRepository::insert(const Album& entity) {
        std::string sql = "INSERT INTO " + _table_name
                          + " (title, release_year, genre, user_id) "
                          + "VALUES (?, ?, ?, ?);";
        SQLite::Statement query = prepare(sql);

        query.bind(1, entity.getName());
        query.bind(2, entity.getYear());
        query.bind(3, entity.getGenre());
        query.bind(4, entity.getUser()->getId());

        return query.exec() > 0;
    };

    bool AlbumRepository::update(const Album& entity) {
        std::string sql =
            "UPDATE " + _table_name
            + " SET title = ?, release_year = ?, genre = ?, user_id = ? "
            + "WHERE id = ?";

        SQLite::Statement query = prepare(sql);
        query.bind(1, entity.getName());
        query.bind(2, entity.getYear());
        query.bind(3, entity.getGenre());
        query.bind(4, entity.getUser()->getId());
        query.bind(5, entity.getId());

        return query.exec() > 0;
    };

    std::shared_ptr<Album>
    AlbumRepository::mapRowToEntity(SQLite::Statement& query) const {
        unsigned id = query.getColumn("id").getInt();
        std::string title = query.getColumn("title").getString();
        int year = query.getColumn("release_year").getInt();
        std::string genre = query.getColumn("genre").getString();
        unsigned user_id = query.getColumn("user_id").getInt();

        return std::make_shared<Album>(id, title, year, genre, user_id);
    };

    bool AlbumRepository::save(Album& entity) {
        if (entity.getId() == 0) {
            return insert(entity);
        } else {
            return update(entity);
        }
    };

    bool AlbumRepository::remove(unsigned id) {
        // todo id exception caso id for negativo entre outros casos

        std::string sql = "DELETE FROM " + _table_name + " WHERE id = ?;";

        SQLite::Statement query = prepare(sql);

        query.bind(1, id);

        return query.exec() > 0;
    };

    std::vector<std::shared_ptr<Album>>
    AlbumRepository::findByTitleAndUser(const std::string& title,
                                        const User& user) const {
        std::string sql = "SELECT * FROM " + _table_name
                          + " WHERE title LIKE ? AND user_id = ?;";

        SQLite::Statement query = prepare(sql);
        query.bind(1, "%" + title + "%");
        query.bind(2, user.getId());

        std::vector<std::shared_ptr<Album>> albums;
        while (query.executeStep()) {
            albums.push_back(mapRowToEntity(query));
        }

        return albums;
    }

    std::vector<std::shared_ptr<Album>>
    AlbumRepository::findByUser(const User& user) const {
        std::string sql =
            "SELECT * FROM " + _table_name + " WHERE user_id = ?;";

        SQLite::Statement query = prepare(sql);
        query.bind(1, user.getId());

        std::vector<std::shared_ptr<Album>> albums;
        while (query.executeStep()) {
            albums.push_back(mapRowToEntity(query));
        }

        return albums;
    }

    std::vector<std::shared_ptr<Album>>
    AlbumRepository::findByArtist(const std::string& artist) const {
        // TODO implementar quando tiver ArtistRepository

    };

    std::shared_ptr<Album> AlbumRepository::findById(unsigned id) const {
        std::string sql = "SELECT * FROM " + _table_name + " WHERE id = ?;";

        SQLite::Statement query = prepare(sql);
        query.bind(1, id);

        if (query.executeStep()) {
            return mapRowToEntity(query);
        }

        return nullptr;
    }

    std::vector<std::shared_ptr<Song>>
    AlbumRepository::getSongs(const Album& album) const {
        SongRepository songRepo(_db);
        return songRepo.findByAlbum(album);
    }

    std::vector<std::shared_ptr<Artist>>
    AlbumRepository::getFeaturingArtists(const Album& album) const {
        // TODO
    };

    std::shared_ptr<Artist>
    AlbumRepository::getArtist(const Album& album) const {
        ArtistRepository artist_Repo(_db);
        return artist_Repo.findById(album.getArtist()->getId());
    };

    size_t AlbumRepository::count() const {
        std::string sql = "SELECT COUNT(*) FROM " + _table_name + ";";

        SQLite::Statement query = prepare(sql);

        if (query.executeStep()) {
            return query.getColumn(0).getInt();
        }

        return 0;
    }

}  // namespace core
