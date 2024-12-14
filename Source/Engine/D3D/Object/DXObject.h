/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */
#pragma once

#include "Engine/Containers/IObject.h"

#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Mage
{
	class DXDevice;

	class DXObject : public IObject
	{
	public:
		virtual ~DXObject() = default;

		DXDevice* GetRootDevice() const { return m_rootDevice; }
		virtual void Destroy() override = 0;

	protected:
		DXDevice* m_rootDevice;
	};
}