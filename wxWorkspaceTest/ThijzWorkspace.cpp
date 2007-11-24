/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#include "stdafx.h"
#include "ThijzWorkspace.h"

const int NODE_WIDTH = 90;
const int NODE_HEIGHT = 64;
const float NODE_PHIGAPBETWEENPORTS = 20.0f * 0.017453f; // 20 degrees
const float NODE_OUTERBORDERSIZE = 3.0f;
const float NODE_PORTRADIUS = 10.0f;
const float NODE_INNERSELECTIONBORDER = 5.0f;
const int NODE_TITLERECTSIZEX = 4;
const int NODE_TITLERECTSIZEY = 2;
const int NODE_TITLERECTCORNERRADIUS = 5;
const int NODE_PORTINNERRADIUS = 4.0f;

ThijzWorkspaceCable::ThijzWorkspaceCable(WorkspaceNode *Input, WorkspaceNode *Output, int FromIndex, int ToIndex)
: WorkspaceCable(Input, Output, FromIndex, ToIndex)
{
	ConnectCable(Input, Output, FromIndex, ToIndex);
}

void ThijzWorkspaceCable::Draw(wxPaintDC* dc, const WorkspaceState& State)
{
	static wxPen lineOuter(wxColour(0,0,0)); //wxColour(149,219,0));
	static wxPen lineOuterSelected(wxColour(0,0,0));
	static wxPen lineInner(wxColour(186,255,0));
	static wxPen lineInnerSelected(wxColour(255,204,0));
	static wxBrush brushSelected(wxColour(255,204,0)); //wxColour(149,219,0));
	static wxBrush brushCap(wxColour(186,255,0)); //wxColour(149,219,0));

	float ZoomFactor = State.ZoomFactor;

	// calculate world location of spline points
	wxPoint posFrom = State.WorldToScreen(PositionBeg);
	wxPoint posTo = State.WorldToScreen(PositionEnd);
	wxPoint p2 = State.WorldToScreen(PositionCurve1);
	wxPoint p3 = State.WorldToScreen(PositionCurve2);

	int cableSize = IsSelected() ? (int)(6.0f * ZoomFactor) : (int)(4.0f * ZoomFactor);

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
		dc->SetBrush(brushSelected);
	else
		dc->SetBrush(brushCap);

	if (IsSelected())
	{
		lineOuterSelected.SetWidth(cableSize+2);
		dc->SetPen(lineOuterSelected);
	}
	else
	{
		lineOuter.SetWidth(cableSize+2);
		dc->SetPen(lineOuter);
	}
	dc->DrawSpline(4, Points);
	dc->DrawPolygon(3, cap, wxWINDING_RULE);
	
	if (IsSelected())
	{
		lineInnerSelected.SetWidth(cableSize);
		dc->SetPen(lineInnerSelected);
	}
	else
	{
		lineInner.SetWidth(cableSize);
		dc->SetPen(lineInner);
	}
	dc->DrawSpline(4, Points);
	dc->DrawPolygon(3, cap, wxWINDING_RULE);
}

ThijzWorkspaceNode::ThijzWorkspaceNode(const wxPoint& Position, float Radius, int InputCount, int OutputCount, const wxString& Title)
: WorkspaceNode(wxRect(Position.x-Radius, Position.y-Radius, Radius*2.0f, Radius*2.0f), InputCount, OutputCount, Title), Radius(Radius)
{
	for (int Index = 0; Index < OutputCount; ++Index)
	{
		float phi = 3.141593f 
			- ((OutputCount - 1) * NODE_PHIGAPBETWEENPORTS) / 2.0f 
			+ Index * NODE_PHIGAPBETWEENPORTS;

		float x = (Radius + NODE_OUTERBORDERSIZE) * cos(phi) + Radius;
		float y = (Radius + NODE_OUTERBORDERSIZE) * sin(phi) + Radius;

		PortInfo p;
		p.Label = "";
		p.ConnectorArea = wxRect((int)(x-NODE_PORTRADIUS), y-NODE_PORTRADIUS, (int)(NODE_PORTRADIUS*2.0f), (int)(NODE_PORTRADIUS*2.0f));
		p.LabelAngle = -phi * 57.295780f;
		p.LabelDirection.x = -cos(phi);
		p.LabelDirection.y = -sin(phi);
		OutputList.push_back(p);
	}

	for (int Index = 0; Index < InputCount; ++Index)
	{
		float phi = 0
			+ ((InputCount - 1) * NODE_PHIGAPBETWEENPORTS) / 2.0f
			- Index * NODE_PHIGAPBETWEENPORTS;

		float x = (Radius+NODE_OUTERBORDERSIZE) * cos(phi) + Radius;
		float y = (Radius+NODE_OUTERBORDERSIZE) * sin(phi) + Radius;

		PortInfo p;
		p.Label = "input";
		p.ConnectorArea = wxRect((int)(x-NODE_PORTRADIUS), y-NODE_PORTRADIUS, (int)(NODE_PORTRADIUS*2.0f), (int)(NODE_PORTRADIUS*2.0f));
		p.LabelAngle = -phi * 57.295780f;
		p.LabelDirection.x = -cos(phi);
		p.LabelDirection.y = -sin(phi);
		InputList.push_back(p);
	}
	
	TitleFont = wxFont(
		10, 
		wxFONTFAMILY_DEFAULT, 
		wxFONTSTYLE_NORMAL, 
		wxFONTWEIGHT_NORMAL,
		false,
		"Tahoma");

	BorderPenExtra = wxPen(wxColor(0, 0, 0), 1);
	BorderBrush = wxBrush(wxColor(149, 219, 0));

	SelectedBodyBrush = wxBrush(wxColor(255, 204, 0));
	SelectedBodyPen = wxPen(wxColor(255, 255, 255), 1);

	BodyBrush = wxBrush(wxColor(255, 255, 255));
	BodyPen = wxPen(wxColor(255, 255, 255), 1);

	PortPen = wxPen(wxColour(149, 219, 0), 1);
	PortBrush = wxBrush(wxColor(255, 255, 255));
	
	TitleBackBrush = wxBrush(wxColour(0, 0, 0));
	TitleBackPen = wxPen(wxColor(255, 255, 255), 1);
	
	TitleForeColor = wxColor(255, 255, 255);
	TitleBackColor = wxColor(0, 0, 0);
}

void ThijzWorkspaceNode::Draw(wxPaintDC* dc, const WorkspaceState& State)
{
	wxRect WorldArea = GetArea();
	wxRect r = State.WorldToScreen(WorldArea);
	wxPoint NodeCenter = r.GetTopLeft() + r.GetSize()/2;

	float ZoomFactor = State.ZoomFactor;
	
	dc->SetBrush(BorderBrush);
	dc->SetPen(BorderPenExtra);
	dc->DrawCircle(NodeCenter, (int)((Radius + NODE_OUTERBORDERSIZE) * ZoomFactor));
	
	float DrawRadius = Radius * ZoomFactor;
	if (IsSelected())
	{
		dc->SetBrush(SelectedBodyBrush);
		SelectedBodyPen.SetWidth(5);
		dc->SetPen(SelectedBodyPen);
		DrawRadius -= 3;
	}
	else
	{
		dc->SetPen(BodyPen);
		dc->SetBrush(BodyBrush);
	}
	dc->DrawCircle(NodeCenter, (int)DrawRadius);
	
	PortPen.SetWidth((int)(NODE_PORTINNERRADIUS * ZoomFactor));
	dc->SetBrush(PortBrush);
	dc->SetPen(BorderPenExtra);

	for (size_t Index = 0; Index < InputList.size(); Index++)
	{
		wxRect PortRect = InputList[Index].ConnectorArea;
		PortRect.x += WorldArea.x;
		PortRect.y += WorldArea.y;
		PortRect = State.WorldToScreen(PortRect);
		
		dc->DrawCircle(PortRect.GetTopLeft() + PortRect.GetSize() / 2, 
			(int)((NODE_PORTRADIUS - NODE_PORTINNERRADIUS) * ZoomFactor));
	}
	
	for (size_t Index = 0; Index < OutputList.size(); Index++)
	{
		wxRect PortRect = OutputList[Index].ConnectorArea;
		PortRect.x += WorldArea.x;
		PortRect.y += WorldArea.y;
		PortRect = State.WorldToScreen(PortRect);
		
		dc->DrawCircle(PortRect.GetTopLeft() + PortRect.GetSize() / 2, 
			(int)(NODE_PORTRADIUS * ZoomFactor));
	}

	TitleFont.SetPointSize((int)(10 * ZoomFactor));
	dc->SetFont(TitleFont);

	wxCoord TextWidth, TextHeight;
	dc->GetTextExtent(Title, &TextWidth, &TextHeight);

	wxPoint TextPosition
		(
			r.x + (r.width / 2) - (TextWidth / 2),
			r.y + (r.height / 2) - (TextHeight / 2)
		);
	
	dc->SetBrush(TitleBackBrush);
	dc->SetPen(TitleBackPen);
	dc->DrawRoundedRectangle(
		TextPosition.x - (int)(NODE_TITLERECTSIZEX * ZoomFactor),
		TextPosition.y - (int)(NODE_TITLERECTSIZEY * ZoomFactor),
		TextWidth+(int)(NODE_TITLERECTSIZEX * ZoomFactor * 2.0f),
		TextHeight+(int)(NODE_TITLERECTSIZEY * ZoomFactor * 2.0f), 
		(int)(NODE_TITLERECTCORNERRADIUS * ZoomFactor));

	dc->SetTextForeground(TitleForeColor);
	dc->SetTextBackground(TitleBackColor);
	dc->DrawText(Title, TextPosition);
}

WorkspaceNode* ThijzWorkspaceFactory::CreateNode(const wxPoint& Position, float Radius, int InputCount, int OutputCount, const wxString& Title)
{
	return new ThijzWorkspaceNode(Position, Radius, InputCount, OutputCount, Title);
}

WorkspaceNode* ThijzWorkspaceFactory::CreateNode(const wxRect& Area, int InputCount, int OutputCount, const wxString& Title)
{
	wxPoint Position = wxPoint(Area.x, Area.y);

	return new ThijzWorkspaceNode(Position, 60.f, InputCount, OutputCount, Title);
}

WorkspaceCable* ThijzWorkspaceFactory::CreateCable(WorkspaceNode *Input, WorkspaceNode *Output, int FromIndex, int ToIndex)
{
	return new ThijzWorkspaceCable(Input, Output, FromIndex, ToIndex);
}