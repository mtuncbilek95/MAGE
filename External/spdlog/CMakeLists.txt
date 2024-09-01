cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# JSON Library
FetchContent_Declare(
        spdlog
        EXCLUDE_FROM_ALL
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.14.1
        GIT_SHALLOW TRUE
)

option(SPDLOG_FMT_EXTERNAL "" ON)
option(SPDLOG_BUILD_TESTS OFF)
FetchContent_MakeAvailable(spdlog)