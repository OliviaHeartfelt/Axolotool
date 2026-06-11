#pragma once

#include "../registry/FRegistryKey.h"

#include <QList>
#include <QReadWriteLock>

namespace APinAllowLists {
    class AllowLists {
        class List {
            QList<FRegistryKey::FRegistryKey> flowList;
            mutable QReadWriteLock lock;

        public:
            void add(const FRegistryKey::FRegistryKey& key) {
                QWriteLocker locker(&lock);
                flowList.append(key);
            }
            bool remove(const FRegistryKey::FRegistryKey& key) {
                QWriteLocker locker(&lock);
                return flowList.removeOne(key);
            }
            bool contains(const FRegistryKey::FRegistryKey& key) {
                if (flowList.size() == 0) return false;
                QReadLocker locker(&lock);
                return flowList.contains(key);
            }
            const qsizetype size() {
                QReadLocker locker(&lock);
                return flowList.size();
            }
        };

        List pFlow;
        List pType;

    public:
        List& flow() { return pFlow; }
        List& type() { return pType; }
    };
}