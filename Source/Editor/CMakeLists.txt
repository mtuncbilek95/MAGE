cmake_minimum_required(VERSION 3.26)

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SUBDIRECTORIES})
add_library(Editor STATIC ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ------------------- INCLUDE DIRECTORIES --------------------
target_include_directories(Editor PRIVATE "${CMAKE_SOURCE_DIR}/Source")
# ------------------------------------------------------------

# --------------------- LINK LIBRARIES -----------------------
target_link_libraries(Editor PUBLIC Engine)
target_link_libraries(Editor PUBLIC Imgui)
# ------------------------------------------------------------

# -------------------- MACRO DEFINITIONS ---------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_compile_definitions("Editor" PUBLIC EDITOR_API=__declspec\(dllexport\))
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "g++" AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_compile_definitions("Resource" PUBLIC EDITOR_API=__attribute__\(\(visibility\(\"default\"\)\)\))
endif()
# ------------------------------------------------------------