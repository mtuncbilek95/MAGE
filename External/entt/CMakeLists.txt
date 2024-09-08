cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# EnTT Library
FetchContent_Declare(
    entt
    EXCLUDE_FROM_ALL
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.13.2
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(entt)
