#include "stdio.h"
#include "gtest/gtest.h"
#include "KinectDriver.h"
#include "thread"

TEST(KinectDriver, lzTest)
{
	lzInt32 val = 1000;
	lzInt32 ret = lzTest(val);
#if (_CPU_CONFIG_ == _CPU_LINUX_32_)
	cout << "_CPU_CONFIG_ _CPU_LINUX_32_" << endl;
#elif (_CPU_CONFIG_ == _CPU_LINUX_64_)
	cout << "_CPU_CONFIG_ _CPU_LINUX_64_" << endl;
#elif (_CPU_CONFIG_ == _CPU_WIN_32_)
	cout << "_CPU_CONFIG_ _CPU_WIN_32_" << endl;
#elif (_CPU_CONFIG_ == _CPU_WIN_64_)
	cout << "_CPU_CONFIG_ _CPU_WIN_64_" << endl;
#elif
	cout << "_CPU_CONFIG_ undefined" << endl;
#endif
	EXPECT_EQ(ret, val);
}

lzInt32 RunLoopCount = 50;
lzInt32 RunLoopUpdateFailCount = 50;
lzBool bIsKinectThreadRun = false;

lzMeshUnitType* mesh;
lzBool kinectLoop()
{
	lzBool ret = lzKinectDriverUpdateFrame();
	if (ret == false)
	{
		return false;
	}

	int num = 0;

	printf("lzKinectDriverAcquireMeshCount");
	lzKinectDriverAcquireMeshCount(&num);

	mesh = new lzMeshUnitType[num];

	lzKinectDriverAcquireModel(num, mesh);
	printf("num %d mesh %f %f %f \n", num, mesh[10].p0.x, mesh[10].p0.y, mesh[10].p0.z);
	
	lzRGB rgb;
	int w = 100;
	int h = 100;

	return true;
}

void kinectDriverThread()
{
	while (bIsKinectThreadRun)
	{
		if (!kinectLoop())
		{
			Sleep(100);
			RunLoopUpdateFailCount--;
			if (RunLoopUpdateFailCount < 0)
			{
				bIsKinectThreadRun = false;
			}
			continue;
		}
		
		RunLoopUpdateFailCount = 50;
		RunLoopCount--;
		if (RunLoopCount < 0)
		{
			bIsKinectThreadRun = false;
		}
	}
}


TEST(KinectDriver, capture)
{
	lzBool ret = false;

	lzKinectDriverOpenLog(true);
	ret = lzKinectDriverOpenSensor();
	if (ret == false)
	{
		std::cout << " open sensor fail" << std::endl;
	}

	bIsKinectThreadRun = true;
	thread th1(kinectDriverThread);
	th1.join();
	
	lzKinectDriverCloseSensor();
	lzKinectDriverCloseLog();
	EXPECT_EQ(0, 0);
}


//TEST(KinectDriver, lzKinectDriverReadRawData)
//{
//	lzBool ret = false;
//
//	lzRGBX** ppRGBX = new lzRGBX*[1];
//	lzInt32** ppDepth = new lzInt32*[1];
//	string filename = "file_0.bin";
//
//	lzMatSize colorSz = { 1920, 1080 };
//	lzMatSize depthSz = { 512,  424 };
//
//	lzKinectDriverReadRawData(filename, ppRGBX, ppDepth, colorSz, depthSz);
//
//	delete[](*ppRGBX);
//	delete[](*ppDepth);
//	delete[](ppRGBX);
//	delete[](ppDepth);
//
//	EXPECT_EQ(0, 0);
//}