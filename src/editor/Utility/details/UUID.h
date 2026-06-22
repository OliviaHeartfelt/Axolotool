#pragma once

namespace UUID {

    struct UUID {
        static inline QByteArray uuidToBytes(const muuid::uuid& id) {
            return QByteArray(reinterpret_cast<const char*>(&id), sizeof(muuid::uuid));
        }
        static inline std::optional<muuid::uuid> bytesToUuid(const QByteArray& bytes) {
            if (bytes.size() != 16) return std::nullopt;

            muuid::uuid id;
            std::memcpy(&id, bytes.constData(), 16);
            return id;
        }
    };
}