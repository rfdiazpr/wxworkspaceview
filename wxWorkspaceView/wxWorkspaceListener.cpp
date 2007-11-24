/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#include "wxWorkspaceListener.h"

void WorkspaceListener::OnConnectCable(WorkspaceCable* Cable)
{
}

void WorkspaceListener::OnDeleteItemPre(WorkspaceItem* Item)
{
}

void WorkspaceListener::OnDeleteItemPost(WorkspaceItem* Item)
{
}

void WorkspaceListener::OnSelectedItems(std::vector<WorkspaceItem*> Items)
{
}

bool WorkspaceListener::ApproveCable(WorkspaceCable* Cable)
{
	return true;
}