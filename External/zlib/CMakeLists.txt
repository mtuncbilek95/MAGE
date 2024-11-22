cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# Make URL variable
set(REPO_URL https://github.com/madler/zlib.git)

# GLFW Library
FetchContent_Declare(
        zlib
		EXCLUDE_FROM_ALL
		GIT_REPOSITORY	${REPO_URL}
		GIT_TAG master
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(zlib)