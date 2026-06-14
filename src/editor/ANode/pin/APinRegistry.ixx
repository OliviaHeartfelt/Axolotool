module;

#include <concepts>

#include <QDataStream>
#include <QColor>

export module APinRegistry;

import ARegistry;

export namespace APinRegistry {

    // Flow
    struct FlowValue {
        qreal degree = 0.0;

        friend QDataStream& operator<<(QDataStream& out, const FlowValue& data) { return out << data.degree; }
        friend QDataStream& operator>>(QDataStream& in, FlowValue& data) {        return in  >> data.degree; }
    };
    static_assert(ARegistry::DescriptorType<FlowValue>);
    using Flow = ARegistry::IRegistry<FlowValue>;

    // Type
    struct TypeValue {
        std::size_t bitSize = 0;

        friend QDataStream& operator<<(QDataStream& out, const TypeValue& other) { return out << other.bitSize; }
        friend QDataStream& operator>>(QDataStream& in, TypeValue& other) {        return in >> other.bitSize; }
    };
    static_assert(ARegistry::DescriptorType<TypeValue>);
    using Type = ARegistry::IRegistry<TypeValue>;

    // Style
    struct StyleValue {
        QColor color = Qt::gray;
        int wireThickness = 2;

        friend QDataStream& operator<<(QDataStream& out, const StyleValue& other) { return out << other.color << other.wireThickness; }
        friend QDataStream& operator>>(QDataStream& in, StyleValue& other) { return in >> other.color >> other.wireThickness; }
    };
    static_assert(ARegistry::DescriptorType<StyleValue>);
    using Style = ARegistry::IRegistry<StyleValue>;
}