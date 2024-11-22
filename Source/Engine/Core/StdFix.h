/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include <array>
template<typename T, size_t N>
using array = std::array<T, N>;

#include <string>
using string = std::string;

#include <string_view>
using stringView = std::string_view;

#include <vector>
template<typename T>
using vector = std::vector<T>;

#include <map>
template<typename K, typename V>
using map = std::map<K, V>;

template<typename K, typename V>
using pair = std::pair<K, V>;

#include <queue>
template<typename T>
using queue = std::queue<T>;

#include <set>
template<typename T>
using set = std::set<T>;

#include <list>
template<typename T>
using List = std::list<T>;

#include <unordered_map>
template<typename K, typename V, typename...Args>
using hashmap = std::unordered_map<K, V, Args...>;

#include <span>
template<typename T>
using span = std::span<T>;

#include <memory>
template<typename T>
using Owned = std::unique_ptr<T>;

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T>
using Weak = std::weak_ptr<T>;

template<typename T, typename... Args>
Owned<T> MakeOwned(Args&&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
Shared<T> MakeShared(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

#include <functional>
template<typename T>
using function = std::function<T>;
using voidPtrFunc = std::function<void*>;
using voidFunc = std::function<void()>;
