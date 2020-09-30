#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FLASH_WINNER			100									//������ʶ
#define IDI_SHOW_CHANGE_BANKER		101									//�ֻ�ׯ��
#define IDI_FLEX_MOVE				102									//������Ϣ
#define IDI_DISPATCH_CARD			103									//���Ʊ�ʶ
#define IDI_DISPATCH_INTERVAL		104									//���Ƽ��
#define IDI_END_INTERVAL			105									//���Ƽ��
#define IDI_FLASH_BET				106									//������ʶ
#define IDI_WIN_TYPE_DELAY			107									//ʤ�������ӳ�
#define IDI_WIN_TYPE				108									//ʤ������


//��ť��ʶ
#define IDC_CM_BUTTON_100			200									//��ť��ʶ
#define IDC_CM_BUTTON_1000			201									//��ť��ʶ
#define IDC_CM_BUTTON_10000			202									//��ť��ʶ
#define IDC_CM_BUTTON_100000		203									//��ť��ʶ
#define IDC_CM_BUTTON_1000000		204									//��ť��ʶ
#define IDC_CM_BUTTON_5000000		205									//��ť��ʶ
#define IDC_CM_BUTTON_10000000		206									//��ť��ʶ
#define IDC_APPY_BANKER				207									//��ť��ʶ
#define IDC_CANCEL_BANKER			208									//��ť��ʶ
#define IDC_SCORE_MOVE_L			209									//��ť��ʶ
#define IDC_SCORE_MOVE_R			210									//��ť��ʶ
#define IDC_VIEW_CHART				211									//��ť��ʶ
#define IDC_BANK_STORAGE			212									//��ť��ʶ
#define IDC_BANK_DRAW				213									//��ť��ʶ

#define IDC_UP						223									//��ť��ʶ
#define IDC_DOWN					224									//��ť��ʶ


//��ը��Ŀ
#define BOMB_EFFECT_COUNT			8									//��ը��Ŀ
#define DEAL_MOVE_COUNT_H			5									//���������
#define DEAL_MOVE_COUNT_S			5									//���������
#define FLEX_MOVE_COUNT				10									//���������


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BT_ADMIN,OpenAdminWnd)
	ON_BN_CLICKED(IDC_CM_BUTTON_100, OnBetButton100)
	ON_BN_CLICKED(IDC_CM_BUTTON_1000, OnBetButton1000)
	ON_BN_CLICKED(IDC_CM_BUTTON_10000, OnBetButton10000)
	ON_BN_CLICKED(IDC_CM_BUTTON_100000, OnBetButton100000)
	ON_BN_CLICKED(IDC_CM_BUTTON_1000000, OnBetButton1000000)
	ON_BN_CLICKED(IDC_CM_BUTTON_5000000, OnBetButton5000000)
	ON_BN_CLICKED(IDC_CM_BUTTON_10000000, OnBetButton10000000)

	ON_BN_CLICKED(IDC_VIEW_CHART, OnViewChart)
	ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
	ON_BN_CLICKED(IDC_SCORE_MOVE_L, OnScoreMoveL)
	ON_BN_CLICKED(IDC_SCORE_MOVE_R, OnScoreMoveR)

	ON_BN_CLICKED(IDC_UP, OnValleysUp)
	ON_BN_CLICKED(IDC_DOWN, OnValleysDown)

	ON_BN_CLICKED(IDC_BANK_STORAGE, OnBankStorage)
	ON_BN_CLICKED(IDC_BANK_DRAW, OnBankDraw)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//������Ϣ
	m_lMeMaxScore = 0L;
	m_lAreaLimitScore = 0L;

	//��ע��Ϣ
	ZeroMemory(m_lAllBet, sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));
	//���������ע
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lBankerWinTempScore=0L;

	//���ֳɼ�
	ZeroMemory(m_lPlayScore, sizeof(m_lPlayScore));
	m_lPlayAllScore = 0;

	//��������
	m_nDealMove = 0;
	m_nDealIndex = 0;
	m_ptDispatchCard.SetPoint(0,0);


	//��������
	m_nFlexMove = 0;
	m_bFlexShow = false;
	m_ptFlexBeing.SetPoint(0,0);
	m_ptFlexMove.SetPoint(0,0);
	m_pImageFlex = NULL;

	//״̬��Ϣ
	m_nWinCount = 0;
	m_nLoseCount = 0;
	m_cbGameStatus = GAME_SCENE_FREE;
	m_lCurrentBet = 0L;
	m_wMeChairID = INVALID_CHAIR;
	m_bShowChangeBanker = false;
	m_bNeedSetGameRecord = false;

	//������
	m_ArrayCurrentFlash.RemoveAll();
	m_ArrayFlashArea.RemoveAll();
	m_nFlashAreaAlpha = 0;
	m_bFlashAreaAlpha = false;

	//���ֳɼ�
	ZeroMemory(m_lPlayScore, sizeof(m_lPlayScore));
	m_lPlayAllScore = 0;	

	//��������
	m_nWinShowArea = INT_MAX;
	ZeroMemory(m_nWinShowIndex, sizeof(m_nWinShowIndex));

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	
	m_sizeWin.SetSize(0,0);

	//��ʷ�ɼ�
	m_lMeStatisticScore=0;

	// ��ׯ�б�
	m_nShowValleyIndex = 0;

	// ����
	m_pClientControlDlg = NULL;
	m_hControlInst = NULL;

	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
	if( m_pClientControlDlg )
	{
		delete m_pClientControlDlg;
		m_pClientControlDlg = NULL;
	}

	if( m_hControlInst )
	{
		FreeLibrary(m_hControlInst);
		m_hControlInst = NULL;
	}
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) return -1;

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewFill.LoadFromResource( hInstance,IDB_VIEW_FILL);
	m_ImageViewBack.LoadImage( hInstance,TEXT("VIEW_BACK"));	
	m_ImageViewBackJB.LoadImage( hInstance,TEXT("VIEW_BACK_JB"));	  
	m_ImageWinFlags.LoadImage( hInstance,TEXT("WIN_FLAGS"));
	m_ImageBetView.LoadImage( hInstance,TEXT("BET_VIEW")); 
	m_ImageScoreNumber.LoadImage( hInstance,TEXT("SCORE_NUMBER"));	
	m_ImageScoreBack.LoadImage( hInstance,TEXT("SCORE_BACK"));	
	m_ImageMeScoreNumber.LoadImage( hInstance,TEXT("ME_SCORE_NUMBER"));	
	m_ImageMeScoreBack.LoadImage( hInstance,TEXT("ME_SCORE_BACK"));	

	m_ImageFrame[AREA_XIAN].LoadImage(  hInstance,TEXT("FRAME_XIAN_JIA") );
	m_ImageFrame[AREA_ZHUANG].LoadImage(  hInstance,TEXT("FRAME_ZHUANG_JIA") );
	m_ImageFrame[AREA_XIAN_TIAN].LoadImage(  hInstance,TEXT("FRAME_XIAN_TIAN_WANG") );
	m_ImageFrame[AREA_ZHUANG_TIAN].LoadImage(  hInstance,TEXT("FRAME_ZHUANG_TIAN_WANG") );
	m_ImageFrame[AREA_PING].LoadImage(  hInstance,TEXT("FRAME_PING_JIA") );
	m_ImageFrame[AREA_TONG_DUI].LoadImage(  hInstance,TEXT("FRAME_TONG_DIAN_PING") );
	m_ImageFrame[AREA_XIAN_DUI].LoadImage(  hInstance,TEXT("FRAME_PLAYER_TWO_PAIR") );
	m_ImageFrame[AREA_ZHUANG_DUI].LoadImage(  hInstance,TEXT("FRAME_BANKER_TWO_PAIR") );


	m_ImageGameEnd.LoadImage(  hInstance, TEXT("GAME_END") );
	m_ImageDealBack.LoadImage( hInstance,TEXT("GAME_END_FRAME"));
	m_ImageGamePoint.LoadImage(  hInstance,TEXT("GAME_POINT") );  

	m_ImageGameEndMyScore.LoadImage(  hInstance, TEXT("GAME_END_MY_SCORE") );
	m_ImageGameEndAllScore.LoadImage(  hInstance, TEXT("GAME_END_ALL_SCORE") );
	m_ImageGameEndPoint.LoadImage(  hInstance, TEXT("GAME_END_CRAD_POINT") );

	m_ImageWinType.LoadImage(  hInstance, TEXT("WIN_TYPE") );					
	m_ImageWinXian.LoadImage(  hInstance, TEXT("WIN_XIAN") );					
	m_ImageWinZhuang.LoadImage(  hInstance, TEXT("WIN_ZHUANG") );				

	m_ImageBetTip.LoadImage( hInstance,TEXT("BET_TIP"));

	m_ImageMeBanker.LoadImage(  hInstance, TEXT("ME_BANKER") );
	m_ImageChangeBanker.LoadImage(  hInstance, TEXT("CHANGE_BANKER") );
	m_ImageNoBanker.LoadImage( hInstance, TEXT("NO_BANKER") );	

	m_ImageBrandBoxRight.LoadImage( hInstance,TEXT("BRAND_BOX_RIGHT"));

	m_ImageTimeNumber.LoadImage( hInstance,TEXT("TIME_NUMBER"));
	m_ImageTimeFlag.LoadImage( hInstance,TEXT("TIME_FLAG"));
	m_ImageTimeBack.LoadImage( hInstance,TEXT("TIME_BACK"));
	m_ImageTimeType.LoadImage( hInstance,TEXT("TIME_TYPE"));

	m_ImagePlayLeft.LoadImage( hInstance,TEXT("PLAY_LEFT"));
	m_ImagePlayMiddle.LoadImage( hInstance,TEXT("PLAY_MIDDLE"));
	m_ImagePlayRight.LoadImage( hInstance,TEXT("PLAY_RIGHT"));

	m_ImageScoreInfo.LoadImage( hInstance,TEXT("SCORE_INFO"));
	m_ImageBankerInfo.LoadImage( hInstance,TEXT("BANKER_INFO"));
	m_ImageWaitValleys.LoadImage( hInstance,TEXT("WAIT_VALLEYS"));
	m_ImageWaitFirst.LoadImage( hInstance,TEXT("WAIT_FIRST"));

	//�����ؼ�
	CRect rcCreate(0,0,0,0);

	//��ע��ť
	m_btBet100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CM_BUTTON_100);
	m_btBet1000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CM_BUTTON_1000);
	m_btBet10000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CM_BUTTON_10000);
	m_btBet100000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CM_BUTTON_100000);
	m_btBet1000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CM_BUTTON_1000000);
	m_btBet5000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CM_BUTTON_5000000);
	m_btBet10000000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_CM_BUTTON_10000000);
	m_btViewChart.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_VIEW_CHART);
		
	//���밴ť
	m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
	m_btCancelBanker.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);

	m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
	m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);

	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_BT_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);

	m_btValleysUp.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_UP);
	m_btValleysDown.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_DOWN);
	m_btValleysUp.ShowWindow(SW_SHOW);
	m_btValleysDown.ShowWindow(SW_SHOW);

	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btBet100.SetButtonImage(IDB_BT_CM_100,hResInstance,false,false);
	m_btBet1000.SetButtonImage(IDB_BT_CM_1000,hResInstance,false,false);
	m_btBet10000.SetButtonImage(IDB_BT_CM_10000,hResInstance,false,false);
	m_btBet100000.SetButtonImage(IDB_BT_CM_100000,hResInstance,false,false);
	m_btBet1000000.SetButtonImage(IDB_BT_CM_1000000,hResInstance,false,false);
	m_btBet5000000.SetButtonImage(IDB_BT_CM_5000000,hResInstance,false,false);
	m_btBet10000000.SetButtonImage(IDB_BT_CM_10000000,hResInstance,false,false);

	m_btViewChart.SetButtonImage(IDB_BT_CHART,hResInstance,false,false);	

	m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false,false);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,hResInstance,false,false);

	m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false,false);
	m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false,false);

	m_btValleysUp.SetButtonImage(IDB_BT_BT_S,hResInstance,false,false);
	m_btValleysDown.SetButtonImage(IDB_BT_BT_X,hResInstance,false,false);

	m_DispatchCard.Create(this);
	m_CardControl[INDEX_PLAYER].Create(this);
	m_CardControl[INDEX_BANKER].Create(this);

	// ����
	m_FontValleysListOne.CreateFont( this, TEXT("����"), 16, 600 );
	m_FontValleysListTwo.CreateFont( this, TEXT("����"), 14, 400 );

	//�˿˿ؼ�
	m_CardControl[INDEX_PLAYER].SetDisplayFlag(true);
	m_CardControl[INDEX_BANKER].SetDisplayFlag(true);

	m_DispatchCard.SetDisplayFlag(false);

	//·���ؼ�
	if (m_DlgViewChart.m_hWnd == NULL) m_DlgViewChart.Create(IDD_VIEW_CHART,this);

	//����
	m_hControlInst = NULL;
	m_pClientControlDlg = NULL;
	m_hControlInst = LoadLibrary(TEXT("BaccaratNewClientControl.dll"));
	if ( m_hControlInst )
	{
		typedef void * (*CREATE)(CWnd* pParentWnd); 
		CREATE ClientControl = (CREATE)GetProcAddress(m_hControlInst,"CreateClientControl"); 
		if ( ClientControl )
		{
			m_pClientControlDlg = static_cast<IClientControlDlg*>(ClientControl(this));
			m_pClientControlDlg->ShowWindow( SW_HIDE );
		}
	}

	m_btBankerStorage.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK_STORAGE);
	m_btBankerDraw.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK_DRAW);

	m_btBankerStorage.SetButtonImage(IDB_BT_STORAGE,hResInstance,false,false);
	m_btBankerDraw.SetButtonImage(IDB_BT_DRAW,hResInstance,false,false);

	if ( m_pClientControlDlg )
	{
		for ( int i = 0; i < CountArray(m_SkinListCtrl); ++i )
		{
			m_SkinListCtrl[i].Create(WS_CHILD, rcCreate, this, 2023 + i);
			m_SkinListCtrl[i].ModifyStyle(0,LVS_REPORT);

			m_SkinListCtrl[i].InsertColumn(0,TEXT("���"));
			m_SkinListCtrl[i].InsertColumn(1,TEXT("���"));

			m_SkinListCtrl[i].ShowWindow(SW_HIDE);
		}
	}

	return 0;
}

//���ý���
VOID CGameClientView::ResetGameView()
{
	//��ע��Ϣ
	ZeroMemory(m_lAllBet, sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));
	//���������ע
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lBankerWinTempScore=0L;

	//���ֳɼ�
	ZeroMemory(m_lPlayScore, sizeof(m_lPlayScore));
	m_lPlayAllScore = 0;

	//��������
	m_nDealMove = 0;
	m_nDealIndex = 0;

	//��������
	m_nFlexMove = 0;
	m_bFlexShow = false;
	m_ptFlexBeing.SetPoint(0,0);
	m_ptFlexMove.SetPoint(0,0);
	m_pImageFlex = NULL;

	//״̬��Ϣ
	m_nWinCount = 0;
	m_nLoseCount = 0;
	m_cbGameStatus = GAME_SCENE_FREE;
	m_lCurrentBet=0L;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	m_lAreaLimitScore=0L;	

	//������
	m_ArrayCurrentFlash.RemoveAll();
	m_ArrayFlashArea.RemoveAll();
	m_nFlashAreaAlpha = 0;
	m_bFlashAreaAlpha = false;

	//��������
	m_nWinShowArea = INT_MAX;
	ZeroMemory(m_nWinShowIndex, sizeof(m_nWinShowIndex));

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	//��ʷ�ɼ�
	m_lMeStatisticScore=0;

	//����б�
	m_ValleysList.RemoveAll();

	// ��ׯ�б�
	m_nShowValleyIndex = 0;

	//�������
	CleanUserBet();

	//���ð�ť
	m_btApplyBanker.ShowWindow(SW_SHOW);
	m_btApplyBanker.EnableWindow(FALSE);
	m_btCancelBanker.ShowWindow(SW_HIDE);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false,false);

}

//�����ؼ�
VOID CGameClientView::RectifyControl(int nWidth, int nHeight)
{
	//����λ��
	CPoint ptBenchmark( nWidth/2 - 470, nHeight/2 - 380 );
	CRect rectArea;

	for ( int i = 0; i < AREA_MAX; ++i )
	{
		m_ArrayBetArea[i].RemoveAll();
	}

	//�ж���
	rectArea.SetRect( ptBenchmark.x + 176, ptBenchmark.y + 162, ptBenchmark.x + 368, ptBenchmark.y + 234);
	m_rcXianDui.SetRect( ptBenchmark.x + 176, ptBenchmark.y + 162, ptBenchmark.x + 368, ptBenchmark.y + 234);
	m_ArrayBetArea[AREA_XIAN_DUI].Add(rectArea);

	//ׯ����
	rectArea.SetRect( ptBenchmark.x + 570, ptBenchmark.y + 162, ptBenchmark.x + 762, ptBenchmark.y + 234);
	m_rcZhuangDui.SetRect( ptBenchmark.x + 570, ptBenchmark.y + 162, ptBenchmark.x + 762, ptBenchmark.y + 234);
	m_ArrayBetArea[AREA_ZHUANG_DUI].Add(rectArea);

	//�м�
	rectArea.SetRect( ptBenchmark.x + 126, ptBenchmark.y + 242, ptBenchmark.x + 368, ptBenchmark.y + 450);
	m_rcXian.SetRect( ptBenchmark.x + 126, ptBenchmark.y + 242, ptBenchmark.x + 368, ptBenchmark.y + 450);
	m_ArrayBetArea[AREA_XIAN].Add(rectArea);

	//ƽ��
	rectArea.SetRect( ptBenchmark.x + 370, ptBenchmark.y + 242, ptBenchmark.x + 570, ptBenchmark.y + 450);
	m_rcPing.SetRect( ptBenchmark.x + 370, ptBenchmark.y + 242, ptBenchmark.x + 570, ptBenchmark.y + 450);
	m_ArrayBetArea[AREA_PING].Add(rectArea);

	//ׯ��
	rectArea.SetRect( ptBenchmark.x + 570, ptBenchmark.y + 242, ptBenchmark.x + 812, ptBenchmark.y + 450);
	m_rcZhuang.SetRect( ptBenchmark.x + 570, ptBenchmark.y + 242, ptBenchmark.x + 812, ptBenchmark.y + 450);
	m_ArrayBetArea[AREA_ZHUANG].Add(rectArea);

	//������
	rectArea.SetRect( ptBenchmark.x + 126, ptBenchmark.y + 451, ptBenchmark.x + 368, ptBenchmark.y + 532);
	m_rcXianTian.SetRect( ptBenchmark.x + 126, ptBenchmark.y + 451, ptBenchmark.x + 368, ptBenchmark.y + 532);
	m_ArrayBetArea[AREA_XIAN_TIAN].Add(rectArea);

	//ͬ��ƽ	
	rectArea.SetRect( ptBenchmark.x + 360, ptBenchmark.y + 451, ptBenchmark.x + 580, ptBenchmark.y + 532);
	m_rcTongDui.SetRect( ptBenchmark.x + 360, ptBenchmark.y + 451, ptBenchmark.x + 580, ptBenchmark.y + 532);
	m_ArrayBetArea[AREA_TONG_DUI].Add(rectArea);

	//ׯ����
	rectArea.SetRect( ptBenchmark.x + 570, ptBenchmark.y + 451, ptBenchmark.x + 812, ptBenchmark.y + 532);
	m_rcZhuangTian.SetRect( ptBenchmark.x + 570, ptBenchmark.y + 451, ptBenchmark.x + 812, ptBenchmark.y + 532);
	m_ArrayBetArea[AREA_ZHUANG_TIAN].Add(rectArea);

	//��������
	m_ptBetNumber[AREA_XIAN].SetPoint( ptBenchmark.x + 244, ptBenchmark.y + 334 );
	m_ptBetNumber[AREA_XIAN_TIAN].SetPoint( ptBenchmark.x + 214, ptBenchmark.y + 477 );
	m_ptBetNumber[AREA_XIAN_DUI].SetPoint( ptBenchmark.x + 280, ptBenchmark.y + 195 );

	m_ptBetNumber[AREA_ZHUANG].SetPoint( ptBenchmark.x + 940 - 244, ptBenchmark.y + 334 );
	m_ptBetNumber[AREA_ZHUANG_TIAN].SetPoint( ptBenchmark.x + 940 - 214, ptBenchmark.y + 477 );
	m_ptBetNumber[AREA_ZHUANG_DUI].SetPoint( ptBenchmark.x + 940 - 280, ptBenchmark.y + 195 );

	m_ptBetNumber[AREA_PING].SetPoint( ptBenchmark.x + 470, ptBenchmark.y + 334 );
	m_ptBetNumber[AREA_TONG_DUI].SetPoint( ptBenchmark.x + 470, ptBenchmark.y + 477 );

	//�ܳ�������
	m_ptAllBetNumber[AREA_XIAN].SetPoint( ptBenchmark.x + 244, ptBenchmark.y + 440 );
	m_ptAllBetNumber[AREA_XIAN_TIAN].SetPoint( ptBenchmark.x + 214, ptBenchmark.y + 522 );
	m_ptAllBetNumber[AREA_XIAN_DUI].SetPoint( ptBenchmark.x + 280, ptBenchmark.y + 234 );

	m_ptAllBetNumber[AREA_ZHUANG].SetPoint( ptBenchmark.x + 940 - 244, ptBenchmark.y + 440 );
	m_ptAllBetNumber[AREA_ZHUANG_TIAN].SetPoint( ptBenchmark.x + 940 - 214, ptBenchmark.y + 522 );
	m_ptAllBetNumber[AREA_ZHUANG_DUI].SetPoint( ptBenchmark.x + 940 - 280, ptBenchmark.y + 234 );

	m_ptAllBetNumber[AREA_PING].SetPoint( ptBenchmark.x + 470, ptBenchmark.y + 440 );
	m_ptAllBetNumber[AREA_TONG_DUI].SetPoint( ptBenchmark.x + 470, ptBenchmark.y + 532 );


	//������λ��
	m_ptBetFrame[AREA_XIAN].SetPoint( ptBenchmark.x + 92, ptBenchmark.y + 244 );
	m_ptBetFrame[AREA_XIAN_TIAN].SetPoint( ptBenchmark.x + 91, ptBenchmark.y + 450 );
	m_ptBetFrame[AREA_XIAN_DUI].SetPoint( ptBenchmark.x + 91, ptBenchmark.y + 163 );

	m_ptBetFrame[AREA_ZHUANG].SetPoint( ptBenchmark.x + 570, ptBenchmark.y + 244 );
	m_ptBetFrame[AREA_ZHUANG_TIAN].SetPoint( ptBenchmark.x + 579, ptBenchmark.y + 450 );
	m_ptBetFrame[AREA_ZHUANG_DUI].SetPoint( ptBenchmark.x + 567, ptBenchmark.y + 162 );

	m_ptBetFrame[AREA_PING].SetPoint( ptBenchmark.x + 361, ptBenchmark.y + 244 );
	m_ptBetFrame[AREA_TONG_DUI].SetPoint( ptBenchmark.x + 356, ptBenchmark.y + 450 );

	
	//�˿˿ؼ�
	m_CardControl[0].SetBenchmarkPos(nWidth/2 - 100, 355);
	m_CardControl[1].SetBenchmarkPos(nWidth/2 + 100, 355);

	//���ƿ�ʼ��
	m_ptDispatchCard.SetPoint( nWidth/2 + 759/2 - 60, 120 );

	//����λ��
	m_ptFlexBeing.SetPoint( nWidth/2, 0);
	m_ptFlexMove.x = nWidth/2;

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//�б�ؼ�
	DeferWindowPos(hDwp, m_btValleysUp,   NULL,nWidth/2 + 759/2 - 224 + 194, 32, 0, 0, uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btValleysDown, NULL,nWidth/2 + 759/2 - 224 + 194, 100, 0, 0, uFlags|SWP_NOSIZE);

	//���밴ť
	CRect rcBet;
	m_btBet100.GetWindowRect(&rcBet);
	int nSpace = 7;
	int nYPos = nHeight - 165;
	int nXPos = nWidth/2 - (rcBet.Width()*7 + nSpace*6)/2 + 85;
	DeferWindowPos(hDwp,m_btBet100,NULL,nXPos,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBet1000,NULL,nXPos + nSpace + rcBet.Width(),nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBet10000,NULL,nXPos + nSpace * 2 + rcBet.Width() * 2,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBet100000,NULL,nXPos + nSpace * 3 + rcBet.Width() * 3,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBet1000000,NULL,nXPos + nSpace * 4 + rcBet.Width() * 4,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBet5000000,NULL,nXPos + nSpace * 5 + rcBet.Width() * 5,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBet10000000,NULL,nXPos + nSpace * 6 + rcBet.Width() * 6,nYPos,0,0,uFlags|SWP_NOSIZE);

	//��ׯ��ť
	DeferWindowPos(hDwp,m_btApplyBanker, NULL, nWidth/2 + 759/2 - 120, 8, 0, 0, uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancelBanker,NULL, nWidth/2 + 759/2 - 120, 8, 0, 0, uFlags|SWP_NOSIZE);

	//��ʷ��¼
	DeferWindowPos(hDwp,m_btScoreMoveL, NULL, nWidth/2 + 12, nHeight - 93,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScoreMoveR, NULL, nWidth/2 + 348,  nHeight - 93,0,0,uFlags|SWP_NOSIZE);

	//�鿴·��
	DeferWindowPos(hDwp,m_btViewChart,NULL, nWidth/2 - 123,nHeight - 34,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBankerStorage,NULL,nWidth/2-123,nHeight-2*rcBet.Height()+23,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBankerDraw,NULL,nWidth/2-123,nHeight-rcBet.Height()-5,0,0,uFlags|SWP_NOSIZE);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	// �任����λ��
	CPoint ptOffset( (nWidth - m_sizeWin.cx)/2,(nHeight - m_sizeWin.cy)/2 ); 
	for ( int i = 0; i < AREA_MAX; ++i )
	{
		for ( int j = 0; j < m_BetInfoArray[i].GetCount(); ++j )
		{
			m_BetInfoArray[i][j].nXPos += ptOffset.x;
			m_BetInfoArray[i][j].nYPos += ptOffset.y;
		}
	}

	// ���ڴ�С
	m_sizeWin.SetSize(nWidth, nHeight);

	if(m_DlgViewChart.GetSafeHwnd())
		m_DlgViewChart.ShowWindow(SW_HIDE);

	if ( m_pClientControlDlg )
	{
		CRect m_WinRect(m_rcXian);
		m_WinRect.top += m_WinRect.Height() / 2;
		m_SkinListCtrl[0].MoveWindow(m_WinRect);
		m_SkinListCtrl[0].SetColumnWidth( 0, m_WinRect.Width()/2 );
		m_SkinListCtrl[0].SetColumnWidth( 1, m_WinRect.Width()/2 );

		m_WinRect = m_rcPing;
		m_WinRect.top += m_WinRect.Height() / 2;
		m_WinRect.OffsetRect(0, 0);
		m_SkinListCtrl[1].MoveWindow(m_WinRect);
		m_SkinListCtrl[1].SetColumnWidth(0, m_WinRect.Width()/2 );
		m_SkinListCtrl[1].SetColumnWidth(1, m_WinRect.Width()/2 );

		m_WinRect = m_rcZhuang;
		m_WinRect.top += m_WinRect.Height() / 2;
		m_WinRect.OffsetRect(5, 0);
		m_SkinListCtrl[2].MoveWindow(m_WinRect);
		m_SkinListCtrl[2].SetColumnWidth(0, m_WinRect.Width()/2 );
		m_SkinListCtrl[2].SetColumnWidth(1, m_WinRect.Width()/2 );

		m_WinRect = m_rcXianTian;
		m_WinRect.top += m_WinRect.Height() / 2;
		m_WinRect.bottom = m_WinRect.top+70;
		m_WinRect.OffsetRect(-13, 0);
		m_SkinListCtrl[3].MoveWindow(m_WinRect);
		m_SkinListCtrl[3].SetColumnWidth(0, m_WinRect.Width()/2 );
		m_SkinListCtrl[3].SetColumnWidth(1, m_WinRect.Width()/2 );

		m_WinRect = m_rcZhuangTian;
		m_WinRect.top += m_WinRect.Height() / 2;
		m_WinRect.bottom = m_WinRect.top+70;
		m_WinRect.OffsetRect(15, 0);
		m_SkinListCtrl[4].MoveWindow(m_WinRect);
		m_SkinListCtrl[4].SetColumnWidth(0, m_WinRect.Width()/2 );
		m_SkinListCtrl[4].SetColumnWidth(1, m_WinRect.Width()/2 );

		m_WinRect = m_rcTongDui;
		m_WinRect.top += m_WinRect.Height() / 2;
		m_WinRect.bottom = m_WinRect.top+70;
		m_WinRect.OffsetRect(0, 0);
		m_SkinListCtrl[5].MoveWindow(m_WinRect);
		m_SkinListCtrl[5].SetColumnWidth(0, m_WinRect.Width()/2 );
		m_SkinListCtrl[5].SetColumnWidth(1, m_WinRect.Width()/2 );

		m_WinRect = m_rcXianDui;
		m_WinRect.top += m_WinRect.Height() / 2;
		m_WinRect.bottom = m_WinRect.top+70;
		m_WinRect.OffsetRect(0, 0);
		m_SkinListCtrl[6].MoveWindow(m_WinRect);
		m_SkinListCtrl[6].SetColumnWidth(0, m_WinRect.Width()/2 );
		m_SkinListCtrl[6].SetColumnWidth(1, m_WinRect.Width()/2 );

		m_WinRect = m_rcZhuangDui;
		m_WinRect.top += m_WinRect.Height() / 2;
		m_WinRect.bottom = m_WinRect.top+70;
		m_WinRect.OffsetRect(0, 0);
		m_SkinListCtrl[7].MoveWindow(m_WinRect);
		m_SkinListCtrl[7].SetColumnWidth(0, m_WinRect.Width()/2 );
		m_SkinListCtrl[7].SetColumnWidth(1, m_WinRect.Width()/2 );
	}

	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	int nTime = GetTickCount();

	//�滭����
	for ( int iW = 0 ; iW < nWidth; iW += m_ImageViewFill.GetWidth() )
	{
		for ( int iH = 0;  iH < nHeight; iH += m_ImageViewFill.GetHeight() )
		{
			m_ImageViewFill.BitBlt(pDC->GetSafeHdc(), iW, iH);
		}
	}

	m_ImageViewBack.DrawImage( pDC, nWidth/2 - m_ImageViewBack.GetWidth()/2, nHeight/2 - 380);
	for( int nY = nHeight/2 - 380 + m_ImageViewBack.GetHeight(); nY <= nHeight; nY += m_ImageViewBackJB.GetHeight() )
	{
		m_ImageViewBackJB.DrawImage( pDC, nWidth/2 - m_ImageViewBackJB.GetWidth()/2, nY);
	}
	m_ImageBrandBoxRight.DrawImage( pDC, nWidth/2 + 759/2 - m_ImageBrandBoxRight.GetWidth(), 60);

	//ʤ���߿�
	DrawFlashFrame(pDC,nWidth,nHeight);

	//������Դ
	CSize SizeBetItem(m_ImageBetView.GetWidth()/BET_COUNT,m_ImageBetView.GetHeight());

	//�滭����
	for (INT i=0;i<8;i++)
	{
		//��������
		LONGLONG lScoreCount=0L;
		LONGLONG lScoreBet[BET_COUNT]={100L,1000L,10000L,100000L,1000000L,5000000L,10000000L};

		//�滭����
		for (INT_PTR j = 0;j< m_BetInfoArray[i].GetCount(); j++)
		{
			//��ȡ��Ϣ
			tagBetInfo * pBetInfo = &m_BetInfoArray[i][j];

			//�ۼ�����
			ASSERT(pBetInfo->cbBetIndex<BET_COUNT);
			lScoreCount += lScoreBet[pBetInfo->cbBetIndex];

			//�滭����
			m_ImageBetView.DrawImage(pDC,pBetInfo->nXPos - SizeBetItem.cx / 2,pBetInfo->nYPos - SizeBetItem.cy /2 ,
				SizeBetItem.cx, SizeBetItem.cy, pBetInfo->cbBetIndex*SizeBetItem.cx, 0);
		}

		//�滭����
		if ( lScoreCount > 0L )	
		{
			TCHAR szScore[128] = {0};
			_sntprintf(szScore,CountArray(szScore),TEXT("%s"), AddComma(lScoreCount));
			DrawNumber(pDC, &m_ImageScoreNumber,TEXT(",0123456789"), szScore, m_ptAllBetNumber[i].x + m_ImageScoreBack.GetWidth()/2, m_ptAllBetNumber[i].y - m_ImageScoreNumber.GetHeight() / 2, DT_CENTER);
			m_ImageScoreBack.DrawImage(pDC, m_ptAllBetNumber[i].x - (INT)(((DOUBLE)(lstrlen(szScore)) / 2.0) * (m_ImageScoreNumber.GetWidth() / 11)) - m_ImageScoreBack.GetWidth()/2 - 3, m_ptAllBetNumber[i].y  - m_ImageScoreBack.GetHeight() / 2 );
		
		}
	}

	//�ҵ���ע
	for ( int i = 0; i < AREA_MAX; ++i )
	{
		if ( m_lPlayBet[i] == 0 )
			continue;

		m_ImageMeScoreBack.DrawImage(pDC, m_ptBetNumber[i].x  - m_ImageMeScoreBack.GetWidth()/2, m_ptBetNumber[i].y - m_ImageMeScoreBack.GetHeight()/2);
		DrawNumber(pDC,&m_ImageMeScoreNumber, TEXT(",0123456789"), AddComma(m_lPlayBet[i]),m_ptBetNumber[i].x,m_ptBetNumber[i].y - m_ImageMeScoreNumber.GetHeight()/2, DT_CENTER);
	}

	//�л�ׯ��
	if ( m_bShowChangeBanker )
	{
		//������ׯ
		if ( m_wMeChairID == m_wBankerUser )
		{
			m_ImageMeBanker.DrawImage(pDC, nWidth / 2 - m_ImageMeBanker.GetWidth() / 2, nHeight / 2 - m_ImageMeBanker.GetHeight() / 2);
		}
		else if ( m_wBankerUser != INVALID_CHAIR )
		{
			m_ImageChangeBanker.DrawImage(pDC, nWidth / 2 - m_ImageChangeBanker.GetWidth() / 2, nHeight / 2 - m_ImageChangeBanker.GetHeight() / 2);
		}
		else
		{
			m_ImageNoBanker.DrawImage(pDC, nWidth / 2 - m_ImageNoBanker.GetWidth() / 2, nHeight / 2 - m_ImageNoBanker.GetHeight() / 2);
		}
	}

	//��ʾ������Ϣ
	if ( m_pImageFlex )
	{
		m_pImageFlex->DrawImage(pDC, m_ptFlexMove.x - m_pImageFlex->GetWidth()/2, m_ptFlexMove.y);

		if ( m_cbGameStatus == GAME_SCENE_END && m_enFlexMode == enFlexDealCrad )
		{
			//��ʾ���
			DrawWinType(pDC, nWidth, nHeight, m_ptFlexMove.x - m_pImageFlex->GetWidth()/2, m_ptFlexMove.y);
		}

		if ( m_cbGameStatus == GAME_SCENE_END && m_enFlexMode == enFlexGameEnd )
		{
			//��ʾ���
			DrawGameOver(pDC, nWidth, nHeight, m_ptFlexMove.x - m_pImageFlex->GetWidth()/2, m_ptFlexMove.y);
		}
	}


	// �滭������Ϣ
	DrawTopInfo(pDC, nWidth, nHeight);

	// �滭�ײ���Ϣ
	DrawBottomInfo(pDC, nWidth, nHeight);

	CString str;
	str.Format(TEXT("%d"), GetTickCount() - nTime);
	//AfxMessageBox(str);
	return;
}

// �滭������Ϣ
void CGameClientView::DrawTopInfo(CDC *pDC, int nWidth, int nHeight)
{
	// �������
	CPoint ptBegin(0,0);

	//----------------------------------------------------------------
	// ���·���Ϣ
	ptBegin.SetPoint( nWidth/2 - 759/2, 0 );
	m_ImageScoreInfo.DrawImage( pDC, ptBegin.x, ptBegin.y );

	//�����ע
	if (m_wBankerUser != INVALID_CHAIR || m_bEnableSysBanker)
	{ 
		pDC->SetTextAlign(TA_LEFT);

		DrawTextString( pDC, AddComma(GetMaxPlayerScore(AREA_ZHUANG)), RGB(78,220,224), RGB(39,27,1), ptBegin.x + 82, ptBegin.y + 18);

		DrawTextString( pDC, AddComma(GetMaxPlayerScore(AREA_XIAN)), RGB(112,223,118), RGB(39,27,1), ptBegin.x + 82, ptBegin.y + 39);

		DrawTextString( pDC, AddComma(GetMaxPlayerScore(AREA_PING)), RGB(253,111,109), RGB(39,27,1), ptBegin.x + 82, ptBegin.y + 60);
	}

	// ���ϵ���ʷ��¼
	int nDrawCount = 0;
	int nIndex = m_nScoreHead;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < 12 )
	{
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		pDC->SetTextAlign(TA_CENTER);

		DrawTextString( pDC, AddComma(ClientGameRecord.cbBankerCount), RGB(205,173,80), RGB(39,27,1), ptBegin.x + 39 + nDrawCount * 15, ptBegin.y + 83);

		DrawTextString( pDC, AddComma(ClientGameRecord.cbPlayerCount), RGB(205,173,80), RGB(39,27,1), ptBegin.x + 39 + nDrawCount * 15, ptBegin.y + 103);

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
		nDrawCount++;
	}

	//----------------------------------------------------------------
	// ׯ����Ϣ
	ptBegin.SetPoint( nWidth/2 - m_ImageBankerInfo.GetWidth()/2, 0 );
	m_ImageBankerInfo.DrawImage( pDC, ptBegin.x, ptBegin.y );

	// ��ʾ��Ϣ
	LONGLONG lBankerScore = 0;
	CString strBankName = TEXT("������ׯ");
	if ( m_wBankerUser != INVALID_CHAIR && GetClientUserItem(m_wBankerUser) )
	{
		IClientUserItem* pClientUserItem = GetClientUserItem(m_wBankerUser);
		lBankerScore = pClientUserItem->GetUserScore();
		strBankName = pClientUserItem->GetNickName();

		// ͷ����ʾ
		DrawUserAvatar(pDC, ptBegin.x + 18, ptBegin.y + 68, pClientUserItem);
	}
	else if( m_bEnableSysBanker )
	{
		strBankName = TEXT("ϵͳ��ׯ");
		lBankerScore = m_lBankerScore;

		m_ImageUserFace.DrawImage(pDC, ptBegin.x + 18, ptBegin.y + 68, FACE_CX, FACE_CY, 0, 0, FACE_CX, FACE_CY);
	}

	// ׯ������
	CRect rcBanker;
	rcBanker.SetRect( ptBegin.x + 131, ptBegin.y + 48, ptBegin.x + 243, ptBegin.y + 62);
	DrawTextString( pDC, strBankName, RGB(205,173,80), RGB(39,27,1), rcBanker, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	
	// ׯ���ܷ�
	rcBanker.OffsetRect(0, 21);
	DrawTextString( pDC, AddComma(lBankerScore), RGB(205,173,80), RGB(39,27,1), rcBanker, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	
	// ׯ�ҳɼ�
	rcBanker.OffsetRect(0, 21);
	DrawTextString( pDC, AddComma(m_lBankerWinScore), RGB(205,173,80), RGB(39,27,1), rcBanker, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	
	// ׯ�Ҿ���
	rcBanker.OffsetRect(0, 21);
	DrawTextString( pDC, AddComma(m_wBankerTime), RGB(205,173,80), RGB(39,27,1), rcBanker, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);
   
	//----------------------------------------------------------------
	// ʱ����ʾ
	int nTimeFlagWidth = m_ImageTimeFlag.GetWidth()/3;
	int nFlagIndex=0;
	if (m_cbGameStatus == GAME_SCENE_FREE) 
		nFlagIndex = 0;
	else if (m_cbGameStatus == GAME_SCENE_BET)
		nFlagIndex = 1;
	else if (m_cbGameStatus == GAME_SCENE_END) 
		nFlagIndex = 2;

	m_ImageTimeFlag.DrawImage(pDC, nWidth/2 - m_ImageTimeFlag.GetWidth()/6, 3, nTimeFlagWidth, m_ImageTimeFlag.GetHeight(),
		nFlagIndex * nTimeFlagWidth, 0);


	//----------------------------------------------------------------
	// ������ׯ�б�
	ptBegin.SetPoint( nWidth/2 + 759/2 - m_ImageWaitValleys.GetWidth(), 0 );
	m_ImageWaitValleys.DrawImage( pDC, ptBegin.x, ptBegin.y );

	int   nCount = 0;
	CRect rect(0, 0, 0, 0);
	CFont* pOldFont = pDC->GetCurrentFont();
	if( m_nShowValleyIndex == 0 )
		rect.SetRect(ptBegin.x + 18, ptBegin.y + 35, ptBegin.x + 187, ptBegin.y + 72);
	else
		rect.SetRect(ptBegin.x + 18, ptBegin.y + 35, ptBegin.x + 187, ptBegin.y + 52);

	for( int i = m_nShowValleyIndex; i < m_ValleysList.GetCount(); ++i )
	{
		IClientUserItem* pIClientUserItem = GetClientUserItem( m_ValleysList[i] );
		if ( pIClientUserItem == NULL  )
			continue;

		if ( i == 0 )
		{
			// ��������
			pDC->SelectObject(&m_FontValleysListOne);

			// ����
			m_ImageWaitFirst.DrawImage( pDC, ptBegin.x + 14, ptBegin.y + 32);

			// ͷ��
			DrawUserAvatar( pDC, rect.left, rect.top, 36, 36, pIClientUserItem );

			// ����
			CRect rectName(rect);
			rectName.left += 50;
			rectName.bottom -= rect.Height() / 2;
			DrawTextString( pDC, pIClientUserItem->GetNickName(), RGB(66,251,241) , RGB(61,47,18), rectName, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);

			// ����
			CRect rectScore(rect);
			rectScore.left += 50;
			rectScore.top += rect.Height() / 2;
			DrawTextString( pDC,AddComma(pIClientUserItem->GetUserScore() ), RGB(56,215,232) , RGB(61,47,18), rectScore, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);

			// λ�õ���
			rect.OffsetRect(0, 43);
			rect.bottom -=20;

			// λ������
			nCount += 2;
		}
		else
		{
			// ��������
			pDC->SelectObject(&m_FontValleysListTwo);

			// ͷ��
			DrawUserAvatar(pDC, rect.left, rect.top + rect.Height()/2 - 18/2, 18, 18, pIClientUserItem );

			// ����
			CRect rectName(rect);
			rectName.left = ptBegin.x + 40;
			rectName.right = ptBegin.x + 90;
			DrawTextString( pDC, pIClientUserItem->GetNickName(), RGB(204,174,84) , RGB(38,26,2), rectName, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);

			// ����
			CRect rectScore(rect);
			rectScore.left = ptBegin.x + 100;
			DrawTextString( pDC,AddComma(pIClientUserItem->GetUserScore() ), RGB(204,174,84) , RGB(38,26,2), rectScore, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);

			// λ�õ���
			rect.OffsetRect(0, 21);

			// λ������
			nCount++;
		}

		if ( nCount == 4 )
		{
			break;
		}
	}
	pDC->SelectObject(pOldFont);
}

// �滭�ײ���Ϣ
void CGameClientView::DrawBottomInfo(CDC *pDC, int nWidth, int nHeight)
{
	// �滭λ��
	int nDrawWidth = m_ImagePlayLeft.GetWidth() + m_ImagePlayMiddle.GetWidth()*3 + m_ImagePlayRight.GetWidth();
	CRect rectDraw( nWidth/2 - nDrawWidth/2, nHeight - m_ImagePlayLeft.GetHeight(), nWidth/2 + nDrawWidth/2, nHeight);

	// �ײ�����
	CPoint ptBegin( rectDraw.left, rectDraw.top );
	m_ImagePlayLeft.DrawImage( pDC, ptBegin.x, ptBegin.y );
	ptBegin.x += m_ImagePlayLeft.GetWidth();
	for( int n = 0; n < 3; n++ )
	{
		m_ImagePlayMiddle.DrawImage( pDC, ptBegin.x, ptBegin.y );
		ptBegin.x += m_ImagePlayMiddle.GetWidth();
	}
	m_ImagePlayRight.DrawImage( pDC, ptBegin.x, ptBegin.y );

	//--------------------------------------------------------------------
	// ʱ����Ϣ
	ptBegin.SetPoint( rectDraw.left, rectDraw.top - 30 );
	if ( m_wMeChairID != INVALID_CHAIR )
	{
		WORD wUserTimer = GetUserClock(m_wMeChairID);
		if ( wUserTimer != 0 ) 
		{
			DrawTime(pDC, wUserTimer, ptBegin.x, ptBegin.y );

			int nTimeFlagWidth = m_ImageTimeType.GetWidth()/3;
			int nFlagIndex=0;
			if (m_cbGameStatus == GAME_SCENE_FREE) 
				nFlagIndex = 1;
			else if (m_cbGameStatus == GAME_SCENE_BET)
				nFlagIndex = 2;
			else if (m_cbGameStatus == GAME_SCENE_END) 
				nFlagIndex = 0;

			ptBegin.SetPoint( rectDraw.left + 60, rectDraw.top - 18 );
			m_ImageTimeType.DrawImage(pDC, ptBegin.x, ptBegin.y - m_ImageTimeType.GetHeight()/2, nTimeFlagWidth, m_ImageTimeType.GetHeight(),
				nFlagIndex * nTimeFlagWidth, 0);

		}
	}

	//--------------------------------------------------------------------
	// ������Ϣ
	ptBegin.SetPoint( rectDraw.left, rectDraw.top );
	if (m_wMeChairID != INVALID_CHAIR)
	{
		//��ȡ���
		IClientUserItem* pMeUserItem = GetClientUserItem(m_wMeChairID);
		if ( pMeUserItem != NULL )
		{
			// ͷ��
			DrawUserAvatar( pDC, ptBegin.x + 17, nHeight - 76, pMeUserItem );

			// ����
			CRect rect( ptBegin.x + 130, nHeight - 93, ptBegin.x + 247, nHeight - 75);
			DrawTextString(pDC, &m_FontValleysListTwo, pMeUserItem->GetNickName(), RGB(205,174,83), RGB(34,26,3), rect, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);

			// ����
			rect.OffsetRect(0, 21);
			LONGLONG lMeBet = 0;
			for( int i = 0; i < AREA_MAX; ++i )
				lMeBet += m_lPlayBet[i];

			DrawTextString(pDC, &m_FontValleysListTwo, AddComma( pMeUserItem->GetUserScore() - lMeBet ), RGB(255,225,45), RGB(34,26,3), rect, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);

			// �ɼ�
			rect.OffsetRect(0, 21);
			DrawTextString(pDC, &m_FontValleysListTwo, AddComma(m_lMeStatisticScore), RGB(83,204,101), RGB(34,26,3), rect, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		
			// ������
			CString strHit;
			FLOAT fHit = 0.f;
			rect.OffsetRect(0, 21);
			if ( m_nWinCount + m_nLoseCount > 0 )
				fHit = (FLOAT)m_nWinCount / ((FLOAT)m_nWinCount + (FLOAT)m_nLoseCount);

			if ( fHit < 0.001f )
				strHit = TEXT("��ս��");
			else
				strHit.Format(TEXT("%.2f%%"), fHit*100);
			
			DrawTextString(pDC, &m_FontValleysListTwo, strHit, RGB(63,156,200), RGB(34,26,3), rect, DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
	}

	//--------------------------------------------------------------------
	// ���¼��·��

	int nIndex = m_nScoreHead;
	int nDrawCount = 0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < 12 )
	{

		int nYPos = 0;
		if ( m_GameRecordArrary[nIndex].cbBankerCount > m_GameRecordArrary[nIndex].cbPlayerCount ) nYPos = rectDraw.bottom - 80;
		else if (m_GameRecordArrary[nIndex].cbBankerCount < m_GameRecordArrary[nIndex].cbPlayerCount ) nYPos = rectDraw.bottom - 53;
		else nYPos = rectDraw.bottom - 26;
		int nXPos = rectDraw.right - 330 + ((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY) * 23;

		int nFlagsIndex = 2;
		if ( m_GameRecordArrary[nIndex].enOperateFlags == enOperateResult_NULL ) nFlagsIndex = 2;
		else if ( m_GameRecordArrary[nIndex].enOperateFlags == enOperateResult_Win) nFlagsIndex = 0;
		else if ( m_GameRecordArrary[nIndex].enOperateFlags == enOperateResult_Lost) nFlagsIndex = 1;

		m_ImageWinFlags.DrawImage( pDC, nXPos - m_ImageWinFlags.GetWidth()/6, nYPos - m_ImageWinFlags.GetHeight()/2, m_ImageWinFlags.GetWidth()/3 , 
			m_ImageWinFlags.GetHeight(),m_ImageWinFlags.GetWidth()/3 * nFlagsIndex, 0 );

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
		nDrawCount++;
	}

	//ͳ�Ƹ���
	nIndex = m_nRecordFirst;
	int nPlayerCount = 0, nBankerCount = 0, nTieCount = 0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast != m_nRecordFirst ) )
	{
		if ( m_GameRecordArrary[nIndex].cbBankerCount < m_GameRecordArrary[nIndex].cbPlayerCount ) nPlayerCount++;
		else if ( m_GameRecordArrary[nIndex].cbBankerCount == m_GameRecordArrary[nIndex].cbPlayerCount ) nTieCount++;
		else nBankerCount++;

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
	}

	CRect rect(rectDraw.right - 61, rectDraw.bottom - 92, rectDraw.right - 38, rectDraw.bottom - 67);
	DrawTextString( pDC, AddComma(nBankerCount), RGB(205,173,80), RGB(39,27,1), rect, DT_END_ELLIPSIS|DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	
	rect.OffsetRect(0, 26);
	rect.bottom += 1;
	DrawTextString( pDC, AddComma(nPlayerCount), RGB(205,173,80), RGB(39,27,1), rect, DT_END_ELLIPSIS|DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	
	rect.OffsetRect(0, 28);
	DrawTextString( pDC, AddComma(nTieCount), RGB(205,173,80), RGB(39,27,1), rect, DT_END_ELLIPSIS|DT_CENTER|DT_VCENTER|DT_SINGLELINE);

}

//��ʾ��Ӯ
void CGameClientView::DrawWinType(CDC *pDC, int nWidth, int nHeight, int nBeginX, int nBeginY )
{
	// ��ʾ��Ӯ
	int nTypeWidth = m_ImageWinType.GetWidth()/5;
	int nTypeHeight = m_ImageWinType.GetHeight();


	//ƽ
	if( m_nWinShowIndex[AREA_PING] > 0 )
	{
		BYTE cbAlpha = (BYTE)((DOUBLE)m_nWinShowIndex[AREA_PING] / 6.0 * 255.0);
		m_ImageWinType.AlphaDrawImage(pDC, nBeginX + 390/2 - nTypeWidth/2, nBeginY + 70, nTypeWidth, nTypeHeight, 0, 0, nTypeWidth, nTypeHeight, cbAlpha);
		m_ImageWinType.AlphaDrawImage(pDC, nBeginX + 390/4*3 - nTypeWidth/2, nBeginY + 70, nTypeWidth, nTypeHeight, nTypeWidth * 3, 0, nTypeWidth, nTypeHeight, 255 - cbAlpha );
		m_ImageWinType.AlphaDrawImage(pDC, nBeginX + 390/4 - nTypeWidth/2, nBeginY + 70, nTypeWidth, nTypeHeight, nTypeWidth * 1, 0, nTypeWidth, nTypeHeight, 255 - cbAlpha );
	}
	else
	{

		// ׯ
		if( m_nWinShowIndex[AREA_ZHUANG] == 0 )
		{
			m_ImageWinType.DrawImage(pDC, nBeginX + 390/4*3 - nTypeWidth/2, nBeginY + 70, nTypeWidth, nTypeHeight, nTypeWidth * 3, 0, nTypeWidth, nTypeHeight );
		}
		else if( m_nWinShowIndex[AREA_ZHUANG] < 6 )
		{
			int nZhuangWidth = m_ImageWinZhuang.GetWidth()/6;
			int nZhuangHeight = m_ImageWinZhuang.GetHeight();
			m_ImageWinZhuang.DrawImage(pDC, nBeginX + 390/4*3 - nZhuangWidth/2, nBeginY + 70, nZhuangWidth, nZhuangHeight, nZhuangWidth * m_nWinShowIndex[AREA_ZHUANG], 0, nZhuangWidth, nZhuangHeight );
		}
		else
		{
			m_ImageWinType.DrawImage(pDC, nBeginX + 390/4*3 - nTypeWidth/2, nBeginY + 70, nTypeWidth, nTypeHeight, nTypeWidth * 4, 0, nTypeWidth, nTypeHeight );;
		}

		// ��
		if( m_nWinShowIndex[AREA_XIAN] == 0 )
		{
			m_ImageWinType.DrawImage(pDC, nBeginX + 390/4 - nTypeWidth/2, nBeginY + 70, nTypeWidth, nTypeHeight, nTypeWidth * 1, 0, nTypeWidth, nTypeHeight );
		}
		else if( m_nWinShowIndex[AREA_XIAN] < 6 )
		{
			int nXianWidth = m_ImageWinXian.GetWidth()/6;
			int nXianHeight = m_ImageWinXian.GetHeight();
			m_ImageWinXian.DrawImage(pDC, nBeginX + 390/4 - nXianWidth/2, nBeginY + 70, nXianWidth, nXianHeight, nXianWidth * m_nWinShowIndex[AREA_XIAN], 0, nXianWidth, nXianHeight );
		}
		else
		{
			m_ImageWinType.DrawImage(pDC, nBeginX + 390/4 - nTypeWidth/2, nBeginY + 70, nTypeWidth, nTypeHeight, nTypeWidth * 2, 0, nTypeWidth, nTypeHeight );
		}
	}

	//�������
	BYTE cbCardData[2][5] = {0};
	WORD cbCardCount[2] = {m_CardControl[INDEX_PLAYER].GetCardCount(), m_CardControl[INDEX_BANKER].GetCardCount() };
	int  nPointWidth = m_ImageGamePoint.GetWidth() / 10;

	// �мҵ���
	if ( cbCardCount[INDEX_PLAYER] > 0 )
	{
		// �������
		m_CardControl[INDEX_PLAYER].GetCardData(cbCardData[INDEX_PLAYER], 5);
		BYTE cbPlayerPoint = m_GameLogic.GetCardListPip(cbCardData[INDEX_PLAYER], (BYTE)cbCardCount[INDEX_PLAYER]);

		//�滭����
		m_ImageGamePoint.DrawImage(pDC, nWidth/2 - 100 - nPointWidth/2, 170, nPointWidth, m_ImageGamePoint.GetHeight(), cbPlayerPoint * nPointWidth, 0);
	}

	// ׯ�ҵ���
	if ( cbCardCount[INDEX_BANKER] > 0 )
	{
		// �������
		m_CardControl[INDEX_BANKER].GetCardData(cbCardData[INDEX_BANKER], 5);
		BYTE cbBankerPoint = m_GameLogic.GetCardListPip(cbCardData[INDEX_BANKER], (BYTE)cbCardCount[INDEX_BANKER]);

		//�滭����
		m_ImageGamePoint.DrawImage(pDC, nWidth/2 + 100 - nPointWidth/2 , 170, nPointWidth, m_ImageGamePoint.GetHeight(), cbBankerPoint * nPointWidth, 0);
	}

	//�滭�˿�
	m_CardControl[INDEX_PLAYER].DrawCardControl(pDC);
	m_CardControl[INDEX_BANKER].DrawCardControl(pDC);

	//�����˿�
	m_DispatchCard.DrawCardControl(pDC);	


	// ������Ϣ
	if( m_CardControl[INDEX_PLAYER].GetCardCount() || m_CardControl[INDEX_BANKER].GetCardCount() )
	{
		// ��������
		UINT   pOldAlign = pDC->SetTextAlign(TA_CENTER);

		// ������ʾ
		int nBegin = 0;
		int nStrCount = 0;
		TCHAR szString[512] = {0};
		_sntprintf(szString, sizeof(szString), TEXT("%s"), m_strDispatchCardTips);
		for(int i = 0; i < lstrlen(szString); ++i  )
		{
			if ( szString[i] == '\n' || i == lstrlen(szString) - 1 )
			{	
				TCHAR szTemp[128];
				memcpy(szTemp, &szString[nBegin], (i - nBegin + 1) * sizeof(TCHAR));

				if ( i == lstrlen(szString) - 1 )
					szTemp[(i - nBegin) + 1] = '\0';
				else
					szTemp[(i - nBegin)] = '\0';

				nBegin = i;
				DrawTextString( pDC, &m_FontValleysListTwo, szTemp, RGB(204,174,84) , RGB(38,26,2),  nWidth/2, 443 + nStrCount * 18 );
				nStrCount++;
			}
		}

		// ��ԭ����
		pDC->SetTextAlign(pOldAlign);
	}
}

//��ʾ���
void CGameClientView::DrawGameOver(CDC *pDC, int nWidth, int nHeight, int nBeginX, int nBeginY )
{
	// ����
	int  nPointWidth = m_ImageGameEndPoint.GetWidth() / 10;
	BYTE cbPlayerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER], m_cbCardCount[INDEX_PLAYER]);
	BYTE cbBankerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER], m_cbCardCount[INDEX_BANKER]);

	//�滭����
	m_ImageGameEndPoint.DrawImage(pDC, nBeginX + 80 , nBeginY + 74, nPointWidth, m_ImageGamePoint.GetHeight(), cbPlayerPoint * nPointWidth, 0);
	m_ImageGameEndPoint.DrawImage(pDC, nBeginX + 321 , nBeginY + 74, nPointWidth, m_ImageGamePoint.GetHeight(), cbBankerPoint * nPointWidth, 0);

	// ����
	DrawNumber(pDC, &m_ImageGameEndMyScore, TEXT("+-,0123456789"), AddComma(m_lPlayScore[AREA_XIAN_DUI], true), nBeginX + 87 , nBeginY + 125, DT_CENTER);
	DrawNumber(pDC, &m_ImageGameEndMyScore, TEXT("+-,0123456789"), AddComma(m_lPlayScore[AREA_ZHUANG_DUI], true), nBeginX + 334 , nBeginY + 125, DT_CENTER);

	DrawNumber(pDC, &m_ImageGameEndMyScore, TEXT("+-,0123456789"), AddComma(m_lPlayScore[AREA_XIAN], true), nBeginX + 87 , nBeginY + 174, DT_CENTER);
	DrawNumber(pDC, &m_ImageGameEndMyScore, TEXT("+-,0123456789"), AddComma(m_lPlayScore[AREA_PING], true), nBeginX + 208 , nBeginY + 174, DT_CENTER);
	DrawNumber(pDC, &m_ImageGameEndMyScore, TEXT("+-,0123456789"), AddComma(m_lPlayScore[AREA_ZHUANG], true), nBeginX + 334 , nBeginY + 174, DT_CENTER);

	DrawNumber(pDC, &m_ImageGameEndMyScore, TEXT("+-,0123456789"), AddComma(m_lPlayScore[AREA_XIAN_TIAN], true), nBeginX + 87 , nBeginY + 223, DT_CENTER);
	DrawNumber(pDC, &m_ImageGameEndMyScore, TEXT("+-,0123456789"), AddComma(m_lPlayScore[AREA_TONG_DUI], true), nBeginX + 208 , nBeginY + 223, DT_CENTER);
	DrawNumber(pDC, &m_ImageGameEndMyScore, TEXT("+-,0123456789"), AddComma(m_lPlayScore[AREA_ZHUANG_TIAN], true), nBeginX + 334 , nBeginY + 223, DT_CENTER);

	LONGLONG lPlayAllScore = 0l;
	for ( int i = 0; i < AREA_MAX; ++i )
	{
		lPlayAllScore += m_lPlayScore[i];
	}
	DrawNumber(pDC, &m_ImageGameEndAllScore, TEXT("+-,0123456789"), AddComma(lPlayAllScore, true), nBeginX + 97 , nBeginY + 259, DT_LEFT);
}

//���ó���
void CGameClientView::SetCurrentBet(LONGLONG lCurrentBet)
{
	//���ñ���
	ASSERT(lCurrentBet>=0L);
	m_lCurrentBet = lCurrentBet;

	if (m_lCurrentBet == 0L && m_cbGameStatus != GAME_SCENE_END )
	{
		for ( int i = 0 ; i < m_ArrayCurrentFlash.GetCount(); ++i )
		{
			m_ArrayCurrentFlash[i].bFlashAreaAlpha = false;
		}
		InvalidGameView(0,0,0,0);
	}
	return;
}

//��ʷ��¼
void CGameClientView::SetGameHistory(enOperateResult OperateResult, BYTE cbPlayerCount, BYTE cbBankerCount, BYTE cbKingWinner, bool bPlayerTwoPair, bool bBankerTwoPair)
{
	//��������
	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.enOperateFlags = OperateResult;
	GameRecord.cbPlayerCount = cbPlayerCount;
	GameRecord.cbBankerCount = cbBankerCount;
	GameRecord.bPlayerTwoPair=bPlayerTwoPair;
	GameRecord.bBankerTwoPair=bBankerTwoPair;
	GameRecord.cbKingWinner=cbKingWinner;

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
void CGameClientView::CleanUserBet()
{
	//��������
	for (BYTE i = 0; i < CountArray(m_BetInfoArray); i++)
		m_BetInfoArray[i].RemoveAll();

	//��ע��Ϣ
	ZeroMemory(m_lAllBet, sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));
	ZeroMemory(m_lPlayScore, sizeof(m_lPlayScore));
	//���������ע
	ZeroMemory(m_lAllPlayBet,sizeof(m_lAllPlayBet));

	//ɾ����ע��Ϣ
	m_strDispatchCardTips = TEXT("");

	//ɾ����ʱ��
	KillTimer(IDI_FLASH_WINNER);
	KillTimer(IDI_DISPATCH_CARD);
	KillTimer(IDI_DISPATCH_INTERVAL);
	KillTimer(IDI_FLASH_BET);
	KillTimer(IDI_WIN_TYPE);
	KillTimer(IDI_WIN_TYPE_DELAY);
	

	//�˿���Ϣ
	ZeroMemory(m_cbCardCount, sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray, sizeof(m_cbTableCardArray));
	m_DispatchCard.SetCardData(NULL, 0);
	m_CardControl[0].SetCardData(NULL, 0);
	m_CardControl[1].SetCardData(NULL, 0);

	for ( int i = 0; i < CountArray(m_SkinListCtrl); ++i )
	{
		if ( m_SkinListCtrl[i].GetSafeHwnd() )
			m_SkinListCtrl[i].DeleteAllItems();
	}
	
	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//����״̬
VOID CGameClientView::SetGameStatus( BYTE cbGameStatus )
{
	m_cbGameStatus = cbGameStatus;

	if ( m_cbGameStatus == GAME_SCENE_BET )
	{
		FlexAnimation(enFlexBetTip, true);

		SetTimer(IDI_FLASH_BET, 30, NULL);
	}
	else if ( m_cbGameStatus == GAME_SCENE_END )
	{
		FlexAnimation(enFlexBetTip, false);

		KillTimer(IDI_FLASH_BET);
	}
	else if ( m_cbGameStatus == GAME_SCENE_FREE )
	{
		//���ý���
		m_CardControl[INDEX_PLAYER].SetCardData(NULL, 0);
		m_CardControl[INDEX_BANKER].SetCardData(NULL, 0);
		FlexAnimation(m_enFlexMode, false);

		KillTimer(IDI_FLASH_BET);
	}
}

//�ҵ�λ��
void CGameClientView::SetMeChairID(WORD wMeChairID)
{
	m_wMeChairID = wMeChairID;
}

//���������ע
void CGameClientView::SetPlayBetScore(LONGLONG lPlayBetScore)
{
	m_lMeMaxScore = lPlayBetScore;
}

//��������
void CGameClientView::SetAreaLimitScore(LONGLONG lAreaLimitScore)
{
	m_lAreaLimitScore = lAreaLimitScore;
}

//ׯ����Ϣ
void CGameClientView::SetBankerInfo(WORD wBankerUser, LONGLONG lBankerScore, LONGLONG lBankerWinScore, WORD wBankerTime)
{
	m_wBankerUser = wBankerUser;
	m_lBankerScore = lBankerScore;
	m_lBankerWinScore = lBankerWinScore;
	m_wBankerTime = wBankerTime;
}

//ׯ����Ϣ
void CGameClientView::SetBankerInfo( WORD wBankerUser, LONGLONG lBankerScore )
{
	m_wBankerUser = wBankerUser;
	m_lBankerScore = lBankerScore;
}

//ׯ����Ϣ
void CGameClientView::SetBankerOverInfo( LONGLONG lBankerWinScore, WORD wBankerTime )
{
	m_lBankerWinTempScore = lBankerWinScore;
	m_wBankerTime = wBankerTime;
}

//����ϵͳ�Ƿ���ׯ
void CGameClientView::SetEnableSysBanker(bool bEnableSysBanker)
{
	m_bEnableSysBanker = bEnableSysBanker;
}

//������ע
void CGameClientView::SetPlayBet(BYTE cbViewIndex, LONGLONG lBetCount)
{
	m_lPlayBet[cbViewIndex] = lBetCount;
}

//ȫ����ע
void CGameClientView::SetAllBet(BYTE cbViewIndex, LONGLONG lBetCount)
{
	m_lAllBet[cbViewIndex] = lBetCount;
}

//��ӳ���
void CGameClientView::AddChip(BYTE cbViewIndex, LONGLONG lScoreCount)
{
	//Ч�����
	ASSERT(cbViewIndex < AREA_MAX);
	if (cbViewIndex >= AREA_MAX) return;

	//��������
	LONGLONG lScoreIndex[BET_COUNT]={100L,1000L,10000L,100000L,1000000L,5000000L,10000000L};

	//�����ж�
	bool bAddBet = (lScoreCount > 0L)?true:false;
	if(lScoreCount < 0L) lScoreCount = (lScoreCount*-1L);

	//���ӳ���
	for (BYTE i = 0;i < CountArray(lScoreIndex); i++)
	{
		//������Ŀ
		BYTE cbScoreIndex=BET_COUNT-i-1;
		LONGLONG lCellCount=lScoreCount/lScoreIndex[cbScoreIndex];

		//�������
		if (lCellCount == 0L) continue;

		//�������
		for (LONGLONG j = 0;j < lCellCount;j++)
		{
			if (true == bAddBet)
			{
				// �������
				tagBetInfo BetInfo;
				INT_PTR nAreaIndex = rand()%m_ArrayBetArea[cbViewIndex].GetCount();
				CRect rect(m_ArrayBetArea[cbViewIndex][nAreaIndex]);
				BetInfo.cbBetIndex = cbScoreIndex;

				//������Դ
				CSize SizeBetItem(m_ImageBetView.GetWidth()/BET_COUNT,m_ImageBetView.GetHeight());
				SizeBetItem += CSize(10,10);

				BetInfo.nXPos = (rand()%(rect.Width() - SizeBetItem.cx)) + rect.left + SizeBetItem.cx/2; 
				BetInfo.nYPos = (rand()%(rect.Height() - SizeBetItem.cy)) + rect.top + SizeBetItem.cy/2;

				//��������
				m_BetInfoArray[cbViewIndex].Add(BetInfo);
			}
			else
			{
				for (int nIndex=0; nIndex<m_BetInfoArray[cbViewIndex].GetCount(); ++nIndex)
				{
					//�Ƴ��ж�
					tagBetInfo &BetInfo=m_BetInfoArray[cbViewIndex][nIndex];
					if (BetInfo.cbBetIndex == cbScoreIndex)
					{
						m_BetInfoArray[cbViewIndex].RemoveAt(nIndex);
						break;
					}
				}
			}
		}

		//������Ŀ
		lScoreCount -= lCellCount*lScoreIndex[cbScoreIndex];
	}

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}


//�����˿�
void CGameClientView::SetCardInfo(BYTE cbCardCount[2], BYTE cbTableCardArray[2][3])
{
	if (cbCardCount != NULL)
	{
		CopyMemory(m_cbCardCount,cbCardCount,sizeof(m_cbCardCount));
		CopyMemory(m_cbTableCardArray,cbTableCardArray,sizeof(m_cbTableCardArray));
	}
	else
	{
		ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
		ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));
	}
}


//���ֳɼ�
void CGameClientView::SetCurGameScore(LONGLONG lPlayScore[AREA_MAX], LONGLONG lPlayAllScore)
{
	memcpy(m_lPlayScore, lPlayScore, sizeof(m_lPlayScore));
	m_lPlayAllScore = lPlayAllScore;

	for ( int i = 0 ; i < AREA_MAX; ++i )
	{
		if ( lPlayScore[i] > 0 )
		{
			m_nWinCount++;
		}
		else if ( lPlayScore[i] < 0)
		{
			m_nLoseCount++;
		}
	}
}

//��ȡ����
BYTE CGameClientView::GetBetArea(CPoint MousePoint)
{
	for ( int i = 0 ; i < AREA_MAX; ++i )
	{
		for ( int n = 0; n < m_ArrayBetArea[i].GetCount(); ++n )
		{
			if ( m_ArrayBetArea[i][n].PtInRect(MousePoint))
			{
				return i;
			}
		}
	}

	return 0xFF;
}

//���밴ť
void CGameClientView::OnBetButton100()
{
	//���ñ���
	m_lCurrentBet=100L;

	return;
}

//���밴ť
void CGameClientView::OnBetButton1000()
{
	//���ñ���
	m_lCurrentBet=1000L;

	return;
}

//���밴ť
void CGameClientView::OnBetButton10000()
{
	//���ñ���
	m_lCurrentBet=10000L;

	return;
}

//���밴ť
void CGameClientView::OnBetButton100000()
{
	//���ñ���
	m_lCurrentBet=100000L;

	return;
}

//���밴ť
void CGameClientView::OnBetButton1000000()
{
	//���ñ���
	m_lCurrentBet=1000000L;

	return;
}
//���밴ť
void CGameClientView::OnBetButton5000000()
{
	//���ñ���
	m_lCurrentBet=5000000L;

	return;
}

//���밴ť
void CGameClientView::OnBetButton10000000()
{
	//���ñ���
	m_lCurrentBet=10000000L;

	return;
}
//�鿴·��
void CGameClientView::OnViewChart()
{
	if (m_DlgViewChart.m_hWnd == NULL) m_DlgViewChart.Create(IDD_VIEW_CHART,this);

	//��ʾ�ж�
	if (!m_DlgViewChart.IsWindowVisible())
	{
		m_DlgViewChart.CenterWindow(this);
		m_DlgViewChart.ShowWindow(SW_SHOW);
	}
	else
	{
		m_DlgViewChart.ShowWindow(SW_HIDE);
	}
}

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	
	if ( nIDEvent == IDI_FLASH_WINNER )				//����ʤ��
	{
		//���ñ���
		if ( m_bFlashAreaAlpha )
		{
			m_nFlashAreaAlpha += 10;
			if ( m_nFlashAreaAlpha > 240)
			{
				m_bFlashAreaAlpha = false;
			}
		}
		else
		{
			m_nFlashAreaAlpha -= 10;
			if ( m_nFlashAreaAlpha < 10)
			{
				m_bFlashAreaAlpha = true;
			}
		}
		

		//���½���
		InvalidGameView(0,0,0,0);

		return;
	}
	else if ( nIDEvent == IDI_FLASH_BET )	//ѡȡ����
	{
		int nIndex = 0;
		while( nIndex < m_ArrayCurrentFlash.GetCount() )
		{
			if( m_ArrayCurrentFlash[nIndex].bFlashAreaAlpha )
			{
				m_ArrayCurrentFlash[nIndex].nFlashAreaAlpha += 30;
				if ( m_ArrayCurrentFlash[nIndex].nFlashAreaAlpha > 255 )
				{
					m_ArrayCurrentFlash[nIndex].nFlashAreaAlpha = 255;
				}
				nIndex++;
			}
			else
			{
				m_ArrayCurrentFlash[nIndex].nFlashAreaAlpha -= 30;
				if ( m_ArrayCurrentFlash[nIndex].nFlashAreaAlpha < 10 )
				{
					m_ArrayCurrentFlash.RemoveAt(nIndex);
					continue;
				}
				nIndex++;
			}
		}

		//���½���
		InvalidGameView(0,0,0,0);

		return;
	}
	else if ( nIDEvent == IDI_SHOW_CHANGE_BANKER )	//�ֻ�ׯ��
	{
		ShowChangeBanker( false );
		return;
	}
	else if ( nIDEvent == IDI_FLEX_MOVE )		//�����ƶ�
	{
		if ( m_pImageFlex == NULL )
			return;
		
		//λ�øı�
		m_ptFlexMove.y = (m_pImageFlex->GetHeight() + 137) * m_nFlexMove / FLEX_MOVE_COUNT + m_ptFlexBeing.y - m_pImageFlex->GetHeight();

		if ( m_bFlexShow )
		{
			m_nFlexMove++;
			if( m_nFlexMove > FLEX_MOVE_COUNT )
			{
				m_nFlexMove = FLEX_MOVE_COUNT;
				KillTimer(IDI_FLEX_MOVE);

				if ( m_cbGameStatus == GAME_SCENE_END && m_enFlexMode == enFlexDealCrad )
				{
					//��ʼ����
					DispatchCard();
				}
			}
		}
		else
		{
			m_nFlexMove--;
			if ( m_nFlexMove < 0 )
			{
				m_nFlexMove = 0;
				KillTimer(IDI_FLEX_MOVE);

				if ( m_cbGameStatus == GAME_SCENE_FREE && m_enFlexMode == enFlexDealCrad )
				{
					FlexAnimation(enFlexNULL, false);
				}
				else if ( m_cbGameStatus == GAME_SCENE_END && m_enFlexMode == enFlexBetTip )
				{
					//��ʾ����
					m_ptFlexMove.y = m_ptFlexBeing.y - m_ImageDealBack.GetHeight();
					FlexAnimation(enFlexDealCrad, true);
				}
				else if ( m_cbGameStatus == GAME_SCENE_END && m_enFlexMode == enFlexDealCrad )
				{
					//��ʾ����
					m_ptFlexMove.y = m_ptFlexBeing.y - m_ImageGameEnd.GetHeight();
					FlexAnimation(enFlexGameEnd, true);
				}
			}
		}
		
		//���½���
		InvalidGameView(0,0,0,0);
		return;
	}
	else if ( nIDEvent == IDI_DISPATCH_CARD )		// ���ƶ���
	{
		CPoint ptMoveCrad;
		CPoint ptObjectiveCrad = m_CardControl[m_nDealIndex].GetBenchmarkPos();

		//��������
		if( m_nDealMove == 0 && !CD2DEngine::GetD2DEngine())
		{
			SendEngineMessage(IDM_PLAY_SOUND,(WPARAM)TEXT("DISPATCH_CARD"),0);
		}

		if( m_nDealMove <= DEAL_MOVE_COUNT_S )
		{
			ptMoveCrad.y = ( ptObjectiveCrad.y - m_ptDispatchCard.y ) * m_nDealMove / DEAL_MOVE_COUNT_S + m_ptDispatchCard.y;
			ptMoveCrad.x = m_ptDispatchCard.x;
		}
		else if( m_nDealMove - DEAL_MOVE_COUNT_S <= DEAL_MOVE_COUNT_H )
		{
			ptMoveCrad.y = ptObjectiveCrad.y;
			ptMoveCrad.x = ( ptObjectiveCrad.x - m_ptDispatchCard.x ) * (m_nDealMove - DEAL_MOVE_COUNT_S) / DEAL_MOVE_COUNT_H + m_ptDispatchCard.x;
		}
	
		m_DispatchCard.SetBenchmarkPos(ptMoveCrad.x,ptMoveCrad.y);
   		m_DispatchCard.ShowCardControl(true);

		m_nDealMove++;
		if ( m_nDealMove > DEAL_MOVE_COUNT_H + DEAL_MOVE_COUNT_S )
		{
			m_nDealMove = 0;
			if ( m_CardControl[m_nDealIndex].GetCardCount() < m_cbCardCount[m_nDealIndex] )
			{
				m_CardControl[m_nDealIndex].SetCardData( m_cbTableCardArray[m_nDealIndex], m_CardControl[m_nDealIndex].GetCardCount() + 1);
			}
			int nNextCrad = (m_nDealIndex + 1)%2;
			if ( m_CardControl[nNextCrad].GetCardCount() < m_cbCardCount[nNextCrad] )
			{
				m_nDealIndex = nNextCrad;
			}

			if ( m_CardControl[INDEX_PLAYER].GetCardCount() == m_cbCardCount[INDEX_PLAYER]
				&& m_CardControl[INDEX_BANKER].GetCardCount() == m_cbCardCount[INDEX_BANKER])
			{
				m_DispatchCard.SetCardData(NULL, 0);

				//������ʾ
				SetDispatchCardTips();

				//��ɷ���
				FinishDispatchCard();
			}
			else 
			{
				//���η�����
				m_DispatchCard.ShowCardControl(false);

				//������ʾ
				SetDispatchCardTips();

				//ɾ����ʱ
				KillTimer(IDI_DISPATCH_CARD);

				//���������ʱ
				if( m_CardControl[m_nDealIndex].GetCardCount() >= 2 )
					SetTimer(IDI_DISPATCH_INTERVAL, 1000, NULL);
				else
					SetTimer(IDI_DISPATCH_INTERVAL, 300, NULL);
			}
		}

		//���½���
		InvalidGameView(0,0,0,0);
		return;
	}
	else if ( nIDEvent == IDI_DISPATCH_INTERVAL )
	{
		//ɾ����ʱ
		KillTimer(IDI_DISPATCH_INTERVAL);

		//��������
		SetTimer(IDI_DISPATCH_CARD, 30, NULL);

		//���½���
		InvalidGameView(0,0,0,0);
		return;
	}
	else if ( nIDEvent == IDI_END_INTERVAL )
	{
		//ɾ����ʱ
		KillTimer(IDI_END_INTERVAL);

		BYTE cbCrad[1] = {0};
		m_CardControl[INDEX_PLAYER].SetCardData(cbCrad, 0);
		m_CardControl[INDEX_BANKER].SetCardData(cbCrad, 0);

		//��������
		FlexAnimation(enFlexDealCrad, false);

		//���½���
		InvalidGameView(0,0,0,0);
		return;
	}
	else if ( nIDEvent == IDI_WIN_TYPE_DELAY)
	{
		//ɾ����ʱ
		KillTimer(IDI_WIN_TYPE_DELAY);

		//��������
		SetTimer(IDI_WIN_TYPE, 40, NULL);

		//���½���
		InvalidGameView(0,0,0,0);

		return;
	}
	// ��Ӯ������
	else if ( nIDEvent == IDI_WIN_TYPE )
	{
		if ( m_nWinShowArea >= CountArray(m_nWinShowIndex) || m_nWinShowArea < 0 )
		{
			KillTimer(IDI_WIN_TYPE);
			return;
		}

		m_nWinShowIndex[m_nWinShowArea]++;

		if ( m_nWinShowIndex[m_nWinShowArea] == 6)
		{
			m_nWinShowArea = INT_MAX;
			KillTimer(IDI_WIN_TYPE);

			//�������Ӯ ������Ӯ����
			for ( int i = 0 ; i < AREA_MAX; ++i )
			{
				if ( m_lPlayScore[i] != 0 )
				{
					SetTimer(IDI_END_INTERVAL, 3000, NULL);
					break;
				}
			}
			return;
		}

		//���½���
		InvalidGameView(0,0,0,0);
		return;
	}

	__super::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	//BYTE cbCrad[3] = {0x03,0x03,0x03};
	//m_CardControl[INDEX_PLAYER].SetCardData(cbCrad, 3);

	//m_CardControl[1].SetCardData(cbCrad, 1);

	////���½���
	InvalidGameView(0,0,0,0);
	if ( m_lCurrentBet != 0L )
	{
		LONGLONG lMaxPlayerScore = 0;
		BYTE cbBetArea = GetBetArea(Point);
		lMaxPlayerScore = GetMaxPlayerScore(cbBetArea);

		if ( lMaxPlayerScore < m_lCurrentBet )
			return ;

		//������Ϣ
		if ( cbBetArea != 0xFF ) 
			SendEngineMessage(IDM_PALY_BET, cbBetArea, 0);
	}

	__super::OnLButtonDown(nFlags,Point);
}

//�����Ϣ
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//���ñ���
	m_lCurrentBet = 0L;
	for ( int i = 0 ; i < m_ArrayCurrentFlash.GetCount(); ++i )
	{
		m_ArrayCurrentFlash[i].bFlashAreaAlpha = false;
	}

	InvalidGameView(0,0,0,0);

	__super::OnLButtonDown(nFlags,Point);
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	if ( m_lCurrentBet != 0L && m_cbGameStatus == GAME_SCENE_BET )
	{
		//��ȡ����
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		BYTE cbBetArea = GetBetArea(MousePoint);

		//���ñ���
		bool bBid = false;
		for ( int i = 0 ; i < m_ArrayCurrentFlash.GetCount(); ++i )
		{
			if ( m_ArrayCurrentFlash[i].cbFlashArea == cbBetArea )
			{
				bBid = true;
				m_ArrayCurrentFlash[i].bFlashAreaAlpha = true;
			}
			else
			{
				m_ArrayCurrentFlash[i].bFlashAreaAlpha = false;
			}
		}

		if ( !bBid && cbBetArea < AREA_MAX )
		{
			tagFlashInfo FlashInfo;
			FlashInfo.cbFlashArea = cbBetArea;
			FlashInfo.bFlashAreaAlpha = true;
			FlashInfo.nFlashAreaAlpha = 10;
			m_ArrayCurrentFlash.Add(FlashInfo);
		}

		//�����ж�
		if (cbBetArea == 0xFF)
		{
			SetCursor( LoadCursor(NULL, IDC_ARROW) );
			return TRUE;
		}

		//��С�ж�
		LONGLONG lMaxPlayerScore = GetMaxPlayerScore(cbBetArea);

		if ( lMaxPlayerScore < m_lCurrentBet )
		{
			SetCursor( LoadCursor(NULL, IDC_NO) );
			return TRUE;
		}

		//���ù��
		switch (m_lCurrentBet)
		{
		case 100:
			{
				SetCursor( LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100)) );
				return TRUE;
			}
		case 1000:
			{
				SetCursor( LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000)) );
				return TRUE;
			}
		case 10000:
			{
				SetCursor( LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_10000)) );
				return TRUE;
			}
		case 100000:
			{
				SetCursor( LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_100000)) );
				return TRUE;
			}
		case 1000000:
			{
				SetCursor( LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_1000000)) );
				return TRUE;
			}
		case 5000000:
			{
				SetCursor( LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_5000000)) );
				return TRUE;
			}	
		case 10000000:
			{
				SetCursor( LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_SCORE_10000000)) );
				return TRUE;
			}
		}
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

//��ׯ��ť
void CGameClientView::OnApplyBanker()
{
	SendEngineMessage(IDM_APPLY_BANKER,1,0);
}

//��ׯ��ť
void CGameClientView::OnCancelBanker()
{
	SendEngineMessage(IDM_APPLY_BANKER,0,0);
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

//���д��
void CGameClientView::OnBankStorage()
{
	//��ȡ�ӿ�
	CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;

	if(NULL!=pIClientKernel)
	{
		CRect btRect;
		m_btBankerStorage.GetWindowRect(&btRect);
		ShowInsureSave(pIClientKernel,CPoint(btRect.right,btRect.top));
	}
}

//����ȡ��
void CGameClientView::OnBankDraw()
{
	//��ȡ�ӿ�
	CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;

	if(NULL!=pIClientKernel)
	{
		CRect btRect;
		m_btBankerDraw.GetWindowRect(&btRect);
		ShowInsureGet(pIClientKernel,CPoint(btRect.right,btRect.top));
	}
}

// �����б���
void CGameClientView::OnValleysUp()
{
	if ( m_nShowValleyIndex > 0 )
		m_nShowValleyIndex--;

	//���½���
	InvalidGameView(0,0,0,0);
}

// �����б���
void CGameClientView::OnValleysDown()
{
	if( m_nShowValleyIndex < m_ValleysList.GetCount() - 1 )
		m_nShowValleyIndex++;

	//���½���
	InvalidGameView(0,0,0,0);
}

//����Ա����
void CGameClientView::OpenAdminWnd()
{
	if( m_pClientControlDlg != NULL )
	{
		if( !m_pClientControlDlg->IsWindowVisible() ) 
		{
			m_pClientControlDlg->ShowWindow(SW_SHOW);
			m_pClientControlDlg->UpdateControl();
		}
		else 
			m_pClientControlDlg->ShowWindow(SW_HIDE);
	}
}


//�����ע
LONGLONG CGameClientView::GetMaxPlayerScore( BYTE cbBetArea )
{	
	if ( m_wMeChairID == INVALID_CHAIR)
		return 0L;

	IClientUserItem* pMeUserItem = GetClientUserItem(m_wMeChairID);
	if ( pMeUserItem == NULL || cbBetArea >= AREA_MAX )
		return 0L;

	if( cbBetArea >= AREA_MAX )
		return 0L;

	//����ע��
	LONGLONG lNowBet = 0l;
	for (int nAreaIndex = 0; nAreaIndex < AREA_MAX; ++nAreaIndex ) 
		lNowBet += m_lPlayBet[nAreaIndex];

	//ׯ�ҽ��
	LONGLONG lBankerScore = -1;

	//������
	BYTE cbMultiple[AREA_MAX] = {MULTIPLE_XIAN, MULTIPLE_PING, MULTIPLE_ZHUANG, 
		MULTIPLE_XIAN_TIAN, MULTIPLE_ZHUANG_TIAN, MULTIPLE_TONG_DIAN, 
		MULTIPLE_XIAN_PING, MULTIPLE_ZHUANG_PING};

	//������Ӯ
	BYTE cbArae[4][4] = {	{ AREA_XIAN_DUI,	255,			AREA_MAX,			AREA_MAX }, 
							{ AREA_ZHUANG_DUI,	255,			AREA_MAX,			AREA_MAX }, 
							{ AREA_XIAN,		AREA_PING,		AREA_ZHUANG,		AREA_MAX },  
							{ AREA_XIAN_TIAN,	AREA_TONG_DUI,	AREA_ZHUANG_TIAN,	255 }};
	//�����趨
	for ( int nTopL = 0; nTopL < 4; ++nTopL )
	{
		if( cbArae[0][nTopL] == AREA_MAX )
			continue;

		for ( int nTopR = 0; nTopR < 4; ++nTopR )
		{
			if( cbArae[1][nTopR] == AREA_MAX )
				continue;

			for ( int nCentral = 0; nCentral < 4; ++nCentral )
			{
				if( cbArae[2][nCentral] == AREA_MAX )
					continue;

				for ( int nBottom = 0; nBottom < 4; ++nBottom )
				{
					if( cbArae[3][nBottom] == AREA_MAX )
						continue;

					BYTE cbWinArea[AREA_MAX] = {FALSE};

					//ָ����ʤ����
					if ( cbArae[0][nTopL] != 255 && cbArae[0][nTopL] != AREA_MAX )
						cbWinArea[cbArae[0][nTopL]] = TRUE;

					if ( cbArae[1][nTopR] != 255 && cbArae[1][nTopR] != AREA_MAX )
						cbWinArea[cbArae[1][nTopR]] = TRUE;

					if ( cbArae[2][nCentral] != 255 && cbArae[2][nCentral] != AREA_MAX )
						cbWinArea[cbArae[2][nCentral]] = TRUE;

					if ( cbArae[3][nBottom] != 255 && cbArae[3][nBottom] != AREA_MAX )
						cbWinArea[cbArae[3][nBottom]] = TRUE;

					//ѡ������Ϊ���ʤ����ͬ�ȼ�������������Ϊ����䡣�Եó������עֵ
					for ( int i = 0; i < 4; i++ )
					{
						for ( int j = 0; j < 4; j++ )
						{
							if ( cbArae[i][j] == cbBetArea )
							{
								for ( int n = 0; n < 4; ++n )
								{
									if ( cbArae[i][n] != 255 && cbArae[i][n] != AREA_MAX )
									{
										cbWinArea[cbArae[i][n]] = FALSE;
									}
								}
								cbWinArea[cbArae[i][j]] = TRUE;
							}
						}
					}

					LONGLONG lScore = m_lBankerScore;
					for (int nAreaIndex = 0; nAreaIndex < AREA_MAX; ++nAreaIndex ) 
					{
						if ( cbWinArea[nAreaIndex] == TRUE )
						{
							lScore -= m_lAllBet[nAreaIndex]*(cbMultiple[nAreaIndex] - 1);
						}
						else if ( cbWinArea[AREA_PING] == TRUE && ( nAreaIndex == AREA_XIAN || nAreaIndex == AREA_ZHUANG ) )
						{

						}
						else
						{
							lScore += m_lAllBet[nAreaIndex];
						}
					}
					if ( lBankerScore == -1 )
						lBankerScore = lScore;
					else
						lBankerScore = min(lBankerScore, lScore);
				}
			}
		}
	}

	//�����ע
	LONGLONG lMaxBet = 0L;

	//�����ע
	lMaxBet = min( m_lMeMaxScore - lNowBet, m_lAreaLimitScore - m_lPlayBet[cbBetArea]);

	lMaxBet = min( m_lMeMaxScore - lNowBet, m_lAreaLimitScore - m_lAllBet[cbBetArea]);

	lMaxBet = min( lMaxBet, lBankerScore / (cbMultiple[cbBetArea] - 1));

	//��������
	ASSERT(lMaxBet >= 0);
	lMaxBet = max(lMaxBet, 0);

	return lMaxBet;
}

//��ʼ����
void CGameClientView::DispatchCard()
{
	//�����˿�
	BYTE cbCardData = 0;
	m_nDealMove = 0;
	m_nDealIndex = 0;
	m_DispatchCard.SetCardData(&cbCardData,1);
	m_DispatchCard.SetBenchmarkPos(m_ptDispatchCard.x,m_ptDispatchCard.y);

	//���ö�ʱ��
	SetTimer(IDI_DISPATCH_CARD, 30, NULL);

	//��Ӯ����
	m_nWinShowArea = INT_MAX;
	ZeroMemory(m_nWinShowIndex,sizeof(m_nWinShowIndex));

	//���ñ�ʶ
	m_bNeedSetGameRecord = true;
}

//��������
void CGameClientView::FinishDispatchCard(bool bScene)
{
	//����ж�
	if (m_bNeedSetGameRecord == false) return;

	//���ñ�ʶ
	m_bNeedSetGameRecord = false;

	//ɾ����ʱ��
	KillTimer(IDI_DISPATCH_CARD);
	KillTimer(IDI_FLEX_MOVE);

	//������Ϣ
	FlexAnimation(enFlexDealCrad, true, false);

	//�����˿�
	m_CardControl[INDEX_PLAYER].SetCardData(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);
	m_CardControl[INDEX_BANKER].SetCardData(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);
	m_DispatchCard.SetCardData(NULL,0);

	//����
	FlashAnimation(true);

	//���ü�¼
	//�˿˵���
	BYTE cbPlayerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);
	BYTE cbBankerPoint = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);

	//��������
	enOperateResult OperateResult = enOperateResult_NULL;
	if (0 < m_lPlayAllScore) OperateResult = enOperateResult_Win;
	else if (m_lPlayAllScore < 0) OperateResult = enOperateResult_Lost;
	else OperateResult = enOperateResult_NULL;

	//�ƶ�Ӯ��
	BYTE cbWinner,cbKingWinner;
	bool bPlayerTwoPair,bBankerTwoPair;
	DeduceWinner(cbWinner, cbKingWinner, bPlayerTwoPair, bBankerTwoPair);

	//�����¼
	if(!bScene)
		SetGameHistory(OperateResult, cbPlayerPoint, cbBankerPoint, cbKingWinner, bPlayerTwoPair,bBankerTwoPair);

	//�ۼƻ���
	m_lMeStatisticScore += m_lPlayAllScore;
	m_lBankerWinScore = m_lBankerWinTempScore;

	//��������
	if ( m_lPlayAllScore > 0 ) 
		SendEngineMessage(IDM_PLAY_SOUND,(WPARAM)TEXT("END_WIN"),0);
	else if ( m_lPlayAllScore < 0 ) 
		SendEngineMessage(IDM_PLAY_SOUND,(WPARAM)TEXT("END_LOST"),0);
	else 
		SendEngineMessage(IDM_PLAY_SOUND,(WPARAM)TEXT("END_DRAW"),0);

	//������Ӯ����
	if( cbPlayerPoint < cbBankerPoint )
		m_nWinShowArea = AREA_ZHUANG;
	else if( cbPlayerPoint > cbBankerPoint )
		m_nWinShowArea = AREA_XIAN;
	else
		m_nWinShowArea = AREA_PING;

	SetTimer(IDI_WIN_TYPE_DELAY, 1100, NULL);
}

//ʤ���߿�
void CGameClientView::DrawFlashFrame(CDC *pDC, int nWidth, int nHeight)
{
	if ( m_cbGameStatus == GAME_SCENE_BET )
	{
		for ( int i = 0; i < m_ArrayCurrentFlash.GetCount(); ++i )
		{
			m_ImageFrame[m_ArrayCurrentFlash[i].cbFlashArea].AlphaDrawImage(pDC, m_ptBetFrame[m_ArrayCurrentFlash[i].cbFlashArea].x, m_ptBetFrame[m_ArrayCurrentFlash[i].cbFlashArea].y, (BYTE)m_ArrayCurrentFlash[i].nFlashAreaAlpha);
		}
	}
	else if ( m_cbGameStatus == GAME_SCENE_END && m_nFlashAreaAlpha > 10 )
	{
		for ( int i = 0; i < m_ArrayFlashArea.GetCount(); ++i )
		{
			m_ImageFrame[m_ArrayFlashArea[i]].AlphaDrawImage(pDC, m_ptBetFrame[m_ArrayFlashArea[i]].x, m_ptBetFrame[m_ArrayFlashArea[i]].y, (BYTE)m_nFlashAreaAlpha);
		}
	}
}

//�ƶ�Ӯ��
void CGameClientView::DeduceWinner( BYTE* pWinArea )
{
	//�����Ƶ�
	BYTE cbPlayerCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);
	BYTE cbBankerCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);

	//ʤ������--------------------------
	//ƽ
	if( cbPlayerCount == cbBankerCount )
	{
		pWinArea[AREA_PING] = TRUE;

		// ͬƽ��
		if ( m_cbCardCount[INDEX_PLAYER] == m_cbCardCount[INDEX_BANKER] )
		{
			for (WORD wCardIndex = 0; wCardIndex < m_cbCardCount[INDEX_PLAYER]; ++wCardIndex )
			{
				BYTE cbBankerValue = m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][wCardIndex]);
				BYTE cbPlayerValue = m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][wCardIndex]);
				if ( cbBankerValue != cbPlayerValue ) break;
			}

			if ( wCardIndex == m_cbCardCount[INDEX_PLAYER] )
			{
				pWinArea[AREA_TONG_DUI] = TRUE;
			}
		}
	}
	// ׯ
	else if ( cbPlayerCount < cbBankerCount)  
	{
		pWinArea[AREA_ZHUANG] = TRUE;

		//�����ж�
		if ( cbBankerCount == 8 || cbBankerCount == 9 )
		{
			pWinArea[AREA_ZHUANG_TIAN] = TRUE;
		}
	}
	// ��
	else 
	{
		pWinArea[AREA_XIAN] = TRUE;

		//�����ж�
		if ( cbPlayerCount == 8 || cbPlayerCount == 9 )
		{
			pWinArea[AREA_XIAN_TIAN] = TRUE;
		}
	}


	//�����ж�
	if (m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][0]) == m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][1]))
	{
		pWinArea[AREA_XIAN_DUI] = TRUE;
	}
	if (m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][0]) == m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][1]))
	{
		pWinArea[AREA_ZHUANG_DUI] = TRUE;
	}
}

//�ƶ�Ӯ��
void CGameClientView::DeduceWinner(BYTE &cbWinner, BYTE &cbKingWinner, bool &bPlayerTwoPair, bool &bBankerTwoPair)
{
	cbWinner = 0;
	cbKingWinner = 0;

	//�����Ƶ�
	BYTE cbPlayerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);
	BYTE cbBankerCount=m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);

	//ʤ�����
	if (cbPlayerCount == cbBankerCount)
	{
		cbWinner=AREA_PING;

		//ͬ��ƽ�ж�
		bool bAllPointSame = false;
		if ( m_cbCardCount[INDEX_PLAYER] == m_cbCardCount[INDEX_BANKER] )
		{
			for (WORD wCardIndex = 0; wCardIndex < m_cbCardCount[INDEX_PLAYER]; ++wCardIndex )
			{
				BYTE cbBankerValue = m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][wCardIndex]);
				BYTE cbPlayerValue = m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][wCardIndex]);
				if ( cbBankerValue != cbPlayerValue ) break;
			}
			if ( wCardIndex == m_cbCardCount[INDEX_PLAYER] ) bAllPointSame = true;
		}
		if ( bAllPointSame ) cbKingWinner = AREA_TONG_DUI;
	}
	else if (cbPlayerCount<cbBankerCount) 
	{
		cbWinner=AREA_ZHUANG;

		//�����ж�
		if ( cbBankerCount == 8 || cbBankerCount == 9 ) cbKingWinner = AREA_ZHUANG_TIAN;
	}
	else 
	{
		cbWinner=AREA_XIAN;

		//�����ж�
		if ( cbPlayerCount == 8 || cbPlayerCount == 9 ) cbKingWinner = AREA_XIAN_TIAN;
	}

	//�����ж�
	bPlayerTwoPair=false;
	bBankerTwoPair=false;
	if (m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][0]) == m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_PLAYER][1]))
		bPlayerTwoPair=true;
	if (m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][0]) == m_GameLogic.GetCardValue(m_cbTableCardArray[INDEX_BANKER][1]))
		bBankerTwoPair=true;
}

//������ʾ
void CGameClientView::SetDispatchCardTips()
{
	if (m_CardControl[INDEX_PLAYER].GetCardCount() + m_CardControl[INDEX_BANKER].GetCardCount() < 4) 
		return;

	//if ( m_CardControl[INDEX_PLAYER].GetCardCount() == m_cbCardCount[INDEX_PLAYER] && m_CardControl[INDEX_BANKER].GetCardCount() == m_cbCardCount[INDEX_BANKER])
	//{
	//	//�������
	//	BYTE cbBankerCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],m_cbCardCount[INDEX_BANKER]);
	//	BYTE cbPlayerCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],m_cbCardCount[INDEX_PLAYER]);

	//	CString strTips;
	//	if ( cbPlayerCount > cbBankerCount)
	//	{
	//		strTips.Format(TEXT("��%d�㣬ׯ%d�㣬��Ӯ��\n"), cbPlayerCount, cbBankerCount);
	//		m_strDispatchCardTips += strTips;
	//	}
	//	else if ( cbPlayerCount < cbBankerCount)
	//	{
	//		strTips.Format(TEXT("��%d�㣬ׯ%d�㣬ׯӮ��\n"), cbPlayerCount, cbBankerCount);
	//		m_strDispatchCardTips += strTips;
	//	}
	//	if ( cbPlayerCount == cbBankerCount)
	//	{
	//		strTips.Format(TEXT("��%d�㣬ׯ%d�㣬ƽ��\n"), cbPlayerCount, cbBankerCount);
	//		m_strDispatchCardTips += strTips;
	//	}
	//	return;
	//}


	//�������
	BYTE cbBankerCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_BANKER],2);
	BYTE cbPlayerTwoCardCount = m_GameLogic.GetCardListPip(m_cbTableCardArray[INDEX_PLAYER],2);

	//�мҲ���	
	if( cbPlayerTwoCardCount <= 5 && cbBankerCount < 8 
		&& m_CardControl[INDEX_PLAYER].GetCardCount() + m_CardControl[INDEX_BANKER].GetCardCount() == 4)
	{		
		CString strTips;
		strTips.Format(TEXT("��%d�㣬ׯ%d�㣬�м�������\n"), cbPlayerTwoCardCount, cbBankerCount);
		m_strDispatchCardTips = strTips;
		return;
	}

	if ( m_CardControl[INDEX_BANKER].GetCardCount() == 3 ) return;

	BYTE cbPlayerThirdCardValue = 0 ;	//�������Ƶ���

	//�������
	cbPlayerThirdCardValue = m_GameLogic.GetCardPip(m_cbTableCardArray[INDEX_PLAYER][2]);

	//ׯ�Ҳ���
	CString strTips;
	if( cbPlayerTwoCardCount < 8 && cbBankerCount < 8 )
	{
		switch(cbBankerCount)
		{
		case 0:
		case 1:
		case 2:
			{
				strTips.Format(TEXT("��ǰ������%d�㣬ׯ%d�㣬ׯ��������\n"), cbPlayerTwoCardCount, cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			break;
		case 3:
			if(m_cbCardCount[INDEX_PLAYER] == 3 && cbPlayerThirdCardValue != 8)
			{
				strTips.Format(TEXT("�е�������%d�㣬ׯ%d�㣬ׯ��������\n"), cbPlayerThirdCardValue, cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			else if (m_cbCardCount[INDEX_PLAYER] == 2) 
			{
				strTips.Format(TEXT("�в����ƣ�ׯ%d�㣬ׯ��������\n"), cbBankerCount);
				m_strDispatchCardTips += strTips;
			}			
			break;
		case 4:
			if(m_cbCardCount[INDEX_PLAYER] == 3 && cbPlayerThirdCardValue != 1 && cbPlayerThirdCardValue != 8 && cbPlayerThirdCardValue != 9 && cbPlayerThirdCardValue != 0)
			{
				strTips.Format(TEXT("�е�������%d�㣬ׯ%d�㣬ׯ��������\n"), cbPlayerThirdCardValue, cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			else if ( m_cbCardCount[INDEX_PLAYER] == 2) 
			{
				strTips.Format(TEXT("�в����ƣ�ׯ%d�㣬ׯ��������\n"), cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			break;
		case 5:
			if( m_cbCardCount[INDEX_PLAYER] == 3 && cbPlayerThirdCardValue != 1 && cbPlayerThirdCardValue != 2 && cbPlayerThirdCardValue != 3  && cbPlayerThirdCardValue != 8 && cbPlayerThirdCardValue != 9 &&  cbPlayerThirdCardValue != 0)
			{
				strTips.Format(TEXT("�е�������%d�㣬ׯ%d�㣬ׯ��������\n"), cbPlayerThirdCardValue, cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			else if ( m_cbCardCount[INDEX_PLAYER] == 2 ) 
			{
				strTips.Format(TEXT("�в����ƣ�ׯ%d�㣬ׯ��������\n"), cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			break;
		case 6:
			if( m_cbCardCount[INDEX_PLAYER] == 3 && ( cbPlayerThirdCardValue == 6 || cbPlayerThirdCardValue == 7 ))
			{
				strTips.Format(TEXT("�е�������%d�㣬ׯ%d�㣬ׯ��������\n"), cbPlayerThirdCardValue, cbBankerCount);
				m_strDispatchCardTips += strTips;
			}
			break;
			//���벹��
		case 7:
		case 8:
		case 9:
			break;
		default:
			break;
		}
	}

	return ;
}

// ��Ӷ���
CString CGameClientView::AddComma( LONGLONG lScore , bool bPlus /*= false*/)
{
	CString strScore;
	CString strReturn;
	LONGLONG lNumber = lScore;
	if ( lScore < 0 )
		lNumber = -lNumber;

	strScore.Format(TEXT("%I64d"), lNumber);

	int nStrCount = 0;
	for( int i = strScore.GetLength() - 1; i >= 0; )
	{
		if( (nStrCount%3) == 0 && nStrCount != 0 )
		{
			strReturn.Insert(0, ',');
			nStrCount = 0;
		}
		else
		{
			strReturn.Insert(0, strScore.GetAt(i));
			nStrCount++;
			i--;
		}
	}

	if ( lScore < 0 )
		strReturn.Insert(0, '-');

	if ( bPlus && lScore > 0 )
		strReturn.Insert(0, '+');

	return strReturn;
}

// ɾ������
LONGLONG CGameClientView::DeleteComma( CString strScore )
{
	LONGLONG lScore = 0l;
	strScore.Remove(',');
	if ( !strScore.IsEmpty() )
		lScore = _ttoi64(strScore);

	return lScore;
}

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};


	//��������
	UINT uAlign = pDC->GetTextAlign();
	UINT nDrawFormat = 0;
	if ( uAlign&TA_CENTER )
		nDrawFormat |= DT_CENTER;
	else if( uAlign&TA_RIGHT )
		nDrawFormat |= DT_RIGHT;
	else
		nDrawFormat |= DT_LEFT;

	if( uAlign&TA_BOTTOM )
		nDrawFormat |= DT_BOTTOM;
	else 
		nDrawFormat |= DT_TOP;

	//�滭�߿�
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		//�滭�ַ�
		CDFontEx::DrawText(this,pDC,  12, 400, pszString, nXPos+nXExcursion[i],nYPos+nYExcursion[i], crFrame, nDrawFormat);
	}
	//�滭�ַ�
	CDFontEx::DrawText(this,pDC,  12, 400, pszString, nXPos,nYPos, crText, nDrawFormat);

}

void CGameClientView::DrawTextString( CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, UINT nDrawFormat )
{
	//��������
	INT nStringLength=lstrlen(pszString);
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		//����λ��
		CRect rcFrame;
		rcFrame.top=rcRect.top+nYExcursion[i];
		rcFrame.left=rcRect.left+nXExcursion[i];
		rcFrame.right=rcRect.right+nXExcursion[i];
		rcFrame.bottom=rcRect.bottom+nYExcursion[i];

		//�滭�ַ�
		CDFontEx::DrawText(this,pDC,  12, 400, pszString, rcFrame, crFrame, nDrawFormat);
	}

	//�滭�ַ�
	CDFontEx::DrawText(this,pDC,  12, 400, pszString, rcRect, crText, nDrawFormat);
}

//�滭�ַ�
void CGameClientView::DrawTextString( CDC * pDC, CDFontEx* pFont, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos )
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};


	//��������
	UINT uAlign = pDC->GetTextAlign();
	UINT nDrawFormat = 0;
	if ( uAlign&TA_CENTER )
		nDrawFormat |= DT_CENTER;
	else if( uAlign&TA_RIGHT )
		nDrawFormat |= DT_RIGHT;
	else
		nDrawFormat |= DT_LEFT;

	if( uAlign&TA_BOTTOM )
		nDrawFormat |= DT_BOTTOM;
	else 
		nDrawFormat |= DT_TOP;

	//�滭�߿�
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		//�滭�ַ�
		pFont->DrawText(pDC, pszString, nXPos+nXExcursion[i],nYPos+nYExcursion[i], crFrame, nDrawFormat);
	}

	//�滭�ַ�
	pFont->DrawText( pDC, pszString, nXPos,nYPos, crText, nDrawFormat);
}

//�滭�ַ�
void CGameClientView::DrawTextString( CDC * pDC, CDFontEx* pFont, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, UINT nDrawFormat )
{
	//��������
	INT nStringLength=lstrlen(pszString);
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		//����λ��
		CRect rcFrame;
		rcFrame.top=rcRect.top+nYExcursion[i];
		rcFrame.left=rcRect.left+nXExcursion[i];
		rcFrame.right=rcRect.right+nXExcursion[i];
		rcFrame.bottom=rcRect.bottom+nYExcursion[i];

		//�滭�ַ�
		pFont->DrawText( pDC, pszString, rcFrame, crFrame, nDrawFormat);
	}

	//�滭�ַ�
	pFont->DrawText( pDC, pszString, rcRect, crText, nDrawFormat);
}

// �滭����
void CGameClientView::DrawNumber( CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum,INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),TEXT("%I64d"),lOutNum);
	DrawNumber(pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos, uFormat);
}

// �滭����
void CGameClientView::DrawNumber( CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, CString szOutNum, INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNumT[128] = {0};
	_sntprintf(szOutNumT,CountArray(szOutNumT),TEXT("%s"),szOutNum);
	DrawNumber(pDC, ImageNumber, szImageNum, szOutNumT, nXPos, nYPos, uFormat);
}


// �滭����
void CGameClientView::DrawNumber(CDC * pDC , CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat /*= DT_LEFT*/)
{
	// ������Դ
	INT nNumberHeight=ImageNumber->GetHeight();
	INT nNumberWidth=ImageNumber->GetWidth()/lstrlen(szImageNum);

	if ( uFormat == DT_CENTER )
	{
		nXPos -= (INT)(((DOUBLE)(lstrlen(szOutNum)) / 2.0) * nNumberWidth);
	}
	else if ( uFormat == DT_RIGHT )
	{
		nXPos -= lstrlen(szOutNum) * nNumberWidth;
	}

	for ( INT i = 0; i < lstrlen(szOutNum); ++i )
	{
		for ( INT j = 0; j < lstrlen(szImageNum); ++j )
		{
			if ( szOutNum[i] == szImageNum[j] && szOutNum[i] != '\0' )
			{
				ImageNumber->DrawImage(pDC, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
				nXPos += nNumberWidth;
				break;
			}
		}
	}
}


// �滭ʱ��
void CGameClientView::DrawTime(CDC * pDC, WORD wUserTime,INT nXPos, INT nYPos)
{
	//��������
	m_ImageTimeBack.DrawImage( pDC, nXPos, nYPos - m_ImageTimeBack.GetHeight()/2 );

	TCHAR szOutNum[128] = {0};
	if ( wUserTime > 99 )
		_sntprintf(szOutNum,CountArray(szOutNum),TEXT("99"));
	else if ( wUserTime < 10 )
		_sntprintf(szOutNum,CountArray(szOutNum),TEXT("0%d"), wUserTime);
	else
		_sntprintf(szOutNum,CountArray(szOutNum),TEXT("%d"), wUserTime);

	//��������
	DrawNumber( pDC, &m_ImageTimeNumber, TEXT("0123456789"), szOutNum, nXPos + m_ImageTimeBack.GetWidth()/2, nYPos - 6, DT_CENTER);
}

//��������
void CGameClientView::FlexAnimation( enFlexMode nFlexMode, bool bShow , bool bMove /*= true*/)
{
	//��ֵ����
	m_enFlexMode = nFlexMode;

	// ָ��
	if ( nFlexMode == enFlexNULL )
	{
		m_pImageFlex = NULL;
		KillTimer(IDI_FLEX_MOVE);
		InvalidGameView(0,0,0,0);
		return;
	}
	else if ( nFlexMode == enFlexBetTip )
	{
		m_pImageFlex = &m_ImageBetTip;
	}
	else if ( nFlexMode == enFlexDealCrad )
	{
		m_pImageFlex = &m_ImageDealBack;
	}
	else if ( nFlexMode == enFlexGameEnd )
	{
		m_pImageFlex = &m_ImageGameEnd;
	}

	//  �޶��� ������ʾ
	if ( !bMove )
	{
		if ( !bShow )
		{
			m_pImageFlex = NULL;
			m_bFlexShow = false;
			m_nFlexMove = 0;
		}
		else
		{
			m_bFlexShow = true;
			m_nFlexMove = FLEX_MOVE_COUNT;
			m_ptFlexMove = m_ptFlexBeing;
			m_ptFlexMove.y += 137;
		}
		return;
	}

	// ״̬һ������ʾ
	if( m_bFlexShow == bShow)
		return;

	// ��������
	m_bFlexShow = bShow;
	SetTimer( IDI_FLEX_MOVE, 30, NULL );
	return;
}

//��˸����
void CGameClientView::FlashAnimation( bool bBegin )
{
	if ( !bBegin )
	{
		m_ArrayFlashArea.RemoveAll();
		m_nFlashAreaAlpha = 0;
		m_bFlashAreaAlpha = false;
		KillTimer(IDI_FLASH_WINNER);
		return;
	}

	//�ƶ����
	BYTE cbWinArea[AREA_MAX] = {FALSE};
	DeduceWinner(cbWinArea);

	for ( int i = 0 ; i < AREA_MAX; ++i )
	{
		if ( cbWinArea[i] == TRUE )
		{
			m_ArrayFlashArea.Add(i);
		}
	}
	m_nFlashAreaAlpha = 0;
	m_bFlashAreaAlpha = TRUE;
	SetTimer( IDI_FLASH_WINNER, 30, NULL );
}

//������ע���
void CGameClientView::ControlBetDetection( BYTE cbViewIndex, LONGLONG lScoreCount, WORD wChair )
{
	if (m_pClientControlDlg == NULL || wChair >= GAME_PLAYER )
		return;

	//��ȡ���
	m_lAllPlayBet[wChair][cbViewIndex] += lScoreCount;

	//�����Ϣ
	IClientUserItem* pUserData = GetClientUserItem(wChair);
	int nListIndex = 0;
	if ( cbViewIndex == AREA_XIAN )
		nListIndex = 0;
	else if ( cbViewIndex == AREA_PING )
		nListIndex = 1;
	else if ( cbViewIndex == AREA_ZHUANG )
		nListIndex = 2;
	else if ( cbViewIndex == AREA_XIAN_TIAN )
		nListIndex = 3;
	else if ( cbViewIndex == AREA_ZHUANG_TIAN )
		nListIndex = 4;
	else if ( cbViewIndex == AREA_TONG_DUI )
		nListIndex = 5;
	else if ( cbViewIndex == AREA_XIAN_DUI )
		nListIndex = 6;
	else if ( cbViewIndex == AREA_ZHUANG_DUI )
		nListIndex = 7;
	else
		ASSERT(FALSE);

	//�������
	LVFINDINFO lvFindInfo;
	ZeroMemory( &lvFindInfo, sizeof( lvFindInfo ) );
	lvFindInfo.flags = LVFI_STRING;
	lvFindInfo.psz = (LPCTSTR)pUserData->GetNickName();

	//��������
	int nItem = m_SkinListCtrl[nListIndex].FindItem( &lvFindInfo );
	if ( nItem == -1 )
	{
		nItem = m_SkinListCtrl[nListIndex].InsertItem(m_SkinListCtrl[nListIndex].GetItemCount(), pUserData->GetNickName());//������
	}


	CString strScore;
	strScore.Format(TEXT("%I64d"), m_lAllPlayBet[wChair][cbViewIndex]);
	m_SkinListCtrl[nListIndex].SetItemText(nItem, 1, strScore);		//��������
}



//////////////////////////////////////////////////////////////////////////
