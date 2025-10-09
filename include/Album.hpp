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

#ifndef ALBUM_HPP
#define ALBUM_HPP

#include "Song.hpp"
#include <string>
#include <vector>

/**
 * @class Album
 * @brief Representa um álbum musical com suas músicas
 *
 * A classe Album armazena informações como nome, artista, gênero e
 * a lista de músicas contidas no álbum. Mantém a ordem original
 * das faixas para reprodução sequencial.
 */
class Album {
private:
  unsigned _id;
  std::string _name;
  std::string _artist;
  std::vector<Song> _songs;
  std::string _genre;
  int _year;

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

  // Getters
  //
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
   * @brief Obtém o nome do artista
   * @return Nome do artista/banda
   */
  std::string getArtist() const;

  /**
   * @brief Obtém a lista de músicas do álbum
   * @return Vector com as músicas do álbum em ordem
   */
  std::vector<Song> getSongs() const;

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
   * @brief Adiciona uma música ao álbum
   * @param song Música a ser adicionada
   */
  void addSong(const Song &song);

  /**
   * @brief Remove uma música do álbum
   * @param songId ID da música a ser removida
   * @return true se a música foi removida, false se não encontrada
   */
  bool removeSong(unsigned songId);

  /**
   * @brief Remove uma música do álbum por índice
   * @param index Índice da música a ser removida
   * @return true se a música foi removida, false se índice inválido
   */
  bool removeSongAt(int index);

  /**
   * @brief Busca uma música no álbum por ID
   * @param songId ID da música a buscar
   * @return Ponteiro para a música encontrada ou nullptr se não encontrada
   */
  const Song *findSongById(unsigned songId) const;

  /**
   * @brief Busca uma música no álbum por título
   * @param title Título da música a buscar
   * @return Ponteiro para a música encontrada ou nullptr se não encontrada
   */
  const Song *findSongByTitle(const std::string &title) const;

  // Metodos

  /**
   * @brief Calcula a duração total do álbum
   * @return Duração total em segundos
   */
  int calculateTotalDuration() const;

  /**
   * @brief Obtém a duração total formatada
   * @return String com duração no formato "HH:MM:SS" ou "MM:SS"
   */
  std::string getFormattedDuration() const;

  /**
   * @brief Obtém informações do álbum em formato de string
   * @return String com nome, artista, ano e quantidade de músicas
   */
  std::string toString() const;

  /**
   * @brief Obtém a próxima música na sequência do álbum
   * @param current Música atual
   * @return Próxima música ou a primeira se for a última
   */
  Song getNextSong(const Song &current) const;

  /**
   * @brief Obtém a música anterior na sequência do álbum
   * @param current Música atual
   * @return Música anterior ou a última se for a primeira
   */
  Song getPreviousSong(const Song &current) const;

  /**
   * @brief Obtém uma música por sua posição no álbum
   * @param index Índice da música (0-based)
   * @return Música na posição especificada
   * @throws std::out_of_range se índice inválido
   */
  Song getSongAt(int index) const;
};

#endif // ALBUM_HPP
