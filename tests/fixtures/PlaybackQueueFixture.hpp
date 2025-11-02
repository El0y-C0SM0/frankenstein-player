#pragma once

#include "core/bd/HistoryPlaybackRepository.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"
#include <memory>

/**
 * @brief Fixture para criar objetos de teste
 */
class PlaybackQueueFixture {
public:
    std::shared_ptr<core::User> user;
    std::shared_ptr<core::HistoryPlaybackRepository> history_repo;

    PlaybackQueueFixture() {
        user = std::make_shared<core::User>("username");
        std::shared_ptr<core::HistoryPlaybackRepository> history_repo = nullptr;
    };

    std::shared_ptr<core::Song> createSong(const std::string& title) {
        auto song = std::make_shared<core::Song>();
        song->setTitle(title);
        song->setDuration(200);
        return song;
    }
};
