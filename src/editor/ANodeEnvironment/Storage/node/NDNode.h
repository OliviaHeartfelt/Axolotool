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

namespace NDNode {

    namespace Config {
        using namespace ::NDNodeDetails::Config;
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

                if (currentTables.contains("node_core", Qt::CaseInsensitive) == value) list.append("node_core");
                if (currentTables.contains("node",      Qt::CaseInsensitive) == value) list.append("node");
                return list;
            });
        }

        // 0. Init
        bool createAllTables() {
            return NDHelpers::useQuery(pool(), [](QSqlQuery& query) {
                return NDNodeDetails::Init::createAllTables(query);
            });
        }
        bool createAllTables(QSqlQuery& query) {
            return NDNodeDetails::Init::createAllTables(query);
        }

        // 1. Create
        bool createNodeCore(const NDNodeDetails::Config::CreateNodeCoreRecord& newNodeCore) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeDetails::Create::createNodeCore(query, newNodeCore);
            });
        }
        bool createNodeCore(QSqlQuery& query, const NDNodeDetails::Config::CreateNodeCoreRecord& newNodeCore) {
            return NDNodeDetails::Create::createNodeCore(query, newNodeCore);
        }

        bool createNode(const NDNodeDetails::Config::CreateNodeRecord& newNode) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeDetails::Create::createNode(query, newNode);
            });
        }
        bool createNode(QSqlQuery& query, const NDNodeDetails::Config::CreateNodeRecord& newNode) {
            return NDNodeDetails::Create::createNode(query, newNode);
        }

        // 2. Read - Node Core
        std::optional<NDNodeDetails::Config::FullNodeCoreRecord> getNodeCore(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeDetails::Read::getNodeCore(query, id);
            });
        }
        std::optional<NDNodeDetails::Config::FullNodeCoreRecord> getNodeCore(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeDetails::Read::getNodeCore(query, id);
        }

        std::optional<QList<NDNodeDetails::Config::FullNodeCoreRecord>> getContributorNodeCores(const muuid::uuid& contributorId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeDetails::Read::getContributorNodeCores(query, contributorId, continueAtFail);
            });
        }
        std::optional<QList<NDNodeDetails::Config::FullNodeCoreRecord>> getContributorNodeCores(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = true) {
            return NDNodeDetails::Read::getContributorNodeCores(query, contributorId, continueAtFail);
        }

        std::optional<QList<NDNodeDetails::Config::FullNodeCoreRecord>> getAllNodeCores(const muuid::uuid& sourceId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeDetails::Read::getAllNodeCores(query, sourceId, continueAtFail);
            });
        }
        std::optional<QList<NDNodeDetails::Config::FullNodeCoreRecord>> getAllNodeCores(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = true) {
            return NDNodeDetails::Read::getAllNodeCores(query, sourceId, continueAtFail);
        }

        // 2. Read - Node
        std::optional<NDNodeDetails::Config::FullNodeRecord> getNode(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeDetails::Read::getNode(query, id);
            });
        }
        std::optional<NDNodeDetails::Config::FullNodeRecord> getNode(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeDetails::Read::getNode(query, id);
        }

        std::optional<QList<NDNodeDetails::Config::FullNodeRecord>> getContributorNodes(const muuid::uuid& contributorId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeDetails::Read::getContributorNodes(query, contributorId, continueAtFail);
            });
        }
        std::optional<QList<NDNodeDetails::Config::FullNodeRecord>> getContributorNodes(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = true) {
            return NDNodeDetails::Read::getContributorNodes(query, contributorId, continueAtFail);
        }

        std::optional<QList<NDNodeDetails::Config::FullNodeRecord>> getAllNodes(const muuid::uuid& sourceId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeDetails::Read::getAllNodes(query, sourceId, continueAtFail);
            });
        }
        std::optional<QList<NDNodeDetails::Config::FullNodeRecord>> getAllNodes(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = true) {
            return NDNodeDetails::Read::getAllNodes(query, sourceId, continueAtFail);
        }

        std::optional<QList<NDNodeDetails::Config::FullNodeRecord>> getNodesInView(const bool continueAtFail = true) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeDetails::Read::getNodesInView(query, continueAtFail);
            });
        }
        std::optional<QList<NDNodeDetails::Config::FullNodeRecord>> getNodesInView(QSqlQuery& query, const bool continueAtFail = true) {
            return NDNodeDetails::Read::getNodesInView(query, continueAtFail);
        }

        // 3. Update
        bool updateNodeCore(const muuid::uuid& id, const NDNodeDetails::Config::UpdateNodeCoreRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeDetails::Update::updateNodeCore(query, id, newProperties);
            });
        }
        bool updateNodeCore(QSqlQuery& query, const muuid::uuid& id, const NDNodeDetails::Config::UpdateNodeCoreRecord& newProperties) {
            return NDNodeDetails::Update::updateNodeCore(query, id, newProperties);
        }

        bool updateNode(const muuid::uuid& id, const NDNodeDetails::Config::UpdateNodeRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeDetails::Update::updateNode(query, id, newProperties);
            });
        }
        bool updateNode(QSqlQuery& query, const muuid::uuid& id, const NDNodeDetails::Config::UpdateNodeRecord& newProperties) {
            return NDNodeDetails::Update::updateNode(query, id, newProperties);
        }

        // 4. Delete
        bool removeNodeCore(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeDetails::Delete::removeNodeCore(query, id);
            });
        }
        bool removeNodeCore(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeDetails::Delete::removeNodeCore(query, id);
        }

        bool removeNode(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeDetails::Delete::removeNode(query, id);
            });
        }
        bool removeNode(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeDetails::Delete::removeNode(query, id);
        }
    };
}