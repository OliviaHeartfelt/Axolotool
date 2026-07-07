#pragma once

#include "details/Config.h"
#include "details/Init.h"
#include "details/Create.h"
#include "details/Read.h"
#include "details/Update.h"
#include "details/Delete.h"

#include "../NDConcepts.h"
#include "../NDHelpers.h"

namespace NDPinSource {
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
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            const QStringList currentTables = db.tables();

            if (currentTables.contains("pin_source",      Qt::CaseInsensitive) == value) list.append("pin_source");
            if (currentTables.contains("pin_contributor", Qt::CaseInsensitive) == value) list.append("pin_contributor");
            if (currentTables.contains("pin_flow",        Qt::CaseInsensitive) == value) list.append("pin_flow");
            if (currentTables.contains("pin_type",        Qt::CaseInsensitive) == value) list.append("pin_type");
            if (currentTables.contains("pin_style",       Qt::CaseInsensitive) == value) list.append("pin_style");
            return list;
        }

        // 0. Init
        bool createAllTables() {
            return NDHelpers::useTransaction(database(), [](QSqlQuery& query) {
                return NDPinSourceDetails::Init::createAllTables(query);
            });
        }

        // 1. Create - Source
        bool createSource(const NDPinSourceDetails::Config::CreatePinSourceRecord& sourceInfo) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Create::createPinSource(query, sourceInfo);
            });
        }
        bool createSource(QSqlQuery& query, const NDPinSourceDetails::Config::CreatePinSourceRecord& sourceInfo) {
            return NDPinSourceDetails::Create::createPinSource(query, sourceInfo);
        }

        // 1. Create - Contributor
        bool createContributor(const NDPinSourceDetails::Config::CreatePinContributorRecord& newContributor) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Create::createPinContributor(query, newContributor);
            });
        }
        bool createContributor(QSqlQuery& query, const NDPinSourceDetails::Config::CreatePinContributorRecord& newContributor) {
            return NDPinSourceDetails::Create::createPinContributor(query, newContributor);
        }

        // 1. Create - Flow
        bool createFlow(const muuid::uuid& contributorId, const NDPinSourceDetails::Config::CreatePinFlowRecord& flowInfo) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Create::createPinFlowSource(query, contributorId, flowInfo);
            });
        }
        bool createFlow(QSqlQuery& query, const muuid::uuid& contributorId, const NDPinSourceDetails::Config::CreatePinFlowRecord& flowInfo) {
            return NDPinSourceDetails::Create::createPinFlowSource(query, contributorId, flowInfo);
        }

        // 1. Create - Type
        bool createType(const muuid::uuid& contributorId, const NDPinSourceDetails::Config::CreatePinTypeRecord& typeInfo) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Create::createPinTypeSource(query, contributorId, typeInfo);
            });
        }
        bool createType(QSqlQuery& query, const muuid::uuid& contributorId, const NDPinSourceDetails::Config::CreatePinTypeRecord& typeInfo) {
            return NDPinSourceDetails::Create::createPinTypeSource(query, contributorId, typeInfo);
        }

        // 1. Create - Style
        bool createStyle(const muuid::uuid& contributorId, const NDPinSourceDetails::Config::CreatePinStyleRecord& styleInfo) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Create::createPinStyleSource(query, contributorId, styleInfo);
            });
        }
        bool createStyle(QSqlQuery& query, const muuid::uuid& contributorId, const NDPinSourceDetails::Config::CreatePinStyleRecord& styleInfo) {
            return NDPinSourceDetails::Create::createPinStyleSource(query, contributorId, styleInfo);
        }

        // 2. Read - Source
        std::optional<NDPinSourceDetails::Config::FullPinSourceRecord> getSource(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Read::getSource(query, id);
            });
        }
        std::optional<NDPinSourceDetails::Config::FullPinSourceRecord> getSource(QSqlQuery& query, const muuid::uuid& id) {
            return NDPinSourceDetails::Read::getSource(query, id);
        }
        std::optional<QList<NDPinSourceDetails::Config::FullPinSourceRecord>> getAllSources(const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Read::getAllSources(query, continueAtFail);
            });
        }
        std::optional<QList<NDPinSourceDetails::Config::FullPinSourceRecord>> getAllSources(QSqlQuery& query, const bool continueAtFail = false) {
            return NDPinSourceDetails::Read::getAllSources(query, continueAtFail);
        }

        // 2. Read - Contributor
        std::optional<NDPinSourceDetails::Config::FullPinContributorRecord> getContributor(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Read::getContributor(query, id);
            });
        }
        std::optional<NDPinSourceDetails::Config::FullPinContributorRecord> getContributor(QSqlQuery& query, const muuid::uuid& id) {
            return NDPinSourceDetails::Read::getContributor(query, id);
        }
        std::optional<QList<NDPinSourceDetails::Config::FullPinContributorRecord>> getAllContributors(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Read::getAllContributors(query, sourceId, continueAtFail);
            });
        }
        std::optional<QList<NDPinSourceDetails::Config::FullPinContributorRecord>> getAllContributors(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDPinSourceDetails::Read::getAllContributors(query, sourceId, continueAtFail);
        }

        // 2. Read - Flow
        std::optional<NDPinSourceDetails::Config::FullPinFlowRecord> getFlow(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Read::getFlow(query, id);
            });
        }
        std::optional<NDPinSourceDetails::Config::FullPinFlowRecord> getFlow(QSqlQuery& query, const muuid::uuid& id) {
            return NDPinSourceDetails::Read::getFlow(query, id);
        }
        std::optional<QList<NDPinSourceDetails::Config::FullPinFlowRecord>> getContributorFlows(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Read::getContributorFlows(query, contributorId, continueAtFail);
            });
        }
        std::optional<QList<NDPinSourceDetails::Config::FullPinFlowRecord>> getContributorFlows(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDPinSourceDetails::Read::getContributorFlows(query, contributorId, continueAtFail);
        }
        std::optional<QList<NDPinSourceDetails::Config::FullPinFlowRecord>> getAllFlows(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Read::getAllFlows(query, sourceId, continueAtFail);
            });
        }
        std::optional<QList<NDPinSourceDetails::Config::FullPinFlowRecord>> getAllFlows(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDPinSourceDetails::Read::getAllFlows(query, sourceId, continueAtFail);
        }

        // 2. Read - Type
        std::optional<NDPinSourceDetails::Config::FullPinTypeRecord> getType(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Read::getType(query, id);
            });
        }
        std::optional<NDPinSourceDetails::Config::FullPinTypeRecord> getType(QSqlQuery& query, const muuid::uuid& id) {
            return NDPinSourceDetails::Read::getType(query, id);
        }
        std::optional<QList<NDPinSourceDetails::Config::FullPinTypeRecord>> getContributorTypes(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Read::getContributorTypes(query, contributorId, continueAtFail);
            });
        }
        std::optional<QList<NDPinSourceDetails::Config::FullPinTypeRecord>> getContributorTypes(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDPinSourceDetails::Read::getContributorTypes(query, contributorId, continueAtFail);
        }
        std::optional<QList<NDPinSourceDetails::Config::FullPinTypeRecord>> getAllTypes(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Read::getAllTypes(query, sourceId, continueAtFail);
            });
        }
        std::optional<QList<NDPinSourceDetails::Config::FullPinTypeRecord>> getAllTypes(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDPinSourceDetails::Read::getAllTypes(query, sourceId, continueAtFail);
        }

        // 2. Read - Style
        std::optional<NDPinSourceDetails::Config::FullPinStyleRecord> getStyle(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Read::getStyle(query, id);
            });
        }
        std::optional<NDPinSourceDetails::Config::FullPinStyleRecord> getStyle(QSqlQuery& query, const muuid::uuid& id) {
            return NDPinSourceDetails::Read::getStyle(query, id);
        }
        std::optional<QList<NDPinSourceDetails::Config::FullPinStyleRecord>> getContributorStyles(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Read::getContributorStyles(query, contributorId, continueAtFail);
            });
        }
        std::optional<QList<NDPinSourceDetails::Config::FullPinStyleRecord>> getContributorStyles(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDPinSourceDetails::Read::getContributorStyles(query, contributorId, continueAtFail);
        }
        std::optional<QList<NDPinSourceDetails::Config::FullPinStyleRecord>> getAllStyles(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Read::getAllStyles(query, sourceId, continueAtFail);
            });
        }
        std::optional<QList<NDPinSourceDetails::Config::FullPinStyleRecord>> getAllStyles(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDPinSourceDetails::Read::getAllStyles(query, sourceId, continueAtFail);
        }

        // 3. Update - Source
        bool updateSource(const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinSourceRecord& newSource) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Update::updateSource(query, id, newSource);
            });
        }
        bool updateSource(QSqlQuery& query, const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinSourceRecord& newSource) {
            return NDPinSourceDetails::Update::updateSource(query, id, newSource);
        }

        // 3. Update - Contributor
        bool updateContributor(const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinContributorRecord& newContributor) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Update::updateContributor(query, id, newContributor);
            });
        }
        bool updateContributor(QSqlQuery& query, const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinContributorRecord& newContributor) {
            return NDPinSourceDetails::Update::updateContributor(query, id, newContributor);
        }

        // 3. Update - Flow
        bool updateFlow(const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinFlowRecord& newFlow) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Update::updateFlow(query, id, newFlow);
            });
        }
        bool updateFlow(QSqlQuery& query, const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinFlowRecord& newFlow) {
            return NDPinSourceDetails::Update::updateFlow(query, id, newFlow);
        }

        // 3. Update - Type
        bool updateType(const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinTypeRecord& newType) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Update::updateType(query, id, newType);
            });
        }
        bool updateType(QSqlQuery& query, const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinTypeRecord& newType) {
            return NDPinSourceDetails::Update::updateType(query, id, newType);
        }

        // 3. Update - Style
        bool updateStyle(const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinStyleRecord& newStyle) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Update::updateStyle(query, id, newStyle);
            });
        }
        bool updateStyle(QSqlQuery& query, const muuid::uuid& id, const NDPinSourceDetails::Config::UpdatePinStyleRecord& newStyle) {
            return NDPinSourceDetails::Update::updateStyle(query, id, newStyle);
        }

        // 4. Delete - Source
        bool removeSource(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Delete::removeSource(query, id);
            });
        }
        bool removeSource(QSqlQuery& query, const muuid::uuid& id) {
            return NDPinSourceDetails::Delete::removeSource(query, id);
        }

        // 4. Delete - Contributor
        bool removeContributor(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Delete::removeContributor(query, id);
            });
        }
        bool removeContributor(QSqlQuery& query, const muuid::uuid& id) {
            return NDPinSourceDetails::Delete::removeContributor(query, id);
        }

        // 4. Delete - Flow
        bool removeFlow(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Delete::removeFlow(query, id);
            });
        }
        bool removeFlow(QSqlQuery& query, const muuid::uuid& id) {
            return NDPinSourceDetails::Delete::removeFlow(query, id);
        }

        // 4. Delete - Type
        bool removeType(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Delete::removeType(query, id);
            });
        }
        bool removeType(QSqlQuery& query, const muuid::uuid& id) {
            return NDPinSourceDetails::Delete::removeType(query, id);
        }

        // 4. Delete - Style
        bool removeStyle(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDPinSourceDetails::Delete::removeStyle(query, id);
            });
        }
        bool removeStyle(QSqlQuery& query, const muuid::uuid& id) {
            return NDPinSourceDetails::Delete::removeStyle(query, id);
        }
    };
}