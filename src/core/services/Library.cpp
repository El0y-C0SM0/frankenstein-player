#include "core/services/Library.hpp"
#include "core/entities/Song.hpp"
#include "core/entities/Album.hpp"
#include "core/entities/Artist.hpp"
#include "core/entities/Playlist.hpp"

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

    std::vector<std::shared_ptr<core::Song>> Library::searchSong(const std::string &query) const
    {
        auto songs = _songRepo->findByTitleAndUser(query, *_user);
        if (songs.empty())
        {
            std::cout << "Nenhuma música encontrada com o termo: " << query << std::endl;
        }

        return songs;
    }

    std::vector<std::shared_ptr<core::Artist>> Library::searchArtist(const std::string &query) const
    {
        auto artists = _artistRepo->findByNameAndUser(query, *_user);
        if (artists.empty())
        {
            std::cout << "Nenhum artista encontrado com o termo: " << query << std::endl;
        }

        return artists;
    }

    std::vector<std::shared_ptr<core::Album>> Library::searchAlbum(const std::string &query) const
    {
        auto albums = _albumRepo->findByTitleAndUser(query, *_user);
        if (albums.empty())
        {
            std::cout << "Nenhum álbum encontrado com o termo: " << query << std::endl;
        }

        return albums;
    }

    std::vector<std::shared_ptr<core::Playlist>> Library::searchPlaylist(const std::string &query) const
    {
        auto playlists = _playlistRepo->findByTitleAndUser(query, *_user);
        if (playlists.empty())
        {
            std::cout << "Nenhuma playlist encontrada com o termo: " << query << std::endl;
        }

        return playlists;
    }
}
