#pragma once

#include "details/Config.h"
#include "details/Init.h"
#include "details/Create.h"
#include "details/Read.h"
#include "details/Update.h"
#include "details/Delete.h"

#include "../NDConcepts.h"

namespace NDCell {

    using namespace ::NDCellDetails::Config;

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
            return db.tables().contains("node_cells", Qt::CaseInsensitive);
        }

        // 0. Init
        bool createTable() {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            const bool wasTableCreated = NDCellDetails::Init::createTable();

            if (!wasTableCreated || !tr.commit()) return false;
            return true;
        }

        // 1. Create
        std::optional<muuid::uuid> createCell(const muuid::uuid& nodeId, const NDCell::RecordInfo& cell, bool overrideOnCollision = false) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return std::nullopt;

            std::optional<muuid::uuid> insertedId = NDCellDetails::Create::create(query, nodeId, cell, overrideOnCollision);

            if (!insertedId || !tr.commit()) return std::nullopt;
            return insertedId;
        }

        // 2. Read
        std::optional<NDCellDetails::Config::Record> get(const muuid::uuid& id) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return std::nullopt;

            auto redord = NDCellDetails::Read::get(query, id);

            if (!redord || !tr.commit()) return std::nullopt;
            return *redord;
        }
        std::optional<QList<NDCellDetails::Config::Record>> getAll(const muuid::uuid& nodeId, const bool continueAtFail = true) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return std::nullopt;

            auto redord = NDCellDetails::Read::getAll(query, nodeId, continueAtFail);

            if (!redord || !tr.commit()) return std::nullopt;
            return *redord;
        }

        // 3. Update
        bool updateLayout(const muuid::uuid& id, const muuid::uuid& nodeId, NDCellDetails::Config::RecordInfo& cellInfo, const bool overrideOnCollision = false) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            const bool isUpdated = NDCellDetails::Update::updateLayout(query, id, nodeId, cellInfo, overrideOnCollision);

            if (!isUpdated || !tr.commit()) return false;
            return true;
        }

        // 4. Delete
        bool removeCell(const muuid::uuid& id) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;
            
            const bool isDeleted = NDCellDetails::Delete::remove(db, id);

            if (!isDeleted || !tr.commit()) return false;
            return true;
        }
    };
}