#ifndef GOLD_VIEW_HEAD_FILE
#define GOLD_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"

struct JettonHeapInfo
{
	BYTE cbLayerCount;
	BYTE cbDrawIndex;
};

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CGoldView
{
	//��������
public:
	bool								m_bBeelinePut;					//ֱ�߰ڷ�
	LONGLONG							m_lGold;						//������Ŀ
	LONGLONG							m_lMaxLayer;					//������
	LONGLONG							m_lCellScore;					//��Ԫע��
	LONGLONG							m_lScoreIndex[JETTON_COUNT];	//������Ŀ
	LONGLONG							m_lDrawOrder[JETTON_COUNT];		//�滭˳��
	JettonHeapInfo						m_JettonHeapInfo[JETTON_COUNT];	//������Ŀ
	BYTE								m_cbFirstIndex;					//��עλ��
	CSize								m_JettonSize;					//�����С
	CPoint								m_ptDrawJetton[JETTON_COUNT];	//�滭λ��

	//��������
protected:
	static bool							m_bInit;						//��ʼ��־
	static CBitImage					m_ImageGold;					//����ͼƬ

	//��������
public:
	//���캯��
	CGoldView();
	//��������
	virtual ~CGoldView();

	//���ܺ���
public:
	//���ðڷ�
	void SetBeelinePut(bool bBeelinePut);
	//���ó���
	void SetGold(LONGLONG lGold);
	//���õ�Ԫ
	void SetCellJetton(LONGLONG lCellJetton);
	//��ȡ����
	LONGLONG GetGold() { return m_lGold; };
	//���ò���
	void SetMaxGoldLayer(LONGLONG lMaxLayer);
	//��ȡ����
	LONGLONG GetMaxGoldLayer(){return m_lMaxLayer;}
	//�滭����
	void DrawGoldView(CDC * pDC, int nXPos, int nYPos, bool bCount,bool bCenter=false);
	//���Ա��ַ�
	LPCTSTR GetGlodString(LONGLONG lGold, TCHAR szString[]);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//�ڲ�����
private:
	//���������
	void RectifyGoldLayer();
};

//////////////////////////////////////////////////////////////////////////

#endif
