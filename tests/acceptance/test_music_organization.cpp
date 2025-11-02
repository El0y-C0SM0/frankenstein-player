#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <doctest/doctest.h>
#include <memory>
#include <string>

#include "core/bd/AlbumRepository.hpp"
#include "core/bd/ArtistRepository.hpp"
#include "core/bd/RepositoryFactory.hpp"
#include "core/bd/SongRepository.hpp"
#include "core/bd/UserRepository.hpp"
#include "core/entities/User.hpp"
#include "core/services/Manager.hpp"

#include "fixtures/ConfigFixture.hpp"
#include "fixtures/DatabaseFixture.hpp"
#include "fixtures/MediaFixture.hpp"

TEST_SUITE("HISTÓRIA DE USUÁRIO: Organização de Músicas") {
    ConfigFixture config = ConfigFixture();
    MediaFixture media = MediaFixture();
    core::RepositoryFactory repo_factory(DatabaseFixture().getDatabase());

    std::unique_ptr<core::SongRepository> song_repo =
        repo_factory.createSongRepository();
    std::unique_ptr<core::ArtistRepository> artist_repo =
        repo_factory.createArtistRepository();
    std::unique_ptr<core::AlbumRepository> album_repo =
        repo_factory.createAlbumRepository();
    std::unique_ptr<core::UserRepository> user_repo =
        repo_factory.createUserRepository();

    void checkSongInDatabase(const MediaFixture::SongTestMock& song,
                             const core::User& user) {
        auto songs = song_repo->findByTitleAndUser(song.title, user);

        CHECK(songs.size() > 0);
        CHECK(songs[0]->getTitle() == song.title);
        CHECK(songs[0]->getDuration() == song.duracao);
        if (!song.artist.empty())
            CHECK(songs[0]->getArtist()->getName() == song.artist);
        if (!song.album.empty())
            CHECK(songs[0]->getAlbum()->getName() == song.album);
        CHECK(songs[0]->getYear() == std::stoi(song.year));
        CHECK(songs[0]->getGenre() == song.genre);
    }

    void clearTestEnvironment() {
        song_repo->removeAll();
        artist_repo->removeAll();
        album_repo->removeAll();
        user_repo->removeAll();

        boost::filesystem::remove_all(config.userMusicDirectory());
    }

    core::Manager manager(config);

    TEST_CASE("CT-AC-01: Adicionar e organizar um album") {
        SUBCASE("Organizar um álbum") {
            core::User user("usertest1",
                            config.userMusicDirectory(),
                            config.inputPublicPath(),
                            1001);
            user_repo->save(user);

            auto song_mock1 =
                media.getSongTestMock("Short_Song_Test_The_Testers");
            auto song_mock2 =
                media.getSongTestMock("Medium_Song_Test_The_Testers");

            boost::filesystem::copy(song_mock1.path, user.getInputPath());
            boost::filesystem::copy(song_mock2.path, user.getInputPath());
            manager.update();

            std::string song1_path_registre =
                user.getInputPath() + song_mock1.artist + "/" + song_mock1.album
                + "/" + song_mock1.title + ".mp3";
            std::string song2_path_registre =
                user.getInputPath() + song_mock2.artist + "/" + song_mock2.album
                + "/" + song_mock2.title + ".mp3";

            CHECK(boost::filesystem::exists(song1_path_registre));
            CHECK(boost::filesystem::exists(song2_path_registre));
            CHECK(song_mock1.album == song_mock2.album);
            CHECK(song_mock1.artist == song_mock2.artist);
            checkSongInDatabase(song_mock1, user);
            checkSongInDatabase(song_mock2, user);

            clearTestEnvironment();
        }

        SUBCASE("Atualizar um álbum existente") {
            core::User user("usertest1",
                            config.userMusicDirectory(),
                            config.inputPublicPath(),
                            1001);
            user_repo->save(user);

            auto song_mock1 =
                media.getSongTestMock("Short_Song_Test_The_Testers");
            auto song_mock2 =
                media.getSongTestMock("Medium_Song_Test_The_Testers");

            boost::filesystem::copy(song_mock1.path, user.getInputPath());
            manager.update();

            std::string song1_path_registre =
                user.getInputPath() + song_mock1.artist + "/" + song_mock1.album
                + "/" + song_mock1.title + ".mp3";

            CHECK(boost::filesystem::exists(song1_path_registre));
            checkSongInDatabase(song_mock1, user);

            boost::filesystem::copy(song_mock2.path, user.getInputPath());
            manager.update();

            std::string song2_path_registre =
                user.getInputPath() + song_mock2.artist + "/" + song_mock2.album
                + "/" + song_mock2.title + ".mp3";

            CHECK(boost::filesystem::exists(song2_path_registre));
            checkSongInDatabase(song_mock2, user);

            CHECK(song_mock1.album == song_mock2.album);
            CHECK(song_mock1.artist == song_mock2.artist);

            auto albuns = album_repo->getAll();
            CHECK(albuns.size() == 1);
            CHECK(albuns[0]->getSongCount() == 2);

            clearTestEnvironment();
        }

        SUBCASE("Organizar musicas de mais de um usuario") {
            core::User user1("usertest1",
                             config.userMusicDirectory(),
                             config.inputPublicPath(),
                             1001);

            core::User user2("usertest2",
                             config.userMusicDirectory(),
                             config.inputPublicPath(),
                             1002);

            user_repo->save(user1);
            user_repo->save(user2);

            const MediaFixture::SongTestMock song_mock1 =
                media.getSongTestMock("Short_Song_Test_The_Testers");
            const MediaFixture::SongTestMock song_mock2 =
                media.getSongTestMock("Short_Song_Examples_Example_Band");

            boost::filesystem::copy(song_mock1.path, user1.getInputPath());
            boost::filesystem::copy(song_mock2.path, user2.getInputPath());
            manager.update();

            std::string song1_path_registre =
                user1.getInputPath() + song_mock1.artist + "/"
                + song_mock1.album + "/" + song_mock1.title + ".mp3";
            std::string song2_path_registre =
                user2.getInputPath() + song_mock2.artist + "/"
                + song_mock2.album + "/" + song_mock2.title + ".mp3";

            CHECK(boost::filesystem::exists(song1_path_registre));
            CHECK(boost::filesystem::exists(song2_path_registre));
            checkSongInDatabase(song_mock1, user1);
            checkSongInDatabase(song_mock2, user2);
            CHECK(song_repo->findByTitleAndUser(song_mock1.title, user2).size()
                  == 1);

            clearTestEnvironment();
        }

        SUBCASE("Organizar música sem usuario") {
            const MediaFixture::SongTestMock song_mock1 =
                media.getSongTestMock("Short_Song_Test_The_Testers");
            boost::filesystem::copy(song_mock1.path, config.inputPublicPath());
            manager.update();

            std::string song1_path_registre =
                config.inputPublicPath() + song_mock1.artist + "/"
                + song_mock1.album + "/" + song_mock1.title + ".mp3";

            CHECK(boost::filesystem::exists(song1_path_registre));

            auto songs = song_repo->getAll();
            CHECK(songs.size() == 1);
            CHECK(songs[0]->getTitle() == song_mock1.title);
            CHECK(songs[0]->getDuration() == song_mock1.duracao);
            if (!song_mock1.artist.empty())
                CHECK(songs[0]->getArtist()->getName() == song_mock1.artist);
            else
                FAIL_CHECK(
                    "Erro: song_mock1.artist está vazio, não foi possível verificar o artista.");
            if (!song_mock1.album.empty())
                CHECK(songs[0]->getAlbum()->getName() == song_mock1.album);
            else
                FAIL_CHECK(
                    "Erro: song_mock1.album está vazio, não foi possível verificar o álbum.");
            CHECK(songs[0]->getYear() == std::stoi(song_mock1.year));
            CHECK(songs[0]->getGenre() == song_mock1.genre);

            clearTestEnvironment();
        }

        SUBCASE("Reorganizar com pasta de entradas vazia") {
            core::User user("usertest1",
                            config.userMusicDirectory(),
                            config.inputPublicPath(),
                            1001);
            user_repo->save(user);

            CHECK_NOTHROW(manager.update());

            clearTestEnvironment();
        }
    }

    TEST_CASE("CT-AC-02: Adicionar e organizar uma musica") {
        SUBCASE("Organizar uma música") {
            core::User user("usertest1",
                            config.userMusicDirectory(),
                            config.inputPublicPath(),
                            1001);
            user_repo->save(user);

            const MediaFixture::SongTestMock song_mock =
                media.getSongTestMock("Short_Song_Test_The_Testers");

            boost::filesystem::copy(song_mock.path, user.getInputPath());

            manager.update();

            std::string song_path_registre =
                user.getInputPath() + song_mock.artist + "/" + song_mock.album
                + "/" + song_mock.title + ".mp3";

            CHECK(boost::filesystem::exists(song_path_registre));
            checkSongInDatabase(song_mock, user);

            clearTestEnvironment();
        }

        SUBCASE("Organizar multiplas musicas") {
            core::User user("usertest1",
                            config.userMusicDirectory(),
                            config.inputPublicPath(),
                            1001);
            user_repo->save(user);

            const auto song_mock1 =
                media.getSongTestMock("Short_Song_Test_The_Testers");
            const auto song_mock2 =
                media.getSongTestMock("Medium_Song_Test_The_Testers");

            boost::filesystem::copy(song_mock1.path, user.getInputPath());
            boost::filesystem::copy(song_mock2.path, user.getInputPath());

            manager.update();

            std::string song1_path_registre =
                user.getInputPath() + song_mock1.artist + "/" + song_mock1.album
                + "/" + song_mock1.title + ".mp3";
            std::string song2_path_registre =
                user.getInputPath() + song_mock2.artist + "/" + song_mock2.album
                + "/" + song_mock2.title + ".mp3";

            CHECK(boost::filesystem::exists(song1_path_registre));
            CHECK(boost::filesystem::exists(song2_path_registre));
            checkSongInDatabase(song_mock1, user);
            checkSongInDatabase(song_mock2, user);

            clearTestEnvironment();
        }

        SUBCASE("Organizar música sem album") {
            core::User user("usertest1",
                            config.userMusicDirectory(),
                            config.inputPublicPath(),
                            1001);
            user_repo->save(user);

            const MediaFixture::SongTestMock song_mock =
                media.getSongTestMock("Song_Test_No_Album");

            boost::filesystem::copy(song_mock.path, user.getInputPath());
            manager.update();

            std::string song_path_registre = user.getInputPath()
                                             + song_mock.artist + "/Singles/"
                                             + song_mock.title + ".mp3";
            CHECK(boost::filesystem::exists(song_path_registre));
            checkSongInDatabase(song_mock, user);

            clearTestEnvironment();
        }

        SUBCASE("Organizar música sem artista") {
            /*
             copia um arquivo de música para o diretório de input
             confere se foi registrado no banco de dados
             confere se o arquivo foi movido para o diretório correto
             */
        }

        SUBCASE("Reorganizar após remoção de musicas") {
            core::User user("usertest1",
                            config.userMusicDirectory(),
                            config.inputPublicPath(),
                            1001);
            user_repo->save(user);

            const MediaFixture::SongTestMock song_mock =
                media.getSongTestMock("Short_Song_Test_The_Testers");

            boost::filesystem::copy(song_mock.path, user.getInputPath());
            manager.update();

            std::string song_path_registre =
                user.getInputPath() + song_mock.artist + "/" + song_mock.album
                + "/" + song_mock.title + ".mp3";
            CHECK(boost::filesystem::exists(song_path_registre));

            boost::filesystem::remove(song_path_registre);

            manager.update();
            CHECK_FALSE(boost::filesystem::exists(song_path_registre));
            CHECK(song_repo->findByTitleAndUser(song_mock.title, user).size()
                  == 0);
        }
    }

    TEST_CASE("CT-AC-03: Adicionar e organizar musicas conforme autores") {
        SUBCASE("Organizar musicas de artistas diferentes") {
            core::User user("usertest1",
                            config.userMusicDirectory(),
                            config.inputPublicPath(),
                            1001);
            user_repo->save(user);

            const MediaFixture::SongTestMock song_mock =
                media.getSongTestMock("Short_Song_Examples_Example_Band");
            boost::filesystem::copy(song_mock.path, user.getInputPath());
            const MediaFixture::SongTestMock song_mock2 =
                media.getSongTestMock("Short_Song_Test_The_Testers");
            boost::filesystem::copy(song_mock2.path, user.getInputPath());
            manager.update();

            std::string song_path_registre =
                user.getInputPath() + song_mock.artist + "/" + song_mock.album
                + "/" + song_mock.title + ".mp3";
            std::string song2_path_registre =
                user.getInputPath() + song_mock2.artist + "/" + song_mock2.album
                + "/" + song_mock2.title + ".mp3";

            CHECK(boost::filesystem::exists(song_path_registre));
            CHECK(boost::filesystem::exists(song2_path_registre));
            checkSongInDatabase(song_mock, user);
            checkSongInDatabase(song_mock2, user);
            auto artists = artist_repo->getAll();

            bool found_artist1 = false;
            bool found_artist2 = false;

            for (const auto& artist : artists) {
                if (artist->getName() == song_mock.artist)
                    found_artist1 = true;
                if (artist->getName() == song_mock2.artist)
                    found_artist2 = true;
            }

            CHECK(found_artist1 == true);
            CHECK(found_artist2 == true);

            clearTestEnvironment();
        }

        SUBCASE("Organizar musicas de mesmo autor de usuários diferentes") {
            core::User user1("usertest1",
                             config.userMusicDirectory(),
                             config.inputPublicPath(),
                             1001);

            core::User user2("usertest2",
                             config.userMusicDirectory(),
                             config.inputPublicPath(),
                             1002);

            user_repo->save(user1);
            user_repo->save(user2);

            const MediaFixture::SongTestMock song_mock1 =
                media.getSongTestMock("Short_Song_Test_The_Testers");
            boost::filesystem::copy(song_mock1.path, user1.getInputPath());
            const MediaFixture::SongTestMock song_mock2 =
                media.getSongTestMock("Medium_Song_Test_The_Testers");
            boost::filesystem::copy(song_mock2.path, user2.getInputPath());
            manager.update();

            std::string song1_path_registre =
                user1.getInputPath() + song_mock1.artist + "/"
                + song_mock1.album + "/" + song_mock1.title + ".mp3";
            std::string song2_path_registre =
                user2.getInputPath() + song_mock2.artist + "/"
                + song_mock2.album + "/" + song_mock2.title + ".mp3";

            CHECK(boost::filesystem::exists(song1_path_registre));
            CHECK(boost::filesystem::exists(song2_path_registre));
            checkSongInDatabase(song_mock1, user1);
            checkSongInDatabase(song_mock2, user2);

            auto artists = artist_repo->getAll();
            CHECK(artist_repo->count() == 1);

            clearTestEnvironment();
        }
    }
}
