/**
 * @file TestPlaylist.cpp
 * @brief Testes de aceitação para a história de usuário Playlist
 *
 *  "Como usuário, gostaria de criar e organizar minhas playlists,
 *         para agilizar o acesso a conjuntos de músicas organizados
 * previamente."
 *
 * Critérios de aceitação cobertos:
 *  - Criar uma playlist e atribuir um nome a ela.
 *  - Adicionar e remover músicas de uma playlist existente.
 *  - Visualizar todas as minhas playlists disponíveis.
 *
 * @author Bruno Vieira
 * @date 2025-10-28
 */

#include <doctest/doctest.h>

#include "core/bd/PlaylistRepository.hpp"
#include "core/entities/Playlist.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/Album.hpp"
#include "core/bd/RepositoryFactory.hpp"
#include "core/services/Library.hpp"


#include "core/services/UsersManager.hpp"
#include "fixtures/ConfigFixture.hpp"
#include "fixtures/DatabaseFixture.hpp"
#include "fixtures/MediaFixture.hpp"


#include <iostream>
#include <memory>
#include <string>
#include <vector>

TEST_SUITE("HISTÓRIA DE USUÁRIO: Reprodução de Música: Playlists") {
    ConfigFixture config = ConfigFixture();
    MediaFixture media = MediaFixture();
    std::shared_ptr<SQLite::Database> db;

    std::shared_ptr<core::Song> createSong(const std::string &title) {
        auto song = std::make_shared<core::Song>();
        song->setTitle(title);
        return song;
    }

    class PlaylistFixture {
    public:
        std::shared_ptr<core::SongRepository> song_repo;
        std::shared_ptr<core::ArtistRepository> artist_repo;
        std::shared_ptr<core::AlbumRepository> album_repo;
        std::shared_ptr<core::UserRepository> user_repo;
        std::shared_ptr<core::PlaylistRepository> playlist_repo;
        core::User user;
        std::vector<std::shared_ptr<core::Song>> songs;
        core::Album album;
        core::Artist artist;

        PlaylistFixture() : user("test_user") {
            setup();

            #if WIN32
            user.setUID("1000");
            #else
            user.setUID(1000);
            #endif

            user_repo->save(user);

            artist = core::Artist(0, "Artist A", user);
            artist_repo->save(artist);

            auto artist_from_repo = artist_repo->findByNameAndUser(artist.getName(), user);
            REQUIRE(!artist_from_repo.empty());

            album = core::Album(0, "Album A", 2020, "Pop", artist, user);
            album_repo->save(album);

            songs.push_back(std::make_shared<core::Song>(0, "Song 1", artist.getId(), album.getId()));
            songs[0]->setUser(user);
            songs.push_back(std::make_shared<core::Song>(0, "Song 2", artist.getId(), album.getId()));
            songs[1]->setUser(user);
            songs.push_back(std::make_shared<core::Song>(0, "Song 3", artist.getId(), album.getId()));
            songs[2]->setUser(user);

            album.setSongsLoader([this]() {
                return this->songs;
            });

            artist.setSongsLoader([this]() {
                return this->songs;
            });

            for (auto &song : songs) {
                song_repo->save(*song);
            }
        }

        void setup() {
            db = DatabaseFixture().getDatabase();

            core::RepositoryFactory repo_factory(db);

            song_repo = repo_factory.createSongRepository();
            artist_repo = repo_factory.createArtistRepository();
            album_repo = repo_factory.createAlbumRepository();
            user_repo = repo_factory.createUserRepository();
            playlist_repo = repo_factory.createPlaylistRepository();
        }

        void clear_repo() {
            playlist_repo->removeAll();
            song_repo->removeAll();
            artist_repo->removeAll();
            album_repo->removeAll();
            user_repo->removeAll();

            setup();
        }
    };

    TEST_CASE_FIXTURE(PlaylistFixture, "CT-AC-01: Criar playlist") {
        auto playlist = std::make_shared<core::Playlist>();
        std::string playlistTitle = "Minha Playlist Favorita";
        playlist->setTitle(playlistTitle);
        playlist->setUser(this->user);


        core::Library library(this->user, *db);
        auto song1_vtr = library.searchSong(this->songs[0]->getTitle());
        REQUIRE(!song1_vtr.empty());
        CHECK_EQ(song1_vtr[0]->getTitle(), this->songs[0]->getTitle());
        playlist->pushFront(*song1_vtr[0]);

        CHECK(library.persist(*playlist));

        auto playlists_repo = this->playlist_repo->getAll();
        REQUIRE(!playlists_repo.empty());
        CHECK_EQ(playlists_repo[0]->getTitle(), playlistTitle);

        auto playlists = library.searchPlaylist(playlistTitle);
        REQUIRE(!playlists.empty());
        CHECK_EQ(playlists[0]->getTitle(), playlistTitle);
    }

    TEST_CASE_FIXTURE(PlaylistFixture,
        "CT-AC-02, adiciona/ remove músicas de um playlist") {

        SUBCASE("Adicionar músicas") {
            core::Playlist playlist;
            playlist.setTitle("Playlist de Teste");
            playlist.setUser(user);

            CHECK(playlist.pushFront(*songs[0]));
            CHECK(playlist.pushBack(*songs[1]));
            CHECK(playlist.insert(*songs[2], 2));

            core::Library library(this->user, *db);
            CHECK(library.persist(playlist));
            auto playlist_from_repo = library.searchPlaylist("Playlist de Teste");
            REQUIRE(!playlist_from_repo.empty());
            playlist = *playlist_from_repo[0];

            std::cout << "Playlist persistida com sucesso." << std::endl;
            auto songs_in_playlist = playlist.getSongs();
            REQUIRE_EQ(songs_in_playlist.size(), 3);
            CHECK(songs_in_playlist[0]->getTitle() == songs[0]->getTitle());
            CHECK(songs_in_playlist[1]->getTitle() == songs[1]->getTitle());
            CHECK(songs_in_playlist[2]->getTitle() == songs[2]->getTitle());

            this->clear_repo();
        }

        SUBCASE("Adicionar um album") {
            core::Playlist playlist;
            playlist.setTitle("Playlist de Teste");
            playlist.setUser(user);

            CHECK(playlist.pushBack(album));

            core::Library library(this->user, *db);
            CHECK(library.persist(playlist));

            auto playlist_from_repo = library.searchPlaylist("Playlist de Teste");
            REQUIRE(!playlist_from_repo.empty());
            playlist = *playlist_from_repo[0];

            auto songs_in_playlist = playlist.getSongs();
            REQUIRE_EQ(songs_in_playlist.size(), 3);
            CHECK(songs_in_playlist[0]->getTitle() == songs[0]->getTitle());
            CHECK(songs_in_playlist[1]->getTitle() == songs[1]->getTitle());
            CHECK(songs_in_playlist[2]->getTitle() == songs[2]->getTitle());

            this->clear_repo();
        }

        SUBCASE("Adicionar um artista") {
            core::Playlist playlist;
            playlist.setTitle("Playlist de Teste");
            playlist.setUser(user);

            CHECK(playlist.pushBack(artist));

            core::Library library(this->user, *db);
            CHECK(library.persist(playlist));

            auto playlist_from_repo = library.searchPlaylist("Playlist de Teste");
            REQUIRE(!playlist_from_repo.empty());
            playlist = *playlist_from_repo[0];

            auto songs_in_playlist = playlist.getSongs();
            REQUIRE_EQ(songs_in_playlist.size(), 3);
            CHECK(songs_in_playlist[0]->getTitle() == songs[0]->getTitle());
            CHECK(songs_in_playlist[1]->getTitle() == songs[1]->getTitle());
            CHECK(songs_in_playlist[2]->getTitle() == songs[2]->getTitle());

            this->clear_repo();
        }

        SUBCASE("Adicionar um vetor de Songs") {
            core::Playlist playlist;
            playlist.setTitle("Playlist de Teste");
            playlist.setUser(user);

            CHECK(playlist.pushBack(songs));

            core::Library library(this->user, *db);
            CHECK(library.persist(playlist));

            auto playlist_from_repo = library.searchPlaylist("Playlist de Teste");
            REQUIRE(!playlist_from_repo.empty());
            playlist = *playlist_from_repo[0];

            auto songs_in_playlist = playlist.getSongs();
            REQUIRE_EQ(songs_in_playlist.size(), 3);
            CHECK(songs_in_playlist[0]->getTitle() == songs[0]->getTitle());
            CHECK(songs_in_playlist[1]->getTitle() == songs[1]->getTitle());
            CHECK(songs_in_playlist[2]->getTitle() == songs[2]->getTitle());

            this->clear_repo();
        }

        SUBCASE("Remover músicas de uma playlist") {
            core::Playlist playlist;
            playlist.setTitle("Playlist de Teste");
            playlist.setUser(user);

            CHECK(playlist.pushBack(songs));

            core::Library library(this->user, *db);
            CHECK(library.persist(playlist));

            auto playlist_from_repo = library.searchPlaylist("Playlist de Teste");
            REQUIRE(!playlist_from_repo.empty());
            playlist = *playlist_from_repo[0];

            auto songs_in_playlist = playlist.getSongs();
            REQUIRE_EQ(songs_in_playlist.size(), 3);

            CHECK(playlist.removeSong(songs[1]->getId()));
            CHECK_EQ(playlist.getSongs().size(), 2);
            CHECK(playlist.getSongs()[0]->getTitle() == songs[0]->getTitle());
            CHECK(playlist.getSongs()[1]->getTitle() == songs[2]->getTitle());

            this->clear_repo();
        }
    }

    TEST_CASE_FIXTURE(PlaylistFixture,
        "CT-AC-03: Visualizar todas as playlists disponíveis") {

        SUBCASE("Listar todas as playlists de um usuário") {
            core::Playlist p1;
            std::string p1_title = "Playlist Teste 1";
            p1.setTitle(p1_title);
            p1.setUser(this->user);
            p1.pushBack(*songs[0]);
            core::Playlist p2;
            std::string p2_title = "Playlist Teste 2";
            p2.setTitle(p2_title);
            p2.setUser(this->user);
            p2.pushBack(*songs[1]);

            core::Library library(this->user, *db);
            CHECK(library.persist(p1));
            CHECK(library.persist(p2));

            auto myPlaylists = library.searchPlaylist("");
            REQUIRE_EQ(myPlaylists.size(), 2);

            this->clear_repo();
        }

        core::User user2("Outro Usuario");
        #if WIN32
        user2.setUID("1001");
        #else
        user2.setUID(1001);
        #endif
        this->user_repo->save(user2);

        SUBCASE("Listar todas as playlists com mais de um usuário") {
            core::Playlist p1;
            std::string p1_title = "Playlist Teste 1";
            p1.setTitle(p1_title);
            p1.setUser(this->user);
            p1.pushBack(*songs[0]);
            core::Playlist p2;
            std::string p2_title = "Playlist Teste 2";
            p2.setTitle(p2_title);
            p2.setUser(user2);
            p2.pushBack(*songs[1]);
            core::Playlist p3;
            std::string p3_title = "Playlist Teste 3";
            p3.setTitle(p3_title);
            p3.setUser(this->user);
            p3.pushBack(*songs[2]);

            core::Library library(this->user, *db);
            CHECK(library.persist(p1));
            CHECK(library.persist(p2));
            CHECK(library.persist(p3));

            auto myPlaylists = library.searchPlaylist("");
            REQUIRE_EQ(myPlaylists.size(), 2);

            CHECK_EQ(myPlaylists[0]->getTitle(), p1_title);
            CHECK_EQ(myPlaylists[1]->getTitle(), p3_title);

            this->clear_repo();
        }
    }
}
