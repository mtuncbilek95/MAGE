cmake_minimum_required(VERSION 3.26)

include(FetchContent)

FetchContent_Declare(
	meta
	EXCLUDE_FROM_ALL
	GIT_REPOSITORY https://github.com/skypjack/meta.git
	GIT_SHALLOW TRUE
)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(meta)
