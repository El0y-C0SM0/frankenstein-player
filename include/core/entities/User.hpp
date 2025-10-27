/**
 * @file User.hpp
 * @author Bruno Vieira
 * @brief entidade  para usuários do sistema
 * @version 0.1
 * @date 2025-10-15
 *
 */

#pragma once

#include <string>
#include <sys/types.h>

#include "core/entities/Entity.hpp"
#include "core/entities/Playlist.hpp"

namespace core
{
    #ifdef _WIN32
        using userid = std::string; /*!< Tipo para representar o ID do usuário no OS */
    #else
        using userid = uid_t;       /*!< Tipo para representar o ID do usuário no OS */
    #endif

    /**
     * @brief Classe que representa um usuário do sistema
     *
     */
    class User : public Entity
    {
    private:
        std::string _username;  /*!< Nome do usuário */
        std::string _home_path; /*!< Caminho do diretório home do usuário */
        std::string _input_path; /*!< Caminho do diretório de entrada de músicas do usuário */
        userid _uid;            /*!< ID do usuário no OS */
        bool _is_current_user;  /*!< Indica se é o usuário atual do sistema */

    public:
        /**
         * @brief Constroi um novo usuário
         *
         * @param _username Nome do usuário
         */
        User(const std::string &_username);

        User(const std::string &_username,
             const std::string &_home_path,
             const std::string &_input_path,
             const userid &_uid);

        /**
         * @brief Destrutor de um usuário
         *
         */
        ~User();

        /**
         * @brief Obtém o nome do usuário
         *
         * @return String com o nome do usuário
         */
        std::string getUsername() const;

        /**
         * @brief Seta o nome do usuário
         *
         * @param username Novo nome do usuário
         */
        void setUsername(const std::string &username);

        /**
         * @brief Obtém o caminho do diretório home do usuário
         *
         * @return String com o caminho do diretório home
         */
        std::string getHomePath() const;

        /**
         * @brief Seta o caminho do diretório home do usuário
         *
         * @param home_path Novo caminho do diretório home
         */
        void setHomePath(const std::string &home_path);

        /**
         * @brief Obtém o caminho do diretório de entrada de músicas do usuário
         *
         * @return String com o caminho do diretório de entrada
         */
        std::string getInputPath() const;

        /**
         * @brief Seta o caminho do diretório de entrada de músicas do usuário
         *
         * @param input_path Novo caminho do diretório de entrada
         */
        void setInputPath(const std::string &input_path);

        /**
         * @brief Obtém o ID do usuário no OS
         *
         * @return ID do usuário
         */
        userid getUID() const;

        /**
         * @brief Seta o ID do usuário no OS
         *
         * @param uid Novo ID do usuário
         */
        void setUID(const userid &uid);

        /**
         * @brief Verifica se é o usuário atual do sistema

         * @return true se for o usuário atual, false caso contrário
         */
        bool isCurrentUser() const;

        /**
         * @brief Compara dois usuários
         * @param other Outro usuário para comparação
         * @return true se forem iguais, false caso contrário
         */
        bool operator==(const Entity &other) const override;

        /**
         * @brief Compara dois usuários para desigualdade
         * @param other Outro usuário para comparação
         * @return true se forem diferentes, false caso contrário
         */
        bool operator!=(const Entity &other) const override;
    };

}
