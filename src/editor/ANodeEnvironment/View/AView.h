#pragma once

#include "canvas/VWCanvas.h"
#include "cell/VWCell.h"
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
	namespace Cell   { using namespace VWCell; }
	namespace Node   { using namespace VWNode; }
	namespace Pin    { using namespace VWPin; }
	namespace Wire   { using namespace VWWire::PermanentWire; }
}