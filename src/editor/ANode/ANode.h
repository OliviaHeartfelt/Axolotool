#pragma once

#include "ANodeGrid.h"
#include <QWidget>
#include <QGridLayout>
#include <QMouseEvent>

#include <QLabel>

class ANode : public QWidget {
	Q_OBJECT

private:
    QGridLayout* mainGrid = nullptr;

    QPoint dragPosition;
    float normX = 0.5f; // 50% across
    float normY = 0.5f; // 50% down
public:
    ANodeGrid* body = nullptr;

    explicit ANode(QWidget* parent = nullptr) : QWidget(parent) {
        mainGrid = new QGridLayout(this);
        mainGrid->setContentsMargins(6, 4, 6, 4);
        mainGrid->setSpacing(4);

        setFixedSize(100, 50);
        setStyleSheet("background-color: #3498db; border: 1px solid #2980b9; border-radius: 5px;");

        body = new ANodeGrid(mainGrid);
        body->addWidget(new QLabel("Node", this), 0, 0, 2, 1);
    }

    void updateFromParentSize(QSize newSize) {
        move(static_cast<int>(normX * newSize.width()),
            static_cast<int>(normY * newSize.height()));
    }
    //void setNodePos(float x, float y) {
    //    normX = x;
    //    normY = y;
    //    move(normX, normY);
    //}

protected:
    void mousePressEvent(QMouseEvent* event) override {
        dragPosition = event->pos();
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        if (event->buttons() & Qt::LeftButton) {
            move(mapToParent(event->pos() - dragPosition));
        }
    }
    //void mouseMoveEvent(QMouseEvent* event) override {
    //    if (event->buttons() & Qt::LeftButton) {
    //        QPoint newPos = pos() + (event->pos() - dragPosition);

    //        move(newPos);

    //        posX = newPos.x();
    //        posY = newPos.y();

    //        event->accept();
    //    }
    //}

};