#pragma once

#include "ANodeGrid.h"
#include <QWidget>
#include <QGridLayout>

class ANode : public QWidget {
	Q_OBJECT

private:
    QGridLayout* mainGrid = nullptr;

public:
    ANodeGrid* body = nullptr;

    explicit ANode(QWidget* parent = nullptr) : QWidget(parent) {
        mainGrid = new QGridLayout(this);
        mainGrid->setContentsMargins(6, 4, 6, 4);
        mainGrid->setSpacing(4);

        body = new ANodeGrid(mainGrid);
    }
};