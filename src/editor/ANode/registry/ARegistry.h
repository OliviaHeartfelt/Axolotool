#pragma once

#include "FRegistryKey.h"
#include "IRegistry.h"

namespace ARegistry {
	using FRegistryKey = ::FRegistryKey::FRegistryKey;

	template<typename T>
	concept DescriptorType = ::IRegistry::DescriptorType<T>;

	template<DescriptorType T>
	using IRegistry = ::IRegistry::IRegistry<T>;
}