#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClient.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

#define IDC_START						100								//��ʼ��ť
#define IDC_SCOREONE					101								//1�ְ�ť
#define IDC_SCORETWO					102								//2�ְ�ť
#define IDC_SCORENONE					103								//����ť
#define IDC_TRUSTEE_CONTROL				104								//�йܿ���
#define IDC_BATCH						105								//ϴ�谴ť	
#define IDC_SCORE						107								//��ְ�ť

//������ʶ
#define IDI_BOMB_EFFECT					101								//������ʶ

#define IDI_BATCH_START					102								//ϴ�趯��

//������Ŀ
#define BOMB_EFFECT_COUNT				12								//������Ŀ
#define DISC_EFFECT_COUNT				8								//����Ч��		
#define BATCH_EFFECT_COUNT				6								//������Ŀ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_BATCH, OnBatch)
	ON_BN_CLICKED(IDC_TRUSTEE_CONTROL,OnStusteeControl)
	ON_BN_CLICKED(IDC_SCORE, OnBnClickedScore)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��־����
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;
	m_bShowScore=false;

	//��Ϸ����
	m_lCellScore=0L;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//��������
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//ϴ�趯��
	m_bBatchEffect=false;
	m_cbBatchFrameIndex=0;

	//����Ч��
	m_wDiscUser=INVALID_CHAIR;
	m_cbDiscFrameIndex=0;

	//��ʷ����
	memset(m_lAllTurnScore,0,sizeof(m_lAllTurnScore));
	memset(m_lLastTurnScore,0,sizeof(m_lLastTurnScore));
	ZeroMemory(m_lUpDownScore,sizeof(m_lUpDownScore));
	m_cbUpDownKingCardData=0;


	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_wUserAction,sizeof(m_wUserAction));
	ZeroMemory(m_cbBatch,sizeof(m_cbBatch));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	FillMemory(m_cbKingCardIndex,sizeof(m_cbKingCardIndex),0xFF);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageWait.LoadFromResource(hInstance,IDB_WAIT_TIP);
	m_ImageBack.LoadFromResource(hInstance,IDB_VIEW_BACK);
	m_ImageUserFlag.LoadFromResource(hInstance,IDB_USER_FLAG);
	m_ImageOutCard.LoadFromResource(hInstance,IDB_OUT_CARD_TIP);
	m_ImageUserAction.LoadFromResource(hInstance,IDB_USER_ACTION);
	m_ImageActionBack.LoadFromResource(hInstance,IDB_ACTION_BACK);
	m_ImageHuangZhuang.LoadFromResource(hInstance,IDB_HUANG_ZHUANG);
	m_ImageTrustee.LoadImage(hInstance,TEXT("TRUSTEE"));
	m_ImageActionAni.LoadImage(AfxGetInstanceHandle(),TEXT("ActionAni"));
	m_ImageDisc.LoadImage(AfxGetInstanceHandle(),TEXT("DISC"));
	m_ImageArrow.LoadImage(AfxGetInstanceHandle(),TEXT("ARROW"));
	m_ImageHistoryScore.LoadFromResource(hInstance,IDB_HISTORY_SCORE);
	m_ImageCenter.LoadFromResource(hInstance,IDB_VIEW_CENTER);
	m_ImageLeft.LoadFromResource(hInstance,IDB_VIEW_LEFT);

	// ϴ��png
	m_PngBatch[0].LoadImage(hInstance,TEXT("BATCH_TOP"));
	m_PngBatch[1].LoadImage(hInstance,TEXT("BATCH_RIGHT"));
	m_PngBatch[2].LoadImage(hInstance,TEXT("BATCH_SELF"));
	m_PngBatch[3].LoadImage(hInstance,TEXT("BATCH_LEFT"));

	//CImageHandle HistoryScoreHandle(&m_ImageHistoryScore);
	m_sizeHistory.SetSize(m_ImageHistoryScore.GetWidth(),m_ImageHistoryScore.GetWidth());


	m_nXFace=32;
	m_nYFace=32;

	m_nXBorder=0;
	m_nYBorder=0;

	m_nYTimer=48;
	m_nXTimer=48;


	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��������
	enDirection Direction[]={Direction_North,Direction_East,Direction_South,Direction_West};

	//���ÿؼ�
	for (WORD i=0;i<4;i++)
	{
		//�û��˿�
		m_HeapCard[i].SetDirection(Direction[i]);
		m_TableCard[i].SetDirection(Direction[i]);
		m_DiscardCard[i].SetDirection(Direction[i]);

		//����˿�
		m_WeaveCard[i][0].SetDisplayItem(true);
		m_WeaveCard[i][1].SetDisplayItem(true);
		m_WeaveCard[i][2].SetDisplayItem(true);
		m_WeaveCard[i][3].SetDisplayItem(true);
		m_WeaveCard[i][0].SetDirection(Direction[i]);
		m_WeaveCard[i][1].SetDirection(Direction[i]);
		m_WeaveCard[i][2].SetDirection(Direction[i]);
		m_WeaveCard[i][3].SetDirection(Direction[i]);
	}




	//���ÿؼ�
	m_UserCard[0].SetDirection(Direction_North);
	m_UserCard[1].SetDirection(Direction_East);
	m_UserCard[2].SetDirection(Direction_West);

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_GameScoreWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,100);
	m_ControlWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,10,NULL);
	m_DrawSiceWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,10,NULL);


	//�����ؼ�
	//��ʼ��ť
	m_btStart.Create(NULL,WS_CHILD,rcCreate,this,IDC_START);
	m_btStart.SetButtonImage(IDB_BT_START,AfxGetInstanceHandle(),false,false);

	//ϴ�谴ť
	m_btBatch.Create(NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,IDC_BATCH);
	m_btBatch.SetButtonImage(IDB_BT_BATCH,AfxGetInstanceHandle(),false,false);

	//�йܰ�ť
	m_btStusteeControl.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_TRUSTEE_CONTROL);
	m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);

	//���ư�ť
	m_btScore.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SCORE);
	m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,AfxGetInstanceHandle(),false,false);
	////������Ƶ
	//for (WORD i=0;i<4;i++)
	//{
	//	//������Ƶ
	//	m_DlgVedioService[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,300+i);
	//	m_DlgVedioService[i].InitVideoService(i==2,true);

	//	//������Ƶ
	//	g_VedioServiceManager.SetVideoServiceControl(i,&m_DlgVedioService[i]);
	//}

	SetTimer(IDI_DISC_EFFECT,250,NULL);
	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��־����
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;
	m_bShowScore=false;

	//��Ϸ����
	m_lCellScore=0L;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbBatch,sizeof(m_cbBatch));

   	//��������
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//ϴ�趯��
	m_bBatchEffect=false;
	m_cbBatchFrameIndex=0;

	//����Ч��
	m_wDiscUser=INVALID_CHAIR;
	m_cbDiscFrameIndex=0;

	//��ʷ����
	memset(m_lAllTurnScore,0,sizeof(m_lAllTurnScore));
	memset(m_lLastTurnScore,0,sizeof(m_lLastTurnScore));
	ZeroMemory(m_lUpDownScore,sizeof(m_lUpDownScore));
	m_cbUpDownKingCardData=0;


	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_wUserAction,sizeof(m_wUserAction));
	FillMemory(m_cbKingCardIndex,sizeof(m_cbKingCardIndex),0xFF);

	//��������
	m_btStart.ShowWindow(SW_HIDE);
	m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameScoreWnd.ShowWindow(SW_HIDE);
	m_DrawSiceWnd.ShowWindow(SW_HIDE);
	m_btBatch.ShowWindow(SW_HIDE);
	m_btScore.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_btStusteeControl.EnableWindow(FALSE);

	//�˿�����
	m_UserCard[0].SetCardData(0,false);
	m_UserCard[1].SetCardData(0,false);
	m_UserCard[2].SetCardData(0,false);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayItem(false);
	m_HandCardControl.SetCardData(NULL,0,0);

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_HeapCard[i].SetCardData(0,0,0);
		m_TableCard[i].SetCardData(NULL,0);
		m_DiscardCard[i].SetCardData(NULL,0);
		m_WeaveCard[i][0].SetCardData(NULL,0);
		m_WeaveCard[i][1].SetCardData(NULL,0);
		m_WeaveCard[i][2].SetCardData(NULL,0);
		m_WeaveCard[i][3].SetCardData(NULL,0);
	}



	//���ٶ�ʱ��
	KillTimer(IDI_DISC_EFFECT);
	KillTimer(IDI_BOMB_EFFECT);

	return;
}

//�����ؼ�
void CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//��������
	m_ptAvatar[0].x=nWidth/2-m_nXFace;
	m_ptAvatar[0].y=5+m_nYBorder;
	m_ptNickName[0].x=nWidth/2+15;
	m_ptNickName[0].y=10+m_nYBorder;
	m_ptClock[0].x=nWidth/2-m_nXFace-m_nXTimer-2;
	m_ptClock[0].y=17+m_nYBorder;
	m_ptReady[0].x=nWidth/2-m_nXFace-m_nXTimer*2-5;
	m_ptReady[0].y=5+m_nYBorder+m_nYTimer/2;
	m_UserFlagPos[0].x=nWidth/2-m_nXFace-m_nXTimer-32;
	m_UserFlagPos[0].y=5+m_nYBorder;
	//m_UserListenPos[0].x=nWidth/2;
	//m_UserListenPos[0].y=m_nYBorder+100;
	m_PointTrustee[0].x=nWidth/2-m_nXFace-20-m_nXFace/2;
	m_PointTrustee[0].y=5+m_nYBorder;


	m_ptAvatar[1].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptAvatar[1].y=nHeight/2-m_nYFace-50;
	m_ptNickName[1].x=nWidth-m_nXBorder-5;
	m_ptNickName[1].y=nHeight/2-30;
	m_ptClock[1].x=nWidth-m_nXBorder-m_nXFace+5;
	m_ptClock[1].y=nHeight/2-m_nYFace-27-m_nYTimer;
	m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer+3;
	m_ptReady[1].y=nHeight/2-m_nYFace-55-m_nYTimer/2+m_nYFace*2+55;
	m_UserFlagPos[1].x=nWidth-m_nXBorder-m_nXTimer+8;
	m_UserFlagPos[1].y=nHeight/2-m_nYFace-83-m_nYTimer;
	//m_UserListenPos[1].x=nWidth-m_nXBorder-93;
	//m_UserListenPos[1].y=nHeight/2-75;
	m_PointTrustee[1].x=nWidth-m_nXBorder-m_nXTimer+8;
	m_PointTrustee[1].y=nHeight/2-m_nYFace-50-35;

	m_ptAvatar[2].x=nWidth/2-m_nXFace;
	m_ptAvatar[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptNickName[2].x=nWidth/2+15+m_nXFace/2;
	m_ptNickName[2].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptClock[2].x=nWidth/2-m_nXFace/2-m_nXTimer-2;
	m_ptClock[2].y=nHeight-m_nYBorder-m_nYTimer-8+40;
	m_ptReady[2].x=nWidth/2-m_nXTimer*2-m_nXFace/2-5;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYTimer/2-10;
	m_UserFlagPos[2].x=nWidth/2-m_nXFace-m_nXTimer-32;
	m_UserFlagPos[2].y=nHeight-m_nYBorder-35;
	//m_UserListenPos[2].x=nWidth/2;
	//m_UserListenPos[2].y=nHeight-m_nYBorder-123;
	m_PointTrustee[2].x=nWidth/2-m_nXFace-20-m_nXFace/2;
	m_PointTrustee[2].y=nHeight-m_nYBorder-m_nYFace-5;

	m_ptAvatar[3].x=m_nXBorder+5;
	m_ptAvatar[3].y=nHeight/2-m_nYFace-50;
	m_ptNickName[3].x=m_nXBorder+5;
	m_ptNickName[3].y=nHeight/2-30;
	m_ptClock[3].x=m_nXBorder+22;
	m_ptClock[3].y=nHeight/2-m_nYFace-27-m_nYTimer;
	m_ptReady[3].x=m_nXBorder-2+m_nXTimer;
	m_ptReady[3].y=nHeight/2-m_nYFace-55-m_nYTimer/2+m_nYFace*2+55;
	m_UserFlagPos[3].x=m_nXBorder+8;
	m_UserFlagPos[3].y=nHeight/2-m_nYFace-83-m_nYTimer;
	//m_UserListenPos[3].x=m_nXBorder+91;
	//m_UserListenPos[3].y=nHeight/2-80;
	m_PointTrustee[3].x=m_nXBorder+5;
	m_PointTrustee[3].y=nHeight/2-m_nYFace-50-35;
	//�û��˿�
	m_UserCard[0].SetControlPoint(nWidth/2-200,m_nYBorder+m_nYFace+10);
	m_UserCard[1].SetControlPoint(nWidth-m_nXBorder-m_nXFace-49,nHeight/2-255);
	m_UserCard[2].SetControlPoint(m_nXBorder+m_nXFace+30,nHeight/2+177);
	m_HandCardControl.SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-5,enXCenter,enYBottom);

	//�����˿�
	m_TableCard[0].SetControlPoint(nWidth/2-179,m_nYBorder+m_nYFace+10);
	m_TableCard[1].SetControlPoint(nWidth-m_nXBorder-m_nXFace-10,nHeight/2-197);
	m_TableCard[2].SetControlPoint(nWidth/2+185,nHeight-m_nYFace-m_nYBorder-15);
	m_TableCard[3].SetControlPoint(m_nXBorder+m_nXFace,nHeight/2+97);

	//�ѻ��˿�
	int nXCenter=nWidth/2;
	int nYCenter=nHeight/2-40;
	m_HeapCard[0].SetControlPoint(nXCenter-276,nYCenter-200);
	m_HeapCard[1].SetControlPoint(nXCenter+238,nYCenter-200);
	m_HeapCard[2].SetControlPoint(nXCenter-235,nYCenter+213);
	m_HeapCard[3].SetControlPoint(nXCenter-276,nYCenter-145);

	//�����˿�
	//m_DiscardCard[0].SetControlPoint(nXCenter-103,nYCenter-100);
	m_DiscardCard[0].SetControlPoint(nXCenter-175,nYCenter-100);
	m_DiscardCard[1].SetControlPoint(nXCenter+103+50,nYCenter-63);
	//m_DiscardCard[2].SetControlPoint(nXCenter+100,nYCenter+112);
	m_DiscardCard[2].SetControlPoint(nXCenter+150,nYCenter+127);
	m_DiscardCard[3].SetControlPoint(nXCenter-137-55,nYCenter+90);

	//����˿�
	m_WeaveCard[0][0].SetControlPoint(nWidth/2+175,m_nYBorder+m_nYFace+55+5+40);
	m_WeaveCard[0][1].SetControlPoint(nWidth/2+75,m_nYBorder+m_nYFace+55+5+40);
	m_WeaveCard[0][2].SetControlPoint(nWidth/2-25,m_nYBorder+m_nYFace+55+5+40);
	m_WeaveCard[0][3].SetControlPoint(nWidth/2-125,m_nYBorder+m_nYFace+55+5+40);

	//����˿�
	m_WeaveCard[1][0].SetControlPoint(nWidth-m_nXBorder-m_nXFace-73-15,nHeight/2+130);
	m_WeaveCard[1][1].SetControlPoint(nWidth-m_nXBorder-m_nXFace-73-15,nHeight/2+35);
	m_WeaveCard[1][2].SetControlPoint(nWidth-m_nXBorder-m_nXFace-73-15,nHeight/2-60);
	m_WeaveCard[1][3].SetControlPoint(nWidth-m_nXBorder-m_nXFace-73-15,nHeight/2-155);

	//����˿�
	m_WeaveCard[2][0].SetControlPoint(nWidth/2-200,nHeight-m_nYFace-m_nYBorder-10-100-3);
	m_WeaveCard[2][1].SetControlPoint(nWidth/2-85,nHeight-m_nYFace-m_nYBorder-10-100-3);
	m_WeaveCard[2][2].SetControlPoint(nWidth/2+30,nHeight-m_nYFace-m_nYBorder-10-100-3);
	m_WeaveCard[2][3].SetControlPoint(nWidth/2+145,nHeight-m_nYFace-m_nYBorder-10-100-3);

	//����˿�
	m_WeaveCard[3][0].SetControlPoint(m_nXBorder+m_nXFace+70+15,nHeight/2-200);
	m_WeaveCard[3][1].SetControlPoint(m_nXBorder+m_nXFace+70+15,nHeight/2-115);
	m_WeaveCard[3][2].SetControlPoint(m_nXBorder+m_nXFace+70+15,nHeight/2-20);
	m_WeaveCard[3][3].SetControlPoint(m_nXBorder+m_nXFace+70+15,nHeight/2+75);

	//���ƴ���
	m_ControlWnd.SetBenchmarkPos(nWidth/2+298,nHeight-m_nYBorder-120);

	//�����ɼ�
	CRect rcScore;
	m_GameScoreWnd.GetWindowRect(&rcScore);
	m_GameScoreWnd.MoveWindow((nWidth-rcScore.Width())/2,(nHeight-rcScore.Height())/2-30,rcScore.Width(),rcScore.Height());

	CRect rcSice;
	m_DrawSiceWnd.GetWindowRect(&rcSice);
	m_DrawSiceWnd.SetBenchmarkPos((nWidth-rcSice.Width())/2,(nHeight-rcSice.Height())/2-40);
	m_DrawSiceWnd.MoveWindow((nWidth-rcSice.Width())/2,(nHeight-rcSice.Height())/2-40,rcSice.Width(),rcSice.Height());


	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//�ƶ�����
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-155-m_nYBorder+30,0,0,uFlags);

	//�ƶ�����
	DeferWindowPos(hDwp,m_btBatch,NULL,(nWidth-rcButton.Width())/2,nHeight-155-m_nYBorder+30,0,0,uFlags);

	
	//�йܰ�ť
	DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-m_nYBorder-5-rcButton.Height(),0,0,uFlags);
	
	//��ְ�ť
	m_btScore.GetWindowRect(&rcScore);
	DeferWindowPos(hDwp,m_btScore,NULL,nWidth-(rcScore.Width()+m_sizeHistory.cx)/2-30,0,0,0,uFlags);

	////��Ƶ����
	//CRect rcAVDlg;
	//m_DlgVedioService[0].GetWindowRect(&rcAVDlg);
	//DeferWindowPos(hDwp,m_DlgVedioService[1],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),nHeight/2+30,0,0,uFlags);
	//DeferWindowPos(hDwp,m_DlgVedioService[3],NULL,m_nXBorder+5,nHeight/2+30,0,0,uFlags);
	//DeferWindowPos(hDwp,m_DlgVedioService[0],NULL,nWidth-m_nXBorder-5-rcAVDlg.Width(),5,0,0,uFlags);
	//m_DlgVedioService[2].GetWindowRect(&rcAVDlg);
	//DeferWindowPos(hDwp,m_DlgVedioService[2],NULL,m_nXBorder+5,nHeight-m_nYBorder-3-rcAVDlg.Height(),0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);



	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageBack,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageCenter,DRAW_MODE_CENTENT);
	//����λͼ
	//CImageHandle ImageHandle(&m_ImageLeft);
	m_ImageLeft.BitBlt(pDC->GetSafeHdc(),m_nXBorder,m_nYBorder);

	//���·�
	if(m_cbUpDownKingCardData != 0)
	{
		g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbUpDownKingCardData,m_nXBorder+50,m_nYBorder+10);
	}

	//�滭Ч��
	INT nItemHeight=m_PngBatch[1].GetHeight();
	INT nItemWidth=m_PngBatch[1].GetWidth()/BATCH_EFFECT_COUNT;

	//�滭�û�
	pDC->SetTextColor(RGB(255,255,0));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		IClientUserItem * pUserData=GetClientUserItem(i);
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextAlign((i==1)?(TA_RIGHT|TA_TOP):(TA_LEFT|TA_TOP));
			DrawTextString(pDC,pUserData->GetNickName(),RGB(255,255,255),RGB(0,0,0),m_ptNickName[i].x,m_ptNickName[i].y);

			//������Ϣ
			WORD wUserTimer=GetUserClock(i);
			if ((wUserTimer!=0)&&(m_wCurrentUser!=INVALID_CHAIR))
			{
				DrawUserClock(pDC,nWidth/2,nHeight/2-40,wUserTimer);
				if(m_wCurrentUser==0)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2-m_ImageArrow.GetHeight()*2,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				if(m_wCurrentUser==1)
					m_ImageArrow.DrawImage(pDC,nWidth/2+m_ImageArrow.GetWidth()/4,nHeight/2-15,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				if(m_wCurrentUser==2)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2+m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
				if(m_wCurrentUser==3)
					m_ImageArrow.DrawImage(pDC,nWidth/2-m_ImageArrow.GetWidth()/4*2,nHeight/2-15,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*m_wCurrentUser,0);
			}
			if((wUserTimer!=0)&&(m_wCurrentUser==INVALID_CHAIR))
			{
				DrawUserClock(pDC,nWidth/2,nHeight/2-40,wUserTimer);
				if(i==0)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2-m_ImageArrow.GetHeight()*2-40,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);
				if(i==1)
					m_ImageArrow.DrawImage(pDC,nWidth/2+m_ImageArrow.GetWidth()/4,nHeight/2-15-40,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);
				if(i==2)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2+m_ImageArrow.GetHeight()-40,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);
				if(i==3)
					m_ImageArrow.DrawImage(pDC,nWidth/2-m_ImageArrow.GetWidth()/4*2,nHeight/2-15-40,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*i,0);

			}

			if (pUserData->GetUserStatus()==US_READY) 
				DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pUserData);

			//�й�
			if(m_bTrustee[i])
			{

				m_ImageTrustee.DrawImage(pDC,m_PointTrustee[i].x,m_PointTrustee[i].y);
			}
		}
	}

	//�û���־
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//����λͼ
		//CImageHandle ImageHandle(&m_ImageUserFlag);
		int nImageWidth=m_ImageUserFlag.GetWidth()/4;
		int nImageHeight=m_ImageUserFlag.GetHeight();

		//�滭��־
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			WORD wIndex=((i+GAME_PLAYER)-m_wBankerUser)%GAME_PLAYER;
			m_ImageUserFlag.TransDrawImage(pDC,m_UserFlagPos[i].x,m_UserFlagPos[i].y,nImageWidth,nImageHeight,nImageWidth*wIndex,0,RGB(255,0,255));
		}
	}
	//��ʷ����
	if (m_bShowScore==true)
	{
		//���ֱ���
		//CImageHandle HandleHistoryScore(&m_ImageHistoryScore);
		m_ImageHistoryScore.BitBlt(pDC->m_hDC,nWidth-m_ImageHistoryScore.GetWidth()-m_nYBorder*4,20);

		//����λ��
		INT nYBenchmark=20;
		INT nXBenchmark=nWidth-m_ImageHistoryScore.GetWidth()-m_nYBorder*2;
		UINT nFormat=DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER;

		//�滭����
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->SetTextColor(RGB(240,242,242));

		//�滭��Ϣ
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��ȡ�û�
			IClientUserItem * pUserData=GetClientUserItem(i);
			if ((pUserData==NULL)) continue;

			//λ�ü���
			CRect rcAccounts(nXBenchmark+2,nYBenchmark+24+i*20,nXBenchmark+73,nYBenchmark+42+i*20);
			CRect rcUpDownScore(nXBenchmark+64,nYBenchmark+24+i*20,nXBenchmark+93,nYBenchmark+42+i*20);
			CRect rcTurnScore(nXBenchmark+94,nYBenchmark+24+i*20,nXBenchmark+147,nYBenchmark+42+i*20);
			CRect rcCollectScore(nXBenchmark+128,nYBenchmark+24+i*20,nXBenchmark+197,nYBenchmark+42+i*20);

			//������Ϣ
			TCHAR szUpDownScore[32]=TEXT(""),szTurnScore[32]=TEXT(""),szCollectScore[32]=TEXT("");
			_sntprintf(szTurnScore,sizeof(szTurnScore),TEXT("%ld"),m_lLastTurnScore[i]);
			_sntprintf(szCollectScore,sizeof(szCollectScore),TEXT("%ld"),m_lAllTurnScore[i]);
			_sntprintf(szUpDownScore,sizeof(szUpDownScore),TEXT("%ld"),m_lUpDownScore[i]);

			//�滭��Ϣ
			
			pDC->DrawText(pUserData->GetNickName(),lstrlen(pUserData->GetNickName()),&rcAccounts,nFormat);
			pDC->DrawText(szUpDownScore,lstrlen(szUpDownScore),&rcUpDownScore,nFormat);
			pDC->DrawText(szTurnScore,lstrlen(szTurnScore),&rcTurnScore,nFormat);
			pDC->DrawText(szCollectScore,lstrlen(szCollectScore),&rcCollectScore,nFormat);
		}
	}
	//�����˿�
	for (WORD i=0;i<4;i++)
	{
		m_TableCard[i].DrawCardControl(pDC);
		m_DiscardCard[i].DrawCardControl(pDC);
		m_WeaveCard[i][0].DrawCardControl(pDC);
		m_WeaveCard[i][1].DrawCardControl(pDC);
		m_WeaveCard[i][2].DrawCardControl(pDC);
		m_WeaveCard[i][3].DrawCardControl(pDC);
	}

	m_WeaveCard[0][4].DrawCardControl(pDC);
	m_WeaveCard[1][4].DrawCardControl(pDC);
	m_WeaveCard[2][4].DrawCardControl(pDC);
	m_WeaveCard[3][4].DrawCardControl(pDC);

	//�ѻ��˿�
	m_HeapCard[3].DrawCardControl(pDC);
	m_HeapCard[0].DrawCardControl(pDC);
	m_HeapCard[1].DrawCardControl(pDC);
	m_HeapCard[2].DrawCardControl(pDC);

	//�û��˿�
	m_UserCard[0].DrawCardControl(pDC);
	m_UserCard[1].DrawCardControl(pDC);
	m_UserCard[2].DrawCardControl(pDC);
	m_HandCardControl.DrawCardControl(pDC);

	//������ʾ
	if (m_bOutCard==true)
	{
		//CImageHandle HandleOutCard(&m_ImageOutCard);
		m_ImageOutCard.TransDrawImage(pDC,(nWidth-m_ImageOutCard.GetWidth())/2,nHeight-130,RGB(255,0,255));
	}

	//�ȴ���ʾ
	if (m_bWaitOther==true)
	{
		//CImageHandle HandleWait(&m_ImageWait);
		m_ImageWait.TransDrawImage(pDC,(nWidth-m_ImageWait.GetWidth())/2,nHeight-130,RGB(255,0,255));
	}

	//��ׯ��־
	if (m_bHuangZhuang==true)
	{
		//CImageHandle HandleHuangZhuang(&m_ImageHuangZhuang);
		m_ImageHuangZhuang.TransDrawImage(pDC,(nWidth-m_ImageHuangZhuang.GetWidth())/2,nHeight/2-103,RGB(255,0,255));
	}



	//�û�״̬
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if ((m_wOutCardUser==i)||(m_wUserAction[i]!=0))
		{
			//����λ��
			int nXPos=0,nYPos=0;
			switch (i)
			{
			case 0:	//����
				{
					nXPos=nWidth/2-32;
					nYPos=m_nYBorder+95;
					break;
				}
			case 1:	//����
				{
					nXPos=nWidth-m_nXBorder-170;
					nYPos=nHeight/2-71;
					break;
				}
			case 2:	//����
				{
					nXPos=nWidth/2-32;
					nYPos=nHeight-m_nYBorder-220;
					break;
				}
			case 3:	//����
				{
					nXPos=m_nXBorder+115;
					nYPos=nHeight/2-71;
					break;
				}
			}

			//�滭����
			if (m_wUserAction[i]!=WIK_NULL)
			{	

				//�滭����
				if (m_bBombEffect==true)
				{

					//�滭Ч��
					INT nItemHeight=m_ImageActionAni.GetHeight()/6;
					INT nItemWidth=m_ImageActionAni.GetWidth()/BOMB_EFFECT_COUNT;

					//�滭����
					int nYImagePos=0;
					if (m_wUserAction[i]&WIK_PENG) nYImagePos=nItemHeight;
					else if (m_wUserAction[i]&WIK_GANG) nYImagePos=nItemHeight*2;
				//	else if (m_cbUserAction[i]&WIK_LISTEN) nYImagePos=nItemHeight*3;
					else if (m_wUserAction[i]&WIK_CHI_HU) nYImagePos=nItemHeight*4;
					else nYImagePos=0;
					m_ImageActionAni.DrawImage(pDC,nXPos-nItemWidth/2+54,nYPos+62-nItemHeight/2,nItemWidth,nItemHeight,
						nItemWidth*(m_cbBombFrameIndex%BOMB_EFFECT_COUNT),nYImagePos,nItemWidth,nItemHeight);
				}
			}
			else
			{	
				//��������
				//CImageHandle ImageHandle(&m_ImageActionBack);
				m_ImageActionBack.BlendDrawImage(pDC,nXPos,nYPos,m_ImageActionBack.GetWidth(),m_ImageActionBack.GetHeight(),
					0,0,RGB(255,0,255),180);


				//�滭�˿�
				g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,m_cbCardData,nXPos+15,nYPos+20);
			}

		}
	}
	//if(m_cbKingCardData!=0xFF)
	//{
	//	//�滭�˿�
	//	g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,m_cbKingCardData,nWidth/2-25,nHeight/2-20);

	//}
	//����Ч��
	if(m_wDiscUser!=INVALID_CHAIR)
	{

		CSize SizeDisc(m_ImageDisc.GetWidth()/DISC_EFFECT_COUNT,m_ImageDisc.GetHeight());
		CPoint pt=m_DiscardCard[m_wDiscUser].GetLastCardPosition();
		//�滭��Ϣ
		m_ImageDisc.DrawImage(pDC,pt.x,pt.y,SizeDisc.cx,SizeDisc.cy,
			m_cbDiscFrameIndex*SizeDisc.cx,0,SizeDisc.cx,SizeDisc.cy);

	}
	//ϴ��Ч��
	if(m_bBatchEffect)
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if(m_cbBatch[i]==1)
			{
				//����λ��
				int nXPos=0,nYPos=0;

				int nXCenter=nWidth/2;
				int nYCenter=nHeight/2-40;
				switch (i)
				{
				case 0:	//����
					{
						nXPos=nXCenter-276;
						nYPos=nYCenter-200;
						break;
					}
				case 1:	//����
					{
						nXPos=nXCenter+238;
						nYPos=nYCenter-200;
						break;
					}
				case 2:	//����
					{
						nXPos=nXCenter-235;
						nYPos=nYCenter+213;
						break;
					}
				case 3:	//����
					{
						nXPos=nXCenter-276;
						nYPos=nYCenter-145;
						break;
					}
				}

				//�滭Ч��
				INT nItemHeight=m_PngBatch[i].GetHeight();
				INT nItemWidth=m_PngBatch[i].GetWidth()/BATCH_EFFECT_COUNT;

				//�滭��Ϣ
				m_PngBatch[i].DrawImage(pDC,nXPos,nYPos,nItemWidth,nItemHeight,
					nItemWidth*(m_cbBatchFrameIndex),0,nItemWidth,nItemHeight);

			}
		}

	}
	return;
}

//��������
void CGameClientView::SetCellScore(LONG lCellScore)
{
	//�����˿�
	if (lCellScore!=m_lCellScore)
	{
		//���ñ���
		m_lCellScore=lCellScore;

		//���½���
		RefreshGameView();
	}

	return;
}

//�����˿�
void CGameClientView::SetHuangZhuang(bool bHuangZhuang)
{
	//�����˿�
	if (bHuangZhuang!=m_bHuangZhuang)
	{
		//���ñ���
		m_bHuangZhuang=bHuangZhuang;

		//���½���
		RefreshGameView();
	}

	return;
}

//ׯ���û�
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
	//�����û�
	if (wBankerUser!=m_wBankerUser)
	{
		//���ñ���
		m_wBankerUser=wBankerUser;

		//���½���
		RefreshGameView();
	}

	return;
}

//״̬��־
void CGameClientView::SetStatusFlag(bool bOutCard, bool bWaitOther)
{
	//���ñ���
	m_bOutCard=bOutCard;
	m_bWaitOther=bWaitOther;

	//���½���
	RefreshGameView();

	return;
}
//���ö���
bool CGameClientView::SetBombEffect(bool bBombEffect)
{
	if (bBombEffect==true)
	{
		//���ñ���
		m_bBombEffect=true;
		m_cbBombFrameIndex=0;

		//����ʱ��
		SetTimer(IDI_BOMB_EFFECT,250,NULL);
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
			RefreshGameView();
		}
	}

	return true;
}
//����ϴ��
bool CGameClientView::SetBatchEffect(bool bBatchEffect)
{
	if (bBatchEffect==true)
	{
		//���ñ���
		m_bBatchEffect=true;
		m_cbBatchFrameIndex=0;
		for (BYTE i=0;i<GAME_PLAYER;i++)
		{
			if(m_cbBatch[i]==1)
				m_HeapCard[i].SetCardData(0,0,0);
		}

		//����ʱ��
		SetTimer(IDI_BATCH_START,250,NULL);
	}
	else
	{
		//ֹͣ����
		if (m_bBatchEffect==true)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BATCH_START);

			//���ñ���
			m_bBatchEffect=false;
			m_cbBatchFrameIndex=0;

			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbBatch[i]==1)
					m_HeapCard[i].SetCardData(0,0,HEAP_FULL_COUNT);
			}

			//���½���
			RefreshGameView();
		}
	}

	return true;
}

//��������
void CGameClientView::SetKingCardIndex(BYTE cbKingCardIndex[2])
{
	//��������
	CopyMemory(m_cbKingCardIndex,cbKingCardIndex,sizeof(m_cbKingCardIndex));
    
	//���½���
	RefreshGameView();


	return;
}


//������Ϣ
void CGameClientView::SetOutCardInfo(WORD wViewChairID, BYTE cbCardData)
{
	//���ñ���
	m_cbCardData=cbCardData;

	m_wOutCardUser=wViewChairID;

	//���½���
	RefreshGameView();

	return;
}

//������Ϣ
void CGameClientView::SetUserAction(WORD wViewChairID, WORD wUserAction)
{
	//���ñ���
	if (wViewChairID<GAME_PLAYER)
	{
		m_wUserAction[wViewChairID]=wUserAction;
		SetBombEffect(true);
	}
	else 
	{
		ZeroMemory(m_wUserAction,sizeof(m_wUserAction));
		if(m_bBombEffect)
			SetBombEffect(false);
	}

	//���½���
	RefreshGameView();


	return;
}


//�����û�
void CGameClientView::SetDiscUser(WORD wDiscUser)
{
	if(m_wDiscUser != wDiscUser)
	{
		//���±���
		m_wDiscUser=wDiscUser;

		//���½���
		RefreshGameView();
	}
	return;
}
//��ʱ���
void CGameClientView::SetCurrentUser(WORD wCurrentUser)
{
	if (m_wCurrentUser != wCurrentUser)
	{
		//���±��� 
		m_wCurrentUser=wCurrentUser;

		//���½���
		RefreshGameView();
	}
	return;
}
//�����й�
void CGameClientView::SetTrustee(WORD wTrusteeUser,bool bTrustee)
{
	//У������ 
	ASSERT(wTrusteeUser>=0&&wTrusteeUser<GAME_PLAYER);

	if(m_bTrustee[wTrusteeUser] !=bTrustee)	
	{
		//��������
		m_bTrustee[wTrusteeUser]=bTrustee;

		//���½���
		RefreshGameView();
	}
	return;

}

//ϴ����
void CGameClientView::SetBatchResult(BYTE cbBatchResult[4])
{
	CopyMemory(m_cbBatch,cbBatchResult,sizeof(m_cbBatch));
}

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�������
	bool bRePaint=false;
	bool bHandle=m_HandCardControl.OnEventSetCursor(MousePoint,bRePaint);

	//�ػ�����
	if (bRePaint==true)
		RefreshGameView();

	//������
	if (bHandle==false)
		__super::OnSetCursor(pWnd,nHitTest,uMessage);

	return TRUE;
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//��ȡ�˿�
	BYTE cbHoverCard=m_HandCardControl.GetHoverCard();


	if (cbHoverCard!=0) 	
	{
		//AfxGetMainWnd()->PostMessage(IDM_OUT_CARD,cbHoverCard,cbHoverCard);
		CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
		if (pGameFrameEngine!=NULL) pGameFrameEngine->PostMessage(IDM_OUT_CARD,cbHoverCard,cbHoverCard);
	}

	return;
}

//��ʼ��ť
void CGameClientView::OnStart()
{
	//������Ϣ
	SendEngineMessage(IDM_START,0,0);

	return;
}

//1�ְ�ť
void CGameClientView::OnBatch()
{
	//������Ϣ
	SendEngineMessage(IDM_BATCH,1,1);

	return;
}


//�Ϲܿ���
void CGameClientView::OnStusteeControl()
{
	SendEngineMessage(IDM_TRUSTEE_CONTROL,0,0);
	return;
}
void CGameClientView::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//��������
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
		RefreshGameView();

		return;
	}
	//��������
	if (nIDEvent==IDI_BATCH_START)
	{
		//ֹͣ�ж�
		if (m_bBatchEffect==false)
		{
			KillTimer(IDI_BATCH_START);
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbBatch[i]==1)
					m_HeapCard[i].SetCardData(0,0,HEAP_FULL_COUNT);
			}
			RefreshGameView();
			//AfxGetMainWnd()->PostMessage(IDM_BATCH_END,0,0);

			CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
			if (pGameFrameEngine!=NULL) pGameFrameEngine->PostMessage(IDM_BATCH_END,0,0);
			return;
		}

		//���ñ���
		if ((m_cbBatchFrameIndex+1)>=BATCH_EFFECT_COUNT)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BOMB_EFFECT);

			//���ñ���
			m_bBatchEffect=false;
			m_cbBatchFrameIndex=0;
		}
		else m_cbBatchFrameIndex++;

		//���½���
		RefreshGameView();

		return;
	}
	if (nIDEvent==IDI_DISC_EFFECT)
	{
		//���ñ���
		if ((m_cbDiscFrameIndex+1)>=DISC_EFFECT_COUNT)
		{
			m_cbDiscFrameIndex=0;
		}
		else m_cbDiscFrameIndex++;

		//���½���
		RefreshGameView();

		return;

	}

	__super::OnTimer(nIDEvent);
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
	RefreshGameView();

	return;
}

//������ͼ
void CGameClientView::RefreshGameView()
{
	CRect rect;
	GetClientRect(&rect);
	InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

	return;
}
//������ͼ
void CGameClientView::RefreshGameView(CRect &rect)
{
	InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

	return;
}
//////////////////////////////////////////////////////////////////////////