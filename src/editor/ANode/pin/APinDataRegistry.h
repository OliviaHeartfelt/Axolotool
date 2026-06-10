#pragma once

#include "../registry/IRegistry.h"
#include <concepts>

#include <QDataStream>
#include <QColor>

namespace APinRegistry {

    namespace Details {
        template<typename T>
        concept InternalValue = requires(const T t, T u, QDataStream & stream) {
            { stream << t } -> std::same_as<QDataStream&>;
            { stream >> u } -> std::same_as<QDataStream&>;
        };

        template<InternalValue T>
        class RegistryHelper {
            IRegistry::FRegistryKey internalKey{};
            T internalValue;

        public:
            RegistryHelper(QString source, QString id, T value)
                : internalKey({ std::move(source), std::move(id) }), internalValue(std::move(value)) {}

            IRegistry::FRegistryKey key() const { return internalKey; }
            T value() const { return internalValue; }

            bool operator==(const RegistryHelper& other) const {
                return this->internalKey.source() == other.internalKey.source() && this->internalKey.id() == other.internalKey.id();
            }

            friend QDataStream& operator<<(QDataStream& out, const RegistryHelper& data) {
                out << data.internalKey.source << data.internalKey.ID << data.internalValue;
                return out;
            }
            friend QDataStream& operator>>(QDataStream& in, RegistryHelper& data) {
                in >> data.internalKey.source >> data.internalKey.ID >> data.internalValue;
                return in;
            }
        };
    }

    // Flow
    struct FlowValue {
        qreal degree = 0.0;

        friend QDataStream& operator<<(QDataStream& out, const FlowValue& data) { return out << data.degree; }
        friend QDataStream& operator>>(QDataStream& in, FlowValue& data) {        return in  >> data.degree; }
    };
    static_assert(IRegistry::DescriptorType<Details::RegistryHelper<FlowValue>>);
    using Flow = IRegistry::IRegistry<Details::RegistryHelper<FlowValue>>;

    // Type
    struct TypeValue {
        std::size_t bitSize = 0;

        friend QDataStream& operator<<(QDataStream& out, const TypeValue& other) { return out << other.bitSize; }
        friend QDataStream& operator>>(QDataStream& in, TypeValue& other) {        return in >> other.bitSize; }
    };
    static_assert(IRegistry::DescriptorType<Details::RegistryHelper<TypeValue>>);
    using Type = IRegistry::IRegistry<Details::RegistryHelper<TypeValue>>;

    // Style
    struct StyleValue {
        QColor color = Qt::gray;
        int wireThickness = 2;

        friend QDataStream& operator<<(QDataStream& out, const StyleValue& other) { return out << other.color << other.wireThickness; }
        friend QDataStream& operator>>(QDataStream& in, StyleValue& other) { return in >> other.color >> other.wireThickness; }
    };
    static_assert(IRegistry::DescriptorType<Details::RegistryHelper<StyleValue>>);
    using Style = IRegistry::IRegistry<Details::RegistryHelper<StyleValue>>;
}