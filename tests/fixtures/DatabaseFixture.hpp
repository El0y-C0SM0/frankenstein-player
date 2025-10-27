#include <memory.h>

#include "ConfigFixture.hpp"
#include "core/bd/DatabaseManager.hpp"

class DatabaseFixture : public core::DatabaseManager {
public:
    DatabaseFixture()
        : core::DatabaseManager(
            ConfigFixture().databasePath(),
            ConfigFixture().databaseSchemaPath()
        )
    {}
};
