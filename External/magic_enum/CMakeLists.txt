cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# JSON Library
FetchContent_Declare(
        magic_enum
        EXCLUDE_FROM_ALL
        GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(magic_enum)