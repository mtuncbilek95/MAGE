cmake_minimum_required (VERSION 3.26 FATAL_ERROR)

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SUBDIRECTORIES})
add_executable("03-ResourceManagement" main.cpp ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ---------------------- INCLUDE DIRECTORIES ----------------
target_include_directories("03-ResourceManagement" PRIVATE "${CMAKE_SOURCE_DIR}/Source")
target_link_libraries("03-ResourceManagement" PRIVATE Engine)
# ------------------------------------------------------------

# ---------------------- COMPILER FLAGS ----------------------
if(WIN32)
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()