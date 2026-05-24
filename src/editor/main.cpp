#include <QApplication>
#include <QFile>
#include <QDirIterator>
#include <QStyle>
#include <QDebug>
#include <QLabel>
#include <QPushButton>

import APushButton;
import ACommonStyles;
import ANode;

import Monitoring;


void initCommonResources(QApplication& app) {
    Q_INIT_RESOURCE(ACommonResources);
    ACommonStyles::loadStyles(app);
}

int main(int argc, char* argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    initCommonResources(a);

    /*
    APushButton* mainBtn = new APushButton("Launch Engine");
    mainBtn->setMinimumSize(200, 60);
    mainBtn->setProperty("axoStyle", "primary");
    
    mainBtn->onPress.set([](decltype(mainBtn->onPress)::event_type event) {
        if (event) {
            qInfo() << "valid event, Event received: " << event->type();
        }
        else {
            qInfo() << "not valid event";
        }
    });
    mainBtn->onDoubleClick.set([](decltype(mainBtn->onDoubleClick)::event_type event) {
        if (event) {
            qInfo() << "valid event, Event received: " << event->type() << event->button();
        }
        else {
            qInfo() << "not valid event";
        }
        });
    mainBtn->onHold.set([]() {
        qInfo() << "holding the button";
    });


    // Force a style refresh
    mainBtn->style()->unpolish(mainBtn);
    mainBtn->style()->polish(mainBtn);

    mainBtn->show();
    */

    ANode* node = new ANode();
    node->setWindowTitle("ANode Test Window");
    node->setMinimumSize(300, 200);

    QLabel* label = new QLabel("Hello, ANode!");
    QPushButton* button = new QPushButton("Click Me");

    node->body->addWidget(label, 0, 0);
    node->body->addWidget(button, 1, 0);

    node->show();

    int rows = 5;
    int cols = 10;
    enum class GridStatus { Success, Error, NullPointer };
    Monitoring::log(
        GridStatus::Success,
        "GridSystem",
        "Initialization complete"
        //{ "Rows", rows },
        //{ "Cols", cols }
    );

    return a.exec();
}