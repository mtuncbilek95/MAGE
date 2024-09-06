#pragma once

/*
 * Everything has been seen in here is just a simple alias for the standard library. Seeing 
 * snake_case in this engine lowers the quality of readability. In future updates, all those
 * aliases will be removed and a wrapper will be created for the standard library in order to
 * protect the codebase from any snake_case in the standard library.
 */

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

#include <unordered_map>
template<typename K, typename V>
using HashMap = std::unordered_map<K, V>;

#include <unordered_set>
template<typename T>
using HashSet = std::unordered_set<T>;

#include <list>
template<typename T>
using List = std::list<T>;

template<typename T>
using Hash = std::hash<T>;

#include <memory>
template<typename T>
using SharedPtr = std::shared_ptr<T>;
template<typename T>
using OwnedPtr = std::unique_ptr<T>;
template<typename T>
using WeakPtr = std::weak_ptr<T>;

#include <functional>
template<typename T>
using Function = std::function<T>;
using VoidFunction = Function<void()>;

#include <deque>
template<typename T>
using Deque = std::deque<T>;

#include <queue>
template<typename T>
using Queue = std::queue<T>;

template<typename T, typename...Args>
inline SharedPtr<T> MakeShared(Args&&...args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename...Args>
inline OwnedPtr<T> MakeOwned(Args&&...args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}