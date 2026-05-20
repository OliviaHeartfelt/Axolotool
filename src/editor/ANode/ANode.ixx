module;

#include "detail/ANode.h"

export module ANode;

void fn() {
	ANode* node = new ANode();
	node->insertHeader(nullptr);
}