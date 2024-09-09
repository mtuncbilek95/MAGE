cmake_minimum_required (VERSION 3.26 FATAL_ERROR)

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SUBDIRECTORIES})
add_executable(EditorExample main.cpp ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ---------------------- INCLUDE DIRECTORIES ----------------
target_include_directories(EditorExample PRIVATE "${CMAKE_SOURCE_DIR}/Source")

target_link_libraries(EditorExample PRIVATE Editor Engine)
target_link_libraries(EditorExample PRIVATE EnTT::EnTT)
# ------------------------------------------------------------

# ---------------------- COMPILER FLAGS ----------------------
if(WIN32)
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()