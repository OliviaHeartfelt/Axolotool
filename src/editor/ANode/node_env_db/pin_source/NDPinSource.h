/*
#include "../NDConcepts.h"

namespace NDPinSource {

    struct ComponentFriendTag {
        template<typename DBContext>
        static typename DBContext::StorageKey createKey() { return {}; }
    };

    template<NDConcepts::DatabaseProvider DBContext>
    class Component {
        DBContext* parent;

    public:
        explicit Component(DBContext* parentCtx) : parent(parentCtx) {}

        // --- Pin Source (Ecosystem) ---
        bool registerSource(const muuid::uuid& id, const QString& name);

        // --- Shared Definition Injectors ---
        bool createType(const muuid::uuid& id, const muuid::uuid& sourceId, const QString& name, std::size_t bitSize);
        bool createStyle(const muuid::uuid& id, const muuid::uuid& sourceId, const QString& name, const QColor& color, int thickness);
        bool createFlow(const muuid::uuid& id, const muuid::uuid& sourceId, const QString& name, double degree);

        // --- Lookups (Used by UI/Canvas to resolve IDs to display properties) ---
        std::optional<NDPinSourceDetails::Config::TypeRecord> getType(const muuid::uuid& id);
        std::optional<NDPinSourceDetails::Config::StyleRecord> getStyle(const muuid::uuid& id);
    };
}
*/