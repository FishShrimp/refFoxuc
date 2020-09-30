#ifndef DRAW_WND_HEAD_FILE
#define DRAW_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////
#define IDM_GAME_START			(WM_USER+401)						//�˿˲���
#define IDM_CARD_MOVE			(WM_USER+402)						//�ƶ�����
#define IDM_SICE_TWO			(WM_USER+403)						//�ƶ�����

//////////////////////////////////////////////////////////////////////////

//���ƴ���
class CDrawSiceWnd : public CWnd
{
	//���ñ���
protected:
	CBitImage						m_ImageBack;						//����ͼ��
	CBitImage						m_ImageDrawSiceFirst;				//��������
	CBitImage						m_ImageDrawSiceSecond;				//��������
	CBitImage						m_ImageDrawSiceUserFirst;			//��������
	CBitImage						m_ImageDrawSiceUserSecond;			//��������
	CBitImage						m_ImageKingSiceFirst;				//��������
	CBitImage						m_ImageKingSiceSecond;				//��������
	BYTE                            m_bSicePos;							//����֡��
	int								m_nXPos;							//Xλ��
	int								m_nYPos;							//Yλ��
	CDC								*ParentDC;							//CDC����	
	//��������
public:
	//���캯��
	CDrawSiceWnd();
	//��������
	virtual ~CDrawSiceWnd();

	//�ڲ�����
public:
	void SetSiceInfo(CDC *dc,BYTE bMoveTime,BYTE SiceFirst,BYTE SiceSecond,BYTE SiceUserFirst,BYTE SiceUserSecond,BYTE KingSiceFirst,BYTE KingSiceSecond);
	void SetBenchmarkPos(int nXPos, int nYPos);
	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif