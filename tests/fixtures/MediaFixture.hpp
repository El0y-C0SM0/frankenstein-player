#pragma once

#include <string>
#include <vector>
#include <map>
#include <boost/filesystem.hpp>

class MediaFixture {
public:
struct SongTestMock {
    std::string path;
    int duracao;
    std::string title;
    std::string artist;
    std::string album;
    std::string year;
    std::string genre;
    std::string track;
};
private:
    std::string _test_media_path;
    std::map<std::string, std::string> _test_media_env;
    std::vector<std::string> _test_files;
    std::map<std::string, SongTestMock> _test_song_mocks;

    bool existsFile(const std::string &file_path) const {
        return boost::filesystem::exists(file_path);
    }

public:

    MediaFixture() {
        _test_media_env["SHORT_TEST_SONG"] = "short_test_song1.mp3";
        _test_media_env["MEDIUM_TEST_SONG"] = "medium_test_song1.mp3";
        _test_media_env["TEST_ALBUM"] = "Test Album";
        _test_media_env["TEST_ARTIST"] = "The Testers";

        _test_media_path = boost::filesystem::current_path().string() + "/tests/fixtures/media/";
        _test_files = {
            _test_media_path + _test_media_env["SHORT_TEST_SONG"],
            _test_media_path + _test_media_env["MEDIUM_TEST_SONG"]
        };

        std::map<std::string, SongTestMock> test_song_mocks;
        _test_song_mocks["Short_Song_Test_The_Testers"] = SongTestMock{
            _test_media_path + "Short_Song_Test_The_Testers.mp3",
            1,
            "Short Song",
            "The Testers",
            "Test",
            "2025",
            "Testing",
            "1/2"
        };
        _test_song_mocks["Medium_Song_Test_The_Testers"] = SongTestMock{
            _test_media_path + "Medium_Song_Test_The_Testers.mp3",
            4,
            "Medium Song",
            "The Testers",
            "Test",
            "2025",
            "Testing",
            "2/2"
        };
        _test_song_mocks["Short_Song_Examples_Example_Band"] = SongTestMock{
            _test_media_path + "Short_Song_Example_Band.mp3",
            2,
            "Short Song",
            "Example Band",
            "Examples",
            "2025",
            "Example Genre",
            "1/2"
        };
        _test_song_mocks["Medium_Song_Examples_Example_Band"] = SongTestMock{
            _test_media_path + "Medium_Song_Example_Band.mp3",
            5,
            "Medium Song",
            "Example Band",
            "Examples",
            "2025",
            "Example Genre",
            "2/2"
        };
        _test_song_mocks["Song_Test_No_Artist"] = SongTestMock{
            _test_media_path + "Song_Test_No_Artist.mp3",
            3,
            "Song Test",
            "",
            "No Album",
            "2025",
            "No Genre",
            "1/1"
        };
        _test_song_mocks["Song_Test_No_Album"] = SongTestMock{
            _test_media_path + "Song_Test_No_Album.mp3",
            3,
            "Song Test",
            "No Artist",
            "",
            "2025",
            "No Genre",
            "1/1"
        };
    }

    const std::string getShortTestSongFileName() const {
        return _test_media_env.at("SHORT_TEST_SONG");
    }

    const std::string getMediumTestSongFileName() const {
        return _test_media_env.at("MEDIUM_TEST_SONG");
    }

    const std::string getAlbumTestName() const {
        return _test_media_env.at("TEST_ALBUM");
    }

    const std::string getArtistTestName() const {
        return _test_media_env.at("TEST_ARTIST");
    }

    const SongTestMock getSongTestMock(const std::string &file_key) const {
        return _test_song_mocks.at(file_key);
    }
};
