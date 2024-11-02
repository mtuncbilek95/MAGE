cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})
add_executable(Tests ${SOURCES})

target_include_directories(Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(Tests PRIVATE "${CMAKE_SOURCE_DIR}/Source")
target_link_libraries(Tests PRIVATE Engine)

target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/contrib)

include(CTest)
include(Catch)
catch_discover_tests(Tests)