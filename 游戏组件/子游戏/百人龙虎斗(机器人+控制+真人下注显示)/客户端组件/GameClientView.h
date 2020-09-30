#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
#include "GameLogic.h"
#include "DlgViewChart.h"
#include "ClientControl.h"

//////////////////////////////////////////////////////////////////////////////////
#ifdef __BANKER___
#include "DlgBank.h"
#endif

//���붨��
#define JETTON_COUNT				6									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON			WM_USER+1200							//��ס��Ϣ
#define IDM_APPLY_BANKER			WM_USER+1201							//������Ϣ

//��������
#define INDEX_LONG				0									//������
#define INDEX_HU				1									//������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//�ṹ����
struct tagAndroidBet
{
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
	WORD							wChairID;							//���λ��
	int								nLeftTime;							//ʣ��ʱ�� (100msΪ��λ)
};

//������Ϣ
struct tagJettonInfo
{
	int								nXPos;								//����λ��
	int								nYPos;								//����λ��
	BYTE							cbJettonIndex;						//��������
};

//��������
typedef CWHArray<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//������
class CGameClientEngine;
//////////////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//������Ϣ
protected:
	LONGLONG							m_lMeMaxScore;						//�����ע
	LONGLONG							m_lAreaLimitScore;					//��������

	//��ע��Ϣ
protected:
	LONGLONG							m_lMeInAllScore[AREA_ALL];			//�Լ�ÿ���������ע

	//ȫ����ע
protected:
	LONGLONG							m_lAreaInAllScore[AREA_ALL];		//ÿ��������ܷ�	


	//λ����Ϣ
protected:
	int								m_nWinFlagsExcursionX;				//ƫ��λ��
	int								m_nWinFlagsExcursionY;				//ƫ��λ��
	int								m_nWinPointsExcursionX;				//ƫ��λ��
	int								m_nWinPointsExcursionY;				//ƫ��λ��
	int								m_nScoreHead;						//�ɼ�λ��

	//λ����Ϣ
protected:
	CPoint							m_ptOffset;							//ƫ��
	CRect							m_rcBetAreaBox[AREA_ALL];			//������ʾ��
	CRect							m_rcBetArea[AREA_ALL];				//������ʾ

	//��С��Ϣ
protected:
	CSize							m_szViewBack;						//������С

	//�˿���Ϣ
protected:	
	BYTE							m_cbCardCount[2];					//�˿���Ŀ
	BYTE							m_cbTableCardArray[2][3];			//�����˿�

	//��������
protected:
	POINT							m_ptDispatchCard;					//����λ��
	BYTE							m_cbSendCount[2];					//�˿���Ŀ
	CCardControl					m_DispatchCard;						//�����˿�
	bool							m_bBombEffect[AREA_ALL];			//��ըЧ��
	BYTE							m_cbBombFrameIndex[AREA_ALL];		//֡������

	//��ʷ��Ϣ
protected:
	LONGLONG						m_lMeStatisticScore;				//��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];//��Ϸ��¼
	int								m_nRecordFirst;						//��ʼ��¼
	int								m_nRecordLast;						//����¼

	//״̬����
protected:
	WORD							m_wMeChairID;						//�ҵ�λ��
	BYTE							m_cbWinnerSide;						//ʤ�����
	BYTE							m_cbAreaFlash;						//ʤ�����
	LONGLONG						m_lCurrentJetton;					//��ǰ����
	CString							m_strDispatchCardTips;				//������ʾ
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��
	bool							m_bNeedSetGameRecord;				//�������

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
	bool							m_blCanStore;                       //�Ƿ��ܱ���

	//���ݱ���
protected:
	CPoint							m_PointJetton[AREA_ALL];				//����λ��
	CPoint							m_PointJettonNumber[AREA_ALL];			//����λ��
	CJettonInfoArray				m_JettonInfoArray[AREA_ALL];			//��������

	//�ؼ�����
public:
	CSkinButton						m_btJetton100;						//���밴ť
	CSkinButton						m_btJetton1000;						//���밴ť
	CSkinButton						m_btJetton10000;					//���밴ť
	CSkinButton						m_btJetton100000;					//���밴ť
	CSkinButton						m_btJetton1000000;					//���밴ť
	CSkinButton						m_btJetton5000000;					//���밴ť
	CSkinButton						m_btViewChart;						//�鿴·��

	CSkinButton						m_btApplyBanker;					//����ׯ��
	CSkinButton						m_btCancelBanker;					//ȡ��ׯ��

	CSkinButton						m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btScoreMoveR;						//�ƶ��ɼ�	
	//�ؼ�����
public:
#ifdef __BANKER___
	//CDlgBank						m_DlgBank;							//���пؼ�
	CSkinButton						m_btBankerStorage;					//��ť
	CSkinButton						m_btBankerDraw;						//ȡ�ť
#endif
	CApplyUser						m_ApplyUser;						//�����б�
	CGameRecord						m_GameRecord;						//��¼�б�
	CCardControl					m_CardControl[2];					//�˿˿ؼ�	
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CDlgViewChart					m_DlgViewChart;						//�鿴·��
	CButton							m_btOpenAdmin;						//ϵͳ����
	CGameClientEngine				*m_pGameClientDlg;					//����ָ��

	//����
public:
	HINSTANCE						m_hInst;
	IClientControlDlg*				m_pClientControlDlg;	

	//�������
protected:
	CBitImage						m_ImageViewFill;					//����λͼ
	CBitImage						m_ImageViewBack;					//����λͼ
	CBitImage						m_ImageWinFlags;					//��־λͼ
	CBitImage						m_ImageJettonView;					//������ͼ
	CBitImage						m_ImageScoreNumber;					//������ͼ
	CBitImage						m_ImageMeScoreNumber;				//������ͼ
	CBitImage						m_ImageTimeFlag;					//ʱ���ʶ
	CPngImage						m_ImageBombEffect;					//����ͼƬ

	//�߿���Դ
protected:
	//�߿�ͼƬ
	CBitImage						m_ImageFrame[AREA_ALL];				//�߿�ͼƬ

	CBitImage						m_ImageMeBanker;					//�л�ׯ��
	CBitImage						m_ImageChangeBanker;				//�л�ׯ��
	CBitImage						m_ImageNoBanker;					//�л�ׯ��

	//������Դ
protected:
	CBitImage						m_ImageGameEnd;						//�ɼ�ͼƬ
	CBitImage						m_ImageGameEndFrame;				//�߿�ͼƬ
	CBitImage						m_ImageGamePoint;					//����ͼƬ
	bool                            m_bFlashResult;


	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

protected:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���غ���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//���ú���
public:
	//������Ϣ
	void SetMeMaxScore(LONGLONG lMeMaxScore);
	//�ҵ�λ��
	inline void SetMeChairID(WORD wMeChairID) { m_wMeChairID=wMeChairID; }
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount);
	//ׯ����Ϣ
	void SetBankerInfo(WORD wBankerUser, LONGLONG lBankerScore);
	//ׯ�ҳɼ�
	void SetBankerScore(WORD wBankerTime, LONGLONG lWinScore) {m_wBankerTime=wBankerTime; m_lTmpBankerWinScore=lWinScore;}
	//���ֳɼ�
	void SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue);
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount);
	//��������
	void SetAreaLimitScore(LONGLONG lAreaLimitScore);	
	//�����˿�
	void SetCardInfo(BYTE cbCardCount[2], BYTE cbTableCardArray[2][3]);
	//���ó���
	void SetCurrentJetton(LONGLONG lCurrentJetton);
	//��ʷ��¼
	void SetGameHistory(enOperateResult OperateResult, BYTE cbResult, BYTE cbLong, BYTE cbHu);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//�ɼ�����
	void SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore);
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}

	//��������
public:	
	//��ʼ����
	void DispatchCard();
	//��������
	void FinishDispatchCard();
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);
	//�滭��ը
	void DrawBombEffect(CDC *pDC);

	//��������
protected:
	//�����ע
	LONGLONG GetMaxPlayerScore(BYTE cbJettonArea);

	//�滭����
protected:
	//��˸�߿�
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);


	//���溯��
public:
	//�������
	void CleanUserJetton();
	//����ʤ��
	void SetWinnerSide(BYTE cbWinnerSide,bool blSet = false);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭����
	void DrawMeJettonNumber(CDC *pDC);
	//������ʾ
	void SetDispatchCardTips();
	//�������
	void AllowControl(BYTE cbStatus);

	//��������
public:
	//��ǰ����
	inline LONGLONG GetCurrentJetton() { return m_lCurrentJetton; }	

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
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
	void DeduceWinner( OUT INT nAreaWin[AREA_ALL], OUT BYTE& cbResult, OUT BYTE& cbAndValues);

	void ClearBank();

	//��ť��Ϣ
protected:

	//�鿴·��
	afx_msg void OnViewChart();
	//�ƶ���ť
	afx_msg void OnScoreMoveL();
	//�ƶ���ť
	afx_msg void OnScoreMoveR();
	//���д��
	afx_msg void OnBankStorage();
	//����ȡ��
	afx_msg void OnBankDraw();
	//����Ա����
	afx_msg void OpenAdminWnd();

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
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif