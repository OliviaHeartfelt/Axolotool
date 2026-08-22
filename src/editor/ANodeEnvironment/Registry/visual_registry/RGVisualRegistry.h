#pragma once

#include "../registry_base/RGBaseRegistry.h"

namespace RGVisualRegistry {

	template<typename T>
	class VisualRegistry {

		RGBaseRegistry::BaseRegistry<muuid::uuid, T> m_visible;
		RGBaseRegistry::BaseRegistry<muuid::uuid, T> m_hidden;

	public:
		bool show(const muuid::uuid& id) {
			std::optional<T> hiddenItemOpt = m_hidden.at(id);
			if (!hiddenItemOpt) return false;

			m_visible.insert(id, *hiddenItemOpt);
			m_hidden.erase(id);
			return true;
		}
		bool hide(const muuid::uuid& id) {
			std::optional<T> visibleItemOpt = m_visible.at(id);
			if (!visibleItemOpt) return false;
			m_hidden.insert(id, *visibleItemOpt);
			m_visible.erase(id);
			return true;
		}

		[[nodiscard]] const RGBaseRegistry::BaseRegistry<muuid::uuid, T>& visible() const { return m_visible; }
		[[nodiscard]] const RGBaseRegistry::BaseRegistry<muuid::uuid, T>& hidden()  const { return m_hidden; }

		bool addVisible(const muuid::uuid& id, T item) { return m_visible.insert(id, item); }
		bool addHidden(const muuid::uuid& id, T item)  { return m_hidden.insert(id, item); }

		[[nodiscard]] std::optional<T> findVisible(const muuid::uuid& id) const { return m_visible.at(id); }
		[[nodiscard]] std::optional<T> findHidden(const muuid::uuid& id)  const { return m_hidden.at(id); }

		bool removeVisible(const muuid::uuid& id) { return m_visible.erase(id); }
		bool removeHidden(const muuid::uuid& id)  { return m_hidden.erase(id); }

		void clearVisible() {
			if (m_visible.empty()) return;
			m_visible.clear();
		}
		void clearVisible(QGraphicsScene* scene) {
			if (!scene || m_visible.empty()) return;
			for (auto& [id, item] : m_visible.unique_access()) {
				scene->removeItem(item);
				delete item;
			}
			m_visible.clear();
		}
		void clearHidden() {
			if (m_hidden.empty()) return;
			m_hidden.clear();
		}
		void clearHidden(QGraphicsScene* scene) {
			if (!scene || m_hidden.empty()) return;
			for (auto& [id, item] : m_hidden.unique_access()) {
				scene->removeItem(item);
				delete item;
			}
			m_hidden.clear();
		}
		void clear() {
			clearVisible();
			clearHidden();
		}
		void clear(QGraphicsScene* scene) {
			clearVisible(scene);
			clearHidden(scene);
		}

		[[nodiscard]] size_t sizeVisible() const { return m_visible.size(); }
		[[nodiscard]] size_t sizeHidden()  const { return m_hidden.size(); }
	};
}