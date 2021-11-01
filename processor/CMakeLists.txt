add_executable(proc src/main.cpp)

set_target_properties(proc PROPERTIES CXX_STANDARD 20)

target_include_directories(
    proc
    PRIVATE
        ${CMAKE_SOURCE_DIR}/common
)

target_link_libraries(
    proc
    PRIVATE
        logger
        pthread
        mq_client
)
