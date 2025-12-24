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
    class CollectionPlaylistFixture {
    public:
        std::vector<std::shared_ptr<core::Song>> songs;
        core::Artist artist;
        core::Album album;
        core::User user;

        CollectionPlaylistFixture() {
            songs.push_back(std::make_shared<core::Song>());
            songs.push_back(std::make_shared<core::Song>());
            songs.push_back(std::make_shared<core::Song>());
            songs.push_back(std::make_shared<core::Song>());
            songs.push_back(std::make_shared<core::Song>());

            for (size_t i = 0; i < songs.size(); ++i) {
                songs[i]->setId(static_cast<unsigned>(i + 1));
                songs[i]->setTitle("Song " + std::to_string(i + 1));
            }

            user = core::User("user");
            artist = core::Artist(1, "Artist A", user);
            album = core::Album(1, "Album A", 2020, "Pop", artist, user);
        }

        std::vector<std::shared_ptr<core::Song>> getSongs() {
            return songs;
        }
    };

    TEST_CASE_FIXTURE(CollectionPlaylistFixture, "Playlist: construtores e getters básicos") {

        SUBCASE("Construtor padrão") {
            core::Playlist p;

            CHECK(p.getTitle().empty());
            CHECK_EQ(p.getUser(), nullptr);
        }

        SUBCASE("Construtor com id e título") {
            core::Playlist p(1, "Rock Classics");

            CHECK_EQ(p.getTitle(), "Rock Classics");
            CHECK_EQ(p.getId(), 1);
            CHECK_EQ(p.getUser(), nullptr);
        }

        SUBCASE("Construtor com id, título e usuário") {
            core::Playlist p(2, "Pop Hits", this->user);

            CHECK_EQ(p.getTitle(), "Pop Hits");
            CHECK_EQ(p.getId(), 2);
            CHECK_NE(p.getUser(), nullptr);
            CHECK_EQ(p.getUser()->getUsername(), user.getUsername());
        }
    }

    TEST_CASE_FIXTURE(CollectionPlaylistFixture, "Playlist: setters e getters") {
        core::Playlist p;

        p.setTitle("My Playlist");
        CHECK_EQ(p.getTitle(), "My Playlist");

        p.setUser(this->user);
        CHECK_NE(p.getUser(), nullptr);
        CHECK_EQ(p.getUser()->getUsername(), this->user.getUsername());
    }

    TEST_CASE_FIXTURE(CollectionPlaylistFixture, "Playlist: Comparações") {
        core::Playlist p1(1, "Playlist A");
        p1.setSongsLoader([this]() { return this->getSongs(); });
        core::Playlist p2(2, "Playlist B");
        p2.setSongsLoader([this]() { return this->getSongs(); });

        CHECK(p1 < p2);
        CHECK(p1 <= p2);
        CHECK_FALSE(p1 > p2);
        CHECK_FALSE(p1 >= p2);

        core::Playlist p3(1, "Playlist A");
        p3.setSongsLoader([this]() { return this->getSongs(); });

        CHECK_NE(p1, p2);
        CHECK_EQ(p1, p3);
        CHECK_FALSE(p1 < p3);
        CHECK(p1 <= p3);
        CHECK_FALSE(p1 > p3);
        CHECK(p1 >= p3);
    }

    TEST_CASE_FIXTURE(CollectionPlaylistFixture, "Playlist: herança com IPlayable") {
        core::Playlist p;
        CHECK_NOTHROW(p.setSongsLoader([this]() { return this->getSongs(); }));

        CHECK_EQ(p.getSongs().size(), this->getSongs().size());

        std::shared_ptr<core::IPlayable> playable = std::make_shared<core::Playlist>(p);
        CHECK_EQ(playable->getPlayableObjects().size(), this->getSongs().size());
    }

    TEST_CASE_FIXTURE(CollectionPlaylistFixture, "Playlist: métodos de ICollection") {
        SUBCASE("Adicionar e acessar musicas") {
            core::Playlist p;
            CHECK_NOTHROW(p.setSongsLoader([this]() { return this->getSongs(); }));

            CHECK_EQ(p.getSongs().size(), this->getSongs().size());

            core::Song s1("Song A", this->artist, this->album);
            s1.setId(98);
            core::Song s2("Song B", this->artist, this->album);
            s2.setId(99);

            p.addSong(s1);
            p.addSong(s2);

             CHECK_EQ(p.getSongs().size(), this->getSongs().size() + 2);

            auto songs = p.getSongs();
            CHECK(songs.size() == this->getSongs().size() + 2);

            CHECK_EQ(p[songs.size() - 2]->getTitle(), s1.getTitle());
            CHECK_EQ(p.getSongAt(songs.size() - 2)->getTitle(), s1.getTitle());
            CHECK(p.getSongAt(9999) == nullptr);
            CHECK_THROWS(p[9999]);
        }

        SUBCASE("Remover musicas") {
            core::Playlist p;
            CHECK_NOTHROW(p.setSongsLoader([this]() { return this->getSongs(); }));

            CHECK_EQ(p.getSongsCount(), this->songs.size());

            core::Song songToRemove("Song to Remove", this->artist, this->album);
            unsigned id = 100;
            songToRemove.setId(id);
            p.addSong(songToRemove);

            CHECK_EQ(p.getSongsCount(), this->songs.size() + 1);
            bool removed;
            CHECK_NOTHROW(removed = p.removeSong(id));
            CHECK(removed);
            CHECK_EQ(p.getSongsCount(), this->songs.size());
        }

        SUBCASE("Pesquisar musicas") {
            core::Playlist p;
            CHECK_NOTHROW(p.setSongsLoader([this]() { return this->getSongs(); }));

            CHECK_EQ(p.getSongsCount(), this->songs.size());

            core::Song s1("Unique Song Title", this->artist, this->album);
            s1.setId(200);
            p.addSong(s1);

            auto foundSong = p.findSongById(200);
            CHECK_NE(foundSong, nullptr);
            CHECK_EQ(foundSong->getTitle(), "Unique Song Title");

            auto foundSongsByTitle = p.findSongByTitle("Unique Song Title");
            CHECK_FALSE(foundSongsByTitle.empty());
            CHECK_EQ(foundSongsByTitle[0]->getTitle(), "Unique Song Title");

            core::Song s2("Common Song Title", this->artist, this->album);
            s2.setId(201);
            core::Song s3("Common Song Title", this->artist, this->album);
            s3.setId(202);
            p.addSong(s2);
            p.addSong(s3);

            auto commonSongs = p.findSongByTitle("Common Song Title");
            CHECK_EQ(commonSongs.size(), 2);

            auto notFoundSong = p.findSongById(9999);
            CHECK_EQ(notFoundSong, nullptr);
        }

        SUBCASE("Calcular duração") {
            core::Playlist p;
            CHECK_NOTHROW(p.setSongsLoader([this]() { return this->getSongs(); }));

            core::Song s1("Song A", this->artist, this->album);
            s1.setId(300);
            s1.setDuration(120);
            core::Song s2("Song B", this->artist, this->album);
            s2.setId(301);
            s2.setDuration(150);
            p.addSong(s1);
            p.addSong(s2);

            unsigned expectedDuration = s1.getDuration() + s2.getDuration();
            for (const auto& song : this->songs) {
                expectedDuration += song->getDuration();
            }

            CHECK_EQ(p.calculateTotalDuration(), expectedDuration);
        }
    }
}
