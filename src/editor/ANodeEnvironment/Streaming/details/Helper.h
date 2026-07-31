#pragma once

namespace STStreamerDetails::Helper {

    template<typename T>
    using Cache = std::unordered_map<muuid::uuid, T>;

    template<typename T>
    using PayloadMap = std::map<muuid::uuid, T>;

    template<typename T, std::invocable FetchFunc>
    const T* getOrFetch(Cache<T>& cache, const muuid::uuid& recordId, FetchFunc&& fetch) {
        auto it = cache.find(recordId);
        if (it != cache.end()) return &it->second;

        if (const std::optional<T> opt = fetch(); opt.has_value()) {
            auto [insertedIt, _] = cache.emplace(recordId, std::move(*opt));
            return &insertedIt->second;
        }
        return nullptr;
    }

    template<typename T, std::invocable FetchFunc>
    bool hydratePayloadMap(Cache<T>& cache, PayloadMap<T>& payloadMap, const muuid::uuid& recordId, FetchFunc&& fetch) {
        if (const T* val = getOrFetch(cache, recordId, std::forward<FetchFunc>(fetch))) {
            payloadMap.emplace(recordId, *val);
            return true;
        }
        return false;
    }

    template<typename T, std::invocable FetchFunc>
    bool hydratePayloadOpt(Cache<T>& cache, std::optional<T>& payloadOpt, const muuid::uuid& recordId, FetchFunc&& fetch) {
        if (const T* val = getOrFetch(cache, recordId, std::forward<FetchFunc>(fetch))) {
            payloadOpt = *val;
            return true;
        }
        return false;
    }
}