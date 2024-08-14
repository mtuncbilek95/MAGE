function(SetupLinux TARGET)
    find_package(Wayland REQUIRED)
    find_package(EGL REQUIRED)

    if(Wayland_FOUND)
        target_include_directories(${TARGET} PUBLIC ${WAYLAND_INCLUDE_DIRS})
        target_link_libraries(${TARGET} PUBLIC ${WAYLAND_LIBRARIES})
    endif()

    if(EGL_FOUND)
        target_include_directories(${TARGET} PUBLIC ${EGL_INCLUDE_DIRS})
        target_link_libraries(${TARGET} PUBLIC ${EGL_LIBRARIES})
    endif()
endfunction()