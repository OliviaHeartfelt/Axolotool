#pragma once

#include "details/Config.h"
#include "details/Init.h"
#include "details/Create.h"
#include "details/Read.h"
#include "details/Update.h"
#include "details/Delete.h"

#include "../NDConcepts.h"
#include "../NDHelpers.h"

namespace NDWidgetSource {

    using namespace ::NDWidgetSourceDetails::Config;

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

            if (currentTables.contains("widget_source",      Qt::CaseInsensitive) == value) list.append("widget_source");
            if (currentTables.contains("widget_contributor", Qt::CaseInsensitive) == value) list.append("widget_contributor");
            if (currentTables.contains("widget_type",        Qt::CaseInsensitive) == value) list.append("widget_type");
            if (currentTables.contains("widget_data",        Qt::CaseInsensitive) == value) list.append("widget_data");
            return list;
        }

        // 0. Init
        bool createAllTables() {
            return NDHelpers::useTransaction(database(), [](QSqlQuery& query) {
                return NDWidgetSourceDetails::Init::createAllTables(query);
            });
        }

        // 1. Create - Source
        bool createWidgetSource(const NDWidgetSourceDetails::Config::CreateWidgetSourceRecord& newSource) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Create::createWidgetSource(query, newSource);
            });
        }
        bool createWidgetSource(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetSourceRecord& newSource) {
            return NDWidgetSourceDetails::Create::createWidgetSource(query, newSource);
        }

        // 1. Create - Contributor
        bool createWidgetContributor(const NDWidgetSourceDetails::Config::CreateWidgetContributorRecord& newContributor) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Create::createWidgetContributor(query, newContributor);
            });
        }
        bool createWidgetContributor(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetContributorRecord& newContributor) {
            return NDWidgetSourceDetails::Create::createWidgetContributor(query, newContributor);
        }

        // 1. Create - Type
        template<NDConcepts::ByteConvertible Metadata>
        bool createWidgetType(const NDWidgetSourceDetails::Config::CreateWidgetTypeRecord<Metadata>& newType) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Create::createWidgetType(query, newType);
            });
        }
        template<NDConcepts::ByteConvertible Metadata>
        bool createWidgetType(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetTypeRecord<Metadata>& newType) {
            return NDWidgetSourceDetails::Create::createWidgetType(query, newType);
        }

        // 1. Create - Data
        template<NDConcepts::ByteConvertible Data>
        bool createWidgetData(const NDWidgetSourceDetails::Config::CreateWidgetDataRecord<Data>& newData) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Create::createWidgetData(query, newData);
            });
        }
        template<NDConcepts::ByteConvertible Data>
        bool createWidgetData(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetDataRecord<Data>& newData) {
            return NDWidgetSourceDetails::Create::createWidgetData(query, newData);
        }

        // 2. Read - Source
        std::optional<NDWidgetSourceDetails::Config::FullWidgetSourceRecord> getWidgetSource(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getWidgetSource(query, id);
            });
        }
        std::optional<NDWidgetSourceDetails::Config::FullWidgetSourceRecord> getWidgetSource(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Read::getWidgetSource(query, id);
        }

        // 2. Read - Contributor
        std::optional<NDWidgetSourceDetails::Config::FullWidgetContributorRecord> getWidgetContributor(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getWidgetContributor(query, id);
            });
        }
        std::optional<NDWidgetSourceDetails::Config::FullWidgetContributorRecord> getWidgetContributor(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Read::getWidgetContributor(query, id);
        }

        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetContributorRecord>> getAllWidgetContributors(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getAllWidgetContributors(query, sourceId, continueAtFail);
            });
        }
        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetContributorRecord>> getAllWidgetContributors(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWidgetSourceDetails::Read::getAllWidgetContributors(query, sourceId, continueAtFail);
        }

        // 2. Read - Type
        template<NDConcepts::ByteConvertible Metadata>
        std::optional<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>> getWidgetType(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getWidgetType(query, id);
            });
        }
        template<NDConcepts::ByteConvertible Metadata>
        std::optional<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>> getWidgetType(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Read::getWidgetType(query, id);
        }

        template<NDConcepts::ByteConvertible Metadata>
        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>>> getContributorWidgetTypes(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getContributorWidgetTypes(query, contributorId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible Metadata>
        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>>> getContributorWidgetTypes(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWidgetSourceDetails::Read::getContributorWidgetTypes(query, contributorId, continueAtFail);
        }

        template<NDConcepts::ByteConvertible Metadata>
        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>>> getAllWidgetTypes(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getAllWidgetTypes(query, sourceId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible Metadata>
        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>>> getAllWidgetTypes(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWidgetSourceDetails::Read::getAllWidgetTypes(query, sourceId, continueAtFail);
        }

        // 2. Read - Data
        template<NDConcepts::ByteConvertible Data>
        std::optional<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>> getWidgetData(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getWidgetData(query, id);
            });
        }
        template<NDConcepts::ByteConvertible Data>
        std::optional<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>> getWidgetData(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Read::getWidgetData(query, id);
        }

        template<NDConcepts::ByteConvertible Data>
        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>>> getContributorWidgetData(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getContributorWidgetData(query, contributorId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible Data>
        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>>> getContributorWidgetData(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWidgetSourceDetails::Read::getContributorWidgetData(query, contributorId, continueAtFail);
        }

        template<NDConcepts::ByteConvertible Data>
        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>>> getAllWidgetData(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Read::getAllWidgetData(query, sourceId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible Data>
        std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>>> getAllWidgetData(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWidgetSourceDetails::Read::getAllWidgetData(query, sourceId, continueAtFail);
        }

        // 3. Update - Source
        bool updateWidgetSource(const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetSourceRecord& newProperties) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Update::updateWidgetSource(query, id, newProperties);
            });
        }
        bool updateWidgetSource(QSqlQuery& query, const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetSourceRecord& newProperties) {
            return NDWidgetSourceDetails::Update::updateWidgetSource(query, id, newProperties);
        }

        // 3. Update - Contributor
        bool updateWidgetContributor(const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetContributorRecord& newProperties) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Update::updateWidgetContributor(query, id, newProperties);
            });
        }
        bool updateWidgetContributor(QSqlQuery& query, const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetContributorRecord& newProperties) {
            return NDWidgetSourceDetails::Update::updateWidgetContributor(query, id, newProperties);
        }

        // 3. Update - Type
        template<NDConcepts::ByteConvertible Metadata>
        bool updateWidgetType(const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetTypeRecord<Metadata>& newProperties) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Update::updateWidgetType(query, id, newProperties);
            });
        }
        template<NDConcepts::ByteConvertible Metadata>
        bool updateWidgetType(QSqlQuery& query, const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetTypeRecord<Metadata>& newProperties) {
            return NDWidgetSourceDetails::Update::updateWidgetType(query, id, newProperties);
        }

        // 3. Update - Data
        template<NDConcepts::ByteConvertible Data>
        bool updateWidgetData(const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetDataRecord<Data>& newProperties) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Update::updateWidgetData(query, id, newProperties);
            });
        }
        template<NDConcepts::ByteConvertible Data>
        bool updateWidgetData(QSqlQuery& query, const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetDataRecord<Data>& newProperties) {
            return NDWidgetSourceDetails::Update::updateWidgetData(query, id, newProperties);
        }

        // 4. Delete - Source
        bool removeWidgetSource(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Delete::removeWidgetSource(query, id);
            });
        }
        bool removeWidgetSource(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Delete::removeWidgetSource(query, id);
        }

        // 4. Delete - Contributor
        bool removeWidgetContributor(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Delete::removeWidgetContributor(query, id);
            });
        }
        bool removeWidgetContributor(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Delete::removeWidgetContributor(query, id);
        }

        // 4. Delete - Type
        bool removeWidgetType(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Delete::removeWidgetType(query, id);
            });
        }
        bool removeWidgetType(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Delete::removeWidgetType(query, id);
        }

        // 4. Delete - data
        bool removeWidgetData(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWidgetSourceDetails::Delete::removeWidgetData(query, id);
            });
        }
        bool removeWidgetData(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Delete::removeWidgetData(query, id);
        }
    };
}