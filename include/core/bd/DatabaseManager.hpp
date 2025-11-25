/**
 * @file DatabaseManager.hpp
 * @brief Gerenciador de banco de dados
 *
 * O arquivo define a classe DatabaseManager, responsável por gerenciar a conexão com o banco de dados SQLite.
 *
 * @author Eloy Maciel
 * @date 2025-10-12
 */

#pragma once


#include <memory>
#include <SQLiteCpp/SQLiteCpp.h>
#include <string>

namespace core {

    /**
     * @brief Gerenciador de banco de dados
     *
     * A classe DatabaseManager é responsável por gerenciar a conexão com o banco de dados SQLite.
     */
    class DatabaseManager {
    private:
        std::shared_ptr<SQLite::Database> _db; /*!< @brief Conexão com o banco de dados SQLite */
        std::string _db_path; /*!< @brief Caminho para o arquivo do banco de dados SQLite */
        std::string _schema_path; /*!< @brief Caminho para o arquivo de esquema do banco de dados SQLite */


    public:
        /**
         * @brief Construtor default
         */
         DatabaseManager() = default;

        /**
         * @brief Construtor do gerenciador de banco de dados
         */
        DatabaseManager(std::string db_path, std::string schema_path);
        virtual ~DatabaseManager();

        /**
         * @brief Obtém a conexão com o banco de dados
         * @return Ponteiro compartilhado para a conexão com o banco de dados SQLite
         */
        std::shared_ptr<SQLite::Database> getDatabase();

        /**
         * @brief Obtém o caminho do arquivo do banco de dados SQLite
         * @return Caminho do arquivo do banco de dados SQLite
         */
        std::string getDatabasePath() const;
    };

}
