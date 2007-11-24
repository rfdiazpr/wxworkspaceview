/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#include "Node.h"

namespace WorkspaceView
{
	Node::Node(const wxRect& Area, int InputCount, int OutputCount, const wxString& Title)
		: Item(WorkspaceItemTypeNode, Area), Title(Title)
	{
	}
	
	size_t Node::AddInputPort(const PortInfo& Port)
	{
		InputList.push_back(Port);

		return InputList.size() - 1;
	}
	
	size_t Node::AddOutputPort(const PortInfo& Port)
	{
		OutputList.push_back(Port);

		return OutputList.size() - 1;
	}
	
	void Node::RemoveInputPort(size_t Index)
	{
		InputList.erase(InputList.begin() + Index);
	}
	
	void Node::RemoveOutputPort(size_t Index)
	{
		OutputList.erase(OutputList.begin() + Index);
	}

	bool Node::HasHitWithConnector(const wxPoint& Position, ConnectorInfo* Result)
	{
		wxPoint RelativePoint = Position - GetArea().GetTopLeft();
		
		for (size_t Index = 0; Index < OutputList.size(); ++Index)
		{
			if (OutputList[Index].ConnectorArea.Contains(RelativePoint))
			{
				Result->Owner = this;
				Result->PortIndex = (int)Index;
				Result->PortIsOutput = true;
				return true;
			}
		}

		for (size_t Index = 0; Index < InputList.size(); ++Index)
		{
			if (InputList[Index].ConnectorArea.Contains(RelativePoint))
			{
				Result->Owner = this;
				Result->PortIndex = (int)Index;
				Result->PortIsOutput = false;
				return true;
			}
		}
		
		return false;
	}

	wxRect Node::GetPortArea(int PortIndex, bool Output) const
	{
		wxRect PortArea;
		
		if (Output)
		{
			if ((int)OutputList.size() <= PortIndex || PortIndex < 0)
				PortArea = wxRect(0, 0, 0, 0);
			else
				PortArea = OutputList[PortIndex].ConnectorArea;
		}
		else
		{
			if ((int)InputList.size() <= PortIndex || PortIndex < 0)
				PortArea = wxRect(0, 0, 0, 0);
			else
				PortArea = InputList[PortIndex].ConnectorArea;
		}

		PortArea.Offset(GetArea().GetTopLeft());

		return PortArea;
	}
}