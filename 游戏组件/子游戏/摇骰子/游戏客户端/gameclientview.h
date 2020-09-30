#pragma once

#include "Stdafx.h"
#include "ScoreView.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//��Ϣ����
#define IDM_START					(WM_APP+100)						//��ʼ��Ϣ
#define IDM_SETCHIP					(WM_APP+101)						//ѹע��Ϣ
#define IDM_THROW_DICE				(WM_APP+102)						//ҡ����Ϣ
#define IDM_LOOK_DICE				(WM_APP+103)						//������Ϣ
#define IDM_SCROLL					(WM_APP+104)						//��������	
#define IDM_DICE_NUM				(WM_APP+105)						//���ݵ���
#define IDM_YELL_DICE				(WM_APP+106)						//������Ϣ
#define IDM_OPEN_DICE				(WM_APP+107)						//������Ϣ
////////////////////////////////////////////////////////////////////////////////////////////////

#define SET_CHIP_COUNT				4									//��ע����	

//��������
enum enAnimalAction
{
	enTABegin = 1,	//������ʼ
	enTAPlay,		//����������
	enTAEnd			//��������
};

//��ť����
enum enUpdateButton
{
	enUBClickBegin = 1,    //�����ʼ
	enUBChip,			   //��ע
	enUBThrow,			   //ҡ��
	enUBYellFirst,		   //���ֺ���
	enUBYell,			   //����
	enUBOpen,			   //����
	enUBEnd,			   //����
	enTest				   //����
};

//��ť�ṹ��
struct tagButtonState
{	// 0 ���� ������ 1 ���� ���� 2 ��ʾ ������ 3 ��ʾ ����
	BYTE bStart;			//��ʼ��ť
	BYTE bSetChip;			//��ע��ť
	BYTE bThrowDice;		//ҡ����ť
	BYTE bLookDice;			//������ť
	BYTE bYellOK;			//������ť
	BYTE bDiceScroll;		//��ҳ��ť
	BYTE bDiceNum;			//���㰴ť
	BYTE bOpenDice;			//������ť
	void Set(BYTE bst, BYTE bsc, BYTE btd, BYTE bld, BYTE byo, BYTE bds, BYTE bdn, BYTE bod)
	{bStart = bst;bSetChip = bsc;bThrowDice = btd;bLookDice = bld;bYellOK = byo;bDiceScroll = bds;bDiceNum = bdn;bOpenDice = bod;}
};

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//�������
protected:
	BYTE							m_bDiceData[GAME_PLAYER][5];		//��������
	bool							m_bShowDice[GAME_PLAYER];			//��ʾ����
public:
	tagDiceYell						m_MaxYell;							//��󺰻�
	bool							m_bShowDiceCup[GAME_PLAYER];		//��ʾ���� 
	BYTE							m_bDiceCount;						//������Ŀ (��������ϵ�ǰѡ�����Ŀ)
	BYTE							m_bNowDicePoint;					//��ǰ���� (���������ѡ�������)
	int								m_nAnimalTime[GAME_PLAYER];			//����ʱ��
	OpenDice_Scene					m_SceneOpenDice;					//��������
	LONGLONG						m_lChip;							//��ע���

	//λ�ñ���
private:
	POINT							m_ptDiceCup[GAME_PLAYER];			//�������
	POINT							m_ptHandDice[GAME_PLAYER];			//��������
	POINT							m_ptYellPanel;						//������λ
	POINT							m_ptWordOpen[GAME_PLAYER];			//��������
	POINT							m_ptMaxYell;						//��󺰻�
	POINT							m_ptChip[GAME_PLAYER];				//��ע����

	INT								m_nXFace;
	INT								m_nYFace;
	INT								m_nXTimer;
	INT								m_nYTimer;

	//�ؼ�����
	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btSetChip[4];						//��ע��ť
	CSkinButton						m_btThrowDice;						//ҡ����ť
	CSkinButton						m_btLookDice;						//������ť
	CSkinButton						m_btYellOK;							//������ť
	CSkinButton						m_btDiceScroll[2];					//������ť (���·���2����ͷ)
	CSkinButton						m_btDiceNum[6];						//���㰴ť	
	CSkinButton						m_btOpenDice;						//������ť

	//λͼ����
protected:
	CBitImage						m_ImageBack;						//������Դ
	CBitImage						m_ImageCenter;						//������Դ
	CBitImage						m_ImageChipBack;					//��ע����
	CBitImage						m_ImageSezi;						//������Դ	
	CBitImage						m_ImageThrowDiceAnimal[GAME_PLAYER];//ҡ������
	CBitImage						m_ImageOpenDiceAnimal[GAME_PLAYER]; //���Ƕ���
			
	CPngImage						m_PngImageUserFrame;				//�û���Ϣ
	CPngImage						m_PngImageYellFrame;				//��������	
	CPngImage						m_PngImageJetton;					//������Դ
	CPngImage						m_PngImageWord;						//������Դ
	CPngImage						m_PngImageJettonSmall;				//������Դ
	CPngImage						m_PngImageNum1;						//��������
	CPngImage						m_PngImageNum2;						//ѡ������
	CPngImage						m_PngImageDiceCup;					//������Դ
	CPngImage						m_PngImageWordSucc;					//���ж���
	CPngImage						m_PngImageWordFail;					//û�п���
	CPngImage						m_PngImageSeziSmall;				//С������

	CPngImage						m_PngOpenDice0;						//���Ƕ���
	CPngImage						m_PngOpenDice1;						//���Ƕ���

	CPngImage						m_PngOpenDiceTop[29];				//���Ƕ���
	CPngImage						m_PngOpenDiceBottom[29];			//���Ƕ���

	//�ؼ����
public:
	CScoreView						m_ScoreView;						//������ͼ

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//��������
protected:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//ҡ������
	void DrawThrowDice(CDC *pDC, WORD wViewChair, int nTime, int nX, int nY);
	//�滭����
	void DrawDiceCup(CDC * pDC);
	//�滭��ע
	void DrawChip(CDC *pDC);
	//�滭����
	void DrawSezi(CDC *pDC);
	//�滭����
	inline void DrawYell(CDC *pDC, BYTE bDiceCount,  BYTE bDicePoint, int nX, int nY);
	//��󺰻�
	void DrawMaxYell(CDC *pDC, int nWidth, int nHeight);
	//�������
	void DrawYellPanel(CDC *pDC);
	//��������
	void DrawOpenDice(CDC *pDC, int nX, int nY);
	//�滭����
	void DrawPicNum(CDC *pDC, CPngImage *pImage, BYTE bNum, int nX, int nY);

	//���ܺ���
public:
	//��������
	void SetHandDice(WORD wChairID, bool bShow, BYTE bDiceData[]);
	//ҡ������
	void OnThrowAnimal(WORD wChairID, enAnimalAction enAction);
	//���°�ť
	void UpdateButton(enUpdateButton nType);
	//������ͼ
	void RefreshGameView();

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

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ʼ��ť
	afx_msg void OnStart();
	//����ѹע
	afx_msg void OnSetChip(UINT nCtrlID);
	//ҡ����ť
	afx_msg void OnThrowDice();
	//������ť
	afx_msg void OnLookDice();
	//�Ϸ���ť
	afx_msg void OnScrollUp();
	//�·���ť
	afx_msg void OnScrollDown();
	//���㰴ť
	afx_msg void OnDiceNum(UINT nCtrlID);
	//������ť
	afx_msg void OnYellOk();
	//������ť
	afx_msg void OnOpenDice();
	
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
