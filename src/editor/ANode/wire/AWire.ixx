module;

#include "AWireData.h"
#include "AWireItem.h"
#include "AWireTemp.h"

export module AWire;

export namespace AWire {
	using WireData = ::AWireData::WireData;
	using WireItem = ::AWireItem::WireItem;
	using WireTemp = ::AWireTemp::WireTemp;
}