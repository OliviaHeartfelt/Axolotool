#pragma once

namespace VWCanvasDetails::CanvasView {

    class CanvasView : public QGraphicsView {
        Q_OBJECT

        bool    leftPanning = false;
        QPointF leftLastPos;

        bool    middlePanning = false;
        QPointF middleOrigin;
        QPointF middleVelocity;
        QTimer* middlePanTimer;

        static constexpr float PAN_A = 0.01f;
        static constexpr float PAN_B = 0.5f;
        static constexpr float MIDDLE_DEADZONE = 8.0f;

        float speedCurve(float x, float a, float b) {
            float magnitude = std::abs(x);
            float speed = (a * magnitude * magnitude + b * magnitude) / 2.0f;
            return std::copysign(speed, x);
        }

    public:
        explicit CanvasView(QGraphicsScene* scene, QWidget* parent = nullptr)
            : QGraphicsView(scene, parent) {

            setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            setRenderHint(QPainter::Antialiasing);
            setDragMode(QGraphicsView::NoDrag);
            setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
            setResizeAnchor(QGraphicsView::AnchorUnderMouse);

            middlePanTimer = new QTimer(this);
            middlePanTimer->setInterval(16);
            connect(middlePanTimer, &QTimer::timeout,
                this, &CanvasView::onMiddlePanTick);
        }

    protected:
        void mousePressEvent(QMouseEvent* event) override {
            if (event->button() == Qt::LeftButton) {
                leftPanning = true;
                leftLastPos = event->position();
                setCursor(Qt::ClosedHandCursor);
                event->accept();
                return;
            }
            if (event->button() == Qt::MiddleButton) {
                middlePanning = true;
                middleOrigin = event->position();
                middleVelocity = {};
                middlePanTimer->start();
                setCursor(Qt::SizeAllCursor);
                event->accept();
                return;
            }
            QGraphicsView::mousePressEvent(event);
        }

        void mouseMoveEvent(QMouseEvent* event) override {
            if (leftPanning) {
                QPointF delta = event->position() - leftLastPos;
                leftLastPos = event->position();

                horizontalScrollBar()->setValue(
                    horizontalScrollBar()->value() - static_cast<int>(delta.x()));
                verticalScrollBar()->setValue(
                    verticalScrollBar()->value() - static_cast<int>(delta.y()));

                event->accept();
                return;
            }
            if (middlePanning) {
                QPointF diff = event->position() - middleOrigin;

                if (QLineF({ 0,0 }, diff).length() < MIDDLE_DEADZONE)
                    middleVelocity = {};
                else
                    middleVelocity = diff;

                event->accept();
                return;
            }
            QGraphicsView::mouseMoveEvent(event);
        }
        void mouseReleaseEvent(QMouseEvent* event) override {
            if (event->button() == Qt::LeftButton && leftPanning) {
                leftPanning = false;
                setCursor(Qt::ArrowCursor);
                event->accept();
                return;
            }
            if (event->button() == Qt::MiddleButton && middlePanning) {
                middlePanning = false;
                middleVelocity = {};
                middlePanTimer->stop();
                setCursor(Qt::ArrowCursor);
                event->accept();
                return;
            }
            QGraphicsView::mouseReleaseEvent(event);
        }

        void wheelEvent(QWheelEvent* event) override {
            if (event->modifiers() & Qt::ControlModifier) {
                const double scaleFactor = event->angleDelta().y() > 0 ? 1.15 : 1.0 / 1.15;
                scale(scaleFactor, scaleFactor);
                event->accept();
                return;
            }
            event->accept();
        }

    private slots:
        void onMiddlePanTick() {
            if (!middlePanning || middleVelocity.isNull()) return;

            float dx = static_cast<float>(middleVelocity.x());
            float dy = static_cast<float>(middleVelocity.y());

            horizontalScrollBar()->setValue(
                horizontalScrollBar()->value() +
                static_cast<int>(speedCurve(dx, PAN_A, PAN_B)));

            verticalScrollBar()->setValue(
                verticalScrollBar()->value() +
                static_cast<int>(speedCurve(dy, PAN_A, PAN_B)));
        }
    };
}