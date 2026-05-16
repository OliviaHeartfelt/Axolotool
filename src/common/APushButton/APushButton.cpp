#include "APushButton.h"
#include <QEnterEvent>
#include <QWheelEvent>
#include <QWidget>

void APushButton::setupConnections() {
	holdTimer.setSingleShot(true);

	connect(&holdTimer, &QTimer::timeout, this, [this]() {
		onHold();
		});
}

APushButton::APushButton(QWidget* parent) {
	this->parent = parent;

	setupConnections();
}

APushButton::APushButton(const QString& text, QWidget* parent) {
	this->setText(text);
	this->parent = parent;

	setupConnections();
}

APushButton::APushButton(const QIcon& icon, const QString& text, QWidget* parent) {
	this->setIcon(icon);
	this->setText(text);
	this->parent = parent;

	setupConnections();
}

void APushButton::mousePressEvent(QMouseEvent* event) {
	onPress(event);
	holdTimer.start(500);

	QPushButton::mousePressEvent(event);
}

void APushButton::mouseReleaseEvent(QMouseEvent* event) {
	holdTimer.stop();
	onRelease(event);

	QPushButton::mouseReleaseEvent(event);
}

void APushButton::mouseDoubleClickEvent(QMouseEvent* event) {
	onDoubleClick(event);

	QPushButton::mouseDoubleClickEvent(event);
}

void APushButton::enterEvent(QEnterEvent* event) {
	onEnter(event);

	QPushButton::enterEvent(event);
}

void APushButton::leaveEvent(QEvent* event) {
	holdTimer.stop();
	onLeave(event);

	QPushButton::leaveEvent(event);
}

//other members: https://code.qt.io/cgit/qt/qtbase.git/tree/src/widgets/kernel/qwidget.h?h=6.11#n663