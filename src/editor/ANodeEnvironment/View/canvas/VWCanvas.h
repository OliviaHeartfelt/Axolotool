#pragma once

#include "../../Registry/ARegistry.h"
#include "../scene/VWNodeScene.h"
#include "details/CanvasView.h"

namespace VWCanvas {

    class VWCanvas : public QWidget {
        using CanvasView = VWCanvasDetails::CanvasView::CanvasView;
        using NodeScene = VWNodeScene::NodeScene;

        ARegistry::Registry* m_registry = nullptr;
        NodeScene* scene = nullptr;
        CanvasView* view = nullptr;

        QLabel* zoomLabel = nullptr;
        QLabel* posLabel = nullptr;

        QLabel* keyLabel = nullptr;
        QTimer* keyFadeTimer = nullptr;
        QGraphicsOpacityEffect* keyOpacityEffect = nullptr;
        QPropertyAnimation* keyFadeAnimation = nullptr;

    public:
        explicit VWCanvas(ARegistry::Registry* registry, QWidget* parent = nullptr) : QWidget(parent), m_registry(registry) {
            if (!m_registry) return;

            scene = new NodeScene(this);
            if (!scene) return;

            view =  new CanvasView(scene, m_registry, this);
            if (!view) return;

            scene->setSceneRect(-10000, -10000, 20000, 20000);

            view->setRenderHint(QPainter::Antialiasing);
            view->setAcceptDrops(true);
            view->setDragMode(QGraphicsView::RubberBandDrag);

            view->setFocusPolicy(Qt::StrongFocus);
            view->installEventFilter(this);
            if (view->viewport()) {
                view->viewport()->installEventFilter(this);
            }

            // --- Keycaster Overlay (Top-Left) ---
            keyLabel = new QLabel(this);
            keyLabel->setStyleSheet(
                "color: #ffffff;"
                "border: 1px solid #88ffffff;"
                "border-radius: 4px;"
                "padding: 6px 10px;"
                "font-weight: bold;"
                "font-size: 13px;"
            );
            keyLabel->move(10, 10);
            keyLabel->hide();

            keyOpacityEffect = new QGraphicsOpacityEffect(keyLabel);
            keyLabel->setGraphicsEffect(keyOpacityEffect);

            keyFadeAnimation = new QPropertyAnimation(keyOpacityEffect, "opacity", this);
            keyFadeAnimation->setDuration(350);
            keyFadeAnimation->setStartValue(1.0);
            keyFadeAnimation->setEndValue(0.0);

            connect(keyFadeAnimation, &QPropertyAnimation::finished, this, [this]() {
                if (keyOpacityEffect->opacity() == 0.0) {
                    keyLabel->hide();
                }
                });

            keyFadeTimer = new QTimer(this);
            keyFadeTimer->setSingleShot(true);
            connect(keyFadeTimer, &QTimer::timeout, this, [this]() {
                keyFadeAnimation->stop();
                keyFadeAnimation->start();
                });


            // --- Zoom Label (Top-Right) ---
            zoomLabel = new QLabel("100%", this);
            if (zoomLabel) {
                zoomLabel->setStyleSheet("color: white; background: rgba(0,0,0,128); padding: 4px;");

                zoomLabel->adjustSize();
                zoomLabel->move(width() - zoomLabel->width() - 10, 10);

                connect(view, &CanvasView::zoomChanged, this, [this](int percentage) {
                    zoomLabel->setText(QString::number(percentage) + "%");
                    zoomLabel->adjustSize();
                    zoomLabel->move(width() - zoomLabel->width() - 10, 10);
                });
            }

            // --- Position Label (Top-Right) ---
            posLabel = new QLabel("x: 0, y: 0", this);
            if (posLabel) {
                posLabel->setStyleSheet("color: white; background: rgba(0,0,0,128); padding: 4px;");

                posLabel->adjustSize();
                posLabel->move(width() - posLabel->width() - 10, 40);

                connect(view, &CanvasView::posChanged, this, [this]() {
                    if (!view->viewport()) return;

                    QPointF pos = view->mapToScene(view->viewport()->rect().center());
                    int cartesianX = qRound(pos.x());
                    int cartesianY = qRound(-pos.y());

                    posLabel->setText(QString("x: %1, y: %2").arg(cartesianX).arg(cartesianY));
                    posLabel->adjustSize();
                    posLabel->move(width() - posLabel->width() - 10, 40);
                    });
            }

            auto* layout = new QVBoxLayout(this);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->addWidget(view);
            setLayout(layout);
        }

        QGraphicsScene* graphicsScene() const { return scene; }
        CanvasView* graphicsView()  const { return view; }

protected:
    bool eventFilter(QObject* watched, QEvent* event) override {

        if (!view->viewport()) return QWidget::eventFilter(watched, event);

        if (event->type() == QEvent::KeyPress) {
            auto* keyEvent = static_cast<QKeyEvent*>(event);
            QString shortcutText = formatKeyEvent(keyEvent);

            if (!shortcutText.isEmpty()) {
                showKeycasterText(shortcutText);
            }
        }
        else if (event->type() == QEvent::MouseButtonPress) {
            auto* mouseEvent = static_cast<QMouseEvent*>(event);
            QString text = formatMouseEvent(mouseEvent);
            if (!text.isEmpty()) {
                showKeycasterText(text);
            }
        }
        else if (event->type() == QEvent::MouseButtonDblClick) {
            auto* mouseEvent = static_cast<QMouseEvent*>(event);
            QString text = formatDoubleClickEvent(mouseEvent);
            if (!text.isEmpty()) {
                showKeycasterText(text);
            }
        }
        else if (event->type() == QEvent::Wheel) {
            auto* wheelEvent = static_cast<QWheelEvent*>(event);
            QString text = formatWheelEvent(wheelEvent);
            if (!text.isEmpty()) {
                showKeycasterText(text);
            }
        }

        return QWidget::eventFilter(watched, event);
    }

    void resizeEvent(QResizeEvent* event) override {
        QWidget::resizeEvent(event);

        if (zoomLabel) {
            zoomLabel->move(width() - zoomLabel->width() - 10, 10);
        }
        if (posLabel) {
            posLabel->move(width() - posLabel->width() - 10, 40);
        }
    }

    private:
        void showKeycasterText(const QString& text) {
            if (!keyLabel) return;

            keyFadeTimer->stop();
            keyFadeAnimation->stop();

            keyOpacityEffect->setOpacity(1.0);
            keyLabel->setText(text);
            keyLabel->adjustSize();
            keyLabel->show();

            keyFadeTimer->start(750);
        }

        QString formatKeyEvent(QKeyEvent* keyEvent) const {
            int key = keyEvent->key();
            Qt::KeyboardModifiers mods = keyEvent->modifiers();

            if (key == Qt::Key_Control || key == Qt::Key_Shift ||
                key == Qt::Key_Alt || key == Qt::Key_Meta) {
                return QString();
            }

            QStringList parts;
            if (mods & Qt::ControlModifier) parts << "Ctrl";
            if (mods & Qt::ShiftModifier)   parts << "Shift";
            if (mods & Qt::AltModifier)     parts << "Alt";
            if (mods & Qt::MetaModifier)    parts << "Cmd";

            QString keyString = QKeySequence(key).toString();
            if (!keyString.isEmpty()) {
                parts << keyString;
            }

            return parts.join(" + ");
        }

        QString formatDoubleClickEvent(QMouseEvent* mouseEvent) const {
            QStringList parts;
            Qt::KeyboardModifiers mods = mouseEvent->modifiers();

            if (mods & Qt::ControlModifier) parts << "Ctrl";
            if (mods & Qt::ShiftModifier)   parts << "Shift";
            if (mods & Qt::AltModifier)     parts << "Alt";
            if (mods & Qt::MetaModifier)    parts << "Cmd";

            parts << "2x LMB";
            return parts.join(" + ");
        }

        QString formatMouseEvent(QMouseEvent* mouseEvent) const {
            QStringList parts;
            Qt::KeyboardModifiers mods = mouseEvent->modifiers();

            if (mods & Qt::ControlModifier) parts << "Ctrl";
            if (mods & Qt::ShiftModifier)   parts << "Shift";
            if (mods & Qt::AltModifier)     parts << "Alt";
            if (mods & Qt::MetaModifier)    parts << "Cmd";

            switch (mouseEvent->button()) {
            case Qt::LeftButton:   parts << "LMB"; break;
            case Qt::RightButton:  parts << "RMB"; break;
            case Qt::MiddleButton: parts << "MMB"; break;
            case Qt::BackButton:   parts << "Mouse Back"; break;
            case Qt::ForwardButton:parts << "Mouse Forward"; break;
            default: return QString();
            }

            return parts.join(" + ");
        }

        QString formatWheelEvent(QWheelEvent* wheelEvent) const {
            QStringList parts;
            Qt::KeyboardModifiers mods = wheelEvent->modifiers();

            if (mods & Qt::ControlModifier) parts << "Ctrl";
            if (mods & Qt::ShiftModifier)   parts << "Shift";
            if (mods & Qt::AltModifier)     parts << "Alt";

            int delta = wheelEvent->angleDelta().y();
            if (delta > 0) {
                parts << "Wheel Up";
            }
            else if (delta < 0) {
                parts << "Wheel Down";
            }
            else {
                return QString();
            }

            return parts.join(" + ");
        }
    };
}