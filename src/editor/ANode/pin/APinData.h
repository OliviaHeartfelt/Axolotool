#pragma once

#include "../registry/IRegistry.h"
#include <format>

#include <QString>
#include <QDataStream>
#include <QDebug>

namespace APinData {
    class PinData {
        IRegistry::FRegistryKey pFlow{};
        IRegistry::FRegistryKey pType{};
        IRegistry::FRegistryKey pStyle{};
        QByteArray pBody;

    public:
        const IRegistry::FRegistryKey& flow() const  { return pFlow; }
        const IRegistry::FRegistryKey& type() const  { return pType; }
        const IRegistry::FRegistryKey& style() const { return pStyle; }
        const QByteArray& body() const { return pBody; }

        IRegistry::FRegistryKey& flow()  { return pFlow; }
        IRegistry::FRegistryKey& type()  { return pType; }
        IRegistry::FRegistryKey& style() { return pStyle; }
        QByteArray& body() { return pBody; }

        void debug() {
            qDebug() << std::format("{{\n\tflow: {},\n\ttype: {} \n}}", pFlow.debug(), pType.debug()).c_str();
        }

        bool operator==(const PinData& other) const {
            return this->pFlow == other.pFlow && this->pType == other.pType;
        }
        friend QDataStream& operator<<(QDataStream& out, const PinData& data) { return out << data.pFlow << data.pType << data.pBody; }
        friend QDataStream& operator>>(QDataStream& in, PinData& data) {        return in  >> data.pFlow >> data.pType >> data.pBody; }
    };
}