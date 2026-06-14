module;

#include <concepts>

export module ARegistry;

import FRegistryKey;
import IRegistry;

export namespace ARegistry {
	using FRegistryKey = ::FRegistryKey::FRegistryKey;

	template<typename T>
	concept DescriptorType = ::IRegistry::DescriptorType<T>;

	template<DescriptorType T>
	using IRegistry = ::IRegistry::IRegistry<T>;
}