cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

# ----------------------- INCLUDES ---------------------------
include(SetupVulkan)
include(SetupGLFW)
# ------------------------------------------------------------

# ---------------------- FILE ADDITION -----------------------
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SUBDIRECTORIES})
add_library(Engine SHARED ${SUBDIRECTORIES})

target_compile_definitions(Engine PRIVATE DELUSION_BUILD_DLL)
set_target_properties(Engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binaries")
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

# ------------------------ DLL OPTIONS -----------------------
if(WIN32)
	set_target_properties(Engine PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()
# ------------------------------------------------------------