/**
 * @file UsersManager.hpp
 * @brief Declaração da classe UsersManager
 *
 * O arquivo UsersManager.hpp contém a declaração da classe UsersManager,
 * que é responsável por gerenciar os usuários do sistema.
 *
 * @author Julia Lima
 * @date 2025-10-13
 */

#pragma once

#include <memory>
#include <vector>

#include "core/entities/User.hpp"
#include "core/bd/UserRepository.hpp"
#include "core/services/ConfigManager.hpp"

namespace core {
    /**
     * @brief Classe responsável por gerenciar os usuários do sistema
     *
     */
    class UsersManager {
    private:
        std::vector<User> _users; /*!< Vetor que armazena os usuários do sistema */
        std::unique_ptr<UserRepository> _userRepository; /*!< Repositório para operações de banco de dados relacionadas a usuários */
        std::shared_ptr<ConfigManager> _configManager;   /*!< Gerenciador de configuração do sistema */

        #if defined(_WIN32)

            /**
            * @brief Obtém o SID do usuário atual no Windows
            * @return String contendo o SID do usuário atual
            */
            std::string getSIDCurrentUser() const;

            /**
            * @brief Obtém a lista de usuários do sistema operacional Windows
            * @param os_users Vetor para armazenar os usuários do sistema operacional
            */
            void getUsersWindows(std::vector<std::shared_ptr<User>> &os_users);
        #endif


        /**
         * @brief Verifica se o usuário público existe no sistema
         * @return true se o usuário público existir, false caso contrário
         */
        bool checkIfPublicUserExists();

        /**
         * @brief Verifica se existem usuários no sistema
         * @return true se existirem usuários, false caso contrário
         */
        bool checkIfUsersExists();

        /**
         * @brief Obtém a lista de usuários do sistema operacional
         * @return Vetor contendo os usuários do sistema operacional
         */
        std::vector<std::shared_ptr<User>> getUsersOS();

    public:
        /**
         * @brief Construtor da classe UsersManager
         *
         */
        UsersManager(ConfigManager &configManager);

        /**
         * @brief Destrutor da classe UsersManager
         */
        ~UsersManager();

        /**
         * @brief Remove um usuário do sistema
         * @param username Nome do usuário a ser removido
         */
        void removeUser(const userid &user_id);

        /**
         * @brief Remove um usuário do sistema
         * @param user Referência para o usuário a ser removido
         */
        void removeUser(User &user);

        /**
         * @brief Atualiza as informações dos usuários do sistema
         */
        void updateUsersList();

        /**
         * @brief Obtem o usuário atual do sistema
         */
        std::shared_ptr<User> getCurrentUser() const;

        /**
         * @brief Obtém um usuário pelo ID
         * @param id ID do usuário a ser obtido
         * @return Ponteiro compartilhado para o usuário encontrado ou nullptr se não encontrado
         */
        std::shared_ptr<User> getUserById(const unsigned id) const;

        /**
         * @brief Obtém um usuário pelo ID do OS
         * @param user_id ID do usuário a ser obtido
         * @return Ponteiro compartilhado para o usuário encontrado ou nullptr se não encontrado
         */
        std::shared_ptr<User> getUserByUserId(const userid &id) const;

        /**
         * @brief Obtém a lista de todos os usuários do sistema
         * @return Vetor contendo todos os usuários
         */
        std::vector<std::shared_ptr<User>> getAllUsers() const;

        /**
         * @brief Obtém o usuário público do sistema
         * @return Ponteiro compartilhado para o usuário público
         */
        std::shared_ptr<User> getPublicUser() const;
    };
}
