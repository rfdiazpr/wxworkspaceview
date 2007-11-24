/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#pragma once

class BasicWorkspaceCable: public WorkspaceCable
{
public:
	/**
	 * Initializes a new instance of this class.
	 */
	BasicWorkspaceCable(WorkspaceNode *Input, WorkspaceNode *Output, int FromIndex, int ToIndex);
public:
	/**
	 * Draws the item to the screen.
	 */
	virtual void Draw(wxPaintDC* dc, const WorkspaceState& State);
};

class BasicWorkspaceNode: public WorkspaceNode
{
public:
	/**
	 * Initializes a new instance of this class.
	 */
	BasicWorkspaceNode(const wxRect& Area, int InputCount, int OutputCount, const wxString& Title);
public:
	/**
	 * Draws the item to the screen.
	 */
	virtual void Draw(wxPaintDC* dc, const WorkspaceState& State);
protected:
	/**
	 * The radius of this node.
	 */
	float Radius;
protected:
	wxFont TitleFont;

	wxPen BorderPenExtra;
	wxBrush BorderBrush;
	
	wxBrush SelectedBodyBrush;
	wxPen SelectedBodyPen;
	
	wxBrush BodyBrush;
	wxPen BodyPen;

	wxPen PortPen;
	wxBrush PortBrush;
	
	wxBrush TitleBackBrush;
	wxPen TitleBackPen;
	
	wxColor TitleForeColor;
	wxColor TitleBackColor;
};

class BasicWorkspaceFactory: public wxWorkspaceFactory
{
public:
	/**
	 * Creates a node from the factory.
	 */
	virtual WorkspaceNode* CreateNode(const wxRect& Area, int InputCount, int OutputCount, const wxString& Title);
	/**
	 * Creates a cable from the factory.
	 */
	virtual WorkspaceCable* CreateCable(WorkspaceNode *Input, WorkspaceNode *Output, int FromIndex, int ToIndex);
};