#include <doctest/doctest.h>

#include "core/entities/Album.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/User.hpp"

TEST_SUITE("Unit Tests - Entity: Album") {

    TEST_CASE("Album: construtor básico e getters") {
        core::Album album(1, "Meu Album", 2024, "Rock", 42);

        CHECK(album.getName() == "Meu Album");
        CHECK(album.getGenre() == "Rock");
        CHECK(album.getYear() == 2024);

        CHECK(album.getUser() == nullptr);
    }

    TEST_CASE("Album: construtor com artista") {
        auto artist = std::make_shared<core::Artist>(1, "Artista Teste", 1);
        core::Album album("Album X", artist, "Rock");

        CHECK(album.getName() == "Album X");
        CHECK(album.getGenre() == "Rock");
        CHECK(album.getArtist() == artist);
        CHECK(album.getYear() == 0);
    }

    TEST_CASE("Album: setters básicos funcionam") {
        core::Album album;
        std::string newGenre = "Jazz";
        int newYear = 1999;

        album.setGenre(newGenre);
        album.setYear(newYear);

        CHECK(album.getGenre() == "Jazz");
        CHECK(album.getYear() == 1999);

        auto artist = std::make_shared<core::Artist>(2, "Novo Artista", 1); // User Id podemos tirar caso de erro
        album.setArtist(artist);
        CHECK(album.getArtist() == artist);

        auto user = std::make_shared<core::User>("jorge");
        album.setUser(user);
        CHECK(album.getUser() == user);
    }

    TEST_CASE("Album: toString não deve ser vazio") {
        core::Album album(10, "TestAlbum", 2020, "Rock", 100);
        CHECK_FALSE(album.toString().empty());
    }

    TEST_CASE("Album: adicionar e contar músicas") {
        auto album = std::make_shared<core::Album>();
        CHECK(album->getSongCount() == 0);

        std::shared_ptr<core::Artist> nullArtist;
        core::Song s1("Musica 1", nullArtist, album);
        core::Song s2("Musica 2", nullArtist, album);

        album->addSong(s1);
        album->addSong(s2);

        CHECK(album->getSongCount() == 2);

        auto found = album->findSongById(2);
        CHECK(found != nullptr);
        CHECK(found->getTitle() == "Musica 2");
    }
}
