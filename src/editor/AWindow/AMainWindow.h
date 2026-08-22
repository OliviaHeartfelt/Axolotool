#pragma once

#include "../ANodeEnvironment/ANodeEnvironment.h"
#include "MockPlugin.h"

namespace AMainWindow {

    class AMainWindow : public QMainWindow {
    public:
        AMainWindow() {
            setMinimumSize(800, 500);

            auto* nodeEnvironment = new ANodeEnvironment::ANodeEnvironment(this, this);
            if (!nodeEnvironment->openDatabase("project.db")) {
                qWarning() << "Failed to open project.db! Exiting setup.";
                return;
            }
            qDebug() << "nodeEnvironment->isOpen(): " << (nodeEnvironment->isOpen() ? "true" : "false");
            assert(nodeEnvironment->isOpen());

            MockPlugin::initPlugin(nodeEnvironment);


            auto* central = new QWidget(this);
            setCentralWidget(central);

            auto* mainLayout = new QHBoxLayout(central);
            mainLayout->setContentsMargins(0, 0, 0, 0);
            mainLayout->setSpacing(0);


            auto* sidebar = new QFrame(central);
            sidebar->setFixedWidth(200);
            sidebar->setStyleSheet(R"(
                QFrame {
                    background-color: #2c3e50;
                }
                QPushButton {
                    background-color: #34495e;
                    color: #ecf0f1;
                    border: 1px solid #455a64;
                    border-radius: 4px;
                    padding: 8px;
                    font-weight: bold;
                }
                QPushButton:hover {
                    background-color: #415b76;
                }
                QPushButton:pressed {
                    background-color: #1abc9c;
                }
            )");

            auto* sidebarLayout = new QVBoxLayout(sidebar);
            sidebarLayout->setContentsMargins(10, 15, 10, 15);
            sidebarLayout->setSpacing(10);

            auto* btnNodeA = new QPushButton("Add Node A", sidebar);
            auto* btnNodeB = new QPushButton("Add Node B", sidebar);
            auto* btnNodeC = new QPushButton("Add Node C", sidebar);
            auto* btnSave = new QPushButton("Save", sidebar);
            auto* btnLoad = new QPushButton("Load", sidebar);

            sidebarLayout->addWidget(btnNodeA);
            sidebarLayout->addWidget(btnNodeB);
            sidebarLayout->addWidget(btnNodeC);
            sidebarLayout->addWidget(btnSave);
            sidebarLayout->addWidget(btnLoad);
            sidebarLayout->addStretch();


            mainLayout->addWidget(sidebar);
            mainLayout->addWidget(nodeEnvironment->canvas(), 1);

            const auto nodeACoreId = muuid::uuid::from_chars("01a01024-8932-7727-9b20-30a7f81ad76a");
            const auto nodeBCoreId = muuid::uuid::from_chars("01a01029-f8e6-73e9-9db3-926c8ad12f52");
            const auto nodeCCoreId = muuid::uuid::from_chars("01a01029-f8e6-73e9-9db3-964174d5c085");

            connect(btnNodeA, &QPushButton::clicked, this, [nodeEnvironment, nodeACoreId]() {
                if (nodeEnvironment && nodeACoreId) {
                    nodeEnvironment->spawnNode(*nodeACoreId);
                }
                });
            connect(btnNodeB, &QPushButton::clicked, this, [nodeEnvironment, nodeBCoreId]() {
                if (nodeEnvironment && nodeBCoreId) {
                    nodeEnvironment->spawnNode(*nodeBCoreId);
                }
                });
            connect(btnNodeC, &QPushButton::clicked, this, [nodeEnvironment, nodeCCoreId]() {
                if (nodeEnvironment && nodeCCoreId) {
                    nodeEnvironment->spawnNode(*nodeCCoreId);
                }
                });

            connect(btnSave, &QPushButton::clicked, this, [nodeEnvironment]() {
                if (nodeEnvironment) {
                    nodeEnvironment->save();
                }
                });
            connect(btnLoad, &QPushButton::clicked, this, [nodeEnvironment]() {
                if (nodeEnvironment) {
                    nodeEnvironment->load();
                }
                });
        }
    };
}

/*
#pragma once

#include "../ANodeEnvironment/ANodeEnvironment.h"
#include "MockPlugin.h"

namespace AMainWindow {

    class AMainWindow : public QMainWindow {
    public:
        AMainWindow() {
            setMinimumSize(600, 400);

            auto* nodeEnvironment = new ANodeEnvironment::ANodeEnvironment(this, this);
            if (!nodeEnvironment->openDatabase("project.db")) return;

            MockPlugin::initPlugin(nodeEnvironment);


            auto* central = new QWidget(this);


            auto* sidebar = new QFrame(central);

            sidebar->setFixedWidth(200);
            sidebar->setStyleSheet("background-color: #2c3e50;");


            setCentralWidget(central);
            auto* mainLayout = new QHBoxLayout(central);

            mainLayout->addWidget(sidebar);
            //mainLayout->addWidget(nodeEnvironment->canvas(), 1);
        }
    };
}
*/