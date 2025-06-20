cmake_minimum_required(VERSION 3.16)

project(TheNorthernLights VERSION 0.1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Dependencies
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(Catch2 REQUIRED)
find_package(tomlplusplus REQUIRED)

# Source files
file(GLOB_RECURSE APP_SOURCES src/*.cpp)
list(FILTER APP_SOURCES EXCLUDE REGEX ".*/main.cpp")

# Main target
add_executable(TheNorthernLights src/main.cpp ${APP_SOURCES})

# Include directories
target_include_directories(TheNorthernLights PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party/httplib/cpp-httplib-master
)

# Link dependencies
target_link_libraries(TheNorthernLights
    PRIVATE spdlog::spdlog
    PRIVATE fmt::fmt
    PRIVATE tomlplusplus::tomlplusplus
)

# Tests
file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)

if(TEST_SOURCES)
    enable_testing()

    # Exclude test_main.cpp if it contains CATCH_CONFIG_MAIN
    list(REMOVE_ITEM TEST_SOURCES ${CMAKE_SOURCE_DIR}/tests/test_main.cpp)

    add_executable(run_tests
        tests/test_main.cpp        # Only one main allowed
        ${TEST_SOURCES}            # All other tests (no main here)
        ${APP_SOURCES}             # App logic (without main.cpp)
    )

    target_include_directories(run_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/third_party/httplib/cpp-httplib-master
    )

    target_link_libraries(run_tests
        PRIVATE Catch2::Catch2
        PRIVATE spdlog::spdlog
        PRIVATE fmt::fmt
        PRIVATE tomlplusplus::tomlplusplus
    )

    include(CTest)
    include(Catch)
    catch_discover_tests(run_tests)
else()
    message(WARNING "Test files not found. Skipping test build.")
endif()
