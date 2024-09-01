#include "Win32Hardware.h"

#include <Windows.h>
#include <SetupAPI.h>
#include <spdlog/spdlog.h>

namespace MAGE
{
	static String GetLastErrorMessage()
	{
		// Get the error message ID, if any.
		DWORD errorID = GetLastError();
		if (errorID == 0)
			return String();

		LPSTR messageBuffer = nullptr;
		// Format the error message
		u64 size = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&messageBuffer, 0, NULL);

		// Buffer it up
		String message(messageBuffer, size);

		// Free the buffer and return the message
		LocalFree(messageBuffer);
		return message;
	}

	const String Win32Hardware::GetCPUName()
	{
		HKEY hKey;
		DWORD dwType = REG_SZ;
		char buffer[256];
		DWORD dwSize = sizeof(buffer);
		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			if (RegQueryValueExA(hKey, "ProcessorNameString", 0, &dwType, (LPBYTE)buffer, &dwSize) == ERROR_SUCCESS)
			{
				RegCloseKey(hKey);
				return String(buffer);
			}
			RegCloseKey(hKey);
		}
		else
		{
			// Log error
			spdlog::error("Failed to open registry key for CPU name: {}", GetLastErrorMessage());
		}
		return String();
	}

	const String Win32Hardware::GetGPUName()
	{
		for (int i = 0; ; i++)
		{
			DISPLAY_DEVICE dd = { sizeof(dd), 0 };
			if (!EnumDisplayDevicesA(NULL, i, &dd, EDD_GET_DEVICE_INTERFACE_NAME))
				break;

			if (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
				return String(dd.DeviceString);
		}

		return String();
	}

	u64 Win32Hardware::GetHostMemorySize()
	{
		MEMORYSTATUSEX memoryStatus;
		memoryStatus.dwLength = sizeof(memoryStatus);
		GlobalMemoryStatusEx(&memoryStatus);
		return memoryStatus.ullTotalPhys;
	}

	u64 Win32Hardware::GetDeviceMemorySize()
	{
		return u64();
	}

	u32 Win32Hardware::GetCPUCores()
	{
		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);
		return systemInfo.dwNumberOfProcessors;
	}

	u32 Win32Hardware::GetCPUThreads()
	{
		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);
		return systemInfo.dwNumberOfProcessors;
	}
}