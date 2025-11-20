/**
 * @file PlaylistRepository.cpp
 * @brief Implementação do repositório de playlists
 *
 * @ingroup bd
 * @author Eloy Maciel
 * @date 2025-11-09
 */

#include "core/bd/PlaylistRepository.hpp"
#include "core/bd/UserRepository.hpp"

namespace core {
    PlaylistRepository::PlaylistRepository(std::shared_ptr<SQLite::Database> db)
        : SQLiteRepositoryBase<Playlist>(db, "playlists") {}

    bool PlaylistRepository::insert(const Playlist& entity) {
        SQLite::Statement query(*_db,
                                "INSERT INTO playlists (title, user_id) "
                                "VALUES (?, ?);");
        query.bind(1, entity.getTitle());
        query.bind(2, entity.getUser()->getId());

        if (!query.exec())
            return false;

        for (const auto& song : entity.getSongs()) {
            if (!addSongToPlaylist(entity,
                                   *std::dynamic_pointer_cast<Song>(song)))
                return false;
        }

        return true;
    }

    bool PlaylistRepository::addSongToPlaylist(const Playlist& playlist,
                                               const Song& song) {
        SQLite::Statement query(*_db,
                                "INSERT INTO playlist_songs (playlist_id, "
                                "song_id) "
                                "VALUES (?, ?);");
        query.bind(1, playlist.getId());
        query.bind(2, song.getId());

        return query.exec() > 0;
    }

    bool PlaylistRepository::update(const Playlist& entity) {
        SQLite::Statement query(*_db,
                                "UPDATE playlists SET title = ?, user_id = ? "
                                "WHERE id = ?;");
        query.bind(1, entity.getTitle());
        query.bind(2, entity.getUser()->getId());
        query.bind(3, entity.getId());

        if (!query.exec())
            return false;

        SQLite::Statement deleteQuery(*_db,
                                      "DELETE FROM playlist_songs WHERE "
                                      "playlist_id = ?;");
        deleteQuery.bind(1, entity.getId());

        if (!deleteQuery.exec())
            return false;

        for (const auto& song : entity.getSongs()) {
            if (!addSongToPlaylist(entity,
                                   *std::dynamic_pointer_cast<Song>(song)))
                return false;
        }

        return true;
    }

    std::shared_ptr<Playlist>
    PlaylistRepository::mapRowToEntity(SQLite::Statement& query) const {
        unsigned id = query.getColumn("id").getUInt();
        std::string title = query.getColumn("title").getString();
        unsigned user_id = query.getColumn("user_id").getUInt();
        Playlist playlist(id, title);

        std::shared_ptr<UserRepository> userRepo =
            std::make_shared<UserRepository>(_db);
        std::shared_ptr<User> user = userRepo->findById(user_id);
        playlist.setUser(*user);

        auto playlistPtr = std::make_shared<Playlist>(playlist);
        playlist.setSongsLoader(
            [this, playlistPtr]() -> std::vector<std::shared_ptr<Song>> {
                return this->getSongs(*playlistPtr);
            });

        return std::make_shared<Playlist>(playlist);
    }

    std::vector<std::shared_ptr<Playlist>>
    PlaylistRepository::findByTitleAndUser(const std::string& title,
                                           const User& user) const {
        SQLite::Statement query(
            *_db,
            "SELECT * FROM playlists WHERE title = ? AND user_id = ?;");
        query.bind(1, title);
        query.bind(2, user.getId());

        std::vector<std::shared_ptr<Playlist>> playlists;
        while (query.executeStep()) {
            playlists.push_back(mapRowToEntity(query));
        }
        return playlists;
    }

    std::vector<std::shared_ptr<Playlist>>
    PlaylistRepository::findByUser(const User& user) const {
        SQLite::Statement query(*_db,
                                "SELECT * FROM playlists WHERE user_id = ?;");
        query.bind(1, user.getId());

        std::vector<std::shared_ptr<Playlist>> playlists;
        while (query.executeStep()) {
            playlists.push_back(mapRowToEntity(query));
        }
        return playlists;
    }

    std::vector<std::shared_ptr<Song>>
    PlaylistRepository::getSongs(const Playlist& playlist) const {
        SQLite::Statement query(*_db,
                                "SELECT s.* FROM songs s "
                                "JOIN playlist_songs ps ON s.id = ps.song_id "
                                "WHERE ps.playlist_id = ?;");
        query.bind(1, playlist.getId());

        std::vector<std::shared_ptr<Song>> songs;
        while (query.executeStep()) {
            unsigned song_id = query.getColumn("id").getUInt();
            std::string file_path = query.getColumn("file_path").getString();
            std::string title = query.getColumn("title").getString();
            unsigned artist_id = query.getColumn("artist_id").getUInt();

            Song song(song_id, file_path, title, artist_id);
            song.setUser(*playlist.getUser());
            songs.push_back(std::make_shared<Song>(song));
        }

        return songs;
    }
}  // namespace core
