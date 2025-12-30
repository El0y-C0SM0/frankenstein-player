#include "core/bd/AlbumRepository.hpp"
#include "SQLiteCpp/Statement.h"
#include "core/bd/ArtistRepository.hpp"
#include "core/bd/SQLiteRepositoryBase.hpp"
#include "core/bd/SongRepository.hpp"
#include "core/entities/Album.hpp"
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

    bool AlbumRepository::insert(Album &entity) {
        std::string sql = "INSERT INTO " + _table_name + " (title, release_year, genre, user_id) " + "VALUES (?, ?, ?, ?);";
        SQLite::Statement query = prepare(sql);

        query.bind(1, entity.getTitle());
        query.bind(2, entity.getYear());
        query.bind(3, entity.getGenre());
        query.bind(4, entity.getUser()->getId());

        bool success = query.exec() > 0;

        if (success)
            entity.setId(static_cast<unsigned>(getLastInsertId()));

        return success;
    };

    bool AlbumRepository::update(const Album &entity) {
        std::string sql =
            "UPDATE " + _table_name + " SET title = ?, release_year = ?, genre = ?" + "WHERE id = ?";

        SQLite::Statement query = prepare(sql);
        query.bind(1, entity.getTitle());
        query.bind(2, entity.getYear());
        query.bind(3, entity.getGenre());
        query.bind(4, entity.getId());

        return query.exec() > 0;
    };

    std::shared_ptr<Album>
    AlbumRepository::mapRowToEntity(SQLite::Statement &query) const {
        unsigned id = query.getColumn("id").getInt();
        std::string title = query.getColumn("title").getString();
        int year = query.getColumn("release_year").getInt();
        std::string genre = query.getColumn("genre").getString();
        unsigned user_id = query.getColumn("user_id").getInt();

        // TODO carregar usuário do album
        // auto album = std::make_shared<Album>(id, title, year, genre, user_id);
        std::shared_ptr<Album> album;

        auto artists_loader = [this, id]() -> std::vector<std::shared_ptr<Artist>> {
            Album tempAlbum;
            tempAlbum.setId(id);
            return this->getFeaturingArtists(tempAlbum);
        };

        auto songs_loader = [this, album]() -> std::vector<std::shared_ptr<Song>> {
            return this->getSongs(*album);
        };

        album->setSongsLoader(songs_loader);
        album->setFeaturingArtistsLoader(artists_loader);
        return album;
    };

    bool AlbumRepository::save(Album &entity) {
        if (entity.getId() == 0) {
            return insert(entity);
        } else {
            return update(entity);
        }
    };

    bool AlbumRepository::remove(unsigned id) {

        std::string sql = "DELETE FROM " + _table_name + " WHERE id = ?;";

        SQLite::Statement query = prepare(sql);

        query.bind(1, id);

        return query.exec() > 0;
    };

    std::vector<std::shared_ptr<Album>>
    AlbumRepository::findByTitleAndUser(const std::string &title,
                                        const User &user) const {
        std::string sql = "SELECT * FROM " + _table_name + " WHERE title LIKE ? AND user_id = ?;";

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
    AlbumRepository::findByUser(const User &user) const {
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
    AlbumRepository::findByArtist(const std::string &artist_name) const {
        // BUSCAR APENAS PELO PRINCIPAL?

        std::string sql = "SELECT alb.* FROM albums alb "
                          "JOIN album_artists aa ON alb.id = aa.album_id "
                          "JOIN artists art ON aa.artist_id = art.id "
                          "WHERE art.name LIKE ? AND aa.is_principal = 1;";

        SQLite::Statement query = prepare(sql);
        query.bind(1, "%" + artist_name + "%");

        std::vector<std::shared_ptr<Album>> albums;
        while (query.executeStep()) {
            albums.push_back(mapRowToEntity(query));
        }

        return albums;
    }

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
    AlbumRepository::getSongs(const Album &album) const {
        SongRepository songRepo(_db);
        return songRepo.findByAlbum(album);
    }

    std::vector<std::shared_ptr<Artist>>
    AlbumRepository::getFeaturingArtists(const Album &album) const {
        std::string sql = "SELECT a.* FROM artists a "
                          "JOIN album_artists aa ON a.id = aa.artist_id "
                          "WHERE aa.album_id = ? AND aa.is_principal = 0;";

        SQLite::Statement query = prepare(sql);
        query.bind(1, album.getId());

        std::vector<std::shared_ptr<Artist>> artists;
        while (query.executeStep()) {
            unsigned id = query.getColumn("id").getInt();
            std::string name = query.getColumn("name").getString();
            unsigned user_id = query.getColumn("user_id").getInt();

            // TODO carregar usuário do artista
            // artists.push_back(std::make_shared<Artist>(id, name, user_id));
            artists.push_back(std::make_shared<Artist>());
        }

        return artists;
    }

    std::shared_ptr<Artist>
    AlbumRepository::getArtist(const Album &album) const {
        ArtistRepository artist_Repo(_db);
        return artist_Repo.findById(album.getArtistId());
    };

    size_t AlbumRepository::count() const {
        std::string sql = "SELECT COUNT(*) FROM " + _table_name + ";";

        SQLite::Statement query = prepare(sql);

        if (query.executeStep()) {
            return query.getColumn(0).getInt();
        }

        return 0;
    }

    bool AlbumRepository::addFeaturingArtist(const Album &album, const Artist &artist, const User &user) const {
        std::string sql = "INSERT OR IGNORE INTO album_artists (album_id, artist_id, user_id, is_principal) "
                          "VALUES (?, ?, ?, 0);";

        SQLite::Statement query = prepare(sql);
        query.bind(1, album.getId());
        query.bind(2, artist.getId());
        query.bind(3, user.getId());

        return query.exec() > 0;
    }

    bool AlbumRepository::setPrincipalArtist(const Album &album, const Artist &artist, const User &user) const {
        std::string delete_sql = "DELETE FROM album_artists WHERE album_id = ? AND is_principal = 1;";
        SQLite::Statement delete_query = prepare(delete_sql);
        delete_query.bind(1, album.getId());
        delete_query.exec();

        std::string insert_sql = "INSERT OR REPLACE INTO album_artists (album_id, artist_id, user_id, is_principal) "
                                 "VALUES (?, ?, ?, 1);";
        SQLite::Statement insert_query = prepare(insert_sql);
        insert_query.bind(1, album.getId());
        insert_query.bind(2, artist.getId());
        insert_query.bind(3, user.getId());

        return insert_query.exec() > 0;
    }

} // namespace core
