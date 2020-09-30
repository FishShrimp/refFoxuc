#ifndef OPENGL_ENGINE_HEAD_HEAD_FILE
#define OPENGL_ENGINE_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

//MFC �ļ�
#include <Afxmt.h>
#include <AtlBase.h>

//OPEN ��
#include <GL/GL.h>		
#include <GL/GLU.h>
#include <GL/GLAux.h>

//ƽ̨�ļ�
#include "Platform.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef OPENGL_ENGINE_CLASS
	#ifdef  OPENGL_ENGINE_DLL
		#define OPENGL_ENGINE_CLASS _declspec(dllexport)
	#else
		#define OPENGL_ENGINE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define OPENGL_ENGINE_DLL_NAME	TEXT("OpenglEngine.dll")			//�������
#else
	#define OPENGL_ENGINE_DLL_NAME	TEXT("OpenglEngineD.dll")			//�������
#endif

//////////////////////////////////////////////////////////////////////////////////
//�ṹ����

#define  GL_PI			((FLOAT)  3.141592654f)

#define  GLCOLOR_RGB(r,g,b)	    ((COLORREF)((((255)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define  GLCOLOR_RGBA(r,g,b,a)  ((COLORREF)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define  GLCOLOR_ARGB(a,r,g,b)  ((COLORREF)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define  GLCOLOR_GetR(rgb)      ((BYTE)((rgb)>>16))
#define  GLCOLOR_GetG(rgb)      ((BYTE)((rgb)>> 8))
#define  GLCOLOR_GetB(rgb)      ((BYTE)((rgb)>> 0))
#define  GLCOLOR_GetA(rgb)      ((BYTE)((rgb)>>24))

/////////////////////////////////////////////////////////////////////////////
// ��������
enum ENUM_TEXTURE_FORMATS{
	TEXTURE_FORMAT_UNKNOWN = 0,
	TEXTURE_FORMAT_BMP = 1,
	TEXTURE_FORMAT_GIF = 2,
	TEXTURE_FORMAT_JPG = 3,
	TEXTURE_FORMAT_PNG = 4,
	TEXTURE_FORMAT_ICO = 5,
	TEXTURE_FORMAT_TIF = 6,
	TEXTURE_FORMAT_TGA = 7,
	TEXTURE_FORMAT_PCX = 8,
	TEXTURE_FORMAT_WBMP = 9,
	TEXTURE_FORMAT_WMF = 10,
	TEXTURE_FORMAT_JP2 = 11,
	TEXTURE_FORMAT_JPC = 12,
	TEXTURE_FORMAT_PGX = 13,
	TEXTURE_FORMAT_PNM = 14,
	TEXTURE_FORMAT_RAS = 15,
	TEXTURE_FORMAT_JBG = 16,
	TEXTURE_FORMAT_MNG = 17,
	TEXTURE_FORMAT_SKA = 18,
	TEXTURE_FORMAT_RAW = 19,
};



//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

#ifndef OPENGL_ENGINE_DLL
	#include "OpenGL.h"
	#include "GLTexture.h"
	#include "GLFont.h"
	#include "Particles.h"
	#include "GLVirtualWindow.h"
	#include "GLVirtualButton.h"
#endif

//////////////////////////////////////////////////////////////////////////

#endif