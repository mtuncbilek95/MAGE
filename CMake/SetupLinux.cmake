function(SetupLinux TARGET)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBUUID REQUIRED uuid)

    if(LIBUUID_FOUND)
        target_include_directories(${TARGET} PUBLIC ${LIBUUID_INCLUDE_DIRS})
        target_link_libraries(${TARGET} PUBLIC ${LIBUUID_LIBRARIES})
    endif()
endfunction()