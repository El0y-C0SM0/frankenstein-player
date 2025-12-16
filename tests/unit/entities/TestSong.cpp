#include <doctest/doctest.h>
#include <memory>
#include <vector>

#include "core/entities/Song.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/Album.hpp"
#include "core/entities/User.hpp"

TEST_SUITE("Unit Tests - Entity: Song") {
    class FixtureSong {
    public:
        core::Artist artist1, artist2;
        std::vector<core::Artist> featuringArtists;
        core::Album album1, album2;
        core::User user;

        FixtureSong() {
            user = core::User("user");
            user.setId(1);
            user.setHomePath("test_home");

            artist1 = core::Artist(1, "Artist A", user);
            artist2 = core::Artist(2, "Artist B", user);
            album1 = core::Album(1, "Album A", 2020, "Pop", artist1, user);
            album2 = core::Album(2, "Album B", 2021, "Rock", artist1, user);

            featuringArtists.emplace_back("Feat1", "Rap");
            featuringArtists.emplace_back("Feat2", "Rock");
        }

        std::shared_ptr<core::Artist> getArtist() {
            return std::make_shared<core::Artist>(artist1);
        }

        std::vector<std::shared_ptr<core::Artist>> getFeaturingArtists() {
            std::vector<std::shared_ptr<core::Artist>> feats;
            for (const auto &a : featuringArtists) {
                feats.push_back(std::make_shared<core::Artist>(a));
            }
            return feats;
        }

        std::shared_ptr<core::Album> getAlbum() {
            return std::make_shared<core::Album>(album1);
        }
    };

    TEST_CASE_FIXTURE(FixtureSong, "Song: construtor e getters básicos") {
        SUBCASE("Construtor padrão") {
            core::Song s;

            CHECK(s.getTitle().empty());
            CHECK(s.getDuration() == 0);
            CHECK(s.getTrackNumber() == 0);
            CHECK(s.getUser() == nullptr);
            CHECK(s.getGenre().empty());
            CHECK(s.getYear() == 0);
        }

        SUBCASE("Construtor com titulo, artista e álbum") {
            core::Song s("Song X", this->artist1, this->album1);
            s.setAlbumLoader([this]() { return this->getAlbum(); });
            s.setArtistLoader([this]() { return this->getArtist(); });

            CHECK(s.getTitle() == "Song X");
            CHECK(s.getArtist() != nullptr);
            CHECK(s.getArtist()->getName() == this->artist1.getName());
            CHECK(s.getAlbum() != nullptr);
            CHECK(s.getAlbum()->getTitle() == this->album1.getTitle());
        }

        SUBCASE("Construtor completo") {
            core::Song s("Full Song", this->artist1, this->album1, this->user);
            s.setAlbumLoader([this]() { return this->getAlbum(); });
            s.setArtistLoader([this]() { return this->getArtist(); });

            CHECK(s.getTitle() == "Full Song");
            CHECK(s.getArtist() != nullptr);
            CHECK(s.getArtist()->getName() == this->artist1.getName());
            CHECK(s.getAlbum() != nullptr);
            CHECK(s.getAlbum()->getTitle() == this->album1.getTitle());
            CHECK(s.getUser() != nullptr);
            CHECK(s.getUser()->getUsername() == this->user.getUsername());
        }
    }

    TEST_CASE_FIXTURE(FixtureSong, "Song: setters básicos") {
        core::Song s;

        s.setTitle("NewTitle");
        CHECK(s.getTitle() == "NewTitle");
        CHECK_THROWS(s.setTitle(""));

        s.setGenre("Metal");
        CHECK(s.getGenre() == "Metal");

        s.setYear(2000);
        CHECK(s.getYear() == 2000);

        s.setDuration(320);
        CHECK(s.getDuration() == 320);

        s.setTrackNumber(7);
        CHECK(s.getTrackNumber() == 7);

        s.setUser(this->user);
        CHECK(s.getUser() != nullptr);
        CHECK(s.getUser()->getUsername() == this->user.getUsername());
    }

    TEST_CASE_FIXTURE(FixtureSong, "Song: featuring artists básico") {
        core::Song s;
        s.setAlbumLoader([this]() { return this->getAlbum(); });
        s.setArtistLoader([this]() { return this->getArtist(); });
        s.setFeaturingArtistsLoader(
            [this]() { return this->getFeaturingArtists(); });

        auto feats = s.getFeaturingArtists();
        CHECK(feats.size() == this->featuringArtists.size());
        CHECK(feats[0]->getName() == "Feat1");
        CHECK(feats[1]->getName() == "Feat2");

        core::Artist feat3("Feat3", "Jazz");

        s.setFeaturingArtists({feat3});
        s.setFeaturingArtistsLoader(
            [this, feat3]() {
                auto r = this->getFeaturingArtists();
                r.push_back(std::make_shared<core::Artist>(feat3));
                return r;
        });
        feats = s.getFeaturingArtists();
        CHECK(feats.size() == this->featuringArtists.size() + 1);
    }


    TEST_CASE_FIXTURE(FixtureSong, "Song: comparações") {
        core::Song song1("Song A", this->artist1, this->album1, this->user);
        song1.setId(1);
        song1.setTrackNumber(1);
        song1.setAlbumLoader([this]() { return this->getAlbum(); });
        song1.setArtistLoader([this]() { return this->getArtist(); });

        core::Song song2("Song B", this->artist1, this->album1, this->user);
        song2.setId(2);
        song2.setTrackNumber(2);
        song2.setAlbumLoader([this]() { return this->getAlbum(); });
        song2.setArtistLoader([this]() { return this->getArtist(); });

        CHECK(song1 < song2);
        CHECK(song1 <= song2);
        CHECK_FALSE(song1 > song2);
        CHECK_FALSE(song1 >= song2);

        core::Song song3("Song A", this->artist1, this->album1, this->user);
        song3.setId(1);
        song3.setTrackNumber(1);
        song3.setAlbumLoader([this]() { return this->getAlbum(); });
        song3.setArtistLoader([this]() { return this->getArtist(); });

        CHECK_NE(song1, song2);
        CHECK_EQ(song1, song3);
        CHECK_FALSE(song1 < song3);
        CHECK(song1 <= song3);
        CHECK_FALSE(song1 > song3);
        CHECK(song1 >= song3);

        core::Song song4("Song C", this->artist1, this->album2, this->user);
        song4.setId(3);
        song4.setTrackNumber(1);
        song4.setAlbumLoader([this]() { return std::make_shared<core::Album>(this->album2); });
        song4.setArtistLoader([this]() { return this->getArtist(); });

        CHECK_LT(song1, song4);
        CHECK_LT(song2, song4);
        CHECK_LE(song2, song4);
        CHECK_FALSE(song4 < song1);
        CHECK_FALSE(song4 <= song1);
    }

    TEST_CASE_FIXTURE(FixtureSong, "Song: IPlayable e IPlaybleObject herança") {
        SUBCASE("IPlayable") {
            core::Song s("Song", this->artist1, this->album1);
            s.setAlbumLoader([this]() { return this->getAlbum(); });
            s.setArtistLoader([this]() { return this->getArtist(); });

            auto playableObjects = s.getPlayableObjects();
            CHECK(playableObjects.size() == 1);
        }

        SUBCASE("IPlayableObject") {
            core::Song s1("Song", this->artist1, this->album1);
            s1.setUser(this->user);
            s1.setAlbumLoader([this]() { return this->getAlbum(); });
            s1.setArtistLoader([this]() { return this->getArtist(); });

            std::shared_ptr<core::IPlayableObject> playableObject1 = std::make_shared<core::Song>(s1);

            std::string expectedPath1 = this->user.getHomePath() +
                this->artist1.getName() + "/" +
                this->album1.getTitle() + "/" +
                s1.getTitle() + ".mp3";
            CHECK_EQ(playableObject1->getAudioFilePath(), expectedPath1);

            core::Song s2(1, "Single Song", this->artist2.getId());
            s2.setUser(this->user);
            s2.setArtistLoader([this]() { return std::make_shared<core::Artist>(this->artist2); });
            s2.setAlbumLoader([this]() { return nullptr; });

            std::shared_ptr<core::IPlayableObject> playableObject2 = std::make_shared<core::Song>(s2);

            std::string expectedPath2 = this->user.getHomePath() +
                this->artist2.getName() + "/" +
                core::SINGLE_ALBUM +
                s2.getTitle() + ".mp3";
            CHECK_EQ(playableObject2->getAudioFilePath(), expectedPath2);
        }
    }
}
