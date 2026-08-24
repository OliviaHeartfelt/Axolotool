module;

#include <QApplication>
#include <QDirIterator>
#include <QDebug>
#include <QDir>

#include <memory>
#include <concepts>
#include <functional>

export module ACommonStyles;

std::function<void(QString& style, const QString path)> addNewStyle = [](QString& style, const QString path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "addNewStyle: \"Could not open file\" path: " << path;
        return;
    }

    QByteArray data = file.readAll();
    if (data.isEmpty()) {
        qWarning() << "addNewStyle: \"File is empty\" path: " << path;
        return;
    }

    style += "\n/* Source: " + path + " */\n";
    style += "\n" + QLatin1String(data);
};

template<typename T>
concept IsQString = std::convertible_to<T, QString>;

template<typename... Args>
concept QFilePaths = (IsQString<Args> && ...);

template<typename... QFilePaths>
void addStyles(QString& style, QFilePaths... path) {
    (addNewStyle(style, path), ...);
}

export namespace ACommonStyles {
    void loadStyles(QApplication& app) {
        QString combinedStyle = app.styleSheet();

        addStyles(combinedStyle, 
            ":/common/APushButton/APushButton.qss"
        );

        app.setStyleSheet(combinedStyle);
    }
}