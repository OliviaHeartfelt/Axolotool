#include "APushButton.h"
#include <QEnterEvent>
#include <QWheelEvent>
#include <QWidget>

#include <QApplication>

void APushButton::setupTimers() {
	holdTimer = new QTimer(this);
	holdTimer->setSingleShot(true);

	connect(holdTimer, &QTimer::timeout, this, [this]() {
		onHold();
	});
}


APushButton::APushButton(QWidget* parent) {
	this->parent = parent;
	setupTimers();
}
APushButton::APushButton(const QString& text, QWidget* parent) {
	this->setText(text);
	this->parent = parent;
	setupTimers();
}
APushButton::APushButton(const QIcon& icon, const QString& text, QWidget* parent) {
	this->setIcon(icon);
	this->setText(text);
	this->parent = parent;
	setupTimers();
}


void APushButton::mousePressEvent(QMouseEvent* event) {
	onPress(event);
	if (holdTimer) 
		holdTimer->start(500);
	QPushButton::mousePressEvent(event);
}

void APushButton::mouseReleaseEvent(QMouseEvent* event) {
	if (holdTimer && holdTimer->isActive()) 
		holdTimer->stop();
	onRelease(event);
	QPushButton::mouseReleaseEvent(event);
}

void APushButton::mouseDoubleClickEvent(QMouseEvent* event) {
	if (onDoubleClick.hasFunction()) {
		onDoubleClick(event);
		return;
	}
	QPushButton::mouseDoubleClickEvent(event);
}


void APushButton::enterEvent(QEnterEvent* event) {
	onEnter(event);
	QPushButton::enterEvent(event);
}

void APushButton::leaveEvent(QEvent* event) {
	if (holdTimer && holdTimer->isActive()) 
		holdTimer->stop();
	onLeave(event);
	QPushButton::leaveEvent(event);
}

//other members: https://code.qt.io/cgit/qt/qtbase.git/tree/src/widgets/kernel/qwidget.h?h=6.11#n663