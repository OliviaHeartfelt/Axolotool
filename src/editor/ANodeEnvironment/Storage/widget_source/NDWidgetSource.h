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

namespace NDWidgetSource {

    namespace Config {
        using namespace ::NDWidgetSourceDetails::Config;
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

                if (currentTables.contains("widget_source",      Qt::CaseInsensitive) == value) list.append("widget_source");
                if (currentTables.contains("widget_contributor", Qt::CaseInsensitive) == value) list.append("widget_contributor");
                if (currentTables.contains("widget_type",        Qt::CaseInsensitive) == value) list.append("widget_type");
                if (currentTables.contains("widget_data",        Qt::CaseInsensitive) == value) list.append("widget_data");
                return list;
            });
        }

        // 0. Init
        bool createAllTables() {
            return NDHelpers::useTransaction(pool(), [](QSqlQuery& query) {
                return NDWidgetSourceDetails::Init::createAllTables(query);
            });
        }
        bool createAllTables(QSqlQuery& query) {
            return NDWidgetSourceDetails::Init::createAllTables(query);
        }

        // 1. Create - Source
        bool createWidgetSource(const NDWidgetSourceDetails::Config::CreateWidgetSourceRecord& newSource) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Create::createWidgetSource(query, newSource);
            });
        }
        bool createWidgetSource(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetSourceRecord& newSource) {
            return NDWidgetSourceDetails::Create::createWidgetSource(query, newSource);
        }

        // 1. Create - Contributor
        bool createWidgetContributor(const NDWidgetSourceDetails::Config::CreateWidgetContributorRecord& newContributor) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Create::createWidgetContributor(query, newContributor);
            });
        }
        bool createWidgetContributor(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetContributorRecord& newContributor) {
            return NDWidgetSourceDetails::Create::createWidgetContributor(query, newContributor);
        }

        // 1. Create - Type
        bool createWidgetType(const NDWidgetSourceDetails::Config::CreateWidgetTypeRecord& newType) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Create::createWidgetType(query, newType);
            });
        }
        bool createWidgetType(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetTypeRecord& newType) {
            return NDWidgetSourceDetails::Create::createWidgetType(query, newType);
        }

        // 1. Create - Data
        bool createWidgetData(const NDWidgetSourceDetails::Config::CreateWidgetDataRecord& newData) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Create::createWidgetData(query, newData);
            });
        }
        bool createWidgetData(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetDataRecord& newData) {
            return NDWidgetSourceDetails::Create::createWidgetData(query, newData);
        }

        // 2. Read - Source
        std::optional<NDWidgetSourceDetails::Config::FullWidgetSourceRecord> getWidgetSource(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getWidgetSource(query, id);
            });
        }
        std::optional<NDWidgetSourceDetails::Config::FullWidgetSourceRecord> getWidgetSource(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Read::getWidgetSource(query, id);
        }

        // 2. Read - Contributor
        std::optional<NDWidgetSourceDetails::Config::FullWidgetContributorRecord> getWidgetContributor(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getWidgetContributor(query, id);
            });
        }
        std::optional<NDWidgetSourceDetails::Config::FullWidgetContributorRecord> getWidgetContributor(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Read::getWidgetContributor(query, id);
        }

        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetContributorRecord>> getAllWidgetContributors(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getAllWidgetContributors(query, sourceId, continueAtFail);
            });
        }
        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetContributorRecord>> getAllWidgetContributors(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWidgetSourceDetails::Read::getAllWidgetContributors(query, sourceId, continueAtFail);
        }

        // 2. Read - Type
        std::optional<NDWidgetSourceDetails::Config::FullWidgetTypeRecord> getWidgetType(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getWidgetType(query, id);
            });
        }
        std::optional<NDWidgetSourceDetails::Config::FullWidgetTypeRecord> getWidgetType(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Read::getWidgetType(query, id);
        }

        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetTypeRecord>> getContributorWidgetTypes(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getContributorWidgetTypes(query, contributorId, continueAtFail);
            });
        }
        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetTypeRecord>> getContributorWidgetTypes(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWidgetSourceDetails::Read::getContributorWidgetTypes(query, contributorId, continueAtFail);
        }

        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetTypeRecord>> getAllWidgetTypes(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getAllWidgetTypes(query, sourceId, continueAtFail);
            });
        }
        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetTypeRecord>> getAllWidgetTypes(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWidgetSourceDetails::Read::getAllWidgetTypes(query, sourceId, continueAtFail);
        }

        // 2. Read - Data
        std::optional<NDWidgetSourceDetails::Config::FullWidgetDataRecord> getWidgetData(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getWidgetData(query, id);
            });
        }
        std::optional<NDWidgetSourceDetails::Config::FullWidgetDataRecord> getWidgetData(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Read::getWidgetData(query, id);
        }

        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetDataRecord>> getContributorWidgetData(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getContributorWidgetData(query, contributorId, continueAtFail);
            });
        }
        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetDataRecord>> getContributorWidgetData(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWidgetSourceDetails::Read::getContributorWidgetData(query, contributorId, continueAtFail);
        }

        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetDataRecord>> getAllWidgetData(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getAllWidgetData(query, sourceId, continueAtFail);
            });
        }
        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetDataRecord>> getAllWidgetData(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWidgetSourceDetails::Read::getAllWidgetData(query, sourceId, continueAtFail);
        }

        // 3. Update - Source
        bool updateWidgetSource(const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetSourceRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Update::updateWidgetSource(query, id, newProperties);
            });
        }
        bool updateWidgetSource(QSqlQuery& query, const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetSourceRecord& newProperties) {
            return NDWidgetSourceDetails::Update::updateWidgetSource(query, id, newProperties);
        }

        // 3. Update - Contributor
        bool updateWidgetContributor(const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetContributorRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Update::updateWidgetContributor(query, id, newProperties);
            });
        }
        bool updateWidgetContributor(QSqlQuery& query, const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetContributorRecord& newProperties) {
            return NDWidgetSourceDetails::Update::updateWidgetContributor(query, id, newProperties);
        }

        // 3. Update - Type
        bool updateWidgetType(const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetTypeRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Update::updateWidgetType(query, id, newProperties);
            });
        }
        bool updateWidgetType(QSqlQuery& query, const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetTypeRecord& newProperties) {
            return NDWidgetSourceDetails::Update::updateWidgetType(query, id, newProperties);
        }

        // 3. Update - Data
        bool updateWidgetData(const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetDataRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Update::updateWidgetData(query, id, newProperties);
            });
        }
        bool updateWidgetData(QSqlQuery& query, const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetDataRecord& newProperties) {
            return NDWidgetSourceDetails::Update::updateWidgetData(query, id, newProperties);
        }

        // 4. Delete - Source
        bool removeWidgetSource(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Delete::removeWidgetSource(query, id);
            });
        }
        bool removeWidgetSource(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Delete::removeWidgetSource(query, id);
        }

        // 4. Delete - Contributor
        bool removeWidgetContributor(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Delete::removeWidgetContributor(query, id);
            });
        }
        bool removeWidgetContributor(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Delete::removeWidgetContributor(query, id);
        }

        // 4. Delete - Type
        bool removeWidgetType(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Delete::removeWidgetType(query, id);
            });
        }
        bool removeWidgetType(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Delete::removeWidgetType(query, id);
        }

        // 4. Delete - data
        bool removeWidgetData(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Delete::removeWidgetData(query, id);
            });
        }
        bool removeWidgetData(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Delete::removeWidgetData(query, id);
        }
    };
}