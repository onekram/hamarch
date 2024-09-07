cmake_minimum_required(VERSION 3.12)

project(
        HammingArchiver
        VERSION 1.0
        DESCRIPTION "C++ Project"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-DPATH="${CMAKE_SOURCE_DIR}")

file(GLOB_RECURSE SOLUTION_SRC src/*.cpp)

add_executable(${PROJECT_NAME} ${SOLUTION_SRC})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/lib)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/archiver)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/archiver_io)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Add test executable
file(GLOB_RECURSE ARCHIVER_IO_SRC ${CMAKE_SOURCE_DIR}/src/archiver_io/*.cpp)
add_executable(archiver_tests ${CMAKE_SOURCE_DIR}/tests/archiver_tests.cpp ${ARCHIVER_IO_SRC})

target_include_directories(archiver_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/archiver_io)
target_link_libraries(archiver_tests PRIVATE GTest::gtest_main)

include(GoogleTest)

# Discover tests
gtest_discover_tests(archiver_tests)
