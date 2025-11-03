/**
 * @file SQLiteRepositoryBase.hpp
 * @brief Implementação base para repositórios SQLite
 * @ingroup bd
 *
 * Fornece funcionalidades comuns para repositórios que usam SQLite,
 *
 * @author Eloy Maciel
 * @date 2025-10-09
 */

#pragma once

#include "core/interfaces/IRepository.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include <memory>

namespace core {

    /**
     * @brief Classe base para repositórios SQLite
     * @tparam T Tipo da entidade gerenciada
     *
     * @details
     * Fornece funcionalidades comuns para repositórios que usam SQLite,
     * mas não implementa a interface IRepository diretamente.
     */
    template <typename T>
    class SQLiteRepositoryBase : public IRepository<T> {
    protected:
        std::shared_ptr<SQLite::Database> _db;
        std::string _table_name;

        /**
         * @brief Prepara uma declaração SQL
         * @copydoc IRepository::prepare
         * @param sql Consulta SQL a ser preparada
         * @return Declaração preparada
         */
        SQLite::Statement prepare(const std::string& sql) const;

        /**
         * @brief Busca entidades por um campo específico
         * @param field Nome do campo a ser filtrado
         * @param value Valor do campo para filtro
         * @return Vetor contendo as entidades que correspondem ao critério
         */
        std::vector<std::shared_ptr<T>>
        findBy(const std::string& field,
               const std::string& value) const override;

        /**
         * @brief Obtém o nome da tabela
         * @return Nome da tabela
         */
        const std::string& getTableName() const;

    public:
        SQLiteRepositoryBase(
            std::shared_ptr<SQLite::Database> db,
            const std::string& table_name
        ) : _db(db), _table_name(table_name) {}

        virtual ~SQLiteRepositoryBase() = default;

        /**
         * @bief Busca uma entidade pelo ID
         * @copydoc IRepository::findById
         * @param id ID da entidade a ser buscada
         * @return Ponteiro compartilhado para a entidade encontrada, ou nullptr se não encontrada
         */
        std::shared_ptr<T> findById(unsigned id) const override;


        /**
         * @brief Obtém o ID da última inserção
         * @copydoc IRepository::getLastInsertId
         * @return ID da última inserção
         */
        unsigned getLastInsertId() const;

        /**
         * @brief Verifica se um ID existe na tabela
         * @copydoc IRepository::exists
         * @param id ID a ser verificado
         * @return true se existe, false caso contrário
         */
        bool exists(unsigned id) const override;

        /**
         * @brief Remove todas as entidades da tabela
         * @copydoc IRepository::removeAll
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool removeAll() override;

        /**
         * @brief Obtém o número total de entidades no repositório
         * @return Quantidade de entidades
         */
        virtual size_t count() const override;
    };

}  // namespace core
