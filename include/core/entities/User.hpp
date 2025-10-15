/**
 * @file User.hpp
 * @author Bruno Vieira
 * @brief entidade  para usuários do sistema
 * @version 0.1
 * @date 2025-10-15
 *
 */

#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../entities/Entity.hpp"
#include "../entities/Playlist.hpp"
namespace core
{
    class User : public core::Entity
    {
    private:
        std::string _username;
        std::string &file_path;
        std::vector<std::shared_ptr<core::Playlist>> _playlists;
        std::unique_ptr<core::Playlist> _likedSongs;

    public:
    /**
     * @brief Constroi um novo usuário
     * 
     * @param _username Nome do usuário
     */
        User(const std::string &_username);

        /**
         * @brief Destrutor de um usuário
         * 
         */
        ~User();
    };

}