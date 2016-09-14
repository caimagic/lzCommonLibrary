/**
 *
 * Copyright (C), 2016 Lang Qi (shanghai) Internet Tech. Co., Ltd.
 *
 * \brief          lzTypes.h c++ file
 * \file         : lzTypes.h
 * \version      : ALPHA
 * \author       : caimagic
 * \date         : 2016/9/13 18:46:51
 * \copyright    : Lang Qi (shanghai) Internet Tech. Co., Ltd.
 * \warning      : 
 *
 * Email         : caizhijie_hit@163.com
 * ClassList     :
 * History       : change history log, including date, author and brief content statement.
 *
 */
 
#ifndef _LZ_TYPES_H__
#define _LZ_TYPES_H__


/********************************************************************************************/
/*			1 Other Header File Including													*/
/********************************************************************************************/
#include "Version.h"

/********************************************************************************************/
/*			2 Macro Definition																*/
/********************************************************************************************/

// function input parameter flag
#ifdef IN
#undef IN
#define IN
#else
#define IN
#endif

// function output parameter flag
#ifdef OUT
#undef OUT
#define OUT
#else
#define OUT
#endif

// basic variables define
#define lzBool 		bool			
#define lzInt8 		char			
#define lzInt16 	short			
#define lzInt32 	int				
#define lzInt64 	long long       
#define lzFloat32 	float			
#define lzFloat64 	double			
#define lzUInt8 	unsigned char	
#define lzUInt16 	unsigned short	
#define lzUInt32 	unsigned int	
#define lzUInt64 	unsigned long long		
#define lzVoid 		void	

#define lzNULL      nullptr

// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� LZ_KINECTDRIVER_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// LZ_KINECTDRIVER_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

#ifdef LZ_EXPORTS
#define LZ_EXPORTS_API extern "C" __declspec(dllexport)
#else
#define LZ_EXPORTS_API extern "C" __declspec(dllimport)
#endif

/********************************************************************************************/
/*			3 Enum Type Definition															*/
/********************************************************************************************/


/********************************************************************************************/
/*			4 Global Variable Declaring														*/
/********************************************************************************************/


/********************************************************************************************/
/*			5 Struct Type Definition														*/
/********************************************************************************************/

typedef struct
{
	lzInt16 x;
	lzInt16 y;
} lzPoint2s;

typedef struct
{
	lzInt16 x;
	lzInt16 y;
	lzInt16 z;
} lzPoint3s;

typedef struct
{
	lzFloat32 x;
	lzFloat32 y;
} lzPoint2f;

typedef struct
{
	lzFloat32 x;
	lzFloat32 y;
	lzFloat32 z;
} lzPoint3f;

typedef struct
{
	lzFloat32 r;
	lzFloat32 g;
	lzFloat32 b;
} lzRGB;

typedef struct
{
	lzFloat32 r;
	lzFloat32 g;
	lzFloat32 b;
	lzFloat32 x;
} lzRGBX;

typedef struct 
{
	lzUInt16 width;
	lzUInt16 height;
} lzImgSize;


/********************************************************************************************/
/*			6 Class Type Definition															*/
/********************************************************************************************/


/********************************************************************************************/
/*			7 Others Definition																*/
/********************************************************************************************/


/********************************************************************************************/
/*			8 Function Declare																*/
/********************************************************************************************/

// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

#endif // _LZ_TYPES_H__



