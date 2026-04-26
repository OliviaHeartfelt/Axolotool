#pragma once
#include <QPushButton>
#include <QEnterEvent>
#include <QWheelEvent>

class AxoButton : public QPushButton {
    Q_OBJECT
public:
    using QPushButton::QPushButton;

    using mousePressFunction = std::function<void(QMouseEvent*)>;

protected:

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent* event) override;
#endif

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;

    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

    //other members: https://code.qt.io/cgit/qt/qtbase.git/tree/src/widgets/kernel/qwidget.h?h=6.11#n663
};