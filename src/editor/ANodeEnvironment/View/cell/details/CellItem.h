#pragma once

namespace WVCellDetails::CellItem {

	class CellItem : public QGraphicsRectItem {

		muuid::uuid m_id;

		bool m_is_new;
		bool m_is_drty = false;

	public:
		CellItem(QGraphicsItem* parent, const std::optional<muuid::uuid>& id = std::nullopt) : QGraphicsRectItem(parent) {
			if (id) {
				m_id = *id;
				m_is_new = false;
			}
			else {
				m_id = muuid::uuid::generate_unix_time_based();
				m_is_new = true;
			}

			setPen(Qt::NoPen);
		}

		const muuid::uuid& id() const { return m_id; }

		bool isUpdateNeeded() const { return m_is_drty; }
		void setUpdate(bool value) { m_is_drty = value; }

		bool isNew() const { return m_is_new; }
		void setIsNew(bool value) { m_is_new = value; }
	};
}