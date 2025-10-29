#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "core/bd/HistoryPlaybackRepository.hpp"
#include "core/include/core/entities/Song.hpp"
#include "core/include/core/entities/User.hpp"
#include "core/include/core/services/PlaybackQueue.hpp"
#include "mocks/MockPlayable.hpp"

#include <memory>
#include <vector>

TEST_CASE("ACEITAÇÃO: Usuário ouvindo e editando fila de músicas") {

  auto user = std::make_shared<core::User>("usuario_teste");
  auto history_repo = std::make_shared<core::HistoryPlaybackRepository>();

  std::vector<std::shared_ptr<core::Song>> musicas;
  for (int i = 1; i <= 5; i++) {
    auto musica = std::make_shared<core::Song>();
    musica->setTitle("Música " + std::to_string(i));
    musica->setDuration(180 + i * 30);
    musicas.push_back(musica);
  }

  MockPlayable playlist(musicas);
  core::PlaybackQueue fila(user, playlist, history_repo);

  SUBCASE("Cenário 1: Avançar na fila de reprodução") {
    CHECK(fila.getCurrentSong()->getTitle() == "Música 1");

    auto proxima = fila.next();

    CHECK(proxima->getTitle() == "Música 2");
    CHECK(fila.getCurrentSong()->getTitle() == "Música 2");
  }

  SUBCASE("Cenário 2: Retroceder na fila de reprodução") {
    fila.next(); // Música 2
    fila.next(); // Música 3
    CHECK(fila.getCurrentSong()->getTitle() == "Música 3");

    auto anterior = fila.previous();

    CHECK(anterior->getTitle() == "Música 2");
    CHECK(fila.getCurrentSong()->getTitle() == "Música 2");
  }

  SUBCASE("Cenário 3: Controlar ordem de reprodução - Reordenar fila") {
    CHECK(fila.at(0)->getTitle() == "Música 1");
    CHECK(fila.at(1)->getTitle() == "Música 2");
    CHECK(fila.at(2)->getTitle() == "Música 3");

    fila.setAleatory(true);
    fila.shuffle();

    CHECK(fila.size() == 5);
    CHECK(fila.isAleatory() == true);
  }

  SUBCASE("Cenário 4: Colocar fila em loop") {
    // Vai até a última música
    while (fila.next() != nullptr) {
    }
    CHECK(fila.getCurrentSong()->getTitle() == "Música 5");

    fila.setLoop(true);
    auto depoisDaUltima = fila.next();

    CHECK(depoisDaUltima->getTitle() == "Música 1");
    CHECK(fila.getCurrentSong()->getTitle() == "Música 1");
  }

  SUBCASE("Cenário 5: Editar fila - Remover música atual") {
    fila.next(); // Música 2
    CHECK(fila.getCurrentSong()->getTitle() == "Música 2");

    bool removido = fila.remove(fila.findCurrentIndex());

    CHECK(removido == true);
    CHECK(fila.size() == 4);
    CHECK(fila.getCurrentSong()->getTitle() == "Música 3");
  }

  SUBCASE("Cenário 6: Adicionar novas músicas à fila existente") {
    CHECK(fila.size() == 5);

    std::vector<std::shared_ptr<core::Song>> novasMusicas;
    auto nova1 = std::make_shared<core::Song>();
    nova1->setTitle("Nova Música 1");
    auto nova2 = std::make_shared<core::Song>();
    nova2->setTitle("Nova Música 2");
    novasMusicas.push_back(nova1);
    novasMusicas.push_back(nova2);

    MockPlayable novas(novasMusicas);
    fila.add(novas);

    CHECK(fila.size() == 7);
    CHECK(fila.at(5)->getTitle() == "Nova Música 1");
    CHECK(fila.at(6)->getTitle() == "Nova Música 2");
  }
}
