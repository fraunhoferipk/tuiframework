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

import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Iterator;

import project.Project;

/**
 * Editor
 * 
 * @author Oliver Belaifa
 */
public class Editor {
	protected Project project;
	
	public Editor() {
	}
	
	public Editor(Project project) {
		this.project = project;
	}
	
	public void setProject(Project project) {
		this.project = project;
	}
	
	public Project getProject() {
		return this.project;
	}
	
	public void assertEntityInstanceExist(EntityID entityID) throws InternalException {
		if ( ! this.project.getServerConfig().getEntityInstanceMap().containsKey(entityID)) {
			throw new InternalException(entityID.getTypeString() + "instance with name \"" + entityID.getName() + "\" does not exist.");
		}
	}
	
	public void assertEntityInstanceNotExist(EntityID entityID) throws InternalException {
		if (this.project.getServerConfig().getEntityInstanceMap().containsKey(entityID)) {
			throw new InternalException(entityID.getTypeString() + "instance with name \"" + entityID.getName() + "\" already exist.");
		}
	}
	
	public void assertEntityTypeExist(EntityID entityTypeID) throws InternalException {
		if ( ! this.project.getEntityTypeDB().getEntityTypeMap().containsKey(entityTypeID)) {
			throw new InternalException(entityTypeID.getTypeString() + "type \"" + entityTypeID.getName() + "\" does not exist.");
		}
	}
	
	public void assertEntityTypeNotExist(EntityID entityTypeID) throws InternalException {
		if (this.project.getEntityTypeDB().getEntityTypeMap().containsKey(entityTypeID)) {
			throw new InternalException(entityTypeID.getTypeString() + "type \"" + entityTypeID.getName() + "\" already exist.");
		}
	}
	
	public void assertPortExist(EntityType entityType, String portName) throws InternalException {
		if (! entityType.getPortMap().containsKey(portName)) {
			throw new InternalException("Port \"" + portName + "\" not found in entity of type \"" + entityType.getEntityID().getTypeString() + "\"");
		}
	}

	public EntityInstance createInstance(String name, EntityID entityTypeID, String description) throws InternalException {
		this.assertEntityTypeExist(entityTypeID);
		EntityType entityType = (EntityType)this.project.getEntityTypeDB().getEntityTypeMap().get(entityTypeID);
		EntityID entityID = new EntityID(name, entityTypeID.getType());
		this.assertEntityInstanceNotExist(entityID);
		EntityInstance inst = new EntityInstance(entityID, entityTypeID.getName(), description);
		
		Hashtable<String, ParameterGroup> clonedParameterGroupMap = ParameterGroup.cloneParameterGroupMap(entityType.getDefaultParameterGroupMap());
		inst.setParameterGroupMap(clonedParameterGroupMap);
		this.project.getServerConfig().getEntityInstanceMap().put(entityID, inst);
		return inst;
	}

	public Connector connectPorts(EntityID srcEntityID, String srcPortName, EntityID dstEntityID, String dstPortName) throws InternalException {
		this.assertEntityInstanceExist(srcEntityID);
		this.assertEntityInstanceExist(dstEntityID);
		
		EntityInstance srcEntity = this.project.getServerConfig().getEntityInstanceMap().get(srcEntityID);
		EntityInstance dstEntity = this.project.getServerConfig().getEntityInstanceMap().get(dstEntityID);
		
		EntityID srcEntityTypeID = new EntityID(srcEntity.getTypeName(), srcEntity.getEntityID().getType());
		EntityID dstEntityTypeID = new EntityID(dstEntity.getTypeName(), dstEntity.getEntityID().getType());
		
		this.assertEntityTypeExist(srcEntityTypeID);
		this.assertEntityTypeExist(dstEntityTypeID);
		
		EntityType srcEntityType = this.project.getEntityTypeDB().getEntityTypeMap().get(srcEntityTypeID);
		EntityType dstEntityType = this.project.getEntityTypeDB().getEntityTypeMap().get(dstEntityTypeID);
		
		this.assertPortExist(srcEntityType, srcPortName);
		this.assertPortExist(dstEntityType, dstPortName);
		
		Connector connector = new Connector(new PortAddress(srcEntity.getEntityID(), srcPortName), new PortAddress(dstEntity.getEntityID(), dstPortName));
		this.project.getServerConfig().connectorVector.add(connector);

		return connector;
	}
	
	
	public void removeEntity(EntityID entityID) {
		
		ArrayList<Connector> newConnectorVector = new ArrayList<Connector>();
		
		ArrayList<Connector> connectorVector = this.project.getServerConfig().getConnectorVector();
		Iterator<Connector> iter = connectorVector.iterator();
		while (iter.hasNext()) {
			Connector connector = iter.next();
			if (! connector.srcAddress.getEntityID().equals(entityID) &&
					! connector.dstAddress.getEntityID().equals(entityID)) {
				newConnectorVector.add(connector);
			}
		}
		this.project.getServerConfig().setConnectorVector(newConnectorVector);
		this.project.getServerConfig().getEntityInstanceMap().remove(entityID);
	}
	
}
