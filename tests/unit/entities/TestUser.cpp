#include <doctest/doctest.h>

#include "core/entities/User.hpp"

TEST_SUITE("Unit Tests - Entity: User") {
    TEST_CASE("User: getters e setters básicos") {
        core::User user("testeuser");
        CHECK(user.getUsername() == "testeuser");

        CHECK(user.getHomePath().empty());
        #ifdef _WIN32
            CHECK(user.getUID().empty());
        #else
            CHECK(user.getUID() == 0);
        #endif

        user.setHomePath("/home/:username:");
        CHECK(user.getHomePath() == "/home/testeuser/");

        user.setInputPath("/music/:username:/input");
        CHECK(user.getInputPath() == "/music/testeuser/input/");

        #ifdef _WIN32
            user.setUID("user123");
            CHECK(user.getUID() == "user123");
        #else
            user.setUID(1001);
            CHECK(user.getUID() == 1001);
        #endif

        CHECK_FALSE(user.isCurrentUser());
        user.setIsCurrentUser(true);
        CHECK(user.isCurrentUser());
    }

    TEST_CASE("User: comparações entre usuários") {
        core::User user1("userA");
        user1.setId(1);
        core::User user2("userA");
        user2.setId(1);
        core::User user3("userB");
        user3.setId(1);
        core::User user4("userA");
        user4.setId(2);
        core::User user5("userC");
        user5.setId(3);

        #ifdef _WIN32
        user1.setUID("1000");
        user2.setUID("1000");
        user3.setUID("2000");
        user4.setUID("1000");
        user5.setUID("3000");
        #else
        user1.setUID(1000);
        user2.setUID(1000);
        user3.setUID(2000);
        user4.setUID(1000);
        user5.setUID(3000);
        #endif

        CHECK_EQ(user1, user2);
        CHECK_NE(user1, user3);
        CHECK_EQ(user1, user4);
        CHECK_NE(user1, user5);
        CHECK_NE(user3, user4);

        CHECK_LE(user1, user2);
        CHECK_LT(user1, user3);
        CHECK_LT(user1, user5);

        CHECK_GE(user1, user2);
        CHECK_GT(user3, user1);
        CHECK_GT(user5, user1);
    }

    TEST_CASE("User: valores extremos e inválidos") {
        core::User user;
        CHECK_THROWS(user.setUsername(""));
        CHECK(user.getUsername().empty());
        CHECK_THROWS(user.setHomePath(""));
        CHECK(user.getHomePath().empty());
        CHECK_THROWS(user.setInputPath(""));
        CHECK(user.getInputPath().empty());
        #ifdef _WIN32
            user.setUID("");
            CHECK(user.getUID().empty());
            user.setUID("!@#$%");
            CHECK(user.getUID() == "!@#$%");
        #else
            user.setUID(0);
            CHECK(user.getUID() == 0);
            user.setUID(-1);
            CHECK(user.getUID() == static_cast<core::userid>(-1));
        #endif
    }
}
