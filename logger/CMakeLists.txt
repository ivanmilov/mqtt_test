include(FetchContent)
FetchContent_Declare(
    fmtlib
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        8.0.1
    )
FetchContent_MakeAvailable(fmtlib)

add_library(logger INTERFACE)

target_include_directories(logger INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
    logger
    INTERFACE
        fmt::fmt-header-only
)
