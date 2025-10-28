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

#include "Album.hpp"
#include "Song.hpp"
#include "core/entities/User.hpp"
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
  std::vector<std::shared_ptr<Song>> _songs;
  std::vector<std::shared_ptr<Album>> _albums;
  std::shared_ptr<core::User> user;

  std::function<std::vector<std::shared_ptr<Song>>()> songsLoader;
  std::function<std::vector<std::shared_ptr<Album>>()> albumsLoader;

public:
  /**
   * @brief Construtor vazio
   */
  Artist();

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
  std::vector<std::shared_ptr<const Song>> getSongs() const;

  /**
   * @brief Obtém todos os álbuns do artista
   * @return Vector com todos os álbuns do artista
   */
  std::vector<std::shared_ptr<const Album>> getAlbums() const;

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
   * @brief Define a função para carregar as músicas do artista
   * @param loader Função que retorna um vetor de músicas
   */
  void setSongsLoader(
      const std::function<std::vector<std::shared_ptr<Song>>()> &loader);

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
} // namespace core
