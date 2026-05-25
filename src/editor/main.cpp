#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QRandomGenerator>


import APushButton;
import ACommonStyles;
import ANode;

import Debug;

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

class MainWindow : public QMainWindow {
public:
    MainWindow() {
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


void initCommonResources(QApplication& app) {
    Q_INIT_RESOURCE(ACommonResources);
    ACommonStyles::loadStyles(app);
}

int main(int argc, char* argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    initCommonResources(app);

    MainWindow win;
    win.show();

    return app.exec();
}