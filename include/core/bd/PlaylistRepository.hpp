/**
 * @file PlaylistRepository.hpp
 * @brief Repositorio de playlists
 * @ingroup bd
 *
 * Repositorio para gerenciar operacoes de CRUD para a entidade Playlist.
 *
 * @author Eloy Maciel
 * @date 2025-10-09
 */

#pragma once

#include <vector>
#include <string>
#include <memory>

#include <SQLiteCpp/SQLiteCpp.h>

#include "core/bd/SQLiteRepositoryBase.hpp"
#include "core/entities/Playlist.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"

namespace core {

    /**
     * @brief Repositorio de playlists
     * Repositorio para gerenciar operacoes de CRUD para a entidade Playlist.
     */
    class PlaylistRepository : public SQLiteRepositoryBase<Playlist> {
    protected:
        /**
         * @brief Insere uma nova playlist no repositório
         * @copydoc IRepository::insert
         * @param entity Playlist a ser inserida
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool insert(const Playlist& entity) override;

        /**
         * @brief Atualiza uma playlist existente no repositório
         * @copydoc IRepository::update
         * @param entity Playlist a ser atualizada
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool update(const Playlist& entity) override;


        /**
         * @brief Adiciona uma música a uma playlist
         * @param playlist Playlist onde a música será adicionada
         * @param song Música a ser adicionada
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool addSongToPlaylist(const Playlist& playlist, const Song& song);

        /**
         * @brief Mapeia uma linha do resultado para uma playlist
         * @copydoc SQLiteRepositoryBase::mapRowToEntity
         * @param query Declaração SQL com o resultado da consulta
         * @return Ponteiro compartilhado para a playlist mapeada
         */
        virtual std::shared_ptr<Playlist>
        mapRowToEntity(SQLite::Statement& query) const override;
    public:
        PlaylistRepository(std::shared_ptr<SQLite::Database> db);
        ~PlaylistRepository() override = default;

        /**
         * @brief Busca playlists pelo título e usuário
         * @param title Título da playlist a ser buscada
         * @param user Usuário dono das playlists
         * @return Vetor contendo as playlists que correspondem ao título fornecido
         */
        std::vector<std::shared_ptr<Playlist>> findByTitleAndUser(const std::string& title, const User& user) const;

        /**
         * @brief Busca playlists pelo usuário
         * @param user Usuário dono das playlists
         * @return Vetor contendo as playlists do usuário fornecido
         */
        std::vector<std::shared_ptr<Playlist>> findByUser(const User& user) const;

        /**
         * @brief Obtém as músicas de uma playlist
         * @param playlist Playlist cujas músicas serão obtidas
         * @return Vetor contendo as músicas da playlist
         */
        std::vector<std::shared_ptr<Song>> getSongs(const Playlist& playlist) const;
    };
}
