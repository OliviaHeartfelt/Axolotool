#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QString>

class AHeader : public QWidget {
    Q_OBJECT;

private:

public:
    explicit AHeader(const QString& title, QWidget* parent = nullptr) : QWidget(parent) {

    }
};