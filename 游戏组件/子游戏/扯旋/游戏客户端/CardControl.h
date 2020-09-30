#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ֵ����
#define	CARD_MASK_COLOR				0xF0								//��ɫ����
#define	CARD_MASK_VALUE				0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////

//X ���з�ʽ
enum enXCollocateMode 
{ 
	enXLeft,						//�����
	enXCenter,						//�ж���
	enXRight,						//�Ҷ���
};

//Y ���з�ʽ
enum enYCollocateMode 
{ 
	enYTop,							//�϶���
	enYCenter,						//�ж���
	enYBottom,						//�¶���
};

//////////////////////////////////////////////////////////////////////////

//�˿˿ؼ�
class CCardControl : public CWnd
{
	//���ñ���
protected:
	bool							m_bFlash;							//��˸��־
	bool							m_bDisable;							//��Ч��־
	bool							m_bPositively;						//��Ӧ��־
	bool							m_bDisplayHead;						//��ʾ�˿�
	DWORD							m_dwCardHSpace;						//������

	//״̬����
protected:
	bool							m_bCaptureMouse;					//�����־
	CWHArray<BYTE>			m_CardDataArray;					//�˿�����

	//λ����Ϣ
protected:
	CRgn							m_CardRegion;						//�˿�����
	CPoint							m_BenchmarkPos;						//��׼λ��
	enXCollocateMode				m_XCollocateMode;					//��ʾģʽ
	enYCollocateMode				m_YCollocateMode;					//��ʾģʽ

	//��̬����
protected:
	static bool						m_bLoad;							//��ʼ��־
	static CSize					m_CardSize;							//�˿˴�С
	static CBitImage				m_ImageCard;						//ͼƬ��Դ
	CBitImage						m_ImageMark;						//ͼƬ��Դ

	//��������
public:
	//���캯��
	CCardControl();
	//��������
	virtual ~CCardControl();

	//���ƺ���
public:
	//������˸
	void SetCardFlash(bool bFlash);
	//������Ч
	void SetDisableCardBack(bool bDisable);
	//���ü��
	void SetCardSpace(DWORD dwCardSpace);
	//������Ӧ
	void SetPositively(bool bPositively);
	//��ʾ�˿�
	void SetDisplayHead(bool bDisplayHead);
	//��׼λ��
	void SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��׼λ��
	void SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��ȡλ��
	CPoint GetCardPos(bool bMaxCount=TRUE);
	//��ȡ��־
	bool GetCardDisable(){return m_bDisable;}
	//��ȡ��־
	bool GetPositively(){return m_bPositively;}
	//��ȡ��־
	bool GetDisplayHead(){return m_bDisplayHead;}
	//��ȡ��־
	bool GetCardFlash(){return m_bFlash;}
	//��������
	void AnalyseMingCard();

	//���ܺ���
public:
	//��ȡ��Ŀ
	DWORD GetCardCount();
	//��ȡ��Ŀ
	DWORD GetUnDisableCardCount();
	//�����˿�
	DWORD GetCardData(BYTE cbCardData[], DWORD dwMaxCount);
	//�����˿�
	BYTE GetCardData(BYTE cbIndex);
	//�����˿�
	DWORD SetCardData(const BYTE bCardData[], DWORD dwCardCount);
	//�����˿�
	DWORD SetNullityCardData(BYTE cbIndex,BYTE cbCardData=0xff);
	//�����˿�
	DWORD GetNullityCardData(BYTE cbCardData=0xff);

	//�ڲ�����
private:
	//����λ��
	void RectifyControl();

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
