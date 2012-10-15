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

import java.awt.Point;

/**
 * PortEntry
 * 
 * @author Oliver Belaifa
 */
public class PortEntry {
	private Point textOffset;
	private Point circleOffset;
	private String text;
	private int height;
	private boolean isConnected = false; 
	private Point connectionPoint;
	private boolean source;
	private PortSet owner;
	
	public PortEntry(Point textOffset, Point circleOffset, String text, int height, Point connectionPoint, boolean source, PortSet owner) {
		this.textOffset = textOffset;
		this.circleOffset = circleOffset;
		this.text = text;
		this.height = height;
		this.connectionPoint = connectionPoint;
		this.source = source;
		this.owner = owner;
	}
	
	
	public boolean isConnected() {
		return isConnected;
	}

	public void setConnected(boolean isConnected) {
		this.isConnected = isConnected;
	}

	public Point getTextOffset() {
		return this.textOffset;
	}

	public void setTextOffset(Point textOffset) {
		this.textOffset = textOffset;
	}

	public Point getCircleOffset() {
		return this.circleOffset;
	}

	public void setCircleOffset(Point circleOffset) {
		this.circleOffset = circleOffset;
	}

	public Point getConnectionPoint() {
		return this.connectionPoint;
	}

	public void setConnectionPoint(Point connectionPoint) {
		this.connectionPoint = connectionPoint;
	}

	public String getText() {
		return this.text;
	}

	public void setText(String text) {
		this.text = text;
	}
	
	public int getHeight() {
		return this.height;
	}
	
	public void setHeight(int height) {
		this.height = height;
	}


	public boolean isSource() {
		return this.source;
	}


	public void setSource(boolean source) {
		this.source = source;
	}


	public PortSet getOwner() {
		return this.owner;
	}


	public void setOwner(PortSet owner) {
		this.owner = owner;
	}
}
