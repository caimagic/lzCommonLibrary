#include "stdio.h"
#include "gtest/gtest.h"
#include "lz_KinectDriver/KinectDriver.h"
#include "thread"

TEST(KinectDriver, lzHello)
{
	lzInt32 ret = lzHello();

	EXPECT_EQ(ret, 500);
}


void myFirstThread()
{
	cout << "Hello thread" << endl;
}

TEST(KinectDriver, capture)
{
	lzKinectDriverOpenLog(true);
	lzKinectDriverCloseLog();

	thread myThread(myFirstThread);
	myThread.join();
	EXPECT_EQ(0, 0);
}
