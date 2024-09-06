cmake_minimum_required(VERSION 3.26)

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SUBDIRECTORIES})
add_library("Resource" STATIC ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ------------------- INCLUDE DIRECTORIES --------------------
target_include_directories("Resource" PRIVATE "${CMAKE_SOURCE_DIR}/Source")
# ------------------------------------------------------------

# --------------------- LINK LIBRARIES -----------------------
target_link_libraries("Resource" PUBLIC Engine)
target_link_libraries("Resource" PUBLIC stb)
# ------------------------------------------------------------

# -------------------- MACRO DEFINITIONS ---------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_compile_definitions("Resource" PUBLIC RESOURCE_API=__declspec\(dllexport\))
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "g++" AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_compile_definitions("Resource" PUBLIC RESOURCE_API=__attribute__\(\(visibility\(\"default\"\)\)\))
endif()
# ------------------------------------------------------------