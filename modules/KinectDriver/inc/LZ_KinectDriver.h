#ifndef __LZ_KINECT_DRIVER_H__
#define __LZ_KINECT_DRIVER_H__

#include "Version.h"
#include "lzTypes.h"
#include "Kinect.h"

// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 LZ_KINECTDRIVER_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// LZ_KINECTDRIVER_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#ifdef LZ_EXPORTS
#define LZ_EXPORTS_API extern "C" __declspec(dllexport)
#else
#define LZ_EXPORTS_API extern "C" __declspec(dllimport)
#endif

#define KINECT_DRIVER_DEPTH_WIDTH	512
#define KINECT_DRIVER_DEPTH_HEIGHT	424
#define KINECT_DRIVER_COLOR_WIDTH	1920
#define KINECT_DRIVER_COLOR_HEIGHT	1080

typedef struct
{
	lzInt32 depthWidth;
	lzInt32 depthHeight;
	lzInt32 colorWidth;
	lzInt32 colorHeight;
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

LZ_EXPORTS_API int fnLZ_KinectDriver(void);

// open kinect 
LZ_EXPORTS_API bool lzKinectDriverOpenSensor(void);

// close kinect
LZ_EXPORTS_API bool lzKinectDriverCloseSensor(void);

// 
LZ_EXPORTS_API bool lzKinectDriverGetParameter(Kinect_Driver_Para_Type* data);

//
LZ_EXPORTS_API bool lzKInectDriverUpdateFrame();

//
LZ_EXPORTS_API bool lzKInectDriverAcquireColorFrame();

//
LZ_EXPORTS_API bool lzKinectDriverAcquireFrame(Kinect_Driver_Frame_Type* data);

// 
LZ_EXPORTS_API bool lzKinectDriverMapDepthFrameToCameraSpace(
	int depthPointCount, void* depthFrameData, int cameraPointCount, void* cameraSpacePoints);


#endif

