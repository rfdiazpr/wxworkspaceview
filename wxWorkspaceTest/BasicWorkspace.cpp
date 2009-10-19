/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#include "stdafx.h"
#include "BasicWorkspace.h"

namespace WorkspaceView
{
	BasicWorkspaceCable::BasicWorkspaceCable(Node *Input, Node *Output, int FromIndex, int ToIndex)
	: Cable(Input, Output, FromIndex, ToIndex)
	{
		ConnectCable(Input, Output, FromIndex, ToIndex);
	}

	void BasicWorkspaceCable::Draw(wxPaintDC* dc, const State& State)
	{
		float ZoomFactor = State.ZoomFactor;

		// calculate world location of spline points
		wxPoint posFrom = State.WorldToScreen(PositionBeg);
		wxPoint posTo = State.WorldToScreen(PositionEnd);
		wxPoint p2 = State.WorldToScreen(PositionCurve1);
		wxPoint p3 = State.WorldToScreen(PositionCurve2);

		int CableSize = (int)(2.f * ZoomFactor);

		wxPoint Points[4] = {posFrom, p2, p3, posTo};

		wxRealPoint dir;
		dir.x = p3.x - posTo.x;
		dir.y = p3.y - posTo.y;

		float len = sqrt(dir.x*dir.x + dir.y*dir.y);
		dir.x /= len;
		dir.y /= len;

		// x' = cos(theta)*x - sin(theta)*y
		// y' = sin(theta)*x + cos(theta)*y
		float theta = 0.33f;
		float lenk = 15.0f * ZoomFactor;

		float nx1 = cos(theta)*dir.x - sin(theta)*dir.y;
		float ny1 = sin(theta)*dir.x + cos(theta)*dir.y;

		float nx2 = cos(-theta)*dir.x - sin(-theta)*dir.y;
		float ny2 = sin(-theta)*dir.x + cos(-theta)*dir.y;

		wxPoint pa( posTo.x + (int)(nx1*lenk), posTo.y + (int)(ny1*lenk));
		wxPoint pb( posTo.x + (int)(nx2*lenk), posTo.y + (int)(ny2*lenk));

		wxPoint cap[3] = { posTo, pb, pa };

		if (IsSelected())
			dc->SetPen(wxPen(wxColor(255, 100, 100), CableSize));
		else
			dc->SetPen(wxPen(wxColor(0, 0, 0), CableSize));

		dc->SetBrush(wxBrush(wxColor(0, 0, 0)));
		dc->DrawSpline(4, Points);
		dc->DrawPolygon(3, cap, wxWINDING_RULE);
	}

	BasicWorkspaceNode::BasicWorkspaceNode(const wxRect& Area, int InputCount, int OutputCount, const wxString& Title)
		: Node(Area, InputCount, OutputCount, Title)
	{
		for (int Index = 0; Index < InputCount; ++Index)
		{
			int X = -InputPortSize.GetWidth() + 1;
			int Y = (Index * (InputPortSize.GetHeight() + InputPortPadding));

			PortInfo Port;
			Port.Label = wxString::Format(_("Input %d"), Index);
			Port.Area = wxRect(wxPoint(X, Y), InputPortSize);

			InputList.push_back(Port);
		}

		for (int Index = 0; Index < OutputCount; ++Index)
		{
			int X = 125;
			int Y = (Index * (OutputPortSize.GetHeight() + OutputPortPadding));

			PortInfo Port;
			Port.Label = wxString::Format(_("Output %d"), Index);
			Port.Area = wxRect(wxPoint(X, Y), OutputPortSize);

			OutputList.push_back(Port);
		}
	}

	void BasicWorkspaceNode::Draw(wxPaintDC* dc, const State& State)
	{
		wxRect WorldArea = GetArea();
		wxRect ScreenArea = State.WorldToScreen(WorldArea);

		float ZoomFactor = State.ZoomFactor;

		TitleFont.SetPointSize((int)(10 * ZoomFactor));
		dc->SetFont(TitleFont);

		wxCoord TextWidth, TextHeight;
		dc->GetTextExtent(Title, &TextWidth, &TextHeight);

		float padding = 5 * ZoomFactor;

		ScreenArea.width = TextWidth 
			+ padding // Right padding
			+ padding; // Left padding

		wxPoint TextPosition(ScreenArea.x + padding, ScreenArea.y + padding);

		if (IsSelected())
			dc->SetPen(wxPen(wxColor(255, 100, 100), 1));
		else
			dc->SetPen(wxPen(wxColor(255, 255, 255), 1));

		dc->SetBrush(wxBrush(wxColor(100, 100, 100)));

		int TitleLineY = ScreenArea.y + padding + padding + TextHeight;

		float InputWidth = 15 * ZoomFactor;
		float InputHeight = 15 * ZoomFactor;
		float InputPadding = 5 * ZoomFactor;

		float OutputWidth = 15 * ZoomFactor;
		float OutputHeight = 15 * ZoomFactor;
		float OutputPadding = 5 * ZoomFactor;

		int TotalHeightInput = InputList.size() * InputPadding
			+ InputList.size() * InputHeight 
			+ InputPadding + InputPadding + TextHeight;

		int TotalHeightOutput = OutputList.size() * OutputPadding
			+ OutputList.size() * InputHeight 
			+ OutputPadding + OutputPadding + TextHeight;

		ScreenArea.height = TotalHeightInput > TotalHeightOutput ? TotalHeightInput : TotalHeightOutput;

		dc->DrawRectangle(ScreenArea);
		dc->DrawLine(
			wxPoint(ScreenArea.x, TitleLineY), 
			wxPoint(ScreenArea.x + ScreenArea.width, TitleLineY) );
		
		dc->SetTextForeground(wxColor(255, 255, 255));
		dc->DrawText(Title, TextPosition);
			
		TitleFont.SetPointSize((int)(8 * ZoomFactor));
		dc->SetFont(TitleFont);

		for (size_t Index = 0; Index < InputList.size(); Index++)
		{
			wxRect PortRect = InputList[Index].Area;
			PortRect.x += WorldArea.x;
			PortRect.y += WorldArea.y;// + padding + padding + TextHeight;
			PortRect = State.WorldToScreen(PortRect);
			dc->DrawRectangle(PortRect);

			//dc->DrawText(InputList[Index].Label, wxPoint(PortRect.x + PortRect.GetWidth(), PortRect.y));
		}

		for (size_t Index = 0; Index < OutputList.size(); Index++)
		{
			wxRect PortRect = OutputList[Index].Area;
			PortRect.x += WorldArea.x;
			PortRect.y += WorldArea.y;// + padding + padding + TextHeight;
			PortRect = State.WorldToScreen(PortRect);
			dc->DrawRectangle(PortRect);
		}
	}

	Node* BasicWorkspaceFactory::CreateNode(const wxRect& Area, int InputCount, int OutputCount, const wxString& Title)
	{
		return new BasicWorkspaceNode(Area, InputCount, OutputCount, Title);
	}

	Cable* BasicWorkspaceFactory::CreateCable(Node* Input, Node* Output, int FromIndex, int ToIndex)
	{
		return new BasicWorkspaceCable(Input, Output, FromIndex, ToIndex);
	}
}