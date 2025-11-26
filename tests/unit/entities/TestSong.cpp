#include <doctest/doctest.h>

#include "core/entities/Song.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/Album.hpp"
#include "core/entities/User.hpp"

TEST_SUITE("Unit Tests - Entity: Song") {

    TEST_CASE("Song: construtor vazio e getters iniciais") {
        core::Song s;

        CHECK(s.getTitle().empty());
        CHECK(s.getDuration() == 0);
        CHECK(s.getTrackNumber() == 0);
        CHECK(s.getArtist() == nullptr);
        CHECK(s.getAlbum() == nullptr);
        CHECK(s.getUser() == nullptr);
        CHECK(s.getGenre().empty());
        CHECK(s.getYear() == 0);
    }

    TEST_CASE("Song: outros construtores ") {
        auto artist = std::make_shared<core::Artist>("Artist A", "Rock");
        auto album = std::make_shared<core::Album>();

        core::Song s("Song X", artist, album);

        CHECK(s.getTitle() == "Song X");
        CHECK(s.getArtist() == artist);
        CHECK(s.getAlbum() == album);

        core::Song s2(10, "/music/file.mp3", "FileSong", 77);

        CHECK(s2.getTitle() == "FileSong");
        CHECK(s2.getAudioFilePath() == "/music/file.mp3");

        unsigned artistId = 50;
        unsigned userId = 99;
        core::Song s2(1, "Title ABC", artistId, userId);

        CHECK(s2.getTitle() == "Title ABC");
        CHECK(s2.getArtist() == nullptr);
        CHECK(s2.getUser() == nullptr);

        core::Artist a("ArtZ", "Jazz");
        core::Album al;
        core::User u("John");

        core::Song s3("FullSong", a, al, u);

        CHECK(s3.getTitle() == "FullSong");
        CHECK(s3.getArtist() != nullptr);
        CHECK(s3.getAlbum() != nullptr);
        CHECK(s3.getUser() != nullptr);
        CHECK(s3.getUser()->getUsername() == "John");
    }

    TEST_CASE("Song: setters básicos") {
        core::Song s;

        s.setTitle("NewTitle");
        CHECK(s.getTitle() == "NewTitle");

        s.setGenre("Metal");
        CHECK(s.getGenre() == "Metal");

        s.setYear(2024);
        CHECK(s.getYear() == 2024);

        s.setDuration(320);
        CHECK(s.getDuration() == 320);
        CHECK(s.getFormattedDuration() == "05:20");

        s.setTrackNumber(7);
        CHECK(s.getTrackNumber() == 7);

        core::User u("Ana");
        s.setUser(u);
        CHECK(s.getUser() != nullptr);
        CHECK(s.getUser()->getUsername() == "Ana");
    }


    TEST_CASE("Song: featuring artists básico") {
        core::Song s;

        auto a1 = std::make_shared<core::Artist>("Feat1", "Rap");
        auto a2 = std::make_shared<core::Artist>("Feat2", "Rock");

        s.setFeaturingArtists(a1);
        s.setFeaturingArtists(a2);

        auto feats = s.getFeaturingArtists();
        CHECK(feats.size() == 2);
        CHECK(feats[0]->getName() == "Feat1");
        CHECK(feats[1]->getName() == "Feat2");

        auto ids = s.getFeaturingArtistsId();
        CHECK(ids.size() == 2);
    }

    TEST_CASE("Song: toString deve retornar algo não vazio") {
        core::Song s;
        s.setTitle("XYZ");
        CHECK_FALSE(s.toString().empty());
    }


    TEST_CASE("Song: operadores == e !=") {
        core::Song s1(10, "/a.mp3", "A", 1);
        core::Song s2(10, "/a.mp3", "A", 1);
        core::Song s3(11, "/b.mp3", "B", 2);

        CHECK(s1 == s2);
        CHECK(s1 != s3);
    }
}