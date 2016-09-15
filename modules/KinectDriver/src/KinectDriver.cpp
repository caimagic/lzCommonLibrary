// LZ_KinectDriver.cpp : 定义 DLL 应用程序的导出函数。
//
#include <memory>
#include "KinectDriver.h"
#include "kinectCapture.h"
#include "g3log/g3log.hpp"
#include "g3log/logworker.hpp"
#include <io.h>
#include "direct.h"

std::unique_ptr<CKinectDriver> kinectDriver;
std::unique_ptr<g3::LogWorker> worker;
std::unique_ptr<g3::FileSinkHandle> handle;

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
const std::string path_to_log_file = ".//log//";
#else
const std::string path_to_log_file = "/tmp/";
#endif

CKinectDriver::CKinectDriver()
{
	pColor = nullptr;
	pDepth = nullptr;
	pCapture = new KinectCapture();

	// initialize frame parameter 
	colorHeight	= 0;
	colorWidth	= 0;
	depthHeight = 0;
	depthWidth	= 0;
	mode = Kinect_Driver_Mesh_Model::NONE;
}

CKinectDriver::~CKinectDriver()
{
	if (pCapture != nullptr)
	{
		delete pCapture;
	}

	if (pColor != nullptr)
	{
		delete pColor;
	}

	if (pDepth != nullptr)
	{
		delete pDepth;
	}
}

lzBool CKinectDriver::OpenSensor()
{
	lzBool ret = true;
	ret = pCapture->Initialize();
	if (ret)
	{
		colorHeight = pCapture->nColorFrameHeight;
		colorWidth = pCapture->nColorFrameWidth;
		depthHeight = pCapture->nDepthFrameHeight;
		depthWidth = pCapture->nDepthFrameWidth;
	}

	return ret;
}


lzBool CKinectDriver::UpdateFrame()
{
	lzBool ret = true;
	ret = pCapture->AcquireFrame();
	return ret;
}


lzBool CKinectDriver::SetParameter(IN Kinect_Driver_Para_Type* para)
{
	colorHeight	= para->colorHeight;
	colorWidth	= para->colorWidth;
	depthHeight = para->depthHeight;
	depthWidth	= para->depthWidth;
	mode		= para->enMeshModel;

	return true;
}


lzBool CKinectDriver::GetParameter(OUT Kinect_Driver_Para_Type* para)
{
	para->colorHeight	= colorHeight;
	para->colorWidth	= colorWidth;
	para->depthHeight	= depthHeight;
	para->depthWidth	= depthWidth;
	para->enMeshModel	= mode;

	return true;
}



LZ_EXPORTS_API lzBool lzKinectDriverOpenLog(lzBool isopen)
{
	if (_access(path_to_log_file.c_str(), 0) == -1)
	{
		_mkdir(path_to_log_file.c_str());
	}

	if (isopen)
	{	
		g3::only_change_at_initialization::setLogLevel(WARNING, true);
		g3::only_change_at_initialization::setLogLevel(INFO, true);
		g3::only_change_at_initialization::setLogLevel(DEBUG, true);
	}
	else
	{
		g3::only_change_at_initialization::setLogLevel(WARNING, false);
		g3::only_change_at_initialization::setLogLevel(INFO, false);
		g3::only_change_at_initialization::setLogLevel(DEBUG, false);
	}

	std::string log_prefix = "KinectDriver";
	worker = g3::LogWorker::createLogWorker();
	handle = worker->addDefaultLogger(log_prefix, path_to_log_file);
	g3::initializeLogging(worker.get());

	return true;
}

// open kinect 
LZ_EXPORTS_API lzBool lzKinectDriverOpenSensor(void)
{
	int ret = true;

	kinectDriver = std::unique_ptr<CKinectDriver>(new CKinectDriver());
	
	ret = kinectDriver->OpenSensor();
	if (!ret)
	{
		LOGF(WARNING, " open kinect fail.");
	}

	return ret;
}

// close kinect
LZ_EXPORTS_API lzBool lzKinectDriverCloseSensor(void)
{
	kinectDriver = nullptr;
	worker = nullptr;
	handle = nullptr;
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
	lzBool ret = true;
	ret = kinectDriver->UpdateFrame();
	return ret;
}

//
LZ_EXPORTS_API lzBool lzKInectDriverAcquireColorFrame(OUT Kinect_Driver_Color_Frame_Type* colorFrame)
{
	if ((kinectDriver->colorHeight != KINECT_DRIVER_COLOR_HEIGHT) 
		|| (kinectDriver->colorWidth != KINECT_DRIVER_COLOR_WIDTH))
	{
		return false;
	}

	kinectDriver->pColor = new lzRGBX[kinectDriver->colorHeight * kinectDriver->colorWidth];

	lzInt32 index = 0;
	for (lzInt32 i = 0; i < kinectDriver->colorHeight; i++)
	{
		for (lzInt32 j = 0; j < kinectDriver->colorWidth; j++)
		{
			// careful, there efficiency maybe low
			kinectDriver->pColor[index].r = kinectDriver->pCapture->pColorRGBX[index].r;
			kinectDriver->pColor[index].g = kinectDriver->pCapture->pColorRGBX[index].g;
			kinectDriver->pColor[index].b = kinectDriver->pCapture->pColorRGBX[index].b;
			kinectDriver->pColor[index].x = kinectDriver->pCapture->pColorRGBX[index].x;

			colorFrame->frame[index].r = kinectDriver->pColor[index].r;
			colorFrame->frame[index].g = kinectDriver->pColor[index].g;
			colorFrame->frame[index].b = kinectDriver->pColor[index].b;

			index++;
		}
	}

	return true;
}

//
LZ_EXPORTS_API lzBool lzKInectDriverAcquireDepthFrame(OUT Kinect_Driver_Depth_Frame_Type* depthFrame)
{
	if ((kinectDriver->depthHeight != KINECT_DRIVER_DEPTH_HEIGHT)
		|| (kinectDriver->depthWidth != KINECT_DRIVER_DEPTH_WIDTH))
	{
		return false;
	}

	kinectDriver->pDepth = new lzInt32[kinectDriver->depthHeight * kinectDriver->depthWidth];

	lzInt32 index = 0;
	for (lzInt32 i = 0; i < kinectDriver->depthHeight; i++)
	{
		for (lzInt32 j = 0; j < kinectDriver->depthWidth; j++)
		{
			// careful, there efficiency maybe low
			kinectDriver->pDepth[index] = (lzInt32)kinectDriver->pCapture->pDepth[index];
			depthFrame->frame[index] = kinectDriver->pDepth[index];

			index++;
		}
	}

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

