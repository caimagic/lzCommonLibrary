#ifndef __MMAP_FILE_H__
#define __MMAP_FILE_H__

#include <string>
#include "Windows.h"
#include "lzTypes.h"

using namespace std;

#define CMMAPFILE_CREATE_NEW	1	//�����ļ������ļ�����������
#define CMMAPFILE_CREATE_ALWAYS 2	//�����ļ������дǰһ���ļ�
#define CMMAPFILE_OPEN_EXISTING 3	//�ļ������Ѿ����ڡ����豸���Ҫ��
#define CMMAPFILE_OPEN_ALWAYS	4	//���ļ��������򴴽���
#define CMMAPFILE_TRUNCATE_EXISTING 5 //�������ļ�����Ϊ�㳤��

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