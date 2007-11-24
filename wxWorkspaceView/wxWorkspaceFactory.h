/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#pragma once

#include "wxWorkspaceNode.h"
#include "wxWorkspaceCable.h"

/**
 * The workspace factory should be used to create nodes/cables/etc.
 */
class wxWorkspaceFactory
{
public:
	/**
	 * Initializes a new instance of this class.
	 */
	wxWorkspaceFactory();
	/**
	 * Cleans up all resources used by this class.
	 */
	virtual ~wxWorkspaceFactory();
public:
	/**
	 * Creates a node from the factory.
	 */
	virtual WorkspaceNode* CreateNode(const wxRect& Area, int InputCount, int OutputCount, const wxString& Title) = 0;
	/**
	 * Creates a cable from the factory.
	 */
	virtual WorkspaceCable* CreateCable(WorkspaceNode *Input, WorkspaceNode *Output, int FromIndex, int ToIndex) = 0;
};