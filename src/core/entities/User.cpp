/**
 * @file User.cpp
 * @brief Implementação da entidade User
 *
 * @ingroup entities
 * @author Julia Lima
 * @date 2025-11-05
 */

#include "core/entities/User.hpp"


namespace core {
    User::User(const std::string& username) :
        _username(username),
        _home_path(""),
        _input_path(""),
        _uid(0),
        _is_current_user(false) {}

    User::User(userid uid) :
        _username(""),
        _home_path(""),
        _input_path(""),
        _uid(uid),
        _is_current_user(false) {}

    User::User(const std::string& username,
               const std::string& home_path,
               const std::string& input_path,
               const userid& uid) :
        _username(username),
        _home_path(home_path),
        _input_path(input_path),
        _uid(uid),
        _is_current_user(false) {}

    User::User(unsigned id,
               const std::string& username,
               const std::string& home_path,
               const std::string& input_path,
               const userid& uid) :
        Entity(id),
        _username(username),
        _home_path(home_path),
        _input_path(input_path),
        _uid(uid),
        _is_current_user(false) {}

    User::~User() = default;

    std::string User::getUsername() const {
        return _username;
    }

    void User::setUsername(const std::string& username) {
        _username = username;
    }

    std::string User::getHomePath() const {
        std::string home_path = _home_path;
        size_t pos = home_path.find(":username:");
        if (pos != std::string::npos)
            home_path.replace(pos, 10, _username);
        return home_path;
    }

    void User::setHomePath(const std::string& home_path) {
        _home_path = home_path;
    }

    std::string User::getInputPath() const {
        std::string input_path = _input_path;
        size_t pos = input_path.find(":username:");
        if (pos != std::string::npos)
            input_path.replace(pos, 10, _username);
        return input_path;
    }

    void User::setInputPath(const std::string& input_path) {
        _input_path = input_path;
    }

    userid User::getUID() const {
        return _uid;
    }

    void User::setUID(const userid& uid) {
        _uid = uid;
    }

    bool User::isCurrentUser() const {
        return _is_current_user;
    }

    void User::setIsCurrentUser(bool is_current_user) {
        _is_current_user = is_current_user;
    }

    bool User::operator==(const Entity& other) const {
        const User* other_user = dynamic_cast<const User*>(&other);
        if (other_user)
            return this->getUID() == other_user->getUID();
        return false;
    }

    bool User::operator!=(const Entity& other) const {
        return !(*this == other);
    }
}
