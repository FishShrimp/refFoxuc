#include "StdAfx.h"
#include "GameClient.h"
#include "GameClientView.h"
//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

#define IDC_START						100								//��ʼ��ť
#define IDC_ONE_SCORE					101								//1 �ְ�ť
#define IDC_TWO_SCORE					102								//2 �ְ�ť
#define IDC_THREE_SCORE					103								//3 �ְ�ť
#define IDC_GIVE_UP_SCORE				104								//������ť
#define IDC_OUT_CARD					105								//���ư�ť
#define IDC_PASS_CARD					106								//PASS��ť
#define IDC_AUTO_OUTCARD                107                             //��ʾ��Ŧ
#define IDC_AUTOPLAY_ON					108								//�йܰ�ť
#define IDC_AUTOPLAY_OFF				109								//ȡ����ť
#define IDC_SORT_CARD			        110                             //�йܰ�Ŧ
#define IDC_SCORE						111								//��ְ�ť
#define IDC_MUSIC						117								//���ְ�ť

//��ը��Ŀ
#define BOMB_EFFECT_COUNT				6								//��ը��Ŀ

//��ը��ʶ
#define IDI_BOMB_EFFECT					101								//��ը��ʶ

#define  NAME_WIDTH						120				

int m_nXFace=50,m_nYFace=50,m_nXTimer=65,m_nYTimer=70;
//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD, OnOutCard)
	ON_BN_CLICKED(IDC_PASS_CARD, OnPassCard)
	ON_BN_CLICKED(IDC_ONE_SCORE, OnOneScore)
	ON_BN_CLICKED(IDC_TWO_SCORE, OnTwoScore)
	ON_BN_CLICKED(IDC_THREE_SCORE, OnThreeScore)
	ON_BN_CLICKED(IDC_AUTO_OUTCARD, OnAutoOutCard)
	ON_BN_CLICKED(IDC_GIVE_UP_SCORE, OnGiveUpScore)
	ON_BN_CLICKED(IDC_SCORE, OnBnClickedScore)
	ON_BN_CLICKED(IDC_AUTOPLAY_ON, OnAutoPlayerOn)
	ON_BN_CLICKED(IDC_AUTOPLAY_OFF,OnAutoPlayerOff)
	ON_BN_CLICKED(IDC_SORT_CARD, OnBnClickedSortCard)
	ON_BN_CLICKED(IDC_MUSIC, OnBnClickedMusic)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��Ϸ����
	m_lBaseScore=0;
	m_wBombTime=1;
	m_cbLandScore=0;
	m_wLandUser=INVALID_CHAIR;
	m_bLeftCardCount=0;

	//״̬����
	m_bShowScore=false;
	m_bLandTitle=false;
	ZeroMemory(m_bPass,sizeof(m_bPass));
	ZeroMemory(m_bScore,sizeof(m_bScore));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bUserTrustee,  sizeof( m_bUserTrustee ) );

	//��ը����
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));
}

//��������
CGameClientView::~CGameClientView()
{
}
//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	//m_ImageBomb.LoadFromResource(this,hInstance,IDB_BOMB);
	m_ImageBack.LoadFromResource(this,hInstance,IDB_VIEW_BACK);
	m_ImageCenter.LoadFromResource(this,hInstance,IDB_VIEW_CENTER);
	m_ImageBombEffect.LoadFromResource(this,hInstance,IDB_BOMB_EFFECT);
	m_ImageHistoryScore.LoadFromResource(this,hInstance,IDB_HISTORY_SCORE);
	m_ImageLeftCard.LoadFromResource(this,hInstance,IDB_LEFT_CARDBACK);
	m_ImageLeftCardText.LoadFromResource(this,hInstance,IDB_LEFT_CARDTEXT);
	m_ImageFrame.LoadFromResource(this,hInstance,IDB_FRAME);
	m_ImageNumber.LoadFromResource(this,hInstance,IDB_NUMBER);
	
	m_ImageCardCount.LoadImage(this,hInstance,TEXT("ONE_CARD_WARNING"));
	m_ImageScore.LoadImage(this,hInstance, TEXT( "SCORE" ) );
	m_ImageLand.LoadImage(this,hInstance, TEXT( "BANKER" ) );
	m_ImageUserTrustee.LoadImage(this,hInstance, TEXT("USER_TRUSTEE"));

	ImageUserReady.LoadImage(this,hInstance, TEXT("USER_READY"));
	ImageTimeBack.LoadImage(this,hInstance,TEXT("TIME_BACK"));
	ImageTimeNumber.LoadImage(this,hInstance,TEXT("TIME_NUMBER"));
	//����λ��
	m_LandSize.cx=m_ImageLand.GetWidth();
	m_LandSize.cy=m_ImageLand.GetHeight();

	//��ȡ��С
	m_sizeHistory.SetSize(m_ImageHistoryScore.GetWidth(),m_ImageHistoryScore.GetHeight());

	//�����ؼ�
	CRect CreateRect(0,0,0,0);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�û��˿�
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);	
		m_UserCardControl[i].SetCardSpace( 16, 0, 0 );

		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);

		//�û��˿�
		if (i!=3)
		{
			m_HandCardControl[i].SetCardSpace(18,18,0);
			m_HandCardControl[i].SetDirection(true);
			m_HandCardControl[i].SetDisplayFlag(false);
			m_HandCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
		}
		else
		{
			m_HandCardControl[i].SetDirection(true);
			m_HandCardControl[i].SetDisplayFlag(true);
			m_HandCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
		}
	}

	//�����˿�
	m_BackCardControl.SetCardSpace(85,0,0);
	m_BackCardControl.SetDisplayFlag(false);
	m_BackCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,41);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btScore.Create(NULL,WS_CHILD,CreateRect,this,IDC_SCORE);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);
	m_btAutoOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_AUTO_OUTCARD);
	m_btAutoPlayOn.Create(TEXT(""), WS_CHILD,CreateRect,this,IDC_AUTOPLAY_ON);
	m_btAutoPlayOff.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_AUTOPLAY_OFF);
	m_btSortCard.Create(NULL,WS_CHILD|WS_DISABLED|WS_VISIBLE,CreateRect,this,IDC_SORT_CARD);
	m_btMusic.Create(NULL,WS_CHILD|WS_DISABLED|WS_VISIBLE,CreateRect,this,IDC_MUSIC);


	m_btStart.SetButtonImage(IDB_START,hInstance,false,false);
	m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hInstance,false,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false,false);
	m_btAutoOutCard.SetButtonImage(IDB_AUTO_OUT_CARD,hInstance,false,false);
	m_btAutoPlayOn.SetButtonImage  (IDB_AUTOPLAY_ON,hInstance,false,false);
	m_btAutoPlayOff.SetButtonImage (IDB_AUTOPLAY_OFF,hInstance,false,false);
	m_btSortCard.SetButtonImage(IDB_COUNT_SORT,hInstance,false,false);
	m_btMusic.SetButtonImage(IDB_BT_MUSIC,hInstance,false,false);

	m_btAutoPlayOn.ShowWindow(SW_HIDE);
	m_btAutoPlayOff.ShowWindow(SW_HIDE);

	return 0;
}
//���ý���
VOID CGameClientView::ResetGameView()
{
	//��Ϸ����
	m_lBaseScore=0;
	m_wBombTime=1;
	m_cbLandScore=0;
	m_wLandUser=INVALID_CHAIR;
	m_bLeftCardCount=0;

	//��ը����
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//״̬����
	m_bShowScore=false;
	m_bLandTitle=false;
	ZeroMemory(m_bPass,sizeof(m_bPass));
	ZeroMemory(m_bScore,sizeof(m_bScore));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bUserTrustee, sizeof( m_bUserTrustee ) );
	ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));

	//���ؿؼ�
	if(m_ScoreView.m_hWnd) m_ScoreView.ShowWindow(SW_HIDE);

	//���ذ�ť
	m_btStart.ShowWindow(SW_HIDE);
	m_btScore.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btPassCard.ShowWindow(SW_HIDE);
	m_btAutoOutCard.ShowWindow(SW_HIDE);
	m_btAutoPlayOn.ShowWindow( SW_SHOW );
	m_btAutoPlayOff.ShowWindow( SW_HIDE );

	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hResInstance,false,false);

	//���ÿؼ�
	m_btOutCard.EnableWindow(FALSE);
	m_btPassCard.EnableWindow(FALSE);
	m_btAutoOutCard.EnableWindow(FALSE);
	m_btSortCard.EnableWindow(FALSE);
	m_btSortCard.SetButtonImage(IDB_COUNT_SORT,AfxGetInstanceHandle(),false,false);

	//�˿˿ؼ�
	m_BackCardControl.SetCardData(NULL,0);
	m_HandCardControl[3].SetPositively(false);
	m_HandCardControl[3].SetDisplayFlag(false);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_UserCardControl[i].SetCardData(NULL,0);
		m_HandCardControl[i].SetCardData(NULL,0);
	}

	return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//��������
	m_ptAvatar[0].x=nWidth/2-m_nXFace/2;
	m_ptAvatar[0].y= 5;
	m_ptNickName[0].x=m_ptAvatar[0].x+m_nXFace-NAME_WIDTH;
	m_ptNickName[0].y=m_ptAvatar[0].y+m_nXFace+5;
	m_ptClock[0].x=nWidth/2-15;
	m_ptClock[0].y=m_ptAvatar[0].y+120;
	m_ptReady[0].x=m_ptClock[0].x;
	m_ptReady[0].y=m_ptClock[0].y;
	m_ptPass[0].x=m_ptClock[0].x-25;
	m_ptPass[0].y=m_ptClock[0].y;
	m_ptWarningCard[0].x=m_ptAvatar[0].x+120;
	m_ptWarningCard[0].y=m_ptAvatar[0].y;
	m_HandCardControl[0].SetBenchmarkPos(	m_ptAvatar[0].x+m_nXFace+5			,m_ptAvatar[0].y		,enXLeft,enYTop);
	m_UserCardControl[0].SetBenchmarkPos(	m_ptAvatar[0].x+20			,m_ptNickName[0].y+60		,enXCenter,enYTop);

	//��������
	m_ptAvatar[1].x= 5;
	m_ptAvatar[1].y=nHeight/2-200;
	m_ptNickName[1].x=m_ptAvatar[1].x+m_nXFace;
	m_ptNickName[1].y=m_ptAvatar[1].y;
	m_ptClock[1].x=200;
	m_ptClock[1].y=m_ptNickName[1].y + 90;
	m_ptReady[1].x=m_ptClock[1].x;
	m_ptReady[1].y=m_ptClock[1].y;
	m_ptPass[1].x=m_ptClock[1].x-25;
	m_ptPass[1].y=m_ptClock[1].y;
	m_ptWarningCard[1].x=m_ptAvatar[1].x+90;
	m_ptWarningCard[1].y=m_ptAvatar[1].y+40;
	m_HandCardControl[1].SetBenchmarkPos(	m_ptAvatar[1].x			,m_ptAvatar[1].y+m_nYFace			,enXLeft,enYTop);
	m_UserCardControl[1].SetBenchmarkPos(	m_ptAvatar[1].x+160		,m_ptNickName[1].y+m_nYFace			,enXLeft,enYTop);

	//��������
	m_ptAvatar[2].x= 5;
	m_ptAvatar[2].y=nHeight/2+70;
	m_ptNickName[2].x=m_ptAvatar[2].x+m_nXFace;
	m_ptNickName[2].y=m_ptAvatar[2].y;
	m_ptClock[2].x=200;
	m_ptClock[2].y=m_ptNickName[2].y -  50;
	m_ptReady[2].x=m_ptClock[2].x;
	m_ptReady[2].y=m_ptClock[2].y;
	m_ptPass[2].x=m_ptClock[2].x-25;
	m_ptPass[2].y=m_ptClock[2].y;
	m_ptWarningCard[2].x=m_ptAvatar[2].x+90;
	m_ptWarningCard[2].y=m_ptAvatar[2].y-90;
	m_HandCardControl[2].SetBenchmarkPos(	m_ptAvatar[2].x				,m_ptAvatar[2].y-5			,enXLeft,enYBottom);
	m_UserCardControl[2].SetBenchmarkPos(	m_ptAvatar[2].x+160			,m_ptNickName[2].y-5			,enXLeft,enYBottom);

	//��������
	m_ptAvatar[5].x=nWidth- m_nXFace-5;
	m_ptAvatar[5].y=nHeight/2-200;
	m_ptNickName[5].x=m_ptAvatar[5].x-NAME_WIDTH;
	m_ptNickName[5].y=m_ptAvatar[5].y;
	m_ptClock[5].x=nWidth-210;
	m_ptClock[5].y=m_ptNickName[5].y + 90;
	m_ptReady[5].x=m_ptClock[5].x;
	m_ptReady[5].y=m_ptClock[5].y;
	m_ptPass[5].x=m_ptClock[5].x-25;
	m_ptPass[5].y=m_ptClock[5].y;
	m_ptWarningCard[5].x=m_ptAvatar[5].x-120;
	m_ptWarningCard[5].y=m_ptAvatar[5].y+40;
	m_HandCardControl[5].SetBenchmarkPos(	m_ptAvatar[5].x+m_nXFace		,m_ptNickName[5].y+m_nYFace			,enXRight,enYTop);
	m_UserCardControl[5].SetBenchmarkPos(	m_ptAvatar[5].x-130			,m_ptNickName[5].y+m_nYFace			,enXRight,enYTop);



	//��������
	m_ptAvatar[4].x=nWidth- m_nXFace-5;
	m_ptAvatar[4].y=nHeight/2+70;
	m_ptNickName[4].x=m_ptAvatar[4].x-NAME_WIDTH;
	m_ptNickName[4].y=m_ptAvatar[4].y;
	m_ptClock[4].x=nWidth-210;
	m_ptClock[4].y=m_ptNickName[4].y - 50;
	m_ptReady[4].x=m_ptClock[4].x;
	m_ptReady[4].y=m_ptClock[4].y;
	m_ptPass[4].x=m_ptClock[4].x-25;
	m_ptPass[4].y=m_ptClock[4].y;
	m_ptWarningCard[4].x=m_ptAvatar[4].x-120;
	m_ptWarningCard[4].y=m_ptAvatar[4].y-90;
	m_HandCardControl[4].SetBenchmarkPos(	m_ptAvatar[4].x+m_nXFace			,m_ptNickName[4].y-5			,enXRight,enYBottom);
	m_UserCardControl[4].SetBenchmarkPos(	m_ptAvatar[4].x-130			,m_ptNickName[4].y-5			,enXRight,enYBottom);

	//��������
	m_ptAvatar[3].x=nWidth/2-m_nXFace/2;
	m_ptAvatar[3].y=nHeight- m_nYFace-5;
	m_ptNickName[3].x=m_ptAvatar[3].x+m_nXFace;
	m_ptNickName[3].y=m_ptAvatar[3].y+5;
	m_ptClock[3].x=m_ptAvatar[3].x-m_nXTimer;
	m_ptClock[3].y=m_ptAvatar[3].y+25;
	m_ptReady[3].x=m_ptClock[3].x;
	m_ptReady[3].y=m_ptClock[3].y;
	m_ptPass[3].x=m_ptClock[3].x-25;
	m_ptPass[3].y=m_ptClock[3].y-10;
	m_ptWarningCard[3].x=m_ptAvatar[3].x+100;
	m_ptWarningCard[3].y=m_ptAvatar[3].y-110;
	m_HandCardControl[3].SetBenchmarkPos(m_ptAvatar[3].x+20			,m_ptAvatar[3].y-20		,enXCenter,enYBottom);
	m_UserCardControl[3].SetBenchmarkPos(m_ptAvatar[3].x+20			,m_ptAvatar[3].y-175		,enXCenter,enYBottom);

	//�ƶ��˿�
	m_BackCardControl.SetBenchmarkPos(nWidth/2, 10,enXCenter,enYTop);

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-210 ,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()/2-80,nHeight-240 ,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAutoOutCard,NULL,nWidth/2-rcButton.Width()/2-0,nHeight-240 ,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2-rcButton.Width()/2+80,nHeight-240 ,0,0,uFlags);
	DeferWindowPos(hDwp,m_btSortCard,NULL,nWidth- rcButton.Width()-15,nHeight-rcButton.Height()*2- 40,0,0,uFlags);

	//��ְ�ť
	CRect rcScore;
	m_btScore.GetWindowRect(&rcScore);
	DeferWindowPos(hDwp,m_btScore,NULL,nWidth-(rcScore.Width()+m_sizeHistory.cx)/2-40,0,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	CRect rcBtAutoPlay;
	m_btAutoPlayOn.GetWindowRect( rcBtAutoPlay );
	m_btAutoPlayOn .MoveWindow( nWidth -   5 - rcBtAutoPlay.Width() ,    nHeight-5-rcBtAutoPlay.Height(), rcBtAutoPlay.Width(), rcBtAutoPlay.Height(), FALSE);
	m_btAutoPlayOff.MoveWindow( nWidth -   5 - rcBtAutoPlay.Width(),     nHeight-5-rcBtAutoPlay.Height(), rcBtAutoPlay.Width(), rcBtAutoPlay.Height(), FALSE);
	m_btSortCard.MoveWindow( nWidth -   5 - rcBtAutoPlay.Width(),		nHeight+100/*-10-rcBtAutoPlay.Height()*2*/, rcBtAutoPlay.Width(), rcBtAutoPlay.Height(), FALSE);
	m_btMusic.MoveWindow(nWidth -   5 - rcBtAutoPlay.Width(),			nHeight-10-rcBtAutoPlay.Height()*2, rcBtAutoPlay.Width(), rcBtAutoPlay.Height(), FALSE);


	return;
}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{

	//�滭����
	DrawViewImage(pDC,m_ImageBack,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageCenter,DRAW_MODE_CENTENT);

	//���Ͽ��
	m_ImageFrame.TransDrawImage(pDC,0,0,RGB(255,0,255));

	//ʣ����
	int yPos=50;
	m_ImageLeftCard.TransDrawImage(pDC,nWidth/2-m_ImageLeftCard.GetWidth()/2,nHeight/2-m_ImageLeftCard.GetHeight()/2-yPos,RGB(255,0,255));
	m_ImageLeftCardText.TransDrawImage(pDC,nWidth/2-m_ImageLeftCard.GetWidth()/2,nHeight/2+25-yPos,RGB(255,0,255));
	DrawNumberString(pDC,m_bLeftCardCount,nWidth/2-m_ImageLeftCard.GetWidth()/2+58,nHeight/2+45-yPos);

	//�滭�û�
	CString szBuffer;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wUserTimer=GetUserClock(i);
		IClientUserItem * pUserItem=GetClientUserItem(i);
//#ifdef DEBUG
//		//DrawTextString( pDC, TEXT("asdasd��˹�����İ�˹�ٰ�˹�ٰ�˹�ٰ�˹�ٰ���"), RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), m_ptNickName[i].x,m_ptNickName[i].y ,14,400,DT_END_ELLIPSIS|((i==0||i ==4||i==5)?DT_RIGHT:DT_LEFT));
//		//DrawTextString( pDC, TEXT("ʣ�ࣺ%d ��"), RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), m_ptNickName[i].x,m_ptNickName[i].y+18,14,400,DT_END_ELLIPSIS|((i==0||i ==4||i==5)?DT_RIGHT:DT_LEFT) );
//		CRect rc;
//		rc.SetRect(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+120,m_ptNickName[i].y+16);
//		CDFontEx::DrawText(this, pDC, 14, 400,TEXT("asdasd��˹�����İ�˹�ٰ�˹�ٰ�˹�ٰ�˹�ٰ���"),rc, RGB(255,255,255),DT_END_ELLIPSIS|((i==0||i ==4||i==5)?DT_RIGHT:DT_LEFT));
//		rc.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+16,m_ptNickName[i].x+120,m_ptNickName[i].y+32);
//
//		CDFontEx::DrawText(this, pDC, 14, 400,TEXT("ʣ�ࣺ%d ��"),rc, RGB(255,255,255),DT_END_ELLIPSIS|((i==0||i ==4||i==5)?DT_RIGHT:DT_LEFT));
//
//		DrawUserTimerEx(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer);
//		DrawUserReadyEx(pDC,m_ptReady[i].x,m_ptReady[i].y);
//		DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,0);
//		m_ImageScore.DrawImage(pDC,m_ptPass[i].x,m_ptPass[i].y);
//		m_ImageCardCount.DrawImage(pDC,m_ptWarningCard[i].x,m_ptWarningCard[i].y);
//#endif

		//�滭�û�
		if (pUserItem!=NULL)
		{
			//�û�����
			CRect rc;
			rc.SetRect(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+120,m_ptNickName[i].y+16);
			CDFontEx::DrawText(this, pDC, 14, 400,pUserItem->GetNickName(),rc, RGB(255,255,255),DT_END_ELLIPSIS|((i==0||i ==4||i==5)?DT_RIGHT:DT_LEFT));
			//ʣ���˿�
			rc.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+16,m_ptNickName[i].x+120,m_ptNickName[i].y+32);
			szBuffer.Format(TEXT("ʣ�ࣺ%d ��"),m_bCardCount[i]);
			CDFontEx::DrawText(this, pDC, 14, 400,TEXT("ʣ�ࣺ%d ��"),rc, RGB(255,255,255),DT_END_ELLIPSIS|((i==0||i ==4||i==5)?DT_RIGHT:DT_LEFT));

			//������Ϣ
			if (wUserTimer!=0) DrawUserTimerEx(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer);
			if (pUserItem->GetUserStatus()==US_READY) DrawUserReadyEx(pDC,m_ptReady[i].x,m_ptReady[i].y);
			if ( !CD2DEngine::GetD2DEngine() )
				DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pUserItem);
			//Pass��־
			if (m_bPass[i]==true) m_ImageScore.DrawImage(pDC,m_ptPass[i].x,m_ptPass[i].y);
			//�滭����
			if(m_bCardCount[i]==1)
			{
				m_ImageCardCount.DrawImage(pDC,m_ptWarningCard[i].x,m_ptWarningCard[i].y);
			}
		}
	}

	//�滭��ը
	if (m_bBombEffect==true)
	{

		//�滭Ч��
		INT nImageHeight=m_ImageBombEffect.GetHeight();
		INT nImageWidth=m_ImageBombEffect.GetWidth()/BOMB_EFFECT_COUNT;
		m_ImageBombEffect.TransDrawImage(pDC,(nWidth-nImageWidth)/2,0,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex%BOMB_EFFECT_COUNT),0,RGB(255,0,255));
	}

//#ifdef DEBUG
//	m_wBombTime =99;
//	m_lBaseScore = 123456789;
//	m_bShowScore =true;
//#endif
	CRect rc ;
	//ը������
	if(m_lBaseScore>0)
	{
		//ը������
		szBuffer.Format(TEXT("%I64d"),m_lBaseScore);
		rc.SetRect(42,5,115,20);
		CDFontEx::DrawText(this, pDC, 14, 400,szBuffer,rc, RGB(255,255,255),DT_RIGHT|DT_END_ELLIPSIS);
	}
	if (m_wBombTime>0)
	{
		//ը����־
		//m_ImageBomb.TransDrawImage(pDC, 5,nHeight- m_ImageBomb.GetHeight()-5,RGB(255,0,255));

		//ը������
		szBuffer.Format(TEXT("%d"),m_wBombTime);
		rc.SetRect(42,34,100,50);
		CDFontEx::DrawText(this, pDC, 14, 400,szBuffer,rc, RGB(255,255,255),DT_RIGHT|DT_END_ELLIPSIS);
	}

	//��ʷ����
	if (m_bShowScore==true)
	{
		//���ֱ���

		m_ImageHistoryScore.BitBlt(pDC->m_hDC,nWidth-m_ImageHistoryScore.GetWidth()-40,27);

		//����λ��
		INT nYBenchmark=27;
		INT nXBenchmark=nWidth-m_ImageHistoryScore.GetWidth()-40;
		UINT nFormat=DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER;

		//�滭��Ϣ
		for (WORD i=0;i<GAME_PLAYER;i++)
		{

			IClientUserItem *pUserItem=GetClientUserItem(i);
			if ((pUserItem==NULL)||(m_pHistoryScore[i]==NULL)) continue;

			//λ�ü���
			CRect rcAccounts(nXBenchmark+2,nYBenchmark+24+i*19,nXBenchmark+73,nYBenchmark+42+i*19);
			CRect rcTurnScore(nXBenchmark+74,nYBenchmark+24+i*19,nXBenchmark+127,nYBenchmark+42+i*19);
			CRect rcCollectScore(nXBenchmark+128,nYBenchmark+24+i*19,nXBenchmark+197,nYBenchmark+42+i*19);

			//������Ϣ
			//������Ϣ
			CString szTurnScore=TEXT(""),szCollectScore=TEXT("");
			szTurnScore.Format(TEXT("%I64d"),m_pHistoryScore[i]->lTurnScore);
			szCollectScore.Format(TEXT("%I64d"),m_pHistoryScore[i]->lCollectScore);

			//�滭��Ϣ
			CDFontEx::DrawText(this, pDC, 14, 400,pUserItem->GetNickName(),rcAccounts, RGB(255,255,255),nFormat);
			CDFontEx::DrawText(this, pDC, 14, 400,szTurnScore,rcTurnScore, RGB(255,255,255),nFormat);
			CDFontEx::DrawText(this, pDC, 14, 400,szCollectScore,rcCollectScore, RGB(255,255,255),nFormat);
		}
	}
	return;
}
//���û���
bool CGameClientView::SetHistoryScore(WORD wChairID,tagHistoryScore * pHistoryScore)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//���ñ���
	m_pHistoryScore[wChairID]=pHistoryScore;

	//���½���
	UpdateGameView(NULL);

	return true;
}

//�����Ϣ
void CGameClientView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//����
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl[3].ShootAllCard(false);
	//SendEngineMessage(IDM_OUT_CARD,1,1);

	return;
}

//���˫��
void CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags, point);

	//�����˿�
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl[3].ShootAllCard(false);
#ifdef DEBUG
	//BYTE bCardData[6]={0,0,0,0,0,0};
	//for(BYTE i=0;i<GAME_PLAYER;i++) SetCardCount(i,6);
	//for(BYTE i=0;i<GAME_PLAYER;i++) 
	//{
	//	 m_HandCardControl[i].SetCardData(bCardData,6);
	//}
	//m_HandCardControl[3].SetCardData(m_bHandCardData,m_bHandCardCount);
#endif
	return;
}
//���ְ�ť
VOID CGameClientView::OnBnClickedScore()
{
	//���ñ���
	m_bShowScore=!m_bShowScore;

	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btScore.SetButtonImage((m_bShowScore==true)?IDB_BT_CLOSE_SCORE:IDB_BT_SHOW_SCORE,hResInstance,false,false);

	//���½���
	UpdateGameView(NULL);

	return;
}


void CGameClientView::UpdateGameView(CRect *rc)
{
	if(rc==NULL)
	{
		InvalidGameView(0,0,0,0);
	}
	else
	{
		InvalidGameView(rc->left,rc->top,rc->Width(),rc->Height());
	}
}


//ը������
void CGameClientView::SetBombTime(WORD wBombTime)
{
	//���ñ���
	m_wBombTime=wBombTime;

	//ˢ�½���
	UpdateGameView(NULL);

	return;
}

//��������
void CGameClientView::SetBaseScore(LONGLONG lBaseScore)
{
	//���ñ���
	m_lBaseScore=lBaseScore;

	//ˢ�½���
	UpdateGameView(NULL);

	return;
}

//��ʾ��ʾ
void CGameClientView::ShowLandTitle(bool bLandTitle)
{
	//���ñ���
	m_bLandTitle=bLandTitle;

	//ˢ�½���
	UpdateGameView(NULL);

	return;
}

//���÷���
void CGameClientView::SetPassFlag(WORD wChairID, bool bPass)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_bPass[i]=bPass;
	}
	else m_bPass[wChairID]=bPass;

	//���½���
	UpdateGameView(NULL);

	return;
}
//����ʣ����
void CGameClientView::SetLeftCardCount(BYTE bCount)
{	
	m_bLeftCardCount=bCount;
	//���½���
	UpdateGameView(NULL);
	return ;
}
//�˿���Ŀ
void CGameClientView::SetCardCount(WORD wChairID, BYTE bCardCount)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_bCardCount[i]=bCardCount;
	}
	else m_bCardCount[wChairID]=bCardCount;

	//���½���
	UpdateGameView(NULL);

	return;
}

//�ڿӷ���
void CGameClientView::SetLandUser(WORD wChairID, BYTE bLandScore)
{
	//���ñ���
	m_wLandUser=wChairID;
	m_cbLandScore=bLandScore;

	//���½���
	UpdateGameView(NULL);

	return;
}

//�ڿӷ���
void CGameClientView::SetLandScore(WORD wChairID, BYTE bLandScore)
{
	//���ñ���
	if (wChairID!=INVALID_CHAIR) m_bScore[wChairID]=bLandScore;
	else memset(m_bScore,0,sizeof(m_bScore));

	//���½���
	UpdateGameView(NULL);

	return;
}

//��ʼ��ť
void CGameClientView::OnStart()
{

	SendEngineMessage(IDM_START,0,0);
	return;
}

//���ư�ť
void CGameClientView::OnOutCard()
{
	SendEngineMessage(IDM_OUT_CARD,1,1);
	return;
}

//������ť
void CGameClientView::OnPassCard()
{
	SendEngineMessage(IDM_PASS_CARD,1,1);
	return;
}

//�зְ�ť
void CGameClientView::OnOneScore()
{
	SendEngineMessage(IDM_LAND_SCORE,1,1);
	return;
}

//�зְ�ť
void CGameClientView::OnTwoScore()
{
	SendEngineMessage(IDM_LAND_SCORE,2,2);
	return;
}

//������ť
void CGameClientView::OnGiveUpScore()
{
	SendEngineMessage(IDM_LAND_SCORE,255,255);
	return;
}

//������ʾ
void CGameClientView::OnAutoOutCard()
{
	SendEngineMessage(IDM_AUTO_OUTCARD,0,0);
	return;
}

//�зְ�ť
void CGameClientView::OnThreeScore()
{
	SendEngineMessage(IDM_LAND_SCORE,3,3);
	return;
}
//////////////////////////////////////////////////////////////////////////

// �Զ��й�
void CGameClientView::OnAutoPlayerOn()
{
	m_btAutoPlayOn.ShowWindow(SW_HIDE);
	m_btAutoPlayOff.ShowWindow(SW_SHOW);
	SendEngineMessage(IDM_LAND_AUTOPLAY,1,1);
	return;
}

// ȡ���й�
void CGameClientView::OnAutoPlayerOff()
{
	m_btAutoPlayOn.ShowWindow(SW_SHOW);
	m_btAutoPlayOff.ShowWindow(SW_HIDE);
	SendEngineMessage(IDM_LAND_AUTOPLAY,0,0);
	return;
}



//���ñ�ը
bool CGameClientView::SetBombEffect(bool bBombEffect)
{
	if (bBombEffect==true)
	{
		//���ñ���
		m_bBombEffect=true;
		m_cbBombFrameIndex=0;

		//����ʱ��
		SetTimer(IDI_BOMB_EFFECT,100,NULL);
	}
	else
	{
		//ֹͣ����
		if (m_bBombEffect==true)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BOMB_EFFECT);

			//���ñ���
			m_bBombEffect=false;
			m_cbBombFrameIndex=0;

			//���½���
			UpdateGameView(NULL);
		}
	}

	return true;
}

//ʱ����Ϣ
VOID CGameClientView::OnTimer(UINT nIDEvent)
{
	//��ը����
	if (nIDEvent==IDI_BOMB_EFFECT)
	{
		//ֹͣ�ж�
		if (m_bBombEffect==false)
		{
			KillTimer(IDI_BOMB_EFFECT);
			return;
		}

		//���ñ���
		if ((m_cbBombFrameIndex+1)>=BOMB_EFFECT_COUNT)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BOMB_EFFECT);

			//���ñ���
			m_bBombEffect=false;
			m_cbBombFrameIndex=0;
		}
		else m_cbBombFrameIndex++;

		//���½���
		UpdateGameView(NULL);

		return;
	}

	__super::OnTimer(nIDEvent);
}


//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos,int nWidth, int nWeight,UINT nFormat)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		CDFontEx::DrawText(this, pDC, nWidth, nWeight,pszString,nXPos+nXExcursion[i],nYPos+nYExcursion[i], crFrame, nFormat);
	}

	//�滭����

	CDFontEx::DrawText(this, pDC, nWidth, nWeight,pszString,nXPos,nYPos, crText, nFormat);

	return;
}

//����й�
void CGameClientView::SetUserTrustee( WORD wChairID, bool bTrustee ) {

	//���ñ���
	if ( INVALID_CHAIR == wChairID ) {
		for ( WORD wChairIdx = 0; wChairIdx < GAME_PLAYER; wChairIdx++ ) {
			m_bUserTrustee[ wChairIdx ] = bTrustee;
		}
	}
	else {
		m_bUserTrustee[ wChairID ] = bTrustee;
	}

	//���½���
	UpdateGameView( NULL );
}

//����ť
VOID CGameClientView::OnBnClickedSortCard()
{
	//������Ϣ
	SendEngineMessage(IDM_SORT_HAND_CARD,0,0);

	return;
}
//�滭����
VOID CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
{

	INT nNumberHeight=m_ImageNumber.GetHeight();
	INT nNumberWidth=m_ImageNumber.GetWidth()/10;

	//������Ŀ
	LONGLONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//λ�ö���
	INT nYDrawPos=nYPos-nNumberHeight/2;
	INT nXDrawPos=nXPos+(INT)lNumberCount*nNumberWidth/2-nNumberWidth;

	//�滭����
	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		LONGLONG lCellNumber=lNumber%10;
		m_ImageNumber.TransDrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,(INT)lCellNumber*nNumberWidth,0,RGB(255,0,255));

		//���ñ���
		lNumber/=10;
		nXDrawPos-=nNumberWidth;
	};

	return;
}

//�滭ʱ��
void CGameClientView::DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
	//��ȡ����
	const INT nNumberHeight=ImageTimeNumber.GetHeight();
	const INT nNumberWidth=ImageTimeNumber.GetWidth()/10;

	//������Ŀ
	LONG lNumberCount=0;
	WORD wNumberTemp=wTime;
	do
	{
		lNumberCount++;
		wNumberTemp/=10;
	} while (wNumberTemp>0L);

	//λ�ö���
	INT nYDrawPos=nYPos-nNumberHeight/2+1;
	INT nXDrawPos=nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth;

	//�滭����
	CSize SizeTimeBack(ImageTimeBack.GetWidth(),ImageTimeBack.GetHeight());
	ImageTimeBack.DrawImage(pDC,nXPos-SizeTimeBack.cx/2,nYPos-SizeTimeBack.cy/2);

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		WORD wCellNumber=wTime%10;
		ImageTimeNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

		//���ñ���
		wTime/=10;
		nXDrawPos-=nNumberWidth;
	};

	return;
}

//�滭׼��
void CGameClientView::DrawUserReadyEx(CDC * pDC, int nXPos, int nYPos)
{
	//�滭׼��
	CSize SizeImage(ImageUserReady.GetWidth(),ImageUserReady.GetHeight());
	ImageUserReady.DrawImage(pDC,nXPos-SizeImage.cx/2,nYPos-SizeImage.cy/2);

	return;
}
//��������
VOID CGameClientView::OnBnClickedMusic()
{
	SendEngineMessage(IDM_MUSIC,0,0);

	return ;
}

//////////////////////////////////////////////////////////////////////////////////
