/***
 * @file Manager
 * @brief Essa classe é responsável por gerenciar as músicas do dispositivo
 * 
 * @date 2025-10-14
 * @author Pedro Gabriel
 *  
 ***/

#pragma once

#include "core/interfaces/IPlayable.hpp"
#include "core/bd/SongRepository.hpp"
#include "core/bd/ArtistRepository.hpp"
#include "core/bd/AlbumRepository.hpp"

#include <boost/filesystem.hpp>
#include <tag.h>
#include <fileref.h>

namespace fs = boost::filesystem;

namespace core {

    class Manager {
    private:

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
        Song* readMetadata(TagLib::FileRef file);

        /**
         * @brief Verifica ou cria o diretório antes de salvar uma música
         * 
         * Verifica se um diretório existe e caso não exista cria o diretório
         */
        void verifyDir(std::string path);

    public:

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

        /**
         * @brief Define artista para músicas deconhecidas 
         * 
         * Atribui artista para uma música cuja o artista não foi identificado
         * Verifica se s música não tem nenhum artista definido
         * 
         */
        void defineArtist(int songId, int artistId);

    };

}

#pragma once