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


package configmodel;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.ArrayList;

/**
 * ServerConfig
 * 
 * @author Oliver Belaifa
 */
public class ServerConfig {
	protected Hashtable<EntityID, EntityInstance> entityInstanceMap = new Hashtable<EntityID, EntityInstance>();
	protected ArrayList<Connector> connectorVector = new ArrayList<Connector>();
	
	public ServerConfig() {
	}
	
	public Hashtable<EntityID, EntityInstance> getEntityInstanceMap() {
		return this.entityInstanceMap;
	}
	
	public void setEntityInstanceMap(Hashtable<EntityID, EntityInstance> entityInstanceMap) {
		this.entityInstanceMap = entityInstanceMap;
	}
	
	public boolean hasInstancedEntityType(EntityID entityTypeID) {
		boolean found = false;
		Enumeration<EntityInstance> en = entityInstanceMap.elements();
		while (en.hasMoreElements()) {
			EntityInstance inst = en.nextElement();
			if (inst.getEntityTypeID().equals(entityTypeID)) {
				found = true;
				break;
			}
		}
		return found;
	}
	
	public ArrayList<Connector> getConnectorVector() {
		return this.connectorVector;
	}
	
	public void setConnectorVector(ArrayList<Connector> connectorVector) {
		this.connectorVector = connectorVector;
	}
	

	
	public void clear() {
		this.entityInstanceMap.clear();
		this.connectorVector.clear();
	}
}
