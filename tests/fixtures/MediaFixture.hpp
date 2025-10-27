#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdio>


class MediaFixture {
private:
    std::string _test_media_path;
    std::map<std::string, std::string> _test_media_env;
    std::vector<std::string> _test_files;

    bool existsFile(const std::string &file_path) const {
        FILE *file = fopen(file_path.c_str(), "r");
        if (file) {
            fclose(file);
            return true;
        }
        return false;
    }

    bool existTestFiles() {
        // TODO
        return true;
    }

    bool createTestFiles() {
        // TODO
        return true;
    }

public:
    MediaFixture() {
        _test_media_env["SHORT_TEST_SONG"] = "short_test_song.mp3";
        _test_media_env["MEDIUM_TEST_SONG"] = "medium_test_song.mp3";
        _test_media_env["TEST_ALBUM"] = "Test Album";
        _test_media_env["TEST_ARTIST"] = "The Testers";

        createTestFiles();
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
};
