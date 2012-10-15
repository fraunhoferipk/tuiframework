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
import java.awt.GradientPaint;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Point;
import java.awt.RenderingHints;
import java.awt.geom.CubicCurve2D;
import java.awt.geom.CubicCurve2D.Double;


import configmodel.EntityID;
import configmodel.EntityInstance;
import configmodel.EntityType;

/**
 * EntityTitle
 * 
 * @author Oliver Belaifa
 */
public class EntityTitle {
	private static Font entityNameFont = new Font("Arial", Font.BOLD, 16);
	private static Font entityTypeFont = new Font("Arial", Font.BOLD, 12);
	private static Color entityNameColor = new Color(255, 255, 255);
	private static Color entityTypeColor = new Color(255, 255, 255);
	private static Color shadowColor = new Color(64, 64, 64);
	private static Color devGradientColor1 = new Color(64, 128, 196);
	private static Color devGradientColor2 = new Color(128, 196, 225);
	private static Color mspGradientColor1 = new Color(64, 196, 128);
	private static Color mspGradientColor2 = new Color(128, 225, 196);
	private static Color tuiGradientColor1 = new Color(196, 128, 64);
	private static Color tuiGradientColor2 = new Color(225, 196, 128);
	
	private static int titleBorder = 7;
	private static int titleLeading = 10;
	private static Dimension iconSize = new Dimension(48, 48);
	private static int iconBorder = 4;
	
	private static Border border = new Border(5, 5, 7, 4);
	
	private ImageDB imageDB;

	private Point entityNameTextOffs;
	private Point entityTypeTextOffs;
	
	private EntityInstance inst;
	private EntityType type;
	
	private Color gradientColor1;
	private Color gradientColor2;
	

	public EntityTitle(EntityInstance inst, EntityType type, ImageDB imageDB) {
		this.inst = inst;
		this.type = type;
		this.imageDB = imageDB;
		switch (this.inst.getEntityID().getType()) {
		case DEV:
			this.gradientColor1 = devGradientColor1;
			this.gradientColor2 = devGradientColor2;
			break;
		case MSP:
			this.gradientColor1 = mspGradientColor1;
			this.gradientColor2 = mspGradientColor2;			
			break;
		case TUI:
			this.gradientColor1 = tuiGradientColor1;
			this.gradientColor2 = tuiGradientColor2;					
			break;
		}
	}
	
	public Dimension getEntityNameDim(Graphics2D g2d) {
		FontMetrics f = g2d.getFontMetrics(entityNameFont);
		//int h1 = f.getAscent() + f.getDescent();
		return new Dimension(f.stringWidth(this.inst.getEntityID().getName()), f.getAscent());
	}
	
	public Dimension getEntityTypeDim(Graphics2D g2d) {
		FontMetrics f = g2d.getFontMetrics(entityTypeFont);
		//int h1 = f.getAscent() + f.getDescent();
		return new Dimension(f.stringWidth("(" + this.type.getEntityID().getName() +  ")"), f.getAscent());
	}
	
	public Dimension minSize(Graphics2D g2d) {
		Dimension d1 = getEntityNameDim(g2d);
		Dimension d2 = getEntityTypeDim(g2d);
				
		Dimension d = new Dimension(iconSize.width + 2*iconBorder + border.getLeft() + border.getRight() + Math.max(d1.width, d2.width),
				Math.max(iconSize.height + 2*iconBorder, d1.height + d2.height + titleLeading) + border.getTop() + border.getBottom());
		
		return d; 
	}
	
	public void calculateOffsets(Graphics2D g2d, int width) {
		Dimension d1 = getEntityNameDim(g2d);
		Dimension d2 = getEntityTypeDim(g2d);
		
		//int offset1 = (width - (border.getLeft() + 2*iconBorder + iconSize.width + border.getRight()) - d1.width)/2;
		//int offset2 = (width - (border.getLeft() + 2*iconBorder + iconSize.width + border.getRight())- d2.width)/2;;
		
		int iconOffset = border.getLeft() + 2*iconBorder + iconSize.width;
		
		//int offset1 = (width - iconOffset - d1.width)/2 + iconOffset;
		//int offset2 = (width - iconOffset - d2.width)/2 + iconOffset;
		
		int offset1 = iconOffset;
		int offset2 = iconOffset;

		this.entityNameTextOffs = new Point(offset1, border.getTop() + d1.height + titleBorder);
		this.entityTypeTextOffs = new Point(offset2, border.getTop() + d1.height + titleLeading + d2.height + titleBorder);
	}

	
	public void draw(Graphics2D g2d, Point offset, int width, boolean selected) {
		Dimension d = minSize(g2d);

        GradientPaint gradient = new GradientPaint(0, offset.y, this.gradientColor1, 0, offset.y + d.height/2, this.gradientColor2, true);
        g2d.setPaint(gradient);
//		g2d.drawString(inst.getEntityID().getName(), 50, 50);
		
        g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);

		//g2d.setColor(new Color(100, 100, 255));
		g2d.fillArc(offset.x, offset.y, 20, 20, 90, 90);
		g2d.fillRect(offset.x + 10, offset.y, width - 20, 10);
		g2d.fillArc(offset.x + width - 20 , offset.y, 20, 20, 0, 90);
		g2d.fillRect(offset.x, offset.y + 10, width, d.height - 10);
		
		Image image = this.imageDB.getImage(type.getEntityID());
		if (image != null) {
		g2d.drawImage(image,
				offset.x + border.getLeft() + iconBorder,
				offset.y + border.getTop() + iconBorder,
				iconSize.width, iconSize.height,
				null);
		}
		
		//g2d.fillRect(offset.x + border.getLeft() + iconBorder, offset.y + border.getTop() + iconBorder, iconSize.width, iconSize.height);
		g2d.setColor(shadowColor);
		g2d.drawRect(offset.x + border.getLeft() + iconBorder - 1,
				offset.y + border.getTop() + iconBorder - 1,
				iconSize.width,
				iconSize.height);
		
		if (selected) {
			g2d.setStroke(new BasicStroke(2));
			g2d.setColor(Color.red);
		} else {
			g2d.setColor(shadowColor);
		}
		
		g2d.drawArc(offset.x, offset.y, 20, 20, 90, 90);
		g2d.drawLine(offset.x + 10, offset.y, offset.x + width - 10, offset.y);
		g2d.drawArc(offset.x + width - 20 , offset.y, 20, 20, 0, 90);
		//g2d.drawRect(offset.x, offset.y + 10, width, d.height - 10);
		g2d.drawLine(offset.x, offset.y + 10, offset.x, offset.y + d.height - 1);
		g2d.drawLine(offset.x +  width, offset.y + 10, offset.x + width, offset.y + d.height - 1);
		g2d.setStroke(new BasicStroke(1));

		/*
// temp {
		g2d.setColor(new Color(255, 255, 255));
		g2d.fillRect(offset.x, offset.y + d.height, width, 200);
		g2d.setColor(shadowColor);
		g2d.drawRect(offset.x, offset.y + d.height, width, 200);
		g2d.setColor(new Color(128, 128, 128));
		g2d.fillOval(offset.x + 4, offset.y + d.height + 20, 12, 12);
		g2d.setColor(shadowColor);
		g2d.drawOval(offset.x + 4, offset.y + d.height + 20, 12, 12);
		
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		g2d.setColor(new Color(0, 0, 0));
		g2d.setFont(entityNameFont);
		g2d.drawString("AOUT1", offset.x + 22, offset.y + d.height + 32);
	
		// } temp    
		
        g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

    	Double curve = new CubicCurve2D.Double(-220, 100, 220, 100, 200, 300, 400, 300);
		g2d.draw(curve);
		
	
        g2d.fillPolygon(new int[] { 400, 390, 390}, new int[] { 300, 306, 294 } , 3);
        

		
*/
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		g2d.setFont(entityNameFont);
		g2d.setColor(shadowColor);
		g2d.drawString(inst.getEntityID().getName(), offset.x + 1 + this.entityNameTextOffs.x, offset.y + this.entityNameTextOffs.y + 1);
		g2d.setColor(entityNameColor);
		g2d.drawString(inst.getEntityID().getName(), offset.x + this.entityNameTextOffs.x, offset.y + this.entityNameTextOffs.y);
		
		g2d.setFont(entityTypeFont);
		g2d.setColor(shadowColor);
		g2d.drawString("(" + type.getEntityID().getName() + ")", offset.x + 1 + this.entityTypeTextOffs.x, offset.y + 1 + this.entityTypeTextOffs.y);
		g2d.setColor(entityTypeColor);
		g2d.drawString("(" + type.getEntityID().getName() + ")", offset.x + this.entityTypeTextOffs.x, offset.y + this.entityTypeTextOffs.y);
		
		/*
        g2d.setColor(new Color(100, 100, 255));
        g2d.fillArc(100, 100, 20, 20, 90, 90);
        g2d.fillRect(110, 100, 100, 10);
        g2d.fillArc(200, 100, 20, 20, 0, 90);
        g2d.fillRect(100, 110, 119, 10);
        g2d.setColor(new Color(200, 200, 255));
        g2d.fillRect(100, 120, 119, 110);
        */
	}
}
