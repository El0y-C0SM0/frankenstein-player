/**
 * @file Song.hpp
 * @brief Definição da classe Song para representar uma música no sistema
 *
 * Esta classe armazena os metadados de uma música e fornece métodos para
 * manipulação e exibição dessas informações. Futuramente será integrada
 * com a biblioteca TagLib para extração automática de metadados.
 *
 * @author Joao Tavares
 * @date 2025-10-09
 */

#pragma once
#include "core/entities/Entity.hpp"
#include "core/entities/User.hpp"
#include "core/interfaces/IPlayable.hpp"
#include "core/interfaces/IPlayableObject.hpp"
#include <functional>
#include <memory>
#include <miniaudio.h>
#include <string>
#include <vector>

namespace core {
    // Forward declaration
    class Album;
    class Artist;
    class SongRepository;

    inline const std::string SINGLE_ALBUM = "Single/";

    /**
     * @class Song
     * @brief Representa uma música com seus metadados
     *
     * A classe Song armazena informações como título, artista, álbum, duração,
     * gênero e ano de lançamento. Pode ser expandida para usar a biblioteca
     * TagLib para extração automática de metadados de arquivos de áudio.
     * Ademais, deve ser implementado a interface IPlayable
     */
    class Song : public core::Entity,
                 public core::IPlayable,
                 public core::IPlayableObject {
    private:
        // std::string _file_path;
        std::string _title;
        // unsigned user_id;
        std::shared_ptr<User> _user;
        unsigned _artist_id;
        mutable std::weak_ptr<Artist> _artist;
        std::vector<unsigned> _featuring_artists_ids;
        unsigned _album_id;
        mutable std::weak_ptr<Album> _album;
        int _duration;
        std::string _genre;
        int _year;
        unsigned _track_number;
        // bool _metadata_loaded;

        bool _artistLoaded = false;
        bool _albumLoaded = false;

        ma_decoder_config decoderConfig;

        std::function<std::shared_ptr<Artist>()> artistLoader;
        std::function<std::vector<std::shared_ptr<Artist>>()>
            featuringArtistsLoader;
        std::function<std::shared_ptr<Album>()> albumLoader;

        std::shared_ptr<Artist> loadArtist() const;
        std::vector<std::shared_ptr<Artist>> loadFeaturingArtists() const;
        std::shared_ptr<Album> loadAlbum() const;

    public:
        /**
         * @brief Construtor padrão - inicializa uma música com valores padrão
         */
        Song();

        /**
         * @brief Construtor com título, artista e álbum
         * @param title Título da música
         * @param artist Ponteiro compartilhado para o artista principal
         * @param album Ponteiro compartilhado para o álbum
         */
        Song(const std::string &title,
             std::shared_ptr<Artist> &artist,
             std::shared_ptr<Album> &album);

        /**
         * @brief Construtor básico para criação de música com informações essenciais
         * @param id Identificador único da música
         * @param title Título da música
         * @param artist_id Identificador do artista principal
         */
        Song(unsigned id,
             std::string title,
             unsigned artist_id);

        /**
         * @brief Construtor para música com ID, título, artista e usuário
         * @param id Identificador único da música
         * @param title Título da música
         * @param artist Referência para o ID do artista
         * @param album Referência para o ID do álbum
         */
        Song(unsigned id,
             const std::string &title,
             unsigned &artist_id,
             unsigned &album_id);

        /**
         * @brief Construtor completo com objetos relacionados
         * @param title Título da música
         * @param artist Referência para o objeto Artist
         * @param album Referência para o objeto Album
         * @param user Referência para o objeto User proprietário
         */
        Song(const std::string &title, const Artist &artist, const Album &album, const User &user);

        /**
         * @brief Construtor de cópia da classe Song
         * @param other Outro objeto Song para copiar
         */
        Song(const Song &other) = default;

        /**
         * @brief Operador de atribuição da classe Song
         * @param other Outro objeto Song para atribuir
         * @return Referência ao objeto Song atribuído
         */
        Song &operator=(const Song &other) = default;

        /**
         * @brief Destrutor da classe Song
         */
        ~Song() = default;

        // Getters
        /**
         * @brief Obtém o título da música
         * @return Título da música
         */
        std::string getTitle() const;
        /**
         * @brief Obtém o artista
         * @return Nome do artista/banda
         */
        std::shared_ptr<const Artist> getArtist() const;

        // /*
        //  * @brief Obtem ID dos artista musica
        //  * @return Vetor com id dos artistas
        //  */
        // std::vector<unsigned> getFeaturingArtistsId() const;

        /**
         * @brief Obtém os artistas colaboradores (featuring)
         * @return Vetor de ponteiros compartilhados para os artistas
         * colaboradores
         */
        std::vector<std::shared_ptr<const Artist>> getFeaturingArtists();

        /*
         * @brief Adciona um artista feat a musica
         * @param artist Artista  a ser adicionado
         */
        void
        setFeaturingArtists(std::shared_ptr<Artist> &
                                artist); // TODO overloard p passar o vector

        /**
         * @brief Obtém o álbum
         * @return Nome do álbum
         */
        std::shared_ptr<const Album> getAlbum() const;
        /**
         * @brief Obtém a duração
         * @return Duração em segundos
         */
        int getDuration() const;
        /**
         * @brief Obtém o número da faixa no album
         * @return Número da faixa
         */
        unsigned getTrackNumber() const;
        /**
         * @brief Obtém o gênero
         * @return Gênero musical
         */
        std::string getGenre() const;
        /**
         * @brief Obtém o ano de lançamento
         * @return Ano de lançamento
         */
        int getYear() const;
        /**
         * @brief Obtém o usuário dono da música
         * @return Usuário dono da música
         */
        std::shared_ptr<const User> getUser() const;
        // Setters
        /**
         * @brief Define o usuário dono da música
         * @param user Novo usuário
         */
        void setUser(const User &user);
        /**
         * @brief Define o título da música
         * @param title Novo título
         */
        void setTitle(const std::string &title);
        /**
         * @brief Define o artista principal
         * @param artist Novo artista
         */
        void setArtist(std::shared_ptr<Artist> &artist);
        /**
         * @brief Define o número da faixa no álbum
         * @param track_number Novo número da faixa
         */
        void setTrackNumber(unsigned track_number);
        /**
         * @brief Define a função para carregar o artista
         * @param loader Função que retorna um ponteiro compartilhado para o
         * artista
         */
        void
        setArtistLoader(const std::function<std::shared_ptr<Artist>()> &loader);
        /**
         * @brief Define a função para carregar os artistas colaboradores
         * @param loader Função que retorna um vetor de ponteiros compartilhados
         * para os artistas colaboradores
         */
        void setFeaturingArtistsLoader(
            const std::function<std::vector<std::shared_ptr<Artist>>()> &
                loader);
        /**
         * @brief Define os artistas colaboradores (featuring)
         * @param artists Vetor de artistas colaboradores
         */
        void setFeaturingArtists(const std::vector<Artist> &artists);
        /**
         * @brief Define função para carregar o álbum
         * @param loader Função que retorna um ponteiro compartilhado para o
         * álbum
         */
        void
        setAlbumLoader(const std::function<std::shared_ptr<Album>()> &loader);
        /**
         * @brief Define o álbum
         * @param album Novo álbum
         */
        void setAlbum(const Album &album);
        /**
         * @brief Define o gênero
         * @param genre Novo gênero
         */
        void setGenre(const std::string &genre);
        /**
         * @brief Define o ano de lançamento
         * @param year Novo ano
         */
        void setYear(int year);

        void setDuration(int sec);
        // Métodos

        /**
         * @brief Carrega metadados do arquivo de áudio
         * @return true se os metadados foram carregados com sucesso, false caso
         * contrário
         * @note Futuramente será implementado com a biblioteca TagLib
         */
        // bool loadMetadata(); Nao sei se vamos usar realmente metadados no
        // inicio

        /**
         * @brief Converte a música para string
         * @return String com todas as informações da música formatadas
         */
        std::string toString() const;

        // Operações no banco de dados será responsabilidade da classe? não
        //

        /**
         * @brief Obtém os objetos reproduzíveis (a própria música)
         * @return Vetor contendo esta música como IPlayableObject
         */
        std::vector<std::shared_ptr<IPlayableObject>>
        getPlayableObjects() const override;

        // Métodos Entity

        /**
         * @brief Compara dois Song
         * @param other Song a ser comparada
         * @return true se as entidades forem iguais, false caso contrário
         */
        bool operator==(const Entity &other) const override;

        /**
         * @brief Compara duas Song para desigualdade
         * @param other Song a ser comparada
         * @return true se as entidades forem diferentes, false caso contrário
         */
        bool operator!=(const Entity &other) const override;

        /**
         * @brief Compara qual Song é menor
         * @param other Song a ser comparada
         * @return true se a entidade atual for menor que a outra, false caso
         * contrário
         */
        bool operator<(const Entity &other) const override;

        /**
         * @brief Compara qual Song é maior ou igual
         * @param other Song a ser comparada
         * @return true se a entidade atual for maior ou igual que a outra, false
         * caso contrário
         */
        bool operator<=(const Entity &other) const override;

        /**
         * @brief Compara qual Song é maior
         * @param other Song a ser comparada
         * @return true se a entidade atual for maior que a outra, false caso
         * contrário
         */
        bool operator>(const Entity &other) const override;

        /**
         * @brief Compara qual Song é maior ou igual
         * @param other Song a ser comparada
         * @return true se a entidade atual for maior ou igual que a outra, false
         * caso contrário
         */
        bool operator>=(const Entity &other) const override;


        /**
         * @brief Obtém o caminho do arquivo de áudio
         * @return Caminho do arquivo de áudio
         */
        std::string getAudioFilePath() const override;

    }; // namespace core
} // namespace core
