/**
 * @file Cli.cpp
 * @ingroup cli
 * @author Bruno Vieira
 * @brief Implementação do servico de comunicação com o usuario
 * @version 0.1
 * @date 2025-11-08
 *
 */

#include "cli/Cli.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include "core/bd/DatabaseManager.hpp"

namespace cli {
    std::string trimSpaces(const std::string& str) {
        size_t firstNonSpace = str.find_first_not_of(" \t\n\r\f\v");

        if (firstNonSpace == std::string::npos) {
            return "";
        }

        return str.substr(firstNonSpace);
    }

    Cli::Cli(core::ConfigManager& config_manager)
        : _config(config_manager) {
        try {
            config_manager.loadConfig();
        } catch (const std::exception& e) {
            std::cerr << "Erro ao carregar o arquivo de configuração:\n\t"
                      << e.what() << std::endl;
            throw;
        }

        try {
            // _db =
            // std::make_shared<SQLite::Database>(config_manager.databasePath(),
            // SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            _db_manager =
                core::DatabaseManager(config_manager.databasePath(),
                                      config_manager.databaseSchemaPath());
        } catch (const std::exception& e) {
            std::cerr << "Erro ao conectar ao banco de dados: " << e.what()
                      << std::endl;
            throw;
        }

        _usersManager = std::make_shared<core::UsersManager>(config_manager);
        _usersManager->updateUsersList();

        try {
            _user = _usersManager->getCurrentUser();
        } catch (const std::exception& e) {
            std::cerr << "Erro ao obter o usuário atual:\n\t" << e.what()
                      << std::endl;
            throw;
        }

        try {
            _manager = std::make_shared<core::FilesManager>(config_manager);
        } catch (const std::exception& e) {
            std::cerr << "Erro ao criar o gerenciador principal:\n\t"
                      << e.what() << std::endl;
            throw;
        }

        // std::string username;
        // std::string home_path;
        // std::string input_path;
        // std::string uid;

        _player = std::make_shared<core::Player>();

        _db = _db_manager.getDatabase();
        _library = std::make_shared<core::Library>(_user, _db);

        try {
            std::ifstream helpFile("../resources/help.json");
            helpFile >> _helpData;
        } catch (const std::exception& e) {
            std::cerr
                << "Erro ao carregar o arquivo de ajuda 'assets/help.json': "
                << e.what() << std::endl;
        }
    }

    void Cli::restart() {
        _player->restart();
    }

    void Cli::rewind(unsigned int seconds) {
        _player->rewind(seconds);
    }

    void Cli::forward(unsigned int seconds) {
        _player->fastForward(seconds);
    }

    void Cli::toggleResumePause() {
        if (_player->isPlaying()) {
            _player->pause();
        } else {
            _player->resume();
        }
    }

    void Cli::next() {
        _player->next();
    }

    void Cli::previous() {
        _player->previous();
    }

    void Cli::setVolume(unsigned int volume) {
        float vol = static_cast<float>(volume) / 100.0f;
        if (vol < 0.0f)
            vol = 0.0f;
        if (vol > 1.0f)
            vol = 1.0f;
        _player->setVolume(vol);
    }

    void Cli::setVolumeUpDown(const std::string& volume) {
        if (volume == "up") {
            _player->setVolume(_player->getVolume() + volumeStep);
        } else if (volume == "down") {
            _player->setVolume(_player->getVolume() - volumeStep);
        }
    }

    void Cli::getVolume() const {
        float vol = _player->getVolume() * 100.0f;
        std::cout << "Nivel de volume: " << static_cast<unsigned int>(vol)
                  << std::endl;
    }

    void Cli::toggleMute(const std::string& command) {
        if (command == "mute") {
            if (_player->isMuted()) {
                std::cout << "O player já está mudo." << std::endl;
                return;
            }

            _player->mute();
        } else if (command == "unmute") {
            if (!_player->isMuted()) {
                std::cout << "O player não está no mudo." << std::endl;
                return;
            }

            _player->unmute();
        } else if (command == "toggle_mute") {
            if (_player->isMuted()) {
                _player->unmute();
            } else {
                _player->mute();
            }
        }
    }

    void Cli::getProgress() const {
        _player->getProgress();
    }

    void Cli::clearQueue() {
        _player->clearPlaylist();
    }

    void Cli::loop(const std::string& command) {
        if (command == "on") {
            if (_player->isLooping()) {
                std::cout << "O loop já está ativado." << std::endl;
                return;
            }
            _player->setLooping();
        } else if (command == "off") {
            if (!_player->isLooping()) {
                std::cout << "O loop já está desativado." << std::endl;
                return;
            }
            _player->unsetLooping();
        }
    }

    void Cli::addToQueue(core::IPlayable& playabel) {
        std::cout << "queue adicionar 6" << std::endl;
        try {
            // core::RepositoryFactory repo_factory(_db);

            // auto tracks = core::PlaybackQueue(_user, playabel,
            // repo_factory.createHistoryPlaybackRepository());
            // _player->addPlaybackQueue(tracks);
            _player->getPlaybackQueue()->add(playabel);
            std::cout << "Adicionado à fila de reprodução." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Erro ao adicionar à fila de reprodução: " << e.what()
                      << std::endl;
        }
    }

    void Cli::showQueue() const {
        auto queue = _player->getPlaybackQueue();
        // std::cout << "Fila de reprodução: \n"
        //           << queue->toString();
        std::cout << "Fila de reprodução detalhada: \n";
        for (size_t i = 0; i < queue->size(); ++i) {
            auto song = queue->at(i);
            if (song) {
                std::cout << i + 1 << ". " << song->getTitle() << " - "
                          << song->getArtist()->getName() << "\n";
            }
        }
    }

    void Cli::like() {
        auto curtidas = _library->searchPlaylist("curtidas");
        auto queue = _player->getCurrentQueue();
        addToPlaylist(*curtidas[0], *queue->getCurrentSong());
    }
    void Cli::deslike() {
        auto curtidas = _library->searchPlaylist("curtidas");
        auto queue = _player->getCurrentQueue();
        removeFromPlaylist(*curtidas[0], *queue->getCurrentSong());
    }

    bool Cli::addToPlaylist(const core::IPlayable& playlist,
                            const core::IPlayable& playabel) {
        _library->addToPlaylist(playlist, playabel);

        return true;
    }

    bool Cli::removeFromPlaylist(const core::IPlayable& playlist,
                                 const core::IPlayable& playabel) {
        _library->removeFromPlaylist(playlist, playabel);
        return true;
    }

    void Cli::play() {
        if (_player->getPlaybackQueue()) {
            _player->play();
        }
    }

    void Cli::shuffle() {
        _player->getPlaybackQueue()->shuffle();
    }

    void Cli::removeFromQueue(unsigned idx) {
        _player->getPlaybackQueue()->remove(idx);
    }

    void Cli::showStatus() const {
        try {
            std::cout << "=== Player Status ===" << std::endl;

            std::string state;
            if (_player->isPlaying())
                state = "Playing";
            else if (_player->isPaused())
                state = "Paused";
            else
                state = "Stopped";

            std::cout << "Estado: " << state << std::endl;

            float vol = _player->getVolume() * 100.0f;
            std::cout << "Volume: " << static_cast<unsigned int>(vol) << "%";
            if (_player->isMuted())
                std::cout << " (muted)";
            std::cout << std::endl;

            std::cout << "Loop: " << (_player->isLooping() ? "on" : "off")
                      << std::endl;

            std::shared_ptr<core::PlaybackQueue> queue;

            queue = _player->getPlaybackQueue();

            if (queue) {
                std::cout << "Tamanho da fila: " << queue->size() << std::endl;

                auto curr = queue->getCurrentSong();
                if (curr) {
                    std::cout << "Musica atual: " << curr->getTitle();
                    if (curr->getArtist())
                        std::cout << " - " << curr->getArtist()->getName();
                    std::cout << std::endl;

                    unsigned int elapsed = 0;
                    float progress = 0.0f;

                    elapsed = _player->getElapsedTime();

                    progress = _player->getProgress();

                    if (progress > 0.0f && elapsed > 0) {
                        // unsigned int total = static_cast<unsigned
                        // int>(elapsed / progress); unsigned int m = elapsed /
                        // 60; unsigned int sec = elapsed % 60; char
                        // bufElapsed[32]; std::snprintf(bufElapsed,
                        // sizeof(bufElapsed), "%02u:%02u", m, sec); m = total /
                        // 60; sec = total % 60; char bufTotal[32];
                        // std::snprintf(bufTotal, sizeof(bufTotal),
                        // "%02u:%02u", m, sec);

                        int totalSeconds = elapsed;
                        int h = totalSeconds / 3600;
                        int m = (totalSeconds % 3600) / 60;
                        int s = totalSeconds % 60;

                        std::string formatted;
                        if (h > 0) {
                            formatted =
                                std::to_string(h) + ":" + (m < 10 ? "0" : "")
                                + std::to_string(m) + ":" + (s < 10 ? "0" : "")
                                + std::to_string(s);
                        } else {
                            formatted = std::to_string(m) + ":"
                                        + (s < 10 ? "0" : "")
                                        + std::to_string(s);
                        }

                        auto totalDuration = _player->getCurrentQueue()
                                                 ->getCurrentSong()
                                                 ->getFormattedDuration();

                        std::cout << "Progresso: " << formatted << " / "
                                  << totalDuration << std::endl;
                    } else {
                        std::cout << "Progresso: 0:0/0:0" << std::endl;
                    }
                } else {
                    std::cout << "Nenhuma musica carregada atualmente."
                              << std::endl;
                }

                auto next = queue->getNextSong();
                if (next) {
                    std::cout << "Proxima musica: " << next->getTitle();
                    if (next->getArtist())
                        std::cout << " - " << next->getArtist()->getName();
                    std::cout << std::endl;
                } else {
                    std::cout << "Proxima musica: (nenhuma)" << std::endl;
                }
            } else {
                std::cout << "Fila: N/D" << std::endl;
            }

            std::cout << "======================" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Erro ao obter status do player: " << e.what()
                      << std::endl;
        }
    }

    void Cli::updateUsers() {
        try {
            _usersManager->updateUsersList();
            std::cout << "Lista de usuários atualizada com sucesso."
                      << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Erro ao atualizar a lista de usuários: " << e.what()
                      << std::endl;
        }
    }

    void Cli::updateSongs() {
        try {
            _manager->update();
            std::cout << "Biblioteca atualizada com sucesso." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Erro ao atualizar a biblioteca: " << e.what()
                      << std::endl;
        }
    }

    void Cli::showHelp() const {
        if (_helpData.empty() || !_helpData.contains("commands")) {
            std::cout << "Nenhuma informação de ajuda disponível." << std::endl;
            return;
        }

        std::cout << "Comandos disponíveis:" << std::endl;
        for (auto it = _helpData["commands"].begin();
             it != _helpData["commands"].end();
             ++it) {
            std::cout << "  " << std::left << std::setw(15) << it.key()
                      << it.value().value("description", "") << std::endl;
        }
        std::cout << "\nDigite 'help <comando>' para mais detalhes."
                  << std::endl;
    }

    void Cli::showHelp(const std::string& topic) const {
        if (topic.empty()) {
            showHelp();
            return;
        }

        if (_helpData.empty() || !_helpData.contains("commands")
            || !_helpData["commands"].contains(topic)) {
            std::cout << "Nenhuma ajuda encontrada para o comando '" << topic
                      << "'." << std::endl;
            return;
        }

        const auto& cmd_info = _helpData["commands"][topic];
        std::cout << "Ajuda para o comando: " << topic << std::endl;
        std::cout << "  Descrição: " << cmd_info.value("description", "N/A")
                  << std::endl;
        std::cout << "  Uso: " << cmd_info.value("usage", "N/A") << std::endl;
        if (cmd_info.contains("aliases")) {
            std::cout << "  Apelidos: " << cmd_info["aliases"].dump()
                      << std::endl;
        }
    }

    void Cli::searchSong(const std::string& query) const {
        auto songs = _library->searchSong(query);
        std::cout << "Procurando por músicas com o termo: " << query
                  << std::endl;
        if (songs.empty()) {
            std::cout << "Nenhuma música encontrada para: " << query
                      << std::endl;
            return;
        } else if (songs.size() == 1) {
            std::cout << "1 música encontrada: " << songs.at(0)->getTitle()
                      << std::endl;
        } else {
            std::cout << songs.size() << "Músicas encontradas: \n";
            for (auto song : songs)
                std::cout << song->getTitle() << std::endl;
        }
    }

    void Cli::searchArtist(const std::string& query) const {
        auto artists = _library->searchArtist(query);
        if (artists.empty()) {
            std::cout << "Nenhum artista encontrado para:" << query
                      << std::endl;
            return;
        } else if (artists.size() == 1) {
            std::cout << "1 artista encontrado: " << artists.at(0)->getName()
                      << std::endl;
        } else {
            std::cout << artists.size() << "Artistas encontrados: \n";
            for (auto artist : artists)
                std::cout << artist->getName() << std::endl;
        }
    }

    void Cli::searchAlbum(const std::string& query) const {
        auto albums = _library->searchAlbum(query);
        if (albums.empty()) {
            std::cout << "Nenhum album encontrado para: " << query << std::endl;
            return;
        } else if (albums.size() == 1) {
            std::cout << "1 album encontrado: " << albums.at(0)->getTitle()
                      << std::endl;
        } else {
            std::cout << albums.size() << "Albuns encontrados: \n";
            for (auto album : albums)
                std::cout << album->getTitle() << " por "
                          << album->getArtist()->getName() << std::endl;
        }
    }

    void Cli::searchPlaylist(const std::string& query) const {
        auto playlists = _library->searchPlaylist(query);
        if (playlists.empty()) {
            std::cout << "Nenhuma playlist encontrada para: " << query
                      << std::endl;
            return;
        } else if (playlists.size() == 1) {
            std::cout << "1 playlist encontrada: "
                      << playlists.at(0)->getTitle() << std::endl;
        } else {
            std::cout << playlists.size() << "Playlists encontradas: \n";
            for (auto playlist : playlists)
                std::cout << playlist->getTitle() << std::endl;
        }
    }

    void Cli::showPlaylist(core::IPlayable& playlist) const {
        auto pl = dynamic_cast<core::Playlist*>(&playlist);
        if (pl) {
            std::cout << "Playlist: " << pl->getTitle() << std::endl;
            auto songs = pl->getSongs();
            for (const auto& song : songs) {
                std::cout << "- " << song->getTitle() << std::endl;
            }
        } else {
            std::cout << "O objeto fornecido não é uma playlist válida."
                      << std::endl;
        }
    }

    void Cli::showAlbum(core::IPlayable& album) const {
        auto al = dynamic_cast<core::Album*>(&album);
        if (al) {
            al->toString();
        } else {
            std::cout << "O objeto fornecido não é uma playlist válida."
                      << std::endl;
        }
    }

    void Cli::showArtist(core::IPlayable& artist) const {
        auto ar = dynamic_cast<core::Artist*>(&artist);
        if (ar) {
            ar->toString();
        } else {
            std::cout << "O objeto fornecido não é uma playlist válida."
                      << std::endl;
        }
    }

    void Cli::start() {
        std::string command;
        std::cout << "Bem-vindo ao frankenstein Music Player!" << std::endl;
        std::cout << "Digite 'help' para ver a lista de comandos disponíveis."
                  << std::endl;

        while (true) {
            std::cout << "frankenstein> ";
            std::getline(std::cin, command);

            if (command == "exit" || command == "quit") {
                std::cout << "Saindo do frankenstein Music Player. Até logo!"
                          << std::endl;
                break;
            }

            bool success = doCommand(command);
            if (!success) {
                std::cout << "Digite um comando valido!" << std::endl;
            }
        }
    }

    bool Cli::doCommand(const std::string& command) {
        std::stringstream ss(command);

        std::string firstCommand;
        ss >> firstCommand;

        try {
            if (firstCommand.empty()) {
                std::cout
                    << "Comando vazio. Por favor, insira um comando válido."
                    << std::endl;
                return false;
            } else if (firstCommand == "info" || firstCommand == "status") {
                showStatus();
                return true;
            } else if (firstCommand == "play" || firstCommand == "pause"
                       || firstCommand == "resume") {
                if (firstCommand == "play") {
                    _player->play();
                    return true;
                }

                toggleResumePause();
                return true;
            } else if (firstCommand == "replay" || firstCommand == "restart") {
                restart();
                return true;
            }

            else if (firstCommand == "volume") {
                std::string volumeCommand;
                if (ss >> volumeCommand) {
                    if (volumeCommand == "up" || volumeCommand == "down") {
                        setVolumeUpDown(volumeCommand);
                        return true;
                    } else if (volumeCommand == "set") {
                        unsigned int num;
                        if (ss >> num) {
                            setVolume(num);
                            return true;
                        }
                    }
                    std::cout << "Comando inválido para volume. Use 'volume "
                                 "up', 'volume down' ou 'volume set <value>'."
                              << std::endl;
                    return false;
                }

                getVolume();
                return true;
            } else if (firstCommand == "rewind") {
                unsigned int seconds;
                if (ss >> seconds) {
                    rewind(seconds);
                    return true;
                } else {
                    std::cout << "Por favor, forneça o número de segundos para "
                                 "retroceder."
                              << std::endl;
                    return false;
                }
            } else if (firstCommand == "forward") {
                unsigned int seconds;
                if (ss >> seconds) {
                    forward(seconds);
                    return true;
                }

                std::cout
                    << "Por favor, forneça o número de segundos para avançar."
                    << std::endl;
                return false;
            } else if (firstCommand == "mute" || firstCommand == "unmute"
                       || firstCommand == "toggle_mute") {
                toggleMute(firstCommand);
                return true;
            } else if (firstCommand == "next") {
                next();
                return true;
            } else if (firstCommand == "previous") {
                previous();
                return true;
            } else if (firstCommand == "like") {
                like();
                return true;
            } else if (firstCommand == "deslike") {
                deslike();
                return true;
            } else if (firstCommand == "shuffle") {
                shuffle();
                return true;
            } else if (firstCommand == "loop") {
                std::string loopCommand;
                if (ss >> loopCommand) {
                    if (loopCommand == "on" || loopCommand == "off") {
                        loop(loopCommand);
                        return true;
                    } else {
                        std::cout << "Comando inválido para loop. Use 'loop "
                                     "on' ou 'loop off'."
                                  << std::endl;

                        return false;
                    }
                }
                showHelp("loop");
                return true;
            } else if (firstCommand == "queue") {
                std::string queueCommand;

                if (ss >> queueCommand) {
                    if (queueCommand == "clear") {
                        clearQueue();
                        return true;
                    } else if (queueCommand == "show") {
                        showQueue();
                        return true;
                    } else if (queueCommand == "add") {
                        std::string playable;
                        std::getline(ss, playable);
                        playable = trimSpaces(playable);
                        if (!playable.empty()) {
                            auto opt = _library->searchSong(playable);
                            if (opt.empty()) {
                                std::cout
                                    << "Música não encontrada: " << playable
                                    << std::endl;
                                return false;
                            }

                            auto sp = opt.at(0);
                            addToQueue(*sp);
                            return true;
                        }

                        std::cout << "Por favor, forneça o nome da música para "
                                     "adicionar à fila."
                                  << std::endl;
                        return false;
                    } else if (queueCommand == "remove") {
                        unsigned idx;
                        if (ss >> idx) {
                            removeFromQueue(idx);
                            return true;
                        } else {
                            std::cout << "Por favor, forneça o índice da "
                                         "música para remover da fila."
                                      << std::endl;
                            return false;
                        }
                    } else {
                        std::cout << "Comando inválido para queue. Use 'queue "
                                     "show', 'queue clear', 'queue add <song>' "
                                     "ou 'queue remove <index>'."
                                  << std::endl;
                        return false;
                    }
                }

                showHelp("queue");
                return true;
            } else if (firstCommand == "playlist") {
                std::string playlistCommand;

                if (ss >> playlistCommand) {
                    if (playlistCommand == "show") {
                        std::string playlist;
                        std::getline(ss, playlist);
                        playlist = trimSpaces(playlist);
                        if (!playlist.empty()) {
                            auto optPl = _library->searchPlaylist(playlist);
                            if (optPl.empty()) {
                                std::cout
                                    << "Playlist não encontrada: " << playlist
                                    << std::endl;
                                return false;
                            }
                            auto spPl = optPl.at(0);
                            showPlaylist(*spPl);
                            return true;
                        }
                        std::cout << "Por favor, forneça o nome da playlist "
                                     "que deseja ver."
                                  << std::endl;
                        showHelp("playlist");
                        return true;
                    } else if (playlistCommand == "add") {
                        std::string playlist;

                        // mudar para std::getline(ss, playlist);
                        if (ss >> playlist) {
                            std::string playable;
                            std::getline(ss, playable);
                            playable = trimSpaces(playable);
                            if (!playable.empty()) {
                                auto optPl = _library->searchPlaylist(playlist);
                                if (optPl.empty()) {
                                    std::cout << "Playlist não encontrada: "
                                              << playlist << std::endl;
                                    return false;
                                }
                                auto optSong = _library->searchSong(playable);
                                if (optSong.empty()) {
                                    std::cout
                                        << "Musica não encontrada: " << playable
                                        << std::endl;
                                    return false;
                                }
                                auto spPl = optPl.at(0);
                                auto spSong = optSong.at(0);
                                addToPlaylist(*spPl, *spSong);
                                return true;
                            }

                            std::cout << "Por favor, forneça o nome da música "
                                         "para adicionar à playlist."
                                      << std::endl;
                            showHelp("playlist");
                            return true;
                        }
                        std::cout << "Por favor, forneça o nome da da playlist "
                                     "e nome da musica a ser adicionada."
                                  << std::endl;
                        showHelp("playlist");
                        return true;
                    } else if (playlistCommand == "remove") {
                        std::string playlist;

                        // mudar para std::getline(ss, playlist);
                        if (ss >> playlist) {
                            std::string playable;
                            std::getline(ss, playable);
                            playable = trimSpaces(playable);
                            if (!playable.empty()) {
                                auto optPl = _library->searchPlaylist(playlist);
                                if (optPl.empty()) {
                                    std::cout << "Playlist não encontrada: "
                                              << playlist << std::endl;
                                    return false;
                                }

                                auto optSong = _library->searchSong(playable);
                                if (optSong.empty()) {
                                    std::cout
                                        << "Música não encontrada: " << playable
                                        << std::endl;
                                    return false;
                                }

                                auto spPl = optPl.at(0);
                                auto spSong = optSong.at(0);
                                removeFromPlaylist(*spPl, *spSong);
                                return true;
                            }

                            std::cout << "Por favor, forneça o nome da música "
                                         "para remover da playlist."
                                      << std::endl;
                            showHelp("playlist");
                            return true;
                        }

                        std::cout << "Por favor, forneça o nome da playlist e "
                                     "nome da musica a ser removida."
                                  << std::endl;
                        showHelp("playlist");
                        return true;
                    }

                    std::cout << "Comando inválido para playlist." << std::endl;
                }

                showHelp("playlist");
                return true;
            } else if (firstCommand == "update_users") {
                updateUsers();
                return true;
            } else if (firstCommand == "update_songs"
                       || firstCommand == "update_library") {
                updateSongs();
                return true;
            } else if (firstCommand == "search") {
                std::string searchType;
                if (ss >> searchType) {
                    std::string query;
                    std::getline(ss, query);
                    query.erase(0, query.find_first_not_of(" "));

                    if (searchType == "music" || searchType == "song") {
                        searchSong(query);
                        return true;
                    } else if (searchType == "artist") {
                        searchArtist(query);
                        return true;
                    } else if (searchType == "album") {
                        searchAlbum(query);
                        return true;
                    } else if (searchType == "playlist") {
                        searchPlaylist(query);
                        return true;
                    } else {
                        std::cout << "Tipo de busca inválido. Use 'music', "
                                     "'artist', 'album' ou 'playlist'."
                                  << std::endl;
                        return false;
                    }
                }

                showHelp("search");
                return true;
            } else if (firstCommand == "help") {
                ss >> firstCommand ? showHelp(firstCommand) : showHelp();
                return true;
            } else {
                std ::cout << "Comando não reconhecido: " << firstCommand
                           << std::endl;
                return false;
            }
        } catch (const std::exception& e) {
            std::cerr << "Erro ao executar o comando '" << command
                      << "': " << e.what() << std::endl;
            return false;
        }
    }
} // namespace cli
