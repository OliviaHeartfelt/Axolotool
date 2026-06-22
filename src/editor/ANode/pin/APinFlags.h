#pragma once

namespace APinFlags {

    struct PinFlags {
        uint64_t defaultAllowFlowValue : 1;
        uint64_t defaultAllowTypeValue : 1;
        uint64_t allowMultipleWires : 1;
        uint64_t isRequired : 1;

        uint64_t priority : 4;

        uint64_t isColorDirty : 1;

        uint64_t customFlags : 55;

        PinFlags() :
            defaultAllowFlowValue(1),
            defaultAllowTypeValue(1),
            allowMultipleWires(0),
            isRequired(1),

            priority(0),

            isColorDirty(1),

            customFlags(0)
        {
        }
    };
    static_assert(sizeof(PinFlags) == 8);
}