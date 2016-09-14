// LZ_KinectDriver.cpp : 定义 DLL 应用程序的导出函数。
//

#include "KinectDriver.h"


// 这是导出变量的一个示例
LZ_EXPORTS_API int nLZ_KinectDriver = 0;

// 这是导出函数的一个示例。
LZ_EXPORTS_API int fnLZ_KinectDriver(void)
{
    return 42;
}

// open kinect 
LZ_EXPORTS_API lzBool lzKinectDriverOpenSensor(void)
{



	return true;
}

// close kinect
LZ_EXPORTS_API lzBool lzKinectDriverCloseSensor(void)
{

	return true;
}

// 
LZ_EXPORTS_API lzBool lzKinectDriverSetParameter(IN Kinect_Driver_Para_Type* para)
{

	return true;
}

// 
LZ_EXPORTS_API lzBool lzKinectDriverGetParameter(OUT Kinect_Driver_Para_Type* para)
{

	return true;
}

//
LZ_EXPORTS_API lzBool lzKInectDriverUpdateFrame()
{

	return true;
}

//
LZ_EXPORTS_API lzBool lzKInectDriverAcquireColorFrame(OUT Kinect_Driver_Color_Frame_Type* colorFrame)
{

	return true;
}

//
LZ_EXPORTS_API lzBool lzKInectDriverAcquireDepthFrame(OUT Kinect_Driver_Depth_Frame_Type* depthFrame)
{

	return true;
}

//
LZ_EXPORTS_API lzBool lzKInectDriverAcquireBodyIndexFrame(OUT Kinect_Driver_BodyIndex_Frame_Type* bodyIndexFrame)
{

	return true;
}

//
LZ_EXPORTS_API lzBool lzKinectDriverAcquireFrame(OUT Kinect_Driver_Frame_Type* frame)
{

	return true;
}

// 
LZ_EXPORTS_API lzBool lzKinectDriverMapDepthFrameToCameraSpace(
	IN lzInt32 depthPointCount,
	IN Kinect_Driver_Depth_Frame_Type* depthFrame,
	IN lzInt32 cameraPointCount,
	OUT CameraSpacePoint* cameraSpacePoints)
{

	return true;
}

// 
LZ_EXPORTS_API lzBool lzKinectDriverMapDepthFrameToColorSpace(
	IN lzInt32 depthPointCount,
	IN Kinect_Driver_Depth_Frame_Type* depthFrame,
	IN lzInt32 colorPointCount,
	OUT ColorSpacePoint* colorSpacePoints)
{

	return true;
}

// 
LZ_EXPORTS_API lzBool lzKinectDriverMapColorFrameToCameraSpace(
	IN lzInt32 colorPointCount,
	IN Kinect_Driver_Color_Frame_Type* colorFrame,
	IN lzInt32 cameraPointCount,
	OUT CameraSpacePoint* cameraSpacePoints)
{

	return true;
}

// 
LZ_EXPORTS_API lzBool lzKinectDriverMapColorFrameToDepthSpace(
	IN lzInt32 colorPointCount,
	IN Kinect_Driver_Color_Frame_Type* colorFrame,
	IN lzInt32 depthPointCount,
	OUT DepthSpacePoint* depthSpacePoints)
{

	return true;
}

//
LZ_EXPORTS_API lzBool lzKinectDriverAcquireMeshCount(OUT lzInt32* count)
{

	return true;
}

//
LZ_EXPORTS_API lzBool lzKinectDriverAcquireModel(OUT Kinect_Driver_Mesh_Type* mesh)
{

	return true;
}

