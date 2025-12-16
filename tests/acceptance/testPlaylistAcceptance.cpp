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

#include "core/entities/Playlist.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"

#include <memory>
#include <string>
#include <vector>

TEST_SUITE("HISTÓRIA DE USUÁRIO: Reprodução de Música: Playlists") {

    std::shared_ptr<core::Song> createSong(const std::string &title) {
        auto song = std::make_shared<core::Song>();
        song->setTitle(title);
        return song;
    }

    TEST_CASE(
        "Usuário cria playlist, adiciona/ remove músicas e lista playlists") {

        core::User user("Usuario Teste");

        auto songA = createSong("Song A");
        auto songB = createSong("Song B");
        auto songC = createSong("Song C");

        // Simular uma coleção de playlists do usuário.
        std::vector<std::shared_ptr<core::Playlist>> myPlaylists;

        auto playlist = std::make_shared<core::Playlist>();
        std::string PlaylistTesteTitulo = "Playlist Teste";
        playlist->setTitle(PlaylistTesteTitulo);

        myPlaylists.push_back(playlist);

        SUBCASE("Criar uma playlist e atribuir um nome a ela") {

            CHECK(myPlaylists.size() == 1);
            CHECK(myPlaylists[0]->getTitle() == PlaylistTesteTitulo);
        }

        SUBCASE("Adicionar e remover músicas de uma playlist existente") {
            playlist->addSong(*songA);
            playlist->addSong(*songB);
            playlist->addSong(*songC);

            auto songs = playlist->getSongs();
            CHECK(songs.size() == 3);

            // Observações, talvez nao ter um metodo de removeSong, e deixar p
            // removar iterando pela playlists

            CHECK(playlist->removeSong(songB->getId()));

            auto songs_after = playlist->getSongs();
            CHECK(songs_after.size() == 2);
        }

        SUBCASE("Visualizar todas as playlists disponíveis") {

            std::vector<std::string> playlistTitles;
            for (const auto &pl : myPlaylists) {
                playlistTitles.push_back(pl->getTitle());
            }

            bool found = false;
            for (const auto &t : playlistTitles) {
                if (t == PlaylistTesteTitulo) {
                    found = true;
                    break;
                }
            }
            CHECK(found);

            CHECK(myPlaylists.size() >= 1);
        }
    }
}
