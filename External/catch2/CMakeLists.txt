cmake_minimum_required(VERSION 3.26)

include(FetchContent)

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG v3.6.0
	GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(Catch2)