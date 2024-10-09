cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# Make URL variable
set(REPO_URL https://github.com/NVIDIAGameWorks/USD.git)

# GLFW Library
FetchContent_Declare(
        usd
		EXCLUDE_FROM_ALL
		GIT_REPOSITORY	${REPO_URL}
		GIT_TAG v19.07
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(usd)