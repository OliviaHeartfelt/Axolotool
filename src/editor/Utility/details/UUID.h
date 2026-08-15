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

        static void toDataStream(QDataStream& out, const muuid::uuid& id) {
            out.writeRawData(reinterpret_cast<const char*>(id.bytes.data()), 16);
        }
        static std::optional<muuid::uuid> fromDataStream(QDataStream& in) {
            muuid::uuid id{};
            const int bytesRead = in.readRawData(reinterpret_cast<char*>(id.bytes.data()), 16);

            if (bytesRead == 16) return id;

            in.setStatus(QDataStream::ReadPastEnd);
            return std::nullopt;
        }
    };
}