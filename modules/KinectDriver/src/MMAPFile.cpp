#include "MMAPFile.h"

CMMAPFile::CMMAPFile()
{
	hFile = NULL;
	hMapFile = NULL;
	m_size_low = 0;
	m_size_high = 0;
	pMVFile = NULL;
	iFileIsOpen = 0;
}


CMMAPFile::~CMMAPFile()
{
}


void* CMMAPFile::MMAP_CreateFile(IN string filename, 
								IN int mode, 
								IN unsigned long OffSet,
								IN unsigned long size_low, 
								IN unsigned long size_high)
{
	if (!iFileIsOpen)
	{
		MMAP_Release();
	}
	
	//WCHAR wszFileName[50];
	//memset(wszFileName, 0, sizeof(wszFileName));
	//MultiByteToWideChar(CP_ACP, 0, filename.c_str(), strlen(filename.c_str()) + 1, wszFileName,
	//	sizeof(wszFileName) / sizeof(wszFileName[0]));

	LPCSTR wszFileName = filename.c_str();

	hFile = CreateFile(
		wszFileName,   //�ļ���
		GENERIC_READ | GENERIC_WRITE, //���ļ����ж�д����
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		mode,
		FILE_ATTRIBUTE_NORMAL,
		0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	//����ֵm_size_high,m_size_low�ֱ��ʾ�ļ���С�ĸ�32λ/��32λ
	if ((0 == size_low) || (0 == size_high))
	{
		m_size_low = size_low;
		m_size_high = size_high;
	}
	else
	{
		m_size_low = GetFileSize(hFile, &m_size_high);
	}

	m_size_low = m_size_low + OffSet;
	//�����ļ����ڴ�ӳ���ļ���
	hMapFile = CreateFileMapping(
		hFile,
		NULL,
		PAGE_READWRITE,  //��ӳ���ļ����ж�д
		m_size_high,
		m_size_low,   //������������64λ������֧�ֵ�����ļ�����Ϊ16EB
		NULL);
	if (hMapFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return NULL;
	}

	//���ļ�����ӳ�䵽���̵ĵ�ַ�ռ�
	pMVFile = MapViewOfFile(
		hMapFile,
		FILE_MAP_READ | FILE_MAP_WRITE,
		0,
		0,
		0);

	if (!pMVFile)
	{
		CloseHandle(hFile);
		CloseHandle(hMapFile);
		return NULL;
	}

	iFileIsOpen = 1;
	return pMVFile;
}

bool CMMAPFile::MMAP_Release()
{
	if (0 == iFileIsOpen)
	{
		return true;
	}

	iFileIsOpen = 0;

	if (hFile)
	{
		CloseHandle(hFile);
	}

	if (hMapFile)
	{
		CloseHandle(hMapFile);
	}

	if (pMVFile)
	{
		UnmapViewOfFile(pMVFile);
	}

	return true;
}

unsigned long CMMAPFile::MMAP_GetFileSize(IN string filename, OUT unsigned long* size_high)
{
	//WCHAR wszFileName[50];
	//memset(wszFileName, 0, sizeof(wszFileName));
	//MultiByteToWideChar(CP_ACP, 0, filename.c_str(), strlen(filename.c_str()) + 1, wszFileName,
	//	sizeof(wszFileName) / sizeof(wszFileName[0]));

	LPCSTR wszFileName = filename.c_str();
	HANDLE FileHandle = CreateFile(
		wszFileName,   //�ļ���
		GENERIC_READ | GENERIC_WRITE, //���ļ����ж�д����
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);
	if (FileHandle == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	//����ֵm_size_high,m_size_low�ֱ��ʾ�ļ���С�ĸ�32λ/��32λ
	unsigned long l_size_low;
	unsigned long l_size_high;
	l_size_low = GetFileSize(FileHandle, &l_size_high);

	CloseHandle(FileHandle);
	*size_high = l_size_high;
	return l_size_low;
}


void* CMMAPFile::MMAP_GetFilePtr()
{
	if (iFileIsOpen)
	{
		return pMVFile;
	}

	return NULL;
}