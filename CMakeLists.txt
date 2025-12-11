cmake_minimum_required(VERSION 3.5)
project(Frankenstein_Media_Player VERSION 1.0.0 LANGUAGES CXX)

# Configuração C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Políticas modernas do CMake
cmake_policy(SET CMP0076 NEW)

# Opções de configuração
option(BUILD_TESTING "Build tests" ON)

# ============================================================================
# CONFIGURAÇÕES DE COBERTURA DE CÓDIGO
# ===========================================================================
if(BUILD_TESTING)
    # Verifica se o compilador suporta cobertura
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Configuring code coverage...")
        # Flags de cobertura (usar listas)
        set(COVERAGE_CXX_FLAGS -g -O0 -fprofile-arcs -ftest-coverage -fno-inline -fno-omit-frame-pointer)
        set(COVERAGE_LINK_FLAGS --coverage)
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/coverage)
        add_compile_options(${COVERAGE_CXX_FLAGS})
        if(POLICY CMP0068) # add_link_options suportado
            add_link_options(${COVERAGE_LINK_FLAGS})
        else()
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}")
        endif()
        message(STATUS "Code coverage enabled")
    else()
        message(WARNING "Code coverage only supported for GCC/Clang")
    endif()
endif()

# ============================================================================
# DEPENDÊNCIAS EXTERNAS
# ============================================================================
# 1. Submódulos
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/SQLiteCpp)

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

# 3. JSON (nlohmann/json - FetchContent)
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

#5 MiniAudio

# Download do MiniAudio
include(FetchContent)

FetchContent_Declare(
    miniaudio
    URL https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h
    DOWNLOAD_NO_EXTRACT true
)

FetchContent_MakeAvailable(miniaudio)

# Cria um target simples
add_library(miniaudio_lib INTERFACE)
target_include_directories(miniaudio_lib INTERFACE
    ${miniaudio_SOURCE_DIR}
)

message(STATUS " MiniAudio configurado - suporte a MP3, WAV, OGG, FLAC")

# ============================================================================
# BIBLIOTECA CORE
# ============================================================================

# Primeiro, verifica se existem arquivos .cpp no src/core
file(GLOB_RECURSE CORE_SOURCES
    "src/core/*.cpp"
    "src/core/**/*.cpp"
    "src/*.cpp"
)

# Se não encontrar arquivos .cpp, cria um dummy temporário
if(NOT CORE_SOURCES)
    message(STATUS "Nenhum arquivo .cpp encontrado. Criando dummy.cpp temporário.")
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/src/core/dummy.cpp
        "// Arquivo temporário para compilação\nvoid dummy_function() {}\n"
    )
    set(CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/core/dummy.cpp)
endif()

# Define a biblioteca do core
add_library(frankenstein_core STATIC
    ${CORE_SOURCES}
)

# Configura includes
target_include_directories(frankenstein_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Inclui taglib se estiver disponível via FetchContent
if(TARGET taglib)
    target_include_directories(frankenstein_core PUBLIC
        $<BUILD_INTERFACE:${taglib_SOURCE_DIR}/taglib>
    )
elseif(TAGLIB_INCLUDE_DIR)
    target_include_directories(frankenstein_core PUBLIC
        ${TAGLIB_INCLUDE_DIR}
    )
endif()

target_include_directories(frankenstein_core PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
)

# Dependências do core
target_link_libraries(frankenstein_core PUBLIC
    SQLiteCpp
    nlohmann_json::nlohmann_json
    ${TAGLIB_LINK_TARGET}
    miniaudio_lib
)

# Aplica flags de cobertura ao core quando apropriado
if(BUILD_TESTING AND (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"))
    target_compile_options(frankenstein_core PRIVATE ${COVERAGE_CXX_FLAGS})
    target_link_options(frankenstein_core PRIVATE ${COVERAGE_LINK_FLAGS})
endif()


# Define que temos suporte a áudio com MiniAudio
target_compile_definitions(frankenstein_core PUBLIC HAVE_MINI_AUDIO)
message(STATUS "MiniAudio habilitado para suporte a áudio.")

# Garante C++11 para todos que usarem o core
target_compile_features(frankenstein_core PUBLIC cxx_std_17)

# ============================================================================
# EXECUTÁVEL PRINCIPAL
# ============================================================================

# Verifica se src/main.cpp existe, senão cria um temporário
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
    add_executable(frankenstein_player src/main.cpp)
else()
    message(STATUS "src/main.cpp não encontrado. Criando temporário.")
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        "// Arquivo main temporário\n#include <iostream>\nint main() { std::cout << \"Frankenstein Media Player\\n\"; return 0; }\n"
    )
    add_executable(frankenstein_player src/main.cpp)
endif()

target_link_libraries(frankenstein_player PRIVATE frankenstein_core)

# ============================================================================
# TESTES (apenas se BUILD_TESTING=ON)
# ============================================================================
if(BUILD_TESTING)
    enable_testing()
    message(STATUS "Configuração de testes ativada.")

    # Testes de aceitação
    file(GLOB_RECURSE ACCEPTANCE_TEST_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/acceptance/*.cpp"
    )

    file(GLOB_RECURSE UNIT_TEST_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/*.cpp"
    )

    message(STATUS "Acceptance tests encontrados: ${ACCEPTANCE_TEST_SOURCES}")
    message(STATUS "Unit tests encontrados: ${UNIT_TEST_SOURCES}")

    set(ALL_TEST_SOURCES ${ACCEPTANCE_TEST_SOURCES} ${UNIT_TEST_SOURCES})

    foreach(testfile IN LISTS ALL_TEST_SOURCES)
        get_filename_component(testname ${testfile} NAME_WE)
        string(REGEX REPLACE "[^A-Za-z0-9_]" "_" testname_sanitized ${testname})
        set(test_target "${testname_sanitized}")

        add_executable(${test_target} tests/test_main.cpp ${testfile})
        target_link_libraries(${test_target} PRIVATE frankenstein_core doctest)

        target_include_directories(${test_target} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tests>
        )

        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_compile_options(${test_target} PRIVATE ${COVERAGE_CXX_FLAGS})
            target_link_options(${test_target} PRIVATE ${COVERAGE_LINK_FLAGS})
        endif()

        add_test(NAME ${test_target} COMMAND ${test_target})
        set_tests_properties(${test_target} PROPERTIES
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            TIMEOUT 120
        )
    endforeach()

    add_custom_target(run_all_tests
         COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
         COMMENT "Running all test suites via ctest"
    )
endif()

# ============================================================================
# CONFIGURAÇÕES ESPECÍFICAS POR PLATAFORMA
# ============================================================================

if(WIN32)
    # Configurações específicas para Windows
    # MiniAudio requer algumas bibliotecas no Windows
    target_link_libraries(frankenstein_core PRIVATE ole32)
    target_link_libraries(frankenstein_core PRIVATE netapi32)
    message(STATUS "Build configurado para Windows com MiniAudio")
elseif(APPLE)
    # macOS requer frameworks do Audio
    target_link_libraries(frankenstein_core PRIVATE
        "-framework AudioToolbox"
        "-framework CoreAudio"
        "-framework CoreFoundation"
    )
    target_link_libraries(test_music PRIVATE
        "-framework AudioToolbox"
        "-framework CoreAudio"
        "-framework CoreFoundation"
    )
    message(STATUS "Build configurado para macOS com MiniAudio")
else()
    # Linux requer ALSA e pthread
    find_library(ASOUND_LIBRARY asound)
    if(ASOUND_LIBRARY)
        target_link_libraries(frankenstein_core PRIVATE ${ASOUND_LIBRARY} pthread)
        message(STATUS "ALSA encontrada para MiniAudio no Linux")
    else()
        message(WARNING "ALSA não encontrada - áudio pode não funcionar no Linux")
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
message(STATUS "Audio library: MiniAudio")
