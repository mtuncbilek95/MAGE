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
			RingDeleter() = default;
			RingDeleter(const RingDeleter&) = delete;
			RingDeleter(RingDeleter&&) = default;

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

		constexpr usize Capacity() const { return m_capacity; }
		constexpr usize Size() const { return m_size; }

		class Iterator
		{
		public:
			Iterator(const RingBuffer& buffer, usize index) : m_buffer(buffer), m_index(index), m_iterateCount(0) {}

			Iterator& operator++()
			{
				m_index = (m_index + 1) % m_buffer.m_capacity;
				++m_iterateCount;
				return *this;
			}

			const T& operator*() const 
			{
				return m_buffer.m_data[m_index];
			}

			bool operator!=(const Iterator& other) const
			{
				return m_iterateCount != other.m_iterateCount;
			}
		
		private:
			const RingBuffer& m_buffer;
			usize m_index;
			usize m_iterateCount;
		};

		Iterator begin() const { return Iterator(*this, m_head); }
		Iterator end() const { return Iterator(*this, m_tail); }

	private:
		inline void Generate(usize capacity) { m_data = static_cast<T*>(::operator new(sizeof(T) * capacity)); }
		inline void Kill()
		{
			std::unique_ptr<T, RingDeleter> deleter(m_data, RingDeleter());

			for (int i = 0; i < m_size; i++)
			{
				try
				{
					m_data[m_size - 1 - i].~T();
				}
				catch (const std::exception& e)
				{
					spdlog::critical("RingBuffer: {}", e.what());
				}
				catch (...)
				{
					spdlog::critical("RingBuffer: Unknown error!");
				}
			}
		}

	private:
		T* m_data;
		usize m_capacity;
		usize m_size;
		usize m_head;
		usize m_tail;
	};
}
