module;

#include <QApplication>
#include <QDirIterator>
#include <QDebug>
#include <QDir>

#include <memory>
#include <concepts>
#include <functional>

import AxoButton;

export module AxoCommonStyles;


std::function<void(QString& style, const QString path)> addNewStyle = [](QString& const style, const QString path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    if (data.isEmpty()) return;

    style += "\n/* Source: " + path + " */\n";
    style += "\n" + QLatin1String(data);
};

template<typename T>
concept IsQString = std::convertible_to<T, QString>;

template<typename... Args>
concept QFilePaths = (IsQString<Args> && ...);

template<typename... QFilePaths>
void addStyles(QString& const style, QFilePaths... path) {
    (addNewStyle(style, path), ...);
}

export namespace AxoCommonStyles {
    void loadStyles(QApplication& app) {
        QString combinedStyle = app.styleSheet();

        addStyles(combinedStyle, 
            ":/AxoButton/AxoButton.qss"
        );

        app.setStyleSheet(combinedStyle);
    }
}