/**
 * @file DatabaseManager.hpp
 * @brief Implementação do gerenciador de banco de dados
 *
 * @ingroup bd
 * @author Eloy Maciel
 * @date 2025-11-04
 */

#include "core/bd/DatabaseManager.hpp"

#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>


namespace core {
    DatabaseManager::DatabaseManager(std::string db_path,
                                    std::string schema_path)
        : _db_path(db_path), _schema_path(schema_path) {
        _db = std::make_shared<SQLite::Database>(
            _db_path,
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        SQLite::Statement query(*_db, "PRAGMA foreign_keys = ON;");
        query.exec();

        boost::filesystem::path schema_file(_schema_path);
        if (boost::filesystem::exists(schema_file)) {
            boost::iostreams::mapped_file_source mapped_file;
            mapped_file.open(_schema_path);
            std::string schema_sql(mapped_file.data(), mapped_file.size());

            _db->exec(schema_sql);

            mapped_file.close();
        } else {
            throw std::runtime_error("Schema file not found");
        }
    }

    DatabaseManager::~DatabaseManager() {}

    std::shared_ptr<SQLite::Database> DatabaseManager::getDatabase() {
        return _db;
    }

    std::string DatabaseManager::getDatabasePath() const {
        return _db_path;
    }
}  // namespace core
