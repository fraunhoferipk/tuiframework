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

import java.util.Hashtable;

/**
 * EntityInstance
 * 
 * @author Oliver Belaifa
 */
public class EntityInstance {
	protected EntityID entityID;
	protected String typeName;
	protected String description;
	protected Hashtable<String, ParameterGroup> parameterGroupMap = new Hashtable<String, ParameterGroup>();
	
	public EntityInstance(EntityID entityID, String typeName, String description) {
		this.entityID = entityID;
		this.typeName = typeName;
		this.description = description;
	}
	
	public EntityID getEntityID() {
		return entityID;
	}

	public void setEntityID(EntityID entityID) {
		this.entityID = entityID;
	}

	public String getTypeName() {
		return this.typeName;
	}
	
	public void setTypeName(String typeName) {
		this.typeName = typeName;
	}
	
	public EntityID getEntityTypeID() {
		return new EntityID(this.typeName, this.entityID.getType());
	}
	
	public void setDescription(String description) {
		this.description = description;
	}
	
	public String getDescription() {
		return this.description;
	}
	
	public Hashtable<String, ParameterGroup> getParameterGroupMap() {
		return this.parameterGroupMap;
	}
	
	public void setParameterGroupMap(Hashtable<String, ParameterGroup> parameterGroupMap) {
		this.parameterGroupMap = parameterGroupMap;
	}
}

