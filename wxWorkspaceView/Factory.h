/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#pragma once

#include "Node.h"
#include "Cable.h"

namespace WorkspaceView
{
	/**
	 * The workspace factory should be used to create nodes/cables/etc.
	 */
	class Factory
	{
	public:
		/**
		 * Creates a node from the factory.
		 */
		virtual Node* CreateNode(const wxRect& Area, int InputCount, int OutputCount, const wxString& Title) = 0;
		/**
		 * Creates a cable from the factory.
		 */
		virtual Cable* CreateCable(Node* Input, Node* Output, int FromIndex, int ToIndex) = 0;
	};
}