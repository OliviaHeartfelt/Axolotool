/*
#pragma once

#include <QWidget>
#include <QGridLayout>

class ANodeGrid {
private:
    QGridLayout* mainGrid = nullptr;

    using CUInt = const unsigned int;
    using CInt = const unsigned int;

    struct GridCell;
    void removeCellAtHelper(const bool hasTwoValues, CInt first = -1, CInt second = -1, const bool deleteIfEmpty = true) {
        if (!mainGrid) return;

        QLayoutItem* item = (hasTwoValues) ? mainGrid->itemAtPosition(first, second) : mainGrid->itemAt(first);
        if (!item) return;

        if (QWidget* w = item->widget()) {
            w->deleteLater();
            delete item;
        }
        else if (deleteIfEmpty && item) {
            delete item;
        }
    }

    GridCell&& cellHelper(const bool hasTwoValues, CInt first = -1, CInt second = -1, Qt::Alignment alignment = Qt::Alignment()) {
        if(!mainGrid) return { nullptr, -1, -1, -1, -1, alignment };

        int r, c, rs, cs = -1;
        QLayoutItem* item = (hasTwoValues) ? mainGrid->itemAtPosition(first, second) : mainGrid->itemAt(first);

        if (item && item->widget()) {
            mainGrid->getItemPosition(mainGrid->indexOf(item->widget()), &r, &c, &rs, &cs);
            return { item->widget(), r, c, rs, cs, alignment };
        }
        else
            return { nullptr, -1, -1, -1, -1, alignment };
    }

    bool existsCellHelper(const bool hasTwoValues, CInt first = -1, CInt second = -1) {
        if (!mainGrid) return false;

        QLayoutItem* item = (hasTwoValues) ? mainGrid->itemAtPosition(first, second) : mainGrid->itemAt(first);
        return static_cast<bool>(item);
    }

    QWidget* cellWidgetHelper(const bool hasTwoValues, CInt first = -1, CInt second = -1) {
        if (!mainGrid) return nullptr;

        QLayoutItem* item = (hasTwoValues) ? mainGrid->itemAtPosition(first, second) : mainGrid->itemAt(first);
        if (item)
            return item->widget();
        else
            return nullptr;
    }

    void addWidgetHelper(QWidget* widget, CInt fromRow, CInt fromColumn, CInt rowSpan = 1, CInt colSpan = 1, const bool isDestructive = true, Qt::Alignment alignment = Qt::Alignment()) {
        if (!mainGrid || !widget || fromRow < 0 || fromColumn < 0 || rowSpan <= 0 || colSpan <= 0) return;

        if (isDestructive)
            deleteCell(fromRow, fromColumn);
        else if (existsCell(fromRow, fromColumn))
            return;

        QWidget* w = widget ? widget : new QWidget();

        mainGrid->addWidget(w, fromRow, fromColumn, rowSpan, colSpan, alignment);
    }

public:
    ANodeGrid(QGridLayout* mainGrid) : mainGrid(mainGrid) {}

    struct GridCell {
        QWidget* widget = nullptr;
        int row = 0;
        int col = 0;
        int rowSpan = 0;
        int colSpan = 0;
        Qt::Alignment alignment = Qt::Alignment();
    };

    // 1. Init
    void initGrid(CUInt rowNum, CUInt columnNum, Qt::Alignment alignment = Qt::Alignment()) {
        if (!mainGrid || (mainGrid->rowCount() == 0 && mainGrid->columnCount() == 0)) return;

        for (unsigned int i = 0; i < rowNum; i++)
            for (unsigned int j = 0; j < columnNum; j++)
                mainGrid->addWidget(new QWidget(), i, j, alignment);
    }

    // 2. Is valid & Exists
    bool isCellValid(const GridCell& slot) {  
        return (mainGrid)? (slot.widget && slot.row >= 0 && slot.col >= 0 && slot.rowSpan >= 1 && slot.colSpan >= 1) : false;
    }
    bool isCellValid(const GridCell&& slot) { 
        return (mainGrid)? (slot.widget && slot.row >= 0 && slot.col >= 0 && slot.rowSpan >= 1 && slot.colSpan >= 1) : false;
    }
    bool isGridValid() { return static_cast<bool>(mainGrid); }

    bool existsCell(CInt row, CInt col) { return existsCellHelper(true, row, col); }
    bool existsCell(CInt index) { return existsCellHelper(false, index); }

    // 3. Access
    GridCell&& cell(CInt row, CInt col, Qt::Alignment alignment = Qt::Alignment()) { return cellHelper(true, row, col, alignment); }
    GridCell&& cell(CInt index, Qt::Alignment alignment = Qt::Alignment()) {         return cellHelper(false, index, alignment); }

    QWidget* cellWidget(CInt row, CInt col) { return cellWidgetHelper(true, row, col); }
    QWidget* cellWidget(CInt index) {         return cellWidgetHelper(false, index); }

    std::vector<GridCell>&& grid() {
        std::vector<GridCell> arr{};
        if(!mainGrid) return std::move(arr);

        arr.reserve(mainGrid->count());

        for (size_t i = 0; i < mainGrid->count(); i++) {
            if (GridCell node = cell(i); isCellValid(node))
                arr.push_back(cell(i));
        }

        return std::move(arr);
    }

    // 4. Create    
    void addWidget(QWidget* widget, CInt fromRow, CInt fromColumn, CInt rowSpan = 1, CInt colSpan = 1, const bool isDestructive = true, Qt::Alignment alignment = Qt::Alignment()) {
        qDebug() << "addWidget()";
        addWidgetHelper(widget, fromRow, fromColumn, rowSpan, colSpan, isDestructive, alignment);
    }
    void addWidget(const GridCell& slot, const bool isDestructive = true) {
        addWidgetHelper(slot.widget, slot.row, slot.col, slot.rowSpan, slot.colSpan, isDestructive, slot.alignment);
    }

    // 5. Delete
    void deleteGrid() {
        if (!mainGrid) return;

        for (int i = mainGrid->count() - 1; i >= 0; --i) {
            QLayoutItem* item = mainGrid->takeAt(i);
            if (item) {
                if (QWidget* widget = item->widget())
                    widget->deleteLater();

                delete item;
            }
        }
    }
    void deleteCell(CInt row, CInt column) { removeCellAtHelper(true, row, column); }
    void deleteCell(CInt index) { removeCellAtHelper(false, index); }

    // 6. Modify
    void gridRebuild(const std::vector<GridCell>& newGrid, const bool isDestructive = true, Qt::Alignment alignment = Qt::Alignment()) {
        if (!mainGrid) return;

        deleteGrid();
        for (const GridCell& slot : newGrid) {
            if (!isCellValid(slot)) continue;

            addWidget(slot.widget, slot.row, slot.col, slot.rowSpan, slot.colSpan, isDestructive, slot.alignment);
        }
    }
};
*/

#pragma once

#include <QGraphicsItem>
#include <QGraphicsSimpleTextItem>
#include <QDebug>
#include <vector>
#include <algorithm>

class ANodeGrid {
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
        qreal availableWidth =  totalRect.width()  - (margin * 2) - ((totalCols - 1) * spacing);
        qreal availableHeight = totalRect.height() - (margin * 2) - ((totalRows - 1) * spacing);

        qreal cellWidth =  availableWidth  / totalCols;
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
    ANodeGrid(QGraphicsRectItem* parent) : parentItem(parent) {}

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
        return (index >= 0)? (index < cells.size()) : false;
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

    const std::vector<GridCell>& getGridRef() const {  return cells; }
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