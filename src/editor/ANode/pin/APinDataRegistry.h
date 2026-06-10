#pragma once

#include "../registry/FRegistryKey.h"
#include "../registry/IRegistry.h"
#include <concepts>

#include <QDataStream>
#include <QColor>

namespace APinRegistry {

    // Flow
    struct FlowValue {
        qreal degree = 0.0;

        friend QDataStream& operator<<(QDataStream& out, const FlowValue& data) { return out << data.degree; }
        friend QDataStream& operator>>(QDataStream& in, FlowValue& data) {        return in  >> data.degree; }
    };
    static_assert(IRegistry::DescriptorType<FlowValue>);
    using Flow = IRegistry::IRegistry<FlowValue>;

    // Type
    struct TypeValue {
        std::size_t bitSize = 0;

        friend QDataStream& operator<<(QDataStream& out, const TypeValue& other) { return out << other.bitSize; }
        friend QDataStream& operator>>(QDataStream& in, TypeValue& other) {        return in >> other.bitSize; }
    };
    static_assert(IRegistry::DescriptorType<TypeValue>);
    using Type = IRegistry::IRegistry<TypeValue>;

    // Style
    struct StyleValue {
        QColor color = Qt::gray;
        int wireThickness = 2;

        friend QDataStream& operator<<(QDataStream& out, const StyleValue& other) { return out << other.color << other.wireThickness; }
        friend QDataStream& operator>>(QDataStream& in, StyleValue& other) { return in >> other.color >> other.wireThickness; }
    };
    static_assert(IRegistry::DescriptorType<StyleValue>);
    using Style = IRegistry::IRegistry<StyleValue>;
}