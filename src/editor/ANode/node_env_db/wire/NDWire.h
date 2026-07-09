#pragma once

#include "details/Config.h"
#include "details/Init.h"
#include "details/Create.h"
#include "details/Read.h"
#include "details/Update.h"
#include "details/Delete.h"

#include "../NDConcepts.h"
#include "../NDHelpers.h"

namespace NDWire {

    namespace Config {
        using namespace ::NDWireDetails::Config;
    }

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

            if (currentTables.contains("wire_core",      Qt::CaseInsensitive) == value) list.append("wire_core");
            if (currentTables.contains("wire_temporary", Qt::CaseInsensitive) == value) list.append("wire_temporary");
            if (currentTables.contains("wire_pins",      Qt::CaseInsensitive) == value) list.append("wire_pins");
            if (currentTables.contains("wire_widgets",   Qt::CaseInsensitive) == value) list.append("wire_widgets");
            if (currentTables.contains("wire_arbitrary", Qt::CaseInsensitive) == value) list.append("wire_arbitrary");
            return list;
        }

        // 0. INIT
        bool createAllTables() {
            return NDHelpers::useTransaction(database(), [](QSqlQuery& query) {
                return NDWireDetails::Init::createAllTables(query);
            });
        }

        // 1. Create - Wire Core
        inline bool createWireCore(const NDWireDetails::Config::CreateWireCoreRecord& newWireCore) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Create::createWireCore(query, newWireCore);
            });
        }
        inline bool createWireCore(QSqlQuery& query, const NDWireDetails::Config::CreateWireCoreRecord& newWireCore) {
            return NDWireDetails::Create::createWireCore(query, newWireCore);
        }

        // 1. Create - Wire Temporary
        inline bool createWireTemporary(const NDWireDetails::Config::CreateWireTemporaryRecord& newWireTemporary) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Create::createWireTemporary(query, newWireTemporary);
            });
        }
        inline bool createWireTemporary(QSqlQuery& query, const NDWireDetails::Config::CreateWireTemporaryRecord& newWireTemporary) {
            return NDWireDetails::Create::createWireTemporary(query, newWireTemporary);
        }

        // 1. Create - Wire Pins
        template<NDConcepts::ByteConvertible State>
        inline bool createWirePins(const NDWireDetails::Config::CreateWirePinRecord<State>& newWirePins) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Create::createWirePins(query, newWirePins);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline bool createWirePins(QSqlQuery& query, const NDWireDetails::Config::CreateWirePinRecord<State>& newWirePins) {
            return NDWireDetails::Create::createWirePins(query, newWirePins);
        }

        // 1. Create - Wire Widgets
        template<NDConcepts::ByteConvertible State>
        inline bool createWireWidgets(const NDWireDetails::Config::CreateWireWidgetsRecord<State>& newWireWidgets) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Create::createWireWidgets(query, newWireWidgets);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline bool createWireWidgets(QSqlQuery& query, const NDWireDetails::Config::CreateWireWidgetsRecord<State>& newWireWidgets) {
            return NDWireDetails::Create::createWireWidgets(query, newWireWidgets);
        }

        // 1. Create - Wire Arbitrary
        template<NDConcepts::ByteConvertible State>
        inline bool createWireArbitrary(const NDWireDetails::Config::CreateWireArbitraryRecord<State>& newWireArbitrary) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Create::createWireArbitrary(query, newWireArbitrary);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline bool createWireArbitrary(QSqlQuery& query, const NDWireDetails::Config::CreateWireArbitraryRecord<State>& newWireArbitrary) {
            return NDWireDetails::Create::createWireArbitrary(query, newWireArbitrary);
        }

        // 2. Read - Wire Core
        inline std::optional<NDWireDetails::Config::FullWireCoreRecord> getWireCore(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getWireCore(query, id);
            });
        }
        inline std::optional<NDWireDetails::Config::FullWireCoreRecord> getWireCore(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireDetails::Read::getWireCore(query, id);
        }
        inline std::optional<QList<NDWireDetails::Config::FullWireCoreRecord>> getContributorWireCores(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getContributorWireCores(query, contributorId, continueAtFail);
            });
        }
        inline std::optional<QList<NDWireDetails::Config::FullWireCoreRecord>> getContributorWireCores(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWireDetails::Read::getContributorWireCores(query, contributorId, continueAtFail);
        }
        inline std::optional<QList<NDWireDetails::Config::FullWireCoreRecord>> getAllWireCores(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getAllWireCores(query, sourceId, continueAtFail);
            });
        }
        inline std::optional<QList<NDWireDetails::Config::FullWireCoreRecord>> getAllWireCores(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWireDetails::Read::getAllWireCores(query, sourceId, continueAtFail);
        }

        // 2. Read - Wire Temporary
        inline std::optional<NDWireDetails::Config::FullWireTemporaryRecord> getWireTemporary(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getWireTemporary(query, id);
            });
        }
        inline std::optional<NDWireDetails::Config::FullWireTemporaryRecord> getWireTemporary(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireDetails::Read::getWireTemporary(query, id);
        }
        inline std::optional<QList<NDWireDetails::Config::FullWireTemporaryRecord>> getContributorWireTemporary(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getContributorWireTemporary(query, contributorId, continueAtFail);
            });
        }
        inline std::optional<QList<NDWireDetails::Config::FullWireTemporaryRecord>> getContributorWireTemporary(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWireDetails::Read::getContributorWireTemporary(query, contributorId, continueAtFail);
        }
        inline std::optional<QList<NDWireDetails::Config::FullWireTemporaryRecord>> getAllWireTemporary(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getAllWireTemporary(query, sourceId, continueAtFail);
            });
        }
        inline std::optional<QList<NDWireDetails::Config::FullWireTemporaryRecord>> getAllWireTemporary(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWireDetails::Read::getAllWireTemporary(query, sourceId, continueAtFail);
        }

        // 2. Read - Wire Pins
        template<NDConcepts::ByteConvertible State>
        inline std::optional<NDWireDetails::Config::FullWirePinRecord<State>> getWirePins(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getWirePins(query, id);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<NDWireDetails::Config::FullWirePinRecord<State>> getWirePins(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireDetails::Read::getWirePins(query, id);
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<QList<NDWireDetails::Config::FullWirePinRecord<State>>> getContributorWirePins(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getContributorWirePins(query, contributorId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<QList<NDWireDetails::Config::FullWirePinRecord<State>>> getContributorWirePins(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWireDetails::Read::getContributorWirePins(query, contributorId, continueAtFail);
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<QList<NDWireDetails::Config::FullWirePinRecord<State>>> getAllWirePins(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getAllWirePins(query, sourceId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<QList<NDWireDetails::Config::FullWirePinRecord<State>>> getAllWirePins(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWireDetails::Read::getAllWirePins(query, sourceId, continueAtFail);
        }
        // 2. Read - Wire Widget
        template<NDConcepts::ByteConvertible State>
        inline std::optional<NDWireDetails::Config::FullWireWidgetsRecord<State>> getWireWidgets(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getWireWidgets(query, id);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<NDWireDetails::Config::FullWireWidgetsRecord<State>> getWireWidgets(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireDetails::Read::getWireWidgets(query, id);
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<QList<NDWireDetails::Config::FullWireWidgetsRecord<State>>> getContributorWireWidgets(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getContributorWireWidgets(query, contributorId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<QList<NDWireDetails::Config::FullWireWidgetsRecord<State>>> getContributorWireWidgets(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWireDetails::Read::getContributorWireWidgets(query, contributorId, continueAtFail);
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<QList<NDWireDetails::Config::FullWireWidgetsRecord<State>>> getAllWireWidgets(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getAllWireWidgets(query, sourceId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<QList<NDWireDetails::Config::FullWireWidgetsRecord<State>>> getAllWireWidgets(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWireDetails::Read::getAllWireWidgets(query, sourceId, continueAtFail);
        }

        // 2. Read - Wire Arbitrary
        template<NDConcepts::ByteConvertible State>
        inline std::optional<NDWireDetails::Config::FullWireArbitraryRecord<State>> getWireArbitrary(const muuid::uuid& id) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getWireArbitrary(query, id);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<NDWireDetails::Config::FullWireArbitraryRecord<State>> getWireArbitrary(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireDetails::Read::getWireArbitrary(query, id);
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<QList<NDWireDetails::Config::FullWireArbitraryRecord<State>>> getContributorWireArbitrary(const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getContributorWireArbitrary(query, contributorId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<QList<NDWireDetails::Config::FullWireArbitraryRecord<State>>> getContributorWireArbitrary(QSqlQuery& query, const muuid::uuid& contributorId, const bool continueAtFail = false) {
            return NDWireDetails::Read::getContributorWireArbitrary(query, contributorId, continueAtFail);
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<QList<NDWireDetails::Config::FullWireArbitraryRecord<State>>> getAllWireArbitrary(const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDHelpers::useQuery(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Read::getAllWireArbitrary(query, sourceId, continueAtFail);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline std::optional<QList<NDWireDetails::Config::FullWireArbitraryRecord<State>>> getAllWireArbitrary(QSqlQuery& query, const muuid::uuid& sourceId, const bool continueAtFail = false) {
            return NDWireDetails::Read::getAllWireArbitrary(query, sourceId, continueAtFail);
        }

        // 3. Update - Wire Core
        inline bool updateWireCore(muuid::uuid id, const NDWireDetails::Config::UpdateWireCoreRecord& newProperties) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Update::updateWireCore(query, id, newProperties);
            });
        }
        inline bool updateWireCore(QSqlQuery& query, muuid::uuid id, const NDWireDetails::Config::UpdateWireCoreRecord& newProperties) {
            return NDWireDetails::Update::updateWireCore(query, id, newProperties);
        }

        // 3. Update - Wire Pins
        template<NDConcepts::ByteConvertible State>
        inline bool updateWirePins(muuid::uuid id, const NDWireDetails::Config::UpdateWirePinRecord<State>& newProperties) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Update::updateWirePins(query, id, newProperties);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline bool updateWirePins(QSqlQuery& query, muuid::uuid id, const NDWireDetails::Config::UpdateWirePinRecord<State>& newProperties) {
            return NDWireDetails::Update::updateWirePins(query, id, newProperties);
        }

        // 3. Update - Wire Widget
        template<NDConcepts::ByteConvertible State>
        inline bool updateWireWidgets(muuid::uuid id, const NDWireDetails::Config::UpdateWireWidgetsRecord<State>& newProperties) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Update::updateWireWidgets(query, id, newProperties);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline bool updateWireWidgets(QSqlQuery& query, muuid::uuid id, const NDWireDetails::Config::UpdateWireWidgetsRecord<State>& newProperties) {
            return NDWireDetails::Update::updateWireWidgets(query, id, newProperties);
        }

        // 3. Update - Wire Arbitrary
        template<NDConcepts::ByteConvertible State>
        inline bool updateWireArbitrary(muuid::uuid id, const NDWireDetails::Config::UpdateWireArbitraryRecord<State>& newProperties) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Update::updateWireArbitrary(query, id, newProperties);
            });
        }
        template<NDConcepts::ByteConvertible State>
        inline bool updateWireArbitrary(QSqlQuery& query, muuid::uuid id, const NDWireDetails::Config::UpdateWireArbitraryRecord<State>& newProperties) {
            return NDWireDetails::Update::updateWireArbitrary(query, id, newProperties);
        }

        
        // 4. Delete - Wire Core
        inline bool removeWireCore(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Delete::removeWireCore(query, id);
            });
        }
        inline bool removeWireCore(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireDetails::Delete::removeWireCore(query, id);
        }

        // 4. Delete - Wire Temporary
        inline bool removeWireTemporary(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Delete::removeWireTemporary(query, id);
            });
        }
        inline bool removeWireTemporary(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireDetails::Delete::removeWireTemporary(query, id);
        }

        // 4. Delete - Wire Pins
        inline bool removeWirePins(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Delete::removeWirePins(query, id);
            });
        }
        inline bool removeWirePins(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireDetails::Delete::removeWirePins(query, id);
        }

        // 4. Delete - Wire Widget
        inline bool removeWireWidgets(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Delete::removeWireWidgets(query, id);
            });
        }
        inline bool removeWireWidgets(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireDetails::Delete::removeWireWidgets(query, id);
        }

        // 4. Delete - Wire Arbitrary
        inline bool removeWireArbitrary(const muuid::uuid& id) {
            return NDHelpers::useTransaction(database(), [&](QSqlQuery& query) {
                return NDWireDetails::Delete::removeWireArbitrary(query, id);
            });
        }
        inline bool removeWireArbitrary(QSqlQuery& query, const muuid::uuid& id) {
            return NDWireDetails::Delete::removeWireArbitrary(query, id);
        }
    };
}