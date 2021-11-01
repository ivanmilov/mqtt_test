cmake_minimum_required(VERSION 3.1...3.21)

project(
    forklift_demo
    VERSION 1.0
    LANGUAGES CXX
)

add_subdirectory(random)
add_subdirectory(logger)
add_subdirectory(mq_client)
add_subdirectory(generator)
add_subdirectory(processor)
