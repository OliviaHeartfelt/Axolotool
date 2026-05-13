#include "AxoButton.h"
#include <QEnterEvent>
#include <QWheelEvent>

void AxoButton::mousePressEvent(QMouseEvent* event) {
	QPushButton::mousePressEvent(event);
}

void AxoButton::mouseReleaseEvent(QMouseEvent* event) {
	QPushButton::mouseReleaseEvent(event);
}

void AxoButton::mouseDoubleClickEvent(QMouseEvent* event) {
	QPushButton::mouseDoubleClickEvent(event);
}

void AxoButton::mouseMoveEvent(QMouseEvent* event) {
	QPushButton::mouseMoveEvent(event);
}

#if QT_CONFIG(wheelevent)
void AxoButton::wheelEvent(QWheelEvent* event) {
	event->QEvent::accept();
}
#endif

void AxoButton::keyPressEvent(QKeyEvent* event) {
	QPushButton::keyPressEvent(event);
}

void AxoButton::keyReleaseEvent(QKeyEvent* event) {
	QPushButton::keyReleaseEvent(event);
}

void AxoButton::focusInEvent(QFocusEvent* event) {
	QPushButton::focusInEvent(event);
}

void AxoButton::focusOutEvent(QFocusEvent* event) {
	QPushButton::focusOutEvent(event);
}

void AxoButton::enterEvent(QEnterEvent* event) {
	QPushButton::enterEvent(event);
}

void AxoButton::leaveEvent(QEvent* event) {
	QPushButton::leaveEvent(event);
}

//other members: https://code.qt.io/cgit/qt/qtbase.git/tree/src/widgets/kernel/qwidget.h?h=6.11#n663