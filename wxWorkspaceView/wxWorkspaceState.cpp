/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#include "wxWorkspaceState.h"

WorkspaceState::WorkspaceState()
{
	ZoomFactor = 1.f;
	Offset = wxPoint(0, 0);
}

wxRect WorkspaceState::WorldToScreen(const wxRect& r) const
{
	wxRect result(
		(int)(r.x * ZoomFactor) + Offset.x,
		(int)(r.y * ZoomFactor) + Offset.y,
		(int)(r.width * ZoomFactor),
		(int)(r.height * ZoomFactor));

	return result;
}

wxRect WorkspaceState::ScreenToWorld(const wxRect& r) const
{
	wxRect result(
		(int)((r.x - Offset.x) / ZoomFactor),
		(int)((r.y - Offset.y) / ZoomFactor),
		(int)(r.width / ZoomFactor),
		(int)(r.height / ZoomFactor));

	return result;
}

wxPoint WorkspaceState::WorldToScreen(const wxPoint& p) const
{
	wxPoint result(
		(int)(p.x * ZoomFactor) + Offset.x,
		(int)(p.y * ZoomFactor) + Offset.y);

	return result;
}

wxPoint WorkspaceState::ScreenToWorld(const wxPoint& p) const
{
	wxPoint result(
		(int)((p.x - Offset.x) / ZoomFactor),
		(int)((p.y - Offset.y) / ZoomFactor));

	return result;
}