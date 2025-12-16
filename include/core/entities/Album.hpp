/**
 * @file Album.hpp
 * @brief Definição da classe Album para representar uma coleção de músicas
 * de um artista
 *
 * Esta classe armazena um álbum de um artista, e fornece métodos para
 * manipulação e exibição das músicas incluídas no álbum. Ademais, deve ser
 * implementado a interface IPlayable
 *
 * @author Joao Tavares
 * @date 2025-10-09
 */

#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "core/bd/AlbumRepository.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/Entity.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"
#include "core/interfaces/ICollection.hpp"
#include "core/interfaces/IPlayable.hpp"

namespace core {
    // forward declaration
    class AlbumRepository;
    /**
     * @class Album
     * @brief Representa um álbum musical com suas músicas
     *
     * A classe Album armazena informações como nome, artista, gênero e
     * a lista de músicas contidas no álbum. Mantém a ordem original
     * das faixas para reprodução sequencial.
     */
    class Album : public Entity,
                  public IPlayable,
                  public ICollection {
    private:
        std::string _title;
        std::shared_ptr<User> _user;
        std::string _genre;
        int _year;
        // unsigned _user_id;
        unsigned _artist_id;
        mutable std::weak_ptr<Artist> _artist;
        mutable std::vector<unsigned> _featuring_artists_ids;
        mutable std::vector<std::shared_ptr<Song>> _songs;

        mutable bool _songsLoaded = false;

        std::function<std::vector<std::shared_ptr<Song>>()> songsLoader;
        std::function<std::shared_ptr<Artist>()> artistLoader;
        std::function<std::vector<std::shared_ptr<Artist>>()>
            featuringArtistsLoader;

        /**
         * @brief Carrega todas as músicas do álbum
         * @return Vector com todas as músicas do álbum
         */
        std::vector<std::shared_ptr<Song>> loadSongs() const;

    public:
        /**
         * @brief Construtor vazio
         */
        Album();

        // Album para mapRowToEntity
        Album(unsigned id,
              std::string title,
              int year,
              std::string genre,
              const Artist &artist,
              User &user);

        /**
         * @brief Construtor da classe Album
         * @param title Nome do álbum
         * @param artist Nome do artista do álbum
         * @param genre Gênero do álbum
         */
        Album(const std::string title,
              const std::string genre,
              const Artist &artist);

        /**
         * @brief Construtor completo da classe Album
         * @param id Identificador único
         * @param title Nome do álbum
         * @param artist Nome do artista
         * @param genre Gênero musical
         * @param year Ano de lançamento
         */
        Album(unsigned id,
              const std::string title,
              int year,
              const std::string genre,
              const Artist &artist);

        /**
         * @brief Construtor de cópia da classe Album
         * @param other Outro objeto Album para copiar
         */
        Album(const Album &other) = default;

        /**
         * @brief Operador de atribuição da classe Album
         * @param other Outro objeto Album para atribuir
         * @return Referência ao objeto Album atribuído
         */
        Album &operator=(const Album &other) = default;

        /**
         * @brief Destrutor padrão da clase Album
         * @note Responsável por liberar a memória dos vetores alocados
         */
        ~Album() = default;

        // Getters
        /**
         * @brief Obtém o nome do álbum
         * @return Nome do álbum
         */
        std::string getTitle() const;

        /*
         * @brief Retorna o usuario do album
         * @return shared_ptr<User>
         */
        std::shared_ptr<const User> getUser() const;

        /**
         * @brief Obtém o artista do álbum
         * @return Nome do artista/banda
         */
        std::shared_ptr<const Artist> getArtist() const;

        /**
         * @brief Obtém os artistas colaboradores (featuring)
         * @return Vetor de ponteiros compartilhados para os artistas
         * colaboradores
         */
        std::vector<std::shared_ptr<const Artist>> getFeaturingArtists() const;

        /**
         * @brief Obtém as músicas do álbum
         * @return Vetor de ponteiros compartilhados para as músicas
         */
        std::vector<std::shared_ptr<Song>> getSongs() const override;

        /**
         * @brief Obtém o gênero do álbum
         * @return Gênero musical principal
         */
        std::string getGenre() const;

        /**
         * @brief Obtém o ano de lançamento
         * @return Ano de lançamento do álbum
         */
        int getYear() const;

        /**
         * @brief Obtém a quantidade de músicas no álbum
         * @return Número total de músicas
         */
        size_t getSongsCount() const override;

        /**
         * @brief Verifica se as músicas do artista foram carregadas
         */
        bool isSongsLoaded() const;

        // Setters

        /**
         * @brief Define o título do álbum
         * @param title Novo título do álbum
         */
        void setTitle(const std::string &title);

        /**
         * @brief Define o artista do álbum
         * @param artist Novo nome do artista
         */
        void setArtist(const Artist &artist);

        /**
         * @brief Define os artistas colaboradores (featuring)
         * @param artists Vetor de artistas colaboradores
         */
        void setFeaturingArtists(const std::vector<Artist> &artists);

        /**
         * @brief Define o gênero do álbum
         * @param genre Novo gênero musical
         */
        void setGenre(const std::string &genre);

        /**
         * @brief Define o ano de lançamento
         * @param year Novo ano de lançamento
         */
        void setYear(int year);

        /**
         * @brief Define o usuário associado ao álbum
         * @param user Ponteiro compartilhado para o usuário
         */
        void setUser(const User &user);

        /**
         * @brief Define a função para carregar o artista do álbum
         * @param loader Função que retorna um ponteiro compartilhado para o
         * artista
         */
        void
        setArtistLoader(const std::function<std::shared_ptr<Artist>()> &loader);

        /**
         * @brief Define a função para carregar os artistas colaboradores do
         * álbum
         * @param loader Função que retorna um vetor de ponteiros compartilhados
         * para os artistas colaboradores
         */
        void setFeaturingArtistsLoader(
            const std::function<std::vector<std::shared_ptr<Artist>>()> &
                loader);

        /**
         * @brief Define a função de carregamento para as músicas do álbum
         * @param loader Função que retorna um vetor de ponteiros compartilhados
         * para IPlayable
         */
        void setSongsLoader(
            const std::function<std::vector<std::shared_ptr<Song>>()> &
                loader) override;

        /**
         * @brief Obtém informações do álbum em formato de string
         * @return String com nome, artista, ano e quantidade de músicas
         */
        std::string toString() const;

        // Métodos Entity

        /**
         * @brief Compara dois Albums
         * @param other Album a ser comparada
         * @return true se as entidades forem iguais, false caso contrário
         */
        bool operator==(const Entity &other) const override;

        /**
         * @brief Compara dois Albums para desigualdade
         * @param other Album a ser comparada
         * @return true se as entidades forem diferentes, false caso contrário
         */
        bool operator!=(const Entity &other) const override;

        /**
         * @brief Compara qual Album é menor por ano se forem do mesmo artista
         * caso contrário por ordem lexicográfica
         * @param other Album a ser comparada
         * @return true se a entidade atual for menor que a outra, false caso
         * contrário
         */
        bool operator<(const Entity &other) const override;

        /**
         * @brief Compara qual Album é maior
         * @param other Album a ser comparada
         * @return true se a entidade atual for maior que a outra, false caso
         * contrário
         */
        bool operator<=(const Entity &other) const override;

        /**
         * @brief Compara qual Album é maior por  por ano se forem do mesmo artista
         * caso contrário por ordem lexicográfica
         * @param other Album a ser comparada
         * @return true se a entidade atual for maior que a outra, false caso
         * contrário
         */
        bool operator>(const Entity &other) const override;

        /**
         * @brief Compara qual Album é maior ou igual
         * @param other Album a ser comparada
         * @return true se a entidade atual for maior ou igual que a outra,
         * false caso contrário
         */
        bool operator>=(const Entity &other) const override;

        /**
         * @brief Obtém os objetos reproduzíveis
         * @return Vetor contendo esta música como IPlayableObject
         */
        std::vector<std::shared_ptr<IPlayableObject>>
        getPlayableObjects() const override;


        /**
         * @brief Adiciona uma música ao álbum
         * @param song Ponteiro compartilhado para IPlayable representando a
         * música a ser adicionada
         */
        void addSong(const Song &song) override;

        /**
         * @brief Remove uma música do álbum pelo ID
         * @param id ID da música a ser removida
         * @return true se a música foi encontrada e removida, false caso
         * contrário
         */
        bool removeSong(unsigned id) override;

        /**
         * @brief Busca uma música pelo ID
         * @param songId ID da música a ser buscada
         * @return Ponteiro compartilhado para IPlayable da música encontrada,
         * ou nullptr se não encontrada
         */
        std::shared_ptr<Song> findSongById(unsigned songId) override;

        /**
         * @brief Busca uma música pelo título
         * @param title Título da música a ser buscada
         * @return Vetor de ponteiros com as músicas encontradas
         */
        std::vector<std::shared_ptr<Song>>
        findSongByTitle(const std::string &title) override;

        /**
         * @brief Calcula a duração total do álbum
         * @return Duração total em segundos
         */
        unsigned calculateTotalDuration() override;

        /**
         * @brief Obtém a música em uma posição específica do álbum
         * @param index Índice da música no álbum
         * @return Ponteiro compartilhado para IPlayable da música na posição
         * especificada, ou nullptr se índice inválido
         */
        std::shared_ptr<Song> getSongAt(int index) override;

        /**
         * @brief Obtém a música em uma posição específica do álbum
         * @param index Índice da música no álbum
         * @return Ponteiro compartilhado para IPlayable da música na posição
         * especificada, ou nullptr se índice inválido
         */
        std::shared_ptr<Song> operator[](int index) override;
    };

} // namespace core
