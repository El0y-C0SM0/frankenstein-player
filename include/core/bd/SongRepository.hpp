/**
 * @file SongRepository.hpp
 * @brief Repositorio de musicas
 * @ingroup bd
 *
 * Repositorio para gerenciar operacoes de CRUD para a entidade Song.
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
#include "core/entities/Song.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/Album.hpp"
// #include "core/entities/User.hpp"
// #include "core/entities/Playlist.hpp"
#include "core/entities/EntitiesFWD.hpp" // TODO incluir usuario e playlist

namespace core {

    /**
     * @brief Repositorio de musicas
     * Repositorio para gerenciar operacoes de CRUD para a entidade Song.
     */
    class SongRepository : public SQLiteRepositoryBase<Song> {
    protected:
        /**
         * @brief Insere uma nova musica no repositório
         * @copydoc IRepository::insert
         * @param entity Musica a ser inserida
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool insert(const Song& entity) override;

        /**
         * @brief Atualiza uma musica existente no repositório
         * @copydoc IRepository::update
         * @param entity Musica a ser atualizada
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool update(const Song& entity) override;

    public:
        SongRepository(std::shared_ptr<SQLite::Database> db);
        ~SongRepository() override = default;

        /**
         * @brief Salva ou atualiza uma musica no repositório
         * @copydoc IRepository::save
         * @param entity Musica a ser salva ou atualizada
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool save(const Song& entity) override;

        /**
         * @brief Remove uma musica do repositório pelo ID
         * @copydoc IRepository::remove
         * @param id ID da musica a ser removida
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool remove(unsigned id) override;

        /**
         * @brief Obtém todas as musicas do repositório
         * @return Vetor contendo todas as musicas
         */
        std::vector<std::shared_ptr<Song>> getAll() const override;

        /**
         * @brief Busca musicas pelo título e usuário
         * @param title Título da musica a ser buscada
         * @param user Usuário dono das musicas
         * @return Vetor contendo as musicas que correspondem ao título fornecido
         */
        std::vector<std::shared_ptr<Song>> findByTitleAndUser(const std::string& title, const User& user) const;

        /**
         * @brief Busca musicas pelo usuário
         * @param user Usuário dono das musicas a serem buscadas
         * @return Vetor contendo as musicas que pertencem ao usuário fornecido
         */
         std::vector<std::shared_ptr<Song>> findByUser(const User& user) const;

        /**
         * @brief Busca musicas pelo artista
         * @param artist Artista da musica a ser buscada
         * @return Vetor contendo as musicas que correspondem ao artista fornecido
         */
        std::vector<std::shared_ptr<Song>> findByArtist(const Artist& artist) const;

         /**
         * @brief Busca musicas pelo album
         * @param album Album da musica a ser buscada
         * @return Vetor contendo as musicas que correspondem ao album fornecido
         */
        std::vector<std::shared_ptr<Song>> findByAlbum(const Album& album) const;

        /**
         * @brief Busca uma musica pelo ID
         * @param id ID da musica a ser buscada
         * @return Ponteiro para a musica encontrada, ou nullptr se não encontrada
         */
        std::shared_ptr<Song> findById(unsigned id) const override;
    };

}
