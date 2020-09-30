#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClient.h"
#include ".\gameclientview.h"
//////////////////////////////////////////////////////////////////////////
//��ť��ʶ

//������ʶ
#define IDI_BOMB_EFFECT					501								//��ը��ʶ

//�궨��
#define BOMB_EFFECT_COUNT				6								//��ը��Ŀ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_START,OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD,OnOutCard)
	ON_BN_CLICKED(IDC_PASS_CARD,OnPassCard)
	ON_BN_CLICKED(IDC_BT_HUNTER,OnReqHunter)
	ON_BN_CLICKED(IDC_SCORE, OnBnClickedScore)
	ON_BN_CLICKED(IDC_LAST_TURN_CARD,OnLastTurnCard)
	ON_BN_CLICKED(IDC_TRUSTEE_CONTROL,OnStusteeControl)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//�ƶ�����
	m_bMoveMouse=false;
	m_bSelectCard=false;
	m_bSwitchCard=false;
	m_wHoverCardItem=INVALID_ITEM;
	m_wMouseDownItem=INVALID_ITEM;
	//���ֱ�־
	m_bLastTurnCard=false;

	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	FillMemory(m_wWinOrder,sizeof(m_wWinOrder),0xFFFF);
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	m_bShowScore=false;

	//��ʷ����
	memset(m_lAllTurnScore,0,sizeof(m_lAllTurnScore));
	memset(m_lLastTurnScore,0,sizeof(m_lLastTurnScore));


	//����λ��
	m_PointWinOrder[0].SetPoint(0,0);
	m_PointWinOrder[1].SetPoint(0,0);
	m_nXFace=48;
	m_nYFace=48;
	m_nXTimer=65;
	m_nYTimer=69;
	m_nXBorder=0;
	m_nYBorder=0;


	//ͼƬ��С
	m_sizeHistory.SetSize(m_ImageHistoryScore.GetWidth(),m_ImageHistoryScore.GetHeight());
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

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewFill.LoadFromResource(hInstance,IDB_VIEW_FILL);
	m_ImageViewBack.LoadFromResource(hInstance,IDB_VIEW_CENTER);
	m_ImageUserPass.LoadFromResource(hInstance,IDB_USER_PASS);
	m_ImageWinOrder.LoadFromResource(hInstance,IDB_WIN_ORDER);
	m_ImageTrustee.LoadImage(hInstance,TEXT("TRUSTEE"));
	m_ImageHistoryScore.LoadFromResource(hInstance,IDB_HISTORY_SCORE);
	m_ImageBombEffect.LoadImage(hInstance,TEXT("CARTOON_BOMB"));
	m_ImageLastTurn.LoadFromResource(hInstance,IDB_LAST_TURN_TIP);

	//�����ؼ�
	CRect CreateRect(0,0,0,0);
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,CreateRect,this,9);

	//�����˿�
	WORD wIndex=0;
	for (wIndex=0;wIndex<GAME_PLAYER;wIndex++)
	{
		//�û��˿�
		m_UserCardControl[wIndex].Load();
		m_UserCardControl[wIndex].SetDirection(true);
		m_UserCardControl[wIndex].SetDisplayFlag(true);	
		m_UserCardControl[wIndex].SetCardSpace(18,22,0);
		
		m_HandCardControl[wIndex].Load();
		m_HandCardControl[wIndex].SetDirection(true);
		m_HandCardControl[wIndex].SetDisplayFlag(false);

		//if(wIndex == 0)
			//m_HandCardControl[wIndex].ShowWindow(SW_HIDE);
	
		//�û��˿�
		if (wIndex!=ME_VIEW_CHAIR)m_HandCardControl[wIndex].SetCardSpace(14,8,0);
	}

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);
	m_btHunter.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_BT_HUNTER);
	m_btStusteeControl.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,CreateRect,this,IDC_TRUSTEE_CONTROL);
	m_btScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_SCORE);
	m_btLastTurnCard.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,CreateRect,this,IDC_LAST_TURN_CARD);
	
	//���ð�ť
	//HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_START,hInstance,false,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false,false);
	m_btHunter.SetButtonImage(IDB_OUT_HUNTER,hInstance,false,false);
	m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,hInstance,false,false);
	m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hInstance,false,false);
	m_btLastTurnCard.SetButtonImage(IDB_BT_LAST_TURN,hInstance,false,false);
#ifdef VIDEO_GAME
	//������Ƶ
	for (wIndex=0;wIndex<GAME_PLAYER;wIndex++)
	{
		//������Ƶ
		m_DlgVedioService[wIndex].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CreateRect,this,200+wIndex);
		m_DlgVedioService[wIndex].InitVideoService(ME_VIEW_CHAIR==wIndex,ME_VIEW_CHAIR==wIndex);
		//������Ƶ
		g_VedioServiceManager.SetVideoServiceControl(wIndex,&m_DlgVedioService[wIndex]);
	}
#endif
	return 0;
}

//�����Ϣ
void CGameClientView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//��������
	CPoint	ptOrigin;			
	CSize	sizeCardControl;	
	CRect	rcCardControl;	

	//�˿˷�Χ
	m_HandCardControl[ME_VIEW_CHAIR].GetOriginPoint(ptOrigin);
	m_HandCardControl[ME_VIEW_CHAIR].GetControlSize(sizeCardControl);
	rcCardControl.SetRect(ptOrigin.x,ptOrigin.y,ptOrigin.x+sizeCardControl.cx,ptOrigin.y+sizeCardControl.cy);

	//�����˿�
	if (rcCardControl.PtInRect(Point))
	{
		if (m_btOutCard.IsWindowVisible()) 
			SendEngineMessage(IDM_OUT_CARD,1,1);
	}
	else
	{
		m_btOutCard.EnableWindow(FALSE);
		m_HandCardControl[ME_VIEW_CHAIR].ShootAllCard(false);
	}

	RefreshGameView();
}
//���˫��
void CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags, point);

	//״̬�ж�
	if (m_HandCardControl[ME_VIEW_CHAIR].GetPositively()==true)
	{
		//��ȡ�˿�
		m_wMouseDownItem=m_HandCardControl[ME_VIEW_CHAIR].SwitchCardPoint(point);

		//�����˿�
		if (m_wMouseDownItem!=INVALID_ITEM)
		{
			//��ȡ�˿�
			tagCardItem * pCardItem=m_HandCardControl[ME_VIEW_CHAIR].GetCardFromIndex(m_wMouseDownItem);

			//���ñ���
			m_bMoveMouse=false;
			m_wHoverCardItem=m_wMouseDownItem;
			//��������
			if (pCardItem->bShoot) 
			{
				m_bSwitchCard=true;
			}
			//������Ϣ
			SendEngineMessage(IDM_LEFT_HIT_CARD,0,0);
		}
		else
		{
			//�����˿�
			m_btOutCard.EnableWindow(FALSE);
			m_HandCardControl[ME_VIEW_CHAIR].ShootAllCard(false);
		}
	}
	else
	{
		//�����˿�
		m_btOutCard.EnableWindow(FALSE);
		m_HandCardControl[ME_VIEW_CHAIR].ShootAllCard(false);
	}
	RefreshGameView();
	return;

	return;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//�ƶ�����
	m_bMoveMouse=false;
	m_bSelectCard=false;
	m_bSwitchCard=false;
	m_wHoverCardItem=INVALID_ITEM;
	m_wMouseDownItem=INVALID_ITEM;

	//���ֱ�־
	m_bLastTurnCard=false;

	//״̬����
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	FillMemory(m_wWinOrder,sizeof(m_wWinOrder),0xFFFF);
	m_bShowScore=false;

	//��ʷ����
	memset(m_lAllTurnScore,0,sizeof(m_lAllTurnScore));
	memset(m_lLastTurnScore,0,sizeof(m_lLastTurnScore));

	//���ؿؼ�
	m_ScoreView.ShowWindow(SW_HIDE);

	//���ذ�ť
	m_btStart.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btPassCard.ShowWindow(SW_HIDE);
//	m_btOutPrompt.ShowWindow(SW_HIDE);
	m_btScore.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_btOutCard.EnableWindow(FALSE);
	m_btPassCard.EnableWindow(FALSE);
	m_btStusteeControl.EnableWindow(FALSE);
//	m_btSortByCount.EnableWindow(FALSE);
//	m_btOne.EnableWindow(FALSE);
//	m_btTwo.EnableWindow(FALSE);
//	m_btThree.EnableWindow(FALSE);
//	m_btFour.EnableWindow(FALSE);
	m_btLastTurnCard.EnableWindow(FALSE);

	//�˿˿ؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_UserCardControl[i].SetCardData(NULL,0);
		m_HandCardControl[i].SetCardData(NULL,0);
		m_HandCardControl[i].SetPositively(false);
		m_HandCardControl[i].SetDisplayFlag(false);
	}

	return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//��������
	int nViewHeight=nHeight-m_nYBorder;

#ifdef VIDEO_GAME
	
	CRect rect ;
	m_DlgVedioService[0].GetWindowRect(&rect) ;

	m_ptAvatar[0].x=nWidth/2-m_nXFace/2;
	m_ptAvatar[0].y=m_nYBorder+5;
	m_ptNickName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptNickName[0].y=m_nYBorder+5;
	m_ptClock[0].x=nWidth/2;
	m_ptClock[0].y=m_nYBorder+200;
	m_ptReady[0].x=nWidth/2;
	m_ptReady[0].y=m_nYBorder+m_nYFace+35;
	m_PointWinOrder[0].x=m_ptAvatar[0].x-220;
	m_PointWinOrder[0].y=m_nYBorder+5;
	m_PointUserPass[0].x=nWidth/2-21;
	m_PointUserPass[0].y=nHeight/2-140;
	m_PointTrustee[0].x=m_ptNickName[0].x+100+m_nXTimer;
	m_PointTrustee[0].y=m_nYBorder+5;
	
	m_PointMessageDlg[0].x=nWidth/2+m_nXFace/2-5;
	m_PointMessageDlg[0].y=m_nYBorder+5+m_nYFace;

	

	m_ptAvatar[1].x=nWidth/2-m_nXFace/2;
	m_ptAvatar[1].y=nViewHeight-m_nYFace-5;
	m_ptNickName[1].x=nWidth/2+5+m_nXFace/2;
	m_ptNickName[1].y=nViewHeight-m_nYFace-2;
	m_ptClock[1].x=nWidth/2;
	m_ptClock[1].y=nHeight-m_nYTimer/2-20-m_nYBorder;
	m_ptReady[1].x=nWidth/2;
	m_ptReady[1].y=nViewHeight-m_nYBorder-220;
	m_PointWinOrder[1].x=m_ptAvatar[1].x-220;
	m_PointWinOrder[1].y=nViewHeight-m_nYFace-5;
	m_PointUserPass[1].x=nWidth/2-21;
	m_PointUserPass[1].y=nHeight-m_nYBorder-m_nYFace-210;
	m_PointTrustee[1].x=m_ptNickName[1].x+100+m_nXTimer;
	m_PointTrustee[1].y=nViewHeight-m_nYFace-5;
	m_PointMessageDlg[1].x=m_ptAvatar[1].x-m_ShowCharacter[1].GetWidth()+5;
	m_PointMessageDlg[1].y=nViewHeight-m_nYFace-5-m_ShowCharacter[1].GetHight();

	//�ƶ���ť
	m_ShowCharacter[0].SetWindowPos(&wndTop,m_PointMessageDlg[0].x,m_PointMessageDlg[0].y,0,0,SWP_NOSIZE);
	m_ShowCharacter[1].SetWindowPos(&wndTop,m_PointMessageDlg[1].x,m_PointMessageDlg[1].y,0,0,SWP_NOSIZE);


	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(25);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	
	//��ʼ��ť
	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-193-m_nYBorder,0,0,uFlags);
	
	//���ư�ť
	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()-40,nHeight-m_nYFace-m_nYBorder-176,0,0,uFlags);
	DeferWindowPos(hDwp,m_btHunter,NULL,(nWidth-rcButton.Width())/2,nHeight-m_nYFace-m_nYBorder-176,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+40,nHeight-m_nYFace-m_nYBorder-176,0,0,uFlags);
	

	//�йܰ�ť
	m_btStusteeControl.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btLastTurnCard,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-m_nYBorder-5-rcButton.Height()*3-10,0,0,uFlags);
	DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-m_nYBorder-5-rcButton.Height()*2-5,0,0,uFlags);
//	DeferWindowPos(hDwp,m_btPlayVoice,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-m_nYBorder-5-rcButton.Height(),0,0,uFlags|SW_SHOW);

	//��Ƶ����
	CRect rcAVDlg;
	m_DlgVedioService[0].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVedioService[0],NULL,m_nXBorder+5,50,0,0,uFlags);
	SetFlowerControlInfo(0,m_nXBorder+5,5);
	m_DlgVedioService[1].GetWindowRect(&rcAVDlg);
	DeferWindowPos(hDwp,m_DlgVedioService[1],NULL,m_nXBorder+5,nHeight-m_nYBorder-3-rcAVDlg.Height(),0,0,uFlags);
	SetFlowerControlInfo(1,m_nXBorder+5,nHeight-m_nYBorder-3-rcAVDlg.Height(),5);

	//��ְ�ť
	CRect rcScore;
	m_btScore.GetWindowRect(&rcScore);
	DeferWindowPos(hDwp,m_btScore,NULL,nWidth-(rcScore.Width()+m_sizeHistory.cx)/2,0,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_HandCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace+20,enXCenter,enYTop);
	m_HandCardControl[1].SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-40,enXCenter,enYBottom);
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,nHeight/2-150,enXCenter,enYTop);
	m_UserCardControl[1].SetBenchmarkPos(nWidth/2,nHeight-212-m_nYBorder,enXCenter,enYBottom);

	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,(nHeight-rcControl.Height()-100)/2,0,0,SWP_NOZORDER|SWP_NOSIZE);

#else

	m_ptAvatar[0].x=nWidth/2-m_nXFace/2;
	m_ptAvatar[0].y=m_nYBorder+5;
	m_ptNickName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptNickName[0].y=m_nYBorder+5;
	m_ptClock[0].x=nWidth/2;
	m_ptClock[0].y=m_nYBorder+200;
	m_ptReady[0].x=nWidth/2;
	m_ptReady[0].y=m_nYBorder+m_nYFace+35;
	m_PointWinOrder[0].x=m_ptAvatar[0].x-220;
	m_PointWinOrder[0].y=m_nYBorder+5;
	m_PointUserPass[0].x=nWidth/2-21;
	m_PointUserPass[0].y=nHeight/2-140;
	m_PointTrustee[0].x=m_ptNickName[0].x+100+m_nXTimer;
	m_PointTrustee[0].y=m_nYBorder+5;
	m_PointMessageDlg[0].x=nWidth/2+m_nXFace/2-5;
    m_PointMessageDlg[0].y=m_nYBorder+5+m_nYFace;

	m_ptAvatar[1].x=nWidth/2-m_nXFace/2;
	m_ptAvatar[1].y=nViewHeight-m_nYFace-5;
	m_ptNickName[1].x=nWidth/2+5+m_nXFace/2;
	m_ptNickName[1].y=nViewHeight-m_nYFace-2;
	m_ptClock[1].x=nWidth/2;
	m_ptClock[1].y=nHeight-m_nYTimer/2-20-m_nYBorder;
	m_ptReady[1].x=nWidth/2;
	m_ptReady[1].y=nViewHeight-m_nYBorder-220;
	m_PointWinOrder[1].x=m_ptAvatar[1].x-220;
	m_PointWinOrder[1].y=nViewHeight-m_nYFace-5;
	m_PointUserPass[1].x=nWidth/2-21;
	m_PointUserPass[1].y=nHeight-m_nYBorder-m_nYFace-210;
	m_PointTrustee[1].x=m_ptNickName[1].x+100+m_nXTimer;
	m_PointTrustee[1].y=nViewHeight-m_nYFace-5;
	
	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(21);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
		//��ʼ��ť
	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-213-m_nYBorder,0,0,uFlags);

	//���ư�ť
	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()-40,nHeight-m_nYFace-m_nYBorder-196,0,0,uFlags);
	DeferWindowPos(hDwp,m_btHunter,NULL,(nWidth-rcButton.Width())/2,nHeight-m_nYFace-m_nYBorder-196,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+40,nHeight-m_nYFace-m_nYBorder-196,0,0,uFlags);
    
	//�йܰ�ť
	m_btStusteeControl.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-m_nYBorder-5-rcButton.Height()*2-5,0,0,uFlags);
//	DeferWindowPos(hDwp,m_btPlayVoice,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-m_nYBorder-5-rcButton.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btLastTurnCard,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-m_nYBorder-5-rcButton.Height()*3-10,0,0,uFlags);

	//��ְ�ť
	CRect rcScore;
	m_btScore.GetWindowRect(&rcScore);
	DeferWindowPos(hDwp,m_btScore,NULL,nWidth-(rcScore.Width()+m_sizeHistory.cx)/2,0,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_HandCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace+20,enXCenter,enYTop);
	m_HandCardControl[1].SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-m_nYBorder-40,enXCenter,enYBottom);
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,nHeight/2-190,enXCenter,enYTop);
	m_UserCardControl[1].SetBenchmarkPos(nWidth/2,nHeight-242-m_nYBorder,enXCenter,enYBottom);
	//SetFlowerControlInfo(0,nWidth/2,m_nYBorder+m_nYFace+BIG_FACE_HEIGHT);
	//SetFlowerControlInfo(1,nWidth/2,nHeight-m_nYBorder-m_nYFace-BIG_FACE_HEIGHT);

	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,(nHeight-rcControl.Height()-100)/2,0,0,SWP_NOZORDER|SWP_NOSIZE);

#endif
	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageViewBack,DRAW_MODE_CENTENT);

	//��ҳ���
	for (int i=0; i<GAME_PLAYER; ++i)
	{
		m_HandCardControl[i].DrawCardControl(pDC);
		m_UserCardControl[i].DrawCardControl(pDC);
	}

	CFont font,*OldFont;
	font.CreateFont(13,0,0,0,600,0,0,0,134,3,2,1,2,TEXT("����"));
	OldFont=pDC->SelectObject(&font);

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wUserTimer=GetUserClock(i);
		IClientUserItem * pClientUserItem=GetClientUserItem(i);

		//�滭�û�
		if (pClientUserItem!=NULL)
		{
			//�滭�û�
					
			CSize NameSize,ScoreSize,LeftCardSize;
			int  nLong=0,nXPrintPos=0;
			CString szScore,szLeftCard;

			nLong=m_nXFace+10;
			NameSize=pDC->GetTextExtent(pClientUserItem->GetNickName());
			NameSize.cx=(NameSize.cx>120)?120:NameSize.cx;
			nLong+=NameSize.cx;
			nLong+=10;

			

			szScore.Format(_T("[��%I64d]"),pClientUserItem->GetUserScore());
			ScoreSize=pDC->GetTextExtent(szScore);
			nLong+=ScoreSize.cx;
			nLong+=10;

			szLeftCard.Format(_T("ʣ��:%d��"),m_bCardCount[i]);
			LeftCardSize=pDC->GetTextExtent(szLeftCard);
			nLong+=LeftCardSize.cx;
			nLong+=10;

			nXPrintPos=(nWidth-nLong)/2;
                   				
			//�û�ͷ��
			DrawUserAvatar(pDC,nXPrintPos,m_ptAvatar[i].y,pClientUserItem);
			nXPrintPos+=m_nXFace;
			nXPrintPos+=10;

			//�û�����
			pDC->SetTextAlign(TA_LEFT|TA_TOP);
			DrawTextString(pDC,pClientUserItem->GetNickName(),(wUserTimer>0)?RGB(255,0,0):RGB(76,153,235),RGB(0,0,0),&CRect(nXPrintPos,m_ptAvatar[i].y+m_nYFace-NameSize.cy,nXPrintPos+NameSize.cx,m_ptAvatar[i].y+m_nYFace));
	
			nXPrintPos+=NameSize.cx;
			nXPrintPos+=10;
			//�����Ŀ
			DrawTextString(pDC,szScore,RGB(255,255,255),RGB(0,0,0),nXPrintPos,m_ptAvatar[i].y+m_nYFace-ScoreSize.cy);

			nXPrintPos+=ScoreSize.cx;
			nXPrintPos+=10;
			//ʣ���˿�
			DrawTextString(pDC,szLeftCard,RGB(255,255,255),RGB(0,0,0),nXPrintPos,m_ptAvatar[i].y+m_nYFace-LeftCardSize.cy);

			//������Ϣ
			if (wUserTimer!=0) DrawUserClock(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer);
			if (pClientUserItem->GetUserStatus()==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);			
			
			//�й�
			if(m_bTrustee[i])
				m_ImageTrustee.DrawImage(pDC,m_PointTrustee[i].x,m_PointTrustee[i].y);
		}
	}

	pDC->SelectObject(OldFont);
	font.DeleteObject();

	//��ʷ����
	if (m_bShowScore==true)
	{
		//���ֱ���
		m_ImageHistoryScore.BitBlt(pDC->m_hDC,nWidth-m_ImageHistoryScore.GetWidth()-2*m_nYBorder,27);

		//����λ��
		INT nYBenchmark=27;
		INT nXBenchmark=nWidth-m_ImageHistoryScore.GetWidth()-2*m_nYBorder;
		UINT nFormat=DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER;

		//�滭����
		//pDC->SetTextAlign(TA_LEFT|TA_TOP);
		//pDC->SetTextColor(RGB(240,242,242));

		//�滭��Ϣ
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��ȡ�û�
			IClientUserItem * pClientUserItem=GetClientUserItem(i);
			if ((pClientUserItem==NULL)) continue;

			//λ�ü���
			CRect rcAccounts(nXBenchmark+2,nYBenchmark+24+i*20,nXBenchmark+73,nYBenchmark+42+i*20);
			CRect rcTurnScore(nXBenchmark+74,nYBenchmark+24+i*20,nXBenchmark+127,nYBenchmark+42+i*20);
			CRect rcCollectScore(nXBenchmark+128,nYBenchmark+24+i*20,nXBenchmark+197,nYBenchmark+42+i*20);

			//������Ϣ
			TCHAR szTurnScore[32]=TEXT(""),szCollectScore[32]=TEXT("");
			_sntprintf(szTurnScore,sizeof(szTurnScore),TEXT("%ld"),m_lLastTurnScore[i]);
			_sntprintf(szCollectScore,sizeof(szCollectScore),TEXT("%ld"),m_lAllTurnScore[i]);

			//�滭��Ϣ
			DrawText(pDC,pClientUserItem->GetNickName(),lstrlen(pClientUserItem->GetNickName()),&rcAccounts,nFormat);
			DrawText(pDC,szTurnScore,lstrlen(szTurnScore),&rcTurnScore,nFormat);
			DrawText(pDC,szCollectScore,lstrlen(szCollectScore),&rcCollectScore,nFormat);
		}
	}

	//������־
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_bPass[i]==true) 
		{
			m_ImageUserPass.TransDrawImage(pDC,m_PointUserPass[i].x,m_PointUserPass[i].y,RGB(255,0,255));
		}
	}

	//�滭��ը
	if (m_bBombEffect==true)
	{
		//�滭Ч��
		INT nImageHeight=m_ImageBombEffect.GetHeight();
		INT nImageWidth=m_ImageBombEffect.GetWidth()/BOMB_EFFECT_COUNT;
		m_ImageBombEffect.DrawImage(pDC,(nWidth-nImageWidth)/2,0,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex%BOMB_EFFECT_COUNT),0);
	}
	////�滭����
	//CImageHandle ImageHnadleWinOrder(&ImageWinOrder);
	//CSize SizeWinOrder(ImageWinOrder.GetWidth()/4,ImageWinOrder.GetHeight());
	//for (WORD i=0;i<GAME_PLAYER;i++)
	//{
	//	if (m_wWinOrder[i]!=0xFFFF)
	//	{
	//		ASSERT(m_PointWinOrder[i].x != 0);
	//		ASSERT(m_PointWinOrder[i].y != 0);

	//		//�滭��Ϣ
	//		ImageWinOrder.TransDrawImage(pDC,m_PointWinOrder[i].x,m_PointWinOrder[i].y,ImageWinOrder.GetWidth()/4,ImageWinOrder.GetHeight(),(m_wWinOrder[i]-1)*SizeWinOrder.cx,0,RGB(255,0,255));
	//	}
	//}

	//���ֱ�־
	if (m_bLastTurnCard==true)
	{
		//�滭��־
		int nImageWidth=m_ImageLastTurn.GetWidth();
		int nImageHegith=m_ImageLastTurn.GetHeight();
		m_ImageLastTurn.TransDrawImage(pDC,(nWidth-nImageWidth)/2,(nHeight-nImageHegith)/2,RGB(255,0,255));
	}
	return;
}


//���÷���
void CGameClientView::SetPassFlag(WORD wChairID, bool bPass)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
			m_bPass[i]=bPass;
	}
	else 
		m_bPass[wChairID]=bPass;

	//���½���
	RefreshGameView();

	return;
}


void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame,CRect*rect)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		DrawText(pDC,pszString,&CRect(rect->left+nXExcursion[i],rect->top+nYExcursion[i],rect->right+nXExcursion[i],rect->bottom+nYExcursion[i]),DT_END_ELLIPSIS |DT_LEFT|DT_SINGLELINE);
	}

	//�滭����
	pDC->SetTextColor(crText);
	DrawText(pDC,pszString,rect,DT_END_ELLIPSIS |DT_LEFT|DT_SINGLELINE);

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
		TextOut(pDC,nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	TextOut(pDC,nXPos,nYPos,pszString,nStringLength);

	return;
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
	RefreshGameView();

	return;
}

//��������
bool CGameClientView::SetUserWinOrder(WORD wChairID, WORD wWinOrder)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_wWinOrder[i]=wWinOrder;
	}
	else
	{
		ASSERT(wChairID<GAME_PLAYER);
		m_wWinOrder[wChairID]=wWinOrder;
	}

	//���½���
	RefreshGameView();

	return true;
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
			RefreshGameView();
		}
	}

	return true;
}

//��������
void CGameClientView::SetLastTurnCard(bool bLastTurnCard)
{
	//���ñ���
	if (bLastTurnCard!=m_bLastTurnCard)
	{
		//���ñ���
		m_bLastTurnCard=bLastTurnCard;

		//���½���
		RefreshGameView();
	}

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

void CGameClientView::OnReqHunter()
{
	SendEngineMessage(IDM_REQ_HUNTER,1,1);
}

//�����й�
void CGameClientView::OnStusteeControl()
{
	SendEngineMessage(IDM_TRUSTEE_CONTROL,0,0);
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
	RefreshGameView();

	return;
}

//�����˿�
void CGameClientView::OnLastTurnCard()
{
	SendEngineMessage(IDM_LAST_TURN_CARD,0,0);
	return;
}
//////////////////////////////////////////////////////////////////////////


void CGameClientView::OnTimer(UINT nIDEvent)
{
	
	switch(nIDEvent)
	{
	case IDI_BOMB_EFFECT:	//��ը����
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
	}

	__super::OnTimer(nIDEvent);
}
//�����Ϣ
VOID CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	//ѡ����
	if ((m_bSelectCard==true)&&(m_bMoveMouse==true))
	{
		//�����˿�
		if (m_wHoverCardItem>m_wMouseDownItem)
		{
			m_HandCardControl[ME_VIEW_CHAIR].SetShootIndex(m_wMouseDownItem,m_wHoverCardItem);
		}
		else
		{
			m_HandCardControl[ME_VIEW_CHAIR].SetShootIndex(m_wHoverCardItem,m_wMouseDownItem);
		}

		//������Ϣ
		SendEngineMessage(IDM_LEFT_HIT_CARD,0,0);
		//���½���
		RefreshGameView();
	}

	//���ñ���
	m_bMoveMouse=false;
	m_bSwitchCard=false;
	m_bSelectCard=false;

	//��������
	m_wMouseDownItem=INVALID_ITEM;
	m_wHoverCardItem=INVALID_ITEM;

	//�ͷ����
	ReleaseCapture();

	return;
}

//�����Ϣ
VOID CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);
	//״̬�ж�
	if (m_HandCardControl[ME_VIEW_CHAIR].GetPositively()==true)
	{
		//��ȡ�˿�
		m_wMouseDownItem=m_HandCardControl[ME_VIEW_CHAIR].SwitchCardPoint(Point);
		//�����˿�
		if (m_wMouseDownItem!=INVALID_ITEM)
		{
			//��ȡ�˿�
			tagCardItem * pCardItem=m_HandCardControl[ME_VIEW_CHAIR].GetCardFromIndex(m_wMouseDownItem);
			//���ñ���
			m_bMoveMouse=false;
			m_wHoverCardItem=m_wMouseDownItem;

			//��������
			if (pCardItem->bShoot==false) 
				m_bSelectCard=true;
			else
				m_bSwitchCard=true;
		}

		//Ĭ�ϴ���
		if (m_bMoveMouse==false)
		{
			//��ȡ����
			m_wMouseDownItem=m_HandCardControl[ME_VIEW_CHAIR].SwitchCardPoint(Point);
			if(m_wMouseDownItem==INVALID_ITEM) return ;
			//���ÿؼ�
			tagCardItem * pCardItem=m_HandCardControl[ME_VIEW_CHAIR].GetCardFromPoint(Point);
			pCardItem->bShoot=!pCardItem->bShoot;
			//������Ϣ
			SendEngineMessage(IDM_LEFT_HIT_CARD,0,0);
		}
	}
	//�����ؼ�
	RefreshGameView();
	return;
}

//�����Ϣ
VOID CGameClientView::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//��������
	WORD wHoverCardItem=INVALID_ITEM;
	//�ƶ��ж�
	if (m_wHoverCardItem!=INVALID_ITEM)
	{
		//��ȡ���
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		//�˿˴�С
		CSize ControlSize;
		m_HandCardControl[ME_VIEW_CHAIR].GetControlSize(ControlSize);

		//�˿�λ��
		CPoint OriginPoint;
		m_HandCardControl[ME_VIEW_CHAIR].GetOriginPoint(OriginPoint);

		//���е���
		if (MousePoint.x<OriginPoint.x) MousePoint.x=OriginPoint.x;
		if (MousePoint.x>(OriginPoint.x+ControlSize.cx)) MousePoint.x=(OriginPoint.x+ControlSize.cx);

		//��ȡ����
		MousePoint.y=OriginPoint.y+DEF_SHOOT_DISTANCE;
		wHoverCardItem=m_HandCardControl[ME_VIEW_CHAIR].SwitchCardPoint(MousePoint);

		//�ƶ�����
		if (wHoverCardItem!=m_wHoverCardItem)
		{
			m_bMoveMouse=true;
		}
	}

	//ѡ���˿�
	if ((m_bSelectCard==true)&&(wHoverCardItem!=m_wHoverCardItem))
	{
		//�����˿�
		if (wHoverCardItem>m_wMouseDownItem)
		{
			m_HandCardControl[ME_VIEW_CHAIR].SetSelectIndex(m_wMouseDownItem,wHoverCardItem);
		}
		else
		{
			m_HandCardControl[ME_VIEW_CHAIR].SetSelectIndex(wHoverCardItem,m_wMouseDownItem);
		}

		//���½���
		RefreshGameView();
	}

	//���ñ���
	m_wHoverCardItem=wHoverCardItem;

	return;
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�˿˲���
	if (m_HandCardControl[ME_VIEW_CHAIR].OnEventSetCursor(MousePoint)==true) return TRUE;

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}