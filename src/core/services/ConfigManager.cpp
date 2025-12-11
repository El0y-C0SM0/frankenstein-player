/**
 * @file ConfigManager.cpp
 * @brief Implementação do gerenciador de configurações
 *
 * @ingroup services
 * @author Eloy Maciel
 * @date 2025-11-05
 */

#include "core/services/ConfigManager.hpp"

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>


namespace core {
    ConfigManager::ConfigManager(const std::string& config_file_path)
        : _config_file_path(config_file_path) {}

    ConfigManager::~ConfigManager() {}

    void ConfigManager::loadConfig() {
        std::filesystem::path config_file(_config_file_path);

        if (!std::filesystem::exists(config_file)) {
            throw std::runtime_error("Config file not found");
        }

        std::ifstream file(_config_file_path);

        if (!file.is_open())
            throw std::runtime_error("Failed to open config file");

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string config_content = buffer.str();

        try {
            _config_data = nlohmann::json::parse(config_content);
        } catch (const nlohmann::json::parse_error& e) {
            throw std::runtime_error("Failed to parse config file, invalid JSON format");
        }
    }

    // bool ConfigManager::saveConfig() {
    //     return true;
    // }

    std::string ConfigManager::getConfigValue(const std::string& key) const {
        if (_config_data.contains(key)) {
            return _config_data[key].get<std::string>();
        }
        return "";
    }

    // void ConfigManager::setConfigValue(const std::string& key,
    //                                    const std::string& value) {
    //     _config_data[key] = value;
    // }

    std::string ConfigManager::databasePath() const {
        if (!_config_data.contains("database")) {

            throw std::runtime_error("Database configuration not found");
        }

        return _config_data["database"].value("filename", "frankenstein.db");
    }

    std::string ConfigManager::databaseSchemaPath() const {
        if (!_config_data.contains("database")) {
            throw std::runtime_error("Database configuration not found");
        } else if (!_config_data["database"].contains("schema_path") ||
                   _config_data["database"]["schema_path"].get<std::string>().empty()) {
            throw std::runtime_error("Database schema path not found");
        }

        return _config_data["database"]["schema_path"].get<std::string>();
    }

    void ConfigManager::validateConfigPaths() const {
        if (!_config_data.contains("paths")) {
            throw std::runtime_error("Paths configuration not found");
        } else if (!_config_data["paths"].contains("user_home") ||
                   _config_data["paths"]["user_home"].get<std::string>().empty()) {
            throw std::runtime_error("User music directory not found");
        } else if (!_config_data["paths"].contains("public_user") ||
            _config_data["paths"]["public_user"].get<std::string>().empty()) {
            throw std::runtime_error("Public music directory not found");
        } else if (!_config_data["paths"].contains("input_public") ||
            _config_data["paths"]["input_public"].get<std::string>().empty()) {
            throw std::runtime_error("Input public path not found");
        } else if (!_config_data["paths"].contains("input_user") ||
            _config_data["paths"]["input_user"].get<std::string>().empty()) {
            throw std::runtime_error("Input user path not found");
        }
    }

    std::string ConfigManager::userMusicDirectory() const {
        validateConfigPaths();

        return _config_data["paths"]["user_home"].get<std::string>();
    }

    std::string ConfigManager::publicMusicDirectory() const {
        validateConfigPaths();

        return _config_data["paths"]["public_user"].get<std::string>();
    }

    std::string ConfigManager::inputPublicPath() const {
        validateConfigPaths();

        return _config_data["paths"]["input_public"].get<std::string>();
    }

    std::string ConfigManager::inputUserPath() const {
        validateConfigPaths();

        return _config_data["paths"]["input_user"].get<std::string>();
    }

    ConfigManager::Enviroment ConfigManager::enviroment() const {
        std::string env = _config_data.value("enviroment", "production");

        if (env == "production")
            return Enviroment::PRODUCTION;
        else if (env == "testing")
            return Enviroment::TESTING;
        else
            return Enviroment::DEVELOPMENT;
    }

    std::string ConfigManager::toString() const {
        std::string result = "ConfigManager:\n";
        result += " - Config file path: " + _config_file_path + "\n";
        result += _config_data.dump(4) + "\n";
        return result;
    }
}
