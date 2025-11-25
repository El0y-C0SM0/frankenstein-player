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
#include <sstream>
#include <iostream>
#include <optional>
#include <SQLiteCpp/SQLiteCpp.h>

#include "core/entities/User.hpp"
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
        std::shared_ptr<core::User> _user;
        std::shared_ptr<core::SongRepository> _songRepo;
        std::shared_ptr<core::ArtistRepository> _artistRepo;
        std::shared_ptr<core::AlbumRepository> _albumRepo;
        std::shared_ptr<core::PlaylistRepository> _playlistRepo;

    public:
        Library(std::shared_ptr<core::User> user, std::shared_ptr<SQLite::Database> db);
        ~Library();

        /**
         * @brief Obtém o repositório de músicas
         *
         * @return ponteiro compartilhado para o repositório de músicas
         */
        std::shared_ptr<core::SongRepository> getSongRepository() const;

        /**
         * @brief Obtém o repositório de artistas
         *
         * @return ponteiro compartilhado para o repositório de artistas
         */
        std::shared_ptr<core::ArtistRepository> getArtistRepository() const;

        /**
         * @brief Obtém o repositório de álbuns
         *
         * @return ponteiro compartilhado para o repositório de álbuns
         */
        std::shared_ptr<core::AlbumRepository> getAlbumRepository() const;

        /**
         * @brief Obtém o repositório de playlists
         *
         * @return ponteiro compartilhado para o repositório de playlists
         */
        std::shared_ptr<core::PlaylistRepository> getPlaylistRepository() const;

        /**
         * @brief Adiciona um IPlayable à playlist atual.
         *
         * @param playlist Objeto IPlayable que representa a playlist onde o IPlayable será adicionado.
         * @param playabel Objeto IPlayable a ser adicionado à playlist.
         *
         * @return booleano indicando se a adição foi bem-sucedida
         */
        bool addToPlaylist(const core::IPlayable &playlist, const core::IPlayable &playabel);

        /**
         * @brief Remove um IPlayable da playlist atual.
         *
         * @param playlist Objeto IPlayable que representa a playlist de onde o IPlayable será removido.
         * @param playabel Objeto IPlayable a ser removido da playlist.
         *
         * @return booleano indicando se a remoção foi bem-sucedida
         */
        bool removeFromPlaylist(const core::IPlayable &playlist, const core::IPlayable &playabel);

        /**
         * @brief Procura pelas playlists do usuário.
         * @param query string de busca.
         *
         */
        std::vector<std::shared_ptr<core::Song>> searchSong(const std::string &query) const;

        /**
         * @brief Procura pelos artistas do usuário.
         * @param query string de busca.
         *
         */
         std::vector<std::shared_ptr<core::Artist>> searchArtist(const std::string &query) const;

        /**
         * @brief Procura pelos álbuns do usuário.
         * @param query string de busca.
         *
         */
         std::vector<std::shared_ptr<core::Album>> searchAlbum(const std::string &query) const;

        /**
         * @brief Procura pelas playlists do usuário.
         * @param query string de busca.
         *
         */
         std::vector<std::shared_ptr<core::Playlist>> searchPlaylist(const std::string &query) const;
    };
}