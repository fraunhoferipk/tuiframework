/*
    Copyright (C) 2010, 2011, 2012 The Fraunhofer Institute for Production Systems and
    Design Technology IPK. All rights reserved.

    This file is part of the TUIFramework library.
    It includes a software framework which contains common code
    providing generic functionality for developing applications
    with a tangible user interface (TUI).
    
    The TUIFramework library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    The TUIFramework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with the TUIFramework.  If not, see <http://www.gnu.org/licenses/>.
*/


package view;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Component;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.dnd.DropTarget;
import java.awt.dnd.DropTargetDragEvent;
import java.awt.dnd.DropTargetDropEvent;
import java.awt.dnd.DropTargetEvent;
import java.awt.dnd.DropTargetListener;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.awt.event.MouseMotionListener;
import java.awt.font.FontRenderContext;
import java.awt.font.LineMetrics;
import java.awt.geom.CubicCurve2D;
import java.awt.geom.Area;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Stack;
import java.util.TreeMap;


import javax.swing.DefaultListModel;
import javax.swing.JDialog;
import javax.swing.JList;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;

import project.Project;

import com.l2fprod.common.propertysheet.PropertySheetTable;
import com.l2fprod.common.propertysheet.PropertySheetTableModel;

import configmodel.*;



/**
* This code was edited or generated using CloudGarden's Jigloo
* SWT/Swing GUI Builder, which is free for non-commercial
* use. If Jigloo is being used commercially (ie, by a corporation,
* company or business for any purpose whatever) then you
* should purchase a license for each developer using Jigloo.
* Please visit www.cloudgarden.com for details.
* Use of Jigloo implies acceptance of these licensing terms.
* A COMMERCIAL LICENSE HAS NOT BEEN PURCHASED FOR
* THIS MACHINE, SO JIGLOO OR THIS CODE CANNOT BE USED
* LEGALLY FOR ANY CORPORATE OR COMMERCIAL PURPOSE.
*/

/**
 * Playground
 * 
 * @author Oliver Belaifa
 */
public class Playground extends JPanel implements DropTargetListener {
	private enum SelectedType {
		NONE,
		ENTITY,
		PORT,
		CONNECTON
	}
	
	private SelectedType selectedType = SelectedType.NONE;
	private EntityID selectedEntityID = null;
	private Connector selectedConnector = null;
	private PortAddress selectedPortAddress = null;
	private String selectedPortTypeName = null;
	private Point mousePressedPoint = null;
	private Point entityInitialPos = null;
	private Point mouseMovePoint = null;
	
	private Project project = new Project();
	private Hashtable<EntityID, Entity> entities = new Hashtable<EntityID, Entity>();
	private Border border = new Border(50, 50, 50, 50);
	
	private static Color shadowColor = new Color(64, 64, 64);
	
	private boolean reArrange = true;
	
	private JList deviceTypeList = null;
	private DefaultListModel deviceListModel = new DefaultListModel();
	
	private JList mspTypeList = null;
	private DefaultListModel mspListModel = new DefaultListModel();
	
	private JList tuiTypeList = null;
	private DefaultListModel tuiListModel = new DefaultListModel();
	
	private EntityCellRenderer entityCellRenderer;
	
	private ParameterGroupPanel parameterGroupPanel;
	private JPopupMenu entityPopupMenu;
	private JPopupMenu connectionPopupMenu;
	
	public Playground(ParameterGroupPanel parameterGroupPanel) {
		this.parameterGroupPanel = parameterGroupPanel;
	}

	
	public void setProject(Project project, JList deviceTypeList, JList mspTypeList, JList tuiTypeList) {
		
		this.deviceListModel = new DefaultListModel();
		this.mspListModel = new DefaultListModel();
		this.tuiListModel = new DefaultListModel();
		
		this.project = project;
		this.entityCellRenderer = new EntityCellRenderer(this.project.getServerConfigView().getImageDB());
		
		this.deviceTypeList = deviceTypeList;
		this.mspTypeList = mspTypeList;
		this.tuiTypeList = tuiTypeList;
		
		this.deviceTypeList.setModel(this.deviceListModel);
		this.deviceTypeList.setCellRenderer(this.entityCellRenderer);
		this.deviceTypeList.setDragEnabled(true);
		this.setDropTarget(new DropTarget(this.deviceTypeList, this));
				
		this.mspTypeList.setModel(this.mspListModel);
		this.mspTypeList.setCellRenderer(this.entityCellRenderer);
		this.mspTypeList.setDragEnabled(true);
		this.setDropTarget(new DropTarget(this.mspTypeList, this));

		this.tuiTypeList.setModel(this.tuiListModel);
		this.tuiTypeList.setCellRenderer(this.entityCellRenderer);
		this.tuiTypeList.setDragEnabled(true);
		this.setDropTarget(new DropTarget(this.tuiTypeList, this));
		
		{
			this.entities.clear();
			this.project.getServerConfigView().getEntityIDVector().clear();
			this.project.getServerConfigView().getEntityInstancePositions().clear();
			Hashtable<EntityID, EntityInstance> entityInstanceMap = this.project.getServerConfig().getEntityInstanceMap();
			Enumeration<EntityInstance> e = entityInstanceMap.elements();
			while (e.hasMoreElements()) {
				EntityInstance inst = e.nextElement();
				EntityType type = this.project.getEntityTypeDB().getEntityTypeMap().get(inst.getEntityTypeID());
				this.entities.put(inst.getEntityID(), new Entity(inst, type, this.project.getServerConfigView()));
				this.project.getServerConfigView().getEntityIDVector().add(inst.getEntityID());
			}
		}
		
		{
			Hashtable<EntityID, EntityType> entityTypeMap = this.project.getEntityTypeDB().getEntityTypeMap();

			TreeMap<String, EntityType> entityTypeTreeMap = new TreeMap<String, EntityType>();
			Enumeration<EntityType> e = entityTypeMap.elements();
			while (e.hasMoreElements()) {
				EntityType entityType = e.nextElement();
				entityTypeTreeMap.put(entityType.getEntityID().getName(), entityType);
			}

			Collection<EntityType> entityTypeCollection = entityTypeTreeMap.values();
			Iterator<EntityType> i = entityTypeCollection.iterator();
			while (i.hasNext()) {
				EntityType entityType = i.next();
				switch (entityType.getEntityID().getType()) {
				case DEV: {
					this.deviceListModel.addElement(entityType);
					break;
				}
				case MSP: {
					this.mspListModel.addElement(entityType);
					break;
				}
				case TUI: {
					this.tuiListModel.addElement(entityType);
					break;
				}
				}

			}
			/*
			e = entityTypeMap.elements();
			while (e.hasMoreElements()) {
				EntityType entityType = e.nextElement();
				switch (entityType.getEntityID().getType()) {
				case DEV: {
					this.deviceListModel.addElement(entityType);
					break;
				}
				case MSP: {
					this.mspListModel.addElement(entityType);
					break;
				}
				case TUI: {
					this.tuiListModel.addElement(entityType);
					break;
				}
				}

			}
			*/
		}

	}
	
	
	Dimension calculateDimension(Graphics2D g2d) {
		Dimension dimension = new Dimension(0, 0);

		Enumeration<Entity> iter = this.entities.elements();
		while (iter.hasMoreElements()) {
			Entity entity = iter.nextElement();
			Dimension d = entity.minSize(g2d);
			Point p = this.project.getServerConfigView().getEntityInstancePosition(entity.getInst().getEntityID());
			int x = d.width + p.x;
			int y = d.height + p.y;
			
			dimension.width = Math.max(dimension.width, x);
			dimension.height = Math.max(dimension.height, y);
		}
		dimension.width += border.getLeft() + border.getRight();
		dimension.height += border.getTop() + border.getBottom();
		return dimension;
	}

	
	void arrange(Graphics2D g2d) {
		
		Hashtable<EntityID.Type, Dimension> dimensions = new Hashtable<EntityID.Type, Dimension>();
		dimensions.put(EntityID.Type.DEV, new Dimension(0, 0));
		dimensions.put(EntityID.Type.MSP, new Dimension(0, 0));
		dimensions.put(EntityID.Type.TUI, new Dimension(0, 0));

		Enumeration<Entity> iter = this.entities.elements();
		while (iter.hasMoreElements()) {
			Entity entity = iter.nextElement();
			Dimension d = entity.minSize(g2d);
			Dimension d2 = dimensions.get(entity.getInst().getEntityID().getType());
			d2.width = Math.max(d.width + border.getLeft() + border.getRight(), d2.width);
			d2.height += d.height + border.getTop() + border.getRight();
		}
		
		Hashtable<EntityID.Type, Point> offsets = new Hashtable<EntityID.Type, Point>();
		
		int offsetX = border.getLeft();
		offsets.put(EntityID.Type.DEV, new Point(offsetX, 0));
		offsetX += border.getRight() + dimensions.get(EntityID.Type.DEV).width + border.getLeft();
		offsets.put(EntityID.Type.MSP, new Point(offsetX, 0));
		offsetX += border.getRight() + dimensions.get(EntityID.Type.MSP).width + border.getLeft();
		offsets.put(EntityID.Type.TUI, new Point(offsetX, 0));
		
		iter = this.entities.elements();
		while (iter.hasMoreElements()) {
			Entity entity = iter.nextElement();
			Dimension d = entity.minSize(g2d);
			
			Point offset = offsets.get(entity.getInst().getEntityID().getType());
			
			this.project.getServerConfigView().setEntityInstancePosition(entity.getInst().getEntityID(), new Point(offset.x, offset.y +  border.getTop()));
			offset.y += border.getTop() + d.height + border.getBottom();
		}
	}
	
	public PortEntry getPort(PortAddress portAddress) {
		Entity entity = this.entities.get(portAddress.getEntityID());
		EntityContent entityContent = entity.getContent();
		return entityContent.getPortEntry(portAddress.getPortName());
	}
	
	public void mousePressed(Point pos, int button) {
		SelectedType oldSelectedType = this.selectedType;
		this.selectedType = SelectedType.NONE;
		
		{
			Stack<EntityID> entityIDStack = new Stack<EntityID>();
			{
				Iterator<EntityID> iter = this.project.getServerConfigView().getEntityIDVector().iterator();
				while (iter.hasNext()) {
					entityIDStack.push(iter.next());
				}
			}
			
			this.parameterGroupPanel.setProperties(new ParameterProperty[0]);
			
			//Iterator<EntityID> iter = this.serverConfigView.getEntityIDVector().iterator();
			//while (iter.hasNext()) {
				//Entity entity = this.entities.get(iter.next());
			while ( ! entityIDStack.isEmpty()) {
				Entity entity = this.entities.get(entityIDStack.pop());
				if (entity.posWithinTitleBar(pos)) {
					this.selectedEntityID = entity.getInst().getEntityID();
					this.selectedType = SelectedType.ENTITY;
					this.project.getServerConfigView().getEntityIDVector().remove(this.selectedEntityID);
					this.project.getServerConfigView().getEntityIDVector().add(this.selectedEntityID);
					System.out.println(entity.getInst().getEntityID().getName());
					this.mousePressedPoint = (Point)pos.clone();
					this.entityInitialPos = this.project.getServerConfigView().getEntityInstancePosition(this.selectedEntityID);
					
					EntityInstance inst = this.project.getServerConfig().getEntityInstanceMap().get(this.selectedEntityID);
					this.parameterGroupPanel.setProperties(new ParameterProperty[0]);
					this.parameterGroupPanel.addParameterGroupMap(inst.getParameterGroupMap());
					this.parameterGroupPanel.expand();
					
					if (button == MouseEvent.BUTTON3) {
						this.getEntityPopupMenu().show((Component)this, (int)pos.getX(), (int)pos.getY());
					}
					
					break;
				}
				
				PortEntry portEntry = entity.positionWithinPort(pos);
				if (portEntry != null) {
					System.out.println(portEntry.getText());
					if (oldSelectedType == SelectedType.PORT) {
						if ( ! portEntry.isConnected() && portEntry.getOwner().getTypeName().equals(this.selectedPortTypeName)) {
							
							PortEntry selectedPortEntry = this.entities.get(this.selectedPortAddress.getEntityID()).getContent().getPortEntry(this.selectedPortAddress.getPortName());
							if (portEntry.isSource() != selectedPortEntry.isSource()) {
								PortAddress portAddress = new PortAddress(entity.getInst().getEntityID(), portEntry.getText());
								if (portEntry.isSource()) {
									this.project.getServerConfig().getConnectorVector().add(new Connector(portAddress, this.selectedPortAddress));
								} else {
									this.project.getServerConfig().getConnectorVector().add(new Connector(this.selectedPortAddress, portAddress));
								}
							} else {
								this.selectedType = SelectedType.PORT;
							}
						} else {
							this.selectedType = SelectedType.PORT;
						}
	
					} else {
						if ( ! portEntry.isConnected()) {
							this.selectedType = SelectedType.PORT;
							this.selectedPortAddress = new PortAddress(entity.getInst().getEntityID(), portEntry.getText());
							this.selectedPortTypeName = portEntry.getOwner().getTypeName();
						}
					}
				}
			}
		}
		{
			if (this.selectedType == SelectedType.NONE) {
				Iterator<Connector> iter = this.project.getServerConfig().getConnectorVector().iterator();
				while (iter.hasNext()) {
					Connector connector = iter.next();
				    PortAddress srcAddress = connector.getSrcAddress();
					PortAddress dstAddress = connector.getDstAddress();
					 
					Entity srcEntity = this.entities.get(srcAddress.getEntityID());
					Entity dstEntity = this.entities.get(dstAddress.getEntityID());
					 
					PortEntry srcPortEntry = srcEntity.getContent().getPortEntry(srcAddress.getPortName());
					PortEntry dstPortEntry = dstEntity.getContent().getPortEntry(dstAddress.getPortName());
					
					Point srcPoint = new Point(srcPortEntry.getConnectionPoint().x + srcEntity.getContentPos().x,
							srcPortEntry.getConnectionPoint().y + srcEntity.getContentPos().y);
					
					Point dstPoint = new Point(dstPortEntry.getConnectionPoint().x + dstEntity.getContentPos().x,
							dstPortEntry.getConnectionPoint().y + dstEntity.getContentPos().y);
					
					CubicCurve2D.Double curve = getCurve(srcPoint, dstPoint);
					
					//Create Area object from this curve to accurately calculate intersection
					Area curveArea = new Area(curve);
					
					if (curveArea.intersects(pos.x - 2, pos.y - 2, 8, 8)) {
						System.out.println(srcAddress.getPortName() + " --> " + dstAddress.getPortName());
						this.selectedType = SelectedType.CONNECTON;
						this.selectedConnector = connector;
						if (button == MouseEvent.BUTTON3) {
							this.getConnectionPopupMenu().show((Component)this, (int)pos.getX(), (int)pos.getY());
						}
					}
				}
			}
		}
		
		this.repaint();
	}
	
	public void mouseReleased(Point pos) {
	}
	
	public void mouseMoved(Point pos) {
		System.out.println(pos);
		this.mouseMovePoint = pos;
		
		{
			Enumeration<Entity> iter = this.entities.elements();
			while (iter.hasMoreElements()) {
				Entity entity = iter.nextElement();
				if (entity.posWithinTitleBar(pos)) {
					System.out.println(entity.getInst().getEntityID().getName());
				}
				PortEntry portEntry = entity.positionWithinPort(pos);
				if (portEntry != null) {
					System.out.println(portEntry.getText());
				}
			}
		}
		
		Iterator<Connector> iter = this.project.getServerConfig().getConnectorVector().iterator();
		while (iter.hasNext()) {
			Connector connector = iter.next();
		    PortAddress srcAddress = connector.getSrcAddress();
			PortAddress dstAddress = connector.getDstAddress();
			 
			Entity srcEntity = this.entities.get(srcAddress.getEntityID());
			Entity dstEntity = this.entities.get(dstAddress.getEntityID());
			 
			PortEntry srcPortEntry = srcEntity.getContent().getPortEntry(srcAddress.getPortName());
			PortEntry dstPortEntry = dstEntity.getContent().getPortEntry(dstAddress.getPortName());
			
			Point srcPoint = new Point(srcPortEntry.getConnectionPoint().x + srcEntity.getContentPos().x,
					srcPortEntry.getConnectionPoint().y + srcEntity.getContentPos().y);
			
			Point dstPoint = new Point(dstPortEntry.getConnectionPoint().x + dstEntity.getContentPos().x,
					dstPortEntry.getConnectionPoint().y + dstEntity.getContentPos().y);
			
			CubicCurve2D.Double curve = getCurve(srcPoint, dstPoint);
			
			if (curve.intersects(pos.x - 4, pos.y - 4, 8, 8)) {
				System.out.println(srcAddress.getPortName() + " --> " + dstAddress.getPortName());
			}
		}
		
		if (this.selectedType == SelectedType.PORT) {
			this.repaint();
		}
		this.deviceTypeList.clearSelection();
		this.mspTypeList.clearSelection();
		this.tuiTypeList.clearSelection();
	}
	
	public void mouseDragged(Point pos) {
		if (this.selectedType == SelectedType.ENTITY) {
			Point p = new Point(this.entityInitialPos.x + pos.x - this.mousePressedPoint.x,
						this.entityInitialPos.y + pos.y - this.mousePressedPoint.y);
			if (p.x < 0) {
				p.x = 0;
			}
			if (p.y < 0) {
				p.y = 0;
			}
			this.project.getServerConfigView().setEntityInstancePosition(this.selectedEntityID, p);
			
			this.repaint();
		}
	}
	
	
	public void deleteSelection() {
		if (this.selectedType == SelectedType.CONNECTON) {
			this.project.getServerConfig().getConnectorVector().remove(this.selectedConnector);
			this.selectedType = SelectedType.NONE;
			this.repaint();
		}
		if (this.selectedType == SelectedType.ENTITY) {
			Editor editor = new Editor(this.project);
			editor.removeEntity(this.selectedEntityID);
			this.entities.remove(this.selectedEntityID);
			this.project.getServerConfigView().getEntityIDVector().remove(this.selectedEntityID);
			this.selectedType = SelectedType.NONE;
			this.parameterGroupPanel.setProperties(new ParameterProperty[0]);
			this.repaint();
		}
		//this.setReArrange(true);
	}
	
	private JPopupMenu getEntityPopupMenu() {
		if(this.entityPopupMenu == null) {
			this.entityPopupMenu = new JPopupMenu();
			JMenuItem name_item = new JMenuItem("Change EntityInstance Name...");
			name_item.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent evt) {
					changeEntityName();
				}
			});
			this.entityPopupMenu.add(name_item);
			
			JMenuItem delete_item = new JMenuItem("Delete Entity");
			delete_item.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent evt) {
					deleteSelection();
				}
			});
			this.entityPopupMenu.add(delete_item);
		}
		return this.entityPopupMenu;
	}
	
	private JPopupMenu getConnectionPopupMenu() {
		if(this.connectionPopupMenu == null) {
			this.connectionPopupMenu = new JPopupMenu();
			JMenuItem delete_item = new JMenuItem("Delete Connection");
			delete_item.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent evt) {
					deleteSelection();
				}
			});
			this.connectionPopupMenu.add(delete_item);
		}
		return this.connectionPopupMenu;
	}
	
	
	public CubicCurve2D.Double getCurve(Point srcPoint, Point dstPoint) {
		int vx = dstPoint.x - srcPoint.x;
		int vy = dstPoint.y - srcPoint.y;
		int cdelta = (int)Math.sqrt(vx*vx + vy*vy);
		
		CubicCurve2D.Double curve = new CubicCurve2D.Double(
				srcPoint.x,
				srcPoint.y,
				srcPoint.x + cdelta/4,
				srcPoint.y,
				dstPoint.x - 10 - cdelta/4,
				dstPoint.y,
				dstPoint.x - 10,
				dstPoint.y);
		
		return curve;
	}
	
	
	public void drawConnectorCurve(Graphics2D g2d, Point srcPoint, Point dstPoint) {
		CubicCurve2D.Double curve = getCurve(srcPoint, dstPoint);
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		g2d.setStroke(new BasicStroke(2));
		g2d.draw(curve);
		g2d.fillPolygon(new int[] { 
				dstPoint.x,
				dstPoint.x - 10,
				dstPoint.x - 10},
				new int[] { 
				dstPoint.y,
				dstPoint.y + 6,
				dstPoint.y - 6 } , 3);
		g2d.setStroke(new BasicStroke(1));		
	}
	
	
	public void drawConnector(Graphics2D g2d, Connector connector) {
	    PortAddress srcAddress = connector.getSrcAddress();
		PortAddress dstAddress = connector.getDstAddress();
		 
		Entity srcEntity = this.entities.get(srcAddress.getEntityID());
		Entity dstEntity = this.entities.get(dstAddress.getEntityID());
		 
		PortEntry srcPortEntry = srcEntity.getContent().getPortEntry(srcAddress.getPortName());
		PortEntry dstPortEntry = dstEntity.getContent().getPortEntry(dstAddress.getPortName());
		 
		srcPortEntry.setConnected(true);
		dstPortEntry.setConnected(true);
		

		this.drawConnectorCurve(g2d, 
				new Point(srcPortEntry.getConnectionPoint().x + srcEntity.getContentPos().x,
						srcPortEntry.getConnectionPoint().y + srcEntity.getContentPos().y),
				new Point(dstPortEntry.getConnectionPoint().x + dstEntity.getContentPos().x,
						dstPortEntry.getConnectionPoint().y + dstEntity.getContentPos().y));
	}

	public boolean isReArrange() {
		return this.reArrange;
	}

	public void setReArrange(boolean reArrange) {
		this.reArrange = reArrange;
	}

	@Override
	protected void paintComponent(Graphics g) {
		 super.paintComponent(g);
				
		 Graphics2D g2d = (Graphics2D) g;
		 
		 if (this.reArrange) {
			 this.arrange(g2d);
			 this.reArrange = false;
		 }
		 //Point offset = new Point(pos.x, pos.y);
		 //Point offset = new Point(55, 55);
		 
		 Dimension d = this.calculateDimension(g2d);
		 this.setPreferredSize(d);
		 
         //Let the scroll pane know to update itself
         //and its scrollbars.
         this.revalidate();
		 
		 Enumeration<Entity> e = this.entities.elements();
		 while(e.hasMoreElements()) {
			 e.nextElement().calculateOffsets(g2d);
		 }

		 Iterator<Connector> iter = this.project.getServerConfig().getConnectorVector().iterator();
		 while (iter.hasNext()) {
			 Connector connector = iter.next();
			 if (this.selectedType == SelectedType.CONNECTON && selectedConnector.equals(connector)) {
				 g2d.setColor(Color.red);
				 this.drawConnector(g2d, connector);
			 } else {
				 g2d.setColor(shadowColor);
				 this.drawConnector(g2d, connector);
			 }
		 }
		 
		 if (this.selectedType == SelectedType.PORT) {
			 Entity entity = this.entities.get(this.selectedPortAddress.getEntityID());
			 PortEntry portEntry = entity.getContent().getPortEntry(this.selectedPortAddress.getPortName());
			 
			 Point connectionPoint = new Point(
					 portEntry.getConnectionPoint().x + entity.getContentPos().x,
					 portEntry.getConnectionPoint().y + entity.getContentPos().y
					 );
			 
			 if (portEntry.isSource()) {
				 this.drawConnectorCurve(g2d, connectionPoint, this.mouseMovePoint);	 
			 } else {
				 this.drawConnectorCurve(g2d, this.mouseMovePoint, connectionPoint);
			 }
			 
		 }
		 /*
		 if (this.selectedType == SelectedType.CONNECTON) {
			 g2d.setColor(Color.red);
			 this.drawConnector(g2d, this.selectedConnector);
		 }
		 */

		 Iterator<EntityID> iter2 = this.project.getServerConfigView().getEntityIDVector().iterator();
		 while (iter2.hasNext()) {
			 Entity entity = this.entities.get(iter2.next());
			 if (this.selectedType == SelectedType.ENTITY && entity.getInst().getEntityID().equals(this.selectedEntityID)) {
				 entity.draw(g2d, true);
			 } else {
				 entity.draw(g2d, false);
			 }			 
		 }
		 /*
		 e = this.entities.elements();
		 while(e.hasMoreElements()) {
			 Entity entity = e.nextElement();
			 if (this.selectedType == SelectedType.ENTITY && entity.getInst().getEntityID().equals(this.selectedEntityID)) {
				 entity.draw(g2d, true);
			 } else {
				 entity.draw(g2d, false);
			 }
		 }
		 */
		 
	}

	@Override
	public void dragEnter(DropTargetDragEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void dragExit(DropTargetEvent e) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void dragOver(DropTargetDragEvent e) {
		// TODO Auto-generated method stub
		
	}

	
	@Override
	public void drop(DropTargetDropEvent e) {
		EntityType type = null;
		if (!this.tuiTypeList.isSelectionEmpty()) {
			type = (EntityType)this.tuiTypeList.getSelectedValue();
		}
		
		if (!this.mspTypeList.isSelectionEmpty()) {
			type = (EntityType)this.mspTypeList.getSelectedValue();
		}
		
		if (!this.deviceTypeList.isSelectionEmpty()) {
			type = (EntityType)this.deviceTypeList.getSelectedValue();
		}
		
		this.tuiTypeList.clearSelection();
		this.mspTypeList.clearSelection();
		this.deviceTypeList.clearSelection();
		
		//EntityType type = (EntityType)this.tuiTypeList.getSelectedValue();
		//System.out.println(type.getEntityID().getName());
		
		Editor editor = new Editor();
		editor.setProject(this.project);
		
		try {
			int nr = 1;
			while (this.project.getServerConfig().getEntityInstanceMap().containsKey(new EntityID("Unnamed"+nr, type.getEntityID().getType()))) {
				++nr;
			}
			EntityInstance inst = editor.createInstance("Unnamed"+ nr++, type.getEntityID(), "");
			Entity entity = new Entity(inst, type, this.project.getServerConfigView());
			this.project.getServerConfigView().setEntityInstancePosition(inst.getEntityID(), e.getLocation());
			this.entities.put(inst.getEntityID(), entity);
			this.project.getServerConfigView().getEntityIDVector().add(inst.getEntityID());
			this.repaint();
		} catch (InternalException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		// TODO Auto-generated method stub
		
	}

	@Override
	public void dropActionChanged(DropTargetDragEvent arg0) {
		// TODO Auto-generated method stub
	}
	
	public void changeEntityName() {
		//JOptionPane.showMessageDialog(null, "Test", "Test Titel", JOptionPane.OK_CANCEL_OPTION);
		if (this.selectedType == SelectedType.ENTITY) {
			String newName = JOptionPane.showInputDialog("Please enter a new name for the selected entity.");
			if (newName == null || newName.isEmpty()) {
				//JOptionPane.showMessageDialog(null, "Name unchanged", "Information", JOptionPane.OK_CANCEL_OPTION);
				return;
			}
			newName = newName.trim();
			newName = newName.replace(' ', '_');
			
			EntityID entityID = new EntityID(newName, this.selectedEntityID.getType());
			if (this.project.getServerConfig().getEntityInstanceMap().containsKey(entityID)) {
				JOptionPane.showMessageDialog(null, "A " + entityID.getTypeString() + " instance with name \"" + entityID.getName() + "\" already exist.", "Information", JOptionPane.OK_CANCEL_OPTION);
				return;
			}

			EntityInstance inst = this.project.getServerConfig().getEntityInstanceMap().remove(this.selectedEntityID);
			inst.setEntityID(entityID);
			this.project.getServerConfig().getEntityInstanceMap().put(inst.getEntityID(), inst);
			
			Iterator<Connector> iter = this.project.getServerConfig().getConnectorVector().iterator();
			while (iter.hasNext()) {
				Connector connector = iter.next();
				if (connector.getSrcAddress().getEntityID().equals(this.selectedEntityID)) {
					connector.getSrcAddress().setEntityID(inst.getEntityID());
				}
				if (connector.getDstAddress().getEntityID().equals(this.selectedEntityID)) {
					connector.getDstAddress().setEntityID(inst.getEntityID());
				}
			}
			
			Entity entity = this.entities.remove(this.selectedEntityID);
			entity.setInst(inst);
			this.entities.put(inst.getEntityID(), entity);
			
			this.project.getServerConfigView().changeEntityID(this.selectedEntityID, inst.getEntityID());
			
			this.selectedEntityID = inst.getEntityID();
			this.repaint();
		}
	}
	
	public void createNewTUIObjectType() {
		TUIObjectDialogue tod = new TUIObjectDialogue(this);
	}
	
	public void addTUIObjectTypeToModel(EntityType newTUIObjectType) {
		tuiListModel.addElement(newTUIObjectType);
		project.getEntityTypeDB().getEntityTypeMap().put(newTUIObjectType.getEntityID(), newTUIObjectType);
		System.out.println("Added the new tui object type");
		//tuiListModel.fireContentsChanged();
	}
	
}
