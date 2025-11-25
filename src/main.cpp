// src/main.cpp
#include <chrono>
#include <iostream>
#include <ostream>
#include <string>
#include <thread>

#include <boost/filesystem.hpp>

#include "core/entities/Song.hpp"
#include "core/services/ConfigManager.hpp"
#include "core/services/PlaybackQueue.hpp"
#include "core/services/Player.hpp"

int main(int argc, char *argv[]) {
    std::string logo =
        "\033[32m"
        "  __                 _                  _       _\n"
        " / _|               | |                | |     (_)\n"
        "| |_ _ __ __ _ _ __ | | _____ _ __  ___| |_ ___ _ _ __\n"
        "|  _| '__/ _` | '_ \\| |/ / _ \\ '_ \\/ __| __/ _ \\ | '_ \\\n"
        "| | | | | (_| | | | |   <  __/ | | \\__ \\ ||  __/ | | | |\n"
        "|_| |_|  \\__,_|_| |_|_|\\_\\___|_| |_|___/\\__\\___|_|_| |_|\n"
        "\033[0m\n";
    std::cout << "\033[32m=======================Esta Vivo!=======================\033[0m" << std::endl;
    std::cout << logo << std::endl;
    std::cout << "\033[32m========================================================\033[0m" << std::endl;

    std::string config_path = "../config/frankenstein.config.json";
    if (argc > 1)
        config_path = argv[1];
    std::cout << "Arquivo de configurações: " << config_path << std::endl;

    core::ConfigManager configManager(config_path);
    // configManager.loadConfig();

    // Exemplo de execução
    core::Player player;
    // player.addPlaybackQueue(const core::PlaybackQueue &track);
    player.play();
    // stateOfPlayer controla o callback.
    while (player.stateOfPlayer() != core::PlayerState::STOPPED) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
