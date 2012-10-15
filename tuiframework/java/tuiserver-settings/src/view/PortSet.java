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

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.RenderingHints;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.SortedMap;
import java.util.TreeMap;

import configmodel.EntityInstance;
import configmodel.EntityType;
import configmodel.Port;

/**
 * PortSet
 * 
 * @author Oliver Belaifa
 */
public class PortSet {
	private static Font portTypeFont = new Font("Arial", Font.BOLD, 14);
	private static Font portNameFont = new Font("Arial", Font.PLAIN, 13);
	private static Color portNameFontColor = new Color(0, 0, 0);
	private static Color portTypeFontColor = new Color(0, 0, 0);
	
	private static int lineLeading = 6;
	private static int middleLeading = 24;
	private static int portCircleSize = 11;
	private static Border border = new Border(2, 0, 2, 0);
	private static Border portCircleBorder = new Border(4, 2, 4, 2); 
	
	private String typeName;
	private ArrayList<String> sources = new ArrayList<String>();
	private ArrayList<String> sinks = new ArrayList<String>();
	
	private Point portTypeTextOffset = new Point(0, 0);
	//private Hashtable<String, PortEntry> portEntryMap = new Hashtable<String, PortEntry>();
	private TreeMap<String, PortEntry> portEntryMap = new TreeMap<String, PortEntry>();

	private int typeHeight = 0;
	private int height = 0;
	private int heightOffset = 0;
	
	public PortSet(String typeName) {
		this.typeName = typeName;
	}
	
	public void add(Port port) {
		if (port.getDataFlowDirection() == Port.DataFlowDirection.Source) {
			this.sources.add(port.getName());
		} else {
			this.sinks.add(port.getName());
		}
	}

	public int getHeight() {
		return this.height;
	}
	
	public int getTypeHeight() {
		return this.typeHeight;
	}
	/*
	public Hashtable<String, PortEntry> getPortEntryMap() {
		return portEntryMap;
	}
	*/
	public TreeMap<String, PortEntry> getPortEntryMap() {
		return portEntryMap;
	}
	
	public Dimension minSize(Graphics2D g2d) {
		
		FontMetrics f = g2d.getFontMetrics(portTypeFont);
		int mth = f.getAscent() + f.getDescent() + lineLeading + border.getTop();
		int mtw = f.stringWidth(this.typeName);
		
		int mh1 = 0;
		int mw1 = 0;
		{
			Iterator<String> iter = sinks.iterator();
			while (iter.hasNext()) {
				f = g2d.getFontMetrics(portNameFont);
				mh1 += f.getAscent() + f.getDescent() + lineLeading;
				mw1 = Math.max(mw1, f.stringWidth(iter.next()) + portCircleBorder.getLeft() + portCircleSize + portCircleBorder.getRight());
			}
		}

		int mh2 = 0;
		int mw2 = 0;
		{
			Iterator<String> iter = sources.iterator();
			while (iter.hasNext()) {
				f = g2d.getFontMetrics(portNameFont);
				mh2 += f.getAscent() + f.getDescent() + lineLeading;
				mw2 = Math.max(mw2, f.stringWidth(iter.next()) + portCircleBorder.getLeft() + portCircleSize + portCircleBorder.getRight() );
			}
		}

		int mw = Math.max(mtw, mw1 + middleLeading + mw2);
		int mh = mth + Math.max(mh1, mh2);
		
		this.height = mh + border.getTop() + border.getBottom();
		
		return new Dimension(mw + border.getLeft() + border.getRight(), mh + border.getTop() + border.getBottom());
	}
	
	
	void sortPorts() {
		{
			String[] stringArray = new String[this.sinks.size()]; 
			this.sinks.toArray(stringArray);
			Arrays.sort(stringArray);
	
			this.sinks.clear();
			for (int i = 0; i < stringArray.length; ++i) {
				this.sinks.add(stringArray[i]);
			}
		}
		{
			String[] stringArray = new String[this.sources.size()]; 
			this.sources.toArray(stringArray);
			Arrays.sort(stringArray);
	
			this.sources.clear();
			for (int i = 0; i < stringArray.length; ++i) {
				this.sources.add(stringArray[i]);
			}
		}

	}
	
	
	public void calculateOffsets(Graphics2D g2d, int width, int heightOffset) {
		this.heightOffset = heightOffset;
		FontMetrics f = g2d.getFontMetrics(portTypeFont);
		int h = heightOffset + f.getAscent() + f.getDescent() + border.getTop();
		int w = f.stringWidth(this.typeName);
		this.portTypeTextOffset.x = (width - w)/2;
		this.portTypeTextOffset.y = h;
		
		h +=  lineLeading;
		this.typeHeight = h;
		
		this.sortPorts();
		this.portEntryMap.clear();
		int h1;
		{
			h1 = h;
			Iterator<String> iter = sinks.iterator();
			while (iter.hasNext()) {
				f = g2d.getFontMetrics(portNameFont);
				h1 += f.getAscent() + f.getDescent() + lineLeading;
				
				String portName = iter.next();
				
				this.portEntryMap.put(portName, new PortEntry(
						new Point(border.getLeft() + portCircleBorder.getLeft() + portCircleSize + portCircleBorder.getRight(), h1 - lineLeading),
						new Point(border.getLeft() + portCircleBorder.getLeft(), h1 - portCircleSize - lineLeading),
						portName, h1,
						new Point(0, h1 - portCircleSize/2 - lineLeading), false, this));
			}
		}
		
		
		{
			h1 = h;
			Iterator<String> iter = sources.iterator();
			while (iter.hasNext()) {
				f = g2d.getFontMetrics(portNameFont);
				h1 += f.getAscent() + f.getDescent() + lineLeading;
				String portName = iter.next();
				int w2 = f.stringWidth(portName);
				
				this.portEntryMap.put(portName, new PortEntry(
						new Point(width - border.getRight() - w2 - portCircleBorder.getLeft() - portCircleSize - portCircleBorder.getRight(), h1 - lineLeading),
						new Point(width - border.getRight() - portCircleBorder.getRight() - portCircleSize, h1 - portCircleSize - lineLeading),
						portName, h1,
						new Point(width, h1 - portCircleSize/2 - lineLeading), true, this));
			}
		}
	}
	
	
	public void draw(Graphics2D g2d, Point offset, int width) {
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
		g2d.setColor(new Color(255, 255, 212));
		g2d.fillRect(offset.x, offset.y + this.heightOffset + 1, width, this.typeHeight - this.heightOffset - 1);
		g2d.setColor(new Color(196, 196, 196));
		g2d.drawLine(offset.x, offset.y + this.typeHeight, offset.x + width, offset.y + this.typeHeight);

		g2d.setColor(portTypeFontColor);
		g2d.setFont(portTypeFont);
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		g2d.drawString(this.typeName, offset.x + this.portTypeTextOffset.x, offset.y + this.portTypeTextOffset.y);
		
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
		
		Iterator<PortEntry> iter = this.portEntryMap.values().iterator();
		while (iter.hasNext()) {
		//Enumeration<PortEntry> pe = this.portEntryMap.elements();
		//while (pe.hasMoreElements()) {
			//PortEntry p = pe.nextElement();
			PortEntry p = iter.next();
			if (p.isConnected()) {
				g2d.setColor(new Color(128, 255, 128));
			} else { 
				g2d.setColor(new Color(255, 128, 128));
			}
				
			g2d.fillOval(offset.x + p.getCircleOffset().x, offset.y + p.getCircleOffset().y, portCircleSize, portCircleSize);
			g2d.setColor(new Color(64, 64, 64));
			g2d.drawOval(offset.x + p.getCircleOffset().x, offset.y + p.getCircleOffset().y, portCircleSize, portCircleSize);
			
			g2d.setColor(portNameFontColor);
			g2d.setFont(portNameFont);
			g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
			g2d.drawString(p.getText(), offset.x + p.getTextOffset().x, offset.y + p.getTextOffset().y);
			g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
			g2d.setColor(new Color(196, 196, 196));
			g2d.drawLine(offset.x, offset.y + p.getHeight(), offset.x + width, offset.y + p.getHeight());
			/*
			g2d.setColor(new Color(255, 0, 0));
			g2d.drawLine(offset.x + p.getConnectionPoint().x - 1, offset.y + p.getConnectionPoint().y, offset.x + p.getConnectionPoint().x + 1, offset.y + p.getConnectionPoint().y);
			g2d.drawLine(offset.x + p.getConnectionPoint().x, offset.y + p.getConnectionPoint().y + 1, offset.x + p.getConnectionPoint().x, offset.y + p.getConnectionPoint().y - 1);
			*/
		}
	}
	
	
	public PortEntry positionWithinPort(Point offset, Point pos) {
		Iterator<PortEntry> iter = this.portEntryMap.values().iterator();
		while (iter.hasNext()) {
		//Enumeration<PortEntry> pe = this.portEntryMap.elements();
		//while (pe.hasMoreElements()) {
			//PortEntry p = pe.nextElement();
			PortEntry p = iter.next();
			if (pos.x >= offset.x + p.getCircleOffset().x && pos.y >= offset.y + p.getCircleOffset().y &&
					pos.x <= offset.x + p.getCircleOffset().x + portCircleSize &&
					pos.y <= offset.y + p.getCircleOffset().y + portCircleSize) {
				return p;
			}
		}
		
		return null;
	}
	
	
	public boolean isSourcePort(String portName) {
		return this.sources.contains(portName);
	}

	public String getTypeName() {
		return this.typeName;
	}

	public void setTypeName(String typeName) {
		this.typeName = typeName;
	}
}
