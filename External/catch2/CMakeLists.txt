cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# Make URL variable
if(NOT DEFINED CATCH_URL)
	set(CATCH_URL https://github.com/catchorg/Catch2.git)
endif()

# Catch2 library
FetchContent_Declare(
	Catch2
	EXCLUDE_FROM_ALL
	GIT_REPOSITORY ${CATCH_URL}
	GIT_TAG v3.6.0
	GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(Catch2)