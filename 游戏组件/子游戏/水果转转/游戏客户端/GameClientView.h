#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
//#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
//#include "GameLogic.h"


//////////////////////////////////////////////////////////////////////////

//���붨��
#define JETTON_COUNT				9									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

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
class CGameClientDlg;
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//������Ϣ
protected:
	__int64							m_iMeMaxScore;							//�����ע
	__int64							m_iMeAreaScore[JETTON_AREA_COUNT];		//�����ע					
	__int64							m_iAllAreaScore[JETTON_AREA_COUNT];		//ȫ����ע
	__int64							m_iAreaScoreLimit[JETTON_AREA_COUNT];	//������ǰ���·�
	CRect							m_rcJettonArea[JETTON_AREA_COUNT];		//����ע����
	CRect							m_rcScoreLimit[JETTON_AREA_COUNT];		//�����������·���д��
	CRect							m_rcApplayBankerNo[4];					//������ׯ����б�
	CRect							m_rcApplayBankerName[4];				//������ׯ�������
	CRect							m_rcApplayBankerScore[4];				//������ׯ��ҽ��
	CRect							m_rcHistoryRecord[MAX_SHOW_HISTORY];
	int								m_nCurCartoonIndex;						//��ǰӦ�û����ĸ���ͨͼ��
	int								m_CartoonTimeElapseArray[72];//96			//���ƿ�ͨ��ת��ʱ����
	int								m_CartoonElapsIndex;					//��ͨ��ʱ�������±�
	BOOL							m_bCanShowResult;						//�Ƿ���ʾ�����
	BYTE							m_cbGoalAnimal;							//ÿ����תֹͣ��λ��

	//λ����Ϣ
protected:
	int								m_nScoreHead;							//�ɼ�λ��


	bool							m_bisLButtonDown;						//�Ƿ�����������

	BYTE							m_bEndCount;							//��Ϸ��������


	//��������
protected:
	bool							m_bBombEffect[JETTON_AREA_COUNT];		//��ըЧ��
	BYTE							m_cbBombFrameIndex[JETTON_AREA_COUNT];	//֡������

	//��ʷ��Ϣ
protected:
	__int64							m_iMeStatisticScore;					//��Ϸ�ɼ�
	tagClientGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//��Ϸ��¼
	int								m_nRecordFirst;							//��ʼ��¼
	int								m_nRecordLast;							//����¼

	//״̬����
protected:
	WORD							m_wMeChairID;						//�ҵ�λ��
	BYTE							m_cbWinnerSide;						//ʤ�����
	BYTE							m_cbAreaFlash;						//ʤ�����
	__int64							m_iCurrentJetton;					//��ǰ����
	CString							m_strDispatchCardTips;				//������ʾ
	bool							m_bShowChangeBanker;				//�ֻ�ׯ��
	bool							m_bNeedSetGameRecord;				//�������

	//ׯ����Ϣ
protected:	
	WORD							m_wBankerUser;						//��ǰׯ��
	WORD							m_wBankerTime;						//��ׯ����
	__int64							m_iBankerScore;						//ׯ�һ���
	__int64							m_iBankerWinScore;					//ׯ�ҳɼ�	
	bool							m_bEnableSysBanker;					//ϵͳ��ׯ
	CFont							m_fontScoreLimit;					//ר�Ż��ƿ��·ֵĻ���

	//���ֳɼ�
public:
	__int64							m_iMeCurGameScore;					//�ҵĳɼ�
	__int64							m_iMeCurGameReturnScore;			//�ҵĳɼ�(������ע��)
	__int64							m_iBankerCurGameScore;				//ׯ�ҳɼ�
	__int64							m_iGameRevenue;						//��Ϸ˰��

	//���ݱ���
protected:
	CPoint							m_PointJetton[JETTON_AREA_COUNT];					//����λ��
	CPoint							m_PointJettonNumber[JETTON_AREA_COUNT];				//����λ��
	CJettonInfoArray				m_JettonInfoArray[JETTON_AREA_COUNT];				//��������
	CPoint							m_PtCardArray[24];					//��ת·���ϸ���������ĵ�

	//�ؼ�����
public:
	
	CSkinButton						m_btApplyBanker;					//����ׯ��
	CSkinButton						m_btCancelBanker;					//ȡ��ׯ��

	CSkinButton						m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btScoreMoveR;						//�ƶ��ɼ�
	CSkinButton						m_btBankerMoveUp;					//���Ϲ�������ׯ���б�
	CSkinButton						m_btBankerMoveDown;					//���¹�������ׯ���б�
	CSkinButton						m_btBankButton;						//���а�ť

	CSkinButton						m_btAddStep1;						//1��Ͷע
	CSkinButton						m_btAddStep10;						//10��Ͷע
	CSkinButton						m_btAddStep100;						//100��Ͷע
	CSkinButton						m_btAddStep1_H;						//1��Ͷע
	CSkinButton						m_btAddStep10_H;					//10��Ͷע
	CSkinButton						m_btAddStep100_H;					//100��Ͷע

	CSkinButton						m_btBetLast;						//��Ѻ��ť
	CSkinButton						m_btBetSmall;						//ѺС��ť
	CSkinButton						m_btBetBig;							//Ѻ��ť


	//�ؼ�����
public:
	CApplyUser						m_ApplyUser;						//�����б�
	CGameRecord						m_GameRecord;						//��¼�б�	
	CGameClientDlg					*m_pGameClientDlg;					//����ָ��

	//�������
protected:
	CBitImage						m_ImageViewFill;					//����λͼ
	CBitImage						m_ImageViewBack;					//����λͼ
	CBitImage						m_ImageJettonView;					//������ͼ
	CBitImage						m_ImageScoreNumber;					//������ͼ
	CBitImage						m_ImageMeScoreNumber;				//������ͼ
	CBitImage						m_ImageTimeFlag;					//ʱ���ʶ
	CBitImage						m_ImageHistoryRecord;				//��ʷ��¼ͼƬ
	CPngImage						m_ImageBombEffect;					//����ͼƬ
	CPngImage						m_ImageCardCartoon;					//��ת����

	CPngImage						m_ImageApple;						//ƻ������
	CPngImage						m_ImageApple2;						//ƻ��X2����
	CPngImage						m_IamgeBar;							//BAR����
	CPngImage						m_IamgeBar2;						//BARX2����
	CPngImage						m_IamgeBell;						//��������
	CPngImage						m_IamgeBell2;						//����X2����
	CPngImage						m_IamgeSeven;						//SEVEN
	CPngImage						m_IamgeSeven2;						//SEVENX2
	CPngImage						m_IamgeStar;						//STAR
	CPngImage						m_IamgeStar2;						//STARX2
	CPngImage						m_IamgeOrange;						//ORANGE
	CPngImage						m_IamgeOrange2;						//ORANGEX2
	CPngImage						m_IamgePaw;							//PAW
	CPngImage						m_IamgePaw2;						//PAWX2
	CPngImage						m_ImageWaterMelon;					//WATERMELON
	CPngImage						m_ImageWaterMelon2;					//WATERMELONX2

	CPngImage						m_ImageBetNumber;					//������ͼ
	CPngImage						m_ImageBetBigNumber;				//ʱ����ͼ

	CPngImage						m_ImageFreeTime;					//����ʱ��
	CPngImage						m_ImageBitSmallTime;				//Ѻ��Сʱ��
	CPngImage						m_ImageJettonTime;					//��עʱ��



	CBitImage						m_ImageKeyPress;					//���¼�ע��ť

	//�߿���Դ
protected:
	CBitImage						m_ImageWinFrame;					//��ע���б߿�

	CBitImage						m_ImageMeBanker;					//�л�ׯ��
	CBitImage						m_ImageChangeBanker;				//�л�ׯ��
	CBitImage						m_ImageNoBanker;					//�л�ׯ��

	//������Դ
protected:
	CBitImage						m_ImageGameEnd;						//�ɼ�ͼƬ

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//���ú���
public:
	//������Ϣ
	void SetMeMaxScore(__int64 iMeMaxScore);
	//�ҵ�λ��
	inline void SetMeChairID(WORD wMeChairID) { m_wMeChairID=wMeChairID;}
	//������ע
	void SetMePlaceJetton(BYTE cbViewIndex, __int64 iJettonCount);
	//ׯ����Ϣ
	void SetBankerInfo(WORD wBankerUser, __int64 iBankerScore);
	//ׯ�ҳɼ�
	void SetBankerScore(WORD wBankerTime, __int64 iWinScore) {m_wBankerTime=wBankerTime; m_iBankerWinScore=iWinScore;}
	//���ֳɼ�
	void SetCurGameScore(__int64 iMeCurGameScore, __int64 iMeCurGameReturnScore, __int64 iBankerCurGameScore, __int64 iGameRevenue);
	//���ó���
	void PlaceUserJetton(BYTE cbViewIndex, __int64 iScoreCount);
	//��������
	void SetAreaLimitScore(__int64 * pLimitArray, int ArraySize);	

	//���ó���
	//void SetCurrentJetton(__int64 iCurrentJetton);
	//��ʷ��¼
	void SetGameHistory(tagGameRecord * pGameRecord, int nRecordCount);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//�ɼ�����
	void SetGameScore(__int64 iMeCurGameScore, __int64 iMeCurGameReturnScore, __int64 iBankerCurGameScore);
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}

	void CreateApplayBankerListRect(int nBaseX, int nBaseY);
	void CreateHistoryRecordRect(int nWidth, int nHeight);
	void UpdateCartoonTimeElapse(BYTE cbStopStep);	
	void CreateTurnPathAreaBasePoint(int nWidth, int nHeight);

	//��������
public:	
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect, WORD wAreaIndex);
	//�滭��ը
	void DrawBombEffect(CDC *pDC);

	//����������ׯ����б�
	void DrawApplyBankerUserList(CDC * pDC);

	//������ʷ��¼
	void DrawHistoryRecord(CDC * pDC);

	//����ʱ����ʾ
	void DrawTimeTip(CDC * pDC, int nWidth, int nHeight);

	//����������ע
	void DrawAreaLimitScore(CDC * pDC);

	//���Ƴ���
	void DrawAreaJetton(CDC * pDC);

	//����ׯ����Ϣ
	void DrawBankerInfo(CDC * pDC, int nWidth, int nHeight);

	//�����м���Ϣ
	void DrawUserInfo(CDC * pDC, int nWidth, int nHeight);

	//�л�ׯ��
	void DrawChangeBanker(CDC * pDC, int nWidth, int nHeight);

	//������ע��ť
	void DrawAddMoney(CDC * pDC, int nWidth, int nHeight);

	//����ʱ��
	virtual void DrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea);

	//��������
protected:

	//��ȡ��ǰ��һ�����ע������
	__int64 GetUserCurrentLeaveScore();
	
	//�滭����
protected:
	//��˸�߿�
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);

	//��ʼ��ת
	void PlayingCartoonEffect();
	
	//������ת
	void EndCartoonEffect();


	//���溯��
public:
	//�������
	void CleanUserJetton();
	//����ʤ��
	void SetWinnerSide(BYTE cbWinnerSide);

	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭����
	void DrawMeJettonNumber(CDC *pDC);

	//��������
public:
	//��ǰ����
	inline __int64 GetCurrentJetton() { return m_iCurrentJetton; }	

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CDC * pDC, __int64 lNumber, INT nXPos, INT nYPos, bool bMeScore = false);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, __int64 iNumber, INT nXPos, INT nYPos);
	//�滭����
	void DrawNumberStringWithSpace(CDC * pDC, __int64 iNumber, CRect rcRect, INT nFormat=-1);
	//��ʾ���
	void ShowGameResult(CDC *pDC, int nWidth, int nHeight);
	//�����ҵķ���
	void DrawMeScore(CDC *pDC, int nWidth, int nHeight);
	//���Ƶ�ǰ�ɼ�
	void DrawCurrentScore(CDC *pDC, int nWidth, int nHeight);
	//͸���滭
	bool DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha);

	//���ƶ���
	void DrawCardCartoon(CDC * pDC);

	//���ƽ�������
	void DrawEndCartoon(CDC * pDC,int nWidth,int nHeight,BYTE bGoalAnimal);

	//��ť��Ϣ
protected:
	//��Ͷ��ť
	afx_msg void OnAddStep1();
	afx_msg void OnAddStep10();
	afx_msg void OnAddStep100();
	afx_msg void OnAddStep1_H();
	afx_msg void OnAddStep10_H();
	afx_msg void OnAddStep100_H(); 

	//��ׯ��ť
	afx_msg void OnApplyBanker();
	//��ׯ��ť
	afx_msg void OnCancelBanker();
	//�ƶ���ť
	afx_msg void OnScoreMoveL();
	//�ƶ���ť
	afx_msg void OnScoreMoveR();

	afx_msg void OnBankerListMoveUp();
	afx_msg void OnBankerListMoveDown();

	afx_msg void OnClickedBank();

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

//////////////////////////////////////////////////////////////////////////

#endif
