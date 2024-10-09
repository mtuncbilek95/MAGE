/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	namespace PlatformPath
	{
		String ENGINE_API GetProgramFilesDir();
		String ENGINE_API GetAppDataDir();
		String ENGINE_API GetTempDir();
		String ENGINE_API GetHomeDir();
		String ENGINE_API GetDesktopDir();
		String ENGINE_API GetDocumentsDir();
		String ENGINE_API GetDownloadsDir();
		String ENGINE_API GetExecutableDir();
		String ENGINE_API GetEngineDir();
	}
}
