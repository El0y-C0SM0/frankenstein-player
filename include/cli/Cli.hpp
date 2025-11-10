/**
 * @file Cli.hpp
 * @ingroup cli
 * @author Bruno Vieira
 * @brief Servico de comunicação com o usuario
 * @version 0.1
 * @date 2025-10-14
 *
 */

#pragma once

#include <string>
#include <memory>
#include <sstream>
#include <iostream>

// #include "core/entities/EntitiesFWD.hpp" // TODO incluir user
#include "core/services/ConfigManager.hpp"
#include "core/entities/User.hpp"
#include "core/services/Player.hpp"

namespace cli
{

  class Cli
  {
  private:
    std::shared_ptr<core::User> _user;
    std::shared_ptr<core::Player> _player;

    /**
     * @brief toca um IPlayable ou um IPlayableObject
     *
     * @param playabel caminho para a música.
     */
    void play(Core::IPlayable &playabel);

    /**
     * @brief recomeça a musica atual
     *
     * chama restart() de palyer.
     *
     */
    void restart();

    /**
     * @brief Retrocede alguns segundos da musica atual
     *
     *  chamar rewind() de player.
     *
     *  @param int seconds para retroceder
     */
    void rewind(unsigned int seconds);

    /**
     * @brief Avança alguns segundos da musica atual
     *
     * chamar fastForward() de player.
     *
     * @param int seconds para avançar
     */
    void forward(unsigned int seconds);

    /**
     * @brief pausar / tocar música
     *
     * Pausa a reprodução da música se ela estiver tocando, retoma a reprodução da
     * música se ela estiver pausada.
     *
     */
    void toggleResumePause();

    /**
     * @brief Avança para a próxima música no queue
     *
     * chama next() de palyer.
     */
    void next();

    /**
     * @brief Volta para a música anterior no queue
     *
     * chama previous() de player.
     */
    void previous();

    /**
     * @brief Define o nível de volume do player
     * @param volume Novo nível de volume entre 0.0 (mudo) e 1.0 (máximo)
     */
    void setVolume(const std::string &volume);

    /**
     * @brief aumenta/diminui o nível de volume do player por um valor específico
     * @param volume up para aumentar, down para diminuir o volume
     */
    void setVolumeUpDown(const std::string &volume);

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
     * 
     * @param command "mute" para mutar, "unmute" para desmutar, "toggle_mute" para alternar entre os dois estados
     */
    void toggleMute(const std::string &command);

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
     * @brief Limpa toda a fila
     *
     * chama clearPlaylist() de player.
     */
    void clearQueue();

    /**
     * @brief Adiciona um IPlayable à playlist atual.
     * @param playabel Objeto IPlayable a ser adicionado à playlist.
     */
    boll addToPlaylist(Core::IPlayable &playabel);

    /**
     * @brief Remove um IPlayable da playlist atual.
     * @param playabel Objeto IPlayable a ser removido da playlist.
     */
    boll removeFromPlaylist(Core::IPlayable &playabel);

    /**
     * @brief Remove um IPlayable da playlist atual pelo índice.
     * @param idx Índice do objeto IPlayable a ser removido da playlist.
     */
    boll removeFromPlaylist(unsigned idx);

    /**
     * @brief Coloca a playlist atual no modo aleatório.
     *
     */
    void shuffle();

    /**
     * @brief adicionar a musica atual a playlist de músicas curtidas
     *
     */
    void like();

    /**
     * @brief Retira a musica atual da playlist de músicas curtidas
     *
     */
    void deslike();

    /**
     * @brief Mostra as informações atuais do player como: volume, progresso da musica atual, prxima musica da lista, etc.
     *
     */
    void showStatus() const;

    /**
     * @brief Mostra as informações da fila de musicas.
     *
     */
    void showQueue() const;

    /**
     * @brief Adiociona uma música na fila.
     * @param playabel Objeto IPlayable a ser adicionado a fil.
     */
    void addToQueue(Core::IPlayable &playabel);

    /**
     * @brief Remove uma música na fila.
     * @param idx Index do objeto IPlayable a ser removido na fila.
     */
    void removeFromQueue(unsigned idx);

    /**
     * @brief ativa/desativa o looping na musica atual.
     *
     * se estiver ativado, desativa. Se estiver desativado, ativa. 
     * 
     * @param command "on" para ativar, "off" para desativar
     */
    void loop(const std::string &command);

    /**
     * @brief Procura pelas playlists do usuário.
     * @param query string de busca.
     *
     */
    void searchMusic(const std::string &query) const;

    /**
     * @brief Procura pelos artistas do usuário.
     * @param query string de busca.
     *
     */
    void searchArtist(const std::string &query) const;

    /**
     * @brief Procura pelos álbuns do usuário.
     * @param query string de busca.
     *
     */
    void searchAlbum(const std::string &query) const;

    /**
     * @brief Procura pelas playlists do usuário.
     * @param query string de busca.
     *
     */
    void searchPlaylist(const std::string &query) const;

    /**
     * @brief Procura pelos podcasts do usuário.
     * @param query string de busca.
     *
     */
    void searchPodcast(const std::string &query) const;

    void showHelp() const;

  public:
    /**
     * @brief Construtor de um novo objeto Cli
     *
     * @param config_manager Gerenciador de configurações para inicializar o Cli
     */
    Cli(core::ConfigManager &config_manager);

    /**
     * @brief Destrutor de um objeto Cli
     *
     */
    ~Cli();

    /**
     * @brief Recebe, trata e realiza uma string com o comando completo a ser realizado.
     *
     * @param command comando digitado e seus parâmetros
     * @return booleano representando se o comando foi realizado com sucesso
     */
    bool doCommand(const std::string &command);
  };
}

#pragma once
