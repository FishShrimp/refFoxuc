#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "HistoryScore.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_PASS_CARD				(WM_USER+102)						//��������
#define IDM_LAND_SCORE				(WM_USER+103)						//�з���Ϣ
#define IDM_AUTO_OUTCARD			(WM_USER+104)                       //��ʾ��Ϣ
#define IDM_LAND_AUTOPLAY			(WM_USER+105)                       //�й���Ϣ
#define IDM_SORT_HAND_CARD			(WM_USER+106)						//�����˿�
#define IDM_MUSIC					(WM_USER+110)						//������Ϣ

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//��Ϸ����
protected:
	WORD							m_wLandUser;						//�����û�
	WORD							m_wBombTime;						//ը������
	LONGLONG							m_lBaseScore;						//��������
	BYTE							m_cbLandScore;						//��������
	BYTE							m_bLeftCardCount;

	//״̬����
public:
	bool							m_bLandTitle;						//������־
	bool							m_bPass[GAME_PLAYER];				//��������
	BYTE							m_bScore[GAME_PLAYER];				//�û��з�
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	bool							m_bUserTrustee[GAME_PLAYER];		//����й�
	bool							m_bShowScore;						//����״̬
	tagHistoryScore	*				m_pHistoryScore[GAME_PLAYER];		//������Ϣ

	//��ը����
protected:
	bool							m_bBombEffect;						//��ըЧ��
	BYTE							m_cbBombFrameIndex;					//֡������

	//λ����Ϣ
public:
	CSize							m_LandSize;							//�ڿӱ�־
	CPoint							m_ptPass[GAME_PLAYER];
	CPoint							m_ptWarningCard[GAME_PLAYER];		//����λ��

	//λͼ����
protected:
	//CBitImageEx						m_ImageBomb;						//ը��ͼƬ
	CBitImageEx						m_ImageBack;						//������Դ
	CBitImageEx						m_ImageCenter;						//������Դ
	CBitImageEx						m_ImageBombEffect;					//ը��Ч��
	CBitImageEx						m_ImageHistoryScore;				//��ʷ����
	CBitImageEx						m_ImageNumber;						//������Դ
	CBitImageEx						m_ImageLeftCard;
	CBitImageEx						m_ImageLeftCardText;
	CBitImageEx						m_ImageFrame;

	CPngImageEx						m_ImageScore;						//����ͼƬ
	CPngImageEx						m_ImageLand;						//ׯ��ͼƬ
	CPngImageEx						m_ImageUserTrustee;					//����й�
	CPngImageEx						m_ImageCardCount;

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//������ť
	CSkinButton						m_btAutoOutCard;					//��ʾ��ť
	CSkinButton						m_btSortCard;						//����ť
	CSkinButton						m_btAutoPlayOn;						// �����Զ��й�
	CSkinButton						m_btAutoPlayOff;					// �ر��Զ��й�
	CSkinButton						m_btScore;							//���ְ�ť
	CSkinButton						m_btMusic;

	//�˿˿ؼ�
public:
	CCardControl					m_BackCardControl;					//�����˿�
	CCardControl					m_UserCardControl[GAME_PLAYER];		//�˿���ͼ
	CCardControl					m_HandCardControl[GAME_PLAYER];		//�����˿�

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ
	CSize							m_sizeHistory;						//���ִ�С


	//������Դ
	CPngImageEx ImageTimeBack;
	CPngImageEx ImageTimeNumber;
	CPngImageEx ImageUserReady;

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();
	//WIN7֧��
	virtual bool RealizeWIN7() { return true; }
	//���غ���
private:
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);

	//���غ���
public:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
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
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos,int nWidth, int nWeight,UINT nFormat=DT_LEFT);
	//����й�
	void SetUserTrustee( WORD wChairID, bool bTrustee );
	//���û���
	bool SetHistoryScore(WORD wChairID,tagHistoryScore * pHistoryScore);
	//����ʣ����
	void SetLeftCardCount(BYTE bCount);

	void UpdateGameView(CRect *rc);

	//�ڲ�����
private:
	//�滭����
	VOID DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
	//�滭ʱ��
	void DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea=99);
	//�滭׼��
	void DrawUserReadyEx(CDC * pDC, int nXPos, int nYPos);
	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//��ʼ��ť
	afx_msg void OnStart();
	//���ư�ť
	afx_msg void OnOutCard();
	//������ť
	afx_msg void OnPassCard();
	//�зְ�ť
	afx_msg void OnOneScore();
	//�зְ�ť
	afx_msg void OnTwoScore();
	//������ť
	afx_msg void OnGiveUpScore();
	//������ʾ
	afx_msg void OnAutoOutCard();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//�зְ�ť
	afx_msg void OnThreeScore();
	// �Զ��й�
	afx_msg void OnAutoPlayerOn();
	// ȡ���й�
	afx_msg void OnAutoPlayerOff();
	//����ť
	VOID OnBnClickedSortCard();
	//���ְ�ť
	VOID OnBnClickedScore();
	//��������
	VOID OnBnClickedMusic();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif