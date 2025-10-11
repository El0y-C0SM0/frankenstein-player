/**
 * @file Player.hpp
 * @brief Classe responsável pelo controle básico de reprodução de músicas
 *
 * Fornece funcionalidades essenciais para reprodução de músicas individuais
 * e playlists, com controles de play, pause, next, previous e volume.
 *
 * @author Joao Tavares
 * @date 2025-11-09
 */

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Song.hpp"
#include <vector>

/**
 * @class Player
 * @brief Controlador de reprodução de áudio com funcionalidades básicas
 *
 * Gerencia uma playlist de músicas e fornece controles essenciais para
 * reprodução sequencial. Esta é uma versão inicial que será expandida
 * posteriormente para suportar a interface IPlayable.
 */
class Player {
private:
  std::vector<Song> _playlist;
  Song *_currentSong;
  int _currentIndex;
  bool _isPlaying;
  bool _isStopped;
  float _volume;

public:
  /**
   * @brief Construtor da classe Player
   *
   * Inicializa o player com estado parado e volume máximo.
   */
  Player();

  /**
   * @brief Reproduz uma música específica
   * @param song Música a ser reproduzida
   */
  void play(const Song &song);

  /**
   * @brief Reproduz uma lista de músicas
   * @param songs Vector de músicas a serem reproduzidas em sequência
   */
  void play(const std::vector<Song> &songs);

  /**
   * @brief Pausa a reprodução atual
   *
   * Mantém a música atual carregada mas para a reprodução.
   */
  void pause();

  /**
   * @brief Retoma a reprodução pausada
   *
   * Continua a reprodução da música atual de onde parou.
   */
  void resume();

  /**
   * @brief Avança para a próxima música na playlist
   *
   * Se estiver na última música, pode parar ou voltar para o início
   * dependendo da implementação.
   */
  void next();

  /**
   * @brief Volta para a música anterior na playlist
   *
   * Se estiver na primeira música, pode parar ou ir para a última
   * dependendo da implementação.
   */
  void previous();

  /**
   * @brief Define o nível de volume do player
   * @param volume Novo nível de volume entre 0.0 (mudo) e 1.0 (máximo)
   */
  void setVolume(float &volume);

  /**
   * @brief Obtém o nível de volume atual
   * @return Volume atual entre 0.0 e 1.0
   */
  float getVolume() const;

  /**
   * @brief Silencia o player
   *
   * Define o volume para 0.0 sem perder a configuração anterior.
   */
  void mute();

  /**
   * @brief Restaura o volume anterior
   *
   * Retorna o volume para o nível definido antes do mute.
   */
  void unmute();

  /**
   * @brief Obtém a música atualmente em reprodução
   * @return Música atual ou música inválida se não houver reprodução
   */
  Song getCurrentSong() const;

  /**
   * @brief Verifica se o player está reproduzindo
   * @return true se está reproduzindo, false se pausado ou parado
   */
  bool isPlaying() const;

  /**
   * @brief Verifica se o player está pausado
   * @return true se está pausado, false se reproduzindo ou parado
   */
  bool isPaused() const;

  /**
   * @brief Obtém o progresso atual da reprodução
   * @return Progresso entre 0.0 (início) e 1.0 (fim) da música atual
   */
  float getProgress() const;

  /**
   * @brief Obtém o tamanho da playlist atual
   * @return Número de músicas na playlist
   */
  int getPlaylistSize() const;

  /**
   * @brief Limpa toda a playlist
   *
   * Remove todas as músicas da playlist e para a reprodução atual.
   */
  void clearPlaylist();

  /**
   * @brief Obtém uma cópia da playlist atual
   * @return Vector com todas as músicas da playlist
   */
  std::vector<Song> getPlaylist() const;

  /**
   * @brief Avança para a próxima música na sequência
   * @internal Método interno para navegação
   */
  void advanceToNext();

  /**
   * @brief Retrocede para a música anterior na sequência
   * @internal Método interno para navegação
   */
  void goToPrevious();

  /**
   * @brief Verifica se existe próxima música na playlist
   * @return true se há próxima música, false caso contrário
   * @internal Método interno para navegação
   */
  bool hasNext() const;

  /**
   * @brief Verifica se existe música anterior na playlist
   * @return true se há música anterior, false caso contrário
   * @internal Método interno para navegação
   */
  bool hasPrevious() const;

  /**
   * @brief Carrega o áudio de uma música para reprodução
   * @param song Música a ser carregada
   * @internal Método interno para carregamento de áudio
   */
  void loadAudio(const Song &song);
};

#endif // PLAYER_HPP
