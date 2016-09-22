 /**
 *
 * Copyright (C), 2016 Lang Qi (shanghai) Internet Tech. Co., Ltd.
 *
 * \brief          KinectDriver.h c++ file
 * \file         : KinectDriver.h
 * \version      : ALPHA
 * \author       : caimagic
 * \date         : 2016/9/16 1:21:47
 * \copyright    : Lang Qi (shanghai) Internet Tech. Co., Ltd.
 * \warning      : 
 *
 * Email         : caizhijie_hit@163.com
 * ClassList     :
 * History       : change history log, including date, author and brief content statement.
 *
 */
 
#ifndef __LZ_KINECT_DRIVER_H__
#define __LZ_KINECT_DRIVER_H__

/********************************************************************************************/
/*			1 Other Header File Including													*/
/********************************************************************************************/
#include "Version.h"
#include "lzTypes.h"
#include "Kinect.h"
#include "ICapture.h"
#include "MMAPFile.h"

/********************************************************************************************/
/*			2 Macro Definition																*/
/********************************************************************************************/
#define KINECT_DRIVER_DEPTH_WIDTH	512
#define KINECT_DRIVER_DEPTH_HEIGHT	424
#define KINECT_DRIVER_COLOR_WIDTH	1920
#define KINECT_DRIVER_COLOR_HEIGHT	1080

/********************************************************************************************/
/*			3 Enum Type Definition															*/
/********************************************************************************************/


/********************************************************************************************/
/*			4 Global Variable Declaring														*/
/********************************************************************************************/


/********************************************************************************************/
/*			5 Struct Type Definition														*/
/********************************************************************************************/

typedef enum
{
	NONE,
	PointCloud,
	MeshSet,
	ModelCount
} Kinect_Driver_Mesh_Model;

typedef struct
{
	lzInt32 depthWidth;
	lzInt32 depthHeight;
	lzInt32 colorWidth;
	lzInt32 colorHeight;
	Kinect_Driver_Mesh_Model  enMeshModel;
} Kinect_Driver_Para_Type;

typedef struct
{
	lzRGB   frame[KINECT_DRIVER_COLOR_WIDTH * KINECT_DRIVER_COLOR_HEIGHT];
} Kinect_Driver_Color_Frame_Type;

typedef struct
{
	lzInt32 frame[KINECT_DRIVER_DEPTH_WIDTH * KINECT_DRIVER_DEPTH_HEIGHT];
} Kinect_Driver_Depth_Frame_Type;

typedef struct
{
	lzUInt8 frame[KINECT_DRIVER_DEPTH_WIDTH * KINECT_DRIVER_DEPTH_HEIGHT];
} Kinect_Driver_BodyIndex_Frame_Type;

typedef struct
{
	Kinect_Driver_Color_Frame_Type color;
	Kinect_Driver_Depth_Frame_Type depth;
	Kinect_Driver_BodyIndex_Frame_Type bodyIndex;
} Kinect_Driver_Frame_Type;

typedef struct
{
	lzPoint3f p1;
	lzPoint3f p2;
	lzPoint3f p3;
	lzPoint2f uv1;
	lzPoint2f uv2;
	lzPoint2f uv3;
	lzUInt32 index;
} Kinect_Driver_Mesh_Type;

/********************************************************************************************/
/*			6 Class Type Definition															*/
/********************************************************************************************/
class LZ_EXPORTS_API CKinectDriver
{
public:
	CKinectDriver();
	~CKinectDriver();

	lzBool OpenSensor(void);
	lzBool UpdateFrame(void);
	lzBool SetParameter(IN Kinect_Driver_Para_Type* para);
	lzBool GetParameter(OUT Kinect_Driver_Para_Type* para);
	lzInt32 CreateModule(void);

	ICapture* pCapture;

	lzRGBX* pColor;
	lzInt32* pDepth;
	lzUInt8* pBodyIndex;
	lzInt32 colorHeight;
	lzInt32 colorWidth;
	lzInt32 depthHeight;
	lzInt32 depthWidth;
	Kinect_Driver_Mesh_Model mode;
	std::vector<lzMeshUnitType> vecMeshUnit;
	
private:

};

/********************************************************************************************/
/*			7 Others Definition																*/
/********************************************************************************************/


/********************************************************************************************/
/*			8 Function Declare																*/
/********************************************************************************************/

LZ_EXPORTS_C int lzHello(void);

LZ_EXPORTS_C lzBool lzKinectDriverOpenLog(lzBool isopen = false);

// open kinect 
LZ_EXPORTS_C lzBool lzKinectDriverOpenSensor(void);

// close kinect
LZ_EXPORTS_C lzBool lzKinectDriverCloseSensor(void);

// 
LZ_EXPORTS_C lzBool lzKinectDriverSetParameter(IN Kinect_Driver_Para_Type* para);

// 
LZ_EXPORTS_C lzBool lzKinectDriverGetParameter(OUT Kinect_Driver_Para_Type* para);

//
LZ_EXPORTS_C lzBool lzKInectDriverUpdateFrame();

//
LZ_EXPORTS_C lzBool lzKInectDriverAcquireColorFrame(OUT Kinect_Driver_Color_Frame_Type* colorFrame);

//
LZ_EXPORTS_C lzBool lzKInectDriverAcquireDepthFrame(OUT Kinect_Driver_Depth_Frame_Type* depthFrame);

//
LZ_EXPORTS_C lzBool lzKInectDriverAcquireBodyIndexFrame(OUT Kinect_Driver_BodyIndex_Frame_Type* bodyIndexFrame);

//
LZ_EXPORTS_C lzBool lzKinectDriverAcquireFrame(OUT Kinect_Driver_Frame_Type* frame);

// 
LZ_EXPORTS_C lzBool lzKinectDriverMapDepthFrameToCameraSpace(
	IN lzInt32 depthPointCount,
	IN Kinect_Driver_Depth_Frame_Type* depthFrame,
	IN lzInt32 cameraPointCount,
	OUT CameraSpacePoint* cameraSpacePoints);

// 
LZ_EXPORTS_C lzBool lzKinectDriverMapDepthFrameToColorSpace(
	IN lzInt32 depthPointCount,
	IN Kinect_Driver_Depth_Frame_Type* depthFrame,
	IN lzInt32 colorPointCount,
	OUT ColorSpacePoint* colorSpacePoints);

// 
LZ_EXPORTS_C lzBool lzKinectDriverMapColorFrameToCameraSpace(
	IN lzInt32 colorPointCount,
	IN Kinect_Driver_Color_Frame_Type* colorFrame,
	IN lzInt32 cameraPointCount,
	OUT CameraSpacePoint* cameraSpacePoints);

// 
LZ_EXPORTS_C lzBool lzKinectDriverMapColorFrameToDepthSpace(
	IN lzInt32 colorPointCount,
	IN Kinect_Driver_Color_Frame_Type* colorFrame,
	IN lzInt32 depthPointCount,
	OUT DepthSpacePoint* depthSpacePoints);

//
LZ_EXPORTS_C lzInt32 lzKinectDriverAcquireMeshCount(OUT lzInt32* count);

//
LZ_EXPORTS_C lzBool lzKinectDriverAcquireModel(OUT Kinect_Driver_Mesh_Type* mesh);

//
LZ_EXPORTS_C lzInt32 lzKinectDriverReadMMAPFileCount(lzUInt32 framecount);

//
LZ_EXPORTS_C lzBool lzKinectDriverReadMMAP(Kinect_Driver_Mesh_Type mesh[], lzInt32 count);

#endif // __LZ_KINECT_DRIVER_H__ 
