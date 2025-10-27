#include <doctest/doctest.h>

#include "core/entities/Song.hpp"
#include "core/entities/Album.hpp"
#include "core/entities/Artist.hpp"
#include "core/services/Player.hpp"

#include "../tests/fixtures/ConfigFixture.hpp" // TODO corrigir include
#include "../tests/fixtures/MediaFixture.hpp"


TEST_SUITE("HISTÓRIA DE USUÁRIO: Reprodução de Música") {
    ConfigFixture config = ConfigFixture();
    MediaFixture media = MediaFixture();

    // TODO definir usuário do artista/musicas/album
    core::Artist artist(media.getArtistTestName(), "Test");
    core::Song short_song(1, media.getShortTestSongFileName(), "Short Testing Song", artist.getName());
    core::Song medium_song(1, media.getMediumTestSongFileName(), "Medium Testing Song", artist.getName());
    core::Album album(media.getAlbumTestName(), artist.getName(), artist.getGenre());

    album.setSongsLoader([&short_song]() {
        return std::vector<std::shared_ptr<core::Song>>{std::make_shared<core::Song>(short_song)};
    });

    artist.setSongsLoader([&short_song]() {
        return std::vector<std::shared_ptr<core::Song>>{std::make_shared<core::Song>(short_song)};
    });

    TEST_CASE("CT-AC-01: Reproduzir uma música/album/artista com sucesso") {
        SUBCASE("Reproduzir música individualmente") {
            core::Player player;
            bool resultado = player.play(short_song);

            CHECK(resultado == true);
            CHECK(player.isPlaying() == true);

            std::shared_ptr<core::IPlayableObject> cur_song = player.getCurrentSong();
            CHECK(cur_song != nullptr);
            CHECK(*cur_song == short_song);

            player.pause();
        }

        SUBCASE("Reproduzir um álbum") {
            core::Player player;
            bool resultado = player.play(album);

            CHECK(resultado == true);
            CHECK(player.isPlaying() == true);
            std::shared_ptr<core::IPlayableObject> cur_song = player.getCurrentSong();
            CHECK(cur_song != nullptr);
            CHECK(*cur_song == short_song);

            player.pause();
        }

        SUBCASE("Reproduzir um artista") {
            core::Player player;
            bool resultado = player.play(artist);

            CHECK(resultado == true);
            CHECK(player.isPlaying() == true);
            std::shared_ptr<core::IPlayableObject> cur_song = player.getCurrentSong();
            CHECK(cur_song != nullptr);
            CHECK(*cur_song == short_song);

            player.pause();
        }

        SUBCASE("Reproduzir nenhum aquivo") {
            core::Player player;
            bool resultado = player.play();

            CHECK(resultado == false);
            CHECK(player.isPlaying() == false);
            std::shared_ptr<core::IPlayableObject> cur_song = player.getCurrentSong();
            CHECK(cur_song == nullptr);
        }
    }

    TEST_CASE("CT-AC-02: Pausar a reprodução") {
        SUBCASE("Pause") {
            core::Player player;
            player.play(short_song);
            CHECK(player.isPlaying() == true);

            player.pause();
            CHECK(player.isPlaying() == false);
            CHECK(player.isPaused() == true);
        }

        SUBCASE("Pause Pause") {
            core::Player player;
            player.play(short_song);
            CHECK(player.isPlaying() == true);

            player.pause();
            CHECK(player.isPlaying() == false);
            CHECK(player.isPaused() == true);

            player.pause();
            CHECK(player.isPlaying() == false);
            CHECK(player.isPaused() == true);
        }

        SUBCASE("Pause Resume") {
            core::Player player;
            player.play(short_song);
            CHECK(player.isPlaying() == true);

            player.pause();
            CHECK(player.isPlaying() == false);
            CHECK(player.isPaused() == true);

            player.resume();
            CHECK(player.isPlaying() == true);
            CHECK(player.isPaused() == false);

            std::shared_ptr<core::IPlayableObject> cur_song = player.getCurrentSong();
            CHECK(cur_song != nullptr);
            CHECK(*cur_song == short_song);
        }
    }

    TEST_CASE("CT-AC-03: Manipular posição de reprodução") {

        SUBCASE("Avançar 2 segundos") {
            CHECK(true); // Placeholder
        }

        SUBCASE("Retroceder 2 segundos") {
            CHECK(true); // Placeholder
        }

        SUBCASE("Reiniciar música") {
            CHECK(true); // Placeholder
        }

        SUBCASE("Avançar valor inválido") {
            CHECK(true); // Placeholder
        }
    }
}
