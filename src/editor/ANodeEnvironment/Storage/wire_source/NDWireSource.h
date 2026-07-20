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

namespace NDWireSource {

    namespace Config {
        using namespace ::NDWireSourceDetails::Config;
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

                if (currentTables.contains("wire_source",      Qt::CaseInsensitive) == value) list.append("wire_source");
                if (currentTables.contains("wire_contributor", Qt::CaseInsensitive) == value) list.append("wire_contributor");
                if (currentTables.contains("wire_style",       Qt::CaseInsensitive) == value) list.append("wire_style");
                if (currentTables.contains("wire_data",        Qt::CaseInsensitive) == value) list.append("wire_data");
                return list;
            });
        }

        // 0. INIT
        bool createAllTables() {
            return NDHelpers::useTransaction(pool(), [](QSqlQuery& query) {
                return NDWireSourceDetails::Init::createAllTables(query);
            });
        }
        bool createAllTables(QSqlQuery& query) {
            return NDWireSourceDetails::Init::createAllTables(query);
        }

        // 1. CREATE
        inline bool createWireSource(const NDWireSourceDetails::Config::CreateWireSourceRecord& newSource) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Create::createWireSource(query, newSource);
            });
        }
        inline bool createWireSource(QSqlQuery& query, const NDWireSourceDetails::Config::CreateWireSourceRecord& newSource) {
            return NDWireSourceDetails::Create::createWireSource(query, newSource);
        }

        inline bool createWireContributor(const NDWireSourceDetails::Config::CreateWireContributorRecord& newContributor) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Create::createWireContributor(query, newContributor);
            });
        }
        inline bool createWireContributor(QSqlQuery& query, const NDWireSourceDetails::Config::CreateWireContributorRecord& newContributor) {
            return NDWireSourceDetails::Create::createWireContributor(query, newContributor);
        }

        inline bool createWireStyle(const NDWireSourceDetails::Config::CreateWireStyleRecord& newStyle) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Create::createWireStyle(query, newStyle);
            });
        }
        inline bool createWireStyle(QSqlQuery& query, const NDWireSourceDetails::Config::CreateWireStyleRecord& newStyle) {
            return NDWireSourceDetails::Create::createWireStyle(query, newStyle);
        }

        inline bool createWireData(const NDWireSourceDetails::Config::CreateWireDataRecord& newData) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Create::createWireData(query, newData);
            });
        }
        inline bool createWireData(QSqlQuery& query, const NDWireSourceDetails::Config::CreateWireDataRecord& newData) {
            return NDWireSourceDetails::Create::createWireData(query, newData);
        }

        // 2. READ
        inline std::optional<NDWireSourceDetails::Config::FullWireSourceRecord> getWireSource(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Read::getWireSource(query, id);
            });
        }
        inline std::optional<NDWireSourceDetails::Config::FullWireSourceRecord> getWireSource(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireSourceDetails::Read::getWireSource(query, id);
        }
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireSourceRecord>> getAllWireSources(const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Read::getAllWireSources(query, continueAtFail);
            });
        }
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireSourceRecord>> getAllWireSources(QSqlQuery& query, const bool continueAtFail = false) {
            return NDWireSourceDetails::Read::getAllWireSources(query, continueAtFail);
        }

        inline std::optional<NDWireSourceDetails::Config::FullWireContributorRecord> getWireContributor(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Read::getWireContributor(query, id);
            });
        }
        inline std::optional<NDWireSourceDetails::Config::FullWireContributorRecord> getWireContributor(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireSourceDetails::Read::getWireContributor(query, id);
        }
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireContributorRecord>> getAllWireContributors(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Read::getAllWireContributors(query, sourceId, continueAtFail);
            });
        }
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireContributorRecord>> getAllWireContributors(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWireSourceDetails::Read::getAllWireContributors(query, sourceId, continueAtFail);
        }

        inline std::optional<NDWireSourceDetails::Config::FullWireStyleRecord> getWireStyle(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Read::getWireStyle(query, id);
            });
        }
        inline std::optional<NDWireSourceDetails::Config::FullWireStyleRecord> getWireStyle(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireSourceDetails::Read::getWireStyle(query, id);
        }
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireStyleRecord>> getContributorWireStyles(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Read::getContributorWireStyles(query, contributorId, continueAtFail);
            });
        }
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireStyleRecord>> getContributorWireStyles(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWireSourceDetails::Read::getContributorWireStyles(query, contributorId, continueAtFail);
        }
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireStyleRecord>> getAllWireStyles(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Read::getAllWireStyles(query, sourceId, continueAtFail);
            });
        }
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireStyleRecord>> getAllWireStyles(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWireSourceDetails::Read::getAllWireStyles(query, sourceId, continueAtFail);
        }

        inline std::optional<NDWireSourceDetails::Config::FullWireDataRecord> getWireData(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Read::getWireData(query, id);
            });
        }
        inline std::optional<NDWireSourceDetails::Config::FullWireDataRecord> getWireData(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireSourceDetails::Read::getWireData(query, id);
        }
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireDataRecord>> getContributorWireData(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Read::getContributorWireData(query, contributorId, continueAtFail);
            });
        }
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireDataRecord>> getContributorWireData(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWireSourceDetails::Read::getContributorWireData(query, contributorId, continueAtFail);
        }
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireDataRecord>> getAllWireData(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Read::getAllWireData(query, sourceId, continueAtFail);
            });
        }
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireDataRecord>> getAllWireData(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWireSourceDetails::Read::getAllWireData(query, sourceId, continueAtFail);
        }

        // 3. UPDATE
        inline bool updateWireSource(muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireSourceRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Update::updateWireSource(query, id, newProperties);
            });
        }
        inline bool updateWireSource(QSqlQuery& query, muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireSourceRecord& newProperties) {
            return NDWireSourceDetails::Update::updateWireSource(query, id, newProperties);
        }

        inline bool updateWireContributor(muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireContributorRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Update::updateWireContributor(query, id, newProperties);
            });
        }
        inline bool updateWireContributor(QSqlQuery& query, muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireContributorRecord& newProperties) {
            return NDWireSourceDetails::Update::updateWireContributor(query, id, newProperties);
        }

        inline bool updateWireStyle(muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireStyleRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Update::updateWireStyle(query, id, newProperties);
            });
        }
        inline bool updateWireStyle(QSqlQuery& query, muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireStyleRecord& newProperties) {
            return NDWireSourceDetails::Update::updateWireStyle(query, id, newProperties);
        }

        inline bool updateWireData(muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireDataRecord& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Update::updateWireData(query, id, newProperties);
            });
        }
        inline bool updateWireData(QSqlQuery& query, muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireDataRecord& newProperties) {
            return NDWireSourceDetails::Update::updateWireData(query, id, newProperties);
        }

        // 4. DELETE
        inline bool removeWireSource(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Delete::removeWireSource(query, id);
            });
        }
        inline bool removeWireSource(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireSourceDetails::Delete::removeWireSource(query, id);
        }

        inline bool removeWireContributor(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Delete::removeWireContributor(query, id);
            });
        }
        inline bool removeWireContributor(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireSourceDetails::Delete::removeWireContributor(query, id);
        }

        inline bool removeWireStyle(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Delete::removeWireStyle(query, id);
            });
        }
        inline bool removeWireStyle(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireSourceDetails::Delete::removeWireStyle(query, id);
        }

        inline bool removeWireData(const muuid::uuid& id) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Delete::removeWireData(query, id);
            });
        }
        inline bool removeWireData(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireSourceDetails::Delete::removeWireData(query, id);
        }
    };
}