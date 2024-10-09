cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# Make URL variable
if(NOT DEFINED ENTT_URL)
    set(ENTT_URL https://github.com/skypjack/entt.git)
endif()

# FMT Library
FetchContent_Declare(
        entt
        EXCLUDE_FROM_ALL
        GIT_REPOSITORY ${ENTT_URL}
        GIT_TAG v3.13.2
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(entt)