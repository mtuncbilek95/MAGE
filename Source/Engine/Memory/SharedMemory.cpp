#include "SharedMemory.h"
#include "OwnedMemory.h"

namespace Mage
{
	SharedMemory::SharedMemory() : m_buffer(nullptr), m_size(0)
	{
	}

	SharedMemory::SharedMemory(buf data, usize size) : m_buffer(data), m_size(size)
	{
	}

	SharedMemory::SharedMemory(const SharedMemory& other) : m_buffer(other.m_buffer), m_size(other.m_size)
	{
	}

	SharedMemory::SharedMemory(SharedMemory&& other) noexcept : m_buffer(other.m_buffer), m_size(other.m_size)
	{
		other.m_buffer = nullptr;
		other.m_size = 0;
	}

	SharedMemory::SharedMemory(const OwnedMemory& other) : m_buffer(other.Data()), m_size(other.Size())
	{
	}

	SharedMemory::~SharedMemory()
	{
		m_buffer = nullptr;
		m_size = 0;
	}

	SharedMemory& SharedMemory::operator=(const SharedMemory& other)
	{
		m_buffer = other.m_buffer;
		m_size = other.m_size;

		return *this;
	}

	SharedMemory& SharedMemory::operator=(const OwnedMemory& other)
	{
		m_buffer = other.Data();
		m_size = other.Size();

		return *this;
	}
}