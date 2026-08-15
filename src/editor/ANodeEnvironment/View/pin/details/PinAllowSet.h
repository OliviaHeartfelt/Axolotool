#pragma once

#include "Concepts.h"

namespace VWPinDetails::PinAllowSet {

	class VWPinAllowSet {
		std::set<muuid::uuid> set;

	public:
		VWPinAllowSet() = default;

		using const_iterator = std::vector<muuid::uuid>::const_iterator;
		VWPinAllowSet(const_iterator begin, const_iterator end) : set(begin, end) {}


		bool insert(const muuid::uuid& id) {
			return set.insert(id).second;
		}
		bool insert(const std::vector<muuid::uuid>& idVec) {
			size_t initialSize = set.size();
			set.insert(idVec.begin(), idVec.end());
			return set.size() - initialSize;
		}
		bool insert(const QList<muuid::uuid>& idList) {
			size_t initialSize = set.size();
			set.insert(idList.begin(), idList.end());
			return set.size() - initialSize;
		}

		bool erase(const muuid::uuid& id) {
			return set.erase(id) > 0;
		}

		size_t erase_if(const std::function<bool(muuid::uuid)>& pred) {
			return std::erase_if(set, pred);
		}
		template<typename Pred>
		size_t erase_if(Pred&& pred) {
			return std::erase_if(set, std::forward<Pred>(pred));
		}

		bool contains(const muuid::uuid& id) const { return set.contains(id); }
		std::size_t size() const { return set.size(); }
		bool empty() const { return set.empty(); }
		void clear() { set.clear(); }
	};
	static_assert(Concepts::PinAllowSetConcept<VWPinAllowSet>);
}