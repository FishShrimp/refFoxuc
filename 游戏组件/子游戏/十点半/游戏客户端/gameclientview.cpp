#include "StdAfx.h"
#include "GameClient.h"
#include "GameClientView.h"
#include "GameClientDlg.h"
#include "DlgInfomation.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

//���ư�ť
#define IDC_START						100								//��ʼ��ť
#define IDC_GIVECARD					102								//��ע
#define IDC_STOPCARD					106								//����
#define IDC_ADDSCORE					107								//��ע
#define IDC_ONESCORE					108								//һ��
#define IDC_TWOSCORE					109								//����
#define IDC_THREESCORE					110								//����
#define IDC_FOURSCORE					111								//�ı�

#define IDI_MOVE_JETTON					100								//�ƶ����붨ʱ��
#define IDI_MOVE_NUMBER					101								//��������
#define IDI_SEND_CARD					102								//���Ʊ�ʶ
#define IDI_HIDE_CARD					103								//�����˿�
#define IDI_USER_ACTION					104								//������ʶ
#define IDI_ACTION_ADD_JETTON			105								//���������Ϣ

#define TIME_MOVE_JETTON				20								//�ƶ�����ʱ��
#define TIME_MOVE_NUMBER				50								//��������ʱ��
#define TIME_ACTION_ADD_JETTON			2000							//���������Ϣʱ��

//���ƶ���
#define SEND_STEP_COUNT					5								//����
#define SPEED_SEND_CARD					10								//�����ٶ�

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	//ϵͳ��Ϣ
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()

	//��ť��Ϣ
	ON_BN_CLICKED(IDC_START, OnBnClickedStart)
	ON_BN_CLICKED(IDC_GIVECARD,OnBnClickedGiveCard)
	ON_BN_CLICKED(IDC_STOPCARD,OnBnClickedStopCard)
	ON_BN_CLICKED(IDC_ADDSCORE,OnBnClickedAddScore)
	ON_BN_CLICKED(IDC_ONESCORE,OnBnClickedOneScore)
	ON_BN_CLICKED(IDC_TWOSCORE,OnBnClickedTwoScore)
	ON_BN_CLICKED(IDC_THREESCORE,OnBnClickedThreeScore)
	ON_BN_CLICKED(IDC_FOURSCORE,OnBnClickedFourScore)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//״̬����
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));
	m_lCellScore = 0L;
	m_lJettonScore = 0L;
	m_wBankerUser = INVALID_CHAIR;
	for( WORD i = 0; i < GAME_PLAYER; i++ )
		m_fUserCardScore[i] = -1;
	m_bWaitUserScore = false;
	m_bShowAddJetton = true;

	//��������
	m_SendCardPos.SetPoint(0,0);
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = SEND_STEP_COUNT;
	m_nXStep = 0;
	m_nYStep = 0;

	//��������
	m_wActionUser=INVALID_CHAIR;
	m_cbUserAction=0;

	//������Դ
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_ImageGameCard.LoadFromResource(hResInstance,IDB_GAME_CARD);
	m_ImageViewFill.LoadFromResource(hResInstance,IDB_VIEW_FILL);
	m_ImageViewBack.LoadFromResource(hResInstance,IDB_VIEW_BACK);
	m_PngNumber.LoadImage(hResInstance,TEXT("CARD_SCORE_NUM"));
	m_PngScoreNum.LoadImage(hResInstance,TEXT("SCORE_NUM"));
	m_PngActionBack.LoadImage(hResInstance,TEXT("ACTION_BACK"));
	m_PngActionFont.LoadImage(hResInstance,TEXT("ACTION_FONT"));
	m_PngActionScoreBack.LoadImage( hResInstance,TEXT("ACTION_SCORE_BACK") );
	m_PngActionScoreNum.LoadImage( hResInstance,TEXT("ACTION_SCORE_NUM") );
	m_PngBkTopLeft.LoadImage( hResInstance,TEXT("BK_TOPLEFT") );
	m_PngBkTopRight.LoadImage( hResInstance,TEXT("BK_TOPRIGHT") );
	m_PngBkBottomLeft.LoadImage( hResInstance,TEXT("BK_BOTTOMLEFT") );
	m_PngBkBottomRight.LoadImage( hResInstance,TEXT("BK_BOTTOMRIGHT") );
	m_ImageTitle.LoadFromResource(hResInstance,IDB_TITLE);
	m_PngCardScore.LoadImage(hResInstance,TEXT("CARD_SCORE"));
	m_PngBanker.LoadImage(hResInstance,TEXT("BANKER_FLAG"));
	m_PngWaitScore.LoadImage(hResInstance,TEXT("WAIT_USER_SCORE"));

	//��ȡ��С
	m_SizeGameCard.SetSize(m_ImageGameCard.GetWidth()/13,m_ImageGameCard.GetHeight()/5);

	m_nXFace=48;
	m_nYFace=48;
	m_nXTimer=65;
	m_nYTimer=69;

	return;
}

//��������
CGameClientView::~CGameClientView()
{
}

//��Ϣ����
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL) 
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//���ý���
VOID CGameClientView::ResetGameView()
{
	//״̬����
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));
	m_lCellScore = 0L;
	m_lJettonScore = 0L;
	m_wBankerUser = INVALID_CHAIR;
	for( WORD i = 0; i < GAME_PLAYER; i++ )
		m_fUserCardScore[i] = -1;
	m_bWaitUserScore = false;
	m_bShowAddJetton = true;

	//��������
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = 0;
	m_nXStep = 0;
	m_nYStep = 0;
	m_SendCardItemArray.RemoveAll();

	//��������
	m_wActionUser=INVALID_CHAIR;
	m_cbUserAction=0;

	//���ư�ť
	m_btStart.ShowWindow(SW_HIDE);
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//׼��λ��
	m_ptReady[0].x = nWidth/2;
	m_ptReady[0].y = 200;
	m_ptReady[1].x = 220;
	m_ptReady[1].y = nHeight/2-4;
	m_ptReady[2].x = nWidth/2;
	m_ptReady[2].y = nHeight-200;
	m_ptReady[3].x = nWidth-200;
	m_ptReady[3].y = nHeight/2-4;

	//ͷ��λ��
	m_ptAvatar[0].x = nWidth/2-m_nXFace/2;
	m_ptAvatar[0].y = 5;
	m_ptAvatar[1].x = 5;
	m_ptAvatar[1].y = nHeight/2-85;
	m_ptAvatar[2].x = nWidth/2-m_nXFace/2;
	m_ptAvatar[2].y = nHeight-5-m_nYFace;
	m_ptAvatar[3].x = nWidth-5-m_nXFace;
	m_ptAvatar[3].y = nHeight/2-85;

	//ʱ��λ��
	m_ptClock[0].x = nWidth/2-m_nXFace-55;
	m_ptClock[0].y = 30;
	m_ptClock[1].x = 30;
	m_ptClock[1].y = nHeight/2-145;
	m_ptClock[2].x = nWidth/2-m_nXFace-100;
	m_ptClock[2].y = nHeight-30;
	m_ptClock[3].x = nWidth-30;
	m_ptClock[3].y = nHeight/2-145;

	m_ptBanker[0].SetPoint( nWidth/2-m_nXFace-25,10 );
	m_ptBanker[1].SetPoint( 15,nHeight/2-115 );
	m_ptBanker[2].SetPoint( nWidth/2-m_nXFace-30,nHeight-30 );
	m_ptBanker[3].SetPoint( nWidth-35,nHeight/2-115 );

	//�÷�����λ��
	m_ptJettons[0].SetPoint(nWidth/2+m_nXFace/2+30,35);
	m_ptJettons[1].SetPoint(75,nHeight/2-85+15);
	m_ptJettons[2].SetPoint(nWidth/2-m_nXFace/2+65,nHeight-5-m_nYFace-5);
	m_ptJettons[3].SetPoint(nWidth-5-m_nXFace-40,nHeight/2-85+10);

	//����λ��
	m_ptUserAction[0].SetPoint(nWidth/2,180);
	m_ptUserAction[1].SetPoint(+50,nHeight/2-120);
	m_ptUserAction[2].SetPoint(nWidth/2,nHeight-180);
	m_ptUserAction[3].SetPoint(nWidth-50,nHeight/2-120);

	//�˿˿ؼ�
	m_CardControl[0].SetBenchmarkPos(nWidth/2,m_nYFace+10,enXCenter,enYTop);
	m_CardControl[1].SetBenchmarkPos(5,nHeight/2+26,enXLeft,enYCenter);
	m_CardControl[2].SetBenchmarkPos(nWidth/2,nHeight-m_nYFace-10,enXCenter,enYBottom);
	m_CardControl[3].SetBenchmarkPos(nWidth-5,nHeight/2+26,enXRight,enYCenter);

	//����ؼ�
	m_JettonControl.SetBenchmarkPos(nWidth/2,nHeight/2-30);

	//���ֿؼ�λ��
	m_NumberControl[0].SetBencbmarkPos(nWidth/2,170,enXCenter);
	m_NumberControl[1].SetBencbmarkPos(155,nHeight/2-80,enXLeft);
	m_NumberControl[2].SetBencbmarkPos(nWidth/2,nHeight-195,enXCenter);
	m_NumberControl[3].SetBencbmarkPos(nWidth-155,nHeight/2-80,enXRight);

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	
	CRect rcButton;
	m_btStart.GetWindowRect(&rcButton);

	//��ʼ��ť
	DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2-rcButton.Width()/2,nHeight-250,0,0,uFlags);

	m_btOneScore.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOneScore,NULL,nWidth/2-rcButton.Width()*2-15,nHeight-200,0,0,uFlags);
	DeferWindowPos(hDwp,m_btTwoScore,NULL,nWidth/2-rcButton.Width()-5,nHeight-200,0,0,uFlags);
	DeferWindowPos(hDwp,m_btThreeScore,NULL,nWidth/2+5,nHeight-200,0,0,uFlags);
	DeferWindowPos(hDwp,m_btFourScore,NULL,nWidth/2+rcButton.Width()+15,nHeight-200,0,0,uFlags);

	m_btGiveCard.GetWindowRect(&rcButton);
	//Ҫ�ư�ť
	DeferWindowPos(hDwp,m_btGiveCard,NULL,nWidth/2+170,nHeight-150,0,0,uFlags);
	DeferWindowPos(hDwp,m_btStopCard,NULL,nWidth/2+170,nHeight-150+rcButton.Height()+5,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAddScore,NULL,nWidth/2+170,nHeight-150+rcButton.Height()*2+10,0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	//������ʼλ��
	m_SendCardPos.SetPoint(nWidth/2-48,nHeight/2-48);

	//SetFlowerControlInfo( 0,m_ptAvatar[0].x+m_nXFace/2-BIG_FACE_WIDTH/2,m_ptAvatar[0].y+m_nYFace );
	//SetFlowerControlInfo( 1,m_ptAvatar[1].x+m_nXFace,m_ptAvatar[1].y+m_nYFace/2-BIG_FACE_HEIGHT/2 );
	//SetFlowerControlInfo( 2,m_ptAvatar[2].x+m_nXFace/2-BIG_FACE_WIDTH/2,m_ptAvatar[2].y-BIG_FACE_HEIGHT );
	//SetFlowerControlInfo( 3,m_ptAvatar[3].x-BIG_FACE_WIDTH,m_ptAvatar[3].y+m_nYFace/2-BIG_FACE_HEIGHT/2 );

	return;
}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageViewBack,DRAW_MODE_CENTENT);
	m_ImageTitle.BitBlt(pDC->m_hDC,80,2);
	//�Ľ�
	m_PngBkTopLeft.DrawImage( pDC,0,0 );
	m_PngBkTopRight.DrawImage( pDC,nWidth-m_PngBkTopRight.GetWidth(),0 );
	m_PngBkBottomLeft.DrawImage( pDC,0,nHeight-m_PngBkBottomLeft.GetHeight() );
	m_PngBkBottomRight.DrawImage( pDC,nWidth-m_PngBkBottomRight.GetWidth(),nHeight-m_PngBkBottomRight.GetHeight() );

	//��Ԫע��
	if( m_lCellScore > 0 )
		DrawNumberString( pDC,m_lCellScore,195,23,m_PngScoreNum,false );

	//�滭�û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IClientUserItem * pClientUserItem=GetClientUserItem(i);
		//test
		//tagUserData *pClientUserItem = new tagUserData;
		//lstrcpy( pClientUserItem->GetNickName(),TEXT("SSSSSSSSkk") );
		//end test
		if (pClientUserItem==NULL) continue;

		//�滭����
		WORD wTime = GetUserClock(i);
		//test
		//wTime = 30;
		//end test
		if( wTime > 0 )
			DrawUserClock(pDC,m_ptClock[i].x,m_ptClock[i].y,wTime);

		//�û�����
		pDC->SetTextAlign((i==3||i==4)?TA_RIGHT:TA_LEFT);
		DrawTextString(pDC,pClientUserItem->GetNickName(),RGB(255,255,255),RGB(0,0,0),m_ptAvatar[i].x+((i==3||i==4)?-5:50),m_ptAvatar[i].y+2);

		//�û�ͷ��
		DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pClientUserItem);

		//�û�����
		TCHAR szScore[64];
		LONGLONG lScore = m_lUserAddScore[i]==0?pClientUserItem->GetUserScore():(pClientUserItem->GetUserScore()-m_lUserAddScore[i]);
		_sntprintf(szScore,CountArray(szScore),TEXT("%I64d"),lScore);
		DrawTextString(pDC,szScore,RGB(255,255,255),RGB(0,0,0),m_ptAvatar[i].x+((i==3||i==4)?-5:50),m_ptAvatar[i].y+12*1+10);

		//ׯ��
		//test
		//m_wBankerUser = i;
		//end test
		if( m_wBankerUser == i )
		{
			m_PngBanker.DrawImage( pDC,m_ptBanker[i].x,m_ptBanker[i].y );
		}
		
		//test
		//delete pClientUserItem;
		//end test
	}

	//�ȴ������ע
	if( m_bWaitUserScore )
	{
		m_PngWaitScore.DrawImage( pDC,(nWidth-m_PngWaitScore.GetWidth())/2,(nHeight-m_PngWaitScore.GetHeight())/2 );
	}

	//�û��˿�			
	for (BYTE i=0;i<GAME_PLAYER;i++) 
	{
		m_CardControl[i].DrawCardControl(pDC);

		//�û�׼��
		IClientUserItem * pClientUserItem=GetClientUserItem(i);
		if (pClientUserItem==NULL) continue;
		//ͬ���־
		BYTE cbUserStatus=pClientUserItem->GetUserStatus();
		//test
		//cbUserStatus = US_READY;
		//end test
		if (cbUserStatus==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
	}
	
	//�滭����
	m_JettonControl.DrawJettonControl(pDC);

	LONGLONG lJettonScore = (LONGLONG)m_JettonControl.GetScore();
	//test
	//lJettonScore = 1234L;
	//end test
	//�滭��������
	if( lJettonScore > 0 )
	{
		DrawNumberString( pDC,lJettonScore,195,50,m_PngScoreNum,false );
	}

	//���������Ϣ
	if( m_bShowAddJetton && m_lJettonScore < lJettonScore )
	{
		m_PngActionScoreBack.DrawImage( pDC,nWidth/2-m_PngActionScoreBack.GetWidth()/2,
			nHeight/2-m_PngActionScoreNum.GetHeight()/2-90 );
		DrawNumberString( pDC,lJettonScore-m_lJettonScore,nWidth/2,nHeight/2-75,m_PngActionScoreNum,true );
	}

	//�÷����ֹ�������
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		m_NumberControl[i].DrawNumberControl(pDC);
	}

	//�������
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if( m_fUserCardScore[i] != -1 )
		{
			INT nXPos,nYPos;
			if( i <= 2 )
			{
				CPoint pt = m_CardControl[i].GetTailPos();
				nXPos = pt.x + 64;
				nYPos = pt.y;
			}
			else
			{
				CPoint pt = m_CardControl[i].GetHeadPos();
				nXPos = pt.x-8;
				nYPos = pt.y;
			}
			
			DrawCardScore( pDC,nXPos,nYPos,m_fUserCardScore[i],i<=2?true:false );
		}
	}

	//�滭�����˿�
	if (m_SendCardItemArray.GetCount()>0)
	{
		//��������
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

		//��ȡ��С
		INT nItemWidth=m_SizeGameCard.cx;
		INT nItemHeight=m_SizeGameCard.cy;

		//�滭�˿�
		m_ImageGameCard.TransDrawImage(pDC,m_SendCardCurPos.x,m_SendCardCurPos.y,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4,RGB(255,0,255));
	}

	//�û�����
	if (m_wActionUser!=INVALID_CHAIR)
	{
		//����λ��
		INT nXPos=m_ptUserAction[m_wActionUser].x-m_PngActionBack.GetWidth()/8;
		INT nYPos=m_ptUserAction[m_wActionUser].y-m_PngActionBack.GetHeight()/2;

		//�滭����
		INT nImageIndex = m_wActionUser;
		m_PngActionBack.DrawImage(pDC,nXPos,nYPos,m_PngActionBack.GetWidth()/4,m_PngActionBack.GetHeight(),
			nImageIndex*m_PngActionBack.GetWidth()/4,0);

		//�滭����
		m_PngActionFont.DrawImage(pDC,nXPos+10,nYPos+25,m_PngActionFont.GetWidth()/3,m_PngActionFont.GetHeight(),
			(m_cbUserAction-1)*m_PngActionFont.GetWidth()/3,0);
	}

	return;
}

//��ʼ��ť
VOID CGameClientView::OnBnClickedStart()
{
	//������Ϣ
	SendEngineMessage(IDM_START,0,0);

	return;
}

//Ҫ��
VOID CGameClientView::OnBnClickedGiveCard()
{
	//������Ϣ
	SendEngineMessage(IDM_GIVE_CARD,0,0);

	return;
}

//ͣ��
VOID CGameClientView::OnBnClickedStopCard()
{
	//������Ϣ
	SendEngineMessage(IDM_STOP_CARD,0,0);

	return;
}

//
VOID CGameClientView::OnBnClickedAddScore()
{
	//������Ϣ
	SendEngineMessage(IDM_ADD_SCORE,0,0);

	return;
}

//1����ע
VOID CGameClientView::OnBnClickedOneScore()
{
	//������Ϣ
	SendEngineMessage(IDM_USER_SCORE,1,0);

	return;
}

//2����ע
VOID CGameClientView::OnBnClickedTwoScore()
{
	//������Ϣ
	SendEngineMessage(IDM_USER_SCORE,2,0);

	return;
}

//3����ע
VOID CGameClientView::OnBnClickedThreeScore()
{
	//������Ϣ
	SendEngineMessage(IDM_USER_SCORE,3,0);

	return;
}

//4����ע
VOID CGameClientView::OnBnClickedFourScore()
{
	//������Ϣ
	SendEngineMessage(IDM_USER_SCORE,4,0);

	return;
}

//��������
INT CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��������
	CRect rcCreate(0,0,0,0);

	//���ư�ť
	m_btStart.Create(NULL,WS_CHILD,rcCreate,this,IDC_START);
	m_btGiveCard.Create(NULL,WS_CHILD,rcCreate,this,IDC_GIVECARD);
	m_btStopCard.Create(NULL,WS_CHILD,rcCreate,this,IDC_STOPCARD);
	m_btAddScore.Create(NULL,WS_CHILD,rcCreate,this,IDC_ADDSCORE);
	m_btOneScore.Create(NULL,WS_CHILD,rcCreate,this,IDC_ONESCORE);
	m_btTwoScore.Create(NULL,WS_CHILD,rcCreate,this,IDC_TWOSCORE);
	m_btThreeScore.Create(NULL,WS_CHILD,rcCreate,this,IDC_THREESCORE);
	m_btFourScore.Create(NULL,WS_CHILD,rcCreate,this,IDC_FOURSCORE);

	//���ư�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_BT_START,hResInstance,false,false);
	m_btGiveCard.SetButtonImage(IDB_BT_GIVECARD,hResInstance,false,false);
	m_btStopCard.SetButtonImage(IDB_BT_STOPCARD,hResInstance,false,false);
	m_btAddScore.SetButtonImage(IDB_BT_ADDSCORE,hResInstance,false,false);
	m_btOneScore.SetButtonImage(IDB_BT_SCORE,hResInstance,false,false);
	m_btTwoScore.SetButtonImage(IDB_BT_SCORE,hResInstance,false,false);
	m_btThreeScore.SetButtonImage(IDB_BT_SCORE,hResInstance,false,false);
	m_btFourScore.SetButtonImage(IDB_BT_SCORE,hResInstance,false,false);

	//������ʾ
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btStart,TEXT("��ʼ��Ϸ"));
	m_ToolTipCtrl.AddTool(&m_btGiveCard,TEXT("Ҫ��"));
	m_ToolTipCtrl.AddTool(&m_btStopCard,TEXT("ͣ��"));
	m_ToolTipCtrl.AddTool(&m_btAddScore,TEXT("�ӱ�"));

	//test
	//m_btStart.ShowWindow( SW_SHOW );
	//m_btGiveCard.ShowWindow( SW_SHOW );
	//m_btStopCard.ShowWindow( SW_SHOW );
	//m_btAddScore.ShowWindow( SW_SHOW );
	//m_btOneScore.ShowWindow( SW_SHOW );
	//m_btTwoScore.ShowWindow( SW_SHOW );
	//m_btThreeScore.ShowWindow( SW_SHOW );
	//m_btFourScore.ShowWindow( SW_SHOW );
	//BYTE byCard[] = {
	//	0x1b,0x0B,0x0C,0x0D,0x0d,0x1b,0x0B,0x01,0x4e
	//};
	//CGameLogic gm;
	//for( WORD i = 0; i < GAME_PLAYER; i++ )
	//{
	//	m_CardControl[i].SetCardData( byCard,9 );
	//	FLOAT fCardScore = gm.GetCardGenre(byCard,9);
	//	SetCardScore( i,fCardScore );
	//	//m_JettonControl[i]
	//}
	//m_lCellScore = 1L;
	//SetUserAction( 2,AC_ADD_SCORE );
	//end test

	return 0;
}

//�Ƿ���
bool CGameClientView::IsDispatchStatus()
{
	return m_SendCardItemArray.GetCount()>0;
}

//ֹͣ����
VOID CGameClientView::ConcludeDispatch()
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

	//������Ϣ
	SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

	//ɾ������
	KillTimer(IDI_SEND_CARD);
	m_SendCardItemArray.RemoveAll();

	//���½���
	RefreshGameView();

	return;
}

//�����˿�
VOID CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
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

		//���ò���,����
		m_nStepCount = SEND_STEP_COUNT;
		m_SendCardCurPos = m_SendCardPos;
		m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
		m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

		RefreshGameView();
	}

	return;
}

//���û���
bool CGameClientView::SetScoreInfo(WORD wChairID, LONGLONG lUserScore)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//���û���
	if (m_lUserScore[wChairID]!=lUserScore)
	{
		m_lUserScore[wChairID] = lUserScore;

		//���½���
		RefreshGameView();
	}

	return true;
}

//���ö���
bool CGameClientView::SetUserAction(WORD wActionUser, BYTE cbUserAction)
{
	//���ö���
	if ((m_wActionUser!=wActionUser)||(m_cbUserAction!=cbUserAction))
	{
		//���ñ���
		m_wActionUser=wActionUser;
		m_cbUserAction=cbUserAction;

		//���½���
		RefreshGameView();

		//����ʱ��
		if (m_wActionUser!=INVALID_CHAIR) SetTimer(IDI_USER_ACTION,3000,NULL);
	}

	return true;
}

//���������
VOID CGameClientView::SetCardScore( WORD wChairId, FLOAT fCardScore )
{
	if( wChairId == INVALID_CHAIR )
	{
		for( WORD i = 0; i < GAME_PLAYER; i++ )
			m_fUserCardScore[i] = -1;
		RefreshGameView();
	}
	else if( m_fUserCardScore[wChairId] != fCardScore )
	{
		m_fUserCardScore[wChairId] = fCardScore;
		RefreshGameView();
	}
}

//���õ�Ԫ��
VOID CGameClientView::SetCellScore( LONGLONG lScore )
{
	m_lCellScore = lScore;
	RefreshGameView();
}

//�ӳ���
VOID CGameClientView::OnUserAddJettons( WORD wChairId, LONGLONG lScore )
{
	if( wChairId == INVALID_CHAIR ) 
	{
		ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));
		//������ӷ�
		m_lJettonScore = 0L;
		return;
	}
	m_lUserAddScore[wChairId] += lScore;
	m_JettonControl.AddScore(lScore,m_ptJettons[wChairId]);
}

//�Ƴ�����
VOID CGameClientView::OnUserRemoveJettons( WORD wChairId, LONGLONG lScore )
{
	if( wChairId == INVALID_CHAIR ) return;
	if( lScore != 0 )
	{
		m_JettonControl.RemoveScore(lScore,m_ptJettons[wChairId]);
	}
	else 
	{
		m_JettonControl.RemoveAllScore( m_ptJettons[wChairId] );
	}
}

//��ʼ���붯��
VOID CGameClientView::BeginMoveJettons()
{
	if( m_JettonControl.BeginMoveJettons() )
	{
		SetTimer( IDI_MOVE_JETTON,TIME_MOVE_JETTON,NULL );
		RefreshGameView();
	}
}

//ֹͣ���붯��
VOID CGameClientView::StopMoveJettons()
{
	if( m_JettonControl.FinishMoveJettons() )
	{
		KillTimer( IDI_MOVE_JETTON );
		//������Ϣ
		SendEngineMessage(IDM_MOVE_JET_FINISH,0,0);
		RefreshGameView();
	}
}

//��ʼ���ֹ���
VOID CGameClientView::BeginMoveNumber()
{
	BOOL bSuccess = FALSE;
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if( m_NumberControl[i].BeginScrollNumber() )
			bSuccess = TRUE;
	}
	if( bSuccess )
	{
		SetTimer( IDI_MOVE_NUMBER,TIME_MOVE_NUMBER,NULL );
		RefreshGameView();
	}
}

//ֹͣ���ֹ���
VOID CGameClientView::StopMoveNumber()
{
	BOOL bMoving = FALSE;
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if( m_NumberControl[i].FinishScrollNumber() )
			bMoving = TRUE;
	}
	if( bMoving )
	{
		KillTimer( IDI_MOVE_NUMBER );
		RefreshGameView();
	}
}

//���ý�����Ϣ
VOID CGameClientView::SetGameEndInfo( WORD wWinner )
{
	if( wWinner == INVALID_CHAIR ) return ;

	m_JettonControl.RemoveAllScore( m_ptJettons[wWinner] );
	if( m_JettonControl.BeginMoveJettons() )
		SetTimer( IDI_MOVE_JETTON,TIME_MOVE_JETTON,NULL );
	RefreshGameView();
}

//��ʱ��
void CGameClientView::OnTimer(UINT nIDEvent)
{
	switch( nIDEvent )
	{
	case IDI_HIDE_CARD:		//�����˿�
		{
			//ɾ��ʱ��
			KillTimer(IDI_HIDE_CARD);

			//�����ж�
			if (m_CardControl[MYSELF_VIEW_ID].GetDisplayHead()==true)
			{
				//���ÿؼ�
				m_CardControl[MYSELF_VIEW_ID].SetDisplayHead(false);

				//���½���
				RefreshGameView();
			}

			return;
		}
	case IDI_SEND_CARD:		//���ƶ���
		{
			//�滭����
			CRect rcDraw( m_SendCardCurPos.x,m_SendCardCurPos.y,
				m_SendCardCurPos.x+m_SizeGameCard.cx,m_SendCardCurPos.y+m_SizeGameCard.cy );

			//����λ��
			m_SendCardCurPos.x += m_nXStep;
			m_SendCardCurPos.y += m_nYStep;

			rcDraw |= CRect( m_SendCardCurPos.x,m_SendCardCurPos.y,
				m_SendCardCurPos.x+m_SizeGameCard.cx,m_SendCardCurPos.y+m_SizeGameCard.cy );

			//���½���
			RefreshGameView(&rcDraw);

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

				RefreshGameView();

				//��������
				if (m_SendCardItemArray.GetCount()>0)
				{
					//��ȡλ��
					wChairID = m_SendCardItemArray[0].wChairID;
					m_nStepCount = SEND_STEP_COUNT;
					m_SendCardCurPos = m_SendCardPos;
					m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
					m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

					//��������
					CGameClientEngine * pGameDlg=(CGameClientEngine *)AfxGetMainWnd();
					pGameDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

					return;
				}
				else 
				{
					//��ɴ���
					KillTimer(IDI_SEND_CARD);
					//������Ϣ
					SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

					return;
				}
			}

			return;
		}
	case IDI_MOVE_JETTON:				//���붯��
		{
			if( !m_JettonControl.PlayMoveJettons() )
			{
				KillTimer(IDI_MOVE_JETTON);

				//������Ϣ
				SendEngineMessage(IDM_MOVE_JET_FINISH,0,0);

				//��ʾ���������Ϣ
				if( m_lJettonScore < m_JettonControl.GetScore() )
				{
					SetTimer( IDI_ACTION_ADD_JETTON,TIME_ACTION_ADD_JETTON,NULL );
				}

				//��������
				CGameClientEngine * pGameDlg=(CGameClientEngine *)AfxGetMainWnd();
				pGameDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

				RefreshGameView();

				return;
			}
			CRect rcDraw;
			//��ȡ��������
			m_JettonControl.GetDrawRect(rcDraw);
			RefreshGameView(&rcDraw);
			return;
		}
	case IDI_ACTION_ADD_JETTON:
		{
			KillTimer( IDI_ACTION_ADD_JETTON );
			m_lJettonScore = (LONGLONG)m_JettonControl.GetScore();
			RefreshGameView();
			return;
		}
	case IDI_MOVE_NUMBER:				//���ֹ���
		{
			BOOL bMoving = FALSE;
			for( INT i = 0; i < GAME_PLAYER; i++ )
			{
				if( m_NumberControl[i].PlayScrollNumber() )
				{
					bMoving = TRUE;
					//��ȡ��������
					CRect rc;
					m_NumberControl[i].GetDrawRect(rc);
					RefreshGameView(&rc);
				}
			}
			if( !bMoving )
				KillTimer(IDI_MOVE_NUMBER);
			return ;
		}
	case IDI_USER_ACTION:	//�û�����
		{
			KillTimer( IDI_USER_ACTION );

			//���ñ���
			m_cbUserAction=0;
			m_wActionUser=INVALID_CHAIR;

			//���½���
			RefreshGameView();

			return;
		}
	}
	__super::OnTimer(nIDEvent);
}

//�������
VOID CGameClientView::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags, point);

	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�˿˵��
	if (m_CardControl[MYSELF_VIEW_ID].OnEventLeftMouseDown(MousePoint)==true)
	{
		//���½���
		RefreshGameView();

		//����ʱ��
		if (m_CardControl[MYSELF_VIEW_ID].GetDisplayHead()==true)
		{
			SetTimer(IDI_HIDE_CARD,2000,NULL);
		}
		else KillTimer(IDI_HIDE_CARD);
	}

	//test
	//static int n = 0;
	//if( ++n <= 1 )
	//{
	//	for( WORD i = 0; i < GAME_PLAYER; i++ )
	//	{
	//		if( i == 0 ) continue;
	//		OnUserAddJettons( i,123456L );
	//	}
	//	BeginMoveJettons();	
	//}
	//else 
	//{
	//	OnUserAddJettons( 0,123456L*3 );
	//	for( WORD i = 0; i < GAME_PLAYER; i++ )
	//	{
	//		if( i == 0 ) continue;
	//		OnUserRemoveJettons( i,123456L*2 );
	//	}
	//	BeginMoveJettons();
	//}

	//CDlgInfomation dlg;
	//if( dlg.DoModal() == IDOK )
	//	SendEngineMessage( WM_CLOSE );
	//for( WORD i = 0; i < GAME_PLAYER; i++ )
	//{
	//	m_NumberControl[i].SetScore( 1234567L );
	//}
	//BeginMoveNumber();
	//end test

	return;
}

//���ù��
BOOL CGameClientView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�˿˲���
	if (m_CardControl[MYSELF_VIEW_ID].OnEventSetCursor(MousePoint)==true) return TRUE;

	return __super::OnSetCursor(pWnd,nHitTest,message);
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
void CGameClientView::RefreshGameView(CRect rect)
{
	GetClientRect(&rect);
	InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

	return;
}
//�滭�Ƶ�
VOID CGameClientView::DrawCardScore( CDC * pDC, INT nXPos, INT nYPos, FLOAT fCardScore, bool bLeftAlign )
{
	int nWidthCardScore = m_PngCardScore.GetWidth()/11;
	int nHeightCardScore = m_PngCardScore.GetHeight();
	int nWidthCardNum = m_PngNumber.GetWidth()/10;
	int nHeightCardNum = m_PngNumber.GetHeight();

	int nXDraw = nXPos;
	int nYDraw = nYPos;

	//����������С����С����С����С������������С����С
	if( fCardScore >= CT_WU_XIAO )
	{
		if( !bLeftAlign )
			nXDraw -= nWidthCardScore*((fCardScore==CT_REN_WU_XIAO||fCardScore==CT_DA_TIAN_WANG)?3:2)/3;
		m_PngCardScore.DrawImage( pDC,nXDraw,nYDraw,nWidthCardScore,nHeightCardScore,
			((BYTE)(CT_DA_TIAN_WANG-fCardScore))*nWidthCardScore,0,nWidthCardScore,nHeightCardScore );
	}
	//����
	else if( fCardScore == CT_ERROR )
	{
		if( !bLeftAlign )
			nXDraw -= nWidthCardScore*2/3;
		m_PngCardScore.DrawImage( pDC,nXDraw,nYDraw,nWidthCardScore,nHeightCardScore,
			10*nWidthCardScore,0,nWidthCardScore,nHeightCardScore );
	}
	//����
	else
	{
		if( !bLeftAlign )
		{
			if( fCardScore >= 10.0 )
				nXDraw -= nHeightCardNum*2;
			else nXDraw -= nHeightCardNum;

			nXDraw -= nWidthCardScore*((fCardScore-(LONGLONG)fCardScore)>0?2:1)/3;
		}

		//������Ŀ
		INT nNumberCount=0;
		LONGLONG lNumber = (LONGLONG)fCardScore;
		LONGLONG lNumberTemp=lNumber;
		do
		{
			nNumberCount++;
			lNumberTemp/=10;
		} while (lNumberTemp!=0);
		nXDraw += nWidthCardNum*(nNumberCount-1);
		//�滭����
		lNumberTemp = lNumber;
		for (INT i=0;i<nNumberCount;i++)
		{
			//�滭����
			INT lCellNumber=INT(lNumberTemp%10);
			m_PngNumber.DrawImage(pDC,nXDraw,nYDraw,nWidthCardNum,nHeightCardNum,lCellNumber*nWidthCardNum,0);

			//���ñ���
			lNumberTemp/=10;
			nXDraw-=nWidthCardNum;
		}

		nXDraw += nWidthCardNum*(nNumberCount+1);
		if( (fCardScore-(LONGLONG)fCardScore) > 0 )
		{
			m_PngCardScore.DrawImage( pDC,nXDraw,nYDraw,nWidthCardScore,nHeightCardScore,
				8*nWidthCardScore,0,nWidthCardScore,nHeightCardScore );
		}
		else
		{
			m_PngCardScore.DrawImage( pDC,nXDraw,nYDraw,nWidthCardScore,nHeightCardScore,
				9*nWidthCardScore,0,nWidthCardScore,nHeightCardScore );
		}
	}
}

//�滭����
VOID CGameClientView::DrawNumberString( CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, CPngImage &PngNumber, bool bHasSign )
{
	//������Դ
	INT nNumberHeight=PngNumber.GetHeight();
	INT nNumberWidth=PngNumber.GetWidth()/(10+(bHasSign?2:0));

	//������Ŀ
	INT nNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		nNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp!=0);

	//λ�ö���
	INT nYDrawPos=INT(nYPos-nNumberHeight/2);
	INT nXDrawPos=INT(nXPos+(nNumberCount+(bHasSign?1:0))*nNumberWidth/2-nNumberWidth);

	BYTE bySignCount = bHasSign?2:0;
	//�滭����
	if( lNumber < 0L ) lNumberTemp = -lNumber;
	else lNumberTemp = lNumber;
	for (INT i=0;i<nNumberCount;i++)
	{
		//�滭����
		INT lCellNumber=INT(lNumberTemp%10);
		PngNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,(lCellNumber+bySignCount)*nNumberWidth,0);

		//���ñ���
		lNumberTemp/=10;
		nXDrawPos-=nNumberWidth;
	}
	//�滭������
	if( bHasSign )
	{
		PngNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,(lNumber>=0?0:1)*nNumberWidth,0);
	}

	return;
}


//��������
bool CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, INT nXPos, INT nYPos)
{
	//��������
	INT nStringLength=lstrlen(pszString);
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//��������
	UINT nTextAlign=pDC->GetTextAlign();
	COLORREF rcTextColor=pDC->GetTextColor();

	//�滭�߿�
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->SetTextColor(crFrame);
		TextOut(pDC,nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	TextOut(pDC,nXPos,nYPos,pszString,nStringLength);

	//��ԭ����
	pDC->SetTextAlign(nTextAlign);
	pDC->SetTextColor(rcTextColor);

	return true;
}

//����ׯ��
VOID CGameClientView::SetBankerUser( WORD wBankerUser )
{
	if( m_wBankerUser != wBankerUser )
	{
		m_wBankerUser = wBankerUser;
		RefreshGameView();
	}
}

//���õȴ�
VOID CGameClientView::SetWaitUserScore( bool bWaitUserScore )
{
	if( m_bWaitUserScore != bWaitUserScore )
	{
		m_bWaitUserScore = bWaitUserScore;
		RefreshGameView();
	}
}

//������ʾ
VOID CGameClientView::ShowAddJettonInfo( bool bShow )
{
	if( m_bShowAddJetton != bShow )
	{
		m_bShowAddJetton = bShow;
		RefreshGameView();
	}
}

//////////////////////////////////////////////////////////////////////////