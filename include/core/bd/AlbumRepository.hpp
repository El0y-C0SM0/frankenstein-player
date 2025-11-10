/**
 * @file AlbumRepository.hpp
 * @brief Repositorio de albuns
 * @ingroup bd
 *
 * Repositorio para gerenciar operacoes de CRUD para a entidade Album.
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
#include "core/entities/EntitiesFWD.hpp"  // TODO incluir usuario
#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"

namespace core {

    /**
     * @brief Repositorio de albuns
     * Repositorio para gerenciar operacoes de CRUD para a entidade Album.
     */
    class AlbumRepository : public SQLiteRepositoryBase<Album> {
    protected:
        /**
         * @brief Insere um novo album no repositório
         * @copydoc IRepository::insert
         * @param entity Album a ser inserido
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool insert(const Album& entity) override;

        /**
         * @brief Atualiza um album existente no repositório
         * @copydoc IRepository::update
         * @param entity Album a ser atualizado
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool update(const Album& entity) override;

        /**
         * @brief Mapeia uma linha do resultado para um artista
         * @copydoc SQLiteRepositoryBase::mapRowToEntity
         * @param query Declaração SQL com o resultado da consulta
         * @return Ponteiro compartilhado para o artista mapeado
         */
        virtual std::shared_ptr<Album>
        mapRowToEntity(SQLite::Statement& query) const override;

    public:
        AlbumRepository(std::shared_ptr<SQLite::Database> db);
        ~AlbumRepository() override = default;

        /**
         * @brief Salva ou atualiza um album no repositório
         * @copydoc IRepository::save
         * @param entity Album a ser salvo ou atualizado
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool save(Album& entity) override;

        /**
         * @brief Remove um album do repositório pelo ID
         * @copydoc IRepository::remove
         * @param id ID do album a ser removido
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool remove(unsigned id) override;

        /**
         * @brief Busca albuns pelo título e usuário
         * @param title Título do album a ser buscado
         * @param user Usuário cujos albuns serão buscados
         * @return Vetor contendo os albuns que correspondem ao título fornecido
         */
        std::vector<std::shared_ptr<Album>>
        findByTitleAndUser(const std::string& title, const User& user) const;

        /**
         * @brief Busca albuns pelo usuário
         * @param user Usuário cujos albuns serão buscados
         * @return Vetor contendo os albuns que pertencem ao usuário fornecido
         */
        std::vector<std::shared_ptr<Album>> findByUser(const User& user) const;

        /**
         * @brief Busca albuns pelo artista e usuário
         * @param artist Nome do artista do album a ser buscado
         * @return Vetor contendo os albuns que correspondem ao artista
         * fornecido
         */
        std::vector<std::shared_ptr<Album>>
        findByArtist(const std::string& artist) const;

        /**
         * @brief Obtém um album pelo ID e usuário
         * @copydoc IRepository::findById
         * @param id ID do album a ser obtido
         * @return Ponteiro compartilhado para o album encontrado, ou nullptr se
         * não encontrado
         */
        std::shared_ptr<Album> findById(unsigned id) const override;

        /**
         * @brief Obtém as músicas de um álbum
         * @param album Álbum cujas músicas serão obtidas
         * @return Vetor contendo as músicas do álbum
         */
        std::vector<std::shared_ptr<Song>> getSongs(const Album& album) const;

        /**
         * @brief Obtém os artistas colaboradores de um álbum
         * @param album Álbum cujos artistas colaboradores serão obtidos
         * @return Vetor contendo os artistas colaboradores do álbum
         */
        std::vector<std::shared_ptr<Artist>>
        getFeaturingArtists(const Album& album) const;

        /**
         * @brief Obtém o artista principal de um álbum
         * @param album Álbum cujo artista principal será obtido
         * @return Ponteiro compartilhado para o artista principal do álbum
         */
        std::shared_ptr<Artist> getArtist(const Album& album) const;

        /**
         * @brief Conta o número total de albuns no repositório
         * @return Número total de albuns
         */
        virtual size_t count() const override;
    };
}  // namespace core
