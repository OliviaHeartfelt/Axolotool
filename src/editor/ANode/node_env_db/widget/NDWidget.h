#pragma once

#include "details/Config.h"
#include "details/Init.h"
#include "details/Create.h"
#include "details/Read.h"
#include "details/Update.h"
#include "details/Delete.h"

#include "../NDConcepts.h"
#include "../NDHelpers.h"

namespace NDWidget {

    namespace Config {
        using namespace ::NDWidgetDetails::Config;
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

            if (currentTables.contains("widget_core", Qt::CaseInsensitive) == value) list.append("widget_core");
            if (currentTables.contains("widget",      Qt::CaseInsensitive) == value) list.append("widget");
            return list;
        }

        // 0. Init
        bool createAllTables() {
            return NDHelpers::useTransaction(database(), [](QSqlQuery& query) {
                return NDWidgetDetails::Init::createAllTables(query);
            });
        }

        // 1. Create
        bool createWidgetCore(const NDWidgetDetails::Config::CreateWidgetCoreRecord& newWidgetCore) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Create::createWidgetCore(query, newWidgetCore);
            });
        }
        bool createWidgetCore(QSqlQuery& query, const NDWidgetDetails::Config::CreateWidgetCoreRecord& newWidgetCore) {
            return NDWidgetDetails::Create::createWidgetCore(query, newWidgetCore);
        }

        template<NDConcepts::ByteConvertible State>
        bool createWidget(const NDWidgetDetails::Config::CreateWidgetRecord& newWidget) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Create::createWidget(query, newWidget);
            });
        }
        template<NDConcepts::ByteConvertible State>
        bool createWidget(QSqlQuery& query, const NDWidgetDetails::Config::CreateWidgetRecord& newWidget) {
            return NDWidgetDetails::Create::createWidget(query, newWidget);
        }

        // 2. Read
        std::optional<NDWidgetDetails::Config::FullWidgetCoreRecord> getWidgetCore(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Read::getWidgetCore(query, id);
            });
        }
        std::optional<NDWidgetDetails::Config::FullWidgetCoreRecord> getWidgetCore(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetDetails::Read::getWidgetCore(query, id);
        }

        std::optional<QList<NDWidgetDetails::Config::FullWidgetCoreRecord>> getContributorWidgetCores(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Read::getContributorWidgetCores(query, contributorId, continueAtFail);
            });
        }
        std::optional<QList<NDWidgetDetails::Config::FullWidgetCoreRecord>> getContributorWidgetCores(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWidgetDetails::Read::getContributorWidgetCores(query, contributorId, continueAtFail);
        }

        std::optional<QList<NDWidgetDetails::Config::FullWidgetCoreRecord>> getAllWidgetCores(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Read::getAllWidgetCores(query, sourceId, continueAtFail);
            });
        }
        std::optional<QList<NDWidgetDetails::Config::FullWidgetCoreRecord>> getAllWidgetCores(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWidgetDetails::Read::getAllWidgetCores(query, sourceId, continueAtFail);
        }

        template<NDConcepts::ByteConvertible State>
        std::optional<NDWidgetDetails::Config::FullWidgetRecord<State>> getWidget(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Read::getWidget(query, id);
            });
        }
        template<NDConcepts::ByteConvertible State>
        std::optional<NDWidgetDetails::Config::FullWidgetRecord<State>> getWidget(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetDetails::Read::getWidget(query, id);
        }

        template<NDConcepts::ByteConvertible State>
        std::optional<QList<NDWidgetDetails::Config::FullWidgetRecord<State>>> getContributorWidgets(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Read::getContributorWidgets(query, contributorId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible State>
        std::optional<QList<NDWidgetDetails::Config::FullWidgetRecord<State>>> getContributorWidgets(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWidgetDetails::Read::getContributorWidgets(query, contributorId, continueAtFail);
        }

        template<NDConcepts::ByteConvertible State>
        std::optional<QList<NDWidgetDetails::Config::FullWidgetRecord<State>>> getAllWidgets(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Read::getAllWidgets(query, sourceId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible State>
        std::optional<QList<NDWidgetDetails::Config::FullWidgetRecord<State>>> getAllWidgets(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWidgetDetails::Read::getAllWidgets(query, sourceId, continueAtFail);
        }

        // 3. Update
        bool updateWidgetCore(const muuid::uuid& id, const NDWidgetDetails::Config::UpdateWidgetCoreRecord& newProperties) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Update::updateWidgetCore(query, id, newProperties);
            });
        }
        bool updateWidgetCore(QSqlQuery& query, const muuid::uuid& id, const NDWidgetDetails::Config::UpdateWidgetCoreRecord& newProperties) {
            return NDWidgetDetails::Update::updateWidgetCore(query, id, newProperties);
        }

        template<NDConcepts::ByteConvertible State>
        bool updateWidget(const muuid::uuid& id, const NDWidgetDetails::Config::UpdateWidgetRecord<State>& newProperties) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Update::updateWidget(query, id, newProperties);
            });
        }
        template<NDConcepts::ByteConvertible State>
        bool updateWidget(QSqlQuery& query, const muuid::uuid& id, const NDWidgetDetails::Config::UpdateWidgetRecord<State>& newProperties) {
            return NDWidgetDetails::Update::updateWidget(query, id, newProperties);
        }

        // 4. Delete
        bool removeWidgetCore(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Delete::removeWidgetCore(query, id);
            });
        }
        bool removeWidgetCore(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetDetails::Delete::removeWidgetCore(query, id);
        }

        bool removeWidget(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetDetails::Delete::removeWidget(query, id);
            });
        }
        bool removeWidget(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetDetails::Delete::removeWidget(query, id);
        }
    };
}