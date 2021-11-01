add_executable(gen src/main.cpp)

set_target_properties(gen PROPERTIES CXX_STANDARD 20)

target_include_directories(
    gen
    PRIVATE
        ${CMAKE_SOURCE_DIR}/common
)

target_link_libraries(
    gen
    PRIVATE
        pthread
        random
        mq_client
        logger
)
