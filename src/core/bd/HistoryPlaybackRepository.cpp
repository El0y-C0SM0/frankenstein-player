/**
 * @file HistoryPlaybackRepository.cpp
 * @brief Implementação do repositório de histórico de reproduções
 *
 * @ingroup bd
 * @author Eloy Maciel
 * @date 2025-11-05
 */


#include "core/bd/HistoryPlaybackRepository.hpp"
#include "core/bd/UserRepository.hpp"
#include "core/bd/SongRepository.hpp"
#include "core/entities/User.hpp"


namespace core {
    HistoryPlaybackRepository::HistoryPlaybackRepository()
        : SQLiteRepositoryBase<HistoryPlayback>(
              nullptr,
              "playback_history") {}

    HistoryPlaybackRepository::HistoryPlaybackRepository(
        std::shared_ptr<SQLite::Database> db)
        : SQLiteRepositoryBase<HistoryPlayback>(
              db,
              "playback_history") {}

    bool HistoryPlaybackRepository::insert(const HistoryPlayback& entity) {
        std::string sql =
            "INSERT INTO " + _table_name +
            " (user_id, song_id, played_at) "
            "VALUES (?, ?, ?);";

        SQLite::Statement query = prepare(sql);
        query.bind(1, static_cast<int>(entity.getUser()->getId()));
        query.bind(2, static_cast<int>(entity.getSong()->getId()));
        query.bind(3, entity.getPlayedAt());

        return query.exec() > 0;
    }

    bool HistoryPlaybackRepository::update(const HistoryPlayback& entity) {
        std::string sql =
            "UPDATE " + _table_name +
            " SET user_id = ?, song_id = ?, played_at = ? "
            "WHERE id = ?;";

        SQLite::Statement query = prepare(sql);
        query.bind(1, static_cast<int>(entity.getUser()->getId()));
        query.bind(2, static_cast<int>(entity.getSong()->getId()));
        query.bind(3, entity.getPlayedAt());
        query.bind(4, static_cast<int>(entity.getId()));

        return query.exec() > 0;
    }

    std::shared_ptr<HistoryPlayback>
    HistoryPlaybackRepository::mapRowToEntity(SQLite::Statement& query) const {
        unsigned id = static_cast<unsigned>(query.getColumn("id").getInt());
        unsigned user_id = static_cast<unsigned>(query.getColumn("user_id").getInt());
        unsigned song_id = static_cast<unsigned>(query.getColumn("song_id").getInt());
        std::time_t played_at = query.getColumn("played_at").getInt64();

        auto user_repo = UserRepository(_db);
        auto user = user_repo.findById(user_id);

        auto song_repo = SongRepository(_db);
        auto song = song_repo.findById(song_id);

        return std::make_shared<HistoryPlayback>(
            id,
            *user,
            *song,
            played_at);
    }

    bool HistoryPlaybackRepository::save(HistoryPlayback& entity) {
        if (entity.getId() == 0)
            return insert(entity);
        else
            return update(entity);
    }

    std::vector<std::shared_ptr<HistoryPlayback>>
    HistoryPlaybackRepository::findByUser(const User& user) const {
        std::vector<std::shared_ptr<HistoryPlayback>> results;
        std::string sql =
            "SELECT * FROM " + _table_name +
            " WHERE user_id = ? "
            "ORDER BY played_at DESC;";

        SQLite::Statement query = prepare(sql);
        query.bind(1, static_cast<int>(user.getId()));

        while (query.executeStep())
            results.push_back(this->mapRowToEntity(query));

        return results;
    }

    bool HistoryPlaybackRepository::insertHistoryPlayback(const HistoryPlayback& entity) {
        return insert(entity);
    }

    bool HistoryPlaybackRepository::insertMultipleHistoryPlaybacks(
        const std::vector<HistoryPlayback>& entities) {
        bool all_inserted = true;
        for (const auto& entity : entities) {
            if (!insert(entity))
                all_inserted = false;
        }
        return all_inserted;
    }

    unsigned HistoryPlaybackRepository::countPlaybacksBySongAndUser(const Song& song) const {
        std::string sql =
            "SELECT COUNT(1) FROM " + _table_name +
            " WHERE song_id = ? AND user_id = ?;" +
            " ORDER BY played_at DESC;";

        SQLite::Statement query = prepare(sql);
        query.bind(1, static_cast<int>(song.getId()));
        query.bind(2, static_cast<int>(song.getUser()->getId()));

        if (query.executeStep())
            return static_cast<unsigned>(query.getColumn(0).getInt());

        return 0;
    }

    unsigned HistoryPlaybackRepository::countPlaybacksBySongAndUser(const Song& song, const User& user) const {
        std::string sql =
            "SELECT COUNT(1) FROM " + _table_name +
            " WHERE song_id = ? AND user_id = ?;";

        SQLite::Statement query = prepare(sql);
        query.bind(1, static_cast<int>(song.getId()));
        query.bind(2, static_cast<int>(user.getId()));

        if (query.executeStep())
            return static_cast<unsigned>(query.getColumn(0).getInt());

        return 0;
    }
}  // namespace core
