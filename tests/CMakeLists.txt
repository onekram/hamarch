include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE ARCHIVER_IO_SRC ${CMAKE_SOURCE_DIR}/src/archiver_io/*.cpp)
add_executable(archiver_tests archiver_tests.cpp ${ARCHIVER_IO_SRC})

target_include_directories(archiver_tests PRIVATE ${CMAKE_SOURCE_DIR}/src/archiver_io)
target_link_libraries(archiver_tests PRIVATE GTest::gtest_main)
