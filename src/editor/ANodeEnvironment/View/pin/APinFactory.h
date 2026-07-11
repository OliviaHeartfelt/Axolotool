#pragma once

#include "APinAllowLists.h"
#include "APinData.h"
#include "APinItem.h"
#include "../registry/ARegistry.h"

using RKey = ::ARegistry::FRegistryKey;

namespace APinFactory {

	APinItem::PinItem* newPin(
		QGraphicsItem* parent,
		RKey flow,
		RKey type,
		RKey style,
		const QList<RKey>& flowAllowList,
		const QList<RKey>& typeAllowList
	) {
		if (!parent) return nullptr;

		auto data = std::make_shared<APinData::PinData>();
		data->flow(flow);
		data->type(type);
		data->style(style);

		auto allowLists = std::make_shared<APinAllowLists::AllowLists>();

		for (const auto& key : flowAllowList) allowLists->flow.add(key);
		for (const auto& key : typeAllowList) allowLists->type.add(key);

		APinItem::PinItem* item = new APinItem::PinItem(parent);
		item->pinData(std::move(data));
		item->allowLists(std::move(allowLists));

		return item;
	}
}