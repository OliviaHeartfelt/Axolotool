#pragma once

#include "../../../Storage/ANodeEnvDB.h"
#include "Concepts.h"

namespace WVCellDetails::CellItem {

	class CellItem : public QGraphicsRectItem {

		muuid::uuid m_id;
		std::optional<muuid::uuid> m_nodeId = std::nullopt;
		std::optional<QString> m_name = std::nullopt;

		short m_row = -1;
		short m_col = -1;
		short m_rowSpan = 0;
		short m_colSpan = 0;

		std::optional<muuid::uuid> m_pinCoreId = std::nullopt;
		std::optional<muuid::uuid> m_widgetId = std::nullopt;

		bool m_is_new;
		bool m_is_drty = false;

	public:
		static std::optional<ANodeEnvDB::Config::Cell::FullCellRecord> toRecord(const CellItem& cell) {
			if (!cell.m_nodeId || !cell.isContentValid() || !cell.isTransformValid()) return std::nullopt;

			return ANodeEnvDB::Config::Cell::FullCellRecord{
				.id = cell.m_id,
				.nodeId = *cell.m_nodeId,
				.name = cell.m_name,
				.row = cell.m_row,
				.col = cell.m_col,
				.rowSpan = cell.m_rowSpan,
				.colSpan = cell.m_colSpan,
				.pinCoreId = cell.m_pinCoreId,
				.widgetId = cell.m_widgetId
			};
		}

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

		const muuid::uuid id() const { return m_id; }

		const std::optional<muuid::uuid> nodeId() const { return m_nodeId; }
		void nodeId(std::optional<muuid::uuid> newValue) { m_nodeId = newValue; }

		bool isUpdateNeeded() const { return m_is_drty; }
		void setUpdate(bool value) { m_is_drty = value; }

		bool isNew() const { return m_is_new; }
		void setIsNew(bool value) { m_is_new = value; }

		std::optional<QString> name() const { return m_name; }
		void name(std::optional<QString> newValue) { m_name = newValue; }

		std::tuple<short, short, short, short> cellTransforms() const {
			return { m_row, m_col, m_rowSpan, m_colSpan };
		}
		void cellTransforms(short row, short col, short rowSpan, short colSpan) {
			m_row = row;
			m_col = col;
			m_rowSpan = rowSpan;
			m_colSpan = colSpan;
		}

		std::optional<muuid::uuid> pinCoreId() const { return m_pinCoreId; }
		void pinCoreId(const std::optional<muuid::uuid>& newId) { m_pinCoreId = newId; }

		std::optional<muuid::uuid> widgetId() const { return m_widgetId; }
		void widgetId(const std::optional<muuid::uuid>& newId) { m_widgetId = newId; }

		bool isContentValid() const {
			return (static_cast<bool>(m_pinCoreId) + static_cast<bool>(m_widgetId) <= 1);
		}
		bool isTransformValid() const {
			return m_row >= 0 && m_col >=0 && m_rowSpan > 0 && m_colSpan > 0;
		}
	};
	static_assert(WVCellDetails::Concepts::CellItem<CellItem>);
}