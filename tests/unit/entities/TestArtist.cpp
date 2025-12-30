#include <doctest/doctest.h>
#include <memory>
#include <vector>

#include "core/entities/Album.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"
#include "core/interfaces/IPlayable.hpp"

TEST_SUITE("Unit Tests - Entity: Artist") {
    class FixtureCollectionArtist {
    public:
        std::vector<std::shared_ptr<core::Album>> albums;
        std::vector<std::shared_ptr<core::Song>> songs;
        core::User user;

        FixtureCollectionArtist() {
            albums.push_back(std::make_shared<core::Album>());
            albums.push_back(std::make_shared<core::Album>());
            albums.push_back(std::make_shared<core::Album>());
            albums.push_back(std::make_shared<core::Album>());
            albums.push_back(std::make_shared<core::Album>());

            songs.push_back(std::make_shared<core::Song>());
            songs.push_back(std::make_shared<core::Song>());
            songs.push_back(std::make_shared<core::Song>());
            songs.push_back(std::make_shared<core::Song>());
            songs.push_back(std::make_shared<core::Song>());

            for (size_t i = 0; i < albums.size(); ++i) {
                albums[i]->setId(static_cast<unsigned>(i + 1));
                albums[i]->setTitle("Album " + std::to_string(i + 1));
            }

            for (size_t i = 0; i < songs.size(); ++i) {
                songs[i]->setId(static_cast<unsigned>(i + 1));
                songs[i]->setTitle("Song " + std::to_string(i + 1));
            }

            user = core::User("user");
            user.setId(1);
        }

        std::vector<std::shared_ptr<core::Album>> getAlbums() {
            return albums;
        }

        std::vector<std::shared_ptr<core::Song>> getSongs() {
            return songs;
        }
    };

    TEST_CASE_FIXTURE(FixtureCollectionArtist, "Artist: construtores báscos") {
        SUBCASE("Construtor padrão") {
            core::Artist a1;
            CHECK(a1.getName().empty());
            CHECK(a1.getGenre().empty());
        }

        SUBCASE("Construtor com nome e genero") {
            core::Artist a2("Metallica", "Metal");
            CHECK_EQ(a2.getName(), "Metallica");
            CHECK_EQ(a2.getGenre(), "Metal");
        }

        SUBCASE("Construtor com id, nome e usuário") {
            core::Artist a3(10, "Banda X", this->user);
            CHECK_EQ(a3.getName(), "Banda X");
            CHECK(a3.getUser() != nullptr);
            CHECK_EQ(a3.getUser()->getUsername(), this->user.getUsername());
        }

        SUBCASE("Construtor com id, nome, genero e usuário") {
            core::Artist a4(20, "Banda Y", "Rock", this->user);
            CHECK_EQ(a4.getName(), "Banda Y");
            CHECK_EQ(a4.getGenre(), "Rock");
            CHECK(a4.getUser() != nullptr);
            CHECK_EQ(a4.getUser()->getUsername(), this->user.getUsername());
        }

        SUBCASE("Construtor de cópia") {
            core::Artist a2(30, "Banda Z", "Pop", this->user);
            auto a4 = a2;
            CHECK_EQ(a4.getName(), a2.getName());
            CHECK_EQ(a4.getGenre(), a2.getGenre());
            CHECK_EQ(a4.getUser()->getUsername(), a2.getUser()->getUsername());
            CHECK_EQ(a4.getId(), a2.getId());
        }
    }

    TEST_CASE_FIXTURE(FixtureCollectionArtist, "Artist: getters e setters") {
        core::Artist artist;
        CHECK_NOTHROW(artist.setAlbumsLoader([this]() { return this->getAlbums(); }));
        CHECK_NOTHROW(artist.setSongsLoader([this]() { return this->getSongs(); }));

        artist.setName("Daft Punk");
        artist.setGenre("Electronic");

        CHECK_EQ(artist.getName(), "Daft Punk");
        CHECK_EQ(artist.getGenre(), "Electronic");

        artist.setUser(this->user);

        CHECK(artist.getUser() != nullptr);
        CHECK_EQ(artist.getUser()->getUsername(), this->user.getUsername());
    }

    TEST_CASE_FIXTURE(FixtureCollectionArtist, "Artist: Collection methods") {
        SUBCASE("Adicionar e acessar músicas") {
            core::Artist artist("Artist Z", "Pop");
            CHECK_NOTHROW(artist.setAlbumsLoader([this]() { return this->getAlbums(); }));
            CHECK_NOTHROW(artist.setSongsLoader([this]() { return this->getSongs(); }));

            CHECK_EQ(artist.getSongsCount(), this->getSongs().size());

            auto album = core::Album();

            std::shared_ptr<core::Song> s1 = std::make_shared<core::Song>("Song A", artist, album);
            s1->setId(50);
            std::shared_ptr<core::Song> s2 = std::make_shared<core::Song>("Song B", artist, album);
            s2->setId(51);

            artist.addSong(*s1);
            artist.addSong(*s2);

            CHECK_EQ(artist.getSongsCount(), this->getSongs().size() + 2);
            CHECK(artist.hasSong());

            auto songs = artist.getSongs();
            CHECK(songs.size() == this->getSongs().size() + 2);

            CHECK_EQ(artist[songs.size() - 2]->getTitle(), "Song A");
            CHECK_EQ(artist.getSongAt(songs.size() - 2)->getTitle(), "Song A");
            CHECK(artist.getSongAt(9999) == nullptr);
            CHECK_THROWS(artist[9999]);
        }

        SUBCASE("Adicionar álbuns") {
            core::Artist artist("Artist Y", "Rock");
            CHECK_NOTHROW(artist.setAlbumsLoader([this]() { return this->getAlbums(); }));
            CHECK_NOTHROW(artist.setSongsLoader([this]() { return this->getSongs(); }));

            CHECK_EQ(artist.getAlbumsCount(), this->getAlbums().size());

            core::Album a1("Album 1", "Rock", artist);
            a1.setId(50);
            core::Album a2("Album 2", "MPB", artist);
            a2.setId(51);

            artist.addAlbum(a1);
            artist.addAlbum(a2);

            CHECK_EQ(artist.getAlbumsCount(), this->getAlbums().size() + 2);
            CHECK(artist.hasAlbum());

            auto albums = artist.getAlbums();
            CHECK(albums.size() == this->getAlbums().size() + 2);

            CHECK_EQ(albums[albums.size() - 2]->getTitle(), "Album 1");
            CHECK_EQ(artist.getAlbumAt(albums.size() - 2)->getTitle(), "Album 1");
            CHECK_NOTHROW(artist.getAlbumAt(9999));
            CHECK(artist.getAlbumAt(9999) == nullptr);
        }

        SUBCASE("Pesquisas músicas e álbuns") {
            core::Artist artist("Artist Y", "Rock");

            std::vector<std::shared_ptr<core::Album>> testAlbums = {
                std::make_shared<core::Album>("Unique Album", "Rock", artist),
                std::make_shared<core::Album>("Common Album", "Rock", artist),
                std::make_shared<core::Album>("Common Album", "Rock", artist)
            };
            testAlbums[0]->setId(1);
            testAlbums[1]->setId(2);
            testAlbums[2]->setId(3);

            std::vector<std::shared_ptr<core::Song>> testSongs = {
                std::make_shared<core::Song>("Unique Title", artist, *testAlbums[0]),
                std::make_shared<core::Song>("Common Title", artist, *testAlbums[1]),
                std::make_shared<core::Song>("Common Title", artist, *testAlbums[2])
            };
            testSongs[0]->setId(1);
            testSongs[1]->setId(2);
            testSongs[2]->setId(3);

            artist.setAlbumsLoader([testAlbums]() { return testAlbums; });
            artist.setSongsLoader([testSongs]() { return testSongs; });

            auto foundSong = artist.findSongByTitle("Unique Title");
            CHECK_FALSE(foundSong.empty());
            CHECK_EQ(foundSong[0]->getTitle(), "Unique Title");

            auto foundAlbum = artist.findAlbumByTitle("Unique Album");
            CHECK_FALSE(foundAlbum.empty());
            CHECK_EQ(foundAlbum[0]->getTitle(), "Unique Album");

            auto notFoundSong = artist.findSongByTitle("Nonexistent Title");
            CHECK(notFoundSong.empty());

            auto notFoundAlbum = artist.findAlbumByTitle("Nonexistent Album");
            CHECK(notFoundAlbum.empty());

            auto commonSong = artist.findSongByTitle("Common Title");
            CHECK_EQ(commonSong.size(), 2);
            auto commonAlbum = artist.findAlbumByTitle("Common Album");
            CHECK_EQ(commonAlbum.size(), 2);

            auto foundSongById = artist.findSongById(1);
            CHECK(foundSongById != nullptr);
            CHECK_EQ(foundSongById->getTitle(), "Unique Title");
            auto foundAlbumById = artist.findAlbumById(1);
            CHECK(foundAlbumById != nullptr);
            CHECK_EQ(foundAlbumById->getTitle(), "Unique Album");
        }

        SUBCASE("Remover músicas e álbuns") {
            core::Artist artist("Artist W", "Jazz");
            CHECK_NOTHROW(artist.setAlbumsLoader([this]() { return this->getAlbums(); }));
            CHECK_NOTHROW(artist.setSongsLoader([this]() { return this->getSongs(); }));

            CHECK_EQ(artist.getSongsCount(), this->getSongs().size());
            CHECK_EQ(artist.getAlbumsCount(), this->getAlbums().size());

            auto album = std::make_shared<core::Album>();
            std::shared_ptr<core::Artist> artistPtr = std::make_shared<core::Artist>(artist);

            std::shared_ptr<core::Song> s1 = std::make_shared<core::Song>("Song To Remove", artist, *album);
            s1->setId(101);
            artist.addSong(*s1);

            core::Album a1("Album To Remove", "Jazz", artist);
            a1.setId(201);
            artist.addAlbum(a1);

            CHECK_EQ(artist.getSongsCount(), this->getSongs().size() + 1);
            CHECK_EQ(artist.getAlbumsCount(), this->getAlbums().size() + 1);

            bool songRemoved = artist.removeSong(101);
            CHECK(songRemoved);
            CHECK_EQ(artist.getSongsCount(), this->getSongs().size());

            bool albumRemoved = artist.removeAlbum(201);
            CHECK(albumRemoved);
            CHECK_EQ(artist.getAlbumsCount(), this->getAlbums().size());

            bool songNotRemoved = artist.removeSong(9999);
            CHECK_FALSE(songNotRemoved);

            bool albumNotRemoved = artist.removeAlbum(9999);
            CHECK_FALSE(albumNotRemoved);
        }

        SUBCASE("Calcular duração total") {
            core::Artist artist("Artist V", "Classical");

            auto album = std::make_shared<core::Album>();

            auto s1 = std::make_shared<core::Song>("Track 1", artist, *album);
            s1->setId(100);
            s1->setDuration(120);

            auto s2 = std::make_shared<core::Song>("Track 2", artist, *album);
            s2->setId(101);
            s2->setDuration(150);

            artist.setSongsLoader([s1, s2]() {
                return std::vector<std::shared_ptr<core::Song>>{s1, s2};
            });

            unsigned totalDuration = artist.calculateTotalDuration();
            CHECK_EQ(totalDuration, s1->getDuration() + s2->getDuration());
        }
    }

    TEST_CASE_FIXTURE(FixtureCollectionArtist, "Artist: herança de Playble") {
        std::shared_ptr<core::Artist> artist = std::make_shared<core::Artist>();
        CHECK_NOTHROW(artist->setAlbumsLoader([this]() { return this->getAlbums(); }));
        CHECK_NOTHROW(artist->setSongsLoader([this]() { return this->getSongs(); }));

        CHECK_EQ(artist->getSongsCount(), this->getSongs().size());

        std::shared_ptr<core::IPlayable> playable = artist;
        CHECK_EQ(playable->getPlayableObjects().size(), 5);
    }

    TEST_CASE_FIXTURE(FixtureCollectionArtist, "Artist:  Comparações") {
        core::Artist artist1(1, "Artist A", this->user);
        artist1.setSongsLoader([this]() { return this->getSongs(); });
        artist1.setAlbumsLoader([this]() { return this->getAlbums(); });
        core::Artist artist2(2, "Artist B", this->user);
        artist2.setSongsLoader([this]() { return this->getSongs(); });
        artist2.setAlbumsLoader([this]() { return this->getAlbums(); });


        CHECK(artist1 < artist2);
        CHECK(artist1 <= artist2);
        CHECK_FALSE(artist1 > artist2);
        CHECK_FALSE(artist1 >= artist2);

        core::Artist artist3(1, "Artist A", this->user);
        artist3.setSongsLoader([this]() { return this->getSongs(); });
        artist3.setAlbumsLoader([this]() { return this->getAlbums(); });

        CHECK_NE(artist1, artist2);
        CHECK_EQ(artist1, artist3);
        CHECK_FALSE(artist1 < artist3);
        CHECK(artist1 <= artist3);
        CHECK_FALSE(artist1 > artist3);
        CHECK(artist1 >= artist3);
    }
}
