/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */
#pragma once

#include "Engine/Definitions/Definitions.h"
#include "Engine/Definitions/StdNames.h"

#include <thread>
#include <mutex>

namespace Mage
{
	template<typename T>
	class NaiveSingleton
	{
	public:
		NaiveSingleton() = default;
		NaiveSingleton(const NaiveSingleton& other) = delete;
		~NaiveSingleton() = default;

		void operator=(const NaiveSingleton&) = delete;

		T* Instance()
		{
			if (m_instance == nullptr)
				m_instance = new T();

			return m_instance;
		}

	private:
		inline static T* m_instance;
	};

	template<typename T>
	class SafeSingleton
	{
	public:
		SafeSingleton() = default;
		SafeSingleton(const SafeSingleton& other) = delete;
		~SafeSingleton() = default;

		void operator=(const SafeSingleton&) = delete;

		static T* Get()
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			if (m_instance == nullptr)
				m_instance = new T();

			return m_instance;
		}

	private:
		inline static T* m_instance;
		inline static std::mutex m_mutex;
	};
}