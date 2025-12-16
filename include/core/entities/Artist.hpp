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

#include <cstddef>
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
        std::string _name;
        std::string _genre;
        mutable std::vector<std::shared_ptr<Song>> _songs;
        mutable bool _songsLoaded = false;
        mutable std::vector<std::shared_ptr<Album>> _albums;
        mutable bool _albumsLoaded = false;
        std::shared_ptr<User> _user; // TODO Trocar para User?
        unsigned _user_id;

        std::function<std::vector<std::shared_ptr<Song>>()> songsLoader;
        std::function<std::vector<std::shared_ptr<Album>>()> albumsLoader;

        /**
         * @brief Carrega todas as músicas do artista
         * @return Vector com todas as músicas do artista
         */
        std::vector<std::shared_ptr<Song>> loadSongs() const;

        /**
         * @brief Carrega todos os álbuns do artista
         * @return Vector com todos os álbuns do artista
         */
        std::vector<std::shared_ptr<Album>> loadAlbums() const;

    public:
        /**
         * @brief Construtor vazio
         */
        Artist();

        /**
         * @brief Construtor da classe Artist com ID, nome, genero e usuário
         * @param id ID do artista
         * @param name Nome do artista
         * @param genre Gênero musical do artista
         * @param user Ponteiro compartilhado para o usuário associado ao artista
         */
        Artist(unsigned id,
               std::string name,
               std::string genre,
               const User &user);

        /**
		 * @brief Construtor da classe Artist ideal para repositórios
		 * @param id ID do artista
		 * @param name Nome do artista
		 * @param user_id ID do usuário associado ao artista
		 */
        Artist(unsigned id, std::string name, const User &user);


        /**
         * @brief Construtor da classe Artist
         * @param name Nome do artista
         * @param genre Gênero musical do artista
         */
        Artist(const std::string &name, const std::string &genre);

        /**
		 * @brief Construtor de cópia da classe Artist
		 * @param other Outro objeto Artist para copiar
		 */
        Artist(const Artist &other) = default;

        /**
         * @brief Operador de atribuição da classe Artist
         * @param other Outro objeto Artist para atribuir
         * @return Referência ao objeto Artist atribuído
		 */
        Artist &operator=(const Artist &other) = default;

        /**
         * @brief Destrutor da classe Artist
         * @note Responsável por liberar a memória dos vetores alocados
         */
        ~Artist() = default;

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
		 * @brief Obtém o usuário associado ao artista
		 * @return Ponteiro compartilhado para o usuário
		 */
        std::shared_ptr<const User> getUser() const;

        /**
         * @brief Obtém todas as músicas do artista
         * @return Vector com todas as músicas do artista
         */
        std::vector<std::shared_ptr<Song>> getSongs() const override;

        /**
         * @brief Verifica se as músicas do artista foram carregadas
         */
        bool isSongsLoaded() const;

        /**
         * @brief Verifica se os álbuns do artista foram carregados
         */
        bool isAlbumsLoaded() const;

        /**
         * @brief Obtém todos os álbuns do artista
         * @return Vector com todos os álbuns do artista
         */
        std::vector<std::shared_ptr<Album>> getAlbums() const;

        /**
         * @brief Obtém a quantidade de músicas do artista
         * @return Número total de músicas
         */
        size_t getSongsCount() const override;

        /**
         * @brief Obtém a quantidade de álbuns do artista
         * @return Número total de álbuns
         */
        size_t getAlbumsCount() const;

        /**
         * @brief Define a função para carregar os álbuns do artista
         * @param loader Função que retorna um vetor de álbuns
         */
        void setAlbumsLoader(
            const std::function<std::vector<std::shared_ptr<Album>>()> &loader);

        /**
         * @brief Define a função para carregar as músicas do artista
		 * @param loader Função que retorna um vetor de músicas
         */
        void setSongsLoader(const std::function<std::vector<std::shared_ptr<Song>>()>
                                &loader) override;

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
         * @brief Define o usuário associado ao artista
         * @param user Ponteiro compartilhado para o usuário
         */
        void setUser(const User &user);

        /**
         * @brief Adiciona uma música ao artista
         * @param song Música a ser adicionada
         */
        void addSong(const Song &song) override;

        /**
         * @brief Adiciona um álbum ao artista
         * @param album Álbum a ser adicionado
         */
        void addAlbum(const Album &album);

        // /**
        //  * @brief Move uma música para uma nova posição no Artist
        //  * @param id ID da música a ser movida
        //  * @param index Nova posição (índice) da música no álbum
        //  * @return true se a operação foi bem-sucedida, false caso contrário
        //  */
        // bool switchSong(unsigned id, unsigned index) override;

        /**
         * @brief Busca uma musica pelo título
         * @param title Título da musica de um artista a ser buscado
         * @return Vetor de ponteiros compartilhados para Song das músicas
         * encontradas
         */
        std::vector<std::shared_ptr<Song>> findSongByTitle(const std::string &title) override;
        /**
         * @brief Busca uma música de um artista pelo ID
         * @param songId ID da música a ser buscada
         * @return Ponteiro compartilhado para IPlayable da música encontrada,
         * ou nullptr se não encontrada
         */
        std::shared_ptr<Song> findSongById(unsigned songId) override;
	    /**
         * @brief Busca um álbum pelo titulo
		 * @param title Nome do álbum a ser buscado
		 * @return Vetor de ponteiros compartilhados para Album dos álbuns
         * encontrados
         */
        std::vector<std::shared_ptr<Album>> findAlbumByTitle(const std::string &title);
        /**
         * @brief Busca um álbum pelo ID
         * @param albumId ID do álbum a ser buscado
		 * @return Ponteiro compartilhado para Album do álbum encontrado,
         */
        std::shared_ptr<Album> findAlbumById(unsigned albumId);

        /**
         * @brief Calcula a duração total de um artista
         * @return Duração total em segundos
         */
        unsigned calculateTotalDuration() override;

        /**
         * @brief Remove um álbum do artista
         * @param albumId ID do álbum a ser removido
         * @return true se o álbum foi removido com sucesso, false caso
         * contrário
         */
        bool removeAlbum(unsigned albumId);

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
		 * @brief Compara qual Artista é menor por ordem lexicográfica
		 * @param other Artista a ser comparada
		 * @return true se a entidade atual for menor que a outra, false caso
		 * contrário
		 */
        bool operator<(const Entity &other) const override;

        /**
		 * @brief Compara qual Artista é maior
		 * @param other Artista a ser comparada
		 * @return true se a entidade atual for maior que a outra, false caso
		 * contrário
		 */
        bool operator<=(const Entity &other) const override;

        /**
         * @brief Compara qual Artista é maior por ordem lexicográfica
         * @param other Artista a ser comparada
         * @return true se a entidade atual for maior que a outra, false caso
		 * contrário
		 */
        bool operator>(const Entity &other) const override;

        /**
         * @brief Compara qual Artista é maior ou igual
         * @param other Artista a ser comparada
         * @return true se a entidade atual for maior ou igual que a outra, false
		 * caso contrário
         */
        bool operator>=(const Entity &other) const override;

        /**
        //  * @brief Obtém um vetor com todas as músicas do álbum de um Artista
        //  *  @param id Album
        //  * @return Vetor de ponteiros compartilhados para Song contendo
        //  * todas as músicas
        //  */
        // std::vector<std::shared_ptr<Song>> getSongsAlbum(unsigned idAlbum);

        // /**
        //  * @brief Adiciona uma música a algum album
        //  * @param song Ponteiro compartilhado para IPlayable representando a
        //  * @param idAlbum unsigned com id do album
        //  * música a ser adicionada
        //  */
        // void addSongAlbum(Song &song, unsigned idAlbum);
        /**
         * @brief Remove uma música do artista pelo ID
         * @param id ID da música a ser removida
         * @return true se a música foi encontrada e removida, false caso
         * contrário
         */
        bool removeSong(unsigned id) override;

        // /**
        //  * @brief Remove uma música do álbum pelo ID
        //  * @param id ID da música a ser removida
        //  *  @param id ID do album alvo
        //  * @return true se a música foi encontrada e removida, false caso
        //  * contrário
        //  */
        // bool removeSongAlbum(unsigned id, unsigned idAlbum);

        /**
        //  * @brief Obtém a próxima música em relação à música atual
        //  * @param current Música atual como referência
        //  * @return Ponteiro compartilhado para IPlayable da próxima música, ou
        //  * nullptr se não houver próxima
        //  */
        // std::shared_ptr<Song>
        // getNextSong(Song &current) override;

        // /**
        //  * @brief Obtém a música anterios em relação à música atual
        //  * @param current Música atual como referência
        //  * @return Ponteiro compartilhado para IPlayable da próxima música, ou
        //  * nullptr se não houver próxima
        //  */
        // std::shared_ptr<Song>
        // getPreviousSong(Song &current) override;

        // /**
        //  * @brief Obtém a próxima música em relação à música atual do album
        //  * @param current Música atual como referência
        //  * @param idAlbum unsigned id do album
        //  * @return Ponteiro compartilhado para IPlayable da próxima música, ou
        //  * nullptr se não houver próxima
        //  */
        // std::shared_ptr<Song>
        // getNextSongAlbum(Song &current, unsigned idAlbum);

        // /**
        //  * @brief Obtém a música anterios em relação à música atual do album
        //  * @param current Música atual como referência
        //  * @param idAlbum unsigned id do album
        //  * @return Ponteiro compartilhado para IPlayable da próxima música, ou
        //  * nullptr se não houver próxima
        //  */
        // std::shared_ptr<Song>
        // getPreviousSongAlbum(Song &current, unsigned idAlbum);
        /**
         * @brief Obtém a música em uma posição específica de songs
         * @param index Índice da música em songs
         * @return Ponteiro compartilhado para IPlayable da música na posição
         * especificada, ou nullptr se índice inválido
         */
        std::shared_ptr<Song> getSongAt(int index) override;

        /**
         * @brief Obtém a música em uma posição específica de songs
         * @param index Índice da música em songs
		 * @return Ponteiro compartilhado para IPlayable da música na posição
         */
        std::shared_ptr<Song> operator[](int index) override;

        /**
         * @brief Obtem o album em uma posição específica de albums
         * @param index Índice do album em albums
         * @return Ponteiro compartilhado para Album na posição especificada,
         * ou nullptr se índice inválido
         */
        std::shared_ptr<Album> getAlbumAt(int index);

        // /**
        //  * @brief Obtém a música em uma posição específica de Album
        //  * @param index Índice da música em songs
        //  * @param idAlbum Id do album
        //  * @return Ponteiro compartilhado para IPlayable da música na posição
        //  * especificada, ou nullptr se índice inválido
        //  */
        // std::shared_ptr<IPlayable> getSongAtAlbum(int index, unsigned idAlbum);


        // TODO IMPLEMENTAÇÃO NO CPP

    };
} // namespace core
