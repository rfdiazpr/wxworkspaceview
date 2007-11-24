/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#include "wxWorkspaceView.h"

IMPLEMENT_DYNAMIC_CLASS(wxWorkspaceView, wxControl);

BEGIN_EVENT_TABLE(wxWorkspaceView, wxControl)
	EVT_PAINT(wxWorkspaceView::OnPaint)
	EVT_SIZE(wxWorkspaceView::OnSize)
	EVT_LEFT_DOWN(wxWorkspaceView::OnLeftMouseDown)
	EVT_LEFT_UP(wxWorkspaceView::OnLeftMouseUp)
	EVT_RIGHT_DOWN(wxWorkspaceView::OnRightMouseDown)
	EVT_RIGHT_UP(wxWorkspaceView::OnRightMouseUp)
	EVT_MOTION(wxWorkspaceView::OnMouseMove)
	EVT_MOUSEWHEEL(wxWorkspaceView::OnMouseWheel)
	EVT_MOUSE_CAPTURE_LOST(wxWorkspaceView::OnMouseCaptureLostEvent)
	EVT_KEY_DOWN(wxWorkspaceView::OnKeyDown)
	EVT_ERASE_BACKGROUND(wxWorkspaceView::OnEraseBackGround)
END_EVENT_TABLE()

wxWorkspaceView::wxWorkspaceView()
{
}

wxWorkspaceView::wxWorkspaceView(WorkspaceView::Factory* Factory, wxWindow* parent, int id, wxPoint pos, wxSize size, int style)
: wxControl(parent, id, pos, size, style | wxNO_FULL_REPAINT_ON_RESIZE | wxCLIP_CHILDREN), Factory(Factory), Listener(0), GridStep(32.f)
{
	wxInitAllImageHandlers();

	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	
	WatermarkPosition = WatermarkPositionNone;

	InteractionState = InteractionStateNormal;
	
	CursorList[CursorNormal] = wxCursor(wxCURSOR_ARROW);
	CursorList[CursorSelect] = wxCursor(wxCURSOR_CROSS);
	CursorList[CursorPan] = wxCursor(wxCURSOR_HAND);
	CursorList[CursorMove] = wxCursor(wxCURSOR_SIZING);
	CursorList[CursorZoom] = wxCursor(wxCURSOR_MAGNIFIER);
	CursorList[CursorConnect] = wxCursor(wxCURSOR_CROSS);
	
	MinimumZoomFactor = 0.25f;
	MaximumZoomFactor = 0.f;

	GridColor = wxColor(207, 207, 207);
	BackColor = wxColor(255, 255, 255);

	SnapToGrid = true;
}

wxWorkspaceView::~wxWorkspaceView()
{
	for (size_t Index = 0; Index < ItemsArray.size(); ++Index)
		delete ItemsArray[Index];

	if (Listener)
	{
		delete Listener;
		Listener = 0;
	}
}

void wxWorkspaceView::SetWatermark(const wxString& Image, EWatermarkPosition Position)
{
	WatermarkImage = wxBitmap(Image, wxBITMAP_TYPE_ANY);
	WatermarkPosition = Position;
}

void wxWorkspaceView::SetBackColor(const wxColor& Color)
{
	BackColor = Color;
}

void wxWorkspaceView::SetGridColor(const wxColor& Color)
{
	GridColor = Color;
}

void wxWorkspaceView::SetListener(WorkspaceView::Listener *NewListener)
{
	if (Listener)
	{
		delete Listener;
		Listener = 0;
	}

	Listener = NewListener;
}

void wxWorkspaceView::AddItem(WorkspaceView::Item* Item)
{
	if (Item)
		ItemsArray.push_back(Item);
}

size_t wxWorkspaceView::ItemCount()
{
	return ItemsArray.size();
}

void wxWorkspaceView::ClearItems()
{
	ItemsArray.clear();
}

void wxWorkspaceView::SelectAll()
{
	for (size_t Index = 0; Index < ItemsArray.size(); ++Index)
		ItemsArray[Index]->Select();
}

void wxWorkspaceView::ClearSelection()
{
	for (size_t Index = 0; Index < ItemsArray.size(); ++Index)
		ItemsArray[Index]->Deselect();
}

void wxWorkspaceView::DeleteSelection()
{
	std::vector<WorkspaceView::Item*> ItemsToDelete, ItemsToKeep;
	
	for (size_t Index = 0; Index < ItemsArray.size(); ++Index)
	{
		if (ItemsArray[Index]->ShouldDeleteOnSelectionDeletion())
			ItemsToDelete.push_back(ItemsArray[Index]);
		else
			ItemsToKeep.push_back(ItemsArray[Index]);
	}

	if (Listener)
		for (size_t Index = 0; Index < ItemsToDelete.size(); ++Index)
			Listener->OnDeleteItemPre(ItemsToDelete[Index]);
	
	ItemsArray = ItemsToKeep;

	for (size_t Index = 0; Index < ItemsToDelete.size(); ++Index)
	{
		if (Listener)
			Listener->OnDeleteItemPost(ItemsToDelete[Index]);

		delete ItemsToDelete[Index];
	}

	Refresh();
	Update();
}

void wxWorkspaceView::AddToSelection(const wxRect& Area, bool OnlyTopMost)
{
	std::vector<WorkspaceView::Item*> Result = GetItemsFromRectangle(Area);

	if (Listener)
		Listener->OnSelectedItems(Result);
	
	if (!Result.empty())
	{
		if (OnlyTopMost)
			Result.back()->Select();
		else
			for (size_t Index = 0; Index < Result.size(); ++Index)
				Result[Index]->Select();
	}
}

WorkspaceView::Item* wxWorkspaceView::GetItemFromRectangle(const wxRect& Area) const
{
	std::vector<WorkspaceView::Item *> Result = GetItemsFromRectangle(Area);
	
	return Result.empty() ? NULL : Result[0];
}

std::vector<WorkspaceView::Item *> wxWorkspaceView::GetItemsFromRectangle(const wxRect &Area) const
{
	std::vector<WorkspaceView::Item *> Result;
	
	int Left = Area.x;
	int Top = Area.y;
	int Right = Area.x + Area.width - 1;
	int Bottom = Area.y + Area.height - 1;

	if (Top > Bottom)
	{
		int Temp = Top;
		Top = Bottom;
		Bottom = Temp;
	}
	
	if (Left > Right)
	{
		int Temp = Left;
		Left = Right;
		Right = Temp;
	}
	
	for (size_t Index = 0; Index < ItemsArray.size(); ++Index)
	{
		WorkspaceView::Item* Item = ItemsArray[Index];

		if (Item->IntersectsWith(Left, Top, Right, Bottom))
			Result.push_back(Item);
	}

	return Result;
}

bool wxWorkspaceView::HasClickedOnConnector(const wxPoint& ScreenPoint, WorkspaceView::ConnectorInfo* Result)
{
	for (size_t Index = 0; Index < ItemsArray.size(); ++Index)
		if (ItemsArray[Index]->HasHitWithConnector(ScreenPoint, Result))
			return true;

	return false;
}

void wxWorkspaceView::DrawGrid(wxBufferedPaintDC &dc, const wxSize &Size)
{
	wxPen GridPen(GridColor);
	dc.SetPen(GridPen);

	int GridSize = (int)(GridStep * ViewState.ZoomFactor);
	GridSize = (GridSize > 1) ? GridSize : 1;
	
	wxPoint StartPos = ViewState.Offset;
	if (StartPos.x > 0)
		StartPos.x %= GridSize;
	else
		StartPos.x = GridSize - ((-StartPos.x) % GridSize);

	if (StartPos.y > 0)
		StartPos.y %= GridSize;
	else
		StartPos.y = GridSize - ((-StartPos.y) % GridSize);

	for (int X = StartPos.x; X < Size.GetWidth(); X += GridSize)
		dc.DrawLine(X, 0, X, Size.GetHeight());

	for (int Y = StartPos.y; Y < Size.GetHeight(); Y += GridSize)
		dc.DrawLine(0, Y, Size.GetWidth(), Y);
}

void wxWorkspaceView::DrawWatermark(wxBufferedPaintDC &dc, const wxSize &Size)
{
	if (WatermarkPosition != WatermarkPositionNone && WatermarkImage.Ok())
	{
		bool Transparent = false;

		int SW = Size.GetWidth();
		int SH = Size.GetHeight();

		int IW = WatermarkImage.GetWidth();
		int IH = WatermarkImage.GetHeight();

		switch (WatermarkPosition)
		{
		case WatermarkPositionUpperLeft:
			dc.DrawBitmap(WatermarkImage, 0, 0, Transparent);
			break;
		case WatermarkPositionUpperRight:
			dc.DrawBitmap(WatermarkImage, SW - IW, 0, Transparent);
			break;
		case WatermarkPositionUpperCenter:
			dc.DrawBitmap(WatermarkImage, (SW - IW) / 2, 0, Transparent);
			break;
		case WatermarkPositionBottomLeft:
			dc.DrawBitmap(WatermarkImage, 0, SH - IH, Transparent);
			break;
		case WatermarkPositionBottomCenter:
			dc.DrawBitmap(WatermarkImage, (SW - IW) / 2, (SH - IH), Transparent);
			break;
		case WatermarkPositionCenterLeft:
			dc.DrawBitmap(WatermarkImage, 0, (SH - IH) / 2, Transparent);
			break;
		case WatermarkPositionCenterRight:
			dc.DrawBitmap(WatermarkImage, (SW - IW), (SH - IH) / 2, Transparent);
			break;
		case WatermarkPositionCenterCenter:
			dc.DrawBitmap(WatermarkImage, (SW - IW) / 2, (SH - IH) / 2, Transparent);
			break;
		default:
			dc.DrawBitmap(WatermarkImage, (SW - IW), (SH - IH), Transparent);
			break;
		}
	}
}

void wxWorkspaceView::DrawSelection(wxBufferedPaintDC &dc, const wxSize &Size)
{
	if (InteractionState == InteractionStateSelect)
	{
		wxRect ScreenRect = ViewState.WorldToScreen(SelectionRect);
		
		static wxBrush selectionBrush(wxColour(255,255,255),wxTRANSPARENT);
		static wxPen selectionPen(wxColour(255,255,255),2, wxSHORT_DASH);

		dc.SetLogicalFunction(wxXOR);
		dc.SetBrush(selectionBrush);
		dc.SetPen(selectionPen);
		dc.DrawRectangle(ScreenRect);
		dc.SetLogicalFunction(wxSET);
	}
}

void wxWorkspaceView::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxBufferedPaintDC dc(this);

	wxSize Size = GetClientSize();
	wxCoord Width = 0, Height = 0;
	GetClientSize(&Width, &Height);

	wxBrush BackgroundBrush(BackColor);
	dc.SetBrush(BackgroundBrush);
	dc.DrawRectangle(0, 0, Width + 1, Height + 1);

	DrawWatermark(dc, Size);
	DrawGrid(dc, Size);

	for (size_t Index = 0; Index < ItemsArray.size(); ++Index)
		ItemsArray[Index]->Draw((wxPaintDC*)&dc, ViewState);

	DrawSelection(dc, Size);
}

void wxWorkspaceView::OnSize(wxSizeEvent& WXUNUSED(event))
{
	Refresh();
}

void wxWorkspaceView::OnLeftMouseDown(wxMouseEvent& event)
{
	if (InteractionState == InteractionStateNormal)
	{
		InitialMouseDownPosition = event.GetPosition();

		wxRect TempRect;
		TempRect.SetLeftTop(InitialMouseDownPosition - wxPoint(4, 4));
		TempRect.SetRightBottom(TempRect.GetLeftTop() + wxPoint(4, 4));
		TempRect = ViewState.ScreenToWorld(TempRect);
		
		wxPoint ScreenPoint = ViewState.ScreenToWorld(InitialMouseDownPosition);
		WorkspaceView::ConnectorInfo ConnectInfo;
		
		if (HasClickedOnConnector(ScreenPoint, &ConnectInfo))
		{
			InteractionState = InteractionStateConnect;

			WorkspaceView::Node* Input = ConnectInfo.PortIsOutput ? NULL : (WorkspaceView::Node*)ConnectInfo.Owner;
			WorkspaceView::Node* Output = ConnectInfo.PortIsOutput ? (WorkspaceView::Node*)ConnectInfo.Owner : NULL;
			int FromIndex = ConnectInfo.PortIndex;
			int ToIndex = ConnectInfo.PortIndex;
			
			WorkspaceView::Cable* NewCable = Factory->CreateCable(Input, Output, FromIndex, ToIndex);

			NewCable->SetFloatingPosition(ScreenPoint);

			ItemsArray.push_back(NewCable);

			Refresh();
		}
		else
		{
			WorkspaceView::Item* Item = GetItemFromRectangle(TempRect);
			
			if (Item)
			{
				if (!event.ControlDown())
					ClearSelection();

				Item->Select();

				InteractionState = InteractionStateMove;

				SelectedItemPositionsOnMoveStart.clear();
				
				for (size_t i = 0; i < ItemsArray.size(); ++i)
					if (ItemsArray[i]->IsSelected())
						SelectedItemPositionsOnMoveStart[ItemsArray[i]] = ItemsArray[i]->GetArea();
			}
			else
			{
				if (event.ControlDown())
				{
					InteractionState = InteractionStatePan;
					PanningOffsetStart = ViewState.Offset;
				}
				else
				{
					if (!event.ShiftDown())
						ClearSelection();

					InteractionState = InteractionStateSelect;
					SelectionRect = TempRect;
				}
			}

			Refresh();
		}

		CaptureMouse();
	}

	UpdateCursor();
}

void wxWorkspaceView::OnLeftMouseUp(wxMouseEvent& event)
{
	if (InteractionState == InteractionStateSelect)
	{
		AddToSelection(SelectionRect, false);
	}
	else if (InteractionState == InteractionStateConnect)
	{
		if (ItemsArray.size() > 0)
		{
			wxPoint ScreenPoint = ViewState.ScreenToWorld(event.GetPosition());

			WorkspaceView::ConnectorInfo ConnectInfo;
			WorkspaceView::Cable* Cable = (WorkspaceView::Cable*)ItemsArray.back();
			
			bool Approved = false;
			
			if (HasClickedOnConnector(ScreenPoint, &ConnectInfo) && 
				Cable->CanConnectFloatingEndTo(ConnectInfo))
			{
				if (!Listener || Listener->ApproveCable(Cable))
				{
					Cable->ConnectFloatingEnd(ConnectInfo);
					
					if (Listener)
						Listener->OnConnectCable(Cable);
					
					Approved = true;
				}
			}
			
			if (!Approved)
			{
				ItemsArray.pop_back();
				delete Cable;
			}
		}
	}

	SelectedItemPositionsOnMoveStart.clear();
	InteractionState = InteractionStateNormal;
	ReleaseCapture();
	UpdateCursor();
	Refresh();
}

void wxWorkspaceView::OnRightMouseDown(wxMouseEvent& event)
{
	if (InteractionState == InteractionStateNormal)
	{
		if (event.ControlDown())
		{
			InitialMouseDownPosition = event.GetPosition();
			InteractionState = InteractionStateZoom;
			ZoomFactorStart = ViewState.ZoomFactor;
			PanningOffsetStart = ViewState.Offset;
			CaptureMouse();
		}
	}
	
	UpdateCursor();
}

void wxWorkspaceView::OnRightMouseUp(wxMouseEvent& event)
{
	if (InteractionState == InteractionStateZoom)
	{
		InteractionState = InteractionStateNormal;
		UpdateCursor();
		ReleaseMouse();
	}
}

void wxWorkspaceView::OnMouseMove(wxMouseEvent& event)
{
	if (InteractionState == InteractionStatePan && event.ControlDown())
	{
		ViewState.Offset = PanningOffsetStart + (event.GetPosition() - InitialMouseDownPosition);
		Refresh();
	}
	else if (InteractionState == InteractionStateZoom && event.ControlDown())
	{
		float ZoomScale = (1.0f + (event.GetPosition().y - InitialMouseDownPosition.y) / 50.0f);
		
		if (ZoomFactorStart * ZoomScale < MinimumZoomFactor)
			ZoomScale = MinimumZoomFactor / ZoomFactorStart;
		
		ViewState.ZoomFactor = ZoomFactorStart * ZoomScale;
		
		wxPoint ClickWorldPos = (InitialMouseDownPosition - PanningOffsetStart);
		ClickWorldPos.x /= ZoomFactorStart;
		ClickWorldPos.y /= ZoomFactorStart;
		
		ViewState.Offset = ClickWorldPos;
		ViewState.Offset.x *= ZoomFactorStart * (1.0f - ZoomScale);
		ViewState.Offset.y *= ZoomFactorStart * (1.0f - ZoomScale);
		ViewState.Offset += PanningOffsetStart;

		Refresh();
	}
	else if (InteractionState == InteractionStateSelect)
	{
		wxRect TempRect;
		TempRect.SetTopLeft(event.GetPosition());
		TempRect.SetRightBottom(TempRect.GetTopLeft());
		TempRect = ViewState.ScreenToWorld(TempRect);

		SelectionRect.SetRightBottom(TempRect.GetRightBottom());
	}
	else if (InteractionState == InteractionStateConnect)
	{
		wxPoint WorldPosition = ViewState.ScreenToWorld(event.GetPosition());
		
		if (ItemsArray.size() > 0)
		{
			WorkspaceView::Cable* Cable = (WorkspaceView::Cable*)ItemsArray.back();
			Cable->SetFloatingPosition(WorldPosition);
		}	
	}
	else if (InteractionState == InteractionStateMove)
	{
		wxPoint Movement = (event.GetPosition() - InitialMouseDownPosition);
		Movement.x /= ViewState.ZoomFactor;
		Movement.y /= ViewState.ZoomFactor;
		
		std::vector<WorkspaceView::Item*> ItemsThatWereMoved;
		
		for (size_t i = 0; i < ItemsArray.size(); ++i)
		{
			if (ItemsArray[i]->IsSelected())
			{
				const wxRect& OrigPosition = SelectedItemPositionsOnMoveStart[ItemsArray[i]];
				
				ItemsThatWereMoved.push_back(ItemsArray[i]);

				ItemsArray[i]->SetPosition(OrigPosition.GetTopLeft() + Movement);
			}
		}

		if (!ItemsThatWereMoved.empty())
		{
			for (size_t i = 0; i < ItemsArray.size(); ++i)
				ItemsArray[i]->ReactToItemsMoved(ItemsThatWereMoved);				
		}
	}

	Refresh();
}

void wxWorkspaceView::OnMouseWheel(wxMouseEvent& event)
{
	// TODO: Implement zooming...
}

void wxWorkspaceView::OnMouseCaptureLostEvent(wxMouseCaptureLostEvent& event)
{
	// Must be declared else the app will crash :)
}

void wxWorkspaceView::OnKeyDown(wxKeyEvent& event)
{
	switch (event.GetKeyCode())
	{	
	case WXK_DELETE:
		DeleteSelection();
		break;
	}
}

void wxWorkspaceView::OnEraseBackGround(wxEraseEvent& WXUNUSED(event))
{
	// Empty to avoid flickering.
}

void wxWorkspaceView::UpdateCursor()
{
	switch (InteractionState)
	{
	case InteractionStatePan:
		SetCursor(CursorList[CursorPan]);
		break;

	case InteractionStateZoom:
		SetCursor(CursorList[CursorZoom]);
		break;

	case InteractionStateNormal:
		SetCursor(CursorList[CursorNormal]);
		break;

	case InteractionStateSelect:
		SetCursor(CursorList[CursorSelect]);
		break;

	case InteractionStateConnect:
		SetCursor(CursorList[CursorConnect]);
		break;

	default:
		SetCursor(CursorList[CursorMove]);
		break;
	}
}