 /**
 *
 * Copyright (C), 2016 Lang Qi (shanghai) Internet Tech. Co., Ltd.
 *
 * \brief          ICapture.h c++ file
 * \file         : ICapture.h
 * \version      : ALPHA
 * \author       : caimagic
 * \date         : 2016/9/14 17:39:20
 * \copyright    : Lang Qi (shanghai) Internet Tech. Co., Ltd.
 * \warning      : 
 *
 * Email         : caizhijie_hit@163.com
 * ClassList     :
 * History       : change history log, including date, author and brief content statement.
 *
 */
 
#ifndef __ICAPTURE_H__
#define __ICAPTURE_H__


/********************************************************************************************/
/*			1 Other Header File Including													*/
/********************************************************************************************/
#include <vector>
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

struct Body
{
	Body()
	{
		bTracked = false;
		vJoints.resize(JointType_Count);
		vJointsInColorSpace.resize(JointType_Count);
	}
	BOOLEAN bTracked;
	std::vector<Joint> vJoints;
	std::vector<lzPoint2f> vJointsInColorSpace;
};

class ICapture
{
public:
	ICapture();
	~ICapture();

	virtual bool Initialize() = 0;
	virtual bool Release() = 0;
	virtual bool AcquireFrame() = 0;
	virtual void MapDepthFrameToCameraSpace(lzPoint3f* pCameraSpacePoints) = 0;
	virtual void MapColorFrameToCameraSpace(lzPoint3f* pCameraSpacePoints) = 0;
	virtual void MapDepthFrameToColorSpace(lzPoint3f* pColorSpacePoints) = 0;
	virtual void MapColorFrameToDepthSpace(lzPoint3f* pDepthSpacePoints) = 0;

	bool bInitialized;

	int nColorFrameHeight, nColorFrameWidth;
	int nDepthFrameHeight, nDepthFrameWidth;

	lzUInt16* pDepth;
	lzUInt16* pInfraRed;
	lzUInt8*  pBodyIndex;
	lzRGBX*   pColorRGBX;

	std::vector<Body> vBodies;
	std::vector<Joint> vJoint;
	ICoordinateMapper* pCoordinateMapper;
};


/********************************************************************************************/
/*			7 Others Definition																*/
/********************************************************************************************/


/********************************************************************************************/
/*			8 Function Declare																*/
/********************************************************************************************/

#endif // __ICAPTURE_H__
