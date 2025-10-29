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
    public:

        /**
         * @brief Obtém a lista de músicas da coleção
         * @return Vector com as músicas da coleção em ordem
         */
        virtual std::vector<const std::shared_ptr<IPlayable>> getSongs()  = 0;

        /**
         * @brief Define a função para carregar as músicas da coleção
         * @param loader Função que retorna um vetor de músicas
         */
        virtual void setSongsLoader(
            const std::function<std::vector<std::shared_ptr<IPlayable>>()> &loader) = 0;

        /**
         * @brief Adiciona uma música ao final da coleção
         * @param song Música a ser adicionada
         */
        virtual void addSong(const IPlayable &song) = 0;

        /**
         * @brief Troca uma música de posição na coleção
         * @param id ID da música a ser adicionada
         * @param index index para o qual a música será transferida
         * @return true caso a mudança seja bem sucedida e false caso o id ou o index seja inválido na coleção
         *
         * @note As outras músicas devem ser realocadas 1 posição para alguma direção, caso seja necessário
         */
        virtual bool switchSong(unsigned id, unsigned index) = 0;

        /**
         * @brief Remove uma música da coleção
         * @param id ID da música a ser removida
         * @return true se a música foi removida, false se não encontrada
         */
        virtual bool removeSong(unsigned id) = 0;

        /**
         * @brief Busca uma música na coleção por ID
         * @param songId ID da música a buscar
         * @return Ponteiro para a música encontrada ou nullptr se não encontrada
         */
        virtual const IPlayable *findSongById(unsigned songId) = 0;

        /**
         * @brief Busca uma música na coleção por título
         * @param title Título da música a buscar
         * @return Ponteiro para a música encontrada ou nullptr se não encontrada
         */
        virtual const IPlayable *findSongByTitle(const std::string &title) = 0;

        /**
         * @brief Calcula a duração total da coleção
         * @return Duração total em segundos
         */
        virtual int calculateTotalDuration() = 0;

        /**
         * @brief Obtém a duração total formatada
         * @return String com duração no formato "HH:MM:SS" ou "MM:SS"
         */
        virtual std::string getFormattedDuration() = 0;

        /**
         * @brief Obtém a próxima música na sequência da coleção
         * @param current Música atual
         * @return Próxima música ou a primeira se for a última
         */
        virtual IPlayable getNextSong(const IPlayable &current) = 0;

        /**
         * @brief Obtém a música anterior na sequência do álbum
         * @param current Música atual
         * @return Música anterior ou a última se for a primeira
         */
        virtual IPlayable getPreviousSong(const IPlayable &current) = 0;

        /**
         * @brief Obtém uma música por sua posição na coleção
         * @param index Índice da música (0-based)
         * @return Música na posição especificada
         * @throws std::out_of_range se índice inválido
         */
        virtual IPlayable getSongAt(int index) = 0;
    };

}

#pragma once
