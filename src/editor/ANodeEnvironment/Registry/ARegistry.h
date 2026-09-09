#pragma once

#include "RGRegistryComponents.h"

namespace ARegistry {

    template<typename T>
    using RegistryT = RGBaseRegistry::BaseRegistry<muuid::uuid, T>;

    struct Registry {
        RGRegistryComponents::NodeRegisters node;
        RGRegistryComponents::NodeViewRegisters nodeView;
        RGRegistryComponents::NodeFunctionRegistry nodeFunction;

        RGRegistryComponents::WireRegisters wire;
        RGRegistryComponents::WireViewRegisters wireView;
        RGRegistryComponents::WireFunctionRegistry wireFunction;
    };
}