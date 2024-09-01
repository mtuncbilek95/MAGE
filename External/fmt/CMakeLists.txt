cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# JSON Library
FetchContent_Declare(
        fmt
        EXCLUDE_FROM_ALL
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.2.1
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(fmt)