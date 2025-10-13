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

#include <string>
#include <memory>
#include <vector>
#include <functional>

#include "core/interfaces/IPlayable.hpp"
#include "core/interfaces/IPlayableObject.hpp"
#include "core/entities/Entity.hpp"


namespace core {
    // Forward declaration
    class Album;
    class Artist;

    /**
    * @class Song
    * @brief Representa uma música com seus metadados
    *
    * A classe Song armazena informações como título, artista, álbum, duração,
    * gênero e ano de lançamento. Pode ser expandida para usar a biblioteca TagLib
    * para extração automática de metadados de arquivos de áudio. Ademais, deve ser
    *implementado a interface IPlayable
    */
    class Song : public core::Entity, public core::IPlayable, public core::IPlayableObject {
    private:
    unsigned _id;
    std::string _file_path;
    std::string _title;
    unsigned _artist_id;
    std::weak_ptr<Artist> _artist;
    std::vector<unsigned> _featuring_artists_ids;
    unsigned _album_id;
    std::weak_ptr<Album> _album;
    int _duration;
    std::string _genre;
    int _year;
    bool _metadata_loaded;

    std::function<std::shared_ptr<Artist>()> artistLoader;
    std::function<std::vector<std::shared_ptr<Artist>>()> featuringArtistsLoader;
    std::function<std::shared_ptr<Album>()> albumLoader;

    public:
    /**
    * @brief Construtor da classe Song
    * @param id Identificador único da música
    * @param file_path Caminho do arquivo de áudio
    * @param title Título da música
    * @param artist Artista/banda
    */
    Song(int id, const std::string &file_path, const std::string &title,
        const std::string &artist);

    // Getters
    /**
    * @brief Obtém o ID da música
    * @return Identificador único da música
    */
    int getId() const;

    /**
    * @brief Obtém o caminho do arquivo
    * @return Caminho completo do arquivo de áudio
    */
    std::string getFilePath() const;
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
    /**
     * @brief Obtém os artistas colaboradores (featuring)
     * @return Vetor de ponteiros compartilhados para os artistas colaboradores
     */
    std::vector<std::shared_ptr<const Artist>> getFeaturingArtists() const;
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
    * @brief Obtém o gênero
    * @return Gênero musical
    */
    std::string getGenre() const;
    /**
    * @brief Obtém o ano de lançamento
    * @return Ano de lançamento
    */
    int getYear() const;
    // Setters
    /**
    * @brief Define o título da música
    * @param title Novo título
    */
    void setTitle(const std::string &title);
    /**
    * @brief Define o artista principal
    * @param artist Novo artista
    */
    void setArtist(const Artist &artist);
    /**
     * @brief Define a função para carregar o artista
     * @param loader Função que retorna um ponteiro compartilhado para o artista
     */
    void setArtistLoader(const std::function<std::shared_ptr<Artist>()> &loader);
    /**
     * @brief Define a função para carregar os artistas colaboradores
     * @param loader Função que retorna um vetor de ponteiros compartilhados para os artistas colaboradores
     */
    void setFeaturingArtistsLoader(const std::function<std::vector<std::shared_ptr<Artist>>()> &loader);
    /**
     * @brief Define os artistas colaboradores (featuring)
     * @param artists Vetor de artistas colaboradores
     */
    void setFeaturingArtists(const std::vector<Artist> &artists);
    /**
     * @brief Define função para carregar o álbum
     * @param loader Função que retorna um ponteiro compartilhado para o álbum
     */
    void setAlbumLoader(const std::function<std::shared_ptr<Album>()> &loader);
    /**
    * @brief Define o álbum
    * @param album Novo álbum
    */
    void setAlbum(const Album &album);
    /**
    * @brief Define a duração
    * @param duration Nova duração em segundos
    */
    void setDuration(int duration);
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
    // Métodos
    /**
    * @brief Carrega metadados do arquivo de áudio
    * @return true se os metadados foram carregados com sucesso, false caso
    * contrário
    * @note Futuramente será implementado com a biblioteca TagLib
    */
    bool loadMetadata();

    /**
    * @brief Obtém a duração formatada
    * @return String com a duração no formato "MM:SS"
    */
    std::string getFormattedDuration() const;

    /**
    * @brief Converte a música para string
    * @return String com todas as informações da música formatadas
    */
    std::string toString() const;

    // Operações no banco de dados será responsabilidade da classe? não
    };

}
