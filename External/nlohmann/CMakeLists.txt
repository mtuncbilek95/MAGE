cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# Make URL variable
if(NOT DEFINED JSON_URL)
    set(JSON_URL https://github.com/nlohmann/json.git)
endif()

# JSON Library
FetchContent_Declare(
        nlohmann
        EXCLUDE_FROM_ALL
        GIT_REPOSITORY ${JSON_URL}
        GIT_TAG v3.11.3
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(nlohmann)