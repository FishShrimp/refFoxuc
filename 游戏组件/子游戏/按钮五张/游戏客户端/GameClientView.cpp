#include "StdAfx.h"
#include "Resource.h"
#include "GameClientEngine.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ť��ʶ
#define IDC_START						100								//��ʼ��ť
#define IDC_FOLLOW						103								//��ע��ť
#define IDC_GIVE_UP						104								//������ť
#define IDC_SHOW_HAND					105								//�����ť
#define IDC_ADD_TIMES1					106								//��ְ�ť
#define IDC_ADD_TIMES2					107								//��ְ�ť
#define IDC_ADD_TIMES3					108								//��ְ�ť

//��ע��ť
#define IDC_ADD_SCORE					113								//��ע��ť

//��ʱ����ʶ
#define IDI_SEND_CARD					100								//���ƶ�ʱ��
#define IDI_SHOW_CARD					101								//
#define IDI_USER_SHOW_HAND				102								//

#define SPEED_MOVE_CARD					30								//

#define TIME_USER_SHOW_HAND				3000							//

//���ƶ���
#define SEND_STEP_COUNT					6								//����
#define SPEED_SEND_CARD					30								//�����ٶ�

#define SHOW_CARD_ANIMATE												//���ÿ��ƶ���,���Ҫȡ������,��ע�͵�

//////////////////////////////////////////////////////////////////////////
																			  
BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)							  
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��������
	m_SendCardPos.SetPoint(0,0);
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = SEND_STEP_COUNT;
	m_nXStep = 0;
	m_nYStep = 0;

	//���ݱ���
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	m_lCellScore = 0L;
	m_bUserShowHand = false;

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageCard.LoadFromResource(hInstance,IDB_CARD);
	m_ImageViewBack.LoadFromResource(hInstance,IDB_VIEW_BACK);
	m_ImageViewFill.LoadFromResource(hInstance,IDB_VIEW_FILL);
	m_PngNumber.LoadImage(hInstance,TEXT("NUMBER"));
	m_PngShowHand.LoadImage( hInstance,TEXT("SHOW_HAND") );


	return;
}

//��������
CGameClientView::~CGameClientView()
{
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,10);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_START);
	m_btFollow.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_FOLLOW);
	m_btGiveUp.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_GIVE_UP);
	m_btShowHand.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_SHOW_HAND);
	m_btAddTimes1.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_ADD_TIMES1);
	m_btAddTimes2.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_ADD_TIMES2);
	m_btAddTimes3.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,rcCreate,this,IDC_ADD_TIMES3);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_BT_START,hInstance,false,false);
	m_btFollow.SetButtonImage(IDB_BT_FOLLOW,hInstance,false,false);
	m_btGiveUp.SetButtonImage(IDB_BT_FANGQI,hInstance,false,false);
	m_btShowHand.SetButtonImage(IDB_BT_SOHA,hInstance,false,false);
	m_btAddTimes1.SetButtonImage(IDB_BT_ADD_SCORE,hInstance,false,false);
	m_btAddTimes2.SetButtonImage(IDB_BT_ADD_SCORE,hInstance,false,false);
	m_btAddTimes3.SetButtonImage(IDB_BT_ADD_SCORE,hInstance,false,false);

	//
	m_PlayerJeton[GAME_PLAYER].SetDrawMode(enDrawMode_Rand);

	//test
	/*BYTE byCard[] = 
	{
		0x01,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,							//���� A - K
		0x11,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,							//÷�� A - K
		0x21,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,							//���� A - K
		0x31,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,
	};
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		m_CardControl[i].SetCardData(byCard,5);
		m_PlayerJeton[i].SetScore(123456L);
		m_lTableScore[i] = 10000L;
	}
	m_PlayerJeton[GAME_PLAYER].SetScore(123456L);
	//m_btStart.ShowWindow(SW_SHOW);
	m_btFollow.ShowWindow(SW_SHOW);
	m_btGiveUp.ShowWindow(SW_SHOW);
	m_btShowHand.ShowWindow(SW_SHOW);
	for( i = 0; i < GAME_PLAYER; i++ )
	{
		m_ScoreView.SetGameScore(i,TEXT("S"),(i%2!=0)?1L:-1L);
		m_ScoreView.SetGameTax(i,i%2==0?-1:1);
	}
	m_ScoreView.SetGameScore(2,TEXT("sssssssssssS"),0);
	m_ScoreView.ShowWindow(SW_SHOW);
	m_lCellScore = 93L;*/
	//end test

	return 0;
}

//�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_START:				//��ʼ��ť
		{
			//������Ϣ
			SendEngineMessage(IDM_START,0,0);
			return TRUE;
		}
	case IDC_FOLLOW:			//��ע��ť
		{
			//������Ϣ
			SendEngineMessage(IDM_FOLLOW,0,0);
			return TRUE;
		}
	case IDC_GIVE_UP:			//������ť
		{
			//������Ϣ
			SendEngineMessage(IDM_GIVE_UP,0,0);
			return TRUE;
		}
	case IDC_SHOW_HAND:			//�����ť
		{
			//������Ϣ
			SendEngineMessage(IDM_SHOW_HAND,0,0);
			return TRUE;
		}
	case IDC_ADD_TIMES1:			//��ע��ť
	case IDC_ADD_TIMES2:
	case IDC_ADD_TIMES3:
		{
			SendEngineMessage(IDM_ADD_SCORE,0,LOWORD(wParam)-IDC_ADD_TIMES1+1);
			return TRUE;
		}
	}

	return __super::OnCommand(wParam, lParam);
}

//���ý���
VOID CGameClientView::ResetGameView()
{
	//��������
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = 0;
	m_nXStep = 0;
	m_nYStep = 0;
	m_SendCardItemArray.RemoveAll();

	//���ݱ���
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	m_lCellScore = 0L;
	m_bUserShowHand = false;

	//���ó���
	for (WORD i=0;i<GAME_PLAYER+1;i++) 
	{
		m_PlayerJeton[i].SetScore(0L);
	}

	//ɾ����ʱ��
	KillTimer(IDI_SEND_CARD);

	//���ذ�ť
	m_btStart.ShowWindow(SW_HIDE);
	m_btGiveUp.ShowWindow(SW_HIDE);
	m_btFollow.ShowWindow(SW_HIDE);
	m_btShowHand.ShowWindow(SW_HIDE);

	//���ؿؼ�
	m_ScoreView.ShowWindow(SW_HIDE);
	for (WORD i=0;i<GAME_PLAYER;i++) m_CardControl[i].SetCardData(NULL,0);

	return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(int nWidth, int nHeight)
{
	//��������
	m_ptAvatar[0].x=nWidth/2+330-40;
	m_ptAvatar[0].y=nHeight/2-232;
	m_ptNickName[0].x=m_ptAvatar[0].x-5;
	m_ptNickName[0].y=m_ptAvatar[0].y+12;
	m_ptClock[0].x=m_ptNickName[0].x - 100;
	m_ptClock[0].y=m_ptNickName[0].y + 10;
	m_ptReady[0].x=m_ptNickName[0].x - 120;
	m_ptReady[0].y=m_ptNickName[0].y + 12;
	m_ptTableScore[0].x = nWidth/2+158;
	m_ptTableScore[0].y = nHeight/2-200;

	//��������
	m_ptAvatar[1].x=nWidth/2+330-40;
	m_ptAvatar[1].y=nHeight/2-40;
	m_ptNickName[1].x=m_ptAvatar[1].x-5;
	m_ptNickName[1].y=m_ptAvatar[1].y+12;
	m_ptClock[1].x=m_ptNickName[1].x - 100;
	m_ptClock[1].y=m_ptNickName[1].y ;
	m_ptReady[1].x=m_ptNickName[1].x - 120;
	m_ptReady[1].y=m_ptNickName[1].y + 10;
	m_ptTableScore[1].x = nWidth/2+145;
	m_ptTableScore[1].y = nHeight/2+25;

	//��������
	m_ptAvatar[2].x=nWidth/2-20;
	m_ptAvatar[2].y=nHeight/2+215;
	m_ptNickName[2].x=m_ptAvatar[2].x+50;
	m_ptNickName[2].y=m_ptAvatar[2].y+12;
	m_ptClock[2].x=m_ptAvatar[2].x-40;
	m_ptClock[2].y=m_ptAvatar[2].y+40/2;
	m_ptReady[2].x=nWidth/2;
	m_ptReady[2].y=nHeight/2+53;
	m_ptTableScore[2].x = nWidth/2;
	m_ptTableScore[2].y = nHeight/2+75;

	//��������
	m_ptAvatar[3].x=nWidth/2-340;
	m_ptAvatar[3].y=nHeight/2-40;
	m_ptNickName[3].x=m_ptAvatar[3].x+50;
	m_ptNickName[3].y=m_ptAvatar[3].y+12;
	m_ptClock[3].x=m_ptNickName[3].x + 100;
	m_ptClock[3].y=m_ptNickName[3].y ;
	m_ptReady[3].x=m_ptNickName[3].x + 120;
	m_ptReady[3].y=m_ptNickName[3].y + 10;
	m_ptTableScore[3].x = nWidth/2-153;
	m_ptTableScore[3].y = nHeight/2+25;

	//��������
	m_ptAvatar[4].x=nWidth/2-340;
	m_ptAvatar[4].y=nHeight/2-232;
	m_ptNickName[4].x=m_ptAvatar[4].x+50;
	m_ptNickName[4].y=m_ptAvatar[4].y+12;
	m_ptClock[4].x=m_ptNickName[4].x + 100;
	m_ptClock[4].y=m_ptNickName[4].y + 10;
	m_ptReady[4].x=m_ptNickName[4].x + 120;
	m_ptReady[4].y=m_ptNickName[4].y + 12;
	m_ptTableScore[4].x = nWidth/2-158;
	m_ptTableScore[4].y = nHeight/2-200;

	//�û��˿�
	m_CardControl[0].SetBenchmarkPos(nWidth/2+258,nHeight/2-183,enXCenter,enYTop);
	m_CardControl[1].SetBenchmarkPos(nWidth/2+225,nHeight/2+10,enXCenter,enYTop);
	m_CardControl[2].SetBenchmarkPos(nWidth/2,nHeight/2+200,enXCenter,enYBottom);
	m_CardControl[3].SetBenchmarkPos(nWidth/2-233,nHeight/2+10,enXCenter,enYTop);
	m_CardControl[4].SetBenchmarkPos(nWidth/2-268,nHeight/2-183,enXCenter,enYTop);

	//��ע��λ��
	INT nXControl = nWidth/2+150;
	INT nYControl = nHeight/2+150;

	//��ť�ؼ�
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	
	//��ʼ��ť
	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2+58,nHeight/2+264,0,0,uFlags);

	//���ư�ť
	m_btFollow.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btFollow,NULL,nXControl,nYControl,0,0,uFlags);
	DeferWindowPos(hDwp,m_btShowHand,NULL,nXControl+rcButton.Width()+1,nYControl,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAddTimes3,NULL,nXControl,nYControl+rcButton.Height()+1,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAddTimes2,NULL,nXControl+rcButton.Width()+1,nYControl+rcButton.Height()+1,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAddTimes1,NULL,nXControl,nYControl+rcButton.Height()*2+1,0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUp,NULL,nXControl+rcButton.Width()+1,nYControl+rcButton.Height()*2+1,0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	//��ҳ���
	m_PlayerJeton[0].SetBenchmarkPos(nWidth/2+138,nHeight/2-140);
	m_PlayerJeton[1].SetBenchmarkPos(nWidth/2+125,nHeight/2-20);
	m_PlayerJeton[2].SetBenchmarkPos(nWidth/2,nHeight/2+45);
	m_PlayerJeton[3].SetBenchmarkPos(nWidth/2-135,nHeight/2-20);
	m_PlayerJeton[4].SetBenchmarkPos(nWidth/2-138,nHeight/2-140);
	//�������
	m_PlayerJeton[GAME_PLAYER].SetBenchmarkPos(nWidth/2-70,nHeight/2-130);

	//������ʼλ��
	m_SendCardPos.SetPoint(nWidth/2+150,nHeight/2-340);

	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,nHeight/2-240,0,0,SWP_NOZORDER|SWP_NOSIZE);
	
	return;
}

//�����ж�
bool CGameClientView::IsDispatchCard()
{
	return m_SendCardItemArray.GetCount()>0;
}

//��ɷ���
void CGameClientView::FinishDispatchCard()
{
	//����ж�
	if (m_SendCardItemArray.GetCount()==0) return;

	//��ɶ���
	for (INT_PTR i=0;i<m_SendCardItemArray.GetCount();i++)
	{
		//��ȡ����
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[i];

		//��ȡ�˿�
		BYTE cbCardData[MAX_COUNT];
		WORD wChairID=pSendCardItem->wChairID;
		BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

		//�����˿�
		if (cbCardCount<MAX_COUNT)
		{
			cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
			m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
		}
	}

	//ɾ������
	KillTimer(IDI_SEND_CARD);
	m_SendCardItemArray.RemoveAll();

	AfxGetMainWnd()->SendMessage(IDM_SEND_CARD_FINISH,0,0);

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//�����˿�
void CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;

	//���ñ���
	tagSendCardItem SendCardItem;
	SendCardItem.wChairID=wChairID;
	SendCardItem.cbCardData=cbCardData;
	m_SendCardItemArray.Add(SendCardItem);

	//��������
	if (m_SendCardItemArray.GetCount()==1) 
	{
		//���ö�ʱ��
		SetTimer(IDI_SEND_CARD,SPEED_SEND_CARD,NULL);

		//
		m_nStepCount = SEND_STEP_COUNT;
		m_SendCardCurPos = m_SendCardPos;
		m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
		m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

		//
		InvalidGameView(0,0,0,0);

		//��������
		CGameClientEngine * pGameClientDlg=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
		pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	}

	return;
}

//������ע
void CGameClientView::SetUserTableScore(WORD wChairID, LONGLONG lTableScore)
{
	//��������
	if (wChairID!=INVALID_CHAIR) m_lTableScore[wChairID]=lTableScore;
	else ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//���ʹ���
	if (nIDEvent==IDI_SEND_CARD)
	{
		//����λ��
		m_SendCardCurPos.x += m_nXStep;
		m_SendCardCurPos.y += m_nYStep;
		//���½���
		InvalidGameView(0,0,0,0);
		//ֹͣ�ж�
		if ( --m_nStepCount == 0 )
		{
			//��ȡ�˿�
			BYTE cbCardData[MAX_COUNT];
			WORD wChairID=m_SendCardItemArray[0].wChairID;
			BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));
			//�����˿�
			if (cbCardCount<MAX_COUNT)
			{
				cbCardData[cbCardCount++]=m_SendCardItemArray[0].cbCardData;
				m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
			}
			//ɾ������
			m_SendCardItemArray.RemoveAt(0);
			//��������
			if (m_SendCardItemArray.GetCount()>0)
			{
				wChairID = m_SendCardItemArray[0].wChairID;
				m_nStepCount = SEND_STEP_COUNT;
				m_SendCardCurPos = m_SendCardPos;
				m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
				m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;
				//��������
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
				return;
			}
			else 
			{	//��ɴ���
				KillTimer(IDI_SEND_CARD);
				PostEngineMessage(IDM_SEND_CARD_FINISH,0,0);
				return;
			}
		}
		return;
	}
	else if( nIDEvent == IDI_USER_SHOW_HAND )
	{
		KillTimer( IDI_USER_SHOW_HAND );
		m_bUserShowHand = false;
		InvalidGameView(0,0,0,0);
		return;
	}
#ifdef SHOW_CARD_ANIMATE
	else if( nIDEvent == IDI_SHOW_CARD )
	{
		if( !m_CardControl[MYSELF_VIEW_ID].PlayMoveCard() )
		{
			KillTimer( IDI_SHOW_CARD );
		}
		InvalidGameView(0,0,0,0);
		return;
	}
#endif

	__super::OnTimer(nIDEvent);
}

//���õ�Ԫע
void CGameClientView::SetCellScore( LONGLONG lCellScore )
{
	if( m_lCellScore == lCellScore ) return;
	m_lCellScore = lCellScore;
	//������ע��ť����
	TCHAR szNum[16];
	_i64tot(m_lCellScore,szNum,10);
    m_btAddTimes1.SetWindowText(szNum);
	
	_i64tot(m_lCellScore*2,szNum,10);
	m_btAddTimes2.SetWindowText(szNum);
	
	_i64tot(m_lCellScore*3,szNum,10);
	m_btAddTimes3.SetWindowText(szNum);
	InvalidGameView(0,0,0,0);
}

//
void CGameClientView::SetUserShowHand( bool bShowHand )
{
	if( m_bUserShowHand != bShowHand )
	{
		m_bUserShowHand = bShowHand;
		
		//
		if( m_bUserShowHand )
			SetTimer( IDI_USER_SHOW_HAND,TIME_USER_SHOW_HAND,NULL );

		InvalidateRect( NULL );
	}
}

VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageViewBack,DRAW_MODE_CENTENT);

	IClientUserItem*pClientUserItem=NULL;
	tagUserInfo* pUserData=NULL;



	//�滭�û�
	TCHAR szBuffer[64]=TEXT("");
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wUserTimer=GetUserClock(i);
		pClientUserItem=GetClientUserItem(i);
		pUserData=(pClientUserItem!=NULL)?pClientUserItem->GetUserInfo():NULL;

		//�滭�û�
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextAlign(((i==0)||(i==1))?TA_RIGHT:TA_LEFT);
			DrawTextString(pDC,pUserData->szNickName,RGB(255,255,255),RGB(0,0,0),m_ptNickName[i].x,m_ptNickName[i].y);

			//�û����
			LONGLONG lLeaveScore=(pUserData->cbUserStatus==US_PLAYING)?m_lUserScore[i]-m_lTableScore[i]:pUserData->lScore;
			_i64tot(lLeaveScore,szBuffer,10);
			DrawTextString(pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptNickName[i].x,m_ptNickName[i].y+16);
			//��ǰ��ע
			if( m_lTableScore[i] > 0L )
			{
				if( i == MYSELF_VIEW_ID ) pDC->SetTextAlign(TA_RIGHT);
				_i64tot(m_lTableScore[i],szBuffer,10);
				DrawTextString(pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptTableScore[i].x,m_ptTableScore[i].y);
			}

			//������Ϣ
			
			//��ʱ������ʾ
			if (wUserTimer!=0) DrawUserClock(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer);
			//�û���ʼ����ʾ
			if (pUserData->cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			//���ó����쳣��������������ڲ�����ָ��Ī��������
			DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pClientUserItem);
		}

		//��ҳ���
		m_PlayerJeton[i].DrawJettonControl(pDC);
		//��������
		LONGLONG lJetonScore = m_PlayerJeton[i].GetScore();
		if( lJetonScore > 0L )
		{
			CPoint pt = m_PlayerJeton[i].GetBenchmarkPos();
			CRect rcDraw(pt.x-55,pt.y+15,pt.x+55,pt.y+27);
			_i64tot(lJetonScore,szBuffer,10);
			pDC->SetTextAlign(TA_LEFT);
			DrawTextString(pDC,szBuffer,RGB(255,255,85),RGB(0,0,0),&rcDraw);
		}
	}
	m_PlayerJeton[GAME_PLAYER].DrawJettonControl(pDC);

	for( WORD i = 0; i < GAME_PLAYER; i++ )
		m_CardControl[i].DrawCardControl(pDC);

	INT nNumberWidth = m_PngNumber.GetWidth()/10;
	INT nNumberHeight = m_PngNumber.GetHeight();

	//��ע��Ϣ
	if( m_lCellScore > 0L )
	{
		INT nXPos;
		INT nYPos = nHeight/2-325;
		int nCellCount = 0;
		BYTE byCell[10];
		LONGLONG lCellScore = m_lCellScore;
		while( lCellScore > 0L )
		{
			byCell[nCellCount++] = (BYTE)(lCellScore-lCellScore/10*10);
			lCellScore /= 10;
			if( nCellCount == 10 ) break;
		}
		nXPos = nWidth/2-200-nCellCount*(nNumberWidth-3)/2;
		for( int j = nCellCount-1; j >= 0; j-- )
		{
			m_PngNumber.DrawImage(pDC,nXPos,nYPos,nNumberWidth,nNumberHeight,byCell[j]*nNumberWidth,0);
			nXPos += (nNumberWidth-3);
		}
	}
	//������ע
	LONGLONG lTableScore=0L;
	//ע:����������ע����
	for( i = 0; i < GAME_PLAYER; i++ )
		lTableScore += m_PlayerJeton[i].GetScore();
	lTableScore+=m_PlayerJeton[GAME_PLAYER].GetScore();
	//test
	//lTableScore = 1234567890L;
	//��ע��Ϣ
	if (lTableScore>0L)
	{
		INT nXPos = nWidth/2+13;
		INT nYPos = nHeight/2-297;
		int nCellCount = 0;
		BYTE byCell[10];
		while( lTableScore > 0L )
		{
			byCell[nCellCount++] = (BYTE)(lTableScore-lTableScore/10*10);
			lTableScore /= 10;
			if( nCellCount == 10 ) break;
		}
		for( int j = nCellCount-1; j >= 0; j-- )
		{
			m_PngNumber.DrawImage(pDC,nXPos,nYPos,nNumberWidth,nNumberHeight,byCell[j]*nNumberWidth,0);
			nXPos += (nNumberWidth-3);
		}
	}

	//�û����
	if( m_bUserShowHand )
	{
		m_PngShowHand.DrawImage( pDC,(nWidth-m_PngShowHand.GetWidth())/2,(nHeight-m_PngShowHand.GetHeight())/2 );
	}

	//�滭�˿�
	if (m_SendCardItemArray.GetCount()>0)
	{
		//��������
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

		//��ȡ��С
		int nItemWidth=m_ImageCard.GetWidth()/13;
		int nItemHeight=m_ImageCard.GetHeight()/5;

		//�滭�˿�
		m_ImageCard.BlendDrawImage(pDC,m_SendCardCurPos.x,m_SendCardCurPos.y,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4,RGB(255,0,255),255);
	}
}
//
void CGameClientView::SetUserScore( WORD wChairId, LONGLONG lScore )
{
	if( wChairId == INVALID_CHAIR )
		ZeroMemory( m_lUserScore,sizeof(m_lUserScore) );
	else m_lUserScore[wChairId] = lScore;
	InvalidateRect( NULL );
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�˿˲���
	if (m_CardControl[2].OnEventSetCursor(MousePoint)==true) return TRUE;

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//�����Ϣ
VOID CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�˿˵��
	if (m_CardControl[MYSELF_VIEW_ID].OnEventLeftMouseDown(MousePoint)==true)
	{
#ifdef	SHOW_CARD_ANIMATE
		if( m_CardControl[MYSELF_VIEW_ID].BeginMoveCard() )
		{
			SetTimer( IDI_SHOW_CARD,SPEED_MOVE_CARD,NULL );
			//���½���
			InvalidGameView(0,0,0,0);
		}
#else
		m_CardControl[MYSELF_VIEW_ID].SetDisplayHead(true);
		InvalidGameView(0,0,0,0);
#endif
	}

	return;
}

//
VOID CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�˿˵��
	if (m_CardControl[MYSELF_VIEW_ID].OnEventLeftMouseUp(MousePoint)==true)
	{
#ifndef	SHOW_CARD_ANIMATE
		m_CardControl[MYSELF_VIEW_ID].SetDisplayHead(false);
		InvalidGameView(0,0,0,0);
		PostMessage( WM_SETCURSOR );
#endif
	}
}

//////////////////////////////////////////////////////////////////////////


