/**

Provides the Kinect device. Starts the kinect and sends the joints
to the KinectDevice at the TUIServer.

@author Erik Sniegula, Oliver Belaifa
@date 21.02.2012, 1.00 created (es)
@date 27.10.2013 updated (obelaifa)
*/

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

#include <iostream>
#include <cstdlib> // GCC 4.3 related build problem
#include <ctime>
#include <sstream>
#include <cstring>
#include <cfloat>
#include <vector>

#include <tuilibs/tuitypes/common/CommonTypeReg.h>
#include <tuilibs/tuitypes/common/SkeletonJointID.h>

#ifdef _WIN32
	#include <tuiframework/sockets/UDPSenderWinSock2.h>
#else
	#include <tuiframework/sockets/UDPSenderSocket.h>
#endif

#include <tuiframework/core/HostMsgQueue.h>
#include <tuiframework/core/HostMsg.h>
#include <string>

#define GLUT_DISABLE_ATEXIT_HACK
#include <glut.h>

#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>
#include <XnPropNames.h>

#include "SceneDrawer.h"

using namespace tuiframework;
using namespace std;
using namespace xn;

static xn::Context g_Context;
static xn::ScriptNode g_scriptNode;
xn::DepthGenerator g_DepthGenerator;
xn::UserGenerator g_UserGenerator;

static XnBool g_bNeedPose = FALSE;
static XnChar g_strPose[20] = "";
XnBool g_bDrawBackground = TRUE;
XnBool g_bDrawPixels = TRUE;
XnBool g_bDrawSkeleton = TRUE;
XnBool g_bPrintID = TRUE;
XnBool g_bPrintState = TRUE;

KinectJoint jointData;
KinectJoint jointDataArr[30];

static int enableOpenGL;

//message-queue receveies kinectJoint pushes, which will be send over
//the udp socket
HostMsgQueue hostMsgQ;
// represents the udp socket
#ifdef _WIN32
	UDPSenderWinSock2 sender(hostMsgQ);
#else
	UDPSenderSocket sender(hostMsgQ);
#endif

// represents the address of the joints receiver
HostAddress hostadd;
// represents the address of the gesture receiver @see tuidevices/kinect/KinectDevice.h
HostAddress hostaddGesture;

/// clean all openNI stuff after use
void CleanupExit();

/// openni user callbacks
void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie);
void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie);
void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie);
void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie);
void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& capability, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie);

void SaveCalibration();
void LoadCalibration();

/// glut and opengl init
void glutDisplay (void);
void glutIdle (void);
void glutKeyboard (unsigned char key, int x, int y);
void glInit (int * pargc, char ** argv);

KinectJoint getJointData( XnUserID user, XnSkeletonJoint eJoint );
void sendJoint(const KinectJoint & jointData);

/// main loop
void executeInputLoop();

#define GL_WIN_SIZE_X 720
#define GL_WIN_SIZE_Y 480

XnBool g_bPause = false;
XnBool g_bRecord = false;
XnBool g_bQuit = false;

//---------------------------------------------------------------------------
// Code: OpenNi
//---------------------------------------------------------------------------


/*

	INIT OPEN_NI CALLBACKS for
	- new user
	- existing user lost
	- detected a pose
	- started calibration
	- finished calibration

*/


// Callback: New user was detected
void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie) {
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	printf("%d New User %d\n", epochTime, nId);
	// New user found
	if (g_bNeedPose) {
		g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
	} else {
		g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
	}
}

// Callback: An existing user was lost
void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie) {
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	printf("%d Lost user %d\n", epochTime, nId);
}

// Callback: Detected a pose
void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie) {
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	printf("%d Pose %s detected for user %d\n", epochTime, strPose, nId);
	g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
	g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}

// Callback: Started calibration
void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie) {
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	printf("%d Calibration started for user %d\n", epochTime, nId);
}

// Callback: Finished calibration
void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& capability, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie){
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	if (eStatus == XN_CALIBRATION_STATUS_OK) {
		// Calibration succeeded
		printf("%d Calibration complete, start tracking user %d\n", epochTime, nId);
		g_UserGenerator.GetSkeletonCap().StartTracking(nId);
	} else {
		// Calibration failed
		printf("%d Calibration failed for user %d\n", epochTime, nId);
        if(eStatus==XN_CALIBRATION_STATUS_MANUAL_ABORT) {
            printf("Manual abort occured, stop attempting to calibrate!");
            return;
        }
		if (g_bNeedPose) {
			g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
		} else {
			g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
		}
	}
}



#define XN_CALIBRATION_FILE_NAME "UserCalibration.bin"

// Save calibration to file
void SaveCalibration() {
	XnUserID aUserIDs[20] = {0};
	XnUInt16 nUsers = 20;
	g_UserGenerator.GetUsers(aUserIDs, nUsers);
	for (int i = 0; i < nUsers; ++i) {
		// Find a user who is already calibrated
		if (g_UserGenerator.GetSkeletonCap().IsCalibrated(aUserIDs[i])) {
			// Save user's calibration to file
			g_UserGenerator.GetSkeletonCap().SaveCalibrationDataToFile(aUserIDs[i], XN_CALIBRATION_FILE_NAME);
			break;
		}
	}
}

// Load calibration from file
void LoadCalibration() {
	XnUserID aUserIDs[20] = {0};
	XnUInt16 nUsers = 20;
	g_UserGenerator.GetUsers(aUserIDs, nUsers);
	for (int i = 0; i < nUsers; ++i) {
		// Find a user who isn't calibrated or currently in pose
		if (g_UserGenerator.GetSkeletonCap().IsCalibrated(aUserIDs[i])) continue;
		if (g_UserGenerator.GetSkeletonCap().IsCalibrating(aUserIDs[i])) continue;

		// Load user's calibration from file
		XnStatus rc = g_UserGenerator.GetSkeletonCap().LoadCalibrationDataFromFile(aUserIDs[i], XN_CALIBRATION_FILE_NAME);
		if (rc == XN_STATUS_OK) {
			// Make sure state is coherent
			g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(aUserIDs[i]);
			g_UserGenerator.GetSkeletonCap().StartTracking(aUserIDs[i]);
		}
		break;
	}
}

#ifdef _WIN32
	#define SAMPLE_XML_PATH "KinectConfig.xml"
#else
	#define SAMPLE_XML_PATH "/home/erik/Sniegula/tuiframework/trunk/KinectDevice/KinectConfig.xml"
#endif

void initOpenNi() {
	XnStatus nRetVal = XN_STATUS_OK;

	xn::EnumerationErrors errors;
	nRetVal = g_Context.InitFromXmlFile(SAMPLE_XML_PATH, g_scriptNode, &errors);
	if (nRetVal == XN_STATUS_NO_NODE_PRESENT) {
		XnChar strError[1024];
		errors.ToString(strError, 1024);
		printf("%s\n", strError);
	} else if (nRetVal != XN_STATUS_OK) {
		printf("Open failed: %s\n", xnGetStatusString(nRetVal));
	} else {
        printf("Kinect Config loaded: %s\n", xnGetStatusString(nRetVal));
    }

	nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
	if (nRetVal != XN_STATUS_OK) {
		printf("No depth generator found. Using a default one...");
		xn::MockDepthGenerator mockDepth;
		nRetVal = mockDepth.Create(g_Context);

		// set some defaults
		XnMapOutputMode defaultMode;
		//resolution
		defaultMode.nXRes = 640;
		defaultMode.nYRes = 480;
		//fps
		defaultMode.nFPS = 30;
		nRetVal = mockDepth.SetMapOutputMode(defaultMode);

		// set FOV
		XnFieldOfView fov;
		fov.fHFOV = 1.0225999419141749;
		fov.fVFOV = 0.79661567681716894;
		nRetVal = mockDepth.SetGeneralProperty(XN_PROP_FIELD_OF_VIEW, sizeof(fov), &fov);

		XnUInt32 nDataSize = defaultMode.nXRes * defaultMode.nYRes * sizeof(XnDepthPixel);
		XnDepthPixel* pData = (XnDepthPixel*)xnOSCallocAligned(nDataSize, 1, XN_DEFAULT_MEM_ALIGN);

		nRetVal = mockDepth.SetData(1, 0, nDataSize, pData);

		g_DepthGenerator = mockDepth;
	}

	nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_USER, g_UserGenerator);
	if (nRetVal != XN_STATUS_OK) {
		nRetVal = g_UserGenerator.Create(g_Context);
	}

	std::cout << "Usergenerator created: " << xnGetStatusString(nRetVal) << std::endl;

	XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected;
	if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON)) {
		printf("Supplied user generator doesn't support skeleton\n");
	}
	nRetVal = g_UserGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);
	nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationStart(UserCalibration_CalibrationStart, NULL, hCalibrationStart);
	nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationComplete(UserCalibration_CalibrationComplete, NULL, hCalibrationComplete);


	if (g_UserGenerator.GetSkeletonCap().NeedPoseForCalibration()) {
		g_bNeedPose = TRUE;
		if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION)) {
			printf("Pose required, but not supported\n");
		}
		nRetVal = g_UserGenerator.GetPoseDetectionCap().RegisterToPoseDetected(UserPose_PoseDetected, NULL, hPoseDetected);
		g_UserGenerator.GetSkeletonCap().GetCalibrationPose(g_strPose);
	}

	g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

    //g_UserGenerator.GetSkeletonCap().SetSmoothing(0.90f);

	nRetVal = g_Context.StartGeneratingAll();
	if (nRetVal != XN_STATUS_OK) {
        printf("Unable to start Generating.");
    } else {
        std::cout << "Start generating: " << xnGetStatusString(nRetVal) << std::endl;
    }
	std::cout << "OpenNi init successful" << std::endl;
}


void CleanupExit() {
	g_scriptNode.Release();
	g_DepthGenerator.Release();
	g_UserGenerator.Release();
	g_Context.Release();
	exit(1);
}

// this function is called each frame
void glutDisplay(void) {
    //send stuff twice every frame
    executeInputLoop();
    executeInputLoop();

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup the OpenGL viewpoint
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	xn::SceneMetaData sceneMD;
	xn::DepthMetaData depthMD;

	g_DepthGenerator.GetMetaData(depthMD);

	glOrtho(0, depthMD.XRes(), depthMD.YRes(), 0, -1.0, 1.0);
	glDisable(GL_TEXTURE_2D);

	if (!g_bPause) {
		// Read next available data
		g_Context.WaitOneUpdateAll(g_UserGenerator);
	}

	// Process the data
	g_DepthGenerator.GetMetaData(depthMD);
	g_UserGenerator.GetUserPixels(0, sceneMD);

	//draw the suff with openGL
	DrawDepthMap(depthMD, sceneMD);
	glutSwapBuffers();
}

//idle event
void glutIdle(void) {
	if (g_bQuit) {
		CleanupExit();
	}
	// Display the frame
	glutPostRedisplay();
}

//kewboard events
void glutKeyboard (unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		CleanupExit();
	case 'b':
		// Draw background?
		g_bDrawBackground = !g_bDrawBackground;
		break;
	case 'x':
		// Draw pixels at all?
		g_bDrawPixels = !g_bDrawPixels;
		break;
	case 's':
		// Draw Skeleton?
		g_bDrawSkeleton = !g_bDrawSkeleton;
		break;
	case 'i':
		// Print label?
		g_bPrintID = !g_bPrintID;
		break;
	case 'l':
		// Print ID & state as label, or only ID?
		g_bPrintState = !g_bPrintState;
		break;
	case'p':
		g_bPause = !g_bPause;
		break;
	case 'S':
		SaveCalibration();
		break;
	case 'L':
		LoadCalibration();
		break;
	}
}


void glInit (int * pargc, char ** argv) {
	glutInit(pargc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(GL_WIN_SIZE_X, GL_WIN_SIZE_Y);
	glutCreateWindow ("Kinect User Tracker");
	//glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);

	glutKeyboardFunc(glutKeyboard);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutIdle);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}


void addJoint(XnUserID userID, XnSkeletonJoint eJoint, int dstJointID, PackedType<Vector3<double> > & packedVector3) {
	XnSkeletonJointPosition joint;
	g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(userID, eJoint, joint);
	
	if (joint.fConfidence == 0) {
		return;
	}

	XnPoint3D pt;
	pt = joint.position;
	g_DepthGenerator.ConvertRealWorldToProjective(1, &pt, &pt);

		//@see: http://graphics.stanford.edu/~mdfisher/Kinect.html
	static const double fx_d = 1.0/5.9421434211923247e+02;
	static const double fy_d = 1.0/5.9104053696870778e+02;
	static const double cx_d = 3.3930780975300314e+02;
	static const double cy_d = 2.4273913761751615e+02;

	double z = pt.Z/1000;
	double x = (pt.X - cx_d)*z*fx_d;
	double y = (pt.Y - cy_d)*z*fy_d*-1;

	vector<pair<int, Vector3<double> > > & v = packedVector3.getItems();
	v.push_back(pair<int, Vector3<double> >(dstJointID, Vector3<double>(x, y, z)));
}


void sendPackedVector3(const PackedType<Vector3<double> > & packedVector3) {
	stringstream ss;
	ss << packedVector3;
	string str = ss.str();

	char * msg = new char[str.length() + 1];
#ifdef _WIN32
	strcpy_s(msg, str.length() + 1, str.c_str());
#else
	strcpy(msg ,str.c_str());
#endif

	HostMsg * hostMsg = new HostMsg(hostadd, msg, str.length() + 1);
	hostMsgQ.push(hostMsg);
}

/// execution loop: runs the hole time: main logic
void executeInputLoop() {
	PackedType<Vector3<double> > packedVector3;
	//get the new informations and sent it over the udp socket to the tui server
	while (true) {
	    /// wait for new kinect data
        g_Context.WaitOneUpdateAll(g_UserGenerator);

        /// max 15 users
		XnUserID aUsers[15];
		XnUInt16 nUsers = 15;
		g_UserGenerator.GetUsers(aUsers, nUsers);

        /// track only the closest person
        //todo: hau das in eine einzelne function!!
		std::vector<XnPoint3D> userCoM(nUsers);
		for (int i = 0; i < nUsers; ++i) {
            g_UserGenerator.GetCoM(aUsers[i], userCoM[i]);
		}

        //calculate the user with the minimum distance to the kinect
        float minDist = FLT_MAX;
        int minUser = -1;
        float tmp_minDist = 0.0f;

		for (int i = 0; i < nUsers; ++i){
		    if (userCoM[i].X + userCoM[i].Y + userCoM[i].Z == 0) {
                continue;
		    }
            tmp_minDist = userCoM[i].X*userCoM[i].X + userCoM[i].Y*userCoM[i].Y + userCoM[i].Z*userCoM[i].Z;
		    if(tmp_minDist < minDist && tmp_minDist > 0.0){
                minDist = tmp_minDist;
                minUser = i;
		    }
		    //std::cout << "user: " << i << tmp_minDist << std::endl;
		}

        /// send the joints if a user is tracked
        if (minUser != -1 && g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[minUser]) && aUsers[minUser]) {
			//std::cout << "sending user: " << aUsers[minUser] << std::endl;
			vector<pair<int, Vector3<double> > > & v = packedVector3.getItems();
			v.clear();

			addJoint(aUsers[minUser], XN_SKEL_HEAD           , SKEL_HEAD           , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_NECK           , SKEL_NECK           , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_TORSO          , SKEL_TORSO          , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_WAIST          , SKEL_WAIST          , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_LEFT_COLLAR    , SKEL_LEFT_COLLAR    , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_LEFT_SHOULDER  , SKEL_LEFT_SHOULDER  , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_LEFT_ELBOW     , SKEL_LEFT_ELBOW     , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_LEFT_WRIST     , SKEL_LEFT_WRIST     , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_LEFT_HAND      , SKEL_LEFT_HAND      , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_LEFT_FINGERTIP , SKEL_LEFT_FINGERTIP , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_RIGHT_COLLAR   , SKEL_RIGHT_COLLAR   , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_RIGHT_SHOULDER , SKEL_RIGHT_SHOULDER , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_RIGHT_ELBOW    , SKEL_RIGHT_ELBOW    , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_RIGHT_WRIST    , SKEL_RIGHT_WRIST    , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_RIGHT_HAND     , SKEL_RIGHT_HAND     , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_RIGHT_FINGERTIP, SKEL_RIGHT_FINGERTIP, packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_LEFT_HIP       , SKEL_LEFT_HIP       , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_LEFT_KNEE      , SKEL_LEFT_KNEE      , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_LEFT_ANKLE     , SKEL_LEFT_ANKLE     , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_LEFT_FOOT      , SKEL_LEFT_FOOT      , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_RIGHT_HIP      , SKEL_RIGHT_HIP      , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_RIGHT_KNEE     , SKEL_RIGHT_KNEE     , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_RIGHT_ANKLE    , SKEL_RIGHT_ANKLE    , packedVector3);
			addJoint(aUsers[minUser], XN_SKEL_RIGHT_FOOT     , SKEL_RIGHT_FOOT     , packedVector3);

			sendPackedVector3(packedVector3);
        }

		//if we use openGL, than we have to break up after one send
		//s.t. openGL can draw a new scene
		if(enableOpenGL) {
			break;
		} else {
			Sleep(10);
		}
	}
}


int main(int argc, char **argv) {
	//// The string must have the format n.n.n.n:p, where n
    /// has the range 0 - 255 and p has the range from 0 to 65535
    if (argc != 3) {
		cout << "Error: Usage <receiver ip>:<receiver port> <0|1> <0|1>." << endl;
		cout << "The string must have the format n.n.n.n:p, where n has the range 0 - 255 and p has the range from 0 to 65535" << endl;
		cout << "The programm is startet with OpenGL, if the second parameter is 1." << endl;
        return 1;
    }

	//serveradd, where the kinectDevice-object is located
	hostadd.fromString(argv[1]);
    std::cout << "TUIDevice - MotionCaptureStub IPAddress: " << argv[1]  << std::endl;

	enableOpenGL = atoi(argv[2]);

	/// initialize openNi, kinect
	initOpenNi();

	/// initialize udp sender, which sends the serialized joints
	sender.setMyPort(0);
	sender.create();

	if(enableOpenGL) {
        std::cout << "starting opengl " << std::endl;
	    glInit(&argc, argv);
		glutMainLoop();
	} else {
		/// actual reading and sending of the data
		executeInputLoop();
	}

	/// close all openNi stuff
	CleanupExit();

    return 0;
}
