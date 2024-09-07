cmake_minimum_required(VERSION 3.26)

# ---------------------- FILE ADDITION -----------------------
include(FilterFolders)
set(BASE_DIRECTORY "${CMAKE_SOURCE_DIR}/UnitTest/Source") # Base directory for the engine
FilterOS(FILTERED_DIRS ${BASE_DIRECTORY}) # Filter out the directories that are not needed for current OS
source_group(TREE "${CMAKE_SOURCE_DIR}/UnitTest/Source" FILES ${FILTERED_FILES})

add_executable(UnitTest ${FILTERED_DIRS})
# ------------------------------------------------------------

# ------------------- INCLUDE DIRECTORIES --------------------
target_include_directories(UnitTest PRIVATE "${CMAKE_SOURCE_DIR}/Source")

target_link_libraries(UnitTest PRIVATE Engine)
target_link_libraries(UnitTest PRIVATE Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/contrib)
include(CTest)
include(Catch)
catch_discover_tests(UnitTest)

add_definitions(-DTEST_FOLDER_PATH=${CMAKE_CURRENT_SOURCE_DIR})