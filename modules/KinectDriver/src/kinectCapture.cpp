#include "KinectCapture.h"
#include <chrono>
#include <exception> 
#include <iostream>

using namespace std;

#include "g3log/g3log.hpp"
#include <g3log/logworker.hpp>
#include <iomanip>
#include <thread>

KinectCapture::KinectCapture()
{
    pKinectSensor           = NULL;
    pCoordinateMapper       = NULL;
    pMultiSourceFrameReader = NULL;
    pColorFrameSource       = NULL;
    pColorFrameReader       = NULL;
    pDepthFrameSource       = NULL;
    pDepthFrameReader       = NULL;
	pBodyFrameSource		= NULL;
	pBodyFrameReader		= NULL;
	pBodyIndexFrameSource	= NULL;
	pBodyIndexFrameReader	= NULL;
	pInfraredFrameSource	= NULL;
	pInfraredFrameReader	= NULL;
}

KinectCapture::~KinectCapture()
{
    SafeRelease(pKinectSensor);
    SafeRelease(pCoordinateMapper);
    SafeRelease(pMultiSourceFrameReader);
    SafeRelease(pColorFrameSource);
    SafeRelease(pColorFrameReader);
    SafeRelease(pDepthFrameSource);
    SafeRelease(pDepthFrameReader);
	SafeRelease(pBodyFrameSource);
	SafeRelease(pBodyFrameReader);
	SafeRelease(pBodyIndexFrameSource);
	SafeRelease(pBodyIndexFrameReader);
	SafeRelease(pInfraredFrameSource);
	SafeRelease(pInfraredFrameReader);
}

bool KinectCapture::Initialize()
{
    HRESULT hr;

    hr = GetDefaultKinectSensor(&pKinectSensor);

    if(FAILED(hr))
    {
        bInitialized = false;
        return bInitialized;
    }

    if(pKinectSensor)
    {
        hr = pKinectSensor->get_CoordinateMapper(&pCoordinateMapper);
        if(!SUCCEEDED(hr))
        {
            LOGF(WARNING, "KinectSensor get_CoordinateMapper fail");
            return false;
        }
            
        hr = pKinectSensor->Open();
        if(SUCCEEDED(hr))
        {
#if KINECT_CAPTURE_MULTI_FRAME
            pKinectSensor->OpenMultiSourceFrameReader(FrameSourceTypes::FrameSourceTypes_Color |
                    FrameSourceTypes::FrameSourceTypes_Depth |
                    FrameSourceTypes::FrameSourceTypes_Body |
                    FrameSourceTypes::FrameSourceTypes_BodyIndex |
					FrameSourceTypes::FrameSourceTypes_Infrared | 
					FrameSourceTypes::FrameSourceTypes_LongExposureInfrared,
                    &pMultiSourceFrameReader);
#else
			hr = pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
			if (!SUCCEEDED(hr))
			{
				LOGF(WARNING, "KinectSensor get_DepthFrameSource fail.");
			}

			hr = pDepthFrameSource->OpenReader(&pDepthFrameReader);
			if (!SUCCEEDED(hr))
			{
				LOGF(WARNING, "KinectSensor DepthFrameSource OpenReader fail.");
			}

            hr = pKinectSensor->get_ColorFrameSource(&pColorFrameSource);
            if(!SUCCEEDED(hr))
            {
				LOGF(WARNING, "KinectSensor get_ColorFrameSource fail");
            }

            hr = pColorFrameSource->OpenReader(&pColorFrameReader);
            if(!SUCCEEDED(hr))
            {
				LOGF(WARNING, "KinectSensor ColorFrameSource OpenReader fail.");
            }

			hr = pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
			if (!SUCCEEDED(hr))
			{
				LOGF(WARNING, "KinectSensor get_BodyFrameSource fail.");
			}

			hr = pBodyFrameSource->OpenReader(&pBodyFrameReader);
			if (!SUCCEEDED(hr))
			{
				LOGF(WARNING, "KinectSensor BodyFrameSource OpenReader fail.");
			}

			hr = pKinectSensor->get_BodyIndexFrameSource(&pBodyIndexFrameSource);
			if (!SUCCEEDED(hr))
			{
				LOGF(WARNING, "KinectSensor get_BodyIndexFrameSource fail.");
			}

			hr = pBodyIndexFrameSource->OpenReader(&pBodyIndexFrameReader);
			if (!SUCCEEDED(hr))
			{
				LOGF(WARNING, "KinectSensor BodyIndexFrameSource OpenReader fail.");
			}

			hr = pKinectSensor->get_InfraredFrameSource(&pInfraredFrameSource);
			if (!SUCCEEDED(hr))
			{
				LOGF(WARNING, "KinectSensor get_InfraredFrameSource fail.");
			}

			hr = pInfraredFrameSource->OpenReader(&pInfraredFrameReader);
			if (!SUCCEEDED(hr))
			{
				LOGF(WARNING, "KinectSensor InfraredFrameSource OpenReader fail.");
			}           

#endif
        }
        else
        {
			LOGF(WARNING, "KinectSensor Open fail.");
        }
    }

    bInitialized = SUCCEEDED(hr);

    if(bInitialized)
    {
        std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
        bool bTemp;

        do
        {
            bTemp = AcquireFrame();

            std::chrono::duration<double> elapsedSeconds = std::chrono::system_clock::now() - start;

            if(elapsedSeconds.count() > 5.0) // ¼ì²é´óÓÚ5s
            {
                bInitialized = false;
				LOGF(WARNING, "KinectSensor Open Time is too long.");
                break;
            }

        }
        while(!bTemp);
    }

    return bInitialized;
}

bool KinectCapture::Release()
{
	LOGF(WARNING, "KinectCapture Release");

	SafeRelease(pKinectSensor);
	SafeRelease(pCoordinateMapper);
	SafeRelease(pMultiSourceFrameReader);
	SafeRelease(pColorFrameSource);
	SafeRelease(pColorFrameReader);
	SafeRelease(pDepthFrameSource);
	SafeRelease(pDepthFrameReader);
	SafeRelease(pBodyFrameSource);
	SafeRelease(pBodyFrameReader);
	SafeRelease(pBodyIndexFrameSource);
	SafeRelease(pBodyIndexFrameReader);
	SafeRelease(pInfraredFrameSource);
	SafeRelease(pInfraredFrameReader);

	pKinectSensor			= NULL;
	pCoordinateMapper		= NULL;
	pMultiSourceFrameReader = NULL;
	pColorFrameSource		= NULL;
	pColorFrameReader		= NULL;
	pDepthFrameSource		= NULL;
	pDepthFrameReader		= NULL;
	pBodyFrameSource		= NULL;
	pBodyFrameReader		= NULL;
	pBodyIndexFrameSource	= NULL;
	pBodyIndexFrameReader	= NULL;
	pInfraredFrameSource	= NULL;
	pInfraredFrameReader	= NULL;

	return true;
}

bool KinectCapture::AcquireFrame()
{
    if(!bInitialized)
    {
        return false;
    }

    HRESULT hr = NULL;

	//Multi frame
    IMultiSourceFrame* pMultiFrame = NULL;
    hr = pMultiSourceFrameReader->AcquireLatestFrame(&pMultiFrame);
	
    if(!SUCCEEDED(hr))
    {
        return false;
    }
	
	GetInfraRedFrame(pMultiFrame);
    GetDepthFrame(pMultiFrame);
    GetColorFrame(pMultiFrame);	
    GetBodyFrame(pMultiFrame);
    GetBodyIndexFrame(pMultiFrame);
	SafeRelease(pMultiFrame);
	return true;
}

void KinectCapture::MapDepthFrameToCameraSpace(lzPoint3f* pCameraSpacePoints)
{
	pCoordinateMapper->MapDepthFrameToCameraSpace(nDepthFrameWidth * nDepthFrameHeight, pDepth, nDepthFrameWidth * nDepthFrameHeight, (CameraSpacePoint*)pCameraSpacePoints);
}

void KinectCapture::MapColorFrameToCameraSpace(lzPoint3f* pCameraSpacePoints)
{
    pCoordinateMapper->MapColorFrameToCameraSpace(nDepthFrameWidth * nDepthFrameHeight, pDepth, nColorFrameWidth * nColorFrameHeight, (CameraSpacePoint*)pCameraSpacePoints);
}

void KinectCapture::MapDepthFrameToColorSpace(lzPoint3f* pColorSpacePoints)
{
	pCoordinateMapper->MapDepthFrameToColorSpace(nDepthFrameWidth * nDepthFrameHeight, pDepth, nDepthFrameWidth * nDepthFrameHeight, (ColorSpacePoint*)pColorSpacePoints);
}

void KinectCapture::MapColorFrameToDepthSpace(lzPoint3f* pDepthSpacePoints)
{
    pCoordinateMapper->MapColorFrameToDepthSpace(nDepthFrameWidth * nDepthFrameHeight, pDepth, nColorFrameWidth * nColorFrameHeight, (DepthSpacePoint*)pDepthSpacePoints);;
}

void KinectCapture::GetDepthFrame(IMultiSourceFrame* pMultiFrame)
{
    IDepthFrameReference* pDepthFrameReference = NULL;
    IDepthFrame* pDepthFrame = NULL;
    pMultiFrame->get_DepthFrameReference(&pDepthFrameReference);
    HRESULT hr = pDepthFrameReference->AcquireFrame(&pDepthFrame);

    if(SUCCEEDED(hr))
    {
        if(pDepth == NULL)
        {
            IFrameDescription* pFrameDescription = NULL;
            hr = pDepthFrame->get_FrameDescription(&pFrameDescription);
            pFrameDescription->get_Width(&nDepthFrameWidth);
            pFrameDescription->get_Height(&nDepthFrameHeight);
            pDepth = new UINT16[nDepthFrameHeight * nDepthFrameWidth];
            SafeRelease(pFrameDescription);
        }

        UINT nBufferSize = nDepthFrameHeight * nDepthFrameWidth;
        hr = pDepthFrame->CopyFrameDataToArray(nBufferSize, pDepth);
    }

    SafeRelease(pDepthFrame);
    SafeRelease(pDepthFrameReference);
}

void KinectCapture::GetColorFrame(IMultiSourceFrame* pMultiFrame)
{
    IColorFrameReference* pColorFrameReference = NULL;
    IColorFrame* pColorFrame = NULL;
    pMultiFrame->get_ColorFrameReference(&pColorFrameReference);
    HRESULT hr = pColorFrameReference->AcquireFrame(&pColorFrame);

    if(SUCCEEDED(hr))
    {
        if(pColorRGBX == NULL)
        {
            IFrameDescription* pFrameDescription = NULL;
            hr = pColorFrame->get_FrameDescription(&pFrameDescription);
            hr = pFrameDescription->get_Width(&nColorFrameWidth);
            hr = pFrameDescription->get_Height(&nColorFrameHeight);
            pColorRGBX = new lzRGBX[nColorFrameWidth * nColorFrameHeight];
            SafeRelease(pFrameDescription);
        }

        UINT nBufferSize = nColorFrameWidth * nColorFrameHeight * sizeof(lzRGBX);
        hr = pColorFrame->CopyConvertedFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(pColorRGBX), ColorImageFormat_Bgra);
    }

    SafeRelease(pColorFrame);
    SafeRelease(pColorFrameReference);
}

void KinectCapture::GetBodyFrame(IMultiSourceFrame* pMultiFrame)
{
    IBodyFrameReference* pBodyFrameReference = NULL;
    IBodyFrame* pBodyFrame = NULL;
    pMultiFrame->get_BodyFrameReference(&pBodyFrameReference);
    HRESULT hr = pBodyFrameReference->AcquireFrame(&pBodyFrame);


    if(SUCCEEDED(hr))
    {
        IBody* bodies[BODY_COUNT] = { NULL };
        pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, bodies);

        vBodies = std::vector<Body>(BODY_COUNT);
		vJoint = std::vector<Joint>(JointType_Count);

        for(int i = 0; i < BODY_COUNT; i++)
        {
            if(bodies[i])
            {
                Joint joints[JointType_Count];
                BOOLEAN isTracked;

                bodies[i]->get_IsTracked(&isTracked);
                bodies[i]->GetJoints(JointType_Count, joints);

                vBodies[i].vJoints.assign(joints, joints + JointType_Count);
                vBodies[i].bTracked = isTracked;
                vBodies[i].vJointsInColorSpace.resize(JointType_Count);

                for(int j = 0; j < JointType_Count; j++)
                {
                    ColorSpacePoint tempPoint;
                    pCoordinateMapper->MapCameraPointToColorSpace(joints[j].Position, &tempPoint);
                    vBodies[i].vJointsInColorSpace[j].x = tempPoint.X;
                    vBodies[i].vJointsInColorSpace[j].y = tempPoint.Y;		

					vJoint[j] = joints[j];
                }
            }
        }
    }

    SafeRelease(pBodyFrame);
    SafeRelease(pBodyFrameReference);
}

void KinectCapture::GetBodyIndexFrame(IMultiSourceFrame* pMultiFrame)
{
    IBodyIndexFrameReference* pBodyIndexFrameReference = NULL;
    IBodyIndexFrame* pBodyIndexFrame = NULL;
    pMultiFrame->get_BodyIndexFrameReference(&pBodyIndexFrameReference);
    HRESULT hr = pBodyIndexFrameReference->AcquireFrame(&pBodyIndexFrame);


    if(SUCCEEDED(hr))
    {
        if(pBodyIndex == NULL)
        {
            pBodyIndex = new BYTE[nDepthFrameHeight * nDepthFrameWidth];
        }

        UINT nBufferSize = nDepthFrameHeight * nDepthFrameWidth;
        hr = pBodyIndexFrame->CopyFrameDataToArray(nBufferSize, pBodyIndex);
    }

    SafeRelease(pBodyIndexFrame);
    SafeRelease(pBodyIndexFrameReference);
}

void KinectCapture::GetInfraRedFrame(IMultiSourceFrame* pMultiFrame)
{
	IInfraredFrameReference* pInfraredFrameReference = NULL;
	IInfraredFrame* pInfraredFrame = NULL;
	pMultiFrame->get_InfraredFrameReference(&pInfraredFrameReference);
	HRESULT hr = pInfraredFrameReference->AcquireFrame(&pInfraredFrame);
	
	int frameHeight = 0;
	int frameWidth = 0;

	if (SUCCEEDED(hr))
	{
		if (pInfraRed == NULL)
		{
			IFrameDescription* pFrameDescription = NULL;
			hr = pInfraredFrame->get_FrameDescription(&pFrameDescription);
			pFrameDescription->get_Width(&frameHeight);
			pFrameDescription->get_Height(&frameWidth);
			pInfraRed = new UINT16[frameHeight * frameWidth];
			SafeRelease(pFrameDescription);
		}

		UINT nBufferSize = frameHeight * frameWidth;
		hr = pInfraredFrame->CopyFrameDataToArray(nBufferSize, pInfraRed);
	}

	SafeRelease(pInfraredFrame);
	SafeRelease(pInfraredFrameReference);

}

bool KinectCapture::CaptureDepthFrame()
{
	IDepthFrame* pDepthFrame = NULL;
	HRESULT hr = pDepthFrameReader->AcquireLatestFrame(&pDepthFrame);

	if (SUCCEEDED(hr))
	{
		if (pDepth == NULL)
		{
			IFrameDescription* pFrameDescription = NULL;
			hr = pDepthFrame->get_FrameDescription(&pFrameDescription);
			pFrameDescription->get_Width(&nDepthFrameWidth);
			pFrameDescription->get_Height(&nDepthFrameHeight);
			pDepth = new UINT16[nDepthFrameHeight * nDepthFrameWidth];
			SafeRelease(pFrameDescription);
		}

		hr = pDepthFrame->CopyFrameDataToArray(nDepthFrameHeight * nDepthFrameWidth, pDepth);
		SafeRelease(pDepthFrame);
		pDepthFrame = NULL;
	}

	if (SUCCEEDED(hr))
	{
		return true;
	}
	return false;
}

bool KinectCapture::CaptureColorFrame()
{
	IColorFrame* pColorFrame = NULL;
	HRESULT hr = pColorFrameReader->AcquireLatestFrame(&pColorFrame);

	if (SUCCEEDED(hr))
	{
		if (pColorRGBX == NULL)
		{
			IFrameDescription* pFrameDescription = NULL;
			hr = pColorFrame->get_FrameDescription(&pFrameDescription);
			hr = pFrameDescription->get_Width(&nColorFrameWidth);
			hr = pFrameDescription->get_Height(&nColorFrameHeight);
			pColorRGBX = new lzRGBX[nColorFrameWidth * nColorFrameHeight];
			SafeRelease(pFrameDescription);
		}

		pColorFrame->CopyConvertedFrameDataToArray(nColorFrameHeight * nColorFrameWidth * sizeof(lzRGBX), reinterpret_cast<BYTE*>(pColorRGBX), ColorImageFormat::ColorImageFormat_Bgra);
		SafeRelease(pColorFrame);
		pColorFrame = NULL;
	}

	if (SUCCEEDED(hr))
	{
		return true;
	}
	return false;
}

bool KinectCapture::CaptureBodyFrame()
{
	IBodyFrame* pBodyFrame = NULL;
	HRESULT hr = pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

	if (SUCCEEDED(hr))
	{
		IBody* bodies[BODY_COUNT] = { NULL };
		pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, bodies);

		vBodies = std::vector<Body>(BODY_COUNT);
		for (int i = 0; i < BODY_COUNT; i++)
		{
			if (bodies[i])
			{
				Joint joints[JointType_Count];
				BOOLEAN isTracked;

				bodies[i]->get_IsTracked(&isTracked);
				bodies[i]->GetJoints(JointType_Count, joints);

				vBodies[i].vJoints.assign(joints, joints + JointType_Count);
				vBodies[i].bTracked = isTracked;
				vBodies[i].vJointsInColorSpace.resize(JointType_Count);

				for (int j = 0; j < JointType_Count; j++)
				{
					ColorSpacePoint tempPoint;
					pCoordinateMapper->MapCameraPointToColorSpace(joints[j].Position, &tempPoint);
					vBodies[i].vJointsInColorSpace[j].x = tempPoint.X;
					vBodies[i].vJointsInColorSpace[j].y = tempPoint.Y;
				}
			}
		}
	}

	SafeRelease(pBodyFrame);
	pBodyFrame = NULL;

	if (SUCCEEDED(hr))
	{
		return true;
	}
	return false;
}

bool KinectCapture::CaptureBodyIndexFrame()
{
	IBodyIndexFrame* pBodyIndexFrame = NULL;
	HRESULT hr = pBodyIndexFrameReader->AcquireLatestFrame(&pBodyIndexFrame);

	if (SUCCEEDED(hr))
	{
		if (pBodyIndex == NULL)
		{
			pBodyIndex = new BYTE[nDepthFrameHeight * nDepthFrameWidth];
		}

		UINT nBufferSize = nDepthFrameHeight * nDepthFrameWidth;
		hr = pBodyIndexFrame->CopyFrameDataToArray(nBufferSize, pBodyIndex);
	}

	SafeRelease(pBodyIndexFrame);
	if (SUCCEEDED(hr))
	{
		return true;
	}
	return false;
}

bool KinectCapture::CaptureInfraRedFrame()
{
	
	return false;
}
