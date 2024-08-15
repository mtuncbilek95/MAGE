function(FilterOS FILTERED_DIR BASE_DIR)
    
    if(WIN32)
        set(EXCLUDE_DIR "${BASE_DIR}/Platform/Linux")
    elseif(UNIX)
        set(EXCLUDE_DIR "${BASE_DIR}/Platform/Win32")
    endif()

    file(GLOB_RECURSE FILTERED_FILES "${BASE_DIR}/*.cpp" "${BASE_DIR}/*.h" "${BASE_DIR}/*.hpp")
    file(GLOB_RECURSE EXCLUDE_FILES "${EXCLUDE_DIR}/*.cpp" "${EXCLUDE_DIR}/*.h" "${EXCLUDE_DIR}/*.hpp")

    list(REMOVE_ITEM FILTERED_FILES ${EXCLUDE_FILES})

    set(${FILTERED_DIR} ${FILTERED_FILES} PARENT_SCOPE)

    # message excluded files
    message(STATUS "----------------- Excluded files: -----------------")
    foreach(EXCLUDE_FILE ${EXCLUDE_FILES})
		message(STATUS "  ${EXCLUDE_FILE}")
    endforeach()
    message(STATUS "----------------- End of excluded files -----------------")
endfunction()