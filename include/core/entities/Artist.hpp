/**
 * @file Artist.hpp
 * @brief Definição da classe Artist para representar um artista musical
 *
 * Esta classe armazena informações de um artista musical, incluindo seu
 * nome, gênero, e coleções de músicas e álbuns. Fornece métodos para
 * gerenciamento e consulta do catálogo do artista.
 *
 * @author Joao Tavares
 * @date 2025-11-09
 */

#pragma once

#include "core/entities/Album.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"

#include "core/entities/Entity.hpp"
#include "core/interfaces/ICollection.hpp"
#include "core/interfaces/IPlayable.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace core {
    /**
     * @class Artist
     * @brief Representa um artista musical com suas músicas e álbuns
     *
     * A classe Artist gerencia as informações básicas de um artista e
     * mantém referências para todas as suas músicas e álbuns associados.
     * Permite operações de busca, adição e remoção de itens do catálogo.
     */
    class Artist : public core::Entity,
                   public core::ICollection,
                   public core::IPlayable {
    private:
        int nome;
        unsigned _id = 0; // TODO nao ta gerando apartir de entity
        std::string _name;
        std::string _genre;
        std::vector<std::shared_ptr<Song>> _songs;
        std::vector<std::shared_ptr<Album>> _albums;
        std::shared_ptr<User> user;
        unsigned _user_id;

        std::function<std::vector<std::shared_ptr<Song>>()> songsLoader;
        std::function<std::vector<std::shared_ptr<Album>>()> albumsLoader;

    public:
        /**
         * @brief Construtor vazio
         */
        Artist();

        Artist(unsigned id, std::string name, unsigned user_id);

        /**
         * @brief Construtor da classe Artist
         * @param name Nome do artista
         * @param genre Gênero musical do artista
         */
        Artist(const std::string &name, const std::string &genre);

        /**
         * @brief Destrutor da classe Artist
         * @note Responsável por liberar a memória dos vetores alocados
         */
        ~Artist();

        /**
         * @brief Obtém o nome do artista
         * @return Nome do artista/banda
         */
        std::string getName() const;

        /**
         * @brief Obtém o gênero musical
         * @return Gênero musical principal do artista
         */
        std::string getGenre() const;

        /**
         * @brief Obtém todas as músicas do artista
         * @return Vector com todas as músicas do artista
         */
        std::vector<std::shared_ptr<Song>> getSongs() const;

        /**
         * @brief Obtém todos os álbuns do artista
         * @return Vector com todos os álbuns do artista
         */
        std::vector<std::shared_ptr<Album>> getAlbums() const;

        /**
         * @brief Obtém a quantidade de músicas do artista
         * @return Número total de músicas
         */
        int getSongsCount() const;

        /**
         * @brief Obtém a quantidade de álbuns do artista
         * @return Número total de álbuns
         */
        int getAlbumsCount() const;

        /**
         * @brief Define a função para carregar os álbuns do artista
         * @param loader Função que retorna um vetor de álbuns
         */
        void setAlbumsLoader(
            const std::function<std::vector<std::shared_ptr<Album>>()> &loader);

        /**
         * @brief Define o nome do artista
         * @param name Novo nome do artista
         */
        void setName(const std::string &name);

        /**
         * @brief Define o gênero musical
         * @param genre Novo gênero musical
         */
        void setGenre(const std::string &genre);

        /**
         * @brief Adiciona uma música ao artista
         * @param song Música a ser adicionada
         */
        void addSong(const Song &song);

        /**
         * @brief Adiciona um álbum ao artista
         * @param album Álbum a ser adicionado
         */
        void addAlbum(const Album &album);

        /**
         * @brief Move uma música para uma nova posição no Artist
         * @param id ID da música a ser movida
         * @param index Nova posição (índice) da música no álbum
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool switchSong(unsigned id, unsigned index) override;

        /**
         * @brief Busca uma musica pelo título
         * @param title Título da musica de um artista a ser buscado
         * @return Ponteiro compartilhado para IPlayable da música encontrada,
         * ou nullptr se não encontrada
         */
        std::shared_ptr<Song> findSongByTitle(const std::string &title) override;
        /**
         * @brief Busca uma música de um artista pelo ID
         * @param songId ID da música a ser buscada
         * @return Ponteiro compartilhado para IPlayable da música encontrada,
         * ou nullptr se não encontrada
         */
        std::shared_ptr<Song> findSongById(unsigned songId) override;

        /**
         * @brief Calcula a duração total de um artista
         * @return Duração total em segundos
         */
        int calculateTotalDuration() override;

        /**
         * @brief Remove um álbum do artista
         * @param albumId ID do álbum a ser removido
         * @return true se o álbum foi removido com sucesso, false caso
         * contrário
         */
        bool removeAlbum(unsigned albumId);

        /**
         * @brief Busca uma música pelo nome
         * @param songName Nome da música a buscar
         * @return Ponteiro para a música encontrada ou nullptr se não
         * encontrada
         */
        // const Song* findSongByName(const std::string& songName); ja tem o
        // findSongByTitle

        /**
         * @brief Busca um álbum pelo nome
         * @param albumName Nome do álbum a buscar
         * @return Ponteiro para o álbum encontrado ou nullptr se não encontrado
         */
        std::shared_ptr<Album> findAlbumByName(const std::string &albumName) const;

        /**
         * @brief Calcula a duração total de todas as músicas
         * @return Duração total em segundos
         */
        int getTotalDuration() const;

        /**
         * @brief Obtém a duração total formatada
         * @return String com duração no formato "HH:MM:SS" ou "MM:SS"
         */
        std::string getFormattedDuration() override;

        /**
         * @brief Converte o artista para string formatada
         * @return String com nome, gênero e estatísticas do artista
         */
        std::string toString() const;

        /**
         * @brief Verifica se o artista tem músicas
         * @return true se possui pelo menos uma música, false caso contrário
         */
        bool hasSong() const;

        /**
         * @brief Verifica se o artista tem álbuns
         * @return true se possui pelo menos um álbum, false caso contrário
         */
        bool hasAlbum() const;

        /**
         * @brief Obtém os objetos reproduzíveis
         * @return Vetor contendo esta música como IPlayableObject
         */

        std::vector<std::shared_ptr<IPlayableObject>>
        getPlayableObjects() const override;

        // Métodos Entity

        /**
         * @brief Compara dois Artistas
         * @param other Artista a ser comparada
         * @return true se as entidades forem iguais, false caso contrário
         */
        bool operator==(const Entity &other) const override;

        /**
         * @brief Compara dois Artistas para desigualdade
         * @param other Artista a ser comparada
         * @return true se as entidades forem diferentes, false caso contrário
         */
        bool operator!=(const Entity &other) const override;

        /**
         * @brief Obtém um vetor com todas as músicas de um Artista
         * @return Vetor de ponteiros compartilhados para IPlayable contendo
         * todas as músicas
         */
        std::vector<std::shared_ptr<IPlayable>> getSongs();

        /**
         * @brief Obtém um vetor com todas as músicas do álbum de um Artista
         *  @param id Album
         * @return Vetor de ponteiros compartilhados para IPlayable contendo
         * todas as músicas
         */
        std::vector<std::shared_ptr<IPlayable>> getSongsAlbum(unsigned idAlbum);

        void setSongsLoader(
            const std::function<std::vector<std::shared_ptr<IPlayable>>()> &loader);

        /**
         * @brief Adiciona uma música ao artista
         * @param song Ponteiro compartilhado para IPlayable representando a
         * música a ser adicionada
         */
        void addSong(std::shared_ptr<Song> song) override;

        /**
         * @brief Adiciona uma música a algum album
         * @param song Ponteiro compartilhado para IPlayable representando a
         * @param idAlbum unsigned com id do album
         * música a ser adicionada
         */
        void addSongAlbum(std::shared_ptr<IPlayable> song, unsigned idAlbum);
        /**
         * @brief Remove uma música do artista pelo ID
         * @param id ID da música a ser removida
         * @return true se a música foi encontrada e removida, false caso
         * contrário
         */
        bool removeSong(unsigned id) override;
        /**
         * @brief Remove uma música do álbum pelo ID
         * @param id ID da música a ser removida
         *  @param id ID do album alvo
         * @return true se a música foi encontrada e removida, false caso
         * contrário
         */
        bool removeSongAlbum(unsigned id, unsigned idAlbum);

        /**
         * @brief Obtém a próxima música em relação à música atual
         * @param current Música atual como referência
         * @return Ponteiro compartilhado para IPlayable da próxima música, ou
         * nullptr se não houver próxima
         */
        std::shared_ptr<Song>
        getNextSong(std::shared_ptr<Song> current) override;

        /**
         * @brief Obtém a música anterios em relação à música atual
         * @param current Música atual como referência
         * @return Ponteiro compartilhado para IPlayable da próxima música, ou
         * nullptr se não houver próxima
         */
        std::shared_ptr<Song>
        getPreviousSong(std::shared_ptr<Song> current) override;

        /**
         * @brief Obtém a próxima música em relação à música atual do album
         * @param current Música atual como referência
         * @param idAlbum unsigned id do album
         * @return Ponteiro compartilhado para IPlayable da próxima música, ou
         * nullptr se não houver próxima
         */
        std::shared_ptr<IPlayable>
        getNextSongAlbum(std::shared_ptr<IPlayable> current, unsigned idAlbum);

        /**
         * @brief Obtém a música anterios em relação à música atual do album
         * @param current Música atual como referência
         * @param idAlbum unsigned id do album
         * @return Ponteiro compartilhado para IPlayable da próxima música, ou
         * nullptr se não houver próxima
         */
        std::shared_ptr<IPlayable>
        getPreviousSongAlbum(std::shared_ptr<IPlayable> current, unsigned idAlbum);
        /**
         * @brief Obtém a música em uma posição específica de songs
         * @param index Índice da música em songs
         * @return Ponteiro compartilhado para IPlayable da música na posição
         * especificada, ou nullptr se índice inválido
         */
        std::shared_ptr<Song> getSongAt(int index) override;

        /**
         * @brief Obtém a música em uma posição específica de Album
         * @param index Índice da música em songs
         * @param idAlbum Id do album
         * @return Ponteiro compartilhado para IPlayable da música na posição
         * especificada, ou nullptr se índice inválido
         */
        std::shared_ptr<IPlayable> getSongAtAlbum(int index, unsigned idAlbum);

        std::shared_ptr<User> getUser() const;

        // TODO IMPLEMENTAÇÃO NO CPP
        void setSongsLoader(const std::function<std::vector<std::shared_ptr<Song>>()>
                                &loader) override;
    };
} // namespace core
