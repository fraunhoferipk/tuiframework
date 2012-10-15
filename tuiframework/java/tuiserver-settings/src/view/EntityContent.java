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
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.RenderingHints;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.TreeMap;

import configmodel.*;

/**
 * EntityContent
 * 
 * @author Oliver Belaifa
 */
public class EntityContent {
	
	private Hashtable<String, PortSet> entries = new Hashtable<String, PortSet>();
	private int height = 0;
	
	public EntityContent(EntityType type) {
		Hashtable<String, Port> portMap = type.getPortMap();
		Enumeration<Port> e = portMap.elements();
		while (e.hasMoreElements()) {
			this.addPort(e.nextElement());
		}
	}
	
	public void addPort(Port port) {
		if ( ! this.entries.containsKey(port.getTypeName())) {
			this.entries.put(port.getTypeName(), new PortSet(port.getTypeName()));
		}
		PortSet e = this.entries.get(port.getTypeName());
		e.add(port);
	}
	
	
	public PortEntry getPortEntry(String portName) {
		PortEntry portEntry = null;
		
		Enumeration<PortSet> iter = this.entries.elements();
		while (iter.hasMoreElements() && portEntry == null) {
			TreeMap<String, PortEntry> portEntryMap = iter.nextElement().getPortEntryMap();
			portEntry = portEntryMap.get(portName);
		}
		
		return portEntry;
	}
	
	
	public Dimension minSize(Graphics2D g2d) {
		Dimension d = new Dimension(0, 0);
		
		Enumeration<PortSet> e = this.entries.elements();
		while (e.hasMoreElements()) {
			Dimension d2 = e.nextElement().minSize(g2d);
			d.width = Math.max(d.width, d2.width);
			d.height += d2.height;
		}
		
		return d;
	}
	
	public void calculateOffsets(Graphics2D g2d, int width) {
		int heightOffset = 0;
		Enumeration<PortSet> e = this.entries.elements();
		while (e.hasMoreElements()) {
			PortSet p = e.nextElement();
			p.calculateOffsets(g2d, width, heightOffset);
			heightOffset += p.getHeight();
		}
		this.height = heightOffset;
	}
	
	
	public void draw(Graphics2D g2d, Point offset, int width, boolean selected) {
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
		g2d.setColor(new Color(255, 255, 255));
		g2d.fillRect(offset.x, offset.y, width, this.height);
		
		int heightOffset = 0;
		Enumeration<PortSet> e = this.entries.elements();
		while (e.hasMoreElements()) {
			g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
			g2d.setColor(new Color(196, 196, 196));
			g2d.drawLine(offset.x, offset.y + heightOffset, offset.x + width, offset.y + heightOffset);
			
			PortSet p = e.nextElement();
			heightOffset += p.getHeight();
			p.draw(g2d, offset, width);
		}
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
		if (selected) {
			g2d.setStroke(new BasicStroke(2));
			g2d.setColor(Color.red);
		} else {
			g2d.setColor(new Color(64, 64, 64));
		}
		
		g2d.drawRect(offset.x, offset.y, width, this.height);
		g2d.setStroke(new BasicStroke(1));
	}
	
	public PortEntry positionWithinPort(Point offset, Point pos) {
		Enumeration<PortSet> e = this.entries.elements();
		while (e.hasMoreElements()) {
			PortEntry portEntry = e.nextElement().positionWithinPort(offset, pos);
			if (portEntry != null) {
				return portEntry;
			}
		}
		return null;
	}
	
	/*
	public boolean isSourcePort(String portName) {
		Enumeration<PortSet> e = this.entries.elements();
		while (e.hasMoreElements()) {
			if (e.nextElement().isSourcePort(portName)) {
				return true;
			}
		}
		return false;
	}
*/

}
