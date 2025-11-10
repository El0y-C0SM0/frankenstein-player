/**
 * @file ArtistRepository.hpp
 * @brief Repositorio de artistas
 * @ingroup bd
 *
 * Repositorio para gerenciar operacoes de CRUD para a entidade Artist.
 *
 * @author Eloy Maciel
 * @date 2025-10-09
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include <SQLiteCpp/SQLiteCpp.h>

#include "core/bd/SQLiteRepositoryBase.hpp"
#include "core/entities/Album.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/EntitiesFWD.hpp"  // TODO incluir usuario
#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"

namespace core {

    /**
     * @brief Repositorio de artistas
     * Repositorio para gerenciar operacoes de CRUD para a entidade Artist.
     */
    class ArtistRepository : public SQLiteRepositoryBase<Artist> {
    protected:
        /**
         * @brief Insere um novo artista no repositório
         * @copydoc IRepository::insert
         * @param entity Artista a ser inserido
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool insert(const Artist& entity) override;

        /**
         * @brief Atualiza um artista existente no repositório
         * @copydoc IRepository::update
         * @param entity Artista a ser atualizado
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool update(const Artist& entity) override;

        /**
         * @brief Mapeia uma linha do resultado para um artista
         * @copydoc SQLiteRepositoryBase::mapRowToEntity
         * @param query Declaração SQL com o resultado da consulta
         * @return Ponteiro compartilhado para o artista mapeado
         */
        virtual std::shared_ptr<Artist>
        mapRowToEntity(SQLite::Statement& query) const override;

    public:
        ArtistRepository(std::shared_ptr<SQLite::Database> db);
        ~ArtistRepository() override = default;

        /**
         * @brief Salva ou atualiza um artista no repositório
         * @copydoc IRepository::save
         * @param entity Artista a ser salvo ou atualizado
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool save(Artist& entity) override;

        /**
         * @brief Remove um artista do repositório pelo ID
         * @copydoc IRepository::remove
         * @param id ID do artista a ser removido
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool remove(unsigned id) override;

        /**
         * @brief Busca artistas pelo nome e usuário
         * @param name Nome do artista a ser buscado
         * @param user Usuário cujos artistas serão buscados
         * @return Vetor contendo os artistas que correspondem ao nome fornecido
         */
        std::vector<std::shared_ptr<Artist>>
        findByNameAndUser(const std::string& name, const User& user) const;

        /**
         * @brief Busca artistas pelo nome
         * @param name Nome do artista a ser buscado
         * @return Vetor contendo os artistas que correspondem ao nome fornecido
         */
        std::vector<std::shared_ptr<Artist>>
        findByName(const std::string& name) const;

        /**
         * @brief Obtém os albuns de um artista
         * @param artist Artista cujos albuns serão obtidos
         * @return Vetor contendo os albuns do artista fornecido
         */
        std::vector<std::shared_ptr<Album>>
        getAlbums(const Artist& artist) const;

        /**
         * @brief Obtém as músicas de um artista
         * @param artist Artista cujas músicas serão obtidas
         * @return Vetor contendo as músicas do artista fornecido
         */
        std::vector<std::shared_ptr<Song>> getSongs(const Artist& artist) const;
    };

}  // namespace core
