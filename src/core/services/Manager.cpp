#include "core/services/Manager.hpp"

namespace core {
    Manager::Manager(ConfigManager& config,
                    std::shared_ptr<SongRepository> songRepo,
                    std::shared_ptr<ArtistRepository> artistRepo,
                    std::shared_ptr<AlbumRepository> albumRepo)
        : _config(config)
        , _songRepo(songRepo)
        , _artistRepo(artistRepo)
        , _albumRepo(albumRepo)
        , _usersManager(UsersManager(config)) {}

    void Manager::move(std::string filePath, std::string newFilePath) {
        fs::path source(filePath);
        fs::path destination(newFilePath);
        
        fs::create_directories(destination.parent_path());
        
        fs::rename(source, destination);
    }

    std::shared_ptr<Song> Manager::readMetadata(TagLib::FileRef file) {
        if(file.isNull() || !file.tag()) {
            std::cout << "Nenhum metadado encontrado" << std::endl;
        }

        TagLib::Tag *tag = file.tag();
        
        std::shared_ptr<Song> song = std::make_shared<Song>();
        song->setTitle(tag->title().isEmpty() ? "Unknown Title" : tag->title().toCString());
        
        std::string artistName = tag->artist().isEmpty() ? "Unknown Artist" : tag->artist().toCString();
        std::vector<std::shared_ptr<Artist>> artists = _artistRepo->findByName(artistName);
        std::shared_ptr<Artist> artist;
        
        if (artists.empty()) {
            artist = std::make_shared<Artist>(artistName);
            _artistRepo->save(*artist);
        } else {
            artist = artists[0];
        }
        song->setArtist(artist);
        
        std::string albumTitle = tag->album().isEmpty() ? "Unknown Album" : tag->album().toCString();
        std::vector<std::shared_ptr<Album>> albums = _albumRepo->findByArtist(artistName);
        std::shared_ptr<Album> album;
        
        bool albumFound = false;
        for (const std::shared_ptr<Album>& existingAlbum : albums) {
            if (existingAlbum->getName() == albumTitle) {
                album = existingAlbum;
                albumFound = true;
                break;
            }
        }
        
        if (!albumFound) {
            album = std::make_shared<Album>(albumTitle, artist);
            _albumRepo->save(*album);
        }
        song->setAlbum(*album);
        
        return song;
    }

    void Manager::verifyDir(std::string path) {
        fs::path dir(path);
        if (!fs::exists(dir)) {
            fs::create_directories(dir);
        }
    }

    void Manager::update() {
        std::shared_ptr<User> currentUser = _usersManager.getCurrentUser();
        std::shared_ptr<User> publicUser = _usersManager.getPublicUser();
        
        std::string userInput = currentUser.get()->getInputPath();
        std::string publicInput = publicUser.get()->getInputPath();

        verifyDir(userInput);
        verifyDir(publicInput);

        std::string inputDirs[] = { userInput, publicInput };
        
        for (const auto& inputDir : inputDirs) {
            if (!fs::exists(inputDir)) {
                continue;
            }

            for (const auto& entry : fs::directory_iterator(inputDir)) {
                if (!entry.is_regular_file()) {
                    continue;
                }

                TagLib::FileRef probe(entry.path().string().c_str());
                if (probe.isNull() || !probe.audioProperties()) {
                    continue;
                }

                std::string filePath = entry.path().string();
                
                TagLib::FileRef f(filePath.c_str());
                if (!f.isNull()) {

                    std::shared_ptr<Song> song = readMetadata(f);
                    _songRepo->save(*song);
                    
                    move(filePath, song->getAudioFilePath());
                        
                }
            }
        }
    }

    bool Manager::isUpdated() {
        std::shared_ptr<User> currentUser = _usersManager.getCurrentUser();
        std::shared_ptr<User> publicUser = _usersManager.getPublicUser();
        
        std::string userInput = currentUser.get()->getInputPath();
        std::string publicInput = publicUser.get()->getInputPath();

        verifyDir(userInput);
        verifyDir(publicInput);

        std::string inputDirs[] = { userInput, publicInput };
        
        for (const auto& dir : inputDirs) {
            if (!fs::exists(dir)) {
                continue;
            }
            
            for (const auto& entry : fs::directory_iterator(dir)) {
                if (entry.is_regular_file()) {
                    return false;
                }
            }
        }
        
        return true;
    }

    void Manager::defineArtist(int songId, int artistId) {
        std::cout << "Método fora do escopo do MVP" << std::endl; 
    }

}