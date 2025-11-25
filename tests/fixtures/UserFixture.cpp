#pragma once

#include <boost/filesystem.hpp>
#include <map>
#include <string>

#ifdef _WIN32
    std::string uid1001 = "1001";
    std::string uid1002 = "1002";
    std::string uid1003 = "1003";
#else
    uid_t uid1001 = 1001;
    uid_t uid1002 = 1002;
    uid_t uid1003 = 1003;
#endif

class UserFixture {
public:
    struct UserTestMock {
        unsigned int id;
        std::string username;
        std::string home_path;
        std::string input_path;
        #ifdef _WIN32
            std::string uid;
        #else
            long uid;
        #endif
        
        bool is_current_user;
    };

private:
    std::string _base_test_path;
    std::map<std::string, UserTestMock> _test_user_mocks;

public:
    UserFixture() {
        _base_test_path =
            (boost::filesystem::current_path() / "tests" / "temp_user_data")
                .string();
        _test_user_mocks["ADMIN_USER"] =
            UserTestMock {0,
                          "admin_test",
                          _base_test_path + "/admin_test/home",
                          _base_test_path + "/admin_test/input",
                          uid1001,
                          true};

        _test_user_mocks["NORMAL_USER"] =
            UserTestMock {0,
                          "normal_user",
                          _base_test_path + "/normal_user/home",
                          _base_test_path + "/normal_user/input",
                          uid1002,
                          false};

        _test_user_mocks["EMPTY_USER"] =
            UserTestMock {0,
                          "empty_user",
                          _base_test_path + "/empty_user/home",
                          _base_test_path + "/empty_user/input",
                          uid1003,
                          false};
    }

    const UserTestMock& getUserMock(const std::string& user_key) const {
        return _test_user_mocks.at(user_key);
    }

    const std::string& getBaseTestPath() const { return _base_test_path; }
};
