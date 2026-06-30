#pragma once

#include "details/Config.h"
#include "details/Init.h"
#include "details/Create.h"
#include "details/Read.h"
#include "details/Update.h"
#include "details/Delete.h"

#include "../NDConcepts.h"

namespace NDWidget {
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
            if (currentTables.contains("widget_core", Qt::CaseInsensitive) == value) list.append("widget_core");
            if (currentTables.contains("widget", Qt::CaseInsensitive) == value) list.append("widget");
            return list;
        }

        // 0. Init
        bool createAllTables() {
            QSqlDatabase db = parent->getDatabase(ComponentFriendTag::createKey<DBContext>());
            QSqlQuery query(db);

            Utility::SqlTransaction tr(db);
            if (!tr.started()) return false;

            const bool wereTablesCreated = NDWidgetDetails::Init::createAllTables(query);

            if (!wereTablesCreated || !tr.commit()) return false;
            return true;
        }

        // 1. Create
        inline std::optional<muuid::uuid> createWidgetCore(const NDWidgetDetails::Config::CreateWidgetCoreRecord& newWidgetCore) {
            return useTransactionUUID([](QSqlQuery& query, const auto& data) {
                return NDWidgetDetails::Create::createWidgetCore(query, data);
            }, newWidgetCore);
        }
        template<NDWidgetDetails::Config::WidgetState State>
        inline std::optional<muuid::uuid> createWidget(const NDWidgetDetails::Config::CreateWidgetRecord& newWidget, const std::optional<State>& state = std::nullopt) {
            return useTransactionUUID([](QSqlQuery& query, const auto& widgetData, const auto& stateData) {
                return NDWidgetDetails::Create::createWidget(query, widgetData, stateData);
            }, newWidget, state);
        }

        // 2. Read
        inline std::optional<NDWidgetDetails::Config::FullWidgetCoreRecord> getWidgetCore(const muuid::uuid& id) {
            return useQuery([](QSqlQuery& query, const auto& localId) {
                return NDWidgetDetails::Read::getWidgetCore(query, localId);
            }, id);
        }
        inline std::optional<QList<NDWidgetDetails::Config::FullWidgetCoreRecord>> getContributorWidgetCores(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return useQuery([](QSqlQuery& query, const auto& cId, const auto& failFlag) {
                return NDWidgetDetails::Read::getContributorWidgetCores(query, cId, failFlag);
            }, contributorId, continueAtFail);
        }
        inline std::optional<QList<NDWidgetDetails::Config::FullWidgetCoreRecord>> getAllWidgetCores(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return useQuery([](QSqlQuery& query, const auto& sId, const auto& failFlag) {
                return NDWidgetDetails::Read::getAllWidgetCores(query, sId, failFlag);
            }, sourceId, continueAtFail);
        }
        template<NDWidgetDetails::Config::WidgetState State>
        inline std::optional<NDWidgetDetails::Config::FullWidgetRecord<State>> getWidget(const muuid::uuid& id) {
            return useQuery([](QSqlQuery& query, const auto& localId) {
                return NDWidgetDetails::Read::getWidget(query, localId);
            }, id);
        }
        template<NDWidgetDetails::Config::WidgetState State>
        inline std::optional<QList<NDWidgetDetails::Config::FullWidgetRecord<State>>> getContributorWidgets(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return useQuery([](QSqlQuery& query, const auto& cId, const auto& failFlag) {
                return NDWidgetDetails::Read::getContributorWidgets(query, cId, failFlag);
            }, contributorId, continueAtFail);
        }
        template<NDWidgetDetails::Config::WidgetState State>
        inline std::optional<QList<NDWidgetDetails::Config::FullWidgetRecord<State>>> getAllWidgets(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return useQuery([](QSqlQuery& query, const auto& sId, const auto& failFlag) {
                return NDWidgetDetails::Read::getAllWidgets(query, sId, failFlag);
            }, sourceId, continueAtFail);
        }

        // 3. Update
        inline bool updateWidgetCore(const muuid::uuid& id, const NDWidgetDetails::Config::updateWidgetCoreRecord& newProperties) {
            return useTransaction([](QSqlQuery& query, const auto& localId, const auto& props) {
                return NDWidgetDetails::Update::updateWidgetCore(query, localId, props);
            }, id, newProperties);
        }
        template<NDWidgetDetails::Config::WidgetState State>
        inline bool updateWidget(const muuid::uuid& id, const NDWidgetDetails::Config::UpdateWidgetRecord<State>& newProperties) {
            return useTransaction([](QSqlQuery& query, const auto& localId, const auto& props) {
                return NDWidgetDetails::Update::updateWidget(query, localId, props);
            }, id, newProperties);
        }

        // 4. Delete
        inline bool removeWidgetCore(const muuid::uuid& id) {
            return useTransaction([](QSqlQuery& query, const auto& localId) {
                return NDWidgetDetails::Delete::removeWidgetCore(query, localId);
            }, id);
        }
        inline bool removeWidget(const muuid::uuid& id) {
            return useTransaction([](QSqlQuery& query, const auto& localId) {
                return NDWidgetDetails::Delete::removeWidget(query, localId);
            }, id);
        }
    };
}