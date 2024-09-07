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

add_subdirectory(tests)