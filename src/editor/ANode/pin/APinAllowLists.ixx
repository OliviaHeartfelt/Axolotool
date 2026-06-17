module;

#include <initializer_list>

#include <QSet>
#include <QReadWriteLock>

export module APinAllowLists;

import ARegistry;

export namespace APinAllowLists {

    class List {
        QSet<ARegistry::FRegistryKey> flowList;
        mutable QReadWriteLock lock;

    public:
        List() = default;
        List(std::initializer_list<ARegistry::FRegistryKey> list) : flowList(list) {}
        ~List() = default;

        List(const List& other) {
            QWriteLocker locker(&other.lock);
            flowList = other.flowList;
        }
        List& operator= (const List& other) {
            if (this == &other) return *this;
            if (this < &other) {
                QWriteLocker locker(&lock);
                QReadLocker otherLocker(&other.lock);
                flowList = other.flowList;
            }
            else {
                QReadLocker otherLocker(&other.lock);
                QWriteLocker locker(&lock);
                flowList = other.flowList;
            }
            return *this;
        }

        List(List&& other) noexcept {
            QWriteLocker otherLocker(&other.lock);
            flowList = std::move(other.flowList);
        }
        List& operator=(List&& other) noexcept {
            if (this == &other) return *this;

            if (this < &other) {
                QWriteLocker locker(&lock);
                QWriteLocker otherLocker(&other.lock);
                flowList = std::move(other.flowList);
            }
            else {
                QWriteLocker otherLocker(&other.lock);
                QWriteLocker locker(&lock);
                flowList = std::move(other.flowList);
            }
            return *this;
        }

        void add(const ARegistry::FRegistryKey& key) {
            QWriteLocker locker(&lock);
            flowList.insert(key);
        }
        bool remove(const ARegistry::FRegistryKey& key) {
            QWriteLocker locker(&lock);
            return flowList.remove(key);
        }
        bool contains(const ARegistry::FRegistryKey& key) const {
            QReadLocker locker(&lock);
            return flowList.contains(key);
        }
        const qsizetype size() const {
            QReadLocker locker(&lock);
            return flowList.size();
        }
    };
    
    struct AllowLists {
        List flow;
        List type;
    };
}