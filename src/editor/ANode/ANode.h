/*
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

protected:
    void mousePressEvent(QMouseEvent* event) override {
        dragPosition = event->pos();
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        if (event->buttons() & Qt::LeftButton) {
            move(mapToParent(event->pos() - dragPosition));
        }
    }
};
*/

#pragma once

#include <QDebug>

#include "ANodeGrid.h"
#include "ACellItem.h"

#include <QGraphicsRectItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#include <QCoreApplication>
#include <QDir>
#include <QGraphicsColorizeEffect>


class ANode : public QGraphicsRectItem {
public:
    ANodeGrid* body = nullptr;

    explicit ANode(QGraphicsItem* parent = nullptr) : QGraphicsRectItem(parent) {
        // Defines the visual boundary size of your item
        setRect(0, 0, 100, 32);

        // Core Flags: Turns on absolute canvas tracking and frictionless dragging mechanics
        setFlags(QGraphicsItem::ItemIsMovable |
            QGraphicsItem::ItemIsSelectable |
            QGraphicsItem::ItemSendsGeometryChanges);

        body = new ANodeGrid(this);
        body->initGrid(2, 1);

        //auto* label = new QGraphicsSimpleTextItem("Node");
        //label->setBrush(Qt::white);
        //body->addItem(label, 0, 0);

        //auto* label1 = new QGraphicsSimpleTextItem("Node");
        //label1->setBrush(Qt::white);
        //body->addItem(label1, 1, 0, 1, 1);

        //QString svgPath = QDir(QCoreApplication::applicationDirPath()).filePath(":/icons/outline/alien.svg");
        //QGraphicsSvgItem* alien = new QGraphicsSvgItem(svgPath);
        //QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect();
        //effect->setColor(Qt::blue); // Target color
        //alien->setGraphicsEffect(effect);
        //body->addItem(alien, 0, 1);

        CellItem* inputCell = new CellItem("Input A", this);
        body->addItem(inputCell, 0, 0);
    }

    ~ANode() {
        delete body;
    }

    // Replaces style sheets via efficient painter updates
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
        Q_UNUSED(widget);

        painter->setRenderHint(QPainter::Antialiasing);

        // Selection highlight mechanism logic
        if (option->state & QStyle::State_Selected) {
            painter->setPen(QPen(QColor("#f1c40f"), 2, Qt::DashLine)); // Highlight yellow border
        }
        else {
            painter->setPen(QPen(QColor("#2980b9"), 1));             // Normal crisp blue border
        }

        painter->setBrush(QColor("#3498db"));
        painter->drawRoundedRect(rect(), 5, 5); // 5px border-radius
    }
};