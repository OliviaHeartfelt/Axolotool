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

namespace NDWire {

    namespace Config {
        using namespace ::NDWireDetails::Config;
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

                if (currentTables.contains("wire_core",      Qt::CaseInsensitive) == value) list.append("wire_core");
                if (currentTables.contains("wire_temporary", Qt::CaseInsensitive) == value) list.append("wire_temporary");
                if (currentTables.contains("wire_pins",      Qt::CaseInsensitive) == value) list.append("wire");
                return list;
            });
        }

        // 0. INIT
        bool createAllTables() {
            return NDHelpers::useTransaction(pool(), [](QSqlQuery& query) {
                return NDWireDetails::Init::createAllTables(query);
            });
        }
        bool createAllTables(QSqlQuery& query) {
            return NDWireDetails::Init::createAllTables(query);
        }

        // 1. Create - Wire Core
        bool createWireCore(const NDWireDetails::Config::CreateWireCoreRecord& newWireCore) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Create::createWireCore(query, newWireCore);
            });
        }
        bool createWireCore(QSqlQuery& query, const NDWireDetails::Config::CreateWireCoreRecord& newWireCore) {
            return NDWireDetails::Create::createWireCore(query, newWireCore);
        }

        // 1. Create - Wire Temporary
        bool createWireTemporary(const NDWireDetails::Config::CreateWireTemporaryRecord& newWireTemporary) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Create::createWireTemporary(query, newWireTemporary);
            });
        }
        bool createWireTemporary(QSqlQuery& query, const NDWireDetails::Config::CreateWireTemporaryRecord& newWireTemporary) {
            return NDWireDetails::Create::createWireTemporary(query, newWireTemporary);
        }

        // 1. Create - Wire
        bool createWire(const NDWireDetails::Config::CreateWireRecord& newWire) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Create::createWire(query, newWire);
            });
        }
        bool createWire(QSqlQuery& query, const NDWireDetails::Config::CreateWireRecord& newWire) {
            return NDWireDetails::Create::createWire(query, newWire);
        }

        // 2. Read - Wire Core
        std::optional<NDWireDetails::Config::FullWireCoreRecord> getWireCore(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getWireCore(query, id);
            });
        }
        std::optional<NDWireDetails::Config::FullWireCoreRecord> getWireCore(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireDetails::Read::getWireCore(query, id);
        }
        std::optional<QList<NDWireDetails::Config::FullWireCoreRecord>> getContributorWireCores(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getContributorWireCores(query, contributorId, continueAtFail);
            });
        }
        std::optional<QList<NDWireDetails::Config::FullWireCoreRecord>> getContributorWireCores(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWireDetails::Read::getContributorWireCores(query, contributorId, continueAtFail);
        }
        std::optional<QList<NDWireDetails::Config::FullWireCoreRecord>> getAllWireCores(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getAllWireCores(query, sourceId, continueAtFail);
            });
        }
        std::optional<QList<NDWireDetails::Config::FullWireCoreRecord>> getAllWireCores(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWireDetails::Read::getAllWireCores(query, sourceId, continueAtFail);
        }

        // 2. Read - Wire Temporary
        std::optional<NDWireDetails::Config::FullWireTemporaryRecord> getWireTemporary(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getWireTemporary(query, id);
            });
        }
        std::optional<NDWireDetails::Config::FullWireTemporaryRecord> getWireTemporary(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireDetails::Read::getWireTemporary(query, id);
        }
        std::optional<QList<NDWireDetails::Config::FullWireTemporaryRecord>> getContributorWireTemporary(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getContributorWireTemporary(query, contributorId, continueAtFail);
            });
        }
        std::optional<QList<NDWireDetails::Config::FullWireTemporaryRecord>> getContributorWireTemporary(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWireDetails::Read::getContributorWireTemporary(query, contributorId, continueAtFail);
        }
        std::optional<QList<NDWireDetails::Config::FullWireTemporaryRecord>> getAllWireTemporary(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getAllWireTemporary(query, sourceId, continueAtFail);
            });
        }
        std::optional<QList<NDWireDetails::Config::FullWireTemporaryRecord>> getAllWireTemporary(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWireDetails::Read::getAllWireTemporary(query, sourceId, continueAtFail);
        }

        // 2. Read - Wire
        std::optional<NDWireDetails::Config::FullWireRecord> getWire(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getWire(query, id);
            });
        }
        std::optional<NDWireDetails::Config::FullWireRecord> getWire(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireDetails::Read::getWire(query, id);
        }
        std::optional<QList<NDWireDetails::Config::FullWireRecord>> getContributorWires(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getContributorWires(query, contributorId, continueAtFail);
                });
        }
        std::optional<QList<NDWireDetails::Config::FullWireRecord>> getContributorWires(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWireDetails::Read::getContributorWires(query, contributorId, continueAtFail);
        }
        std::optional<QList<NDWireDetails::Config::FullWireRecord>> getAllWires(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getAllWires(query, sourceId, continueAtFail);
                });
        }
        std::optional<QList<NDWireDetails::Config::FullWireRecord>> getAllWires(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWireDetails::Read::getAllWires(query, sourceId, continueAtFail);
        }
        inline std::optional<QList<NDWireDetails::Config::FullWireRecord>> getWiresInView(const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getWiresInView(query, continueAtFail);
            });
        }
        inline std::optional<QList<NDWireDetails::Config::FullWireRecord>> getWiresInView(QSqlQuery& query, const bool continueAtFail = false) {
            return NDWireDetails::Read::getWiresInView(query, continueAtFail);
        }

        // 3. Update - Wire Core
        bool updateWireCore(muuid::uuid id, const NDWireDetails::Config::UpdateWireCoreRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Update::updateWireCore(query, id, newProperties);
            });
        }
        bool updateWireCore(QSqlQuery& query, muuid::uuid id, const NDWireDetails::Config::UpdateWireCoreRecord& newProperties) {
            return NDWireDetails::Update::updateWireCore(query, id, newProperties);
        }

        // 3. Update - Wire
        bool updateWire(muuid::uuid id, const NDWireDetails::Config::UpdateWireRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Update::updateWire(query, id, newProperties);
                });
        }
        bool updateWire(QSqlQuery& query, muuid::uuid id, const NDWireDetails::Config::UpdateWireRecord& newProperties) {
            return NDWireDetails::Update::updateWire(query, id, newProperties);
        }

        
        // 4. Delete - Wire Core
        bool removeWireCore(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Delete::removeWireCore(query, id);
            });
        }
        bool removeWireCore(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireDetails::Delete::removeWireCore(query, id);
        }

        // 4. Delete - Wire Temporary
        bool removeWireTemporary(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Delete::removeWireTemporary(query, id);
            });
        }
        bool removeWireTemporary(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireDetails::Delete::removeWireTemporary(query, id);
        }

        // 4. Delete - Wire
        bool removeWire(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireDetails::Delete::removeWire(query, id);
            });
        }
        bool removeWire(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireDetails::Delete::removeWire(query, id);
        }
    };
}