#pragma once

#include "../ASVG/ASVG.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QString>
#include <QLabel>

class ABodyRowItem : public QWidget {
    Q_OBJECT;

private:
    QHBoxLayout* mainHBox = nullptr;

public:
    explicit ABodyRowItem(QWidget* parent = nullptr) : QWidget(parent) {
        mainHBox = new QHBoxLayout(parent);
        mainHBox->setContentsMargins(0, 0, 0, 0);
        mainHBox->setSpacing(4);
    }

    void setContentsMargins(const int left, const int top, const int right, const int bottom) {
        mainHBox->setContentsMargins(left, top, right, bottom);
    }
    void setSpacing(const int value) {
        mainHBox->setSpacing(4);
    }

    void addLabel(QWidget* widget) {
        mainHBox->addWidget(widget);
    }
    void addLabel(QString str = "") {
        QLabel* label = new QLabel(str);
        mainHBox->addWidget(label);
    }
    void addIcon(QString path) {
        ASVG* svg = new ASVG(path, this);
        mainHBox->addWidget(svg);
    }
    void addStretch(const int stretch = 0) {
        mainHBox->addStretch(stretch);
    }
    void addSpacing(const int size = 0) {
        mainHBox->addSpacing(size);
    }
};