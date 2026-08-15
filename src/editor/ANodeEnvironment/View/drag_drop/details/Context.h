#pragma once

namespace VWDragDropDetails::Context {

    template<typename T>
    concept ItemData = requires (T t, const T c_t, QDataStream dataStream) {
        { dataStream << c_t } -> std::same_as<QDataStream&>;
        { dataStream >> t }   -> std::same_as<QDataStream&>;
        { c_t == c_t }        -> std::same_as<bool>;
    };
}