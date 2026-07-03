#pragma once

#include "details/Config.h"
#include "details/Init.h"
#include "details/Create.h"
#include "details/Read.h"
#include "details/Update.h"
#include "details/Delete.h"

#include "../NDConcepts.h"

namespace NDWidgetSource {

    struct ComponentFriendTag {
        template<typename DBContext>
        static typename DBContext::StorageKey createKey() { return {}; }
    };

    template<NDConcepts::DatabaseProvider DBContext>
    class Component {
        DBContext* parent;

        template<typename Func, typename... Args>
            requires ((std::is_lvalue_reference_v<Args>&& std::is_const_v<std::remove_reference_t<Args>>) && ...)
        std::optional<muuid::uuid> useTransactionUUID(Func&& fn, Args&&... args) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return std::nullopt;

            std::optional<muuid::uuid> id = std::invoke(std::forward<Func>(fn), query, std::forward<Args>(args)...);
            if (!id) return std::nullopt;

            if (!tr.commit()) return std::nullopt;
            return id;
        }

        template<typename Func, typename... Args>
            requires ((std::is_lvalue_reference_v<Args>&& std::is_const_v<std::remove_reference_t<Args>>) && ...)
        bool useTransaction(Func&& fn, Args&&... args) {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            if (!std::invoke(std::forward<Func>(fn), query, std::forward<Args>(args)...)) return false;

            if (!tr.commit()) return false;
            return true;
        }

        template<typename Func, typename... Args>
            requires ((std::is_lvalue_reference_v<Args>&& std::is_const_v<std::remove_reference_t<Args>>) && ...)
        auto useQuery(Func&& fn, Args&&... args) const {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);
            return std::invoke(std::forward<Func>(fn), query, std::forward<Args>(args)...);
        }

    public:
        explicit Component(DBContext* parentCtx) : parent(parentCtx) {}

        QStringList existsTables(const bool value) const {
            QStringList list;
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            const QStringList currentTables = db.tables();
            if (currentTables.contains("widget_source", Qt::CaseInsensitive) == value) list.append("widget_source");
            if (currentTables.contains("widget_contributor", Qt::CaseInsensitive) == value) list.append("widget_contributor");
            if (currentTables.contains("widget_type", Qt::CaseInsensitive) == value) list.append("widget_type");
            if (currentTables.contains("widget_data", Qt::CaseInsensitive) == value) list.append("widget_data");
            return list;
        }

        // 0. Init
        bool createAllTables() {
            return useTransaction([](QSqlQuery& query) {
                return NDWidgetSourceDetails::Init::createAllTables(query);
            });
        }

        // 1. Create - Source
        inline std::optional<muuid::uuid> createWidgetSource(const NDWidgetSourceDetails::Config::CreateWidgetSourceRecord& newSource) {
            return useTransactionUUID([](QSqlQuery& query, const auto& data) {
                return NDWidgetSourceDetails::Create::createWidgetSource(query, data);
            }, newSource);
        }
        inline std::optional<muuid::uuid> createWidgetSource(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetSourceRecord& newSource) {
            return NDWidgetSourceDetails::Create::createWidgetSource(query, newSource);
        }

        // 1. Create - Contributor
        inline std::optional<muuid::uuid> createWidgetContributor(const NDWidgetSourceDetails::Config::CreateWidgetContributorRecord& newContributor) {
            return useTransactionUUID([](QSqlQuery& query, const auto& data) {
                return NDWidgetSourceDetails::Create::createWidgetContributor(query, data);
            }, newContributor);
        }
        inline std::optional<muuid::uuid> createWidgetContributor(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetContributorRecord& newContributor) {
            return NDWidgetSourceDetails::Create::createWidgetContributor(query, newContributor);
        }

        // 1. Create - Type
        template<NDWidgetSourceDetails::Config::ByteConvertible Metadata>
        inline std::optional<muuid::uuid> createWidgetType(const NDWidgetSourceDetails::Config::CreateWidgetTypeRecord<Metadata>& newType) {
            return useTransactionUUID([](QSqlQuery& query, const auto& data) {
                return NDWidgetSourceDetails::Create::createWidgetType(query, data);
            }, newType);
        }
        template<NDWidgetSourceDetails::Config::ByteConvertible Metadata>
        inline std::optional<muuid::uuid> createWidgetType(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetTypeRecord<Metadata>& newType) {
            return NDWidgetSourceDetails::Create::createWidgetType(query, newType);
        }

        // 1. Create - Data
        template<NDWidgetSourceDetails::Config::ByteConvertible Data>
        inline std::optional<muuid::uuid> createWidgetData(const NDWidgetSourceDetails::Config::CreateWidgetDataRecord<Data>& newData) {
            return useTransactionUUID([](QSqlQuery& query, const auto& data) {
                return NDWidgetSourceDetails::Create::createWidgetData(query, data);
            }, newData);
        }
        template<NDWidgetSourceDetails::Config::ByteConvertible Data>
        inline std::optional<muuid::uuid> createWidgetData(QSqlQuery& query, const NDWidgetSourceDetails::Config::CreateWidgetDataRecord<Data>& newData) {
            return NDWidgetSourceDetails::Create::createWidgetData(query, newData);
        }


        // 2. Read - Source
        inline std::optional<NDWidgetSourceDetails::Config::FullWidgetSourceRecord> getWidgetSource(const muuid::uuid& id) {
            return useQuery([](QSqlQuery& query, const auto& localId) {
                return NDWidgetSourceDetails::Read::getWidgetSource(query, localId);
            }, id);
        }
        inline std::optional<NDWidgetSourceDetails::Config::FullWidgetSourceRecord> getWidgetSource(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Read::getWidgetSource(query, id);
        }

        // 2. Read - Contributor
        inline std::optional<NDWidgetSourceDetails::Config::FullWidgetContributorRecord> getWidgetContributor(const muuid::uuid& id) {
            return useQuery([](QSqlQuery& query, const auto& localId) {
                return NDWidgetSourceDetails::Read::getWidgetContributor(query, localId);
            }, id);
        }
        inline std::optional<NDWidgetSourceDetails::Config::FullWidgetContributorRecord> getWidgetContributor(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Read::getWidgetContributor(query, id);
        }
        inline std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetContributorRecord>> getAllWidgetContributors(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return useQuery([](QSqlQuery& query, const auto& localId, const bool& localContinueAtFail) {
                return NDWidgetSourceDetails::Read::getAllWidgetContributors(query, localId, localContinueAtFail);
            }, sourceId, continueAtFail);
        }
        inline std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetContributorRecord>> getAllWidgetContributors(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWidgetSourceDetails::Read::getAllWidgetContributors(query, sourceId, continueAtFail);
        }

        // 2. Read - Type
        template<NDWidgetSourceDetails::Config::ByteConvertible Metadata>
        inline std::optional<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>> getWidgetType(const muuid::uuid& id) {
            return useQuery([](QSqlQuery& query, const auto& localId) {
                return NDWidgetSourceDetails::Read::getWidgetType(query, localId);
            }, id);
        }
        template<NDWidgetSourceDetails::Config::ByteConvertible Metadata>
        inline std::optional<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>> getWidgetType(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Read::getWidgetType(query, id);
        }
        template<NDWidgetSourceDetails::Config::ByteConvertible Metadata>
        inline std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>>> getContributorWidgetTypes(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return useQuery([](QSqlQuery& query, const auto& localId, const bool& localContinueAtFail) {
                return NDWidgetSourceDetails::Read::getContributorWidgetTypes(query, localId, localContinueAtFail);
            }, contributorId, continueAtFail);
        }
        template<NDWidgetSourceDetails::Config::ByteConvertible Metadata>
        inline std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>>> getContributorWidgetTypes(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWidgetSourceDetails::Read::getContributorWidgetTypes(query, contributorId, continueAtFail);
        }
        template<NDWidgetSourceDetails::Config::ByteConvertible Metadata>
        inline std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>>> getAllWidgetTypes(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return useQuery([](QSqlQuery& query, const auto& localId, const bool& localContinueAtFail) {
                return NDWidgetSourceDetails::Read::getAllWidgetTypes(query, localId, localContinueAtFail);
            }, sourceId, continueAtFail);
        }
        template<NDWidgetSourceDetails::Config::ByteConvertible Metadata>
        inline std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetTypeRecord<Metadata>>> getAllWidgetTypes(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWidgetSourceDetails::Read::getAllWidgetTypes(query, sourceId, continueAtFail);
        }

        // 2. Read - Data
        template<NDWidgetSourceDetails::Config::ByteConvertible Data>
        inline std::optional<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>> getWidgetData(const muuid::uuid& id) {
            return useQuery([](QSqlQuery& query, const auto& localId) {
                return NDWidgetSourceDetails::Read::getWidgetData(query, localId);
            }, id);
        }
        template<NDWidgetSourceDetails::Config::ByteConvertible Data>
        inline std::optional<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>> getWidgetData(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Read::getWidgetData(query, id);
        }
        template<NDWidgetSourceDetails::Config::ByteConvertible Data>
        inline std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>>> getContributorWidgetData(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return useQuery([](QSqlQuery& query, const auto& localId, const bool& localContinueAtFail) {
                return NDWidgetSourceDetails::Read::getContributorWidgetData(query, localId, localContinueAtFail);
            }, contributorId, continueAtFail);
        }
        template<NDWidgetSourceDetails::Config::ByteConvertible Data>
        inline std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>>> getContributorWidgetData(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWidgetSourceDetails::Read::getContributorWidgetData(query, contributorId, continueAtFail);
        }
        template<NDWidgetSourceDetails::Config::ByteConvertible Data>
        inline std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>>> getAllWidgetData(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return useQuery([](QSqlQuery& query, const auto& localId, const bool& localContinueAtFail) {
                return NDWidgetSourceDetails::Read::getAllWidgetData(query, localId, localContinueAtFail);
            }, sourceId, continueAtFail);
        }
        template<NDWidgetSourceDetails::Config::ByteConvertible Data>
        inline std::optional<QList<NDWidgetSourceDetails::Config::FullWidgetDataRecord<Data>>> getAllWidgetData(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWidgetSourceDetails::Read::getAllWidgetData(query, sourceId, continueAtFail);
        }

        // 3. Update - Source
        inline bool updateWidgetSource(const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetSourceRecord& newProperties) {
            return useTransaction([](QSqlQuery& query, const auto& localId, const auto& props) {
                return NDWidgetSourceDetails::Update::updateWidgetSource(query, localId, props);
            }, id, newProperties);
        }
        inline bool updateWidgetSource(QSqlQuery& query, const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetSourceRecord& newProperties) {
            return NDWidgetSourceDetails::Update::updateWidgetSource(query, id, newProperties);
        }

        // 3. Update - Contributor
        inline bool updateWidgetContributor(const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetContributorRecord& newProperties) {
            return useTransaction([](QSqlQuery& query, const auto& localId, const auto& props) {
                return NDWidgetSourceDetails::Update::updateWidgetContributor(query, localId, props);
            }, id, newProperties);
        }
        inline bool updateWidgetContributor(QSqlQuery& query, const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetContributorRecord& newProperties) {
            return NDWidgetSourceDetails::Update::updateWidgetContributor(query, id, newProperties);
        }

        // 3. Update - Type
        template<NDWidgetSourceDetails::Config::ByteConvertible Metadata>
        inline bool updateWidgetType(const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetTypeRecord<Metadata>& newProperties) {
            return useTransaction([](QSqlQuery& query, const auto& localId, const auto& props) {
                return NDWidgetSourceDetails::Update::updateWidgetType(query, localId, props);
            }, id, newProperties);
        }
        template<NDWidgetSourceDetails::Config::ByteConvertible Metadata>
        inline bool updateWidgetType(QSqlQuery& query, const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetTypeRecord<Metadata>& newProperties) {
            return NDWidgetSourceDetails::Update::updateWidgetType(query, id, newProperties);
        }

        // 3. Update - Data
        template<NDWidgetSourceDetails::Config::ByteConvertible Data>
        inline bool updateWidgetData(const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetDataRecord<Data>& newProperties) {
            return useTransaction([](QSqlQuery& query, const auto& localId, const auto& props) {
                return NDWidgetSourceDetails::Update::updateWidgetData(query, localId, props);
            }, id, newProperties);
        }
        template<NDWidgetSourceDetails::Config::ByteConvertible Data>
        inline bool updateWidgetData(QSqlQuery& query, const muuid::uuid& id, const NDWidgetSourceDetails::Config::UpdateWidgetDataRecord<Data>& newProperties) {
            return NDWidgetSourceDetails::Update::updateWidgetData(query, id, newProperties);
        }

        // 4. Delete - Source
        inline bool removeWidgetSource(const muuid::uuid& id) {
            return useTransaction([](QSqlQuery& query, const auto& localId) {
                return NDWidgetSourceDetails::Delete::removeWidgetSource(query, localId);
            }, id);
        }
        inline bool removeWidgetSource(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Delete::removeWidgetSource(query, id);
        }

        // 4. Delete - Contributor
        inline bool removeWidgetContributor(const muuid::uuid& id) {
            return useTransaction([](QSqlQuery& query, const auto& localId) {
                return NDWidgetSourceDetails::Delete::removeWidgetContributor(query, localId);
            }, id);
        }
        inline bool removeWidgetContributor(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Delete::removeWidgetContributor(query, id);
        }

        // 4. Delete - Type
        inline bool removeWidgetType(const muuid::uuid& id) {
            return useTransaction([](QSqlQuery& query, const auto& localId) {
                return NDWidgetSourceDetails::Delete::removeWidgetType(query, localId);
            }, id);
        }
        inline bool removeWidgetType(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Delete::removeWidgetType(query, id);
        }

        // 4. Delete - data
        inline bool removeWidgetData(const muuid::uuid& id) {
            return useTransaction([](QSqlQuery& query, const auto& localId) {
                return NDWidgetSourceDetails::Delete::removeWidgetData(query, localId);
            }, id);
        }
        inline bool removeWidgetData(QSqlQuery& query, const muuid::uuid& id) {
            return NDWidgetSourceDetails::Delete::removeWidgetData(query, id);
        }
    };
}