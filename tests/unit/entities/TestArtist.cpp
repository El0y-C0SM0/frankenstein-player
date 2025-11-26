#include <doctest/doctest.h>

#include "core/entities/Artist.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/Album.hpp"
#include "core/entities/User.hpp"

TEST_SUITE("Unit Tests - Entity: Artist") {

    TEST_CASE("Artist: construtores báscos") {
        core::Artist a1;
        CHECK(a1.getName().empty());
        CHECK(a1.getGenre().empty());
        CHECK(a1.getSongsCount() == 0);
        CHECK(a1.getAlbumsCount() == 0);

        core::Artist a2("Metallica", "Metal");
        CHECK(a2.getName() == "Metallica");
        CHECK(a2.getGenre() == "Metal");

        core::Artist a3(10, "Banda X", 99);
        CHECK(a3.getName() == "Banda X");
    }

    TEST_CASE("Artist: setters básicos") {
        core::Artist artist;

        artist.setName("Daft Punk");
        artist.setGenre("Electronic");

        CHECK(artist.getName() == "Daft Punk");
        CHECK(artist.getGenre() == "Electronic");

        core::User u("usuario_teste");
        artist.setUser(u);

        CHECK(artist.getUser() != nullptr);
        CHECK(artist.getUser()->getUsername() == "usuario_teste");
    }

    TEST_CASE("Artist: adicionar músicas") {
        core::Artist artist("Artist Z", "Pop");

        auto albumPtr = std::make_shared<core::Album>();
        std::shared_ptr<core::Artist> artistPtr = std::make_shared<core::Artist>(artist);

        std::shared_ptr<core::Song> s1 = std::make_shared<core::Song>("Song A", artistPtr, albumPtr);
        std::shared_ptr<core::Song> s2 = std::make_shared<core::Song>("Song B", artistPtr, albumPtr);

        artist.addSong(*s1);
        artist.addSong(*s2);

        CHECK(artist.getSongsCount() == 2);
        CHECK(artist.hasSong());

        auto songs = artist.getSongs();
        CHECK(songs.size() == 2);
    }

    TEST_CASE("Artist: encontrar música por titulo e ID") {
        core::Artist artist("TestArtist", "Indie");

        auto albumPtr = std::make_shared<core::Album>();
        std::shared_ptr<core::Artist> pArtist = std::make_shared<core::Artist>(artist);

        auto s1 = std::make_shared<core::Song>("Intro", pArtist, albumPtr);
        s1->setId(1);
        auto s2 = std::make_shared<core::Song>("Hit", pArtist, albumPtr);
        s2->setId(2);

        artist.addSong(*s1);
        artist.addSong(*s2);

        CHECK(artist.findSongByTitle("Hit")->getId() == 2);
        CHECK(artist.findSongById(1)->getTitle() == "Intro");
        CHECK(artist.findSongByTitle("DoesNotExist") == nullptr);
    }

    TEST_CASE("Artist: remoção de música") {
        core::Artist artist("Remoção", "Rock");

        auto albumPtr = std::make_shared<core::Album>();
        std::shared_ptr<core::Artist> artistPtr = std::make_shared<core::Artist>(artist);

        auto s1 = std::make_shared<core::Song>("S1", artistPtr, albumPtr);
        s1->setId(11);
        auto s2 = std::make_shared<core::Song>("S2", artistPtr, albumPtr);
        s2->setId(12);

        artist.addSong(*s1);
        artist.addSong(*s2);

        CHECK(artist.getSongsCount() == 2);

        CHECK(artist.removeSong(11) == true);
        CHECK(artist.getSongsCount() == 1);

        CHECK(artist.removeSong(99) == false);
        CHECK(artist.getSongsCount() == 1);
    }

    TEST_CASE("Artist: adicionar e remover álbuns") {
        core::Artist artist("AlbumTest", "Pop");

        core::Album a1(1, "Album1", 2020, "Pop", 10);
        core::Album a2(2, "Album2", 2021, "Pop", 10);

        artist.addAlbum(a1);
        artist.addAlbum(a2);

        CHECK(artist.getAlbumsCount() == 2);
        CHECK(artist.hasAlbum());

        CHECK(artist.findAlbumByName("Album2") != nullptr);
        CHECK(artist.findAlbumByName("NonExistent") == nullptr);

        CHECK(artist.removeAlbum(1) == true);
        CHECK(artist.getAlbumsCount() == 1);

        CHECK(artist.removeAlbum(999) == false);
    }

    TEST_CASE("Artist: calcula duração") {
        core::Artist artist("DurTest", "Chill");

        auto album = std::make_shared<core::Album>();
        std::shared_ptr<core::Artist> pArtist = std::make_shared<core::Artist>(artist);

        auto s1 = std::make_shared<core::Song>("Track 1", pArtist, album);
        s1->setDuration(90);
        s1->setId(1);

        auto s2 = std::make_shared<core::Song>("Track 2", pArtist, album);
        s2->setDuration(200);
        s2->setId(2);

        artist.addSong(*s1);
        artist.addSong(*s2);

        CHECK(artist.calculateTotalDuration() == 290);
        CHECK(artist.getTotalDuration() == 290);

        CHECK(artist.getFormattedDuration() == "04:50");
    }

    TEST_CASE("Artist: altera ordem") {
        core::Artist artist("Switch", "Test");

        auto albumPtr = std::make_shared<core::Album>();
        std::shared_ptr<core::Artist> pArtist = std::make_shared<core::Artist>(artist);

        auto s1 = std::make_shared<core::Song>("A", pArtist, albumPtr);
        auto s2 = std::make_shared<core::Song>("B", pArtist, albumPtr);
        auto s3 = std::make_shared<core::Song>("C", pArtist, albumPtr);

        s1->setId(1);
        s2->setId(2);
        s3->setId(3);

        artist.addSong(*s1);
        artist.addSong(*s2);
        artist.addSong(*s3);

        CHECK(artist.switchSong(3, 0) == true);
    }


    TEST_CASE("Artist: toString deve retornar algo não vazio") {
        core::Artist artist("ToStringArtist", "Jazz");

        CHECK_FALSE(artist.toString().empty());
    }

    TEST_CASE("Artist: playable objects deve retornar músicas") {
        core::Artist artist("PlayTest", "Rock");

        auto albumPtr = std::make_shared<core::Album>();
        auto pArtist = std::make_shared<core::Artist>(artist);

        auto s1 = std::make_shared<core::Song>("PX", pArtist, albumPtr);
        s1->setDuration(120);
        s1->setId(1);

        artist.addSong(*s1);

        auto objs = artist.getPlayableObjects();
        CHECK(objs.size() == 1);
    }

    TEST_CASE("Artist: operador de igualdade e desigualdade") {
        core::Artist a1(1, "A", 10);
        core::Artist a2(1, "A", 10);
        core::Artist a3(2, "B", 20);

        CHECK(a1 == a2);
        CHECK(a1 != a3);
    }
}
