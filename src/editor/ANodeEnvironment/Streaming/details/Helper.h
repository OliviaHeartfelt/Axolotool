#pragma once

#include "../../Registry/ARegistry.h"

namespace STStreamerDetails::Helper {

    template<typename T>
    using Cache = ARegistry::RegistryT<T>;

    template<typename T, typename FetchFunc>
    inline std::optional<T> getOrFetch(Cache<T>& cacheRegistry, const muuid::uuid& recordId, FetchFunc&& fetch) {
        if (auto record = cacheRegistry.at(recordId)) {
            return record;
        }

        if (std::optional<T> opt = std::forward<FetchFunc>(fetch)()) {
            cacheRegistry.insert(recordId, *opt);
            return opt;
        }

        return std::nullopt;
    }

    template<typename T, typename FetchFunc>
    inline bool ensureCached(Cache<T>& cacheRegistry, const muuid::uuid& recordId, FetchFunc&& fetch) {
        return getOrFetch(cacheRegistry, recordId, std::forward<FetchFunc>(fetch)).has_value();
    }
}