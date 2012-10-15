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
 * EntityType
 * 
 * @author Oliver Belaifa
 */
public class EntityType {
	protected EntityID entityID;
	protected String description;
	protected Hashtable<String, Port> portMap;
	Hashtable<String, ParameterTypeDef> parameterTypeDefMap = new Hashtable<String, ParameterTypeDef>();
 	Hashtable<String, String> parameterTypeMap = new Hashtable<String, String>();
 	Hashtable<String, ParameterGroup> defaultParameterGroupMap = new Hashtable<String, ParameterGroup>();
	
	public EntityType(EntityID entityID, Hashtable<String, Port> portMap, String description) {
		this.entityID = entityID;
		this.portMap = portMap;
		this.description = description;		
	}
	
	public Hashtable<String, ParameterTypeDef> getParameterTypeDefMap() {
		return this.parameterTypeDefMap;
	}

	public void setParameterTypeDefMap(
			Hashtable<String, ParameterTypeDef> parameterTypeDefMap) {
		this.parameterTypeDefMap = parameterTypeDefMap;
	}

	public Hashtable<String, String> getParameterTypeMap() {
		return this.parameterTypeMap;
	}

	public void setParameterTypeMap(Hashtable<String, String> parameterTypeMap) {
		this.parameterTypeMap = parameterTypeMap;
	}

	public Hashtable<String, ParameterGroup> getDefaultParameterGroupMap() {
		return this.defaultParameterGroupMap;
	}

	public void setDefaultParameterGroupMap(
			Hashtable<String, ParameterGroup> defaultParameterGroupMap) {
		this.defaultParameterGroupMap = defaultParameterGroupMap;
	}

	public void setPortMap(Hashtable<String, Port> portMap) {
		this.portMap = portMap;
	}

	public EntityID getEntityID() {
		return this.entityID;
	}

	public void setEntityID(EntityID entityID) {
		this.entityID = entityID;
	}
	
	public void setDescription(String description) {
		this.description = description;
	}
	
	public String getDescription() {
		return this.description;
	}
	
	public Hashtable<String, Port> getPortMap() {
		return this.portMap;
	}

	public void setPortVector(Hashtable<String, Port> portMap) {
		this.portMap = portMap;
	}
	
	
	

}
