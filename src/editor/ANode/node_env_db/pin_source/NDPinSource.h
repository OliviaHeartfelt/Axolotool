#pragma once

#include "details/Config.h"
#include "details/Init.h"
#include "details/Create.h"
#include "details/Read.h"
#include "details/Update.h"
#include "details/Delete.h"

#include "../NDConcepts.h"

namespace NDPinSource {
    struct ComponentFriendTag {
        template<typename DBContext>
        static typename DBContext::StorageKey createKey() { return {}; }
    };

    template<NDConcepts::DatabaseProvider DBContext>
    class Component {
        DBContext* parent;

        template<typename T, typename Callable>
        std::optional<muuid::uuid> useTransaction(const T& data, Callable&& fn) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return std::nullopt;

            std::optional<muuid::uuid> id = std::forward<Callable>(fn)(query, data);
            if (!id) return std::nullopt;

            if (!tr.commit()) return std::nullopt;
            return id;
        }
        template<typename Callable>
        auto useQuery(Callable&& fn) const -> decltype(std::forward<Callable>(fn)(std::declval<QSqlQuery&>())) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);
            return std::forward<Callable>(fn)(query);
        }

    public:
        explicit Component(DBContext* parentCtx) : parent(parentCtx) {}

        QStringList existsTables(const bool value) const {
            QStringList list;
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            const QStringList currentTables = db.tables();
            if (currentTables.contains("pin_source",      Qt::CaseInsensitive) == value) list.append("pin_source");
            if (currentTables.contains("pin_contributor", Qt::CaseInsensitive) == value) list.append("pin_contributor");
            if (currentTables.contains("flow",            Qt::CaseInsensitive) == value) list.append("flow");
            if (currentTables.contains("type",            Qt::CaseInsensitive) == value) list.append("type");
            if (currentTables.contains("style",           Qt::CaseInsensitive) == value) list.append("style");
            return list;
        }

        // 0. Init
        bool createAllTables() {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            const bool wereTablesCreated = NDPinSourceDetails::Init::createAllTables(query);

            if (!wereTablesCreated || !tr.commit()) return false;
            return true;
        }

        // 1. Create - Source
        std::optional<muuid::uuid> createSource(const NDPinSourceDetails::Config::SourceInfo& sourceInfo) {
            return useTransaction(sourceInfo, [](QSqlQuery& query, const NDPinSourceDetails::Config::SourceInfo& info) {
                return NDPinSourceDetails::Create::createPinSource(query, info);
                });
        }
        std::optional<muuid::uuid> createSource(QSqlQuery& query, const NDPinSourceDetails::Config::SourceInfo& sourceInfo) {
            return NDPinSourceDetails::Create::createPinSource(query, sourceInfo);
        }

        // 1. Create - Contributor
        std::optional<muuid::uuid> createContributor(const NDPinSourceDetails::Config::ContributorInfo& newContributor) {
            return useTransaction(newContributor, [](QSqlQuery& query, const NDPinSourceDetails::Config::ContributorInfo& info) {
                return NDPinSourceDetails::Create::createPinContributor(query, info);
                });
        }
        std::optional<muuid::uuid> createContributor(QSqlQuery& query, const NDPinSourceDetails::Config::ContributorInfo& newContributor) {
            return NDPinSourceDetails::Create::createPinContributor(query, newContributor);
        }

        // 1. Create - Flow
        std::optional<muuid::uuid> createFlow(muuid::uuid contributorId, const NDPinSourceDetails::Config::FlowInfo& flowInfo) {
            return useTransaction(flowInfo, [contributorId](QSqlQuery& query, const NDPinSourceDetails::Config::FlowInfo& info) {
                return NDPinSourceDetails::Create::createPinFlowSource(query, contributorId, info);
                });
        }
        std::optional<muuid::uuid> createFlow(QSqlQuery& query, muuid::uuid contributorId, const NDPinSourceDetails::Config::FlowInfo& flowInfo) {
            return NDPinSourceDetails::Create::createPinFlowSource(query, contributorId, flowInfo);
        }

        // 1. Create - Type
        std::optional<muuid::uuid> createType(muuid::uuid contributorId, const NDPinSourceDetails::Config::TypeInfo& typeInfo) {
            return useTransaction(typeInfo, [contributorId](QSqlQuery& query, const NDPinSourceDetails::Config::TypeInfo& info) {
                return NDPinSourceDetails::Create::createPinTypeSource(query, contributorId, info);
                });
        }
        std::optional<muuid::uuid> createType(QSqlQuery& query, muuid::uuid contributorId, const NDPinSourceDetails::Config::TypeInfo& typeInfo) {
            return NDPinSourceDetails::Create::createPinTypeSource(query, contributorId, typeInfo);
        }

        // 1. Create - Style
        std::optional<muuid::uuid> createStyle(muuid::uuid contributorId, const NDPinSourceDetails::Config::StyleInfo& styleInfo) {
            return useTransaction(styleInfo, [contributorId](QSqlQuery& query, const NDPinSourceDetails::Config::StyleInfo& info) {
                return NDPinSourceDetails::Create::createPinStyleSource(query, contributorId, info);
                });
        }
        std::optional<muuid::uuid> createStyle(QSqlQuery& query, muuid::uuid contributorId, const NDPinSourceDetails::Config::StyleInfo& styleInfo) {
            return NDPinSourceDetails::Create::createPinStyleSource(query, contributorId, styleInfo);
        }

        // 2.0. Read - Source
        std::optional<NDPinSourceDetails::Config::Source> getSource(const muuid::uuid& id) {
            return useQuery([id](QSqlQuery& query) {
                return NDPinSourceDetails::Create::getSource(query, id);
                });
        }
        std::optional<QList<NDPinSourceDetails::Config::Source>> getAllSources(const bool continueAtFail = false) {
            return useQuery([continueAtFail](QSqlQuery& query) {
                return NDPinSourceDetails::Create::getAllSources(query, continueAtFail);
                });
        }

        // 2.1. Read - Contributor
        std::optional<NDPinSourceDetails::Config::Contributor> getContributor(const muuid::uuid& id) {
            return useQuery([id](QSqlQuery& query) {
                return NDPinSourceDetails::Create::getContributor(query, id);
                });
        }
        std::optional<QList<NDPinSourceDetails::Config::Contributor>> getAllContributors(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return useQuery([sourceId, continueAtFail](QSqlQuery& query) {
                return NDPinSourceDetails::Create::getAllContributors(query, sourceId, continueAtFail);
                });
        }

        // 2.2. Read - Flow
        std::optional<NDPinSourceDetails::Config::Flow> getFlow(const muuid::uuid& id) {
            return useQuery([id](QSqlQuery& query) {
                return NDPinSourceDetails::Create::getFlow(query, id);
                });
        }
        std::optional<QList<NDPinSourceDetails::Config::Flow>> getContributorFlows(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return useQuery([contributorId, continueAtFail](QSqlQuery& query) {
                return NDPinSourceDetails::Create::getContributorFlows(query, contributorId, continueAtFail);
                });
        }
        std::optional<QList<NDPinSourceDetails::Config::Flow>> getAllFlows(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return useQuery([sourceId, continueAtFail](QSqlQuery& query) {
                return NDPinSourceDetails::Create::getAllFlows(query, sourceId, continueAtFail);
                });
        }

        // 2.3. Read - Type
        std::optional<NDPinSourceDetails::Config::Type> getType(const muuid::uuid& id) {
            return useQuery([id](QSqlQuery& query) {
                return NDPinSourceDetails::Create::getType(query, id);
                });
        }
        std::optional<QList<NDPinSourceDetails::Config::Type>> getContributorTypes(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return useQuery([contributorId, continueAtFail](QSqlQuery& query) {
                return NDPinSourceDetails::Create::getContributorTypes(query, contributorId, continueAtFail);
                });
        }
        std::optional<QList<NDPinSourceDetails::Config::Type>> getAllTypes(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return useQuery([sourceId, continueAtFail](QSqlQuery& query) {
                return NDPinSourceDetails::Create::getAllTypes(query, sourceId, continueAtFail);
                });
        }

        // 2.4. Read - Style
        std::optional<NDPinSourceDetails::Config::Style> getStyle(const muuid::uuid& id) {
            return useQuery([id](QSqlQuery& query) {
                return NDPinSourceDetails::Create::getStyle(query, id);
                });
        }
        std::optional<QList<NDPinSourceDetails::Config::Style>> getContributorStyles(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return useQuery([contributorId, continueAtFail](QSqlQuery& query) {
                return NDPinSourceDetails::Create::getContributorStyles(query, contributorId, continueAtFail);
                });
        }
        std::optional<QList<NDPinSourceDetails::Config::Style>> getAllStyles(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return useQuery([sourceId, continueAtFail](QSqlQuery& query) {
                return NDPinSourceDetails::Create::getAllStyles(query, sourceId, continueAtFail);
                });
        }

        // 3. Update


        // 4. Delete

    };
}