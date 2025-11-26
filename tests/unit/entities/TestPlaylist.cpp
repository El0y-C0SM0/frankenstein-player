/**
 * @file TestPlaylist.cpp
 * @brief Testes unitários para a classe Playlist
 * @author Bruno Vieira
 * @date 2025-10-27
 */

#include <doctest/doctest.h>

#include "core/entities/Playlist.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/User.hpp"

#include <memory>
#include <string>

TEST_SUITE("Unit Tests - Entity: Playlist") {

    TEST_CASE("Playlist: Construtores") {

        SUBCASE("Construtor padrão") {
            core::Playlist p;

            CHECK(p.getTitulo().empty());
            CHECK(p.getSongs().empty());
            CHECK(p.getUser() == nullptr);
        }

        SUBCASE("Construtor com id e título") {
            core::Playlist p(10, "Rock Classics");

            CHECK(p.getTitle() == "Rock Classics");
            CHECK(p.getTitulo() == "Rock Classics");
            CHECK(p.getSongs().empty());
        }
    }

    TEST_CASE("Playlist: setTitulo e getTitulo funcionam corretamente") {

        core::Playlist p;

        SUBCASE("Definindo e obtendo o título") {
            p.setTitulo("Chill Vibes");
            CHECK(p.getTitulo() == "Chill Vibes");
            CHECK(p.getTitle() == "Chill Vibes");
        }

        SUBCASE("Alterando título após definir") {
            p.setTitulo("Workout");
            p.setTitulo("Focus Mode");
            CHECK(p.getTitulo() == "Focus Mode");
        }
    }

    TEST_CASE("Playlist: adicionar e acessar músicas (integração simples)") {

        core::Playlist p;

        auto art = std::make_shared<core::Artist>("A", "Pop");
        auto alb = std::make_shared<core::Album>();

        auto s1 = std::make_shared<core::Song>("S1", art, alb);
        auto s2 = std::make_shared<core::Song>("S2", art, alb);
        auto s3 = std::make_shared<core::Song>("S3", art, alb);

        SUBCASE("Adicionar músicas com addSong()") {
            p.addSong(*s1);
            p.addSong(*s2);

            auto songs = p.getSongs();
            CHECK(songs.size() == 2);
            CHECK(songs[0]->getTitle() == "S1");
            CHECK(songs[1]->getTitle() == "S2");
        }

        SUBCASE("Encontrar música por ID e Título") {
            s1->setDuration(100);
            s2->setDuration(200);
            s3->setDuration(300);

            p.addSong(*s1);
            p.addSong(*s2);
            p.addSong(*s3);

            CHECK(p.findSongByTitle("S1")->getDuration() == 100);
            CHECK(p.findSongByTitle("S3")->getDuration() == 300);
            CHECK(p.findSongByTitle("NOPE") == nullptr);
        }

        SUBCASE("switchSong troca posições corretamente") {
            p.addSong(*s1);
            p.addSong(*s2);
            p.addSong(*s3);

            CHECK(p.switchSong(s3->getId(), 0) == true);

            auto songs = p.getSongs();
            CHECK(songs[0]->getTitle() == "S3");
            CHECK(songs[1]->getTitle() == "S1");
            CHECK(songs[2]->getTitle() == "S2");
        }

        SUBCASE("removeSong remove corretamente") {
            p.addSong(*s1);
            p.addSong(*s2);

            CHECK(p.removeSong(s1->getId()) == true);
            CHECK(p.getSongs().size() == 1);

            CHECK(p.removeSong(999) == false);
        }

        SUBCASE("getNextSong e getPreviousSong") {
            p.addSong(*s1);
            p.addSong(*s2);
            p.addSong(*s3);

            CHECK(p.getNextSong(*s1)->getTitle() == "S2");
            CHECK(p.getNextSong(*s2)->getTitle() == "S3");
            CHECK(p.getNextSong(*s3) == nullptr);

            CHECK(p.getPreviousSong(*s3)->getTitle() == "S2");
            CHECK(p.getPreviousSong(*s1) == nullptr);
        }

        SUBCASE("getSongAt retorna corretamente ou nullptr") {
            p.addSong(*s1);
            p.addSong(*s2);

            CHECK(p.getSongAt(0)->getTitle() == "S1");
            CHECK(p.getSongAt(1)->getTitle() == "S2");
            CHECK(p.getSongAt(5) == nullptr);
        }

        SUBCASE("calculateTotalDuration soma durações") {
            s1->setDuration(120);
            s2->setDuration(30);
            s3->setDuration(50);

            p.addSong(*s1);
            p.addSong(*s2);
            p.addSong(*s3);

            CHECK(p.calculateTotalDuration() == 200);
            CHECK(p.getFormattedDuration() == "03:20");
        }

        SUBCASE("Loader de músicas é chamado apenas quando necessário") {
            bool loaderCalled = false;

            p.setSongsLoader([&]() {
                loaderCalled = true;
                return std::vector<std::shared_ptr<core::Song>>{s1, s2};
            });

            CHECK(loaderCalled == false);

            auto songs = p.getSongs();
            CHECK(loaderCalled == true);
            CHECK(songs.size() == 2);
        }
    }

    TEST_CASE("Playlist: Construtor e Destrutor") {

        SUBCASE("Criação e destruição não causam leaks ou crashes") {
            core::Playlist* p = new core::Playlist();
            p->setTitulo("Temporary Playlist");

            auto artist = std::make_shared<core::Artist>("TmpA", "Rock");
            auto album = std::make_shared<core::Album>();
            auto song = std::make_shared<core::Song>("TempSong", artist, album);

            p->addSong(*song);

            CHECK(p->getSongs().size() == 1);

            delete p;
        }

        SUBCASE("Playlist com loader no destrutor") {
            core::Playlist* p = new core::Playlist();

            p->setSongsLoader([]() {
                return std::vector<std::shared_ptr<core::Song>>{
                    std::make_shared<core::Song>()
                };
            });

            CHECK(p->getSongs().size() == 1);

            delete p;
        }
    }
}
