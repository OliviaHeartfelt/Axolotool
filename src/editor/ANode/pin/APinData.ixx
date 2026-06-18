export module APinData;

import ARegistry;

export namespace APinData {

    class PinData {
        ARegistry::FRegistryKey pFlow;
        ARegistry::FRegistryKey pType;
        ARegistry::FRegistryKey pStyle;
        mutable QReadWriteLock lock;

    public:
        PinData() : pFlow({}), pType({}), pStyle({}) {}
        PinData(const ARegistry::FRegistryKey& pFlow, const ARegistry::FRegistryKey& pType, const ARegistry::FRegistryKey& pStyle) :
            pFlow(pFlow), pType(pType), pStyle(pStyle) {}
        ~PinData() = default;

        PinData(const PinData& other) {
            QReadLocker otherLocker(&other.lock);
            pFlow = other.pFlow; pType = other.pType; pStyle = other.pStyle;
        }
        PinData& operator=(const PinData& other) {
            if (this == &other) return *this;
            if (this < &other) {
                QWriteLocker locker(&lock);
                QReadLocker otherLocker(&other.lock); 
                pFlow = other.pFlow; pType = other.pType; pStyle = other.pStyle;
            }
            else {
                QReadLocker otherLocker(&other.lock);
                QWriteLocker locker(&lock);
                pFlow = other.pFlow; pType = other.pType; pStyle = other.pStyle;
            }
            return *this;
        }

        PinData(PinData&& other) noexcept {
            pFlow = std::move(other.pFlow); pType = std::move(other.pType); pStyle = std::move(other.pStyle);
        }
        PinData& operator=(PinData&& other) noexcept {
            if (this == &other) return *this;
            QWriteLocker locker(&lock);
            pFlow = std::move(other.pFlow); pType = std::move(other.pType); pStyle = std::move(other.pStyle);
            return *this;
        }

        const ARegistry::FRegistryKey flow() { QReadLocker locker(&lock);  return pFlow; }
        const ARegistry::FRegistryKey type() const {    QReadLocker locker(&lock);  return pType; }
        const ARegistry::FRegistryKey style() const {   QReadLocker locker(&lock);  return pStyle; }

        void flow(ARegistry::FRegistryKey newFlow) { QWriteLocker locker(&lock); pFlow = newFlow; }
        void type(ARegistry::FRegistryKey newType) { QWriteLocker locker(&lock); pType = newType; }
        void style(ARegistry::FRegistryKey newStyle) { QWriteLocker locker(&lock); pStyle = newStyle; }

        void debug() const {
            QReadLocker locker(&lock);
            qDebug() << std::format("{{\n\tflow: {},\n\ttype: {},\n\tstyle: {}\n}}", pFlow.debug(), pType.debug(), pStyle.debug()).c_str();
        }

        bool operator==(const PinData& other) const {
            QReadLocker locker(&lock);
            QReadLocker otherLocker(&other.lock);
            return this->pFlow == other.pFlow && this->pType == other.pType;
        }
        friend QDataStream& operator<<(QDataStream& out, const PinData& data) {
            QReadLocker locker(&data.lock);
            return out << data.pFlow << data.pType;
        }
        friend QDataStream& operator>>(QDataStream& in, PinData& data) {        
            QReadLocker locker(&data.lock);
            return in  >> data.pFlow >> data.pType; 
        }
    };
}