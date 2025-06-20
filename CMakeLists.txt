cmake_minimum_required(VERSION 3.16)

project(TheNorthernLights VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Dependencies
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(Catch2 REQUIRED)

# Source + include
file(GLOB_RECURSE SOURCES src/*.cpp)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Main target
add_executable(TheNorthernLights ${SOURCES})
target_include_directories(TheNorthernLights PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(TheNorthernLights
    PRIVATE spdlog::spdlog
    PRIVATE fmt::fmt
)

# Test target
file(GLOB TEST_SOURCES tests/*.cpp)

if(TEST_SOURCES)
    enable_testing()

    add_executable(run_tests ${TEST_SOURCES})
    target_include_directories(run_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(run_tests
        PRIVATE Catch2::Catch2
        PRIVATE spdlog::spdlog
        PRIVATE fmt::fmt
    )

    include(CTest)
    include(Catch)
    catch_discover_tests(run_tests)
else()
    message(WARNING "Test files not found. Skipping test build.")
endif()
