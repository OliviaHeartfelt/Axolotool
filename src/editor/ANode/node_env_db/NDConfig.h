#pragma once

namespace NDConfig {

    struct CellSpawnInfo {
        QString cellType;
        short row;
        short col;
        short rowSpan = 1;
        short colSpan = 1;
    };

    const int currentSchemaVersion() { return 1; }
}