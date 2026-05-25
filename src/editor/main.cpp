#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QRandomGenerator>


import APushButton;
import ACommonStyles;
import AWindow;

void initCommonResources(QApplication& app) {
    Q_INIT_RESOURCE(ACommonResources);
    ACommonStyles::loadStyles(app);
}

int main(int argc, char* argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    initCommonResources(app);

    AMainWindow win;
    win.show();

    return app.exec();
}