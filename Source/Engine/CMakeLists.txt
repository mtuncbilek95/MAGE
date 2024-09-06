cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

# ----------------------- INCLUDES ---------------------------
include(SetupVulkan)
include(SetupGLFW)
# ------------------------------------------------------------

# ---------------------- FILE ADDITION -----------------------
include(FilterFolders)
set(BASE_DIRECTORY "${CMAKE_SOURCE_DIR}/Source/Engine") # Base directory for the engine
FilterOS(FILTERED_DIRS ${BASE_DIRECTORY}) # Filter out the directories that are not needed for current OS
source_group(TREE "${CMAKE_SOURCE_DIR}/Source/Engine" FILES ${FILTERED_FILES})

add_library(Engine STATIC ${FILTERED_DIRS})
# ------------------------------------------------------------

# ------------------- INCLUDE DIRECTORIES --------------------
target_include_directories(Engine PRIVATE "${CMAKE_SOURCE_DIR}/Source")

SetupVulkan(Engine)
SetupGLFW(Engine)
DefineGlmMacros(Engine)

target_link_libraries(Engine PUBLIC glfw glm::glm nlohmann_json::nlohmann_json magic_enum fmt spdlog)

if(WIN32)
	include(SetupWindows)
    SetupDX12(Engine)
    SetupWin32(Engine)
endif()

# ------------------------------------------------------------

# -------------------- MACRO DEFINITIONS ---------------------
if(WIN32)
	target_compile_definitions(Engine PUBLIC ENGINE_API=__declspec\(dllexport\))
endif()
# ------------------------------------------------------------

