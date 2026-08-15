#pragma once

namespace VWNodeDetails::NodeGrid {

    struct GridCell {
        QGraphicsItem* item = nullptr;
        int row = -1;
        int col = -1;
        int rowSpan = 0;
        int colSpan = 0;
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

        void refreshLayout() {
            if (!isDirty || !parentItem) return;

            std::vector<qreal> rowHeights(totalRows, 20.0);
            std::vector<qreal> colWidths(totalCols, 80.0); 

            for (const auto& cell : cells) {
                if (!cell.item) continue;

                QRectF itemBounds = cell.item->boundingRect();
                qreal reqW = itemBounds.width();
                qreal reqH = itemBounds.height();

                if (auto* rectItem = dynamic_cast<QGraphicsRectItem*>(cell.item)) {
                    if (rectItem->rect().width() > 0)  reqW = rectItem->rect().width();
                    if (rectItem->rect().height() > 0) reqH = rectItem->rect().height();
                }

                if (cell.colSpan == 1 && cell.col >= 0 && cell.col < totalCols) {
                    colWidths[cell.col] = std::max(colWidths[cell.col], reqW);
                }
                if (cell.rowSpan == 1 && cell.row >= 0 && cell.row < totalRows) {
                    rowHeights[cell.row] = std::max(rowHeights[cell.row], reqH);
                }
            }

            qreal totalContentWidth = std::accumulate(colWidths.begin(), colWidths.end(), 0.0) + (margin * 2) + ((totalCols - 1) * spacing);

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
                if (!cell.item) continue;

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
        }

        void destroyItem(QGraphicsItem* item) {
            if (!item) return;
            item->setParentItem(nullptr);
            if (auto* scene = item->scene()) {
                scene->removeItem(item);
            }
            delete item;
        }

    public:
        Grid(QGraphicsRectItem* parent, int _totalRows = 1, int _totalCols = 1, qreal _margin = 6.0, qreal _spacing = 4.0) : 
            parentItem(parent), totalRows(_totalRows), totalCols(_totalCols), margin(_margin), spacing(_spacing) {}

        ~Grid() {
            deleteGrid(false);
        }

        void initGrid(const int rows, const int cols, const bool refresh = true) {
            deleteGrid(refresh);
            totalRows = std::max(1, rows);
            totalCols = std::max(1, cols);
        }
        void setDimensions(const int rows, const int cols, const bool refresh = true) {
            totalRows = std::max(1, rows);
            totalCols = std::max(1, cols);
            isDirty = true;
            if (refresh) refreshLayout();
        }

        void refresh() {
            isDirty = true;
            refreshLayout();
        }

        bool isCellValid(const GridCell& slot) const {
            return slot.item && slot.row >= 0 && slot.col >= 0 && slot.rowSpan >= 1 && slot.colSpan >= 1;
        }
        bool isCellValid(QGraphicsItem* item, const int row, const int col, const int rowSpan = 1, const int colSpan = 1) const {
            return (item && row >= 0 && col >= 0 && rowSpan >= 1 && colSpan >= 1);
        }
        bool isCellValid(const int row, const int col, const int rowSpan = 1, const int colSpan = 1) const {
            return (row >= 0 && col >= 0 && rowSpan >= 1 && colSpan >= 1);
        }
        bool isCellValid(const int index) {
            return index >= 0 && index < cells.size();
        }

        bool existsCell(const int row, const int col) const {
            return std::any_of(cells.begin(), cells.end(), [row, col](const GridCell& c) {
                return c.row == row && c.col == col;
            });
        }
        bool existsCell(const int index) const {
            return (index >= 0) ? (index < cells.size()) : false;
        }
        bool isOverlapingCell(const int row, const int col, int rowSpan = 1, int colSpan = 1) const {
            return std::any_of(cells.begin(), cells.end(), [row, col, rowSpan, colSpan](const GridCell& cell) {
                bool rowOverlap = std::max(cell.row, row) < std::min(cell.row + cell.rowSpan, row + rowSpan);
                bool colOverlap = std::max(cell.col, col) < std::min(cell.col + cell.colSpan, col + colSpan);
                return rowOverlap && colOverlap;
            });
        }

        void deleteOverlappingCells(int row, int col, int rowSpan = 1, int colSpan = 1, bool refresh = true) {
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
                if (refresh) refreshLayout();
            }
        }

        // Access
        std::optional<GridCell> cellAt(const int row, const int col) {
            if (!isCellValid(row, col)) return std::nullopt;
            for (auto& c : cells) {
                if (c.row == row && c.col == col) return c;
            }
            return std::nullopt;
        }
        std::optional<GridCell> cellAt(const int index) {
            if (!isCellValid(index)) return std::nullopt;
            return cells.at(index);
        }

        QGraphicsItem* cellItem(const int row, const int col) {
            if (!isCellValid(row, col)) return nullptr;
            if (const auto cell = cellAt(row, col))
                return cell->item;
            else
                return nullptr;
        }
        QGraphicsItem* cellItem(const int index) {
            if (!isCellValid(index)) return nullptr;
            return cells.at(index).item;
        }

        const std::vector<GridCell>& getGridRef() const { return cells; }
        const std::vector<GridCell>  getGridCopy() const { return cells; }


        // Create 
        bool addItem(QGraphicsItem* item, int row, int col, int rowSpan = 1, int colSpan = 1, const bool refresh = true, const bool overrideOnCollision = false) {
            if (!parentItem || !isCellValid(item, row, col, rowSpan, colSpan)) return false;

            if (isOverlapingCell(row, col, rowSpan, colSpan)) {
                if (overrideOnCollision) deleteOverlappingCells(row, col, rowSpan, colSpan, false);
                else return false;
            }            

            item->setParentItem(parentItem);
            GridCell newCell{ item, row, col, rowSpan, colSpan };

            cells.push_back(newCell);
            isDirty = true;
            if (refresh) refreshLayout();
            return true;
        }

        // Delete
        void deleteGrid(const bool refresh = true) {
            for (auto& c : cells)
                if (c.item) destroyItem(c.item);

            cells.clear();
            isDirty = true;
            if (refresh) refreshLayout();
        }

        void deleteCell(const GridCell& slot, const bool refresh = true) {
            deleteCell(slot.row, slot.col, refresh);
        }
        void deleteCell(const int row, const int col, const bool refresh = true) {
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
                if (refresh) refreshLayout();
            }
        }
        void deleteCell(const int index, const bool refresh = true) {
            if (!parentItem || !isCellValid(index)) return;

            destroyItem(cells[index].item);
            cells.erase(cells.begin() + index);
            isDirty = true;
            if (refresh) refreshLayout();
        }
    };
}