#include <doctest/doctest.h>

#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "core/bd/DatabaseManager.hpp"
#include "core/bd/SongRepository.hpp"
#include "core/entities/Song.hpp"

#include "fixtures/ConfigFixture.hpp"

TEST_SUITE("Unit Tests - core::SongRepository") {
    std::unique_ptr<core::DatabaseManager> createTempDB() {
        ConfigFixture config;
        std::string db_path = config.databasePath();
        std::string schema_path = config.databaseSchemaPath();

        return std::unique_ptr<core::DatabaseManager>(new core::DatabaseManager(db_path, schema_path));
    }

    TEST_CASE("SongRepository: Inserir e remover") {
        core::SongRepository repo(createTempDB()->getDatabase());

        core::Song song1 = core::Song(1, "media/test/music1.mp3", "Música teste 1", 1);
        core::Song song2 = core::Song(2, "media/test/music2.mp3", "Minha Música", 1);
        core::Song song3 = core::Song(3, "media/test/music3.mp3", "Música teste 2", 2);

        CHECK(repo.count() == 0);
        CHECK(repo.save(song1) == true);
        CHECK(repo.count() == 1);

        CHECK(repo.save(song2) == true);
        CHECK(repo.count() == 2);

        CHECK(repo.remove(3) == false);
        CHECK(repo.count() == 2);
        CHECK(repo.remove(2) == true);
        CHECK(repo.count() == 1);
    }

    TEST_CASE("SongRepository: Buscar") {
        core::SongRepository repo(createTempDB()->getDatabase());

        core::Song song1 = core::Song(1, "media/test/music1.mp3", "Música teste 1", 1);
        core::Song song2 = core::Song(2, "media/test/music2.mp3", "Minha Música", 1);
        core::Song song3 = core::Song(3, "media/test/music3.mp3", "Música teste 2", 2);

        CHECK(repo.count() == 0);
        CHECK(repo.save(song1) == true);
        CHECK(repo.save(song2) == true);

        std::shared_ptr<core::Song> ptr = repo.findById(2);
        CHECK(ptr != nullptr);
        CHECK(ptr->getTitle() == "Minha Música");
        CHECK(ptr->getArtist() != nullptr);
        CHECK(ptr->getFilePath() == "media/test/music2.mp3");

        CHECK(repo.findById(3) == nullptr);
        CHECK(repo.save(song3) == true);
        CHECK(repo.findById(3) != nullptr);
    }

    TEST_CASE("SongRepository: Get All") {
        core::SongRepository repo(createTempDB()->getDatabase());

        core::Song song1 = core::Song(1, "media/test/music1.mp3", "Música teste 1", 1);
        core::Song song2 = core::Song(2, "media/test/music2.mp3", "Minha Música", 1);
        core::Song song3 = core::Song(3, "media/test/music3.mp3", "Música teste 2", 2);

        CHECK(repo.save(song1) == true);
        CHECK(repo.save(song2) == true);

        std::vector<std::shared_ptr<core::Song>> vector = repo.getAll();
        CHECK(vector.size() == 2);
        CHECK(vector.end()->get()->getTitle() == "Minha Música");

        CHECK(repo.save(song3) == true);
        vector = repo.getAll();
        CHECK(vector.size() == 3);
        CHECK(vector.end()->get()->getTitle() == "Música teste 2");
        CHECK(vector.begin()->get()->getTitle() == "Música teste 1");
    }
}
