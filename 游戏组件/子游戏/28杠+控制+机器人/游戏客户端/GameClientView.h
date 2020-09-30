#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
#include "../��Ϸ������/GameLogic.h"
//#include "DlgBank.h"
#include "ClientControl.h"

//////////////////////////////////////////////////////////////////////////
//ʱ���ʶ
#define IDI_FLASH_WINNER			100									//������ʶ
#define IDI_SHOW_CHANGE_BANKER		101									//�ֻ�ׯ��
#define IDI_DISPATCH_CARD			102									//���Ʊ�ʶ
#define IDI_SHOWDISPATCH_CARD_TIP	103									//������ʾ
#define IDI_OPENCARD	104												//������ʾ
#define IDI_MOVECARD_END			105									//�Ƅ��ƽY��
#define IDI_POSTCARD	106											   //������ʾ
#define IDI_CHECK_TIMER			107

//���붨��
#define JETTON_COUNT				7									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON			WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ
#define IDM_CONTINUE_CARD			WM_USER+202							//��������
#define IDM_AUTO_OPEN_CARD			WM_USER+203							//��ť��ʶ
#define IDM_OPEN_CARD				WM_USER+204		
#define IDM_MANAGE_CONTROL			WM_USER+205							//������Ϣ
#define IDM_OPEN_SOUND			    WM_USER+206							//��ť��ʶ
#define IDM_CLOSE_SOUND				WM_USER+207		
#define IDM_POSTCARD_SOUND               (WM_USER+208)	                    //�������


//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��ʷ��¼
//#ifdef _DEBUG
//#define MAX_SCORE_HISTORY			18									//��ʷ����
//#else
//#define MAX_SCORE_HISTORY			65									//��ʷ����
//#endif

#define MAX_SCORE_HISTORY			65									//��ʷ����

#define MAX_FALG_COUNT				14									//��ʶ����

//������Ϣ
struct tagJettonInfo
{
	int								nXPos;								//����λ��
	int								nYPos;								//����λ��
	BYTE							cbJettonIndex;						//��������
};

//�������
enum enOperateResult
{
	enOperateResult_NULL,
	enOperateResult_Win,
	enOperateResult_Lost
};

//��¼��Ϣ
struct tagClientGameRecord
{
	enOperateResult					enOperateShunMen;					//������ʶ
	enOperateResult					enOperateDuiMen;					//������ʶ
	enOperateResult					enOperateDaoMen;					//������ʶ
	bool							bWinShunMen;						//˳��ʤ��
	bool							bWinDuiMen;							//����ʤ��
	bool							bWinDaoMen;							//����ʤ��
	bool							bWinShunMenSame;						//˳��ʤ��
	bool							bWinDuiMenSame;							//����ʤ��
	bool							bWinDaoMenSame;							//����ʤ��
};

//������ʾ
enum enDispatchCardTip
{
	enDispatchCardTip_NULL,
	enDispatchCardTip_Continue,											//��������
	enDispatchCardTip_Dispatch											//����ϴ��
};

//��������
typedef CWHArray<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//������
class CGameClientEngine;
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//������Ϣ
protected:
	LONGLONG						m_lMeMaxScore;						//�����ע
    LONGLONG						m_lAreaLimitScore;					//��������

	//��ע��Ϣ
protected:
	LONGLONG						m_lUserJettonScore[AREA_ARRY_COUNT];	//������ע
	LONGLONG						m_lAllJettonScore[AREA_ARRY_COUNT];	//ȫ����ע

	//λ����Ϣ
protected:
	int								m_nWinFlagsExcursionX;				//ƫ��λ��
	int								m_nWinFlagsExcursionY;				//ƫ��λ��
	int								m_nScoreHead;						//�ɼ�λ��
	CRect							m_rcShunMen;						//�м�����
	CRect							m_rcJiaoL;							//����������
	CRect							m_rcQiao;							//��������
	CRect							m_rcDuiMen;							//ƽ����
	CRect							m_rcDaoMen;							//ͬ��ƽ����
	CRect							m_rcJiaoR;							//ׯ������
	int								m_OpenCardIndex;					//�_�����
	int								m_PostCardIndex;					//�l�����
	int								m_PostStartIndex;					//������ʼλ��


	//�˿���Ϣ
public:	
    BYTE							m_cbTableCardArray[4][2];			//�����˿�
	bool							m_blMoveFinish;
	BYTE							m_bcfirstShowCard;
	BYTE							m_bcShowCount;
	bool							m_blAutoOpenCard;					//���Զ�

	//��ʷ��Ϣ
protected:
	LONGLONG						m_lMeStatisticScore;				//��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//��Ϸ��¼
	int								m_nRecordFirst;						//��ʼ��¼
	int								m_nRecordLast;						//����¼

	//״̬����
protected:
	WORD							m_wMeChairID;						//�ҵ�λ��
	BYTE							m_cbAreaFlash;						//ʤ�����
	LONGLONG						m_lCurrentJetton;					//��ǰ����
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��
	bool							m_bNeedSetGameRecord;				//�������
	bool							m_bWinShunMen;						//ʤ����ʶ
	bool							m_bWinDuiMen;						//ʤ����ʶ
	bool							m_bWinDaoMen;						//ʤ����ʶ
	bool							m_bFlashResult;						//��ʾ���
	bool							m_bShowGameResult;					//��ʾ���
	enDispatchCardTip				m_enDispatchCardTip;				//������ʾ

	//ׯ����Ϣ
protected:	
	WORD							m_wBankerUser;						//��ǰׯ��
	WORD							m_wBankerTime;						//��ׯ����
	LONGLONG						m_lBankerScore;						//ׯ�һ���
	LONGLONG						m_lBankerWinScore;					//ׯ�ҳɼ�	
	LONGLONG						m_lTmpBankerWinScore;				//ׯ�ҳɼ�	
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ

	//���ֳɼ�
public:
	LONGLONG						m_lMeCurGameScore;					//�ҵĳɼ�
	LONGLONG						m_lMeCurGameReturnScore;			//�ҵĳɼ�
	LONGLONG						m_lBankerCurGameScore;				//ׯ�ҳɼ�
	LONGLONG						m_lGameRevenue;						//��Ϸ˰��

	//���ݱ���
protected:
	CPoint							m_PointJetton[AREA_COUNT];			//����λ��
	CPoint							m_PointJettonNumber[AREA_COUNT];	//����λ��
	CJettonInfoArray				m_JettonInfoArray[AREA_COUNT];		//��������

	//�ؼ�����
public:
	CSkinButton						m_btJetton100;						//���밴ť
	CSkinButton						m_btJetton1000;						//���밴ť
	CSkinButton						m_btJetton10000;					//���밴ť
	CSkinButton						m_btJetton100000;					//���밴ť
	CSkinButton						m_btJetton500000;					//���밴ť
	CSkinButton						m_btJetton1000000;					//���밴ť
	CSkinButton						m_btJetton5000000;					//���밴ť
	CSkinButton						m_btJetton10000000;					//���밴ť
	CSkinButton						m_btApplyBanker;					//����ׯ��
	CSkinButton						m_btCancelBanker;					//ȡ��ׯ��
	CSkinButton						m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btScoreMoveR;						//�ƶ��ɼ�
	CSkinButton						m_btAutoOpenCard;					//�Զ�����
	CSkinButton						m_btOpenCard;						//�ֶ�����
	CSkinButton						m_btBank;							//���а�ť
	CSkinButton						m_btContinueCard;					//��������
	CSkinButton				        m_btBankerStorage;					//��ť
	CSkinButton						m_btBankerDraw;						//ȡ�ť
	CSkinButton						m_btSoundOpen;	
	CSkinButton						m_btSoundClose;	

	CSkinButton						m_btUp;					            //��ť
	CSkinButton						m_btDown;						    //ȡ�ť	
	CButton							m_btOpenAdmin;                      //����Ա����

	CBitImageEx						m_btImageJetton1000;					//���밴ť
	CBitImageEx						m_btImageJetton10000;					//���밴ť
	CBitImageEx						m_btImageJetton100000;					//���밴ť
	CBitImageEx						m_btImageJetton500000;					//���밴ť
	CBitImageEx						m_btImageJetton1000000;					//���밴ť
	CBitImageEx						m_btImageJetton5000000;					//���밴ť
	CBitImageEx						m_btImageJetton10000000;				//���밴ť



	//�ؼ�����
public:
	HINSTANCE						m_hInst;
	IClientControlDlg			   *m_pAdminControl;						//����
	CApplyUser						m_ApplyUser;						//�����б�
	CGameRecord						m_GameRecord;						//��¼�б�
	CCardControl					m_CardControl[5];					//�˿˿ؼ�	
	CGameClientEngine					*m_pGameClientDlg;					//����ָ��
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	//CDlgBank						m_DlgBank;							//���пؼ�
	//CAdminControl				    m_AdminDlg;							//����Ա����
	

	//�������
protected:
	CBitImageEx						m_ImageViewFill;					//����λͼ
	CBitImageEx						m_ImageViewBack;					//����λͼ
	CBitImageEx						m_ImageWinFlags;					//��־λͼ
	CBitImageEx						m_ImageJettonView;					//������ͼ
	CBitImageEx						m_ImageScoreNumber;					//������ͼ
	CBitImageEx						m_ImageMeScoreNumber;				//������ͼ
	CBitImageEx						m_ImageTimeFlag;					//ʱ���ʶ
	CBitImageEx						m_ImageDispatchCardTip;				//������ʾ

	struct TotoalPress
	{
		CPngImage n_Png_tatoalflag;
		CPngImage n_Png_tipC;
		CPngImage n_Png_tipN;
		
	};


	CPngImage						m_PngScoreNumber;				//������ͼ

	TotoalPress                     m_TotoalPress;               

	//�߿���Դ
protected:
	CBitImageEx						m_ImageFrameShunMen;				//�߿�ͼƬ
	CBitImageEx						m_ImageFrameJiaoL;					//�߿�ͼƬ
	CBitImageEx						m_ImageFrameQiao;					//�߿�ͼƬ
	CBitImageEx						m_ImageFrameDuiMen;					//�߿�ͼƬ
	CBitImageEx						m_ImageFrameDaoMen;					//�߿�ͼƬ
	CBitImageEx						m_ImageFrameJiaoR;					//�߿�ͼƬ

	CBitImageEx						m_ImageMeBanker;					//�л�ׯ��
	CBitImageEx						m_ImageChangeBanker;				//�л�ׯ��
	CBitImageEx						m_ImageNoBanker;					//�л�ׯ��

	CPngImage					    m_ImageTimer;					//�л�ׯ��
	CBitImageEx                      m_ImageTimerNumber;

	//������Դ
protected:
	CBitImageEx						m_ImageGameEnd;						//�ɼ�ͼƬ

	//����
public:
	BOOL							m_blCanStore;                       //�Ƿ��ܱ���

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

	//���ú���
public:
	//������Ϣ
	void SetMeMaxScore(LONGLONG lMeMaxScore);
	//�ҵ�λ��
	void SetMeChairID(DWORD dwMeUserID);
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);
	//ׯ����Ϣ
	void SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore);
	//ׯ�ҳɼ�
	void SetBankerScore(WORD wBankerTime, LONGLONG lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//���ֳɼ�
	void SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue);
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount);
	//��������
	void SetAreaLimitScore(LONGLONG lAreaLimitScore);	
	//�����˿�
	void SetCardInfo(BYTE cbTableCardArray[4][2]);
	//���ó���
	void SetCurrentJetton(LONGLONG lCurrentJetton);
	//��ʷ��¼
	void SetGameHistory(bool bWinShunMen, bool bWinDaoMen, bool bWinDuiMen);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//�ɼ�����
	void SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore);
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
	//������ʾ
	void SetDispatchCardTip(enDispatchCardTip DispatchCardTip);

	void SetFirstShowCard(BYTE bcCard);

	//������ͼ
	void RefreshGameView();
	
	void ClearBank();
	//
	void SetBankerAllResult(LONGLONG lAllWinScore){m_lBankerWinScore = lAllWinScore;}

	//��������
public:	
	//��ʼ����
	void DispatchCard();
	//��������
	void FinishDispatchCard();
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);

	//��������
public:
	//�����ע
	LONGLONG GetUserMaxJetton();

	//�滭����
protected:
	//��˸�߿�
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);

	//���溯��
public:
	//�������
	void CleanUserJetton();
	//����ʤ��
	void SetWinnerSide(bool bWinShunMen, bool bWinDuiMen, bool bWinDaoMen, bool bSet);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭����
	void DrawMeJettonNumber(CDC *pDC);
	//�滭ׯ��
	void DrawBankerInfo(CDC *pDC,int nWidth,int nHeight);
	//�滭���
	void DrawMeInfo(CDC *pDC,int nWidth,int nHeight);

	//��������
public:
	//��ǰ����
	inline LONGLONG GetCurrentJetton() { return m_lCurrentJetton; }	
	
	void SetMoveCardTimer();
	//���ճ���ʱ��������
	void SetFinishDiscard();

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false,bool blTimer=false);
	//�滭����
	void DrawNumberStringWithPng(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat=-1);
	//�滭��ʶ
	void DrawWinFlags(CDC * pDC);
	//��ʾ���
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//͸���滭
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);
	//�ƶ�Ӯ��
	void DeduceWinner(bool &bWinShunMen, bool &bWinDuiMen, bool &bWinDaoMen);

	void DrawJettonPress(CDC* pDC,int iWidth,int iHeight,LONGLONG lTotal,LONGLONG lHaveJetton);

	void DrawGameTimer(CDC* pDC,int iWidth,int iHeight,int iTimer);

	LONGLONG GetBankMaxJetton();

	//��ť��Ϣ
protected:
	//��ׯ��ť
	afx_msg void OnApplyBanker();
	//��ׯ��ť
	afx_msg void OnCancelBanker();
	//�ƶ���ť
	afx_msg void OnScoreMoveL();
	//�ƶ���ť
	afx_msg void OnScoreMoveR();
	//�ֹ�����
	afx_msg void OnOpenCard();
	//�Զ�����
	afx_msg void OnAutoOpenCard();

	afx_msg void OnOpenSound();

	afx_msg void OnCloseSound();
	//���д��
	afx_msg void OnBankStorage();
	//����ȡ��
	afx_msg void OnBankDraw();
	//�Ϸ�
	afx_msg void OnUp();
	//�·�
	afx_msg void OnDown();
	//����Ա����
	afx_msg void OpenAdminWnd();
	//��Ӯ����
	afx_msg void OnControl();

	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);	
	//�����Ϣ
	afx_msg void OnRButtonDown(UINT nFlags, CPoint Point);	
    //�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//�ؼ�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    //����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//�����ؼ����UP��Ϣ
	LRESULT OnViLBtUp(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()	

};
#endif