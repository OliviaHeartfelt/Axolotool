#include <QApplication>
#include <QFile>
#include <QDirIterator>
#include <QStyle>
#include <QDebug>

import AxoButton;
import AxoCommonStyles;


void initCommonResources(QApplication& app) {
    Q_INIT_RESOURCE(common_resources);
    AxoCommonStyles::loadStyles(app);
}

int main(int argc, char* argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    initCommonResources(a);


    AxoButton* mainBtn = new AxoButton("Launch Engine");
    mainBtn->setMinimumSize(200, 60);
    mainBtn->setProperty("axoStyle", "primary");

    // Force a style refresh
    mainBtn->style()->unpolish(mainBtn);
    mainBtn->style()->polish(mainBtn);

    mainBtn->show();

    return a.exec();
}