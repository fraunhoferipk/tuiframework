/*
    Copyright (C) 2010, 2011, 2012 The Fraunhofer Institute for Production Systems and
    Design Technology IPK. All rights reserved.

    This file is part of the TUIFramework library.
    It includes a software framework which contains common code
    providing generic functionality for developing applications
    with a tangible user interface (TUI).
    designed for creating applications with a tangible user interface (TUI).
    
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

/**
 * PortAddress
 * 
 * @author Oliver Belaifa
 */
public class PortAddress {
	protected EntityID entityID;
	protected String portName;

	public PortAddress(EntityID entityID, String portName) {
		this.entityID = entityID;
		this.portName = portName;
	}
	
	public EntityID getEntityID() {
		return this.entityID;
	}

	public void setEntityID(EntityID entityID) {
		this.entityID = entityID;
	}

	public void setPortName(String portName) {
		this.portName = portName;
	}
	
	public String getPortName() {
		return this.portName;
	}
	
	@Override
	public boolean equals(Object obj) {
		return this.entityID.equals(((PortAddress)obj).getEntityID()) && this.portName.equals(((PortAddress)obj).getPortName());
	}
}

