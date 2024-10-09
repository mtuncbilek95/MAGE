#pragma once

#if defined(DELUSION_WINDOWS)
#if defined(DELUSION_BUILD_DLL)
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#else
#define ENGINE_API
#endif
