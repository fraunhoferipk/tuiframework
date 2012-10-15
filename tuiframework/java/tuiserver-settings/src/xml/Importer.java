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
import java.io.IOException;
import java.util.Hashtable;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import project.Project;
import view.ServerConfigView;

import configmodel.Editor;
import configmodel.EntityID;
import configmodel.EntityInstance;
import configmodel.EntityType;
import configmodel.InternalException;
import configmodel.ParameterGroup;
import configmodel.ParameterTypeDef;
import configmodel.Port;
import configmodel.ServerConfig;
import configmodel.Port.DataFlowDirection;

/**
 * Importer
 * 
 * @author Oliver Belaifa
 */
public class Importer {
	
	public Port extractPort(Element rootElement, boolean isTUI) {
		String name = rootElement.getAttribute("name");
		String typeName = rootElement.getAttribute("typename");
		String sink = rootElement.getAttribute("sink");
		String source = rootElement.getAttribute("source");
		Port.DataFlowDirection dataFlowDirection = Port.DataFlowDirection.Sink;
		if (isTUI) {
			// TODO tui bug fix
			if (rootElement.getAttribute("sink").equals("1")) {
				dataFlowDirection = Port.DataFlowDirection.Source;
			}
		} else {
			if (rootElement.getAttribute("source").equals("1")) {
				dataFlowDirection = Port.DataFlowDirection.Source;
			}
		}
		
		String description = "";
		NodeList nl = rootElement.getChildNodes();
		for (int i = 0; i < nl.getLength(); ++i) {
			Node node = nl.item(i);
			if (node instanceof Element) {
				Element element = (Element)node;
				if (element.getNodeName().equals("Description")) {
					description = element.getTextContent();
					description.trim();
				}
			}
		}
		
		return new Port(name, typeName, dataFlowDirection, description);
	}
	
	public Hashtable<String, Port> extractPortTypeSequence(Element rootElement, boolean isTUI) {
		Hashtable<String, Port> portMap = new Hashtable<String, Port>();
		NodeList nl = rootElement.getChildNodes();
		for (int i = 0; i < nl.getLength(); ++i) {
			Node node = nl.item(i);
			if (node instanceof Element) {
				Element element = (Element)node;
				if (element.getNodeName().equals("PortType")) {
					Port port = extractPort(element, isTUI);
					portMap.put(port.getName(), port);
				}
			}
		}
		return portMap;
	}
	
	public ParameterTypeDef extractParameterTypeDef(Element rootElement) {
		Hashtable<String, String> parameterTypeMap = new Hashtable<String, String>();
		String name = rootElement.getAttribute("name");
		String description = "";
		
		NodeList nl = rootElement.getChildNodes();
		for (int i = 0; i < nl.getLength(); ++i) {
			Node node = nl.item(i);
			if (node instanceof Element) {
				Element element = (Element)node;
				if (element.getNodeName().equals("ParameterType")) {
					String parameterTypeName = element.getAttribute("name");
					String parameterTypeType = element.getAttribute("type");
					parameterTypeMap.put(parameterTypeName, parameterTypeType);
				}
				if (element.getNodeName().equals("Description")) {
					description = element.getFirstChild().getNodeValue();
					description.trim();
				}
			}
		}

		return new ParameterTypeDef(name, parameterTypeMap, description);
	}
	
	public Hashtable<String, ParameterTypeDef> extractParameterTypeDefSequence(Element rootElement) {
		Hashtable<String, ParameterTypeDef> parameterTypeDefMap = new Hashtable<String, ParameterTypeDef>();
		NodeList nl = rootElement.getChildNodes();
		for (int i = 0; i < nl.getLength(); ++i) {
			Node node = nl.item(i);
			if (node instanceof Element) {
				Element element = (Element)node;
				if (element.getNodeName().equals("ParameterTypeDef")) {
					ParameterTypeDef parameterTypeDef = extractParameterTypeDef(element);
					parameterTypeDefMap.put(parameterTypeDef.getName(), parameterTypeDef);
				}
			}
		}
		
		return parameterTypeDefMap;
	}

	Hashtable<String, String> extractParameterTypeSequence(Element rootElement) {
		Hashtable<String, String> parameterTypeMap = new Hashtable<String, String>();
		NodeList nl = rootElement.getChildNodes();
		for (int i = 0; i < nl.getLength(); ++i) {
			Node node = nl.item(i);
			if (node instanceof Element) {
				Element element = (Element)node;
				if (element.getNodeName().equals("ParameterType")) {
					String parameterTypeName = element.getAttribute("name");
					String parameterTypeType = element.getAttribute("type");
					parameterTypeMap.put(parameterTypeName, parameterTypeType);
				}
			}
		}	
		return parameterTypeMap;
	}
	
	
	public void handleEntityTypeImage(ServerConfigView serverConfigView, String path, EntityID entityID) {
		try {
			if (serverConfigView.getImageDB().hasImage(path) || serverConfigView.getImageDB().loadImage(path)) {
				serverConfigView.getImageDB().setEntityTypeImage(entityID, path);
			}
		} catch (InternalException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public EntityType extractEntityType(EntityID.Type type, Element rootElement, boolean isTUI, ServerConfigView serverConfigView) {
		String name = rootElement.getAttribute("name");
		String description = "";
		Hashtable<String, Port> portMap = new Hashtable<String, Port>();
		Hashtable<String, ParameterTypeDef> parameterTypeDefMap = new Hashtable<String, ParameterTypeDef>();
		Hashtable<String, String> parameterTypeMap = new Hashtable<String, String>();
		Hashtable<String, ParameterGroup> parameterGroupMap = new Hashtable<String, ParameterGroup>();
		
		EntityID entityID = new EntityID(name, type);
		
		NodeList nl = rootElement.getChildNodes();
		for (int i = 0; i < nl.getLength(); ++i) {
			Node node = nl.item(i);
			if (node instanceof Element) {
				Element element = (Element)node;
				if (element.getNodeName().equals("Image")) {
					String path = element.getAttribute("path");
					handleEntityTypeImage(serverConfigView, path, entityID);
				}
				if (element.getNodeName().equals("Description")) {
					description = element.getFirstChild().getNodeValue();
					description.trim();
				}
				if (element.getNodeName().equals("PortTypeSequence")) {
					portMap = extractPortTypeSequence(element, isTUI);
				}
				if (element.getNodeName().equals("ParameterTypeDefSequence")) {
					parameterTypeDefMap = extractParameterTypeDefSequence(element);
				}
				if (element.getNodeName().equals("ParameterTypeSequence")) {
					parameterTypeMap = extractParameterTypeSequence(element); 
				}
				if (element.getNodeName().equals("ParameterGroup")) {
					ParameterGroup parameterGroup = extractParameterGroup(element);
					parameterGroupMap.put(parameterGroup.getName(), parameterGroup);
				}
				
			}
		}
		
		EntityType entityType = new EntityType(entityID, portMap, description);
		entityType.setParameterTypeDefMap(parameterTypeDefMap);
		entityType.setParameterTypeMap(parameterTypeMap);
		entityType.setDefaultParameterGroupMap(parameterGroupMap);
		return entityType;
	}
	
	
	public void handleDefaultEntityTypeImage(ServerConfigView serverConfigView, String path, EntityID.Type type) {
		try {
			if (serverConfigView.getImageDB().hasImage(path) || serverConfigView.getImageDB().loadImage(path)) {
				serverConfigView.getImageDB().setDefaultTypeImage(EntityID.Type.DEV, path);
			}
		} catch (InternalException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	public void extractDeviceTypeSequence(Editor editor, Element rootElement, ServerConfigView serverConfigView) {
		NodeList nl = rootElement.getChildNodes();
		for (int i = 0; i < nl.getLength(); ++i) {
			Node node = nl.item(i);
			if (node instanceof Element) {
				Element element = (Element)node;
				if (element.getNodeName().equals("DeviceType")) {
					EntityType entityType = extractEntityType(EntityID.Type.DEV, element, false, serverConfigView);
					editor.getProject().getEntityTypeDB().getEntityTypeMap().remove(entityType.getEntityID());
					editor.getProject().getEntityTypeDB().getEntityTypeMap().put(entityType.getEntityID(), entityType);
				}
				if (element.getNodeName().equals("Image")) {
					String path = element.getAttribute("path");
					handleDefaultEntityTypeImage(serverConfigView, path, EntityID.Type.DEV);
				}
			}
		}	
	}
	
	
	public void extractMSPTypeSequence(Editor editor, Element rootElement, ServerConfigView serverConfigView) {
		NodeList nl = rootElement.getChildNodes();
		for (int i = 0; i < nl.getLength(); ++i) {
			Node node = nl.item(i);
			if (node instanceof Element) {
				Element element = (Element)node;
				if (element.getNodeName().equals("MSPType")) {
					EntityType entityType = extractEntityType(EntityID.Type.MSP, element, false, serverConfigView);
					editor.getProject().getEntityTypeDB().getEntityTypeMap().remove(entityType.getEntityID());
					editor.getProject().getEntityTypeDB().getEntityTypeMap().put(entityType.getEntityID(), entityType);
				}
				if (element.getNodeName().equals("Image")) {
					String path = element.getAttribute("path");
					handleDefaultEntityTypeImage(serverConfigView, path, EntityID.Type.MSP);
				}

			}
			
		}	
	}
	
	
	public void extractTUIObjectTypeSequence(Editor editor, Element rootElement, ServerConfigView serverConfigView) {
		NodeList nl = rootElement.getChildNodes();
		for (int i = 0; i < nl.getLength(); ++i) {
			Node node = nl.item(i);
			if (node instanceof Element) {
				Element element = (Element)node;
				if (element.getNodeName().equals("TUIObjectType")) {
					EntityType entityType = extractEntityType(EntityID.Type.TUI, element, true, serverConfigView);
					editor.getProject().getEntityTypeDB().getEntityTypeMap().remove(entityType.getEntityID());
					editor.getProject().getEntityTypeDB().getEntityTypeMap().put(entityType.getEntityID(), entityType);
				}
				if (element.getNodeName().equals("Image")) {
					String path = element.getAttribute("path");
					handleDefaultEntityTypeImage(serverConfigView, path, EntityID.Type.TUI);
				}
			}
		}	
	}
	
	
	public ParameterGroup extractParameterGroup(Element rootElement) {
		String parameterGroupName = rootElement.getAttribute("name");
		ParameterGroup parameterGroup = new ParameterGroup(parameterGroupName);
		NodeList nl = rootElement.getChildNodes();
		for (int i = 0; i < nl.getLength(); ++i) {
			Node node = nl.item(i);
			if (node instanceof Element) {
				Element element = (Element)node;
				if (element.getNodeName().equals("ParameterGroup")) {
					ParameterGroup childParameterGroup = extractParameterGroup(element);
					parameterGroup.getParameterGroupMap().put(childParameterGroup.getName(), childParameterGroup);
				}
				if (element.getNodeName().equals("Parameter")) {
					String name = element.getAttribute("name");
					String value = element.getAttribute("value");
					parameterGroup.getParameterMap().put(name, value);
				}
			}
		}
		return parameterGroup;
	}
	
	
	public EntityInstance extractEntityInstance(EntityID.Type type, Editor editor, Element rootElement) throws InternalException {
		String name = rootElement.getAttribute("name");
		String typeName = rootElement.getAttribute("typename");
		String description = "";
		Hashtable<String, ParameterGroup> parameterGroupMap = new Hashtable<String, ParameterGroup>();
		
		NodeList nl = rootElement.getChildNodes();
		for (int i = 0; i < nl.getLength(); ++i) {
			Node node = nl.item(i);
			if (node instanceof Element) {
				Element element = (Element)node;
				if (element.getNodeName().equals("Description")) {
					description = element.getFirstChild().getNodeValue();
					description.trim();
				}
				if (element.getNodeName().equals("ParameterGroup")) {
					ParameterGroup parameterGroup = extractParameterGroup(element);
					parameterGroupMap.put(parameterGroup.getName(), parameterGroup);
				}
			}
		}
		EntityID entityTypeID = new EntityID(typeName, type);
		
		EntityInstance entityInstance = editor.createInstance(name, entityTypeID, description);
		entityInstance.setParameterGroupMap(parameterGroupMap);
		return entityInstance;
	}
	
	public void extractDeviceInstanceSequence(Editor editor, Element rootElement) {
		NodeList nl = rootElement.getChildNodes();
		for (int i = 0; i < nl.getLength(); ++i) {
			Node node = nl.item(i);
			if (node instanceof Element) {
				Element element = (Element)node;
				if (element.getNodeName().equals("DeviceInstance")) {
					try {
						EntityInstance entityInstance = extractEntityInstance(EntityID.Type.DEV, editor, element);
						editor.getProject().getServerConfig().getEntityInstanceMap().put(entityInstance.getEntityID(), entityInstance);
					} catch (InternalException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}
		}		
	}
	
	public void extractMSPInstanceSequence(Editor editor, Element rootElement) {
		NodeList nl = rootElement.getChildNodes();
		for (int i = 0; i < nl.getLength(); ++i) {
			Node node = nl.item(i);
			if (node instanceof Element) {
				Element element = (Element)node;
				if (element.getNodeName().equals("MSPInstance")) {
					try {
						EntityInstance entityInstance = extractEntityInstance(EntityID.Type.MSP, editor, element);
						editor.getProject().getServerConfig().getEntityInstanceMap().put(entityInstance.getEntityID(), entityInstance);
					} catch (InternalException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}
		}		
	}
	
	public void extractTUIObjectInstanceSequence(Editor editor, Element rootElement) {
		NodeList nl = rootElement.getChildNodes();
		for (int i = 0; i < nl.getLength(); ++i) {
			Node node = nl.item(i);
			if (node instanceof Element) {
				Element element = (Element)node;
				if (element.getNodeName().equals("TUIObjectInstance")) {
					try {
						EntityInstance entityInstance = extractEntityInstance(EntityID.Type.TUI, editor, element);
						editor.getProject().getServerConfig().getEntityInstanceMap().put(entityInstance.getEntityID(), entityInstance);
					} catch (InternalException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}
		}		
	}
	
	private EntityID.Type getEntityType(String entityTypeTag) throws InternalException {
		if (entityTypeTag.equals("DEV")) {
			return EntityID.Type.DEV;
		}
		if (entityTypeTag.equals("MSP")) {
			return EntityID.Type.MSP;
		}
		if (entityTypeTag.equals("TUI")) {
			return EntityID.Type.TUI;
		}
		throw new InternalException("Invalid entity type tag \"" + entityTypeTag + "\"");
		
	}
	
	public void extractConnector(Editor editor, Element rootElement) {
		EntityID.Type srcEntityType = EntityID.Type.DEV;
		String srcEntityName = "";
		String srcPortName = "";
		EntityID.Type dstEntityType = EntityID.Type.DEV;
		String dstEntityName = "";
		String dstPortName = "";
		
		try {
			NodeList nl = rootElement.getChildNodes();
			for (int i = 0; i < nl.getLength(); ++i) {
				Node node = nl.item(i);
				if (node instanceof Element) {
					Element element = (Element)node;
					if (element.getNodeName().equals("SrcAddress")) {
						srcEntityType = getEntityType(element.getAttribute("entityType"));
						srcEntityName = element.getAttribute("entityName");
						srcPortName = element.getAttribute("portName");
					}
					if (element.getNodeName().equals("DstAddress")) {
						dstEntityType = getEntityType(element.getAttribute("entityType"));
						dstEntityName = element.getAttribute("entityName");
						dstPortName = element.getAttribute("portName");						
					}
				}
			}
			editor.connectPorts(new EntityID(srcEntityName, srcEntityType), srcPortName, new EntityID(dstEntityName, dstEntityType), dstPortName);
		} catch (InternalException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	
	public void extractConnectorSequence(Editor editor, Element rootElement) {
		NodeList nl = rootElement.getChildNodes();
		for (int i = 0; i < nl.getLength(); ++i) {
			Node node = nl.item(i);
			if (node instanceof Element) {
				Element element = (Element)node;
				if (element.getNodeName().equals("Connector")) {
					extractConnector(editor, element);
				}
			}
		}
	}
	
	
	public void extractServerConfig(Element rootElement, Project project) {
		project.getServerConfig().clear();
		Editor editor = new Editor(project);
		NodeList nl = rootElement.getChildNodes();
		for (int i = 0; i < nl.getLength(); ++i) {
			Node node = nl.item(i);
			if (node instanceof Element) {
				Element element = (Element)node;
				if (element.getNodeName().equals("DeviceTypeSequence")) {
					extractDeviceTypeSequence(editor, element, project.getServerConfigView());
				}
				if (element.getNodeName().equals("MSPTypeSequence")) {
					extractMSPTypeSequence(editor, element, project.getServerConfigView());
				}
				if (element.getNodeName().equals("TUIObjectTypeSequence")) {
					extractTUIObjectTypeSequence(editor, element, project.getServerConfigView());
				}
				if (element.getNodeName().equals("DeviceInstanceSequence")) {
					extractDeviceInstanceSequence(editor, element);
				}
				if (element.getNodeName().equals("MSPInstanceSequence")) {
					extractMSPInstanceSequence(editor, element);
				}
				if (element.getNodeName().equals("TUIObjectInstanceSequence")) {
					extractTUIObjectInstanceSequence(editor, element);
				}
				if (element.getNodeName().equals("ConnectorSequence")) {
					extractConnectorSequence(editor, element);
				}
			}
		}	
	}
	
	public void importServerConfig(File file, Project project) {
		System.out.println(file.getAbsolutePath());
		
		DocumentBuilderFactory domainBuilderFactory = DocumentBuilderFactory.newInstance();
		domainBuilderFactory.setXIncludeAware(true);
		domainBuilderFactory.setNamespaceAware(true);
		try {
			DocumentBuilder domainBuilder = domainBuilderFactory.newDocumentBuilder();
			
			Document dom = domainBuilder.parse(file);
			NodeList nodeList = dom.getElementsByTagName("ServerConfig");
			if (nodeList.getLength() == 0) {
				throw new InternalException("ServerConfig elements does not exist");
			}
			if (nodeList.getLength() > 1) {
				throw new InternalException("More than one ServerConfig element found - import action aborted.");
			}
			Element serverConfigElement = (Element)nodeList.item(0);
			this.extractServerConfig(serverConfigElement, project);
		} catch (ParserConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (SAXException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InternalException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	public void readProject(File file, Project project) {
		System.out.println(file.getAbsolutePath());
		
		DocumentBuilderFactory domainBuilderFactory = DocumentBuilderFactory.newInstance();
		domainBuilderFactory.setXIncludeAware(true);
		domainBuilderFactory.setNamespaceAware(true);
		try {
			DocumentBuilder domainBuilder = domainBuilderFactory.newDocumentBuilder();
			
			Document dom = domainBuilder.parse(file);
			NodeList nodeList = dom.getElementsByTagName("TUISettingsProject");
			if (nodeList.getLength() == 0) {
				throw new InternalException("TUISettingsProject elements does not exist");
			}
			if (nodeList.getLength() > 1) {
				throw new InternalException("More than one ServerConfig element found - import action aborted.");
			}
			Element serverConfigElement = (Element)nodeList.item(0);
			this.extractServerConfig(serverConfigElement, project);
		} catch (ParserConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (SAXException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InternalException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
