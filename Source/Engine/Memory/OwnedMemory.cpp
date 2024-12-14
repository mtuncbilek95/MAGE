#include "OwnedMemory.h"

namespace Mage
{
	OwnedMemory::OwnedMemory() : m_buffer(nullptr), m_size(0)
	{
	}

	OwnedMemory::OwnedMemory(buf data, usize size) : m_buffer(nullptr), m_size(0)
	{
		Allocate(data, size);
	}

	OwnedMemory::OwnedMemory(u8* begin, u8* end) : m_buffer(nullptr), m_size(0)
	{
		usize size = end - begin;
		if (size > 0)
		{
			m_buffer = new char[size];
			std::memcpy(m_buffer, begin, size);
			m_size = size;
		}
	}

	OwnedMemory::OwnedMemory(const OwnedMemory& other) : m_buffer(nullptr), m_size(0)
	{
		Allocate(other.m_buffer, other.m_size);
	}

	OwnedMemory::OwnedMemory(OwnedMemory&& other) noexcept : m_buffer(other.m_buffer), m_size(other.m_size)
	{
		other.m_buffer = nullptr;
		other.m_size = 0;
	}

	OwnedMemory::~OwnedMemory()
	{
		delete static_cast<char*>(m_buffer);
	}

	OwnedMemory& OwnedMemory::operator=(OwnedMemory& other)
	{
		if (this != &other)
		{
			void* newData = nullptr;
			if (other.m_buffer && other.m_size > 0)
			{
				newData = new char[other.m_size];
				std::memcpy(newData, other.m_buffer, other.m_size);
			}

			delete static_cast<char*>(m_buffer);

			m_buffer = newData;
			m_size = other.m_size;
		}
		return *this;
	}

	OwnedMemory& OwnedMemory::operator=(OwnedMemory&& other) noexcept
	{
		if (this != &other)
		{
			delete[] static_cast<char*>(m_buffer);

			m_buffer = other.m_buffer;
			m_size = other.m_size;

			other.m_buffer = nullptr;
			other.m_size = 0;
		}
		return *this;
	}

	void OwnedMemory::Allocate(const buf data, const usize size)
	{
		if (data && size > 0)
		{
			m_buffer = new char[size];
			std::memcpy(m_buffer, data, size);
			m_size = size;
		}
	}
}
