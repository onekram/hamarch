cmake_minimum_required(VERSION 3.12)

project(
        labwork6
        VERSION 1.0
        DESCRIPTION "C++ Year 2023 labword6"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(lib)
add_subdirectory(bin)
add_subdirectory(archiver)
add_subdirectory(archiver_io)
enable_testing()
add_subdirectory(tests)

