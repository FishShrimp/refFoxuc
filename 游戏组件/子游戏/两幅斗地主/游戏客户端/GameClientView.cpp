#include "StdAfx.h"
#include "Resource.h"
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

//��ը��Ŀ
#define BOMB_EFFECT_COUNT				6								//��ը��Ŀ

//��ը��ʶ
#define IDI_BOMB_EFFECT					101								//��ը��ʶ
#define NAME_WIDTH						120	
static int m_nXFace =48;
static int m_nYFace =48;
static int m_nYBorder=0;
static int m_nXBorder=0;
static int m_nXTimer=60;
static int m_nYTimer=60;
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView() 
{
	//��Ϸ����
	m_lBaseScore=0;
	m_wBombTime=1;
	m_cbLandScore=0;
	m_wLandUser=INVALID_CHAIR;

	//״̬����
	m_bLandTitle=false;
	ZeroMemory(m_bPass,sizeof(m_bPass));
	ZeroMemory(m_bScore,sizeof(m_bScore));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bUserTrustee, sizeof( m_bUserTrustee ) );
	m_bShowScore=false;

	//��ը����
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//λ����Ϣ
	ZeroMemory(m_ptScore,sizeof(m_ptScore));
	ZeroMemory(m_ptLand,sizeof(m_ptLand));
	m_bDeasilOrder = false;



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
	m_ImageHistoryScore.LoadFromResource(this,hInstance,IDB_HISTORY_SCORE);
	m_ImageBomb.LoadFromResource(this,hInstance,IDB_BOMB);
	m_ImageBack.LoadFromResource(this,hInstance,IDB_VIEW_BACK);
	m_ImageCenter.LoadFromResource(this,hInstance,IDB_VIEW_CENTER);
	m_ImageBombEffect.LoadFromResource(this,hInstance,IDB_BOMB_EFFECT);

	m_ImageScore.LoadImage(this,hInstance, TEXT( "SCORE" ) );
	m_ImageLand.LoadImage(this,hInstance, TEXT( "BANKER" ) );
	m_ImageUserTrustee.LoadImage(this,hInstance,  TEXT("USER_TRUSTEE"));


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
		m_UserCardControl[i].SetCardSpace( 18, 22, 0 );

		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,50+i);

		if ( i == 0 || i == ME_VIEW_CHAIR )
			m_UserCardControl[i].SetDirection(true);
		else
			m_UserCardControl[i].SetDirection(false);

		if ( i ==ME_VIEW_CHAIR || i == 0 )
			m_HandCardControl[i].SetDirection(true);
		else
			m_HandCardControl[i].SetDirection(false);

		//�û��˿�
		if (i!=ME_VIEW_CHAIR)
		{
			m_HandCardControl[i].SetCardSpace(8,8,0);
			m_HandCardControl[i].SetDisplayFlag(false);
			m_HandCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
		}
		else
		{
			m_HandCardControl[i].SetDisplayFlag(false);
			m_HandCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
		}
	}

	//�����˿�
	m_BackCardControl.SetCardSpace(14,0,0);
	m_BackCardControl.SetDisplayFlag(false);
	m_HandCardControl[ME_VIEW_CHAIR].SetSinkWindow(this);
	m_BackCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,41);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btScore.Create(NULL,WS_CHILD,CreateRect,this,IDC_SCORE);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);
	m_btOneScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_ONE_SCORE);
	m_btTwoScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_TWO_SCORE);
	m_btGiveUpScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_GIVE_UP_SCORE);
	m_btAutoOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_AUTO_OUTCARD);
	m_btThreeScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_THREE_SCORE);
	m_btAutoPlayOn.Create(TEXT(""), WS_CHILD,CreateRect,this,IDC_AUTOPLAY_ON);
	m_btAutoPlayOff.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_AUTOPLAY_OFF);
	m_btSortCard.Create(NULL,WS_CHILD|WS_DISABLED|WS_VISIBLE,CreateRect,this,IDC_SORT_CARD);
    

	//���ð�ť

	m_btStart.SetButtonImage(IDB_START,hInstance,false,false);
	m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hInstance,false,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false,false);
	m_btOneScore.SetButtonImage(IDB_ONE_SCORE,hInstance,false,false);
	m_btTwoScore.SetButtonImage(IDB_TWO_SCORE,hInstance,false,false);
	m_btGiveUpScore.SetButtonImage(IDB_GIVE_UP,hInstance,false,false);
	m_btAutoOutCard.SetButtonImage(IDB_AUTO_OUT_CARD,hInstance,false,false);
	m_btThreeScore.SetButtonImage(IDB_THREE_SCORE,hInstance,false,false);
	m_btAutoPlayOn.SetButtonImage  (IDB_AUTOPLAY_ON,hInstance,false,false);
	m_btAutoPlayOff.SetButtonImage (IDB_AUTOPLAY_OFF,hInstance,false,false);
	m_btSortCard.SetButtonImage(IDB_COUNT_SORT,hInstance,false,false);

    m_btAutoPlayOn.ShowWindow(SW_SHOW);
    m_btAutoPlayOff.ShowWindow(SW_HIDE);

	m_ScoreView.Create(IDD_GAME_SCORE,this);

	return 0;
}

//�����Ϣ
void CGameClientView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//�����˿�
		//�����˿�
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl[ME_VIEW_CHAIR].ShootAllCard(false);

	return;
}

//���˫��
void CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags, point);

	//�����˿�
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl[ME_VIEW_CHAIR].ShootAllCard(false);

	SetBombEffect(true);

	return;
}

//���ý���
VOID CGameClientView::ResetGameView()
{
	//��Ϸ����
	m_lBaseScore=0;
	m_wBombTime=1;
	m_cbLandScore=0;
	m_wLandUser=INVALID_CHAIR;
	m_bShowScore=false;

	//��ը����
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//״̬����
	m_bLandTitle=false;
	ZeroMemory(m_bPass,sizeof(m_bPass));
	ZeroMemory(m_bScore,sizeof(m_bScore));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bUserTrustee, sizeof( m_bUserTrustee ) );
    
	//���ؿؼ�
	m_ScoreView.ShowWindow(SW_HIDE);
	m_btScore.ShowWindow(SW_HIDE);
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hResInstance,false,false);

	//���ذ�ť
	m_btStart.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btPassCard.ShowWindow(SW_HIDE);
	m_btGiveUpScore.ShowWindow(SW_HIDE);
	m_btAutoOutCard.ShowWindow(SW_HIDE);
	m_btThreeScore.ShowWindow(SW_HIDE);
	m_btAutoPlayOn.ShowWindow(SW_SHOW);
	m_btAutoPlayOff.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_btOutCard.EnableWindow(FALSE);
	m_btPassCard.EnableWindow(FALSE);
	m_btAutoOutCard.EnableWindow(FALSE);
	m_btSortCard.EnableWindow(FALSE);
	m_btSortCard.SetButtonImage(IDB_COUNT_SORT,AfxGetInstanceHandle(),false,false);

	//�˿˿ؼ�
	m_BackCardControl.SetCardData(NULL,0);
	m_HandCardControl[ME_VIEW_CHAIR].SetPositively(false);
	m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag(false);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_UserCardControl[i].SetCardData(NULL,0);
		m_HandCardControl[i].SetCardData(NULL,0);
		if (i!=ME_VIEW_CHAIR)
			m_HandCardControl[i].SetCardSpace(8,8,0);
	}

	return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{

	//��������
	m_ptAvatar[0].x=nWidth/2-m_nXFace/2;
	m_ptAvatar[0].y=m_nYBorder+5;
	m_ptNickName[0].x=nWidth/2+5+m_nXFace/2;
	m_ptNickName[0].y=m_nYBorder+2;
	m_ptClock[0].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptClock[0].y=m_nYBorder+m_nYTimer/2+2;
	m_ptReady[0].x=nWidth/2+10;
	m_ptReady[0].y=nHeight/2-150;
	m_ptScore[0].x=nWidth/2-21;
	m_ptScore[0].y=m_nYBorder+m_nYFace+150;
	m_ptLand[0].x=nWidth/2-m_nXFace/2-m_LandSize.cx-m_nXTimer*2;
	m_ptLand[0].y=m_nXBorder+5;

	//��������
	m_ptAvatar[1].x=m_nXBorder+5;
	m_ptAvatar[1].y=nHeight/2-m_nYFace;
	m_ptNickName[1].x=m_nXBorder+5;
	m_ptNickName[1].y=nHeight/2+5;
	m_ptClock[1].x=m_nXBorder+2+m_nXTimer/2;
	m_ptClock[1].y=m_ptAvatar[1].y-75;
	m_ptReady[1].x=nWidth/2-150;
	m_ptReady[1].y=nHeight/2;
	m_ptScore[1].x=m_nXBorder+m_nXFace+147-10;
	m_ptScore[1].y=nHeight/2-m_nYFace-30;
	m_ptLand[1].x=m_nXBorder+5;
	m_ptLand[1].y=m_ptAvatar[1].y-35;

	//��������
	m_ptAvatar[3].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptAvatar[3].y=nHeight/2-m_nYFace;
	m_ptNickName[3].x=nWidth-m_nXBorder-5-NAME_WIDTH;
	m_ptNickName[3].y=nHeight/2+5;
	m_ptClock[3].x=nWidth-m_nXBorder-m_nXTimer/2-2;
	m_ptClock[3].y=m_ptAvatar[3].y-75;
	m_ptReady[3].x=nWidth/2+170;
	m_ptReady[3].y=nHeight/2;
	m_ptScore[3].x=nWidth-m_nXBorder-m_nXFace-190-7;
	m_ptScore[3].y=nHeight/2-m_nYFace-30;
	m_ptLand[3].x=nWidth-m_nXBorder-m_LandSize.cx-5;
	m_ptLand[3].y=m_ptAvatar[3].y-35;

	//�û��˿�
	m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-147,nHeight/2-40,enXRight,enYCenter);
	m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+147,nHeight/2-40,enXLeft,enYCenter);
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,190,enXCenter,enYTop);
	m_HandCardControl[3].SetBenchmarkPos(nWidth-m_nXBorder-m_nXFace-50,nHeight/2,enXRight,enYCenter);
	m_HandCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+50,nHeight/2,enXLeft,enYCenter);
	m_HandCardControl[0].SetBenchmarkPos(nWidth/2+40,130,enXCenter,enYCenter);	


	//��������
	m_ptAvatar[ME_VIEW_CHAIR].x=nWidth/2-m_nXFace/2;
	m_ptAvatar[ME_VIEW_CHAIR].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptNickName[ME_VIEW_CHAIR].x=nWidth/2+5+m_nXFace/2;
	m_ptNickName[ME_VIEW_CHAIR].y=nHeight-m_nYBorder-m_nYFace-2;
	m_ptClock[ME_VIEW_CHAIR].x=nWidth/2-m_nXFace/2-m_nXTimer/2-5;
	m_ptClock[ME_VIEW_CHAIR].y=nHeight-m_nYBorder-m_nYTimer/2-2;
	m_ptReady[ME_VIEW_CHAIR].x=nWidth/2 + 10;
	m_ptReady[ME_VIEW_CHAIR].y=nHeight/2+150;
	m_ptScore[ME_VIEW_CHAIR].x=nWidth/2-21;
	m_ptScore[ME_VIEW_CHAIR].y=nHeight-m_nYBorder-m_nYFace-210;
	m_ptLand[ME_VIEW_CHAIR].x=nWidth/2-m_nXFace/2-m_LandSize.cx-m_nXTimer*2;
	m_ptLand[ME_VIEW_CHAIR].y=nHeight-m_nXBorder-m_LandSize.cy-5;

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-230,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()*3/2-10,nHeight-230,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btAutoOutCard,NULL,nWidth/2-rcButton.Width()/2,nHeight-217-m_nYBorder,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAutoOutCard,NULL,nWidth+10,nHeight+10,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+rcButton.Width()/2+10,nHeight-230,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOneScore,NULL,nWidth/2-rcButton.Width()*2-12,nHeight-230,0,0,uFlags);
	DeferWindowPos(hDwp,m_btTwoScore,NULL,nWidth/2-rcButton.Width()-2,nHeight-230,0,0,uFlags);
	DeferWindowPos(hDwp,m_btThreeScore,NULL,nWidth/2+10,nHeight-230,0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUpScore,NULL,nWidth/2+30+rcButton.Width()-10,nHeight-230,0,0,uFlags);
	DeferWindowPos(hDwp,m_btSortCard,NULL,nWidth-m_nXBorder-rcButton.Width()-15,nHeight-rcButton.Height()*2-m_nYBorder-40,0,0,uFlags);

	//��ְ�ť
	CRect rcScore;
	m_btScore.GetWindowRect(&rcScore);
	DeferWindowPos(hDwp,m_btScore,NULL,nWidth-(rcScore.Width()+m_sizeHistory.cx)/2-40,0,0,0,uFlags);

	EndDeferWindowPos(hDwp);

	CRect rcBtAutoPlay;
	m_btAutoPlayOn.GetWindowRect( rcBtAutoPlay );
	m_btAutoPlayOn .MoveWindow( nWidth - m_nXBorder - 5 - rcBtAutoPlay.Width() , nHeight-5-rcBtAutoPlay.Height(), rcBtAutoPlay.Width(), rcBtAutoPlay.Height(), FALSE);
    m_btAutoPlayOff.MoveWindow( nWidth - m_nXBorder - 5 - rcBtAutoPlay.Width(), nHeight-5-rcBtAutoPlay.Height(), rcBtAutoPlay.Width(), rcBtAutoPlay.Height(), FALSE);
	m_btSortCard.MoveWindow( nWidth - m_nXBorder - 5 - rcBtAutoPlay.Width(), nHeight-10-2*rcBtAutoPlay.Height(), rcBtAutoPlay.Width(), rcBtAutoPlay.Height(), FALSE);

	//�ƶ��˿�
	m_BackCardControl.SetBenchmarkPos(m_nXBorder+5,m_nYBorder+5,enXLeft,enYTop);
	m_HandCardControl[ME_VIEW_CHAIR].SetBenchmarkPos(nWidth/2+45,nHeight-m_nYFace-m_nYBorder-25,enXCenter,enYBottom);
	m_UserCardControl[ME_VIEW_CHAIR].SetBenchmarkPos(nWidth/2,nHeight-223-m_nYBorder,enXCenter,enYBottom);

	return;
}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageBack,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageCenter,DRAW_MODE_CENTENT);

	//�滭�û�
	TCHAR szBuffer[64]=TEXT("");
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
//#ifdef DEBUG
//		CRect rc(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+NAME_WIDTH,m_ptNickName[i].y+16);
//		CDFontEx::DrawText(this, pDC, 14, 400,TEXT("asd����������ռ����˵���ԣ������׶Σ�������˹�����"),rc, RGB(240,240,0),(i==3?DT_RIGHT:DT_LEFT)|DT_END_ELLIPSIS);
//
//		rc.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+16,m_ptNickName[i].x+NAME_WIDTH,m_ptNickName[i].y+32);
//		//DrawTextString( pDC, szBuffer, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), m_ptNickName[i].x,m_ptNickName[i].y+18 ,14,400,(i==3?DT_RIGHT:DT_LEFT)|DT_END_ELLIPSIS);
//		CDFontEx::DrawText(this, pDC, 14, 400,TEXT("ʣ�ࣺ%d ��"),rc, RGB(240,240,0),(i==3?DT_RIGHT:DT_LEFT)|DT_END_ELLIPSIS);
//
//		DrawUserClock(pDC,m_ptClock[i].x,m_ptClock[i].y,22);
//		DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
//
//		DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,0);
//
//
//		//�йܱ�ʶ
//		if ( m_bUserTrustee[ i ] ) {
//			if ( i == ME_VIEW_CHAIR )
//				m_ImageUserTrustee.DrawImage( pDC, m_ptLand[ i ].x + 40, m_ptLand[ i ].y  );
//			else if ( i == 0 )
//				m_ImageUserTrustee.DrawImage( pDC, m_ptLand[ i ].x + 40, m_ptLand[ i ].y );
//			else if ( i == 1 )
//				m_ImageUserTrustee.DrawImage( pDC, m_ptAvatar[ i ].x , m_ptAvatar[ i ].y + 100);
//			else
//				m_ImageUserTrustee.DrawImage( pDC, m_ptAvatar[ i ].x , m_ptAvatar[ i ].y + 100);
//		}
//
//#endif
		//��������
		IClientUserItem *pUserItem =GetClientUserItem(i);

		//�滭�û�
		if (pUserItem!=NULL)
		{
			//�û�����
			CRect rc(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+NAME_WIDTH,m_ptNickName[i].y+16);
			CDFontEx::DrawText(this, pDC, 14, 400,pUserItem->GetNickName(),rc, RGB(240,240,0),(i==3?DT_RIGHT:DT_LEFT)|DT_END_ELLIPSIS);

			rc.SetRect(m_ptNickName[i].x,m_ptNickName[i].y+16,m_ptNickName[i].x+NAME_WIDTH,m_ptNickName[i].y+32);
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("ʣ�ࣺ%d ��"),m_bCardCount[i]);
			CDFontEx::DrawText(this, pDC, 14, 400,szBuffer,rc, RGB(240,240,0),(i==3?DT_RIGHT:DT_LEFT)|DT_END_ELLIPSIS);

			WORD wUserTimer=GetUserClock(i);
			//������Ϣ
			if (wUserTimer!=0) 
				DrawUserClock(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer);
			if (pUserItem->GetUserStatus()==US_READY)
				DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			if ( !CD2DEngine::GetD2DEngine() )
				DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pUserItem);

			//�йܱ�ʶ
			if ( m_bUserTrustee[ i ] ) {
			if ( i == ME_VIEW_CHAIR )
				m_ImageUserTrustee.DrawImage( pDC, m_ptLand[ i ].x + 40, m_ptLand[ i ].y  );
			else if ( i == 0 )
				m_ImageUserTrustee.DrawImage( pDC, m_ptLand[ i ].x + 40, m_ptLand[ i ].y );
			else if ( i == 1 )
				m_ImageUserTrustee.DrawImage( pDC, m_ptAvatar[ i ].x , m_ptAvatar[ i ].y + 100);
			else
				m_ImageUserTrustee.DrawImage( pDC, m_ptAvatar[ i ].x , m_ptAvatar[ i ].y + 100);
		}
		}
	}

//#ifdef DEBUG
//	for(int i=0;i<GAME_PLAYER;i++){
//		m_bPass[i] =true;
//	}
//
//	m_wLandUser = (m_wLandUser+1)%GAME_PLAYER;
//	//�˿˸߶�
//	CRect CardRect;
//	m_BackCardControl.GetWindowRect( &CardRect );
//	ScreenToClient( &CardRect );
//
//	//�滭��Ϣ
//	m_ImageLand.DrawImage(pDC,m_ptLand[m_wLandUser].x,m_ptLand[m_wLandUser].y);
//	_sntprintf(szBuffer,sizeof(szBuffer),TEXT("������%s"),TEXT("adadasdasdasdasdasdasdasdasda��˹���Ǵ���"));
//	CRect rc(m_nXBorder+10,CardRect.bottom+10,m_nXBorder+10+NAME_WIDTH,CardRect.bottom+10+16);
//	CDFontEx::DrawText(this, pDC, 14, 400,szBuffer,rc, RGB(240,240,0),DT_LEFT|DT_END_ELLIPSIS);
//
//	//�滭ͷ��
//	DrawUserAvatar(pDC,m_nXBorder+5,m_nYBorder+5,0);
//
//	//���Ʒ���
//	LONGLONG lScore=21346456464;
//	_sntprintf(szBuffer,sizeof(szBuffer),TEXT("������%I64d ��"),lScore);
//	rc.SetRect(m_nXBorder+10,CardRect.bottom+10+16,m_nXBorder+10+NAME_WIDTH,CardRect.bottom+10+32);
//	CDFontEx::DrawText(this, pDC, 14, 400,szBuffer,rc, RGB(240,240,0),DT_LEFT|DT_END_ELLIPSIS);
//
//#endif
	//�зֱ�־
	int nXImagePos,nImageWidth=m_ImageScore.GetWidth(),nImageHeight=m_ImageScore.GetHeight();
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if ((m_bScore[i]!=0)||(m_bPass[i]==true))
		{
			if ( m_bPass[i]==true ) nXImagePos=m_ImageScore.GetWidth()*4/5;
			else if ( m_bScore[i]==255 ) nXImagePos=m_ImageScore.GetWidth()*3/5;
			else nXImagePos=(m_bScore[i]-1)*m_ImageScore.GetWidth()/5;
			m_ImageScore.DrawImage(pDC,m_ptScore[i].x,m_ptScore[i].y,nImageWidth/5,nImageHeight,nXImagePos,0);
		}
	}

	//ׯ��λ��
	if (m_wLandUser!=INVALID_CHAIR)
	{
		//��ȡ�û�
		IClientUserItem *pUserItem=GetClientUserItem(m_wLandUser);
		if (pUserItem!=NULL)
		{
			//��������
			TCHAR szBuffer[64]=TEXT("");

			//�˿˸߶�
			CRect CardRect;
			m_BackCardControl.GetWindowRect( &CardRect );
			ScreenToClient( &CardRect );

			//�滭��Ϣ
			m_ImageLand.DrawImage(pDC,m_ptLand[m_wLandUser].x,m_ptLand[m_wLandUser].y);
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("������%s"),pUserItem->GetNickName());
			CRect rc(m_nXBorder+10,CardRect.bottom+10,m_nXBorder+10+NAME_WIDTH,CardRect.bottom+10+16);
			CDFontEx::DrawText(this, pDC, 14, 400,szBuffer,rc, RGB(240,240,0),DT_LEFT|DT_END_ELLIPSIS);
			//�滭ͷ��
			DrawUserAvatar(pDC,m_nXBorder+5,m_nYBorder+5,pUserItem);

			//���Ʒ���
			LONGLONG lScore=m_cbLandScore*m_lBaseScore;
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("������%I64d ��"),lScore);
			rc.SetRect(m_nXBorder+10,CardRect.bottom+10+16,m_nXBorder+10+NAME_WIDTH,CardRect.bottom+10+32);
			CDFontEx::DrawText(this, pDC, 14, 400,szBuffer,rc, RGB(240,240,0),DT_LEFT|DT_END_ELLIPSIS);
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

	//ը������
	if (m_wBombTime>1)
	{
		//�˿˸߶�
		CRect CardRect;
		m_BackCardControl.GetWindowRect( &CardRect );
		ScreenToClient( &CardRect );
		m_ImageBomb.TransDrawImage(pDC,m_nXBorder+5,50+CardRect.bottom,RGB(255,0,255));

		//ը������
		TCHAR szBuffer[64]=TEXT("");
		_sntprintf(szBuffer,sizeof(szBuffer),TEXT("ը��������%d"),m_wBombTime);
		DrawTextString( pDC, szBuffer, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), m_nXBorder+m_ImageBomb.GetWidth()+10,CardRect.bottom+60 ,14,400);
	}

	//������Ϣ
	if (m_bLandTitle==true)
	{
		DrawTextString( pDC, TEXT("���ȴ��û��з�"), RGB( 201,229,133 ), RGB( 0, 0, 0 ), nWidth/2,250 ,26,400,DT_CENTER);
	}

	//��ʷ����
	if (m_bShowScore==true)
	{

		m_ImageHistoryScore.BitBlt(pDC->m_hDC,nWidth-m_ImageHistoryScore.GetWidth()-40,27);

		//����λ��
		INT nYBenchmark=27;
		INT nXBenchmark=nWidth-m_ImageHistoryScore.GetWidth()-40;
		UINT nFormat=DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER;


		//�滭��Ϣ
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��ȡ�û�
			IClientUserItem *pUserItem = GetClientUserItem(i);

			if ((pUserItem!=NULL)&&m_pHistoryScore[i]!=NULL)
			{
				//λ�ü���
				CRect rcAccounts(nXBenchmark+2,nYBenchmark+24+i*19,nXBenchmark+73,nYBenchmark+42+i*19);
				CRect rcTurnScore(nXBenchmark+74,nYBenchmark+24+i*19,nXBenchmark+127,nYBenchmark+42+i*19);
				CRect rcCollectScore(nXBenchmark+128,nYBenchmark+24+i*19,nXBenchmark+197,nYBenchmark+42+i*19);

				//������Ϣ
				TCHAR szTurnScore[16]=TEXT(""),szCollectScore[16]=TEXT("");
				_sntprintf(szTurnScore,CountArray(szTurnScore),TEXT("%I64d"),m_pHistoryScore[i]->lTurnScore);
				_sntprintf(szCollectScore,CountArray(szCollectScore),TEXT("%I64d"),m_pHistoryScore[i]->lCollectScore);

				//�滭��Ϣ

				DrawTextString(pDC,pUserItem->GetNickName(),RGB(254,247,137),RGB(0,0,0),&rcAccounts,14,400,nFormat);	
				DrawTextString(pDC,szTurnScore,RGB(254,247,137),RGB(0,0,0),&rcTurnScore,14,400,nFormat);	
				DrawTextString(pDC,szCollectScore,RGB(254,247,137),RGB(0,0,0),&rcCollectScore,14,400,nFormat);	
			}
		}
	}
	return;
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

void CGameClientView::UpdateGameView(CRect  *rc)
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
	else ZeroMemory(m_bScore,sizeof(m_bScore));

	//���½���
	UpdateGameView(NULL);

	return;
}

//��Ϣ����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{

	switch( LOWORD(wParam) )
	{
	case IDC_START:
		{
			SendEngineMessage(IDM_START,0,0);
			return TRUE;
		}
	case IDC_OUT_CARD:
		{
			SendEngineMessage(IDM_OUT_CARD,1,1);
			return TRUE;
		}
	case IDC_PASS_CARD:
		{
			SendEngineMessage(IDM_PASS_CARD,1,1);
			return TRUE;
		}
	case IDC_ONE_SCORE:
		{
			SendEngineMessage(IDM_LAND_SCORE,1,1);
			return TRUE;
		}
	case IDC_TWO_SCORE:
		{
			SendEngineMessage(IDM_LAND_SCORE,2,2);
			return TRUE;
		}
	case IDC_THREE_SCORE:
		{
			SendEngineMessage(IDM_LAND_SCORE,3,3);
			return TRUE;
		}
	case IDC_AUTO_OUTCARD:
		{
			SendEngineMessage(IDM_AUTO_OUTCARD,0,0);
			return TRUE;
		}
	case IDC_GIVE_UP_SCORE:
		{
			SendEngineMessage(IDM_LAND_SCORE,255,255);
			return TRUE;
		}
	case IDC_AUTOPLAY_ON:
		{
			m_btAutoPlayOn.ShowWindow(SW_HIDE);
			m_btAutoPlayOff.ShowWindow(SW_SHOW);
			SendEngineMessage(IDM_LAND_AUTOPLAY,1,1);
			return TRUE;
		}
	case IDC_AUTOPLAY_OFF:
		{
			m_btAutoPlayOn.ShowWindow(SW_SHOW);
			m_btAutoPlayOff.ShowWindow(SW_HIDE);
			SendEngineMessage(IDM_LAND_AUTOPLAY,0,0);
			return TRUE;
		}
	case IDC_SORT_CARD:
		{
			SendEngineMessage(IDM_SORT_HAND_CARD,0,0);
			return TRUE;
		}
	case IDC_SCORE:
		{
			//���ñ���
			m_bShowScore=!m_bShowScore;

			//���ð�ť
			HINSTANCE hResInstance=AfxGetInstanceHandle();
			m_btScore.SetButtonImage((m_bShowScore==true)?IDB_BT_CLOSE_SCORE:IDB_BT_SHOW_SCORE,hResInstance,false,false);

			//���½���
			UpdateGameView(NULL);
			return TRUE;
		}

	}
	
	return __super::OnCommand(wParam,lParam);
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
		SetTimer(IDI_BOMB_EFFECT,50,NULL);
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
	//pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		//pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
		CDFontEx::DrawText(this, pDC, nWidth, nWeight,pszString,nXPos+nXExcursion[i],nYPos+nYExcursion[i], crFrame, nFormat);
	}

	//�滭����
	//pDC->SetTextColor(crText);
	//pDC->TextOut(nXPos,nYPos,pszString,nStringLength);
	CDFontEx::DrawText(this, pDC, nWidth, nWeight,pszString,nXPos,nYPos, crText, nFormat);

	return;
}
//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect,int nWidth, int nWeight,UINT nFormat)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	CRect rcDraw;
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		rcDraw.CopyRect(lpRect);
		rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
		CDFontEx::DrawText(this, pDC, nWidth, nWeight,pszString,&rcDraw, crFrame, nFormat);
	}

	//�滭����
	rcDraw.CopyRect(lpRect);
	CDFontEx::DrawText(this, pDC, nWidth, nWeight,pszString,&rcDraw, crText, nFormat);
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

