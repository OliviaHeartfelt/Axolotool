#pragma once

#include <QPointF>
#include <QDataStream>
#include <QDebug>

namespace AWireData {
    struct WireData {
        QPointF pos{};
        qreal dgrOrigin = 0;
        qreal dgrTargetDefault = 180;

        void debug() {
            qDebug().nospace()
                << "pos: " << pos << "\n"
                << "dgrOrigin: " << dgrOrigin << "\n"
                << "dgrTargetDefault: " << dgrTargetDefault << "\n";
        }
    };

    inline QDataStream& operator<<(QDataStream& out, const WireData& data) {
        out << data.pos;
        return out;
    }

    inline QDataStream& operator>>(QDataStream& in, WireData& data) {
        in >> data.pos;
        return in;
    }
}