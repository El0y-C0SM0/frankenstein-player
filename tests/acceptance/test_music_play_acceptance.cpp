#include <doctest/doctest.h>

#include <boost/filesystem.hpp>
#include <memory>
#include <string>
#include <vector>

#include "core/entities/Album.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"
#include "core/services/PlaybackQueue.hpp"
#include "core/services/Player.hpp"

#include "fixtures/ConfigFixture.hpp"  // TODO corrigir include
#include "fixtures/MediaFixture.hpp"

TEST_SUITE("HISTÓRIA DE USUÁRIO: Reprodução de Música") {
    ConfigFixture config = ConfigFixture();
    MediaFixture media = MediaFixture();

    class PlayerFixture {
    public:
        std::shared_ptr<core::Artist> artist;
        std::shared_ptr<core::Album> album;
        std::shared_ptr<core::Song> short_song;
        std::shared_ptr<core::Song> medium_song;
        MediaFixture::SongTestMock short_song_mock, medium_song_mock;

        PlayerFixture() {
            auto user = std::make_shared<core::User>("test_user");
            user->setHomePath(config.userMusicDirectory());
            short_song_mock =
                media.getSongTestMock("Short_Song_Test_The_Testers");
            medium_song_mock =
                media.getSongTestMock("Medium_Song_Test_The_Testers");

            artist =
                std::make_shared<core::Artist>(short_song_mock.artist, "Test");
            album = std::make_shared<core::Album>(short_song_mock.album,
                                                  artist->getName(),
                                                  artist->getGenre());

            short_song = std::make_shared<core::Song>(short_song_mock.title,
                                                      *album,
                                                      *artist,
                                                      *user);
            medium_song = std::make_shared<core::Song>(medium_song_mock.title,
                                                       *album,
                                                       *artist,
                                                       *user);

            album->setSongsLoader([this]() {
                return std::vector<std::shared_ptr<core::IPlayable>> {
                    short_song,
                    medium_song};
            });

            artist->setSongsLoader([this]() {
                return std::vector<std::shared_ptr<core::IPlayable>> {
                    short_song,
                    medium_song};
            });

            boost::filesystem::create_directories(
                short_song->getAudioFilePath());
            boost::filesystem::create_directories(
                medium_song->getAudioFilePath());

            boost::filesystem::copy_file(
                short_song_mock.path,
                short_song->getAudioFilePath(),
                boost::filesystem::copy_option::overwrite_if_exists);
            boost::filesystem::copy_file(
                medium_song_mock.path,
                medium_song->getAudioFilePath(),
                boost::filesystem::copy_option::overwrite_if_exists);
        }

        ~PlayerFixture() {
            boost::filesystem::remove(short_song->getAudioFilePath());
            boost::filesystem::remove(medium_song->getAudioFilePath());
        }
    };

    TEST_CASE_FIXTURE(
        PlayerFixture,
        "CT-AC-01: Reproduzir uma música/album/artista com sucesso") {
        SUBCASE("Reproduzir música individualmente") {
            core::PlaybackQueue queue;
            queue += *short_song;
            core::Player player(queue);

            player.play();
            CHECK(player.isPlaying() == true);

            std::shared_ptr<const core::Song> cur_song =
                player.getPlaybackQueue()->getCurrentSong();
            CHECK(cur_song != nullptr);
            CHECK(*cur_song == *short_song);

            player.pause();
        }

        SUBCASE("Reproduzir um álbum") {
            core::PlaybackQueue queue;
            queue += *album;
            core::Player player(queue);

            player.play();
            CHECK(player.isPlaying() == true);
            std::shared_ptr<const core::Song> cur_song =
                player.getPlaybackQueue()->getCurrentSong();
            CHECK(cur_song != nullptr);
            CHECK(*cur_song == *short_song);

            player.fastForward(2);
            std::shared_ptr<const core::Song> next_song =
                player.getPlaybackQueue()->getCurrentSong();
            CHECK(next_song != nullptr);
            CHECK(*next_song == *medium_song);

            player.pause();
        }

        SUBCASE("Reproduzir um artista") {
            core::PlaybackQueue queue;
            queue += *artist;
            core::Player player(queue);

            player.play();
            CHECK(player.isPlaying() == true);
            std::shared_ptr<const core::Song> cur_song =
                player.getPlaybackQueue()->getCurrentSong();
            CHECK(cur_song != nullptr);
            CHECK(*cur_song == *short_song);

            player.fastForward(2);
            std::shared_ptr<const core::Song> next_song =
                player.getPlaybackQueue()->getCurrentSong();
            CHECK(next_song != nullptr);
            CHECK(*next_song == *medium_song);

            player.pause();
        }

        SUBCASE("Reproduzir nenhum aquivo") {
            core::Player player;
            player.play();

            CHECK(player.isPlaying() == false);
            std::shared_ptr<const core::Song> cur_song =
                player.getPlaybackQueue()->getCurrentSong();
            CHECK(cur_song == nullptr);
        }
    }

    TEST_CASE_FIXTURE(PlayerFixture, "CT-AC-02: Pausar a reprodução") {
        SUBCASE("Pause") {
            core::PlaybackQueue queue;
            queue += *short_song;
            core::Player player(queue);

            player.play();
            CHECK(player.isPlaying() == true);

            player.pause();
            CHECK(player.isPlaying() == false);
            CHECK(player.isPaused() == true);
        }

        SUBCASE("Pause Pause") {
            core::PlaybackQueue queue;
            queue += *short_song;
            core::Player player(queue);

            player.play();
            CHECK(player.isPlaying() == true);

            player.pause();
            CHECK(player.isPlaying() == false);
            CHECK(player.isPaused() == true);

            player.pause();
            CHECK(player.isPlaying() == false);
            CHECK(player.isPaused() == true);
        }

        SUBCASE("Pause Resume") {
            core::PlaybackQueue queue;
            queue += *short_song;
            core::Player player(queue);

            player.play();
            CHECK(player.isPlaying() == true);

            player.pause();
            CHECK(player.isPlaying() == false);
            CHECK(player.isPaused() == true);

            player.resume();
            CHECK(player.isPlaying() == true);
            CHECK(player.isPaused() == false);

            std::shared_ptr<const core::Song> cur_song =
                player.getPlaybackQueue()->getCurrentSong();
            CHECK(cur_song != nullptr);
            CHECK(*cur_song == *short_song);
        }
    }

    TEST_CASE_FIXTURE(PlayerFixture,
                      "CT-AC-03: Manipular posição de reprodução") {

        SUBCASE("Avançar 2 segundos") {
            core::PlaybackQueue queue;
            queue += *medium_song;
            core::Player player(queue);

            player.play();
            CHECK(player.isPlaying() == true);

            unsigned start_time = player.getElapsedTime();

            player.fastForward(2);
            unsigned new_time = player.getElapsedTime();

            CHECK(new_time >= start_time + 2);
            CHECK(player.isPlaying() == true);
        }

        SUBCASE("Retroceder 2 segundos") {
            core::PlaybackQueue queue;
            queue += *medium_song;
            core::Player player(queue);

            player.play();
            CHECK(player.isPlaying() == true);

            player.fastForward(3);
            unsigned start_time = player.getElapsedTime();

            player.rewind(2);
            unsigned new_time = player.getElapsedTime();

            CHECK(new_time <= start_time - 2);
            CHECK(player.isPlaying() == true);
        }

        SUBCASE("Reiniciar música") {
            core::PlaybackQueue queue;
            queue += *medium_song;
            core::Player player(queue);

            player.play();
            CHECK(player.isPlaying() == true);

            player.fastForward(2);
            unsigned new_time = player.getElapsedTime();

            player.restart();
            CHECK(player.isPlaying() == true);

            unsigned restarted_time = player.getElapsedTime();
            CHECK(restarted_time < new_time);
        }

        SUBCASE("Avançar valor inválido") {
            core::PlaybackQueue queue;
            queue += *short_song;
            core::Player player(queue);

            player.play();
            CHECK(player.isPlaying() == true);

            unsigned start_time = player.getElapsedTime();

            CHECK_NOTHROW(player.fastForward(5000));
            CHECK(player.isPlaying() == false);
        }
    }
}
