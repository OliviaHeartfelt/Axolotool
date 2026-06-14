module;

#include <QDebug>
#include <QGraphicsRectItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#include <QCoreApplication>
#include <QDir>
#include <QGraphicsColorizeEffect>

export module ANode;

import ACellItem;
import ANodeGrid;

export namespace ANode {

    class ANode : public QGraphicsRectItem {
    public:
        ANodeGrid::Grid* body = nullptr;

        explicit ANode(QGraphicsItem* parent = nullptr) : QGraphicsRectItem(parent) {
            // Defines the visual boundary size of your item
            setRect(0, 0, 100, 32);

            // Core Flags: Turns on absolute canvas tracking and frictionless dragging mechanics
            setFlags(QGraphicsItem::ItemIsMovable |
                QGraphicsItem::ItemIsSelectable |
                QGraphicsItem::ItemSendsGeometryChanges);

            body = new ANodeGrid::Grid(this);
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

            ACellItem::CellItem* inputCell = new ACellItem::CellItem("Input A", this);
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
}