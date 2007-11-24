/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#pragma once

#include <vector>
#include "Cable.h"
#include "Item.h"

namespace WorkspaceView
{
	class Listener
	{
	public:
		virtual void OnConnectCable(Cable* Cable);
		virtual void OnDeleteItemPre(Item* Item);
		virtual void OnDeleteItemPost(Item* Item);
		virtual void OnSelectedItems(std::vector<Item*> Items);
		virtual bool ApproveCable(Cable* Cable);
	};
}