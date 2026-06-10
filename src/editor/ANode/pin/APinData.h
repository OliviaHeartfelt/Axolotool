#pragma once

#include "../registry/FRegistryKey.h"
#include <format>

#include <QString>
#include <QDataStream>
#include <QDebug>

namespace APinData {
    class PinData {
        FRegistryKey::FRegistryKey pFlow{};
        FRegistryKey::FRegistryKey pType{};
        FRegistryKey::FRegistryKey pStyle{};

    public:
        const FRegistryKey::FRegistryKey& flow() const  { return pFlow; }
        const FRegistryKey::FRegistryKey& type() const  { return pType; }
        const FRegistryKey::FRegistryKey& style() const { return pStyle; }

        FRegistryKey::FRegistryKey& flow()  { return pFlow; }
        FRegistryKey::FRegistryKey& type()  { return pType; }
        FRegistryKey::FRegistryKey& style() { return pStyle; }

        void debug() {
            qDebug() << std::format("{{\n\tflow: {},\n\ttype: {} \n}}", pFlow.debug(), pType.debug()).c_str();
        }

        bool operator==(const PinData& other) const {
            return this->pFlow == other.pFlow && this->pType == other.pType;
        }
        friend QDataStream& operator<<(QDataStream& out, const PinData& data) { return out << data.pFlow << data.pType; }
        friend QDataStream& operator>>(QDataStream& in, PinData& data) {        return in  >> data.pFlow >> data.pType; }
    };
}