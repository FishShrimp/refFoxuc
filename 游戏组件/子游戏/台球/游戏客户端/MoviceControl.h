#pragma once

#include "Stdafx.h"
#include <list>
//#include <iostream>
#include <gdiplus.h>
#include <math.h>
//#include "Picture.h"
/*
	�������ڻ���������
	by liu
*/
using namespace std;
class CMoviceControl
{
public:
	CMoviceControl();
	bool  Create(LPCTSTR LoadPatch,BYTE MaxFrame,LPCTSTR ImgType,Color colorAlpha);
	void  Destroy();
	void  DrawMovice(CDC *pDc);
	//��֡���ۼ�
	bool  AddFrame(bool bIsAlwaysDraw = false);
	//��λ
	void  ResetPlace(int beginx,int beginy,int Pace = 0,BYTE DrawNum = 1,int Angle = 0,bool IsRotate = false);
	void  EndMovice();
	BOOL  IsDrawMove(){return m_bIsDraw;}
	CRect GetDrawRect();
	int   GetWidth(){return m_iWidth;}
	int   GetHeight(){return m_iHeight;}
private:
	BYTE		m_byCurFrame;	//��ǰ������֡��
	BYTE		m_byMaxFrame;	//���֡��
	BYTE		m_byCopyDrawNum;//ͬʱ���Ĵ���
	BOOL		m_bIsDraw;		//�Ƿ�



	list<Image*>	m_ListMovice;		//ͼƬ
	
	ImageAttributes m_remapAttributes;  //��ͼ������
	Point			m_beginPoints[3];	//��ʼλ��
	Point			m_drawPoints[3];	//����λ��

	int				m_iPace;			//�ٶ�
	Point			m_endPoint;			//������
	int				m_iAngle;			//�Ƕ�
	int			    m_iWidth;			//���
	int 			m_iHeight;			//�߶�
	bool			m_bIsRotate;		//�Ƿ���ת
};