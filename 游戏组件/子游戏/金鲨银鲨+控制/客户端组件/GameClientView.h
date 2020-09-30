#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ObjectMove.h"
#include "D3DTextureIndex.h"
#include "GameLogic.h"
#include "WindowChip.h"
#include "WindowOver.h"
#include "FishDraw.h"
#include "WindowTip.h"
#include "ConstantlyButton.h"
#include "BetWnd.h"
#include "ClientControl.h"

//////////////////////////////////////////////////////////////////////////////////

//��ͼλ��
#define MYSELF_VIEW_ID				0									//��ͼλ��

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewD3D
{
	//��Ϸ����
protected:
	int										m_nCurrentNote;						//��ǰע
	int										m_nAnimalMultiple[ANIMAL_MAX];		//���ﱶ��
	BYTE									m_cbGameStatus;						//��Ϸ״̬

	//��Ϸ��¼
protected:
	CWHArray<int>							m_ArrayTurnTableRecord;				//ת�̼�¼
	BOOL									m_bShowTurnTableRecord;				//��ʾ��Ϸ��¼

	//��ұ���
protected:
	WORD									m_wMineChairID;						//�Լ�λ��
	LONGLONG								m_lPlayChip;						//��ҳ���
	LONGLONG								m_lPlayBet[ANIMAL_MAX];				//�����ע
	LONGLONG								m_lPlayAllBet[ANIMAL_MAX];			//���������ע
	
	//��ť���
protected:
	BOOL									m_bKeyDown[ANIMAL_MAX];				//��ť���
	BOOL									m_bKeySend[ANIMAL_MAX];				//��ť���
	CLapseCount								m_KeyLapse[ANIMAL_MAX];				//��ťʱ��

	//�÷ֱ���
protected:
	LONGLONG								m_lPlayWin[2];						//��ҵ÷�
	LONGLONG								m_lPlayPrizes;						//��Ҳʽ�
	LONGLONG								m_lPlayShowWin;						//�����ʾ�÷�
	LONGLONG								m_lPlayShowPrizes;					//�����ʾ�ʽ�
	CLapseCount								m_PlayPrizesLapse;					//�ʽ�ʱ��

	//λ�ñ���
protected:
	CPoint									m_ptBenchmark;						//��׼λ��
	CSize									m_sizeStage;						//��̨��С
	CPoint									m_ptAnimalMultiple[ANIMAL_MAX];		//���ﱶ��λ��
	CPoint									m_ptPlayBet[ANIMAL_MAX];			//�����עλ��

	//ת�̶���
protected:
	BOOL									m_bTurnTwoTime;						//ת2��
	int										m_nTurnTarget[2];					//ת��Ŀ��
	int										m_nPrizesMultiple;					//�ʽ�	

	int										m_nBrightCount;						//������
	int										m_nTurnTableCurrent;				//��ǰת������
	int										m_nTurnTableTarget;					//Ŀ��ת������ 
	int										m_nTurnTableCount;					//��ʱ���� 
	int										m_nTurnTableDelay;					//ת����ʱ
	int										m_nTurnCount;						//ת������
	int										m_nTurnTransit;						//��ת��
	int										m_nTurnTableLaps;					//Ȧ��
	BOOL									m_bTurnTable;						//��ʼתȦ
	CLapseCount								m_TurnTableLapse;					//תȦ��ʧʱ��

	//Ӯ����
protected:
	BOOL									m_nWinShow;							//Ӯ��ʾ
	int										m_nWinIndex;						//Ӯ����
	int										m_nWinFrame;						//Ӯ����
	CLapseCount								m_nWinFrameTime;					//Ӯ��֡ʱ��
	CLapseCount								m_nWinContinueTime;					//Ӯ����ʱ��

	BOOL									m_nPrizesShow;						//�ɲ���ʾ
	CLapseCount								m_nPrizesContinueTime;				//�ɲʳ���ʱ��
public:
	CButton							        m_btOpenAdmin;						//ϵͳ����
	//����
public:
	HINSTANCE						        m_hInst;
	IClientControlDlg*			           	m_pClientControlDlg;

	BOOL									m_nGoldSilverShow;					//��ʾ������
	CLapseCount								m_nGoldSilverContinueTime;			//���������ʱ��

	//���綯��
protected:
	BOOL									m_nLaserShow;						//��ʾ����
	BOOL									m_nLaserDown;						//��������
	int										m_nLaserY;							//����λ��

	//��������
protected:
	tgaAnimalsDraw							m_AnimalsDraw[TURAN_TABLE_MAX];		//��������λ��

	//��Դ����
protected:
	CD3DTexture								m_ImageKeepOut;						//�ڵ�
	CD3DTexture								m_ImageMainBack;					//����ͼƬ
	CD3DTextureIndex						m_ImageAnimals;						//����ͼƬ
	CD3DTextureIndex						m_ImageAnimalsBack;					//���ﱳ��
	CD3DTextureIndex						m_ImageAnimalFlicker[3];			//���ﱳ������
	CD3DTexture								m_ImageNumberGreen;					//��ɫ����
	CD3DTexture								m_ImageNumberRed;					//��ɫ����
	CD3DTexture								m_ImageNumberYellow;				//��ɫ����
	CD3DTexture								m_ImageNumberBlue;					//��ɫ����

	CD3DTexture								m_ImageTimeBack;					//ʱ�䱳��
	CD3DTexture								m_ImageTimeNumber;					//ʱ������
	CD3DTextureIndex						m_ImageAnimalsWin[ANIMAL_MAX];		//����Ӯ����
	CD3DTextureIndex						m_ImageWinPrizes;					//�ʽ�Ӯ����
	CD3DTextureIndex						m_ImageWinBright;					//Ӯ����
	CD3DTextureIndex						m_ImageAnimalsWinS[ANIMAL_MAX];		//����Ӯ����
	CD3DTexture								m_ImagePrizesNumber;				//�ʽ�����
	CD3DTextureIndex						m_ImageAnimalRecord;				//��Ϸ��¼

	CD3DTexture								m_ImageLaser[2];					//���⶯��

	CD3DTextureIndex						m_ImageSharkWin[2];					//����Ӯ����


	//��ť����
public:
	CConstantlyButton						m_btSelectBet[ANIMAL_MAX];			//ѡ����ע
	CVirtualButton							m_btOperationRenewal;				//��ѹ
	CVirtualButton							m_btOperationCancel;				//ȡ��
	CVirtualButton							m_btOperationSwitch;				//�л�

	//���ڱ���
public:
	CWindowChip								m_WindowChip;						//����һ�����
	CWindowOver								m_WindowOver;						//��������
	CFishDraw								m_FishDraw;							//����滭
	CWindowTip								m_WindowTip;						//���㴰��
	CBetWnd									m_BetWnd;							//��ע��ʾ����

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//���غ���
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


	//�滭����
protected:
	//�滭������Ϣ
	VOID DrawPlayInfo(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);

	// ���溯��
private:
	// �滭����
	void DrawNumber(CD3DDevice * pD3DDevice , CD3DTexture* pImageNumber, TCHAR* szImageNum, TCHAR* szOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT, BYTE cbAlpha = 255);	
	// �滭����
	void DrawRotateNumber(CD3DDevice * pD3DDevice , CD3DTexture* pImageNumber, TCHAR* szImageNum, TCHAR* szOutNum, INT nXPos, INT nYPos, UINT uFormat);	
	//�滭�ַ�
	VOID DrawTextString(CD3DDevice* pD3DDevice, CD3DFont * pD3DFont,  LPCTSTR pszString, CRect rcDraw, UINT nFormat, COLORREF crText, COLORREF crFrame);

	//���ú���
public:
	//����״̬
	VOID SetGameStatus(BYTE cbGameStatus) { m_cbGameStatus = cbGameStatus; }
	//�����Լ�λ��
	void SetMineChairID( WORD wMineChairID ) { m_wMineChairID = wMineChairID; }
	//���õ�ǰע
	void SetCurrentNote( int nCurrentNote ) { m_nCurrentNote = nCurrentNote; }
	//������ҳ���
	void SetPlayChip( LONGLONG lPlayChip ) { m_lPlayChip = lPlayChip; }
	//���������ע
	void SetPlayBet( LONGLONG lPlayBet[ANIMAL_MAX] ) { CopyMemory(m_lPlayBet, lPlayBet, sizeof(m_lPlayBet)); }
	//���������ע
	void SetPlayBet( LONGLONG lPlayBet, int nAnimalIndex ) { m_lPlayBet[nAnimalIndex] = lPlayBet; }
	//�������������ע
	void SetPlayAllBet( LONGLONG lPlayAllBet[ANIMAL_MAX] ) { CopyMemory(m_lPlayAllBet, lPlayAllBet, sizeof(m_lPlayAllBet)); }
	//�������������ע
	void SetPlayAllBet( LONGLONG lPlayAllBet, int nAnimalIndex ) { m_lPlayAllBet[nAnimalIndex] = lPlayAllBet; }
	//���ñ���
	void SetAnimalMultiple( int nAnimalMultiple[ANIMAL_MAX] ) { CopyMemory(m_nAnimalMultiple, nAnimalMultiple, sizeof(m_nAnimalMultiple)); }
	//������ʾ��¼
	void SetShowTurnTableRecord( BOOL bShowTurnTableRecord ) { m_bShowTurnTableRecord = bShowTurnTableRecord; }
	//������ʾ�÷�
	void SetPlayShowWin( LONGLONG lPlayShowWin ) { m_lPlayShowWin = lPlayShowWin; }
	//������ʾ�ʽ�
	void SetPlayShowPrizes( LONGLONG lPlayShowPrizes ) { m_lPlayShowPrizes = lPlayShowPrizes; }
	//��Ӽ�¼
	void AddTurnTableRecord( int nAnimalIndex );

	//���ܺ���
public:
	//��Ϸ����
	void GameOver( BOOL bTurnTwoTime, int nTurnTableTarget[2], int nPrizesMultiple, LONGLONG lPlayWin[2], LONGLONG lPlayPrizes );
	//��ʼתȦ
	void BeginTurnTable(  bool bFirst, int nTurnTableTarget );
	//����תȦ
	void EndTurnTable();
	//��ʾӮ
	void BeginWin( int nAnimalIndex );
	//����Ӯ
	void EndWin();
	//��ʾ����
	void BeginLaser();
	//��������
	void EndLaser();

	//��ʾ������
	void BeginGoldSilver();
	//����������
	void EndGoldSilver();


	// ��ť��Ϣ
public:
	// ʨ�Ӱ�ť
	VOID OnBnClickedAnimalLion();
	// ��è��ť
	VOID OnBnClickedAnimalPanda();
	// ���Ӱ�ť
	VOID OnBnClickedAnimalMonkey();
	// ���Ӱ�ť
	VOID OnBnClickedAnimalRabbit();
	// ��ӥ��ť
	VOID OnBnClickedAnimalEagle();
	// ��ȸ��ť
	VOID OnBnClickedAnimalPeacock();
	// ���Ӱ�ť
	VOID OnBnClickedAnimalPigeon();
	// ���Ӱ�ť
	VOID OnBnClickedAnimalSwallow();
	// ���谴ť
	VOID OnBnClickedAnimalSliverShark();
	// ���谴ť
	VOID OnBnClickedAnimalGoldShark();
	// ���ݰ�ť
	VOID OnBnClickedAnimalBird();
	// ���ް�ť
	VOID OnBnClickedAnimalBeast();
	// ��ѹ��ť
	VOID OnBnClickedOperationRenewal();
	// ȡ����ť
	VOID OnBnClickedOperationCancel();
	// �л���ť
	VOID OnBnClickedOperationSwitch();
	//�������
	void AllowControl(BYTE cbStatus);
protected:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);


	//��Ϣ����
public:
	//������
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//������
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//ϵͳ��Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//��ʱ��
	afx_msg void OnTimer(UINT nIDEvent);
	//���̰���
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//��������
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//ʧȥ����
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//����Ա����
	afx_msg void OpenAdminWnd();


	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////////////

#endif