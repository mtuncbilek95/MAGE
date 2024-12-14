#include "PlatformFile.h"

#if defined(MAGE_WINDOWS)
#include "Engine/Win32/Win32File.h"
typedef Mage::Win32File PlatformLayer;
#endif

namespace Mage
{
	b8 PlatformFile::Exists(const path& dst)
	{
		return PlatformLayer::Exists(dst);
	}

	b8 PlatformFile::Create(const path& dst)
	{
		return PlatformLayer::Create(dst);
	}

	b8 PlatformFile::Read(const path& dst, ownBuf& buffer, b8 null, usize start, usize end)
	{
		return PlatformLayer::Read(dst, buffer, null, start, end);
	}

	b8 PlatformFile::Read(const path& dst, string& buffer, usize start, usize end)
	{
		return PlatformLayer::Read(dst, buffer, start, end);
	}

	b8 PlatformFile::Write(const path& dst, const shareBuf& buffer, usize start)
	{
		return PlatformLayer::Write(dst, buffer, start);
	}

	b8 PlatformFile::Write(const path& dst, const string& buffer, usize start)
	{
		return PlatformLayer::Write(dst, buffer, start);
	}

	b8 PlatformFile::Rename(const path& src, const path& dst)
	{
		return PlatformLayer::Rename(src, dst);
	}

	b8 PlatformFile::Copy(const path& src, const path& dst)
	{
		return PlatformLayer::Copy(src, dst);
	}

	b8 PlatformFile::Move(const path& src, const path& dst)
	{
		return PlatformLayer::Move(src, dst);
	}

	b8 PlatformFile::Delete(const path& dst)
	{
		return PlatformLayer::Delete(dst);
	}
}