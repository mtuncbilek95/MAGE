#include "OwnedBuffer.h"

namespace MAGE
{
	OwnedBuffer::OwnedBuffer() : m_bufferData(nullptr), m_bufferSize(0)
	{
	}

	OwnedBuffer::OwnedBuffer(buf data, usize size) : m_bufferData(nullptr), m_bufferSize(0)
	{
		Allocate(data, size);
	}

	OwnedBuffer::OwnedBuffer(u8* begin, u8* end) : m_bufferData(nullptr), m_bufferSize(0)
	{
		usize size = end - begin;
		if (size > 0)
		{
			m_bufferData = new char[size];
			std::memcpy(m_bufferData, begin, size);
			m_bufferSize = size;
		}
	}

	OwnedBuffer::OwnedBuffer(const OwnedBuffer& other) : m_bufferData(nullptr), m_bufferSize(0)
	{
		Allocate(other.m_bufferData, other.m_bufferSize);
	}

	OwnedBuffer::OwnedBuffer(OwnedBuffer&& other) noexcept : m_bufferData(other.m_bufferData), m_bufferSize(other.m_bufferSize)
	{
		other.m_bufferData = nullptr;
		other.m_bufferSize = 0;
	}

	OwnedBuffer::~OwnedBuffer()
	{
		delete static_cast<char*>(m_bufferData);
	}

	OwnedBuffer& OwnedBuffer::operator=(OwnedBuffer& other)
	{
		if (this != &other)
		{
			void* newData = nullptr;
			if (other.m_bufferData && other.m_bufferSize > 0)
			{
				newData = new char[other.m_bufferSize];
				std::memcpy(newData, other.m_bufferData, other.m_bufferSize);
			}

			delete static_cast<char*>(m_bufferData);

			m_bufferData = newData;
			m_bufferSize = other.m_bufferSize;
		}
		return *this;
	}

	OwnedBuffer& OwnedBuffer::operator=(OwnedBuffer&& other) noexcept
	{
		if (this != &other)
		{
			delete[] static_cast<char*>(m_bufferData);

			m_bufferData = other.m_bufferData;
			m_bufferSize = other.m_bufferSize;

			other.m_bufferData = nullptr;
			other.m_bufferSize = 0;
		}
		return *this;
	}

	void OwnedBuffer::Allocate(const buf data, const usize size)
	{
		if (data && size > 0)
		{
			m_bufferData = new char[size];
			std::memcpy(m_bufferData, data, size);
			m_bufferSize = size;
		}
	}
} // namespace IOAlloc
