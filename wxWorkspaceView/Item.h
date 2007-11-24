/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#pragma once

#ifdef WX_GCH
	#include <wx_pch.h>
#else
	#include <wx/wx.h>
#endif

#include <map>
#include <vector>
#include "State.h"

namespace WorkspaceView
{
	/**
	 * The RTTI values of the items.
	 */
	enum EWorkspaceItemType
	{
		WorkspaceItemTypeUnknown,
		WorkspaceItemTypeCable,
		WorkspaceItemTypeNode
	};

	class Item;

	/**
	 * Connector information
	 */
	struct ConnectorInfo
	{
		Item* Owner;
		int PortIndex;
		bool PortIsOutput;
	};

	/**
	 * The base class for all items in the workspace view.
	 */
	class Item
	{
	public:
		/**
		 * Initializes a new instance of this class.
		 */
		Item(EWorkspaceItemType ItemType, const wxRect& Area);
	public:
		/**
		 * Draws the item to the screen.
		 */
		virtual void Draw(wxPaintDC* dc, const State& State) = 0;
	public:
		/**
		 * Returns true if this item can be deleted; false otherwise.
		 */
		virtual bool ShouldDeleteOnSelectionDeletion() const;
		/**
		 * Returns true if this items hits with an connector.
		 */
		virtual bool HasHitWithConnector(const wxPoint& Position, ConnectorInfo* Result);
		/**
		 * This will be called when items are moved.
		 */
		virtual void ReactToItemsMoved(std::vector<Item*> ItemsThatWereMoved);
	public:
		/**
		 * Whether or not this item is selected.
		 */
		bool IsSelected() const;
		/**
		 * Selects this item.
		 */
		void Select();
		/**
		 * Deselects this item.
		 */
		void Deselect();
		/**
		 * Toggles the selection of this item.
		 */
		void ToggleSelection();
	public:
		/**
		 * Returns true if this item is a read only item.
		 */
		bool IsReadOnly() const;
		/**
		 * Makes this item a read only item; or not ;)
		 */
		void SetReadOnly(bool Value);
	public:
		/**
		 * Returns the area of this item.
		 */
		const wxRect& GetArea() const;
		/**
		 * Sets the area of this item.
		 */
		void SetArea(const wxRect& NewArea);
		/**
		 * Sets the position of this item.
		 */
		void SetPosition(const wxPoint& Position);
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
		 * Returns the RTTI value of this item.
		 */
		EWorkspaceItemType GetItemType() const;
	protected:
		/**
		 * The bounding box of this item ;)
		 */
		wxRect Area;
		/**
		 * Whether or not this item has been selected.
		 */
		bool Selected;
		/**
		 * Whether or not this item is a readonly item.
		 */
		bool ReadOnly;
		/**
		 * The RTTI value of this item.
		 */
		EWorkspaceItemType ItemType;
	};
}