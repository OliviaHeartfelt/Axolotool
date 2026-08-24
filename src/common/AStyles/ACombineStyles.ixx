module;

#include <QApplication>
#include <QDirIterator>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

export module ACombineStyles;

export namespace ACombineStyles {
    QString combineStyles(const QString& resourceFolder = QStringLiteral(":/"), const QStringList& nameFilters = QStringList()) {
        QStringList concatenatedStyles;

        QDirIterator it(resourceFolder, nameFilters, QDir::Files, QDirIterator::Subdirectories);

        while (it.hasNext()) {
            QString currentFilePath = it.next();
            QFile file(currentFilePath);

            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream stream(&file);
                QString svgContent = stream.readAll();
                file.close();

                concatenatedStyles.append(svgContent);
            }
            else {
                qWarning() << "Failed to open embedded resource:" << currentFilePath;
            }
        }
        return concatenatedStyles.join("\n");
    }
    void combineStyles(QApplication& app, const QString& resourceFolder = QStringLiteral(":/"), const QStringList& nameFilters = QStringList()) {
        QString combinedStyle = app.styleSheet();
        combinedStyle += combineStyles(resourceFolder, nameFilters);
        app.setStyleSheet(combinedStyle);
    }
}