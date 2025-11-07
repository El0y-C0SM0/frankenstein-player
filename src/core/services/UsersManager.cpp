/**
 * @file UsersManager.cpp
 * @brief Implementação do gerenciador de usuários
 *
 * @ingroup services
 * @author Julia Lima
 * @date 2025-11-05
 */


#include "core/services/UsersManager.hpp"

#include "core/bd/DatabaseManager.hpp"
#include "core/bd/RepositoryFactory.hpp"
#include "core/bd/UserRepository.hpp"
#include "core/entities/User.hpp"

#include <string>
#if defined(_WIN32)
    #include <windows.h>
    #include <lm.h>
    #include <sddl.h>

    #pragma comment(lib, "netapi32.lib")
    #pragma comment(lib, "advapi32.lib")
#else
    #include <unistd.h>
    #include <pwd.h>
#endif
#include <memory>
#include <vector>


namespace core {

    #if defined(_WIN32)
    #endif

    UsersManager::UsersManager(ConfigManager &configManager)
        : _configManager(std::make_shared<ConfigManager>(configManager)) {

        DatabaseManager db_manager(_configManager->databasePath(),
                                   _configManager->databaseSchemaPath());
        // RepositoryFactory repo_factory(db_manager.getDatabase());
        // _userRepository = repo_factory.createUserRepository();
        _userRepository = std::unique_ptr<UserRepository>(
            new UserRepository(db_manager.getDatabase()));

        if (!checkIfPublicUserExists()) {
            User public_user("public");
            public_user.setHomePath(_configManager->publicMusicDirectory());
            public_user.setInputPath(_configManager->inputPublicPath());
            public_user.setUID(0);

            if (!_userRepository->save(public_user)) {
                assert (false &&
                       "Erro ao criar o usuário público no banco de dados.");
                // TODO exception
                throw std::runtime_error(
                    "Erro ao criar o usuário público no banco de dados.");
            }

        }

        _users.clear();
        auto stored_users = _userRepository->getAll();
        for (const auto& user : stored_users)
            _users.push_back(*user);
    }

    UsersManager::~UsersManager() {}

    bool UsersManager::checkIfPublicUserExists() {
        auto users = _userRepository->findById(1);
        if (users != nullptr && users->getUsername() == "public")
            return true;
        else if (users != nullptr && users->getUsername() != "public") {
            assert (false && "ID 1 no banco de dados não corresponde ao usuário público.");
            // TODO exception
            throw std::runtime_error("ID 1 no banco de dados não corresponde ao usuário público.");
            return false;
        }

        return false;
    }

    bool UsersManager::checkIfUsersExists() {
        return _userRepository->count() > 0;
    }

    void UsersManager::removeUser(const userid &user_id) {
        for (auto it = _users.begin(); it != _users.end(); ++it) {
            if (it->getUID() == user_id) {
                if (_userRepository->remove(it->getId())) {
                    _users.erase(it);
                } else {
                    assert (false && "Erro ao remover o usuário do banco de dados.");
                    // TODO exception
                    throw std::runtime_error("Erro ao remover o usuário do banco de dados.");
                }
                return;
            }
        }
    }

    void UsersManager::removeUser(User &user) {
        removeUser(user.getUID());
    }

    void UsersManager::updateUsersList() {
        auto os_users = getUsersOS();
        auto users_db = _userRepository->getAll();

        for (const auto& os_user : os_users) {
            bool user_exists = false;


            for (auto& stored_user : users_db) {
                if ((*stored_user) != (*os_user))
                    continue;

                user_exists = true;

                if (stored_user->getUsername() == os_user->getUsername() &&
                    stored_user->getHomePath() == os_user->getHomePath() &&
                    stored_user->getInputPath() == os_user->getInputPath())
                    break;

                stored_user->setUsername(os_user->getUsername());
                stored_user->setHomePath(_configManager->userMusicDirectory());
                stored_user->setInputPath(_configManager->inputUserPath());

                if (!_userRepository->save(*stored_user)) {
                    assert (false && "Erro ao atualizar o usuário no banco de dados.");
                    // TODO exception
                    throw std::runtime_error("Erro ao atualizar o usuário no banco de dados.");
                }

                break;
            }

            if (!user_exists) {
                if (_userRepository->save(*os_user)) {
                    _users.push_back(*os_user);
                } else {
                    assert (false && "Erro ao salvar o usuário no banco de dados.");
                    // TODO exception
                    throw std::runtime_error("Erro ao salvar o usuário no banco de dados.");
                }
            }
        }
    }

    std::shared_ptr<User> UsersManager::getCurrentUser() const {
        #if defined(_WIN32)
            userid current_uid = getSIDCurrentUser();
        #else
            userid current_uid = getuid();
        #endif
        auto user = _userRepository->findByUID(current_uid);
        user->setIsCurrentUser(true);
        return user;
    }

    std::shared_ptr<User> UsersManager::getUserById(unsigned id) const {
        return _userRepository->findById(id);
    }

    std::shared_ptr<User> UsersManager::getUserByUserId(const userid &user_id) const {
        return _userRepository->findByUID(user_id);
    }

    std::vector<std::shared_ptr<User>> UsersManager::getAllUsers() const {
        return _userRepository->getAll();
    }

    std::shared_ptr<User> UsersManager::getPublicUser() const {
        return _userRepository->findById(1);
    }

    std::vector<std::shared_ptr<User>> UsersManager::getUsersOS() {
        std::vector<std::shared_ptr<User>> os_users;

        #if defined(_WIN32)
            getUsersWindows(os_users);
        #else
            struct passwd *pw;
            setpwent();
            pw = getpwent();

            if (pw == nullptr) {
                endpwent();
                assert (false && "Erro ao acessar a lista de usuários do sistema.");
                // TODO exception
                throw std::runtime_error("Erro ao acessar a lista de usuários do sistema.");
            }

            while (pw != nullptr) {
                userid uid = pw->pw_uid;

                if (uid < 1000 || uid == 65534) {
                    pw = getpwent();
                    continue;
                }

                std::string username = pw->pw_name;
                std::string home_path = _configManager->userMusicDirectory();
                std::string input_path = _configManager->inputUserPath();

                auto user = std::make_shared<User>(username, home_path, input_path, uid);
                os_users.push_back(user);
                pw = getpwent();
            }
            endpwent();
        #endif

        return os_users;
    }
}
