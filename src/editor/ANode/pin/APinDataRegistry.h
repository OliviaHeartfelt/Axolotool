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
            IRegistry::FRegistryKey internalKey{ "", "" };
            T internalValue;

        public:
            IRegistry::FRegistryKey key() const { return internalKey; }
            T value() const { return internalValue; }

            bool operator==(const RegistryHelper& other) const {
                return this->internalKey.source == other.internalKey.source && this->internalKey.ID == other.internalKey.ID;
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

        friend QDataStream& operator<<(QDataStream& out, const FlowValue& data) {
            out << data.degree;
            return out;
        }
        friend QDataStream& operator>>(QDataStream& in, FlowValue& data) {
            in >> data.degree;
            return in;
        }
    };
    static_assert(IRegistry::DescriptorType<Details::RegistryHelper<FlowValue>>);
    using FlowRegistry = IRegistry::IRegistry<Details::RegistryHelper<FlowValue>>;



    // Type
    struct TypeValue {
        std::size_t byteSize = 0;
        QColor wireColor = Qt::gray;
        int wireThickness = 2;

        friend QDataStream& operator<<(QDataStream& out, const TypeValue& data) {
            out << data.byteSize << data.wireColor << data.wireThickness;
            return out;
        }
        friend QDataStream& operator>>(QDataStream& in, TypeValue& data) {
            in >> data.byteSize >> data.wireColor >> data.wireThickness;
            return in;
        }
    };
    static_assert(IRegistry::DescriptorType<Details::RegistryHelper<TypeValue>>);
    using TypeRegistry = IRegistry::IRegistry<Details::RegistryHelper<TypeValue>>;


}