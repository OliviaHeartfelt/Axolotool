#pragma once

#include "../registry/IRegistry.h"

#include <QString>
#include <QDataStream>
#include <QDebug>

namespace APinData {
    struct PinData {
        IRegistry::FRegistryKey flow{ "", "" };
        IRegistry::FRegistryKey type{ "", "" };
        size_t typeSize = 0;
        QByteArray body;

        void debug() {
            qDebug().nospace()
                << "flow: { source: " << flow.source << ", ID: " << flow.ID << "}" << "\n"
                << "flow: { source: " << type.source << ", ID: " << type.ID << "}" << "\n";
        }
    };

    inline QDataStream& operator<<(QDataStream& out, const PinData& data) {
        out << data.flow << data.type << data.body;
        return out;
    }

    inline QDataStream& operator>>(QDataStream& in, PinData& data) {
        in >> data.flow >> data.type >> data.body;
        return in;
    }
}