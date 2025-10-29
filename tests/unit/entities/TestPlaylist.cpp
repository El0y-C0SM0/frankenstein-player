/**
 * @file TestPlaylist.cpp
 * @brief Testes unitários para a classe Playlist
 * @author Bruno Vieira
 * @date 2025-10-27
 */

#include <doctest/doctest.h>

#include "core/entities/Playlist.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"

#include <memory>
#include <string>

TEST_SUITE("Unit Tests - Entity: Playlist")
{
    class PlaylistFixture : public PlaylistFixture
    {
    public:
        PlaylistFixture() : core::Playlist() {}

        PlaylistFixture(unsigned id, const std::string &title)
            : core::Playlist(id, title) {}

        ~PlaylistFixture() = default;
    };

    // TESTE 1: COnstrutores
    TEST_CASE("Playlist: Construtores")
    {
        SUBCASE("construtor padrao")
        {
            PlaylistFixture p;
            CHECK(p.getTitulo() == "");

            CHECK(p.getId() == 0);

            CHECK(p.calculateTotalDuration() == 0);

            CHECK(p.removeSong() == false);

            CHECK(p.findSongByTitle("teste") == nullptr);

            CHECK_THROWS_AS(p.getNextSong(*(new core::Song())), std::exception);

            CHECK_THROWS_AS(p.getPreviousSong(*(new core::Song())), std::exception);

            CHECK_THROWS_AS(p.getSongAt(), std::out_of_range);
        }

        SUBCASE("construtor com id e título")
        {
            unsigned testId = 123;
            std::string Title = "titulo da playlist";

            PlaylistFixture p(testId, testTitle);

            CHECK(p.getTitulo() == testTitle);

            CHECK(p.getId() == testId);

            CHECK(p.calculateTotalDuration() == 0);

            CHECK(p.removeSong() == false);

            CHECK(p.findSongByTitle("teste") == nullptr);

            CHECK_THROWS_AS(p.getNextSong(*(new core::Song())), std::exception);

            CHECK_THROWS_AS(p.getPreviousSong(*(new core::Song())), std::exception);

            CHECK_THROWS_AS(p.getSongAt(), std::out_of_range);
        }
    }

    // TESTE 3: getters e Setters
    TEST_CASE("Playlist: setTitulo e getTitulo funcionam corretamente")
    {
        PlaylistFixture p;
        REQUIRE(p.getTitulo() == "");

        std::string testTitle = "titulo teste";

        p.setTitulo(testTitle);

        CHECK(p.getTitulo() == testTitle);

        unsigned testId = 123;

        p.setId(testId);

        CHECK(p.getId() == testId);
    }

    // TESTE 4: Interação simples com músicas
    TEST_CASE("Playlist: adicionar e acessar músicas (integração simples)")
    {
        PlaylistFixture playlisTeste;

        SUBCASE("adicionar músicas")
        {
            core::Song musicaTeste1("Musica um");
            core::Song musicaTeste2("Musica dois");

            playlisTeste.addSong(musicaTeste1);
            playlisTeste.addSong(musicaTeste2);

            CHECK(p.size() == 2);

            CHECK(p.at(0)->getTitle() == "Musica um");

            CHECK(p.at(1)->getTitle() == "Musica dois");

            CHECK(p.findSongByTitle("Musica um") == musicaTeste1);

            CHECK(p.at(2)->getTitle() == "");
        }

        SUBCASE("alterar musica na posição válida")
        {
            CHECK(switchSong(2, 0) == true);
        }

        SUBCASE("alterar musica na posição inválida")
        {
            CHECK(switchSong(3, 0) == true);
        }
    }

    // TESTE 5: Contratos de destrutor
    TEST_CASE("Playlist: Contrutor e Destrutor")
    {
        {
            PlaylistFixture PLocal;
            PLocal.setTitulo("Temp");
            CHECK(PLocal.getTitulo() == "Temp");
        }
        CHECK(true);
    }
}