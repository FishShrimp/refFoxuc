#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "RecordGameList.h"
#include "ApplyUserList.h"
#include "..\��Ϸ������\GameLogic.h"
#include "ClientControl.h"

//////////////////////////////////////////////////////////////////////////

//���붨��
#define JETTON_COUNT				6									//������Ŀ
#define JETTON_RADII				68									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON			WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER			WM_USER+201							//������Ϣ		
#define IDM_SOUND					WM_USER+206		
#define IDM_ADMIN_COMMDN			WM_USER+207


//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��ʷ��¼
#define MAX_SCORE_HISTORY			65									//��ʷ����
#define MAX_FALG_COUNT				12									//��ʶ����
#define COUNT_DRAWINFO              18

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
	enOperateResult					enOperateMen[AREA_COUNT+1];			//������ʶ
	bool							bWinMen[AREA_COUNT+1];				//˳��ʤ��

};

struct tagAndroidBet
{
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
	WORD							wChairID;							//���λ��
	int								nLeftTime;							//ʣ��ʱ�� (100msΪ��λ)
};

//��������
typedef CWHArray<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;

//������
class CGameClientEngine;

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//������Ϣ
protected:
	LONGLONG						m_lMeMaxScore;						//�����ע
    LONGLONG						m_lAreaLimitScore;					//��������
	LONGLONG						m_lRobotMaxJetton;					//������

	//��ע��Ϣ
protected:
	LONGLONG						m_lUserJettonScore[AREA_COUNT+1];	//������ע
	LONGLONG						m_lAllJettonScore[AREA_COUNT+1];	//ȫ����ע
	LONGLONG						m_lUserAddScore[AREA_COUNT];		//��ע��Ϣ
	
	//λ����Ϣ
protected:
	int								m_nWinFlagsExcursionX;				//ƫ��λ��
	int								m_nWinFlagsExcursionY;				//ƫ��λ��
	int								m_nScoreHead;						//�ɼ�λ��
	CPoint							m_PointJetton[AREA_COUNT];			//����λ��
	CPoint							m_PointAnimalRand[AREA_ALL];		//���Ｘ��
	CPoint                          m_PointAddButton[AREA_COUNT];		//��עλ��
	CPoint                          m_PointJettonScore[AREA_COUNT];		//����λ��
	CPoint                          m_LuZiStart;						//·����Ϣ
	int								m_TopHeight;						//λ����Ϣ
	int								m_LifeWidth;						//λ����Ϣ
	CRect							m_MeInfoRect;						//λ����Ϣ						
	CRect							m_CarRect[ANIMAL_COUNT];			//λ����Ϣ	

	//�˿���Ϣ
public:	
    BYTE							m_cbTableCardArray[2];				//�����˿�
	
				
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
	bool							m_bFlashResult;						//��ʾ���
	bool							m_bShowGameResult;					//��ʾ���
	bool                            m_bShowShaYuResult;					//�������
	bool                            m_bShowAnotherResult;				//��������
	int                             m_nPlayGameTimes;					//��������
	bool							m_DrawBack;							//�滭����
	

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
	bool							m_blMoveFinish;						//�ƶ�����
	//���ݱ���
protected:
	
	bool							m_bWinFlag[AREA_COUNT];				//ʤ����ʶ
	INT								m_nAnimalPercent[AREA_ALL];			//������
	int								m_CarIndex ;						//��ǰindex
	int                             m_ShaYuIndex;                       //����index
	BYTE							m_cbShaYuAddMulti;                  //���㽱��
	BYTE							m_cbOneDigit;						//������λ
	BYTE							m_cbTwoDigit;						//����ʮλ
	BYTE							m_cbRandOneDigit;					//������λ
	BYTE							m_cbRandTwoDigit;					//����ʮλ

	BYTE                            m_cbRandAddMulti;                   //��ǰ����
	BYTE                            m_cbAnimalIndex;					//��������
	bool                            m_bShowCartoon;						//��ʾ��ͨ
	
	CWHArray<tagAndroidBet>			m_ArrayAndroid;						//������ע

	//�ƶ������
	BYTE							m_nNumberOpenSide;
	BYTE							m_nCurRunIndex1;
	BYTE							m_NumberIndex1;
	bool							m_blRungingNumber1;

	BYTE							m_nCurRunIndex2;
	BYTE							m_NumberIndex2;
	bool							m_blRungingNumber2;

	//�ؼ�����
public:
	HINSTANCE						m_hInst;
	IClientControlDlg			    *m_pAdminControl;					//���ƿؼ�
	CGameRecord						m_GameRecord;						//��¼�б�
	CGameClientEngine				*m_pGameClientDlg;					//����ָ��
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	
	//����
public:
	bool							m_blCanStore;                       //�Ƿ񱣴�

	//��������
protected:
	bool							m_bFlashrandShow;					//�������
	bool							m_blRungingCar;						//�Ƿ��ƶ�
	int								m_nTimerStep;						//�ƶ���ֵ
	int								m_nCarOpenSide;						//���ж���
	int								m_nCurRunIndex;						//��ǰ����
	int								m_nTotoalRun;						//�ƶ�Ȧ��
	CRect							m_RectArea[8];						//����λ��
	int                             m_nAllRunTimes;

	//��ť����
public:
	CSkinButton						m_btJetton10;						//���밴ť
	CSkinButton						m_btJetton100;						//���밴ť
	CSkinButton						m_btJetton1000;						//���밴ť
	CSkinButton						m_btJetton10000;					//���밴ť
	CSkinButton						m_btJetton100000;					//���밴ť
	CSkinButton						m_btScoreMoveL;						//�ƶ��ɼ�
	CSkinButton						m_btScoreMoveR;						//�ƶ��ɼ�
	CSkinButton						m_btBankStorage;					//��ť
	CSkinButton						m_btBankDraw;						//ȡ�ť
	CButton							m_btOpenAdmin;						//ϵͳ����
	CSkinButton						m_btAdd[AREA_COUNT];				//���Ӱ�ť
	
	//�������
protected:
	CBitImage						m_ImageViewFill;					//����λͼ
	CBitImage						m_ImageScoreNumber;					//������ͼ
	CBitImage						m_ImageMeScoreNumber;				//������ͼ
	CPngImage						m_ImageTimeFlagPng;					//ʱ���ʶ
	CPngImage						m_ViewBackPng;						//����ͼƬ
	CPngImage						m_idb_selPng;						//ͼ������
	CPngImage						m_ImageWinFlags;					//��־λͼ
	CPngImage                       m_pngTimeBack;						//ͼƬ��Ϣ
	CPngImage                       m_pngResultFShayu;					//ͼƬ��Ϣ
	CPngImage                       m_pngResultFOther;					//ͼƬ��Ϣ
	CPngImage                       m_pngResultAnimal;					//ͼƬ��Ϣ
	CPngImage                       m_pngNumberASymbol;					//ͼƬ��Ϣ
	CPngImage                       m_pngLuZiR;							//ͼƬ��Ϣ
	CPngImage                       m_pngLuZiM;							//ͼƬ��Ϣ
	CPngImage                       m_pngLuZiL;							//ͼƬ��Ϣ
	CPngImage                       m_pngLuZiAnimal;					//ͼƬ��Ϣ
	CPngImage                       m_pngCoinNumber;					//ͼƬ��Ϣ
	CPngImage                       m_pngCarton[AREA_ALL];				//ͼƬ��Ϣ
	CPngImage                       m_pngAnimalLight;					//ͼƬ��Ϣ
	CPngImage                       m_pngTimeNumber;					//ͼƬ��Ϣ
	
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
	//ȡ�����
	void OnBankDraw();
	//������
	void OnBankStorage();
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
	//��������ע
	void AndroidBet(BYTE cbViewIndex, LONGLONG lScoreCount);
	//��������
	void SetAreaLimitScore(LONGLONG lAreaLimitScore);	
	//�����˿�
	void SetCardInfo(BYTE cbTableCardArray[2]);
	//��ʷ��¼
	void SetGameHistory(BYTE *bcResulte);
	//�ֻ�ׯ��
	void ShowChangeBanker( bool bChangeBanker );
	//�ɼ�����
	void SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore);
	//����ϵͳ��ׯ
	void EnableSysBanker(bool bEnableSysBanker) {m_bEnableSysBanker=bEnableSysBanker;}
		
	//ִ��ʣ�����еĻ��嶯��
	void PerformAllBetAnimation();
	//���û�����������
	void SetRobotMaxJetton(LONGLONG lRobotMaxJetton){ m_lRobotMaxJetton = lRobotMaxJetton; };
	//�������㽱��
	void SetShaYuAddMulti(BYTE cbShaYuAddMulti);//{ m_cbShaYuAddMulti = cbShaYuAddMulti; };  
	//������ͼ
	void RefreshGameView();
	//���ü���
	void SetAnimalPercent( INT nAnimalPercent[AREA_ALL] );
	//�����ƶ�
	void StartRunCar(int m_nTimerStep);
	//�����ƶ�
	void StartRunAnotherCar(int iTimer);
	//�����ƶ�
	void RuningCar(int m_nTimerStep);
	//�����ƶ�
	void RuningAnotherCar(int iTimer);
	//�������
	void StartRandShowSide();
		
	//��������
public:	
	//��ʼ����
	void DispatchCard();
	//��������
	void FinishDispatchCard( bool bRecord = true );
	
	//��������
public:
	//�����ע
	LONGLONG GetUserMaxJetton(BYTE cbJettonArea = 0);
	//������
	void  GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea);
	//���ͳ���
	void ReduceJettonNumber();
	//�滭����
protected:
	//��˸�߿�
	void FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC);

	//���溯��
public:
	//�������
	void CleanUserJetton();
	//����ʤ��
	void SetWinnerSide(bool blWin[],bool bSet);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭����
	void DrawMeJettonNumber(CDC *pDC);
	//�滭ׯ��
	void DrawBankerInfo(CDC *pDC,int nWidth,int nHeight);
	//�滭���
	void DrawMeInfo(CDC *pDC,int nWidth,int nHeight);
	//ȡ������
	void ClearAreaFlash() { m_cbAreaFlash = 0xFF; }

	//��������
public:
	//��ǰ����
	inline LONGLONG GetCurrentJetton() { return m_lCurrentJetton; }	

	void KillCardTime();

	void SwitchToCheck();

	void SwithToNormalView();

	void StartHandle_Leave();

	//�ڲ�����
private:
	//��ȡ����
	BYTE GetJettonArea(CPoint MousePoint);
	//�滭����
	void DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos,bool blTimer= false,bool bMeScore = false);
	//�滭����
	void DrawNumStrWithSpace(CDC * pDC, LONGLONG lNumber,CRect&rcPrint,INT nFormat=-1);
	//�滭����
	void DrawNumStrWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
	//�滭����
	void DrawSymbolNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos,int nHalf=0);
	//�滭����
	void DrawPercentNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos);
	//ͼƬ����
	int DrawPicNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos,enXCollocateMode xMode=enXCenter);
	//�滭��ʶ
	void DrawWinFlags(CDC * pDC);
	//��ʾ���
	void DrawShaYuResult(CDC *pDC, int nWidth, int nHeight);
	//��ʾ���
	void DrawAnotherResult(CDC *pDC, int nWidth, int nHeight);
	//��ȡ����
	CString GetAnimalStrInfo(BYTE cbAllIndex);
	
	//�ƶ�Ӯ��
	void DeduceWinner(bool bWinMen[]);
	//������ע
	void ButtonAddScore(BYTE cbJettonArea);
	//�ύ��ע
	void OnPlaceJetton();
	//��ȡ����
	BYTE GetAnimalInfo(BYTE cbAllIndex);
	//��ȡ����
	BYTE GetCartoonIndex(BYTE cbAllIndex);
	//��ȡ����
	BYTE GetAnimalMulti(BYTE cbAllIndex);
	//����״̬
	void ResetButtonState();
	

	void StartRunNumber1(int iTimer);
	void StartRunNumber2(int iTimer);
	void RuningNumber1(int iTimer);
	void RuningNumber2(int iTimer);

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

	afx_msg void OnUp();

	afx_msg void OnDown();

	afx_msg void OnAddScore0();
	afx_msg void OnAddScore1();
	afx_msg void OnAddScore2();
	afx_msg void OnAddScore3();
	afx_msg void OnAddScore4();
	afx_msg void OnAddScore5();
	afx_msg void OnAddScore6();
	afx_msg void OnAddScore7();
	afx_msg void OnAddScore8();
	afx_msg void OnAddScore9();
	afx_msg void OnAddScore10();


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
	//����Ա����
	afx_msg void OpenAdminWnd();

	DECLARE_MESSAGE_MAP()	

};

//////////////////////////////////////////////////////////////////////////

#endif
