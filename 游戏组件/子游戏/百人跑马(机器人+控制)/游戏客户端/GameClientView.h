#ifndef GAME_CLIENT_VIEW_HEAD_FILE
#define GAME_CLIENT_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "MyImage.h"
#include "DialogBet.h"
#include "DialogRecord.h"
#include "DialogStatistics.h"
#include "DialogBetRecord.h"
#include "UserFaceWnd.h"
#include "DirectSound.h"
#include "DialogControl.h"


//////////////////////////////////////////////////////////////////////////


//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewD3D
{
	//������Ϣ
private:
	INT						m_nStreak;							//����
	WORD					m_wKindID;							//��ϷID
	tagBASE					m_tagBenchmark;						//��׼�ṹ
	INT						m_nBetEndTime;						//��ע����ʱ��

	//��Ϸ����
private:
	WORD					m_wMeChairID;						//�ҵ�λ��
	BYTE					m_cbGameStatus;						//����״̬
	bool					m_bRacing;							//��������
	bool					m_bMeOrAll;							//��ʾ�Լ�����ȫ��(trueΪMe, falseΪAll)

	//��Ϸ��Ϣ
private:
	INT						m_nMultiple[AREA_ALL];				//������
	BYTE					m_bRanking[RANKING_NULL];			//����

	//����
protected:
	INT						m_nBetPlayerCount;					//��ע����
	LONGLONG				m_lPlayerBet[AREA_ALL];				//�����ע
	LONGLONG				m_lPlayerBetAll[AREA_ALL];			//������ע
	LONGLONG				m_lPlayerWinning;					//�����Ӯ

	//��С
private:
	CSize					m_szTotalSize;						//�ܴ�С
	CSize					m_szBackdrop;						//�����д�С
	CSize					m_szBackdropHand;					//����ͷ��С
	CSize					m_szBackdropTai;					//����β��С
	
	//��ƥ
public:	
	INT						m_nHorsesSpeed[HORSES_ALL][STEP_SPEED];	//ÿƥ���ÿ���ٶ�(��׼ֵ)
	INT						m_nHorsesSpeedIndex[HORSES_ALL];	//ÿƥ���ٶ�����
	INT						m_nHorsesBasicSpeed;				//��ƥ�����ٶ�
	INT						m_nInFrameDelay;					//��ƥ��֡�ӳ�
	BYTE					m_cbHorsesIndex[HORSES_ALL];		//������
	TCHAR					m_szHorsesName[HORSES_ALL][HORSES_NAME_LENGTH];	//��ƥ����
	CPoint					m_ptHorsesPos[HORSES_ALL];			//��λ��
	CMyD3DTexture			m_ImageHorses[HORSES_ALL];			//��
	CDirectSound			m_HorsesSound;						//��ƥ����
	CDirectSound			m_GameOverSound;					//��ƥ����

	//��Ϸ����
public:
	bool					m_bGameOver;						//��Ϸ����
	CPoint					m_ptGameOver;						//λ��
	CD3DTexture				m_ImageGameOver;					//��ͼ
	CSkinButton				m_btGameOver;						//��ť

	//����
public:
	CMyD3DTexture			m_ImageBackdropHand;				//����ͷ
	CMyD3DTexture			m_ImageBackdropTail;				//����β
	CMyD3DTexture			m_ImageBackdrop;					//������
	CMyD3DTexture			m_ImageDistanceFrame;				//����

	//״̬��
public:
	CD3DTexture				m_ImageUserInfoL;					//��
	CD3DTexture				m_ImageUserInfoM;					//��
	CD3DTexture				m_ImageUserInfoR;					//��
	CD3DTexture				m_ImageUserInfoShu;					//������

	//������
public:
	CMyD3DTexture			m_ImagePenHand;						//Χ��ͷ
	CMyD3DTexture			m_ImagePenNum;						//Χ������
	CMyD3DTexture			m_ImagePenTail;						//Χ��β

	//ʱ��
public:
	CD3DTexture				m_ImageTimeNumer;					//����
	CD3DTexture				m_ImageTimeBet;						//��עʱ��
	CD3DTexture				m_ImageTimeBetEnd;					//��ע����
	CD3DTexture				m_ImageTimeFree;					//����ʱ��
	CD3DTexture				m_ImageTimeHorseEnd;				//�������
	CD3DTexture				m_ImageTimeHorse;					//����

	//�ؼ�����
public:
	CDialogPlayBet			m_DlgPlayBet;						//��ע����
	CDialogRecord			m_DlgRecord;						//��Ϸ��¼
	CDialogStatistics		m_DlgStatistics;					//ͳ�ƴ���
	CDialogBetRecord		m_DlgBetRecord;						//��¼����
	//CUserFaceWnd			m_WndUserFace;						//�������
	CDialogControl			m_DlgControl;						//���ƴ���
	CButton					m_btOpenControl;					//ϵͳ����

	//��ť
public:
	CSkinButton				m_btPlayerBetShow;					//��ʾ����Ͷע
	CSkinButton				m_btAllBetShow;						//��ʾȫ��Ͷע

	CSkinButton				m_btPlayerBet;						//����Ͷע
	CSkinButton				m_btStatistics;						//ͳ��
	CSkinButton				m_btExplain;						//˵��

	CSkinButton				m_btRecord;							//��ʷ��¼
	CSkinButton				m_btBetRecord;						//��ע��¼

	//������Դ
protected:
	CD3DTexture				m_TextureBack;						//������Դ

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

	//���溯��
protected:
	//��������
	virtual VOID CartoonMovie();
	//�����豸
	virtual VOID InitGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);



	//���ú���
public:
	//����ID
	void SetKindID( WORD wKindID ) { m_wKindID = wKindID; }
	//���ó���
	void SetStreak( INT nStreak ) { m_nStreak = nStreak; }
	//�����ҵ�λ��
	void SetMeChairID( WORD wMeChairID ) { m_wMeChairID = wMeChairID; }
	//����״̬
	void SetGameStatus(BYTE bGameStatus) { m_cbGameStatus = bGameStatus; }
	//������Ӯ
	void SetPlayerWinning( LONGLONG lPlayerWinning ) { m_lPlayerWinning = lPlayerWinning; }
	//������ע����ʱ��
	void SetBetEndTime( INT	nBetEndTime ) { m_nBetEndTime = nBetEndTime ;}
	//������ע����
	void SetBetPlayerCount( INT nBetPlayerCount) { m_nBetPlayerCount = nBetPlayerCount; }
	//����������
	void SetHorsesName( WORD wMeChairID, TCHAR szName[HORSES_NAME_LENGTH]);
	//����������ע
	void SetAllBet(BYTE cbArea, LONGLONG lScore);
	//���õ�����ע��ע
	void SetPlayerBet(WORD wMeChairID, BYTE cbArea, LONGLONG lScore);
	//��������
	void SetRanking(BYTE bRanking[RANKING_NULL]);



	//�滭����
private:
	//�滭����
	void DrawBackdrop( CD3DDevice * pD3DDevice );
	//�滭��
	void DrawHorses( CD3DDevice * pD3DDevice );
	//�滭ʱ����ʾ
	void DrawTimeClew( CD3DDevice * pD3DDevice );
	//�滭�����Ϣ
	void DrawUserInfo( CD3DDevice * pD3DDevice );
	//�滭������Ϣ
	void DrawGameOver( CD3DDevice * pD3DDevice );

	//���溯��
private:
	//��������
	void DrawTextString( CD3DDevice * pD3DDevice, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭����
	void DrawNumber(CD3DDevice * pD3DDevice , int nXPos, int nYPos, int nNumer, bool bTime = false );

	//������
public:
	//����ʼ
	void HorsesStart(INT nHorsesSpeed[HORSES_ALL][COMPLETION_TIME]);
	//�������
	void HorsesEnd();
	//��һ�ֿ�ʼ
	void NweHorses();

	//��ť��Ϣ
public:
	//������ע��ʾ
	afx_msg void OnPlayerBetShow();
	//ȫ����ע��ʾ
	afx_msg void OnAllBetShow();
	//������ע
	afx_msg void OnPlayerBet();
	//��ʷ��¼
	afx_msg void OnRecordShow();
	//ͳ�ƴ���
	afx_msg void OnStatistics();
	//˵��
	afx_msg void OnExplain();
	//�ɼ��ر�
	afx_msg void OnGameOverClose();
	//��ע�ɼ�
	afx_msg void OnBetRecordShow();
	//����Ա����
	afx_msg void OpenControlWnd();

	//��Ϣӳ��
public:
	//��ʱ��
	afx_msg void OnTimer(UINT nIDEvent);
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//��������
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
