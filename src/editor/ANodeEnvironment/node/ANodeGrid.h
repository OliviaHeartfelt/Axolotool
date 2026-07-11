#pragma once

namespace ANodeGrid {

    class Grid {
    public:
        struct GridCell {
            QGraphicsItem* item = nullptr;
            int row = -1;
            int col = -1;
            int rowSpan = 0;
            int colSpan = 0;
            Qt::Alignment alignment = Qt::Alignment();
        };

    private:
        QGraphicsRectItem* parentItem = nullptr;
        std::vector<GridCell> cells{};

        int totalRows = 1;
        int totalCols = 1;
        qreal margin = 0.0;
        qreal spacing = 0.0;

        // Helper to calculate pixel geometry for a cell based on its grid position
        void updateCellGeometry(const GridCell& cell) {
            if (!cell.item || !parentItem) return;

            QRectF totalRect = parentItem->rect();

            // Calculate cell sizes dynamically based on parent bounds
            qreal availableWidth = totalRect.width() - (margin * 2) - ((totalCols - 1) * spacing);
            qreal availableHeight = totalRect.height() - (margin * 2) - ((totalRows - 1) * spacing);

            qreal cellWidth = availableWidth / totalCols;
            qreal cellHeight = availableHeight / totalRows;

            // Compute local coordinates
            qreal x = margin + cell.col * (cellWidth + spacing);
            qreal y = margin + cell.row * (cellHeight + spacing);
            qreal w = cell.colSpan * cellWidth + (cell.colSpan - 1) * spacing;
            qreal h = cell.rowSpan * cellHeight + (cell.rowSpan - 1) * spacing;

            // Apply geometry changes safely based on item type
            cell.item->setPos(x, y);
            if (auto* rectItem = dynamic_cast<QGraphicsRectItem*>(cell.item)) {
                rectItem->setRect(0, 0, w, h);
            }
        }

    public:
        Grid(QGraphicsRectItem* parent) : parentItem(parent) {}

        // 1. Init
        void initGrid(const int rows, const int cols) {
            totalRows = std::max(1, rows);
            totalCols = std::max(1, cols);
            deleteGrid();
        }

        // 2. Is valid & Exists...
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

        // 3. Access
        GridCell cellAt(const int row, const int col) {
            if (!isCellValid(row, col)) return GridCell();
            for (auto& c : cells) {
                if (c.row == row && c.col == col) return c;
            }
            return GridCell();
        }
        GridCell cellAt(const int index) {
            if (!isCellValid(index)) return GridCell();
            return cells.at(index);
        }

        QGraphicsItem* cellItem(const int row, const int col) {
            if (!isCellValid(row, col)) return nullptr;
            return cellAt(row, col).item;
        }
        QGraphicsItem* cellItem(const int index) {
            if (!isCellValid(index)) return nullptr;
            return cells.at(index).item;
        }

        const std::vector<GridCell>& getGridRef() const { return cells; }
        const std::vector<GridCell>  getGridCopy() const { return cells; }



        // 4. Create 
        void addItem(QGraphicsItem* item, int row, int col, int rowSpan = 1, int colSpan = 1) {
            if (!parentItem || !isCellValid(item, row, col, rowSpan, colSpan)) return;

            deleteCell(row, col);

            item->setParentItem(parentItem);
            GridCell newCell{ item, row, col, rowSpan, colSpan };

            updateCellGeometry(newCell);
            cells.push_back(newCell);
        }

        // 5. Delete
        void deleteGrid() {
            if (!parentItem) return;
            for (auto& c : cells)
                if (c.item) delete c.item;

            cells.clear();
        }

        void deleteCell(const GridCell& slot) {
            deleteCell(slot.row, slot.col);
        }
        void deleteCell(const int row, const int col) {
            if (!parentItem || !isCellValid(row, col)) return;

            auto it = std::remove_if(cells.begin(), cells.end(), [row, col, this](GridCell& c) {
                if (c.row == row && c.col == col) {
                    if (c.item && parentItem->scene())
                        delete c.item;
                    return true;
                }
                return false;
                });
            cells.erase(it, cells.end());
        }
        void deleteCell(const int index) {
            if (!parentItem || index < 0 || index >= cells.size()) return;

            cells.erase(cells.begin() + index);
        }

        // 6. Other
        void refreshLayout() {
            for (const auto& cell : cells)
                updateCellGeometry(cell);
        }
    };
}