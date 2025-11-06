/**
 * @file PlaybackQueue.hpp
 * @brief Servico de fila de reproducões
 *
 * Servico para gerenciar a fila de músicas a serem reproduzidas.
 *
 * @ingroup services
 * @author Eloy Maciel
 * @date 2025-10-11
 */

#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <random>
#include <vector>

#include "core/bd/HistoryPlaybackRepository.hpp"
#include "core/entities/Playlist.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"
#include "core/interfaces/IPlayable.hpp"

#define MAX_SIZE_DEFAULT 200

namespace core {

    /**
     * @brief Servico de fila de reproducões
     *
     * Servico para gerenciar a fila de músicas a serem reproduzidas.
     */
    class PlaybackQueue {
    private:
        std::vector<std::shared_ptr<Song>>
            _queue;                            /*!< @brief Fila de músicas */
        std::vector<size_t> _indices_aleatory; /*!< @brief Índices embaralhados
                                                  para reprodução aleatória */
        size_t _current;  /*!< @brief Índice da música atual na fila */
        size_t _max_size; /*!< @brief Tamanho máximo da fila */
        bool _aleatory;   /*!< @brief Indica se a reprodução é aleatória */
        bool _loop;      /*!< @brief Indica se a reprodução está em loop */
        std::shared_ptr<HistoryPlaybackRepository>
            _history_repo; /*!< @brief Repositório de histórico de reprodução */
        std::shared_ptr<User> _current_user; /*!< @brief Usuário atual */

        /**
         * @brief Adiciona registro de reprodução ao histórico
         * @param song Música que foi reproduzida
         */
        void addToHistory(const Song& song);

        size_t getCurrentIndex() const;

    public:
        PlaybackQueue();
        PlaybackQueue(std::shared_ptr<User> current_user,
                      const IPlayable& playable,
                      std::shared_ptr<HistoryPlaybackRepository> history_repo,
                      size_t max_size = MAX_SIZE_DEFAULT);
        PlaybackQueue(std::shared_ptr<User> current_user,
                      std::shared_ptr<HistoryPlaybackRepository> history_repo,
                      size_t max_size = MAX_SIZE_DEFAULT);

        ~PlaybackQueue();

        /**
         * @brief Adiciona uma música, album, artista ou playlist à fila
         * @param song Música a ser adicionada
         */
        void add(const IPlayable& tracks);

        /**
         * @brief Adiciona outra fila de reprodução à fila atual
         * @param other_queue Fila de reprodução a ser adicionada
         */
        void add(const PlaybackQueue& other_queue);

        /**
         * @brief Adiciona uma música, album, artista ou playlist à fila
         * @param song Música a ser adicionada
         */
        void operator+=(const IPlayable& tracks);

        /**
         * @brief Adiciona outra fila de reprodução à fila atual
         * @param other_queue Fila de reprodução a ser adicionada
         */
        void operator+=(const PlaybackQueue& other_queue);

        /**
         * @brief Remove uma música da fila pelo índice
         * @param index Índice da música a ser removida
         * @return true se a música foi removida, false caso contrário
         */
        bool remove(size_t index);

        /**
         * @brief Encontra o proximo índice na fila da música pesquisada
         * @param song Música a ser encontrada
         * @return Índice da música ou -1 se não encontrada
         */
        int findNextIndex(const Song& song) const;

        /**
         * @brief Encontra o índice da música atual na fila
         * @return Índice da música atual ou -1 se a fila estiver vazia
         */
        int findCurrentIndex() const;

        /**
         * @brief Encontra o índice da música anterior na fila
         * @return Índice da música anterior ou -1 se a fila estiver vazia
         */
        int findPreviousIndex() const;

        /**
         * @brief Obtém a música no índice especificado
         * @param index Índice da música a ser obtida
         * @return Ponteiro para a música ou nullptr se o índice for inválido
         */
        std::shared_ptr<Song> at(size_t index) const;

        /**
         * @brief Obtém a próxima música na fila
         * @return Próxima música ou nullptr se a fila estiver vazia
         */
        std::shared_ptr<Song> getNextSong();

        /**
         * @brief Obtém a música atual na fila
         * @return Música atual ou nullptr se a fila estiver vazia
         */
        std::shared_ptr<const Song> getCurrentSong() const;

        /**
         * @brief Obtém a música anterior na fila
         * @return Música anterior ou nullptr se a fila estiver vazia
         */
        std::shared_ptr<const Song> getPreviousSong() const;

        /**
         * @brief Avança para a próxima música na fila
         * @return Próxima música ou nullptr se a fila estiver vazia
         */
        std::shared_ptr<const Song> next();

        /**
         * @brief Avança para a próxima música na fila
         * @return Próxima música ou nullptr se a fila estiver vazia
         */
        std::shared_ptr<const Song> operator++();

        /**
         * @brief Retorna para a música anterior na fila
         * @return Música anterior ou nullptr se a fila estiver vazia
         */
        std::shared_ptr<const Song> previous();

        /**
         * @brief Retorna para a música anterior na fila
         * @return Música anterior ou nullptr se a fila estiver vazia
         */
        std::shared_ptr<const Song> operator--();

        /**
         * @brief Obtém uma visualização de parte da fila a partir
         * da música atual, incluindo atual.
         * Tamanho será menor que o passado se o fim ou início da fila for
         * alcançado
         * @param before Número de músicas antes da atual
         * @param after Número de músicas após a atual
         * @return Vetor de músicas na visualização
         */
        std::vector<std::shared_ptr<const Song>>
        getQueueView(size_t before, size_t after) const;

        /**
         * @brief Obtém uma visualização de parte da fila, tamanho será menor
         * que o passado se o fim da fila for alcançado
         * @param start Índice inicial da visualização
         * @param count Número de músicas na visualização
         * @return Vetor de músicas na visualização
         */
        std::vector<std::shared_ptr<const Song>>
        getQueueSegment(size_t start, size_t count) const;

        /**
         * @brief Limpa a fila de reprodução
         */
        void clear();

        /**
         * @brief Obtém o tamanho da fila
         * @return Número de músicas na fila
         */
        size_t size() const;

        /**
         * @brief Verifica se a fila está vazia
         * @return true se a fila estiver vazia, false caso contrário
         */
        bool empty() const;

        /**
         * @brief Habilita ou desabilita a reprodução aleatória
         * @param aleatory true para habilitar, false para desabilitar
         */
        void setAleatory(bool aleatory);

        /**
         * @brief Alterna o estado da reprodução aleatória
         * @return Novo estado da reprodução aleatória
         */
        bool toggleAleatory();

        /**
         * @brief Verifica se a reprodução aleatória está habilitada
         * @return true se estiver habilitada, false caso contrário
         */
        bool isAleatory() const;

        /**
         * @brief Habilita ou desabilita o loop da fila
         * @param loop true para habilitar, false para desabilitar
         */
        void setLoop(bool loop);

        /**
         * @brief Alterna o estado do loop da fila
         * @return Novo estado do loop da fila
         */
        bool toggleLoop();

        /**
         * @brief Verifica se o loop da fila está habilitado
         * @return true se estiver habilitado, false caso contrário
         */
        bool isLoop() const;

        /**
         * @brief Embaralha a fila de reprodução
         */
        void shuffle();

        /**
         * @brief Move uma música de um índice para outro na fila
         * @param from Índice da música a ser movida
         * @param to Índice para onde a música será movida
         * @return true se a música foi movida, false caso contrário
         */
        bool move(size_t from, size_t to);

        /**
         * @brief Obtém a representação em string da fila de reprodução
         * @return String representando a fila
         */
        std::string toString() const;

        std::string toStringDetailed() const;
    };

}  // namespace core
