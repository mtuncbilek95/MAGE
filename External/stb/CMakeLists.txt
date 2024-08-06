cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# STBI Library
FetchContent_Declare(
        stb
        EXCLUDE_FROM_ALL
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(stb)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})