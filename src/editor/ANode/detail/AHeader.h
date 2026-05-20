#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QString>
#include <QLabel>

class AHeader : public QWidget {
    Q_OBJECT;

private:
    QHBoxLayout* headerLayout = nullptr;

public:
    explicit AHeader(const QString& title, QWidget* parent = nullptr) : QWidget(parent) {
        headerLayout = new QHBoxLayout(parent);
        
    }

    void setTitle(const QString& title) {
        QLabel* titleLabel = new QLabel(QString("<b>%1</b>").arg(title), this);
        headerLayout->addWidget(titleLabel);
    }
};