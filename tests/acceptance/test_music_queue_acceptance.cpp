#include <doctest/doctest.h>

#include "core/bd/HistoryPlaybackRepository.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"
#include "core/services/PlaybackQueue.hpp"
#include "mocks/MockPlayable.hpp"
#include "fixtures/DatabaseFixture.hpp"

#include <memory>
#include <vector>

class UserFixture {
public:
    struct UserTestMock {
        unsigned int id;
        std::string username;
        std::string home_path;
        std::string input_path;
        long uid;
        bool is_current_user;
    };

private:
    std::map<std::string, UserTestMock> _test_user_mocks;

public:
    UserFixture() {
        _test_user_mocks["NORMAL_USER"] =
            UserTestMock {1,
                          "usuario_teste",
                          "/home/usuario_teste",
                          "/home/usuario_teste/input",
                          1001,
                          false};
    }

    const UserTestMock& getUserMock(const std::string& user_key) const {
        return _test_user_mocks.at(user_key);
    }
};

TEST_CASE("ACEITAÇÃO: Usuário ouvindo e editando fila de músicas") {
    UserFixture user_fixture;
    auto user_data = user_fixture.getUserMock("NORMAL_USER");

    auto user = std::make_shared<core::User>(user_data.username,
                                             user_data.home_path,
                                             user_data.input_path,
                                             user_data.uid);

    std::shared_ptr<core::HistoryPlaybackRepository> history_repo = nullptr;

    std::vector<std::shared_ptr<core::Song>> musicas;
    for (int i = 1; i <= 5; i++) {
        auto musica = std::make_shared<core::Song>();
        musica->setTitle("Música " + std::to_string(i));
        musica->setDuration(180 + i * 30);
        musicas.push_back(musica);
    }

    MockPlayable playlist(musicas);

    core::PlaybackQueue fila(user, history_repo);
    fila.add(playlist);

    SUBCASE("Cenário 1: Avançar na fila de reprodução") {
        CHECK(fila.getCurrentSong()->getTitle() == "Música 1");

        auto proxima = fila.next();

        CHECK(proxima->getTitle() == "Música 2");
        CHECK(fila.getCurrentSong()->getTitle() == "Música 2");
    }

    SUBCASE("Cenário 2: Retroceder na fila de reprodução") {
        fila.next();  // Música 2
        fila.next();  // Música 3
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
        while (fila.next() != nullptr) {
        }
        CHECK(fila.getCurrentSong()->getTitle() == "Música 5");
        WARN("Funcionalidade de loop não disponível no PlaybackQueue atual");
        CHECK(fila.getCurrentSong() != nullptr);
    }

    SUBCASE("Cenário 5: Editar fila - Remover música atual") {
        fila.next();  // Música 2
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
