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
    public:
        PlaylistRepository(std::shared_ptr<SQLite::Database> db);
        ~PlaylistRepository() override = default;

        /**
         * @brief Salva ou atualiza uma playlist no repositório
         * @copydoc IRepository::save
         * @param entity Playlist a ser salva ou atualizada
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool save(const Playlist& entity) override;

        /**
         * @brief Remove uma playlist do repositório pelo ID
         * @copydoc IRepository::remove
         * @param id ID da playlist a ser removida
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool remove(unsigned id) override;

        /**
         * @brief Obtém todas as playlists do repositório
         * @copydoc IRepository::getAll
         * @return Vetor contendo todas as playlists
         */
        std::vector<std::shared_ptr<Playlist>> getAll() const override;

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
         * @brief Busca playlists pelo ID
         * @copydoc IRepository::findById
         * @param id ID da playlist a ser buscada
         * @return Ponteiro para a playlist encontrada, ou nullptr se não encontrada
         */
        std::shared_ptr<Playlist> findById(unsigned id) const override;

        /**
         * @brief Adiciona uma música a uma playlist
         * @param playlist Playlist onde a música será adicionada
         * @param song Música a ser adicionada
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool addSongToPlaylist(const Playlist& playlist, const Song& song);

        /**
         * @brief Remove uma música de uma playlist
         * @param playlist Playlist de onde a música será removida
         * @param song Música a ser removida
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool removeSongFromPlaylist(const Playlist& playlist, const Song& song);

        /**
         * @brief Obtém as músicas de uma playlist
         * @param playlist Playlist cujas músicas serão obtidas
         * @return Vetor contendo as músicas da playlist
         */
        std::vector<std::shared_ptr<Song>> getSongs(const Playlist& playlist) const;
    };
}
