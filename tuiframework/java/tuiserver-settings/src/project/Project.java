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

import view.ImageDB;
import view.ServerConfigView;
import configmodel.EntityID;
import configmodel.EntityType;
import configmodel.InternalException;
import configmodel.ServerConfig;

/**
 * Project
 * 
 * @author Oliver Belaifa
 */
public class Project {
	protected EntityTypeDB entityTypeDB = new EntityTypeDB();
	protected ServerConfig serverConfig = new ServerConfig();
	protected ServerConfigView serverConfigView = new ServerConfigView();
	
	public Project() {
	}

	
	public void setDefaultImages() {
		ImageDB imageDB = this.serverConfigView.getImageDB();
		try {
			imageDB.loadImage("images/DefaultDEV.png");
			imageDB.loadImage("images/DefaultMSP.png");
			//imageDB.loadImage("DefaultTUI", "images/DefaultTUI.png");
			imageDB.loadImage("images/TUI1.png");

			imageDB.setDefaultTypeImage(EntityID.Type.DEV, "images/DefaultDEV.png");
			imageDB.setDefaultTypeImage(EntityID.Type.MSP, "images/DefaultMSP.png");
			imageDB.setDefaultTypeImage(EntityID.Type.TUI, "images/TUI1.png");
			
		} catch (InternalException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}	
	}
	
	
	public EntityTypeDB getEntityTypeDB() {
		return this.entityTypeDB;
	}
	
	
	public ServerConfig getServerConfig() {
		return this.serverConfig;
	}
	
	
	public void setServerConfig(ServerConfig serverConfig) {
		this.serverConfig = serverConfig;
	}
	
	
	public ServerConfigView getServerConfigView() {
		return this.serverConfigView;
	}
}
