#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QString>
#include <QLabel>

class ABody : public QWidget {
    Q_OBJECT;

private:
    QGridLayout* mainGrid = nullptr;

public:
    explicit ABody(const QString& title, QWidget* parent = nullptr) : QWidget(parent) {
        mainGrid = new QGridLayout(parent);
        mainGrid->setContentsMargins(6, 4, 6, 4);
        mainGrid->setSpacing(4);
    }

    void addWidget(QWidget* widget, int row, int column, Qt::Alignment alignment = Qt::Alignment()) {
        mainGrid->addWidget(widget, row, column, alignment);
    }
    void addWidget(QWidget* widget, int fromRow, int fromColumn, int rowSpan, int columnSpan, Qt::Alignment alignment = Qt::Alignment()) {
        mainGrid->addWidget(widget, fromRow, fromColumn, rowSpan, columnSpan, alignment);
    }
};