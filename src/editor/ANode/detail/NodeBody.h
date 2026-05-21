#pragma once
#include <vector>

#include <QWidget>
#include <QGridLayout>
#include <QString>
#include <QLabel>

class NodeBody : public QWidget {
    Q_OBJECT;

private:
    QGridLayout* mainGrid = nullptr;

    using CUInt = const unsigned int;

public:
    struct NodeSlot {
        QWidget* widget = nullptr;
        int row = 0;
        int col = 0;
        int rowSpan = 0;
        int colSpan = 0;
        Qt::Alignment alignment = Qt::Alignment();
    };

    explicit NodeBody(const QString& title, QWidget* parent = nullptr) : QWidget(parent) {
        mainGrid = new QGridLayout(parent);
        mainGrid->setContentsMargins(6, 4, 6, 4);
        mainGrid->setSpacing(4);
    }

    void nodeBodyInit(CUInt rowNum, CUInt columnNum, Qt::Alignment alignment = Qt::Alignment()) {
        if (!mainGrid || (mainGrid->rowCount() == 0 && mainGrid->columnCount() == 0)) return;

        for (unsigned int i = 0; i < rowNum; i++)
            for (unsigned int j = 0; j < columnNum; j++)
                mainGrid->addWidget(new QWidget(), i, j, alignment);
    }
    void nodeBodyDelete() {
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
    void removeAtPosition(CUInt row, CUInt column) {
        QLayoutItem* item = mainGrid->itemAtPosition(row, column);

        if (item) {
            mainGrid->removeItem(item);

            if (QWidget* w = item->widget()) {
                w->deleteLater();
            }
            delete item;
        }
    }

    bool nodeIsSlotValid(const NodeSlot& slot) {
        return (slot.widget && slot.row >= 0 && slot.col >= 0 && slot.rowSpan >= 1 && slot.colSpan >= 1);
    }
    bool nodeIsSlotValid(const NodeSlot&& slot) {
        return (slot.widget && slot.row >= 0 && slot.col >= 0 && slot.rowSpan >= 1 && slot.colSpan >= 1);
    }
    NodeSlot&& nodeSlot(CUInt row, CUInt col, Qt::Alignment alignment = Qt::Alignment()) {
        int r, c, rs, cs = -1;
        QLayoutItem* item = mainGrid->itemAtPosition(row, col);
        if (item)
            mainGrid->getItemPosition(mainGrid->indexOf(item->widget()), &r, &c, &rs, &cs);

        return { item->widget(), (int)r, (int)c, (int)rs, (int)cs, alignment };
    }
    NodeSlot&& nodeSlot(CUInt index, Qt::Alignment alignment = Qt::Alignment()) {
        int r, c, rs, cs = -1;
        QLayoutItem* item = mainGrid->takeAt(index);
        if (item)
            mainGrid->getItemPosition(mainGrid->indexOf(item->widget()), &r, &c, &rs, &cs);

        return { item->widget(), (int)r,(int)c, (int)rs, (int)cs, alignment };
    }
    std::vector<NodeSlot> nodeBody() {
        std::vector<NodeSlot> arr{};
        arr.reserve(mainGrid->count());

        for (size_t i = 0; i < mainGrid->count(); i++) {
            //if(auto nodeSlot(i))
            //arr.push_back(nodeSlot(i));
        }

        return arr;
    }
    void nodeBodyRebuild(const std::vector<NodeSlot>& newGrid,  Qt::Alignment alignment = Qt::Alignment()) {
        nodeBodyDelete();
        for (const NodeSlot& slot : newGrid) {
            if (!nodeIsSlotValid(slot))
                continue;

            nodeBodyAddWidget(slot.widget, slot.row, slot.col, slot.rowSpan, slot.colSpan, slot.alignment);
        }
    }
    void nodeBodyAddWidget(QWidget* widget, CUInt row, CUInt column, Qt::Alignment alignment = Qt::Alignment()) {
        removeAtPosition(row, column);
        mainGrid->addWidget((widget) ? widget : new QWidget(), row, column, alignment);
    }
    void nodeBodyAddWidget(QWidget* widget, CUInt fromRow, CUInt fromColumn, CUInt rowSpan, CUInt colSpan, Qt::Alignment alignment = Qt::Alignment()) {
        removeAtPosition(fromRow, fromColumn);
        mainGrid->addWidget((widget) ? widget : new QWidget(), fromRow, fromColumn, rowSpan, colSpan, alignment);
    }
    void nodeBodyAddWidget(const NodeSlot& slot) {
        if (!nodeIsSlotValid(slot))
            return;

        removeAtPosition(slot.row, slot.col);
        mainGrid->addWidget((slot.widget) ? slot.widget : new QWidget(), slot.row, slot.col, slot.rowSpan, slot.colSpan, slot.alignment);
    }
    void nodeBodyAddWidget(const NodeSlot&& slot) {
        if (!nodeIsSlotValid(slot))
            return;

        removeAtPosition(slot.row, slot.col);
        mainGrid->addWidget((slot.widget) ? slot.widget : new QWidget(), slot.row, slot.col, slot.rowSpan, slot.colSpan, slot.alignment);
    }
};