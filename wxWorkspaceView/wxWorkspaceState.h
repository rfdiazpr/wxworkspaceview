/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#pragma once

#ifdef WX_GCH
	#include <wx_pch.h>
#else
	#include <wx/wx.h>
#endif

/**
 * This class represents the current state of the workspace view.
 */
class WorkspaceState
{
public:
	/**
	 * Initializes a new instance of this class.
	 */
	WorkspaceState();
public:
	wxRect WorldToScreen(const wxRect& r) const;
	wxRect ScreenToWorld(const wxRect& r) const;
	wxPoint WorldToScreen(const wxPoint& p) const;
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