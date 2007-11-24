/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#include "wxWorkspaceItem.h"

WorkspaceItem::WorkspaceItem(EWorkspaceItemType ItemType, const wxRect& Area)
: ItemType(ItemType), Area(Area), Selected(false), ReadOnly(false)
{
}

bool WorkspaceItem::ShouldDeleteOnSelectionDeletion() const
{
	return Selected && !ReadOnly;
}

bool WorkspaceItem::HasHitWithConnector(const wxPoint& Position, ConnectorInfo* Result)
{
	return false;
}

void WorkspaceItem::ReactToItemsMoved(std::vector<WorkspaceItem*> ItemsThatWereMoved)
{
}

bool WorkspaceItem::IsSelected() const
{
	return Selected;
}

void WorkspaceItem::Select()
{
	Selected = true;
}

void WorkspaceItem::Deselect()
{
	Selected = false;
}

void WorkspaceItem::ToggleSelection()
{
	Selected = !Selected;
}

bool WorkspaceItem::IsReadOnly() const
{
	return ReadOnly;
}

void WorkspaceItem::SetReadOnly(bool Value)
{
	ReadOnly = Value;
}

const wxRect& WorkspaceItem::GetArea() const
{
	return Area;
}

void WorkspaceItem::SetArea(const wxRect& NewArea)
{
	Area = NewArea;
}

void WorkspaceItem::SetPosition(const wxPoint& Position)
{
	Area.x = Position.x;
	Area.y = Position.y;
}

bool WorkspaceItem::IntersectsWith(float Left, float Top, float Right, float Bottom) const
{
	wxRect Rect(Left, Top, Right - Left, Bottom - Top);

	return IntersectsWith(Rect);
}

bool WorkspaceItem::IntersectsWith(const wxRect& Rect) const
{
	return Area.Intersects(Rect);
}

EWorkspaceItemType WorkspaceItem::GetItemType() const
{
	return ItemType;
}