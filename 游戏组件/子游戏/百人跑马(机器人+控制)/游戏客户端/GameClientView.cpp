#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientDlg.h"
#include ".\gameclientview.h"


//////////////////////////////////////////////////////////////////////////
//��ʱ��ʶ		
#define IDI_HORSES_START			101							//����ʼ
#define IDI_HORSES_END				102							//�������
#define IDI_PLAY_SOUNDS				103							//��������

//��ť��ʶ
#define IDC_PLAYER_BET_SHOW			201							//����Ͷע
#define IDC_ALL_BET_SHOW			202							//ȫ��Ͷע
#define IDC_PLAYER_BET				203							//��ע
#define IDC_TONGJI					204							//ͳ��
#define IDC_SHUOMING				205							//˵��
#define IDC_JILU					206							//��¼
#define IDC_GAME_OVER				207							//��Ϸ����
#define IDC_BET_RECORD				208							//��ע��¼
#define IDC_ADMIN					209							//��ť��ʶ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewD3D)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN() 
	ON_BN_CLICKED(IDC_ALL_BET_SHOW, OnAllBetShow)
	ON_BN_CLICKED(IDC_PLAYER_BET_SHOW, OnPlayerBetShow)
	ON_BN_CLICKED(IDC_PLAYER_BET, OnPlayerBet)
	ON_BN_CLICKED(IDC_JILU, OnRecordShow)
	ON_BN_CLICKED(IDC_TONGJI, OnStatistics)
	ON_BN_CLICKED(IDC_GAME_OVER, OnGameOverClose)
	ON_BN_CLICKED(IDC_BET_RECORD, OnBetRecordShow)
	ON_BN_CLICKED(IDC_SHUOMING, OnExplain)
	ON_BN_CLICKED(IDC_ADMIN,OpenControlWnd)
	//�Զ���Ϣ
	ON_WM_DESTROY()
//	ON_WM_SIZE()
END_MESSAGE_MAP()

//���캯��
CGameClientView::CGameClientView()
{
	//������Ϣ
	m_nStreak = INT_MAX;
	m_wKindID = INVALID_WORD;
	ZeroMemory(&m_tagBenchmark, sizeof(m_tagBenchmark));
	m_nBetEndTime = 0;

	//��Ϸ����
	m_wMeChairID = INVALID_CHAIR;
	m_cbGameStatus = GS_BET;
	m_bMeOrAll = true;
	m_bRacing = false;

	//��Ϸ��Ϣ
	for ( int i = 0; i < CountArray(m_nMultiple); ++i)
		m_nMultiple[i] = 1;
	for ( int i = 0; i < CountArray(m_bRanking); ++i)
		m_bRanking[i] = HORSES_ALL;

	//��
	ZeroMemory(m_cbHorsesIndex, sizeof(m_cbHorsesIndex));
	ZeroMemory(m_ptHorsesPos, sizeof(m_ptHorsesPos));
	ZeroMemory(m_nHorsesSpeed, sizeof(m_nHorsesSpeed));
	ZeroMemory(m_nHorsesSpeedIndex, sizeof(m_nHorsesSpeedIndex));
	ZeroMemory(m_szHorsesName, sizeof(m_szHorsesName));
	m_nHorsesBasicSpeed = HAND_LENGTH;
	m_nInFrameDelay = 0;

	//��Ϸ����
	m_bGameOver = false;

	//��Ϸ����
	m_nBetPlayerCount = 0;
	memset(m_lPlayerBet, 0, sizeof(m_lPlayerBet));
	memset(m_lPlayerBetAll, 0, sizeof(m_lPlayerBetAll));
	m_lPlayerWinning = 0l;

	m_ptHorsesPos[HORSES_ONE].SetPoint(HORSES_X_POS, HORSES_ONE_Y_POS);
	m_ptHorsesPos[HORSES_TWO].SetPoint(HORSES_X_POS, HORSES_TWO_Y_POS);
	m_ptHorsesPos[HORSES_THREE].SetPoint(HORSES_X_POS, HORSES_THREE_Y_POS);
	m_ptHorsesPos[HORSES_FOUR].SetPoint(HORSES_X_POS, HORSES_FOUR_Y_POS);
	m_ptHorsesPos[HORSES_FIVE].SetPoint(HORSES_X_POS, HORSES_FIVE_Y_POS);
	m_ptHorsesPos[HORSES_SIX].SetPoint(HORSES_X_POS, HORSES_SIX_Y_POS);

}

//��������
CGameClientView::~CGameClientView()
{

}

//��������
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGameFrameView::OnCreate(lpCreateStruct) == -1)
		return -1;

	HINSTANCE hInstance = AfxGetInstanceHandle();
	CRect CreateRect(0,0,0,0);

	////�������
	//if( m_WndUserFace.GetSafeHwnd() == NULL )
	//{
	//	m_WndUserFace.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CreateRect, this, 1001);
	//	m_WndUserFace.SetFrameView(this);
	//}

	//��ע�ؼ�
	if( m_DlgPlayBet.GetSafeHwnd() == NULL )
		m_DlgPlayBet.Create(IDD_DIALOG_BET,this);

	//��ʷ��¼
	if( m_DlgRecord.GetSafeHwnd() == NULL )
		m_DlgRecord.Create(IDD_DIALOG_LOG,this);

	//ͳ�ƴ���
	if( m_DlgStatistics.GetSafeHwnd() == NULL )
		m_DlgStatistics.Create(IDD_DIALOG_TONGJI,this);

	//��¼����
	if( m_DlgBetRecord.GetSafeHwnd() == NULL )
		m_DlgBetRecord.Create(IDD_DIALOG_BET_LOG,this);

	//��ť
	m_btPlayerBetShow.Create(TEXT(""),WS_CHILD|WS_VISIBLE|WS_DISABLED,CreateRect,this,IDC_PLAYER_BET_SHOW);
	m_btAllBetShow.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CreateRect,this,IDC_ALL_BET_SHOW);
	m_btPlayerBet.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CreateRect,this,IDC_PLAYER_BET);
	m_btStatistics.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CreateRect,this,IDC_TONGJI);
	m_btExplain.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CreateRect,this,IDC_SHUOMING);
	m_btRecord.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CreateRect,this,IDC_JILU);
	m_btGameOver.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_GAME_OVER);
	m_btBetRecord.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CreateRect,this,IDC_BET_RECORD);

	m_btPlayerBetShow.SetButtonImage(IDB_BT_GEREN,hInstance,false,false);
	m_btAllBetShow.SetButtonImage(IDB_BT_QUANCHANG,hInstance,false,false);
	m_btPlayerBet.SetButtonImage(IDB_BT_XIAZHU,hInstance,false,false);
	m_btStatistics.SetButtonImage(IDB_BT_TONGJI,hInstance,false,false);
	m_btExplain.SetButtonImage(IDB_BT_SHUOMING,hInstance,false,false);
	m_btRecord.SetButtonImage(IDB_BT_HISTORYSCORE,hInstance,false,false);
	m_btGameOver.SetButtonImage(IDB_BT_CLOSEGAMESCORE,hInstance,false,false);
	m_btBetRecord.SetButtonImage(IDB_BT_JILU,hInstance,false,false);

	//��ƥ����
	m_HorsesSound.Create(TEXT("HOS"));
	m_GameOverSound.Create(TEXT("WAV_GAME_OVER"));

	m_btOpenControl.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_ADMIN);
	m_btOpenControl.ShowWindow(SW_HIDE);

	
	return 0;
}


//���ý���
void CGameClientView::ResetGameView()
{
	//������Ϣ
	m_wKindID = INVALID_WORD;

	//��Ϸ����
	m_bRacing = false;
	ZeroMemory(&m_tagBenchmark, sizeof(m_tagBenchmark));
	for ( int i = 0; i < CountArray(m_bRanking); ++i)
		m_bRanking[i] = HORSES_ALL;
	m_nBetEndTime = 0;
	m_bMeOrAll = true;

	//��
	ZeroMemory(m_cbHorsesIndex, sizeof(m_cbHorsesIndex));
	ZeroMemory(m_ptHorsesPos, sizeof(m_ptHorsesPos));
	ZeroMemory(m_nHorsesSpeed, sizeof(m_nHorsesSpeed));
	ZeroMemory(m_nHorsesSpeedIndex, sizeof(m_nHorsesSpeedIndex));
	m_nHorsesBasicSpeed = HAND_LENGTH;

	//��Ϸ����
	m_nBetPlayerCount = 0;
	memset(m_lPlayerBet, 0, sizeof(m_lPlayerBet));
	memset(m_lPlayerBetAll, 0, sizeof(m_lPlayerBetAll));
	m_lPlayerWinning = 0l;

	//��Ϸ����
	m_bGameOver = false;

}

//�����ؼ�
void CGameClientView::RectifyControl( int nWidth, int nHeight )
{

	if( m_tagBenchmark.ptBase.x == m_szTotalSize.cx - m_tagBenchmark.nWidth )
	{
		m_tagBenchmark.ptBase.x = m_szTotalSize.cx - nWidth;
	}
	m_tagBenchmark.nWidth = nWidth;
	m_tagBenchmark.nHeight = nHeight;

	
	if ( m_DlgPlayBet.GetSafeHwnd() )
		m_DlgPlayBet.ShowWindow(SW_HIDE);

	if ( m_DlgRecord.GetSafeHwnd() )
		m_DlgRecord.ShowWindow(SW_HIDE);

	if ( m_DlgStatistics.GetSafeHwnd() )
		m_DlgStatistics.ShowWindow(SW_HIDE);

	if ( m_DlgBetRecord.GetSafeHwnd() )
		m_DlgBetRecord.ShowWindow(SW_HIDE);

	////����
	//if ( m_WndUserFace.GetSafeHwnd() )
	//	m_WndUserFace.MoveWindow(18, m_tagBenchmark.nHeight - m_ImageUserInfoL.GetHeight() + 20, 34, 34);

	//������Ϸλ��
	m_ptGameOver.SetPoint( m_tagBenchmark.nWidth / 2 - m_ImageGameOver.GetWidth()/2, m_tagBenchmark.nHeight/2 - m_ImageGameOver.GetHeight()/2);

	//��ť
	CRect rcButton;
	HDWP hDwp = BeginDeferWindowPos( 32 );
	const UINT uFlags = SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	m_btPlayerBetShow.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp, m_btPlayerBetShow, NULL, 190, nHeight - 104, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btAllBetShow, NULL, 190 + rcButton.Width(), nHeight - 104, 0, 0, uFlags);

	m_btPlayerBet.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp, m_btPlayerBet, NULL, m_ImageUserInfoL.GetWidth() + (nWidth - m_ImageUserInfoL.GetWidth()) / 3 / 2 - rcButton.Width()/2, nHeight - 134, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btStatistics, NULL, m_ImageUserInfoL.GetWidth() + (nWidth - m_ImageUserInfoL.GetWidth()) / 3 * 1 + (nWidth - m_ImageUserInfoL.GetWidth()) / 3 / 2 - rcButton.Width()/2, nHeight - 134, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btExplain, NULL, m_ImageUserInfoL.GetWidth() +  (nWidth - m_ImageUserInfoL.GetWidth()) / 3 * 2 + (nWidth - m_ImageUserInfoL.GetWidth()) / 3 / 2 - rcButton.Width()/2, nHeight - 134, 0, 0, uFlags);

	DeferWindowPos(hDwp, m_btRecord, NULL, nWidth - 110, 5, 0, 0, uFlags);

	DeferWindowPos(hDwp, m_btGameOver, NULL, m_ptGameOver.x + 493 , m_ptGameOver.y + 43, 0, 0, uFlags);

	DeferWindowPos(hDwp, m_btBetRecord, NULL, 42, nHeight - 50 , 0, 0, uFlags);
	

	EndDeferWindowPos(hDwp);
}

//�滭����
void CGameClientView::DrawGameView(CD3DDevice * pD3DDevice, int nWidth, int nHeight)
{
	m_tagBenchmark.nWidth = nWidth;
	m_tagBenchmark.nHeight = nHeight;

	TRACE(TEXT("Horse DrawGameView\n"));

	//����
	DrawBackdrop(pD3DDevice);

	//��
	DrawHorses(pD3DDevice);

	//ʱ����ʾ
	DrawTimeClew(pD3DDevice);

	//�����Ϣ
	DrawUserInfo(pD3DDevice);

	//������Ϣ
	DrawGameOver(pD3DDevice);

}

//�滭����
void CGameClientView::DrawBackdrop( CD3DDevice * pD3DDevice )
{
	//����
	m_ImageBackdropHand.DrawImage(pD3DDevice, &m_tagBenchmark, 0, 0);
	int nBackX = m_ImageBackdropHand.GetWidth();
	for (int i = 0; i < BACK_COUNT; ++i)
	{
		m_ImageBackdrop.DrawImage(pD3DDevice, &m_tagBenchmark, nBackX, 0);
		nBackX += m_ImageBackdrop.GetWidth();
	}
	m_ImageBackdropTail.DrawImage(pD3DDevice, &m_tagBenchmark, m_szTotalSize.cx - m_ImageBackdropTail.GetWidth(), 0);
}

//�滭��
void CGameClientView::DrawHorses( CD3DDevice * pD3DDevice )
{
	//Χ����ʼ
	INT nPenX = 123;

	//��
	for ( int nHorses = 0; nHorses < HORSES_ALL; ++nHorses )
	{
		INT nHorsesIndex = HORSES_ALL - nHorses - 1;
		m_ImageHorses[nHorsesIndex].DrawImage(pD3DDevice, &m_tagBenchmark, m_ptHorsesPos[nHorsesIndex].x, m_ptHorsesPos[nHorsesIndex].y, 
									m_ImageHorses[nHorsesIndex].GetWidth() / 5, m_ImageHorses[nHorsesIndex].GetHeight(), 
									m_ImageHorses[nHorsesIndex].GetWidth() / 5 * m_cbHorsesIndex[nHorsesIndex], 0);

		//����
		//if ( m_bRacing )
		//{
		//	DrawTextString(pD3DDevice, m_szHorsesName[nHorsesIndex], D3DCOLOR_XRGB(255,20,20), D3DCOLOR_XRGB(230,230,230),
		//		m_ptHorsesPos[nHorsesIndex].x + m_ImageHorses[nHorsesIndex].GetWidth() / 5 / 2 + 35, m_ptHorsesPos[nHorsesIndex].y + 8);
		//}

		//Χ����
		m_ImagePenNum.DrawImage(pD3DDevice, &m_tagBenchmark, nPenX + m_ImagePenHand.GetWidth(), m_ptHorsesPos[nHorsesIndex].y + 75,
								m_ImagePenNum.GetWidth() / 6, m_ImagePenNum.GetHeight(), 
								m_ImagePenNum.GetWidth() / 6 * nHorsesIndex, 0);

	}

	//����
	INT nXPos = (m_szTotalSize.cx - HAND_LENGTH - TAIL_LENGTH) / 12;
	for ( int i = 0; i < 11; i ++ )
	{
		m_ImageDistanceFrame.DrawImage(pD3DDevice, &m_tagBenchmark, nXPos, 429, 
			m_ImageDistanceFrame.GetWidth()/11 , m_ImageDistanceFrame.GetHeight(),
			m_ImageDistanceFrame.GetWidth()/11 * i , 0);
		nXPos += (m_szTotalSize.cx - HAND_LENGTH - TAIL_LENGTH) / 12;
	}

	//Χ��ͷ
	m_ImagePenHand.DrawImage(pD3DDevice, &m_tagBenchmark, nPenX, 131);

	//Χ��β
	m_ImagePenTail.DrawImage(pD3DDevice, &m_tagBenchmark, m_szTotalSize.cx - 239, 56);
}

//�滭ʱ����ʾ
void CGameClientView::DrawTimeClew( CD3DDevice * pD3DDevice )
{
	if ( m_wMeChairID == INVALID_CHAIR || m_nStreak == INT_MAX)
		return;

	int nUserTime = GetUserClock(m_wMeChairID); 

	if ( nUserTime < 0 )
		return;

	if ( m_cbGameStatus == GS_FREE )
	{
		m_ImageTimeFree.DrawImage(pD3DDevice, 50, m_tagBenchmark.nHeight - 175);
		DrawNumber(pD3DDevice, 193, m_tagBenchmark.nHeight - 175,  nUserTime/60, true);
		DrawNumber(pD3DDevice, 253, m_tagBenchmark.nHeight - 175,  nUserTime%60, true);
		DrawNumber(pD3DDevice, 525, m_tagBenchmark.nHeight - 175,  m_nStreak + 1);
	}
	else if ( m_cbGameStatus == GS_BET )
	{
		nUserTime += m_nBetEndTime;
		m_ImageTimeBet.DrawImage(pD3DDevice, 50, m_tagBenchmark.nHeight - 175);
		DrawNumber(pD3DDevice, 497, m_tagBenchmark.nHeight - 175,  nUserTime/60, true);
		DrawNumber(pD3DDevice, 558, m_tagBenchmark.nHeight - 175,  nUserTime%60, true);
		DrawNumber(pD3DDevice, 327, m_tagBenchmark.nHeight - 175,  m_nStreak + 1);
	}
	else if ( m_cbGameStatus == GS_BET_END )
	{
		m_ImageTimeBetEnd.DrawImage(pD3DDevice, 50, m_tagBenchmark.nHeight - 175);
		DrawNumber(pD3DDevice, 497, m_tagBenchmark.nHeight - 175,  nUserTime/60, true);
		DrawNumber(pD3DDevice, 558, m_tagBenchmark.nHeight - 175,  nUserTime%60, true);
		DrawNumber(pD3DDevice, 327, m_tagBenchmark.nHeight - 175,  m_nStreak + 1);
	}
	else if ( m_cbGameStatus == GS_HORSES )
	{
		if( m_bRacing )
		{
			m_ImageTimeHorse.DrawImage(pD3DDevice, 50, m_tagBenchmark.nHeight - 175);
		}
		else
		{
			m_ImageTimeHorseEnd.DrawImage(pD3DDevice, 50, m_tagBenchmark.nHeight - 175);
			DrawNumber(pD3DDevice, 194, m_tagBenchmark.nHeight - 175,  nUserTime/60, true);
			DrawNumber(pD3DDevice, 255, m_tagBenchmark.nHeight - 175,  nUserTime%60, true);
		}
	}
}

//�滭�����Ϣ
void CGameClientView::DrawUserInfo( CD3DDevice * pD3DDevice )
{
	//����
	m_ImageUserInfoL.DrawImage(pD3DDevice, 0, m_tagBenchmark.nHeight - m_ImageUserInfoL.GetHeight());
	for ( int nX = m_ImageUserInfoL.GetWidth(); nX < m_tagBenchmark.nWidth; nX += m_ImageUserInfoM.GetWidth())
	{
		m_ImageUserInfoM.DrawImage(pD3DDevice, nX, m_tagBenchmark.nHeight - m_ImageUserInfoM.GetHeight());
	}
	m_ImageUserInfoR.DrawImage(pD3DDevice, m_tagBenchmark.nWidth - m_ImageUserInfoR.GetWidth(), m_tagBenchmark.nHeight - m_ImageUserInfoR.GetHeight());

	//������Ϣ
	if ( m_wMeChairID == INVALID_CHAIR)
		return;

	IClientUserItem * pUserData = GetClientUserItem( m_wMeChairID );
	//m_WndUserFace.SetUserData(pUserData);
	if ( pUserData == NULL )
		return;

	DrawUserAvatar(pD3DDevice,20,m_tagBenchmark.nHeight - m_ImageUserInfoL.GetHeight() + 21,32,32,pUserData);

	TCHAR szUserInfo[128];
	CRect rcUserInfo(60, m_tagBenchmark.nHeight - m_ImageUserInfoL.GetHeight() + 13, 190, m_tagBenchmark.nHeight - m_ImageUserInfoL.GetHeight() + 28);
	_sntprintf(szUserInfo, sizeof(szUserInfo), TEXT("�û�����%s"), pUserData->GetNickName());
	m_D3DFont.DrawText(pD3DDevice, szUserInfo, rcUserInfo, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_LEFT, D3DCOLOR_XRGB(255,255,255));

	//�����ע
	LONGLONG lUserAllBet = 0;
	for ( int i = 0 ; i < AREA_ALL ; ++i)
		lUserAllBet += m_lPlayerBet[i];

	rcUserInfo.OffsetRect(0, 18);
	_sntprintf(szUserInfo, sizeof(szUserInfo), TEXT("��Ϸ�ң�%I64d"), pUserData->GetUserScore() - lUserAllBet );
	m_D3DFont.DrawText(pD3DDevice, szUserInfo, rcUserInfo, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_LEFT , D3DCOLOR_XRGB(255,255,255));

	rcUserInfo.OffsetRect(0, 18);
	_sntprintf(szUserInfo, sizeof(szUserInfo), TEXT("��  ע��%I64d"),lUserAllBet);
	m_D3DFont.DrawText(pD3DDevice, szUserInfo, rcUserInfo, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_LEFT , D3DCOLOR_XRGB(255,255,255));

	//��ע�������û���
	LONGLONG lAllBet = 0;
	for ( int i = 0 ; i < AREA_ALL ; ++i)
		lAllBet += m_lPlayerBetAll[i];
	m_DlgPlayBet.SetScore(pUserData->GetUserScore() - lUserAllBet);
	m_DlgRecord.SetScore(pUserData->GetUserScore() - lUserAllBet, lAllBet, m_nBetPlayerCount);


	//��ע��Ϣ----------------------------------------------------------
	//��׼λ��
	CPoint ptBenchmark;
	//��ʾ��С
	CSize sizeDisplayColumn;	
	ptBenchmark.SetPoint( m_ImageUserInfoL.GetWidth(), m_tagBenchmark.nHeight - m_ImageUserInfoL.GetHeight() + 52);
	sizeDisplayColumn.SetSize( m_tagBenchmark.nWidth - m_ImageUserInfoL.GetWidth(), 84);

	//����
	for ( int i = 1; i < 5; ++i )
	{
		m_ImageUserInfoShu.DrawImage(pD3DDevice, ptBenchmark.x + sizeDisplayColumn.cx / 5 * i, ptBenchmark.y);
	}

	//��ʾ��ע
	CRect rcBetName;
	CRect rcBetCount;
	//��ע
	LONGLONG lBet[AREA_ALL] = {0};
	TCHAR szBet[64] = {0};
	if ( m_bMeOrAll )
		memcpy(lBet, m_lPlayerBet, sizeof(lBet));
	else
		memcpy(lBet, m_lPlayerBetAll, sizeof(lBet));

	//��������
	rcBetName.SetRect( ptBenchmark.x , ptBenchmark.y, ptBenchmark.x + sizeDisplayColumn.cx / 5, ptBenchmark.y + sizeDisplayColumn.cy / 6);
	m_D3DFont.DrawText(pD3DDevice, TEXT("1 - 6"), rcBetName, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));
	rcBetName.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	m_D3DFont.DrawText(pD3DDevice, TEXT("1 - 5"), rcBetName, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));
	rcBetName.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	m_D3DFont.DrawText(pD3DDevice, TEXT("1 - 4"), rcBetName, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));
	rcBetName.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	m_D3DFont.DrawText(pD3DDevice, TEXT("1 - 3"), rcBetName, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));
	rcBetName.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	m_D3DFont.DrawText(pD3DDevice, TEXT("1 - 2"), rcBetName, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));


	rcBetName.SetRect( ptBenchmark.x , ptBenchmark.y + sizeDisplayColumn.cy / 6 * 2, ptBenchmark.x + sizeDisplayColumn.cx / 5, ptBenchmark.y + sizeDisplayColumn.cy / 6 * 3);
	m_D3DFont.DrawText(pD3DDevice, TEXT("2 - 6"), rcBetName, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));
	rcBetName.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	m_D3DFont.DrawText(pD3DDevice, TEXT("2 - 5"), rcBetName, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));
	rcBetName.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	m_D3DFont.DrawText(pD3DDevice, TEXT("2 - 4"), rcBetName, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));
	rcBetName.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	m_D3DFont.DrawText(pD3DDevice, TEXT("2 - 3"), rcBetName, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));
	rcBetName.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	m_D3DFont.DrawText(pD3DDevice, TEXT("3 - 6"), rcBetName, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));

	rcBetName.SetRect( ptBenchmark.x , ptBenchmark.y + sizeDisplayColumn.cy / 6 * 4, ptBenchmark.x + sizeDisplayColumn.cx / 5, ptBenchmark.y + sizeDisplayColumn.cy / 6 * 5 );
	m_D3DFont.DrawText(pD3DDevice, TEXT("3 - 5"), rcBetName, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));
	rcBetName.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	m_D3DFont.DrawText(pD3DDevice, TEXT("3 - 4"), rcBetName, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));
	rcBetName.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	m_D3DFont.DrawText(pD3DDevice, TEXT("4 - 6"), rcBetName, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));
	rcBetName.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	m_D3DFont.DrawText(pD3DDevice, TEXT("4 - 5"), rcBetName, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));
	rcBetName.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	m_D3DFont.DrawText(pD3DDevice, TEXT("5 - 6"), rcBetName, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));


	//������ע
	rcBetCount.SetRect( ptBenchmark.x , ptBenchmark.y + sizeDisplayColumn.cy / 6, ptBenchmark.x + sizeDisplayColumn.cx / 5, ptBenchmark.y + sizeDisplayColumn.cy / 6 * 2);
	_sntprintf(szBet, sizeof(szBet), TEXT("%I64d"), lBet[AREA_1_6]);
	m_D3DFont.DrawText(pD3DDevice, szBet, rcBetCount, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,255));
	rcBetCount.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	_sntprintf(szBet, sizeof(szBet), TEXT("%I64d"), lBet[AREA_1_5]);
	m_D3DFont.DrawText(pD3DDevice, szBet, rcBetCount, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,255));
	rcBetCount.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	_sntprintf(szBet, sizeof(szBet), TEXT("%I64d"), lBet[AREA_1_4]);
	m_D3DFont.DrawText(pD3DDevice, szBet, rcBetCount, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,255));
	rcBetCount.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	_sntprintf(szBet, sizeof(szBet), TEXT("%I64d"), lBet[AREA_1_3]);
	m_D3DFont.DrawText(pD3DDevice, szBet, rcBetCount, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,255));
	rcBetCount.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	_sntprintf(szBet, sizeof(szBet), TEXT("%I64d"), lBet[AREA_1_2]);
	m_D3DFont.DrawText(pD3DDevice, szBet, rcBetCount, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,255));


	rcBetCount.SetRect( ptBenchmark.x , ptBenchmark.y + sizeDisplayColumn.cy / 6 * 3, ptBenchmark.x + sizeDisplayColumn.cx / 5, ptBenchmark.y + sizeDisplayColumn.cy / 6 * 4);
	_sntprintf(szBet, sizeof(szBet), TEXT("%I64d"), lBet[AREA_2_6]);
	m_D3DFont.DrawText(pD3DDevice, szBet, rcBetCount, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,255));
	rcBetCount.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	_sntprintf(szBet, sizeof(szBet), TEXT("%I64d"), lBet[AREA_2_5]);
	m_D3DFont.DrawText(pD3DDevice, szBet, rcBetCount, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,255));
	rcBetCount.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	_sntprintf(szBet, sizeof(szBet), TEXT("%I64d"), lBet[AREA_2_4]);
	m_D3DFont.DrawText(pD3DDevice, szBet, rcBetCount, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,255));
	rcBetCount.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	_sntprintf(szBet, sizeof(szBet), TEXT("%I64d"), lBet[AREA_2_3]);
	m_D3DFont.DrawText(pD3DDevice, szBet, rcBetCount, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,255));;
	rcBetCount.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	_sntprintf(szBet, sizeof(szBet), TEXT("%I64d"), lBet[AREA_3_6]);
	m_D3DFont.DrawText(pD3DDevice, szBet, rcBetCount, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,255));

	rcBetCount.SetRect( ptBenchmark.x , ptBenchmark.y + sizeDisplayColumn.cy / 6 * 5, ptBenchmark.x + sizeDisplayColumn.cx / 5, ptBenchmark.y + sizeDisplayColumn.cy );
	_sntprintf(szBet, sizeof(szBet), TEXT("%I64d"), lBet[AREA_3_5]);
	m_D3DFont.DrawText(pD3DDevice, szBet, rcBetCount, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,255));
	rcBetCount.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	_sntprintf(szBet, sizeof(szBet), TEXT("%I64d"), lBet[AREA_3_4]);
	m_D3DFont.DrawText(pD3DDevice, szBet, rcBetCount, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,255));
	rcBetCount.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	_sntprintf(szBet, sizeof(szBet), TEXT("%I64d"), lBet[AREA_4_6]);
	m_D3DFont.DrawText(pD3DDevice, szBet, rcBetCount, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,255));
	rcBetCount.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	_sntprintf(szBet, sizeof(szBet), TEXT("%I64d"), lBet[AREA_4_5]);
	m_D3DFont.DrawText(pD3DDevice, szBet, rcBetCount, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,255));
	rcBetCount.OffsetRect(sizeDisplayColumn.cx / 5 , 0);
	_sntprintf(szBet, sizeof(szBet), TEXT("%I64d"), lBet[AREA_5_6]);
	m_D3DFont.DrawText(pD3DDevice, szBet, rcBetCount, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,255));
	
}

//�滭������Ϣ
void CGameClientView::DrawGameOver( CD3DDevice * pD3DDevice )
{
	if (!m_bGameOver)
		return;

	m_ImageGameOver.DrawImage(pD3DDevice, m_ptGameOver.x, m_ptGameOver.y);

	CRect rcet;
	TCHAR szInfo[128] = {0};
	rcet.SetRect(m_ptGameOver.x + 170, m_ptGameOver.y + 105, m_ptGameOver.x + 255, m_ptGameOver.y + 120);
	
	for ( int i = 0; i < CountArray(m_bRanking); ++i)
	{
		if ( m_bRanking[i] >= HORSES_ALL)
		{
			ASSERT(FALSE);
			continue;
		}
		
		if ( m_bRanking[i] == HORSES_ONE )
		{
			if(_tcscmp(TEXT("һ����"), m_szHorsesName[m_bRanking[i]] ) == 0)
				_sntprintf(szInfo,sizeof(szInfo),TEXT("һ����") );
			else
				_sntprintf(szInfo,sizeof(szInfo),TEXT("һ�ţ�%s"), m_szHorsesName[m_bRanking[i]]);
		}
		else if ( m_bRanking[i] == HORSES_TWO )
		{
			if(_tcscmp(TEXT("������"), m_szHorsesName[m_bRanking[i]] ) == 0)
				_sntprintf(szInfo,sizeof(szInfo),TEXT("������") );
			else
				_sntprintf(szInfo,sizeof(szInfo),TEXT("���ţ�%s"), m_szHorsesName[m_bRanking[i]]);
		}
		else if ( m_bRanking[i] == HORSES_THREE )
		{
			if(_tcscmp(TEXT("������"), m_szHorsesName[m_bRanking[i]] ) == 0)
				_sntprintf(szInfo,sizeof(szInfo),TEXT("������") );
			else
				_sntprintf(szInfo,sizeof(szInfo),TEXT("���ţ�%s"), m_szHorsesName[m_bRanking[i]]);
		}
		else if ( m_bRanking[i] == HORSES_FOUR )
		{
			if(_tcscmp(TEXT("�ĺ���"), m_szHorsesName[m_bRanking[i]] ) == 0)
				_sntprintf(szInfo,sizeof(szInfo),TEXT("�ĺ���") );
			else
				_sntprintf(szInfo,sizeof(szInfo),TEXT("�ĺţ�%s"), m_szHorsesName[m_bRanking[i]]);
		}
		else if ( m_bRanking[i] == HORSES_FIVE )
		{
			if(_tcscmp(TEXT("�����"), m_szHorsesName[m_bRanking[i]] ) == 0)
				_sntprintf(szInfo,sizeof(szInfo),TEXT("�����") );
			else
				_sntprintf(szInfo,sizeof(szInfo),TEXT("��ţ�%s"), m_szHorsesName[m_bRanking[i]]);
		}
		else if ( m_bRanking[i] == HORSES_SIX )
		{
			if(_tcscmp(TEXT("������"), m_szHorsesName[m_bRanking[i]] ) == 0)
				_sntprintf(szInfo,sizeof(szInfo),TEXT("������") );
			else
				_sntprintf(szInfo,sizeof(szInfo),TEXT("���ţ�%s"), m_szHorsesName[m_bRanking[i]]);
		}

		m_D3DFont.DrawText(pD3DDevice, szInfo, rcet, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_LEFT, D3DCOLOR_XRGB(255,255,255));

		if(i%2 == 0)
			rcet.OffsetRect(110,0);
		else
			rcet.OffsetRect(-110,40);
	}

	if ( m_wMeChairID != INVALID_CHAIR )
	{
		IClientUserItem * pUserData = GetClientUserItem( m_wMeChairID );

		if( pUserData != NULL)
		{
			rcet.SetRect(m_ptGameOver.x + 387, m_ptGameOver.y + 120, m_ptGameOver.x + 500, m_ptGameOver.y + 140);
			m_D3DFont.DrawText(pD3DDevice, pUserData->GetNickName(), rcet, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));


			rcet.SetRect(m_ptGameOver.x + 387, m_ptGameOver.y + 150, m_ptGameOver.x + 500, m_ptGameOver.y + 170);
			_sntprintf(szInfo,sizeof(szInfo),TEXT("%I64d"), m_lPlayerWinning);
			m_D3DFont.DrawText(pD3DDevice, szInfo, rcet, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER, D3DCOLOR_XRGB(255,255,0));

		}
	}
}

//��������
void CGameClientView::DrawTextString(CD3DDevice * pD3DDevice, LPCTSTR pszString, D3DCOLOR crText, D3DCOLOR crFrame, int nXPos, int nYPos)
{
	//��ȥ��׼��
	nXPos -= m_tagBenchmark.ptBase.x;

	if( nXPos < 0 || nXPos > m_tagBenchmark.nWidth )
		return;

	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		m_D3DFont.DrawText(pD3DDevice, pszString, nXPos+nXExcursion[i], nYPos+nYExcursion[i], TA_CENTER|DT_TOP, crFrame);
	}

	//�滭����
	m_D3DFont.DrawText(pD3DDevice, pszString, nXPos, nYPos, TA_CENTER|DT_TOP, crText);
}

//�滭����
void CGameClientView::DrawNumber(CD3DDevice * pD3DDevice ,  int nXPos, int nYPos, int nNumer, bool bTime/* = false*/ )
{
	//������Դ
	INT nNumberHeight=m_ImageTimeNumer.GetHeight();
	INT nNumberWidth=m_ImageTimeNumer.GetWidth()/10;

	//������Ŀ
	LONG lNumberCount=0;
	LONG lNumberTemp = nNumer;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//λ�ö���
	INT nYDrawPos = nYPos;
	INT nXDrawPos = nXPos+lNumberCount*nNumberWidth/2-nNumberWidth;
	if ( bTime && lNumberCount == 1)
		nXDrawPos = nXPos+2*nNumberWidth/2-nNumberWidth;

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		LONG lCellNumber = nNumer%10L;
		m_ImageTimeNumer.DrawImage(pD3DDevice,nXDrawPos,nYDrawPos,nNumberWidth - 1,nNumberHeight,lCellNumber*nNumberWidth + 1,0);

		//���ñ���
		nNumer /= 10;
		nXDrawPos-=nNumberWidth;

		if ( bTime && lNumberCount == 1)
		{
			m_ImageTimeNumer.DrawImage(pD3DDevice,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,0,0);
		}

	};

	return;
}

//��ʱ��
void CGameClientView::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent == IDI_HORSES_START )		//����ʼ
	{
		//��ͼ�ƶ� -------------------------------------
		//����β��ͼ��ֹͣ
		if ( m_tagBenchmark.ptBase.x >= m_szTotalSize.cx - m_tagBenchmark.nWidth)
		{
			//��ͼֹͣ
			m_tagBenchmark.ptBase.x = m_szTotalSize.cx - m_tagBenchmark.nWidth;

			//��ƥ�ƶ�
			for ( int nHorses = 0; nHorses < HORSES_ALL; ++nHorses )
			{
				m_ptHorsesPos[nHorses].x += BASIC_SPEED;
			}

			//����Ƿ��յ�
			INT nOverCount = 0;
			for ( int nHorses = 0; nHorses < HORSES_ALL; ++nHorses )
			{
				if ( m_ptHorsesPos[nHorses].x >= m_szTotalSize.cx - TAIL_LENGTH - m_ImageHorses[nHorses].GetWidth()/5 )
				{
					nOverCount++;
				}
			}

			if ( nOverCount >= 2 )
			{
				HorsesEnd();
				m_GameOverSound.Play();
			}
		}
		else if( m_bRacing )
		{
			//�ƶ�
			int nHorsesFirstPos = 0;			//��ƥ��һλ��
			int nHorsesFinallyPos = INT_MAX;	//��ƥ���λ��
			for ( int nHorses = 0; nHorses < HORSES_ALL; ++nHorses )
			{
				m_ptHorsesPos[nHorses].x += BASIC_SPEED;
				int temp = m_ptHorsesPos[nHorses].x - m_tagBenchmark.nWidth / 2 + m_ImageHorses[nHorses].GetWidth()/5/2;
				if ( temp > nHorsesFirstPos || nHorsesFirstPos == 0)
					nHorsesFirstPos = temp;

				if ( temp < nHorsesFinallyPos || nHorsesFirstPos == 0)
					nHorsesFinallyPos = temp;
			}

			//��ͼ�������ƶ�
			if(m_tagBenchmark.ptBase.x <= nHorsesFinallyPos + (nHorsesFirstPos - nHorsesFinallyPos)/2 )
				m_tagBenchmark.ptBase.x = nHorsesFinallyPos + (nHorsesFirstPos - nHorsesFinallyPos)/2;
		}

		//��ƥ�ƶ�-----------------------------------
		if ( m_bRacing )
		{
			//��ƥ�ƶ�
			for ( int nHorses = 0; nHorses < HORSES_ALL; ++nHorses )
			{
				if ( m_nHorsesSpeedIndex[nHorses] > 0 && m_nHorsesSpeedIndex[nHorses] != STEP_SPEED - 1)
				{
					m_ptHorsesPos[nHorses].x -= (m_nHorsesSpeed[nHorses][m_nHorsesSpeedIndex[nHorses] - 1]);
					m_ptHorsesPos[nHorses].x += (m_nHorsesSpeed[nHorses][m_nHorsesSpeedIndex[nHorses]]);
				}
				else if( m_nHorsesSpeedIndex[nHorses] == 0 )
				{
					m_ptHorsesPos[nHorses].x += (m_nHorsesSpeed[nHorses][m_nHorsesSpeedIndex[nHorses]]);
				}

				m_nHorsesSpeedIndex[nHorses]++;
				if( m_nHorsesSpeedIndex[nHorses] == STEP_SPEED)
					m_nHorsesSpeedIndex[nHorses] = STEP_SPEED - 1;
			}

			//����Ƿ��յ�
			INT nOverCount = 0;
			for ( int nHorses = 0; nHorses < HORSES_ALL; ++nHorses )
			{
				if ( m_ptHorsesPos[nHorses].x >= m_szTotalSize.cx - TAIL_LENGTH - m_ImageHorses[nHorses].GetWidth()/5)
				{
					nOverCount++;
				}
			}

			if ( nOverCount >= 2 )
			{
				HorsesEnd();
				m_GameOverSound.Play();
			}
		}

		//��ƥ��֡-------------------------------------
		if( m_bRacing )
		{
			m_nInFrameDelay++;
			if( m_nInFrameDelay%2 == 1 )
			{
				for ( int nHorses = 0; nHorses < HORSES_ALL; ++nHorses )
				{
					m_cbHorsesIndex[nHorses] = (m_cbHorsesIndex[nHorses] + 1)%5;
				}
			}
		}
	}
	else if ( IDI_HORSES_END == nIDEvent )// �������
	{
		KillTimer(IDI_HORSES_END);
		m_bGameOver = true;
		m_btGameOver.ShowWindow(SW_SHOW);
	}
	else if ( IDI_PLAY_SOUNDS == nIDEvent )
	{
		if ( m_bRacing )
		{
			m_HorsesSound.Play();
		}
		else
		{
			m_HorsesSound.Stop();
			KillTimer(IDI_PLAY_SOUNDS);
		}
	}

	CGameFrameView::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CGameFrameView::OnLButtonDown(nFlags, point);
}

//������ע��ʾ
void CGameClientView::OnPlayerBetShow()
{
	m_bMeOrAll = true;
	m_btPlayerBetShow.EnableWindow(FALSE);
	m_btAllBetShow.EnableWindow(TRUE);
}

//ȫ����ע��ʾ
void CGameClientView::OnAllBetShow()
{
	m_bMeOrAll = false;
	m_btPlayerBetShow.EnableWindow(TRUE);
	m_btAllBetShow.EnableWindow(FALSE);
}

//��ʷ��¼
void CGameClientView::OnRecordShow()
{
	if(m_DlgRecord.IsWindowVisible())
	{
		m_DlgRecord.ShowWindow(SW_HIDE);
	}
	else
	{
		CRect rcButton;
		m_btRecord.GetWindowRect(&rcButton);
		ScreenToClient(&rcButton);
		m_DlgRecord.SetWindowPos(NULL, rcButton.left - 50, rcButton.top + 40, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);;
		m_DlgRecord.ShowWindow(SW_SHOW);
	}
}

//������ע
void CGameClientView::OnPlayerBet()
{
	if(m_DlgPlayBet.IsWindowVisible())
	{
		m_DlgPlayBet.ShowWindow(SW_HIDE);
	}
	else
	{
		CRect rcButton;
		CRect rcBet;
		m_DlgPlayBet.GetWindowRect(&rcBet);
		m_btPlayerBet.GetWindowRect(&rcButton);
		ScreenToClient(&rcButton);
		m_DlgPlayBet.SetWindowPos(NULL, rcButton.left - 100, rcButton.top - rcBet.Height() - 40, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);;
		m_DlgPlayBet.ShowWindow(SW_SHOW);
		m_DlgPlayBet.SetForegroundWindow();
	}
}


//ͳ�ƴ���
void CGameClientView::OnStatistics()
{
	if(m_DlgStatistics.IsWindowVisible())
	{
		m_DlgStatistics.ShowWindow(SW_HIDE);
	}
	else
	{
		CRect rcButton;
		CRect rcStatistics;
		m_DlgStatistics.GetWindowRect(&rcStatistics);
		m_btStatistics.GetWindowRect(&rcButton);
		ScreenToClient(&rcButton);
		m_DlgStatistics.SetWindowPos(NULL, rcButton.left, rcButton.top - rcStatistics.Height() - 40, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);;
		m_DlgStatistics.ShowWindow(SW_SHOW);
		m_DlgStatistics.SetForegroundWindow();
	}
}

//��ע�ɼ�
void CGameClientView::OnBetRecordShow()
{
	if(m_DlgBetRecord.IsWindowVisible())
	{
		m_DlgBetRecord.ShowWindow(SW_HIDE);
	}
	else
	{
		CRect rcButton;
		CRect rcBetRecord;
		m_DlgBetRecord.GetWindowRect(&rcBetRecord);
		m_btBetRecord.GetWindowRect(&rcButton);
		ScreenToClient(&rcButton);
		m_DlgBetRecord.SetWindowPos(NULL, rcButton.left, rcButton.top - rcBetRecord.Height() - 20, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);;
		m_DlgBetRecord.ShowWindow(SW_SHOW);
		m_DlgBetRecord.SetForegroundWindow();
	}
}

//˵��
void CGameClientView::OnExplain()
{
	if ( m_wKindID == INVALID_WORD )
		return;

	//TCHAR szhttp[256];
	//_sntprintf(szhttp, sizeof(szhttp), TEXT("http://%s/GameRule.asp?KindID=%d"),szStationPage,m_wKindID);

	//ShellExecute(NULL, TEXT("open"), szhttp, NULL, NULL, SW_SHOWDEFAULT);
}

//�ɼ��ر�
void CGameClientView::OnGameOverClose()
{
	m_bGameOver = false;
	m_btGameOver.ShowWindow(SW_HIDE);
}

//����Ա����
void CGameClientView::OpenControlWnd()
{
	//��Ȩ��
	//if((GetClientUserItem(m_wMeChairID)->dwUserRight&UR_GAME_CONTROL)!=0)
	{
		if (NULL==m_DlgControl.m_hWnd) 
			m_DlgControl.Create(IDD_DIALOG_SYSTEM,this);

		if(!m_DlgControl.IsWindowVisible()) 
		{
			m_DlgControl.ShowWindow(SW_SHOW);
			m_DlgControl.SetForegroundWindow();
		}
		else 
		{
			m_DlgControl.ShowWindow(SW_HIDE);
		}
	}
}

//����ʼ
void CGameClientView::HorsesStart(INT nHorsesSpeed[HORSES_ALL][COMPLETION_TIME])
{
	//����
	ZeroMemory(m_cbHorsesIndex, sizeof(m_cbHorsesIndex));
	ZeroMemory(m_nHorsesSpeed, sizeof(m_nHorsesSpeed));
	ZeroMemory(m_nHorsesSpeedIndex, sizeof(m_nHorsesSpeedIndex));

	//֡�����
	for ( int nHorses = 0; nHorses < HORSES_ALL; ++nHorses )
		m_cbHorsesIndex[nHorses] = rand()%5;

	//�õ���ƥ�ٶ�(ģ���ٶ�תΪ��ȷ�ٶ�)
	for ( int nHorses = 0 ; nHorses < HORSES_ALL; ++nHorses )
	{
		//�ٲ�����
		int nSpeedIndex = STEP_SPEED - 1;	

		for( int nTime = COMPLETION_TIME - 1; nTime >= 0 ; nTime-- )
		{
			//�ٶ�
			int nVelocity = 1;	
			//����ά��
			int nMaintenance = 0;
			//Ƶ��
			int nFrequency = (rand() + GetTickCount()*2 ) % FREQUENCY + 1;
			//���ٶ�
			int nAcceleration = ACCELERATION;

			if ( nTime % 2 == 0 )
			{
				nAcceleration = ((rand() + GetTickCount())%ACCELERATION + 1);
				nAcceleration = -nAcceleration;
			}
			else
			{
				nAcceleration = ((rand() + GetTickCount())%ACCELERATION + 2);
			}

			//�����ٶ�
			nVelocity = nHorsesSpeed[nHorses][nTime];

			//50Ϊ������.. Ҫ��0 - 50 ���ٵ����ڵ��ٶ�
			while( nSpeedIndex >= 50 )
			{
				m_nHorsesSpeed[nHorses][nSpeedIndex] = nVelocity;

				//�ﵽ���ٽ���Ǹ��ٽ�
				if ( (nTime % 2 == 0 && nTime > 0 && nVelocity <= nHorsesSpeed[nHorses][nTime - 1])		//���ٽ�
					|| (nTime % 2 == 1 &&  nTime > 0 && nVelocity >= nHorsesSpeed[nHorses][nTime - 1]) )//���ٽ�
				{
					break;
				}

				//������һ���ٶ�
				if( nMaintenance < nFrequency )
				{
					nMaintenance++;
				}
				else
				{
					nFrequency = (rand() + GetTickCount()*2 ) % (FREQUENCY/2) + 1;
					nMaintenance = 0;
					nVelocity += nAcceleration;
				}
				nSpeedIndex--;
			}

			//100��ǰ�ļ�����
			for ( int i = 0; i < 50; ++i)
			{
				m_nHorsesSpeed[nHorses][i] = m_nHorsesSpeed[nHorses][50]  * i / 50;
			}
			
		}
	}

	m_bRacing = true;
	m_nInFrameDelay = 0;
	m_HorsesSound.Play();
	SetTimer(IDI_HORSES_START, 30, NULL);		//����ʼ
	SetTimer(IDI_PLAY_SOUNDS, 4000, NULL);		//��������
}

//�������
void CGameClientView::HorsesEnd()
{
	m_bRacing = false;
	KillTimer(IDI_HORSES_START);
	KillTimer(IDI_PLAY_SOUNDS);
	m_HorsesSound.Stop();

	if ( m_bRanking[RANKING_SECOND] < 0 || m_bRanking[RANKING_SECOND] > (HORSES_ALL - 1))
	{
		return;
	}

	//��ͼֹͣ
	m_tagBenchmark.ptBase.x = m_szTotalSize.cx - m_tagBenchmark.nWidth;

	int nHorsesPos = m_szTotalSize.cx - TAIL_LENGTH - m_ImageHorses[m_bRanking[RANKING_SECOND]].GetWidth()/5 - m_nHorsesSpeed[m_bRanking[RANKING_SECOND]][STEP_SPEED - 1];
	
	//�յ㶨λ
	for ( int nHorses = 0; nHorses < HORSES_ALL; ++nHorses )
	{
		m_ptHorsesPos[nHorses].x = nHorsesPos +  m_nHorsesSpeed[nHorses][STEP_SPEED - 1];
	}

	//������������
	SetTimer(IDI_HORSES_END,2000,NULL);
}

//��һ�ֿ�ʼ
void CGameClientView::NweHorses()
{
	m_bRacing = false;
	m_bGameOver = false;
	KillTimer(IDI_HORSES_START);
	KillTimer(IDI_HORSES_END);
	KillTimer(IDI_PLAY_SOUNDS);
	m_btGameOver.ShowWindow(SW_HIDE);
	m_HorsesSound.Stop();

	m_ptHorsesPos[HORSES_ONE].SetPoint(HORSES_X_POS, HORSES_ONE_Y_POS);
	m_ptHorsesPos[HORSES_TWO].SetPoint(HORSES_X_POS, HORSES_TWO_Y_POS);
	m_ptHorsesPos[HORSES_THREE].SetPoint(HORSES_X_POS, HORSES_THREE_Y_POS);
	m_ptHorsesPos[HORSES_FOUR].SetPoint(HORSES_X_POS, HORSES_FOUR_Y_POS);
	m_ptHorsesPos[HORSES_FIVE].SetPoint(HORSES_X_POS, HORSES_FIVE_Y_POS);
	m_ptHorsesPos[HORSES_SIX].SetPoint(HORSES_X_POS, HORSES_SIX_Y_POS);

	m_tagBenchmark.ptBase.x = 0;
	m_tagBenchmark.ptBase.y = 0;

	//����
	ZeroMemory(m_cbHorsesIndex, sizeof(m_cbHorsesIndex));
	ZeroMemory(m_nHorsesSpeed, sizeof(m_nHorsesSpeed));
	ZeroMemory(m_nHorsesSpeedIndex, sizeof(m_nHorsesSpeedIndex));
	
	//����
	m_nBetPlayerCount = 0;
	memset(m_lPlayerBet, 0, sizeof(m_lPlayerBet));
	memset(m_lPlayerBetAll, 0, sizeof(m_lPlayerBetAll));
	m_lPlayerWinning = 0l;
}

//����������
void CGameClientView::SetHorsesName( WORD wMeChairID, TCHAR szName[HORSES_NAME_LENGTH] )
{
	memcpy( m_szHorsesName[wMeChairID], szName, sizeof(m_szHorsesName[wMeChairID]) );
}

//����������ע
void CGameClientView::SetAllBet( BYTE cbArea, LONGLONG lScore )
{
	m_lPlayerBetAll[cbArea] = lScore;
}

//��Ҽ�ע
void CGameClientView::SetPlayerBet( WORD wMeChairID, BYTE cbArea, LONGLONG lScore )
{
	if ( wMeChairID == m_wMeChairID )
	{
		m_lPlayerBet[cbArea] += lScore;
	}
	m_lPlayerBetAll[cbArea] += lScore;
}

//��������
void CGameClientView::SetRanking(BYTE bRanking[RANKING_NULL])
{
	memcpy( m_bRanking, bRanking, sizeof(m_bRanking));
}

//��������
VOID  CGameClientView::CartoonMovie()
{

}

//�����豸
VOID CGameClientView::InitGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)
{
	//��������
	HINSTANCE hResInstance=AfxGetInstanceHandle();

	//������Դ
	m_ImagePenHand.LoadImage(pD3DDevice, hResInstance, TEXT("PEN_HEAD"),TEXT("PNG") );
	m_ImagePenNum.LoadImage(pD3DDevice, hResInstance, TEXT("PEN_NUM"),TEXT("PNG") );
	m_ImagePenTail.LoadImage(pD3DDevice, hResInstance, TEXT("PEN_TAIL"),TEXT("PNG") );

	m_ImageBackdrop.LoadImage( pD3DDevice, hResInstance, TEXT("FULL_BACK"), TEXT("PNG") );				//����
	m_ImageBackdropHand.LoadImage( pD3DDevice, hResInstance, TEXT("BACK_HAND"), TEXT("PNG") );			//����β
	m_ImageBackdropTail.LoadImage( pD3DDevice, hResInstance, TEXT("BACK_TAIL"), TEXT("PNG") );			//����ͷ
	m_ImageDistanceFrame.LoadImage( pD3DDevice, hResInstance, TEXT("DISTANCE_FRAME"), TEXT("PNG") );	//��������

	//��Ϣ��
	m_ImageUserInfoL.LoadImage( pD3DDevice, hResInstance, TEXT("USER_INFO_L"),TEXT("PNG") );
	m_ImageUserInfoM.LoadImage( pD3DDevice, hResInstance, TEXT("USER_INFO_M"),TEXT("PNG") );
	m_ImageUserInfoR.LoadImage( pD3DDevice, hResInstance, TEXT("USER_INFO_R"),TEXT("PNG") );
	m_ImageUserInfoShu.LoadImage( pD3DDevice, hResInstance, TEXT("USER_INFO_SHU"),TEXT("PNG") );

	//��ƥ
	m_ImageHorses[HORSES_ONE].LoadImage( pD3DDevice, hResInstance, TEXT("HORSE1"),TEXT("PNG") );
	m_ImageHorses[HORSES_TWO].LoadImage( pD3DDevice, hResInstance, TEXT("HORSE2"),TEXT("PNG") );
	m_ImageHorses[HORSES_THREE].LoadImage( pD3DDevice, hResInstance, TEXT("HORSE3"),TEXT("PNG") );
	m_ImageHorses[HORSES_FOUR].LoadImage( pD3DDevice, hResInstance, TEXT("HORSE4"),TEXT("PNG") );
	m_ImageHorses[HORSES_FIVE].LoadImage( pD3DDevice, hResInstance, TEXT("HORSE5"),TEXT("PNG") );
	m_ImageHorses[HORSES_SIX].LoadImage( pD3DDevice, hResInstance, TEXT("HORSE6"),TEXT("PNG") );

	m_ImageTimeNumer.LoadImage( pD3DDevice, hResInstance, TEXT("TIME_NUMBER"), TEXT("PNG") );			//����
	m_ImageTimeBet.LoadImage( pD3DDevice, hResInstance, TEXT("TIPS_BET"), TEXT("PNG")  );;				//��עʱ��
	m_ImageTimeBetEnd.LoadImage( pD3DDevice, hResInstance, TEXT("TIPS_BET_END"), TEXT("PNG") );			//��ע����
	m_ImageTimeFree.LoadImage( pD3DDevice, hResInstance, TEXT("TIPS_FREE"), TEXT("PNG") );				//����ʱ��
	m_ImageTimeHorseEnd.LoadImage( pD3DDevice, hResInstance, TEXT("TIPS_HORSE"), TEXT("PNG") );			//�������
	m_ImageTimeHorse.LoadImage( pD3DDevice, hResInstance, TEXT("TIPS_HORSE_END"), TEXT("PNG") );		//����



	m_ImageGameOver.LoadImage(  pD3DDevice, hResInstance, TEXT("GAME_SCORE"), TEXT("PNG")  );			//��Ϸ����

	m_szBackdrop.SetSize( m_ImageBackdrop.GetWidth(), m_ImageBackdrop.GetHeight());
	m_szBackdropHand.SetSize( m_ImageBackdropHand.GetWidth(), m_ImageBackdropHand.GetHeight());
	m_szBackdropTai.SetSize( m_ImageBackdropTail.GetWidth(), m_ImageBackdropTail.GetHeight());
	m_szTotalSize.cx = m_szBackdropHand.cx + m_szBackdropTai.cx + m_szBackdrop.cx * BACK_COUNT;
	m_szTotalSize.cy = m_szBackdrop.cy;

	//������Ⱦ
	StartRenderThread();
}


//���ٴ���
void CGameClientView::OnDestroy()
{
	__super::OnDestroy();

	//m_WndUserFace.SetFrameView(NULL);
	//m_WndUserFace.SetUserData(NULL);
}
