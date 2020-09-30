#pragma once

#include "Stdafx.h"
#include "JettonControl.h"
#include "NumberControl.h"
#include "CardControl.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ͼλ��
#define MYSELF_VIEW_ID				2									//��ͼλ��

#define AC_GIVE_CARD				1									//Ҫ�ƶ���
#define AC_STOP_CARD				2									//ͣ�ƶ���
#define AC_ADD_SCORE				3									//�ӱ�����

//������Ϣ
#define IDM_START					(WM_USER+200)						//��ʼ��Ϣ
#define IDM_GIVE_CARD				(WM_USER+201)						//Ҫ����Ϣ
#define IDM_STOP_CARD				(WM_USER+202)						//ͣ����Ϣ
#define IDM_ADD_SCORE				(WM_USER+207)						//�ӱ���Ϣ
#define IDM_USER_SCORE				(WM_USER+208)						//��ע��Ϣ

//֪ͨ��Ϣ
#define IDM_SEND_CARD_FINISH		(WM_USER+300)						//�������
#define IDM_MOVE_JET_FINISH			(WM_USER+301)						//�ƶ��������

//////////////////////////////////////////////////////////////////////////

//��������
struct tagSendCardItem
{
	WORD							wChairID;							//�����û�
	BYTE							cbCardData;							//��������
};

//����˵��
typedef CWHArray<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

///////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//״̬����
protected:
	LONGLONG						m_lUserScore[GAME_PLAYER];			//�û�����
	LONGLONG						m_lUserAddScore[GAME_PLAYER];		//�û���ע��
	FLOAT							m_fUserCardScore[GAME_PLAYER];		//�û��˿˷�
	LONGLONG						m_lCellScore;						//��Ԫ����
	LONGLONG						m_lJettonScore;						//�ܳ�����
	WORD							m_wBankerUser;						//ׯ��
	bool							m_bWaitUserScore;					//�ȴ���ע
	bool							m_bShowAddJetton;					//��ʾ��ʶ

	//���ƶ�������
protected:
	CPoint							m_SendCardPos;						//����λ��
	CPoint							m_SendCardCurPos;					//���Ƶ�ǰλ��
	INT								m_nStepCount;						//����
	INT								m_nXStep;							//����
	INT								m_nYStep;							//����
	CSendCardItemArray				m_SendCardItemArray;				//��������

	//��������
protected:
	WORD							m_wActionUser;						//�����û�
	BYTE							m_cbUserAction;						//������ʶ

	//λ�ñ���
protected:
	CPoint							m_ptUserAction[GAME_PLAYER];		//����λ��
	CPoint							m_ptJettons[GAME_PLAYER];			//����λ��
	CPoint							m_ptBanker[GAME_PLAYER];			//ׯ��λ��

	INT								m_nXFace;
	INT								m_nYFace;
	INT								m_nXTimer;
	INT								m_nYTimer;

	//��С����
	CSize							m_SizeGameCard;						//�˿˴�С

	//��Դ����
protected:
	CBitImage						m_ImageGameCard;					//�˿���Դ
	CBitImage						m_ImageViewFill;					//������Դ
	CBitImage						m_ImageViewBack;					//������Դ
	CPngImage						m_PngActionBack;					//��������
	CPngImage						m_PngActionFont;					//����
	CPngImage						m_PngNumber;						//�˿˷�����
	CPngImage						m_PngScoreNum;						//���������
	CPngImage						m_PngActionScoreBack;				//��ע��ʾ
	CPngImage						m_PngActionScoreNum;				//��ע��ʾ
	CPngImage						m_PngBkTopLeft;						//
	CPngImage						m_PngBkTopRight;					//
	CPngImage						m_PngBkBottomLeft;					//
	CPngImage						m_PngBkBottomRight;					//
	CBitImage						m_ImageTitle;						//����λͼ
	CPngImage						m_PngCardScore;						//�Ƶ�
	CPngImage						m_PngBanker;						//ׯ�ұ�־
	CPngImage						m_PngWaitScore;						//�ȴ���ע

	//���ư�ť
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btGiveCard;						//Ҫ�ư�ť
	CSkinButton						m_btStopCard;						//ͣ�ư�ť
	CSkinButton						m_btAddScore;						//�ӱ���ť
	CSkinButton						m_btOneScore;						//һ����ť
	CSkinButton						m_btTwoScore;						//������ť
	CSkinButton						m_btThreeScore;						//������ť
	CSkinButton						m_btFourScore;						//�ı���ť

	//�ؼ�����
public:
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CCardControl					m_CardControl[GAME_PLAYER];			//�˿˿ؼ�
	CJettonControl					m_JettonControl;
	CNumberControl					m_NumberControl[GAME_PLAYER];

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//���غ���
public:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

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

	//���ܺ���
public:
	//���û���
	bool SetScoreInfo(WORD wChairID, LONGLONG lUserScore);
	//���ö���
	bool SetUserAction(WORD wActionUser, BYTE cbUserAction);
	//���������
	VOID SetCardScore( WORD wChairId, FLOAT fCardScore );
	//���õ�Ԫ��
	VOID SetCellScore( LONGLONG lScore );
	//������Ϸ������Ϣ
	VOID SetGameEndInfo( WORD wWinner );
	//����ׯ��
	VOID SetBankerUser( WORD wBankerUser );
	//���õȴ�
	VOID SetWaitUserScore( bool bWaitUserScore );
	//������ʾ
	VOID ShowAddJettonInfo( bool bShow );

	//��������
public:
	//�Ƿ���
	bool IsDispatchStatus();
	//ֹͣ����
	VOID ConcludeDispatch();
	//�����˿�
	VOID DispatchUserCard(WORD wChairID, BYTE cbCardData);

	//��Ҽ�ע
	VOID OnUserAddJettons( WORD wChairId, LONGLONG lScore );
	//��ҵ÷�
	VOID OnUserRemoveJettons( WORD wChairId, LONGLONG lScore );
	//��ʼ���붯��
	VOID BeginMoveJettons();
	//ֹͣ���붯��
	VOID StopMoveJettons();

	//��ʼ���ֶ���
	VOID BeginMoveNumber();
	//ֹͣ���ֶ���
	VOID StopMoveNumber();

	//��ť��Ϣ
public:
	//��ʼ��ť
	VOID OnBnClickedStart();
	//��ע
	VOID OnBnClickedGiveCard();
	//����
	VOID OnBnClickedStopCard();
	//�ӱ�
	VOID OnBnClickedAddScore();
	//1����ע
	VOID OnBnClickedOneScore();
	//2����ע
	VOID OnBnClickedTwoScore();
	//3����ע
	VOID OnBnClickedThreeScore();
	//4����ע
	VOID OnBnClickedFourScore();

	//��Ϣ����
public:
	//��������
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ʱ��
	VOID OnTimer(UINT nIDEvent);
	//���
	VOID OnLButtonDown(UINT nFlags, CPoint point);
	//�������
	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//������ͼ
	void RefreshGameView();
	//������ͼ
	void RefreshGameView(CRect rect);

	//�ڲ�����
protected:
	//�滭����,����bHasSign��ʾλͼ�Ƿ�������
	VOID DrawNumberString( CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, CPngImage &PngNumber, bool bHasSign );
	//�滭�ַ�
	bool DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, INT nXPos, INT nYPos);
	//�滭�Ƶ�
	VOID DrawCardScore( CDC * pDC, INT nXPos, INT nYPos, FLOAT fCardScore, bool bLeftAlign );

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////