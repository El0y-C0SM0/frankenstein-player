/**
 * @file UserRepository.hpp
 * @brief Repositorio de usuarios
 * @ingroup bd
 *
 * Repositorio para gerenciar operacoes de CRUD para a entidade User.
 *
 * @author Eloy Maciel
 * @date 2025-10-09
 */

#pragma once

#include <cstddef>
#include <vector>

#include <SQLiteCpp/SQLiteCpp.h>

#include "core/bd/SQLiteRepositoryBase.hpp"
// #include "core/entities/User.hpp"
#include "core/entities/EntitiesFWD.hpp" // TODO incluir usuario
#include "core/entities/User.hpp"

namespace core {

    /**
     * @brief Repositorio de usuarios
     * Repositorio para gerenciar operacoes de CRUD para a entidade User.
     */
    class UserRepository : public SQLiteRepositoryBase<User> {
    protected:
        /**
         * @brief Insere um novo usuario no repositório
         * @copydoc IRepository::insert
         * @param entity Usuario a ser inserido
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool insert(const User& entity) override;

        /**
         * @brief Atualiza um usuario existente no repositório
         * @copydoc IRepository::update
         * @param entity Usuario a ser atualizado
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool update(const User& entity) override;

    public:
        UserRepository(std::shared_ptr<SQLite::Database> db);
        ~UserRepository() override = default;

        /**
         * @brief Salva ou atualiza um usuario no repositório
         * @copydoc IRepository::save
         * @param entity Usuario a ser salvo ou atualizado
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool save(const User& entity) override;

        /**
         * @brief Remove um usuario do repositório pelo ID
         * @copydoc IRepository::remove
         * @param id ID do usuario a ser removido
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool remove(unsigned id) override;

        /**
         * @brief Obtém todos os usuarios do repositório
         * @copydoc IRepository::getAll
         * @return Vetor contendo todos os usuarios
         */
        std::vector<std::shared_ptr<User>> getAll() const override;

        /**
         * @brief Busca usuarios pelo nome de usuário
         * @param username Nome de usuário a ser buscado
         * @return Vetor contendo os usuarios que correspondem ao nome de usuário fornecido
         */
        std::vector<std::shared_ptr<User>> findByUsername(const std::string& username) const;

        /**
         * @brief Conta o número total de usuarios no repositório
         * @return Número total de usuarios
         */
        size_t count() const override;
    };

}
