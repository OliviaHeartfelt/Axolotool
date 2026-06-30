#pragma once

#include "details/Config.h"
#include "details/Init.h"
#include "details/Create.h"
#include "details/Read.h"
#include "details/Update.h"
#include "details/Delete.h"

#include "../NDConcepts.h"

namespace NDPin {
    struct ComponentFriendTag {
        template<typename DBContext>
        static typename DBContext::StorageKey createKey() { return {}; }
    };

    template<NDConcepts::DatabaseProvider DBContext>
    class Component {
        DBContext* parent;

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
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            const bool wereTablesCreated = NDPinDetails::Init::createAllTables(query);

            if (!wereTablesCreated || !tr.commit()) return false;
            return true;
        }

        // 1. Create
        std::optional<muuid::uuid> createPin(const NDPinDetails::Config::CreatePinRecord& newPin) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return std::nullopt;

            std::optional<muuid::uuid> pinId = NDPinDetails::Create::create(query, newPin);
            if (!pinId) return std::nullopt;

            if (!tr.commit()) return std::nullopt;
            return pinId;
        }
        bool createAllowFlows(const muuid::uuid pinId, const QList<muuid::uuid>& allowedTypes) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            if (!NDPinDetails::Create::createAllowFlows(query, pinId, allowedTypes)) return false;

            if (!tr.commit()) return false;
            return true;
        }
        bool createAllowTypes(const muuid::uuid pinId, const QList<muuid::uuid>& allowedTypes) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            if (!NDPinDetails::Create::createAllowTypes(query, pinId, allowedTypes)) return false;

            if (!tr.commit()) return false;
            return true;
        }

        // 2. Read
        inline std::optional<NDPinDetails::Config::FullPinRecord> get(const muuid::uuid& id, const bool continueAtFail = false) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return std::nullopt;

            const auto pinFullRecord = NDPinDetails::Read::get(query, id, continueAtFail);
            if (!pinFullRecord) return std::nullopt;

            if (!tr.commit()) return std::nullopt;
            return pinFullRecord;
        }
        inline std::optional<NDPinDetails::Config::PinRecord> getPin(const muuid::uuid& id) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            return NDPinDetails::Read::getPin(query, id);
        }
        inline std::optional<QList<muuid::uuid>> getAllowFlows(const muuid::uuid& pinId, const bool continueAtFail = false) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            return NDPinDetails::Read::getAllowFlows(query, pinId, continueAtFail);
        }
        inline std::optional<QList<muuid::uuid>> getAllowTypes(const muuid::uuid& pinId, const bool continueAtFail = false) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            return NDPinDetails::Read::getAllowTypes(query, pinId, continueAtFail);
        };

        // 3. Update
        inline bool updatePin(const muuid::uuid& id, const NDPinDetails::Config::UpdatePinRecord& updateProperties) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            if (!NDPinDetails::Update::updatePin(query, id, updateProperties)) return false;

            if (!tr.commit()) return false;
            return true;
        }

        // 4. Delete
        inline bool removePin(const muuid::uuid& id) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            if (!NDPinDetails::Delete::remove(query, id)) return false;

            if (!tr.commit()) return false;
            return true;
        };
        inline bool removeAllowFlow(const muuid::uuid& pinId, const std::optional<muuid::uuid>& flowId = std::nullopt) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            if (flowId) {
                if (!NDPinDetails::Delete::removeAllowFlow(query, pinId, *flowId) return false;
            }
            else {
                if (!NDPinDetails::Delete::removeAllowFlow(query, pinId) return false;
            }

            if (!tr.commit()) return false;
            return true;
        };
        inline bool removeAllowType(const muuid::uuid& pinId, const std::optional<muuid::uuid>& typeId = std::nullopt) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            if (typeId) {
                if (!NDPinDetails::Delete::removeAllowType(query, pinId, *typeId) return false;
            }
            else {
                if (!NDPinDetails::Delete::removeAllowType(query, pinId) return false;
            }

            if (!tr.commit()) return false;
            return true;
        };
    };
}