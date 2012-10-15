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
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Point;
import java.awt.RenderingHints;

import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.ListCellRenderer;


import configmodel.EntityType;

/**
 * EntityCellRenderer
 * 
 * @author Oliver Belaifa
 */
public class EntityCellRenderer extends JLabel implements ListCellRenderer {
	private static final long serialVersionUID = 1040593544648181193L;
	
	private static Font entryFont = new Font("Arial", Font.BOLD, 14);
	
	private static Color entityNameColor = new Color(255, 255, 255);
	private static Color entityTypeColor = new Color(255, 255, 255);
	private static Color shadowColor = new Color(64, 64, 64);
	private static Color devGradientColor1 = new Color(64, 128, 196);
	private static Color devGradientColor2 = new Color(128, 196, 225);
	private static Color mspGradientColor1 = new Color(64, 196, 128);
	private static Color mspGradientColor2 = new Color(128, 225, 196);
	private static Color tuiGradientColor1 = new Color(196, 128, 64);
	private static Color tuiGradientColor2 = new Color(225, 196, 128);

	private static Dimension iconSize = new Dimension(48, 48);
	private static int iconBorder = 1;
	private static int middleLeading = 4;
	private static Border border = new Border(1, 1, 1, 1);
	
	private EntityType entityType;
	
	public ImageDB imageDB;
	private boolean isSelected = false;
	
	public EntityCellRenderer(ImageDB imageDB) {
		this.imageDB = imageDB;
	}

	@Override
	public Component getListCellRendererComponent(JList list,
            Object value,
            int index,
            boolean isSelected,
            boolean cellHasFocus) {
		this.entityType = (EntityType)value;
		this.isSelected = isSelected;
		
		this.setPreferredSize(this.minSize());
		this.setBackground(Color.blue);

		return this;
	}
	
	
	public Dimension minSize() {
		
		Dimension d1 = new Dimension(iconSize.width + 2*iconBorder + border.getLeft() + border.getRight(),
				iconSize.height + 2*iconBorder + border.getTop() + border.getBottom());
		FontMetrics f = this.getFontMetrics(entryFont);
		Dimension d2 = new Dimension(f.stringWidth(this.entityType.getEntityID().getName()), f.getAscent());
		return new Dimension(d1.width + middleLeading + d2.width, Math.max(d1.height, d2.height)); 
	}
	

	@Override
	public void paint(Graphics g) {
		// TODO Auto-generated method stub
		
		super.paint(g);

		Graphics2D g2d = (Graphics2D)g;
		Dimension minSize = this.minSize();
		Point iconOffset = new Point(this.border.getLeft() + this.iconBorder, this.border.getTop() + this.iconBorder);
		
		FontMetrics f = g2d.getFontMetrics(entryFont);
		Point entityTypeNameOffset = new Point(this.border.getLeft() + iconSize.width + 2*iconBorder + this.border.getRight() + middleLeading,
				(minSize.height - f.getAscent())/2 + f.getAscent());
		
		
		Image image = this.imageDB.getImage(entityType.getEntityID());
		g2d.drawImage(image, iconOffset.x, iconOffset.y, iconSize.width, iconSize.height, null);

		
		g2d.setStroke(new BasicStroke(2));
		if (this.isSelected) {
			g2d.setColor(Color.red);
		} else {
			switch (this.entityType.getEntityID().getType()) {
			case DEV:
				g2d.setColor(devGradientColor1);
				break;
			case MSP:
				g2d.setColor(mspGradientColor1);
				break;
			case TUI:
				g2d.setColor(tuiGradientColor1);
				break;
			}
			
		}
		
		//g2d.setColor(this.isSelected ? Color.red : shadowColor);
		g2d.drawRect(iconOffset.x, iconOffset.y, 49, 49);
		g2d.setStroke(new BasicStroke(1));
		
		if (! this.isSelected) {
			g2d.setColor(shadowColor);
		}

		
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		g2d.setFont(entryFont);
		g2d.drawString(entityType.getEntityID().getName(), entityTypeNameOffset.x, entityTypeNameOffset.y);
		
		g2d.setColor(Color.black);
		
	}
	
}
