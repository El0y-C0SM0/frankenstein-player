#pragma once

#include <string>

#include "core/services/ConfigManager.hpp"

class ConfigFixture : public core::ConfigManager {
public:
    ConfigFixture() : core::ConfigManager("../tests/config/test.config.json") {
        loadConfig();
    }
};
