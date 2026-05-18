#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>

class ANode : public QWidget {
	Q_OBJECT

private:
    QVBoxLayout* masterLayout = nullptr;
    QWidget* bodyContainer = nullptr;
    QGridLayout* bodyGrid = nullptr;
    int rowCount = 0;

public:
    explicit ANode(const QString& title, QWidget* parent = nullptr) : QWidget(parent) {
        masterLayout = new QVBoxLayout(this);
        masterLayout->setContentsMargins(0, 0, 0, 0);
        masterLayout->setSpacing(0);


    }
};