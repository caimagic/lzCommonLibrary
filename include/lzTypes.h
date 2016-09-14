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

// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 LZ_KINECTDRIVER_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// LZ_KINECTDRIVER_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

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



