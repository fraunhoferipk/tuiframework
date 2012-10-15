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


#include "ServerConfigXMLReader.h"

#include "XMLParseErrorReporter.h"
#include "XMLDOMErrorReporter.h"
#include "XMLNodeFilter.h"

#include "../logging/Logger.h"

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>

#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

#include <iostream>


#include <map>
#include <vector>

using namespace XERCES_CPP_NAMESPACE;
using namespace tuiframework;
using namespace std;

namespace tuiframework {

static Port extractPort(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {

    Port port;
    int dataFlowDirection = 0;

    DOMNode * node = domTreeWalker->getCurrentNode();
    DOMNamedNodeMap * nodeMap = node->getAttributes();

    DOMNode * nameAttribute = nodeMap->getNamedItem(XMLString::transcode("name"));
    if (nameAttribute) {
        port.setName(XMLString::transcode(nameAttribute->getNodeValue()));
        //TFDEBUG(XMLString::transcode(nameAttribute->getNodeValue()));
    } else {
        TFERROR("");
    }

    DOMNode * typenameAttribute = nodeMap->getNamedItem(XMLString::transcode("typename"));
    if (typenameAttribute) {
        port.setTypeName(XMLString::transcode(typenameAttribute->getNodeValue()));
        //TFDEBUG(XMLString::transcode(typenameAttribute->getNodeValue()));
    } else {
        TFERROR("");
    }

    DOMNode * sinkAttribute = nodeMap->getNamedItem(XMLString::transcode("sink"));
    if (sinkAttribute) {
        if (XMLString::compareString(XMLString::transcode(sinkAttribute->getNodeValue()), "1") == 0) {
            dataFlowDirection |= static_cast<int>(Port::Sink);
            //TFDEBUG("DataFlowDirection = " << dataFlowDirection);
        }
    } else {
        TFERROR("");
    }

    DOMNode * sourceAttribute = nodeMap->getNamedItem(XMLString::transcode("source"));
    if (sourceAttribute) {
        if (XMLString::compareString(XMLString::transcode(sourceAttribute->getNodeValue()), "1") == 0) {
            dataFlowDirection |= static_cast<int>(Port::Source);

            //TFDEBUG("DataFlowDirection = " << dataFlowDirection);
        }
    } else {
        TFERROR("");
    }

    port.setDataFlowDirection(static_cast<Port::DataFlowDirection>(dataFlowDirection));

    while (node) {
        //TFDEBUG("");
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "Description") == 0) {
            XMLCh * textContent = XMLString::replicate(node->getTextContent());
            XMLString::trim(textContent);
            port.setDescription(XMLString::transcode(textContent));
            XMLString::release(&textContent);
        }

        node = domTreeWalker->nextNode();
    }

    return port;
}


static map<string, Port> extractPortMap(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {
    map<string, Port> portMap;

    DOMNode * node = domTreeWalker->getCurrentNode();
    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "PortType") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            Port port = extractPort(domDocument, d);
            portMap[port.getName()] = port;
            d->release();
            delete nodeFilter;
        }
        node = domTreeWalker->nextNode();
    }

    return portMap;
}


static ParameterGroup extractParameterGroup(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {

    map<string, string> parameterMap;
    map<string, ParameterGroup> parameterGroupMap;

    DOMNode * node = domTreeWalker->nextNode();
    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "Parameter") == 0) {
            DOMNamedNodeMap * nodeMap = node->getAttributes();

            string name;
            DOMNode * nameAttribute = nodeMap->getNamedItem(XMLString::transcode("name"));
            if (nameAttribute) {
                name = XMLString::transcode(nameAttribute->getNodeValue());
            }
            string value;
            DOMNode * valueAttribute = nodeMap->getNamedItem(XMLString::transcode("value"));
            if (valueAttribute) {
                value = XMLString::transcode(valueAttribute->getNodeValue());
            }
            parameterMap[name] = value;

            //TFINFO("Parameter: " << name << " -> " << value);
        } else if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "ParameterGroup") == 0) {

            DOMNamedNodeMap * nodeMap = node->getAttributes();
            string name;
            DOMNode * nameAttribute = nodeMap->getNamedItem(XMLString::transcode("name"));
            if (nameAttribute) {
                name = XMLString::transcode(nameAttribute->getNodeValue());
            }

            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            ParameterGroup pg = extractParameterGroup(domDocument, d);
            pg.setName(name);
            parameterGroupMap[name] = pg;
            d->release();
            delete nodeFilter;
        }
        node = domTreeWalker->nextNode();
    }

    ParameterGroup parameterGroup;
    parameterGroup.setParameterMap(parameterMap);
    parameterGroup.setParameterGroupMap(parameterGroupMap);

    return parameterGroup;
}



static DeviceInstance extractDeviceInstance(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {
    DeviceInstance deviceInstance;
    map<string, ParameterGroup> parameterGroupMap;

    DOMNode * node = domTreeWalker->getCurrentNode();
    DOMNamedNodeMap * nodeMap = node->getAttributes();

    DOMNode * nameAttribute = nodeMap->getNamedItem(XMLString::transcode("name"));
    if (nameAttribute) {
        deviceInstance.setName(XMLString::transcode(nameAttribute->getNodeValue()));
    }

    DOMNode * typenameAttribute = nodeMap->getNamedItem(XMLString::transcode("typename"));
    if (typenameAttribute) {
        deviceInstance.setDeviceTypeName(XMLString::transcode(typenameAttribute->getNodeValue()));
    }

    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "ParameterGroup") == 0) {
            DOMNamedNodeMap * nodeMap = node->getAttributes();
            string name;
            DOMNode * nameAttribute = nodeMap->getNamedItem(XMLString::transcode("name"));
            if (nameAttribute) {
               name = XMLString::transcode(nameAttribute->getNodeValue());
            }

            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);

            ParameterGroup pg = extractParameterGroup(domDocument, d);
            pg.setName(name);
            parameterGroupMap[name] = pg;

            d->release();
            delete nodeFilter;
        }
        node = domTreeWalker->nextNode();
    }

    ParameterGroup parameterGroup;
    parameterGroup.setName("root");
    parameterGroup.setParameterGroupMap(parameterGroupMap);
    deviceInstance.setParameterGroup(parameterGroup);

    return deviceInstance;
}



static DeviceType extractDeviceType(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {
    DeviceType deviceType;
    DOMNode * node = domTreeWalker->getCurrentNode();
    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "PortTypeSequence") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            deviceType.setPortMap(extractPortMap(domDocument, d));
            d->release();
            delete nodeFilter;
        } else if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "Description") == 0) {
            XMLCh * textContent = XMLString::replicate(node->getTextContent());
            XMLString::trim(textContent);
            deviceType.setDescription(XMLString::transcode(textContent));
            XMLString::release(&textContent);
        }

        node = domTreeWalker->nextNode();
    }
    return deviceType;
}


static PortAddress extractPortAddress(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {
    PortAddress portAddress;

    DOMNode * node = domTreeWalker->getCurrentNode();
    DOMNamedNodeMap * nodeMap = node->getAttributes();

    // entityType = DEV | TUI | MSP
    DOMNode * entityTypeAttribute = nodeMap->getNamedItem(XMLString::transcode("entityType"));
    if (entityTypeAttribute) {
        if (XMLString::compareString(XMLString::transcode(entityTypeAttribute->getNodeValue()), "DEV") == 0) {
            //TFINFO("DEV");
            portAddress.setOwnerType(PortAddress::DEVICE);
        } else if (XMLString::compareString(XMLString::transcode(entityTypeAttribute->getNodeValue()), "TUI") == 0) {
            //TFINFO("TUI");
            portAddress.setOwnerType(PortAddress::TUIOBJECT);
        } else if (XMLString::compareString(XMLString::transcode(entityTypeAttribute->getNodeValue()), "MSP") == 0) {
            //TFINFO("MSP");
            portAddress.setOwnerType(PortAddress::MSP);
        } else {
            // TODO throw Exception
            TFERROR("");
        }
    } else {
        TFERROR("No entityType found")
    }

    DOMNode * entityNameAttribute = nodeMap->getNamedItem(XMLString::transcode("entityName"));
    if (entityNameAttribute) {
        portAddress.setName(XMLString::transcode(entityNameAttribute->getNodeValue()));
    } else {
        TFERROR("No entityName found")
    }
    DOMNode * portNameAttribute = nodeMap->getNamedItem(XMLString::transcode("portName"));
    if (portNameAttribute) {
        portAddress.setPortName(XMLString::transcode(portNameAttribute->getNodeValue()));
    } else {
        TFERROR("No portName found")
    }

    return portAddress;
}



static Connector extractConnector(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {
    Connector connector;

    DOMNode * node = domTreeWalker->getCurrentNode();
    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "SrcAddress") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            PortAddress portAddress = extractPortAddress(domDocument, d);
            connector.setSrcAddress(portAddress);
            d->release();
            delete nodeFilter;
        }

        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "DstAddress") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            PortAddress portAddress = extractPortAddress(domDocument, d);
            connector.setDstAddress(portAddress);
            d->release();
            delete nodeFilter;
        }

        node = domTreeWalker->nextNode();
    }

    return connector;
}


static map<string, Connector> extractConnectorMap(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {
    map<string, Connector> connectorMap;

    DOMNode * node = domTreeWalker->getCurrentNode();
    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "Connector") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            Connector connector = extractConnector(domDocument, d);
            // TODO extractConnectorMap
            //connectorMap[connector.getChannel2Name()] = connector;
            d->release();
            delete nodeFilter;
        }
        node = domTreeWalker->nextNode();
    }
    return connectorMap;
}


static TUIObjectType extractTUIObjectType(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {
    TUIObjectType tuiObjectType;
    DOMNode * node = domTreeWalker->getCurrentNode();
    DOMNamedNodeMap * nodeMap = node->getAttributes();
    DOMNode * nameAttribute = nodeMap->getNamedItem(XMLString::transcode("name"));
    if (nameAttribute) {
        tuiObjectType.setName(XMLString::transcode(nameAttribute->getNodeValue()));
    }

    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "PortTypeSequence") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            tuiObjectType.setPortMap(extractPortMap(domDocument, d));
            d->release();
            delete nodeFilter;
        } else if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "Description") == 0) {
            XMLCh * textContent = XMLString::replicate(node->getTextContent());
            XMLString::trim(textContent);
            tuiObjectType.setDescription(XMLString::transcode(textContent));
            XMLString::release(&textContent);
        }
        node = domTreeWalker->nextNode();
    }
    return tuiObjectType;
}


static TUIObjectInstance extractTUIObjectInstance(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {

    TUIObjectInstance tuiObjectInstance;
    DOMNode * node = domTreeWalker->getCurrentNode();
    DOMNamedNodeMap * nodeMap = node->getAttributes();

    DOMNode * nameAttribute = nodeMap->getNamedItem(XMLString::transcode("name"));
    if (nameAttribute) {
        tuiObjectInstance.setName(XMLString::transcode(nameAttribute->getNodeValue()));
    }

    DOMNode * tuiTypeNameAttribute = nodeMap->getNamedItem(XMLString::transcode("typename"));
    if (tuiTypeNameAttribute) {
        tuiObjectInstance.setTypeName(XMLString::transcode(tuiTypeNameAttribute->getNodeValue()));
    }

    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "Description") == 0) {
            XMLCh * textContent = XMLString::replicate(node->getTextContent());
            XMLString::trim(textContent);
            tuiObjectInstance.setDescription(XMLString::transcode(textContent));
            XMLString::release(&textContent);
        }
        node = domTreeWalker->nextNode();
    }

    return tuiObjectInstance;
}





static MSPInstance extractMSPInstance(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {
    MSPInstance mspInstance;
    map<string, ParameterGroup> parameterGroupMap;

    DOMNode * node = domTreeWalker->getCurrentNode();
    DOMNamedNodeMap * nodeMap = node->getAttributes();

    DOMNode * nameAttribute = nodeMap->getNamedItem(XMLString::transcode("name"));
    if (nameAttribute) {
        mspInstance.setName(XMLString::transcode(nameAttribute->getNodeValue()));
    }

    DOMNode * typeNameAttribute = nodeMap->getNamedItem(XMLString::transcode("typename"));

    if (typeNameAttribute) {
        //TFDEBUG(XMLString::transcode(typeNameAttribute->getNodeValue()));
        mspInstance.setTypeName(XMLString::transcode(typeNameAttribute->getNodeValue()));
    }

    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "ParameterGroup") == 0) {
            DOMNamedNodeMap * nodeMap = node->getAttributes();
            string name;
            DOMNode * nameAttribute = nodeMap->getNamedItem(XMLString::transcode("name"));
            if (nameAttribute) {
               name = XMLString::transcode(nameAttribute->getNodeValue());
            }

            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            ParameterGroup pg = extractParameterGroup(domDocument, d);
            pg.setName(name);
            parameterGroupMap[name] = pg;
            d->release();
            delete nodeFilter;
        } else if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "Description") == 0) {
            XMLCh * textContent = XMLString::replicate(node->getTextContent());
            XMLString::trim(textContent);
            mspInstance.setDescription(XMLString::transcode(textContent));
            XMLString::release(&textContent);
        }
        node = domTreeWalker->nextNode();
    }

    ParameterGroup parameterGroup;
    parameterGroup.setName("root");
    parameterGroup.setParameterGroupMap(parameterGroupMap);
    mspInstance.setParameterGroup(parameterGroup);

    return mspInstance;
}


static ServerStartupConfig extractServerStartupConfig(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {
    ServerStartupConfig serverStartupConfig;
    DOMNode * node = domTreeWalker->getCurrentNode();
    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "Network") == 0) {
            DOMNamedNodeMap * nodeMap = node->getAttributes();
            DOMNode * portAttribute = nodeMap->getNamedItem(XMLString::transcode("port"));
            if (portAttribute) {
                
                serverStartupConfig.setPortNr(XMLString::parseInt(portAttribute->getNodeValue()));
            }
        }
        node = domTreeWalker->nextNode();
    }

    return serverStartupConfig;
}


static map<string, DeviceType> extractDeviceTypeMap(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {
    map<string, DeviceType> deviceTypeMap;

    DOMNode * node = domTreeWalker->getCurrentNode();
    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "DeviceType") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            DeviceType deviceType = extractDeviceType(domDocument, d);
            deviceTypeMap[deviceType.getDeviceTypeName()] = deviceType;
            d->release();
            delete nodeFilter;
        }
        node = domTreeWalker->nextNode();
    }

    return deviceTypeMap;
}


static map<string, DeviceInstance> extractDeviceInstanceMap(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {
    map<string, DeviceInstance> deviceInstanceMap;

    DOMNode * node = domTreeWalker->getCurrentNode();
    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "DeviceInstance") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            DeviceInstance deviceInstance = extractDeviceInstance(domDocument, d);
            deviceInstanceMap[deviceInstance.getName()] = deviceInstance;
            d->release();
            delete nodeFilter;
        }
        node = domTreeWalker->nextNode();
    }

    return deviceInstanceMap;
}


static map<string, TUIObjectType> extractTUIObjectTypeMap(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {
    map<string, TUIObjectType> tuiObjectTypeMap;

    DOMNode * node = domTreeWalker->getCurrentNode();
    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "TUIObjectType") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            TUIObjectType tuiObjectType = extractTUIObjectType(domDocument, d);
            tuiObjectTypeMap[tuiObjectType.getName()] = tuiObjectType;
            d->release();
            delete nodeFilter;
        }
        node = domTreeWalker->nextNode();
    }
    return tuiObjectTypeMap;
}


static map<string, TUIObjectInstance> extractTUIObjectInstanceMap(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {
    map<string, TUIObjectInstance> tuiObjectInstanceMap;

    DOMNode * node = domTreeWalker->getCurrentNode();
    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "TUIObjectInstance") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            TUIObjectInstance tuiObjectInstance = extractTUIObjectInstance(domDocument, d);
            tuiObjectInstanceMap[tuiObjectInstance.getName()] = tuiObjectInstance;
            d->release();
            delete nodeFilter;
        }
        node = domTreeWalker->nextNode();
    }

    return tuiObjectInstanceMap;
}


static vector<Connector> extractConnectorVector(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {
    vector<Connector> connectorVector;

    DOMNode * node = domTreeWalker->getCurrentNode();
    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "Connector") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            Connector connector = extractConnector(domDocument, d);
            connectorVector.push_back(connector);
            d->release();
            delete nodeFilter;
        }
        node = domTreeWalker->nextNode();
    }
    return connectorVector;
}



static map<string, MSPInstance> extractMSPInstanceMap(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {
    map<string, MSPInstance> mspInstanceMap;

    DOMNode * node = domTreeWalker->getCurrentNode();
    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "MSPInstance") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            MSPInstance mspInstance = extractMSPInstance(domDocument, d);
            mspInstanceMap[mspInstance.getName()] = mspInstance;
            d->release();
            delete nodeFilter;
        }
        node = domTreeWalker->nextNode();
    }

    return mspInstanceMap;
}



static ServerConfig extractServerConfig(DOMDocument * domDocument, DOMTreeWalker * domTreeWalker) {
    ServerConfig serverConfig;

    DOMNode * node = domTreeWalker->getCurrentNode();
    while (node) {
        if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "ServerStartupConfig") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            serverConfig.setServerStartupConfig(extractServerStartupConfig(domDocument, d));
            d->release();
            delete nodeFilter;
        } else if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "DeviceTypeSequence") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            serverConfig.setDeviceTypeMap(extractDeviceTypeMap(domDocument, d));
            d->release();
            delete nodeFilter;
        } else if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "DeviceInstanceSequence") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            serverConfig.setDeviceInstanceMap(extractDeviceInstanceMap(domDocument, d));
            d->release();
            delete nodeFilter;
        } else if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "TUIObjectTypeSequence") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            serverConfig.setTUIObjectTypeMap(extractTUIObjectTypeMap(domDocument, d));
            d->release();
            delete nodeFilter;
        } else if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "TUIObjectInstanceSequence") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            serverConfig.setTUIObjectInstanceMap(extractTUIObjectInstanceMap(domDocument, d));
            d->release();
            delete nodeFilter;        
        } else if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "ConnectorSequence") == 0) {
              XMLNodeFilter * nodeFilter = new XMLNodeFilter();
              DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
              serverConfig.setConnectorVector(extractConnectorVector(domDocument, d));
              d->release();
              delete nodeFilter;
        } else if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "MSPTypeSequence") == 0) {
            XMLNodeFilter * nodeFilter = new XMLNodeFilter();
            DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
            //serverConfig.setMSPTypeMap(extractMSPTypeMap(domDocument, d));
            d->release();
            delete nodeFilter;
        } else if (XMLString::compareString(XMLString::transcode(node->getNodeName()), "MSPInstanceSequence") == 0) {
              XMLNodeFilter * nodeFilter = new XMLNodeFilter();
              DOMTreeWalker * d = domDocument->createTreeWalker(node, DOMNodeFilter::SHOW_ALL, nodeFilter, true);
              serverConfig.setMSPInstanceMap(extractMSPInstanceMap(domDocument, d));
              d->release();
              delete nodeFilter;
        }

        node = domTreeWalker->nextNode();
    }

    return serverConfig;
}


static void removeBaseAttr(DOMDocument * domDocument) {

    XMLNodeFilter * nodeFilter = new XMLNodeFilter();
    DOMTreeWalker * domTreeWalker = domDocument->createTreeWalker(domDocument, DOMNodeFilter::SHOW_ALL, nodeFilter, true);

    DOMNode * node = domTreeWalker->getCurrentNode();
    while (node) {
        DOMNamedNodeMap * nodeMap = node->getAttributes();
        if (nodeMap != 0) {
            DOMNode * attr = nodeMap->getNamedItem(XMLString::transcode("xml:base"));
            if (attr) {
                DOMNode * node = nodeMap->removeNamedItem(XMLString::transcode("xml:base"));

                TFINFO("[Server Config] filtered attribute: " << XMLString::transcode(node->getNodeName()) << "=" << XMLString::transcode(node->getNodeValue()));
                node->release();
            }
        }
        node = domTreeWalker->nextNode();
    }
    delete nodeFilter;
}



ServerConfigXMLReader::ServerConfigXMLReader() {
}


ServerConfigXMLReader::~ServerConfigXMLReader() {
}


const ServerConfig & ServerConfigXMLReader::getServerConfig() const {
    return this->serverConfig;
}


bool ServerConfigXMLReader::readServerConfig(const std::string & path) {

    this->serverConfig = ServerConfig();

    try {
        XMLPlatformUtils::Initialize();
    }  catch (const XMLException& toCatch) {

        char * message = XMLString::transcode(toCatch.getMessage());
        cout << "Error during initialization! :\n" << message << "\n";
        XMLString::release(&message);

        return false;
    }

    XercesDOMParser * parser = new XercesDOMParser();

    //parser->setValidationScheme(XercesDOMParser::Val_Auto);
    parser->setDoNamespaces(true);
    parser->setDoXInclude(true);
    //parser->setValidationScheme(XercesDOMParser::Val_Always);
  //  parser->setDoSchema(true);
  //  parser->setValidationSchemaFullChecking(true);

    XMLParseErrorReporter * xmlParseErrorReporter = new XMLParseErrorReporter();;
    parser->setErrorHandler(xmlParseErrorReporter);
    try {
        parser->parse(path.c_str());
    }  catch (const XMLException & toCatch) {
        char * message = XMLString::transcode(toCatch.getMessage());
        cout << "Exception message is: \n" << message << "\n";
        XMLString::release(&message);
        return false;
    }
    catch (const DOMException& toCatch) {
        char * message = XMLString::transcode(toCatch.msg);
        cout << "Exception message is: \n" << message << "\n";
        XMLString::release(&message);
        return false;
    }
    catch (...) {
        cout << "Unexpected Exception \n" ;
        return false;
    }

    DOMDocument * domDocument = parser->getDocument();
    removeBaseAttr(domDocument);

    // get a serializer, an instance of DOMLSSerializer
    XMLCh tempStr[3] = {chLatin_L, chLatin_S, chNull};
    DOMImplementation *impl          = DOMImplementationRegistry::getDOMImplementation(tempStr);
    DOMLSSerializer   *theSerializer = ((DOMImplementationLS*)impl)->createLSSerializer();
    DOMLSOutput       *theOutputDesc = ((DOMImplementationLS*)impl)->createLSOutput();

    // set user specified output encoding
    theOutputDesc->setEncoding(0);

    XMLDOMErrorReporter * xmlDOMErrorReporter = new XMLDOMErrorReporter();
    DOMConfiguration * serializerConfig = theSerializer->getDomConfig();
    serializerConfig->setParameter(XMLUni::fgDOMErrorHandler, xmlDOMErrorReporter);

    // set feature if the serializer supports the feature/mode
    if (serializerConfig->canSetParameter(XMLUni::fgDOMWRTSplitCdataSections, true))
        serializerConfig->setParameter(XMLUni::fgDOMWRTSplitCdataSections, true);

    if (serializerConfig->canSetParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true))
        serializerConfig->setParameter(XMLUni::fgDOMWRTDiscardDefaultContent, true);

    if (serializerConfig->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, false))
        serializerConfig->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, false);

    if (serializerConfig->canSetParameter(XMLUni::fgDOMWRTBOM, false))
        serializerConfig->setParameter(XMLUni::fgDOMWRTBOM, false);
/*
    XMLFormatTarget * myFormTarget = new StdOutFormatTarget();
    theOutputDesc->setByteStream(myFormTarget);

    theSerializer->write(domDocument, theOutputDesc);
*/
    MemBufFormatTarget * myFormTarget = new MemBufFormatTarget();
    theOutputDesc->setByteStream(myFormTarget);

    theSerializer->write(domDocument, theOutputDesc);
/*
    XMLFormatTarget * myFormTarget2 = new StdOutFormatTarget();
    theOutputDesc->setByteStream(myFormTarget2);
    theSerializer->write(domDocument, theOutputDesc);
*/

    MemBufInputSource * memInput = new MemBufInputSource(myFormTarget->getRawBuffer(), myFormTarget->getLen(), path.c_str());
    XercesDOMParser * parser2 = new XercesDOMParser();
    
    parser2->setDoNamespaces(true);
    parser2->setDoXInclude(true);
  //  parser2->setValidationScheme(XercesDOMParser::Val_Always);
   // parser2->setDoSchema(true);
   // parser2->setValidationSchemaFullChecking(true);

    parser2->setErrorHandler(xmlParseErrorReporter);
    try {
        parser2->parse(*memInput);
    }  catch (const XMLException & toCatch) {
        char * message = XMLString::transcode(toCatch.getMessage());
        cout << "Exception message is: \n" << message << "\n";
        XMLString::release(&message);
        return false;
    }
    catch (const DOMException& toCatch) {
        char * message = XMLString::transcode(toCatch.msg);
        cout << "Exception message is: \n" << message << "\n";
        XMLString::release(&message);
        return false;
    }
    catch (...) {
        cout << "Unexpected Exception \n" ;
        return false;
    }

    DOMDocument * domDocument2 = parser2->getDocument();
    XMLNodeFilter * xmlNodeFilter = new XMLNodeFilter();
    DOMTreeWalker * domTreeWalker = domDocument->createTreeWalker(domDocument2, DOMNodeFilter::SHOW_ALL, xmlNodeFilter, true);

    this->serverConfig = extractServerConfig(domDocument2, domTreeWalker);

    domTreeWalker->release();
    delete xmlNodeFilter;
 

    delete xmlDOMErrorReporter;
    delete xmlParseErrorReporter;
    delete parser;

    XMLPlatformUtils::Terminate();

    return true;
}


}

