/**
 * @file Library.hpp
 * @ingroup cli
 * @author Bruno Vieira
 * @brief Servico de acesso ao banco de dados
 * @version 0.1
 * @date 2025-10-14
 *
 */

#pragma once

#include <string>
#include <memory>
#include <SQLiteCpp/SQLiteCpp.h>

#include "core/entities/User.hpp"
#include "core/services/ConfigManager.hpp"
#include "core/services/Player.hpp"
#include "core/interfaces/IPlayable.hpp"
#include "core/bd/SongRepository.hpp"
#include "core/bd/ArtistRepository.hpp"
#include "core/bd/AlbumRepository.hpp"
#include "core/bd/PlaylistRepository.hpp"
#include "core/bd/RepositoryFactory.hpp"

namespace core
{
    class Library
    {
    private:
        std::shared_ptr<core::User> _user, _public_user;
        std::shared_ptr<core::SongRepository> _songRepo;
        std::shared_ptr<core::ArtistRepository> _artistRepo;
        std::shared_ptr<core::AlbumRepository> _albumRepo;
        std::shared_ptr<core::PlaylistRepository> _playlistRepo;

    public:
        [[deprecated]] Library(std::shared_ptr<core::User> user, std::shared_ptr<SQLite::Database> db);
        Library(const User &user, SQLite::Database &db);
        Library(ConfigManager &config);
        Library(ConfigManager &config, SQLite::Database &db);
        ~Library();

        /**
         * @brief Obtém o repositório de músicas
         *
         * @return ponteiro compartilhado para o repositório de músicas
         */
        [[deprecated("Usar RepositoryFactory")]] std::shared_ptr<core::SongRepository> getSongRepository() const;

        /**
         * @brief Obtém o repositório de artistas
         *
         * @return ponteiro compartilhado para o repositório de artistas
         */
        [[deprecated("Usar RepositoryFactory")]] std::shared_ptr<core::ArtistRepository> getArtistRepository() const;

        /**
         * @brief Obtém o repositório de álbuns
         *
         * @return ponteiro compartilhado para o repositório de álbuns
         */
        [[deprecated("Usar RepositoryFactory")]] std::shared_ptr<core::AlbumRepository> getAlbumRepository() const;

        /**
         * @brief Obtém o repositório de playlists
         *
         * @return ponteiro compartilhado para o repositório de playlists
         */
        [[deprecated("Usar RepositoryFactory")]] std::shared_ptr<core::PlaylistRepository> getPlaylistRepository() const;

        /**
         * @brief Adiciona um IPlayable à playlist atual.
         *
         * @param playlist Objeto IPlayable que representa a playlist onde o IPlayable será adicionado.
         * @param playabel Objeto IPlayable a ser adicionado à playlist.
         *
         * @return booleano indicando se a adição foi bem-sucedida
         */
        [[deprecated("Manipular diretamente na Playlist")]] bool addToPlaylist(const core::IPlayable &playlist, const core::IPlayable &playabel);

        /**
         * @brief Remove um IPlayable da playlist atual.
         *
         * @param playlist Objeto IPlayable que representa a playlist de onde o IPlayable será removido.
         * @param playabel Objeto IPlayable a ser removido da playlist.
         *
         * @return booleano indicando se a remoção foi bem-sucedida
         */
        [[deprecated("Manipular diretamente na Playlist")]] bool removeFromPlaylist(const core::IPlayable &playlist, const core::IPlayable &playabel);

        /**
         * @brief Procura pelas playlists do usuário.
         * @param query string de busca.
         *
         */
        [[nodiscard]] std::vector<std::shared_ptr<core::Song>> searchSong(const std::string &query) const;

        /**
         * @brief Procura pelos artistas do usuário.
         * @param query string de busca.
         *
         */
        [[nodiscard]] std::vector<std::shared_ptr<core::Artist>> searchArtist(const std::string &query) const;

        /**
         * @brief Procura pelos álbuns do usuário.
         * @param query string de busca.
         *
         */
        [[nodiscard]] std::vector<std::shared_ptr<core::Album>> searchAlbum(const std::string &query) const;

        /**
         * @brief Procura pelas playlists do usuário.
         * @param query string de busca.
         *
         */
        [[nodiscard]] std::vector<std::shared_ptr<core::Playlist>> searchPlaylist(const std::string &query) const;

        /**
         * @brief Registra uma música no banco de dados.
         * @param song Objeto Song a ser registrado.
         * @return true se o registro for bem-sucedido, false caso contrário.
         */
        bool persist(core::Song &song);
        /**
         * @brief Registra um artista no banco de dados.
         * @param artist Objeto Artist a ser registrado.
         * @return true se o registro for bem-sucedido, false caso contrário.
         */
        bool persist(core::Artist &artist);
        /**
         * @brief Registra um álbum no banco de dados.
         * @param album Objeto Album a ser registrado.
         * @return true se o registro for bem-sucedido, false caso contrário.
         */
        bool persist(core::Album &album);
        /**
         * @brief Registra uma playlist no banco de dados.
         * @param playlist Objeto Playlist a ser registrado.
         * @return true se o registro for bem-sucedido, false caso contrário.
         */
        bool persist(core::Playlist &playlist);

        /**
         * @brief Obtém o usuário associado à biblioteca.
         * @return ponteiro compartilhado para o usuário
         */
        std::shared_ptr<core::User> getUser() const;
    };
}
