/**

Provides the Kinect device. Starts the kinect and sends the joints
to the KinectDevice at the TUIServer.

@author Erik Sniegula, Student Fu Berlin
@date 21.02.2012, 1.00 created (es)

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
//#include "SimpleFilter.h"
//#include "FingerDetection.h"
//#include "FingerDetection.h"

using namespace tuiframework;
using namespace std;
using namespace xn;
//using namespace cv;

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define LENGTHOF(arr)			(sizeof(arr)/sizeof(arr[0]))
#define FOR_ALL(arr, action)	{for(int i = 0; i < LENGTHOF(arr); ++i){action(arr[i])}}

#define ADD_GESTURE(name)		{if(g_GestureGenerator.AddGesture(name, NULL) != XN_STATUS_OK){printf("Unable to add gesture"); CleanupExit();}}
#define REMOVE_GESTURE(name)	{if(g_GestureGenerator.RemoveGesture(name) != XN_STATUS_OK){printf("Unable to remove gesture"); CleanupExit();}}

#define ADD_ALL_GESTURES		FOR_ALL(cGestures, ADD_GESTURE)
#define REMOVE_ALL_GESTURES		FOR_ALL(cGestures, REMOVE_GESTURE)


//---------------------------------------------------------------------------
// Consts
//---------------------------------------------------------------------------
// Gestures to track
static const char			cClickStr[] = "Click";
static const char			cWaveStr[] = "Wave";
static const char* const	cGestures[] = { cClickStr, cWaveStr};
const float c_grabSquareDistance = 0.04f; //distance between left and right hand


//---------------------------------------------------------------------------
// Globals
//---------------------------------------------------------------------------
xn::Context g_Context;
xn::ScriptNode g_scriptNode;
xn::DepthGenerator g_DepthGenerator;
xn::UserGenerator g_UserGenerator;
xn::ImageGenerator g_ImageGenerator;
xn::IRGenerator g_IRGenerator;
xn::GestureGenerator g_GestureGenerator;
xn::HandsGenerator g_HandsGenerator;


XnBool g_bNeedPose = FALSE;
XnChar g_strPose[20] = "";
XnBool g_bDrawBackground = TRUE;
XnBool g_bDrawPixels = TRUE;
XnBool g_bDrawSkeleton = TRUE;
XnBool g_bPrintID = TRUE;
XnBool g_bPrintState = TRUE;

KinectJoint jointData;
KinectJoint jointDataArr[30];

Gesture gestureData;

bool leftHandOverShoulder = false;
bool handsTogether = false;

//SimpleFilter *filter;

int enableOpenGL;

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

/// openni gesture callbacks
void XN_CALLBACK_TYPE Gesture_Process(	xn::GestureGenerator&	generator, const XnChar* strGesture, const XnPoint3D* pPosition, XnFloat	fProgress, void* pCookie);
void XN_CALLBACK_TYPE Gesture_Recognized(	xn::GestureGenerator&	generator, const XnChar* strGesture, const XnPoint3D* pIDPosition, const XnPoint3D*	pEndPosition, void*	pCookie);
void XN_CALLBACK_TYPE Hand_Create(	xn::HandsGenerator& generator, XnUserID	nId, const XnPoint3D* pPosition, XnFloat fTime, void* pCookie);
void XN_CALLBACK_TYPE Hand_Update(	xn::HandsGenerator& generator, XnUserID	nId, const XnPoint3D* pPosition, XnFloat fTime, void* pCookie);
void XN_CALLBACK_TYPE Hand_Destroy(	xn::HandsGenerator& generator, XnUserID	nId, XnFloat fTime, void* pCookie);

void SaveCalibration();
void LoadCalibration();

/// glut and opengl init
void glutDisplay (void);
void glutIdle (void);
void glutKeyboard (unsigned char key, int x, int y);
void glInit (int * pargc, char ** argv);



KinectJoint getJointData( XnUserID user, XnSkeletonJoint eJoint );
void sendJoint(const KinectJoint & jointData);
void sendGesture(const Gesture & gesture);
void gestureLeftHandOverShoulder();
void gestureHandsTogether( XnUserID aUsers[], int minUser);
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
void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	printf("%d New User %d\n", epochTime, nId);
	// New user found
	if (g_bNeedPose)
	{
		g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
	}
	else
	{
		g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
	}
}
// Callback: An existing user was lost
void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	printf("%d Lost user %d\n", epochTime, nId);
}
// Callback: Detected a pose
void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie)
{
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	printf("%d Pose %s detected for user %d\n", epochTime, strPose, nId);
	g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
	g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}
// Callback: Started calibration
void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie)
{
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	printf("%d Calibration started for user %d\n", epochTime, nId);
}
// Callback: Finished calibration
void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& capability, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie)
{
	XnUInt32 epochTime = 0;
	xnOSGetEpochTime(&epochTime);
	if (eStatus == XN_CALIBRATION_STATUS_OK)
	{
		// Calibration succeeded
		printf("%d Calibration complete, start tracking user %d\n", epochTime, nId);
		g_UserGenerator.GetSkeletonCap().StartTracking(nId);
	}
	else
	{
		// Calibration failed
		printf("%d Calibration failed for user %d\n", epochTime, nId);
        if(eStatus==XN_CALIBRATION_STATUS_MANUAL_ABORT)
        {
            printf("Manual abort occured, stop attempting to calibrate!");
            return;
        }
		if (g_bNeedPose)
		{
			g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
		}
		else
		{
			g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
		}
	}
}

//---------------------------------------------------------------------------
// Hooks
//---------------------------------------------------------------------------
void XN_CALLBACK_TYPE Gesture_Process(	xn::GestureGenerator&	generator,
													const XnChar*			strGesture,
													const XnPoint3D*		pPosition,
													XnFloat					fProgress,
													void*					pCookie)	{}

void XN_CALLBACK_TYPE Gesture_Recognized(	xn::GestureGenerator&	generator,
														const XnChar*			strGesture,
														const XnPoint3D*		pIDPosition,
														const XnPoint3D*		pEndPosition,
														void*					pCookie)
{

    if(!strcmp(strGesture, cClickStr)){
        printf("Gesture recognized: %s @ (%f,%f,%f) - EndPosition: (%f,%f,%f) \n", strGesture, pIDPosition->X, pIDPosition->Y, pIDPosition->Z, pEndPosition->X, pEndPosition->Y, pEndPosition->Z);
        gestureData = Gesture(Gesture::Click, Gesture::NoHand, pIDPosition->X, pIDPosition->Y, pIDPosition->Z);
        std::cout << gestureData << std::endl;
        sendGesture(gestureData);
        return;
    }

    if(!strcmp(strGesture, cWaveStr)){
        printf("Gesture recognized: %s @ (%f,%f,%f) - EndPosition: (%f,%f,%f) \n", strGesture, pIDPosition->X, pIDPosition->Y, pIDPosition->Z, pEndPosition->X, pEndPosition->Y, pEndPosition->Z);
        //todo: test position -> left or right hand
        gestureData = Gesture(Gesture::Wave, Gesture::NoHand, pIDPosition->X, pIDPosition->Y, pIDPosition->Z);
        sendGesture(gestureData);
        return;
    }



	printf("Gesture recognized but not handled: %s\n", strGesture);
}

void XN_CALLBACK_TYPE Hand_Create(	xn::HandsGenerator& generator,
												XnUserID			nId,
												const XnPoint3D*	pPosition,
												XnFloat				fTime,
												void*				pCookie)
{
	printf("New Hand: %d @ (%f,%f,%f)\n", nId, pPosition->X, pPosition->Y, pPosition->Z);

}

void XN_CALLBACK_TYPE Hand_Update(	xn::HandsGenerator& generator,
												XnUserID			nId,
												const XnPoint3D*	pPosition,
												XnFloat				fTime,
												void*				pCookie)
{

    std::cout << "hand update" << std::endl;

}

void XN_CALLBACK_TYPE Hand_Destroy(	xn::HandsGenerator& generator,
													XnUserID			nId,
													XnFloat				fTime,
													void*				pCookie)
{
	printf("Lost Hand: %d\n", nId);
}



#define XN_CALIBRATION_FILE_NAME "UserCalibration.bin"

// Save calibration to file
void SaveCalibration()
{
	XnUserID aUserIDs[20] = {0};
	XnUInt16 nUsers = 20;
	g_UserGenerator.GetUsers(aUserIDs, nUsers);
	for (int i = 0; i < nUsers; ++i)
	{
		// Find a user who is already calibrated
		if (g_UserGenerator.GetSkeletonCap().IsCalibrated(aUserIDs[i]))
		{
			// Save user's calibration to file
			g_UserGenerator.GetSkeletonCap().SaveCalibrationDataToFile(aUserIDs[i], XN_CALIBRATION_FILE_NAME);
			break;
		}
	}
}
// Load calibration from file
void LoadCalibration()
{
	XnUserID aUserIDs[20] = {0};
	XnUInt16 nUsers = 20;
	g_UserGenerator.GetUsers(aUserIDs, nUsers);
	for (int i = 0; i < nUsers; ++i)
	{
		// Find a user who isn't calibrated or currently in pose
		if (g_UserGenerator.GetSkeletonCap().IsCalibrated(aUserIDs[i])) continue;
		if (g_UserGenerator.GetSkeletonCap().IsCalibrating(aUserIDs[i])) continue;

		// Load user's calibration from file
		XnStatus rc = g_UserGenerator.GetSkeletonCap().LoadCalibrationDataFromFile(aUserIDs[i], XN_CALIBRATION_FILE_NAME);
		if (rc == XN_STATUS_OK)
		{
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

void initOpenNi(){

		XnStatus nRetVal = XN_STATUS_OK;

		xn::EnumerationErrors errors;
		nRetVal = g_Context.InitFromXmlFile(SAMPLE_XML_PATH, g_scriptNode, &errors);
		if (nRetVal == XN_STATUS_NO_NODE_PRESENT)
		{
			XnChar strError[1024];
			errors.ToString(strError, 1024);
			printf("%s\n", strError);
		}
		else if (nRetVal != XN_STATUS_OK)
		{
			printf("Open failed: %s\n", xnGetStatusString(nRetVal));
		}
		else{
            printf("Kinect Config loaded: %s\n", xnGetStatusString(nRetVal));
        }

		nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
		if (nRetVal != XN_STATUS_OK)
		{
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
		if (nRetVal != XN_STATUS_OK)
		{
			nRetVal = g_UserGenerator.Create(g_Context);
		}

		std::cout << "Usergenerator created: " << xnGetStatusString(nRetVal) << std::endl;

		XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected;
		if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON))
		{
			printf("Supplied user generator doesn't support skeleton\n");
		}
		nRetVal = g_UserGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);
		nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationStart(UserCalibration_CalibrationStart, NULL, hCalibrationStart);
		nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationComplete(UserCalibration_CalibrationComplete, NULL, hCalibrationComplete);


		if (g_UserGenerator.GetSkeletonCap().NeedPoseForCalibration())
		{
			g_bNeedPose = TRUE;
			if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
			{
				printf("Pose required, but not supported\n");
			}
			nRetVal = g_UserGenerator.GetPoseDetectionCap().RegisterToPoseDetected(UserPose_PoseDetected, NULL, hPoseDetected);
			g_UserGenerator.GetSkeletonCap().GetCalibrationPose(g_strPose);
		}

		g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);


        nRetVal = g_GestureGenerator.Create(g_Context);
        if (nRetVal != XN_STATUS_OK)
        {
            printf("Unable to create GestureGenerator.");
        }
        else{
            std::cout << "GestureGenerator create: " << xnGetStatusString(nRetVal) << std::endl;
        }


        nRetVal = g_HandsGenerator.Create(g_Context);
        if (nRetVal != XN_STATUS_OK)
        {
            printf("Unable to create HandsGenerator.");
        }
        else{
            std::cout << "HandsGenerator create: " << xnGetStatusString(nRetVal) << std::endl;
        }

        // Register callbacks
        // Using this as cookie
        XnCallbackHandle	chandle;
        nRetVal = g_GestureGenerator.RegisterGestureCallbacks(Gesture_Recognized, Gesture_Process, NULL, chandle);
        if (nRetVal != XN_STATUS_OK)
        {
            printf("Unable to register gesture callbacks.");
        }
        else{
            std::cout << "GestureGenerator callbacks register: " << xnGetStatusString(nRetVal) << std::endl;
        }

        nRetVal = g_HandsGenerator.RegisterHandCallbacks(Hand_Create, Hand_Update, Hand_Destroy, NULL, chandle);
        if (nRetVal != XN_STATUS_OK)
        {
            printf("Unable to register hand callbacks.");
        }
        else{
            std::cout << "HandsGenerator callbacks register: " << xnGetStatusString(nRetVal) << std::endl;
        }


        //g_UserGenerator.GetSkeletonCap().SetSmoothing(0.90f);

		nRetVal = g_Context.StartGeneratingAll();
		if (nRetVal != XN_STATUS_OK)
        {
            printf("Unable to start Generating.");
        }
        else{
            std::cout << "Start generating: " << xnGetStatusString(nRetVal) << std::endl;
        }

        ADD_ALL_GESTURES;

		std::cout << "OpenNi init successful" << std::endl;

	}


void CleanupExit()
{
    std::cout << "cleaning up" << std::endl;
    g_ImageGenerator.Release();
    g_IRGenerator.Release();
	g_scriptNode.Release();
	g_DepthGenerator.Release();
	g_UserGenerator.Release();
	g_Context.Release();
    std::cout << "cleaning up done" << std::endl;

	exit (1);
}

// this function is called each frame
void glutDisplay (void)
{
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

	if (!g_bPause)
	{
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
void glutIdle (void)
{
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
void glInit (int * pargc, char ** argv)
{
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


/**


*/
KinectJoint getJointData( XnUserID user, XnSkeletonJoint eJoint ) {

	// openNi joint positions
	XnSkeletonJointPosition joint;
	g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition( user, eJoint, joint );


//    if(eJoint == XN_SKEL_RIGHT_HAND){
//        XnSkeletonJointOrientation ori;
//        g_UserGenerator.GetSkeletonCap().GetSkeletonJointOrientation( user, eJoint, ori );
//        std::cout << "orientation: " << ori.orientation.elements[0] << ", " <<
//        ori.orientation.elements[1] << ", " <<
//        ori.orientation.elements[2] << ", " <<
//        ori.orientation.elements[3] << ", " <<
//        ori.orientation.elements[4] << ", " <<
//        ori.orientation.elements[5] << ", " <<
//        ori.orientation.elements[6] << ", " <<
//        ori.orientation.elements[7] << ", " <<
//        ori.orientation.elements[8] << ", " <<   std::endl;
//    }

	XnPoint3D pt[1];
	pt[0] = joint.position;
	double confidence = joint.fConfidence;


	g_DepthGenerator.ConvertRealWorldToProjective( 1, pt, pt );

	//@see: http://graphics.stanford.edu/~mdfisher/Kinect.html
	static const double fx_d = 1.0 / 5.9421434211923247e+02;
	static const double fy_d = 1.0 / 5.9104053696870778e+02;
	static const double cx_d = 3.3930780975300314e+02;
	static const double cy_d = 2.4273913761751615e+02;

	double z = pt[0].Z / 1000;
	double x = ( pt[0].X - cx_d ) * z * fx_d;
	double y = ( pt[0].Y - cy_d ) * z * fy_d * -1;

	KinectJoint myJointData( user, eJoint, x, y, z, confidence );
	return myJointData;
}


void sendJoint(const KinectJoint & jointData){

	///serialize the joint
	stringstream ss;
	ss << jointData;
	string s = ss.str();

////////////////
	cout << s << endl;
////////////////

	char * msg = new char[s.length()+1];
#ifdef _WIN32
	strcpy_s(msg,s.length()+1,s.c_str());
#else
	strcpy(msg,s.c_str());
#endif

	/// msg to send over udp
	HostMsg * hostMsg = new HostMsg(hostadd, msg, s.length()+1);

	/// push the msg, s.t. it can be send by the UDPSenderWinSock2
	hostMsgQ.push(hostMsg);

}

/// send the gesture via UDP to the tui-server
void sendGesture(const Gesture & gesture){
    //std::cout << "sending gesture: " << gesture << std::endl;


	///serialize the gesture
	stringstream ss;
	ss << gesture;
	string s = ss.str();

	//std::cout << gesture << std::endl;

	char * msg = new char[s.length()+1];
#ifdef _WIN32
	strcpy_s(msg,s.length()+1,s.c_str());
#else
	strcpy(msg,s.c_str());
#endif

	/// msg to send over udp
	HostMsg * hostMsg = new HostMsg(hostaddGesture, msg, s.length()+1);

	/// push the msg, s.t. it can be send by the UDPSenderWinSock2
	hostMsgQ.push(hostMsg);

}


/// execution loop: runs the hole time: main logic
void executeInputLoop() {

	//get the new informations and sent it over the udp socket to the tui server
	while (true)
	{
	    /// wait for new kinect data
        g_Context.WaitOneUpdateAll( g_UserGenerator );

        /// max 15 users
		XnUserID aUsers[15];
		XnUInt16 nUsers = 15;
		g_UserGenerator.GetUsers( aUsers, nUsers );

        /// track only the closest person
        //todo: hau das in eine einzelne function!!
		std::vector<XnPoint3D> userCoM(nUsers);
		for ( int i = 0; i < nUsers; ++i ){
            g_UserGenerator.GetCoM(aUsers[i], userCoM[i]);
		}

        //calculate the user with the minimum distance to the kinect
        float minDist = FLT_MAX;
        int minUser = -1;
        float tmp_minDist = 0.0f;

		for ( int i = 0; i < nUsers; ++i ){
		    if(userCoM[i].X + userCoM[i].Y + userCoM[i].Z == 0){
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
        if ( minUser != -1 && g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[minUser]) && aUsers[minUser] )
        {
			//std::cout << "sending user: " << aUsers[minUser] << std::endl;

            /// get the recalculated jointData
            /// and send the kinect joint through the udp socket, if the confidence is high

            /// right elbow
            jointDataArr[XN_SKEL_RIGHT_ELBOW] = getJointData( aUsers[minUser], XN_SKEL_RIGHT_ELBOW );
            if (  jointDataArr[XN_SKEL_RIGHT_ELBOW].getConfidence() > 0/*filter->process( XN_SKEL_RIGHT_ELBOW, jointData.getPosition())*/)
                sendJoint(jointDataArr[XN_SKEL_RIGHT_ELBOW]);

            /// right hand
            jointDataArr[XN_SKEL_RIGHT_HAND] = getJointData( aUsers[minUser], XN_SKEL_RIGHT_HAND );
            if (  jointDataArr[XN_SKEL_RIGHT_HAND].getConfidence() > 0/*filter->process( XN_SKEL_RIGHT_HAND, jointData.getPosition())*/)
                sendJoint(jointDataArr[XN_SKEL_RIGHT_HAND]);

            /// left elbow
            jointDataArr[XN_SKEL_LEFT_ELBOW] = getJointData( aUsers[minUser], XN_SKEL_LEFT_ELBOW );
			if (  jointDataArr[XN_SKEL_LEFT_ELBOW].getConfidence() > 0/*filter->process( XN_SKEL_LEFT_ELBOW, jointData.getPosition())*/)
                sendJoint(jointDataArr[XN_SKEL_LEFT_ELBOW]);

            /// left hand
            jointDataArr[XN_SKEL_LEFT_HAND] = getJointData( aUsers[minUser], XN_SKEL_LEFT_HAND );
           if (  jointDataArr[XN_SKEL_LEFT_HAND].getConfidence() > 0/*filter->process( XN_SKEL_LEFT_HAND, jointData.getPosition())*/)
                sendJoint(jointDataArr[XN_SKEL_LEFT_HAND]);

            /// rest of the skeleton
            /// -----------
            jointDataArr[XN_SKEL_HEAD] = getJointData( aUsers[minUser], XN_SKEL_HEAD );
           if (  jointDataArr[XN_SKEL_HEAD].getConfidence() > 0/*filter->process( XN_SKEL_HEAD, jointData.getPosition())*/)
                sendJoint(jointDataArr[XN_SKEL_HEAD]);

            ///we only need head, hands and elbows till now
//
//            jointDataArr[XN_SKEL_NECK] = getJointData( aUsers[minUser], XN_SKEL_NECK );
//            if (  jointDataArr[XN_SKEL_NECK].getConfidence() > 0/*filter->process( XN_SKEL_NECK, jointData.getPosition())*/)
//                sendJoint(jointDataArr[XN_SKEL_NECK]);
//
//            jointDataArr[XN_SKEL_TORSO] = getJointData( aUsers[minUser], XN_SKEL_TORSO );
//            if (  jointDataArr[XN_SKEL_TORSO].getConfidence() > 0/*filter->process( XN_SKEL_TORSO, jointData.getPosition())*/)
//                sendJoint(jointDataArr[XN_SKEL_TORSO]);
//
//            jointDataArr[XN_SKEL_LEFT_SHOULDER] = getJointData( aUsers[minUser], XN_SKEL_LEFT_SHOULDER );
//            if (  jointDataArr[XN_SKEL_LEFT_SHOULDER].getConfidence() > 0/*filter->process( XN_SKEL_LEFT_SHOULDER, jointData.getPosition())*/)
//                sendJoint(jointDataArr[XN_SKEL_LEFT_SHOULDER]);
//
//            jointDataArr[XN_SKEL_RIGHT_SHOULDER] = getJointData( aUsers[minUser], XN_SKEL_RIGHT_SHOULDER );
//            if (  jointDataArr[XN_SKEL_RIGHT_SHOULDER].getConfidence() > 0/*filter->process( XN_SKEL_RIGHT_SHOULDER, jointData.getPosition())*/)
//                sendJoint(jointDataArr[XN_SKEL_RIGHT_SHOULDER]);
//
//            jointDataArr[XN_SKEL_LEFT_HIP] = getJointData( aUsers[minUser], XN_SKEL_LEFT_HIP );
//            if (  jointDataArr[XN_SKEL_LEFT_HIP].getConfidence() > 0/*filter->process( XN_SKEL_LEFT_HIP, jointData.getPosition())*/)
//                sendJoint(jointDataArr[XN_SKEL_LEFT_HIP]);
//
//            jointDataArr[XN_SKEL_LEFT_KNEE] = getJointData( aUsers[minUser], XN_SKEL_LEFT_KNEE );
//            if (  jointDataArr[XN_SKEL_LEFT_KNEE].getConfidence() > 0/*filter->process( XN_SKEL_LEFT_KNEE, jointData.getPosition())*/)
//                sendJoint(jointDataArr[XN_SKEL_LEFT_KNEE]);
//
//            jointDataArr[XN_SKEL_LEFT_FOOT] = getJointData( aUsers[minUser], XN_SKEL_LEFT_FOOT );
//            if (  jointDataArr[XN_SKEL_LEFT_FOOT].getConfidence() > 0/*filter->process( XN_SKEL_LEFT_FOOT, jointData.getPosition())*/)
//                sendJoint(jointDataArr[XN_SKEL_LEFT_FOOT]);
//
//            jointDataArr[XN_SKEL_RIGHT_HIP] = getJointData( aUsers[minUser], XN_SKEL_RIGHT_HIP );
//            if (  jointDataArr[XN_SKEL_RIGHT_HIP].getConfidence() > 0/*filter->process( XN_SKEL_RIGHT_HIP, jointData.getPosition())*/)
//                sendJoint(jointDataArr[XN_SKEL_RIGHT_HIP]);
//
//            jointDataArr[XN_SKEL_RIGHT_KNEE] = getJointData( aUsers[minUser], XN_SKEL_RIGHT_KNEE );
//            if (  jointDataArr[XN_SKEL_RIGHT_KNEE].getConfidence() > 0/*filter->process( XN_SKEL_RIGHT_KNEE, jointData.getPosition())*/)
//                sendJoint(jointDataArr[XN_SKEL_RIGHT_KNEE]);
//
//            jointDataArr[XN_SKEL_RIGHT_FOOT] = getJointData( aUsers[minUser], XN_SKEL_RIGHT_FOOT );
//            if (  jointDataArr[XN_SKEL_RIGHT_FOOT].getConfidence() > 0/*filter->process( XN_SKEL_RIGHT_FOOT, jointData.getPosition())*/)
//                sendJoint(jointDataArr[XN_SKEL_RIGHT_FOOT]);
//


            /// send the gesture. todo:gesture has no hand and position at the moment

            gestureLeftHandOverShoulder();

            gestureHandsTogether(aUsers, minUser);

        }

		//if we use openGL, than we have to break up after one send
		//s.t. openGL can draw a new scene
		if(enableOpenGL)
			break;
	}
}

//send a HandOverShoulder gesture it the state changes
void gestureLeftHandOverShoulder(){

    if(!leftHandOverShoulder && jointDataArr[XN_SKEL_LEFT_SHOULDER].getPosition().getY() + 0.20< jointDataArr[XN_SKEL_LEFT_HAND].getPosition().getY()){
        Gesture gesture = Gesture(  Gesture::LeftHandOverShoulder,
                                    Gesture::LeftHand,
                                    jointDataArr[XN_SKEL_LEFT_HAND].getPosition().getX(),
                                    jointDataArr[XN_SKEL_LEFT_HAND].getPosition().getY(),
                                    jointDataArr[XN_SKEL_LEFT_HAND].getPosition().getZ());
        sendGesture(gesture);
        leftHandOverShoulder = !leftHandOverShoulder;
        std::cout << "over left shoulder" << std::endl;
    }
    else if (leftHandOverShoulder && jointDataArr[XN_SKEL_LEFT_SHOULDER].getPosition().getY() + 0.20 >= jointDataArr[XN_SKEL_LEFT_HAND].getPosition().getY()){
        Gesture gesture = Gesture(  Gesture::NoLeftHandOverShoulder,
                                    Gesture::LeftHand,
                                    jointDataArr[XN_SKEL_LEFT_HAND].getPosition().getX(),
                                    jointDataArr[XN_SKEL_LEFT_HAND].getPosition().getY(),
                                    jointDataArr[XN_SKEL_LEFT_HAND].getPosition().getZ());
        sendGesture(gesture);
        leftHandOverShoulder = !leftHandOverShoulder;
        std::cout << "no more over left shoulder" << std::endl;
    }
}


void gestureHandsTogether(XnUserID aUsers[], int minUser){
    //Grabbing, wenn die haende nah beieinader sind und kein grabbing mehr, wenn die haende
    //wieder auseinander gehen

    /// calculate square distance between the hands for a gesture
    KinectJoint lHJ = getJointData( aUsers[minUser], XN_SKEL_LEFT_HAND );
    KinectJoint rHJ = getJointData( aUsers[minUser], XN_SKEL_RIGHT_HAND );

    float handDist =    (lHJ.getPosition().getX() - rHJ.getPosition().getX())*(lHJ.getPosition().getX() - rHJ.getPosition().getX()) +
                        (lHJ.getPosition().getY() - rHJ.getPosition().getY())*(lHJ.getPosition().getY() - rHJ.getPosition().getY()) +
                        (lHJ.getPosition().getZ() - rHJ.getPosition().getZ())*(lHJ.getPosition().getZ() - rHJ.getPosition().getZ());

    //std::cout << "hand distance: " << handDist << std::endl;

    if( !handsTogether && handDist <= c_grabSquareDistance){
        Gesture gesture = Gesture(Gesture::Grab, Gesture::BothHands, 0.0f, 0.0f, 0.0f);
        sendGesture(gesture);
        handsTogether = !handsTogether;
        std::cout << "hands together" << std::endl;
    }
    else if (handsTogether && handDist > c_grabSquareDistance){
        Gesture gesture = Gesture(Gesture::NoMoreGrab, Gesture::NoHand, 0.0f, 0.0f, 0.0f);
        sendGesture(gesture);
        handsTogether = !handsTogether;
        std::cout << "hands nomore together" << std::endl;
    }
}

//opencv
//void initKinect(bool initImage, bool initDepth, bool initIR, bool initUser) {
//	XnStatus nRetVal = XN_STATUS_OK;
//
//	// Initialize context object
//	nRetVal = g_Context.Init();
//	cout << "init : " << xnGetStatusString(nRetVal) << endl;
//
//	// default output mode
//	XnMapOutputMode outputMode;
//	outputMode.nXRes = 640;
//	outputMode.nYRes = 480;
//	outputMode.nFPS = 30;
//
//	// Create an ImageGenerator node
//	if (initImage) {
//        nRetVal = g_ImageGenerator.Create(g_Context);
//        cout << "g_ImageGenerator.Create : " << xnGetStatusString(nRetVal) << endl;
//        nRetVal = g_ImageGenerator.SetMapOutputMode(outputMode);
//        cout << "g_ImageGenerator.SetMapOutputMode : " << xnGetStatusString(nRetVal) << endl;
//        nRetVal = g_ImageGenerator.GetMirrorCap().SetMirror(true);
//        cout << "g_ImageGenerator.GetMirrorCap().SetMirror : " << xnGetStatusString(nRetVal) << endl;
//	}
//
//	// Create a DepthGenerator node
//	if (initDepth) {
//		nRetVal = g_DepthGenerator.Create(g_Context);
//		cout << "g_DepthGenerator.Create : " << xnGetStatusString(nRetVal) << endl;
//		nRetVal = g_DepthGenerator.SetMapOutputMode(outputMode);
//		cout << "g_DepthGenerator.SetMapOutputMode : " << xnGetStatusString(nRetVal) << endl;
//		nRetVal = g_DepthGenerator.GetMirrorCap().SetMirror(true);
//		cout << "g_DepthGenerator.GetMirrorCap().SetMirror : " << xnGetStatusString(nRetVal) << endl;
//	}
//
//	// Create an IRGenerator node
//	if (initIR) {
//        nRetVal = g_IRGenerator.Create(g_Context);
//        cout << "g_IRGenerator.Create : " << xnGetStatusString(nRetVal) << endl;
//        nRetVal = g_IRGenerator.SetMapOutputMode(outputMode);
//        cout << "g_IRGenerator.SetMapOutputMode : " << xnGetStatusString(nRetVal) << endl;
//	}
//
//	// create user generator
//	if (initUser) {
//		nRetVal = g_UserGenerator.Create(g_Context);
//		cout << "g_UserGenerator.Create : " << xnGetStatusString(nRetVal) << endl;
//		g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
//	}
//
//
//	// Make it start generating data
//	nRetVal = g_Context.StartGeneratingAll();
//	cout << "g_Context.StartGeneratingAll : " << xnGetStatusString(nRetVal) << endl;
//}


//int FingerTracking()
//{
//    const cv::Size frameSize(640, 480);
//    const unsigned int maxUsers = 20;
//    const cv::Mat emptyMat();
//
//    // frame buffers
//    cv::Mat bgrMat(frameSize, CV_8UC3);
//    cv::Mat depthMat(frameSize, CV_16UC1);
//    cv::Mat depthMat8(frameSize, CV_8UC1);
//    cv::Mat depthMatBgr(frameSize, CV_8UC3);
//
//
//	/// initialize openNi, kinect
//    initKinect(true, true, false, true);
//
//    const float minHandExtension = 0.2f; // in meters
//	const double grabConvexity = 0.8;
//
//	XnUserID userIds[maxUsers] = {0};
//	XnUInt16 nUsers = maxUsers;
//	char key = 0;
//
//	SkeletonCapability skelCap = g_UserGenerator.GetSkeletonCap();
//
//	Mat mask(frameSize, CV_8UC1);
//
//
//	while ( (key = (char) waitKey(1)) != 27 ) {
//	    //std::cout << "waiting" << std::endl;
//		g_Context.WaitAndUpdateAll();
//        //std::cout << "waiting done! " << std::endl;
//		// acquire bgr image
//		{
//			Mat mat(frameSize, CV_8UC3, (unsigned char*) g_ImageGenerator.GetImageMap());
//			cvtColor(mat, bgrMat, CV_RGB2BGR);
//		}
//
//		// acquire depth image
//		{
//			Mat mat(frameSize, CV_16UC1, (unsigned char*) g_DepthGenerator.GetDepthMap());
//			mat.copyTo(depthMat);
//			depthMat.convertTo(depthMat8, CV_8UC1, 255.0f / 3000.0f);
//			cvtColor(depthMat8, depthMatBgr, CV_GRAY2BGR);
//		}
//
//
//		// iterate over all user
//		nUsers = g_UserGenerator.GetNumberOfUsers();
//		g_UserGenerator.GetUsers(userIds, nUsers);
//		float conf; // joint confidence
//		float rh[3]; // right hand coordinates (x[px], y[px], z[meters])
//		float lh[3]; // left hand coordinates
//		float t[3]; // torso coordinates
//
//		for (int i=0; i<nUsers; i++) {
//			int id = userIds[i];
//			std::cout << "user id: " << id << std::endl;
//
//            getJointImgCoordinates(skelCap, id, XN_SKEL_TORSO, t);
//			// torso
//			//if ( getJointImgCoordinates(skelCap, id, XN_SKEL_TORSO, t) == 1 ) {
//			    std::cout << "torso: " << t[0] << ", " << t[1] << ", " << t[2] << std::endl;
//
//				unsigned char shade = 255 - (unsigned char)(t[2] *  128.0f);
//				circle(depthMatBgr, Point(t[0], t[1]), 10, Scalar(shade, 0, 0), -1);
//
//				// right hand
//				if (
//					(getJointImgCoordinates(skelCap, id, XN_SKEL_RIGHT_HAND, rh) == 1) /* confident detection */ &&
//					(rh[2] < t[2] - minHandExtension) /* user extends hand towards screen */ &&
//					(rh[1] < t[1]) /* user raises his hand */
//				) {
//					unsigned char shade = 255 - (unsigned char)(rh[2] *  128.0f);
//					Scalar color(0, 0, shade);
//
//					vector<Point> handContour;
//					getHandContour(depthMat, rh, handContour, frameSize);
//					bool grasp = convexity(handContour) > grabConvexity;
//					int thickness = grasp ? CV_FILLED : 3;
//					circle(depthMatBgr, Point(rh[0], rh[1]), 10, color, thickness);
//
//					vector<Point> fingerTips;
//					detectFingerTips(handContour, fingerTips, &depthMatBgr);
//					std::cout << "r: " << fingerTips << std::endl;
//				}
//
//				// left hand
//				if (
//					(getJointImgCoordinates(skelCap, id, XN_SKEL_LEFT_HAND, lh) == 1) &&
//					(lh[2] < t[2] - minHandExtension) &&
//					(lh[1] < t[1]) /* user raises his hand */
//				) {
//					unsigned char shade = 255 - (unsigned char)(lh[2] *  128.0f);
//					Scalar color(0, shade, 0);
//
//					vector<Point> handContour;
//					getHandContour(depthMat, lh, handContour, frameSize);
//					bool grasp = convexity(handContour) > grabConvexity;
//					int thickness = grasp ? CV_FILLED : 3;
//					circle(depthMatBgr, Point(lh[0], lh[1]), 10, color, thickness);
//
//					vector<Point> fingerTips;
//					detectFingerTips(handContour, fingerTips, &depthMatBgr);
//					std::cout << "l: " << fingerTips << std::endl;
//				}
//
//			//}
//		}
//
//		//imshow("depthMatBgr", depthMatBgr);
//		//std::cout << bgrMat << std::endl;
//		//imshow("bgrMat", bgrMat);
//	}
//
//	return 0;
//
//}


int main(int argc, char **argv)
{

	//// The string must have the format n.n.n.n:p, where n
    /// has the range 0 - 255 and p has the range from 0 to 65535
    if (argc != 5) {
		cout << "Error: Usage <receiver ip>:<receiver port> <0|1> <0|1>." << endl;
		cout << "The string must have the format n.n.n.n:p, where n has the range 0 - 255 and p has the range from 0 to 65535" << endl;
		cout << "The programm is startet with OpenGL, if the second parameter is 1." << endl;
		cout << "The programm is startet with openCV, if the last parameter is 1. - doesnt work yet" << endl;
        return 1;
    }

    if(atoi(argv[4]) == 1){
        std::cout << "starting opencv: finger tracking - doesnt work yet" << std::endl;
        //FingerTracking();
        return 0;
    }

	//filter = new SimpleFilter( 25, 0.005, 0.05 );

	//if third parameter is 0, start without OpenGL
	enableOpenGL = atoi(argv[3]);

	//serveradd, where the kinectDevice-object is located

	hostadd.fromString(argv[1]);
    std::cout << "TUI-Server: KinectDevice ip: " << argv[1]  << std::endl;
    hostaddGesture.fromString(argv[2]);
    std::cout << "TUI-Server: Gesture ip: " << argv[2] << std::endl;


	/// initialize openNi, kinect
	initOpenNi();

	/// initialize udp sender, which sends the serialized joints
	sender.setMyPort(0);
	sender.create();

	if(enableOpenGL){
        std::cout << "starting opengl " << std::endl;
	    glInit(&argc, argv);
		glutMainLoop();
	}
	else{
		/// actual reading and sending of the data
		executeInputLoop();
	}

	/// close all openNi stuff
	CleanupExit();

    return 0;
}
