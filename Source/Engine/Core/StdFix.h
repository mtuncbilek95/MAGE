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
using Array = std::array<T, N>;

#include <string>
using String = std::string;

#include <string_view>
using StringView = std::string_view;

#include <vector>
template<typename T>
using Vector = std::vector<T>;

#include <map>
template<typename K, typename V>
using Map = std::map<K, V>;

template<typename K, typename V>
using Pair = std::pair<K, V>;

#include <set>
template<typename T>
using Set = std::set<T>;

#include <list>
template<typename T>
using List = std::list<T>;

#include <unordered_map>
template<typename K, typename V>
using HashMap = std::unordered_map<K, V>;

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
using Function = std::function<T>;
using VoidFunction = std::function<void*>;