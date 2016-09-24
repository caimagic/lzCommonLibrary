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


void myFirstThread()
{
	cout << "Hello thread" << endl;
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




	thread myThread(myFirstThread);
	myThread.join();

	lzKinectDriverCloseLog();
	EXPECT_EQ(0, 0);
}
