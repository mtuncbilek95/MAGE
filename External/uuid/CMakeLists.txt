cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# EnTT Library
FetchContent_Declare(
    uuid
    EXCLUDE_FROM_ALL
    GIT_REPOSITORY https://github.com/libstud/libstud-uuid.git
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(uuid)