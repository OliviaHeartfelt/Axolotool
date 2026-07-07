#pragma once

#include "details/Config.h"
#include "details/Init.h"
#include "details/Create.h"
#include "details/Read.h"
#include "details/Update.h"
#include "details/Delete.h"

#include "../NDConcepts.h"
#include "../NDHelpers.h"

namespace NDPin {
    struct ComponentFriendTag {
        template<typename DBContext>
        static typename DBContext::StorageKey createKey() { return {}; }
    };

    template<NDConcepts::DatabaseProvider DBContext>
    class Component {
        DBContext* parent;

        QSqlDatabase database() const { return parent->getDatabase(ComponentFriendTag::createKey<DBContext>()); }

    public:
        explicit Component(DBContext* parentCtx) : parent(parentCtx) {}

        QStringList existsTables(const bool value) const {
            QStringList list;
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            const QStringList currentTables = db.tables();

            if (currentTables.contains("pin",            Qt::CaseInsensitive) == value) list.append("pin");
            if (currentTables.contains("pin_allow_flow", Qt::CaseInsensitive) == value) list.append("pin_allow_flow");
            if (currentTables.contains("pin_allow_type", Qt::CaseInsensitive) == value) list.append("pin_allow_type");
            return list;
        }

        // 0. Init
        bool createAllTables() {
            return NDHelpers::useQuery(database(), [](QSqlQuery& query) {
                return NDPinDetails::Init::createAllTables(query);
            });
        }

        // 1. Create
        bool createPin(const NDPinDetails::Config::CreatePinRecord& newPin) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinDetails::Create::create(query, newPin);
            });
        }
        bool createPin(QSqlQuery& query, const NDPinDetails::Config::CreatePinRecord& newPin) {
            return NDPinDetails::Create::create(query, newPin);
        }

        bool createAllowFlows(const muuid::uuid& pinId, const QList<muuid::uuid>& allowedTypes) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinDetails::Create::createAllowFlows(query, pinId, allowedTypes);
            });
        }
        bool createAllowFlows(QSqlQuery& query, const muuid::uuid& pinId, const QList<muuid::uuid>& allowedTypes) {
            return NDPinDetails::Create::createAllowFlows(query, pinId, allowedTypes);
        }

        bool createAllowTypes(const muuid::uuid& pinId, const QList<muuid::uuid>& allowedTypes) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinDetails::Create::createAllowTypes(query, pinId, allowedTypes);
            });
        }
        bool createAllowTypes(QSqlQuery& query, const muuid::uuid& pinId, const QList<muuid::uuid>& allowedTypes) {
            return NDPinDetails::Create::createAllowTypes(query, pinId, allowedTypes);
        }

        // 2. Read
        std::optional<NDPinDetails::Config::FullPinRecord> get(const muuid::uuid& id, const bool continueAtFail = false) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) -> std::optional<NDPinDetails::Config::FullPinRecord> {
                return NDPinDetails::Read::get(query, id, continueAtFail);
            });
        }
        std::optional<NDPinDetails::Config::FullPinRecord> get(QSqlQuery& query, const muuid::uuid& id, const bool continueAtFail = false) {
            return NDPinDetails::Read::get(query, id, continueAtFail);
        }

        std::optional<NDPinDetails::Config::PinRecord> getPin(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDPinDetails::Read::getPin(query, id);
            });
        }
        std::optional<NDPinDetails::Config::PinRecord> getPin(QSqlQuery& query, const muuid::uuid& id) {
            return NDPinDetails::Read::getPin(query, id);
        }

        std::optional<QList<muuid::uuid>> getAllowFlows(const muuid::uuid& pinId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDPinDetails::Read::getAllowFlows(query, pinId, continueAtFail);
            });
        }
        std::optional<QList<muuid::uuid>> getAllowFlows(QSqlQuery& query, const muuid::uuid& pinId, const bool continueAtFail = false) {
            return NDPinDetails::Read::getAllowFlows(query, pinId, continueAtFail);
        }

        std::optional<QList<muuid::uuid>> getAllowTypes(const muuid::uuid& pinId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDPinDetails::Read::getAllowTypes(query, pinId, continueAtFail);
            });
        }
        std::optional<QList<muuid::uuid>> getAllowTypes(QSqlQuery& query, const muuid::uuid& pinId, const bool continueAtFail = false) {
            return NDPinDetails::Read::getAllowTypes(query, pinId, continueAtFail);
        }

        // 3. Update
        bool updatePin(const muuid::uuid& id, const NDPinDetails::Config::UpdatePinRecord& updateProperties) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinDetails::Update::updatePin(query, id, updateProperties);
            });
        }
        bool updatePin(QSqlQuery& query, const muuid::uuid& id, const NDPinDetails::Config::UpdatePinRecord& updateProperties) {
            return NDPinDetails::Update::updatePin(query, id, updateProperties);
        }

        // 4. Delete
        bool removePin(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinDetails::Delete::remove(query, id);
            });
        }
        bool removePin(QSqlQuery& query, const muuid::uuid& id) {
            return NDPinDetails::Delete::remove(query, id);
        }

        bool removeAllowFlow(const muuid::uuid& pinId) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinDetails::Delete::removeAllowFlow(query, pinId);
            });
        }
        bool removeAllowFlow(QSqlQuery& query, const muuid::uuid& pinId) {
            return NDPinDetails::Delete::removeAllowFlow(query, pinId);
        }

        bool removeAllowFlow(const muuid::uuid& pinId, const muuid::uuid& flowId) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinDetails::Delete::removeAllowFlow(query, pinId, flowId);
            });
        }
        bool removeAllowFlow(QSqlQuery& query, const muuid::uuid& pinId, const muuid::uuid& flowId) {
            return NDPinDetails::Delete::removeAllowFlow(query, pinId, flowId);
        }

        bool removeAllowType(const muuid::uuid& pinId) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinDetails::Delete::removeAllowType(query, pinId);
            });
        }
        bool removeAllowType(QSqlQuery& query, const muuid::uuid& pinId) {
            return NDPinDetails::Delete::removeAllowType(query, pinId);
        }

        bool removeAllowType(const muuid::uuid& pinId, const muuid::uuid& typeId) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinDetails::Delete::removeAllowType(query, pinId, typeId);
            });
        }
        bool removeAllowType(QSqlQuery& query, const muuid::uuid& pinId, const muuid::uuid& typeId) {
            return NDPinDetails::Delete::removeAllowType(query, pinId, typeId);
        }
    };
}