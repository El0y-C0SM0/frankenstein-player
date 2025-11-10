/**
 * @file Collection.hpp
 * @brief Define métodos para coleções de IPlayable
 */
#pragma once
#include "core/interfaces/IPlayable.hpp"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace core {
    class Song;
    class ICollection {
    public:
        virtual ~ICollection() = default;
        /**
         * @brief Obtém a lista de músicas da coleção
         * @return Vector com as músicas da coleção em ordem
         */
        // virtual std::vector<std::shared_ptr<IPlayable>> getSongs() const = 0;

        virtual std::vector<std::shared_ptr<Song>> getSongs() const = 0;

        virtual void setSongsLoader(
            const std::function<std::vector<std::shared_ptr<Song>>()>&
                loader) = 0;

        virtual void addSong(std::shared_ptr<IPlayable> song) = 0;

        virtual bool switchSong(unsigned id, unsigned index) = 0;

        virtual bool removeSong(unsigned id) = 0;

        virtual std::shared_ptr<IPlayable> findSongById(unsigned songId) = 0;

        virtual std::shared_ptr<IPlayable>
        findSongByTitle(const std::string& title) = 0;

        virtual int calculateTotalDuration() = 0;

        virtual std::string getFormattedDuration() = 0;

        virtual std::shared_ptr<IPlayable>
        getNextSong(std::shared_ptr<IPlayable> current) = 0;

        virtual std::shared_ptr<IPlayable>
        getPreviousSong(std::shared_ptr<IPlayable> current) = 0;

        virtual std::shared_ptr<IPlayable> getSongAt(int index) = 0;
    };
};  // namespace core
