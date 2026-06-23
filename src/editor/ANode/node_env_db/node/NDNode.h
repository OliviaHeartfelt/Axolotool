#pragma once

#include "details/Config.h"
#include "details/Init.h"
#include "details/Create.h"
#include "details/Read.h"
#include "details/Update.h"
#include "details/Delete.h"

#include "../cell/details/Create.h"
#include "../NDConcepts.h"

namespace NDNode {

    using namespace ::NDNodeDetails::Config;

    struct ComponentFriendTag {
        template<typename DBContext>
        static typename DBContext::StorageKey createKey() { return {}; }
    };

    template<NDConcepts::DatabaseProvider DBContext>
    class Component {
        DBContext* parent;

    public:
        explicit Component(DBContext* parentCtx) : parent(parentCtx) {}

        bool existsTable() const {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            return db.tables().contains("nodes", Qt::CaseInsensitive);
        }

        // 0. Init
        bool createTable() {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            const bool wasTableCreated = NDNodeDetails::Init::createTable(query);

            if (!wasTableCreated || !tr.commit()) return false;
            return true;
        }

        // 1. Create
        std::optional<muuid::uuid> create(const QString& title, const short rowNum, const short colNum, const QList<NDCell::RecordInfo>& cells = {}, const QPointF pos = { 0.0, 0.0 }) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return std::nullopt;
            
            std::optional<muuid::uuid> nodeId = NDNodeDetails::Create::create(query, title, rowNum, colNum, pos);
            if (!nodeId) return std::nullopt;

            for (const auto& cell : cells)
                if (!::NDCellDetails::Create::create(query, nodeId.value(), cell)) return std::nullopt;

            if (!tr.commit()) return std::nullopt;
            return nodeId;
        }

        // 2. Read
        std::optional<NDNodeDetails::Config::Record> get(const muuid::uuid& id) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return std::nullopt;

            const auto record = NDNodeDetails::Read::get(query, id);

            if (!record || !tr.commit()) return std::nullopt;
            return *record;
        }
        std::optional<QList<NDNodeDetails::Config::Record>> getAll(const bool continueAtFail = true) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return std::nullopt;

            const auto record = NDNodeDetails::Read::getAll(query, continueAtFail);

            if (!record || !tr.commit()) return std::nullopt;
            return *record;
        }

        // 3. Update
        bool updateGeometry(const muuid::uuid& id, const QPointF& pos, const double w = -1.0, const double h = -1.0) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            const bool isUpdated = NDNodeDetails::Update::updateGeometry(query, id, pos, w, h);

            if (!isUpdated || !tr.commit()) return false;
            return true;
        }

        // 4. Delete
        bool remove(const muuid::uuid& id) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            const bool isDeleted = NDNodeDetails::Delete::remove(db, id);

            if (!isDeleted || !tr.commit()) return false;
            return true; return tr.commit();
        }
    };
}