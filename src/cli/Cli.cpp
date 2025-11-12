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
#include <unistd.h>
#include <cstdlib>
#include <iostream>

namespace cli
{
    Cli::Cli(core::ConfigManager &config_manager)
    {
        std::string username;
        std::string home_path;
        std::string input_path;
        std::string uid;
        try
        {
            username = config_manager.getConfigValue("user_username");
            if (username.empty())
            {
                throw std::runtime_error("Username is empty in config");
            }
        }
        catch (const std::exception &e)
        {
            username = std::string();
        }

        try
        {
            home_path = config_manager.userMusicDirectory();
        }
        catch (const std::exception &e)
        {
            home_path = std::string();
        }

        try
        {
            input_path = config_manager.inputUserPath();
        }
        catch (const std::exception &e)
        {
            if (!home_path.empty())
                input_path = home_path + "/input/";
            else
                input_path = std::string();
        }
        try
        {
            uid = config_manager.getConfigValue("user_id");
        }
        catch (const std::exception &e)
        {
            uid = std::string();
        }
        _user = std::make_shared<core::User>(username, home_path, input_path, std::stoi(uid));

        _player = std::make_shared<core::Player>();
    }

    /*
        void Cli::play(Core::IPlayable &playabel){
            _player.play(playabel);
        }
    */

    void Cli::restart()
    {
        _player->restart();
    }

    void Cli::rewind(unsigned int seconds)
    {
        _player->rewind(seconds);
    }

    void Cli::forward(unsigned int seconds)
    {
        _player->fastForward(seconds);
    }

    void Cli::toggleResumePause()
    {
        if (_player->isPlaying())
        {
            _player->pause();
        }
        else
        {
            _player->resume();
        }
    }

    void Cli::next()
    {
        _player->next();
    }

    void Cli::previous()
    {
        _player->previous();
    }

    void Cli::setVolume(unsigned int volume)
    {
        float vol = static_cast<float>(volume) / 100.0f;
        if (vol < 0.0f)
            vol = 0.0f;
        if (vol > 1.0f)
            vol = 1.0f;
        _player->setVolume(vol);
    }

    void Cli::setVolumeUpDown(const std::string &volume)
    {
        if (volume == "up")
        {
            _player->setVolume(_player->getVolume() + volumeStep);
        }
        else if (volume == "down")
        {
            _player->setVolume(_player->getVolume() - volumeStep);
        }
    }

    std::string Cli::getVolume() const
    {
        float vol = _player->getVolume() * 100.0f;
        std::cout << "Nivel de volume: " << static_cast<unsigned int>(vol) << std::endl;
    }

    void Cli::toggleMute(const std::string &command)
    {
        if (command == "mute")
        {
            if (_player->isMuted())
            {
                std::cout << "O player já está mudo." << std::endl;
                return;
            }
            _player->mute();
        }
        else if (command == "unmute")
        {
            if (!_player->isMuted())
            {
                std::cout << "O player não está no mudo." << std::endl;
                return;
            }
            _player->unmute();
        }
        else if (command == "toggle_mute")
        {
            if (_player->isMuted())
            {
                _player->unmute();
            }
            else
            {
                _player->mute();
            }
        }
    }

    std::string Cli::getProgress() const
    {
        _player->getProgress();
    }

    void Cli::clearQueue()
    {
        _player->clearPlaylist();
    }

    void Cli::showQueue() const
    {
        std::cout << _player->getPlaybackQueue()->toString();
    }

    void Cli::loop(const std::string &command)
    {
        if (command == "on")
        {
            if (_player->isLooping())
            {
                std::cout << "O loop já está ativado." << std::endl;
                return;
            }
            _player->setLooping();
        }
        else if (command == "off")
        {
            if (!_player->isLooping())
            {
                std::cout << "O loop já está desativado." << std::endl;
                return;
            }
            _player->unsetLooping();
        }
    }

    // TODO: implementar (depende de player)
    /*
        std::string Cli::getCurrentSong() const
        {
            _player->getCurrentSong();
        }

        void Cli::play(Core::IPlayable &playabel)
        {
            _player->play(playabel);
        }

        void Cli::shuffle()
        {
            _player->shuffle();
        }

        void Cli::like()
        {
            _player->likeCurrentSong();
        }
        void Cli::deslike()
        {
            _player->deslikeCurrentSong();
        }

        void Cli::addToQueue(Core::IPlayable &playabel)
        {
            _player->addToPlaybackQueue(playabel);
        }
        void Cli::removeFromQueue(unsigned idx)
        {
            _player->removeFromPlaybackQueue(idx);
        }
        void Cli::showQueue() const
        {
            _player->showPlaybackQueue();
        }
        void Cli::showStatus() const
        {
            _player->showStatus();
        }

    */

    // TODO implementar resolvePlayable
    /*
        std::optional<std::shared_ptr<Core::IPlayable>> Cli::resolvePlayable(const std::string &str)
        {
            return std::nullopt;
        }

        bool Cli::showPlaylist(Core::IPlayable &playlist)
        {
            return false;
        }

        bool Cli::addToPlaylist(Core::IPlayable &playlist, Core::IPlayable &playabel)
        {
            return false;
        }

        bool Cli::emoveFromPlaylist(Core::IPlayable &playlist, Core::IPlayable &playabel)
        {
            return false;
        }

        bool Cli::removeFromPlaylist(Core::IPlayable &playlist, unsigned int idx)
        {
            return false;
        }

        void Cli::searchMusic(const std::string &query) const
        {
        }

        void Cli::searchArtist(const std::string &query) const
        {
        }

        void Cli::searchAlbum(const std::string &query) const
        {
        }

        void Cli::searchPlaylist(const std::string &query) const
        {
        }
    */

    bool Cli::doCommand(const std::string &command)
    {
        std::stringstream ss(command);

        std::string firstCommand;
        ss >> firstCommand;

        if (firstCommand.empty())
        {
            std::cout << "Comando vazio. Por favor, insira um comando válido." << std::endl;
            return false;
        }
        else if (firstCommand == "info" || firstCommand == "status")
        {
            showStatus();
            return true;
        }
        else if (firstCommand == "play" || firstCommand == "pause" || firstCommand == "resume")
        {
            if (firstCommand == "play")
            {
                std::string playable;
                std::getline(ss, playable);
                if (playable.empty())
                {
                    toggleResumePause();
                    return true;
                }

                auto optPlayable = resolvePlayable(playable);
                if (!optPlayable)
                {
                    std::cout << "Não encontrado: " << playable << std::endl;
                    return false;
                }
                auto sp = *optPlayable;
                play(*sp);
                return true;
            }

            toggleResumePause();
            return true;
        }
        else if (firstCommand == "replay" || firstCommand == "restart")
        {
            restart();
            return true;
        }

        else if (firstCommand == "volume")
        {
            std::string volumeCommand;
            if (ss >> volumeCommand)
            {
                if (volumeCommand == "up" || volumeCommand == "down")
                {
                    setVolumeUpDown(volumeCommand);
                    return true;
                }
                else if (volumeCommand == "set")
                {
                    unsigned int num;
                    if (ss >> num)
                    {
                        setVolume(num);
                        return true;
                    }
                }
                std::cout << "Comando inválido para volume. Use 'volume up', 'volume down' ou 'volume set <value>'." << std::endl;
                return false;
            }

            std::string current_volume = getVolume();
            std::cout << "Volume atual: " << current_volume << std::endl;

            return true;
        }
        else if (firstCommand == "rewind")
        {
            unsigned int seconds;
            if (ss >> seconds)
            {
                rewind(seconds);
                return true;
            }
            else
            {
                std::cout << "Por favor, forneça o número de segundos para retroceder." << std::endl;
                return false;
            }
        }
        else if (firstCommand == "forward")
        {
            unsigned int seconds;
            if (ss >> seconds)
            {
                forward(seconds);
                return true;
            }

            std::cout << "Por favor, forneça o número de segundos para avançar." << std::endl;
            return false;
        }
        else if (firstCommand == "mute" || firstCommand == "unmute" || firstCommand == "toggle_mute")
        {
            toggleMute(firstCommand);
            return true;
        }
        else if (firstCommand == "next")
        {
            next();
            return true;
        }
        else if (firstCommand == "previous")
        {
            previous();
            return true;
        }
        else if (firstCommand == "like")
        {
            like();
            return true;
        }
        else if (firstCommand == "deslike")
        {
            deslike();
            return true;
        }
        else if (firstCommand == "shuffle")
        {
            shuffle();
            return true;
        }
        else if (firstCommand == "loop")
        {
            std::string loopCommand;
            if (ss >> loopCommand)
            {
                if (loopCommand == "on" || loopCommand == "off")
                {
                    loop(loopCommand);
                    return true;
                }
                else
                {
                    std::cout << "Comando inválido para loop. Use 'loop on' ou 'loop off'." << std::endl;

                    return false;
                }
            }
            showHelp("loop");
            return true;
        }
        else if (firstCommand == "queue")
        {
            std::string queueCommand;

            if (ss >> queueCommand)
            {
                if (queueCommand == "clear")
                {
                    clearQueue();
                    return true;
                }
                else if (queueCommand == "show")
                {
                    showQueue();
                    return true;
                }
                else if (queueCommand == "add")
                {
                    std::string playable;
                    std::getline(ss, playable);
                    playable = trim(playable);
                    if (!playable.empty())
                    {
                        auto opt = resolvePlayable(playable);
                        if (!opt)
                        {
                            std::cout << "Não encontrado: " << playable << std::endl;
                            return false;
                        }
                        auto sp = *opt;
                        addToQueue(*sp);
                        return true;
                    }

                    std::cout << "Por favor, forneça o nome da música para adicionar à fila." << std::endl;
                    return false;
                }
                else if (queueCommand == "remove")
                {
                    unsigned idx;
                    if (ss >> idx)
                    {
                        removeFromQueue(idx);
                        return true;
                    }
                    else
                    {
                        std::cout << "Por favor, forneça o índice da música para remover da fila." << std::endl;
                        return false;
                    }
                }
                else
                {
                    std::cout << "Comando inválido para queue. Use 'queue show', 'queue clear', 'queue add <song>' ou 'queue remove <index>'." << std::endl;
                    return false;
                }
            }

            showHelp("queue");
            return true;
        }
        else if (firstCommand == "playlist")
        {
            std::string playlistCommand;

            if (ss >> playlistCommand)
            {
                if (playlistCommand == "show")
                {
                    std::string playlist;
                    std::getline(ss, playlist);
                    playlist = trim(playlist);
                    if (!playlist.empty())
                    {
                        auto optPl = resolvePlayable(playlist);
                        if (!optPl)
                        {
                            std::cout << "Playlist não encontrada: " << playlist << std::endl;
                            return false;
                        }
                        auto spPl = *optPl;
                        showPlaylist(*spPl);
                        return true;
                    }
                    std::cout << "Por favor, forneça o nome da playlist que deseja ver." << std::endl;
                    showHelp("playlist");
                    return true;
                }
                else if (playlistCommand == "add")
                {
                    std::string playlist;
                    if (ss >> playlist)
                    {
                        std::string playable;
                        std::getline(ss, playable);
                        playable = trim(playable);
                        if (!playable.empty())
                        {
                            auto optPl = resolvePlayable(playlist);
                            if (!optPl)
                            {
                                std::cout << "Playlist não encontrada: " << playlist << std::endl;
                                return false;
                            }
                            auto optSong = resolvePlayable(playable);
                            if (!optSong)
                            {
                                std::cout << "Música não encontrada: " << playable << std::endl;
                                return false;
                            }
                            auto spPl = *optPl;
                            auto spSong = *optSong;
                            addToPlaylist(*spPl, *spSong);
                            return true;
                        }

                        std::cout << "Por favor, forneça o nome da música para adicionar à playlist." << std::endl;
                        showHelp("playlist");
                        return true;
                    }
                    std::cout << "Por favor, forneça o nome da da playlist e nome da musica a ser adicionada." << std::endl;
                    showHelp("playlist");
                    return true;
                }
                else if (playlistCommand == "remove")
                {
                    std::string playlist;
                    if (ss >> playlist)
                    {
                        std::string playable;
                        std::getline(ss, playable);
                        playable = trim(playable);
                        if (!playable.empty())
                        {
                            try
                            {
                                size_t pos;
                                int value = std::stoi(playable, &pos);

                                // Verifica se toda a string foi convertida
                                if (pos == playable.length())
                                {
                                    auto optPl = resolvePlayable(playlist);
                                    if (!optPl)
                                    {
                                        std::cout << "Playlist não encontrada: " << playlist << std::endl;
                                        return false;
                                    }
                                    auto spPl = *optPl;
                                    if (value < 0)
                                    {
                                        std::cout << "Índice inválido: " << value << std::endl;
                                        return false;
                                    }
                                    removeFromPlaylist(*spPl, static_cast<unsigned int>(value));
                                    return true;
                                }
                                std::cout << "Por favor, forneça o nome da música ou posicao dela para remover da playlist." << std::endl;
                                showHelp("playlist");
                                return true;
                            }
                            catch (const std::invalid_argument &)
                            {
                                // String não é um número válido -> trata como nome da música
                                {
                                    auto optPl = resolvePlayable(playlist);
                                    if (!optPl)
                                    {
                                        std::cout << "Playlist não encontrada: " << playlist << std::endl;
                                        return false;
                                    }
                                    auto optSong = resolvePlayable(playable);
                                    if (!optSong)
                                    {
                                        std::cout << "Música não encontrada: " << playable << std::endl;
                                        return false;
                                    }
                                    auto spPl = *optPl;
                                    auto spSong = *optSong;
                                    removeFromPlaylist(*spPl, *spSong);
                                    return true;
                                }
                            }
                            catch (const std::out_of_range &)
                            {
                                std::cout << "O valor fornecido está fora do intervalo." << std::endl;
                                return false;
                            }
                        }

                        std::cout << "Por favor, forneça o nome da música ou posicao dela para remover da playlist." << std::endl;
                        showHelp("playlist");
                        return true;
                    }

                    std::cout << "Por favor, forneça o nome da da playlist e nome da musica a ser removida." << std::endl;
                    showHelp("playlist");
                    return true;
                }

                std::cout << "Comando inválido para playlist." << std::endl;
            }

            showHelp("playlist");
            return true;
        }
        else if (firstCommand == "search")
        {
            std::string searchType;
            if (ss >> searchType)
            {
                std::string query;
                std::getline(ss, query);
                query.erase(0, query.find_first_not_of(" "));

                if (searchType == "music")
                {
                    searchMusic(query);
                    return true;
                }
                else if (searchType == "artist")
                {
                    searchArtist(query);
                    return true;
                }
                else if (searchType == "album")
                {
                    searchAlbum(query);
                    return true;
                }
                else if (searchType == "playlist")
                {
                    searchPlaylist(query);
                    return true;
                }
                else if (searchType == "podcast")
                {
                    searchPodcast(query);
                    return true;
                }
                else
                {
                    std::cout << "Tipo de busca inválido. Use 'music', 'artist', 'album', 'playlist' ou 'podcast'." << std::endl;
                    return false;
                }
            }

            showHelp("search");
            return true;
        }
        else if (firstCommand == "help")
        {
            showHelp();
            return true;
        }
        else
        {
            std ::cout << "Comando não reconhecido: " << firstCommand << std::endl;
            return false;
        }
    }
}