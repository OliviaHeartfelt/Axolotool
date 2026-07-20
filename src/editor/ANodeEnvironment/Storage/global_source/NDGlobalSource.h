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

namespace NDGlobalSource {

    namespace Config {
        using namespace ::NDGlobalSourceDetails::Config;
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

                if (currentTables.contains("global_source", Qt::CaseInsensitive) == value) list.append("global_source");
                return list;
            });
        }

        // 0. INIT
        bool createAllTables() {
            return NDHelpers::useTransaction(pool(), [](QSqlQuery& query) {
                return NDGlobalSourceDetails::Init::createAllTables(query);
                });
        }
        bool createAllTables(QSqlQuery& query) {
            return NDGlobalSourceDetails::Init::createAllTables(query);
        }

        // 1. CREATE
        inline bool createGlobalSource(const NDGlobalSourceDetails::Config::CreateGlobalSourceRecord& newSource) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDGlobalSourceDetails::Create::createGlobalSource(query, newSource);
                });
        }
        inline bool createGlobalSource(QSqlQuery& query, const NDGlobalSourceDetails::Config::CreateGlobalSourceRecord& newSource) {
            return NDGlobalSourceDetails::Create::createGlobalSource(query, newSource);
        }
        
        // 2. READ
        inline std::optional<NDGlobalSourceDetails::Config::FullGlobalSourceRecord> getGlobalSource(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDGlobalSourceDetails::Read::getGlobalSource(query, id);
                });
        }
        inline std::optional<NDGlobalSourceDetails::Config::FullGlobalSourceRecord> getGlobalSource(QSqlQuery& query, const muuid::uuid& id) {
                return NDGlobalSourceDetails::Read::getGlobalSource(query, id);
        }
        inline std::optional<QList<NDGlobalSourceDetails::Config::FullGlobalSourceRecord>> getAllGlobalSources(const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDGlobalSourceDetails::Read::getAllGlobalSources(query, continueAtFail);
                });
        }
        inline std::optional<QList<NDGlobalSourceDetails::Config::FullGlobalSourceRecord>> getAllGlobalSources(QSqlQuery& query, const bool continueAtFail = false) {
            return NDGlobalSourceDetails::Read::getAllGlobalSources(query, continueAtFail);
        }
        
        // 3. UPDATE
        inline bool updateGlobalSource(muuid::uuid id, const NDGlobalSourceDetails::Config::UpdateGlobalSourceRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDGlobalSourceDetails::Update::updateGlobalSource(query, id, newProperties);
                });
        }
        inline bool updateGlobalSource(QSqlQuery& query, muuid::uuid id, const NDGlobalSourceDetails::Config::UpdateGlobalSourceRecord& newProperties) {
            return NDGlobalSourceDetails::Update::updateGlobalSource(query, id, newProperties);
        }
        
        // 4. DELETE
        inline bool removeGlobalSource(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDGlobalSourceDetails::Delete::removeGlobalSource(query, id);
                });
        }
        inline bool removeGlobalSource(QSqlQuery& query, const muuid::uuid& id) {
            return NDGlobalSourceDetails::Delete::removeGlobalSource(query, id);
        }
    };
}