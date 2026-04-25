#pragma once
#include <QPushButton>
#include <QWidget>
#include <QFile>
#include <QApplication>

class AxoButton : public QPushButton {
    Q_OBJECT
public:
    using QPushButton::QPushButton;

protected:

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;

    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

    //void paintEvent(QPaintEvent* event) override;
    //void moveEvent(QMoveEvent* event) override;
    //void resizeEvent(QResizeEvent* event) override;
    //void closeEvent(QCloseEvent* event) override;


};