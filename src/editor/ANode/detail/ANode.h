#pragma once

#include "AHeader.h"
#include "ABody.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>

class ANode : public QWidget {
	Q_OBJECT

private:
    QVBoxLayout* masterLayout = nullptr;
    // https://doc.qt.io/qt-6/qlayout.html
    // https://doc.qt.io/qt-6/qboxlayout.html

    void initNode() {
        masterLayout = new QVBoxLayout(this);
        masterLayout->setContentsMargins(0, 0, 0, 0);
        masterLayout->setSpacing(4);

        masterLayout->addWidget(new QWidget(this));
        masterLayout->addWidget(new QWidget(this));
    }

public:

    explicit ANode(const QString& title, QWidget* parent = nullptr) : QWidget(parent) { initNode(); }
    ANode(QWidget* parent = nullptr) : QWidget(parent) { initNode(); }

    int nodeCount() { masterLayout->count(); }
    void nodeAddRow() { masterLayout->addWidget(new QWidget(this)); }
    void nodeInitRows(const unsigned int N) {
        if (masterLayout->count() == 0) return;

        for (unsigned int i = 0; i < N; i++) 
            masterLayout->addWidget(new QWidget(this));
    }
    void nodeResizeRows(const unsigned int N, bool isDestructive = true) {
        const unsigned int nodeCount = masterLayout->count();
        if (N == nodeCount) return; // is exact size

        if (N > nodeCount) // add elements
            for (unsigned int i = 0; i < (N - nodeCount); i++) 
                masterLayout->addWidget(new QWidget(this));

        else if (N < nodeCount) { // remove elements
            if (isDestructive == false) return;

            for (unsigned int i = 0; i < (nodeCount - N); i++) {
                QLayoutItem* widget = masterLayout->itemAt(nodeCount - 1 - i);
                masterLayout->removeItem(widget);
                widget->widget()->deleteLater();
            }
        }
    }

    QWidget* nodeGet(const int index) { 
        QLayoutItem* widget = masterLayout->itemAt(index);
        if (not widget) return nullptr;

        return widget->widget();
    }
    void nodeSet(
        const int index
    ) { 
        QLayoutItem* oldWidget = masterLayout->itemAt(index);
        if (oldWidget) {
            masterLayout->removeItem(oldWidget);
            if (QWidget* w = oldWidget->widget()) 
                w->deleteLater();

            delete oldWidget;
        }
        masterLayout->insertWidget(index, new QWidget(this));
    }
    bool nodeSet(
        const int index,
        QWidget* newWidget,
        int stretch = 0,
        Qt::Alignment alignment = Qt::Alignment(),
        bool override = true
    ) {
        if (index < 0 || index >= masterLayout->count()) return false;

        QLayoutItem* oldItem = masterLayout->itemAt(index);
        if (oldItem && oldItem->widget() && !override) return false;

        QWidget* target = (newWidget) ? newWidget : new QWidget(this);

        if (oldItem) {
            QWidget* oldWidget = oldItem->widget();
            masterLayout->replaceWidget(oldWidget, target);
            if (oldWidget) 
                oldWidget->deleteLater();
        }
        else
            masterLayout->insertWidget(index, target);
    }
};