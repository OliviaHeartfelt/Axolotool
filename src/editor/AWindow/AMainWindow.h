#pragma once

#include "ACanvas.h"
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

class AMainWindow : public QMainWindow {
public:
    AMainWindow() {
        setMinimumSize(600, 400);

        auto* central = new QWidget(this);
        auto* mainLayout = new QVBoxLayout(central);

        auto* spawnBtn = new QPushButton("Spawn Node");
        auto* canvas = new Canvas(this);

        mainLayout->addWidget(spawnBtn);
        mainLayout->addWidget(canvas, 1); // Canvas takes remaining space
        setCentralWidget(central);

        connect(spawnBtn, &QPushButton::clicked, [canvas]() {
            canvas->addNode(20, 20); // Spawns at 20,20 on the canvas
        });
    }
};