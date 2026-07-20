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

namespace NDWidget {

    namespace Config {
        using namespace ::NDWidgetDetails::Config;
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

                if (currentTables.contains("widget_core", Qt::CaseInsensitive) == value) list.append("widget_core");
                if (currentTables.contains("widget",      Qt::CaseInsensitive) == value) list.append("widget");
                return list;
            });
        }

        // 0. Init
        bool createAllTables() {
            return NDHelpers::useTransaction(pool(), [](QSqlQuery& query) {
                return NDWidgetDetails::Init::createAllTables(query);
            });
        }
        bool createAllTables(QSqlQuery& query) {
            return NDWidgetDetails::Init::createAllTables(query);
        }

        // 1. Create
        bool createWidgetCore(const NDWidgetDetails::Config::CreateWidgetCoreRecord& newWidgetCore) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Create::createWidgetCore(query, newWidgetCore);
            });
        }
        bool createWidgetCore(QSqlQuery& query, const NDWidgetDetails::Config::CreateWidgetCoreRecord& newWidgetCore) {
            return NDWidgetDetails::Create::createWidgetCore(query, newWidgetCore);
        }

        bool createWidget(const NDWidgetDetails::Config::CreateWidgetRecord& newWidget) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Create::createWidget(query, newWidget);
            });
        }
        bool createWidget(QSqlQuery& query, const NDWidgetDetails::Config::CreateWidgetRecord& newWidget) {
            return NDWidgetDetails::Create::createWidget(query, newWidget);
        }

        // 2. Read
        std::optional<NDWidgetDetails::Config::FullWidgetCoreRecord> getWidgetCore(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Read::getWidgetCore(query, id);
            });
        }
        std::optional<NDWidgetDetails::Config::FullWidgetCoreRecord> getWidgetCore(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetDetails::Read::getWidgetCore(query, id);
        }

        std::optional<QList<NDWidgetDetails::Config::FullWidgetCoreRecord>> getContributorWidgetCores(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Read::getContributorWidgetCores(query, contributorId, continueAtFail);
            });
        }
        std::optional<QList<NDWidgetDetails::Config::FullWidgetCoreRecord>> getContributorWidgetCores(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWidgetDetails::Read::getContributorWidgetCores(query, contributorId, continueAtFail);
        }

        std::optional<QList<NDWidgetDetails::Config::FullWidgetCoreRecord>> getAllWidgetCores(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Read::getAllWidgetCores(query, sourceId, continueAtFail);
            });
        }
        std::optional<QList<NDWidgetDetails::Config::FullWidgetCoreRecord>> getAllWidgetCores(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWidgetDetails::Read::getAllWidgetCores(query, sourceId, continueAtFail);
        }

        std::optional<NDWidgetDetails::Config::FullWidgetRecord> getWidget(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Read::getWidget(query, id);
            });
        }
        std::optional<NDWidgetDetails::Config::FullWidgetRecord> getWidget(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetDetails::Read::getWidget(query, id);
        }

        std::optional<QList<NDWidgetDetails::Config::FullWidgetRecord>> getContributorWidgets(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Read::getContributorWidgets(query, contributorId, continueAtFail);
            });
        }
        std::optional<QList<NDWidgetDetails::Config::FullWidgetRecord>> getContributorWidgets(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWidgetDetails::Read::getContributorWidgets(query, contributorId, continueAtFail);
        }

        std::optional<QList<NDWidgetDetails::Config::FullWidgetRecord>> getAllWidgets(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Read::getAllWidgets(query, sourceId, continueAtFail);
            });
        }
        std::optional<QList<NDWidgetDetails::Config::FullWidgetRecord>> getAllWidgets(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWidgetDetails::Read::getAllWidgets(query, sourceId, continueAtFail);
        }

        // 3. Update
        bool updateWidgetCore(const muuid::uuid& id, const NDWidgetDetails::Config::UpdateWidgetCoreRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Update::updateWidgetCore(query, id, newProperties);
            });
        }
        bool updateWidgetCore(QSqlQuery& query, const muuid::uuid& id, const NDWidgetDetails::Config::UpdateWidgetCoreRecord& newProperties) {
            return NDWidgetDetails::Update::updateWidgetCore(query, id, newProperties);
        }

        bool updateWidget(const muuid::uuid& id, const NDWidgetDetails::Config::UpdateWidgetRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Update::updateWidget(query, id, newProperties);
            });
        }
        bool updateWidget(QSqlQuery& query, const muuid::uuid& id, const NDWidgetDetails::Config::UpdateWidgetRecord& newProperties) {
            return NDWidgetDetails::Update::updateWidget(query, id, newProperties);
        }

        // 4. Delete
        bool removeWidgetCore(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Delete::removeWidgetCore(query, id);
            });
        }
        bool removeWidgetCore(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetDetails::Delete::removeWidgetCore(query, id);
        }

        bool removeWidget(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Delete::removeWidget(query, id);
            });
        }
        bool removeWidget(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetDetails::Delete::removeWidget(query, id);
        }
    };
}