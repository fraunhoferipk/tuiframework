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


package test;

import java.util.ArrayList;
import java.util.Hashtable;


import view.ImageDB;
import view.ServerConfigView;

import configmodel.*;
import configmodel.Port.DataFlowDirection;
import configmodel.EntityID.Type;

/**
 * TestServerConfig
 * 
 * @author Oliver Belaifa
 */
public class TestServerConfig {
	public static void addSourcePort(Hashtable<String, Port> portMap, String name, String typeName) {
		portMap.put(name, new Port(name, typeName, DataFlowDirection.Source, ""));
	}
	
	public static void addSinkPort(Hashtable<String, Port> portMap, String name, String typeName) {
		portMap.put(name, new Port(name, typeName, DataFlowDirection.Sink, ""));
	}

	public static EntityType createDeviceTypeDummyDA() {
		Hashtable<String, Port> portMap = new Hashtable<String, Port>();
		addSinkPort(portMap, "AIN1", "Analog");
		addSinkPort(portMap, "AIN2", "Analog");
		addSinkPort(portMap, "AIN3", "Analog");
		addSinkPort(portMap, "AIN4", "Analog");
		addSourcePort(portMap, "AOUT1", "Analog");
		addSourcePort(portMap, "AOUT1", "Analog");
		addSourcePort(portMap, "AOUT2", "Analog");
		addSourcePort(portMap, "AOUT3", "Analog");
		addSourcePort(portMap, "AOUT4", "Analog");
		addSinkPort(portMap, "DIN1", "Digital");
		addSinkPort(portMap, "DIN2", "Digital");
		addSinkPort(portMap, "DIN3", "Digital");
		addSinkPort(portMap, "DIN4", "Digital");
		addSourcePort(portMap, "DOUT1", "Digital");
		addSourcePort(portMap, "DOUT2", "Digital");
		addSourcePort(portMap, "DOUT3", "Digital");
		addSourcePort(portMap, "DOUT4", "Digital");
		
		return new EntityType(new EntityID("DummyDeviceDAType", Type.DEV), portMap, "");
	}
	
	public static EntityType createDeviceTypeDummyTracker() {
		Hashtable<String, Port> portMap = new Hashtable<String, Port>();
		addSourcePort(portMap, "MOUT1", "Matrix4x4");
		addSourcePort(portMap, "MOUT2", "Matrix4x4");
		addSourcePort(portMap, "MOUT3", "Matrix4x4");
		addSourcePort(portMap, "MOUT4", "Matrix4x4");

		return new EntityType(new EntityID("DummyDeviceTrackerType", Type.DEV), portMap, "");
	}
	
	public static EntityType createMSPTypeDummyMatrix4x4Transf() {
		Hashtable<String, Port> portMap = new Hashtable<String, Port>();
		addSinkPort(portMap, "IN", "Matrix4x4");
		addSourcePort(portMap, "OUT", "Matrix4x4");
		
		return new EntityType(new EntityID("MatrixTransfType", Type.MSP), portMap, "");
	}
	
	public static EntityType createTUIObjectTypeAnalogPen() {
		Hashtable<String, Port> portMap = new Hashtable<String, Port>();
		addSinkPort(portMap, "Transf", "Matrix4x4");
		addSinkPort(portMap, "Pressure", "Analog");
		addSourcePort(portMap, "LED", "Digital");
		
		return new EntityType(new EntityID("AnalogPenType", Type.TUI), portMap, "");
	}
	
	public static EntityType createTUIObjectTypeBezierTool() {
		Hashtable<String, Port> portMap = new Hashtable<String, Port>();
		addSinkPort(portMap, "Transf1", "Matrix4x4");
		addSinkPort(portMap, "Button1", "Digital");
		addSinkPort(portMap, "Transf2", "Matrix4x4");
		addSinkPort(portMap, "Button2", "Digital");
		
		return new EntityType(new EntityID("BezierToolType", Type.TUI), portMap, "");
	}
	
	
	public static ServerConfig createServerConfig() {
		/*
		ServerConfig sc = new ServerConfig();
		{
			EntityType e = createDeviceTypeDummyDA();
			sc.getEntityTypeMap().put(e.getEntityID(), e);
		}
		{
			EntityType e = createDeviceTypeDummyTracker();
			sc.getEntityTypeMap().put(e.getEntityID(), e);
		}
		{
			EntityType e = createMSPTypeDummyMatrix4x4Transf();
			sc.getEntityTypeMap().put(e.getEntityID(), e);
		}
		{
			EntityType e = createTUIObjectTypeAnalogPen();
			sc.getEntityTypeMap().put(e.getEntityID(), e);
		}
		{
			EntityType e = createTUIObjectTypeBezierTool();
			sc.getEntityTypeMap().put(e.getEntityID(), e);
		}
		
		Editor editor = new Editor(sc);
		
		try {
			editor.createInstance("TrackerDevice", new EntityID("DummyDeviceTrackerType", Type.DEV), "");
			editor.createInstance("SignalDevice", new EntityID("DummyDeviceDAType", Type.DEV), "");
			editor.createInstance("Transformer1", new EntityID("MatrixTransfType", Type.MSP), "");
			editor.createInstance("Transformer2", new EntityID("MatrixTransfType", Type.MSP), "");
			editor.createInstance("BezierTool", new EntityID("BezierToolType", Type.TUI), "");
			editor.createInstance("AnalogPen", new EntityID("AnalogPenType", Type.TUI), "");
			
			editor.connectPorts(new EntityID("TrackerDevice", Type.DEV), "MOUT1", new EntityID("Transformer1", Type.MSP), "IN");
			editor.connectPorts(new EntityID("TrackerDevice", Type.DEV), "MOUT2", new EntityID("Transformer2", Type.MSP), "IN");
			
			editor.connectPorts(new EntityID("Transformer1", Type.MSP), "OUT", new EntityID("BezierTool", Type.TUI), "Transf1");
			editor.connectPorts(new EntityID("Transformer2", Type.MSP), "OUT", new EntityID("BezierTool", Type.TUI), "Transf2");
			editor.connectPorts(new EntityID("SignalDevice", Type.DEV), "DOUT1", new EntityID("BezierTool", Type.TUI), "Button1");
			editor.connectPorts(new EntityID("SignalDevice", Type.DEV), "DOUT2", new EntityID("BezierTool", Type.TUI), "Button2");
			
			editor.connectPorts(new EntityID("TrackerDevice", Type.DEV), "MOUT3", new EntityID("AnalogPen", Type.TUI), "Transf");
			editor.connectPorts(new EntityID("SignalDevice", Type.DEV), "AOUT1", new EntityID("AnalogPen", Type.TUI), "Pressure");
			
			editor.connectPorts(new EntityID("AnalogPen", Type.TUI), "LED", new EntityID("SignalDevice", Type.DEV), "AIN1");
			
		} catch (InternalException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		
		
		return sc;
		*/
		return new ServerConfig();
	}
	
	
	public static ServerConfigView createServerConfigView() {
		ServerConfigView scv = new ServerConfigView();
		ImageDB imageDB = scv.getImageDB();
		try {
			imageDB.loadImage("images/DefaultDEV.png");
			imageDB.loadImage("images/DefaultMSP.png");
			//imageDB.loadImage("DefaultTUI", "images/DefaultTUI.png");
			imageDB.loadImage("images/TUI1.png");

			imageDB.setDefaultTypeImage(EntityID.Type.DEV, "images/DefaultDEV.png");
			imageDB.setDefaultTypeImage(EntityID.Type.MSP, "images/DefaultMSP.png");
			imageDB.setDefaultTypeImage(EntityID.Type.TUI, "images/TUI1.png");
			
			//imageDB.removeImage("DefaultMSP");
			
		} catch (InternalException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return scv;
	}
	 
}
