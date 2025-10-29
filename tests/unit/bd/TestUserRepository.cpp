#include <doctest/doctest.h>

#include <memory>
#include <vector>
#include <string>
#include <cstdio>

#include "core/bd/DatabaseManager.hpp"
#include "core/bd/UserRepository.hpp"
#include "core/entities/User.hpp"

#include "fixtures/ConfigFixture.hpp"

TEST_SUITE("Unit Tests - UserRepository") {
    std::unique_ptr<core::DatabaseManager> createTempDB() {
        ConfigFixture config;
        std::string db_path = config.databasePath();
        std::string schema_path = config.databaseSchemaPath();

        return std::unique_ptr<core::DatabaseManager>(new core::DatabaseManager(db_path, schema_path));
    }

    TEST_CASE("UserRepository: inserir e buscar usuário") {
        core::UserRepository repo(createTempDB());

        core::User user("usuario1");
        user.setHomePath("/home/usuario1");
        #ifdef _WIN32
            user.setUID("winuid1");
        #else
            user.setUID(101);
        #endif

        CHECK(repo.insert(user) == true);

        auto all_users = repo.getAll();
        CHECK(all_users.size() == 1);
        CHECK(all_users[0]->getUsername() == "usuario1");
        CHECK(all_users[0]->getHomePath() == "/home/usuario1");
        #ifdef _WIN32
            CHECK(all_users[0]->getUID() == "winuid1");
        #else
            CHECK(all_users[0]->getUID() == 101);
        #endif
    }

    TEST_CASE("UserRepository: salvar (insert/update) usuário") {
        core::UserRepository repo(createTempDB());

        core::User user("usuario2");
        user.setHomePath("/home/usuario2");
        #ifdef _WIN32
            user.setUID("winuid2");
        #else
            user.setUID(202);
        #endif

        CHECK(repo.save(user) == true);

        auto all_users = repo.getAll();
        CHECK(all_users.size() == 1);

        // Atualiza usuário
        all_users[0]->setHomePath("/home/usuario2_mod");
        #ifdef _WIN32
            all_users[0]->setUID("winuid2_mod");
        #else
            all_users[0]->setUID(303);
        #endif
        CHECK(repo.save(*all_users[0]) == true);

        auto updated_user = repo.findById(all_users[0]->getId());
        CHECK(updated_user != nullptr);
        CHECK(updated_user->getHomePath() == "/home/usuario2_mod");
        #ifdef _WIN32
            CHECK(updated_user->getUID() == "winuid2_mod");
        #else
            CHECK(updated_user->getUID() == 303);
        #endif
    }

    TEST_CASE("UserRepository: remover usuário") {
        core::UserRepository repo(createTempDB());

        core::User user("usuario3");
        CHECK(repo.insert(user) == true);

        auto all_users = repo.getAll();
        CHECK(all_users.size() == 1);

        unsigned id = all_users[0]->getId();
        CHECK(repo.remove(id) == true);

        auto after_remove = repo.getAll();
        CHECK(after_remove.empty());
        CHECK(repo.findById(id) == nullptr);
    }

    TEST_CASE("UserRepository: buscar por username") {
        core::UserRepository repo(createTempDB());

        core::User user1("alice");
        core::User user2("bob");
        core::User user3("alice");

        CHECK(repo.insert(user1) == true);
        CHECK(repo.insert(user2) == true);
        CHECK(repo.insert(user3) == true);

        auto found_alice = repo.findByUsername("alice");
        CHECK(found_alice.size() == 2);
        for (const auto& u : found_alice)
            CHECK(u->getUsername() == "alice");

        auto found_bob = repo.findByUsername("bob");
        CHECK(found_bob.size() == 1);
        CHECK(found_bob[0]->getUsername() == "bob");

        auto found_none = repo.findByUsername("charlie");
        CHECK(found_none.empty());
    }

    TEST_CASE("UserRepository: getAll retorna todos usuários") {
        core::UserRepository repo(createTempDB());

        CHECK(repo.getAll().empty());

        core::User user1("u1");
        core::User user2("u2");
        core::User user3("u3");

        CHECK(repo.insert(user1) == true);
        CHECK(repo.insert(user2) == true);
        CHECK(repo.insert(user3) == true);

        auto all_users = repo.getAll();
        CHECK(all_users.size() == 3);

        std::vector<std::string> names;
        for (const auto& u : all_users)
            names.push_back(u->getUsername());

        CHECK(std::find(names.begin(), names.end(), "u1") != names.end());
        CHECK(std::find(names.begin(), names.end(), "u2") != names.end());
        CHECK(std::find(names.begin(), names.end(), "u3") != names.end());
    }

    TEST_CASE("UserRepository: findById retorna nullptr para inexistente") {
        core::UserRepository repo(createTempDB());

        auto user = repo.findById(9999);
        CHECK(user == nullptr);

        core::User user("usuario1");
        user.setHomePath("/home/usuario1");
        #ifdef _WIN32
            core::userid user_id = "winuid1";
        #else
            core::userid user_id = 101;
        #endif
        user.setUID(user_id);

        CHECK(repo.insert(user) == true);

        unsigned id = repo.getLastInsertId();
        auto found_user = repo.findById(id);
        CHECK(found_user != nullptr);
        CHECK(found_user->getUsername() == "usuario1");
    }
}
