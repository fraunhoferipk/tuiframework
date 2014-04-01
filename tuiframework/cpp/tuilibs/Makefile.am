lib_LIBRARIES =
lib_LTLIBRARIES =

lib_LTLIBRARIES += libCommonTUITypes.la
libCommonTUITypes_la_CXXFLAGS = -fPIC -rdynamic -c -I$(INCDIR) -ggdb -g -O3
libCommonTUITypes_la_LIBADD = -lTUIFrameworkCore
libCommonTUITypes_la_LDFLAGS = -L../tuiframework -version-info 1:1:0
libCommonTUITypes_ladir = $(includedir)/tuilibs

libCommonTUITypes_la_SOURCES = \
    tuitypes/common/CommonTUITypesPlugin.cpp \
    tuitypes/common/CommonTypeReg.cpp \
    tuitypes/common/Gesture.cpp \
    tuitypes/common/HapticAngle.cpp \
    tuitypes/common/KinectJoint.cpp \
    tuitypes/common/Matrix4Data.cpp \
    tuitypes/common/MouseData.cpp \
    tuitypes/common/TrackerData.cpp \
    tuitypes/common/Vector3d.cpp \
    tuitypes/common/VectorList3d.cpp

nobase_libCommonTUITypes_la_HEADERS = \
    tuitypes/common/CommonTUITypesPlugin.h \
    tuitypes/common/CommonTypeReg.h \
    tuitypes/common/Gesture.h \
    tuitypes/common/HapticAngle.h \
    tuitypes/common/KinectJoint.h \
    tuitypes/common/Matrix4Data.h \
    tuitypes/common/Matrix4.h \
    tuitypes/common/MouseData.h \
    tuitypes/common/PackedType.h \
    tuitypes/common/SkeletonJointID.h \
    tuitypes/common/TrackerData.h \
    tuitypes/common/Vector4.h \
    tuitypes/common/Vector3d.h \
    tuitypes/common/VectorList3d.h

################################

lib_LTLIBRARIES += libDummyTUIDevice.la
libDummyTUIDevice_la_CXXFLAGS = -fPIC -rdynamic -c -I$(INCDIR) -ggdb -g -O3
libDummyTUIDevice_la_LIBADD = -lCommonTUITypes -lTUIFrameworkServer -lTUIFrameworkCore
libDummyTUIDevice_la_LDFLAGS = -L../tuiframework -version-info 1:1:0

libDummyTUIDevice_ladir = ${includedir}/tuilibs

libDummyTUIDevice_la_SOURCES = \
    dummy/DummyDevDA.cpp \
    dummy/DummyDevMatrix4.cpp \
    dummy/DummyDevPackedVector4.cpp \
    dummy/DummyDevPlugin.cpp \
    dummy/DummyDevTracker.cpp

nobase_libDummyTUIDevice_la_HEADERS = \
    dummy/DummyDevDA.h \
    dummy/DummyDevMatrix4.h \
    dummy/DummyDevPackedVector4.h \
    dummy/DummyDevPlugin.h \
    dummy/DummyDevTracker.h

libDummyTUIDevicexml_DATA = \
    dummy/DummyDeviceDA.xml \
    dummy/DummyDevMatrix4.xml \
    dummy/DummyDevPackedVector4.xml \
    dummy/DummyDeviceTracker.xml

libDummyTUIDevicexmldir = $(datadir)/@PACKAGE@

################################

lib_LTLIBRARIES += libDTrackTUIDevice.la
libDTrackTUIDevice_la_CXXFLAGS = -fPIC -rdynamic -c -I$(INCDIR) -ggdb -g -O3
libDTrackTUIDevice_la_LIBADD = -lCommonTUITypes -lTUIFrameworkServer -lTUIFrameworkCore
libDTrackTUIDevice_la_LDFLAGS = -L../tuiframework -version-info 1:1:0

libDTrackTUIDevice_ladir = ${includedir}/tuilibs

libDTrackTUIDevice_la_SOURCES = \
    dtrack/DTrack2.cpp \
    dtrack/DTrackDev.cpp \
    dtrack/DTrackDevPlugin.cpp

nobase_libDTrackTUIDevice_la_HEADERS = \
    dtrack/DTrack2.h \
    dtrack/DTrackDev.h \
    dtrack/DTrackDevPlugin.h

libDTrackTUIDevicexml_DATA = \
    dtrack/DTrackDevice.xml

libDTrackTUIDevicexmldir = $(datadir)/@PACKAGE@


################################

lib_LTLIBRARIES += libKinectTUIDevice.la
libKinectTUIDevice_la_CXXFLAGS = -fPIC -rdynamic -c -I$(INCDIR) -ggdb -g -O3
libKinectTUIDevice_la_LIBADD = -lCommonTUITypes -lTUIFrameworkServer -lTUIFrameworkCore
libKinectTUIDevice_la_LDFLAGS = -L../tuiframework -version-info 1:1:0

libKinectTUIDevice_ladir = ${includedir}/tuilibs

libKinectTUIDevice_la_SOURCES = \
    kinect/KinectDev.cpp \
    kinect/SimpleFilter.cpp \
    kinect/KinectDevPlugin.cpp

nobase_libKinectTUIDevice_la_HEADERS = \
    kinect/KinectDev.h \
    kinect/KinectDevPlugin.h \
    kinect/SimpleFilter.h

libKinectTUIDevicexml_DATA = \
    kinect/KinectDevice.xml

libKinectTUIDevicexmldir = $(datadir)/@PACKAGE@

################################    

if PHIDGET21_AVAILABLE
lib_LTLIBRARIES += libPhidgetTUIDevice.la
libPhidgetTUIDevice_la_CXXFLAGS = -fPIC -rdynamic -c -I$(INCDIR) -ggdb -g -O3
libPhidgetTUIDevice_la_LIBADD = -lCommonTUITypes -lTUIFrameworkServer -lTUIFrameworkCore -lphidget21
libPhidgetTUIDevice_la_LDFLAGS = -L../tuiframework -version-info 1:1:0

libPhidgetTUIDevice_ladir = ${includedir}/tuilibs

libPhidgetTUIDevice_la_SOURCES = \
    phidgets/PhidgetsDev.cpp \
    phidgets/PhidgetsDevPlugin.cpp

nobase_libPhidgetTUIDevice_la_HEADERS = \
    phidgets/PhidgetsDev.h \
    phidgets/PhidgetsDevPlugin.h

libPhidgetTUIDevicexml_DATA = \
    phidgets/PhidgetDevice.xml

libPhidgetTUIDevicexmldir = $(datadir)/@PACKAGE@

endif

################################

lib_LTLIBRARIES += libVector4DepackMSP.la
libVector4DepackMSP_la_CXXFLAGS = -fPIC -rdynamic -c -I$(INCDIR) -ggdb -g -O3
libVector4DepackMSP_la_LIBADD = -lCommonTUITypes -lTUIFrameworkServer -lTUIFrameworkCore
libVector4DepackMSP_la_LDFLAGS = -L../tuiframework -version-info 1:1:0

libVector4DepackMSP_ladir = ${includedir}/tuilibs

libVector4DepackMSP_la_SOURCES = \
    vector4depack/Vector4SkeletonDepackMSP.cpp \
    vector4depack/Vector4DepackPlugin.cpp

nobase_libVector4DepackMSP_la_HEADERS = \
    vector4depack/Vector4SkeletonDepackMSP.h \
    vector4depack/Vector4DepackPlugin.h

libVector4DepackMSPxml_DATA = \
    vector4depack/Vector4SkeletonDepack.xml

libVector4DepackMSPxmldir = $(datadir)/@PACKAGE@

################################

lib_LTLIBRARIES += libMatrix4OpMSP.la
libMatrix4OpMSP_la_CXXFLAGS = -fPIC -rdynamic -c -I$(INCDIR) -ggdb -g -O3
libMatrix4OpMSP_la_LIBADD = -lCommonTUITypes -lTUIFrameworkServer -lTUIFrameworkCore
libMatrix4OpMSP_la_LDFLAGS = -L../tuiframework -version-info 1:1:0

libMatrix4OpMSP_ladir = ${includedir}/tuilibs

libMatrix4OpMSP_la_SOURCES = \
    matrix4op/Matrix4DuplicateMSP.cpp \
    matrix4op/Matrix4GetTranslationMSP.cpp \
    matrix4op/Matrix4InverseMSP.cpp \
    matrix4op/Matrix4MultiplicationMSP.cpp \
    matrix4op/Matrix4OpPlugin.cpp \
    matrix4op/Matrix4PackedDuplicateMSP.cpp \
    matrix4op/Matrix4SetTranslationMSP.cpp \
    matrix4op/Matrix4TimesMSP.cpp \
    matrix4op/Vector4DuplicateMSP.cpp \
    matrix4op/Vector4Matrix4MultiplicationMSP.cpp \
    matrix4op/Vector4PackedDuplicateMSP.cpp \
    matrix4op/Vector4TimesMSP.cpp
    
nobase_libMatrix4OpMSP_la_HEADERS = \
    matrix4op/Matrix4DuplicateMSP.h \
    matrix4op/Matrix4GetTranslationMSP.h \
    matrix4op/Matrix4InverseMSP.h \
    matrix4op/Matrix4MultiplicationMSP.h \
    matrix4op/Matrix4OpPlugin.h \
    matrix4op/Matrix4PackedDuplicateMSP.h \
    matrix4op/Matrix4SetTranslationMSP.h \
    matrix4op/Matrix4TimesMSP.h \
    matrix4op/Vector4DuplicateMSP.h \
    matrix4op/Vector4Matrix4MultiplicationMSP.h \
    matrix4op/Vector4PackedDuplicateMSP.h \
    matrix4op/Vector4TimesMSP.h

libMatrix4OpMSPxml_DATA = \
    matrix4op/Matrix4Duplicate.xml \
    matrix4op/Matrix4GetTranslation.xml \
    matrix4op/Matrix4Inverse.xml \
    matrix4op/Matrix4Multiplication.xml \
    matrix4op/Matrix4PackedDuplicate.xml \
    matrix4op/Matrix4SetTranslation.xml \
    matrix4op/Matrix4Times.xml \
    matrix4op/Vector4Duplicate.xml \
    matrix4op/Vector4Matrix4Multiplication.xml \
    matrix4op/Vector4PackedDuplicate.xml \
    matrix4op/Vector4Times.xml

libMatrix4OpMSPxmldir = $(datadir)/@PACKAGE@

################################

lib_LTLIBRARIES += libKinectTransfTUIMSP.la
libKinectTransfTUIMSP_la_CXXFLAGS = -fPIC -rdynamic -c -I$(INCDIR) -ggdb -g -O3
libKinectTransfTUIMSP_la_LIBADD = -lCommonTUITypes -lTUIFrameworkServer -lTUIFrameworkCore
libKinectTransfTUIMSP_la_LDFLAGS = -L../tuiframework -version-info 1:1:0

libKinectTransfTUIMSP_ladir = ${includedir}/tuilibs

libKinectTransfTUIMSP_la_SOURCES = \
    kinecttransf/KinectARTTransfUnit.cpp \
    kinecttransf/MSPKinectARTTransformation.cpp \
    kinecttransf/KinectTransfPlugin.cpp

nobase_libKinectTransfTUIMSP_la_HEADERS = \
    kinecttransf/KinectARTTransfUnit.h \
    kinecttransf/MSPKinectARTTransformation.h \
    kinecttransf/KinectTransfPlugin.h

################################

lib_LTLIBRARIES += libHapticTUIDevice.la
libHapticTUIDevice_la_CXXFLAGS = -fPIC -rdynamic -c -I$(INCDIR) -ggdb -g -O3
libHapticTUIDevice_la_LIBADD = -lCommonTUITypes -lTUIFrameworkServer -lTUIFrameworkCore
libHapticTUIDevice_la_LDFLAGS = -L../tuiframework -version-info 1:1:0

libHapticTUIDevice_ladir = ${includedir}/tuilibs

libHapticTUIDevice_la_SOURCES = \
    haptic/HapticDev.cpp

nobase_libHapticTUIDevice_la_HEADERS = \
    haptic/HapticDev.h

libHapticTUIDevicexml_DATA = \
    haptic/HapticDevice.xml

libHapticTUIDevicexmldir = $(datadir)/@PACKAGE@

################################

lib_LTLIBRARIES += libRFCommTUIDevice.la
libRFCommTUIDevice_la_CXXFLAGS = -fPIC -rdynamic -c -I$(INCDIR) -ggdb -g -O3
libRFCommTUIDevice_la_LIBADD = -lCommonTUITypes -lTUIFrameworkServer -lTUIFrameworkCore
libRFCommTUIDevice_la_LDFLAGS = -L../tuiframework -version-info 1:1:0

libRFCommTUIDevice_ladir = ${includedir}/tuilibs

libRFCommTUIDevice_la_SOURCES = \
    rfcomm/RFCommDev.cpp \
    rfcomm/TTYChannel.cpp

nobase_libRFCommTUIDevice_la_HEADERS = \
    rfcomm/RFCommDev.h \
    rfcomm/TTYChannel.h

libRFCommTUIDevicexml_DATA = \
    rfcomm/RFCommDevice.xml

libRFCommTUIDevicexmldir = $(datadir)/@PACKAGE@

################################

lib_LTLIBRARIES += libMouseTUIDevice.la
libMouseTUIDevice_la_CXXFLAGS = -fPIC -rdynamic -c -I$(INCDIR) -ggdb -g -O3
libMouseTUIDevice_la_LIBADD = -lCommonTUITypes -lTUIFrameworkServer -lTUIFrameworkCore
libMouseTUIDevice_la_LDFLAGS = -L../tuiframework -version-info 1:1:0

libMouseTUIDevice_ladir = ${includedir}/tuilibs

libMouseTUIDevice_la_SOURCES = \
    mouse/MouseDev.cpp \
    mouse/MouseDevPlugin.cpp

nobase_libMouseTUIDevice_la_HEADERS = \
    mouse/MouseDev.h \
    mouse/MouseDevPlugin.h

libMouseTUIDevicexml_DATA = \
    mouse/MouseDev.xml

libMouseTUIDevicexmldir = $(datadir)/@PACKAGE@

################################

lib_LTLIBRARIES += libMotionCaptureStubTUIDevice.la
libMotionCaptureStubTUIDevice_la_CXXFLAGS = -fPIC -rdynamic -c -I$(INCDIR) -ggdb -g -O3
libMotionCaptureStubTUIDevice_la_LIBADD = -lCommonTUITypes -lTUIFrameworkServer -lTUIFrameworkCore
libMotionCaptureStubTUIDevice_la_LDFLAGS = -L../tuiframework -version-info 1:1:0

libMotionCaptureStubTUIDevice_ladir = ${includedir}/tuilibs

libMotionCaptureStubTUIDevice_la_SOURCES = \
    motioncapture/MotionCaptureStubDev.cpp \
    motioncapture/MotionCaptureStubDevPlugin.cpp

nobase_libMotionCaptureStubTUIDevice_la_HEADERS = \
    motioncapture/MotionCaptureStubDev.h \
    motioncapture/MotionCaptureStubDevPlugin.h

libMotionCaptureStubTUIDevicexml_DATA = \
    motioncapture/MotionCaptureStub.xml

libMotionCaptureStubTUIDevicexmldir = $(datadir)/@PACKAGE@

################################

lib_LTLIBRARIES += libMouseDemuxTUIMSP.la
libMouseDemuxTUIMSP_la_CXXFLAGS = -fPIC -rdynamic -c -I$(INCDIR) -ggdb -g -O3
libMouseDemuxTUIMSP_la_LIBADD = -lCommonTUITypes -lTUIFrameworkServer -lTUIFrameworkCore
libMouseDemuxTUIMSP_la_LDFLAGS = -L../tuiframework -version-info 1:1:0

libMouseDemuxTUIMSP_ladir = ${includedir}/tuilibs

libMouseDemuxTUIMSP_la_SOURCES = \
    mousedemux/MouseDemuxMSP.cpp \
    mousedemux/MouseDemuxPlugin.cpp 

nobase_libMouseDemuxTUIMSP_la_HEADERS = \
    mousedemux/MouseDemuxMSP.h \
    mousedemux/MouseDemuxPlugin.h

libMouseDemuxTUIMSPxml_DATA = \
    mousedemux/MouseDemuxMSP.xml

libMouseDemuxTUIMSPxmldir = $(datadir)/@PACKAGE@

################################

lib_LTLIBRARIES += libTypeConverterMSP.la
libTypeConverterMSP_la_CXXFLAGS = -fPIC -rdynamic -c -I$(INCDIR) -ggdb -g -O3
libTypeConverterMSP_la_LIBADD = -lCommonTUITypes -lTUIFrameworkServer -lTUIFrameworkCore
libTypeConverterMSP_la_LDFLAGS = -L../tuiframework -version-info 1:1:0

libTypeConverterMSP_ladir = ${includedir}/tuilibs

libTypeConverterMSP_la_SOURCES = \
    typeconverter/AnalogToDigitalMSP.cpp \
    typeconverter/DigitalToAnalogMSP.cpp \
    typeconverter/TrackerToMatrix4MSP.cpp \
    typeconverter/TypeConverterPlugin.cpp 

nobase_libTypeConverterMSP_la_HEADERS = \
    typeconverter/AnalogToDigitalMSP.h \
    typeconverter/DigitalToAnalogMSP.h \
    typeconverter/TrackerToMatrix4MSP.h \
    typeconverter/TypeConverterPlugin.h 

libTypeConverterMSPxml_DATA = \
    typeconverter/TypeConverterMSP.xml

libTypeConverterMSPxmldir = $(datadir)/@PACKAGE@

################################

lib_LTLIBRARIES += libVRPNTUIDevice.la
libVRPNTUIDevice_la_CXXFLAGS = -fPIC -rdynamic -c -I$(INCDIR) -ggdb -g -O3
libVRPNTUIDevice_la_LIBADD = -lCommonTUITypes -lTUIFrameworkServer -lTUIFrameworkCore -lvrpn
libVRPNTUIDevice_la_LDFLAGS = -L../tuiframework -version-info 1:1:0

libVRPNTUIDevice_ladir = ${includedir}/tuilibs

libVRPNTUIDevice_la_SOURCES = \
    vrpn/VRPNTrackerDev.cpp \
    vrpn/VRPNDevPlugin.cpp

nobase_libVRPNTUIDevice_la_HEADERS = \
    vrpn/VRPNTrackerDev.h \
    vrpn/VRPNDevPlugin.h

libVRPNTUIDevicexml_DATA = \
    vrpn/VRPNTrackerDev.xml

libVRPNTUIDevicexmldir = $(datadir)/@PACKAGE@

################################

lib_LTLIBRARIES += libMatrix4CalibrationDev.la
libMatrix4CalibrationDev_la_CXXFLAGS = -fPIC -rdynamic -c -I$(INCDIR) -ggdb -g -O3
libMatrix4CalibrationDev_la_LIBADD = -lCommonTUITypes -lTUIFrameworkServer -lTUIFrameworkCore
libMatrix4CalibrationDev_la_LDFLAGS = -L../tuiframework -version-info 1:1:0

libMatrix4CalibrationDev_ladir = ${includedir}/tuilibs

libMatrix4CalibrationDev_la_SOURCES = \
    matrix4calibration/Matrix4CalibrationDev.cpp \
    matrix4calibration/Matrix4CalibrationPlugin.cpp

nobase_libMatrix4CalibrationDev_la_HEADERS = \
    matrix4calibration/Matrix4CalibrationDev.h \
    matrix4calibration/Matrix4CalibrationPlugin.h

libMatrix4CalibrationDevxml_DATA = \
    matrix4calibration/Matrix4Calibration.xml 

libMatrix4CalibrationDevxmldir = $(datadir)/@PACKAGE@

