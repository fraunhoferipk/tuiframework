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


package xml;

import java.io.File;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Map.Entry;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Result;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Comment;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import project.Project;
import view.ImageDB;
import view.ServerConfigView;

import configmodel.Connector;
import configmodel.EntityID;
import configmodel.EntityInstance;
import configmodel.EntityType;
import configmodel.ParameterGroup;
import configmodel.ParameterTypeDef;
import configmodel.Port;
import configmodel.ServerConfig;

/**
 * Exporter
 * 
 * @author Oliver Belaifa
 */
public class Exporter {
	
	private Document createDocument() {
		DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		DocumentBuilder db;
		try {
			db = dbf.newDocumentBuilder();
		} catch (ParserConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}
		Document doc = db.newDocument();
		return doc;
	}
	
	private Element appendRootElement(Document doc) {
		
		//Comment descrip = doc.createComment("document xmlns:xi='http://www.w3.org/2001/XInclude' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xsi:noNamespaceSchemaLocation='ServerConfig.xsd'");
		Comment descrip = doc.createComment("document xmlns:xi='http://www.w3.org/2001/XInclude'");
		doc.appendChild(descrip);
		Element rootElement = doc.createElement("document");
		rootElement.setAttribute("xmlns:xi", "http://www.w3.org/2001/XInclude");
		doc.appendChild(rootElement);
		return rootElement;
	}
	
	private void appendStartup(Document doc, Element rootElement) {
		Element startupElement = doc.createElement("ServerStartupConfig");
		rootElement.appendChild(startupElement);
	}
	
	private void appendDescription(Document doc, Element rootElement, String description) {
		Element descriptionElement = doc.createElement("Description");
		rootElement.appendChild(descriptionElement);
		descriptionElement.setNodeValue(description);
	}
	
	private void appendPortTypeSequence(Document doc, Element rootElement, Hashtable<String, Port> portMap, boolean isTUI) {
		Element portSequenceElement = doc.createElement("PortTypeSequence");
		rootElement.appendChild(portSequenceElement);
		Enumeration<Port> e = portMap.elements();
		while (e.hasMoreElements()) {
			Port p = e.nextElement();
			Element portElement = doc.createElement("PortType");
			portSequenceElement.appendChild(portElement);
			portElement.setAttribute("name", p.getName());
			portElement.setAttribute("typename", p.getTypeName());
			if (isTUI) {
				// TODO tui bug fix
				portElement.setAttribute("source", p.getDataFlowDirection().equals(Port.DataFlowDirection.Source) ? "0" : "1");
				portElement.setAttribute("sink", p.getDataFlowDirection().equals(Port.DataFlowDirection.Sink) ? "0" : "1");
			}
			else {
				portElement.setAttribute("source", p.getDataFlowDirection().equals(Port.DataFlowDirection.Source) ? "1" : "0");
				portElement.setAttribute("sink", p.getDataFlowDirection().equals(Port.DataFlowDirection.Sink) ? "1" : "0");
			}
			this.appendDescription(doc, portElement, p.getDescription());
		}
	}

	
	private void appendParameterType(Document doc, Element rootElement, Entry<String, String> parameterType) {
		Element parameterTypeElement = doc.createElement("ParameterType");
		rootElement.appendChild(parameterTypeElement);
		parameterTypeElement.setAttribute("name", parameterType.getKey());
		parameterTypeElement.setAttribute("type", parameterType.getValue());
	}
	
	
	private void appendParameterTypeDef(Document doc, Element rootElement, ParameterTypeDef parameterTypeDef) {
		Element parameterTypeDefElement = doc.createElement("ParameterTypeDef");
		rootElement.appendChild(parameterTypeDefElement);
		parameterTypeDefElement.setAttribute("name", parameterTypeDef.getName());
		
		Iterator<Entry<String, String> > iter = parameterTypeDef.getParameterTypeMap().entrySet().iterator();
		while (iter.hasNext()) {
			appendParameterType(doc, parameterTypeDefElement, iter.next());
		}
	}
	
	private void appendParameterTypeDefSequence(Document doc, Element rootElement, Hashtable<String, ParameterTypeDef> parameterTypeDefMap) {
		Element parameterTypeDefSequenceElement = doc.createElement("ParameterTypeDefSequence");
		rootElement.appendChild(parameterTypeDefSequenceElement);
		Enumeration<ParameterTypeDef> e = parameterTypeDefMap.elements();
		while (e.hasMoreElements()) {
			ParameterTypeDef parameterTypeDef = e.nextElement();
			appendParameterTypeDef(doc, parameterTypeDefSequenceElement, parameterTypeDef);
		}
	}
	
	private void appendParameterTypeSequence(Document doc, Element rootElement, Hashtable<String, String> parameterTypeMap) {
		Element parameterTypeSequenceElement = doc.createElement("ParameterTypeSequence");
		rootElement.appendChild(parameterTypeSequenceElement);
		Iterator<Entry<String, String> > iter = parameterTypeMap.entrySet().iterator();
		while (iter.hasNext()) {
			appendParameterType(doc, parameterTypeSequenceElement, iter.next());
		}
	}
	
	
	private void appendDefaultImageEntityType(Document doc, Element rootElement, ImageDB imageDB, EntityID.Type entityType) {
		String path = imageDB.getDefaultEntityTypeImagePath(entityType);
		if (path != null) {
			Element imageElement = doc.createElement("Image");
			imageElement.setAttribute("path", path);
			rootElement.appendChild(imageElement);
		}
	}
	
	private void appendImageEntityType(Document doc, Element rootElement, ImageDB imageDB, EntityID entityType) {
		String path = imageDB.getEntityTypeImagePath(entityType);
		if (path != null) {
			Element imageElement = doc.createElement("Image");
			imageElement.setAttribute("path", path);
			rootElement.appendChild(imageElement);
		}
	}
	
	private void appendDeviceTypeSequence(Document doc, Element rootElement, Hashtable<EntityID, EntityType> entityTypeMap, ServerConfigView serverConfigView) {
		Element typeSequenceElement = doc.createElement("DeviceTypeSequence");
		rootElement.appendChild(typeSequenceElement);
		
		this.appendDefaultImageEntityType(doc, typeSequenceElement, serverConfigView.getImageDB(), EntityID.Type.DEV);
		
		Enumeration<EntityType> e = entityTypeMap.elements();
		while (e.hasMoreElements()) {
			EntityType entityType = e.nextElement();
			if (entityType.getEntityID().getType().equals(EntityID.Type.DEV)) {
				Element typeElement = doc.createElement("DeviceType");
				typeElement.setAttribute("name", entityType.getEntityID().getName());
				typeSequenceElement.appendChild(typeElement);
				this.appendImageEntityType(doc, typeElement, serverConfigView.getImageDB(), entityType.getEntityID());
				this.appendPortTypeSequence(doc, typeElement, entityType.getPortMap(), false);
				this.appendParameterTypeDefSequence(doc, typeElement, entityType.getParameterTypeDefMap());
				this.appendParameterTypeSequence(doc, typeElement, entityType.getParameterTypeMap());
				this.appendParameterGroupMap(doc, typeElement, entityType.getDefaultParameterGroupMap());
			}
		}
	}
	
	private void appendMSPTypeSequence(Document doc, Element rootElement, Hashtable<EntityID, EntityType> entityTypeMap, ServerConfigView serverConfigView) {
		Element typeSequenceElement = doc.createElement("MSPTypeSequence");
		rootElement.appendChild(typeSequenceElement);
		
		this.appendDefaultImageEntityType(doc, typeSequenceElement, serverConfigView.getImageDB(), EntityID.Type.MSP);
		
		Enumeration<EntityType> e = entityTypeMap.elements();
		while (e.hasMoreElements()) {
			EntityType entityType = e.nextElement();
			if (entityType.getEntityID().getType().equals(EntityID.Type.MSP)) {
				Element typeElement = doc.createElement("MSPType");
				typeElement.setAttribute("name", entityType.getEntityID().getName());
				typeSequenceElement.appendChild(typeElement);
				this.appendImageEntityType(doc, typeElement, serverConfigView.getImageDB(), entityType.getEntityID());
				this.appendPortTypeSequence(doc, typeElement, entityType.getPortMap(), false);
				this.appendParameterTypeDefSequence(doc, typeElement, entityType.getParameterTypeDefMap());
				this.appendParameterTypeSequence(doc, typeElement, entityType.getParameterTypeMap());
				this.appendParameterGroupMap(doc, typeElement, entityType.getDefaultParameterGroupMap());
			}
		}
	}
	
	private void appendTUIObjectTypeSequence(Document doc, Element rootElement, Hashtable<EntityID, EntityType> entityTypeMap, ServerConfigView serverConfigView) {
		Element typeSequenceElement = doc.createElement("TUIObjectTypeSequence");
		rootElement.appendChild(typeSequenceElement);
		
		this.appendDefaultImageEntityType(doc, typeSequenceElement, serverConfigView.getImageDB(), EntityID.Type.TUI);
		
		Enumeration<EntityType> e = entityTypeMap.elements();
		while (e.hasMoreElements()) {
			EntityType entityType = e.nextElement();
			if (entityType.getEntityID().getType().equals(EntityID.Type.TUI)) {
				Element typeElement = doc.createElement("TUIObjectType");
				typeElement.setAttribute("name", entityType.getEntityID().getName());
				typeSequenceElement.appendChild(typeElement);
				this.appendImageEntityType(doc, typeElement, serverConfigView.getImageDB(), entityType.getEntityID());
				this.appendPortTypeSequence(doc, typeElement, entityType.getPortMap(), true);
			}
		}
	}
	
	private void appendParameter(Document doc, Element rootElement, Entry<String, String> parameterType) {
		Element parameterTypeElement = doc.createElement("Parameter");
		rootElement.appendChild(parameterTypeElement);
		parameterTypeElement.setAttribute("name", parameterType.getKey());
		parameterTypeElement.setAttribute("value", parameterType.getValue());
	}
	
	private void appendParameterGroupMap(Document doc, Element rootElement, Hashtable<String, ParameterGroup> parameterGroupMap) {
		Enumeration<ParameterGroup> e =  parameterGroupMap.elements();
		while (e.hasMoreElements()) {
			ParameterGroup parameterGroup = e.nextElement();
			Element parameterGroupElement = doc.createElement("ParameterGroup");
			rootElement.appendChild(parameterGroupElement);
			parameterGroupElement.setAttribute("name", parameterGroup.getName());
			Iterator<Entry<String, String> > iter = parameterGroup.getParameterMap().entrySet().iterator();
			while (iter.hasNext()) {
				appendParameter(doc, parameterGroupElement, iter.next());
			}
			this.appendParameterGroupMap(doc, parameterGroupElement, parameterGroup.getParameterGroupMap());
		}
	}
	
	
	private void appendDeviceInstanceSequence(Document doc, Element rootElement, Hashtable<EntityID, EntityInstance> entityInstMap) {
		Element instSequenceElement = doc.createElement("DeviceInstanceSequence");
		rootElement.appendChild(instSequenceElement);
		
		Enumeration<EntityInstance> e = entityInstMap.elements();
		while (e.hasMoreElements()) {
			EntityInstance entityInstance = e.nextElement();
			if (entityInstance.getEntityID().getType().equals(EntityID.Type.DEV)) {
				Element instElement = doc.createElement("DeviceInstance");
				instSequenceElement.appendChild(instElement);
				instElement.setAttribute("name", entityInstance.getEntityID().getName());
				instElement.setAttribute("typename", entityInstance.getTypeName());
				this.appendParameterGroupMap(doc, instElement, entityInstance.getParameterGroupMap());
			}
		}
	}
	
	
	private void appendMSPInstanceSequence(Document doc, Element rootElement, Hashtable<EntityID, EntityInstance> entityInstMap) {
		Element instSequenceElement = doc.createElement("MSPInstanceSequence");
		rootElement.appendChild(instSequenceElement);
		
		Enumeration<EntityInstance> e = entityInstMap.elements();
		while (e.hasMoreElements()) {
			EntityInstance entityInstance = e.nextElement();
			if (entityInstance.getEntityID().getType().equals(EntityID.Type.MSP)) {
				Element instElement = doc.createElement("MSPInstance");
				instSequenceElement.appendChild(instElement);
				instElement.setAttribute("name", entityInstance.getEntityID().getName());
				instElement.setAttribute("typename", entityInstance.getTypeName());
				this.appendParameterGroupMap(doc, instElement, entityInstance.getParameterGroupMap());
			}
		}
	}
	
	
	private void appendTUIObjectInstanceSequence(Document doc, Element rootElement, Hashtable<EntityID, EntityInstance> entityInstMap) {
		Element instSequenceElement = doc.createElement("TUIObjectInstanceSequence");
		rootElement.appendChild(instSequenceElement);
		
		Enumeration<EntityInstance> e = entityInstMap.elements();
		while (e.hasMoreElements()) {
			EntityInstance entityInstance = e.nextElement();
			if (entityInstance.getEntityID().getType().equals(EntityID.Type.TUI)) {
				Element instElement = doc.createElement("TUIObjectInstance");
				instSequenceElement.appendChild(instElement);
				instElement.setAttribute("name", entityInstance.getEntityID().getName());
				instElement.setAttribute("typename", entityInstance.getTypeName());
				this.appendParameterGroupMap(doc, instElement, entityInstance.getParameterGroupMap());
			}
		}
	}
	
	
	private String getEntityTypeTag(EntityID.Type type) {
		switch (type) {
		case DEV: return "DEV";
		case MSP: return "MSP";
		case TUI: return "TUI";
		}
		return null;
	}
	
	
	private void appendConnector(Document doc, Element rootElement, Connector connector) {
		Element connectorElement = doc.createElement("Connector");
		rootElement.appendChild(connectorElement);
		Element srcAddressElement = doc.createElement("SrcAddress");
		Element dstAddressElement = doc.createElement("DstAddress");
		connectorElement.appendChild(srcAddressElement);
		connectorElement.appendChild(dstAddressElement);
		srcAddressElement.setAttribute("entityType", getEntityTypeTag(connector.getSrcAddress().getEntityID().getType()));
		srcAddressElement.setAttribute("entityName", connector.getSrcAddress().getEntityID().getName());
		srcAddressElement.setAttribute("portName", connector.getSrcAddress().getPortName());
		dstAddressElement.setAttribute("entityType", getEntityTypeTag(connector.getDstAddress().getEntityID().getType()));
		dstAddressElement.setAttribute("entityName", connector.getDstAddress().getEntityID().getName());
		dstAddressElement.setAttribute("portName", connector.getDstAddress().getPortName());
	}
	
	
	private void appendConnectorSequence(Document doc, Element rootElement, ArrayList<Connector> connectorVector) {
		Element connectorSequenceElement = doc.createElement("ConnectorSequence");
		rootElement.appendChild(connectorSequenceElement);
		Iterator<Connector> iter = connectorVector.iterator();
		while (iter.hasNext()) {
			this.appendConnector(doc, connectorSequenceElement, iter.next());
		}
	}
	
	
	public void appendServerConfig(Document doc, Element rootElement, Project project) {
		Element serverConfigElement = doc.createElement("ServerConfig");
		rootElement.appendChild(serverConfigElement);
		this.appendStartup(doc, serverConfigElement);
		Hashtable<EntityID, EntityType> instancedEntityTypeMap = project.getEntityTypeDB().getInstancedEntityTypeMap(project.getServerConfig());
		this.appendDeviceTypeSequence(doc, serverConfigElement, instancedEntityTypeMap, project.getServerConfigView());
		this.appendMSPTypeSequence(doc, serverConfigElement, instancedEntityTypeMap, project.getServerConfigView());
		this.appendTUIObjectTypeSequence(doc, serverConfigElement, instancedEntityTypeMap, project.getServerConfigView());
		this.appendDeviceInstanceSequence(doc, serverConfigElement, project.getServerConfig().getEntityInstanceMap());
		this.appendMSPInstanceSequence(doc, serverConfigElement, project.getServerConfig().getEntityInstanceMap());
		this.appendTUIObjectInstanceSequence(doc, serverConfigElement, project.getServerConfig().getEntityInstanceMap());
		this.appendConnectorSequence(doc, serverConfigElement, project.getServerConfig().getConnectorVector());
	}
	
	
	public void exportServerConfig(File file, Project project) {
		TransformerFactory transformerFactory = TransformerFactory.newInstance(); 
		try {
			Transformer serializer = transformerFactory.newTransformer();
			serializer.setOutputProperty(OutputKeys.INDENT, "yes");
			serializer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "3");
			Document doc = createDocument();
			Element rootElement = appendRootElement(doc);
			appendServerConfig(doc, rootElement, project);
			DOMSource src = new DOMSource(doc);
			Result dest = new StreamResult(file);
			serializer.transform(src, dest);
		} catch (TransformerConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (TransformerException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	public void appendProject(Document doc, Element rootElement, Project project) {
		Element projectElement = doc.createElement("TUISettingsProject");
		rootElement.appendChild(projectElement);
		this.appendDeviceTypeSequence(doc, projectElement, project.getEntityTypeDB().getEntityTypeMap(), project.getServerConfigView());
		this.appendMSPTypeSequence(doc, projectElement, project.getEntityTypeDB().getEntityTypeMap(), project.getServerConfigView());
		this.appendTUIObjectTypeSequence(doc, projectElement, project.getEntityTypeDB().getEntityTypeMap(), project.getServerConfigView());
	}
	
	
	public void saveProject(File file, Project project) {
		TransformerFactory transformerFactory = TransformerFactory.newInstance(); 
		try {
			
			Transformer serializer = transformerFactory.newTransformer();
			serializer.setOutputProperty(OutputKeys.INDENT, "yes");
			serializer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "3");
			Document doc = createDocument();
			Element rootElement = appendRootElement(doc);
			appendProject(doc, rootElement, project);
			DOMSource src = new DOMSource(doc);
			Result dest = new StreamResult(file);
			serializer.transform(src, dest);
		} catch (TransformerConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (TransformerException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
