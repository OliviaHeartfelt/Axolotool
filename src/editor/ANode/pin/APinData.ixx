module;

#include <format>

#include <QString>
#include <QDataStream>
#include <QDebug>

export module APinData;

import ARegistry;

export namespace APinData {

    class PinData {
        ARegistry::FRegistryKey pFlow{};
        ARegistry::FRegistryKey pType{};
        ARegistry::FRegistryKey pStyle{};

    public:
        const ARegistry::FRegistryKey& flow() const  { return pFlow; }
        const ARegistry::FRegistryKey& type() const  { return pType; }
        const ARegistry::FRegistryKey& style() const { return pStyle; }

        ARegistry::FRegistryKey& flow()  { return pFlow; }
        ARegistry::FRegistryKey& type()  { return pType; }
        ARegistry::FRegistryKey& style() { return pStyle; }

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