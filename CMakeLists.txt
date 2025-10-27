cmake_minimum_required(VERSION 3.15)
project(Frankenstein_Media_Player VERSION 1.0.0 LANGUAGES CXX)

# Configuração C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Políticas modernas do CMake
cmake_policy(SET CMP0076 NEW)

# Opções de configuração
option(BUILD_TESTING "Build tests" ON)

# ============================================================================
# DEPENDÊNCIAS EXTERNAS
# ============================================================================

# 1. Submódulos
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/SQLiteCpp)
# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/TagLibCpp)

include(FetchContent)

# 2. Doctest (FetchContent para testes)
if(BUILD_TESTING)

    FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.11
    )
    FetchContent_MakeAvailable(doctest)
endif()

# 3. JSON (nlohmann/json - FetchContent

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

# 4. Taglib (tenta find_package, senão FetchContent)
find_path(TAGLIB_INCLUDE_DIR taglib/fileref.h)
find_library(TAGLIB_LIB tag)

if(TAGLIB_INCLUDE_DIR AND TAGLIB_LIB)
    message(STATUS "Taglib encontrada no sistema.")
    set(TAGLIB_LINK_TARGET ${TAGLIB_LIB})
else()
    message(STATUS "Taglib NÃO encontrada no sistema. Usando FetchContent.")
    FetchContent_Declare(
        taglib
        GIT_REPOSITORY https://github.com/taglib/taglib.git
        GIT_TAG v2.0
    )
    FetchContent_MakeAvailable(taglib)
    set(TAGLIB_LINK_TARGET taglib)
endif()

# 5. SFML (find_package)
find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED)

# 6. Boost Filesystem (find_package)
find_package(Boost 1.65 COMPONENTS filesystem)


# ============================================================================
# BIBLIOTECA CORE
# ============================================================================

# Define a biblioteca do core
add_library(frankenstein_core STATIC)

# Configura sources do core
target_sources(frankenstein_core PRIVATE
    src/main.cpp
    # Adicione outros arquivos .cpp do core aqui
)

# Configura includes
target_include_directories(frankenstein_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib>/taglib
)

target_include_directories(frankenstein_core PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
)

# Dependências do core
target_link_libraries(frankenstein_core PUBLIC
    SQLiteCpp
    nlohmann_json::nlohmann_json
    ${TAGLIB_LINK_TARGET}
    Boost::filesystem
)

# Adiciona SFML se disponível
if(SFML_FOUND AND TARGET SFML::audio)
    message(STATUS "SFML encontrada. Habilitando suporte a áudio.")
    target_link_libraries(frankenstein_core PUBLIC SFML::audio)
    target_compile_definitions(frankenstein_core PUBLIC HAVE_SFML_AUDIO)
endif()

# Garante C++11 para todos que usarem o core
target_compile_features(frankenstein_core PUBLIC cxx_std_11)

# ============================================================================
# EXECUTÁVEL PRINCIPAL
# ============================================================================

add_executable(frankenstein_player src/main.cpp)
target_link_libraries(frankenstein_player PRIVATE frankenstein_core)

# Configurações de instalação
# install(TARGETS frankenstein_player DESTINATION bin)
# install(FILES config/frankenstein.config.json DESTINATION etc/frankenstein)
# install(DIRECTORY data/ DESTINATION share/frankenstein)

# ============================================================================
# TESTES (apenas se BUILD_TESTING=ON)
# ============================================================================

if(BUILD_TESTING)
    enable_testing()

    message(STATUS "Configuração de testes ativada.")
    # Testes de aceitação
    add_executable(frankentein_tests
        tests/acceptance/test_music_play_acceptance.cpp
    )

    target_link_libraries(frankentein_tests PRIVATE
         frankenstein_core
         doctest
    )

    target_include_directories(frankentein_tests PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tests>
    )

    # Registrar todos os testes no CTest
    add_test(NAME acceptance_tests COMMAND frankentein_tests)

    # Configurar propriedades dos testes
    set_tests_properties(acceptance_tests PROPERTIES TIMEOUT 120)

    # Alvo personalizado para rodar todos os testes
    add_custom_target(run_all_tests
         COMMAND frankentein_tests
         DEPENDS frankentein_tests
         COMMENT "Running all test suites"
    )
endif()

# ============================================================================
# CONFIGURAÇÕES ESPECÍFICAS POR PLATAFORMA
# ============================================================================

if(WIN32)
    # Configurações específicas para Windows
    if(BUILD_WITH_SFML AND SFML_FOUND)
        # Copia DLLs do SFML para o diretório de build
        add_custom_command(TARGET frankenstein_player POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SFML_ROOT}/bin/openal32.dll"
            $<TARGET_FILE_DIR:frankenstein_player>
        )
    endif()
endif()

# ============================================================================
# CONFIGURAÇÕES DE COMPILAÇÃO
# ============================================================================

# Avisos rigorosos
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(frankenstein_core PRIVATE -Wall -Wextra -Wpedantic)
elseif(MSVC)
    target_compile_options(frankenstein_core PRIVATE /W4)
endif()

# Informações de build
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Testing enabled: ${BUILD_TESTING}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
