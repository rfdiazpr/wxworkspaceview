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
#include <wx/dcbuffer.h>
#include "Factory.h"
#include "Listener.h"
#include "State.h"
#include "Item.h"

/**
 * The position of the watermark, this can be on the following 9 position.
 */
enum EWatermarkPosition
{
	WatermarkPositionNone,
	WatermarkPositionUpperLeft,
	WatermarkPositionUpperRight,
	WatermarkPositionUpperCenter,
	WatermarkPositionBottomLeft,
	WatermarkPositionBottomRight,
	WatermarkPositionBottomCenter,
	WatermarkPositionCenterLeft,
	WatermarkPositionCenterRight,
	WatermarkPositionCenterCenter
};

/**
 * The cursors used on the workspace.
 */
enum ECursor
{
	CursorSelect,
	CursorPan,
	CursorMove,
	CursorZoom,
	CursorConnect,
	CursorNormal,
	TotalCursors
};

/**
 * The available interaction states.
 */
enum EInteractionState
{
	InteractionStateNormal,
	InteractionStateSelect,
	InteractionStateConnect,
	InteractionStatePan,
	InteractionStateZoom,
	InteractionStateMove
};

class wxWorkspaceView: public wxControl
{
public:
	/**
	 * Initializes a new instance of this class.
	 */
	wxWorkspaceView();
	/**
	 * Initializes a new instance of this class.
	 */
	wxWorkspaceView(WorkspaceView::Factory* Factory, wxWindow* parent, int id = wxID_ANY, wxPoint pos = wxDefaultPosition, wxSize size = wxSize(640, 480), int style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL);
	/**
	 * Cleans up all resources used by this class.
	 */
	~wxWorkspaceView();
public:
	/** 
	 * Enables/Disables AA
	 */
	void SetAntiAlias(bool Enable);
	/** 
	 * Sets the watermark position.
	 */
	void SetWatermark(const wxString& Image, EWatermarkPosition Position);
	/**
	 * Sets the background color of this workspace.
	 */
	void SetBackColor(const wxColor& Color);
	/**
	 * Sets the color of the grid.
	 */
	void SetGridColor(const wxColor& Color);
public:
	/**
	 * Sets the listener of this workspace and deletes the current one.
	 */
	void SetListener(WorkspaceView::Listener* NewListener);
public:
	/**
	 * Adds an item to the workspace.
	 */
	void AddItem(WorkspaceView::Item* Item);
	/**
	 * Returns the total amount of items on the workspace.
	 */
	size_t ItemCount();
	/**
	 * Clears all items from the workspace.
	 */
	void ClearItems();
public:
	/**
	 * Select all items on the workspace.
	 */
	void SelectAll();
	/**
	 * Deselect the current selection.
	 */
	void ClearSelection();
	/**
	 * Deletes the current selection.
	 */
	void DeleteSelection();
	/**
	 * Adds all items in a certain rectangle to the current selection.
	 */
	void AddToSelection(const wxRect& Area, bool OnlyTopMost);
public:
	/**
	 * Gets an item from the given rectangle.
	 */
	WorkspaceView::Item* GetItemFromRectangle(const wxRect& Area) const;
	/**
	 * Gets all the items in the given rectangle.
	 */
	std::vector<WorkspaceView::Item *> GetItemsFromRectangle(const wxRect& Area) const;
private:
	/**
	 * Assure that a given rectangle has a positive width and height.
	 */
	wxRect NormalizeRectangle(const wxRect& Rect);
private:
	/**
	 * Whether or not a mouse click was on a connector.
	 */
	bool HasClickedOnConnector(const wxPoint& ScreenPoint, WorkspaceView::ConnectorInfo* Result);
private:
	/**
	 * Draws the whole view.
	 */
	void DrawView(wxDC& dc);
	/**
	 * Draws the grid to the screen.
	 */
	void DrawGrid(wxDC& dc, const wxSize& Size);
	/**
	 * Draws a watermark to the screen if one has been set.
	 */
	void DrawWatermark(wxDC& dc, const wxSize& Size);
	/**
	 * Draw the selection rectangle.
	 */
	void DrawSelection(wxDC& dc, const wxSize& Size);
private:
	/**
	 * Paint our control using AA (uses wx's graphics contexts)
	 */
	void PaintAntiAliased();
	/**
	 * Paint our control without any AA
	 */
	void PaintAliased();
private:
	/**
	 * The list with all workspace items. This is what it's all about baby.
	 */
	std::vector<WorkspaceView::Item*> ItemsArray;
private:
	/**
	 * The current view state of the workspace.
	 */
	WorkspaceView::State ViewState;
private:
	/**
	 * This indicates the position of the watermark, if set to none; no watermark 
	 * will be drawn.
	 */
	EWatermarkPosition WatermarkPosition;
	/**
	 * The watermark image.
	 */
	wxBitmap WatermarkImage;
private:
	/**
	 * The cursors used when events change in the workspace.
	 */
	wxCursor CursorList[TotalCursors];
	/**
	 * Updates the cursor according to the current editing state.
	 */
	void UpdateCursor();
private:
	/**
	 * The interaction state that's set for editing.
	 */
	EInteractionState InteractionState;
	/**
	 * The panning start offset. Used when panning.
	 */
	wxPoint PanningOffsetStart;
	/**
	 * The mouse position on a left mouse down event.
	 */
	wxPoint InitialMouseDownPosition;
private:
	/**
	 * Used when changing the zoom.
	 */
	float ZoomFactorStart;
	/**
	 * The minimum zoom factor. Set to 0.f to disable.
	 */
	float MinimumZoomFactor;
	/**
	 * The maximum zoom factor. Set to 0.f to disable.
	 */
	float MaximumZoomFactor;
private:
	/**
	 * The selection rectangle.
	 */
	wxRect SelectionRect;
	/**
	 * A map with items and coordinates.
	 */
	std::map<WorkspaceView::Item*, wxRect> SelectedItemPositionsOnMoveStart;
private:
	/**
	 * The factory used to create items.
	 */
	WorkspaceView::Factory* Factory;
	/**
	 * The listener of this workspace.
	 */
	WorkspaceView::Listener* Listener;
private:
	/**
	 * Whether or not snap to grid is enabled.
	 */
	bool SnapToGrid;
	/**
	 * The size of the grid.
	 */
	float GridStep;
private:
	bool AntiAliased;
	wxColor BackColor;
	wxColor GridColor;
private:
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnLeftMouseDown(wxMouseEvent& event);
	void OnLeftMouseUp(wxMouseEvent& event);
	void OnRightMouseDown(wxMouseEvent& event);
	void OnRightMouseUp(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	void OnMouseCaptureLostEvent(wxMouseCaptureLostEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnEraseBackGround(wxEraseEvent& event);
public:
	DECLARE_EVENT_TABLE();
	DECLARE_DYNAMIC_CLASS(wxWorkspaceView);
};