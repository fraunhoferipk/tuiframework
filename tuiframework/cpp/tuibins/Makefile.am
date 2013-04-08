bin_PROGRAMS =

bin_PROGRAMS += testtuiclient1

testtuiclient1_CXXFLAGS = -fPIC -rdynamic -I../tuilibs -I$(INCDIR) -ggdb -g -O3

testtuiclient1_LDADD = -lCommonTUITypes -lTUIFrameworkClient -lTUIFrameworkCore -lTUIFrameworkXML -lpthread
testtuiclient1_LDFLAGS = -L../tuiframework -L../tuilibs

testtuiclient1_SOURCES = \
    tuiclient1/tuiclient.cpp \
    tuiclient1/Init.cpp \
    tuiclient1/MyTUIObject.cpp


#################################

bin_PROGRAMS += testtuiclient2

testtuiclient2_CXXFLAGS = -fPIC -rdynamic -I../tuilibs -I$(INCDIR) -ggdb -g -O3

testtuiclient2_LDADD = -lCommonTUITypes -lTUIFrameworkClient -lTUIFrameworkCore -lpthread
testtuiclient2_LDFLAGS = -L../tuiframework -L../tuilibs

testtuiclient2_SOURCES = \
    tuiclient2/tuiclient.cpp \
    tuiclient2/Init.cpp \
    tuiclient2/MyTUIObject.cpp


#################################

bin_PROGRAMS += testtuimouseclient

testtuimouseclient_CXXFLAGS = -fPIC -rdynamic -I../tuilibs -I$(INCDIR) -ggdb -g -O3

testtuimouseclient_LDADD = -lCommonTUITypes -lTUIFrameworkClient -lTUIFrameworkCore -lpthread
testuimouseclient_LDFLAGS = -L../tuiframework -L../tuilibs

testtuimouseclient_SOURCES = \
    tuimouseclient/tuimouseclient.cpp \
    tuimouseclient/Init.cpp \
    tuimouseclient/TUIMouse.cpp


#################################

bin_PROGRAMS += testtuiserver1

testtuiserver1_CXXFLAGS = -fPIC -rdynamic -I../tuilibs -I$(INCDIR) -ggdb -g -O3

testtuiserver1_LDADD =  -lDummyTUIDevice \
    -lKinectTUIDevice -lDTrackTUIDevice -lM4TransfTUIMSP -lKinectTransfTUIMSP \
    -lCommonTUITypes -lTUIFrameworkServer -lTUIFrameworkCore -lTUIFrameworkXML -lpthread -lxerces-c 
testtuiserver1_LDFLAGS = -L../tuiframework -L../tuilibs

testtuiserver1_SOURCES = \
    tuiserver1/tuiserver.cpp

testtuiserver1xml_DATA = \
    tuiserver1/testconfigcomplete.xml \
    tuiserver1/testconfigpart1.xml \
    tuiserver1/testconfigpart2.xml

testtuiserver1xmldir = $(datadir)/@PACKAGE@


################################

bin_PROGRAMS += testtuiserver2

testtuiserver2_CXXFLAGS = -fPIC -ggdb -g -O3 -rdynamic

testtuiserver2_LDADD = -lTUIFrameworkServer -lTUIFrameworkCore -lTUIFrameworkXML -lpthread -lxerces-c $(LIBDL)
testtuiserver2_LDFLAGS = -L../tuiframework -L../tuilibs

testtuiserver2_SOURCES = \
    tuiserver2/tuiserver.cpp

testtuiserver2xml_DATA = \
    tuiserver2/mousetestconfig.xml \
    tuiserver2/testconfig.xml


testtuiserver2xmldir = $(datadir)/@PACKAGE@

