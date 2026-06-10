#pragma once

#include "../registry/IRegistry.h"

#include <QList>
#include <QReadWriteLock>

namespace APinAllowLists {
    class AllowLists {
        class List {
            QList<IRegistry::FRegistryKey> flowList;
            mutable QReadWriteLock lock;

        public:
            void add(const IRegistry::FRegistryKey& key) {
                QWriteLocker locker(&lock);
                flowList.append(key);
            }
            bool remove(const IRegistry::FRegistryKey& key) {
                QWriteLocker locker(&lock);
                return flowList.removeOne(key);
            }
            bool contains(const IRegistry::FRegistryKey& key) {
                QReadLocker locker(&lock);
                return flowList.contains(key);
            }
        };

        List pFlow;
        List pType;

    public:
        List& flow() { return pFlow; }
        List& type() { return pType; }
    };
}