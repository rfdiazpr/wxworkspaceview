/**
 * @author Thijs Kruithof, Walter Tamboer
 */

#pragma once

#include "Item.h"
#include <vector>

namespace WorkspaceView
{
	/**
	 * This struct represents information about a connector.
	 */
	struct PortInfo
	{
		/**
		 * The label of this port.
		 */
		wxString Label;
		/**
		 * The area in which a user can click to start drawing cables.
		 */
		wxRect Area;
	};

	/**
	 * This is the base class for nodes which can be drawn on the workspace.
	 * Nodes should be created through the WorkspaceFactory.
	 */
	class Node: public Item
	{
	public:
		/**
		 * Initializes a new instance of this class.
		 */
		Node(const wxRect& Area, int InputCount, int OutputCount, const wxString& Title);
	public:
		/**
		 * Adds a new input port to this node and returns the index of it.
		 */
		virtual size_t AddInputPort(const PortInfo& Port);
		/**
		 * Adds a new output port to this node and returns the index of it.
		 */
		virtual size_t AddOutputPort(const PortInfo& Port);
		/**
		 * Removes an input port by its index.
		 */
		void RemoveInputPort(size_t Index);
		/**
		 * Removes an input port by its index.
		 */
		void RemoveOutputPort(size_t Index);
	public:
		/**
		 * Returns true if this items hits with an connector.
		 */
		virtual bool HasHitWithConnector(const wxPoint& Position, ConnectorInfo* Result);
	public:
		/**
		 * Gets the area of a specific port.
		 */
		wxRect GetPortArea(int PortIndex, bool Output) const;
	public:
		/**
		 * Called when the zoom of the workspace has been changed.
		 */
		virtual void OnZoom(float NewZoom);
	protected:
		/**
		 * The output connector list.
		 */
		std::vector<PortInfo> OutputList;
		/**
		 * The input connector list.
		 */
		std::vector<PortInfo> InputList;
	protected:
		/**
		 * The area of an input port.
		 */
		wxSize InputPortSize;
		/**
		 * The padding between input ports.
		 */
		int InputPortPadding;
		/**
		 * The area of an output port.
		 */
		wxSize OutputPortSize;
		/**
		 * The padding between output ports.
		 */
		int OutputPortPadding;
	protected:
		/**
		 * The title of this node.
		 */
		wxString Title;
		/**
		 * The font of the title.
		 */
		wxFont TitleFont;
	};
}