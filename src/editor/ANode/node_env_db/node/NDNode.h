#pragma once

#include "details/Config.h"
#include "details/Init.h"
#include "details/Create.h"
#include "details/Read.h"
#include "details/Update.h"
#include "details/Delete.h"

#include "../NDConcepts.h"
#include "../NDHelpers.h"

namespace NDNode {

    namespace Config {
        using namespace ::NDNodeDetails::Config;
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

            if (currentTables.contains("node_core", Qt::CaseInsensitive) == value) list.append("node_core");
            if (currentTables.contains("node",      Qt::CaseInsensitive) == value) list.append("node");
            return list;
        }

        // 0. Init
        bool createAllTables() {
            return NDHelpers::useQuery(database(), [](QSqlQuery& query) {
                return NDNodeDetails::Init::createAllTables(query);
            });
        }

        // 1. Create
        inline bool createNodeCore(const NDNodeDetails::Config::CreateNodeCoreRecord& newNodeCore) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDNodeDetails::Create::createNodeCore(query, newNodeCore);
            });
        }
        inline bool createNodeCore(QSqlQuery& query, const NDNodeDetails::Config::CreateNodeCoreRecord& newNodeCore) {
            return NDNodeDetails::Create::createNodeCore(query, newNodeCore);
        }

        template<NDConcepts::ByteConvertible State>
        inline bool createNode(const NDNodeDetails::Config::CreateNodeRecord<State>& newNode) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDNodeDetails::Create::createNode(query, newNode);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline bool createNode(QSqlQuery& query, const NDNodeDetails::Config::CreateNodeRecord<State>& newNode) {
            return NDNodeDetails::Create::createNode(query, newNode);
        }

        // 2. Read - Node Core
        inline std::optional<NDNodeDetails::Config::FullNodeCoreRecord> getNodeCore(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDNodeDetails::Read::getNodeCore(query, id);
            });
        }
        inline std::optional<NDNodeDetails::Config::FullNodeCoreRecord> getNodeCore(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeDetails::Read::getNodeCore(query, id);
        }

        inline std::optional<QList<NDNodeDetails::Config::FullNodeCoreRecord>> getContributorNodeCores(const muuid::uuid& contributorId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDNodeDetails::Read::getContributorNodeCores(query, contributorId, continueAtFail);
            });
        }
        inline std::optional<QList<NDNodeDetails::Config::FullNodeCoreRecord>> getContributorNodeCores(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = true) {
            return NDNodeDetails::Read::getContributorNodeCores(query, contributorId, continueAtFail);
        }

        inline std::optional<QList<NDNodeDetails::Config::FullNodeCoreRecord>> getAllNodeCores(const muuid::uuid& sourceId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDNodeDetails::Read::getAllNodeCores(query, sourceId, continueAtFail);
            });
        }
        inline std::optional<QList<NDNodeDetails::Config::FullNodeCoreRecord>> getAllNodeCores(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = true) {
            return NDNodeDetails::Read::getAllNodeCores(query, sourceId, continueAtFail);
        }

        // 2. Read - Node
        template<NDConcepts::ByteConvertible State>
        inline std::optional<NDNodeDetails::Config::FullNodeRecord<State>> getNode(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDNodeDetails::Read::getNode<State>(query, id);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<NDNodeDetails::Config::FullNodeRecord<State>> getNode(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeDetails::Read::getNode<State>(query, id);
        }

        template<NDConcepts::ByteConvertible State>
        inline std::optional<QList<NDNodeDetails::Config::FullNodeRecord<State>>> getContributorNodes(const muuid::uuid& contributorId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDNodeDetails::Read::getContributorNodes<State>(query, contributorId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<QList<NDNodeDetails::Config::FullNodeRecord<State>>> getContributorNodes(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = true) {
            return NDNodeDetails::Read::getContributorNodes<State>(query, contributorId, continueAtFail);
        }

        template<NDConcepts::ByteConvertible State>
        inline std::optional<QList<NDNodeDetails::Config::FullNodeRecord<State>>> getAllNodes(const muuid::uuid& sourceId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDNodeDetails::Read::getAllNodes<State>(query, sourceId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<QList<NDNodeDetails::Config::FullNodeRecord<State>>> getAllNodes(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = true) {
            return NDNodeDetails::Read::getAllNodes<State>(query, sourceId, continueAtFail);
        }

        // 3. Update
        inline bool updateNodeCore(const muuid::uuid& id, const NDNodeDetails::Config::UpdateNodeCoreRecord& newProperties) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDNodeDetails::Update::updateNodeCore(query, id, newProperties);
            });
        }
        inline bool updateNodeCore(QSqlQuery& query, const muuid::uuid& id, const NDNodeDetails::Config::UpdateNodeCoreRecord& newProperties) {
            return NDNodeDetails::Update::updateNodeCore(query, id, newProperties);
        }

        template<NDConcepts::ByteConvertible State>
        inline bool updateNode(const muuid::uuid& id, const NDNodeDetails::Config::UpdateNodeRecord<State>& newProperties) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDNodeDetails::Update::updateNode(query, id, newProperties);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline bool updateNode(QSqlQuery& query, const muuid::uuid& id, const NDNodeDetails::Config::UpdateNodeRecord<State>& newProperties) {
            return NDNodeDetails::Update::updateNode(query, id, newProperties);
        }

        // 4. Delete
        inline bool removeNodeCore(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDNodeDetails::Delete::removeNodeCore(query, id);
            });
        }
        inline bool removeNodeCore(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeDetails::Delete::removeNodeCore(query, id);
        }

        inline bool removeNode(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDNodeDetails::Delete::removeNode(query, id);
            });
        }
        inline bool removeNode(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeDetails::Delete::removeNode(query, id);
        }
    };
}