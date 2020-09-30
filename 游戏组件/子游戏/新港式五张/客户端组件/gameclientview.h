#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "HistoryRecord.h"
#include "GameLogic.h"
#include "PlayOperate.h"
#include "MoveDraw.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ͼλ��
#define MYSELF_VIEW_ID				2									//��ͼλ��

//������ʶ
#define AC_NO_ADD					0									//���Ӷ���
#define AC_FOLLOW					1									//��ע����
#define AC_ADD_SCORE				2									//��ע����
#define AC_SHOW_HAND				3									//�������
#define AC_GIVE_UP					4									//��������

//������Ϣ
#define IDM_START					(WM_USER+200)						//��ʼ��Ϣ
#define IDM_FOLLOW					(WM_USER+201)						//��ע��Ϣ
#define IDM_GIVE_UP					(WM_USER+202)						//������Ϣ
#define IDM_ADD_SCORE				(WM_USER+203)						//��ע��Ϣ
#define IDM_SHOW_HAND				(WM_USER+204)						//�����Ϣ

//֪ͨ��Ϣ
#define IDM_SEND_CARD_FINISH		(WM_USER+300)						//�������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��������
struct tagSendCardItem
{
	WORD							wChairID;							//�����û�
	BYTE							cbCardData;							//��������
};

//����˵��
typedef CWHArray<tagSendCardItem,tagSendCardItem &>	CSendCardItemArray;
typedef CWHArray<stuMoveChip*>						CChipArray;


//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//״̬����
protected:
	BOOL							m_bFirstChange;				
	LONG							m_lCellScore;						//��Ϸ��ע
	LONGLONG						m_lDrawMaxScore;					//�����
	LONGLONG						m_lUserScore[GAME_PLAYER];			//�û�����
	LONGLONG						m_lUserAddScore[GAME_PLAYER];		//�û���������ֵ
	LONGLONG						m_lUserAddAllScore[GAME_PLAYER];	//�û���������ֵ
	LONGLONG						m_lUserSaveScore[GAME_PLAYER];		//�û���������
	LONGLONG						m_lTableScore[GAME_PLAYER];			//��ע��Ŀ
	LONGLONG						m_lOverScore[GAME_PLAYER];			//��������
	LONGLONG						m_lOverSaveScore[GAME_PLAYER];		//�����������

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

	//����״̬
protected:
	bool							m_bShowScore;						//����״̬
	tagHistoryScore	*				m_pHistoryScore[GAME_PLAYER];		//������Ϣ

	//���ư�ť
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CPlayOperate					m_PlayOperate;						//��Ҳ���

	//��Ϸ�ؼ�
public:
	CGameLogic						m_GameLogic;
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�

	//�˿˳���
public:
	CCardControl					m_CardControl[GAME_PLAYER];			//�˿˿ؼ�
	LONGLONG						m_lPalyBetChip[GAME_PLAYER];		//�����ע
	LONGLONG						m_lALLBetChip;						//������ע
	CChipArray						m_ArrayPlayChip;					//�����ע����
	CChipArray						m_ArrayBetChip;						//��ע����
	CChipArray						m_ArrayOverChip;					//��������

	CRect							m_rectBetChip;						//��ע����
	CSize							m_sizeWin;							//���ڴ�С


	//λ�ñ���
protected:
	CPoint							m_ptUserInfo[GAME_PLAYER];			//�����Ϣ
	CPoint							m_ptUserAction[GAME_PLAYER];		//����λ��
	CPoint							m_ptPalyBetChip[GAME_PLAYER];		//�����ע
	CPoint							m_ptPalyPos[GAME_PLAYER];			//�����ע
	CPoint							m_ptOverScore[GAME_PLAYER];			//��������

	//��Դ��С
protected:
	CSize							m_SizeHistory;						//���ִ�С
	CSize							m_SizeGameCard;						//�˿˴�С

	//��Դ����
protected:
	CPngImage						m_ImageReady;						// ׼��

	CPngImage						m_ImageGameCard;					// �˿���Դ
	CBitImage						m_ImageViewFill;					// ������Դ
	CPngImage						m_ImageViewBack;					// ������Դ

	CPngImage						m_ImageActionBack;					// ��������
	CPngImage						m_ImageActionFont;					// ����
	CPngImage						m_ImageShowHand;					// ���

	CPngImage						m_ImageUserInfoH;					// ��ұ�����
	CPngImage						m_ImageUserInfoV;					// ��ұ�����
	CPngImage						m_ImageUserPalyH;					// ��ұ�����
	CPngImage						m_ImageUserPalyV;					// ��ұ�����
	CPngImage						m_ImageChip;						// ����λͼ

	CPngImage						m_ImageCellScore;					// �ͷֱ���
	CPngImage						m_ImgaeScoreTotal;					// �������
	CPngImage						m_ImageTipInfo;						// С��ʿ

	CPngImage						m_ImageCellNumber;					// �ͷ�����
	CPngImage						m_ImgaeScoreTotalNumber;			// ��ע����

	CPngImage						m_ImgaeScoreBackR;					// ��������
	CPngImage						m_ImgaeScoreBackL;					// ��������
	CPngImage						m_ImgaeScoreBackM;					// ��������

	CPngImage						m_ImgaeScoreSum;					// ��������
	CPngImage						m_ImgaeScoreWin;					// ��������
	CPngImage						m_ImgaeScoreLose;					// ��������

	CPngImage						m_ImageTime;						// ʱ������

	CPngImage						m_ImageCradType;					// ����
	CDFontEx						m_DFontEx;							// ����

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

	//���ƽӿ�
public:
	//���ý���
	virtual VOID ResetGameView();

	//�̳к���
protected:
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//�Ƿ��������
	virtual bool RealizeWIN7() { return true; }

	//���ƺ���
public:
	//�Ƿ���
	bool IsDispatchStatus();
	//ֹͣ����
	VOID ConcludeDispatch();
	//�����˿�
	VOID DispatchUserCard(WORD wChairID, BYTE cbCardData);

	//���ܺ���
public:
	//��Ϸ��ע
	bool SetCellScore(LONG lCellScore);
	//�����ע
	bool SetDrawMaxScore(LONGLONG lDrawMaxScore);
	//���û���
	bool SetUserScore(WORD wChairID, LONGLONG lUserScore);
	//���û���
	bool SetUserSaveScore(WORD wChairID, LONGLONG lUserScore);
	//���ý�������
	bool SetOverScore(WORD wChairID, LONGLONG lUserScore);
	//���ö���
	bool SetUserAction(WORD wActionUser, BYTE cbUserAction);
	//���û���
	bool SetHistoryScore(WORD wChairID,tagHistoryScore * pHistoryScore);
	//������ע
	void SetUserTableScore(WORD wChairID, LONGLONG lTableScore);
	// ���������ע
	void AddDeskChip( LONGLONG lScore, WORD wChairID );
	// ��������ע
	void AddPlayChip( LONGLONG lScore, WORD wChairID );
	// �����Ҽ�ע
	void AddPlayBetChip( LONGLONG lScore, WORD wChairID );
	// �������
	void ChioRecovery( WORD wChairID );

	
	//�ڲ�����
private:
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_LEFT);
	// �滭�ַ�
	bool DrawTextStringEx(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, INT nDrawFormat);
	// �滭�ַ�
	bool DrawTextStringEx(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, INT nXPos, INT nYPos);
	// �滭��������
	void DrawOverScore(CDC * pDC, LONGLONG lOutNum, INT nXPos, INT nYPos );
	// ��Ӷ���
	CString AddComma( LONGLONG lScore );

	//��ť��Ϣ
public:
	//��ʼ��ť
	VOID OnBnClickedStart();
	//��ע��ť
	VOID OnBnClickedFollow();
	//������ť
	VOID OnBnClickedGiveUp();
	//��ע��ť
	VOID OnBnClickedAddScore();
	//��ע��ť
	VOID OnBnClickedAddScoreOne();
	//��ע��ť
	VOID OnBnClickedAddScoreTwo();
	//��ע��ť
	VOID OnBnClickedAddScoreThree();
	//��ע��ť
	VOID OnBnClickedAddScoreFour();
	//�����ť
	VOID OnBnClickedShowHand();
	//���ְ�ť
	VOID OnBnClickedADD();
	//���ְ�ť
	VOID OnBnClickedSUB();

	//��Ϣ����
public:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//��������
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	// ��Ϣ����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif