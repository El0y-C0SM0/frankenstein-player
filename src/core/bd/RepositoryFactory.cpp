/**
 * @file RepositoryFactory.cpp
 * @brief Implementação
 *
 * @ingroup bd
 * @author Eloy Maciel
 * @date 2025-11-05
 */


#include "core/bd/RepositoryFactory.hpp"
#include "core/bd/ArtistRepository.hpp"


namespace core {
    RepositoryFactory::RepositoryFactory(
        std::shared_ptr<SQLite::Database> db)
        : _db(db) {}

    RepositoryFactory::~RepositoryFactory() {
        _db.reset();
    }

    std::unique_ptr<core::ArtistRepository> RepositoryFactory::createArtistRepository() {
        return std::unique_ptr<core::ArtistRepository>(new core::ArtistRepository(_db));
    }

    std::unique_ptr<core::AlbumRepository> RepositoryFactory::createAlbumRepository() {
        return std::unique_ptr<core::AlbumRepository>(new core::AlbumRepository(_db));
    }

    std::unique_ptr<core::SongRepository> RepositoryFactory::createSongRepository() {
        return std::unique_ptr<core::SongRepository>(new core::SongRepository(_db));
    }

    std::unique_ptr<core::PlaylistRepository> RepositoryFactory::createPlaylistRepository() {
        return std::unique_ptr<core::PlaylistRepository>(new core::PlaylistRepository(_db));
    }

    std::unique_ptr<core::HistoryPlaybackRepository> RepositoryFactory::createHistoryPlaybackRepository() {
        return std::unique_ptr<core::HistoryPlaybackRepository>(new core::HistoryPlaybackRepository(_db));
    }

    std::unique_ptr<core::UserRepository> RepositoryFactory::createUserRepository() {
        return std::unique_ptr<core::UserRepository>(new core::UserRepository(_db));
    }
}
