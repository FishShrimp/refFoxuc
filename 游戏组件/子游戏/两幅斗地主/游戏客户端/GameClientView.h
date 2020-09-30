#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "..\��Ϸ������\HistoryRecord.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_PASS_CARD				(WM_USER+102)						//��������
#define IDM_LAND_SCORE				(WM_USER+103)						//�з���Ϣ
#define IDM_AUTO_OUTCARD			(WM_USER+104)                       //��ʾ��Ϣ
#define IDM_LAND_AUTOPLAY			(WM_USER+105)                       //�й���Ϣ
#define IDM_SORT_HAND_CARD			(WM_USER+106)						//�����˿�


//��������
#define ME_VIEW_CHAIR				2									//�Լ�λ��

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//��Ϸ����
protected:
	WORD							m_wLandUser;						//�����û�
	WORD							m_wBombTime;						//ը������
	LONGLONG							m_lBaseScore;						//��������
	BYTE							m_cbLandScore;						//��������
	bool							m_bShowScore;						//����״̬
	tagHistoryScore	*				m_pHistoryScore[GAME_PLAYER];		//������Ϣ

	//״̬����
public:
	bool							m_bLandTitle;						//������־
	bool							m_bPass[GAME_PLAYER];				//��������
	BYTE							m_bScore[GAME_PLAYER];				//�û��з�
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	bool							m_bDeasilOrder;						//˳ʱ���
	bool							m_bUserTrustee[GAME_PLAYER];		//����й�
	
	//��ը����
protected:
	bool							m_bBombEffect;						//��ըЧ��
	BYTE							m_cbBombFrameIndex;					//֡������

	//λ����Ϣ
public:
	CSize							m_LandSize;							//�ڿӱ�־
	CPoint							m_ptScore[GAME_PLAYER];				//�з�λ��
	CPoint							m_ptLand[GAME_PLAYER];				//����λ��
	CSize							m_sizeHistory;						//���ִ�С

	//λͼ����
protected:
	CBitImageEx						m_ImageBomb;						//ը��ͼƬ
	CBitImageEx						m_ImageBack;						//������Դ
	CBitImageEx						m_ImageCenter;						//������Դ
	CBitImageEx						m_ImageBombEffect;					//ը��Ч��
	CPngImageEx						m_ImageScore;						//����ͼƬ
	CPngImageEx						m_ImageLand;						//ׯ��ͼƬ
	CPngImageEx						m_ImageUserTrustee;					//����й�
	CBitImageEx						m_ImageHistoryScore;				//��ʷ����

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//������ť
	CSkinButton						m_btOneScore;						//1 �ְ�ť
	CSkinButton						m_btTwoScore;						//2 �ְ�ť
	CSkinButton						m_btThreeScore;						//3 �ְ�ť
	CSkinButton						m_btGiveUpScore;					//������ť
	CSkinButton						m_btAutoOutCard;					//��ʾ��ť
	CSkinButton						m_btSortCard;						//����ť
	CSkinButton						m_btAutoPlayOn;						// �����Զ��й�
	CSkinButton						m_btAutoPlayOff;					// �ر��Զ��й�
	CSkinButton						m_btScore;							//���ְ�ť

	//�˿˿ؼ�
public:
	CCardControl					m_BackCardControl;					//�����˿�
	CCardControl					m_UserCardControl[GAME_PLAYER];		//�˿���ͼ
	CCardControl					m_HandCardControl[GAME_PLAYER];		//�����˿�

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
private:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//WIN7֧��
	virtual bool RealizeWIN7() { return true; }
	//���غ���
public:
	//��Ϣ����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//ը������
	void SetBombTime(WORD wBombTime);
	//��������
	void SetBaseScore(LONGLONG lBaseScore);
	//��ʾ��ʾ
	void ShowLandTitle(bool bLandTitle);
	//���÷���
	void SetPassFlag(WORD wChairID, bool bPass);
	//�˿���Ŀ
	void SetCardCount(WORD wChairID, BYTE bCardCount);
	//�ڿӷ���
	void SetLandUser(WORD wChairID, BYTE bLandScore);
	//�ڿӷ���
	void SetLandScore(WORD wChairID, BYTE bLandScore);
	//��ȡ˳��
	bool IsDeasilOrder() { return m_bDeasilOrder; }
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos,int nWidth, int nWeight,UINT nFormat=DT_LEFT);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect,int nWidth, int nWeight,UINT nFormat=DT_LEFT);

	//����й�
	void SetUserTrustee( WORD wChairID, bool bTrustee );
	//���û���
	bool SetHistoryScore(WORD wChairID,tagHistoryScore * pHistoryScore);

	void UpdateGameView(CRect *rc);

	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
