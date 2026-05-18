#include <QApplication>
#include <QFile>
#include <QDirIterator>
#include <QStyle>
#include <QDebug>

import APushButton;
import ACommonStyles;


void initCommonResources(QApplication& app) {
    Q_INIT_RESOURCE(ACommonResources);
    ACommonStyles::loadStyles(app);
}

int main(int argc, char* argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    initCommonResources(a);


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

    return a.exec();
}