#include <doctest/doctest.h>
#include <memory>
#include <string>

#include "core/entities/Album.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"

TEST_SUITE("Unit Tests - Entity: Album") {
    class FixtureCollectionAlbum {
    public:
        std::vector<std::shared_ptr<core::Song>> songs;
        core::Artist artist1, artist2;
        core::User user;

        FixtureCollectionAlbum() {
            songs.push_back(std::make_shared<core::Song>());
            songs.push_back(std::make_shared<core::Song>());
            songs.push_back(std::make_shared<core::Song>());
            songs.push_back(std::make_shared<core::Song>());
            songs.push_back(std::make_shared<core::Song>());

            user = core::User("user");
            artist1 = core::Artist(1, "Artist A", user);
            artist2 = core::Artist(2, "Artist B", user);
        }

        std::vector<std::shared_ptr<core::Song>> getSongs() {
            return songs;
        }

        std::shared_ptr<core::Artist> getArtist1() {
            return std::make_shared<core::Artist>(artist1);
        }

        std::shared_ptr<core::Artist> getArtist2() {
            return std::make_shared<core::Artist>(artist2);
        }
    };

    TEST_CASE_FIXTURE(FixtureCollectionAlbum, "Album: construtores e getters básicos") {
        SUBCASE("Construtor padrão") {
            core::Album emptyAlbum;
            CHECK(emptyAlbum.getTitle().empty());
        }

        SUBCASE("Construtor completo") {
            core::Album fullAlbum(1, "Meu Album", 2000, "Rock", this->artist1, this->user);
            fullAlbum.setArtistLoader([this]() { return this->getArtist1(); });
            CHECK_EQ(fullAlbum.getId(), 1);
            CHECK_EQ(fullAlbum.getTitle(), "Meu Album");
            CHECK_EQ(fullAlbum.getGenre(), "Rock");
            CHECK_EQ(fullAlbum.getYear(), 2000);
            CHECK(fullAlbum.getArtist() != nullptr);
            CHECK_EQ(fullAlbum.getArtist()->getName(), this->artist1.getName());
            CHECK(fullAlbum.getUser() != nullptr);
            CHECK_EQ(fullAlbum.getUser()->getUsername(), this->user.getUsername());
        }

        SUBCASE("Construto com titulo, genero e artista") {
            core::Album basicAlbum("Meu Album", "Rock", this->artist1);
            basicAlbum.setArtistLoader([this]() { return this->getArtist1(); });
            CHECK(basicAlbum.getTitle() == "Meu Album");
            CHECK(basicAlbum.getGenre() == "Rock");
            CHECK(basicAlbum.getArtist() != nullptr);
            CHECK(basicAlbum.getArtist()->getName() == this->artist1.getName());
        }

        SUBCASE("Construtor com id, titulo, genero e artista") {
            core::Album idAlbum(1, "Meu Album", 2000, "Rock", this->artist1);
            idAlbum.setArtistLoader([this]() { return this->getArtist1(); });
            CHECK_EQ(idAlbum.getId(), 1);
            CHECK_EQ(idAlbum.getTitle(), "Meu Album");
            CHECK_EQ(idAlbum.getGenre(), "Rock");
            CHECK_EQ(idAlbum.getYear(), 2000);
            CHECK(idAlbum.getArtist() != nullptr);
            CHECK_EQ(idAlbum.getArtist()->getName(), this->artist1.getName());
        }

        SUBCASE("Construtor de cópia") {
            core::Album rawAlbum(1, "Meu Album", 2000, "Rock", this->artist1);
            rawAlbum.setArtistLoader([this]() { return this->getArtist1(); });
            auto copyAlbum = rawAlbum;
            CHECK_EQ(copyAlbum.getId(), rawAlbum.getId());
            CHECK_EQ(copyAlbum.getTitle(), rawAlbum.getTitle());
            CHECK_EQ(copyAlbum.getGenre(), rawAlbum.getGenre());
            CHECK_EQ(copyAlbum.getYear(), rawAlbum.getYear());
            CHECK(copyAlbum.getArtist() != nullptr);
            CHECK_EQ(copyAlbum.getArtist()->getName(), rawAlbum.getArtist()->getName());
        }
    }

    TEST_CASE_FIXTURE(FixtureCollectionAlbum, "Album: setters básicos") {
        core::Album album;
        std::string newGenre = "Jazz";
        int newYear = 1999;

        album.setGenre(newGenre);
        album.setYear(newYear);

        CHECK_EQ(album.getGenre(), newGenre);
        CHECK_EQ(album.getYear(), newYear);

        album.setArtist(this->artist1);
        album.setArtistLoader([this]() { return this->getArtist1(); });
        CHECK_EQ(album.getArtist()->getName(), this->artist1.getName());

        album.setUser(this->user);
        CHECK_EQ(album.getUser()->getUsername(), this->user.getUsername());

        std::string newTitle = "Novo Título";
        CHECK_THROWS(album.setTitle(""));
        album.setTitle(newTitle);
        CHECK_EQ(album.getTitle(), newTitle);
    }

    TEST_CASE_FIXTURE(FixtureCollectionAlbum, "Album: herança com IPlayble") {
        auto album = std::make_shared<core::Album>();
        CHECK_NOTHROW(album->setSongsLoader([this]() { return this->getSongs(); }));

        CHECK_EQ(album->getSongs().size(), this->getSongs().size());

        std::shared_ptr<core::IPlayable> playable = album;
        CHECK_EQ(playable->getPlayableObjects().size(), this->getSongs().size());
    }

    TEST_CASE_FIXTURE(FixtureCollectionAlbum, "Album: comparações") {
        core::Album album1(1, "Album A", 2001, "Rock", this->artist1, this->user);
        album1.setArtistLoader([this]() { return this->getArtist1(); });
        album1.setSongsLoader([this]() { return this->getSongs(); });
        core::Album album2(2, "Album B", 2002, "Pop", this->artist1, this->user);
        album2.setArtistLoader([this]() { return this->getArtist1(); });
        album2.setSongsLoader([this]() { return this->getSongs(); });

        CHECK_NE(album1, album2);
        CHECK_LT(album1, album2);
        CHECK_LE(album1, album2);
        CHECK_FALSE(album1 > album2);
        CHECK_FALSE(album1 >= album2);

        core::Album album3(1, "Album B", 1990, "Rock", this->artist2, this->user);
        album3.setArtistLoader([this]() { return this->getArtist2(); });
        album3.setSongsLoader([this]() { return this->getSongs(); });

        CHECK_LT(album1, album3);
        CHECK_LE(album1, album3);
        CHECK_FALSE(album1 > album3);
        CHECK_FALSE(album1 >= album3);

        core::Album album4(1, "Album A", 2001, "Rock", this->artist1, this->user);
        album4.setArtistLoader([this]() { return this->getArtist1(); });
        album4.setSongsLoader([this]() { return this->getSongs(); });

        CHECK_EQ(album1, album4);
        CHECK_FALSE(album1 < album4);
        CHECK(album1 <= album4);
        CHECK_FALSE(album1 > album4);
        CHECK(album1 >= album4);
    }

    TEST_CASE_FIXTURE(FixtureCollectionAlbum, "Album: métodos de ICollection") {
        SUBCASE("Adicionar e acessar músicas") {
            core::Album album("Meu Album", "Rock", this->artist1);
            album.setSongsLoader([this]() { return this->getSongs(); });

            CHECK_EQ(album.getSongsCount(), this->getSongs().size());

            std::shared_ptr<core::Song> s1 = std::make_shared<core::Song>("Song A", this->artist1, album);
            std::shared_ptr<core::Song> s2 = std::make_shared<core::Song>("Song B", this->artist1, album);

            album.addSong(*s1);
            album.addSong(*s2);

            CHECK_EQ(album.getSongsCount(), this->getSongs().size() + 2);

            auto songs = album.getSongs();
            CHECK(songs.size() == this->getSongs().size() + 2);

            CHECK_EQ(album[songs.size() - 2]->getTitle(), "Song A");
            CHECK_EQ(album.getSongAt(songs.size() - 2)->getTitle(), "Song A");
            CHECK(album.getSongAt(9999) == nullptr);
            CHECK_THROWS(album[9999]);
        }

        SUBCASE("Remover músicas") {
            core::Album album("Meu Album", "Rock", this->artist1);
            album.setSongsLoader([this]() { return this->getSongs(); });

            CHECK_EQ(album.getSongsCount(), this->getSongs().size());

            core::Song songToRemove("Song to Remove", this->artist1, album);
            unsigned id = 100;
            songToRemove.setId(id);
            album.addSong(songToRemove);

            CHECK_EQ(album.getSongsCount(), this->getSongs().size() + 1);

            bool removed;
            CHECK_NOTHROW(removed = album.removeSong(id));
            CHECK(removed);
            CHECK_EQ(album.getSongsCount(), this->getSongs().size());

            auto notFoundSong = album.findSongById(songToRemove.getId());
            CHECK(notFoundSong == nullptr);
        }

        SUBCASE("Pesquisar músicas") {
            core::Album album("Meu Album", "Rock", this->artist1);
            album.setSongsLoader([this]() { return this->getSongs(); });

            core::Song song1("Unique Title", this->artist1, album);
            song1.setId(200);
            core::Song song2("Common Title", this->artist1, album);
            song2.setId(201);
            core::Song song3("Common Title", this->artist1, album);
            song3.setId(202);
            album.addSong(song1);
            album.addSong(song2);
            album.addSong(song3);

            auto foundById = album.findSongById(200);
            CHECK(foundById != nullptr);
            CHECK_EQ(foundById->getTitle(), "Unique Title");

            auto foundByTitle = album.findSongByTitle("Unique Title");
            CHECK(foundByTitle.size() == 1);
            CHECK_EQ(foundByTitle[0]->getId(), 200);

            auto commonSongs = album.findSongByTitle("Common Title");
            CHECK(commonSongs.size() == 2);

            auto notFoundById = album.findSongById(9999);
            CHECK(notFoundById == nullptr);
        }

        SUBCASE("Calcular duração total") {
            core::Album album("Meu Album", "Rock", this->artist1);
            album.setSongsLoader([this]() { return this->getSongs(); });

            auto song1 = core::Song("Song 1", this->artist1, album);
            song1.setDuration(210);
            album.addSong(song1);
            auto song2 = core::Song("Song 2", this->artist1, album);
            song2.setDuration(180);
            album.addSong(song2);

            CHECK_EQ(album.getSongsCount(), this->getSongs().size() + 2);

            unsigned expectedDuration = song1.getDuration() + song2.getDuration();
            for (const auto& song : this->getSongs()) {
                expectedDuration += song->getDuration();
            }

            CHECK_EQ(album.calculateTotalDuration(), expectedDuration);
        }
    }
}
