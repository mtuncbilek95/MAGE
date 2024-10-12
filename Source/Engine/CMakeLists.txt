cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

# ----------------------- INCLUDES ---------------------------
include(SetupVulkan)
include(SetupGLFW)
# ------------------------------------------------------------

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SUBDIRECTORIES})
add_library(Engine STATIC ${SUBDIRECTORIES})
# ------------------------------------------------------------

# ------------------- INCLUDE DIRECTORIES --------------------
target_include_directories(Engine PRIVATE "${CMAKE_SOURCE_DIR}/Source")
target_link_libraries(Engine PUBLIC glfw glm::glm fmt spdlog magic_enum EnTT::EnTT)

SetupVulkan(Engine)
SetupGLFW(Engine)
DefineGlmMacros(Engine)

if(WIN32)
	include(SetupWindows)
    SetupDX12(Engine)
    SetupWin32(Engine)
endif()
# ------------------------------------------------------------