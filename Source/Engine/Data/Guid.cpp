#include "Guid.h"

#include <format>

#if defined(DELUSION_WINDOWS)
#include <objbase.h>
#endif

namespace MAGE
{
#if defined(DELUSION_WINDOWS)
	Guid Guid::GenerateID()
	{
		Guid guid = {};
		CoCreateGuid((GUID*)(&guid.m_a));
		return guid;
	}
#endif

	Guid::Guid() : m_a(0), m_b(0), m_c(0), m_d(0)
	{
	}

	Guid::Guid(u32 a, u16 b, u16 c, u64 d) : m_a(a), m_b(b), m_c(c), m_d(d)
	{
	}

	Guid::Guid(const Guid& other) : m_a(other.m_a), m_b(other.m_b), m_c(other.m_c), m_d(other.m_d)
	{
	}

	Guid::Guid(const string& str) : m_a(0), m_b(0), m_c(0), m_d(0)
	{
		std::from_chars_result result = std::from_chars(str.data(), str.size() + str.data(), m_a, 16);
	}

	string Guid::ToString()
	{
		char buffer[64];
		std::snprintf(buffer, sizeof(buffer), "%08x-%04x-%04x-%04x-%012llx", m_a, m_b, m_c, (u16)m_d, m_d >> 16);
		return buffer;
	}

	b8 Guid::operator==(const Guid& other) const
	{
		return other.m_a == m_a && other.m_b == m_b && other.m_c == m_c && other.m_d == m_d;
	}

	b8 Guid::operator!=(const Guid& other) const
	{
		return !(other == *this);
	}

	usize GuidHash::operator()(const Guid& other) const
	{
		usize seed = 0;
		auto hashCombine = [](usize seed, usize value) {
			return seed ^ (value + 0x9e3779b9 + (seed << 6) + (seed >> 2));
			};

		seed = hashCombine(seed, std::hash<u32>{}(other.m_a));
		seed = hashCombine(seed, std::hash<u16>{}(other.m_b));
		seed = hashCombine(seed, std::hash<u16>{}(other.m_c));
		seed = hashCombine(seed, std::hash<u64>{}(other.m_d));

		return seed;
	}
}
