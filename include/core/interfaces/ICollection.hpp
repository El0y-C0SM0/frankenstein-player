/**
 * @file Collection.hpp
 * @brief Define métodos para coleções de IPlayable
 */
#pragma once
#include "core/interfaces/IPlayable.hpp"
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace core {
    class Song;

    class ICollection {
    public:
        virtual ~ICollection() = default;

        /**
         * @brief Obtém a lista de músicas da coleção
         * @return Vector com as músicas da coleção em ordem
         */
        virtual std::vector<std::shared_ptr<Song>> getSongs() const = 0;

        /**
		 * @brief Define a função de carregamento de músicas da coleção
		 * @param loader Função que retorna um vector de músicas
		 */
        virtual void setSongsLoader(
            const std::function<std::vector<std::shared_ptr<Song>>()>&
                loader) = 0;

        /**
         * @brief Obtém a quantidade de músicas na coleção
         * @return Número total de músicas
         */
        virtual std::size_t getSongsCount() const = 0;

        /**
		 * @brief Adiciona uma música à coleção
		 * @param song Música a ser adicionada
		 */
        virtual void addSong(const Song& song) = 0;

   //      /**
 		//  * @brief Troca a posição de uma música na coleção
   //  	 * @param id ID da música a ser movida
		 // * @param index Nova posição da música
		 // * @return true se a troca foi bem-sucedida, false caso contrário
		 // */
   //      virtual bool switchSong(unsigned id, unsigned index) = 0;

        /**
         * @brief Remove uma música da coleção pelo ID
         * @param id ID da música a ser removida
		 * @return true se a remoção foi bem-sucedida, false caso contrário
         */
        virtual bool removeSong(unsigned id) = 0;

        /**
         * @brief Encontra uma música na coleção pelo ID
		 * @param songId ID da música a ser encontrada
         */
        virtual std::shared_ptr<Song> findSongById(unsigned songId) = 0;

        /**
		 * @brief Encontra uma música na coleção pelo título
		 * @param title Título da música a ser encontrada
		 */
        virtual std::vector<std::shared_ptr<Song>>
        findSongByTitle(const std::string& title) = 0;

        /**
		 * @brief Calcula a duração total da coleção em segundos
		 * @return Duração total em segundos
		 */
        virtual unsigned calculateTotalDuration() = 0;

        // // ! remover
        // virtual std::string getFormattedDuration() = 0;

        // ! remover
        // virtual std::shared_ptr<Song>
        // getNextSong(Song& current) = 0;

        // // ! remover
        // virtual std::shared_ptr<Song>
        // getPreviousSong(Song& current) = 0;

        /**
         * @brief Obtém a música na posição especificada
         * @param index Índice da música a ser obtida
		 * @return Ponteiro compartilhado para a música na posição especificada
			* ou nullptr se índice inválido
         */
        virtual std::shared_ptr<Song> getSongAt(int index) = 0;

        /**
		 * @brief Sobrecarga do operador de indexação para acessar músicas
		 * @param index Índice da música a ser obtida
		 * @return Ponteiro compartilhado para a música na posição especificada
		*/
        virtual std::shared_ptr<Song> operator[](int index) = 0;
    };
};  // namespace core
