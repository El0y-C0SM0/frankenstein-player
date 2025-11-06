// src/main.cpp
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>

int main() {
    std::string logo =
        "  __                 _                  _       _\n"
        " / _|               | |                | |     (_)\n"
        "| |_ _ __ __ _ _ __ | | _____ _ __  ___| |_ ___ _ _ __\n"
        "|  _| '__/ _` | '_ \\| |/ / _ \\ '_ \\/ __| __/ _ \\ | '_ \\\n"
        "| | | | | (_| | | | |   <  __/ | | \\__ \\ ||  __/ | | | |\n"
        "|_| |_|  \\__,_|_| |_|_|\\_\\___|_| |_|___/\\__\\___|_|_| |_|\n"
        "\n";
    std::cout << logo << std::endl;
    std::cout << "Frankenstein Media Player iniciado!" << std::endl;
    std::cout << "Path local" << boost::filesystem::current_path() << std::endl;

    const std::string config_path = "../config/frankenstein.config.json";
    std::cout << "Arquivo de configurações: " << config_path << std::endl;

    return 0;
}
