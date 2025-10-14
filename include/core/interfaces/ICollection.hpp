/**
 * @file Collection.hpp
 * @brief Define métodos para coleções de IPlayable
 *
 * Essa intergface tem métodos relacionados à coleções de IPlayables.
 *
 * @author Pedro Gabriel
 * @date 2025-10-13
 */

#pragma once
#include "IPlayable.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace core {

    class ICollection {

        /**
         * @brief Define a função para carregar as músicas da coleção
         * @param loader Função que retorna um vetor de músicas
         */
        void setSongsLoader(
            const std::function<std::vector<std::shared_ptr<IPlayable>>()> &loader);

        /**
         * @brief Adiciona uma música ao final da playlist
         * @param song Música a ser adicionada
         */
        void addSong(const IPlayable &song);

        /**
         * @brief Troca uma música de posição na playlist
         * @param id ID da música a ser adicionada
         * @param index index para o qual a música será transferida
         * @return true caso a mudança seja bem sucedida e false caso o id ou o index seja inválido na playlist
         * 
         * @note As outras músicas devem ser realocadas 1 posição para alguma direção, caso seja necessário
         */
        bool switchSong(unsigned id, unsigned index);

        /**
         * @brief Remove uma música da playlist
         * @param id ID da música a ser removida
         * @return true se a música foi removida, false se não encontrada
         */
        bool removeSong(unsigned id);

        /**
         * @brief Busca uma música na Playlist por ID
         * @param songId ID da música a buscar
         * @return Ponteiro para a música encontrada ou nullptr se não encontrada
         */
        const IPlayable *findSongById(unsigned songId) const;

        /**
         * @brief Busca uma música na playlist por título
         * @param title Título da música a buscar
         * @return Ponteiro para a música encontrada ou nullptr se não encontrada
         */
        const IPlayable *findSongByTitle(const std::string &title) const;

        /**
         * @brief Calcula a duração total da Playlist
         * @return Duração total em segundos
         */
        int calculateTotalDuration() const;

        /**
         * @brief Obtém a duração total formatada
         * @return String com duração no formato "HH:MM:SS" ou "MM:SS"
         */
        std::string getFormattedDuration() const;
    
        /**
         * @brief Obtém a próxima música na sequência da playlist
         * @param current Música atual
         * @return Próxima música ou a primeira se for a última
         */
        IPlayable getNextSong(const IPlayable &current) const;

        /**
         * @brief Obtém a música anterior na sequência do álbum
         * @param current Música atual
         * @return Música anterior ou a última se for a primeira
         */
        IPlayable getPreviousSong(const IPlayable &current) const;

        /**
         * @brief Obtém uma música por sua posição na playlist
         * @param index Índice da música (0-based)
         * @return Música na posição especificada
         * @throws std::out_of_range se índice inválido
         */
        IPlayable getSongAt(int index) const;
    };

}

#pragma once
