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
        userid UsersManager::getSIDCurrentUser() const {
            HANDLE tokenHandle = nullptr;
            if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &tokenHandle)) {
                assert(false && "Erro ao abrir o token do processo.");
                throw std::runtime_error("Erro ao abrir o token do processo.");
            }

            DWORD tokenInfoLength = 0;
            GetTokenInformation(tokenHandle, TokenUser, nullptr, 0, &tokenInfoLength);
            if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
                CloseHandle(tokenHandle);
                assert(false && "Erro ao obter o tamanho das informações do token.");
                throw std::runtime_error("Erro ao obter o tamanho das informações do token.");
            }

            std::vector<BYTE> tokenInfoBuffer(tokenInfoLength);
            if (!GetTokenInformation(tokenHandle, TokenUser, tokenInfoBuffer.data(), tokenInfoLength, &tokenInfoLength)) {
                CloseHandle(tokenHandle);
                assert(false && "Erro ao obter as informações do token.");
                throw std::runtime_error("Erro ao obter as informações do token.");
            }

            CloseHandle(tokenHandle);

            TOKEN_USER* tokenUser = reinterpret_cast<TOKEN_USER*>(tokenInfoBuffer.data());
            LPSTR sidString = nullptr;
            if (!ConvertSidToStringSidA(tokenUser->User.Sid, &sidString)) {
                assert(false && "Erro ao converter SID para string.");
                throw std::runtime_error("Erro ao converter SID para string.");
            }

            std::string sid(sidString);
            LocalFree(sidString);

            return sid;
        }

        void UsersManager::getUsersWindows(std::vector<std::shared_ptr<User>> &os_users) {
            LPUSER_INFO_0 pBuf = NULL;
            DWORD dwLevel = 0;
            DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
            DWORD dwEntriesRead = 0;
            DWORD dwTotalEntries = 0;
            DWORD dwResumeHandle = 0;
            DWORD i;
            NET_API_STATUS nStatus;

            nStatus = NetUserEnum(NULL,
                dwLevel,
                FILTER_NORMAL_ACCOUNT,
                (LPBYTE*)&pBuf,
                dwPrefMaxLen,
                &dwEntriesRead,
                &dwTotalEntries,
                &dwResumeHandle);

            if ((nStatus == NERR_Success) && (pBuf != NULL)) {
                LPUSER_INFO_0 pTmpBuf = pBuf;

                for (i = 0; i < dwEntriesRead; i++, pTmpBuf++) {
                    if (pTmpBuf == NULL) break;


                    std::wstring wusername(pTmpBuf->usri0_name);
                    std::string username(wusername.begin(), wusername.end());
                    std::string home_path = _configManager->userMusicDirectory();
                    std::string input_path = _configManager->inputUserPath();
                    std::string uid = "";

                    DWORD cbSid = 0;
                    DWORD cchDomain = 0;
                    SID_NAME_USE sidUse;
                    LookupAccountNameW(
                        NULL,
                        pTmpBuf->usri0_name,
                        NULL,
                        &cbSid,
                        NULL,
                        &cchDomain,
                        &sidUse
                    );

                    PSID pSid = (PSID)LocalAlloc(LPTR, cbSid);
                    LPWSTR pDomain = (LPWSTR)LocalAlloc(LPTR, cchDomain * sizeof(WCHAR));

                    if (!LookupAccountNameW(
                        NULL,
                        pTmpBuf->usri0_name,
                        pSid,
                        &cbSid,
                        pDomain,
                        &cchDomain,
                        &sidUse
                    )) {
                        if (pSid) LocalFree(pSid);
                        if (pDomain) LocalFree(pDomain);
                        continue;
                    }

                    LPWSTR stringSid = NULL;
                    if (ConvertSidToStringSidW(pSid, &stringSid)) {
                        std::wstring ws(stringSid);
                        uid = std::string(ws.begin(), ws.end());
                        LocalFree(stringSid);
                    }

                    if (pSid) LocalFree(pSid);
                    if (pDomain) LocalFree(pDomain);

                    auto user = std::make_shared<User>(username, home_path, input_path, uid);
                    os_users.push_back(user);
                }

                if (pBuf != NULL) NetApiBufferFree(pBuf);
            }
        }
    #endif

    UsersManager::UsersManager(ConfigManager &configManager)
        : _configManager(std::make_shared<ConfigManager>(configManager)) {

        DatabaseManager db_manager(_configManager->databasePath(),
                                   _configManager->databaseSchemaPath());
        RepositoryFactory repo_factory(db_manager.getDatabase());
        _userRepository = repo_factory.createUserRepository();

        if (!checkIfPublicUserExists()) {
            User public_user("public");
            public_user.setHomePath(_configManager->publicMusicDirectory());
            public_user.setInputPath(_configManager->inputPublicPath());
            public_user.setUID(0);

            if (!_userRepository->save(public_user)) {

                throw std::runtime_error(
                    "Erro ao criar o usuário público no banco de dados.");
            }

        }

        _users.clear();
        auto stored_users = _userRepository->getAll();
        for (const auto& user : stored_users)
            _users.push_back(*user);
    }

    UsersManager::UsersManager(ConfigManager &configManager, SQLite::Database &db)
        : _configManager(std::make_shared<ConfigManager>(configManager)) {

        RepositoryFactory repo_factory(std::shared_ptr<SQLite::Database>(&db, [](SQLite::Database*){}));
        _userRepository = repo_factory.createUserRepository();

        if (!checkIfPublicUserExists()) {
            User public_user("public");
            public_user.setHomePath(_configManager->publicMusicDirectory());
            public_user.setInputPath(_configManager->inputPublicPath());
            public_user.setUID(0);

            if (!_userRepository->save(public_user)) {

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
                try {
                stored_user->setHomePath(_configManager->userMusicDirectory());
                stored_user->setInputPath(_configManager->inputUserPath());
                } catch (const std::invalid_argument &e) {
                    throw std::runtime_error("Erro ao atualizar os caminhos do usuário: " + std::string(e.what()));
                }

                if (!_userRepository->save(*stored_user)) {
                    throw std::runtime_error("Erro ao atualizar o usuário no banco de dados.");
                }

                break;
            }

            if (!user_exists) {
                if (_userRepository->save(*os_user)) {
                    _users.push_back(*os_user);
                } else {
                    throw std::runtime_error("Erro ao salvar o usuário no banco de dados.");
                }
            }
        }
    }

    std::shared_ptr<User> UsersManager::getCurrentUser() const {
        #if defined(_WIN32)
            userid current_uid;
            try {
                current_uid = getSIDCurrentUser();
            } catch (const std::runtime_error &e) {
                throw std::runtime_error("Erro ao obter o SID do usuário atual: " + std::string(e.what()));
            }
        #else
            userid current_uid = getuid();
        #endif

        auto user = _userRepository->findByUID(current_uid);
        // std::cout << (user == nullptr) << std::endl;
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

            if (os_users.empty()) {
                throw std::runtime_error("Erro ao acessar a lista de usuários do sistema.");
            }
        #else
            struct passwd *pw;
            setpwent();
            pw = getpwent();

            if (pw == nullptr) {
                endpwent();
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
