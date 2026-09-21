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
#include "../NDParser.h"

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

        bool importGlobalSource(const QJsonObject& doc, QSqlQuery& query, bool isResourceOptional = true) {
            QJsonValue jsonValue = doc.value("global_source");
            if (jsonValue.isUndefined()) return isResourceOptional;
            if (!jsonValue.isArray()) return false;
            QJsonArray jsonArray = jsonValue.toArray();

            for (const QJsonValue& val : jsonArray) {
                if (!val.isObject()) {
                    qWarning() << "Parsing failed: Array element is not a JSON object.";
                    return false;
                }

                auto optNewRecord = NDGlobalSourceDetails::Config::CreateGlobalSourceRecord::Parse(val.toObject());
                if (!optNewRecord) return false;

                if (!NDGlobalSourceDetails::Create::createGlobalSource(query, *optNewRecord)) return false;
            }
            return true;
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
        bool createGlobalSource(const NDGlobalSourceDetails::Config::CreateGlobalSourceRecord& newSource) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDGlobalSourceDetails::Create::createGlobalSource(query, newSource);
                });
        }
        bool createGlobalSource(QSqlQuery& query, const NDGlobalSourceDetails::Config::CreateGlobalSourceRecord& newSource) {
            return NDGlobalSourceDetails::Create::createGlobalSource(query, newSource);
        }
        
        // 2. READ
        std::optional<NDGlobalSourceDetails::Config::FullGlobalSourceRecord> getGlobalSource(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDGlobalSourceDetails::Read::getGlobalSource(query, id);
                });
        }
        std::optional<NDGlobalSourceDetails::Config::FullGlobalSourceRecord> getGlobalSource(QSqlQuery& query, const muuid::uuid& id) {
                return NDGlobalSourceDetails::Read::getGlobalSource(query, id);
        }
        std::optional<QList<NDGlobalSourceDetails::Config::FullGlobalSourceRecord>> getAllGlobalSources(const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDGlobalSourceDetails::Read::getAllGlobalSources(query, continueAtFail);
                });
        }
        std::optional<QList<NDGlobalSourceDetails::Config::FullGlobalSourceRecord>> getAllGlobalSources(QSqlQuery& query, const bool continueAtFail = false) {
            return NDGlobalSourceDetails::Read::getAllGlobalSources(query, continueAtFail);
        }
        
        // 3. UPDATE
        bool updateGlobalSource(muuid::uuid id, const NDGlobalSourceDetails::Config::UpdateGlobalSourceRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDGlobalSourceDetails::Update::updateGlobalSource(query, id, newProperties);
                });
        }
        bool updateGlobalSource(QSqlQuery& query, muuid::uuid id, const NDGlobalSourceDetails::Config::UpdateGlobalSourceRecord& newProperties) {
            return NDGlobalSourceDetails::Update::updateGlobalSource(query, id, newProperties);
        }
        
        // 4. DELETE
        bool removeGlobalSource(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDGlobalSourceDetails::Delete::removeGlobalSource(query, id);
                });
        }
        bool removeGlobalSource(QSqlQuery& query, const muuid::uuid& id) {
            return NDGlobalSourceDetails::Delete::removeGlobalSource(query, id);
        }
    };
}