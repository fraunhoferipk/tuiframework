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

import configmodel.*;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.GradientPaint;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Polygon;
import java.awt.RenderingHints;
import java.awt.geom.CubicCurve2D;
import java.awt.geom.QuadCurve2D;
import java.awt.geom.CubicCurve2D.Double;


/**
 * Entity
 * 
 * @author Oliver Belaifa
 */
public class Entity {

	private static Font entityNameFont = new Font("Arial", Font.BOLD, 14);
	private static Font entityTypeFont = new Font("Arial", Font.BOLD, 11);
	private static Color entityNameColor = new Color(255, 255, 255);
	private static Color entityTypeColor = new Color(255, 255, 255);
	private static Color shadowColor = new Color(64, 64, 64);
	private static Color devGradientColor1 = new Color(64, 128, 196);
	private static Color devGradientColor2 = new Color(128, 196, 225);
	private static Color mspGradientColor1 = new Color(64, 196, 128);
	private static Color mspGradientColor2 = new Color(128, 225, 196);
	private static Color tuiGradientColor1 = new Color(196, 128, 64);
	private static Color tuiGradientColor2 = new Color(225, 196, 128);
	
	private static int titleLeading = 2;
	private static Dimension iconSize = new Dimension(32, 32);
	private static int iconBorder = 2;
	
	private static Border border = new Border(5, 5, 7, 4);

	private Point entityNameTextOffs;
	private Point entityTypeTextOffs;
	
	private EntityTitle title;
	private EntityContent content;
	
	private EntityInstance inst;
	private EntityType type;
	
	private Dimension titleDim = new Dimension(0, 0);
	private Dimension contentDim = new Dimension(0, 0);
	private Dimension dimension = new Dimension(0, 0);
	
	private ServerConfigView serverConfigView;
	
	public Entity(EntityInstance inst, EntityType type, ServerConfigView serverConfigView) {
		this.inst = inst;
		this.type = type;
		this.serverConfigView = serverConfigView;
		this.title = new EntityTitle(this.inst, this.type, this.serverConfigView.getImageDB());
		this.content = new EntityContent(this.type);
	}
	
	public EntityContent getContent() {
		return content;
	}
	
	public EntityInstance getInst() {
		return this.inst;
	}

	public void setInst(EntityInstance inst) {
		this.inst = inst;
	}

	public EntityType getType() {
		return this.type;
	}

	public void setType(EntityType type) {
		this.type = type;
	}

	public Point getContentPos() {
		Point pos = this.serverConfigView.getEntityInstancePosition(this.inst.getEntityID());
		return new Point(pos.x, pos.y + titleDim.height);
	}
	
	protected Dimension minSize(Graphics2D g2d) {
		this.titleDim = this.title.minSize(g2d);
		this.contentDim = this.content.minSize(g2d);
		this.dimension = new Dimension(Math.max(titleDim.width, contentDim.width), titleDim.height + contentDim.height);
		return dimension;
	}
	
	public void calculateOffsets(Graphics2D g2d) {
		this.titleDim = this.title.minSize(g2d);
		this.contentDim = this.content.minSize(g2d);
		this.dimension = new Dimension(Math.max(titleDim.width, contentDim.width), titleDim.height + contentDim.height);
		this.title.calculateOffsets(g2d, this.dimension.width);
		this.content.calculateOffsets(g2d, this.dimension.width);
	}
	
	public void draw(Graphics2D g2d, boolean selected) {
		Point pos = this.serverConfigView.getEntityInstancePosition(this.inst.getEntityID());
		this.title.draw(g2d, pos, this.dimension.width, selected);
	    this.content.draw(g2d, new Point(pos.x, pos.y + titleDim.height), this.dimension.width, selected);
	}
	
	public boolean posWithinTitleBar(Point pos) {
		Point entityPos = this.serverConfigView.getEntityInstancePosition(this.inst.getEntityID());
		if (pos.x >= entityPos.x &&	pos.y >= entityPos.y &&
				pos.x <= entityPos.x + this.titleDim.width &&
				pos.y <= entityPos.y + this.titleDim.height) {
			return true;
		}
		
		return false;
	}
	
	public PortEntry positionWithinPort(Point pos) {
		Point entityPos = this.serverConfigView.getEntityInstancePosition(this.inst.getEntityID());
		
		if (pos.x >= entityPos.x && pos.y >= entityPos.y + this.titleDim.height &&
				pos.x <= entityPos.x + this.dimension.width &&
				pos.y <= entityPos.y + this.dimension.height) {
			return this.content.positionWithinPort(new Point(entityPos.x, entityPos.y + titleDim.height), pos);
		}
		return null;
		
		//return this.content.positionWithinPort(new Point(entityPos.x, entityPos.y + titleDim.height), pos);
	}
}
