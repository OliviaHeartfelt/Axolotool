#pragma once

#include "ACallback.h";

#include <QPushButton>
#include <QEnterEvent>
#include <QWheelEvent>

#include <QTimer>
#include <QWidget>
#include <QString>

class APushButton : public QPushButton {
    Q_OBJECT;
    
    QWidget* parent = nullptr;

public:
    explicit APushButton(QWidget* parent = nullptr);
    explicit APushButton(const QString& text, QWidget* parent = nullptr);
    explicit APushButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr);

    ACallback::Event<QMouseEvent*> onPress;
    ACallback::Event<QMouseEvent*> onRelease;
    ACallback::Event<QMouseEvent*> onDoubleClick;
    ACallback::Empty               onHold;
                 
    ACallback::Event<QEnterEvent*> onEnter;
    ACallback::Event<QEvent*>      onLeave;

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    QTimer holdTimer;

    void setupConnections();
};

//other members: https://code.qt.io/cgit/qt/qtbase.git/tree/src/widgets/kernel/qwidget.h?h=6.11#n663