/**
 * @file UserRepository.cpp
 * @brief Implementação do repositório de usuários
 *
 * @ingroup bd
 * @author Julia Lima
 * @date 2025-11-05
 */


#include "core/bd/UserRepository.hpp"


namespace core {
    UserRepository::UserRepository(std::shared_ptr<SQLite::Database> db) :
        SQLiteRepositoryBase<User>(db, "users") {}

    bool UserRepository::insert(const User& entity) {
        SQLite::Statement query(*_db,
                                "INSERT INTO users (username, home_path, "
                                "input_path, uid) "
                                "VALUES (?, ?, ?, ?);");
        query.bind(1, entity.getUsername());
        query.bind(2, entity.getHomePath());
        query.bind(3, entity.getInputPath());
        #if defined(_WIN32)
                query.bind(4, entity.getUID());
        #else
                query.bind(4, std::to_string(entity.getUID()));
        #endif

        return query.exec() > 0;
    }

    bool UserRepository::update(const User& entity) {
        SQLite::Statement query(*_db,
                                "UPDATE users SET username = ?, home_path = "
                                "?, input_path = ?, uid = ? "
                                "WHERE id = ?;");
        query.bind(1, entity.getUsername());
        query.bind(2, entity.getHomePath());
        query.bind(3, entity.getInputPath());
        #if defined(_WIN32)
                query.bind(4, entity.getUID());
        #else
                query.bind(4, std::to_string(entity.getUID()));
        #endif
        query.bind(5, entity.getId());

        return query.exec() > 0;
    }

    bool UserRepository::save(User& entity) {
        if (entity.getUID() == 0 && count() > 1)
            return false;

        if (entity.getId() == 0)
            return insert(entity);
        else
            return update(entity);
    }

    std::shared_ptr<User> UserRepository::findByUsername(
        const std::string& username) const {
        auto results = findBy("username", username);
        if (!results.empty())
            return results.front();
        return nullptr;
    }

    std::shared_ptr<User> UserRepository::findByUID(const userid& uid) const {
        std::string uid_str;
        #if defined(_WIN32)
            uid_str = uid;
        #else
            uid_str = std::to_string(uid);
        #endif
        auto results = findBy("uid", uid_str);
        if (!results.empty())
            return results.front();
        return nullptr;
    }

    std::shared_ptr<User> UserRepository::mapRowToEntity(SQLite::Statement& query) const {
        unsigned id = static_cast<unsigned>(query.getColumn("id").getInt());
        std::string username = query.getColumn("username").getString();
        std::string home_path = query.getColumn("home_path").getString();
        std::string input_path = query.getColumn("input_path").getString();
        userid uid;
        #if defined(_WIN32)
            uid = query.getColumn("uid").getString();
        #else
            uid = static_cast<userid>(std::stoul(query.getColumn("uid").getString()));
        #endif

        return std::make_shared<User>(id, username, home_path, input_path, uid);
    }
}
