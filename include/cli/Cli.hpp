/**
 * @file Cli.hpp
 * @ingroup services
 * @author Bruno Vieira
 * @brief Servico de comunicação com o usuario
 * @version 0.1
 * @date 2025-10-14
 *
 */

#pragma once

#include <string>
#include <memory>

#include "core/entities/EntitiesFWD.hpp" // TODO incluir user
#include "../core/entities/Player.hpp"
namespace cli
{

    class Cli
    {
    private:
        std::shared_ptr<core::User> _user;
        std::shared_ptr<Player> player;

        /**
         * @brief toca um IPlayable ou um IPlayableObject
         *
         * @param playabel caminho para a música.
         */
        void play(std::string &playabel);

        /**
         * @brief pausar / tocar música
         *
         * Pausa a reprodução da música se ela estiver tocando, retoma a reprodução da
         * música se ela estiver pausada.
         *
         */
        void togglePlayPause();

        /**
         * @brief Avança para a próxima música na playlist
         *
         * chama next() de palyer.
         */
        void next();

        /**
         * @brief Volta para a música anterior na playlist
         *
         * chama previous() de player.
         */
        void previous();

        /**
         * @brief Define o nível de volume do player
         * @param volume Novo nível de volume entre 0.0 (mudo) e 1.0 (máximo)
         */
        void setVolume(std::string &volume);

        /**
         * @brief Obtém o nível de volume atual
         *
         * chama getVolume de player
         *
         * @return string do valor atual do volume
         */
        std::string getVolume() const;

        /**
         * @brief Silencia o player
         *
         * muta ou desmuta o volume
         */
        void toggleMute();

        /**
         * @brief Obtém a música atualmente em reprodução
         * @return IPlayableObject atual inválida se não houver reprodução
         */
        std::string getCurrentSong() const;

        /**
         * @brief Obtém o progresso atual da reprodução
         * @return String com o valor de progresso da música.
         */
        std::string getProgress() const;

        /**
         * @brief Limpa toda a playlist
         *
         * chama clearPlaylist() de player.
         */
        void clearPlaylist();

        /**
         * @brief Obtém uma cópia da playlist atual
         * @return Vector com todas as músicas da playlist
         */
        std::vector<std::string> getPlaylist() const;

    public:
        /** /**
         * @brief Construtor de um novo objeto Cli
         *
         * @param user Usuario que ira utilizar o cli
         * @param player Player de musica que sera controlado pelo cli
         */
        Cli(const core::User &user, const Player &player);

        /**
         * @brief Destrutor de um objeto Cli
         *
         */
        ~Cli();

        /**
         * @brief Recebe e realiza um comando específico
         *
         * @param command comando e seus parâmetros
         * @return booleano representando se o comando foi realizado com sucesso
         */
        bool doComand(std::string command);
    };
}

#pragma once
