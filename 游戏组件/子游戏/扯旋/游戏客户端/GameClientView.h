#pragma once

#include "Stdafx.h"
#include "GoldView.h"
#include "ScoreView.h"
#include "GameOption.h"
#include "CardControl.h"
#include "JetonControl.h"

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ
#define IDC_START						100								//��ʼ��ť
#define	IDC_INVEST						101								//�±���ť
#define	IDC_ADD_SCORE					102								//��ע��ť
#define	IDC_FOLLOW						103								//��ע��ť
#define	IDC_SHOW_HAND					104								//ȫѹ��ť
#define	IDC_PASS						105								//���Ӱ�ť
#define	IDC_GIVE_UP						106								//������ť
#define	IDC_CLEAR_SCORE					107								//����ť
#define	IDC_OPEN_CARD					108								//���ư�ť
#define	IDC_DECREASE1					109								//��ע��ť
#define	IDC_DECREASE2					110								//��ע��ť
#define	IDC_DECREASE3					111								//��ע��ť
#define	IDC_DECREASE4					112								//��ע��ť

//��Ϣ����
#define IDM_OPEN_CARD_FINISH		WM_USER+121							//�������
#define IDM_SEND_CARD_FINISH		WM_USER+120							//�������
#define IDM_START					WM_USER+100							//��ʼ��Ϣ
#define IDM_ADD_SCORE				WM_USER+102							//��ע��Ϣ
#define IDM_CLEAR_SCORE				WM_USER+112							//������Ϣ
#define IDM_OPEN_CARD				WM_USER+113							//������Ϣ
#define IDM_DECREASE				WM_USER+114							//��ע��Ϣ

#define	MY_VIEW_CHAIR_ID				2								//����ٶ�
//////////////////////////////////////////////////////////////////////////

//�ƶ�����
#define MOVE_USER_ADD				0								//��ע������ע
#define MOVE_POOL_ALLSCORE			1								//��ע���������м�
#define MOVE_USER_GETSCORE			2								//�мӳ���������ע
#define MOVE_BACK_SCORE				3								//���س���

//�ṹ����
//////////////////////////////////////////////////////////////////////////

//����״̬
struct tagJettonStatus
{
	//������Ϣ
	WORD							wMoveCount;							//�ƶ�����
	WORD							wMoveIndex;							//�ƶ�����

	//������Ϣ
	CPoint							ptFrom;								//����λ��
	CPoint							ptDest;								//Ŀ��λ��
	CPoint							ptCourse;							//����λ��
	LONGLONG						lGold;								//������Ŀ

	//�ƶ���ʽ
	INT								iMoveType;							//�ƶ���ʽ
};

//��������
struct tagSendCardItem
{
	WORD							wChairID;							//�����û�
	BYTE							cbCardData;							//��������
};

//����˵��
typedef CWHArray<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//��Ϸ����
protected:
	bool							m_bUserOffLine[GAME_PLAYER];		//�û�����				
	bool							m_bWaitCompareHeap;					//���ƱȽ�
	bool							m_bWaitCompareTail;					//���ƱȽ�
	bool							m_bWaitInvest;						//�ȴ��±�
	bool							m_bWaitOpenCard;					//�ȴ�̯��
	bool							m_bHitPositively;					//�����Ӧ
	bool							m_bUserOperate[GAME_PLAYER];		//�����û�
	WORD							m_wBankerUser;						//ׯ���û�
	BYTE							m_cbHitCardData[6];					//�����Ϣ
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�û�����
	LONGLONG						m_lCellScore;						//��Ԫע��
	LONGLONG						m_lUserScore[GAME_PLAYER];			//�û�����
	LONGLONG						m_lTableScore[GAME_PLAYER];			//������
	LONGLONG						m_lCenterScore;						//�ܽ����
	TCHAR							m_szHeapCard[GAME_PLAYER][LEN_NICKNAME];//ͷ������
	TCHAR							m_szTailCard[GAME_PLAYER][LEN_NICKNAME];//β������

	//��������
protected:
	WORD							m_wActionUser;						//�����û�
	WORD							m_wUserAction;						//������ʶ

	//��������
protected:
	WORD							m_wSendCount;						//�ƶ�����
	WORD							m_wSendIndex;						//�ƶ�����
	CSendCardItemArray				m_SendCardItemArray;				//��������

	//��������
protected:
	bool							m_bFlashCard;						//ɢ���־
	WORD							m_wDelayCount;						//�ӳٴ���
	WORD							m_wDelayIndex;						//�ӳٴ���
	WORD							m_wCurrentOpenUser;					//�����û�
	WORD							m_wHeapMaxUser;						//ͷ���û�

	//��������
public:
	bool							m_bJettonAction;					//������־
	tagJettonStatus					m_JettonStatus[GAME_PLAYER];		//������Ϣ

	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btInvest;							//�±���ť
	CSkinButton						m_btAddScore;						//��ע��ť
	CSkinButton						m_btFollow;							//��ע��ť
	CSkinButton						m_btShowHand;						//ȫѹ��ť
	CSkinButton						m_btPass;							//���Ӱ�ť
	CSkinButton						m_btGiveUp;							//������ť
	CSkinButton						m_btClearScore;						//����ť
	CSkinButton						m_btOpenCard;						//���ư�ť
	CSkinButton						m_btDecrease[CONTROL_COUNT];		//��ע��ť

	//�ؼ�����
public:
	CScoreView						m_ScoreView;						//�ɼ�����
	CCardControl					m_CardControl[GAME_PLAYER];			//�û��˿�
	CJettonControl					m_JetonControl;						//����ؼ�
	CGoldView						m_MoveGoldView[GAME_PLAYER];		//����ؼ�
	CGoldView						m_AddGoldView[GAME_PLAYER];			//����ؼ�
	CGoldView						m_UserGoldView[GAME_PLAYER];		//����ؼ�
	CGoldView						m_CenterGoldView;					//���ĳ���

	//λ����Ϣ
protected:
	CPoint							m_ptBanker[GAME_PLAYER];			//ׯ��λ��
	CPoint							m_ptCenterJetton;					//����λ��
	CPoint							m_ptUserJetton[GAME_PLAYER];		//����λ��
	CPoint							m_ptMyJetton;						//����λ��
	CPoint							m_ptUserAction[GAME_PLAYER];		//����λ��
	CPoint							m_SendEndingPos[GAME_PLAYER];		//����λ��
	CPoint							m_SendStartPos;						//ʼ��λ��
	CPoint							m_SendCardPos;						//����λ��
	CPoint							m_ptOperateBack[GAME_PLAYER];		//������ʾ
	CPoint							m_ptJettonBack[GAME_PLAYER];		//���뱳��
	CPoint							m_ptAddJetton[GAME_PLAYER];			//����λ��
	CPoint							m_ptHitCard[6];						//���λ��
	CPoint							m_ptControlBack;					//���λ��

	int								m_nXFace;
	int								m_nYFace;
	int								m_nXBorder;
	int								m_nYBorder;
	int								m_nYTimer;
	int								m_nXTimer;
	//�������
protected:
	CBitImage						m_ImageBanker;						//ׯ����Դ
	CBitImage						m_ImageCard;						//�˿���Դ
	CBitImage						m_ImageViewBack;					//������Դ
	CBitImage						m_ImageControlBack;					//������Դ

	//��ʾ����
protected:
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//���غ���
protected:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, INT nWidth, INT nHeight);

	//WIN7֧��
	virtual bool RealizeWIN7() { return true; }

	//���ܺ���
public:
	//���ö���
	void SetOffLinkUser(WORD wChairID,bool bOffLink);
	//��������
	void SetCardType(WORD wChairID,LPCTSTR pszHeapString,LPCTSTR pszTailString);
	//��ȡ�˿�
	bool GetHitCardData(BYTE cbCardData[],BYTE cbCardCount);
	//���ò���
	void SetUserOperate(WORD wChairID,bool bOperate);
	//�˿�����
	void SetHitCardData(BYTE cbCardData[],BYTE cbCardCount);
	//���õ��
	void SetHitPositively(bool bPositively);
	//�û�����
	void SetUserJetton(WORD wChairID,LONGLONG lJetton);
	//���õ�Ԫ
	void SetCellScore(LONGLONG lCellScore);
	//������ע
	void SetUserTableScore(WORD wChairID, LONGLONG lTableScore);
	//���ĳ���
	void SetCenterScore(LONGLONG lCenterScore);

	//�Ƚ���Ϣ
	void SetOpenCardInfo(BYTE cbCardData[GAME_PLAYER][MAX_COUNT],WORD wCurrentOpenUser,WORD wHeapMaxUser);
	//���ö���
	bool SetUserAction(WORD wActionUser, WORD cbUserAction);
	//�ƶ���Ϣ
	void SetJettonMoveInfo(WORD wChairID, int iMoveType, LONGLONG lTableScore);
	//���ƶ���
	bool CartoonOpenCard();
	//�ƶ�����
	bool CartoonMoveJetton();
	//�ƶ��˿�
	bool CartoonMoveCard();
	//��ɷ���
	void FinishDispatchCard();

	//������־
	bool IsMoveing(){return (m_bJettonAction || m_SendCardItemArray.GetCount()>0);}
	//�ƶ���־
	bool IsCardMoveing(){return (m_SendCardItemArray.GetCount()>0);}
	//�ƶ���־
	bool IsJettonMoveing(){return m_bJettonAction;}
	//�����˿�
	void DispatchUserCard(WORD wChairID, BYTE cbCardData);
	//ׯ�ұ�־
	void SetBankerUser(WORD wBankerUser);
	//�ȴ��±�
	void SetWaitInvest(bool bWaitInvest);
	//�ȴ�����
	void SetWaitOpenCard(bool bWaitOpenCard);
	//�ȴ��Ƚ�
	void SetWiatCompare(bool bWaitCompareHeap,bool bWaitCompareTail);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect);
	//������ͼ
	void RefreshGameView();
	//������ͼ
	void RefreshGameView(CRect &rect);

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�ƶ���Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
