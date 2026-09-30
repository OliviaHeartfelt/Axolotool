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

namespace NDCell {

    namespace Config {
        using namespace NDCellDetails::Config;
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

                if (currentTables.contains("cell",        Qt::CaseInsensitive) == value) list.append("cell");
                if (currentTables.contains("cell_origin", Qt::CaseInsensitive) == value) list.append("cell_origin");
                return list;
            });
        }

        bool importCell(const QJsonObject& doc, QSqlQuery& query, bool isResourceOptional = true) {
            QJsonValue jsonValue = doc.value("cell");
            if (jsonValue.isUndefined()) return isResourceOptional;
            if (!jsonValue.isArray()) return false;
            QJsonArray jsonArray = jsonValue.toArray();

            for (const QJsonValue& val : jsonArray) {
                if (!val.isObject()) {
                    qWarning() << "Parsing failed: Array element is not a JSON object.";
                    return false;
                }

                auto optNewRecord = NDCellDetails::Config::CreateCellRecord::Parse(val.toObject());
                if (!optNewRecord) return false;

                if (!NDCellDetails::Create::createCell(query, *optNewRecord)) return false;
            }
            return true;
        }
        bool importCellOrigin(const QJsonObject& doc, QSqlQuery& query, bool isResourceOptional = true) {
            QJsonValue jsonValue = doc.value("cell_origin");
            if (jsonValue.isUndefined()) return isResourceOptional;
            if (!jsonValue.isArray()) return false;
            QJsonArray jsonArray = jsonValue.toArray();

            for (const QJsonValue& elementValue : jsonArray) {
                if (!elementValue.isObject()) continue;

                QJsonObject innerObject = elementValue.toObject();
                QStringList keys = innerObject.keys();

                if (!keys.isEmpty()) {
                    auto optNodeCoreID = muuid::uuid::from_chars(keys.first().toStdString());
                    if (!optNodeCoreID) continue;

                    QJsonArray nestedArray = innerObject.value(keys.first()).toArray();

                    for (const QJsonValue& innerElement : nestedArray) {
                        if (!innerElement.isObject()) {
                            qWarning() << "Parsing failed: Array element is not a JSON object.";
                            return false;
                        }

                        auto optNewRecord = NDCellDetails::Config::CreateCellOriginRecord::Parse(innerElement.toObject(), *optNodeCoreID);
                        if (!optNewRecord) return false;

                        if (!NDCellDetails::Create::createCellOrigin(query, *optNewRecord)) return false;
                    }
                }
            }
            return true;
        }

        // 0. Init
        bool createAllTables() {
            return NDHelpers::useQuery(pool(), [](QSqlQuery& query) {
                return NDCellDetails::Init::createAllTables(query);
            });
        }
        bool createAllTables(QSqlQuery& query) {
            return NDCellDetails::Init::createAllTables(query);
        }

        // 1. Create
        bool createCell(const NDCellDetails::Config::CreateCellRecord& newCell, bool overrideOnCollision = false) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDCellDetails::Create::createCell(query, newCell, overrideOnCollision);
                });
        }
        bool createCell(QSqlQuery& query, const NDCellDetails::Config::CreateCellRecord& newCell, bool overrideOnCollision = false) {
            return NDCellDetails::Create::createCell(query, newCell, overrideOnCollision);
        }
        bool createCellOrigin(const NDCellDetails::Config::CreateCellOriginRecord& newCellOrigin, bool overrideOnCollision = false) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDCellDetails::Create::createCellOrigin(query, newCellOrigin, overrideOnCollision);
                });
        }
        bool createCellOrigin(QSqlQuery& query, const NDCellDetails::Config::CreateCellOriginRecord& newCellOrigin, bool overrideOnCollision = false) {
            return NDCellDetails::Create::createCellOrigin(query, newCellOrigin, overrideOnCollision);
        }

        // 2. Read
        std::optional<NDCellDetails::Config::FullCellRecord> getCell(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDCellDetails::Read::getCell(query, id);
            });
        }
        std::optional<NDCellDetails::Config::FullCellRecord> getCell(QSqlQuery& query, const muuid::uuid& id) {
            return NDCellDetails::Read::getCell(query, id);
        }

        std::optional<QList<NDCellDetails::Config::FullCellRecord>> getAllCells(const muuid::uuid& nodeId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDCellDetails::Read::getAllCells(query, nodeId, continueAtFail);
            });
        }
        std::optional<QList<NDCellDetails::Config::FullCellRecord>> getAllCells(QSqlQuery& query, const muuid::uuid& nodeId, const bool continueAtFail = true) {
            return NDCellDetails::Read::getAllCells(query, nodeId, continueAtFail);
        }

        std::optional<QList<Config::FullCellOriginRecord>> getNodeCoreOriginCells(const muuid::uuid& nodeCoreId, const bool continueAtFail = true) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDCellDetails::Read::getNodeCoreOriginCells(query, nodeCoreId, continueAtFail);
                });
        }
        std::optional<QList<Config::FullCellOriginRecord>> getNodeCoreOriginCells(QSqlQuery& query, const muuid::uuid& nodeCoreId, const bool continueAtFail = true) {
            return NDCellDetails::Read::getNodeCoreOriginCells(query, nodeCoreId, continueAtFail);
        }

        // 3. Update
        bool updateLayout(const muuid::uuid& id, const NDCellDetails::Config::UpdateCellRecord& newCellInfo, const bool overrideOnCollision = false) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDCellDetails::Update::updateCell(query, id, newCellInfo, overrideOnCollision);
            });
        }
        bool updateLayout(QSqlQuery& query, const muuid::uuid& id, const NDCellDetails::Config::UpdateCellRecord& newCellInfo, const bool overrideOnCollision = false) {
            return NDCellDetails::Update::updateCell(query, id, newCellInfo, overrideOnCollision);
        }

        bool updateCellOrigin(const muuid::uuid& id, const NDCellDetails::Config::UpdateCellOriginRecord& newCellInfo, const bool overrideOnCollision = false) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDCellDetails::Update::updateCellOrigin(query, id, newCellInfo, overrideOnCollision);
                });
        }
        bool updateCellOrigin(QSqlQuery& query, const muuid::uuid& id, const NDCellDetails::Config::UpdateCellOriginRecord& newCellInfo, const bool overrideOnCollision = false) {
            return NDCellDetails::Update::updateCellOrigin(query, id, newCellInfo, overrideOnCollision);
        }

        // 4. Delete
        bool removeCell(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDCellDetails::Delete::removeCell(query, id);
                });
        }
        bool removeCell(QSqlQuery& query, const muuid::uuid& id) {
            return NDCellDetails::Delete::removeCell(query, id);
        }

        bool removeCellOrigin(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDCellDetails::Delete::removeCellOrigin(query, id);
                });
        }
        bool removeCellOrigin(QSqlQuery& query, const muuid::uuid& id) {
            return NDCellDetails::Delete::removeCellOrigin(query, id);
        }
    };
}