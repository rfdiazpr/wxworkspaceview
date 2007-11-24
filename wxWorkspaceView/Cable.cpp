/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#include "Cable.h"
#include <algorithm>

namespace WorkspaceView
{
	const float Cable::CABLE_CURVEPOINT_OFFSET = 50.0f;

	Cable::Cable(Node* Input, Node* Output, int FromIndex, int ToIndex)
		: Item(WorkspaceItemTypeCable, wxRect(0, 0, 0, 0)), Input(Input), Output(Output), FromIndex(FromIndex), ToIndex(ToIndex)
	{
		ConnectCable(Input, Output, FromIndex, ToIndex);
	}

	bool Cable::IntersectsWith(float Left, float Top, float Right, float Bottom) const
	{
		wxRect Rect(Left, Top, Right - Left, Bottom - Top);

		return IntersectsWith(Rect);
	}

	bool Cable::IntersectsWith(const wxRect& Rect) const
	{
		if (!Item::IntersectsWith(Rect))
			return false;
		
		// Have to check the containment of our cable, we do this by 
		// processing line segments of our cable.

		wxPoint OrderedP1;
		wxPoint OrderedP2;

		for (int Index = 1; Index < NUM_CABLE_SEGMENTS; ++Index)
		{
			const wxPoint& p1 = SegmentList[Index - 1].Position;
			const wxPoint& p2 = SegmentList[Index].Position;

			// check rectangle spanning p1 and p2 to overlap with rectangle
			// specified by left,top,right,bottom using separating axis theory.
			// but.. we have no guarantee of the positions of p1 and p2

			// horizontal borders of linesegment
			if (p1.x > p2.x) 
			{ 
				if ((p1.x < Rect.GetLeft()) || (p2.x > Rect.GetRight()))
					continue; 
			}
			else
			{ 
				if ((p2.x < Rect.GetLeft()) || (p1.x > Rect.GetRight()))
					continue; 
			}

			// vertical borders of linesegment
			if (p1.y > p2.y) 
			{ 
				if ((p1.y < Rect.GetTop()) || (p2.y > Rect.GetBottom()))
					continue; 
			}
			else
			{ 
				if ((p2.y < Rect.GetTop()) || (p1.y > Rect.GetBottom()))
					continue; 
			}

			return true;		
		}

		return false;
	}

	void Cable::ConnectCable(const Node* Input, const Node* Output, int FromIndex, int ToIndex)
	{
		this->Input = Input;
		this->Output = Output;
		this->FromIndex = FromIndex;
		this->ToIndex = ToIndex;
		
		wxRealPoint DirectionFrom(0,0);
		wxRealPoint DirectionTo(0,0);

		if (Output)
		{
			const wxRect& FromArea = Output->GetPortArea(FromIndex, true);
			PositionBeg = FromArea.GetTopLeft() + FromArea.GetSize() / 2;
			
			const wxPoint& NodeCenter = Output->GetArea().GetTopLeft() + Output->GetArea().GetSize() / 2;
			DirectionFrom.x = PositionBeg.x - NodeCenter.x;
			DirectionFrom.y = PositionBeg.y - NodeCenter.y;

			float Len = sqrtf(DirectionFrom.x * DirectionFrom.x + DirectionFrom.y * DirectionFrom.y);
			DirectionFrom.x /= Len;
			DirectionFrom.y /= Len;
		}
		else
			PositionBeg = FloatingPosition;

		if (Input)
		{
			const wxRect& ToArea = Input->GetPortArea(ToIndex, false);
			PositionEnd = ToArea.GetTopLeft() + ToArea.GetSize()/2;

			const wxPoint& NodeCenter = Input->GetArea().GetTopLeft() + Input->GetArea().GetSize() / 2;
			DirectionTo.x = PositionEnd.x - NodeCenter.x;
			DirectionTo.y = PositionEnd.y - NodeCenter.y;

			float Len = sqrtf(DirectionTo.x * DirectionTo.x + DirectionTo.y * DirectionTo.y);
			DirectionTo.x /= Len;
			DirectionTo.y /= Len;
		}
		else
			PositionEnd = FloatingPosition;
		
		// our area's rectangle will include our 4 spline points:
		// START, P1, P2 and END
		
		wxPoint p2, p3;
		p2.x = PositionBeg.x + DirectionFrom.x * CABLE_CURVEPOINT_OFFSET;
		p2.y = PositionBeg.y + DirectionFrom.y * CABLE_CURVEPOINT_OFFSET;

		p3.x = PositionEnd.x + DirectionTo.x * CABLE_CURVEPOINT_OFFSET;
		p3.y = PositionEnd.y + DirectionTo.y * CABLE_CURVEPOINT_OFFSET;

		PositionCurve1 = p2;
		PositionCurve2 = p3;

		wxRect TempArea(PositionBeg, wxSize(1, 1));
		TempArea += wxRect(PositionEnd, wxSize(1,1));
		TempArea += wxRect(p2, wxSize(1, 1));
		TempArea += wxRect(p3, wxSize(1, 1));
		TempArea.Inflate(4,4);
		SetArea(TempArea);	

		// calculate cable linesegments and store them for easier cable clicking!

		// our cable's spline consists of 4 points:
		// START, P1, P2 and END
		// we cut up our polyline in the following parts:
		// - line from START to C1, with C1 = (START+P1)/2
		// - quadratic curve from last point to C2, using P1 as controlpoint, with C2 = (P1+P2)/2
		// - quadratic curve from last point to C3, using P2 as controlpoint, with C3 = (P2+END)/2
		// - line from last point to END

		wxPoint Points[4] = {PositionBeg, p2, p3, PositionEnd};

		float t = 0;
		int cntr = 0;

		// we have NUM_CABLE_SEGMENTS segments:
		// - 2 segments for head and tail line
		// - (NUMSEGMENTS-1)/2 segments per quadratic curve
		int NumSegmentsPerCurve = (NUM_CABLE_SEGMENTS - 2) / 2;
		float ts = 1.0f / (NumSegmentsPerCurve - 1);

		wxRealPoint pp0(Points[0].x, Points[0].y);
		wxRealPoint pp1(Points[1].x, Points[1].y);
		wxRealPoint pp2(Points[2].x, Points[2].y);
		wxRealPoint pp3(Points[3].x, Points[3].y);

		float x1 = pp0.x;
		float y1 = pp0.y;

		float x2 = pp1.x;
		float y2 = pp1.y;
		float cx = (x1+x2)/2.0f;
		float cy = (y1+y2)/2.0f;

		LineSegment* SegmentFinger = &SegmentList[0];

		// add start point
		SegmentFinger->Position.x = (int)x1;
		SegmentFinger->Position.y = (int)y1;
		++SegmentFinger;
		++cntr;

		float curx;
		float cury;

		for (int SegmentIndex = 0; SegmentIndex < 2; SegmentIndex++)
		{
			// two quadratic splines

			if (SegmentIndex == 0)
			{
				x1 = cx;
				y1 = cy;
				x2 = (pp1.x+pp2.x)/2.0f;
				y2 = (pp1.y+pp2.y)/2.0f;
				cx = pp1.x; 
				cy = pp1.y;
				curx = x1;
				cury = y1;
			} else
			{
				x1 = x2;
				y1 = y2;
				x2 = (pp2.x+pp3.x)/2.0f;
				y2 = (pp2.y+pp3.y)/2.0f;
				cx = pp2.x; 
				cy = pp2.y;
			}


			float cube1x = x1;
			float cube1y = y1;
			float cube2x = x1/3.0f + (cx*2.0f)/3.0f;
			float cube2y = y1/3.0f + (cy*2.0f)/3.0f;
			float cube3x = cx/3.0f + (x2*2.0f)/3.0f;
			float cube3y = cy/3.0f + (y2*2.0f)/3.0f;	
			float cube4x = x2;
			float cube4y = y2;	

			t = 0;
			int NumSegments = 0;

			while (NumSegments < NumSegmentsPerCurve)
			{
				float tox = 
					(1-t)*(1-t)*(1-t)*cube1x + 
					3.0f*t*(1-t)*(1-t)*cube2x +
					3.0f*t*t*(1-t)*cube3x +
					t*t*t*cube4x;
				float toy = 
					(1-t)*(1-t)*(1-t)*cube1y + 
					3.0f*t*(1-t)*(1-t)*cube2y +
					3.0f*t*t*(1-t)*cube3y +
					t*t*t*cube4y;

				SegmentFinger->Position.x = (int)tox;
				SegmentFinger->Position.y = (int)toy;
				++SegmentFinger;
				++NumSegments;
				++cntr;

				t += ts;
				if (t > 1)
					t = 1;
			}
		}

		// add tail segment
		SegmentFinger->Position.x = (int)pp3.x;
		SegmentFinger->Position.y = (int)pp3.y;
		++SegmentFinger;
		++cntr;
	}

	bool Cable::CanConnectFloatingEndTo(const ConnectorInfo& ConnectInfo)
	{
		if (!Output)
			return ConnectInfo.PortIsOutput;
		else
			return !ConnectInfo.PortIsOutput;
	}

	void Cable::ConnectFloatingEnd(const ConnectorInfo& ConnectInfo)
	{
		if (!Output)
		{
			Output = (Node*)ConnectInfo.Owner;
			FromIndex = ConnectInfo.PortIndex;
		}
		else
		{
			Input = (Node*)ConnectInfo.Owner;
			ToIndex = ConnectInfo.PortIndex;
		}
		
		ConnectCable(Input, Output, FromIndex, ToIndex);
	}

	void Cable::SetFloatingPosition(const wxPoint& NewFloatingPosition)
	{
		FloatingPosition = NewFloatingPosition;

		ConnectCable(Input, Output, FromIndex, ToIndex);
	}

	bool Cable::ShouldDeleteOnSelectionDeletion() const
	{
		if (Item::ShouldDeleteOnSelectionDeletion())
			return true;
		
		if (IsReadOnly())
			return false;

		if (Output && Output->IsSelected() && !Output->IsReadOnly() )
			return true;

		if (Input && Input->IsSelected() && !Input->IsReadOnly() )
			return true;

		return false;
	}

	void Cable::ReactToItemsMoved(std::vector<Item*> ItemsThatWereMoved)
	{
		if ((std::find(ItemsThatWereMoved.begin(), ItemsThatWereMoved.end(), Output) != ItemsThatWereMoved.end()) ||
			(std::find(ItemsThatWereMoved.begin(), ItemsThatWereMoved.end(), Input) != ItemsThatWereMoved.end()))
		{
			ConnectCable(Input, Output, FromIndex, ToIndex);
		}
	}
}