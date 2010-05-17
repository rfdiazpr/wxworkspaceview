#include "stdafx.h"
#include "ThijzWorkspace.h"
#include "BasicWorkspace.h"

class wxWorkspaceFrame: public wxFrame
{
public:
	wxWorkspaceFrame(wxFrame *parent, wxWindowID id = wxID_ANY, 
		const wxString& title = _T("wxWorkspaceView Sample"), 
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE|wxCLIP_CHILDREN|wxNO_FULL_REPAINT_ON_RESIZE)
		: wxFrame(parent, id, title, pos, size, style)
	{
		WorkspaceView::ThijzWorkspaceFactory* Factory = new WorkspaceView::ThijzWorkspaceFactory();
		//WorkspaceView::BasicWorkspaceFactory* Factory = new WorkspaceView::BasicWorkspaceFactory();

		Workspace = new wxWorkspaceView(Factory, this);
		//Workspace->SetWatermark(wxT("Watermark.jpg"), WatermarkPositionBottomRight);
		//Workspace->SetBackColor(wxColor(0, 0, 0));
		//Workspace->SetGridColor(wxColor(60, 60, 60));
		//Workspace->SetAntiAlias(false);

		WorkspaceView::Node* Item1 = Factory->CreateNode(wxRect(100, 100, 100, 100), 2, 4, wxT("This is Sample Node 1"));
		WorkspaceView::Node* Item2 = Factory->CreateNode(wxRect(300, 300, 100, 100), 2, 2, wxT("Sample Node 2"));
		WorkspaceView::Cable* Item3 = Factory->CreateCable(Item1, Item2, 0, 0);

		Item1->Select();

		Workspace->AddItem(Item1);
		Workspace->AddItem(Item2);
		Workspace->AddItem(Item3);
	}
private:
	wxWorkspaceView *Workspace;
};

class wxWorkspaceApplication: public wxApp
{
public:
	virtual bool OnInit()
	{
		// Create the main frame window
		wxWorkspaceFrame *Frame =  new wxWorkspaceFrame(
			NULL, wxID_ANY, wxT("wxWorkspaceView Sample"), 
			wxDefaultPosition, wxSize(640, 480));
		Frame->Show(true);

		return true;
	}
};

IMPLEMENT_APP(wxWorkspaceApplication)