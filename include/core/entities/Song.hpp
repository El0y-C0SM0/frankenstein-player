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

#include "core/interfaces/IPlayable.hpp"
#include "core/interfaces/IPlayableObject.hpp"
#include "core/entities/Entity.hpp"

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
  std::string _artist;
  std::string _album;
  int _duration;
  std::string _genre;
  int _year;
  bool _metadata_loaded;

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
  std::string getArtist() const;
  /**
   * @brief Obtém o álbum
   * @return Nome do álbum
   */
  std::string getAlbum() const;
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
   * @brief Define o artista
   * @param artist Novo artista
   */
  void setArtist(const std::string &artist);
  /**
   * @brief Define o álbum
   * @param album Novo álbum
   */
  void setAlbum(const std::string &album);
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

  // Operações no banco de dados será responsabilidade da classe?
};
