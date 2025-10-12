/**
 * @file RepositoryFactory.hpp
 * @brief Contrato para fabrica de repositórios
 *
 * O arquivo define a interface RepositoryFactory, que especifica métodos para criar repositórios
 *
 * @author Eloy Maciel
 * @date 2025-10-12
 */

#pragma once

#include <future>
#include <memory>
#include <SQLiteCpp/SQLiteCpp.h>

#include "core/bd/ArtistRepository.hpp"
#include "core/bd/AlbumRepository.hpp"
#include "core/bd/SongRepository.hpp"
#include "core/bd/PlaylistRepository.hpp"
#include "core/bd/HistoryPlaybackRepository.hpp"


namespace core {

    /**
     * @brief Fabrica de repositórios
     *
     * A interface RepositoryFactory especifica métodos para criar repositórios
     * para diferentes entidades, como Artist, Album, Song, Playlist e HistoryPlayback.
     */
    class RepositoryFactory {
    private:
        std::shared_ptr<SQLite::Database> _db; /*!< @brief Conexão com o banco de dados SQLite */

    public:
        /**
         * @brief Construtor da fábrica de repositórios
         * @param db Ponteiro compartilhado para a conexão com o banco de dados SQLite
         */
        RepositoryFactory(std::shared_ptr<SQLite::Database> db);

        virtual ~RepositoryFactory();

        /**
         * @brief Cria um repositório de artistas
         * @return Ponteiro para o repositório de artistas
         */
        virtual std::unique_ptr<ArtistRepository> createArtistRepository();

        /**
         * @brief Cria um repositório de álbuns
         * @return Ponteiro para o repositório de álbuns
         */
        virtual std::unique_ptr<AlbumRepository> createAlbumRepository();

        /**
         * @brief Cria um repositório de músicas
         * @return Ponteiro para o repositório de músicas
         */
        virtual std::unique_ptr<SongRepository> createSongRepository();

        /**
         * @brief Cria um repositório de playlists
         * @return Ponteiro para o repositório de playlists
         */
        virtual std::unique_ptr<PlaylistRepository> createPlaylistRepository();

        /**
         * @brief Cria um repositório de histórico de reproduções
         * @return Ponteiro para o repositório de histórico de reproduções
         */
        virtual std::unique_ptr<HistoryPlaybackRepository> createHistoryPlaybackRepository();
    };

}
