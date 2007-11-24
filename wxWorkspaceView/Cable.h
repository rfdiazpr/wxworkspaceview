/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#pragma once

#include "Item.h"
#include "Node.h"

namespace WorkspaceView
{
	// An even number please. The higher the number, the 
	// more precision we have with detecting cable hits.
	const static int NUM_CABLE_SEGMENTS = 64;

	/**
	 * A cable which can be drawn between nodes.
	 * Cables should be created through the WorkspaceFactory.
	 */
	class Cable: public Item
	{
	public:
		/**
		 * Initializes a new instance of this class.
		 */
		Cable(Node* Input, Node* Output, int FromIndex, int ToIndex);
	public:
		/**
		 * Checks if this item intersects with the given rectangle.
		 */
		virtual bool IntersectsWith(float Left, float Top, float Right, float Bottom) const;
		/**
		 * Checks if this item intersects with the given rectangle.
		 */
		virtual bool IntersectsWith(const wxRect& Rect) const;
	public:
		/**
		 * Connects the cable from an input to an output node.
		 */
		void ConnectCable(const Node* Input, const Node* Output, int FromIndex, int ToIndex);
		/**
		 * Checks if we can connect to a port.
		 */
		bool CanConnectFloatingEndTo(const ConnectorInfo& ConnectInfo);
		/**
		 * Connect a port to another port.
		 */
		void ConnectFloatingEnd(const ConnectorInfo& ConnectInfo);
		/**
		 * Sets the current floating position.
		 */
		void SetFloatingPosition(const wxPoint& NewFloatingPosition);
	public:
		/**
		 * Returns true if this item can be deleted; false otherwise.
		 */
		virtual bool ShouldDeleteOnSelectionDeletion() const;
		/**
		 * This will be called when items are moved.
		 */
		virtual void ReactToItemsMoved(std::vector<Item*> ItemsThatWereMoved);
	protected:
		/**
		 * The node this cable comes from.
		 */
		const Node* Output;
		/**
		 * The node this cable is going to.
		 */
		const Node* Input;
		/**
		 * The output port of this node.
		 */
		int FromIndex;
		/**
		 * The input port of this node.
		 */
		int ToIndex;
	protected:
		const static float CABLE_CURVEPOINT_OFFSET;
	protected:
		wxPoint PositionBeg;
		wxPoint PositionEnd;
		wxPoint PositionCurve1;
		wxPoint PositionCurve2;
		wxPoint FloatingPosition;
	protected:
		struct LineSegment
		{
			wxPoint Position;
		};
		LineSegment SegmentList[NUM_CABLE_SEGMENTS];
	};
}