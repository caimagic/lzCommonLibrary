 /**
 *
 * Copyright (C), 2016 Lang Qi (shanghai) Internet Tech. Co., Ltd.
 *
 * \brief          kinectCapture.h c++ file
 * \file         : kinectCapture.h
 * \version      : ALPHA
 * \author       : caimagic
 * \date         : 2016/9/15 0:06:26
 * \copyright    : Lang Qi (shanghai) Internet Tech. Co., Ltd.
 * \warning      : 
 *
 * Email         : caizhijie_hit@163.com
 * ClassList     :
 * History       : change history log, including date, author and brief content statement.
 *
 */
 
#ifndef __KINECT_CAPTURE_H__
#define __KINECT_CAPTURE_H__


/********************************************************************************************/
/*			1 Other Header File Including													*/
/********************************************************************************************/
#include "ICapture.h"
#include "Kinect.h"
#include "lzTypes.h"

/********************************************************************************************/
/*			2 Macro Definition																*/
/********************************************************************************************/


/********************************************************************************************/
/*			3 Enum Type Definition															*/
/********************************************************************************************/


/********************************************************************************************/
/*			4 Global Variable Declaring														*/
/********************************************************************************************/


/********************************************************************************************/
/*			5 Struct Type Definition														*/
/********************************************************************************************/


/********************************************************************************************/
/*			6 Class Type Definition															*/
/********************************************************************************************/

class KinectCapture : public ICapture
{
public:
	KinectCapture();
	~KinectCapture();

	bool Initialize();
	bool Release();
	bool AcquireFrame();
	void MapDepthFrameToCameraSpace(lzPoint3f* pCameraSpacePoints);
	void MapColorFrameToCameraSpace(lzPoint3f* pCameraSpacePoints);
	void MapDepthFrameToColorSpace(lzPoint3f* pColorSpacePoints);
	void MapColorFrameToDepthSpace(lzPoint3f* pDepthSpacePoints);

private:
	//ICoordinateMapper* pCoordinateMapper;
	IKinectSensor* pKinectSensor;
	IMultiSourceFrameReader* pMultiSourceFrameReader;
	IColorFrameSource* pColorFrameSource;
	IColorFrameReader* pColorFrameReader;
	IDepthFrameSource* pDepthFrameSource;
	IDepthFrameReader* pDepthFrameReader;
	IBodyFrameSource* pBodyFrameSource;
	IBodyFrameReader* pBodyFrameReader;
	IBodyIndexFrameSource* pBodyIndexFrameSource;
	IBodyIndexFrameReader* pBodyIndexFrameReader;
	IInfraredFrameSource* pInfraredFrameSource;
	IInfraredFrameReader* pInfraredFrameReader;

	void GetDepthFrame(IMultiSourceFrame* pMultiFrame);
	void GetColorFrame(IMultiSourceFrame* pMultiFrame);
	void GetBodyFrame(IMultiSourceFrame* pMultiFrame);
	void GetBodyIndexFrame(IMultiSourceFrame* pMultiFrame);
	void GetInfraRedFrame(IMultiSourceFrame* pMultiFrame);

	bool CaptureDepthFrame();
	bool CaptureColorFrame();
	bool CaptureBodyFrame();
	bool CaptureBodyIndexFrame();
	bool CaptureInfraRedFrame();
};

/********************************************************************************************/
/*			7 Others Definition																*/
/********************************************************************************************/


/********************************************************************************************/
/*			8 Function Declare																*/
/********************************************************************************************/


#endif // __KINECT_CAPTURE_H__