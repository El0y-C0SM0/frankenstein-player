/**
 * @file ConfigManager.hpp
 * @brief Gerenciador de Configuracoes do Sistema
 *
 * O arquivo define a classe ConfigManager, responsavel por carregar,
 * salvar e gerenciar as configuracoes do sistema.
 *
 * @author Eloy Maciel
 * @date 2025-10-21
 */

#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace core {

    /**
     * @class ConfigManager
     * @brief Classe para gerenciar as configuracoes do sistema
     */
    class ConfigManager {
    private:
        std::string _config_file_path; /*!< @brief Caminho do arquivo de configuracoes */
        nlohmann::json _config_data;   /*!< @brief Dados de configuracao carregados */

    public:
        enum Enviroment {
            DEVELOPMENT,
            PRODUCTION,
            TESTING
        };

        /**
         * @brief Construtor da classe ConfigManager
         * @param config_file_path Caminho do arquivo de configuracoes
         */
        ConfigManager(const std::string& config_file_p1ath);
        ~ConfigManager();

        /**
         * @brief Carrega as configuracoes do arquivo
         * @return true se o carregamento foi bem-sucedido, false caso contrário
         */
        bool loadConfig();

        /**
         * @brief Salva as configuracoes no arquivo
         * @return true se a salvamento foi bem-sucedido, false caso contrário
         */
        bool saveConfig();

        /**
         * @brief Obtém o valor de uma configuracao
         * @param key Chave da configuracao
         * @return Valor da configuracao como string, ou string vazia se a chave nao existir
         */
        std::string getConfigValue(const std::string& key) const;

        /**
         * @brief Define o valor de uma configuracao
         * @param key Chave da configuracao
         * @param value Valor da configuracao
         */
        void setConfigValue(const std::string& key, const std::string& value);

        /**
         * @brief Obtém o caminho do banco de dados a partir das configuracoes
         * @return Caminho do banco de dados
         */
        std::string databasePath() const;

        /**
         * @brief Obtém o caminho para o schema do banco de dados a partir das configuracoes
         * @return Caminho do schema do banco de dados
         */
        std::string databaseSchemaPath() const;

        /**
         * @brief Obtém o diretório de músicas de usuário a partir das configuracoes
         * @return Diretório de músicas de usuário
         */
        std::string userMusicDirectory() const;

        /**
         * @brief Obtém o diretório de músicas do usuário público a partir das configuracoes
         * @return Diretório de músicas do usuário público
         */
        std::string publicMusicDirectory() const;

        /**
         * @brief Obtém o diretório de entrada de músicas a partir das configuracoes
         * @return Diretório de entrada de músicas
         */
        std::string inputPublicPath() const;

        /**
         * @brief Obtém o diretório de entrada de músicas de usuário a partir das configuracoes
         * @return Diretório de entrada de músicas de usuário
         */
        std::string inputUserPath() const;

        /**
         * @brief Obtém o ambiente de execução a partir das configuracoes
         * @return Ambiente de execução (DEVELOPMENT ou PRODUCTION)
         */
        Enviroment enviroment() const;
    };
}
