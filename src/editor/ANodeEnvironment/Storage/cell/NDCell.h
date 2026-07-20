#pragma once

#include "details/Config.h"
#include "details/Init.h"
#include "details/Create.h"
#include "details/Read.h"
#include "details/Update.h"
#include "details/Delete.h"

#include "../NDConcepts.h"
#include "../NDHelpers.h"
#include "../NDPool.h"

namespace NDCell {

    namespace Config {
        using namespace ::NDCellDetails::Config;
    }

    template<typename DBContext>
    class Component {
        DBContext* parent;

        NDPool::DatabasePool& pool() const { return parent->getPool(); }

    public:
        explicit Component(DBContext* parentCtx) : parent(parentCtx) {
            static_assert(NDConcepts::DatabaseProvider<DBContext>, "DBContext must satisfy DatabaseProvider"); 
        }


        std::optional<QStringList> existsTables(const bool value) const {
            return NDHelpers::useQuery(pool(), [value](QSqlQuery& query) -> std::optional<QStringList> {
                const QSqlDriver* driver = query.driver();
                if (!driver) return std::nullopt;

                QStringList list;
                QStringList currentTables = driver->tables(QSql::Tables);

                if (currentTables.contains("node_cells", Qt::CaseInsensitive) == value) list.append("node_cells");
                return list;
            });
        }

        // 0. Init
        bool createAllTables() {
            return NDHelpers::useQuery(pool(), [](QSqlQuery& query) {
                return NDCellDetails::Init::createAllTables(query);
            });
        }
        bool createAllTables(QSqlQuery& query) {
            return NDCellDetails::Init::createAllTables(query);
        }

        // 1. Create
        bool createCell(const NDCellDetails::Config::CreateCellRecord& newCell, bool overrideOnCollision = false) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDCellDetails::Create::create(query, newCell, overrideOnCollision);
            });
        }
        bool createCell(QSqlQuery& query, const NDCellDetails::Config::CreateCellRecord& newCell, bool overrideOnCollision = false) {
            return NDCellDetails::Create::create(query, newCell, overrideOnCollision);
        }

        // 2. Read
        std::optional<NDCellDetails::Config::FullCellRecord> getCell(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDCellDetails::Read::getCell(query, id);
            });
        }
        std::optional<NDCellDetails::Config::FullCellRecord> getCell(QSqlQuery& query, const muuid::uuid& id) {
            return NDCellDetails::Read::getCell(query, id);
        }

        std::optional<QList<NDCellDetails::Config::FullCellRecord>> getAllCells(const muuid::uuid& nodeId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDCellDetails::Read::getAllCells(query, nodeId, continueAtFail);
            });
        }
        std::optional<QList<NDCellDetails::Config::FullCellRecord>> getAllCells(QSqlQuery& query, const muuid::uuid& nodeId, const bool continueAtFail = true) {
            return NDCellDetails::Read::getAllCells(query, nodeId, continueAtFail);
        }

        // 3. Update
        bool updateLayout(const muuid::uuid& id, const NDCellDetails::Config::UpdateCellRecord& newCellInfo, const bool overrideOnCollision = false) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDCellDetails::Update::updateCell(query, id, newCellInfo, overrideOnCollision);
            });
        }
        bool updateLayout(QSqlQuery& query, const muuid::uuid& id, const NDCellDetails::Config::UpdateCellRecord& newCellInfo, const bool overrideOnCollision = false) {
            return NDCellDetails::Update::updateCell(query, id, newCellInfo, overrideOnCollision);
        }

        // 4. Delete
        bool removeCell(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDCellDetails::Delete::remove(query, id);
            });
        }
        bool removeCell(QSqlQuery& query, const muuid::uuid& id) {
            return NDCellDetails::Delete::remove(query, id);
        }
    };
}