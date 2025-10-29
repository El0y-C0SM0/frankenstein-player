/**
 * @file Playlist.hpp
 * @brief Definição de uma coleção de músicas escolhidas pelo usuário.
 *
 * Essa classe armazena um conjunto de músicas à gosto do usuário.
 *
 * @author Pedro Gabriel
 * @date 2025-10-13
 */

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "EntitiesFWD.hpp"
#include "Song.hpp"
#include "core/entities/Entity.hpp"
#include "core/interfaces/ICollection.hpp"
#include "core/interfaces/IPlayable.hpp"
#include "core/util/Datetime.hpp"

namespace core {
/**
 * @class Playlist
 * @brief Representa uma playlist de músicas
 *
 * Representa a entidade Playlist.
 *
 */
class Playlist : public Entity, public ICollection, public IPlayable {
private:
  std::string _titulo;
  unsigned _user_id;
  std::shared_ptr<User> _user;
  mutable std::vector<std::shared_ptr<Song>> _songs;

public:
  /**
   * @brief Construtor padrão da classe Playlist
   *
   */
  Playlist();

  /**
   * @brief Construtor de classe Playlist
   *
   * @param id Identificador único
   * @param titulo Título da Playlist
   *
   */
  Playlist(const unsigned id, const const std::string titulo);

  /**
   * @brief Destrutor padrão da classe Playlist
   * @note Libera a memória dos vetores alocados
   */
  ~Playlist();

  std::string getTitulo();

  void setTitulo(const std::string nome);

  // Outros métodos já estão específicados no ICollection.hpp

  /**
   * @brief Compara duas Playlist
   * @param other Playlist a ser comparada
   * @return true se as entidades forem iguais, false caso contrário
   */
  virtual bool operator==(const Entity &other) const override;

  /**
   * @brief Compara duas Playlists para desigualdade
   * @param other Playlist a ser comparada
   * @return true se as entidades forem diferentes, false caso contrário
   */
  virtual bool operator!=(const Entity &other) const override;
};
} // namespace core
