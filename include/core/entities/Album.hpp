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

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "core/entities/Artist.hpp"
#include "core/entities/Entity.hpp"
#include "core/entities/Song.hpp"
#include "core/interfaces/ICollection.hpp"
#include "core/interfaces/IPlayable.hpp"

namespace core {

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
                  public ICollection,
                  public IPlayableObject {
    private:
        unsigned _id;
        std::string _file_path;
        std::string _name;
        unsigned _artist_id;
        std::shared_ptr<User> _user;
        unsigned _user_id;
        mutable std::shared_ptr<Artist> _artist;
        mutable std::vector<unsigned> _featuring_artists_ids;
        mutable std::vector<std::shared_ptr<Song>> _songs;
        std::string _genre;
        int _year;

        std::function<std::vector<std::shared_ptr<Song>>()> songsLoader;
        std::function<std::shared_ptr<Artist>()> artistLoader;
        std::function<std::vector<std::shared_ptr<Artist>>()>
            featuringArtistsLoader;

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
              unsigned user_id);

        /**
         * @brief Construtor da classe Album
         * @param name Nome do álbum
         * @param artist Nome do artista do álbum
         * @param genre Gênero do álbum
         */
        Album(const std::string name,
              std::shared_ptr<Artist> artist,
              const std::string genre);

        /**
         * @brief Construtor completo da classe Album
         * @param id Identificador único
         * @param name Nome do álbum
         * @param artist Nome do artista
         * @param genre Gênero musical
         * @param year Ano de lançamento
         */
        Album(unsigned id,
              const std::string name,
              std::shared_ptr<Artist> artist,
              const std::string genre,
              int year);
        /**
         * @brief Destrutor padrão da clase Album
         * @note Responsável por liberar a memória dos vetores alocados
         */
        ~Album();

        // Getters
        /**
         * @brief Obtém o nome do álbum
         * @return Nome do álbum
         */
        std::string getName() const;

        /*
         * @brief Retorna o usuario do album
         * @return shared_ptr<User>
         */
        std::shared_ptr<User> getUser() const;
        /**
         * @brief Obtém o artista do álbum
         * @return Nome do artista/banda
         */
        std::shared_ptr<Artist> getArtist() const;

        /**
         * @brief Obtém os artistas colaboradores (featuring)
         * @return Vetor de ponteiros compartilhados para os artistas
         * colaboradores
         */
        std::vector<std::shared_ptr<const Artist>> getFeaturingArtists() const;

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
        int getSongCount() const;

        // Setters

        /**
         * @brief Define o caminho do album (se precisar dpeende da
         * implementação)
         * @param path
         */
        void setFilePath(std::string &path);

        /**
         * @brief Define o artista do álbum
         * @param artist Novo nome do artista
         */
        void setArtist(const std::shared_ptr<Artist> &artist);

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
         * @brief Obtém os objetos reproduzíveis
         * @return Vetor contendo esta música como IPlayableObject
         */
        std::vector<std::shared_ptr<IPlayableObject>>
        getPlayableObjects() const override;

        /**
         * @brief Obtém o caminho do arquivo de áudio
         * @return Caminho do arquivo de áudio
         */
        std::string getAudioFilePath() const override;

        /**
         * @brief Obtém um vetor com todas as músicas do álbum
         * @return Vetor de ponteiros compartilhados para IPlayable contendo
         * todas as músicas
         */
        std::vector<std::shared_ptr<IPlayable>> getSongs();

        /**
         * @brief Define a função de carregamento para as músicas do álbum
         * @param loader Função que retorna um vetor de ponteiros compartilhados
         * para IPlayable
         */
        void setSongsLoader(
            const std::function<std::vector<std::shared_ptr<IPlayable>>()> &
                loader);

        /**
         * @brief Adiciona uma música ao álbum
         * @param song Ponteiro compartilhado para IPlayable representando a
         * música a ser adicionada
         */
        void addSong(Song &song) override;

        /**
         * @brief Move uma música para uma nova posição no álbum
         * @param id ID da música a ser movida
         * @param index Nova posição (índice) da música no álbum
         * @return true se a operação foi bem-sucedida, false caso contrário
         */
        bool switchSong(unsigned id, unsigned index) override;

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
         * @return Ponteiro compartilhado para IPlayable da música encontrada,
         * ou nullptr se não encontrada
         */
        std::shared_ptr<Song>
        findSongByTitle(const std::string &title) override;

        /**
         * @brief Calcula a duração total do álbum
         * @return Duração total em segundos
         */
        int calculateTotalDuration() override;

        /**
         * @brief Obtém a duração formatada do álbum
         * @return String com a duração no formato "HH:MM:SS"
         */
        std::string getFormattedDuration() override;

        /**
         * @brief Obtém a próxima música em relação à música atual
         * @param current Música atual como referência
         * @return Ponteiro compartilhado para IPlayable da próxima música, ou
         * nullptr se não houver próxima
         */
        std::shared_ptr<Song>
        getNextSong(Song &current) override;

        /**
         * @brief Obtém a música anterior em relação à música atual
         * @param current Música atual como referência
         * @return Ponteiro compartilhado para IPlayable da música anterior, ou
         * nullptr se não houver anterior
         */
        std::shared_ptr<Song>
        getPreviousSong(Song &current) override;

        /**
         * @brief Obtém a música em uma posição específica do álbum
         * @param index Índice da música no álbum
         * @return Ponteiro compartilhado para IPlayable da música na posição
         * especificada, ou nullptr se índice inválido
         */
        std::shared_ptr<Song> getSongAt(int index) override;

        std::vector<std::shared_ptr<Song>> getSongs() const override;

        void setSongsLoader(
            const std::function<std::vector<std::shared_ptr<Song>>()> &loader)
            override;
    };

} // namespace core
