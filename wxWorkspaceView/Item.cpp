/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#include "Item.h"

namespace WorkspaceView
{
	Item::Item(EWorkspaceItemType ItemType, const wxRect& Area)
		: ItemType(ItemType), Area(Area), Selected(false), ReadOnly(false)
	{
	}

	bool Item::ShouldDeleteOnSelectionDeletion() const
	{
		return Selected && !ReadOnly;
	}

	bool Item::HasHitWithConnector(const wxPoint& Position, ConnectorInfo* Result)
	{
		return false;
	}

	void Item::ReactToItemsMoved(std::vector<Item*> ItemsThatWereMoved)
	{
	}

	bool Item::IsSelected() const
	{
		return Selected;
	}

	void Item::Select()
	{
		Selected = true;
	}

	void Item::Deselect()
	{
		Selected = false;
	}

	void Item::ToggleSelection()
	{
		Selected = !Selected;
	}

	bool Item::IsReadOnly() const
	{
		return ReadOnly;
	}

	void Item::SetReadOnly(bool Value)
	{
		ReadOnly = Value;
	}

	const wxRect& Item::GetArea() const
	{
		return Area;
	}

	void Item::SetArea(const wxRect& NewArea)
	{
		Area = NewArea;
	}

	void Item::SetPosition(const wxPoint& Position)
	{
		Area.x = Position.x;
		Area.y = Position.y;
	}

	bool Item::IntersectsWith(float Left, float Top, float Right, float Bottom) const
	{
		wxRect Rect(Left, Top, Right - Left, Bottom - Top);

		return IntersectsWith(Rect);
	}

	bool Item::IntersectsWith(const wxRect& Rect) const
	{
		return Area.Intersects(Rect);
	}

	void Item::OnZoom(float NewZoom)
	{
	}

	EWorkspaceItemType Item::GetItemType() const
	{
		return ItemType;
	}
}