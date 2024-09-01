#include "PlatformTime.h"

#if defined(MAGE_WINDOWS)
#include "Win32/Win32Time.h"
typedef MAGE::Win32Time Dependency;
#endif

namespace MAGE
{
	PlatformTime& PlatformTime::Get()
	{
		static Dependency instance;
		return instance;
	}

	void PlatformTime::Start()
	{
		StartImpl();
	}

	void PlatformTime::Stop()
	{
		StopImpl();
	}

	f64 PlatformTime::GetInNanoSec()
	{
		return GetInNanoSecImpl();
	}

	f64 PlatformTime::GetInMicroSec()
	{
		return GetInMicroSecImpl();
	}

	f64 PlatformTime::GetInMilliSec()
	{
		return GetInMilliSecImpl();
	}

	f64 PlatformTime::GetInSec()
	{
		return GetInSecImpl();
	}

	const Time PlatformTime::GetDateTime()
	{
		return Dependency::GetDateTimeOS();
	}
}