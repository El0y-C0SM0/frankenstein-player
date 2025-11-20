/**
 * @file SQLiteRepositoryBase.cpp
 * @brief Implementação de repositórios SQLite
 *
 * @ingroup bd
 * @author Eloy Maciel
 * @date 2025-11-04
 */

#ifndef SQLITE_REPOSITORY_BASE_TPP
#define SQLITE_REPOSITORY_BASE_TPP

// #include "core/bd/SQLiteRepositoryBase.hpp"

#include <cstddef>
#include <memory>

namespace core {
    template <typename T>
    SQLiteRepositoryBase<T>::SQLiteRepositoryBase(
        std::shared_ptr<SQLite::Database> db,
        const std::string& table_name)
        : _db(db), _table_name(table_name) {}

    template <typename T>
    SQLite::Statement SQLiteRepositoryBase<T>::prepare(
        const std::string& sql) const {
        return SQLite::Statement(*_db, sql);
    }

    template <typename T>
    std::vector<std::shared_ptr<T>>
    SQLiteRepositoryBase<T>::findBy(const std::string& field,
                                   const std::string& value) const {
        std::vector<std::shared_ptr<T>> results;
        std::string sql = "SELECT * FROM " + _table_name +
                          " WHERE " + field + " = ?";
        SQLite::Statement query = prepare(sql);
        query.bind(1, value);

        while (query.executeStep())
            results.push_back(this->mapRowToEntity(query));

        return results;
    }

    template <typename T>
    const std::string& SQLiteRepositoryBase<T>::getTableName() const {
        return _table_name;
    }

    template <typename T>
    std::shared_ptr<T> SQLiteRepositoryBase<T>::findById(unsigned id) const {
        std::string sql = "SELECT * FROM " + _table_name + " WHERE id = ?";
        SQLite::Statement query = prepare(sql);
        query.bind(1, static_cast<int>(id));

        if (query.executeStep())
            return this->mapRowToEntity(query);

        return nullptr;
    }

    template <typename T>
    std::vector<std::shared_ptr<T>> SQLiteRepositoryBase<T>::getAll() const {
        std::vector<std::shared_ptr<T>> results;
        std::string sql = "SELECT * FROM " + _table_name;
        SQLite::Statement query = prepare(sql);

        while (query.executeStep())
            results.push_back(this->mapRowToEntity(query));

        return results;
    }

    template <typename T>
    unsigned SQLiteRepositoryBase<T>::getLastInsertId() const {
        return static_cast<unsigned>(_db->getLastInsertRowid());
    }

    template <typename T>
    bool SQLiteRepositoryBase<T>::exists(unsigned id) const {
        std::string sql = "SELECT COUNT(1) FROM " + _table_name + " WHERE id = ?";
        SQLite::Statement query = prepare(sql);
        query.bind(1, static_cast<int>(id));

        if (query.executeStep())
            return query.getColumn(0).getInt() > 0;

        return false;
    }

    template <typename T>
    bool SQLiteRepositoryBase<T>::removeAll() {
        std::string sql = "DELETE FROM " + _table_name;
        SQLite::Statement query = prepare(sql);
        return query.exec() > 0;
    }

    template <typename T>
    bool SQLiteRepositoryBase<T>::remove(unsigned id) {
        std::string sql = "DELETE FROM " + _table_name + " WHERE id = ?";
        SQLite::Statement query = prepare(sql);
        query.bind(1, static_cast<int>(id));
        return query.exec() > 0;
    }

    template <typename T>
    bool SQLiteRepositoryBase<T>::save(T& entity) {
        if (entity.getId() == 0)
            return insert(entity);
        else
            return update(entity);
    }

    template <typename T>
    size_t SQLiteRepositoryBase<T>::count() const {
        std::string sql = "SELECT COUNT(1) FROM " + _table_name;
        SQLite::Statement query = prepare(sql);

        if (query.executeStep())
            return static_cast<size_t>(query.getColumn(0).getInt());

        return 0;
    }
}

#endif // SQLITE_REPOSITORY_BASE_TPP
