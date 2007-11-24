/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#include "State.h"

namespace WorkspaceView
{
	State::State()
	{
		ZoomFactor = 1.f;
		Offset = wxPoint(0, 0);
	}

	wxRect State::WorldToScreen(const wxRect& r) const
	{
		wxRect result(
			(int)(r.x * ZoomFactor) + Offset.x,
			(int)(r.y * ZoomFactor) + Offset.y,
			(int)(r.width * ZoomFactor),
			(int)(r.height * ZoomFactor));

		return result;
	}

	wxRect State::ScreenToWorld(const wxRect& r) const
	{
		wxRect result(
			(int)((r.x - Offset.x) / ZoomFactor),
			(int)((r.y - Offset.y) / ZoomFactor),
			(int)(r.width / ZoomFactor),
			(int)(r.height / ZoomFactor));

		return result;
	}

	wxPoint State::WorldToScreen(const wxPoint& p) const
	{
		wxPoint result(
			(int)(p.x * ZoomFactor) + Offset.x,
			(int)(p.y * ZoomFactor) + Offset.y);

		return result;
	}

	wxPoint State::ScreenToWorld(const wxPoint& p) const
	{
		wxPoint result(
			(int)((p.x - Offset.x) / ZoomFactor),
			(int)((p.y - Offset.y) / ZoomFactor));

		return result;
	}
}