/***
 * @file FilesManager
 * @brief Essa classe é responsável por gerenciar as músicas do dispositivo
 *
 * @date 2025-10-14
 * @author Pedro Gabriel
 *
 ***/

#pragma once

#include "core/entities/User.hpp"
#include "core/interfaces/IPlayable.hpp"
#include "core/bd/SongRepository.hpp"
#include "core/bd/ArtistRepository.hpp"
#include "core/bd/AlbumRepository.hpp"
#include "core/services/ConfigManager.hpp"
#include "core/services/UsersManager.hpp"

#include <exception>
#include <boost/filesystem.hpp>
#include <memory>
#include <sstream>
#ifdef _WIN32
    #include <taglib/tag.h>
    #include <taglib/fileref.h>
#elif __linux__
    #include <taglib/tag.h>
    #include <taglib/fileref.h>
#endif
#include <string>

namespace fs = boost::filesystem;

namespace core {

    class FilesManager {
    private:
        ConfigManager& _config;
        std::shared_ptr<SongRepository> _songRepo;
        std::shared_ptr<ArtistRepository> _artistRepo;
        std::shared_ptr<AlbumRepository> _albumRepo;
        core::UsersManager _usersManager;


        /**
         * @brief Move um arquivo de um diretório para o outro
         *
         * @param filePath path para o arquivo original
         * @param newFilePath diretório para o qual o arquivo será transferido - incluí o novo nome do arquivo
         *
         */
        void move(std::string filePath, std::string newFilePath);

        /**
         * @brief Lê os metadados do arquivo
         *
         * Lê todos os metadados do arquivo e trata todas as informações segundo as regras
         * de negócio (nomeação de diretórios com base em nome de artistas)
         *
         * @return Retorna uma instância do objeto com os dados de título, artista e path tratados
         *
         */
        std::shared_ptr<Song> readMetadata(TagLib::FileRef file, User &user);

        /**
         * @brief Verifica ou cria o diretório antes de salvar uma música
         *
         * Verifica se um diretório existe e caso não exista cria o diretório
         */
        void verifyDir(std::string path);

        /**
         * @brief Retira os espaços do inicio e do fim
         *
         */
        std::string cleanString(const std::string& str);

    public:

        /**
         * @brief Construtor da classe FilesManager
         * @param config Instância da configuração do sistema
         * @param songRepo Repositório de músicas
         * @param artistRepo Repositório de artistas
         * @param albumRepo Repositório de álbuns
         */
        FilesManager(ConfigManager& config,
                std::shared_ptr<SongRepository> songRepo,
                std::shared_ptr<ArtistRepository> artistRepo,
                std::shared_ptr<AlbumRepository> albumRepo);

        FilesManager(ConfigManager &config);

        FilesManager(ConfigManager &config, SQLite::Database& db);

        /***
         * @brief Atualiza toda a organização das músicas com base no diretório temporário
         *
         * Responsável pela lógica de organizar as músicas do diretório temporário
         *
         */
        void update();

        /**
         * @brief Verifica se há arquivos a serem carregados no diretório temporário
         *
         * @return true se não houver nenhuma atualização a ser feita e false caso exista alguma música no diretório temporário
         */
        bool isUpdated();
    };

}

#pragma once
