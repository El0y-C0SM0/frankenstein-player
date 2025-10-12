/**
 * @file HistoryPlayback.hpp
 * @brief Entidade de histórico de reprodução
 *
 * O Arquivo define a entidade HistoryPlayback, que representa um registro de reprodução de uma música,
 *
 * @author Eloy Maciel
 * @date 2025-10-12
 */


#pragma once

#include <string>
#include <memory>
#include <ctime>

#include "core/entities/Entity.hpp"
#include "core/entities/Song.hpp"
// #include "core/entities/User.hpp"
#include "core/entities/EntitiesFWD.hpp" // TODO incluir user


namespace core {

    /**
     * @brief Entidade de histórico de reprodução
     *
     * Representa um registro de reprodução de uma música, incluindo informações como
     * o usuário, a música e timestamp da reprodução.
     */
    class HistoryPlayback : public Entity {
    private:
        std::shared_ptr<User> _user; // Associação com a entidade User
        std::shared_ptr<Song> _song; // Associação com a entidade Song
        std::time_t _played_at;

    public:
        HistoryPlayback();
        HistoryPlayback(std::shared_ptr<User> user, std::shared_ptr<Song> song, std::time_t played_at);
        HistoryPlayback(std::shared_ptr<User> user, std::shared_ptr<Song> song);
        ~HistoryPlayback() override = default;

        /**
         * @brief Obtém o usuário
         * @return Ponteiro compartilhado para o usuário
         */
        std::shared_ptr<const User> getUser() const;

        /**
         * @brief Define o usuário
         * @param user Referência para o usuário
         */
        void setUser(const User& user);

        /**
         * @brief Obtém a música associada ao histórico de reprodução
         * @return Ponteiro compartilhado para a música
         */
        std::shared_ptr<const Song> getSong() const;

        /**
         * @brief Define a música associada ao histórico de reprodução
         * @param song Referência para a música
         */
        void setSong(const Song& song);

        /**
         * @brief Obtém o timestamp da reprodução
         * @return Timestamp da reprodução
         */
        std::time_t getPlayedAt() const;

        /**
         * @brief Define o timestamp da reprodução
         * @param played_at Timestamp da reprodução
         */
        void setPlayedAt(std::time_t played_at);

        /**
         * @brief Obtém uma representação em string do histórico de reprodução
         * @return String representando o histórico de reprodução
         */
        std::string toString() const;
    };

}
