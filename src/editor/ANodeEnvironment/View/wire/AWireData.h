#pragma once

namespace AWireData {

    struct Data {
        QPointF pos{};
        qreal dgrOrigin = 0;
        qreal dgrTargetDefault = 180;

        void debug() {
            qDebug().nospace()
                << "pos: " << pos << "\n"
                << "dgrOrigin: " << dgrOrigin << "\n"
                << "dgrTargetDefault: " << dgrTargetDefault << "\n";
        }

        friend inline QDataStream& operator<<(QDataStream& out, const Data& data) { return out << data.pos; }
        friend inline QDataStream& operator>>(QDataStream& in, Data& data) {        return  in >> data.pos; }
    };
}