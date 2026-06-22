#pragma once

#include "ACanvas.h"

namespace AMainWindow {

    class AMainWindow : public QMainWindow {
    public:
        AMainWindow() {
            setMinimumSize(600, 400);

            auto* central = new QWidget(this);
            auto* mainLayout = new QHBoxLayout(central);
            setCentralWidget(central);

            auto* sidebar = new QFrame();
            sidebar->setFixedWidth(200);
            sidebar->setStyleSheet("background-color: #2c3e50;");

            ACanvas::Canvas* scene = new ACanvas::Canvas();

            mainLayout->addWidget(sidebar);
            mainLayout->addWidget(scene->getView(), 1);

        }
    };
}