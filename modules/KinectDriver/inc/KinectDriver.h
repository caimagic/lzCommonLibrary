#ifndef __LZ_KINECT_DRIVER_H__
#define __LZ_KINECT_DRIVER_H__

#include "Version.h"
#include "lzTypes.h"
#include "Kinect.h"

#define KINECT_DRIVER_DEPTH_WIDTH	512
#define KINECT_DRIVER_DEPTH_HEIGHT	424
#define KINECT_DRIVER_COLOR_WIDTH	1920
#define KINECT_DRIVER_COLOR_HEIGHT	1080

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
	Kinect_Driver_Mesh_Model  bMeshModel;
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
	lzInt32 frame[KINECT_DRIVER_DEPTH_WIDTH * KINECT_DRIVER_DEPTH_HEIGHT];
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
} Kinect_Driver_Mesh_Type;

LZ_EXPORTS_API int fnLZ_KinectDriver(void);

// open kinect 
LZ_EXPORTS_API lzBool lzKinectDriverOpenSensor(void);

// close kinect
LZ_EXPORTS_API lzBool lzKinectDriverCloseSensor(void);

// 
LZ_EXPORTS_API lzBool lzKinectDriverSetParameter(IN Kinect_Driver_Para_Type* para);

// 
LZ_EXPORTS_API lzBool lzKinectDriverGetParameter(OUT Kinect_Driver_Para_Type* para);

//
LZ_EXPORTS_API lzBool lzKInectDriverUpdateFrame();

//
LZ_EXPORTS_API lzBool lzKInectDriverAcquireColorFrame(OUT Kinect_Driver_Color_Frame_Type* colorFrame);

//
LZ_EXPORTS_API lzBool lzKInectDriverAcquireDepthFrame(OUT Kinect_Driver_Depth_Frame_Type* depthFrame);

//
LZ_EXPORTS_API lzBool lzKInectDriverAcquireBodyIndexFrame(OUT Kinect_Driver_BodyIndex_Frame_Type* bodyIndexFrame);

//
LZ_EXPORTS_API lzBool lzKinectDriverAcquireFrame(OUT Kinect_Driver_Frame_Type* frame);

// 
LZ_EXPORTS_API lzBool lzKinectDriverMapDepthFrameToCameraSpace(
	IN lzInt32 depthPointCount, 
	IN Kinect_Driver_Depth_Frame_Type* depthFrame,
	IN lzInt32 cameraPointCount,
	OUT CameraSpacePoint* cameraSpacePoints);

// 
LZ_EXPORTS_API lzBool lzKinectDriverMapDepthFrameToColorSpace(
	IN lzInt32 depthPointCount,
	IN Kinect_Driver_Depth_Frame_Type* depthFrame,
	IN lzInt32 colorPointCount,
	OUT ColorSpacePoint* colorSpacePoints);

// 
LZ_EXPORTS_API lzBool lzKinectDriverMapColorFrameToCameraSpace(
	IN lzInt32 colorPointCount,
	IN Kinect_Driver_Color_Frame_Type* colorFrame,
	IN lzInt32 cameraPointCount,
	OUT CameraSpacePoint* cameraSpacePoints);

// 
LZ_EXPORTS_API lzBool lzKinectDriverMapColorFrameToDepthSpace(
	IN lzInt32 colorPointCount,
	IN Kinect_Driver_Color_Frame_Type* colorFrame,
	IN lzInt32 depthPointCount,
	OUT DepthSpacePoint* depthSpacePoints);

//
LZ_EXPORTS_API lzBool lzKinectDriverAcquireMeshCount(OUT lzInt32* count);

//
LZ_EXPORTS_API lzBool lzKinectDriverAcquireModel(OUT Kinect_Driver_Mesh_Type* mesh);

#endif

