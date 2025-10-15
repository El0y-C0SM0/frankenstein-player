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

#include "Song.hpp"

namespace core {

/**
 * @class Album
 * @brief Representa um álbum musical com suas músicas
 *
 * A classe Album armazena informações como nome, artista, gênero e
 * a lista de músicas contidas no álbum. Mantém a ordem original
 * das faixas para reprodução sequencial.
 */
class Album : public IPlayable, ICollection {
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
  std::function<std::vector<std::shared_ptr<Artist>>()> featuringArtistsLoader;

public:
  /**
   * @brief Construtor da classe Album
   * @param name Nome do álbum
   * @param artist Nome do artista do álbum
   * @param genre Gênero do álbum
   */
  Album(const std::string name, const std::string artist,
        const std::string genre);

  /**
   * @brief Construtor completo da classe Album
   * @param id Identificador único
   * @param name Nome do álbum
   * @param artist Nome do artista
   * @param genre Gênero musical
   * @param year Ano de lançamento
   */
  Album(unsigned id, const std::string name, const std::string artist,
        const std::string genre, int year);
  /**
   * @brief Destrutor padrão da clase Album
   * @note Responsável por liberar a memória dos vetores alocados
   */
  ~Album();

  // Getters

  /**
   * @brief Obtém o ID do álbum
   * @return Identificador único do álbum
   */
  unsigned getId() const;

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
   * @return Vetor de ponteiros compartilhados para os artistas colaboradores
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
  void setArtist(const std::string &artist);

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
   * @param loader Função que retorna um ponteiro compartilhado para o artista
   */
  void setArtistLoader(const std::function<std::shared_ptr<Artist>()> &loader);

  /**
   * @brief Define a função para carregar os artistas colaboradores do álbum
   * @param loader Função que retorna um vetor de ponteiros compartilhados para
   * os artistas colaboradores
   */
  void setFeaturingArtistsLoader(
      const std::function<std::vector<std::shared_ptr<Artist>>()> &loader);

  /**
   * @brief Obtém informações do álbum em formato de string
   * @return String com nome, artista, ano e quantidade de músicas
   */
  std::string toString() const;

};

} // namespace core

#pragma once
