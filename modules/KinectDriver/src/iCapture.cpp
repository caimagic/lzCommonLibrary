#include "ICapture.h"

ICapture::ICapture()
{
    bInitialized = false;

    nColorFrameHeight = 0;
    nColorFrameWidth = 0;
    nDepthFrameHeight = 0;
    nDepthFrameWidth = 0;

    pDepth = NULL;
    pColorRGBX = NULL;
    pBodyIndex = NULL;
	pInfraRed = NULL;
}

ICapture::~ICapture()
{
    if(pDepth != NULL)
    {
        delete[] pDepth;
        pDepth = NULL;
    }

    if(pColorRGBX != NULL)
    {
        delete[] pColorRGBX;
        pColorRGBX = NULL;
    }

    if(pBodyIndex != NULL)
    {
        delete[] pBodyIndex;
        pBodyIndex = NULL;
    }

	if (pInfraRed != NULL)
	{
		delete[] pInfraRed;
		pInfraRed = NULL;
	}
}