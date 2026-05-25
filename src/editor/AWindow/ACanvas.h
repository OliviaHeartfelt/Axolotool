#pragma once

#include "../ANode/ANode.h"
#include <QWidget>

class Canvas : public QWidget {
public:
    Canvas(QWidget* parent) : QWidget(parent) {
        setStyleSheet("background-color: #ecf0f1;");
        setLayout(nullptr);
    }

    void addNode(int x, int y) {
        ANode* node = new ANode(this);
        node->move(x, y);
        node->show();
    }
};