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

lzInt32 RunLoopCount = 20;
lzBool bIsKinectThreadRun = false;

lzBool kinectLoop()
{
	lzBool ret = lzKinectDriverUpdateFrame();
	if (ret == false)
	{
		return false;
	}

	char buf[40];
	sprintf(buf, "file_%d.bin", RunLoopCount);
	string filename = buf;
	std::cout << "buf " << buf << std::endl;
	lzKinectDriverSaveRawData(filename);

	return true;
}

void kinectDriverThread()
{
	while (bIsKinectThreadRun)
	{
		if (!kinectLoop())
		{
			Sleep(100);
			continue;
		}

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
