#include "Monitor.h"

#if defined(DELUSION_WINDOWS)
#include <Windows.h>
#endif

namespace MAGE
{
	Monitor::Monitor(const MonitorProps& desc) : m_props(desc)
	{
	}

	Monitor::~Monitor()
	{
	}

#if defined(DELUSION_WINDOWS)
    static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC, LPRECT, LPARAM lParam)
    {
        vector<MonitorProps>* monitorPropsList = reinterpret_cast<vector<MonitorProps>*>(lParam);

        MonitorProps props;
        MONITORINFOEX monitorInfo = {};
        monitorInfo.cbSize = sizeof(MONITORINFOEX);

        if (GetMonitorInfo(hMonitor, &monitorInfo)) {
            props.monitorName = monitorInfo.szDevice;
            props.isPrimary = (monitorInfo.dwFlags & MONITORINFOF_PRIMARY) != 0;

            DEVMODE devMode = {};
            devMode.dmSize = sizeof(DEVMODE);
            if (EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &devMode)) {
                props.currentSize = { static_cast<u32>(devMode.dmPelsWidth), static_cast<u32>(devMode.dmPelsHeight) };
                props.currentHertz = devMode.dmDisplayFrequency;
            }

            for (int i = 0; EnumDisplaySettings(monitorInfo.szDevice, i, &devMode); ++i) {
                props.supportedSizes.push_back({ static_cast<u32>(devMode.dmPelsWidth), static_cast<u32>(devMode.dmPelsHeight) });
            }

            monitorPropsList->push_back(std::move(props));
        }

        return TRUE;
    }

	Monitor MonitorHelper::GetPrimaryMonitor()
	{
        vector<MonitorProps> monitorPropsList;
        EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, reinterpret_cast<LPARAM>(&monitorPropsList));

        for (auto& prop : monitorPropsList)
            if (prop.isPrimary)
                return Monitor(prop);
	}

	vector<Monitor> MonitorHelper::GetAllMonitors()
	{
        vector<MonitorProps> monitorPropsList;
        vector<Monitor> monitors;
        EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, reinterpret_cast<LPARAM>(&monitorPropsList));

        for (auto& prop : monitorPropsList)
            monitors.emplace_back(prop);

        return monitors;
	}
#endif
}


