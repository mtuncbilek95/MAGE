cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

# ----------------------- INCLUDES ---------------------------
include(SetupVulkan)
include(SetupGLFW)
# ------------------------------------------------------------

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SUBDIRECTORIES})
add_library(Editor STATIC ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ------------------- INCLUDE DIRECTORIES --------------------
target_include_directories(Editor PRIVATE "${CMAKE_SOURCE_DIR}/Source")
target_link_libraries(Editor PUBLIC Engine Imgui)
# ------------------------------------------------------------