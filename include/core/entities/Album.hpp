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
        std::string _name;
        unsigned _artist_id;
        mutable std::weak_ptr<Artist> _artist;
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

        /**
         * @brief Construtor da classe Album
         * @param name Nome do álbum
         * @param artist Nome do artista do álbum
         * @param genre Gênero do álbum
         */
        Album(const std::string name,
              const std::string artist,
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
              const std::string artist,
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

        /**
         * @brief Obtém o artista do álbum
         * @return Nome do artista/banda
         */
        std::string getArtist() const;

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
         * @brief Define o artista do álbum
         * @param artist Novo nome do artista
         */
        void setArtist(const std::string& artist);

        /**
         * @brief Define os artistas colaboradores (featuring)
         * @param artists Vetor de artistas colaboradores
         */
        void setFeaturingArtists(const std::vector<Artist>& artists);

        /**
         * @brief Define o gênero do álbum
         * @param genre Novo gênero musical
         */
        void setGenre(const std::string& genre);

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
        setArtistLoader(const std::function<std::shared_ptr<Artist>()>& loader);

        /**
         * @brief Define a função para carregar os artistas colaboradores do
         * álbum
         * @param loader Função que retorna um vetor de ponteiros compartilhados
         * para os artistas colaboradores
         */
        void setFeaturingArtistsLoader(
            const std::function<std::vector<std::shared_ptr<Artist>>()>&
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
        virtual bool operator==(const Entity& other) const override;

        /**
         * @brief Compara dois Albums para desigualdade
         * @param other Album a ser comparada
         * @return true se as entidades forem diferentes, false caso contrário
         */
        virtual bool operator!=(const Entity& other) const override;

        /**
         * @brief Obtém os objetos reproduzíveis (a própria música)
         * @return Vetor contendo esta música como IPlayableObject
         */
        std::vector<std::shared_ptr<IPlayableObject>>
        getPlayableObjects() const override;

        /**
         * @brief Obtém o caminho do arquivo de áudio
         * @return Caminho do arquivo de áudio
         */
        virtual std::string getAudioFilePath() const override;

        std::vector<std::shared_ptr<IPlayable>> getSongs() override;
        void setSongsLoader(
            const std::function<std::vector<std::shared_ptr<IPlayable>>()>&
                loader) override;
        void addSong(std::shared_ptr<IPlayable> song) override;
        bool switchSong(unsigned id, unsigned index) override;
        bool removeSong(unsigned id) override;
        std::shared_ptr<IPlayable> findSongById(unsigned songId) override;
        std::shared_ptr<IPlayable>
        findSongByTitle(const std::string& title) override;
        int calculateTotalDuration() override;
        std::string getFormattedDuration() override;
        std::shared_ptr<IPlayable>
        getNextSong(std::shared_ptr<IPlayable> current) override;
        std::shared_ptr<IPlayable>
        getPreviousSong(std::shared_ptr<IPlayable> current) override;
        std::shared_ptr<IPlayable> getSongAt(int index) override;
    };

}  // namespace core

#pragma once
