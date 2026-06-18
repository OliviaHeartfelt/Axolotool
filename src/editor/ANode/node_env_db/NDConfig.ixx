export module NDConfig;

export namespace NDConfig {

    struct CellSpawnInfo {
        QString cellType;
        short row;
        short col;
        short rowSpan = 1;
        short colSpan = 1;
    };
}