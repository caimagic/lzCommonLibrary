#ifndef __MMAP_FILE_H__
#define __MMAP_FILE_H__

#include <string>
#include "Windows.h"
#include "lzTypes.h"

using namespace std;

#define CMMAPFILE_CREATE_NEW	1	//创建文件；如文件存在则会出错
#define CMMAPFILE_CREATE_ALWAYS 2	//创建文件，会改写前一个文件
#define CMMAPFILE_OPEN_EXISTING 3	//文件必须已经存在。由设备提出要求
#define CMMAPFILE_OPEN_ALWAYS	4	//如文件不存在则创建它
#define CMMAPFILE_TRUNCATE_EXISTING 5 //将现有文件缩短为零长度

class LZ_EXPORTS_API CMMAPFile
{
public:
	CMMAPFile();
	~CMMAPFile();
private:
	string m_filename;
	HANDLE hFile;
	HANDLE hMapFile;
	unsigned long m_size_low;
	unsigned long m_size_high;
	void* pMVFile;
	int iFileIsOpen;
public:
	void* MMAP_CreateFile(IN string filename,
						IN int mode = CMMAPFILE_OPEN_ALWAYS,
						IN unsigned long OffSet = 0,
						IN unsigned long size_low = 0,
						IN unsigned long size_high = 0);
	bool MMAP_Release();
	unsigned long MMAP_GetFileSize(IN string filename, OUT unsigned long* size_high);
	void* MMAP_GetFilePtr();
};

#endif