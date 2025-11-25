/**
 * @file Player.hpp
 * @brief Classe responsável pelo controle básico de reprodução de músicas
 *
 * Fornece funcionalidades essenciais para reprodução de músicas individuais
 * e playlists, com controles de play, pause, next, previous e volume.
 *
 * @author Joao Tavares
 * @date 2025-11-09
 */
#pragma once

#include "core/entities/Song.hpp"
#include "core/services/PlaybackQueue.hpp"
#include <atomic>
#include <memory>
#include <miniaudio.h>
#include <vector>

namespace core {

    enum class PlayerState { STOPPED,
                             PLAYING,
                             PAUSED };

    /**
     * @class Player
     * @brief Controlador de reprodução de áudio com funcionalidades básicas
     *
     * Gerencia uma playlist de músicas e fornece controles essenciais para
     * reprodução sequencial. Esta é uma versão inicial que será expandida
     * posteriormente para suportar a interface IPlayable.
     */
    class Player {
    private:
        std::vector<std::shared_ptr<core::PlaybackQueue>> _queue;
        std::shared_ptr<core::Song>
            _currentSong;
        int _currentQueueIndex;
        int _currentSongIndex;
        PlayerState _playerState;
        bool _isLooping;
        float _volume;
        float _previousVolume;

        // miniaudio
        ma_engine _audioEngine;
        ma_sound _currentSound;
        bool _audioInitialized;

        std::atomic<bool> _inCallback;
        ma_uint64 _songStartTime;
        bool _hasSongStartTime;

        /**
         * @brief Callback para quando uma música termina
         */
        static void sound_end_callback(void *pUserData, ma_sound *pSound);

        /**
         * @brief Carrega e prepara uma música para reprodução
         */
        bool loadCurrentSong();

        /**
         * @brief Limpa o som atual
         */
        void cleanupCurrentSound();

    public:
        /**
         * @brief Construtor da classe Player
         * Inicializa o player com estado parado e volume máximo.
         * Deve criar o vetor de _queue vazio.
         */
        Player();

        /**
         * @brief Construtor da classe Player
         * Inicializa o player com estado playing e volume máximo.
         * Deve criar o vetor de _queue com tracks no index 0
         */
        Player(const core::PlaybackQueue &tracks);

        /**
         * @brief Destrutor
         * Libera recursos
         */
        ~Player();

        std::shared_ptr<PlaybackQueue> getCurrentQueue() const;

        /**
         * @brief Adicionar uma Queue ao vector _queue
         *
         */
        void addPlaybackQueue(const core::PlaybackQueue &tracks);

        /**
         * @brief Reproduz música atual na Queue
         * @param PlayBackQueue a ser reproduzida
         */

        void play();

        /**
         * @brief Pausa a reprodução atual
         * Mantém a música atual carregada mas para a reprodução
         * Altera o estado bool do Player e o Enum
         */
        void pause();

        /**
         * @brief Logica para tocar proxima música
         * @param PlayBackQueue a ser reproduzida
         * Lógica para reprodução automatica, deve analisar se está em loop e
         * se for a ultima musica da queue, deve iniciar a mesma
         */
        void playNextSong();

        /**
         * @brief Retoma a reprodução pausada
         * Continua a reprodução da música atual de onde parou.
         * Altera o estado bool do Player e o Enum
         */
        void resume();

        /**
         * @brief Reinicia a música atual
         */
        void restart();

        /**
         * @brief Retrocede alguns segundos da musica atual
         * @param int seconds para retroceder
         */

        void rewind(unsigned int seconds);

        /**
         * @brief Avança e retroce na música relativamente
         * @param int seconds / negativo retroce, posição avança
         */
        void seek(int seconds);

        /**
         * @brief Avança alguns segundos da musica atual
         * @param int seconds para avançar
         */
        void fastForward(unsigned int seconds);

        /**
         * @brief Obtém tempo decorrido da música atual em segundos
         * @return Tempo decorrido em segundos
         */
        unsigned int getElapsedTime() const;

        /**
         * @brief Avança para a próxima música na playlist
         * Iterar sobre a QUEUE, caso o index atual nao tiver musica
         * alterar para a proxima posição
         * Se estiver na última música, pode parar ou voltar para o início
         * dependendo da implementação.
         */
        void next();

        /**
         * @brief Volta para a música anterior na playlist
         *
         * Depende a implementação, pois pode voltar de Queue ou apenas
         * nas músicas de uma queue.
         */
        void previous();

        /**
         * @brief Define a musica atual em looping
         */
        void setLooping();

        /**
         * @brief Remove o looping atual
         */

        void unsetLooping();

        /**
         * @brief Define o nível de volume do player
         * @param volume Novo nível de volume entre 0.0 (mudo) e 1.0 (máximo)
         */

        void setVolume(float volume);

        /**
         * @brief Obtém o nível de volume atual
         * @return Volume atual entre 0.0 e 1.0
         */
        float getVolume() const;

        /**
         * @brief Silencia o player
         *
         * Define o volume para 0.0 sem perder a configuração anterior.
         */
        void mute();

        /**
         * @brief Restaura o volume anterior
         *
         * Retorna o volume para o nível definido antes do mute.
         */
        void unmute();

        /**
         * @brief Obtém o estado atual do Player
         * @return Enum do tipo PlayerState
         */
        PlayerState stateOfPlayer() const;

        /**
         * @brief Verifica se o player está reproduzindo
         * @return true se está reproduzindo, false se pausado ou parado
         */
        bool isPlaying() const;

        /**
         * @brief Verifica se o player está pausado
         * @return true se está pausado, false se reproduzindo ou parado
         */
        bool isPaused() const;

        /**
         * @brief Verifica se o player está em loop
         * @return true se está em loop false caso contrário
         */

        bool isLooping() const;

        /**
         * @brief Obtém o progresso atual da reprodução
         * @return Progresso entre 0.0 (início) e 1.0 (fim) da música atual
         */
        float getProgress() const;

        /**
         * @brief Obtém o tamanho da playlist atual
         * Iterar sobra o index atual da _queue
         * @return Número de músicas na playlist
         */
        int getPlaylistSize() const;

        const std::shared_ptr<PlaybackQueue> getPlaybackQueue() const;

        /**
         * @brief Limpa toda a playlist
         * Remove todas as músicas do index da queue ou todas queue, depende
         * da implementação.
         */
        void clearPlaylist();

        /**
         * @brief Verifica se existe próxima música na playlist
         * Utiliza do método da Classe PlaybackQueue
         * @return true se há próxima música, false caso contrário
         */
        bool hasNext() const;

        /**
         * @brief Verifica se existe música anterior na playlist
         * Utiliza do método da Classe PlaybackQueue
         * @return true se há música anterior, false caso contrário
         */
        bool hasPrevious() const;

        ma_uint64 getEngineTime() const;
    };
} // namespace core
