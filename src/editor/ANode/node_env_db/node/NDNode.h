#pragma once

#include "details/Config.h"
#include "details/Init.h"
#include "details/Create.h"
#include "details/Read.h"
#include "details/Update.h"
#include "details/Delete.h"

#include "../cell/details/Create.h"
#include "../NDConcepts.h"
#include "../NDHelpers.h"

namespace NDNode {

    using namespace ::NDNodeDetails::Config;

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

        bool existsTable() const {
            return database().tables().contains("nodes", Qt::CaseInsensitive);
        }

        // 0. Init
        bool createAllTables() {
            return NDHelpers::useQuery(database(), [](QSqlQuery& query) {
                return NDNodeDetails::Init::createTable(query);
            });
        }

        // 1. Create
        std::optional<muuid::uuid> create(const NDCellDetails::Config::CreateCellRecord newNode, QList<NDCell::Config::CreateCellRecord>& cells = {}, const bool overrideOnCollision = false, const bool continueAtFail = false) {
            return NDHelpers::useTransaction<muuid::uuid>(database(), [&](QSqlQuery& query) -> std::optional<muuid::uuid> {
                std::optional<muuid::uuid> nodeId = NDNodeDetails::Create::create(query, newNode);
                if (!nodeId) return std::nullopt;

                for (auto& cell : cells) {
                    cell.nodeId = *nodeId;

                    if (!NDCellDetails::Create::create(query, *nodeId, cell, overrideOnCollision)) {
                        if (continueAtFail)
                            continue;
                        else
                            return std::nullopt;
                    }
                }
                return nodeId;
            });
        }
        std::optional<muuid::uuid> create(QSqlQuery& query, const NDCellDetails::Config::CreateCellRecord newNode, QList<NDCell::Config::CreateCellRecord>& cells = {}, const bool overrideOnCollision = false, const bool continueAtFail = false) {
            std::optional<muuid::uuid> nodeId = NDNodeDetails::Create::create(query, newNode);
            if (!nodeId) return std::nullopt;

            for (auto& cell : cells) {
                cell.nodeId = *nodeId;

                if (!NDCellDetails::Create::create(query, *nodeId, cell, overrideOnCollision)) {
                    if (continueAtFail)
                        continue;
                    else
                        return std::nullopt;
                }
            }
            return nodeId;
        }

        // 2. Read
        std::optional<NDNodeDetails::Config::FullNodeRecord> get(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDNodeDetails::Read::get(query, id);
            });
        }
        std::optional<NDNodeDetails::Config::FullNodeRecord> get(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeDetails::Read::get(query, id);
        }

        std::optional<QList<NDNodeDetails::Config::FullNodeRecord>> getAll(const bool continueAtFail = true) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDNodeDetails::Read::getAll(query, continueAtFail);
            });
        }
        std::optional<QList<NDNodeDetails::Config::FullNodeRecord>> getAll(QSqlQuery& query, const bool continueAtFail = true) {
            return NDNodeDetails::Read::getAll(query, continueAtFail);
        }

        // 3. Update
        bool updateNode(const muuid::uuid& id, const NDNodeDetails::Config::UpdateNodeRecord& newProperties) {
            return NDHelpers::useTransaction<bool>(database(), [&](QSqlQuery& query) {
                return NDNodeDetails::Update::updateNode(query, id, newProperties);
            });
        }
        bool updateNode(QSqlQuery& query, const muuid::uuid& id, const NDNodeDetails::Config::UpdateNodeRecord newProperties) {
            return NDNodeDetails::Update::updateNode(query, id, newProperties);
        }

        // 4. Delete
        bool remove(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery&) {
                return NDNodeDetails::Delete::remove(database(), id);
            });
        }
        bool remove(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeDetails::Delete::remove(query, id);
        }
    };
}