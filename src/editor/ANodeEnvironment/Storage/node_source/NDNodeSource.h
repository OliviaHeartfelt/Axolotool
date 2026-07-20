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

namespace NDNodeSource {

    namespace Config {
        using namespace ::NDNodeSourceDetails::Config;
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

                if (currentTables.contains("node_source",      Qt::CaseInsensitive) == value) list.append("node_source");
                if (currentTables.contains("node_contributor", Qt::CaseInsensitive) == value) list.append("node_contributor");
                if (currentTables.contains("node_type",        Qt::CaseInsensitive) == value) list.append("node_type");
                if (currentTables.contains("node_data",        Qt::CaseInsensitive) == value) list.append("node_data");
                return list;
            });
        }

        // 0. Init
        bool createAllTables() {
            return NDHelpers::useQuery(pool(), [](QSqlQuery& query) {
                return NDNodeSourceDetails::Init::createAllTables(query);
                });
        }
        bool createAllTables(QSqlQuery& query) {
            return NDNodeSourceDetails::Init::createAllTables(query);
        }

        // 1. Create
        inline bool createNodeSource(const NDNodeSourceDetails::Config::CreateNodeSourceRecord& newNodeSource) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Create::createNodeSource(query, newNodeSource);
                });
        }
        inline bool createNodeSource(QSqlQuery& query, const NDNodeSourceDetails::Config::CreateNodeSourceRecord& newNodeSource) {
            return NDNodeSourceDetails::Create::createNodeSource(query, newNodeSource);
        }

        inline bool createNodeContributor(const NDNodeSourceDetails::Config::CreateNodeContributorRecord& newNodeContributor) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Create::createNodeContributor(query, newNodeContributor);
                });
        }
        inline bool createNodeContributor(QSqlQuery& query, const NDNodeSourceDetails::Config::CreateNodeContributorRecord& newNodeContributor) {
            return NDNodeSourceDetails::Create::createNodeContributor(query, newNodeContributor);
        }


        template<NDConcepts::ByteConvertible Metadata>
        inline bool createNodeType(const NDNodeSourceDetails::Config::CreateNodeTypeRecord<Metadata>& newNodeType) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Create::createNodeType(query, newNodeType);
                });
        }
        template<NDConcepts::ByteConvertible Metadata>
        inline bool createNodeType(QSqlQuery& query, const NDNodeSourceDetails::Config::CreateNodeTypeRecord<Metadata>& newNodeType) {
            return NDNodeSourceDetails::Create::createNodeType(query, newNodeType);
        }


        template<NDConcepts::ByteConvertible Data>
        inline bool createNodeData(const NDNodeSourceDetails::Config::CreateNodeDataRecord<Data>& newNodeData) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Create::createNodeData(query, newNodeData);
                });
        }
        template<NDConcepts::ByteConvertible Data>
        inline bool createNodeData(QSqlQuery& query, const NDNodeSourceDetails::Config::CreateNodeDataRecord<Data>& newNodeData) {
            return NDNodeSourceDetails::Create::createNodeData(query, newNodeData);
        }


        // 2. Read - Node Source
        inline std::optional<NDNodeSourceDetails::Config::FullNodeSourceRecord> getNodeSource(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Read::getNodeSource(query, id);
                });
        }
        inline std::optional<NDNodeSourceDetails::Config::FullNodeSourceRecord> getNodeSource(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeSourceDetails::Read::getNodeSource(query, id);
        }
        inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeSourceRecord>> getAllNodeSources(const bool continueAtFail = true) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Read::getAllNodeSources(query, continueAtFail);
                });
        }
        inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeSourceRecord>> getAllNodeSources(QSqlQuery& query, const bool continueAtFail = true) {
            return NDNodeSourceDetails::Read::getAllNodeSources(query, continueAtFail);
        }

        // 2. Read - Node Contributor
        inline std::optional<NDNodeSourceDetails::Config::FullNodeContributorRecord> getNodeContributor(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Read::getNodeContributor(query, id);
                });
        }
        inline std::optional<NDNodeSourceDetails::Config::FullNodeContributorRecord> getNodeContributor(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeSourceDetails::Read::getNodeContributor(query, id);
        }
        inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeContributorRecord>> getAllNodeContributors(const muuid::uuid& sourceId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Read::getAllNodeContributors(query, sourceId, continueAtFail);
                });
        }
        inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeContributorRecord>> getAllNodeContributors(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = true) {
            return NDNodeSourceDetails::Read::getAllNodeContributors(query, sourceId, continueAtFail);
        }

        // 2. Read - Node Type
        template<NDConcepts::ByteConvertible Metadata>
        inline std::optional<NDNodeSourceDetails::Config::FullNodeTypeRecord<Metadata>> getNodeType(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Read::getNodeType(query, id);
                });
        }
        template<NDConcepts::ByteConvertible Metadata>
        inline std::optional<NDNodeSourceDetails::Config::FullNodeTypeRecord<Metadata>> getNodeType(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeSourceDetails::Read::getNodeType(query, id);
        }
        template<NDConcepts::ByteConvertible Metadata>
        inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeTypeRecord<Metadata>>> getContributorNodeTypes(const muuid::uuid& contributorId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Read::getContributorNodeTypes<Metadata>(query, contributorId, continueAtFail);
                });
        }
        template<NDConcepts::ByteConvertible Metadata>
        inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeTypeRecord<Metadata>>> getContributorNodeTypes(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = true) {
            return NDNodeSourceDetails::Read::getContributorNodeTypes<Metadata>(query, contributorId, continueAtFail);
        }
        template<NDConcepts::ByteConvertible Metadata>
        inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeTypeRecord<Metadata>>> getAllNodeTypes(const muuid::uuid& sourceId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Read::getAllNodeTypes<Metadata>(query, sourceId, continueAtFail);
                });
        }
        template<NDConcepts::ByteConvertible Metadata>
        inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeTypeRecord<Metadata>>> getAllNodeTypes(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = true) {
            return NDNodeSourceDetails::Read::getAllNodeTypes<Metadata>(query, sourceId, continueAtFail);
        }

        // 2. Read - Node Data
        template<NDConcepts::ByteConvertible Data>
        inline std::optional<NDNodeSourceDetails::Config::FullNodeDataRecord<Data>> getNodeData(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Read::getNodeData(query, id);
                });
        }
        template<NDConcepts::ByteConvertible Data>
        inline std::optional<NDNodeSourceDetails::Config::FullNodeDataRecord<Data>> getNodeData(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeSourceDetails::Read::getNodeData(query, id);
        }
        template<NDConcepts::ByteConvertible Data>
        inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeDataRecord<Data>>> getContributorNodeData(const muuid::uuid& contributorId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Read::getContributorNodeData<Data>(query, contributorId, continueAtFail);
                });
        }
        template<NDConcepts::ByteConvertible Data>
        inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeDataRecord<Data>>> getContributorNodeData(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = true) {
            return NDNodeSourceDetails::Read::getContributorNodeData<Data>(query, contributorId, continueAtFail);
        }
        template<NDConcepts::ByteConvertible Data>
        inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeDataRecord<Data>>> getAllNodeData(const muuid::uuid& sourceId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Read::getAllNodeData<Data>(query, sourceId, continueAtFail);
                });
        }
        template<NDConcepts::ByteConvertible Data>
        inline std::optional<QList<NDNodeSourceDetails::Config::FullNodeDataRecord<Data>>> getAllNodeData(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = true) {
            return NDNodeSourceDetails::Read::getAllNodeData<Data>(query, sourceId, continueAtFail);
        }

        // 3. Update - Node Source
        inline bool updateNodeSource(const muuid::uuid& id, const NDNodeSourceDetails::Config::UpdateNodeSourceRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Update::updateNodeSource(query, id, newProperties);
                });
        }
        inline bool updateNodeSource(QSqlQuery& query, const muuid::uuid& id, const NDNodeSourceDetails::Config::UpdateNodeSourceRecord& newProperties) {
            return NDNodeSourceDetails::Update::updateNodeSource(query, id, newProperties);
        }

        // 3. Update - Node Contributor
        inline bool updateNodeContributor(const muuid::uuid& id, const NDNodeSourceDetails::Config::UpdateNodeContributorRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Update::updateNodeContributor(query, id, newProperties);
                });
        }
        inline bool updateNodeContributor(QSqlQuery& query, const muuid::uuid& id, const NDNodeSourceDetails::Config::UpdateNodeContributorRecord& newProperties) {
            return NDNodeSourceDetails::Update::updateNodeContributor(query, id, newProperties);
        }

        // 3. Update - Node Type
        template<NDConcepts::ByteConvertible Metadata>
        inline bool updateNodeType(const muuid::uuid& id, const NDNodeSourceDetails::Config::UpdateNodeTypeRecord<Metadata>& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Update::updateNodeType(query, id, newProperties);
                });
        }
        template<NDConcepts::ByteConvertible Metadata>
        inline bool updateNodeType(QSqlQuery& query, const muuid::uuid& id, const NDNodeSourceDetails::Config::UpdateNodeTypeRecord<Metadata>& newProperties) {
            return NDNodeSourceDetails::Update::updateNodeType(query, id, newProperties);
        }

        // 3. Update - Node Data
        template<NDConcepts::ByteConvertible Data>
        inline bool updateNodeData(const muuid::uuid& id, const NDNodeSourceDetails::Config::UpdateNodeDataRecord<Data>& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Update::updateNodeData(query, id, newProperties);
                });
        }
        template<NDConcepts::ByteConvertible Data>
        inline bool updateNodeData(QSqlQuery& query, const muuid::uuid& id, const NDNodeSourceDetails::Config::UpdateNodeDataRecord<Data>& newProperties) {
            return NDNodeSourceDetails::Update::updateNodeData(query, id, newProperties);
        }

        // 4. Delete - Node Source
        inline bool removeNodeSource(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Delete::removeNodeSource(query, id);
                });
        }
        inline bool removeNodeSource(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeSourceDetails::Delete::removeNodeSource(query, id);
        }

        // 4. Delete - Node Contributor
        inline bool removeNodeContributor(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Delete::removeNodeContributor(query, id);
                });
        }
        inline bool removeNodeContributor(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeSourceDetails::Delete::removeNodeContributor(query, id);
        }

        // 4. Delete - Node Type
        inline bool removeNodeType(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Delete::removeNodeType(query, id);
                });
        }
        inline bool removeNodeType(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeSourceDetails::Delete::removeNodeType(query, id);
        }

        // 4. Delete - Node Data
        inline bool removeNodeData(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDNodeSourceDetails::Delete::removeNodeData(query, id);
                });
        }
        inline bool removeNodeData(QSqlQuery& query, const muuid::uuid& id) {
            return NDNodeSourceDetails::Delete::removeNodeData(query, id);
        }
    };
}