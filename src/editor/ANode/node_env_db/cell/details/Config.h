#pragma once

namespace NDCellDetails::Config {

    struct Record {
        muuid::uuid id;
        muuid::uuid nodeId;
        short row = -1;
        short col = -1;
        short rowSpan = 0;
        short colSpan = 0;
        QString cellType = "";
    };
    struct RecordInfo {
        short row = -1;
        short col = -1;
        short rowSpan = 0;
        short colSpan = 0;
        QString cellType = "";
    };
}