cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# Make URL variable
if(NOT DEFINED SPD_URL)
    set(SPD_URL https://github.com/gabime/spdlog.git)
endif()

# JSON Library
FetchContent_Declare(
        spdlog
        EXCLUDE_FROM_ALL
        GIT_REPOSITORY ${SPD_URL}
        GIT_TAG v1.14.1
        GIT_SHALLOW TRUE
)

option(SPDLOG_FMT_EXTERNAL "" ON)
option(SPDLOG_BUILD_TESTS OFF)

FetchContent_MakeAvailable(spdlog)