/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#pragma once

namespace WorkspaceView
{
	class BasicWorkspaceCable: public Cable
	{
	public:
		/**
		 * Initializes a new instance of this class.
		 */
		BasicWorkspaceCable(Node* Input, Node* Output, int FromIndex, int ToIndex);
	public:
		/**
		 * Draws the item to the screen.
		 */
		virtual void Draw(wxPaintDC* dc, const State& State);
	};

	class BasicWorkspaceNode: public Node
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
		virtual void Draw(wxPaintDC* dc, const State& State);
	protected:
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

	class BasicWorkspaceFactory: public Factory
	{
	public:
		/**
		 * Creates a node from the factory.
		 */
		virtual Node* CreateNode(const wxRect& Area, int InputCount, int OutputCount, const wxString& Title);
		/**
		 * Creates a cable from the factory.
		 */
		virtual Cable* CreateCable(Node *Input, Node *Output, int FromIndex, int ToIndex);
	};
}