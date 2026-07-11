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

    struct ComponentFriendTag {
        template<typename DBContext>
        static typename DBContext::StorageKey createKey() { return {}; }
    };

    template<NDConcepts::DatabaseProvider DBContext>
    class Component {
        DBContext* parent;

        NDPool::DatabasePool& pool() const { return parent->getPool(); }

    public:
        explicit Component(DBContext* parentCtx) : parent(parentCtx) {}

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

        template<NDConcepts::ByteConvertible Metadata>
        inline bool createWireStyle(const NDWireSourceDetails::Config::CreateWireStyleRecord<Metadata>& newStyle) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Create::createWireStyle(query, newStyle);
            });
        }
        template<NDConcepts::ByteConvertible Metadata>
        inline bool createWireStyle(QSqlQuery& query, const NDWireSourceDetails::Config::CreateWireStyleRecord<Metadata>& newStyle) {
            return NDWireSourceDetails::Create::createWireStyle(query, newStyle);
        }

        template<NDConcepts::ByteConvertible Data>
        inline bool createWireData(const NDWireSourceDetails::Config::CreateWireDataRecord<Data>& newData) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Create::createWireData(query, newData);
            });
        }
        template<NDConcepts::ByteConvertible Data>
        inline bool createWireData(QSqlQuery& query, const NDWireSourceDetails::Config::CreateWireDataRecord<Data>& newData) {
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

        template<NDConcepts::ByteConvertible Metadata>
        inline std::optional<NDWireSourceDetails::Config::FullWireStyleRecord<Metadata>> getWireStyle(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Read::getWireStyle<Metadata>(query, id);
            });
        }
        template<NDConcepts::ByteConvertible Metadata>
        inline std::optional<NDWireSourceDetails::Config::FullWireStyleRecord<Metadata>> getWireStyle(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireSourceDetails::Read::getWireStyle<Metadata>(query, id);
        }
        template<NDConcepts::ByteConvertible Metadata>
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireStyleRecord<Metadata>>> getContributorWireStyles(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Read::getContributorWireStyles<Metadata>(query, contributorId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible Metadata>
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireStyleRecord<Metadata>>> getContributorWireStyles(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWireSourceDetails::Read::getContributorWireStyles<Metadata>(query, contributorId, continueAtFail);
        }
        template<NDConcepts::ByteConvertible Metadata>
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireStyleRecord<Metadata>>> getAllWireStyles(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Read::getAllWireStyles<Metadata>(query, sourceId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible Metadata>
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireStyleRecord<Metadata>>> getAllWireStyles(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWireSourceDetails::Read::getAllWireStyles<Metadata>(query, sourceId, continueAtFail);
        }

        template<NDConcepts::ByteConvertible Data>
        inline std::optional<NDWireSourceDetails::Config::FullWireDataRecord<Data>> getWireData(const muuid::uuid& id) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Read::getWireData<Data>(query, id);
            });
        }
        template<NDConcepts::ByteConvertible Data>
        inline std::optional<NDWireSourceDetails::Config::FullWireDataRecord<Data>> getWireData(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireSourceDetails::Read::getWireData<Data>(query, id);
        }
        template<NDConcepts::ByteConvertible Data>
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireDataRecord<Data>>> getContributorWireData(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Read::getContributorWireData<Data>(query, contributorId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible Data>
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireDataRecord<Data>>> getContributorWireData(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWireSourceDetails::Read::getContributorWireData<Data>(query, contributorId, continueAtFail);
        }
        template<NDConcepts::ByteConvertible Data>
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireDataRecord<Data>>> getAllWireData(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Read::getAllWireData<Data>(query, sourceId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible Data>
        inline std::optional<QList<NDWireSourceDetails::Config::FullWireDataRecord<Data>>> getAllWireData(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWireSourceDetails::Read::getAllWireData<Data>(query, sourceId, continueAtFail);
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

        template<NDConcepts::ByteConvertible Metadata>
        inline bool updateWireStyle(muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireStyleRecord<Metadata>& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Update::updateWireStyle(query, id, newProperties);
            });
        }
        template<NDConcepts::ByteConvertible Metadata>
        inline bool updateWireStyle(QSqlQuery& query, muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireStyleRecord<Metadata>& newProperties) {
            return NDWireSourceDetails::Update::updateWireStyle(query, id, newProperties);
        }

        template<NDConcepts::ByteConvertible Data>
        inline bool updateWireData(muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireDataRecord<Data>& newProperties) {
            return NDHelpers::useTransaction(pool(), [&](QSqlQuery& query) {
                return NDWireSourceDetails::Update::updateWireData(query, id, newProperties);
            });
        }
        template<NDConcepts::ByteConvertible Data>
        inline bool updateWireData(QSqlQuery& query, muuid::uuid id, const NDWireSourceDetails::Config::UpdateWireDataRecord<Data>& newProperties) {
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