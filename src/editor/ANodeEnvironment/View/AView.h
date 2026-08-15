#pragma once

#include "canvas/VWCanvas.h"
#include "node/VWNode.h"
#include "pin/VWPin.h"
#include "widget/VWWidget.h"
#include "wire/VWWire.h"

namespace AView {

	namespace Context {
		namespace Node   { using namespace VWNode::Context; }
		namespace Pin    { using namespace VWPin::Context; }
		namespace Widget { using namespace VWWidget::Context; }
		namespace Wire   { using namespace VWWire::Context; }
	}

	namespace Canvas { using namespace VWCanvas; }
	namespace Node   { using namespace VWNode::Node; }
	namespace Pin    { using namespace VWPin; }
	namespace Wire   { using namespace VWWire::PermanentWire; }
}