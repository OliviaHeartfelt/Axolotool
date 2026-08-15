#pragma once

namespace ARegistryDetails::RegistryTraits {

    using UUIDPairKey = std::pair<muuid::uuid, muuid::uuid>;

    struct WirePairHash {
        std::size_t operator()(const UUIDPairKey& p) const noexcept {
            const std::size_t h1 = std::hash<muuid::uuid>{}(p.first);
            const std::size_t h2 = std::hash<muuid::uuid>{}(p.second);

            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };

    struct WirePairEqual {
        bool operator()(const UUIDPairKey& lhs, const UUIDPairKey& rhs) const noexcept { return lhs == rhs; }
    };
}