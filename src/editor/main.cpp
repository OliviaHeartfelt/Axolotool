#include "./AWindow/AMainWindow.h"
#include "./Utility/Utility.h"

void initCommonResources(QApplication& app) {
    Q_INIT_RESOURCE(TablerIconsResources);

    Utility::StylesUtility::combineStyles(app, ":/icons", { "*.svg" });
}

int main(int argc, char* argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    initCommonResources(app);

    AMainWindow::AMainWindow win;
    win.show();

    return app.exec();
}