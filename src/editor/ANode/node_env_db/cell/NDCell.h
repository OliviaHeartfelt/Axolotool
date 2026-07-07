#pragma once

#include "details/Config.h"
#include "details/Init.h"
#include "details/Create.h"
#include "details/Read.h"
#include "details/Update.h"
#include "details/Delete.h"

#include "../NDConcepts.h"
#include "../NDHelpers.h"

namespace NDCell {

    namespace Config {
        using namespace ::NDCellDetails::Config;
    }

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
            const QStringList currentTables = database().tables();

            if (currentTables.contains("node_cells", Qt::CaseInsensitive) == value) list.append("node_cells");
            return list;
        }

        // 0. Init
        bool createAllTables() {
            return NDHelpers::useQuery(database(), [](QSqlQuery& query) {
                return NDCellDetails::Init::createAllTables(query);
            });
        }

        // 1. Create
        bool createCell(const NDCellDetails::Config::CreateCellRecord& newCell, bool overrideOnCollision = false) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDCellDetails::Create::create(query, newCell, overrideOnCollision);
            });
        }
        bool createCell(QSqlQuery& query, const NDCellDetails::Config::CreateCellRecord& newCell, bool overrideOnCollision = false) {
            return NDCellDetails::Create::create(query, newCell, overrideOnCollision);
        }

        // 2. Read
        std::optional<NDCellDetails::Config::FullCellRecord> getCell(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDCellDetails::Read::getCell(query, id);
            });
        }
        std::optional<NDCellDetails::Config::FullCellRecord> getCell(QSqlQuery& query, const muuid::uuid& id) {
            return NDCellDetails::Read::getCell(query, id);
        }

        std::optional<QList<NDCellDetails::Config::FullCellRecord>> getAllCells(const muuid::uuid& nodeId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDCellDetails::Read::getAllCells(query, nodeId, continueAtFail);
            });
        }
        std::optional<QList<NDCellDetails::Config::FullCellRecord>> getAllCells(QSqlQuery& query, const muuid::uuid& nodeId, const bool continueAtFail = true) {
            return NDCellDetails::Read::getAllCells(query, nodeId, continueAtFail);
        }

        // 3. Update
        bool updateLayout(const muuid::uuid& id, const NDCellDetails::Config::UpdateCellRecord& newCellInfo, const bool overrideOnCollision = false) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDCellDetails::Update::updateCell(query, id, newCellInfo, overrideOnCollision);
            });
        }
        bool updateLayout(QSqlQuery& query, const muuid::uuid& id, const NDCellDetails::Config::UpdateCellRecord& newCellInfo, const bool overrideOnCollision = false) {
            return NDCellDetails::Update::updateCell(query, id, newCellInfo, overrideOnCollision);
        }

        // 4. Delete
        bool removeCell(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDCellDetails::Delete::remove(query, id);
            });
        }
        bool removeCell(QSqlQuery& query, const muuid::uuid& id) {
            return NDCellDetails::Delete::remove(query, id);
        }
    };
}