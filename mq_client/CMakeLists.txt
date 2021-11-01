add_library(mq_client INTERFACE)

target_include_directories(mq_client INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
    mq_client
    INTERFACE
        paho-mqttpp3
        paho-mqtt3as
        logger
)
