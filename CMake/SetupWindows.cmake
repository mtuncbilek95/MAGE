function(SetupDX12 TARGET)
	target_link_libraries(${TARGET} PUBLIC d3d12.lib dxgi.lib dxguid.lib)
endfunction()

function(SetupWin32 TARGET)
	target_link_libraries(${TARGET} PUBLIC user32.lib gdi32.lib shell32.lib Setupapi.lib)
endfunction()