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


package project;

import java.util.Enumeration;
import java.util.Hashtable;

import configmodel.EntityID;
import configmodel.EntityType;
import configmodel.ServerConfig;

/**
 * EntityTypeDB
 * 
 * @author Oliver Belaifa
 */
public class EntityTypeDB {
	protected Hashtable<EntityID, EntityType> entityTypeMap = new Hashtable<EntityID, EntityType>();
	
	
	public EntityTypeDB() {
	}
	
	
	public Hashtable<EntityID, EntityType> getEntityTypeMap() {
		return this.entityTypeMap;
	}
	
	public Hashtable<EntityID, EntityType> getInstancedEntityTypeMap(ServerConfig serverConfig) {
		Hashtable<EntityID, EntityType> instancedEntityTypeMap = new Hashtable<EntityID, EntityType>();
		Enumeration<EntityType> en = this.entityTypeMap.elements();
		while (en.hasMoreElements()) {
			EntityType entityType = en.nextElement();
			if (serverConfig.hasInstancedEntityType(entityType.getEntityID())) {
				instancedEntityTypeMap.put(entityType.getEntityID(), entityType);
			}
		}
		return instancedEntityTypeMap;
	}
	
	public void clear() {
		this.entityTypeMap.clear();
	}
}
