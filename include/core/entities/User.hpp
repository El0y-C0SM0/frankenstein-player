/**
 * @file User.hpp
 * @author Bruno Vieira
 * @brief entidade  para usuários do sistema
 * @version 0.1
 * @date 2025-10-15
 *
 */

#pragma once

#include <climits>
#include <ctime>
#include <string>
#include <sys/types.h>

#include "core/entities/Entity.hpp"

namespace core {
#ifdef _WIN32
    using userid =
        std::string; /*!< Tipo para representar o ID do usuário no OS */
#else
    using userid = uid_t; /*!< Tipo para representar o ID do usuário no OS */
#endif

    /**
     * @brief Classe que representa um usuário do sistema
     *
     */
    class User : public Entity {
    private:
        std::string _username;   /*!< Nome do usuário */
        std::string _home_path;  /*!< Caminho do diretório home do usuário */
        std::string _input_path; /*!< Caminho do diretório de entrada de músicas
                                    do usuário */
        userid _uid;             /*!< ID do usuário no OS */
        bool _is_current_user;   /*!< Indica se é o usuário atual do sistema */

    public:
        User();

        /**
         * @brief Constroi um novo usuário
         *
         * @param _username Nome do usuário
         */
        User(const std::string &username);

        /**
         * @brief Constroi um novo usuário
         *
         * @param username Nome do usuário
         * @param home_path Caminho do diretório home do usuário
         * @param input_path Caminho do diretório de entrada de músicas do
         * usuário
         * @param uid ID do usuário no OS
         */
        User(const std::string &username,
             const std::string &home_path,
             const std::string &input_path,
             const userid &uid);

        /**
         * @brief Constroi um novo usuário
         *
         * @param id ID do usuário na aplicação
         * @param username Nome do usuário
         * @param home_path Caminho do diretório home do usuário
         * @param input_path Caminho do diretório de entrada de músicas do
         * usuário
         * @param uid ID do usuário no OS
         */
        User(unsigned id,
             const std::string &username,
             const std::string &home_path,
             const std::string &input_path,
             const userid &uid);

        /**
		 * @brief Construtor de cópia
		 *
		 * @param other Usuário a ser copiado
		 */
        User(const User &other) = default;

        /**
		 * @brief Operador de atribuição
		 *
		 * @param other Usuário a ser atribuído
		 * @return Referência para o usuário atual
		 */
        User &operator=(const User &other) = default;

        /**
         * @brief Destrutor de um usuário
         *
         */
        ~User() = default;

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

        void setIsCurrentUser(bool is_current_user);

        // Métodos de Entity

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

        /**
         * @brief Compara qual usuário é menor
         * @param other Outro usuário para comparação
         * @return true se o usuário atual for menor que o outro, false caso
         * contrário
         */
        bool operator<(const Entity &other) const override;

        /**
         * @brief Compara qual usuário é maior
         * @param other Outro usuário para comparação
         * @return true se o usuário atual for maior que o outro, false caso
         * contrário
         */
        bool operator>(const Entity &other) const override;

        /**
         * @brief Compara qual usuário é maior ou igual
         * @param other Outro usuário para comparação
         * @return true se o usuário atual for maior ou igual que o outro, false
         * caso contrário
         */
        bool operator>=(const Entity &other) const override;

        /**
         * @brief Compara qual usuário é menor ou igual
         * @param other Outro usuário para comparação
         * @return true se o usuário atual for menor ou igual que o outro, false
         * caso contrário
         */
        bool operator<=(const Entity &other) const override;
    };

} // namespace core
