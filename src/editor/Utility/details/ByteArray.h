#pragma once

namespace ByteArray {

    struct ByteArray {

        static QByteArray toQByteArray(const std::vector<uint8_t>& vec) {
            return QByteArray(reinterpret_cast<const char*>(vec.data()), static_cast<qsizetype>(vec.size()));
        }
    };
}