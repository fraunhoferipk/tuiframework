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

import java.util.Hashtable;

import configmodel.InternalException;
import configmodel.ServerConfig;

/**
 * ServerConfigTable
 * 
 * @author Oliver Belaifa
 */
public class ServerConfigTable {
	private Hashtable<String, ServerConfig> serverConfigTable = new Hashtable<String, ServerConfig>();
	private String selectedServerConfigName = null;
	
	public ServerConfigTable() {
	}
	
	public void createServerConfig(String name) throws InternalException {
		if (this.serverConfigTable.containsKey(name)) {
			throw new InternalException("A Configuration with name \"" + name +	"\" already exist. Please choose another name.");
		}
		this.serverConfigTable.put(name, new ServerConfig());
	}
	
	public void deleteServerConfig(String name) throws InternalException {
		if (this.serverConfigTable.containsKey(name)) {
			this.serverConfigTable.remove(name);
			if (this.selectedServerConfigName.equals(name)) {
				this.selectedServerConfigName = null;
			}
		} else {
			throw new InternalException("A Configuration with name \"" + name +	"\" does not exist. Please choose another name.");
		}
	}
	
	public void selectServerConfig(String name) throws InternalException {
		if (this.serverConfigTable.containsKey(name)) {
			this.selectedServerConfigName = name; 
		} else {
			throw new InternalException("A Configuration with name \"" + name +	"\" does not exist. Please choose another name.");
		}
	}
	
	public void deselectServerConfig() {
		this.selectedServerConfigName = null;
	}
	
	public ServerConfig getSelectedServerConfig() throws InternalException {
		if (this.selectedServerConfigName == null) {
			throw new InternalException("No Configuration selected.");
		}
		return this.serverConfigTable.get(this.selectedServerConfigName);
	}
	
	public String getSelectedServerConfigName() {
		return this.selectedServerConfigName;
	}
	
	/// Accessor
	public void setServerConfigTable(Hashtable<String, ServerConfig> serverConfigTable) {
		this.serverConfigTable = serverConfigTable;
	}
	
	
	public Hashtable<String, ServerConfig> getServerConfigTable() {
		return this.serverConfigTable;
	}
	
	
	public void setSelectedServerConfigName(String selectedServerConfigName) {
		this.selectedServerConfigName = selectedServerConfigName;
	}
	
}
