// LZ_KinectDriver.cpp : 定义 DLL 应用程序的导出函数。
//
#include <memory>
#include <iostream>
#include "KinectDriver.h"
#include "kinectCapture.h"
#include "g3log/g3log.hpp"
#include "g3log/logworker.hpp"
#include <io.h>
#include "direct.h"
#include "tbb/task_scheduler_init.h" 
#include "tbb/blocked_range.h" 
#include "tbb/parallel_for.h" 

std::unique_ptr<CKinectDriver> kinectDriver;
std::unique_ptr<g3::LogWorker> worker;
std::unique_ptr<g3::FileSinkHandle> handle;

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
const std::string path_to_log_file = ".//log//";
#else
const std::string path_to_log_file = "/tmp/";
#endif

CKinectDriver::CKinectDriver():
	bFrameUpdateFlag(false)
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

	tbb::task_scheduler_init init;
}

CKinectDriver::~CKinectDriver()
{
	LOGF(WARNING, "enter CKinectDriver::~CKinectDriver");
	if (pCapture != nullptr)
	{
		pCapture->Release();
		delete pCapture;
	}

	if (pColor != nullptr)
	{
		delete[] pColor;
	}

	if (pDepth != nullptr)
	{
		delete[] pDepth;
	}

	if (pBodyIndex != nullptr)
	{
		delete[] pBodyIndex;
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
	bFrameUpdateFlag = false;

	ret = pCapture->AcquireFrame();
	if (ret == true)
	{
		bFrameUpdateFlag = true;
	}

	for (int i = 0; i < KINECT_DRIVER_COLOR_HEIGHT; i++)
	{
		for (int j = 0; j < KINECT_DRIVER_COLOR_WIDTH; j++)
		{
			pColor[i*KINECT_DRIVER_COLOR_WIDTH + j].r = pCapture->pColorRGBX[i*KINECT_DRIVER_COLOR_WIDTH + j].r;
			pColor[i*KINECT_DRIVER_COLOR_WIDTH + j].g = pCapture->pColorRGBX[i*KINECT_DRIVER_COLOR_WIDTH + j].g;
			pColor[i*KINECT_DRIVER_COLOR_WIDTH + j].b = pCapture->pColorRGBX[i*KINECT_DRIVER_COLOR_WIDTH + j].b;
		}
	}

	for (int i = 0; i < KINECT_DRIVER_DEPTH_HEIGHT; i++)
	{
		for (int j = 0; j < KINECT_DRIVER_DEPTH_WIDTH; j++)
		{
			pDepth[i*KINECT_DRIVER_DEPTH_WIDTH + j] = pCapture->pDepth[i*KINECT_DRIVER_DEPTH_WIDTH + j];
			pBodyIndex[i*KINECT_DRIVER_DEPTH_WIDTH + j] = pCapture->pBodyIndex[i*KINECT_DRIVER_DEPTH_WIDTH + j];
		}
	}

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

	lzInt32 step = 3;
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

			/* 取2 3 0点画三角形 */
			if ((abs(p2.z - p3.z) < z_th) && (abs(p3.z - p0.z) < z_th) && (abs(p0.z - p2.z) < z_th))
			{
				meshUnit.p0 = p2;
				meshUnit.p1 = p3;
				meshUnit.p2 = p0;

				meshUnit.uv0.x = uv2.x / (lzFloat32)pCapture->nColorFrameWidth;
				meshUnit.uv0.y = uv2.y / (lzFloat32)pCapture->nColorFrameHeight;
				meshUnit.uv1.x = uv3.x / (lzFloat32)pCapture->nColorFrameWidth;
				meshUnit.uv1.y = uv3.y / (lzFloat32)pCapture->nColorFrameHeight;
				meshUnit.uv2.x = uv0.x / (lzFloat32)pCapture->nColorFrameWidth;
				meshUnit.uv2.y = uv0.y / (lzFloat32)pCapture->nColorFrameHeight;

				vecMeshUnit.push_back(meshUnit);
				tri_count_flag++;
			}

			if (!tri_count_flag)
			{
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
				}

				/* 取3 0 1点画三角形 */
				if ((abs(p3.z - p0.z) < z_th) && (abs(p0.z - p1.z) < z_th) && (abs(p1.z - p3.z) < z_th))
				{
					meshUnit.p0 = p3;
					meshUnit.p1 = p0;
					meshUnit.p2 = p1;

					meshUnit.uv0.x = uv3.x / (lzFloat32)pCapture->nColorFrameWidth;
					meshUnit.uv0.y = uv3.y / (lzFloat32)pCapture->nColorFrameHeight;
					meshUnit.uv1.x = uv0.x / (lzFloat32)pCapture->nColorFrameWidth;
					meshUnit.uv1.y = uv0.y / (lzFloat32)pCapture->nColorFrameHeight;
					meshUnit.uv2.x = uv1.x / (lzFloat32)pCapture->nColorFrameWidth;
					meshUnit.uv2.y = uv1.y / (lzFloat32)pCapture->nColorFrameHeight;

					vecMeshUnit.push_back(meshUnit);
				}
			}
		}
	}

	delete[] pCameraSpacePoint;
	delete[] pColorSpacePoint;
	return vecMeshUnit.size();
}

LZ_EXPORTS_C lzInt32 lzTest(lzInt32 val)
{
	return val;
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


LZ_EXPORTS_C lzBool lzKinectDriverCloseLog()
{
	if ((worker == nullptr)&&(handle == nullptr))
	{
		return false;
	}

	g3::only_change_at_initialization::setLogLevel(WARNING, false);
	g3::only_change_at_initialization::setLogLevel(INFO, false);
	g3::only_change_at_initialization::setLogLevel(DEBUG, false);
	worker = nullptr;
	handle = nullptr;
	return true;
}


// open kinect 
LZ_EXPORTS_C lzBool lzKinectDriverOpenSensor(void)
{
	lzBool ret = false;

	kinectDriver = std::unique_ptr<CKinectDriver>(new CKinectDriver());
	ret = kinectDriver->OpenSensor();

	return ret;
}

/** \brief lzKinectDriverCloseSensor
*
*	\code {.cpp}
	kinectDriver = nullptr;
	worker = nullptr;
	handle = nullptr;
	return true;
*	\endcode
*
* 	\fn    
*	\note  
*   \author	caimagic
*   \date   2016/9/23 20:49:10
*	\param[in] null
*	\param[out]  null
*	\warning  fuck！！！
*	\return	
*	\sa 
*/
LZ_EXPORTS_C lzBool lzKinectDriverCloseSensor(void)
{
	kinectDriver.release();
	worker.release();
	handle.release();
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
LZ_EXPORTS_C lzBool lzKinectDriverUpdateFrame()
{
	lzBool ret = false;
	ret = kinectDriver->UpdateFrame();
	return ret;
}

//
LZ_EXPORTS_C lzBool lzKinectDriverAcquireColorFrame(OUT lzRGB* colorFrame)
{
	if (kinectDriver->bFrameUpdateFlag == false)
	{
		return false;
	}

	lzInt32 index = 0;
	for (lzInt32 i = 0; i < kinectDriver->colorHeight; i++)
	{
		for (lzInt32 j = 0; j < kinectDriver->colorWidth; j++)
		{
			colorFrame[index].r = kinectDriver->pColor[index].r;
			colorFrame[index].g = kinectDriver->pColor[index].g;
			colorFrame[index].b = kinectDriver->pColor[index].b;
			index++;
		}
	}

	return true;
}

//
LZ_EXPORTS_C lzBool lzKinectDriverAcquireDepthFrame(OUT Kinect_Driver_Depth_Frame_Type* depthFrame)
{
	if (kinectDriver->bFrameUpdateFlag == false)
	{
		return false;
	}

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
LZ_EXPORTS_C lzBool lzKinectDriverAcquireBodyIndexFrame(OUT Kinect_Driver_BodyIndex_Frame_Type* bodyIndexFrame)
{
	if (kinectDriver->bFrameUpdateFlag == false)
	{
		return false;
	}

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
//	lzBool retColorFrame = lzKinectDriverAcquireColorFrame(&frame->color);
//	lzBool retDepthFrame = lzKinectDriverAcquireDepthFrame(&frame->depth);
//	lzBool retBodyIndexFrame = lzKinectDriverAcquireBodyIndexFrame(&frame->bodyIndex);

//	if ((retColorFrame == false)||(retDepthFrame == false)||(retBodyIndexFrame == false))
//	{
//		LOGF(WARNING, "ret color %d depth %d BodyIndex %d", retColorFrame, retDepthFrame, retBodyIndexFrame);
//		return false;
//	}

	return true;
}

//
LZ_EXPORTS_C lzBool lzKinectDriverWriteRawData(IN string filename)
{
	if (kinectDriver->bFrameUpdateFlag == false)
	{
		return false;
	}

	CMMAPFile* pMMAPFile = new CMMAPFile();
	unsigned long size_low = kinectDriver->colorHeight*kinectDriver->colorWidth * sizeof(lzRGBX);
	size_low += kinectDriver->depthHeight*kinectDriver->depthWidth * sizeof(lzInt32);
	unsigned long size_high = 0;

	char* pvFile = (char*)pMMAPFile->MMAP_CreateFile(filename, CMMAPFILE_CREATE_ALWAYS, 0, size_low, size_high);
	if (NULL == pvFile)
	{
		LOGF(WARNING, "open file fail %s", filename.c_str());
		return false;
	}

	lzRGBX* ptrColor = (lzRGBX*)pvFile;
	for (int i = 0; i < kinectDriver->colorHeight; i++)
	{
		for (int j = 0; j < kinectDriver->colorWidth; j++)
		{
			*ptrColor = kinectDriver->pColor[i*kinectDriver->colorWidth + j];
			ptrColor++;			
		}
	}

	lzInt32* ptrDepth = (lzInt32*)(pvFile + kinectDriver->colorHeight*kinectDriver->colorWidth * sizeof(lzRGBX));
	for (int i = 0; i < kinectDriver->depthHeight; i++)
	{
		for (int j = 0; j < kinectDriver->depthWidth; j++)
		{
			*ptrDepth = kinectDriver->pDepth[i*kinectDriver->depthWidth + j];
			ptrDepth++;
		}
	}
	
	pMMAPFile->MMAP_Release();
	delete pMMAPFile;

	return true;
}

//
LZ_EXPORTS_C lzBool lzKinectDriverReadRawData(IN string filename, lzRGBX** ptrColor, lzInt32** ptrDepth, lzMatSize colorSize, lzMatSize depthSize)
{
	// check file exists
	if ((_access(filename.c_str(), 0)) == -1)
	{
		LOGF(WARNING, "file %s is not exist.", filename.c_str());
		return false;
	}

	CMMAPFile* pMMAPFile = new CMMAPFile();
	unsigned long size_low = colorSize.height*colorSize.width * sizeof(lzRGBX);
	size_low += depthSize.height*depthSize.width * sizeof(lzInt32);
	unsigned long size_high = 0;

	char* pvFile = (char*)pMMAPFile->MMAP_CreateFile(filename, CMMAPFILE_OPEN_EXISTING, 0, size_low, size_high);
	if (NULL == pvFile)
	{
		LOGF(WARNING, "open file fail %s", filename.c_str());
		return false;
	}

	*ptrColor = new lzRGBX[colorSize.height*colorSize.width];
	*ptrDepth = new lzInt32[depthSize.height*depthSize.width];
	lzRGBX* pColor = *ptrColor;
	lzInt32* pDepth = *ptrDepth;

	lzRGBX* ptr1 = (lzRGBX*)pvFile;
	for (int i = 0; i < colorSize.height; i++)
	{
		for (int j = 0; j < colorSize.width; j++)
		{
			*pColor = ptr1[i*colorSize.width + j];
			pColor++;
		}
	}

	lzInt32* ptr2 = (lzInt32*)(pvFile + colorSize.height*colorSize.width * sizeof(lzRGBX));
	for (int i = 0; i < depthSize.height; i++)
	{
		for (int j = 0; j < depthSize.width; j++)
		{
			*pDepth = ptr2[i*depthSize.width + j];
			pDepth++;
		}
	}

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
LZ_EXPORTS_C lzBool lzKinectDriverAcquireMeshCount(OUT lzInt32* count)
{
	lzInt32 meshCount = 0;
	meshCount = kinectDriver->CreateModule();
	*count = meshCount;

	if (meshCount == 0)
	{
		return false;
	}
	return true;
}


//
LZ_EXPORTS_C lzBool lzKinectDriverAcquireModel(IN lzInt32 count, OUT lzMeshUnitType* mesh)
{
	lzInt32 meshSize = kinectDriver->vecMeshUnit.size();
	if (count != meshSize)
	{
		return false;
	}

	for (int i = 0; i < count; i++)
	{
		memcpy(&(mesh[i]), &(kinectDriver->vecMeshUnit[i]), sizeof(lzMeshUnitType));
	}

	return true;
}

//
char* ptr = NULL;
lzInt32 meshCount = 0;
lzInt32 filecount = 0;

LZ_EXPORTS_C lzInt32 lzKinectDriverReadMMAPFileCount(lzUInt32 framecount)
{
	CMMAPFile* mmapFile = new CMMAPFile();
	char temp[100];
	sprintf_s(temp, "D:\\file\\%d.bin", framecount);
	string filename(temp);

	unsigned long size_low = 0;
	unsigned long size_high = 0;
	size_low = mmapFile->MMAP_GetFileSize(filename, &size_high);
	char* pvFile = (char*)mmapFile->MMAP_CreateFile(filename, CMMAPFILE_OPEN_EXISTING, 0, size_low, size_high);
	if (NULL == pvFile)
	{
		LOGF(WARNING, "open file fail %s", filename.c_str());
		return 0;
	}

	ptr = pvFile;

	meshCount = *(int*)ptr;
	ptr += sizeof(int);

	return meshCount;
}

//
LZ_EXPORTS_C lzBool lzKinectDriverReadMMAP(Kinect_Driver_Mesh_Type mesh[], lzInt32 count)
{
	if ((count == 0) || (meshCount == 0))
	{
		return false;
	}
	
	lzInt32 c = meshCount < count ? meshCount : count;

	for (int i = 0; i < c; i++)
	{
		memcpy(&mesh[i], ptr, sizeof(Kinect_Driver_Mesh_Type));
		ptr += sizeof(Kinect_Driver_Mesh_Type);
	}

	return true;
}