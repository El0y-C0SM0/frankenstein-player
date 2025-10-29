/**
 * @file test_PlaybackQueue.cpp
 * @brief Testes unitários para a classe PlaybackQueue
 * @author João Tavares
 * @date 2025-10-27
 */

#include <doctest/doctest.h>

#include "core/include/core/entities/Playlist.hpp"
#include "core/include/core/entities/Song.hpp"
#include "core/include/core/entities/User.hpp"
#include "core/include/core/interfaces/IPlayable.hpp"
#include "core/include/core/services/PlaybackQueue.hpp"
#include "fixture/PlaybackQueueFixture.hpp"
#include "mocks/MockPlayable.hpp"

#include <memory>
#include <vector>

// CONSTRUTORES
TEST_CASE_FIXTURE(PlaybackQueueFixture,
                  "PlaybackQueue - Construtor sem músicas cria fila vazia") {
  auto queue(user, history_repo);

  CHECK(queue.empty());
  CHECK(queue.size() == 0);
  CHECK(queue.getCurrentSong() == nullptr);
  CHECK_FALSE(queue.isAleatory());
}

TEST_CASE_FIXTURE(PlaybackQueueFixture,
                  "PlaybackQueue - Construtor com IPlayable preenche a fila") {
  std::vector<std::shared_ptr<core::Song>> initial_songs = {
      createSong("Song A"), createSong("Song B"), createSong("Song C")};
  MockPlayable playable(initial_songs);

  core::PlaybackQueue queue(user, playable, history_repo);

  CHECK_FALSE(queue.empty());
  CHECK(queue.size() == 3);
  CHECK(queue.getCurrentSong() != nullptr);
  CHECK(queue.getCurrentSong()->getTitle() == "Song A");
}

// TESTES DE ADIÇÃO
TEST_CASE_FIXTURE(PlaybackQueueFixture,
                  "PlaybackQueue - Adicionar músicas via add()") {
  core::PlaybackQueue queue(user, history_repo);

  std::vector<std::shared_ptr<core::Song>> songs_to_add = {
      createSong("New Song 1"), createSong("New Song 2")};
  MockPlayable playable(songs_to_add);

  queue.add(playable);

  CHECK(queue.size() == 2);
  CHECK(queue.getCurrentSong()->getTitle() == "New Song 1");
}

TEST_CASE_FIXTURE(PlaybackQueueFixture,
                  "PlaybackQueue - Operador += adiciona músicas") {
  core::PlaybackQueue queue(user, history_repo);

  std::vector<std::shared_ptr<core::Song>> songs_to_add = {
      createSong("Song X"), createSong("Song Y")};
  MockPlayable playable(songs_to_add);

  queue += playable;

  CHECK(queue.size() == 2);
  CHECK(queue.at(1)->getTitle() == "Song Y");
}

// TESTES DE NAVEGAÇÃO
TEST_CASE_FIXTURE(PlaybackQueueFixture,
                  "PlaybackQueue - Navegação entre músicas") {
  std::vector<std::shared_ptr<core::Song>> songs = {
      createSong("First"), createSong("Second"), createSong("Third")};
  MockPlayable playable(songs);
  core::PlaybackQueue queue(user, playable, history_repo);

  SUBCASE("Música inicial") {
    CHECK(queue.getCurrentSong()->getTitle() == "First");
    CHECK(queue.findCurrentIndex() == 0);
  }

  SUBCASE("Avançar para próxima música") {
    auto next = queue.next();
    CHECK(next->getTitle() == "Second");
    CHECK(queue.findCurrentIndex() == 1);
  }

  SUBCASE("Operador ++") {
    auto next = ++queue;
    CHECK(next->getTitle() == "Second");
  }

  SUBCASE("Voltar para música anterior após avançar") {
    queue.next(); // Vai para Second
    queue.next(); // Vai para Third

    auto previous = queue.previous();
    CHECK(previous->getTitle() == "Second");
    CHECK(queue.findCurrentIndex() == 1);
  }
}

// TESTES DE REMOÇÃO
TEST_CASE_FIXTURE(PlaybackQueueFixture,
                  "PlaybackQueue - Remover músicas da fila") {
  std::vector<std::shared_ptr<core::Song>> songs = {
      createSong("Song 1"), createSong("Song 2"), createSong("Song 3")};
  MockPlayable playable(songs);
  core::PlaybackQueue queue(user, playable, history_repo);

  SUBCASE("Remover índice válido") {
    CHECK(queue.remove(1) == true);
    CHECK(queue.size() == 2);
    CHECK(queue.at(0)->getTitle() == "Song 1");
    CHECK(queue.at(1)->getTitle() == "Song 3");
  }

  SUBCASE("Remover índice inválido") {
    CHECK(queue.remove(5) == false);
    CHECK(queue.size() == 3);
  }
}

// TESTES DE BUSCA
TEST_CASE_FIXTURE(PlaybackQueueFixture,
                  "PlaybackQueue - Buscar músicas na fila") {
  auto song1 = createSong("Find Me");
  auto song2 = createSong("Another Song");
  auto song3 = createSong("Find Me");

  std::vector<std::shared_ptr<core::Song>> songs = {song1, song2, song3};
  MockPlayable playable(songs);
  core::PlaybackQueue queue(user, playable, history_repo);

  SUBCASE("Encontrar próxima ocorrência") {
    CHECK(queue.findNextIndex(*song1) == 0);
    CHECK(queue.findNextIndex(*song3) == 2);
  }

  SUBCASE("Encontrar índice atual após navegação") {
    queue.next();
    CHECK(queue.findCurrentIndex() == 1);
  }
  SUBCASE("Busca por index não existente") { CHECK(queue.at(99) == nullptr); }
}

// TESTES DE VISUALIZAÇÃO
TEST_CASE_FIXTURE(PlaybackQueueFixture,
                  "PlaybackQueue - Visualizações da fila") {
  std::vector<std::shared_ptr<core::Song>> songs;
  for (int i = 1; i <= 5; i++) {
    songs.push_back(createSong("Song " + std::to_string(i)));
  }
  MockPlayable playable(songs);
  core::PlaybackQueue queue(user, playable, history_repo);

  SUBCASE("Visualização ao redor da música atual") {
    queue.next();

    auto view = queue.getQueueView(1, 2);
    CHECK(view.size() == 4);
    CHECK(view[0]->getTitle() == "Song 1");
    CHECK(view[1]->getTitle() == "Song 2");
    CHECK(view[2]->getTitle() == "Song 3");
    CHECK(view[3]->getTitle() == "Song 4");
  }
  SUBCASE("Segmento maior que a queue") {
    queue.next();
    auto view = queue.getQueueView(1, 6);
    CHECK(view.size() == 5);
  }

  SUBCASE("Segmento da fila") {
    auto segment = queue.getQueueSegment(1, 3);
    CHECK(segment.size() == 3);
    CHECK(segment[0]->getTitle() == "Song 2");
    CHECK(segment[1]->getTitle() == "Song 3");
    CHECK(segment[2]->getTitle() == "Song 4");
  }
}

// TESTES DE MODO ALEATÓRIO
TEST_CASE_FIXTURE(PlaybackQueueFixture,
                  "PlaybackQueue - Modo aleatório e embaralhamento") {
  std::vector<std::shared_ptr<core::Song>> songs = {
      createSong("A"), createSong("B"), createSong("C")};
  MockPlayable playable(songs);
  core::PlaybackQueue queue(user, playable, history_repo);

  SUBCASE("Ativar e desativar modo aleatório") {
    queue.setAleatory(true);
    CHECK(queue.isAleatory() == true);

    queue.setAleatory(false);
    CHECK(queue.isAleatory() == false);
  }

  SUBCASE("Embaralhar mantém todas as músicas") {
    queue.shuffle();
    CHECK(queue.size() == 3);
    // A ordem pode mudar, mas todas as músicas devem estar presentes
  }
}

// TESTES DE LIMPEZA
TEST_CASE_FIXTURE(PlaybackQueueFixture, "PlaybackQueue - Limpar fila") {
  std::vector<std::shared_ptr<core::Song>> songs = {createSong("Song 1"),
                                                    createSong("Song 2")};
  std::vector<std::shared_ptr<core::Song>> emptySong = {};

  SUBCASE("Happy path") {
    MockPlayable playable(songs);

    core::PlaybackQueue queue(user, playable, history_repo);

    CHECK(queue.size() == 2);
    queue.clear();
    CHECK(queue.empty());
    CHECK(queue.size() == 0);
    CHECK(queue.getCurrentSong() == nullptr);
  }
  SUBCASE("Playable vazio") {
    MockPlayable emptyPlayable(emptySong);
    core::PlaybackQueue queue(user, emptyPlayable, history_repo);
    CHECK_FALSE(queue.empty());
  }
}
