#pragma once

namespace VWNodeDetails::NodeGrid {

    struct GridCell {
        QGraphicsItem* item = nullptr;
        int row = -1;
        int col = -1;
        int rowSpan = 1;
        int colSpan = 1;
        Qt::Alignment alignment = Qt::Alignment();
    };

    class Grid {
        QGraphicsRectItem* parentItem = nullptr;
        std::vector<GridCell> cells{};

        int totalRows = 1;
        int totalCols = 1;
        qreal margin = 6.0;
        qreal spacing = 4.0;

        bool isDirty = true;
        bool m_isUpdateNeeded = false;

        static QSizeF getItemSize(const QGraphicsItem* item) {
            if (!item) return { 0, 0 };
            if (const auto* rectItem = dynamic_cast<const QGraphicsRectItem*>(item)) {
                const QRectF r = rectItem->rect();
                if (r.width() > 0 && r.height() > 0) return r.size();
            }
            return item->boundingRect().size();
        }

        void destroyItem(QGraphicsItem* item) {
            if (!item) return;
            delete item;
        }

        void refreshLayout() {
            if (!isDirty || !parentItem) return;

            std::vector<qreal> rowHeights(totalRows, 20.0);
            std::vector<qreal> colWidths(totalCols, 80.0);

            for (const auto& cell : cells) {
                if (!cell.item) continue;

                QSizeF sz = getItemSize(cell.item);

                if (cell.colSpan == 1 && cell.col >= 0 && cell.col < totalCols) {
                    colWidths[cell.col] = std::max(colWidths[cell.col], sz.width());
                }
                if (cell.rowSpan == 1 && cell.row >= 0 && cell.row < totalRows) {
                    rowHeights[cell.row] = std::max(rowHeights[cell.row], sz.height());
                }
            }

            qreal totalContentWidth = std::accumulate(colWidths.begin(), colWidths.end(), 0.0)
                + (margin * 2)
                + ((totalCols - 1) * spacing);

            if (parentItem->rect().width() > totalContentWidth) {
                qreal extraWidthPerCol = (parentItem->rect().width() - totalContentWidth) / totalCols;
                for (auto& w : colWidths) w += extraWidthPerCol;
            }

            std::vector<qreal> colX(totalCols, margin);
            for (int c = 1; c < totalCols; ++c) {
                colX[c] = colX[c - 1] + colWidths[c - 1] + spacing;
            }

            std::vector<qreal> rowY(totalRows, margin);
            for (int r = 1; r < totalRows; ++r) {
                rowY[r] = rowY[r - 1] + rowHeights[r - 1] + spacing;
            }

            for (const auto& cell : cells) {
                if (!cell.item || cell.col < 0 || cell.row < 0 || cell.col >= totalCols || cell.row >= totalRows) continue;

                qreal x = colX[cell.col];
                qreal y = rowY[cell.row];

                qreal w = 0.0;
                for (int c = cell.col; c < cell.col + cell.colSpan && c < totalCols; ++c) {
                    w += colWidths[c];
                }
                w += (cell.colSpan - 1) * spacing;

                qreal h = 0.0;
                for (int r = cell.row; r < cell.row + cell.rowSpan && r < totalRows; ++r) {
                    h += rowHeights[r];
                }
                h += (cell.rowSpan - 1) * spacing;

                cell.item->setPos(x, y);

                if (auto* rectItem = dynamic_cast<QGraphicsRectItem*>(cell.item)) {
                    rectItem->setRect(0, 0, w, h);
                }
            }

            qreal finalGridWidth = colX.back() + colWidths.back() + margin;
            qreal finalGridHeight = rowY.back() + rowHeights.back() + margin;

            parentItem->setRect(0, 0, std::max(parentItem->rect().width(), finalGridWidth), finalGridHeight);

            isDirty = false;
            m_isUpdateNeeded = true;
        }

    public:
        Grid(QGraphicsRectItem* parent, int rows = 1, int cols = 1, qreal _margin = 6.0, qreal _spacing = 4.0)
            : parentItem(parent), totalRows(std::max(1, rows)), totalCols(std::max(1, cols)), margin(_margin), spacing(_spacing) {
        }

        ~Grid() = default;

        int rowNum() const { return totalRows; }
        int colNum() const { return totalCols; }

        bool isUpdateNeeded() const { return m_isUpdateNeeded; }
        void isUpdateNeeded(bool newValue) { m_isUpdateNeeded = newValue; }

        void initGrid(const int rows, const int cols, const bool doRefresh = true) {
            deleteGrid(doRefresh);
            totalRows = std::max(1, rows);
            totalCols = std::max(1, cols);
        }

        void setDimensions(const int rows, const int cols, const bool doRefresh = true) {
            totalRows = std::max(1, rows);
            totalCols = std::max(1, cols);
            isDirty = true;
            if (doRefresh) refreshLayout();
        }

        void refresh() {
            isDirty = true;
            refreshLayout();
        }

        bool isCellValid(const GridCell& slot) const {
            return slot.item && isCellValid(slot.row, slot.col, slot.rowSpan, slot.colSpan);
        }
        bool isCellValid(QGraphicsItem* item, const int row, const int col, const int rowSpan = 1, const int colSpan = 1) const {
            return item && isCellValid(row, col, rowSpan, colSpan);
        }
        bool isCellValid(const int row, const int col, const int rowSpan = 1, const int colSpan = 1) const {
            return row >= 0 && col >= 0 && rowSpan >= 1 && colSpan >= 1;
        }
        bool isIndexValid(const int index) const {
            return index >= 0 && index < static_cast<int>(cells.size());
        }

        bool existsCell(const int row, const int col) const {
            return std::any_of(cells.begin(), cells.end(), [row, col](const GridCell& c) {
                return c.row == row && c.col == col;
                });
        }

        bool isOverlappingCell(const int row, const int col, int rowSpan = 1, int colSpan = 1) const {
            return std::any_of(cells.begin(), cells.end(), [row, col, rowSpan, colSpan](const GridCell& cell) {
                bool rowOverlap = std::max(cell.row, row) < std::min(cell.row + cell.rowSpan, row + rowSpan);
                bool colOverlap = std::max(cell.col, col) < std::min(cell.col + cell.colSpan, col + colSpan);
                return rowOverlap && colOverlap;
                });
        }

        std::optional<GridCell> cellAt(const int row, const int col) const {
            if (!isCellValid(row, col)) return std::nullopt;
            for (const auto& c : cells) {
                if (c.row == row && c.col == col) return c;
            }
            return std::nullopt;
        }

        std::optional<GridCell> cellAt(const int index) const {
            if (!isIndexValid(index)) return std::nullopt;
            return cells.at(index);
        }

        QGraphicsItem* cellItem(const int row, const int col) const {
            auto cellOpt = cellAt(row, col);
            return cellOpt ? cellOpt->item : nullptr;
        }

        QGraphicsItem* cellItem(const int index) const {
            return isIndexValid(index) ? cells.at(index).item : nullptr;
        }

        const std::vector<GridCell>& getGridRef() const { return cells; }

        bool addItem(QGraphicsItem* item, int row, int col, int rowSpan = 1, int colSpan = 1, const bool doRefresh = true, const bool overrideOnCollision = false) {
            if (!parentItem || !isCellValid(item, row, col, rowSpan, colSpan)) return false;

            if (isOverlappingCell(row, col, rowSpan, colSpan)) {
                if (overrideOnCollision) {
                    deleteOverlappingCells(row, col, rowSpan, colSpan, false);
                }
                else {
                    return false;
                }
            }

            totalRows = std::max(totalRows, row + rowSpan);
            totalCols = std::max(totalCols, col + colSpan);

            item->setParentItem(parentItem);
            cells.push_back(GridCell{ item, row, col, rowSpan, colSpan });

            isDirty = true;
            if (doRefresh) refreshLayout();
            return true;
        }

        void deleteOverlappingCells(int row, int col, int rowSpan = 1, int colSpan = 1, bool doRefresh = true) {
            auto it = std::remove_if(cells.begin(), cells.end(), [this, row, col, rowSpan, colSpan](GridCell& c) {
                bool rowOverlap = std::max(c.row, row) < std::min(c.row + c.rowSpan, row + rowSpan);
                bool colOverlap = std::max(c.col, col) < std::min(c.col + c.colSpan, col + colSpan);
                if (rowOverlap && colOverlap) {
                    destroyItem(c.item);
                    return true;
                }
                return false;
                });

            if (it != cells.end()) {
                cells.erase(it, cells.end());
                isDirty = true;
                if (doRefresh) refreshLayout();
            }
        }

        void deleteGrid(const bool doRefresh = true) {
            for (auto& c : cells) {
                if (c.item) destroyItem(c.item);
            }
            cells.clear();
            isDirty = true;
            if (doRefresh) refreshLayout();
        }

        void deleteCell(const GridCell& slot, const bool doRefresh = true) {
            deleteCell(slot.row, slot.col, doRefresh);
        }

        void deleteCell(const int row, const int col, const bool doRefresh = true) {
            if (!parentItem || !isCellValid(row, col)) return;

            auto it = std::remove_if(cells.begin(), cells.end(), [this, row, col](GridCell& c) {
                if (c.row == row && c.col == col) {
                    destroyItem(c.item);
                    return true;
                }
                return false;
                });

            if (it != cells.end()) {
                cells.erase(it, cells.end());
                isDirty = true;
                if (doRefresh) refreshLayout();
            }
        }

        void deleteCell(const int index, const bool doRefresh = true) {
            if (!parentItem || !isIndexValid(index)) return;

            destroyItem(cells[index].item);
            cells.erase(cells.begin() + index);
            isDirty = true;
            if (doRefresh) refreshLayout();
        }
    };
}