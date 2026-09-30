#pragma once

namespace RGBaseRegistryDetails::Concepts {

	template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
	concept BaseRegistryConcept = std::copyable<T> && requires {
		typename std::unordered_map<Key, T, Hash, KeyEqual>;
	};
}