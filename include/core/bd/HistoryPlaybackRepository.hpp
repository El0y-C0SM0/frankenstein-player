/**
 * @file HistoryPlaybackRepository.hpp
 * @brief Repositorio de historico de reproducoes
 * @ingroup bd
 *
 * Repositorio para gerenciar operacoes de CRUD para a entidade HistoryPlayback.
 *
 * @author Eloy Maciel
 * @date 2025-10-09
 */

#pragma once

#include <memory>
#include <vector>

#include <SQLiteCpp/SQLiteCpp.h>

#include "core/bd/SQLiteRepositoryBase.hpp"
#include "core/entities/HistoryPlayback.hpp"
#include "core/entities/User.hpp"

namespace core {

    /**
     * @brief Repositorio de historico de reproducoes
     * Repositorio para gerenciar operacoes de CRUD para a entidade
     * HistoryPlayback.
     */
    class HistoryPlaybackRepository
        : public SQLiteRepositoryBase<HistoryPlayback> {
    protected:
        /**
         * @brief Insere um novo historico de reproducao no repositório
         * @copydoc IRepository::insert
         * @param entity Historico de reproducao a ser inserido
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool insert(const HistoryPlayback& entity) override;

        /**
         * @brief Atualiza um historico de reproducao existente no repositório
         * @copydoc IRepository::update
         * @param entity Historico de reproducao a ser atualizado
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool update(const HistoryPlayback& entity) override;

        /**
         * @brief Mapeia uma linha do resultado para um historico de reproducao
         * @copydoc SQLiteRepositoryBase::mapRowToEntity
         * @param query Declaração SQL com o resultado da consulta
         * @return Ponteiro compartilhado para o historico de reproducao mapeado
         */
        std::shared_ptr<HistoryPlayback>
        mapRowToEntity(SQLite::Statement& query) const override;
    public:
        HistoryPlaybackRepository();
        HistoryPlaybackRepository(std::shared_ptr<SQLite::Database> db);
        ~HistoryPlaybackRepository() override = default;

        /**
         * @brief Salva ou atualiza um historico de reproducao no repositório
         * @copydoc IRepository::save
         * @param entity Historico de reproducao a ser salvo ou atualizado
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool save(HistoryPlayback& entity) override;

        /**
         * @brief Busca historicos de reproducoes pelo usuário
         * @param user Usuário cujos historicos de reproducoes serão buscados
         * @return Vetor contendo os historicos de reproducoes do usuário
         * fornecido
         */
        std::vector<std::shared_ptr<HistoryPlayback>>
        findByUser(const User& user) const;

        /**
         * @brief Insere um historico de reproducao no repositório
         * @param entity Historico de reproducao a ser inserido
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool insertHistoryPlayback(const HistoryPlayback& entity);

        /**
         * @brief Insere multiplos historicos de reproducoes no repositório
         * @param entities Vetor contendo os historicos de reproducoes a serem
         * inseridos
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool
        insertMultipleHistoryPlaybacks(const std::vector<HistoryPlayback>& entities);

        /**
         * @brief Conta o número de reproduções de uma música
         * @param user Usuário cujas reproduções serão contadas
         * @return Número de reproduções da música pelo usuário
         */
        unsigned countPlaybacksBySongAndUser(const Song& song) const;

        /**
         * @brief Conta o número de reproduções de uma música
         * @param user Usuário cujas reproduções serão contadas
         * @return Número de reproduções da música pelo usuário
         */
        unsigned countPlaybacksBySongAndUser(const Song& song, const User& user) const;
    };

}  // namespace core
