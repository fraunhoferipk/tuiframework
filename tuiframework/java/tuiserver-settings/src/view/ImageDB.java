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

import java.awt.Image;
import java.io.File;
import java.io.IOException;
import java.util.Enumeration;
import java.util.Hashtable;

import javax.imageio.ImageIO;

import configmodel.EntityID;
import configmodel.InternalException;

/**
 * ImageDB
 * 
 * @author Oliver Belaifa
 */
public class ImageDB {
	private Hashtable<String, Image> imageTable = new Hashtable<String, Image>();
	private Hashtable<EntityID.Type, String> defaultTypeImageMap = new Hashtable<EntityID.Type, String>();
	private Hashtable<EntityID, String> typeNameImageMap = new Hashtable<EntityID, String>();
	
	public ImageDB() {
	}
	
	
	public Hashtable<String, Image> getImageTable() {
		return this.imageTable;
	}

	
	public void setImageDB(Hashtable<String, Image> imageTable) {
		this.imageTable = imageTable;
	}

	
	public Hashtable<EntityID.Type, String> getDefaultTypeImageMap() {
		return this.defaultTypeImageMap;
	}

	
	public void setDefaultTypeImageMap(
			Hashtable<EntityID.Type, String> defaultTypeImageMap) {
		this.defaultTypeImageMap = defaultTypeImageMap;
	}

	
	public Hashtable<EntityID, String> getTypeNameImageMap() {
		return this.typeNameImageMap;
	}
	

	public void setTypeNameImageMap(Hashtable<EntityID, String> typeNameImageMap) {
		this.typeNameImageMap = typeNameImageMap;
	}

	
	public boolean hasImage(String path) {
		return this.imageTable.containsKey(path);
	}
	
	public boolean loadImage(String path) throws InternalException {
		if (this.imageTable.containsKey(path)) {
			throw new InternalException("Image name \"" + path + "\" already exist in the image database.");
		}
		try {
			this.imageTable.put(path, ImageIO.read(new File(path)));
        } catch (IOException e) {
        	e.printStackTrace();
        	return false;
        }
        return true;
	}
	
	
	public void setDefaultTypeImage(EntityID.Type type, String path) throws InternalException {
		if ( ! this.imageTable.containsKey(path)) {
			throw new InternalException("Image name \"" + path + "\" not found in the image database.");
		}
		defaultTypeImageMap.put(type, path);
	}
	
	
	public void setEntityTypeImage(EntityID entityTypeID, String path) throws InternalException {
		if ( ! this.imageTable.containsKey(path)) {
			throw new InternalException("Image name \"" + path + "\" not found in the image database.");
		}
		this.typeNameImageMap.put(entityTypeID, path);
	}
	
	
	public void removeEntityTypeImage(EntityID entityTypeID) {
		this.typeNameImageMap.remove(entityTypeID);
	}
	
	
	public Image getImage(EntityID entityTypeID) {
		if (this.typeNameImageMap.containsKey(entityTypeID)) {
			String path = this.typeNameImageMap.get(entityTypeID);
			return this.imageTable.get(path);
		}
		
		String path = this.defaultTypeImageMap.get(entityTypeID.getType());
		if (path != null) {
			return this.imageTable.get(path);
		}
		return null;
	}
	
	
	public String getEntityTypeImagePath(EntityID entityTypeID) {
		return this.typeNameImageMap.get(entityTypeID);
	}
	
	public String getDefaultEntityTypeImagePath(EntityID.Type entityType) {
		return this.defaultTypeImageMap.get(entityType);
	}
	
	
	public void removeImage(String path) throws InternalException {
		if ( ! this.imageTable.containsKey(path)) {
			throw new InternalException("Image name \"" + path + "\" not found in the image database.");
		}
		{ 
			Enumeration<EntityID> e = this.typeNameImageMap.keys();
			while (e.hasMoreElements()) {
				EntityID key = e.nextElement();
				String value = this.typeNameImageMap.get(key);
				if (path.equals(value)) {
					this.typeNameImageMap.remove(key);
				}
			}
		}
		{ 
			Enumeration<EntityID.Type> e = this.defaultTypeImageMap.keys();
			while (e.hasMoreElements()) {
				EntityID.Type key = e.nextElement();
				String value = this.defaultTypeImageMap.get(key);
				if (path.equals(value)) {
					this.defaultTypeImageMap.remove(key);
				}
			}
		}
		
		this.imageTable.remove(path);
	}

	
}
