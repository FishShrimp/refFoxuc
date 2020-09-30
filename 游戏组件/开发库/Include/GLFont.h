#pragma once

#include "OpenGL.h"
#include "OpenglEngineHead.h"

struct mapTextInfo
{
	UCHAR*	pBits;					//BITMAPָ��
	INT		nWidtht;				//���ֿ��
	INT		nHeight;				//���ָ߶�
	DWORD	dwRetentionTime;		//ͣ��ʱ��

	mapTextInfo( UCHAR* pParBits, INT nParWidtht, INT nParHeight )
	{
		pBits = pParBits;
		nWidtht = nParWidtht;
		nHeight = nParHeight;
		dwRetentionTime = timeGetTime();
	}
};
typedef map< CString , mapTextInfo >			mapText;


class OPENGL_ENGINE_CLASS CGLFont  
{

protected:
	HFONT							m_hFont;				// ����
	HFONT							m_hOldFont;				// ������
	HDC								m_Hdc;					// ���ھ��
	mapText							m_mapText;				// �������


	// 
public:
	CGLFont();
	virtual ~CGLFont();

	// ��������
public:
	// ɾ������
	bool DeleteFont();
	// ɾ������
	bool DeleteCache();
	// ��������
	bool CreateFont(int nHeight, int nWeight, LPCTSTR lpszFaceName);

	// �������
public:
	// �������
	bool DrawText(CGLDevice* pOpenGL, LPCTSTR pszString, CRect rcDraw, UINT uFormat, COLORREF GLColor);
	// �������
	bool TextOut(CGLDevice* pOpenGL, LPCTSTR pszString, int nPosX, int nPosY, COLORREF GLColor);

	// ��ȡ���ִ�С
	CSize GetTextSize(LPCTSTR pszString);


};

