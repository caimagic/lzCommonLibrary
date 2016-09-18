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
	pCapture = new KinectCapture();
	pColor = new lzRGBX[KINECT_DRIVER_COLOR_WIDTH * KINECT_DRIVER_COLOR_HEIGHT];
	pDepth = new lzInt32[KINECT_DRIVER_DEPTH_WIDTH * KINECT_DRIVER_DEPTH_HEIGHT];
	pBodyIndex = new lzUInt8[KINECT_DRIVER_DEPTH_WIDTH * KINECT_DRIVER_DEPTH_HEIGHT];

	// initialize frame parameter 
	colorHeight	= KINECT_DRIVER_COLOR_HEIGHT;
	colorWidth	= KINECT_DRIVER_COLOR_WIDTH;
	depthHeight = KINECT_DRIVER_DEPTH_HEIGHT;
	depthWidth	= KINECT_DRIVER_DEPTH_WIDTH;
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
	LOGF(INFO, "enter KinectDriver OpenSensor.");
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

lzInt32 CKinectDriver::CreateModule(void)
{
	vecMeshUnit.clear();
	CameraSpacePoint* pCameraSpacePoint = new CameraSpacePoint[KINECT_DRIVER_DEPTH_HEIGHT * KINECT_DRIVER_DEPTH_WIDTH];
	ColorSpacePoint* pColorSpacePoint = new ColorSpacePoint[KINECT_DRIVER_DEPTH_HEIGHT * KINECT_DRIVER_DEPTH_WIDTH];

	pCapture->pCoordinateMapper->MapDepthFrameToCameraSpace(KINECT_DRIVER_DEPTH_HEIGHT * KINECT_DRIVER_DEPTH_WIDTH,
		pCapture->pDepth,
		KINECT_DRIVER_DEPTH_HEIGHT * KINECT_DRIVER_DEPTH_WIDTH,
		pCameraSpacePoint);
	
	pCapture->pCoordinateMapper->MapDepthFrameToColorSpace(KINECT_DRIVER_DEPTH_HEIGHT * KINECT_DRIVER_DEPTH_WIDTH,
		pCapture->pDepth,
		KINECT_DRIVER_DEPTH_HEIGHT * KINECT_DRIVER_DEPTH_WIDTH,
		pColorSpacePoint);

	lzPoint3f p0, p1, p2, p3;
	lzPoint2f uv0, uv1, uv2, uv3;

	lzInt32 step = 5;
	lzInt32 index[4] = { 0, 0, 0, 0 };
	for (lzInt32 i = 0; i < pCapture->nDepthFrameHeight - step; i += step)
	{
		for (lzInt32 j = 0; j < pCapture->nDepthFrameWidth - step; j += step)
		{
			index[0] = i*pCapture->nDepthFrameWidth + j;
			index[1] = i*pCapture->nDepthFrameWidth + j + step;
			index[2] = (i + step)*pCapture->nDepthFrameWidth + j + step;
			index[3] = (i + step)*pCapture->nDepthFrameWidth + j;
			
			p0.x = pCameraSpacePoint[index[0]].X;
			p0.y = pCameraSpacePoint[index[0]].Y;
			p0.z = pCameraSpacePoint[index[0]].Z;
			p1.x = pCameraSpacePoint[index[1]].X;
			p1.y = pCameraSpacePoint[index[1]].Y;
			p1.z = pCameraSpacePoint[index[1]].Z;
			p2.x = pCameraSpacePoint[index[2]].X;
			p2.y = pCameraSpacePoint[index[2]].Y;
			p2.z = pCameraSpacePoint[index[2]].Z;
			p3.x = pCameraSpacePoint[index[3]].X;
			p3.y = pCameraSpacePoint[index[3]].Y;
			p3.z = pCameraSpacePoint[index[3]].Z;

			uv0.x = pColorSpacePoint[index[0]].X;
			uv0.y = pColorSpacePoint[index[0]].Y;
			uv1.x = pColorSpacePoint[index[1]].X;
			uv1.y = pColorSpacePoint[index[1]].Y;
			uv2.x = pColorSpacePoint[index[2]].X;
			uv2.y = pColorSpacePoint[index[2]].Y;
			uv3.x = pColorSpacePoint[index[3]].X;
			uv3.y = pColorSpacePoint[index[3]].Y;

			lzInt32 tri_count_flag = 0;
			lzFloat32 z_th = 0.05f;
			// 判断点的有效性
			if ((p0.x < -10.0f) || (p0.x > 10.0f) || (p0.y < -10.0f) || (p0.y > 10.0f) || (p0.z < -10.0f) || (p0.z > 10.0f))
			{
				continue;
			}

			if ((p1.x < -10.0f) || (p1.x > 10.0f) || (p1.y < -10.0f) || (p1.y > 10.0f) || (p1.z < -10.0f) || (p1.z > 10.0f))
			{
				continue;
			}

			if ((p2.x < -10.0f) || (p2.x > 10.0f) || (p2.y < -10.0f) || (p2.y > 10.0f) || (p2.z < -10.0f) || (p2.z > 10.0f))
			{
				continue;
			}

			if ((p3.x < -10.0f) || (p3.x > 10.0f) || (p3.y < -10.0f) || (p3.y > 10.0f) || (p3.z < -10.0f) || (p3.z > 10.0f))
			{
				continue;
			}

			lzMeshUnitType meshUnit;

			/* 取0 1 2点画三角形 */
			if ((abs(p0.z - p1.z) < z_th) && (abs(p0.z - p2.z) < z_th) && (abs(p1.z - p2.z) < z_th))
			{
				meshUnit.p0 = p0;
				meshUnit.p1 = p1;
				meshUnit.p2 = p2;

				meshUnit.uv0.x = uv0.x / (lzFloat32)pCapture->nColorFrameWidth;
				meshUnit.uv0.y = uv0.y / (lzFloat32)pCapture->nColorFrameHeight;
				meshUnit.uv1.x = uv1.x / (lzFloat32)pCapture->nColorFrameWidth;
				meshUnit.uv1.y = uv1.y / (lzFloat32)pCapture->nColorFrameHeight;
				meshUnit.uv2.x = uv2.x / (lzFloat32)pCapture->nColorFrameWidth;
				meshUnit.uv2.y = uv2.y / (lzFloat32)pCapture->nColorFrameHeight;

				vecMeshUnit.push_back(meshUnit);
				tri_count_flag++;
			}

			/* 取1 2 3点画三角形 */
			if ((abs(p1.z - p2.z) < z_th) && (abs(p1.z - p3.z) < z_th) && (abs(p2.z - p3.z) < z_th))
			{
				meshUnit.p0 = p1;
				meshUnit.p1 = p2;
				meshUnit.p2 = p3;

				meshUnit.uv0.x = uv1.x / (lzFloat32)pCapture->nColorFrameWidth;
				meshUnit.uv0.y = uv1.y / (lzFloat32)pCapture->nColorFrameHeight;
				meshUnit.uv1.x = uv2.x / (lzFloat32)pCapture->nColorFrameWidth;
				meshUnit.uv1.y = uv2.y / (lzFloat32)pCapture->nColorFrameHeight;
				meshUnit.uv2.x = uv3.x / (lzFloat32)pCapture->nColorFrameWidth;
				meshUnit.uv2.y = uv3.y / (lzFloat32)pCapture->nColorFrameHeight;

				vecMeshUnit.push_back(meshUnit);
				tri_count_flag++;
			}
		}
	}

	delete[] pCameraSpacePoint;
	delete[] pColorSpacePoint;
	return false;
}

LZ_EXPORTS_C int lzHello(void)
{
	return 500;
}

LZ_EXPORTS_C lzBool lzKinectDriverOpenLog(lzBool isopen)
{
	if (isopen)
	{	
		if (_access(path_to_log_file.c_str(), 0) == -1)
		{
			_mkdir(path_to_log_file.c_str());
		}
		g3::only_change_at_initialization::setLogLevel(WARNING, true);
		g3::only_change_at_initialization::setLogLevel(INFO, true);
		g3::only_change_at_initialization::setLogLevel(DEBUG, true);
		std::string log_prefix = "KinectDriver";
		worker = g3::LogWorker::createLogWorker();
		handle = worker->addDefaultLogger(log_prefix, path_to_log_file);
		g3::initializeLogging(worker.get());
		LOGF(INFO, "lzKinectDriverOpenLog succ.");
	}
	else
	{
		g3::only_change_at_initialization::setLogLevel(WARNING, false);
		g3::only_change_at_initialization::setLogLevel(INFO, false);
		g3::only_change_at_initialization::setLogLevel(DEBUG, false);
		LOGF(INFO, "lzKinectDriverOpenLog fail.");
	}

	return true;
}

#define KINECT_CAPTURE_MULTI_FRAME 1

// open kinect 
LZ_EXPORTS_C lzBool lzKinectDriverOpenSensor(void)
{
	kinectDriver = std::unique_ptr<CKinectDriver>(new CKinectDriver());
	kinectDriver->OpenSensor();

	return true;
}

// close kinect
LZ_EXPORTS_C lzBool lzKinectDriverCloseSensor(void)
{
	kinectDriver = nullptr;
	worker = nullptr;
	handle = nullptr;
	return true;
}

// 
LZ_EXPORTS_C lzBool lzKinectDriverSetParameter(IN Kinect_Driver_Para_Type* para)
{
	kinectDriver->SetParameter(para);
	return true;
}

// 
LZ_EXPORTS_C lzBool lzKinectDriverGetParameter(OUT Kinect_Driver_Para_Type* para)
{
	kinectDriver->GetParameter(para);
	return true;
}

//
LZ_EXPORTS_C lzBool lzKInectDriverUpdateFrame()
{
	lzBool ret = true;
	ret = kinectDriver->UpdateFrame();
	return ret;
}

//
LZ_EXPORTS_C lzBool lzKInectDriverAcquireColorFrame(OUT Kinect_Driver_Color_Frame_Type* colorFrame)
{
	if ((kinectDriver->colorHeight != KINECT_DRIVER_COLOR_HEIGHT) 
		|| (kinectDriver->colorWidth != KINECT_DRIVER_COLOR_WIDTH))
	{
		return false;
	}

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
LZ_EXPORTS_C lzBool lzKInectDriverAcquireDepthFrame(OUT Kinect_Driver_Depth_Frame_Type* depthFrame)
{
	if ((kinectDriver->depthHeight != KINECT_DRIVER_DEPTH_HEIGHT)
		|| (kinectDriver->depthWidth != KINECT_DRIVER_DEPTH_WIDTH))
	{
		return false;
	}

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
LZ_EXPORTS_C lzBool lzKInectDriverAcquireBodyIndexFrame(OUT Kinect_Driver_BodyIndex_Frame_Type* bodyIndexFrame)
{
	if ((kinectDriver->depthHeight != KINECT_DRIVER_DEPTH_HEIGHT)
		|| (kinectDriver->depthWidth != KINECT_DRIVER_DEPTH_WIDTH))
	{
		return false;
	}

	lzInt32 index = 0;
	for (lzInt32 i = 0; i < kinectDriver->depthHeight; i++)
	{
		for (lzInt32 j = 0; j < kinectDriver->depthWidth; j++)
		{
			// careful, there efficiency maybe low
			kinectDriver->pDepth[index] = (lzInt32)kinectDriver->pCapture->pDepth[index];

			if (kinectDriver->pCapture->pBodyIndex[index])
			{
				bodyIndexFrame->frame[index] = 1;
			}
			else
			{
				bodyIndexFrame->frame[index] = 0;
			}

			index++;
		}
	}

	return true;
}

//
LZ_EXPORTS_C lzBool lzKinectDriverAcquireFrame(OUT Kinect_Driver_Frame_Type* frame)
{
	lzKInectDriverAcquireColorFrame(&frame->color);
	lzKInectDriverAcquireDepthFrame(&frame->depth);
	lzKInectDriverAcquireBodyIndexFrame(&frame->bodyIndex);
	return true;
}

// 
LZ_EXPORTS_C lzBool lzKinectDriverMapDepthFrameToCameraSpace(
	IN lzInt32 depthPointCount,
	IN Kinect_Driver_Depth_Frame_Type* depthFrame,
	IN lzInt32 cameraPointCount,
	OUT CameraSpacePoint* cameraSpacePoints)
{

	return true;
}

// 
LZ_EXPORTS_C lzBool lzKinectDriverMapDepthFrameToColorSpace(
	IN lzInt32 depthPointCount,
	IN Kinect_Driver_Depth_Frame_Type* depthFrame,
	IN lzInt32 colorPointCount,
	OUT ColorSpacePoint* colorSpacePoints)
{

	return true;
}

// 
LZ_EXPORTS_C lzBool lzKinectDriverMapColorFrameToCameraSpace(
	IN lzInt32 colorPointCount,
	IN Kinect_Driver_Color_Frame_Type* colorFrame,
	IN lzInt32 cameraPointCount,
	OUT CameraSpacePoint* cameraSpacePoints)
{

	return true;
}

// 
LZ_EXPORTS_C lzBool lzKinectDriverMapColorFrameToDepthSpace(
	IN lzInt32 colorPointCount,
	IN Kinect_Driver_Color_Frame_Type* colorFrame,
	IN lzInt32 depthPointCount,
	OUT DepthSpacePoint* depthSpacePoints)
{

	return true;
}

//
LZ_EXPORTS_C lzInt32 lzKinectDriverAcquireMeshCount(OUT lzInt32* count)
{
	lzInt32 meshCount = 0;
	meshCount = kinectDriver->CreateModule();
	return meshCount;
}

//
LZ_EXPORTS_C lzBool lzKinectDriverAcquireModel(OUT Kinect_Driver_Mesh_Type* mesh)
{






	return true;
}

