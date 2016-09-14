#ifndef __LZ_KINECT_DRIVER_H__
#define __LZ_KINECT_DRIVER_H__

#include "Version.h"
#include "lzTypes.h"
#include "Kinect.h"

// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� LZ_KINECTDRIVER_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// LZ_KINECTDRIVER_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

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

