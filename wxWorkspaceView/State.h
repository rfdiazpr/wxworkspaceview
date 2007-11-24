/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#pragma once

#ifdef WX_GCH
	#include <wx_pch.h>
#else
	#include <wx/wx.h>
#endif

namespace WorkspaceView
{
	/**
	 * This class represents the current state of the workspace view.
	 */
	class State
	{
	public:
		/**
		 * Initializes a new instance of this class.
		 */
		State();
	public:
		/**
		 * Converts the rectangle to the space used on the control.
		 */
		wxRect WorldToScreen(const wxRect& r) const;
		/**
		 * Converts the rectangle to screen space.
		 */
		wxRect ScreenToWorld(const wxRect& r) const;
		/**
		 * Converts the point to the space used on the control.
		 */
		wxPoint WorldToScreen(const wxPoint& p) const;
		/**
		 * Converts the point to screen space.
		 */
		wxPoint ScreenToWorld(const wxPoint& p) const;
	public:
		/**
		 * Gets or sets the zoom factor.
		 */
		float ZoomFactor;
		/**
		 * Gets or sets the offset to start drawing at.
		 */
		wxPoint Offset;
	};
}