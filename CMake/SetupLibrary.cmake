function(SetupLibrary TARGET)

	# Get the target type (STATIC or SHARED or EXEC)
	get_target_property(TARGET_TYPE ${TARGET} TYPE)
	message(STATUS "Setting up library ${TARGET} of type ${TARGET_TYPE}")

	if(TARGET_TYPE STREQUAL "EXECUTABLE")
	# Set the output directory for the executable
		set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
	endif()

endfunction()