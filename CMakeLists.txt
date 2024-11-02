cmake_minimum_required(VERSION 3.26)

# ----------------------- PROJECT NAME -----------------------
project("MAGE:Delusion")
# ------------------------------------------------------------

# ----------------------- CXX FLAGS --------------------------
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  cmake_policy(SET CMP0077 NEW)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "Enable Update Fetch" FORCE)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "CMake Warn Deprecated" FORCE)
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS OFF CACHE BOOL "CMake Suppresse Developer Warnings" FORCE)
# ------------------------------------------------------------

# ----------------- PREPROCESSOR DEFINITIONS -----------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")
# ------------------------------------------------------------

# --------------------- SETUP CONFIG -------------------------
include(SetupConfig)
SetupConfig()

option(ENABLE_CATCH_TEST "Enable testing" ON) # This one needs to be set ON or OFF in Build/${BuildType-OS}/CMakeCache.txt
# ------------------------------------------------------------

# ----------------------- SUBDIRECTORIES ---------------------
add_subdirectory("External")
add_subdirectory("Source/Engine")
add_subdirectory("Source/Editor")

add_subdirectory("Tests")
	
add_subdirectory("Example/01-Editor")
add_subdirectory("Example/02-Standalone")
add_subdirectory("Example/03-General")
# ------------------------------------------------------------