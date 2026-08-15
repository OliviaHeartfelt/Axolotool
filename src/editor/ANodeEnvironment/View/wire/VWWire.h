#pragma once

#include "details/Context.h"
#include "details/CreateWire.h"
#include "details/WireItem.h"
#include "details/WireTemp.h"

namespace VWWire {

	namespace Context {
		using namespace VWWireDetails::Context;
	}

	namespace PermanentWire {
		using namespace VWWireDetails::CreateWire;
		using namespace VWWireDetails::WireItem;
	}

	namespace TemporaryWire {
		using namespace VWWireDetails::WireTemp;
	}
}