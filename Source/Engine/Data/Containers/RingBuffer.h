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
	template<typename T>
	class RingBuffer final
	{
		struct RingDeleter
		{
			void operator()(T* buffer) const { ::operator delete(buffer); }
		};

	public:
		RingBuffer(usize capacity = 1000) : m_capacity(capacity), m_head(0), m_tail(0), m_size(0)
		{
			Generate(capacity);
		}

		RingBuffer(const RingBuffer& copy) : m_capacity(copy.m_capacity), m_size(0)
		{
			Generate(copy.m_capacity);

			try
			{
				for (int i = 0; i < m_size; i++)
					Push(copy.m_data[i]);
			}
			catch (...)
			{
				Kill();
				throw;
			}
		}

		~RingBuffer()
		{
			Kill();
		}

		void Push(const T& value)
		{
			new(m_data + m_head) T(value);

			m_head = (m_head + 1) % m_capacity;

			if (m_size == m_capacity)
				m_tail = (m_tail + 1) % m_capacity;
			else
				++m_size;
		}

	private:
		inline void Generate(usize capacity) { m_data = static_cast<T*>(::operator new(sizeof(T) * capacity)); }
		inline void Kill()
		{
			Owned<T, RingDeleter> deleter(buffer, RingDeleter());

			for (int i = 0; i < m_size; i++)
				buffer[m_size - 1 - i].~T();
		}

	private:
		T* data;
		usize m_capacity;
		usize m_size;
		usize m_head;
		usize m_tail;
	};
}
