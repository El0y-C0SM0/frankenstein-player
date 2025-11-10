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

#include <memory>
#include <string>
#include <vector>
#include <memory.h>

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
    class Playlist : public Entity, public ICollection, public IPlayable {
    private:
        std::string _titulo;
        unsigned _user_id;
        std::shared_ptr<User> _user;
        mutable std::vector<std::shared_ptr<Song>> _songs;

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
         * @param titulo Título da Playlist
         *
         */
        Playlist(const unsigned id, const std::string titulo);

        /**
         * @brief Destrutor padrão da classe Playlist
         * @note Libera a memória dos vetores alocados
         */
        ~Playlist();

        std::string getTitulo();
        std::string getTitle() const;

        void setTitulo(const std::string nome);

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

        // metodos ICollection
        virtual std::vector<std::shared_ptr<Song>> getSongs() const override;

        virtual void setSongsLoader(
            const std::function<std::vector<std::shared_ptr<Song>>()>&
                loader) override;

        virtual void addSong(std::shared_ptr<IPlayable> song) override;

        virtual bool switchSong(unsigned id, unsigned index) override;

        virtual bool removeSong(unsigned id) override;

        virtual std::shared_ptr<IPlayable>
        findSongById(unsigned songId) override;

        virtual std::shared_ptr<IPlayable>
        findSongByTitle(const std::string& title) override;

        virtual int calculateTotalDuration() override;

        virtual std::string getFormattedDuration() override;

        virtual std::shared_ptr<IPlayable>
        getNextSong(std::shared_ptr<IPlayable> current) override;

        virtual std::shared_ptr<IPlayable>
        getPreviousSong(std::shared_ptr<IPlayable> current) override;

        virtual std::shared_ptr<IPlayable> getSongAt(int index) override;

        // IPlayable
        /**
         * @brief Obtém os objetos reproduzíveis
         * @return Vetor de ponteiros compartilhados para objetos reproduzíveis
         */
        virtual std::vector<std::shared_ptr<IPlayableObject>>
        getPlayableObjects() const override;


        std::shared_ptr<User> getUser() const;
        void setUser(const User& user);
    };
}  // namespace core
