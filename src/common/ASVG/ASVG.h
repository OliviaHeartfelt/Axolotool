#pragma once
#include <QWidget>
#include <QSvgRenderer>
#include <memory>
#include <QPainter>

class ASVG : public QWidget {
    Q_OBJECT

private:
    std::unique_ptr<QSvgRenderer> svgRenderer;

public:
    explicit ASVG(const QString& iconPath, QWidget* parent = nullptr) : QWidget(parent) {
        setFixedSize(24, 24);
        setSVG(iconPath);
    }
    ASVG(const QString& iconPath, const unsigned int d, QWidget* parent = nullptr) : QWidget(parent) {
        setFixedSize(d, d);
        setSVG(iconPath);
    }
    ASVG(const QString& iconPath, const unsigned int w, const unsigned int h, QWidget* parent = nullptr) : QWidget(parent) {
        setFixedSize(w, h);
        setSVG(iconPath);
    }

    void setSize(const unsigned int w, const unsigned int h) {
        this->setFixedSize(w, h);
    }
    bool hasSVG() const {
        return svgRenderer and svgRenderer->isValid();
    }
    bool setSVG(const QString& iconPath, bool override = true) {
        if (not override and svgRenderer and not svgRenderer->isValid())
            return false;

        svgRenderer = std::make_unique<QSvgRenderer>(iconPath, this);

        if (not svgRenderer or not svgRenderer->isValid())
            return false;

        update();
        return true;
    }
    void fillPath() {

    }

protected:
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        if (svgRenderer and svgRenderer->isValid()) {
            svgRenderer->render(&painter, rect());
        }
        else {
            painter.setBrush(Qt::darkGray);
            painter.drawEllipse(rect().adjusted(2, 2, -2, -2));
        }
    }
};