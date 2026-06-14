module;

#include "FRegistryKey.h"
#include "IRegistry.h"
#include <concepts>

export module ARegistry;

export namespace ARegistry {
	using FRegistryKey = ::FRegistryKey::FRegistryKey;

	template<typename T>
	concept DescriptorType = ::IRegistry::DescriptorType<T>;

	template<DescriptorType T>
	using IRegistry = ::IRegistry::IRegistry<T>;
}