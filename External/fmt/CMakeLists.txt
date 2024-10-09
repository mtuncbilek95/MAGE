cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# Make URL variable
if(NOT DEFINED FMT_URL)
    set(FMT_URL https://github.com/fmtlib/fmt.git)
endif()

# FMT Library
FetchContent_Declare(
        fmt
        EXCLUDE_FROM_ALL
        GIT_REPOSITORY ${FMT_URL}
        GIT_TAG 10.2.1
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(fmt)