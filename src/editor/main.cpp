//#include <QApplication>
//#include <QMainWindow>
//#include <QPushButton>
//#include <QVBoxLayout>
//#include <QWidget>
//#include <QLabel>
//#include <QRandomGenerator>

//import ACommonStyles;
import AWindow;

import ACombineStyles;

void initCommonResources(QApplication& app) {
    //Q_INIT_RESOURCE(ACommonResources);
    Q_INIT_RESOURCE(TablerIconsResources);

    //ACommonStyles::loadStyles(app);
    ACombineStyles::combineStyles(app, ":/icons", { "*.svg" });
}

int main(int argc, char* argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    initCommonResources(app);

    AWindow::AMainWindow win;
    win.show();

    return app.exec();
}