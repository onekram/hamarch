add_executable(${PROJECT_NAME} main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE argparser)
target_link_libraries(${PROJECT_NAME} PRIVATE archiver)
target_link_libraries(${PROJECT_NAME} PRIVATE archiver_io)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})