/**
 * @file Playlist.hpp
 * @brief Definição de uma coleção de músicas escolhidas pelo usuário.
 *
 * Essa classe armazena um conjunto de músicas à gosto do usuário.
 *
 * @author Pedro Gabriel
 * @date 2025-10-13
 */

#pragma once

#include <memory.h>
#include <memory>
#include <string>
#include <vector>
#include <cassert>

#include "core/entities/Entity.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"
#include "core/interfaces/ICollection.hpp"
#include "core/interfaces/IPlayable.hpp"

namespace core {
    /**
     * @class Playlist
     * @brief Representa uma playlist de músicas
     *
     * Representa a entidade Playlist.
     *
     */
    class Playlist : public Entity,
                     public ICollection,
                     public IPlayable {
    private:
        std::string _title;
        std::shared_ptr<User> _user;
        mutable std::vector<std::shared_ptr<Song>> _songs;
        std::function<std::vector<std::shared_ptr<Song>>()> _loader;
        mutable bool _songsLoaded = false;

        std::vector<std::shared_ptr<Song>> loadSongs() const;

    public:
        /**
         * @brief Construtor padrão da classe Playlist
         *
         */
        Playlist();

        /**
         * @brief Construtor de classe Playlist
         *
         * @param id Identificador único
         * @param title Título da Playlist
         *
         */
        Playlist(const unsigned id, const std::string title);

        /**
         * @brief Construtor de classe Playlist
         *
         * @param id Identificador único
         * @param title Título da Playlist
         * @param user Usuário dono da Playlist
         *
         */
        Playlist(const unsigned id, const std::string title, const User &user);

        /**
         * @brief Construtor de cópia da classe Playlist
         * @param other Outro objeto Playlist para copiar
         */
        Playlist(const Playlist &other) = default;

        /**
         * @brief Operador de atribuição da classe Playlist
         * @param other Outro objeto Playlist para atribuir
         * @return Referência ao objeto Playlist atribuído
         */
        Playlist &operator=(const Playlist &other) = default;

        /**
         * @brief Destrutor padrão da classe Playlist
         * @note Libera a memória dos vetores alocados
         */
        ~Playlist() = default;

        /**
         * @brief Getter de título
         */
        std::string getTitle() const;

        /**
         * @brief Setter de título
         */
        void setTitle(const std::string &title);

        /**
         * @brief Compara duas Playlist
         * @param other Playlist a ser comparada
         * @return true se as entidades forem iguais, false caso contrário
         */
        virtual bool operator==(const Entity& other) const override;

        /**
         * @brief Compara duas Playlists para desigualdade
         * @param other Playlist a ser comparada
         * @return true se as entidades forem diferentes, false caso contrário
         */
        virtual bool operator!=(const Entity& other) const override;

        /**
         * @brief Compara qual Playlist é menor
         * @param other Playlist a ser comparada
         * @return true se a entidade atual for menor que a outra, false caso
         * contrário
         */
        virtual bool operator<(const Entity& other) const override;

        /**
         * @brief Compara qual Playlist é maior
         * @param other Playlist a ser comparada
         * @return true se a entidade atual for maior que a outra, false caso
         * contrário
         */
        virtual bool operator>(const Entity& other) const override;

        /**
         * @brief Compara qual Playlist é maior ou igual
         * @param other Playlist a ser comparada
         * @return true se a entidade atual for maior ou igual que a outra, false
         * caso contrário
         */
        virtual bool operator>=(const Entity& other) const override;

        /**
         * @brief Compara qual Playlist é menor ou igual
         * @param other Playlist a ser comparada
         * @return true se a entidade atual for menor ou igual que a outra, false
         * caso contrário
         */
        virtual bool operator<=(const Entity& other) const override;

        // ICollection

        /**
         * @brief Getter do vector de músicas de ICollection
         *
         */
        virtual std::vector<std::shared_ptr<Song>> getSongs() const override;

        /**
         * @brief Getter da contagem de músicas na coleção
         *
         * @return size_t número de músicas na coleção
         */
        size_t getSongsCount() const override;

        /**
         * @brief Setter da função de loader
         *
         * @param loader função para fazer load de música
         */
        virtual void setSongsLoader(
            const std::function<std::vector<std::shared_ptr<Song>>()>& loader)
            override;

        /**
         * @brief Adiciona música à coleção
         *
         * @param song música a ser adicionada
         */
        virtual void addSong(const Song& song) override;

        // /**
        //  * @brief Troca uma música de posição na playlist
        //  *
        //  * @param id da música a ser trocada de posição
        //  * @param index para o qual a música será realocada
        //  * @return true caso a troca seja bem sucedida
        //  * @return false caso a troca não seja bem sucedida
        //  */
        // virtual bool switchSong(unsigned id, unsigned index) override;

        /**
         * @brief
         *
         * @param id
         * @return true
         * @return false
         */
        virtual bool removeSong(unsigned id) override;

        /**
         * @brief Acha uma música pelo ID da música
         *
         * @param songId id da música
         * @return std::shared_ptr<Song> para a instância da música
         * @return nullptr caso a música não exista
         */
        virtual std::shared_ptr<Song>
        findSongById(unsigned songId) override;

        /**
         * @brief Acha uma música pelo título da música
         *
         * @param title título da música
         * @return std::shared_ptr<Song> para a instância da música
         * @return nullptr caso a música não exista
         */
        virtual std::vector<std::shared_ptr<Song>>
        findSongByTitle(const std::string& title) override;

        /**
         * @brief Calcula duração total da música
         *
         * @return duração da música em segundos
         */
        virtual unsigned calculateTotalDuration() override;

        // /**
        //  * @brief Calcula a duração e formata a string
        //  *
        //  * @return std::string no formato hh:mm
        //  */
        // virtual std::string getFormattedDuration() override;

        // /**
        //  * @brief Pega a próxima música na ordem da playlist
        //  *
        //  * @param current música atual
        //  * @return std::shared_ptr<Song> para a próxima
        //  */
        // virtual std::shared_ptr<Song>
        // getNextSong(Song &current) override;

        // /**
        //  * @brief Pega a música anterior
        //  *
        //  * @param current música atual
        //  * @return std::shared_ptr<Song> para a anterior
        //  */
        // virtual std::shared_ptr<Song>
        // getPreviousSong(Song &current) override;

        /**
         * @brief Pega a música na posição
         *
         * @param index posição da música
         * @return std::shared_ptr<Song> para a música no index indicado
         * @return nullptr caso o index esteja fora do escopo
         */
        virtual std::shared_ptr<Song> getSongAt(int index) override;

        /**
         * @brief Operador de indexação para acessar músicas na playlist
         *
         * @param index posição da música
         * @return std::shared_ptr<Song> para a música no index indicado
         * @return nullptr caso o index esteja fora do escopo
         */
        virtual std::shared_ptr<Song> operator[](int index) override;

        // IPlayable
        /**
         * @brief Obtém os objetos reproduzíveis
         * @return Vetor de ponteiros compartilhados para objetos reproduzíveis
         */
        virtual std::vector<std::shared_ptr<IPlayableObject>>
        getPlayableObjects() const override;

        /**
         * @brief Getter de user da interface IPlayable
         *
         * @return std::shared_ptr<User> para o usuário
         */
        std::shared_ptr<const User> getUser() const;

        /**
         * @brief Setter para o user da interface IPlayable
         *
         * @param user usuário
         */
        void setUser(const User& user);
    };
}  // namespace core
