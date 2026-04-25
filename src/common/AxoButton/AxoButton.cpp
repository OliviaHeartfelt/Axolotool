#include "AxoButton.h"
//#include <QEnterEvent>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QApplication>

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


//QPropertyAnimation* anim = new QPropertyAnimation(this, "geometry");
//anim->setDuration(100); // 100 milliseconds
//anim->setStartValue(geometry());
//anim->setEndValue(geometry().adjusted(-2, -2, 2, 2)); // Slightly "pop" out
//anim->start(QAbstractAnimation::DeleteWhenStopped);

//// Create a color using HSV: Hue (0-359), Saturation (0-255), Value (0-255)
//QColor hoverColor = QColor::fromHsv(180, 200, 255);
//
//// Convert it to an RGBA string for the Style Sheet
//QString style = QString("background-color: %1;").arg(hoverColor.name(QColor::HexArgb));
//setStyleSheet(style);

//setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #444, stop:1 #222); ");
//setStyleSheet(":hover { background-color: #333; }");
//setStyleSheet(":pressed { background-color: #333; }");