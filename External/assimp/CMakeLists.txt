cmake_minimum_required(VERSION 3.26)

include(FetchContent)

# ASSIMP Library
FetchContent_Declare(
	assimp
    EXCLUDE_FROM_ALL
	GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(assimp)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Assimp build tests" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Assimp install" FORCE)
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "Assimp install PDB" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "Assimp warnings as errors" FORCE)