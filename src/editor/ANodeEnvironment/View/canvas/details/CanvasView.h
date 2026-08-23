#pragma once

#include "../../../Registry/ARegistry.h"
#include "../../node/VWNode.h"

namespace VWCanvasDetails::CanvasView {

    class CanvasView : public QGraphicsView {
        Q_OBJECT

        ARegistry::Registry* m_registry = nullptr;

        bool    leftPanning = false;
        QPointF leftLastPos;

        bool    middlePanning = false;
        QPointF middleOrigin;
        QPointF middleVelocity;
        QTimer* middlePanTimer;

        QBrush m_gridBrush;

        static constexpr float PAN_A = 0.01f;
        static constexpr float PAN_B = 0.5f;
        static constexpr float MIDDLE_DEADZONE = 8.0f;

        float speedCurve(float x, float a, float b) {
            float speed = (a * x * x + b * std::abs(x)) / 2.0f;
            return std::copysign(speed, x);
        }

        void deleteSelectedNodes() {
            if (!m_registry) return;

            QList<QGraphicsItem*> selected = scene()->selectedItems();
            if (selected.isEmpty()) return;

            for (QGraphicsItem* item : selected) {
                auto* node = dynamic_cast<VWNode::NodeItem::Node*>(item);
                if (!node) continue;

                for (auto* pin : node->pins()) {
                    if (pin) {
                        m_registry->nodeView.pinViewRegistry.hide(pin->id());
                        qDebug() << "> Pin Removed! #Pins:" << m_registry->nodeView.pinViewRegistry.sizeVisible() + 1 << "->" << m_registry->nodeView.pinViewRegistry.sizeVisible();
                        pin->removeAllWires();
                    }
                }

                for (auto* cell : node->cells()) {
                    if (cell) {
                        m_registry->nodeView.cellViewRegistry.hide(cell->id());
                        qDebug() << "> Cell Removed! #Cells:" << m_registry->nodeView.cellViewRegistry.sizeVisible() + 1 << "->" << m_registry->nodeView.cellViewRegistry.sizeVisible();
                    }
                }

                m_registry->nodeView.nodeViewRegistry.hide(node->id());
                qDebug() << "> Node Removed! #Nodes:" << m_registry->nodeView.nodeViewRegistry.sizeVisible() + 1 << "->" << m_registry->nodeView.nodeViewRegistry.sizeVisible();
                scene()->removeItem(node);
            }
        }

    public:
        explicit CanvasView(QGraphicsScene* scene, ARegistry::Registry* registry, QWidget* parent = nullptr)
            : QGraphicsView(scene, parent), m_registry(registry) 
        {

            QSurfaceFormat format;
            format.setSwapInterval(0);
            format.setSamples(8);
            
            auto* openGLWidget = new QOpenGLWidget(this);
            openGLWidget->setFormat(format);
            
            this->setViewport(openGLWidget);
            
            setRenderHint(QPainter::Antialiasing, true);
            setRenderHint(QPainter::SmoothPixmapTransform, true);

            setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
            setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            setRenderHint(QPainter::Antialiasing);
            setDragMode(QGraphicsView::NoDrag);
            setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
            setResizeAnchor(QGraphicsView::AnchorUnderMouse);

            int gridStep = 32;
            QPixmap tile(gridStep, gridStep);
            tile.fill(QColor("#1e1e1e"));

            QPainter tilePainter(&tile);
            tilePainter.setPen(QColor("#3a3a3a"));
            tilePainter.drawPoint(0, 0);

            m_gridBrush = QBrush(tile);

            middlePanTimer = new QTimer(this);
            middlePanTimer->setInterval(16);
            connect(middlePanTimer, &QTimer::timeout,
                this, &CanvasView::onMiddlePanTick);
        }

    signals:
        void zoomChanged(int percentage);
        void posChanged();

    protected:
        void mousePressEvent(QMouseEvent* event) override {
            if (event->button() == Qt::LeftButton) {
                if (itemAt(event->position().toPoint())) {
                    QGraphicsView::mousePressEvent(event);
                    return;
                }
                scene()->clearSelection();

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
        void mouseMoveEvent(QMouseEvent* event) override {
            if (leftPanning) {
                QPointF delta = event->position() - leftLastPos;
                leftLastPos = event->position();

                horizontalScrollBar()->setValue(
                    horizontalScrollBar()->value() - static_cast<int>(delta.x()));
                verticalScrollBar()->setValue(
                    verticalScrollBar()->value() - static_cast<int>(delta.y()));

                event->accept();

                emit posChanged();
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

        void wheelEvent(QWheelEvent* event) override {
            if (event->modifiers() & Qt::ControlModifier) {
                const double scaleFactor = event->angleDelta().y() > 0 ? 1.15 : 1.0 / 1.15;
                scale(scaleFactor, scaleFactor);

                int currentZoom = static_cast<int>(transform().m11() * 100);

                emit zoomChanged(currentZoom);

                event->accept();
                emit posChanged();

                return;
            }
            event->accept();
        }

        void keyPressEvent(QKeyEvent* event) override {
            if (m_registry && event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
                deleteSelectedNodes();
                event->accept();
                return;
            }
            QGraphicsView::keyPressEvent(event);
        }

        void drawBackground(QPainter* painter, const QRectF& rect) {
            painter->fillRect(rect, m_gridBrush);
        }

    private slots:
        void onMiddlePanTick() {
            if (!middlePanning || middleVelocity.isNull()) return;

            float dx = static_cast<float>(middleVelocity.x());
            float dy = static_cast<float>(middleVelocity.y());

            horizontalScrollBar()->setValue(
                horizontalScrollBar()->value() +
                static_cast<int>(speedCurve(dx, PAN_A, PAN_B))
            );

            verticalScrollBar()->setValue(
                verticalScrollBar()->value() +
                static_cast<int>(speedCurve(dy, PAN_A, PAN_B))
            );

            emit posChanged();
        }
    };
}