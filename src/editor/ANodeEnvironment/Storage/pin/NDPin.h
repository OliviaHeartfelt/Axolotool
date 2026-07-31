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

namespace NDPin {

    namespace Config {
        using namespace ::NDPinDetails::Config;
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

                if (currentTables.contains("pin_core",       Qt::CaseInsensitive) == value) list.append("pin_core");
                if (currentTables.contains("pin",            Qt::CaseInsensitive) == value) list.append("pin");
                if (currentTables.contains("pin_allow_flow", Qt::CaseInsensitive) == value) list.append("pin_allow_flow");
                if (currentTables.contains("pin_allow_type", Qt::CaseInsensitive) == value) list.append("pin_allow_type");
                return list;
            });
        }

        // 0. Init
        bool createAllTables() {
            return NDHelpers::useQuery(pool(), [](QSqlQuery& query) {
                return NDPinDetails::Init::createAllTables(query);
            });
        }
        bool createAllTables(QSqlQuery& query) {
            return NDPinDetails::Init::createAllTables(query);
        }

        // 1. Create
        bool createPinCore(const NDPinDetails::Config::CreatePinCoreRecord& newPinCore) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Create::createPinCore(query, newPinCore);
            });
        }
        bool createPinCore(QSqlQuery& query, const NDPinDetails::Config::CreatePinCoreRecord& newPinCore) {
            return NDPinDetails::Create::createPinCore(query, newPinCore);
        }
        bool createPin(const NDPinDetails::Config::CreatePinRecord& newPin) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Create::createPin(query, newPin);
                });
        }
        bool createPin(QSqlQuery& query, const NDPinDetails::Config::CreatePinRecord& newPin) {
            return NDPinDetails::Create::createPin(query, newPin);
        }

        bool createAllowFlows(const muuid::uuid& pinId, const QList<muuid::uuid>& allowedTypes) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Create::createAllowFlows(query, pinId, allowedTypes);
            });
        }
        bool createAllowFlows(QSqlQuery& query, const muuid::uuid& pinId, const QList<muuid::uuid>& allowedTypes) {
            return NDPinDetails::Create::createAllowFlows(query, pinId, allowedTypes);
        }

        bool createAllowTypes(const muuid::uuid& pinId, const QList<muuid::uuid>& allowedTypes) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Create::createAllowTypes(query, pinId, allowedTypes);
            });
        }
        bool createAllowTypes(QSqlQuery& query, const muuid::uuid& pinId, const QList<muuid::uuid>& allowedTypes) {
            return NDPinDetails::Create::createAllowTypes(query, pinId, allowedTypes);
        }

        // 2. Read
        std::optional<NDPinDetails::Config::CompletePinCore> getFullPinCore(const muuid::uuid& id, const bool continueAtFail = false) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) -> std::optional<NDPinDetails::Config::CompletePinCore> {
                return NDPinDetails::Read::getFullPinCore(query, id, continueAtFail);
            });
        }
        std::optional<NDPinDetails::Config::CompletePinCore> getFullPinCore(QSqlQuery& query, const muuid::uuid& id, const bool continueAtFail = false) {
            return NDPinDetails::Read::getFullPinCore(query, id, continueAtFail);
        }
        std::optional<NDPinDetails::Config::FullPinCoreRecord> getPinCore(const muuid::uuid& id, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Read::getPinCore(query, id);
                });
        }
        std::optional<NDPinDetails::Config::FullPinCoreRecord> getPinCore(QSqlQuery& query, const muuid::uuid& id, const bool continueAtFail = false) {
            return NDPinDetails::Read::getPinCore(query, id);
        }
        std::optional<QList<NDPinDetails::Config::FullPinCoreRecord>> getContributorPinCores(const muuid::uuid& id, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Read::getContributorPinCores(query, id, continueAtFail);
                });
        }
        std::optional<QList<NDPinDetails::Config::FullPinCoreRecord>> getPigetContributorPinCoresnCore(QSqlQuery& query, const muuid::uuid& id, const bool continueAtFail = false) {
            return NDPinDetails::Read::getContributorPinCores(query, id, continueAtFail);
        }
        std::optional<QList<NDPinDetails::Config::FullPinCoreRecord>> getAllPinCores(const muuid::uuid& id, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Read::getAllPinCores(query, id, continueAtFail);
                });
        }
        std::optional<QList<NDPinDetails::Config::FullPinCoreRecord>> getAllPinCores(QSqlQuery& query, const muuid::uuid& id, const bool continueAtFail = false) {
            return NDPinDetails::Read::getAllPinCores(query, id, continueAtFail);
        }

        std::optional<NDPinDetails::Config::FullPinRecord> getPin(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Read::getPin(query, id);
            });
        }
        std::optional<NDPinDetails::Config::FullPinRecord> getPin(QSqlQuery& query, const muuid::uuid& id) {
            return NDPinDetails::Read::getPin(query, id);
        }
        std::optional<QList<NDPinDetails::Config::FullPinRecord>> getCorePins(const muuid::uuid& id, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Read::getCorePins(query, id);
                });
        }
        std::optional<QList<NDPinDetails::Config::FullPinRecord>> getCorePins(QSqlQuery& query, const muuid::uuid& id, const bool continueAtFail = false) {
            return NDPinDetails::Read::getCorePins(query, id);
        }
        std::optional<QList<NDPinDetails::Config::FullPinRecord>> getContributorPins(const muuid::uuid& id, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Read::getContributorPins(query, id);
                });
        }
        std::optional<QList<NDPinDetails::Config::FullPinRecord>> getContributorPins(QSqlQuery& query, const muuid::uuid& id, const bool continueAtFail = false) {
            return NDPinDetails::Read::getContributorPins(query, id);
        }
        std::optional<QList<NDPinDetails::Config::FullPinRecord>> getAllPins(const muuid::uuid& id, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Read::getAllPins(query, id);
                });
        }
        std::optional<QList<NDPinDetails::Config::FullPinRecord>> getAllPins(QSqlQuery& query, const muuid::uuid& id, const bool continueAtFail = false) {
            return NDPinDetails::Read::getAllPins(query, id);
        }

        std::optional<QList<muuid::uuid>> getAllowFlows(const muuid::uuid& pinId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Read::getAllowFlows(query, pinId, continueAtFail);
            });
        }
        std::optional<QList<muuid::uuid>> getAllowFlows(QSqlQuery& query, const muuid::uuid& pinId, const bool continueAtFail = false) {
            return NDPinDetails::Read::getAllowFlows(query, pinId, continueAtFail);
        }

        std::optional<QList<muuid::uuid>> getAllowTypes(const muuid::uuid& pinId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Read::getAllowTypes(query, pinId, continueAtFail);
            });
        }
        std::optional<QList<muuid::uuid>> getAllowTypes(QSqlQuery& query, const muuid::uuid& pinId, const bool continueAtFail = false) {
            return NDPinDetails::Read::getAllowTypes(query, pinId, continueAtFail);
        }

        // 3. Update
        bool updatePinCore(const muuid::uuid& id, const NDPinDetails::Config::UpdatePinCoreRecord& updateProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Update::updatePinCore(query, id, updateProperties);
            });
        }
        bool updatePinCore(QSqlQuery& query, const muuid::uuid& id, const NDPinDetails::Config::UpdatePinCoreRecord& updateProperties) {
            return NDPinDetails::Update::updatePinCore(query, id, updateProperties);
        }

        // 4. Delete
        bool removePinCore(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Delete::removePinCore(query, id);
            });
        }
        bool removePinCore(QSqlQuery& query, const muuid::uuid& id) {
            return NDPinDetails::Delete::removePinCore(query, id);
        }
        bool removePin(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Delete::removePin(query, id);
                });
        }
        bool removePin(QSqlQuery& query, const muuid::uuid& id) {
            return NDPinDetails::Delete::removePin(query, id);
        }

        bool removeAllowFlow(const muuid::uuid& pinId) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Delete::removeAllowFlow(query, pinId);
            });
        }
        bool removeAllowFlow(QSqlQuery& query, const muuid::uuid& pinId) {
            return NDPinDetails::Delete::removeAllowFlow(query, pinId);
        }

        bool removeAllowFlow(const muuid::uuid& pinId, const muuid::uuid& flowId) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Delete::removeAllowFlow(query, pinId, flowId);
            });
        }
        bool removeAllowFlow(QSqlQuery& query, const muuid::uuid& pinId, const muuid::uuid& flowId) {
            return NDPinDetails::Delete::removeAllowFlow(query, pinId, flowId);
        }

        bool removeAllowType(const muuid::uuid& pinId) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Delete::removeAllowType(query, pinId);
            });
        }
        bool removeAllowType(QSqlQuery& query, const muuid::uuid& pinId) {
            return NDPinDetails::Delete::removeAllowType(query, pinId);
        }

        bool removeAllowType(const muuid::uuid& pinId, const muuid::uuid& typeId) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDPinDetails::Delete::removeAllowType(query, pinId, typeId);
            });
        }
        bool removeAllowType(QSqlQuery& query, const muuid::uuid& pinId, const muuid::uuid& typeId) {
            return NDPinDetails::Delete::removeAllowType(query, pinId, typeId);
        }
    };
}