/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#pragma once

namespace WorkspaceView
{
	struct ThijzPortInfo: public PortInfo
	{
		float LabelAngle;
		wxRealPoint LabelDirection;
	};

	class ThijzWorkspaceCable: public Cable
	{
	public:
		/**
		 * Initializes a new instance of this class.
		 */
		ThijzWorkspaceCable(Node* Input, Node* Output, int FromIndex, int ToIndex);
	public:
		/**
		 * Draws the item to the screen.
		 */
		virtual void Draw(wxPaintDC* dc, const State& State);
	};

	class ThijzWorkspaceNode: public Node
	{
	public:
		/**
		 * Initializes a new instance of this class.
		 */
		ThijzWorkspaceNode(const wxPoint& Position, float Radius, int InputCount, int OutputCount, const wxString& Title);
	public:
		/**
		 * Draws the item to the screen.
		 */
		virtual void Draw(wxPaintDC* dc, const State& State);
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

	class ThijzWorkspaceFactory: public Factory
	{
	public:
		/**
		 * Creates a node from the factory.
		 */
		virtual Node* CreateNode(const wxPoint& Position, float Radius, int InputCount, int OutputCount, const wxString& Title);
		/**
		 * Creates a node from the factory.
		 */
		virtual Node* CreateNode(const wxRect& Area, int InputCount, int OutputCount, const wxString& Title);
		/**
		 * Creates a cable from the factory.
		 */
		virtual Cable* CreateCable(Node* Input, Node* Output, int FromIndex, int ToIndex);
	};
}