/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#include "Listener.h"

namespace WorkspaceView
{
	void Listener::OnConnectCable(Cable* Cable)
	{
	}

	void Listener::OnDeleteItemPre(Item* Item)
	{
	}

	void Listener::OnDeleteItemPost(Item* Item)
	{
	}

	void Listener::OnSelectedItems(std::vector<Item*> Items)
	{
	}

	void OnZoom(float NewZoomValue)
	{
	}

	bool Listener::ApproveCable(Cable* Cable)
	{
		return true;
	}
}