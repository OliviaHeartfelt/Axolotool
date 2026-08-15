#pragma once

namespace VWWidgetDetails::WidgetWrapper {

    class WidgetWrapper : public QGraphicsItem {
        QGraphicsItem* m_child_widget = nullptr;
        muuid::uuid m_id;
        muuid::uuid m_core_id;
        bool m_is_new = true;
        bool m_is_dirty = false;

    public:
        WidgetWrapper(QGraphicsItem* parent, QGraphicsItem* childWidget, const muuid::uuid& coreId, const std::optional<muuid::uuid>& id)
            : QGraphicsItem(parent), m_core_id(coreId)
        {
            if (id) {
                m_id = *id;
                m_is_new = false;
            }
            else {
                m_id = muuid::uuid::generate_unix_time_based();
                m_is_new = true;
            }

            if (childWidget) {
                m_child_widget = childWidget;
                m_child_widget->setParentItem(this);
            }
        }

        const muuid::uuid& id() const { return m_id; }
        const muuid::uuid& coreId() const { return m_core_id; }

        bool isUpdateNeeded() const { return m_is_dirty; }
        void setUpdate(bool value) { m_is_dirty = value; }

        bool isNew() const { return m_is_new; }
        void setIsNew(bool value) { m_is_new = value; }

        void setWidget(QGraphicsItem* newWidget) {
            if (m_child_widget == newWidget) return;

            if (m_child_widget) {
                m_child_widget->setParentItem(nullptr);
                delete m_child_widget;
            }

            m_child_widget = newWidget;
            if (m_child_widget) {
                m_child_widget->setParentItem(this);
            }
        }

        QRectF boundingRect() const override {
            return m_child_widget ? m_child_widget->boundingRect() : QRectF();
        }

        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override {}
    };
}