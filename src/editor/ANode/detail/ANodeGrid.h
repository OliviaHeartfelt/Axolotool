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
        qDebug() << "addWidgetHelper(), before 1st check, widget: " << (static_cast<bool>(widget) ? "true" : "false") << " mainGrid: " << (static_cast<bool>(mainGrid) ? "true" : "false");
        if (!mainGrid || !widget || fromRow < 0 || fromColumn < 0 || rowSpan <= 0 || colSpan <= 0) return;

        qDebug() << "addWidgetHelper(), before isDestructive check";
        if (isDestructive)
            deleteCell(fromRow, fromColumn);
        else if (existsCell(fromRow, fromColumn))
            return;

        qDebug() << "addWidgetHelper(), before \"widget ? widget : new QWidget();\", widget is: " << (static_cast<bool>(widget) ? "true" : "false");
        QWidget* w = widget ? widget : new QWidget();

        qDebug() << "addWidgetHelper(), before final addWidget()";
        mainGrid->addWidget(w, fromRow, fromColumn, rowSpan, colSpan, alignment);
    }

public:
    ANodeGrid(QGridLayout* mainGrid) : mainGrid(mainGrid) {}
    ANodeGrid(QWidget* parent = nullptr) {
        mainGrid = new QGridLayout(parent);
    }

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