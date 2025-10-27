#include <doctest/doctest.h>

#include "core/entities/User.hpp"

TEST_SUITE("Unit Tests - Entity: User") {
    TEST_CASE("User: criação e getters funcionam corretamente") {
        core::User user("testeuser");
        CHECK(user.getUsername() == "testeuser");

        CHECK(user.getHomePath() == "");
        #ifdef _WIN32
            CHECK(user.getUID() == "");
        #else
            CHECK(user.getUID() == 0);
        #endif
        CHECK_FALSE(user.isCurrentUser());
    }

    TEST_CASE("User: setters alteram os valores corretamente") {
        core::User user("testeuser0");
        user.setUsername("testeuser1");
        CHECK(user.getUsername() == "testeuser1");

        user.setHomePath("/home/testeuser1");
        CHECK(user.getHomePath() == "/home/testeuser1");

        #ifdef _WIN32
            user.setUID("user123");
            CHECK(user.getUID() == "user123");
        #else
            user.setUID(1001);
            CHECK(user.getUID() == 1001);
        #endif
    }

    TEST_CASE("User: isCurrentUser retorna corretamente") {
        core::User user("joao");
        CHECK_FALSE(user.isCurrentUser());
    }

    TEST_CASE("User: valores extremos e inválidos") {
        core::User user("");
        CHECK(user.getUsername() == "");
        user.setHomePath("");
        CHECK(user.getHomePath() == "");
        #ifdef _WIN32
            user.setUID("");
            CHECK(user.getUID() == "");
            user.setUID("!@#$%");
            CHECK(user.getUID() == "!@#$%");
        #else
            user.setUID(0);
            CHECK(user.getUID() == 0);
            user.setUID(-1);
            CHECK(user.getUID() == static_cast<core::userid>(-1));
        #endif
    }

    TEST_CASE("User: múltiplos usuários independentes") {
        core::User user1("testeuser1");
        core::User user2("testeuser2");
        user1.setHomePath("/home/testeuser1");
        user2.setHomePath("/home/testeuser2");
        CHECK(user1.getUsername() == "testeuser1");
        CHECK(user2.getUsername() == "testeuser2");
        CHECK(user1.getHomePath() == "/home/testeuser1");
        CHECK(user2.getHomePath() == "/home/testeuser2");
        #ifdef _WIN32
            user1.setUID("A1");
            user2.setUID("B2");
            CHECK(user1.getUID() == "A1");
            CHECK(user2.getUID() == "B2");
        #else
            user1.setUID(101);
            user2.setUID(202);
            CHECK(user1.getUID() == 101);
            CHECK(user2.getUID() == 202);
        #endif
        CHECK_FALSE(user1 == user2);
        CHECK(user1 != user2);
    }
}
