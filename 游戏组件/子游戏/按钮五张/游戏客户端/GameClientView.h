#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "CardControl.h"
#include "JettonControl.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

//��Ϣ����
#define IDM_START					WM_USER+100							//��ʼ��Ϣ
#define IDM_FOLLOW					WM_USER+102							//��ע��Ϣ
#define IDM_GIVE_UP					WM_USER+103							//������Ϣ
#define IDM_ADD_SCORE				WM_USER+104							//��ע��Ϣ
#define IDM_SHOW_HAND				WM_USER+105							//�����Ϣ
#define IDM_SEND_CARD_FINISH		WM_USER+106							//�������

#define MYSELF_VIEW_ID				2									//�Լ�λ��

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��������
struct tagSendCardItem
{
	WORD							wChairID;							//�����û�
	BYTE							cbCardData;							//��������
};

//����˵��
typedef CWHArray<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//���ݱ���
protected:
	LONGLONG						m_lUserScore[GAME_PLAYER];			//
	LONGLONG						m_lTableScore[GAME_PLAYER];			//��ע��Ŀ
	LONGLONG						m_lCellScore;						//��Ԫע
	bool							m_bUserShowHand;					//

	//��������
protected:
	CPoint							m_SendCardPos;						//����λ��
	CPoint							m_SendCardCurPos;					//���Ƶ�ǰλ��
	INT								m_nStepCount;						//����
	INT								m_nXStep;							//����
	INT								m_nYStep;							//����
	CSendCardItemArray				m_SendCardItemArray;				//��������

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btGiveUp;							//������ť
	CSkinButton						m_btFollow;							//��ע��ť
	CSkinButton						m_btShowHand;						//�����ť
	CSkinButton						m_btAddTimes3;						//
	CSkinButton						m_btAddTimes2;						//
	CSkinButton						m_btAddTimes1;						//

	//λ����Ϣ
protected:
	CPoint							m_ptTableScore[GAME_PLAYER];		//��ǰ��ע��Ϣ

	//�ؼ�����
public:
	CScoreView						m_ScoreView;						//�ɼ�����
	CCardControl					m_CardControl[GAME_PLAYER];			//�û��˿�
	CJettonControl					m_PlayerJeton[GAME_PLAYER+1];		//��ҳ���

	//�������
protected:
	CBitImage						m_ImageCard;						//�˿���Դ
	CBitImage						m_ImageViewBack;					//������Դ
	CBitImage						m_ImageViewFill;					//�����Դ
	CPngImage						m_PngNumber;						//����λͼ
	CPngImage						m_PngShowHand;						//

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//���غ���
protected:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//�̳к���
private:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(int nWidth, int nHeight);
	//���ƽӿ�
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//���ܺ���
public:
	//�����ж�
	bool IsDispatchCard();
	//��ɷ���
	void FinishDispatchCard();
	//�����˿�
	void DispatchUserCard(WORD wChairID, BYTE cbCardData);
	//������ע
	void SetUserTableScore(WORD wChairID, LONGLONG lTableScore);
	//���õ�Ԫע
	void SetCellScore( LONGLONG lCellScore );
	//
	void SetUserShowHand( bool bShowHand );
	//
	void SetUserScore( WORD wChairId, LONGLONG lScore );

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//
	VOID OnLButtonUp(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
