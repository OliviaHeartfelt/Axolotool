#pragma once

#include "../ANodeEnvironment/ANodeEnvironment.h"

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

            mainLayout->addWidget(sidebar);

            auto* nodeEnvironment = new ANodeEnvironment::ANodeEnvironment(this, this);
            mainLayout->addWidget(nodeEnvironment->canvas(), 1);

            if (nodeEnvironment->openDatabase("project.db")) {
                //nodeEnvironment->loadChunk(initialChunkId);
            }
        }
    };
}