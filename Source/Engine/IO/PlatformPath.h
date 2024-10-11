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
		String GetProgramFilesDir();
		String GetAppDataDir();
		String GetTempDir();
		String GetHomeDir();
		String GetDesktopDir();
		String GetDocumentsDir();
		String GetDownloadsDir();
		String GetExecutableDir();
		String GetEngineDir();
	}
}
