#ifndef JETTON_CONTROL_HEAD_FILE
#define JETTON_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

struct JettonLayerInfo
{
	BYTE cbLayerCount;
	BYTE cbDrawIndex;
};
//////////////////////////////////////////////////////////////////////////

//����ؼ�
class CJettonControl
{
	//�������
protected:
	LONGLONG						m_lUserHaveCount;					//���г���
	LONGLONG						m_lCellJetton;						//��Ԫ����
	LONGLONG						m_lUserJetton;						//������Ŀ
	LONGLONG						m_lUserAddJetton;					//��ע��Ŀ
	LONGLONG						m_lMinJetton;						//���ٳ���
	LONGLONG						m_lScoreIndex[JETTON_COUNT];		//������Ŀ
	BYTE							m_cbChangeStatus[CONTROL_COUNT];	//����״̬
	BYTE							m_cbFirstIndex;						//��ǰ����
	BYTE							m_cbMaxLayerCount;					//�������
	JettonLayerInfo					m_AddLayerInfo[CONTROL_COUNT];		//������Ϣ
	JettonLayerInfo					m_UserLayerInfo[CONTROL_COUNT];		//������Ϣ

	//�滭����
protected:
	CSize							m_BigJettonSize;					//�����С
	CSize							m_SmaJettonSize; 					//�����С
	CSize							m_JettonBackSize; 					//������С
	CPoint							m_UserJettonlPost;					//��׼λ��
	CPoint							m_AddJettonPost;					//��עλ��

	//��������
protected:
	CBitImage						m_ImageBigJetton;					//����ͼƬ
	CBitImage						m_ImageSmaJetton;					//����ͼƬ
	CBitImage						m_ImageJettonBack;					//���뱳��
	CSkinButton						*m_pbtDecrease;						//��ע��ť
	CSkinButton						*m_pbtClearJetton;					//����ť
	CSkinButton						*m_pbtAdd;							//��ע��ť
	
	//��������
public:
	//���캯��
	CJettonControl();
	//��������
	virtual ~CJettonControl();

	//���ܺ���
public:
	//��������
	void SetUserHaveCount(LONGLONG lHaveCound);
	//���ð�ť
	void SetButton(CSkinButton *btDecrease,CSkinButton *pbtClearJetton,CSkinButton *pbtAdd);
	//���¿ؼ�
	void UpdataButton();
	//�滭λ��
	void SetControlPost(CPoint ControlPost,CPoint AddPost);
	//��Ԫ����
	void SetJettonLayer(BYTE cbJettonLayer);
	//���õ�Ԫ
	void SetCellJetton(LONGLONG lCellJetton);
	//��С����
	void SetMinJetton(LONGLONG lMinJetton);
	//���ó���
	void SetUserJetton(LONGLONG lUserJetton);
	//���ó���
	void SetAddJetton(LONGLONG lAddJetton);
	//�������
	void CountJettonInfo(LONGLONG lScore,JettonLayerInfo LayerInfo[CONTROL_COUNT],BYTE cbChangeIndex=0);
	//����ж�
	bool EstimateHitJetton(CPoint CurrentPoint);
	//�ƶ��ж�
	bool EstimateMove(CPoint CurrentPoint);
	//���ٳ���
	bool DecreaseJetton(WORD wControlID);
	//�����ע
	void ClearAddJetton();
	//��ȡ����
	LONGLONG GetUserJetton(){return m_lUserJetton;}
	//��ȡ����
	LONGLONG GetAddJetton(){return m_lUserAddJetton;}
	//��ȡ����
	LONGLONG GetMinJetton(){return m_lMinJetton;}
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//�滭����
public:
	//�滭����
	void DrawJettonControl(CDC * pDC);
};

//////////////////////////////////////////////////////////////////////////

#endif
