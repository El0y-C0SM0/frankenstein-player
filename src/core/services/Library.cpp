#include "core/services/Library.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/Album.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/Playlist.hpp"
#include "core/services/UsersManager.hpp"
#include "core/bd/DatabaseManager.hpp"

namespace core
{
    Library::Library(std::shared_ptr<core::User> _user, std::shared_ptr<SQLite::Database> db) : _user(_user)
    {
        core::RepositoryFactory repo_factory(db);
        _songRepo = repo_factory.createSongRepository();
        _albumRepo = repo_factory.createAlbumRepository();
        _artistRepo = repo_factory.createArtistRepository();
        _playlistRepo = repo_factory.createPlaylistRepository();
    }

    Library::Library(const User &user, SQLite::Database &db) : _user(std::make_shared<User>(user))
    {
        RepositoryFactory repo_factory(std::shared_ptr<SQLite::Database>(&db, [](SQLite::Database*){}));
        _songRepo = repo_factory.createSongRepository();
        _albumRepo = repo_factory.createAlbumRepository();
        _artistRepo = repo_factory.createArtistRepository();
        _playlistRepo = repo_factory.createPlaylistRepository();
    }

    Library::Library(ConfigManager &config)
    {
        DatabaseManager db_manager(config.databasePath(), config.databaseSchemaPath());
        RepositoryFactory repo_factory(db_manager.getDatabase());
        _songRepo = repo_factory.createSongRepository();
        _albumRepo = repo_factory.createAlbumRepository();
        _artistRepo = repo_factory.createArtistRepository();
        _playlistRepo = repo_factory.createPlaylistRepository();

        UsersManager users_manager(config, *db_manager.getDatabase());
        _public_user = users_manager.getPublicUser();
        _user = users_manager.getCurrentUser();
    }

    Library::Library(ConfigManager &config, SQLite::Database &db)
    {
        RepositoryFactory repo_factory(std::shared_ptr<SQLite::Database>(&db, [](SQLite::Database*){}));
        _songRepo = repo_factory.createSongRepository();
        _albumRepo = repo_factory.createAlbumRepository();
        _artistRepo = repo_factory.createArtistRepository();
        _playlistRepo = repo_factory.createPlaylistRepository();

        UsersManager users_manager(config, db);
        _public_user = users_manager.getPublicUser();
        _user = users_manager.getCurrentUser();
    }

    Library::~Library() {}

    std::shared_ptr<core::SongRepository> Library::getSongRepository() const
    {
        return _songRepo;
    }

    std::shared_ptr<core::ArtistRepository> Library::getArtistRepository() const
    {
        return _artistRepo;
    }

    std::shared_ptr<core::AlbumRepository> Library::getAlbumRepository() const
    {
        return _albumRepo;
    }

    std::shared_ptr<core::PlaylistRepository> Library::getPlaylistRepository() const
    {
        return _playlistRepo;
    }

    bool Library::addToPlaylist(const core::IPlayable &playlist, const core::IPlayable &playable)
    {
        auto pl = dynamic_cast<const Playlist *>(&playlist);
        auto song = dynamic_cast<const Song *>(&playable);
        if (pl && song)
        {
            // return _playlistRepo->addSongToPlaylist(*pl, *song);
            return false;
        }

        return false;
    }

    bool Library::removeFromPlaylist(const core::IPlayable &playlist, const core::IPlayable &playable)
    {
        auto pl = dynamic_cast<const Playlist *>(&playlist);
        auto song = dynamic_cast<const Song *>(&playable);
        if (pl && song)
        {
            // return _playlistRepo->removeSongFromPlaylist(*pl, *song);
            return false;
        }
        return false;
    }

    std::vector<std::shared_ptr<core::Song>> Library::searchSong(const std::string &query) const {
        return _songRepo->findByTitleAndUser(query, *_user);
    }

    std::vector<std::shared_ptr<core::Artist>> Library::searchArtist(const std::string &query) const {
        return _artistRepo->findByNameAndUser(query, *_user);
    }

    std::vector<std::shared_ptr<core::Album>> Library::searchAlbum(const std::string &query) const {
        return _albumRepo->findByTitleAndUser(query, *_user);
    }

    std::vector<std::shared_ptr<core::Playlist>> Library::searchPlaylist(const std::string &query) const {
        return _playlistRepo->findByTitleAndUser(query, *_user);
    }

    bool Library::persist(Song &song) {
        return _songRepo->save(song);
    }

    bool Library::persist(Artist &artist) {
        return _artistRepo->save(artist);
    }

    bool Library::persist(Album &album) {
        return _albumRepo->save(album);
    }

    bool Library::persist(Playlist &playlist) {
        return _playlistRepo->save(playlist);
    }

    std::shared_ptr<core::User> Library::getUser() const {
        return _user;
    }
}
