#include "StdAfx.h"
#include "Resource.h"
#include "GameLogic.h"
#include "GameClientView.h"
#include "GameClientEngine.h"
//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FLASH_WINNER			100									//������ʶ
#define IDI_SHOW_CHANGE_BANKER		101									//�ֻ�ׯ��
#define IDI_DISPATCH_CARD			102									//���Ʊ�ʶ
#define IDI_BOMB_EFFECT				200									//��ը��ʶ


//��ť��ʶ
#define IDC_JETTON_BUTTON_100		200									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000		201									//��ť��ʶ
#define IDC_JETTON_BUTTON_10000		202									//��ť��ʶ
#define IDC_JETTON_BUTTON_100000	203									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000000	204									//��ť��ʶ
#define IDC_JETTON_BUTTON_5000000	205									//��ť��ʶ
#define IDC_APPY_BANKER				206									//��ť��ʶ
#define IDC_CANCEL_BANKER			207									//��ť��ʶ
#define IDC_SCORE_MOVE_L			209									//��ť��ʶ
#define IDC_SCORE_MOVE_R			210									//��ť��ʶ
#define IDC_VIEW_CHART				211									//��ť��ʶ
#define IDC_ADMIN					212									//��ť��ʶ


//��ը��Ŀ
#define BOMB_EFFECT_COUNT			8									//��ը��Ŀ

void ViewMakeString(CString &strNum,LONGLONG lNumber)
{
	CString strTempNum;
	strTempNum.Format(_T("%I64d"), (lNumber>0?lNumber:-lNumber));

	int nLength = strTempNum.GetLength();
	for (int i = 0; i < int((nLength-1)/3); i++)
		strTempNum.Insert(nLength - 3*(i+1), _T(","));
	strNum.Format(_T("%s%s"), (lNumber<0?_T("-"):_T("")), strTempNum);
	return;
}


//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//ȫ����ע
	ZeroMemory(m_lAreaInAllScore,sizeof(m_lAreaInAllScore));

	//��ע��Ϣ
	ZeroMemory(m_lMeInAllScore,sizeof(m_lMeInAllScore));

	//���Ʊ���
	ZeroMemory(m_cbSendCount,sizeof(m_cbSendCount));

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;

	//���ֳɼ�
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	//��������
	ZeroMemory(m_bBombEffect,sizeof(m_bBombEffect));
	ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));

	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbWinnerSide=0xFF;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	m_bFlashResult = false;
	m_blCanStore=false;

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	//��ʷ�ɼ�
	m_lMeStatisticScore=0;

	//�ؼ�����
	m_pGameClientDlg=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	
	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewFill.LoadFromResource(hInstance,IDB_VIEW_FILL);
	m_ImageViewBack.LoadFromResource(hInstance,IDB_VIEW_BACK);
	m_ImageWinFlags.LoadFromResource(hInstance,IDB_WIN_FLAGS);
	m_ImageJettonView.LoadFromResource(hInstance,IDB_JETTOM_VIEW);
	m_ImageScoreNumber.LoadFromResource(hInstance,IDB_SCORE_NUMBER);
	m_ImageMeScoreNumber.LoadFromResource(hInstance,IDB_ME_SCORE_NUMBER);


	m_ImageFrame[AREA_LONG].LoadFromResource(hInstance, IDB_K_LONG );
	m_ImageFrame[AREA_PING].LoadFromResource( hInstance,IDB_K_HE );
	m_ImageFrame[AREA_HU].LoadFromResource( hInstance,IDB_K_HU );
	m_ImageFrame[AREA_2_13].LoadFromResource(hInstance, IDB_K_2_13 );
	m_ImageFrame[AREA_14].LoadFromResource(hInstance, IDB_K_14 );
	m_ImageFrame[AREA_15_26].LoadFromResource(hInstance, IDB_K_15_26 );
	m_ImageFrame[AREA_2_6].LoadFromResource( hInstance,IDB_K_2_6 );
	m_ImageFrame[AREA_7_11].LoadFromResource(hInstance, IDB_K_7_11 );
	m_ImageFrame[AREA_12_16].LoadFromResource(hInstance, IDB_K_12_16 );
	m_ImageFrame[AREA_17_21].LoadFromResource(hInstance, IDB_K_17_21 );
	m_ImageFrame[AREA_22_26].LoadFromResource( hInstance,IDB_K_22_26 );


	m_ImageGameEnd.LoadFromResource( hInstance,IDB_GAME_END );
	m_ImageGameEndFrame.LoadFromResource( hInstance,IDB_GAME_END_FRAME );
	m_ImageGamePoint.LoadFromResource( hInstance,IDB_GAME_POINT );

	m_ImageMeBanker.LoadFromResource(hInstance, IDB_ME_BANKER );
	m_ImageChangeBanker.LoadFromResource( hInstance,IDB_CHANGE_BANKER );
	m_ImageNoBanker.LoadFromResource( hInstance,IDB_NO_BANKER );	

	m_ImageTimeFlag.LoadFromResource(hInstance,IDB_TIME_FLAG);

	m_ImageBombEffect.LoadImage(hInstance,TEXT("FIRE_EFFECT"));

	m_szViewBack.SetSize( m_ImageViewBack.GetWidth(), m_ImageViewBack.GetHeight() );

	m_pClientControlDlg = NULL;
	m_hInst = NULL;

	return;
}

//��������
CGameClientView::~CGameClientView()
{
	if( m_pClientControlDlg )
	{
		delete m_pClientControlDlg;
		m_pClientControlDlg = NULL;
	}

	if( m_hInst )
	{
		FreeLibrary(m_hInst);
		m_hInst = NULL;
	}
}


//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	AfxSetResourceHandle(AfxGetInstanceHandle());
	//�����ؼ�
	CRect rcCreate(0,0,0,0);

	m_GameRecord.Create(IDD_DLG_GAME_RECORD,this);
	m_ApplyUser.Create( IDD_DLG_GAME_RECORD	, this);
	m_GameRecord.ShowWindow(SW_HIDE);

	//��ע��ť
	m_btJetton100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100);
	m_btJetton1000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000);
	m_btJetton10000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10000);
	m_btJetton100000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100000);
	m_btJetton1000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000000);
	m_btJetton5000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_5000000);
	m_btViewChart.Create(NULL,WS_CHILD,rcCreate,this,IDC_VIEW_CHART);

	//���밴ť
	m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
	m_btCancelBanker.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);

	m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
	m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);



	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btJetton100.SetButtonImage(IDB_BT_JETTON_100,hResInstance,false,false);
	m_btJetton1000.SetButtonImage(IDB_BT_JETTON_1000,hResInstance,false,false);
	m_btJetton10000.SetButtonImage(IDB_BT_JETTON_10000,hResInstance,false,false);
	m_btJetton100000.SetButtonImage(IDB_BT_JETTON_100000,hResInstance,false,false);
	m_btJetton1000000.SetButtonImage(IDB_BT_JETTON_1000000,hResInstance,false,false);
	m_btJetton5000000.SetButtonImage(IDB_BT_JETTON_5000000,hResInstance,false,false);

	m_btViewChart.SetButtonImage(IDB_BT_CHART,hResInstance,false,false);	

	m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false,false);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,hResInstance,false,false);

	m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false,false);
	m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false,false);

	//�˿˿ؼ�
	m_CardControl[INDEX_LONG].SetDisplayFlag(true);
	m_CardControl[INDEX_HU].SetDisplayFlag(true);

#ifdef __BANKER___
	m_btBankerStorage.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK_STORAGE);
	m_btBankerDraw.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK_DRAW);

	m_btBankerStorage.SetButtonImage(IDB_BT_STORAGE,hResInstance,false,false);
	m_btBankerDraw.SetButtonImage(IDB_BT_DRAW,hResInstance,false,false);
#endif

	//·���ؼ�
	if (m_DlgViewChart.m_hWnd==NULL) m_DlgViewChart.Create(IDD_VIEW_CHART,this);


	//���ư�ť
	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);

	//����
	m_hInst = NULL;
	m_pClientControlDlg = NULL;
	m_hInst = LoadLibrary(TEXT("LongHuDouClientControl.dll"));
	if ( m_hInst )
	{
		typedef void * (*CREATE)(CWnd* pParentWnd); 
		CREATE ClientControl = (CREATE)GetProcAddress(m_hInst,"CreateClientControl"); 
		if ( ClientControl )
		{
			m_pClientControlDlg = static_cast<IClientControlDlg*>(ClientControl(this));
			m_pClientControlDlg->ShowWindow( SW_HIDE );
		}
	}
	return 0;
}


//���ý���
VOID CGameClientView::ResetGameView()
{
	//ȫ����ע
	ZeroMemory(m_lAreaInAllScore,sizeof(m_lAreaInAllScore));

	//��ע��Ϣ
	ZeroMemory(m_lMeInAllScore,sizeof(m_lMeInAllScore));

	//���Ʊ���
	ZeroMemory(m_cbSendCount,sizeof(m_cbSendCount));

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;

	//���ֳɼ�
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	//��������
	ZeroMemory(m_bBombEffect,sizeof(m_bBombEffect));
	ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));

	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbWinnerSide=0xFF;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	m_blCanStore=false;

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;		

	m_lAreaLimitScore=0L;	

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	//��ʷ�ɼ�
	m_lMeStatisticScore=0;

	//���Ʊ���
	ZeroMemory(m_cbSendCount,sizeof(m_cbSendCount));

	//����б�
	m_ApplyUser.ClearAll();

	//�������
	CleanUserJetton();

	//��������
	ZeroMemory(m_bBombEffect,sizeof(m_bBombEffect));
	ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));

	//���ð�ť
	m_btApplyBanker.ShowWindow(SW_SHOW);
	m_btApplyBanker.EnableWindow(FALSE);
	m_btCancelBanker.ShowWindow(SW_HIDE);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false,false);

	return;
}
//�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_JETTON_BUTTON_100:
		m_lCurrentJetton=100L;
		break;
	case IDC_JETTON_BUTTON_1000:
		m_lCurrentJetton=1000L;
		break;
	case IDC_JETTON_BUTTON_10000:
		m_lCurrentJetton=10000L;
		break;
	case IDC_JETTON_BUTTON_100000:
		m_lCurrentJetton=100000L;
		break;
	case IDC_JETTON_BUTTON_1000000:
		m_lCurrentJetton=1000000L;
		break;
	case IDC_JETTON_BUTTON_5000000:
		m_lCurrentJetton=5000000L;
		break;
	case IDC_VIEW_CHART:
			OnViewChart();
		break;
	case IDC_APPY_BANKER:
		SendEngineMessage(IDM_APPLY_BANKER,1,0);		
		break;
	case IDC_CANCEL_BANKER:
		SendEngineMessage(IDM_APPLY_BANKER,0,0);
		break;
	case IDC_SCORE_MOVE_L:
		OnScoreMoveL();
		break;
	case IDC_SCORE_MOVE_R:
		OnScoreMoveR();
		break;
	case IDC_BANK_STORAGE:
		OnBankStorage();
		break;
	case IDC_BANK_DRAW:
		OnBankDraw();
		break;
	case IDC_ADMIN:
		OpenAdminWnd();
		break;
	}
	return __super::OnCommand(wParam, lParam);
}
//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//λ����Ϣ
	m_nWinFlagsExcursionX = nWidth/2-148;	
	m_nWinFlagsExcursionY = nHeight/2 + 251;
	m_nWinPointsExcursionX = nWidth/2-353;
	m_nWinPointsExcursionY = nHeight/2 - 267;	

	//����λ��
	int nCenterX = nWidth/2, nCenterY = nHeight/2;

	//λ����Ϣ
	m_ptOffset.x = (nWidth - m_szViewBack.cx)/2;
	m_ptOffset.y = (nHeight - m_szViewBack.cy)/2;

	//����λ��
	m_rcBetAreaBox[AREA_LONG].left = m_ptOffset.x + 93;
	m_rcBetAreaBox[AREA_LONG].top = m_ptOffset.y + 193;
	m_rcBetAreaBox[AREA_LONG].right = m_rcBetAreaBox[AREA_LONG].left + 186;
	m_rcBetAreaBox[AREA_LONG].bottom = m_rcBetAreaBox[AREA_LONG].top + 125;

	m_rcBetAreaBox[AREA_PING].left = m_ptOffset.x + 276;
	m_rcBetAreaBox[AREA_PING].top = m_ptOffset.y + 193;
	m_rcBetAreaBox[AREA_PING].right = m_rcBetAreaBox[AREA_PING].left + 267;
	m_rcBetAreaBox[AREA_PING].bottom = m_rcBetAreaBox[AREA_PING].top + 125;

	m_rcBetAreaBox[AREA_HU].left = m_ptOffset.x + 540;
	m_rcBetAreaBox[AREA_HU].top = m_ptOffset.y + 193;
	m_rcBetAreaBox[AREA_HU].right = m_rcBetAreaBox[AREA_HU].left + 185;
	m_rcBetAreaBox[AREA_HU].bottom = m_rcBetAreaBox[AREA_HU].top + 125;

	m_rcBetAreaBox[AREA_2_13].left = m_ptOffset.x + 93;
	m_rcBetAreaBox[AREA_2_13].top = m_ptOffset.y + 315;
	m_rcBetAreaBox[AREA_2_13].right = m_rcBetAreaBox[AREA_2_13].left + 186;
	m_rcBetAreaBox[AREA_2_13].bottom = m_rcBetAreaBox[AREA_2_13].top + 86;

	m_rcBetAreaBox[AREA_14].left = m_ptOffset.x + 276;
	m_rcBetAreaBox[AREA_14].top = m_ptOffset.y + 315;
	m_rcBetAreaBox[AREA_14].right = m_rcBetAreaBox[AREA_14].left+ 267;
	m_rcBetAreaBox[AREA_14].bottom = m_rcBetAreaBox[AREA_14].top + 86;

	m_rcBetAreaBox[AREA_15_26].left = m_ptOffset.x + 540;
	m_rcBetAreaBox[AREA_15_26].top = m_ptOffset.y + 315;
	m_rcBetAreaBox[AREA_15_26].right = m_rcBetAreaBox[AREA_15_26].left + 185;
	m_rcBetAreaBox[AREA_15_26].bottom = m_rcBetAreaBox[AREA_15_26].top + 86;

	m_rcBetAreaBox[AREA_2_6].left = m_ptOffset.x + 93;
	m_rcBetAreaBox[AREA_2_6].top = m_ptOffset.y + 398;
	m_rcBetAreaBox[AREA_2_6].right = m_rcBetAreaBox[AREA_2_6].left + 130;
	m_rcBetAreaBox[AREA_2_6].bottom = m_rcBetAreaBox[AREA_2_6].top + 86;

	m_rcBetAreaBox[AREA_7_11].left = m_ptOffset.x + 220;
	m_rcBetAreaBox[AREA_7_11].top = m_ptOffset.y + 398;
	m_rcBetAreaBox[AREA_7_11].right = m_rcBetAreaBox[AREA_7_11].left + 131;
	m_rcBetAreaBox[AREA_7_11].bottom = m_rcBetAreaBox[AREA_7_11].top + 86;

	m_rcBetAreaBox[AREA_12_16].left = m_ptOffset.x + 348;
	m_rcBetAreaBox[AREA_12_16].top = m_ptOffset.y + 398;
	m_rcBetAreaBox[AREA_12_16].right = m_rcBetAreaBox[AREA_12_16].left + 131;
	m_rcBetAreaBox[AREA_12_16].bottom = m_rcBetAreaBox[AREA_12_16].top + 86;

	m_rcBetAreaBox[AREA_17_21].left = m_ptOffset.x + 476;
	m_rcBetAreaBox[AREA_17_21].top = m_ptOffset.y + 398;
	m_rcBetAreaBox[AREA_17_21].right = m_rcBetAreaBox[AREA_17_21].left + 129;
	m_rcBetAreaBox[AREA_17_21].bottom = m_rcBetAreaBox[AREA_17_21].top + 86;

	m_rcBetAreaBox[AREA_22_26].left = m_ptOffset.x + 602;
	m_rcBetAreaBox[AREA_22_26].top = m_ptOffset.y + 398;
	m_rcBetAreaBox[AREA_22_26].right = m_rcBetAreaBox[AREA_22_26].left + 123;
	m_rcBetAreaBox[AREA_22_26].bottom = m_rcBetAreaBox[AREA_22_26].top + 86;

	for(int i = 0 ; i < AREA_ALL; ++i)
	{
		m_rcBetArea[i].top = m_rcBetAreaBox[i].top + 3;
		m_rcBetArea[i].bottom = m_rcBetAreaBox[i].bottom - 3;
		m_rcBetArea[i].left = m_rcBetAreaBox[i].left + 3;
		m_rcBetArea[i].right = m_rcBetAreaBox[i].right - 3;
	}



	int ExcursionY=10;
	for ( int i = 0; i < AREA_ALL; ++i )
	{
		//��������
		m_PointJettonNumber[i].SetPoint((m_rcBetArea[i].right+m_rcBetArea[i].left)/2, (m_rcBetArea[i].bottom+m_rcBetArea[i].top)/2-ExcursionY);
		//����λ��
		m_PointJetton[i].SetPoint(m_rcBetArea[i].left, m_rcBetArea[i].top);
	}


	//�˿˿ؼ�
	m_CardControl[0].SetBenchmarkPos(CPoint(nWidth/2-25+17,nHeight/2-190),enXRight,enYTop);
	m_CardControl[1].SetBenchmarkPos(CPoint(nWidth/2+25-15,nHeight/2-190),enXLeft,enYTop);

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//�б�ؼ�
	DeferWindowPos(hDwp,m_ApplyUser,NULL,nWidth/2 + 118,nHeight/2-314,256,80,uFlags);

	//���밴ť
	CRect rcJetton;
	m_btJetton100.GetWindowRect(&rcJetton);
	int nYPos = nHeight/2+147;
	int nXPos = nWidth/2-130+8;
	int nSpace = 5;

	DeferWindowPos(hDwp,m_btJetton100,NULL,nXPos,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000,NULL,nXPos + nSpace + rcJetton.Width(),nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton10000,NULL,nXPos + nSpace * 2 + rcJetton.Width() * 2,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton100000,NULL,nXPos + nSpace * 3 + rcJetton.Width() * 3,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000000,NULL,nXPos + nSpace * 4 + rcJetton.Width() * 4,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton5000000,NULL,nXPos + nSpace * 5 + rcJetton.Width() * 5,nYPos,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btViewChart,NULL,nWidth/2-377,nHeight/2+180,0,0,uFlags|SWP_NOSIZE);

	//��ׯ��ť
	DeferWindowPos(hDwp,m_btApplyBanker,NULL,nWidth/2+280,nHeight/2-352,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancelBanker,NULL,nWidth/2+280,nHeight/2-352,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btScoreMoveL,NULL,nWidth/2-203,nHeight/2+272,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScoreMoveR,NULL,nWidth/2+302,nHeight/2+270,0,0,uFlags|SWP_NOSIZE);

#ifdef __BANKER___
	m_btBankerDraw.GetWindowRect(&rcJetton);
	DeferWindowPos(hDwp,m_btBankerStorage,NULL,nWidth/2-370,nHeight/2+192,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBankerDraw,NULL,nWidth/2-310,nHeight/2+192,0,0,uFlags|SWP_NOSIZE);
#endif
	//�����ƶ�
	EndDeferWindowPos(hDwp);

	return;
}


//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageViewBack,DRAW_MODE_CENTENT);
	
	//��ȡ״̬
	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();

	//״̬��ʾ
	CFont static InfoFont;
	InfoFont.CreateFont(-16,0,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
	CFont * pOldFont=pDC->SelectObject(&InfoFont);
	pDC->SetTextColor(RGB(255,234,0));
	CRect rcDispatchCardTips(m_nWinPointsExcursionX, m_nWinPointsExcursionY+50, m_nWinPointsExcursionX + 220, m_nWinPointsExcursionY+115);

	if (m_pGameClientDlg->GetGameStatus()==GAME_SCENE_PLACE_JETTON)
		pDC->DrawText( TEXT("��Ϸ��ʼ�ˣ������·�"), rcDispatchCardTips, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_WORDBREAK );
	//else if (m_pGameClientDlg->GetGameStatus()==GS_GAME_END)
	//	pDC->DrawText( m_strDispatchCardTips, rcDispatchCardTips, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_WORDBREAK );

	pDC->SelectObject(pOldFont);
	InfoFont.DeleteObject();

	//ʱ����ʾ

	int nTimeFlagWidth = m_ImageTimeFlag.GetWidth()/3;
	int nFlagIndex=0;
	if (cbGameStatus==GAME_STATUS_FREE) 
		nFlagIndex=0;
	else if (cbGameStatus==GAME_SCENE_PLACE_JETTON) 
		nFlagIndex=1;
	else if (cbGameStatus==GAME_SCENE_GAME_END)
		nFlagIndex=2;

	m_ImageTimeFlag.BitBlt(pDC->GetSafeHdc(), nWidth/2-230 + 25, nHeight/2+195, nTimeFlagWidth, m_ImageTimeFlag.GetHeight(),
		nFlagIndex * nTimeFlagWidth, 0);

	//�����ע
	pDC->SetTextColor(RGB(255,234,0));
	LONGLONG lLimitScore=GetMaxPlayerScore(AREA_LONG);
	if ( !m_bEnableSysBanker && m_wBankerUser == INVALID_CHAIR )
		lLimitScore = 0l;
	DrawNumberStringWithSpace(pDC,lLimitScore,nWidth/2-300, nHeight/2 - 347);

	lLimitScore=GetMaxPlayerScore(AREA_HU);
	if ( !m_bEnableSysBanker && m_wBankerUser == INVALID_CHAIR )
		lLimitScore = 0l;
	DrawNumberStringWithSpace(pDC,lLimitScore,nWidth/2-300, nHeight/2 - 322);

	lLimitScore=GetMaxPlayerScore(AREA_PING);
	if ( !m_bEnableSysBanker && m_wBankerUser == INVALID_CHAIR )
		lLimitScore = 0l;
	DrawNumberStringWithSpace(pDC,lLimitScore,nWidth/2-300, nHeight/2 - 299);

	//ʤ���߿�
	FlashJettonAreaFrame(nWidth,nHeight,pDC);

	//������Դ
	CSize SizeJettonItem(m_ImageJettonView.GetWidth()/JETTON_COUNT,m_ImageJettonView.GetHeight());

	//�滭����
	for (INT i=0;i<AREA_ALL;i++)
	{
		//��������
		LONGLONG lScoreCount=0L;
		LONGLONG lScoreJetton[JETTON_COUNT]={100L,1000L,10000L,100000L,1000000L,5000000L};

		//�滭����
		for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
		{
			//��ȡ��Ϣ
			tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];

			//�ۼ�����
			ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
			lScoreCount+=lScoreJetton[pJettonInfo->cbJettonIndex];

			//�滭����
			m_ImageJettonView.TransDrawImage(pDC,pJettonInfo->nXPos+m_PointJetton[i].x,
				pJettonInfo->nYPos+m_PointJetton[i].y,SizeJettonItem.cx,SizeJettonItem.cy,
				pJettonInfo->cbJettonIndex*SizeJettonItem.cx,0,RGB(255,0,255) );
		}

		//�滭����
		if (lScoreCount>0L)	DrawNumberString(pDC,lScoreCount,m_PointJettonNumber[i].x,m_PointJettonNumber[i].y);
	}

	//ׯ����Ϣ																											
	pDC->SetTextColor(RGB(255,234,0));
	
	//��ȡ���
	IClientUserItem *pUserItem =NULL;
	tagUserInfo  *pUserData = NULL;
	if(m_wBankerUser != INVALID_CHAIR)
		pUserItem= GetClientUserItem(m_wBankerUser);
	if(pUserItem != NULL)
	{
		pUserData = pUserItem->GetUserInfo();
	}
	
	//λ����Ϣ
	CRect static StrRect;
	StrRect.left = nWidth/2-85;
	StrRect.top = nHeight/2 - 340;
	StrRect.right = StrRect.left + 100;
	StrRect.bottom = StrRect.top + 15;
	

	//ׯ������
	pDC->DrawText(pUserData == NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("������ׯ")):pUserData->szNickName, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP| DT_SINGLELINE );

	//ׯ���ܷ�
	StrRect.left = nWidth/2-85;
	StrRect.top = nHeight/2 - 316;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC,pUserData==NULL?0:pUserData->lScore, StrRect);

	//ׯ�Ҿ���
	StrRect.left = nWidth/2-85;
	StrRect.top = nHeight/2 - 266;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC,m_wBankerTime,StrRect);

	//ׯ�ҳɼ�
	StrRect.left = nWidth/2-85;
	StrRect.top = nHeight/2 - 291;
	StrRect.right = StrRect.left + 190;
	StrRect.bottom = StrRect.top + 15;
	DrawNumberStringWithSpace(pDC,m_lBankerWinScore,StrRect);

	//�滭�û�
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		IClientUserItem *pUserItem =GetClientUserItem(m_wMeChairID);

		tagUserInfo *pMeUserData = NULL;
		if(pUserItem!=NULL)
			pMeUserData = pUserItem->GetUserInfo();
		if ( pMeUserData != NULL )
		{
			//��Ϸ��Ϣ
			pDC->SetTextColor(RGB(255,255,255));


			LONGLONG lMeJetton = 0;
			for (int nAreaIndex = 0; nAreaIndex < AREA_ALL; ++nAreaIndex ) 
				lMeJetton += m_lMeInAllScore[nAreaIndex];

			CRect rcAccount(CPoint(nWidth/2-297,nHeight/2+268),CPoint(nWidth/2-297+73,nHeight/2+272+15));
			CRect rcGameScore(CPoint(nWidth/2-297,nHeight/2+294),CPoint(nWidth/2-297+73,nHeight/2+294+10));
			CRect rcResultScore(CPoint(nWidth/2-297,nHeight/2+294+23),CPoint(nWidth/2-297+73,nHeight/2+294+10+23));

			CString strM;
			ViewMakeString(strM, pMeUserData->lScore-lMeJetton);
			pDC->DrawText(strM,lstrlen(strM),rcGameScore,DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);

			ViewMakeString(strM, m_lMeStatisticScore);
			pDC->DrawText(strM,lstrlen(strM),rcResultScore,DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);

			//DrawNumberStringWithSpace(pDC,pMeUserData->lScore-lMeJetton,rcGameScore,DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);
			//DrawNumberStringWithSpace(pDC,m_lMeStatisticScore,rcResultScore,DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);
			pDC->DrawText(pMeUserData->szNickName,lstrlen(pMeUserData->szNickName),rcAccount,DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS);
		}
	}

	//�л�ׯ��
	if ( m_bShowChangeBanker )
	{
		int	nXPos = nWidth / 2 - 130;
		int	nYPos = nHeight / 2 - 160;

		//������ׯ
		if ( m_wMeChairID == m_wBankerUser )
		{
			m_ImageMeBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
		else if ( m_wBankerUser != INVALID_CHAIR )
		{
			m_ImageChangeBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
		else
		{
			m_ImageNoBanker.BitBlt(pDC->GetSafeHdc(), nXPos, nYPos);
		}
	}

	//�ҵ���ע
	DrawMeJettonNumber(pDC);

	//�滭ʱ��
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		WORD wUserTimer=GetUserClock(m_wMeChairID);
		if (wUserTimer!=0) 
			DrawUserClock(pDC,nWidth/2-255 + 25,nHeight/2-260+435,wUserTimer);
	}

	//����״̬
	if (cbGameStatus==GAME_SCENE_GAME_END)
	{
		int	nXPos = nWidth / 2 - 129;
		int	nYPos = nHeight / 2 - 208;

		CRect rcAlpha(nXPos, nYPos, nXPos+ m_ImageGameEndFrame.GetWidth(), nYPos+m_ImageGameEndFrame.GetHeight());
		DrawAlphaRect(pDC, &rcAlpha, RGB(74,70,73), 0.8f);

		//�滭�߿�
		m_ImageGameEndFrame.TransDrawImage(pDC, nXPos, nYPos, RGB(255,0,255));

		//�滭�˿�
		m_CardControl[INDEX_LONG].DrawCardControl(pDC);
		m_CardControl[INDEX_HU].DrawCardControl(pDC);

		//�����˿�
		if (m_ptDispatchCard.x!=0 && m_ptDispatchCard.y!=0) 
		{
			m_DispatchCard.SetBenchmarkPos(m_ptDispatchCard.x,m_ptDispatchCard.y,enXCenter,enYCenter);
			m_DispatchCard.DrawCardControl(pDC);
		}		
	}

	//ʤ����־
	DrawWinFlags(pDC);

	//��ʾ���
	ShowGameResult(pDC, nWidth, nHeight);	

	//�˿˵���
	if (m_cbSendCount[INDEX_LONG]+m_cbSendCount[INDEX_HU]!=0)
	{
		//��������
		BYTE cbPlayerPoint, cbBankerPoint;
		int	nXPos = nWidth / 2 - 129;
		int	nYPos = nHeight / 2 - 208;		

		//�滭����
		int nPointWidth = m_ImageGamePoint.GetWidth() / 14;
		if (m_cbSendCount[INDEX_LONG]!=0)
		{
			//�������
			cbPlayerPoint = m_GameLogic.GetCardPip(m_cbTableCardArray[INDEX_LONG][0]);
			m_ImageGamePoint.TransDrawImage(pDC, nXPos + 28, nYPos + 10, nPointWidth, m_ImageGamePoint.GetHeight(),
				cbPlayerPoint * nPointWidth, 0, RGB(255, 0, 255));
		}
		if (m_cbSendCount[INDEX_HU]!=0)
		{
			//�������
			cbBankerPoint = m_GameLogic.GetCardPip(m_cbTableCardArray[INDEX_HU][0]);
			m_ImageGamePoint.TransDrawImage(pDC, nXPos + 28 + 136, nYPos + 10, nPointWidth, m_ImageGamePoint.GetHeight(),
				cbBankerPoint * nPointWidth, 0, RGB(255, 0, 255));
		}
	}

	//��ըЧ��
	DrawBombEffect(pDC);

	return;
}

//������Ϣ
void CGameClientView::SetMeMaxScore(LONGLONG lMeMaxScore)
{
	if (m_lMeMaxScore!=lMeMaxScore)
	{
		//���ñ���
		m_lMeMaxScore=lMeMaxScore;
	}

	return;
}

//�����ע
void CGameClientView::SetAreaLimitScore(LONGLONG lAreaLimitScore)
{
	if ( m_lAreaLimitScore!= lAreaLimitScore )
	{
		//���ñ���
		m_lAreaLimitScore=lAreaLimitScore;
	}
}

//���ó���
void CGameClientView::SetCurrentJetton(LONGLONG lCurrentJetton)
{
	//���ñ���
	ASSERT(lCurrentJetton >= 0L);
	m_lCurrentJetton = lCurrentJetton;

	if ( m_lCurrentJetton == 0l )
	{
		m_cbAreaFlash = 0xFF;
	}

	return;
}

//��ʷ��¼
void CGameClientView::SetGameHistory(enOperateResult OperateResult, BYTE cbResult, BYTE cbLong, BYTE cbHu)
{
	//��������
	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.enOperateFlags = OperateResult;
	GameRecord.cbResult = cbResult;
	GameRecord.cbLong = cbLong;
	GameRecord.cbHu = cbHu;

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
	{
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
		if ( m_nScoreHead < m_nRecordFirst ) m_nScoreHead = m_nRecordFirst;
	}

	int nHistoryCount = (m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( 12 < nHistoryCount ) m_btScoreMoveR.EnableWindow(TRUE);

	//�Ƶ����¼�¼
	if ( 12 < nHistoryCount )
	{
		m_nScoreHead = (m_nRecordLast - 12 + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
		m_btScoreMoveL.EnableWindow(TRUE);
		m_btScoreMoveR.EnableWindow(FALSE);
	}

	//����·��
	if (m_DlgViewChart.m_hWnd!=NULL)
	{
		m_DlgViewChart.SetGameRecord(GameRecord);
	}

	return;
}

//�������
void CGameClientView::CleanUserJetton()
{
	//��������
	for (BYTE i=0;i<CountArray(m_JettonInfoArray);i++)
	{
		m_JettonInfoArray[i].RemoveAll();
	}

	//ȫ����ע
	ZeroMemory(m_lAreaInAllScore,sizeof(m_lAreaInAllScore));

	//��ע��Ϣ
	ZeroMemory(m_lMeInAllScore,sizeof(m_lMeInAllScore));
	m_strDispatchCardTips=TEXT("");

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//������ע
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<AREA_ALL);
	if (cbViewIndex>=AREA_ALL) return;

	m_lMeInAllScore[cbViewIndex] = lJettonCount;

	//���½���
	InvalidGameView(0,0,0,0);
}

//�����˿�
void CGameClientView::SetCardInfo(BYTE cbCardCount[2], BYTE cbTableCardArray[2][3])
{
	if (cbCardCount!=NULL)
	{
		m_cbCardCount[0]=cbCardCount[0];
		m_cbCardCount[1]=cbCardCount[1];
		CopyMemory(m_cbTableCardArray,cbTableCardArray,sizeof(m_cbTableCardArray));

		//��ʼ����
		DispatchCard();
	}
	else
	{
		ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
		ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));
	}
}

//���ó���
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount)
{
	//Ч�����
	ASSERT(cbViewIndex<AREA_ALL);
	if (cbViewIndex>=AREA_ALL) return;

	//����ը��
	if (lScoreCount==5000000L) SetBombEffect(true,cbViewIndex);

	//��������
	bool bPlaceJetton=false;
	LONGLONG lScoreIndex[JETTON_COUNT]={100L,1000L,10000L,100000L,1000000L,5000000L};

	//�߿���
	int nFrameWidth=0, nFrameHeight=0;
	int nBorderWidth=6;

	m_lAreaInAllScore[cbViewIndex] += lScoreCount;

	nFrameWidth = m_rcBetArea[cbViewIndex].right-m_rcBetArea[cbViewIndex].left;
	nFrameHeight = m_rcBetArea[cbViewIndex].bottom-m_rcBetArea[cbViewIndex].top;

	nFrameWidth += nBorderWidth;
	nFrameHeight += nBorderWidth;

	//�����ж�
	bool bAddJetton=lScoreCount>0?true:false;

	if ( lScoreCount < 0 )
	{
		lScoreCount = -lScoreCount;
	}
	//���ӳ���
	for (BYTE i=0;i<CountArray(lScoreIndex);i++)
	{
		//������Ŀ
		BYTE cbScoreIndex=JETTON_COUNT-i-1;
		LONGLONG lCellCount=lScoreCount/lScoreIndex[cbScoreIndex];

		//�������
		if (lCellCount==0L) continue;

		//�������
		for (LONGLONG j=0;j<lCellCount;j++)
		{
			if (true==bAddJetton)
			{
				//�������
				tagJettonInfo JettonInfo;
				int nJettonSize=68;
				JettonInfo.cbJettonIndex=cbScoreIndex;
				JettonInfo.nXPos=rand()%(nFrameWidth-nJettonSize);
				JettonInfo.nYPos=rand()%(nFrameHeight-nJettonSize);

				//��������
				bPlaceJetton=true;
				m_JettonInfoArray[cbViewIndex].Add(JettonInfo);
			}
			else
			{
				for (int nIndex=0; nIndex<m_JettonInfoArray[cbViewIndex].GetCount(); ++nIndex)
				{
					//�Ƴ��ж�
					tagJettonInfo &JettonInfo=m_JettonInfoArray[cbViewIndex][nIndex];
					if (JettonInfo.cbJettonIndex==cbScoreIndex)
					{
						m_JettonInfoArray[cbViewIndex].RemoveAt(nIndex);
						break;
					}
				}
			}
		}

		//������Ŀ
		lScoreCount-=lCellCount*lScoreIndex[cbScoreIndex];
	}

	//���½���
	if (bPlaceJetton==true) 	InvalidGameView(0,0,0,0);

	return;
}

//���ֳɼ�
void CGameClientView::SetCurGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore, LONGLONG lGameRevenue)
{
	m_lMeCurGameScore=lMeCurGameScore;			
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;			
	m_lBankerCurGameScore=lBankerCurGameScore;			
	m_lGameRevenue=lGameRevenue;					
}

//����ʤ��
void CGameClientView::SetWinnerSide(BYTE cbWinnerSide,bool blSet)
{
	//���ñ���
	m_cbWinnerSide=cbWinnerSide;
	m_cbAreaFlash=cbWinnerSide;
	m_bFlashResult=blSet;

	//����ʱ��
	if (cbWinnerSide!=0xFF)
	{
		SetTimer(IDI_FLASH_WINNER,500,NULL);
	}
	else KillTimer(IDI_FLASH_WINNER);

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//��ȡ����
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	for ( int i = 0 ; i < AREA_ALL ; ++i)
	{
		if (m_rcBetArea[i].PtInRect(MousePoint)) 
			return i;
	}
	return 0xFF;
}

//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool bMeScore)
{
	//������Դ
	CSize SizeScoreNumber(m_ImageScoreNumber.GetWidth()/10,m_ImageScoreNumber.GetHeight());

	if ( bMeScore ) SizeScoreNumber.SetSize(m_ImageMeScoreNumber.GetWidth()/10, m_ImageMeScoreNumber.GetHeight());

	//������Ŀ
	INT lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//λ�ö���
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=nXPos+lNumberCount*SizeScoreNumber.cx/2-SizeScoreNumber.cx;

	//�滭����
	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		INT lCellNumber=(INT)(lNumber%10);
		if ( bMeScore )
		{
			m_ImageMeScoreNumber.TransDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
		}
		else
		{
			m_ImageScoreNumber.TransDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
		}

		//���ñ���
		lNumber/=10;
		nXDrawPos-=SizeScoreNumber.cx;
	};

	return;
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
{
	CString strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONGLONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	while (lNumber>0)
	{
		strTmpNumber1.Format(TEXT("%ld"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if (nNumberCount==4)
		{
			strTmpNumber2 += (TEXT(" ") +strNumber);
			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		strTmpNumber2 += (TEXT(" ") +strNumber);
		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	//�������
	TextOut(pDC,nXPos,nYPos,strNumber,lstrlen(strNumber));
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, LONGLONG lNumber, CRect rcRect, INT nFormat)
{
	CString strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	if (lNumber==0) strNumber=TEXT("0");
	int nNumberCount=0;
	LONGLONG lTmpNumber=lNumber;
	if (lNumber<0) lNumber=-lNumber;
	while (lNumber>0)
	{
		strTmpNumber1.Format(TEXT("%ld"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if (nNumberCount==4)
		{
			strTmpNumber2 += (TEXT(" ") +strNumber);
			strNumber=strTmpNumber2;
			nNumberCount=0;
			strTmpNumber2=TEXT("");
		}
		lNumber/=10;
	}

	if (strTmpNumber2.IsEmpty()==FALSE)
	{
		strTmpNumber2 += (TEXT(" ") +strNumber);
		strNumber=strTmpNumber2;
	}

	if (lTmpNumber<0) strNumber=TEXT("-")+strNumber;

	//�������
	if (nFormat==-1) pDC->DrawText(strNumber,rcRect,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
	else pDC->DrawText(strNumber,rcRect,nFormat);
}
//�鿴·��
void CGameClientView::OnViewChart()
{
	if (m_DlgViewChart.m_hWnd==NULL) m_DlgViewChart.Create(IDD_VIEW_CHART,this);

	//��ʾ�ж�
	if (!m_DlgViewChart.IsWindowVisible())
	{
		m_DlgViewChart.ShowWindow(SW_SHOW);
		m_DlgViewChart.CenterWindow();
	}
	else
	{
		m_DlgViewChart.ShowWindow(SW_HIDE);
	}
}

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//����ʤ��
	if (nIDEvent==IDI_FLASH_WINNER)
	{
		//���ñ���
		if (m_cbAreaFlash!=m_cbWinnerSide)
		{
			m_cbAreaFlash=m_cbWinnerSide;
		}
		else 
		{
			m_cbAreaFlash=0xFF;
		}
		//���ñ���
		m_bFlashResult=!m_bFlashResult;

		//���½���
		InvalidGameView(0,0,0,0);

		return;
	}

	//�ֻ�ׯ��
	if ( nIDEvent == IDI_SHOW_CHANGE_BANKER )
	{
		ShowChangeBanker( false );

		return;
	}

	else if (nIDEvent==IDI_DISPATCH_CARD)
	{
		bool bPlayerCard=false;
		if ((m_cbSendCount[0]+m_cbSendCount[1])<4)
		{
			//��������
			WORD wIndex=(m_cbSendCount[0]+m_cbSendCount[1])%2;
			if (wIndex==0) bPlayerCard=true;
		}
		else if (m_cbSendCount[INDEX_LONG]<m_cbCardCount[INDEX_LONG]) bPlayerCard=true;

		//�����ж�
		CRect rcView;
		GetClientRect(&rcView);
		int nEndPosX = 0;
		if (bPlayerCard) nEndPosX=rcView.Width()/2;
		else nEndPosX=rcView.Width()/2+110;
		int nEndPosY = rcView.Height()/2-185;
		if ( (m_ptDispatchCard.x > nEndPosX || m_ptDispatchCard.y < nEndPosY) &&
			m_ptDispatchCard.x != 0 && m_ptDispatchCard.y != 0 )
		{
			//�޸�λ��
			if (m_ptDispatchCard.y < nEndPosY) m_ptDispatchCard.y += 73; 
			else m_ptDispatchCard.x -= 115;

			//���½���
			InvalidGameView(0,0,0,0);

			return;
		}

		//ͣ���˿�
		static BYTE cbStopCount=0;
		if (cbStopCount<6)
		{			
			cbStopCount++;
			return;
		}
		else 
		{
			cbStopCount=0;			
		}

		if ( m_cbSendCount[INDEX_LONG]+m_cbSendCount[INDEX_HU]<m_cbCardCount[INDEX_LONG] + m_cbCardCount[INDEX_HU]-1 ) 
		{
			m_ptDispatchCard.x = rcView.Width()/2+220;
			m_ptDispatchCard.y = rcView.Height()/2-200;	
		}
		else
		{
			m_DispatchCard.SetCardData(NULL,0);
		}
	
		//��������
		m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"));

		//�����˿�
		if ((m_cbSendCount[0]+m_cbSendCount[1])<2)
		{
			//��������
			WORD wIndex=(m_cbSendCount[0]+m_cbSendCount[1])%2;

			//�����˿�
			m_cbSendCount[wIndex]++;
			m_CardControl[wIndex].SetCardData(m_cbTableCardArray[wIndex],m_cbSendCount[wIndex]);

			//������ʾ
			SetDispatchCardTips();

			//���½���
			InvalidGameView(0,0,0,0);

			return;
		}

		m_ptDispatchCard.x = 0;
		m_ptDispatchCard.y = 0;

		//��ɷ���
		FinishDispatchCard();

		//���½���
		InvalidGameView(0,0,0,0);

		return;
	}

	//��ը����
	if (nIDEvent<=IDI_BOMB_EFFECT+AREA_22_26 && IDI_BOMB_EFFECT+AREA_LONG<=nIDEvent)
	{
		WORD wIndex=nIDEvent-IDI_BOMB_EFFECT;
		//ֹͣ�ж�
		if (m_bBombEffect[wIndex]==false)
		{
			KillTimer(nIDEvent);
			return;
		}

		//���ñ���
		if ((m_cbBombFrameIndex[wIndex]+1)>=BOMB_EFFECT_COUNT)
		{
			//ɾ��ʱ��
			KillTimer(nIDEvent);

			//���ñ���
			m_bBombEffect[wIndex]=false;
			m_cbBombFrameIndex[wIndex]=0;
		}
		else m_cbBombFrameIndex[wIndex]++;

		//���½���
		InvalidGameView(0,0,0,0);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	if (m_lCurrentJetton != 0L)
	{
		BYTE cbJettonArea = GetJettonArea(Point);

		LONGLONG lMaxPlayerScore = GetMaxPlayerScore(cbJettonArea);

		if ( lMaxPlayerScore < m_lCurrentJetton )
		{
			return;
		}

		if (cbJettonArea != 0xFF) 
			SendEngineMessage(IDM_PLACE_JETTON,cbJettonArea,(LPARAM)&m_lCurrentJetton);
	}

	__super::OnLButtonDown(nFlags,Point);
}

//�����Ϣ
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//���ñ���
	m_lCurrentJetton=0L;
	
	if (m_pGameClientDlg->GetGameStatus()!=GAME_SCENE_GAME_END && m_cbAreaFlash!=0xFF)
	{
		m_cbAreaFlash = 0xFF;
		InvalidGameView(0,0,0,0);
	}

	__super::OnLButtonDown(nFlags,Point);
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	if (m_lCurrentJetton != 0L)
	{
		//��ȡ����
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		BYTE cbJettonArea=GetJettonArea(MousePoint);

		//���ñ���
		if ( m_cbAreaFlash!= cbJettonArea )
		{
			m_cbAreaFlash = cbJettonArea;
			InvalidGameView(0,0,0,0);
		}


		LONGLONG lMaxPlayerScore = 0l;
		if ( cbJettonArea != 0xFF)
		{
			lMaxPlayerScore = GetMaxPlayerScore(cbJettonArea);
		}

		//�����ж�
		if (cbJettonArea==0xFF)
		{
			SetCursor(LoadCursor(NULL,IDC_ARROW));
			return TRUE;
		}

		if ( lMaxPlayerScore < m_lCurrentJetton && cbJettonArea != 0xFF)
		{
			m_cbAreaFlash = 0xFF;
			SetCursor(LoadCursor(NULL,IDC_NO));
			return TRUE;
		}

		//���ù��
		switch (m_lCurrentJetton)
		{
		case 100:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100)));
				return TRUE;
			}
		case 1000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000)));
				return TRUE;
			}
		case 10000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_10000)));
				return TRUE;
			}
		case 100000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100000)));
				return TRUE;
			}
		case 1000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000000)));
				return TRUE;
			}
		case 5000000:
			{
				SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_5000000)));
				return TRUE;
			}
		default:
			{
				m_cbAreaFlash = 0xFF;
				return TRUE;
			}
		}
	}
	else
	{
		m_cbAreaFlash = 0xFF;
	}

	return __super::OnSetCursor(pWnd, nHitTest, uMessage);
}
//�ֻ�ׯ��
void CGameClientView::ShowChangeBanker( bool bChangeBanker )
{
	//�ֻ�ׯ��
	if ( bChangeBanker )
	{
		SetTimer( IDI_SHOW_CHANGE_BANKER, 3000, NULL );
		m_bShowChangeBanker = true;
	}
	else
	{
		KillTimer( IDI_SHOW_CHANGE_BANKER );
		m_bShowChangeBanker = false ;
	}

	//���½���
	InvalidGameView(0,0,0,0);
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
//ׯ����Ϣ
void CGameClientView::SetBankerInfo(WORD wBankerUser, LONGLONG lBankerScore) 
{
	//�л��ж�
	if (m_wBankerUser!=wBankerUser)
	{
		m_wBankerUser=wBankerUser;
		m_wBankerTime=0L;
		m_lBankerWinScore=0L;	
		m_lTmpBankerWinScore=0L;
	}
	m_lBankerScore=lBankerScore;
}

//�滭��ʶ
void CGameClientView::DrawWinFlags(CDC * pDC)
{

	int nIndex = m_nScoreHead;
	COLORREF clrOld ;
	clrOld = pDC->SetTextColor(RGB(255,234,0));
	CString strPoint;
	int nDrawCount = 0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < 12 )
	{
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		int nYPos = 0;
		if ( ClientGameRecord.cbResult == AREA_LONG ) nYPos = m_nWinFlagsExcursionY ;
		else if (ClientGameRecord.cbResult == AREA_HU ) nYPos = m_nWinFlagsExcursionY + 31;
		else nYPos = m_nWinFlagsExcursionY + 2 * 31;

		int nXPos = m_nWinFlagsExcursionX + ((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY) * 37;

		int nFlagsIndex = 0;
		if ( ClientGameRecord.enOperateFlags == enOperateResult_NULL ) nFlagsIndex = 0;
		else if ( ClientGameRecord.enOperateFlags == enOperateResult_Win) nFlagsIndex = 1;
		else if ( ClientGameRecord.enOperateFlags == enOperateResult_Lost) nFlagsIndex = 2;

		m_ImageWinFlags.TransDrawImage( pDC, nXPos, nYPos, m_ImageWinFlags.GetWidth()/3 , 
			m_ImageWinFlags.GetHeight(),m_ImageWinFlags.GetWidth()/3 * nFlagsIndex, 0, RGB(255, 0, 255) );


		UINT oldAlign = pDC->SetTextAlign(TA_CENTER|TA_TOP);
		strPoint.Format(TEXT("%d"), ClientGameRecord.cbLong);
		TextOut(pDC,m_nWinPointsExcursionX + nDrawCount * 17 + 9, m_nWinPointsExcursionY, strPoint,lstrlen(strPoint));

		strPoint.Format(TEXT("%d"), ClientGameRecord.cbHu);
		TextOut(pDC,m_nWinPointsExcursionX + nDrawCount * 17 + 9, m_nWinPointsExcursionY + 21, strPoint,lstrlen(strPoint));
		pDC->SetTextAlign(oldAlign);

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
		nDrawCount++;
	}
	pDC->SetTextColor(clrOld);

	//ͳ�Ƹ���
	nIndex = m_nRecordFirst;
	int nLongCount = 0, nHuCount = 0, nPingCount = 0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ))
	{
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		if ( ClientGameRecord.cbResult == AREA_LONG ) nLongCount++;
		else if (ClientGameRecord.cbResult == AREA_HU ) nHuCount++;
		else nPingCount++;
		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
	}

	int nWidth, nHeight;
	CRect rcClient;
	GetClientRect(&rcClient);
	nWidth = rcClient.Width();
	nHeight = rcClient.Height();
	CFont font;
	VERIFY(font.CreateFont(25,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,TEXT("Arial")));                

	pDC->SetTextColor(RGB(255,255,255));
	CFont* def_font = pDC->SelectObject(&font);
	strPoint.Format(TEXT("%d"), nLongCount);
	TextOut(pDC,nWidth/2-350+5*120+78, nHeight/2+257, strPoint,lstrlen(strPoint));
	strPoint.Format(TEXT("%d"), nHuCount);
	TextOut(pDC,nWidth/2-350+5*120+78, nHeight/2+288, strPoint,lstrlen(strPoint));
	strPoint.Format(TEXT("%d"), nPingCount);
	TextOut(pDC,nWidth/2-350+5*120+78, nHeight/2+320, strPoint,lstrlen(strPoint));
	pDC->SelectObject(def_font);
	font.DeleteObject();
}

//�ƶ���ť
void CGameClientView::OnScoreMoveL()
{
	if ( m_nRecordFirst == m_nScoreHead ) return;

	m_nScoreHead = (m_nScoreHead - 1 + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( m_nScoreHead == m_nRecordFirst ) m_btScoreMoveL.EnableWindow(FALSE);

	m_btScoreMoveR.EnableWindow(TRUE);

	//���½���
	InvalidGameView(0,0,0,0);
}

//�ƶ���ť
void CGameClientView::OnScoreMoveR()
{
	int nHistoryCount = ( m_nRecordLast - m_nScoreHead + MAX_SCORE_HISTORY ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount == 12 ) return;

	m_nScoreHead = ( m_nScoreHead + 1 ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount-1 == 12 ) m_btScoreMoveR.EnableWindow(FALSE);

	m_btScoreMoveL.EnableWindow(TRUE);

	//���½���
	InvalidGameView(0,0,0,0);
}

//��ʾ���
void CGameClientView::ShowGameResult(CDC *pDC, int nWidth, int nHeight)
{
	//��ʾ�ж�
	if (m_pGameClientDlg->GetGameStatus()!=GAME_SCENE_GAME_END || m_cbSendCount[INDEX_LONG]+m_cbSendCount[INDEX_HU]!=0 ||
		m_ptDispatchCard.x!=0 || m_ptDispatchCard.y!=0) return;

	int	nXPos = nWidth / 2 - 129;
	int	nYPos = nHeight / 2 - 208;


	//�滭�߿�
	m_ImageGameEndFrame.TransDrawImage(pDC, nXPos, nYPos, RGB(255,0,255));

	CRect rcAlpha(nXPos+2, nYPos+70, nXPos+2 + m_ImageGameEnd.GetWidth(), nYPos+70+m_ImageGameEnd.GetHeight());
	DrawAlphaRect(pDC, &rcAlpha, RGB(74,70,73), 0.8f);
	m_ImageGameEnd.TransDrawImage(pDC, nXPos+2, nYPos+70, RGB(255,0,255));

	//�������
	BYTE cbPlayerPoint, cbBankerPoint;
	cbPlayerPoint = m_GameLogic.GetCardPip(m_cbTableCardArray[INDEX_LONG][0]);

	//�������
	cbBankerPoint = m_GameLogic.GetCardPip(m_cbTableCardArray[INDEX_HU][0]);

	//�滭����
	int nPointWidth = m_ImageGamePoint.GetWidth() / 14;
	m_ImageGamePoint.TransDrawImage(pDC, nXPos + 28, nYPos + 10, nPointWidth, m_ImageGamePoint.GetHeight(),
		cbPlayerPoint * nPointWidth, 0, RGB(255, 0, 255));
	m_ImageGamePoint.TransDrawImage(pDC, nXPos + 28 + 136, nYPos + 10, nPointWidth, m_ImageGamePoint.GetHeight(),
		cbBankerPoint * nPointWidth, 0, RGB(255, 0, 255));

	pDC->SetTextColor(RGB(255,234,0));
	CRect rcMeWinScore, rcMeReturnScore;
	rcMeWinScore.left = nXPos+2 + 40;
	rcMeWinScore.top = nYPos+70 + 32;
	rcMeWinScore.right = rcMeWinScore.left + 111;
	rcMeWinScore.bottom = rcMeWinScore.top + 34;

	rcMeReturnScore.left = nXPos+2 + 150;
	rcMeReturnScore.top = nYPos+70 + 32;
	rcMeReturnScore.right = rcMeReturnScore.left + 111;
	rcMeReturnScore.bottom = rcMeReturnScore.top + 34;

	CString strMeGameScore, strMeReturnScore;
	DrawNumberStringWithSpace(pDC,m_lMeCurGameScore,rcMeWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	DrawNumberStringWithSpace(pDC,m_lMeCurGameReturnScore,rcMeReturnScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	CRect rcBankerWinScore;
	rcBankerWinScore.left = nXPos+2 + 40;
	rcBankerWinScore.top = nYPos+70 + 69;
	rcBankerWinScore.right = rcBankerWinScore.left + 111;
	rcBankerWinScore.bottom = rcBankerWinScore.top + 34;

	CString strBankerCurGameScore;
	strBankerCurGameScore.Format(TEXT("%I64d"), m_lBankerCurGameScore);
	DrawNumberStringWithSpace(pDC,m_lBankerCurGameScore,rcBankerWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}

//͸���滭
bool CGameClientView::DrawAlphaRect(CDC* pDC, LPRECT lpRect, COLORREF clr, FLOAT fAlpha)
{
	ASSERT(pDC != 0 && lpRect != 0 && clr != CLR_NONE);
	if(pDC == 0 || lpRect == 0 || clr == CLR_NONE)
	{
		return false;
	}
	//ȫ͸��
	if( abs(fAlpha) <= 0.000001 )
	{
		return true;
	}

	for(INT l=lpRect->top; l<lpRect->bottom; l++)
	{
		for(INT k=lpRect->left; k<lpRect->right; k++)
		{
			COLORREF clrBk = pDC->GetPixel(k, l);
			COLORREF clrBlend = RGB(GetRValue(clrBk)*(1-fAlpha)+GetRValue(clr)*fAlpha, 
				GetGValue(clrBk)*(1-fAlpha)+GetGValue(clr)*fAlpha,
				GetBValue(clrBk)*(1-fAlpha)+GetBValue(clr)*fAlpha);
			pDC->SetPixel(k, l, clrBlend);
		}
	}

	return true;
}

//�����ע
LONGLONG CGameClientView::GetMaxPlayerScore(BYTE cbJettonArea)
{	

	if ( cbJettonArea >= AREA_ALL )
	{
		return 0l;
	}

	LONGLONG lMaxPlayerScore = 0;

	LONGLONG lBankerScore = m_lBankerScore;

	//������
	BYTE cbMultiple[AREA_ALL] = {MULTIPLE_KONG, MULTIPLE_LONG, MULTIPLE_PING, MULTIPLE_HU, 
		MULTIPLE_2_13, MULTIPLE_14, MULTIPLE_15_26, 
		MULTIPLE_2_6, MULTIPLE_7_11, MULTIPLE_12_16, MULTIPLE_17_21, MULTIPLE_22_26};

	//ׯ���ж�
	LONGLONG lOtherAreaScore = 0;
	for ( int i = 0; i < AREA_ALL ; ++i)
	{
		lOtherAreaScore += (m_lAreaInAllScore[i]*(cbMultiple[i] - 1));
	}

	lBankerScore -= lOtherAreaScore;

	lMaxPlayerScore = m_lAreaLimitScore - m_lAreaInAllScore[cbJettonArea];

	lMaxPlayerScore = min(lMaxPlayerScore, lBankerScore/(cbMultiple[cbJettonArea] - 1));

	return lMaxPlayerScore;
}


//�ɼ�����
void CGameClientView::SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore)
{
	m_lMeCurGameScore=lMeCurGameScore;
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;
	m_lBankerCurGameScore=lBankerCurGameScore;	
}

//�滭����
void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//�滭����
	for (int i = 0; i < AREA_ALL; ++i)
	{
		if (m_lMeInAllScore[i] > 0)
		{
			DrawNumberString(pDC,m_lMeInAllScore[i],m_PointJettonNumber[i].x,m_PointJettonNumber[i].y+25, true);
		}
	}
}

//��ʼ����
void CGameClientView::DispatchCard()
{
	//���ñ���
	m_cbSendCount[INDEX_LONG]=0;
	m_cbSendCount[INDEX_HU]=0;

	//���ý���
	m_CardControl[INDEX_LONG].SetCardData(NULL,0);
	m_CardControl[INDEX_HU].SetCardData(NULL,0);
	m_cbAreaFlash = 0xFF;

	//�����˿�
	BYTE cbCardData=0;
	m_DispatchCard.SetCardData(&cbCardData,1);

	//����λ��
	CRect rcView;
	GetClientRect(&rcView);
	m_ptDispatchCard.x = rcView.Width()/2+220;
	m_ptDispatchCard.y = rcView.Height()/2-200;

	//���ö�ʱ��
	SetTimer(IDI_DISPATCH_CARD,2,NULL);

	//���ñ�ʶ
	m_bNeedSetGameRecord=true;
}

//��������
void CGameClientView::FinishDispatchCard()
{
	//����ж�
	if (m_bNeedSetGameRecord==false) return;

	//���ñ�ʶ
	m_bNeedSetGameRecord=false;

	//ɾ����ʱ��
	KillTimer(IDI_DISPATCH_CARD);

	//�����˿�
	m_CardControl[INDEX_LONG].SetCardData(m_cbTableCardArray[0],m_cbCardCount[0]);
	m_CardControl[INDEX_HU].SetCardData(m_cbTableCardArray[1],m_cbCardCount[1]);
	m_DispatchCard.SetCardData(NULL,0);

	//�������
	ZeroMemory(m_cbSendCount,sizeof(m_cbSendCount));

	//��������
	enOperateResult OperateResult = enOperateResult_NULL;
	if (0 < m_lMeCurGameScore) OperateResult = enOperateResult_Win;
	else if (m_lMeCurGameScore < 0) OperateResult = enOperateResult_Lost;
	else OperateResult = enOperateResult_NULL;

	//�ƶ�Ӯ��
	INT nAreaWin[AREA_ALL] = {0};
	BYTE cbResult = 0;
	BYTE cbAndValues = 0;
	DeduceWinner(nAreaWin, cbResult, cbAndValues);

	//�����¼
	SetGameHistory(OperateResult, cbResult,(m_cbTableCardArray[0][0]&0x0F), (m_cbTableCardArray[1][0]&0x0F));

	//�ۼƻ���
	m_lMeStatisticScore+=m_lMeCurGameScore;
	m_lBankerWinScore=m_lTmpBankerWinScore;

	//����Ӯ�� 
	// [5/6/2010 WMY]
	SetWinnerSide(cbResult);

	//��������
	if (m_lMeCurGameScore>0) m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_WIN"));
	else if (m_lMeCurGameScore<0) m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_LOST"));
	else m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_DRAW"));

}

//ʤ���߿�
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
	if (m_cbAreaFlash==0xFF && false==m_bFlashResult) return;

	//������Դ
	CSize SizeWinFlags(m_ImageWinFlags.GetWidth()/3,m_ImageWinFlags.GetHeight());

	//λ�ñ���
	int nXPos = 0;
	int nYPos = 0;
	if (false==m_bFlashResult)
	{
		m_ImageFrame[m_cbAreaFlash].TransDrawImage(pDC,m_rcBetAreaBox[m_cbAreaFlash].left, m_rcBetAreaBox[m_cbAreaFlash].top, RGB(255,0,255));

	}
	else
	{
		//�����ж�
		if (m_pGameClientDlg->GetGameStatus()==GAME_SCENE_GAME_END && m_cbSendCount[0]+m_cbSendCount[0]==0)
		{
			//�ƶ�Ӯ��
			//������Ӯ(�����ע��)
			INT nAreaWin[AREA_ALL] = {0};
			BYTE cbResult = 0;
			BYTE cbAndValues = 0;
			DeduceWinner(nAreaWin, cbResult, cbAndValues);

			for(int i = 0 ; i < AREA_ALL; ++i)
			{
				if ( nAreaWin[i] == RESULT_WIN )
				{
					m_ImageFrame[i].TransDrawImage(pDC,m_rcBetAreaBox[i].left, m_rcBetAreaBox[i].top, RGB(255,0,255));
				}
			}
		}
	}
}


//�ƶ�Ӯ��
void CGameClientView::DeduceWinner( OUT INT nAreaWin[AREA_ALL], OUT BYTE& cbResult, OUT BYTE& cbAndValues)
{
	//���ñ���
	BYTE cbDragonValue = (m_cbTableCardArray[0][0]&0x0F);
	BYTE cbTigerValue = (m_cbTableCardArray[1][0]&0x0F);
	for( int i = 0 ; i < AREA_ALL; ++i )
		nAreaWin[i] = RESULT_LOSE;

	//��ֵ
	cbAndValues = cbDragonValue + cbTigerValue;

	//��С
	if ( cbDragonValue == cbTigerValue )
		cbResult = AREA_PING;
	else if ( cbDragonValue > cbTigerValue )
		cbResult = AREA_LONG;
	else
		cbResult = AREA_HU;

	//������Ӯ
	if( cbResult == AREA_LONG )							//��
		nAreaWin[AREA_LONG] = RESULT_WIN;
	else if( cbResult == AREA_PING )					//ƽ
		nAreaWin[AREA_PING] = RESULT_WIN;
	else if( cbResult == AREA_HU )						//��
		nAreaWin[AREA_HU] = RESULT_WIN;

	if ( 2 <= cbAndValues && cbAndValues <= 13)			//2 - 13
		nAreaWin[AREA_2_13] = RESULT_WIN;
	else if ( 14 == cbAndValues )						//14
		nAreaWin[AREA_14] = RESULT_WIN;
	else if ( 15 <= cbAndValues && cbAndValues <= 26 )	//15 - 26
		nAreaWin[AREA_15_26] = RESULT_WIN;

	if ( 2 <= cbAndValues && cbAndValues <= 6 )			//2 - 6
		nAreaWin[AREA_2_6] = RESULT_WIN;	
	else if ( 7 <= cbAndValues && cbAndValues <= 11 )	//7 - 11
		nAreaWin[AREA_7_11] = RESULT_WIN;	
	else if ( 12 <= cbAndValues && cbAndValues <= 16 )	//12 - 16
		nAreaWin[AREA_12_16] = RESULT_WIN;
	else if ( 17 <= cbAndValues && cbAndValues <= 21 )	//17 - 21
		nAreaWin[AREA_17_21] = RESULT_WIN;
	else if ( 22 <= cbAndValues && cbAndValues <= 26 )	//22 - 26
		nAreaWin[AREA_22_26] = RESULT_WIN;

	return;
}

//������ʾ
void CGameClientView::SetDispatchCardTips()
{
	return ;
}

//���ñ�ը
bool CGameClientView::SetBombEffect(bool bBombEffect, WORD wAreaIndex)
{
	if (bBombEffect==true)
	{
		//���ñ���
		m_bBombEffect[wAreaIndex]=true;
		m_cbBombFrameIndex[wAreaIndex]=0;

		//����ʱ��
		SetTimer(IDI_BOMB_EFFECT+wAreaIndex,100,NULL);
	}
	else
	{
		//ֹͣ����
		if (m_bBombEffect[wAreaIndex]==true)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BOMB_EFFECT+wAreaIndex);

			//���ñ���
			m_bBombEffect[wAreaIndex]=false;
			m_cbBombFrameIndex[wAreaIndex]=0;

			//���½���
			InvalidGameView(0,0,0,0);
		}
	}

	return true;
}

//�滭��ը
void CGameClientView::DrawBombEffect(CDC *pDC)
{
	//�滭��ը
	INT nImageHeight=m_ImageBombEffect.GetHeight();
	INT nImageWidth=m_ImageBombEffect.GetWidth()/BOMB_EFFECT_COUNT;

	for ( int i = 0; i < AREA_ALL; ++i )
	{
		if (m_bBombEffect[i]==true)
		{
			m_ImageBombEffect.DrawImage(pDC,m_PointJettonNumber[i].x-nImageWidth/2,m_PointJettonNumber[i].y,nImageWidth,nImageHeight,
				nImageWidth*(m_cbBombFrameIndex[i]%BOMB_EFFECT_COUNT),0);
		}
	}
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//���д��
void CGameClientView::OnBankStorage()
{
#ifdef __BANKER___
	//��ȡ�ӿ�
	CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;

	if(NULL!=pIClientKernel)
	{
		CRect btRect;
		m_btBankerStorage.GetWindowRect(&btRect);
		ShowInsureSave(pIClientKernel,CPoint(btRect.right,btRect.top));
	}
#endif
}

//����ȡ��
void CGameClientView::OnBankDraw()
{
#ifdef __BANKER___
	//��ȡ�ӿ�
	CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;

	if(NULL!=pIClientKernel)
	{
		CRect btRect;
		m_btBankerDraw.GetWindowRect(&btRect);
		ShowInsureGet(pIClientKernel,CPoint(btRect.right,btRect.top));
	}
#endif
}
//����Ա����
void CGameClientView::OpenAdminWnd()
{
	//��Ȩ��
	if(m_pClientControlDlg != NULL )
	//if (m_pClientControlDlg != NULL && (CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)))
	
	{
		if(!m_pClientControlDlg->IsWindowVisible()) 
			m_pClientControlDlg->ShowWindow(SW_SHOW);
		else 
			m_pClientControlDlg->ShowWindow(SW_HIDE);
	}
}

//�������
void CGameClientView::AllowControl(BYTE cbStatus)
{
	if(m_pClientControlDlg != NULL && m_pClientControlDlg->m_hWnd!=NULL && m_hInst)
	{
		bool bEnable=false;
		switch(cbStatus)
		{
		case GAME_STATUS_FREE: bEnable=true; break;
		case GAME_SCENE_GAME_END:bEnable=false;break;
		case GAME_SCENE_PLACE_JETTON:bEnable=true;break;
		default:bEnable=false;break;
		}
		m_pClientControlDlg->OnAllowControl(bEnable);
	}
}
//////////////////////////////////////////////////////////////////////////////////
