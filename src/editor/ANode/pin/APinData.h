#pragma once

#include <QDataStream>
#include <QDebug>

namespace APinData {
    struct PinData {
        qint32 originID = -10; // Unique identifier for the source node
        qint16 pinID = -1;    // Index or ID of the specific pin on that node
        QString flow = "";    // Flow direction ("in", "out", "up", "down", "dgr45", etc.)
        QString type = "";    // Data type ("float", "int", "generic", etc.)
        QByteArray payload;

        void debug() {
            qDebug().nospace()
                << "originID: " << originID << "\n"
                << "pinID:    " << pinID << "\n"
                << "flow:     " << flow << "\n"
                << "type:     " << type << "\n";
        }
    };

    inline QDataStream& operator<<(QDataStream& out, const PinData& data) {
        out << data.originID << data.pinID << data.flow << data.type << data.payload;
        return out;
    }

    inline QDataStream& operator>>(QDataStream& in, PinData& data) {
        in >> data.originID >> data.pinID >> data.flow >> data.type >> data.payload;
        return in;
    }
}