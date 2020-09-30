#include "StdAfx.h"
#include "GameLogic.h"
#include "GameClient.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

#define IDC_START						100								//��ʼ��ť
#define IDC_TRUSTEE_CONTROL				104								//�йܿ���
#define IDC_SCORE						105								//��ְ�ť

//������ʶ
#define IDI_BOMB_EFFECT					101								//������ʶ
#define IDI_DISC_EFFECT					102								//����Ч��
#define IDI_MOVE_CARD					103								//

//������Ŀ
#define BOMB_EFFECT_COUNT				12								//������Ŀ
#define DISC_EFFECT_COUNT				8								//����Ч��		

#define MOVE_STEP_COUNT					4								//�ƶ�����
#define TIME_MOVE_CARD					5								//�ƶ�����ʱʱ��
//////////////////////////////////////////////////////////////////////////////////




BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��־����
	m_bShowScore=false;
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;
	ZeroMemory(m_bListenStatus,sizeof(m_bListenStatus));
	ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));

	//��Ϸ����
	m_lCellScore=0L;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_cbFengQuan = GAME_PLAYER;

	//��������
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//����Ч��
	m_wDiscUser=INVALID_CHAIR;
	m_cbDiscFrameIndex=0;

	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));

	//���ƶ�������
	m_bEnableAnimate = true;
	m_bCardMoving = false;

	return;
}

//��������
CGameClientView::~CGameClientView()
{
}

//���ý���
VOID CGameClientView::ResetGameView()
{
	//��־����
	m_bShowScore=false;
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bHuangZhuang=false;
	ZeroMemory(m_bListenStatus,sizeof(m_bListenStatus));
	ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));

	//��Ϸ����
	m_lCellScore=0L;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_cbFengQuan = GAME_PLAYER;

	//��������
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//����Ч��
	m_wDiscUser=INVALID_CHAIR;
	m_cbDiscFrameIndex=0;

	//�û�״̬
	m_cbCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));

	//���ƶ�������
	m_bEnableAnimate = true;
	m_bCardMoving = false;

	//��������
	m_btStart.ShowWindow(SW_HIDE);
	m_ControlWnd.ShowWindow(SW_HIDE);
	m_btScore.ShowWindow(SW_HIDE);
	m_GameScore.RestorationData();
	m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,AfxGetInstanceHandle(),false,false);

	//���ÿؼ�
	m_btStusteeControl.EnableWindow(FALSE);

	//�˿�����
	m_UserCard.SetCardData(0,false);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayItem(false);
	m_HandCardControl.SetCardData(NULL,0,0);

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_HeapCard[i].SetHeapCardInfo(0,0);
		m_TableCard[i].SetCardData(NULL,0);
		m_DiscardCard[i].SetCardData(NULL,0);
		for( BYTE j = 0; j < CountArray(m_WeaveCard[i]); j++ )
			m_WeaveCard[i][j].SetCardData(NULL,0);
	}

	//���ٶ�ʱ��
	KillTimer( IDI_MOVE_CARD );
	KillTimer(IDI_DISC_EFFECT);
	KillTimer(IDI_BOMB_EFFECT);
	m_DrawSiceWnd.StopSicing(false);
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	int m_nXFace =50,m_nYFace=50,m_nXBorder=0,m_nYBorder=0,m_nXTimer=0,m_nYTimer=0;
	//��������
	m_ptAvatar[0].x=nWidth/2-m_nXFace;
	m_ptAvatar[0].y=5+m_nYBorder;
	m_ptNickName[0].x=nWidth/2+5;
	m_ptNickName[0].y=20+m_nYBorder;
	m_ptClock[0].x=nWidth/2-m_nXFace-m_nXTimer-2;
	m_ptClock[0].y=17+m_nYBorder;
	m_ptReady[0].x=nWidth/2-m_nXFace-110;
	m_ptReady[0].y=m_nYBorder+20;
	m_UserFlagPos[0].x=nWidth/2-m_nXFace-32;
	m_UserFlagPos[0].y=m_nYBorder+8;
	m_PointTrustee[0].x=nWidth/2-m_nXFace-69;
	m_PointTrustee[0].y=5+m_nYBorder;
	m_UserListenPos[0].x=nWidth/2-m_nXFace+5;
	m_UserListenPos[0].y=m_nYBorder+m_nYFace+20;

	m_ptAvatar[1].x=nWidth/2-m_nXFace;
	m_ptAvatar[1].y=nHeight-m_nYBorder-m_nYFace-5;
	m_ptNickName[1].x=nWidth/2+5;
	m_ptNickName[1].y=nHeight-m_nYBorder-m_nYFace+8;
	m_ptClock[1].x=nWidth/2-m_nXFace/2-m_nXTimer-2;
	m_ptClock[1].y=nHeight-m_nYBorder-m_nYTimer-8+40;
	m_ptReady[1].x=nWidth/2-m_nXFace-110;
	m_ptReady[1].y=nHeight-m_nYBorder-20;
	m_UserFlagPos[1].x=nWidth/2-m_nXFace-32;
	m_UserFlagPos[1].y=nHeight-m_nYBorder-30;
	m_PointTrustee[1].x=nWidth/2-m_nXFace-69;
	m_PointTrustee[1].y=nHeight-m_nYBorder-35;
	m_UserListenPos[1].x=nWidth/2-m_nXFace;
	m_UserListenPos[1].y=nHeight-m_nYBorder-140;

	//�û��˿�
	m_UserCard.SetControlPoint(nWidth/2-240,m_nYBorder+m_nYFace+10);
	m_HandCardControl.SetBenchmarkPos(nWidth/2+10,nHeight-m_nYFace-m_nYBorder-9,enXCenter,enYBottom);

	//�����˿�
	m_TableCard[0].SetControlPoint(nWidth/2+200,m_nYBorder+m_nYFace+10);
	m_TableCard[1].SetControlPoint(nWidth/2-200,nHeight-m_nYFace-m_nYBorder-63);

	//�ѻ��˿�
	int nXCenter=nWidth/2;
	int nYCenter=nHeight/2-20-20;
	m_HeapCard[0].SetControlPoint(nXCenter-289+44,nYCenter-215);
	m_HeapCard[1].SetControlPoint(nXCenter+290-55,nYCenter+212);

	//�����˿�
	m_DiscardCard[0].SetControlPoint(nXCenter+176,nYCenter-153);
	m_DiscardCard[1].SetControlPoint(nXCenter-184,nYCenter+160);

	//����˿�
	int nXControl = nWidth/2+255;
	int nYControl = m_nYBorder+m_nYFace+10;
	for( BYTE i = 0; i < CountArray(m_WeaveCard[0]); i++ )
	{
		m_WeaveCard[0][i].SetControlPoint(nXControl,nYControl);
		nXControl -= 105;
	}

	//����˿�
	nXControl = nWidth/2-295;
	nYControl = nHeight-m_nYFace-m_nYBorder-63;
	for( BYTE i = 0; i < CountArray(m_WeaveCard[1]); i++ )
	{
		m_WeaveCard[1][i].SetControlPoint(nXControl,nYControl);
		nXControl += 110;
	}

	//ɫ�Ӵ���
	CRect rcSice;
	m_DrawSiceWnd.GetWindowRect(&rcSice);
	m_DrawSiceWnd.SetBenchmarkPos((nWidth-rcSice.Width())/2,(nHeight-rcSice.Height())/2);
	m_DrawSiceWnd.MoveWindow((nWidth-rcSice.Width())/2,(nHeight-rcSice.Height())/2,rcSice.Width(),rcSice.Height());

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//�ƶ�����
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-m_nYBorder-175,0,0,uFlags);
	//�ƶ�����
	DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-m_nYBorder-rcButton.Height(),0,0,uFlags);

	//���ƴ���
	CRect rcControlWnd;
	m_ControlWnd.GetWindowRect(&rcControlWnd);
	DeferWindowPos(hDwp,m_ControlWnd,NULL,nWidth-rcControlWnd.Width()-30,nHeight-rcControlWnd.Height()-132,0,0,uFlags);

	//��ְ�ť
	CRect rcScore;
	m_btScore.GetWindowRect(&rcScore);
	DeferWindowPos(hDwp,m_btScore,NULL,nWidth-(rcScore.Width()+m_sizeHistory.cx)/2-5,0,0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	CRect rcGameScore;
	m_GameScore.GetWindowRect(&rcGameScore);
	CPoint ptPos( (nWidth-rcGameScore.Width())/2,(nHeight-rcGameScore.Height())*2/5-30 );
	ClientToScreen( &ptPos );
	m_GameScore.SetWindowPos( NULL,ptPos.x,ptPos.y,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOCOPYBITS );

	return;
}



//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//�滭����

	DrawViewImage(pDC,m_ImageBack,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageCenter,DRAW_MODE_CENTENT);

	//�滭�û�
	//pDC->SetTextColor(RGB(255,255,0));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
//#ifdef DEBUG
//		CRect rcInfo(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+120,m_ptNickName[i].y+16);
//		DrawTextString(pDC,TEXT("��˹�ٰ�˹�ٰ�˹�ٰ�˹��ʱ����˹�ٰ���˹�ٰ��� ��"),RGB(255,255,255),RGB(0,0,0),rcInfo,14,400,DT_END_ELLIPSIS);
//		DrawUserTimerEx(pDC,nWidth/2,nHeight/2,20);
//		m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2-m_ImageArrow.GetHeight()*2,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*(i==0?0:2),0);
//		DrawUserReadyEx(pDC,m_ptReady[i].x,m_ptReady[i].y);
//		DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,0);
//		m_ImageTrustee.DrawImage(pDC,m_PointTrustee[i].x,m_PointTrustee[i].y);
//#endif
		//��ȡ�û�
		IClientUserItem *pUserItem=GetClientUserItem(i);

		if (pUserItem!=NULL)
		{
			//�û�����
			CRect rcInfo(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+120,m_ptNickName[i].y+16);
			DrawTextString(pDC,pUserItem->GetNickName(),RGB(255,255,255),RGB(0,0,0),rcInfo,14,400,DT_END_ELLIPSIS);

			//������Ϣ
			WORD wUserTimer=GetUserClock(i);

			if ((wUserTimer!=0)&&(m_wCurrentUser!=INVALID_CHAIR))
			{
				DrawUserTimerEx(pDC,nWidth/2,nHeight/2,wUserTimer);
				if(m_wCurrentUser==0)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2-m_ImageArrow.GetHeight()*2,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*(m_wCurrentUser==0?0:2),0);
				else if(m_wCurrentUser==1)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2+m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*(m_wCurrentUser==0?0:2),0);
			}
			if((wUserTimer!=0)&&(m_wCurrentUser==INVALID_CHAIR))
			{
				DrawUserTimerEx(pDC,nWidth/2,nHeight/2,wUserTimer);
				if(i==0)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2-m_ImageArrow.GetHeight()*2,m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*(i==0?0:2),0);
				else if(i==1)
					m_ImageArrow.DrawImage(pDC,nWidth/2-15,nHeight/2+m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4,m_ImageArrow.GetHeight(),m_ImageArrow.GetWidth()/4*(i==0?0:2),0);
			}

			if (pUserItem->GetUserStatus()==US_READY) 
				DrawUserReadyEx(pDC,m_ptReady[i].x,m_ptReady[i].y);
			if ( !CD2DEngine::GetD2DEngine() )
				DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pUserItem);

			//�й�
			if(m_bTrustee[i])
			{
				m_ImageTrustee.DrawImage(pDC,m_PointTrustee[i].x,m_PointTrustee[i].y);
			}
		}

	}

//#ifdef DEBUG
//m_wBankerUser =( m_wBankerUser+1)%GAME_PLAYER;
//m_bOutCard =true;
//m_bHuangZhuang =true;
//#endif
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		int nImageWidth=m_ImageUserFlag.GetWidth()/4;
		int nImageHeight=m_ImageUserFlag.GetHeight();

		//�滭��־
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			WORD wIndex=(m_wBankerUser+GAME_PLAYER-i)%GAME_PLAYER;
			m_ImageUserFlag.DrawImage(pDC,m_UserFlagPos[wIndex].x,m_UserFlagPos[wIndex].y,nImageWidth,nImageHeight,nImageWidth*i,0);
			break;
		}
	}

	//�����˿�
	m_DiscardCard[0].DrawCardControl( pDC );
	m_DiscardCard[1].DrawCardControl( pDC );

	//�û��˿�
	m_UserCard.DrawCardControl(pDC);

	//�ѻ��˿�
	m_HeapCard[0].DrawCardControl(pDC);
	m_HeapCard[1].DrawCardControl(pDC);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_TableCard[i].DrawCardControl(pDC);
		for( BYTE j = 0; j < CountArray(m_WeaveCard[i]); j++ )
			m_WeaveCard[i][j].DrawCardControl(pDC,true);
	}

	//�����˿�
	m_HandCardControl.DrawCardControl(pDC);

	//���Ʊ�־
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if (m_bListenStatus[i]==true)
		{
			m_PngListenFlag.DrawImage( pDC,m_UserListenPos[i].x,m_UserListenPos[i].y );
		}
	}

	//������ʾ
	if (m_bOutCard==true)
	{
		m_ImageOutCard.DrawImage(pDC,(nWidth-m_ImageOutCard.GetWidth())/2,nHeight-165);
	}

	//�ȴ���ʾ
	if (m_bWaitOther==true)
	{
		m_ImageWait.DrawImage(pDC,(nWidth-m_ImageWait.GetWidth())/2,nHeight-165);
	}
	//��ׯ��־
	if (m_bHuangZhuang==true)
	{
		m_ImageHuangZhuang.DrawImage(pDC,(nWidth-m_ImageHuangZhuang.GetWidth())/2,nHeight/2);
	}

	//�û�״̬
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if ((m_wOutCardUser==i)||(m_cbUserAction[i]!=0))
		{
			//����λ��
			int nXPos=0,nYPos=0;
			switch (i)
			{
			case 0:	//����
				{
					nXPos=nWidth/2-32;
					nYPos=95;
					break;
				}
			case 1:	//����
				{
					nXPos=nWidth/2-32;
					nYPos=nHeight-220;
					break;
				}
			}

			//�滭����
			if (m_cbUserAction[i]!=WIK_NULL)
			{	
				//�滭����
				if (m_bBombEffect==true)
				{
					//�滭Ч��
					INT nItemHeight=m_ImageActionAni.GetHeight()/7;
					INT nItemWidth=m_ImageActionAni.GetWidth()/BOMB_EFFECT_COUNT;

					//�滭����
					int nYImagePos=0;
					if (m_cbUserAction[i]&WIK_PENG) nYImagePos=nItemHeight;
					else if (m_cbUserAction[i]&WIK_GANG) nYImagePos=nItemHeight*2;
					else if( m_cbUserAction[i]&WIK_LISTEN ) nYImagePos = nItemHeight*3;
					else if (m_cbUserAction[i]&WIK_CHI_HU) nYImagePos=nItemHeight*4;
					else if( m_cbUserAction[i]&WIK_REPLACE ) nYImagePos = nItemHeight*6;
					else nYImagePos=0;
					m_ImageActionAni.DrawImage(pDC,nXPos-nItemWidth/2+54,nYPos+42-nItemHeight/2,nItemWidth,nItemHeight,
						nItemWidth*(m_cbBombFrameIndex%BOMB_EFFECT_COUNT),nYImagePos,nItemWidth,nItemHeight);
				}
			}
			else
			{	
				//��������
				m_ImageActionBack.DrawImage(pDC,nXPos,nYPos);

				//�滭�˿�
				g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,m_cbCardData,nXPos+15,nYPos+13,false);
			}

		}
	}

	//����Ч��
	if(m_wDiscUser!=INVALID_CHAIR)
	{
		CSize SizeDisc(m_ImageDisc.GetWidth()/DISC_EFFECT_COUNT,m_ImageDisc.GetHeight());
		CPoint pt=m_DiscardCard[m_wDiscUser].GetLastCardPosition();
		pt.Offset(-SizeDisc.cx/2,-SizeDisc.cy);
		//�滭��Ϣ
		m_ImageDisc.DrawImage(pDC,pt.x,pt.y,SizeDisc.cx,SizeDisc.cy,
			m_cbDiscFrameIndex*SizeDisc.cx,0,SizeDisc.cx,SizeDisc.cy);
	}

	//���ƻ��ƶ���
	DrawMoveCardItem(pDC);

	//��ʷ����
	if (m_bShowScore==true)
	{


		//����λ��
		INT nYBenchmark=27;
		INT nXBenchmark=nWidth-m_ImageHistoryScore.GetWidth()-5;
		m_ImageHistoryScore.BitBlt(pDC->m_hDC,nWidth-m_ImageHistoryScore.GetWidth()-5,27);
		UINT nFormat=DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER;

		//�滭����
		pDC->SetTextColor(RGB(254,247,137));

		//�滭��Ϣ
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��ȡ�û�
			IClientUserItem *pUserItem=GetClientUserItem(i);
			if ((pUserItem==NULL)||(m_pHistoryScore[i]==NULL)) continue;

			//λ�ü���
			CRect rcAccounts(nXBenchmark+2,nYBenchmark+24+i*19,nXBenchmark+73,nYBenchmark+42+i*19);
			CRect rcTurnScore(nXBenchmark+74,nYBenchmark+24+i*19,nXBenchmark+127,nYBenchmark+42+i*19);
			CRect rcCollectScore(nXBenchmark+128,nYBenchmark+24+i*19,nXBenchmark+197,nYBenchmark+42+i*19);

			//������Ϣ
			CString szTurnScore,szCollectScore;
			szTurnScore.Format(TEXT("%I64d"),m_pHistoryScore[i]->lTurnScore);
			szCollectScore.Format(TEXT("%I64d"),m_pHistoryScore[i]->lCollectScore);

			//�滭��Ϣ
			CString strAccounts;
			strAccounts.Format(TEXT("%s"),pUserItem->GetNickName());
			CDFontEx::DrawText(this, pDC, 14, 400,strAccounts,&rcAccounts, RGB(254,247,137), DT_END_ELLIPSIS|DT_LEFT);
			CDFontEx::DrawText(this, pDC, 14, 400,szTurnScore,&rcTurnScore, RGB(254,247,137), DT_END_ELLIPSIS|DT_LEFT);
			CDFontEx::DrawText(this, pDC, 14, 400,szCollectScore,&rcCollectScore, RGB(254,247,137), DT_END_ELLIPSIS|DT_LEFT);

			//pDC->DrawText(strAccounts,lstrlen(strAccounts),&rcAccounts,nFormat);
			//pDC->DrawText(szTurnScore,lstrlen(szTurnScore),&rcTurnScore,nFormat);
			//pDC->DrawText(szCollectScore,lstrlen(szCollectScore),&rcCollectScore,nFormat);
		}
	}
	return;
}

//�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{

	switch (LOWORD(wParam))
	{
	case IDC_START:			
		{
			//������Ϣ
		SendEngineMessage(IDM_START,0,0);
			return TRUE;
		}
	case IDC_TRUSTEE_CONTROL:			
		{
			//������Ϣ
		SendEngineMessage(IDM_TRUSTEE_CONTROL,0,0);
			return TRUE;
		}
	case IDC_SCORE:			
		{
			//������Ϣ
			//���ñ���
			m_bShowScore=!m_bShowScore;

			//���ð�ť
			HINSTANCE hResInstance=AfxGetInstanceHandle();
			m_btScore.SetButtonImage((m_bShowScore==true)?IDB_BT_CLOSE_SCORE:IDB_BT_SHOW_SCORE,hResInstance,false,false);

			//���½���
			InvalidGameView(0,0,0,0 );

			return TRUE;
		}

	}

	return __super::OnCommand(wParam, lParam);
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//������Դ
	g_CardResource.LoadResource(this);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageCenter.LoadFromResource(this,hInstance,IDB_VIEW_CENTER);
	
	m_ImageHistoryScore.LoadFromResource(this,hInstance,IDB_HISTORY_SCORE);
	m_ImageBack.LoadFromResource(this,hInstance,IDB_VIEW_BACK);
	m_ImageUserFlag.LoadImage( this,hInstance,TEXT("USER_FLAG") );
	m_ImageOutCard.LoadImage( this,hInstance,TEXT("OUT_CARD_TIP") );
	m_ImageActionBack.LoadImage( this,hInstance,TEXT("ACTION_BACK") );
	m_ImageTrustee.LoadImage(this,hInstance,TEXT("TRUSTEE"));
	m_ImageActionAni.LoadImage(this,hInstance,TEXT("ActionAni"));
	m_ImageDisc.LoadImage(this,hInstance,TEXT("DISC"));
	m_ImageArrow.LoadImage(this,hInstance,TEXT("ARROW"));
	m_ImageWait.LoadImage( this,hInstance,TEXT("WAIT_TIP") );
	m_PngFengQuan.LoadImage( this,hInstance,TEXT("DRAW_WIND") );
	m_PngListenFlag.LoadImage( this,hInstance,TEXT("LISTEN_FLAG") );
	m_ImageHuangZhuang.LoadImage(this,hInstance,TEXT("HUANG_ZHUANG"));
	//��ȡ��С
	m_sizeHistory.cx=m_ImageHistoryScore.GetWidth();
	m_sizeHistory.cy=m_ImageHistoryScore.GetHeight();


	ImageTimeBack.LoadImage(this,hInstance,TEXT("TIME_BACK"));
	ImageTimeNumber.LoadImage(this,hInstance,TEXT("TIME_NUMBER"));
	ImageUserReady.LoadImage(this,hInstance,TEXT("USER_READY"));


	//��������
	enDirection Direction[]={Direction_North,Direction_South};

	m_HandCardControl.LoadResource(this);
	//���ÿؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//�û��˿�
		m_HeapCard[i].SetDirection(Direction[i]);
		m_TableCard[i].SetDirection(Direction[i]);
		m_DiscardCard[i].SetDirection(Direction[i]);

		//����˿�
		for( BYTE j = 0; j < CountArray(m_WeaveCard[i]); j++ )
		{
			m_WeaveCard[i][j].SetDisplayItem(true);
			m_WeaveCard[i][j].SetDirection(Direction[i]);
			m_WeaveCard[i][j].LoadResource(this);
		}
	}

	//���ÿؼ�
	m_UserCard.SetDirection(Direction_North);

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_ControlWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,10,NULL);
	m_DrawSiceWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,11,NULL);
	m_DrawSiceWnd.SetWindowSink(this);
	m_HandCardControl.SetWindowSink(this);
	m_ControlWnd.SetWindowSink(this);
	m_GameScore.Create( IDD_GAME_SCORE,this );

	//�����ؼ�
	m_btStart.Create(NULL,WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_START);
	m_btStart.SetButtonImage(IDB_BT_START,AfxGetInstanceHandle(),false,false);
	m_btScore.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SCORE);
	m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,AfxGetInstanceHandle(),false,false);

	//�йܰ�ť
	m_btStusteeControl.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_TRUSTEE_CONTROL);
	m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);

	return 0;
}

//�滭�˿˶���
void CGameClientView::DrawMoveCardItem( CDC *pDC )
{
	if( m_bCardMoving )
	{
		int nXDraw = m_MoveCardItem.ptFrom.x, nYDraw = m_MoveCardItem.ptFrom.y;
		switch( m_enMoveDirection )
		{
		case Direction_South:
		case Direction_North:
			{
				//����
				if( m_MoveCardItem.cbCardData != 0 )
				{
					if( m_enMoveDirection == Direction_South )
						g_CardResource.m_ImageTableBottom.DrawCardItem( pDC,m_MoveCardItem.cbCardData,nXDraw,nYDraw,false );
					else g_CardResource.m_ImageTableTop.DrawCardItem( pDC,m_MoveCardItem.cbCardData,nXDraw,nYDraw,false );
				}
				//����
				else
				{
					if( m_MoveCardItem.cbCardCount == 1 )
					{
						g_CardResource.m_ImageHeapSingleH.DrawImage( pDC,nXDraw,nYDraw );
					}
					else
					{
						ASSERT( m_MoveCardItem.cbCardCount == 4 );
						g_CardResource.m_ImageHeapDoubleH.DrawImage( pDC,nXDraw,nYDraw );
						nXDraw += g_CardResource.m_ImageHeapDoubleH.GetWidth();
						g_CardResource.m_ImageHeapDoubleH.DrawImage( pDC,nXDraw,nYDraw );
					}
				}
			}
			break;
		default:
			ASSERT(FALSE);
		}
	}
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
	InvalidGameView(0,0,0,0);

	return true;
}

//��������
void CGameClientView::SetCellScore(LONGLONG lCellScore)
{
	//�����˿�
	if (lCellScore!=m_lCellScore)
	{
		//���ñ���
		m_lCellScore=lCellScore;

		//���½���
	InvalidGameView(0,0,0,0);
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
	InvalidGameView(0,0,0,0);
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
	InvalidGameView(0,0,0,0);
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
	InvalidGameView(0,0,0,0);

	return;
}

//������Ϣ
void CGameClientView::SetOutCardInfo(WORD wViewChairID, BYTE cbCardData)
{
	//���ñ���
	m_cbCardData=cbCardData;
	m_wOutCardUser=wViewChairID;

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//������Ϣ
void CGameClientView::SetUserAction(WORD wViewChairID, BYTE bUserAction)
{
	//���ñ���
	if (wViewChairID<GAME_PLAYER)
	{
		m_cbUserAction[wViewChairID]=bUserAction;
		SetBombEffect(true);
	}
	else 
	{
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		if(m_bBombEffect)
			SetBombEffect(false);
	}

	//���½���
	InvalidGameView(0,0,0,0);

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
	InvalidGameView(0,0,0,0);
		}
	}

	return true;
}

//�����û�
void CGameClientView::SetDiscUser(WORD wDiscUser)
{
	if(m_wDiscUser != wDiscUser)
	{
		//���±���
		m_wDiscUser=wDiscUser;

		if( m_wDiscUser != INVALID_CHAIR )
			SetTimer( IDI_DISC_EFFECT,250,NULL );
		else KillTimer( IDI_DISC_EFFECT );

		//���½���
	InvalidGameView(0,0,0,0);
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
	InvalidGameView(0,0,0,0);
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
	InvalidGameView(0,0,0,0);
	}
	return;
}

//���Ʊ�־
void CGameClientView::SetUserListenStatus(WORD wViewChairID, bool bListenStatus)
{
	//���ñ���
	if (wViewChairID<GAME_PLAYER)
	{
		SetBombEffect(true);
		m_cbUserAction[wViewChairID]=WIK_LISTEN;
		m_bListenStatus[wViewChairID]=bListenStatus;
	}
	else 
		ZeroMemory(m_bListenStatus,sizeof(m_bListenStatus));

	//���½���
	InvalidGameView(0,0,0,0);

	return;
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
	//pDC->SetTextColor(crFrame);
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

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�������
	CRect rcRePaint;
	bool bHandle=m_HandCardControl.OnEventSetCursor(MousePoint,rcRePaint);

	//�ػ�����
	if (rcRePaint.IsRectEmpty()==false)
		InvalidGameView(rcRePaint.left,rcRePaint.top,rcRePaint.Width(),rcRePaint.Height());

	//������
	if (bHandle==false)
		return __super::OnSetCursor(pWnd,nHitTest,uMessage);

	return TRUE;
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//�˿��¼�
	m_HandCardControl.OnEventLeftHitCard();

	//test
	//BYTE byCard[] = {
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,					
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09
	//};
	//BYTE byCardCount = 14;
	//static int n = 0;
	//if( ++n==1 )
	//{
	//	m_HandCardControl.SetDisplayItem(true);
	//	m_HandCardControl.SetPositively(true);
	//	m_HandCardControl.SetCardData(byCard,MAX_COUNT-1-3-3,byCard[MAX_COUNT-1]);
	//	for( BYTE i = 0; i < GAME_PLAYER; i++ )
	//	{
	//		m_HeapCard[i].SetHeapCardInfo(27,0,2);
	//		//m_HeapCard[i].SetCardData( 0,0x01 );
	//		//m_HeapCard[i].SetCardEmpty(5,4 );
	//		m_TableCard[i].SetCardData(byCard,byCardCount-3-3-3-3);
	//		m_DiscardCard[i].SetCardData(byCard,27);
	//		m_WeaveCard[i][0].SetCardData(byCard,3);
	//		m_WeaveCard[i][1].SetCardData( byCard,3 );
	//		m_WeaveCard[i][2].SetCardData( byCard,3 );
	//		m_WeaveCard[i][3].SetCardData( byCard,3 );
	//		m_WeaveCard[i][0].SetDirectionCardPos( 0 );
	//		m_WeaveCard[i][1].SetDirectionCardPos( 1 );
	//		m_WeaveCard[i][2].SetDirectionCardPos( 2 );
	//		m_WeaveCard[i][3].SetDirectionCardPos( 3 );
	//		if( i < 3 )
	//			m_UserCard.SetCardData(MAX_COUNT-1-3-3,true);
	//		m_bTrustee[i] = true;
	//		m_cbUserAction[i] = WIK_REPLACE;
	//		m_bListenStatus[i] = true;
	//	}
	//	m_bWaitOther = true;
	//	m_bHuangZhuang = true;
	//	m_bOutCard = true;
	//	m_wOutCardUser = 3;
	//	m_cbCardData = 0x01;
	//	m_cbUserAction[1] = WIK_NULL;
	//	SetDiscUser( 1 );
	//	SetBombEffect(true);
	//	tagScoreInfo ScoreInfo;
	//	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	//	tagWeaveInfo WeaveInfo;
	//	ZeroMemory(&WeaveInfo,sizeof(WeaveInfo));
	//	ScoreInfo.wChiHuUser = 0;
	//	ScoreInfo.wProvideUser = 0;
	//	ScoreInfo.cbFanCount = 88;
	//	ScoreInfo.cbCardCount = 13;
	//	for( WORD i = 0; i < GAME_PLAYER; i++ )
	//	{
	//		lstrcpy( ScoreInfo.szUserName[i],TEXT("SSSSDFSDFSDF") );
	//		ScoreInfo.lGameScore[i] = 123L*((i%2)?-1:1);
	//		ScoreInfo.lMingGang[i] = 123;
	//		ScoreInfo.lLangGang[i] = 123;
	//	}
	////}

	//ScoreInfo.lCellScore = 1;

	//	CChiHuRight chr;
	//	ScoreInfo.cbHuaCardCount = 3;
	//	ScoreInfo.cbFanCount = 23;
	//	m_GameScore.SetScoreInfo(ScoreInfo,WeaveInfo,chr);
	//m_GameScore.ShowWindow( SW_SHOW );
	//SetHuangZhuang(true);
	//m_cbFengQuan = 0;
	//m_btStart.ShowWindow( SW_SHOW );

	//	tagSelectCardInfo sci[MAX_COUNT];
	//	ZeroMemory( sci,sizeof(sci) );
	//	sci[0].cbActionCard = 0x03;
	//	sci[0].wActionMask = WIK_LEFT;
	//	sci[0].cbCardCount = 2;
	//	sci[0].cbCardData[0] = 0x04;
	//	sci[0].cbCardData[1] = 0x05;
	//	sci[1].cbActionCard = 0x03;
	//	sci[1].wActionMask = WIK_CENTER;
	//	sci[1].cbCardCount = 2;
	//	sci[1].cbCardData[0] = 0x02;
	//	sci[1].cbCardData[1] = 0x04;
	//	sci[2].cbActionCard = 0x03;
	//	sci[2].wActionMask = WIK_RIGHT;
	//	sci[2].cbCardCount = 2;
	//	sci[2].cbCardData[0] = 0x01;
	//	sci[2].cbCardData[1] = 0x02;
	//	m_HandCardControl.OnEventUserAction( sci,3 );
	//	m_ControlWnd.SetControlInfo( WIK_PENG|WIK_CHI_HU|WIK_LEFT|WIK_LISTEN|WIK_GANG );

	//m_DrawSiceWnd.SetSiceInfo( GetDC(),250,0,MAKEWORD(2,6) );
	//m_DrawSiceWnd.ShowWindow( SW_SHOW );
	//}
	//else
	//{
	//m_HeapCard[0].SetHeapCardInfo(0,27,2);
	//		tagMoveCardItem mci;
	//		mci.cbCardCount = 1;
	//		mci.cbCardData = 1;
	//		mci.wViewChairId = 0;
	//		mci.ptFrom = m_HeapCard[0].GetDispatchCardPos( false, 0,27,2);
	//		mci.ptTo = CPoint(500,500);
	//		//m_HeapCard[0].SetCardEmpty( 6+1,4 );
	//		OnMoveCardItem( mci );
	//}
	//end test

	return;
}

//��ʼ��ť
//void CGameClientView::OnStart()
//{
//	//������Ϣ
//	SendEngineMessage(IDM_START,0,0);
//
//	return;
//}
//
////�Ϲܿ���
//void CGameClientView::OnStusteeControl()
//{
//	SendEngineMessage(IDM_TRUSTEE_CONTROL,0,0);
//	return;
//}

//���ְ�ť
//void CGameClientView::OnBnClickedScore()
//{
//	//���ñ���
//	m_bShowScore=!m_bShowScore;
//
//	//���ð�ť
//	HINSTANCE hResInstance=AfxGetInstanceHandle();
//	m_btScore.SetButtonImage((m_bShowScore==true)?IDB_BT_CLOSE_SCORE:IDB_BT_SHOW_SCORE,hResInstance,false,false);
//
//	//���½���
//	InvalidGameView(0,0,0,0 );
//
//	return;
//}

//��ʱ��
void CGameClientView::OnTimer(UINT nIDEvent)
{
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
		InvalidGameView(0,0,0,0 );

		return;
	}
	else if (nIDEvent==IDI_DISC_EFFECT)
	{
		//���ñ���
		if ((m_cbDiscFrameIndex+1)>=DISC_EFFECT_COUNT)
		{
			m_cbDiscFrameIndex=0;
		}
		else m_cbDiscFrameIndex++;

		//���½���
		InvalidGameView(0,0,0,0 );

		return;

	}
	else if( nIDEvent == IDI_MOVE_CARD )
	{
		//�жϵ�ǰ�����Ƿ����
		if( m_nStepCount == 0 )
		{
			//ɾ����ʱ��
			KillTimer( IDI_MOVE_CARD );

			//��FALSE
			m_bCardMoving = false;

			//���Ͷ���������Ϣ
			PostEngineMessage( IDM_MOVE_CARD_FINISH ,0,0);

			//������ͼ
			InvalidGameView(0,0,0,0 );
		}
		//������û����
		else
		{
			//���ٲ���
			m_nStepCount--;

			//���û滭����
			CSize size(m_MoveCardItem.cbCardCount==4?80:60,m_MoveCardItem.cbCardCount==4?80:60);
			m_rcCardMove = CRect(m_MoveCardItem.ptFrom,size);

			//�ƶ�
			m_MoveCardItem.ptFrom.x += m_nXStep;
			m_MoveCardItem.ptFrom.y += m_nYStep;

			//���ϻ滭����
			CSize size2( m_MoveCardItem.cbCardCount==4?80:60,m_MoveCardItem.cbCardCount==4?80:60 );
			CRect rcDraw( m_MoveCardItem.ptFrom,size2 );
			m_rcCardMove.UnionRect( &m_rcCardMove,&rcDraw );

			//������ͼ
			InvalidGameView( m_rcCardMove.left,m_rcCardMove.top,m_rcCardMove.Width(),m_rcCardMove.Height() );

		}
		return;
	}

	__super::OnTimer(nIDEvent);
}

//���÷�Ȧ
void CGameClientView::SetFengQuan( BYTE cbFengQuan )
{
	if( m_cbFengQuan != cbFengQuan )
	{
		m_cbFengQuan = cbFengQuan;
		InvalidGameView(0,0,0,0 );
	}
}

//������
void CGameClientView::EnableAnimate( bool bAnimate )
{
	m_bEnableAnimate = bAnimate;

	if( !m_bEnableAnimate && m_bCardMoving )
		StopMoveCard();
}

//��������,�����Ͷ���������Ϣ
bool CGameClientView::StopMoveCard()
{
	//�ж�
	if( !m_bCardMoving ) return false;

	KillTimer( IDI_MOVE_CARD );

	m_bCardMoving = false;

	//������Ϣ
	SendEngineMessage( IDM_MOVE_CARD_FINISH,0,0 );

	InvalidGameView(0,0,0,0 );

	return true;
}

//�˿˶���
void CGameClientView::OnMoveCardItem( const tagMoveCardItem &MoveCardItem ,bool bdebug )
{
	if( m_bCardMoving ) return;

	//�Ƿ�������
	if( !m_bEnableAnimate )
	{
		//������Ϣ
		SendEngineMessage( IDM_MOVE_CARD_FINISH ,0,0 );
		return;
	}

	m_bCardMoving = true;
	//���ñ���
	m_MoveCardItem = MoveCardItem;
	m_nStepCount = MOVE_STEP_COUNT;
	m_nXStep = (MoveCardItem.ptTo.x-MoveCardItem.ptFrom.x)/m_nStepCount;
	m_nYStep = (MoveCardItem.ptTo.y-MoveCardItem.ptFrom.y)/m_nStepCount;
	switch( MoveCardItem.wViewChairId )
	{
	case 0:
		m_enMoveDirection = Direction_North;
		break;
	case 1:
		m_enMoveDirection = Direction_South;
		break;
	}

	//���û滭����
	CSize size(MoveCardItem.cbCardCount==4?80:50,MoveCardItem.cbCardCount==4?80:50);
	CRect rcDraw(MoveCardItem.ptFrom,size);
	m_rcCardMove = rcDraw;

	//���ö�ʱ��
	SetTimer( IDI_MOVE_CARD,/*bdebug?1000:*/TIME_MOVE_CARD,NULL );
	InvalidGameView( m_rcCardMove.left,m_rcCardMove.top,m_rcCardMove.Width(),m_rcCardMove.Height() );
}

//�滭ʱ��
void CGameClientView::DrawUserTimerEx(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{

	//��ȡ����
	const INT nNumberHeight=ImageTimeNumber.GetHeight();
	const INT nNumberWidth=ImageTimeNumber.GetWidth()/10;

	//������Ŀ
	INT lNumberCount=0;
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
	for (INT i=0;i<lNumberCount;i++)
	{
		//�滭����
		WORD wCellNumber=wTime%10;
		ImageTimeNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

		//���ñ���
		wTime/=10;
		nXDrawPos-=nNumberWidth;
	}

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

//////////////////////////////////////////////////////////////////////////////////
