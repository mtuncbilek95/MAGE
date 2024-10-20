#include "RawBuffer.h"
#include "OwnedBuffer.h"

namespace MAGE
{
	RawBuffer::RawBuffer() : m_bufferData(nullptr), m_bufferSize(0)
	{
	}

	RawBuffer::RawBuffer(buf data, usize size) : m_bufferData(data), m_bufferSize(size)
	{
	}

	RawBuffer::RawBuffer(const RawBuffer& other) : m_bufferData(other.m_bufferData), m_bufferSize(other.m_bufferSize)
	{
	}

	RawBuffer::RawBuffer(const OwnedBuffer& other) : m_bufferData(other.Data()), m_bufferSize(other.Size())
	{
	}

	RawBuffer::~RawBuffer()
	{
		m_bufferData = nullptr;
		m_bufferSize = 0;
	}

	RawBuffer& RawBuffer::operator=(const RawBuffer& other)
	{
		m_bufferData = other.m_bufferData;
		m_bufferSize = other.m_bufferSize;
		return *this;
	}
}
