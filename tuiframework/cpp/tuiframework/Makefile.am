lib_LIBRARIES = 
lib_LIBRARIES += libTUIFrameworkCore.a 
lib_LIBRARIES += libTUIFrameworkXML.a
lib_LIBRARIES += libTUIFrameworkServer.a
lib_LIBRARIES += libTUIFrameworkClient.a

libTUIFrameworkXML_a_CXXFLAGS = -fPIC -c -I$(INCDIR) -ggdb -g -O3
libTUIFrameworkXML_a_CXXFLAGS += $(XERCES_CPPFLAGS)

libTUIFrameworkXML_adir = ${includedir}/tuiframework

libTUIFrameworkXML_a_SOURCES = \
    xml/ServerConfigXMLReader.cpp \
    xml/XMLDOMErrorReporter.cpp \
    xml/XMLNodeFilter.cpp \
    xml/XMLParseErrorReporter.cpp

nobase_libTUIFrameworkXML_a_HEADERS = \
    xml/ServerConfigXMLReader.h \
    xml/XMLDOMErrorReporter.h \
    xml/XMLNodeFilter.h \
    xml/XMLParseErrorReporter.h


libTUIFrameworkServer_a_CXXFLAGS = -fPIC -c -I$(INCDIR) -ggdb -g -O3

libTUIFrameworkServer_adir = ${includedir}/tuiframework

libTUIFrameworkServer_a_SOURCES = \
    server/Connector.cpp \
    server/DeviceConfig.cpp \
    server/DeviceContainer.cpp \
    server/DeviceDescriptor.cpp \
    server/DeviceFactory.cpp \
    server/DeviceFactorySingleton.cpp \
    server/DeviceInstance.cpp \
    server/DeviceType.cpp \
    server/EPAddressTranslator.cpp \
    server/MSPBase.cpp \
    server/MSPConfig.cpp \
    server/MSPContainer.cpp \
    server/MSPFactory.cpp \
    server/MSPFactorySingleton.cpp \
    server/MSPInstance.cpp \
    server/MSPType.cpp \
    server/ParameterGroup.cpp \
    server/PluginLib.cpp \
    server/PluginShelf.cpp \
    server/PluginShelfSingleton.cpp \
    server/PortAddress.cpp \
    server/ServerConfig.cpp \
    server/ServerStartupConfig.cpp \
    server/TUIServerApp.cpp
        
nobase_libTUIFrameworkServer_a_HEADERS = \
    server/Connector.h \
    server/DeviceConfig.h \
    server/DeviceContainer.h \
    server/DeviceDescriptor.h \
    server/DeviceFactory.h \
    server/DeviceFactorySingleton.h \
    server/DeviceInstance.h \
    server/DeviceType.h \
    server/EPAddressTranslator.h \
    server/IDeviceFactory.h \
    server/IMSPFactory.h \
    server/MSPBase.h \
    server/MSPConfig.h \
    server/MSPContainer.h \
    server/MSPFactory.h \
    server/MSPFactorySingleton.h \
    server/MSPInstance.h \
    server/MSPType.h \
    server/ParameterGroup.h \
    server/PluginLib.h \
    server/PluginShelf.h \
    server/PluginShelfSingleton.h \
    server/PortAddress.h \
    server/ServerConfig.h \
    server/ServerStartupConfig.h \
    server/TUIServerApp.h
    
    
libTUIFrameworkClient_a_CXXFLAGS = -fPIC -c -I$(INCDIR) -ggdb -g -O3

libTUIFrameworkClient_adir = ${includedir}/tuiframework

libTUIFrameworkClient_a_SOURCES = \
    client/TUIClientApp.cpp \
    client/TUIClientAppProvider.cpp \
    client/TUIEventHandler.cpp \
    client/TUIEventHandlerSingleton.cpp \
    client/TUIObjectStub.cpp \
    client/TUIObjectStubContainer.cpp

nobase_libTUIFrameworkClient_a_HEADERS = \
    client/client.h \
    client/TUIClientApp.h \
    client/TUIClientAppProvider.h \
    client/TUIEventHandler.h \
    client/TUIEventHandlerSingleton.h \
    client/TUIObjectStub.h \
    client/TUIObjectStubContainer.h 

libTUIFrameworkCore_a_CXXFLAGS = -fPIC -c -I$(INCDIR) -ggdb -g -O3

libTUIFrameworkCore_a_CXXFLAGS += -DUSE_TFDEBUG
	
libTUIFrameworkCore_adir = ${includedir}/tuiframework

libTUIFrameworkCore_a_SOURCES = \
    core/AttachedObjects.cpp \
    core/EPAddress.cpp \
    core/EventChannelFactory.cpp \
    core/EventDeserializer.cpp \
    core/EventFactory.cpp \
    core/EventSerializer.cpp \
    core/EventTerminator.cpp \
    core/Exception.cpp \
    core/GUIDEventTypeIDVector.cpp \
    core/HostAddress.cpp \
    core/HostMsg.cpp \
    core/HostMsgDispatcher.cpp \
    core/ISerializable.cpp \
    core/Port.cpp \
    core/SystemCmd.cpp \
    core/TUIObjectDescriptor.cpp \
    core/TUIObjectInstance.cpp \
    core/TUIObjectType.cpp \
    core/TypeRegistration.cpp \
    sockets/MulticastListener.cpp \
    sockets/UDPReceiverSocket.cpp \
    sockets/UDPSenderSocket.cpp \
    logging/Logger.cpp \
    logging/SStreamSingleton.cpp

nobase_libTUIFrameworkCore_a_HEADERS = ${core_headers} $(sockets_headers) $(client_headers) $(logging_headers)

core_headers = \
    core/AttachedObjects.h \
    core/EPAddress.h \
    core/EPEventMsg.h \
    core/Event.h \
    core/EventChannel.h \
    core/EventChannelFactory.h \
    core/EventDelegate.h \
    core/EventDeserializer.h \
    core/EventFactory.h \
    core/EventMsg.h \
    core/EventMsgRouter.h \
    core/EventQueue.h \
    core/EventRouter.h \
    core/EventSerializer.h \
    core/EventTerminator.h \
    core/Exception.h \
    core/FactoryDS.h \
    core/Factory.h \
    core/GUIDEventTypeIDVector.h \
    core/HostAddress.h \
    core/HostEventMsg.h \
    core/HostMsg.h \
    core/HostMsgDispatcher.h \
    core/HostMsgQueue.h \
    core/IDevice.h \
    core/IEvent.h \
    core/IEventChannel.h \
    core/IEventChannelFactory.h \
    core/IEventFactory.h \
    core/IEventMsg.h \
    core/IEventSink.h \
    core/IEventSource.h \
    core/IHostMsgSink.h \
    core/IHostMsgSource.h \
    core/IMSP.h \
    core/ISerializable.h \
    core/ISerializedDataSink.h \
    core/ISerializedDataSource.h \
    core/IThreadMessageSink.h \
    core/ITUIServer.h \
    core/Multiton.h \
    core/Port.h \
    core/Queue.h \
    core/SerializedDataQueue.h \
    core/Singleton.h \
    core/SystemCmd.h \
    core/TUIObjectDescriptor.h \
    core/TUIObjectInstance.h \
    core/TUIObjectType.h \
    core/TypeRegistration.h \
    core/Version.h


sockets_headers = \
    sockets/MulticastListener.h \
    sockets/UDPReceiverSocket.h \
    sockets/UDPSenderSocket.h
	

logging_headers = \
    logging/Logger.h \
    logging/SStreamSingleton.h 

