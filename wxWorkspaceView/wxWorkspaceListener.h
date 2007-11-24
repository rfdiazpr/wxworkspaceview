/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#pragma once

#include <vector>
#include "wxWorkspaceCable.h"
#include "wxWorkspaceItem.h"

class WorkspaceListener
{
public:
	virtual void OnConnectCable(WorkspaceCable* Cable);
	virtual void OnDeleteItemPre(WorkspaceItem* Item);
	virtual void OnDeleteItemPost(WorkspaceItem* Item);
	virtual void OnSelectedItems(std::vector<WorkspaceItem*> Items);
	virtual bool ApproveCable(WorkspaceCable* Cable);
};