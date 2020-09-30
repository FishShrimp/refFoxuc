#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView : public CDialog
{
	//��������
protected:
	LONGLONG						m_lGameTax;							//��Ϸ˰��
	LONGLONG						m_lGameScore[GAME_PLAYER];			//��Ϸ�÷�
	TCHAR							m_szUserName[GAME_PLAYER][LEN_ACCOUNTS];//�û�����

	//��Դ����
protected:
	CBitImage						m_ImageBack;						//����ͼ��	
    CSkinResourceManager *          m_pSkinResourceManager;             //��Դ������

	//�ؼ�����
public:
	//��ť�ؼ�
	CSkinButton						m_btEnsure;							//ȷ����ť

	//��������
public:
	//���캯��
	CScoreView(void);
	//��������
	virtual ~CScoreView(void);
	
	//���غ���
public:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK() { return; }
	//ȡ����Ϣ
	virtual void OnCancel() { return; }

	//��Ϣӳ��
protected:
	//ȷ����ť
	afx_msg void OnEnsureButton() {ShowWindow(SW_HIDE);}

	//���ܺ���
public:
	//���û���
	void ResetScore();
	//����˰��
	void SetGameTax(LONGLONG lGameTax);
	//���û���
	void SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONGLONG lScore);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};

//////////////////////////////////////////////////////////////////////////

#endif
