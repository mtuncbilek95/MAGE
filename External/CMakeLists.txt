cmake_minimum_required(VERSION 3.26)

add_subdirectory(glfw)
add_subdirectory(imgui)
add_subdirectory(glm)
add_subdirectory(nlohmann)
add_subdirectory(stb)
add_subdirectory(magic_enum)

if(UNIX)
	add_subdirectory(uuid)
endif()

if(ENABLE_CATCH_TEST)
	add_subdirectory(catch2)
endif()