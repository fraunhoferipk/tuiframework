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
import java.util.ArrayList;
import java.util.Hashtable;

import configmodel.*;

/**
 * ServerConfigView
 * 
 * @author Oliver Belaifa
 */
public class ServerConfigView {
	private ImageDB imageDB = new ImageDB();
	private Hashtable<EntityID, Point> entityInstancePositions = new Hashtable<EntityID, Point>();
	private ArrayList<EntityID> entityIDVector = new ArrayList<EntityID>();
	
	protected Hashtable<EntityID, Entity> entityViewMap = new Hashtable<EntityID, Entity>();
	
	public ServerConfigView() {
	}
	
	public ArrayList<EntityID> getEntityIDVector() {
		return entityIDVector;
	}

	public void setEntityIDVector(ArrayList<EntityID> entityIDVector) {
		this.entityIDVector = entityIDVector;
	}

	public ImageDB getImageDB() {
		return this.imageDB;
	}

	public void setImageDB(ImageDB imageDB) {
		this.imageDB = imageDB;
	}

	public void setEntityInstancePositions(Hashtable<EntityID, Point> entityInstancePositions) {
		this.entityInstancePositions = entityInstancePositions;
	}
	
	public Point getEntityInstancePosition(EntityID entityID) {
		Point p = this.entityInstancePositions.get(entityID);
		if (p == null) {
			return new Point(0, 0);
		}
		return p;
	}
	
	public void setEntityInstancePosition(EntityID entityID, Point pos) {
		this.entityInstancePositions.put(entityID, pos);
	}
	
	public void removeEntityInstancePosition(EntityID entityID) {
		this.entityInstancePositions.remove(entityID);
	}

	public Hashtable<EntityID, Point> getEntityInstancePositions() {
		return this.entityInstancePositions;
	}
	
	public void changeEntityID(EntityID oldEntityID, EntityID newEntityID) {
		Point point = this.entityInstancePositions.remove(oldEntityID);
		this.entityInstancePositions.put(newEntityID, point);
		this.entityIDVector.remove(oldEntityID);
		this.entityIDVector.add(newEntityID);
	}
	
}
