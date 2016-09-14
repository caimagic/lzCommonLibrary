#pragma once

#include "stdafx.h"
#include "ICapture.h"
#include "Kinect.h"
#include "utils.h"

class KinectCapture : public ICapture
{
public:
	KinectCapture();
	~KinectCapture();

	bool Initialize();
	bool Release();
	bool AcquireFrame();
	void MapDepthFrameToCameraSpace(Point3f *pCameraSpacePoints);
	void MapColorFrameToCameraSpace(Point3f *pCameraSpacePoints);
	void MapDepthFrameToColorSpace(Point2f *pColorSpacePoints);
	void MapColorFrameToDepthSpace(Point2f *pDepthSpacePoints);

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
