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

#ifndef ARTIST_HPP
#define ARTIST_HPP

#include "Album.hpp"
#include "Song.hpp"
#include <string>
#include <vector>

/**
 * @class Artist
 * @brief Representa um artista musical com suas músicas e álbuns
 *
 * A classe Artist gerencia as informações básicas de um artista e
 * mantém referências para todas as suas músicas e álbuns associados.
 * Permite operações de busca, adição e remoção de itens do catálogo.
 */
class Artist {
private:
  unsigned _id;
  std::string _name;
  std::string _genre;
  std::vector<Song> *_songs;
  std::vector<Album> *_albums;

public:
  /**
   * @brief Construtor da classe Artist
   * @param name Nome do artista
   * @param genre Gênero musical do artista
   */
  Artist(const std::string &name, std::string &genre);

  /**
   * @brief Destrutor da classe Artist
   * @note Responsável por liberar a memória dos vetores alocados
   */
  ~Artist();

  /**
   * @brief Obtém o ID do artista
   * @return Identificador único do artista
   */
  unsigned getId() const;

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
  std::vector<Song> getSongs() const;

  /**
   * @brief Obtém todos os álbuns do artista
   * @return Vector com todos os álbuns do artista
   */
  std::vector<Album> getAlbums() const;

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
  void setSong(const Song &song);

  /**
   * @brief Adiciona um álbum ao artista
   * @param album Álbum a ser adicionado
   */
  void setAlbum(const Album &album);

  /**
   * @brief Remove uma música do artista
   * @param songId ID da música a ser removida
   * @return true se a música foi removida com sucesso, false caso contrário
   */
  bool removeSong(unsigned songId);

  /**
   * @brief Remove um álbum do artista
   * @param albumId ID do álbum a ser removido
   * @return true se o álbum foi removido com sucesso, false caso contrário
   */
  bool removeAlbum(unsigned albumId);

  /**
   * @brief Busca uma música pelo nome
   * @param songName Nome da música a buscar
   * @return Ponteiro para a música encontrada ou nullptr se não encontrada
   */
  const Song *findSongByName(const std::string &songName);

  /**
   * @brief Busca um álbum pelo nome
   * @param albumName Nome do álbum a buscar
   * @return Ponteiro para o álbum encontrado ou nullptr se não encontrado
   */
  const Album *findAlbumByName(const std::string &albumName);

  /**
   * @brief Calcula a duração total de todas as músicas
   * @return Duração total em segundos
   */
  int getTotalDuration() const;

  /**
   * @brief Obtém a duração total formatada
   * @return String com duração no formato "HH:MM:SS" ou "MM:SS"
   */
  std::string getFormattedDuration() const;

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
};

#endif // ARTIST_HPP
