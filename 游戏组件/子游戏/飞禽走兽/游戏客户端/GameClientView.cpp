#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientEngine.h"


//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FLASH_WINNER			 100									//������ʶ
#define IDI_SHOW_CHANGE_BANKER		 101									//�ֻ�ׯ��
//#define IDI_DISPATCH_CARD			 102									//���Ʊ�ʶ
#define IDI_SHOWDISPATCH_CARD_TIP	 103									//������ʾ
#define IDI_MOVECARD_END			 104									//�Ƅ��ƽY��
#define IDI_HANDLELEAVE_ANIMATION	 105  
#define IDI_FLASH_CARD				 106									//������ʶ
#define IDI_FLASH_RAND_SIDE			 107									//������ʶ
#define IDI_END_FLASH_CARD			 108
#define IDI_FLASH_ADD_MULTI          109									//������ʶ
#define IDI_END_ADD_MULTI            110									//��������
#define IDI_FLASH_ANOTHER_CARD       111		
#define IDI_SHOW_ANOTHER_CARD_RESULT 112	
#define IDI_SHOW_ANIMAL              113									//��ʾ����



#define IDI_FLASH_NUMBER1              114									//
#define IDI_END_FLASH_NUMBER1          115									//

#define IDI_FLASH_NUMBER2              116									//
#define IDI_END_FLASH_NUMBER2          117									//

//��������ע,�������ж�ʱ����������(1000�Ժ�),ר�������ṩ
#define IDI_ANDROID_BET				1000								

//��ť��ʶ
#define IDC_JETTON_BUTTON_10		200									//��ť��ʶ
#define IDC_JETTON_BUTTON_100		201									//��ť��ʶ
#define IDC_JETTON_BUTTON_1000		202									//��ť��ʶ
#define IDC_JETTON_BUTTON_10000		203									//��ť��ʶ
#define IDC_JETTON_BUTTON_100000	204									//��ť��ʶ
#define IDC_APPY_BANKER				206									//��ť��ʶ
#define IDC_CANCEL_BANKER			207									//��ť��ʶ
#define IDC_SCORE_MOVE_L			209									//��ť��ʶ
#define IDC_SCORE_MOVE_R			210									//��ť��ʶ
#define IDC_VIEW_CHART				211									//��ť��ʶ
#define IDC_AUTO_OPEN_CARD			213									//��ť��ʶ
#define IDC_OPEN_CARD				214									//��ť��ʶ
#define IDC_BANK					215									//��ť��ʶ
#define IDC_BANK_STORAGE			217									//��ť��ʶ
#define IDC_BANK_DRAW				218									//��ť��ʶ
#define IDC_UP						223									//��ť��ʶ
#define IDC_DOWN					224									//��ť��ʶ
#define IDC_ADMIN					226									//��ť��ʶ

#define IDC_BUTTON_ADD_0			227									//��ť��ʶ
#define IDC_BUTTON_ADD_1			228									//��ť��ʶ
#define IDC_BUTTON_ADD_2			229									//��ť��ʶ
#define IDC_BUTTON_ADD_3			230									//��ť��ʶ
#define IDC_BUTTON_ADD_4			231									//��ť��ʶ
#define IDC_BUTTON_ADD_5			232									//��ť��ʶ
#define IDC_BUTTON_ADD_6			233									//��ť��ʶ
#define IDC_BUTTON_ADD_7			234									//��ť��ʶ
#define IDC_BUTTON_ADD_8			235									//��ť��ʶ
#define IDC_BUTTON_ADD_9			236									//��ť��ʶ
#define IDC_BUTTON_ADD_10			237									//��ť��ʶ

#define IDC_COMMIT_PLACE_JETTON     238									//��ť��ʶ
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_ADMIN,OpenAdminWnd)
	ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
	ON_BN_CLICKED(IDC_SCORE_MOVE_L, OnScoreMoveL)
	ON_BN_CLICKED(IDC_SCORE_MOVE_R, OnScoreMoveR)
	ON_BN_CLICKED(IDC_BUTTON_ADD_0,OnAddScore0)
	ON_BN_CLICKED(IDC_BUTTON_ADD_1,OnAddScore1)
	ON_BN_CLICKED(IDC_BUTTON_ADD_2,OnAddScore2)
	ON_BN_CLICKED(IDC_BUTTON_ADD_3,OnAddScore3)
	ON_BN_CLICKED(IDC_BUTTON_ADD_4,OnAddScore4)
	ON_BN_CLICKED(IDC_BUTTON_ADD_5,OnAddScore5)
	ON_BN_CLICKED(IDC_BUTTON_ADD_6,OnAddScore6)
	ON_BN_CLICKED(IDC_BUTTON_ADD_7,OnAddScore7)
	ON_BN_CLICKED(IDC_BUTTON_ADD_8,OnAddScore8)
	ON_BN_CLICKED(IDC_BUTTON_ADD_9,OnAddScore9)
	ON_BN_CLICKED(IDC_BUTTON_ADD_10,OnAddScore10)
	ON_MESSAGE(WM_VIEWLBTUP,OnViLBtUp)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView() 
{
	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));
	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_lBankerScore=0L;	
	m_lBankerWinScore=0L;
	m_lTmpBankerWinScore=0;
	m_CarIndex = 0;
	m_lRobotMaxJetton = 5000000l;

	//���ֳɼ�
	m_lMeCurGameScore=0L;	
	m_lMeCurGameReturnScore=0L;
	m_lBankerCurGameScore=0L;
	m_lGameRevenue=0L;

	//״̬��Ϣ
	m_lCurrentJetton=0L;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=FALSE;
	m_bNeedSetGameRecord=FALSE;
	
	m_bFlashResult=FALSE;
	m_blMoveFinish = FALSE;
	

	m_lMeCurGameScore=0L;			
	m_lMeCurGameReturnScore=0L;	
	m_lBankerCurGameScore=0L;	
	m_blCanStore=false;

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
	m_ImageWinFlags.LoadImage(hInstance,TEXT("IDB_IDB_FLAG_PNG"));
	m_ImageScoreNumber.LoadFromResource(hInstance,IDB_SCORE_NUMBER);
	m_ImageMeScoreNumber.LoadFromResource(hInstance,IDB_ME_SCORE_NUMBER);
	m_ViewBackPng.LoadImage(hInstance,TEXT("IDB_VIEW_BACK_PNG"));
	m_idb_selPng.LoadImage(hInstance,TEXT("IDB_IDB_SEL_PNG"));
	m_ImageTimeFlagPng.LoadImage(hInstance,TEXT("IDB_TIME_PNG"));
	m_pngTimeBack.LoadImage(hInstance,TEXT("TIME_BACK"));
	m_pngResultFShayu.LoadImage(hInstance,TEXT("RESULT_FRAME_SHAYU"));
	m_pngResultFOther.LoadImage(hInstance,TEXT("RESULT_FRAME_OTHER"));
	m_pngResultAnimal.LoadImage(hInstance,TEXT("RESULT_ANIMAL"));
	m_pngNumberASymbol.LoadImage(hInstance,TEXT("NUMBER_AND_SYMBOL"));
	m_pngLuZiR.LoadImage(hInstance,TEXT("LUZI_RIGHT"));
	m_pngLuZiM.LoadImage(hInstance,TEXT("LUZI_MIDDLE"));
	m_pngLuZiL.LoadImage(hInstance,TEXT("LUZI_LEFT"));
	m_pngLuZiAnimal.LoadImage(hInstance,TEXT("LUZI_ANIMAL"));
	m_pngCoinNumber.LoadImage(hInstance,TEXT("COIN_NUMBER"));
	TCHAR szBuffer[128]=TEXT("");
	for (int i=0;i<AREA_ALL;i++)
	{
		myprintf(szBuffer,CountArray(szBuffer),TEXT("CARTOON_%d"),i+1);
		m_pngCarton[i].LoadImage(hInstance,szBuffer);
	}
	m_pngAnimalLight.LoadImage(hInstance,TEXT("ANIMAL_LIGHT"));
	m_pngTimeNumber.LoadImage(hInstance,TEXT("TIME_NUMBER"));

	//��Ϸ��Ϣ
	for ( int i = 0; i < CountArray(m_nAnimalPercent); ++i)
		m_nAnimalPercent[i] = 0;

	m_cbRandAddMulti=0;
	m_cbShaYuAddMulti=0;

	m_ShaYuIndex = 0;

	m_bShowShaYuResult = false;
	m_bShowAnotherResult = false;
	m_cbAnimalIndex=0;
	m_bShowCartoon=false;
	m_nPlayGameTimes=0;

	m_cbOneDigit = 0;						
	m_cbTwoDigit = 0;						
	m_cbRandOneDigit = 0;					
	m_cbRandTwoDigit = 0;		

	m_nAllRunTimes=1;
	
	return;
}

//��������
CGameClientView::~CGameClientView(void)
{

	if(m_hInst)
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
	
	m_GameRecord.ShowWindow(SW_HIDE);
	
	//��ע��ť
	m_btJetton10.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10);
	m_btJetton100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100);
	m_btJetton1000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_1000);
	m_btJetton10000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_10000);
	m_btJetton100000.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_JETTON_BUTTON_100000);

	m_btJetton10.SetButtonColor(RGB(255,0,0));					
	m_btJetton100.SetButtonColor(RGB(255,0,0));							
	m_btJetton1000.SetButtonColor(RGB(255,0,0));							
	m_btJetton10000.SetButtonColor(RGB(255,0,0));								
	m_btJetton100000.SetButtonColor(RGB(255,0,0));	
	
	for ( int i = 0; i < AREA_COUNT; ++i)
	{
		m_btAdd[i].Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BUTTON_ADD_0+i);
	}
	
	m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
	m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);

		
	m_btBankStorage.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BANK_STORAGE);
	m_btBankDraw.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BANK_DRAW);

	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	

	m_btJetton10.SetButtonImage(IDB_BT_JETTON_10,hResInstance,false,false);
	m_btJetton100.SetButtonImage(IDB_BT_JETTON_100,hResInstance,false,false);
	m_btJetton1000.SetButtonImage(IDB_BT_JETTON_1000,hResInstance,false,false);
	m_btJetton10000.SetButtonImage(IDB_BT_JETTON_10000,hResInstance,false,false);
	m_btJetton100000.SetButtonImage(IDB_BT_JETTON_100000,hResInstance,false,true);
	
	m_btAdd[0].SetButtonImage( IDB_BT_ANIMAL_1,  hResInstance , false, false);
	m_btAdd[1].SetButtonImage( IDB_BT_ANIMAL_2,  hResInstance , false, false);
	m_btAdd[2].SetButtonImage( IDB_BT_ANIMAL_3,  hResInstance , false, false);
	m_btAdd[3].SetButtonImage( IDB_BT_ANIMAL_4,  hResInstance , false, false);
	m_btAdd[4].SetButtonImage( IDB_BT_ANIMAL_5,  hResInstance , false, false);
	m_btAdd[5].SetButtonImage( IDB_BT_ANIMAL_6,  hResInstance , false, false);
	m_btAdd[6].SetButtonImage( IDB_BT_ANIMAL_7,  hResInstance , false, false);
	m_btAdd[7].SetButtonImage( IDB_BT_ANIMAL_8,  hResInstance , false, false);
	m_btAdd[8].SetButtonImage( IDB_BT_ANIMAL_9,  hResInstance , false, false);
	m_btAdd[9].SetButtonImage( IDB_BT_ANIMAL_10,  hResInstance , false, false);
	m_btAdd[10].SetButtonImage( IDB_BT_ANIMAL_11,  hResInstance , false, false);
	m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false,false);
	m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false,false);
	m_btBankStorage.SetButtonImage(IDB_BT_STORAGE,hResInstance,false,false);
	m_btBankDraw.SetButtonImage(IDB_BT_DRAW,hResInstance,false,false);
	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);
	SwitchToCheck();
	//����
	m_hInst = NULL;
	m_pAdminControl = NULL;
	m_hInst = LoadLibrary(TEXT("ZodiacBattleClientControl.dll"));
	if ( m_hInst )
	{
		typedef void * (*CREATE)(CWnd* pParentWnd); 
		CREATE ClientControl = (CREATE)GetProcAddress(m_hInst,"CreateClientControl"); 
		if ( ClientControl )
		{
			m_pAdminControl = static_cast<IClientControlDlg*>(ClientControl(this));
			m_pAdminControl->ShowWindow( SW_HIDE );
		}
	}
	
	m_btBankStorage.ShowWindow(SW_SHOW);
	m_btBankDraw.ShowWindow(SW_SHOW);

	return 0;
}

//���ý���
VOID CGameClientView::ResetGameView()
{
	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

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

	//״̬��Ϣ
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	
	m_bFlashResult=false;
	m_bShowGameResult=false;
	
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
	//�������
	CleanUserJetton();
	m_cbRandAddMulti=0;
	m_cbShaYuAddMulti=0;
	m_ShaYuIndex=0;

	m_bShowShaYuResult = false;
	m_bShowAnotherResult = false;
	m_cbAnimalIndex=0;
	m_bShowCartoon=false;
	ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));
	m_cbOneDigit = 0;						
	m_cbTwoDigit = 0;						
	m_cbRandOneDigit = 0;					
	m_cbRandTwoDigit = 0;	

	return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//λ����Ϣ
	CSize Size;
	Size.cy = Size.cy/2;
	int iWidth =756;
	int iHeight =705;

	int LifeWidth = (nWidth-iWidth)/2;
	int TopHeight = (nHeight-iHeight)/2;

	m_TopHeight = TopHeight;
	m_LifeWidth = LifeWidth;

	int ix = nWidth/2+20;
	int iy = nHeight/2+6;
	int iAready = 52;
	int iAreadx =77;
	
	int iStartX = nWidth/2+20-362-17-5+75;
	int iStartY = nHeight/2+6-276-8-36-15;

	int xLength=75;
	int yLength=65;
	for (int i = 0;i<7;i++)
	{
		m_CarRect[i].top =iStartY;
		m_CarRect[i].left =iStartX+i*xLength+6;
	}

	for (int i = 7;i<14;i++)
	{
		m_CarRect[i].top =iStartY+(i-7)*yLength;
		m_CarRect[i].left =iStartX+7*xLength +21;
	}

	for (int i = 20;i>13;i--)
	{
		m_CarRect[i].top =iStartY+6*yLength;
		m_CarRect[i].left =iStartX+(20-i)*xLength;
	}

	for (int i = 27;i>20;i--)
	{
		m_CarRect[i].top =iStartY+(27-i)*yLength;
		m_CarRect[i].left =iStartX-xLength;
	}

	m_RectArea[0].SetRect(ix-279,iy-138,ix-151,iy);
	m_RectArea[1].SetRect(ix-151,iy-138,ix-21 ,iy);
	m_RectArea[2].SetRect(ix-21 ,iy-138,ix+109,iy);
	m_RectArea[3].SetRect(ix+109,iy-138,ix+239,iy);
	
	m_RectArea[4].SetRect(ix-279,iy,ix-151,iy+138);
	m_RectArea[5].SetRect(ix-151,iy,ix-21 ,iy+138);
	m_RectArea[6].SetRect(ix-21 ,iy,ix+109,iy+138);
	m_RectArea[7].SetRect(ix+109,iy,ix+239,iy+138);

	
	int JettonBeginX=m_LifeWidth+100;
	int JettonBeginY=TopHeight+197;
	int xJettonLength = 58;
	for (int i = 0;i<AREA_COUNT;i++)
	{
		m_PointAnimalRand[i].SetPoint(JettonBeginX+i*xJettonLength, JettonBeginY);
		m_PointJetton[i].SetPoint(m_RectArea[i].left, m_RectArea[i].top);
	}

	int nYAddPos =TopHeight+613-24+13;
	int nXAddPos =LifeWidth+197+74;

	CRect rcJettonAdd;
	m_btAdd[0].GetWindowRect(&rcJettonAdd);
	int addSpaceAdd=63;
	int addScoreWidth=30;
		
	for ( int i = 0; i < AREA_COUNT-3; ++i)
	{
		if(i==0||i==1)m_PointAddButton[i].SetPoint(nXAddPos + addSpaceAdd * i-47,nYAddPos-27);
		else m_PointAddButton[i].SetPoint(nXAddPos + addSpaceAdd * i-47+2,nYAddPos-27);

		m_PointJettonScore[i].SetPoint(m_PointAddButton[i].x+rcJettonAdd.Width()-addSpaceAdd+18+49-53,m_PointAddButton[i].y+23+27);

	}

	nXAddPos+=274;
	nYAddPos-=66;

	for ( int i = AREA_COUNT-3; i < AREA_COUNT; ++i)
	{
		m_PointAddButton[i].SetPoint(nXAddPos + (addSpaceAdd+20) * (i-AREA_COUNT+3)-68,nYAddPos-12);
		m_PointJettonScore[i].SetPoint(m_PointAddButton[i].x+rcJettonAdd.Width()-(addSpaceAdd+20)+15-53+68,m_PointAddButton[i].y+18+12);

	}
	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(33);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	m_MeInfoRect.top = TopHeight+22;
	m_MeInfoRect.left = LifeWidth+265+389;

	m_btBankStorage.EnableWindow(TRUE);

	//���밴ť
	CRect rcJetton;
	m_btJetton10.GetWindowRect(&rcJetton);
	int nYPos =TopHeight+613-41;
	int nXPos =LifeWidth+197+103;
	int nSpace=8;
	m_nWinFlagsExcursionX = LifeWidth+220;	
	m_nWinFlagsExcursionY = TopHeight+593;
	nYPos -=46;
	DeferWindowPos(hDwp,m_btJetton10,NULL,nXPos+2,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton100,NULL,nXPos + nSpace + rcJetton.Width(),nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton1000,NULL,nXPos + nSpace * 2 + rcJetton.Width() * 2-2,nYPos,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton10000,NULL,nXPos+2 ,nYPos +25 ,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btJetton100000,NULL,nXPos + nSpace  + rcJetton.Width() ,nYPos +25 ,0,0,uFlags|SWP_NOSIZE);
	
	for ( int i = 0; i < AREA_COUNT; ++i)
	{
		DeferWindowPos(hDwp,m_btAdd[i],NULL,m_PointAddButton[i].x,m_PointAddButton[i].y,0,0,uFlags|SWP_NOSIZE);

		m_btAdd[i].EnableWindow(TRUE);

	}

	m_LuZiStart.SetPoint(m_LifeWidth+130,TopHeight+223);
	DeferWindowPos(hDwp,m_btScoreMoveL,NULL,m_LuZiStart.x+6,m_LuZiStart.y+5,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScoreMoveR,NULL,m_LuZiStart.x+441,m_LuZiStart.y+5,0,0,uFlags|SWP_NOSIZE);
	//������ť
	DeferWindowPos(hDwp,m_btBankStorage,NULL,nWidth/2+180 -517,nHeight/2+283,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBankDraw,NULL,nWidth/2+240 -517,nHeight/2+283,0,0,uFlags|SWP_NOSIZE);
	//�����ƶ�
	EndDeferWindowPos(hDwp);

	return;
}
void CGameClientView::SwitchToCheck()
{
	SwithToNormalView();
	return;

}
void CGameClientView::SwithToNormalView()
{
	m_btScoreMoveL.ShowWindow(SW_SHOW);
	m_btScoreMoveR.ShowWindow(SW_SHOW);
	m_btJetton10.ShowWindow(SW_SHOW);
	m_btJetton100.ShowWindow(SW_SHOW);
	m_btJetton1000.ShowWindow(SW_SHOW);
	m_btJetton10000.ShowWindow(SW_SHOW);
	m_btJetton100000.ShowWindow(SW_SHOW);
	m_DrawBack = true;
}
//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{

	//��ȡ���
	IClientUserItem * pClientUserItem=m_wBankerUser==INVALID_CHAIR ? NULL :GetClientUserItem(m_wBankerUser);
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);
	m_ViewBackPng.DrawImage(pDC,(nWidth-m_ViewBackPng.GetWidth())/2,nHeight/2-m_ViewBackPng.GetHeight()/2,\
		m_ViewBackPng.GetWidth() ,m_ViewBackPng.GetHeight(),0,0,m_ViewBackPng.GetWidth() ,m_ViewBackPng.GetHeight());

	if(m_blRungingCar)
	{
		int index=m_CarIndex%2==0?0:1;
		m_idb_selPng.DrawImage(pDC ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,\
			m_idb_selPng.GetWidth()/2 ,m_idb_selPng.GetHeight(),(index)*(m_idb_selPng.GetWidth()/2),0,m_idb_selPng.GetWidth()/2 ,m_idb_selPng.GetHeight());

		int AnimalIndex=GetAnimalInfo(m_CarIndex+1);
		m_pngAnimalLight.DrawImage(pDC ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,\
			m_pngAnimalLight.GetWidth()/9 ,m_pngAnimalLight.GetHeight(),(AnimalIndex)*(m_pngAnimalLight.GetWidth()/9),0,m_pngAnimalLight.GetWidth()/9 ,m_pngAnimalLight.GetHeight());
	}
	if(m_bFlashrandShow)
	{
		int index=m_CarIndex%2==0?0:1;
		m_idb_selPng.DrawImage(pDC ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,\
			m_idb_selPng.GetWidth()/2 ,m_idb_selPng.GetHeight(),(index)*(m_idb_selPng.GetWidth()/2),0,m_idb_selPng.GetWidth()/2 ,m_idb_selPng.GetHeight());

		int AnimalIndex=GetAnimalInfo(m_CarIndex+1);
		m_pngAnimalLight.DrawImage(pDC ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,\
			m_pngAnimalLight.GetWidth()/9 ,m_pngAnimalLight.GetHeight(),(AnimalIndex)*(m_pngAnimalLight.GetWidth()/9),0,m_pngAnimalLight.GetWidth()/9 ,m_pngAnimalLight.GetHeight());
	}

	//��ʾ��ͨ
	if (m_bShowCartoon)
	{
		int nAnimalIndex=GetCartoonIndex(m_cbTableCardArray[0]);
		int XLength = m_pngCarton[nAnimalIndex].GetWidth()/15;
		int YHeight=m_pngCarton[nAnimalIndex].GetHeight();
		m_pngCarton[nAnimalIndex].DrawImage(pDC, nWidth/2-106,nHeight/2-60,XLength,YHeight,(m_cbAnimalIndex%15)*XLength,0,XLength,YHeight);
	}
	
	//��ȡ״̬
	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();

	//ʱ����ʾ
	if(m_DrawBack)
	{
		int nTimeFlagWidth = m_ImageTimeFlagPng.GetWidth()/3;
		int nFlagIndex=0;
		if (cbGameStatus==GAME_STATUS_FREE) nFlagIndex=0;
		else if (cbGameStatus==GS_PLACE_JETTON) nFlagIndex=1;
		else if (cbGameStatus==GS_GAME_END) nFlagIndex=2;

		int xBegin = nWidth/2-70-81;
		int yBegin = (nHeight-705)/2+160+397;
		m_ImageTimeFlagPng.DrawImage(pDC,xBegin-10, yBegin-6, nTimeFlagWidth, m_ImageTimeFlagPng.GetHeight(),
			nFlagIndex*nTimeFlagWidth,0);
		WORD wUserTimer =0;
		if (m_wMeChairID!=INVALID_CHAIR)
		{
			wUserTimer = GetUserClock(m_wMeChairID);
		}
		if (wUserTimer>=0)
		{	m_pngTimeBack.DrawImage(pDC,xBegin+7,yBegin-55);
			DrawNumberString(pDC,wUserTimer,xBegin+28,yBegin-22,true);
		}
	}
	CRect rcDispatchCardTips(m_LifeWidth+612, m_TopHeight+297,m_LifeWidth+812, m_TopHeight+412);

	//�滭��ע
	for ( int i = 0; i < AREA_COUNT; ++i)
	{
		//if (m_lUserJettonScore[i+1]==0) continue;
		
		//�滭����
		pDC->SetTextColor(RGB(255,255,255));

		CString strJettonNumber;
		strJettonNumber.Format(TEXT("%I64d"), m_lUserJettonScore[i+1]);
		

		CString strJettonAll;
		strJettonAll.Format(TEXT("%I64d"), m_lAllJettonScore[i+1]);

		int j=i;
		if (i==0) j=ID_YAN_ZI-1;
		else if (i==1) j=ID_GE_ZI-1;
		else if (i==2) j=ID_KONG_QUE-1;
		else if (i==3) j=ID_LAO_YING-1;
		else if (i==4) j=ID_SHI_ZI-1;
		else if (i==5) j=ID_HOU_ZI-1;
		else if (i==6) j=ID_XIONG_MAO-1;
		else if (i==7) j=ID_TU_ZI-1;
		else if (i==8) j=ID_FEI_QIN-1;
		else if (i==9) j=ID_SHA_YU-1;
		else if (i==10) j=ID_ZOU_SHOU-1;

		if (i==ID_YAN_ZI-1) j=0;
		else if (i==ID_GE_ZI-1) j=1;
		else if (i==ID_KONG_QUE-1) j=2;
		else if (i==ID_LAO_YING-1) j=3;
		else if (i==ID_SHI_ZI-1) j=4;
		else if (i==ID_HOU_ZI-1) j=5;
		else if (i==ID_XIONG_MAO-1) j=6;
		else if (i==ID_TU_ZI-1) j=7;
		else if (i==ID_FEI_QIN-1) j=8;
		else if (i==ID_SHA_YU-1) j=9;
		else if (i==ID_ZOU_SHOU-1) j=10;

		if (m_lUserJettonScore[i+1]!=0)
		pDC->TextOut(m_PointJettonScore[j].x,m_PointJettonScore[j].y-5,strJettonNumber);	

		if(m_lAllJettonScore[i+1]!=0)
		{
			pDC->SetTextColor(RGB(255,0,0));
			pDC->TextOut(m_PointJettonScore[j].x,m_PointJettonScore[j].y+7,strJettonAll);	
		}
		

		

	}
	//ʤ���߿�
	FlashJettonAreaFrame(nWidth,nHeight,pDC);

	//�滭�ٷֱ�
	for (INT i=0;i<AREA_ALL;i++)
	{
		DrawPercentNumberString(pDC,m_nAnimalPercent[i],m_PointAnimalRand[i].x+5,m_PointAnimalRand[i].y);
	}
	//�滭ׯ��
	if(m_DrawBack)
	{
		//�滭�û�
		DrawMeInfo(pDC,nWidth,nHeight);
	}

	//�ҵ���ע
	DrawMeJettonNumber(pDC);
	//ʤ����־
	DrawWinFlags(pDC);

	int startResultX=nWidth/2-100-42;
	int startResultY=nHeight/2-374+146;

	//�滭���
	if (m_bShowShaYuResult)
	{
		DrawShaYuResult(pDC, startResultX, startResultY);
		if (m_bShowAnotherResult)
		{
			startResultY += m_pngResultFShayu.GetHeight(); 
			DrawAnotherResult(pDC, startResultX+20, startResultY);
		}

	}
	else
	{
		DrawAnotherResult(pDC, startResultX+20, startResultY);
	}
	return;
}

//��ȡ����
BYTE CGameClientView::GetAnimalInfo(BYTE cbAllIndex)
{
	static BYTE AnimalInfo[ANIMAL_COUNT]={4,4,4,1,8,8,8,7,7,7,1,6,6,6,5,5,5,1,3,3,3,2,2,2,1,0,0,0};

	if(cbAllIndex>=1&&cbAllIndex<=28)
	{
		return AnimalInfo[cbAllIndex-1];
	}
	
	
	////����
	//if(1==cbAllIndex||cbAllIndex==2||cbAllIndex==3)
	//{
	//	return 4;
	//}
	////����
	//else if(5==cbAllIndex||cbAllIndex==6||cbAllIndex==7)
	//{
	//	return 8;

	//}
	////����
	//else if(8==cbAllIndex||cbAllIndex==9||cbAllIndex==10)
	//{
	//	return 7;
	//}
	////��ȸ
	//else if(12==cbAllIndex||cbAllIndex==13||cbAllIndex==14)
	//{
	//	return 6;
	//}
	////��ӥ
	//else if(15==cbAllIndex||cbAllIndex==16||cbAllIndex==17)
	//{
	//	return 5;
	//}
	////ʨ��
	//else if(19==cbAllIndex||cbAllIndex==20||cbAllIndex==21)
	//{
	//	return 3;
	//}
	////��è
	//else if(22==cbAllIndex||cbAllIndex==23||cbAllIndex==24)
	//{
	//	return 2;
	//}
	////����
	//else if(26==cbAllIndex||cbAllIndex==27||cbAllIndex==28)
	//{
	//	return 0;
	//}
	////����
	//else
	//{
	//	return 1;
	//}
	return 0;
}


//��ȡ����
BYTE CGameClientView::GetCartoonIndex(BYTE cbAllIndex)
{
	
	static BYTE CartoonIndex[ANIMAL_COUNT]={0,0,0,8,1,1,1,2,2,2,8,3,3,3,4,4,4,8,5,5,5,6,6,6,8,7,7,7};

	if(cbAllIndex>=1&&cbAllIndex<=28)
	{
		return CartoonIndex[cbAllIndex-1];
	}
	
	
	////����
	//if(1==cbAllIndex||cbAllIndex==2||cbAllIndex==3)
	//{
	//	return 0;
	//}
	////����
	//else if(5==cbAllIndex||cbAllIndex==6||cbAllIndex==7)
	//{
	//	return 1;

	//}
	////����
	//else if(8==cbAllIndex||cbAllIndex==9||cbAllIndex==10)
	//{
	//	return 2;
	//}
	////��ȸ
	//else if(12==cbAllIndex||cbAllIndex==13||cbAllIndex==14)
	//{
	//	return 3;
	//}
	////��ӥ
	//else if(15==cbAllIndex||cbAllIndex==16||cbAllIndex==17)
	//{
	//	return 4;
	//}
	////ʨ��
	//else if(19==cbAllIndex||cbAllIndex==20||cbAllIndex==21)
	//{
	//	return 5;
	//}
	////��è
	//else if(22==cbAllIndex||cbAllIndex==23||cbAllIndex==24)
	//{
	//	return 6;
	//}
	////����
	//else if(26==cbAllIndex||cbAllIndex==27||cbAllIndex==28)
	//{
	//	return 7;
	//}
	////����
	//else
	//{
	//	return 8;
	//}
	return 0;
}

//��ȡ����
BYTE CGameClientView::GetAnimalMulti(BYTE cbAllIndex)
{
	static BYTE AnimalMulti[ANIMAL_COUNT]={12,12,12,24,8,8,8,6,6,6,24,6,6,6,12,12,12,24,8,8,8,6,6,6,24,6,6,6};

	if(cbAllIndex>=1&&cbAllIndex<=28)
	{
		return AnimalMulti[cbAllIndex-1];
	}
	
	
	////����
	//if(1==cbAllIndex||cbAllIndex==2||cbAllIndex==3)
	//{
	//	return 12;
	//}
	////����
	//else if(5==cbAllIndex||cbAllIndex==6||cbAllIndex==7)
	//{
	//	return 8;

	//}
	////����
	//else if(8==cbAllIndex||cbAllIndex==9||cbAllIndex==10)
	//{
	//	return 6;
	//}
	////��ȸ
	//else if(12==cbAllIndex||cbAllIndex==13||cbAllIndex==14)
	//{
	//	return 6;
	//}
	////��ӥ
	//else if(15==cbAllIndex||cbAllIndex==16||cbAllIndex==17)
	//{
	//	return 12;
	//}
	////ʨ��
	//else if(19==cbAllIndex||cbAllIndex==20||cbAllIndex==21)
	//{
	//	return 8;
	//}
	////��è
	//else if(22==cbAllIndex||cbAllIndex==23||cbAllIndex==24)
	//{
	//	return 6;
	//}
	////����
	//else if(26==cbAllIndex||cbAllIndex==27||cbAllIndex==28)
	//{
	//	return 6;
	//}
	////����
	//else
	//{
	//	return 24;
	//}
	return 0;
}
//������Ϣ
void CGameClientView::SetMeMaxScore(LONGLONG lMeMaxScore)
{
	if (m_lMeMaxScore!=lMeMaxScore)
	{
		//���ñ���
		m_lMeMaxScore=lMeMaxScore;
	}

#ifdef _DEBUG
	m_lMeMaxScore = 333330;
#endif

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
//��ʷ��¼
void CGameClientView::SetGameHistory(BYTE *bcResulte)
{
	//��������
	BYTE bcResulteTmp[AREA_COUNT];
	memcpy(bcResulteTmp,bcResulte,AREA_COUNT);
	tagClientGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];


	for (int i = 1;i<=AREA_COUNT;i++)
	{

		if(bcResulteTmp[i-1]>0)
		{
			GameRecord.enOperateMen[i]=enOperateResult_Win;

		}else
		{
			GameRecord.enOperateMen[i]=enOperateResult_Lost;

		}
	}
	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast+1) % MAX_SCORE_HISTORY;
	if ( m_nRecordLast == m_nRecordFirst )
	{
		m_nRecordFirst = (m_nRecordFirst+1) % MAX_SCORE_HISTORY;
		if ( m_nScoreHead < m_nRecordFirst ) m_nScoreHead = m_nRecordFirst;
	}

	int nHistoryCount = (m_nRecordLast - m_nRecordFirst + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( MAX_FALG_COUNT < nHistoryCount ) m_btScoreMoveR.EnableWindow(TRUE);

	//�Ƶ����¼�¼
	if ( MAX_FALG_COUNT < nHistoryCount )
	{
		m_nScoreHead = (m_nRecordLast - MAX_FALG_COUNT + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
		m_btScoreMoveL.EnableWindow(TRUE);
		m_btScoreMoveR.EnableWindow(FALSE);
	}

	return;
}

//�������
void CGameClientView::CleanUserJetton()
{
	
	//��ע��Ϣ
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//ȫ����ע
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	m_bShowShaYuResult = false;
	m_bShowAnotherResult = false;
	m_bShowCartoon = false;

	m_cbShaYuAddMulti = 0;

	KillTimer(IDI_SHOW_ANIMAL);

	KillTimer(IDI_FLASH_NUMBER1);
	KillTimer(IDI_END_FLASH_NUMBER1);

	KillTimer(IDI_FLASH_NUMBER2);
	KillTimer(IDI_END_FLASH_NUMBER2);

	m_ShaYuIndex=0;

	ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));

	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));

	m_cbOneDigit = 0;						
	m_cbTwoDigit = 0;						
	m_cbRandOneDigit = 0;					
	m_cbRandTwoDigit = 0;	

	//���½���
	RefreshGameView();

	return;
}

//������ע
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=AREA_COUNT);
	if (cbViewIndex>AREA_COUNT) return;

	m_lUserJettonScore[cbViewIndex]=lJettonCount;

	//���½���
	RefreshGameView();
}

//�����˿�
void CGameClientView::SetCardInfo(BYTE cbTableCardArray[2])
{
	if (cbTableCardArray!=NULL)
	{
		CopyMemory(m_cbTableCardArray,cbTableCardArray,sizeof(m_cbTableCardArray));

		//��ʼ����
		DispatchCard();
	}
	else
	{
		ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));
	}
}

//���ó���
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, LONGLONG lScoreCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=AREA_COUNT);
	if (cbViewIndex>AREA_COUNT) return;

	m_lAllJettonScore[cbViewIndex] += lScoreCount;

	RefreshGameView();

	return;
}

//��������ע
void CGameClientView::AndroidBet(BYTE cbViewIndex, LONGLONG lScoreCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=AREA_COUNT);
	if (cbViewIndex>AREA_COUNT) 
		return;

	if ( lScoreCount <= 0L )
		return;

	tagAndroidBet Androi;
	Androi.cbJettonArea = cbViewIndex;
	Androi.lJettonScore = lScoreCount;
	m_ArrayAndroid.Add(Androi);
	SetTimer(IDI_ANDROID_BET,100,NULL);
	int nHaveCount = 0;
	for ( int i = 0 ; i < m_ArrayAndroid.GetCount(); ++i)
	{
		if(m_ArrayAndroid[i].lJettonScore > 0)
			nHaveCount++;
	}
	UINT nElapse = 0;
	if ( nHaveCount <= 1 )
		nElapse = 260;
	else if ( nHaveCount <= 2 )
		nElapse = 160;
	else
		nElapse = 100;

	SetTimer(IDI_ANDROID_BET+m_ArrayAndroid.GetCount(),nElapse,NULL);
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
void CGameClientView::SetWinnerSide(bool blWin[], bool bSet)
{	
	//����ʱ��
	for (int i= 0;i<AREA_COUNT;i++){

		m_bWinFlag[i]=blWin[i];
	}
	if (true==bSet)
	{
		//���ö�ʱ��
		SetTimer(IDI_FLASH_WINNER,500,NULL);

		//ȫʤ�ж�
		bool blWinAll = true;

		for (int i= 0;i<AREA_COUNT;i++){

			if(m_bWinFlag[i]==true){
				blWinAll = false;
			}
		}
		
	}
	else 
	{
		//�����ʱ��
		KillTimer(IDI_FLASH_WINNER);

		//ȫʤ�ж�
		bool blWinAll = true;

		for (int i= 0;i<AREA_COUNT;i++){

			if(m_bWinFlag[i]==true){
				blWinAll = false;
			}
		}
		if (blWinAll){
		}
	}

	//���ñ���
	m_bFlashResult=bSet;
	m_bShowGameResult=bSet;
	m_cbAreaFlash=0xFF;

	//���½���
	RefreshGameView();

	return;
}

//��ȡ����
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	for (int i = 0;i<AREA_COUNT;i++)
	{
		if(m_RectArea[i].PtInRect(MousePoint))
		{
			return i+1;
		}
	}
	return 0xFF;
}
//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos, bool blTimer,bool bMeScore)
{
	//������Դ
	CSize SizeScoreNumber(m_ImageScoreNumber.GetWidth()/11,m_ImageScoreNumber.GetHeight());

	if ( bMeScore ) 
		SizeScoreNumber.SetSize(m_ImageMeScoreNumber.GetWidth()/11,m_ImageMeScoreNumber.GetHeight());

	if(blTimer)
	{
		SizeScoreNumber.SetSize(m_pngTimeNumber.GetWidth()/10, m_pngTimeNumber.GetHeight());
		
	}

	//������Ŀ
	LONGLONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//λ�ö���
	INT nOffset=8;
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=(INT)(nXPos+lNumberCount*SizeScoreNumber.cx/2+lNumberCount/4*(SizeScoreNumber.cx-nOffset)/2-SizeScoreNumber.cx);

	
	//�滭����
	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		if(i!=0&&i%3==0)
		{
			if(!blTimer)
			{
				if ( bMeScore )
				{
					m_ImageMeScoreNumber.TransDrawImage(pDC,nXDrawPos+nOffset,nYDrawPos,SizeScoreNumber.cx-nOffset,SizeScoreNumber.cy,
													10*SizeScoreNumber.cx+nOffset/2,0,RGB(255,0,255));
				}
				else
				{
					m_ImageScoreNumber.TransDrawImage(pDC,nXDrawPos+nOffset,nYDrawPos,SizeScoreNumber.cx-nOffset,SizeScoreNumber.cy,
													10*SizeScoreNumber.cx+nOffset/2,0,RGB(255,0,255));
				}
				nXDrawPos-=(SizeScoreNumber.cx-nOffset);
			}
		}
		LONG lCellNumber=(LONG)(lNumber%10);
		if(blTimer)
		{
			m_pngTimeNumber.DrawImage(pDC,nXDrawPos,nYDrawPos-2,SizeScoreNumber.cx,SizeScoreNumber.cy,
				lCellNumber*SizeScoreNumber.cx,0);
		}
		else
		{
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

		}
		//���ñ���
		lNumber/=10;
		nXDrawPos-=SizeScoreNumber.cx;
	};
	return;
}

//�滭����
void CGameClientView::DrawNumStrWithSpace(CDC * pDC, LONGLONG lNumber,CRect&rcPrint,INT nFormat)
{
	LONGLONG lTmpNumber = lNumber;
	CString strNumber;
	CString strTmpNumber1;
	CString strTmpNumber2;
	bool blfirst = true;
	bool bLongNum = false;
	int nNumberCount = 0;

	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();

	if ( lNumber == 0 )
		strNumber=TEXT("0");

	if ( lNumber < 0 ) 
		lNumber =- lNumber;

	if( lNumber >= 100 )
		bLongNum = true;

	while(lNumber > 0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if ( nNumberCount == 3 )
		{
			if(blfirst)
			{
				strTmpNumber2 += (TEXT("") +strNumber);
				blfirst = false;
			}
			else
			{
				strTmpNumber2 += (TEXT(",") +strNumber);
			}

			strNumber = strTmpNumber2;
			nNumberCount = 0;
			strTmpNumber2 = TEXT("");
		}
		lNumber /= 10;
	}

	if ( strTmpNumber2.IsEmpty() == FALSE )
	{
		if( bLongNum )
			strTmpNumber2 += (TEXT(",") +strNumber);
		else
			strTmpNumber2 += strNumber;

		strNumber = strTmpNumber2;
	}

	if ( lTmpNumber < 0 ) 
		strNumber = TEXT("-") + strNumber;
	//�������
	if (nFormat==-1) pDC->DrawText(strNumber,rcPrint,DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_SINGLELINE);
	else pDC->DrawText(strNumber,rcPrint,nFormat);
}

//�滭����
void CGameClientView::DrawNumStrWithSpace(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
{
	LONGLONG lTmpNumber = lNumber;
	CString strNumber;
	CString strTmpNumber1;
	CString strTmpNumber2;
	bool blfirst = true;
	bool bLongNum = false;
	int nNumberCount = 0;

	strTmpNumber1.Empty();
	strTmpNumber2.Empty();
	strNumber.Empty();

	if ( lNumber == 0 )
		strNumber=TEXT("0");

	if ( lNumber < 0 ) 
		lNumber =- lNumber;

	if( lNumber >= 100 )
		bLongNum = true;

	while(lNumber > 0)
	{
		strTmpNumber1.Format(TEXT("%I64d"),lNumber%10);
		nNumberCount++;
		strTmpNumber2 = strTmpNumber1+strTmpNumber2;

		if ( nNumberCount == 3 )
		{
			if(blfirst)
			{
				strTmpNumber2 += (TEXT("") +strNumber);
				blfirst = false;
			}
			else
			{
				strTmpNumber2 += (TEXT(",") +strNumber);
			}

			strNumber = strTmpNumber2;
			nNumberCount = 0;
			strTmpNumber2 = TEXT("");
		}
		lNumber /= 10;
	}

	if ( strTmpNumber2.IsEmpty() == FALSE )
	{
		if( bLongNum )
			strTmpNumber2 += (TEXT(",") +strNumber);
		else
			strTmpNumber2 += strNumber;

		strNumber = strTmpNumber2;
	}

	if ( lTmpNumber < 0 ) 
		strNumber = TEXT("-") + strNumber;
	//�������
	pDC->TextOut(nXPos,nYPos,strNumber);
}

//�滭����
void CGameClientView::DrawSymbolNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos,int nHalf)
{
	//������Դ
	CSize SizeScoreNumber(m_pngNumberASymbol.GetWidth()/15,m_pngNumberASymbol.GetHeight());

	//������Ŀ
	LONGLONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//λ�ö���
	INT nOffset=8;
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=(INT)(nXPos)+ lNumberCount*(SizeScoreNumber.cx-7) ;

	//�滭����
	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		
		LONG lCellNumber=(LONG)(lNumber%10);
		{
			int yIndex=0,yLength=SizeScoreNumber.cy;
			if (nHalf==1) 
			{
				yIndex = m_pngNumberASymbol.GetHeight()/2;
				yLength=yLength/2;
			}
			else if (nHalf==2) 
			{
				yLength=yLength/2;
			}
			m_pngNumberASymbol.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,yLength,
				lCellNumber*SizeScoreNumber.cx,yIndex,SizeScoreNumber.cx,yLength);
		}
		
		//���ñ���
		lNumber/=10;
		nXDrawPos-=(SizeScoreNumber.cx-7);
	};
	return;
}


//�滭����
void CGameClientView::DrawPercentNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
{
	//������Դ
	CSize SizeScoreNumber(m_pngNumberASymbol.GetWidth()/15,m_pngNumberASymbol.GetHeight());

	//������Ŀ
	LONGLONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//λ�ö���
	INT nOffset=8;
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=(INT)(nXPos)+ lNumberCount*(SizeScoreNumber.cx-9) ;

	if (lNumber<10) nXDrawPos+=15;

	INT nPercentPos = nXDrawPos+SizeScoreNumber.cx;

	
	//�滭����
	for (LONGLONG i=0;i<lNumberCount;i++)
	{

		LONG lCellNumber=(LONG)(lNumber%10);
		{
			int yIndex=0;
			m_pngNumberASymbol.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy-yIndex,
				lCellNumber*SizeScoreNumber.cx,yIndex,SizeScoreNumber.cx,SizeScoreNumber.cy-yIndex);
		}

		//���ñ���
		lNumber/=10;
		nXDrawPos-=(SizeScoreNumber.cx-9);
	};

	int SystomIndex=13;
	int nSybomWidth=m_pngNumberASymbol.GetWidth()/15;
	m_pngNumberASymbol.DrawImage(pDC,nPercentPos-5,nYDrawPos,nSybomWidth,m_pngNumberASymbol.GetHeight(),
		SystomIndex*nSybomWidth,0,nSybomWidth,m_pngNumberASymbol.GetHeight());


	return;
}


//ͼƬ����
int CGameClientView::DrawPicNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos,enXCollocateMode xMode)
{
	//������Դ
	CSize SizeScoreNumber(m_pngNumberASymbol.GetWidth()/15,m_pngNumberASymbol.GetHeight());

	//������Ŀ
	LONGLONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber<0?-lNumber:lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//λ�ö���
	INT Distance=-7;
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=nXPos;
	INT nAllLenth=lNumberCount*SizeScoreNumber.cx+(lNumberCount-1)*Distance;

	if(lNumber<0) 
		nAllLenth+=(SizeScoreNumber.cx+Distance);

	//����λ��
	switch (xMode)
	{
	case enXLeft:	
		{
			nXDrawPos=(INT)(nXPos)+ nAllLenth ;
			break; 
		}
	case enXCenter: 
		{
			nXDrawPos=(INT)(nXPos)+ nAllLenth/2 ;
			break; 
		}
	case enXRight:	
		{
			nXDrawPos=(INT)(nXPos) ;
			break; 
		}
	}

	lNumberTemp=lNumber<0?-lNumber:lNumber;

	for (LONGLONG i=0;i<lNumberCount;i++)
	{

		LONG lCellNumber=(LONG)(lNumberTemp%10);
		{
			int yIndex=0,yLength=SizeScoreNumber.cy;
			
			m_pngNumberASymbol.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,yLength,
				lCellNumber*SizeScoreNumber.cx,yIndex,SizeScoreNumber.cx,yLength);
		}

		//���ñ���
		lNumberTemp/=10;
		nXDrawPos-=(SizeScoreNumber.cx+Distance);
	};

	if(lNumber<0)
	{
		//����
		int SystomIndex=14;

		m_pngNumberASymbol.DrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
			SystomIndex*SizeScoreNumber.cx,0,SizeScoreNumber.cx,SizeScoreNumber.cy);
	}
	


	return nXDrawPos;
}


void CGameClientView::KillCardTime()
{
	KillTimer(IDI_FLASH_WINNER);

}
void CGameClientView::StartRunCar(int iTimer)
{
	m_bFlashrandShow = false;
	KillTimer(IDI_FLASH_RAND_SIDE);
	KillTimer(IDI_FLASH_CARD);
	KillTimer(IDI_END_FLASH_CARD);
	m_nTimerStep = 400;
	SetTimer(IDI_FLASH_CARD,iTimer,NULL);
	m_nCarOpenSide = m_cbTableCardArray[0];
	m_nTotoalRun = m_nCarOpenSide+ANIMAL_COUNT*3-2;
	m_nCurRunIndex = 0;
	m_CarIndex = 1;
	m_blRungingCar = true;
	SetTimer(IDI_END_FLASH_CARD,12*1000,NULL);

}

void CGameClientView::StartRunAnotherCar(int iTimer)
{
	m_bFlashrandShow = false;
	KillTimer(IDI_FLASH_RAND_SIDE);
	KillTimer(IDI_FLASH_ANOTHER_CARD);
	KillTimer(IDI_SHOW_ANOTHER_CARD_RESULT);
	m_nTimerStep = 400;
	SetTimer(IDI_FLASH_ANOTHER_CARD,iTimer,NULL);
	m_nCarOpenSide = m_cbTableCardArray[1];
	m_nTotoalRun = (ANIMAL_COUNT-m_cbTableCardArray[0]+2)+m_nCarOpenSide+ANIMAL_COUNT*3-2;
	m_nCurRunIndex = 0;
	//m_CarIndex = m_cbTableCardArray[0];
	m_blRungingCar = true;
	SetTimer(IDI_SHOW_ANOTHER_CARD_RESULT,12*1000,NULL);
	
}

void CGameClientView::StartRunNumber1(int iTimer)
{
	m_nTimerStep = 300;
	SetTimer(IDI_FLASH_NUMBER1,iTimer,NULL);
	m_nNumberOpenSide = m_cbTwoDigit;
	m_nTotoalRun = m_nNumberOpenSide+10*2-2;
	m_nCurRunIndex1 = 0;
	m_NumberIndex1 = 1;
	m_blRungingNumber1 = true;
	SetTimer(IDI_END_FLASH_NUMBER1,12*1000,NULL);
}

void CGameClientView::StartRunNumber2(int iTimer)
{
	m_nTimerStep = 300;
	SetTimer(IDI_FLASH_NUMBER2,iTimer,NULL);
	m_nNumberOpenSide = m_cbOneDigit;
	m_nTotoalRun = m_nNumberOpenSide+10*2-2;
	m_nCurRunIndex2 = 0;
	m_NumberIndex2 = 1;
	m_blRungingNumber2 = true;
	SetTimer(IDI_END_FLASH_NUMBER2,12*1000,NULL);
}

void CGameClientView::RuningCar(int iTimer)
{
	if(m_nCurRunIndex<10)
	{
		m_nTimerStep-=43;

	}
	if(m_nCurRunIndex >= m_nTotoalRun-15)
	{
		m_nTimerStep+=47 /*8*(m_nCurRunIndex-m_nTotoalRun+15)*/;
	}
	if(m_nCurRunIndex==m_nTotoalRun)
	{
			KillTimer(IDI_FLASH_CARD);
			KillTimer(IDI_END_FLASH_CARD);
			
			//��ʾ����
			if (m_cbShaYuAddMulti!=0)
			{
				
				m_cbAnimalIndex=0;
				m_bShowCartoon=true;
				SetTimer(IDI_SHOW_ANIMAL,100,NULL);
				
				m_cbRandAddMulti=0;
				//SetTimer(IDI_FLASH_ADD_MULTI,100,NULL);
				//SetTimer(IDI_END_ADD_MULTI,1500,NULL);

				StartRunNumber1(10);
				m_bShowShaYuResult = true;
				m_bShowAnotherResult = false;
			}
			else
			{
				
				m_cbAnimalIndex=0;
				m_bShowCartoon=true;
				SetTimer(IDI_SHOW_ANIMAL,100,NULL);
				return;
				
				
			}
			
			return ;

	}
	if(m_nTimerStep<0)
	{
		return ;
	}
	KillTimer(IDI_FLASH_CARD);
	SetTimer(IDI_FLASH_CARD,iTimer,NULL);

}

void CGameClientView::RuningAnotherCar(int iTimer)
{
	if(m_nCurRunIndex<10)
	{
		m_nTimerStep-=43;

	}
	if(m_nCurRunIndex >= m_nTotoalRun-15)
	{
		m_nTimerStep+=47 /*8*(m_nCurRunIndex-m_nTotoalRun+15)*/;
	}
	if(m_nCurRunIndex==m_nTotoalRun)
	{
		KillTimer(IDI_FLASH_ANOTHER_CARD);
		KillTimer(IDI_SHOW_ANOTHER_CARD_RESULT);
		
		{
					
			m_blRungingCar = false;
			//���ö�ʱ��
			m_blMoveFinish = true;
			//m_cbRandAddMulti = 0;
			this->DispatchCard();
			FinishDispatchCard();
			m_bShowShaYuResult = true;
			m_bShowAnotherResult = true;
		}

		return ;

	}
	if(m_nTimerStep<0)
	{
		return ;
	}
	KillTimer(IDI_FLASH_ANOTHER_CARD);
	SetTimer(IDI_FLASH_ANOTHER_CARD,iTimer,NULL);

}




void CGameClientView::RuningNumber1(int iTimer)
{
	if(m_nCurRunIndex1<10)
	{
		m_nTimerStep-=30;

	}
	if(m_nCurRunIndex1 >= m_nTotoalRun-6)
	{
		m_nTimerStep+=37;//47 /*8*(m_nCurRunIndex-m_nTotoalRun+15)*/;
	}
	if(m_nCurRunIndex1==m_nTotoalRun+2)
	{
		KillTimer(IDI_FLASH_NUMBER1);
		KillTimer(IDI_END_FLASH_NUMBER1);
		{

			StartRunNumber2(10);
			return;


		}

		return ;

	}
	if(m_nTimerStep<0)
	{
		return ;
	}
	KillTimer(IDI_FLASH_NUMBER1);
	SetTimer(IDI_FLASH_NUMBER1,iTimer,NULL);

}

void CGameClientView::RuningNumber2(int iTimer)
{
	if(m_nCurRunIndex2<10)
	{
		m_nTimerStep-=30;

	}
	if(m_nCurRunIndex2 >= m_nTotoalRun-6)
	{
		m_nTimerStep+=37 /*8*(m_nCurRunIndex-m_nTotoalRun+15)*/;
	}
	if(m_nCurRunIndex2==m_nTotoalRun+2)
	{
		KillTimer(IDI_FLASH_NUMBER2);
		KillTimer(IDI_END_FLASH_NUMBER2);
		{
			m_cbRandAddMulti=m_cbShaYuAddMulti;
			StartRunAnotherCar(20);
			return;


		}

		return ;

	}
	if(m_nTimerStep<0)
	{
		return ;
	}
	KillTimer(IDI_FLASH_NUMBER2);
	SetTimer(IDI_FLASH_NUMBER2,iTimer,NULL);

}

void CGameClientView::StartRandShowSide()
{
	KillTimer(IDI_FLASH_RAND_SIDE);
	m_nTimerStep = 100;
	m_bFlashrandShow = true;
	SetTimer(IDI_FLASH_RAND_SIDE,m_nTimerStep,NULL);

	if (m_pGameClientDlg->GetGameStatus()==GS_PLACE_JETTON) m_nPlayGameTimes++;
	

   
}
//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//��ʾ����	
	if(IDI_SHOW_ANIMAL == nIDEvent)
	{
		m_cbAnimalIndex++;// = (m_cbAnimalIndex+1)%15;
		if (m_cbAnimalIndex==14)
		{
			//���ö�ʱ��
			if (!m_cbShaYuAddMulti)
			{
				m_blRungingCar = false;
				m_blMoveFinish = true;
				this->DispatchCard();
				FinishDispatchCard();
				m_bShowAnotherResult = true;

			}
			
		}
		//���½���
		RefreshGameView();
		return;
	}
	

	//�ƶ�����1
	if(IDI_FLASH_NUMBER1 == nIDEvent)
	{
		
		m_cbRandTwoDigit = (m_cbRandTwoDigit+1)%10;
		m_nCurRunIndex1++;
		RuningNumber1(m_nTimerStep);
		//SendEngineMessage(IDM_SOUND,3,3);
		//���½���
		RefreshGameView();
	}
	//�����ƶ�1
	if(IDI_END_FLASH_NUMBER1==nIDEvent)
	{
		StartRunNumber2(10);
		KillTimer(IDI_FLASH_NUMBER1);
		KillTimer(IDI_END_FLASH_NUMBER1);
		return;
	}

	//�ƶ�����2
	if(IDI_FLASH_NUMBER2 == nIDEvent)
	{
		m_cbRandOneDigit = (m_cbRandOneDigit+1)%10;
		m_nCurRunIndex2++;
		RuningNumber2(m_nTimerStep);
		//SendEngineMessage(IDM_SOUND,3,3);
		//���½���
		RefreshGameView();
	}
	//�����ƶ�2
	if(IDI_END_FLASH_NUMBER2==nIDEvent)
	{
		RuningAnotherCar(10);
		KillTimer(IDI_FLASH_NUMBER2);
		KillTimer(IDI_END_FLASH_NUMBER2);
			return;
	}
	       
	//�ƶ�
	if(IDI_FLASH_CARD == nIDEvent)
	{
		m_CarIndex = (m_CarIndex+1)%ANIMAL_COUNT;
		m_nCurRunIndex++;
		 RuningCar(m_nTimerStep);
		SendEngineMessage(IDM_SOUND,3,3);

		//���½���
		RefreshGameView();
	}
	//�����ƶ�
	if(IDI_END_FLASH_CARD==nIDEvent)
	{
		for (int i = m_nCurRunIndex;i<m_nTotoalRun;i++)
		{
			m_CarIndex = (m_CarIndex+1)%ANIMAL_COUNT;
			m_nCurRunIndex++;
			if(m_nCurRunIndex==m_nTotoalRun)
			{
				KillTimer(IDI_FLASH_CARD);
				KillTimer(IDI_END_FLASH_CARD);

								
				//��ʾ����
				if (m_cbShaYuAddMulti!=0)
				{
					//m_cbRandAddMulti=0;
					//SetTimer(IDI_FLASH_ADD_MULTI,100,NULL);
					//SetTimer(IDI_END_ADD_MULTI,1500,NULL);
					StartRunNumber1(10);
					m_bShowShaYuResult = true;
					m_bShowAnotherResult = false;

				}
				else
				{
					//m_cbAnimalIndex=0;
					//m_bShowCartoon=true;
					//SetTimer(IDI_SHOW_ANIMAL,100,NULL);
				}
				
				return ;
			}
		}
	}
	//�������(��������ʱ��)
	if(IDI_FLASH_ADD_MULTI==nIDEvent)
	{
		m_cbRandAddMulti++;
		if (m_cbRandAddMulti == m_cbShaYuAddMulti)
		{
			KillTimer(IDI_FLASH_ADD_MULTI);
			m_ShaYuIndex = m_CarIndex;
			StartRunAnotherCar(20);
		}
		//���½���
		RefreshGameView();
		return;

	}
	//�����������(��������ʱ��)
	if(IDI_END_ADD_MULTI==nIDEvent)
	{
		m_cbRandAddMulti = m_cbShaYuAddMulti;
		KillTimer(IDI_FLASH_ADD_MULTI);
		KillTimer(IDI_END_ADD_MULTI);
		m_ShaYuIndex = m_CarIndex;
		StartRunAnotherCar(20);
		//���½���
		RefreshGameView();
		return;

	}

	//�����ƶ�һ��(��������ʱ��)
	if(IDI_FLASH_ANOTHER_CARD == nIDEvent)
	{
		m_CarIndex = (m_CarIndex+1)%ANIMAL_COUNT;
		m_nCurRunIndex++;
		RuningAnotherCar(m_nTimerStep);
		SendEngineMessage(IDM_SOUND,3,3);

		//���½���
		RefreshGameView();
	}
	//�����ƶ�һ�ν���(��������ʱ��)
	if(IDI_SHOW_ANOTHER_CARD_RESULT==nIDEvent)
	{
		for (int i = m_nCurRunIndex;i<m_nTotoalRun;i++)
		{
			m_CarIndex = (m_CarIndex+1)%ANIMAL_COUNT;
			m_nCurRunIndex++;
			if(m_nCurRunIndex==m_nTotoalRun)
			{
				KillTimer(IDI_FLASH_ANOTHER_CARD);
				KillTimer(IDI_SHOW_ANOTHER_CARD_RESULT);

				m_blRungingCar = false;

				{
					m_blMoveFinish = true;
					//���ö�ʱ��
					this->DispatchCard();
					FinishDispatchCard();
					//m_cbRandAddMulti = 0;
				}

				return ;
			}
		}
	}

	if(IDI_FLASH_RAND_SIDE ==nIDEvent )
	{
		m_bFlashrandShow=!m_bFlashrandShow;
		m_CarIndex = rand()%ANIMAL_COUNT;
		//���½���
		RefreshGameView();
	}
	//����ʤ��
	if (nIDEvent==IDI_FLASH_WINNER)
	{
		//���ñ���
		m_bFlashResult=!m_bFlashResult;

		//���½���
		RefreshGameView();
		return;
	}
	//�ֻ�ׯ��
	else if ( nIDEvent == IDI_SHOW_CHANGE_BANKER )
	{
		ShowChangeBanker( false );
		return;
	}
	
	else if ( nIDEvent == IDI_ANDROID_BET )
	{
		//���½���
		RefreshGameView();
		return;
	}
	else if ( nIDEvent >= (UINT)(IDI_ANDROID_BET + 1) && nIDEvent < (UINT)(IDI_ANDROID_BET + m_ArrayAndroid.GetCount() + 1) )
	{
		INT_PTR Index = nIDEvent - IDI_ANDROID_BET - 1;
		if (Index < 0 || Index >= m_ArrayAndroid.GetCount())
		{
			ASSERT(FALSE);
			KillTimer(nIDEvent);
			return;
		}

		if ( m_ArrayAndroid[Index].lJettonScore > 0 )
		{
			LONGLONG lScoreIndex[] = {5000000L,1000000L,100000L,10000L,1000L,100L};
			BYTE cbViewIndex = m_ArrayAndroid[Index].cbJettonArea;

			//���ӳ���
			for (BYTE i=0;i<CountArray(lScoreIndex);i++)
			{
				if(  lScoreIndex[i] > m_lRobotMaxJetton)
					continue;

				if ( m_ArrayAndroid[Index].lJettonScore >= lScoreIndex[i] )
				{
					m_ArrayAndroid[Index].lJettonScore -= lScoreIndex[i];
					m_lAllJettonScore[cbViewIndex] += lScoreIndex[i];

					tagJettonInfo JettonInfo;
					int iWSize = m_RectArea[cbViewIndex-1].right - m_RectArea[cbViewIndex-1].left - 60;
					int iHSize = m_RectArea[cbViewIndex-1].bottom - m_RectArea[cbViewIndex-1].top - 95;
					JettonInfo.nXPos=rand()%(iWSize);
					JettonInfo.nYPos=rand()%(iHSize);
					JettonInfo.cbJettonIndex = JETTON_COUNT - i - 1;

					
					//��������
					AfxGetMainWnd()->PostMessage(IDM_SOUND,7,7);
					break;
				}
			}
		}
		return;
	}

	__super::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);
}
void CGameClientView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags,Point);
}
//���������ؼ���������Ϣ
LRESULT CGameClientView::OnViLBtUp(WPARAM wParam, LPARAM lParam)
{
	CPoint *pPoint = (CPoint*)lParam;

	ScreenToClient(pPoint);
    OnLButtonUp(1,*pPoint);
	return 1;
}

//����ƶ���Ϣ
void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	return __super::OnMouseMove(nFlags,point);
}
//�����Ϣ
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//���ñ���
	//m_lCurrentJetton=0L;

	if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END && m_cbAreaFlash!=0xFF)
	{
		m_cbAreaFlash=0xFF;
		RefreshGameView();
	}
	
	__super::OnLButtonDown(nFlags,Point);
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
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
	RefreshGameView();
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
//ׯ����Ϣ
void CGameClientView::SetBankerInfo(DWORD dwBankerUserID, LONGLONG lBankerScore) 
{
	//ׯ�����Ӻ�
	WORD wBankerUser=INVALID_CHAIR;

	//�������Ӻ�
	if (0!=dwBankerUserID)
	{
		for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
		{
			//tagUserData const *pUserData=GetUserInfo(wChairID);
			IClientUserItem * pClientUserItem=GetClientUserItem(wChairID);
			if (NULL!=pClientUserItem && dwBankerUserID==pClientUserItem->GetUserID())
			{
				wBankerUser=wChairID;
				break;
			}
		}
	}

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
	

	m_pngLuZiL.DrawImage(pDC,m_LuZiStart.x,m_LuZiStart.y);
	for (int i=0;i<MAX_FALG_COUNT-2;i++)
	{
		m_pngLuZiM.DrawImage(pDC,m_LuZiStart.x+m_pngLuZiL.GetWidth()+i*(m_pngLuZiM.GetWidth()),m_LuZiStart.y);
	}
	m_pngLuZiR.DrawImage(pDC,m_LuZiStart.x+m_pngLuZiL.GetWidth()+m_pngLuZiM.GetWidth()*(MAX_FALG_COUNT-2),m_LuZiStart.y);
	
	
	//�ǿ��ж�
	if (m_nRecordLast==m_nRecordFirst) return;
	int nIndex = m_nScoreHead;
	COLORREF static clrOld ;
	clrOld = pDC->SetTextColor(RGB(52,116,23));
	int nDrawCount=0;
	while ( nIndex != m_nRecordLast && ( m_nRecordLast!=m_nRecordFirst ) && nDrawCount < MAX_FALG_COUNT )
	{
		//ʤ����ʶ
		tagClientGameRecord &ClientGameRecord = m_GameRecordArrary[nIndex];
		//λ�ñ���
		int static nYPos=0,nXPos=0;
		nYPos=m_LuZiStart.y+4;
		nXPos=m_LuZiStart.x+((nIndex - m_nScoreHead + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY)*34+22;

		//ʤ����ʶ
		int static nFlagsIndex=0;

		for (int i = 1;i<=AREA_COUNT-2;i++)
		{
			if(ClientGameRecord.enOperateMen[i]==enOperateResult_Win)
			{
				switch(i)
				{
					case ID_TU_ZI:nFlagsIndex=4; break;
					case ID_YAN_ZI:nFlagsIndex=8; break;
					case ID_GE_ZI:nFlagsIndex=7; break;
					case ID_KONG_QUE:nFlagsIndex=6; break;
					case ID_LAO_YING:nFlagsIndex=5; break;
					case ID_SHI_ZI:nFlagsIndex=3; break;
					case ID_XIONG_MAO:nFlagsIndex=2; break;
					case ID_HOU_ZI:nFlagsIndex=0; break;
					case ID_SHA_YU:nFlagsIndex=1; break;
				}
			}
		}
		//�滭��ʶ
		m_pngLuZiAnimal.DrawImage( pDC, nXPos, nYPos, m_pngLuZiAnimal.GetWidth()/9, \
			m_pngLuZiAnimal.GetHeight(),m_pngLuZiAnimal.GetWidth()/9 * nFlagsIndex, 0);
		//�ƶ��±�
		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
		nDrawCount++;
	}
	pDC->SetTextColor(clrOld);
}
//�ƶ���ť
void CGameClientView::OnScoreMoveL()
{
	if ( m_nRecordFirst == m_nScoreHead ) return;

	m_nScoreHead = (m_nScoreHead - 1 + MAX_SCORE_HISTORY) % MAX_SCORE_HISTORY;
	if ( m_nScoreHead == m_nRecordFirst ) m_btScoreMoveL.EnableWindow(FALSE);

	m_btScoreMoveR.EnableWindow(TRUE);

	//���½���
	RefreshGameView();
}

//�ƶ���ť
void CGameClientView::OnScoreMoveR()
{
	int nHistoryCount = ( m_nRecordLast - m_nScoreHead + MAX_SCORE_HISTORY ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount == MAX_FALG_COUNT ) return;

	m_nScoreHead = ( m_nScoreHead + 1 ) % MAX_SCORE_HISTORY;
	if ( nHistoryCount-1 == MAX_FALG_COUNT ) m_btScoreMoveR.EnableWindow(FALSE);

	m_btScoreMoveL.EnableWindow(TRUE);

	//���½���
	RefreshGameView();
}
//��ʾ���
void CGameClientView::DrawShaYuResult(CDC *pDC, int nWidth, int nHeight)
{

	if (false==m_bShowShaYuResult) return;

	int	nXPos = nWidth +5;
	int	nYPos = nHeight;
	int nSybomWidth=m_pngNumberASymbol.GetWidth()/15;
	int indexAnimal = 1;
	//����
	m_pngResultFShayu.DrawImage(pDC,nXPos,nYPos);
	//����
	m_pngResultAnimal.DrawImage(pDC,nXPos+8,nYPos+35,m_pngResultAnimal.GetWidth()/9,m_pngResultAnimal.GetHeight(),
		indexAnimal*m_pngResultAnimal.GetWidth()/9,0,m_pngResultAnimal.GetWidth()/9,m_pngResultAnimal.GetHeight());

	int yDis = nYPos+60;
	//�˺� 
	int SystomIndex=11;
	int xBegin = nXPos+70;
	m_pngNumberASymbol.DrawImage(pDC,xBegin+5,yDis+2,nSybomWidth,m_pngNumberASymbol.GetHeight(),
		SystomIndex*nSybomWidth,0,nSybomWidth,m_pngNumberASymbol.GetHeight());

	//24
	xBegin+=(nSybomWidth+5-55);
	DrawSymbolNumberString(pDC,24,xBegin+33,yDis+16);

	//+
	SystomIndex=10;
	xBegin+=(nSybomWidth*2+51);
	m_pngNumberASymbol.DrawImage(pDC,xBegin-2,yDis,nSybomWidth,m_pngNumberASymbol.GetHeight(),
		SystomIndex*nSybomWidth,0,nSybomWidth,m_pngNumberASymbol.GetHeight());

	xBegin+=(nSybomWidth+5);
	int xLen = 0;
	/*if (m_cbRandAddMulti<10)
	{
		xLen+=7;
	}*/
	//�����
	//DrawSymbolNumberString(pDC,m_cbRandAddMulti+1,xBegin-18+xLen,yDis+8,true);
	
	//�����
	//DrawSymbolNumberString(pDC,m_cbShaYuAddMulti,xBegin-18+xLen,yDis-23);

	//�����
	//DrawSymbolNumberString(pDC,m_cbRandAddMulti-1,xBegin-18+xLen,yDis+46+8,true);


	//�����(��)
	DrawSymbolNumberString(pDC,m_cbRandOneDigit==0?9:(m_cbRandOneDigit-1)%10,xBegin-14+xLen,yDis+9,1);

	//�����(��)
	DrawSymbolNumberString(pDC,m_cbRandTwoDigit==0?9:(m_cbRandTwoDigit-1)%10,xBegin+2+xLen,yDis+9,1);


	//�����(��)
	DrawSymbolNumberString(pDC,m_cbRandOneDigit,xBegin-14+xLen,yDis+22);

	//�����(��)
	DrawSymbolNumberString(pDC,m_cbRandTwoDigit,xBegin+2+xLen,yDis+22);

	//�����(��)
	DrawSymbolNumberString(pDC,(m_cbRandOneDigit+1)%10,xBegin-14+xLen,yDis+53,2);

	//�����(��)
	DrawSymbolNumberString(pDC,(m_cbRandTwoDigit+1)%10,xBegin+2+xLen,yDis+53,2);

	//=
	xBegin+=(nSybomWidth*2+5);
	SystomIndex=12;
	m_pngNumberASymbol.DrawImage(pDC,xBegin-12,yDis,nSybomWidth,m_pngNumberASymbol.GetHeight(),
		SystomIndex*nSybomWidth,0,nSybomWidth,m_pngNumberASymbol.GetHeight());


	if (m_cbRandAddMulti==m_cbShaYuAddMulti)
	{
		//���
		DrawSymbolNumberString(pDC,m_cbRandAddMulti+24,xBegin-10,yDis+16);
	}
	

	
}

//��ʾ���
void CGameClientView::DrawAnotherResult(CDC *pDC, int nWidth, int nHeight)
{

	if (false==m_bShowAnotherResult) return;

	int	nXPos = nWidth +15;
	int	nYPos = nHeight-10;
	int nSybomWidth=m_pngNumberASymbol.GetWidth()/15;
	
	int indexAnimal = 1;
	int CurAnimal = 0;
	if (m_cbShaYuAddMulti!=0)
	{
		CurAnimal = m_cbTableCardArray[1];

	}
	else
	{
		CurAnimal = m_cbTableCardArray[0];

	}
	indexAnimal = GetAnimalInfo(CurAnimal);
	
	//����
	m_pngResultFOther.DrawImage(pDC,nXPos,nYPos);
	//����
	m_pngResultAnimal.DrawImage(pDC,nXPos+18,nYPos+35,m_pngResultAnimal.GetWidth()/9,m_pngResultAnimal.GetHeight(),
		indexAnimal*m_pngResultAnimal.GetWidth()/9,0,m_pngResultAnimal.GetWidth()/9,m_pngResultAnimal.GetHeight());

	int yDis = nYPos+53;
	//�˺� 
	int SystomIndex=11;
	int xBegin = nXPos+100;
	m_pngNumberASymbol.DrawImage(pDC,xBegin,yDis,nSybomWidth,m_pngNumberASymbol.GetHeight(),
		SystomIndex*nSybomWidth,0,nSybomWidth,m_pngNumberASymbol.GetHeight());

	//����
	int nMulti=GetAnimalMulti(CurAnimal);
	xBegin+=(nSybomWidth+5);
	DrawSymbolNumberString(pDC,nMulti,xBegin-25,yDis+16);

	
	int nCoinNumber=DrawPicNumberString(pDC,m_lMeCurGameScore,xBegin-8,yDis+50,enXCenter);

	//�÷�
	m_pngCoinNumber.DrawImage(pDC,nCoinNumber-45,yDis+43,m_pngCoinNumber.GetWidth()/12*2,m_pngCoinNumber.GetHeight(),
		10*m_pngCoinNumber.GetWidth()/12,0,m_pngCoinNumber.GetWidth()/12*2,m_pngCoinNumber.GetHeight());


}



void   CGameClientView::GetAllWinArea(BYTE bcWinArea[],BYTE bcAreaCount,BYTE InArea)
{
	if (InArea==0xFF)
	{
		return ;
	}
	ZeroMemory(bcWinArea,bcAreaCount);


	LONGLONG lMaxSocre = 0;

	for (int i = 0;i<ANIMAL_COUNT;i++)
	{
		BYTE bcOutCadDataWin[AREA_COUNT];
		BYTE bcData[1];
		bcData[0]=i+1;
	    m_GameLogic.GetCardType(bcData[0],1,bcOutCadDataWin);
		for (int j= 0;j<AREA_COUNT;j++)
		{

			if(bcOutCadDataWin[j]>1&&j==InArea-1)
			{
				LONGLONG Score = 0; 
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
				{
					if(bcOutCadDataWin[nAreaIndex-1]>1)
					{
						 Score += m_lAllJettonScore[nAreaIndex]*(bcOutCadDataWin[nAreaIndex-1]);
					}
				}
				if(Score>=lMaxSocre)
				{
					lMaxSocre = Score;
					CopyMemory(bcWinArea,bcOutCadDataWin,bcAreaCount);

				}
				break;
			}
		}
	}
}
//�����ע
LONGLONG CGameClientView::GetUserMaxJetton(BYTE cbJettonArea)
{
	if (cbJettonArea==0xFF)
		return 0;

	//����ע��
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex];
	//ׯ�ҽ��
	LONGLONG lBankerScore = 0x7fffffffffffffff;
	if (m_wBankerUser!=INVALID_CHAIR)
		lBankerScore = m_lBankerScore;

	BYTE bcWinArea[AREA_COUNT];
	LONGLONG LosScore = 0;
	LONGLONG WinScore = 0;

	GetAllWinArea(bcWinArea,AREA_COUNT,cbJettonArea);

	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
	{
		if(bcWinArea[nAreaIndex-1]>1)
		{
			LosScore+=m_lAllJettonScore[nAreaIndex]*(bcWinArea[nAreaIndex-1]);
		}
		else
		{
			if(bcWinArea[nAreaIndex-1]==0)
			{
				WinScore+=m_lAllJettonScore[nAreaIndex];
			}
		}
	}

	LONGLONG lTemp = lBankerScore;
	lBankerScore = lBankerScore + WinScore - LosScore;

	if ( lBankerScore < 0 )
	{
		if (m_wBankerUser!=INVALID_CHAIR)
		{
			lBankerScore = m_lBankerScore;
		}
		else
		{
			lBankerScore = 0x7fffffffffffffff;
		}
	}

	//��������
	LONGLONG lMeMaxScore;

	if((m_lMeMaxScore - lNowJetton)>m_lAreaLimitScore)
	{
		lMeMaxScore= m_lAreaLimitScore;
	}
	else
	{
		lMeMaxScore = m_lMeMaxScore-lNowJetton;
		lMeMaxScore = lMeMaxScore;
	}

	//ׯ������
	lMeMaxScore=min(lMeMaxScore,(lBankerScore)/(bcWinArea[cbJettonArea-1]));

	//��������
	lMeMaxScore = max(lMeMaxScore, 0);

	return lMeMaxScore;
}

//�ɼ�����
void CGameClientView::SetGameScore(LONGLONG lMeCurGameScore, LONGLONG lMeCurGameReturnScore, LONGLONG lBankerCurGameScore)
{
	m_lMeCurGameScore=lMeCurGameScore;
	m_lMeCurGameReturnScore=lMeCurGameReturnScore;
	m_lBankerCurGameScore=lBankerCurGameScore;	
}

void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//�滭����
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
	{
		if (m_lUserJettonScore[nAreaIndex] > 0 )
		{
			//�滭����
			pDC->SetTextColor(RGB(255,255,255));
			

			CString strJettonNumber;
			strJettonNumber.Format(TEXT("%I64d"), m_lUserJettonScore[nAreaIndex]);

			//pDC->TextOut(m_PointAnimalRand[nAreaIndex-1].x,m_PointAnimalRand[nAreaIndex-1].y+25,strJettonNumber);


		}
			//DrawNumberString(pDC,m_lUserJettonScore[nAreaIndex],m_PointAnimalRand[nAreaIndex-1].x,m_PointAnimalRand[nAreaIndex-1].y+25, false,true);
	}
}

//��ʼ����
void CGameClientView::DispatchCard()
{
	
	//���ñ�ʶ
	m_bNeedSetGameRecord=true;
}

//��������
void CGameClientView::FinishDispatchCard( bool bRecord /*= true*/ )
{
	//����ж�
	if (m_bNeedSetGameRecord==false) return;

	//���ñ�ʶ
	m_bNeedSetGameRecord=false;

	//ɾ����ʱ��
	//KillTimer(IDI_DISPATCH_CARD);

	BYTE  bcResulteOut[AREA_COUNT];
	memset(bcResulteOut,0,AREA_COUNT);

	m_GameLogic.GetCardType(m_cbTableCardArray[0],1,bcResulteOut);

	//�����¼
	if (bRecord)
	{
		SetGameHistory(bcResulteOut);
	}

	//�ۼƻ���
	m_lMeStatisticScore+=m_lMeCurGameScore;
	m_lBankerWinScore=m_lTmpBankerWinScore;


	bool blWin[AREA_COUNT];
	for (int i = 0;i<AREA_COUNT;i++){

		if(bcResulteOut[i]>0)
		{
			blWin[i]=true;
		}
		else
		{
			blWin[i]=false;
		}
	}

	//����Ӯ��
	SetWinnerSide(blWin, true);

	//��������
	if (m_lMeCurGameScore>0) 
	{
		SendEngineMessage(IDM_SOUND,5,5);

	}
	else if (m_lMeCurGameScore<0) 
	{
		SendEngineMessage(IDM_SOUND,4,4);

	}
}

//ʤ���߿�
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
	//�Ϸ��ж�
	if(m_bFlashResult&&!m_blRungingCar)
	{
		//m_idb_selPng.DrawImage(pDC ,m_CarRect[m_CarIndex-1].left,m_CarRect[m_CarIndex-1].top,\
		//	m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight(),(m_CarIndex%8)*(m_idb_selPng.GetWidth()/8),0,m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight());


		int index=m_CarIndex%2==0?0:1;
		m_idb_selPng.DrawImage(pDC ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,\
			m_idb_selPng.GetWidth()/2 ,m_idb_selPng.GetHeight(),(index)*(m_idb_selPng.GetWidth()/2),0,m_idb_selPng.GetWidth()/2 ,m_idb_selPng.GetHeight());


		int AnimalIndex=GetAnimalInfo(m_CarIndex+1);
		m_pngAnimalLight.DrawImage(pDC ,m_CarRect[m_CarIndex].left,m_CarRect[m_CarIndex].top,\
			m_pngAnimalLight.GetWidth()/9 ,m_pngAnimalLight.GetHeight(),(AnimalIndex)*(m_pngAnimalLight.GetWidth()/9),0,m_pngAnimalLight.GetWidth()/9 ,m_pngAnimalLight.GetHeight());

		if (m_ShaYuIndex!=0)
		{
			//m_idb_selPng.DrawImage(pDC ,m_CarRect[m_ShaYuIndex-1].left,m_CarRect[m_ShaYuIndex-1].top,\
			//	m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight(),(m_ShaYuIndex%8)*(m_idb_selPng.GetWidth()/8),0,m_idb_selPng.GetWidth()/8 ,m_idb_selPng.GetHeight());


			int index=m_CarIndex%2==0?0:1;
			m_idb_selPng.DrawImage(pDC ,m_CarRect[m_ShaYuIndex].left,m_CarRect[m_ShaYuIndex].top,\
				m_idb_selPng.GetWidth()/2 ,m_idb_selPng.GetHeight(),(index)*(m_idb_selPng.GetWidth()/2),0,m_idb_selPng.GetWidth()/2 ,m_idb_selPng.GetHeight());


			int AnimalIndex=GetAnimalInfo(m_ShaYuIndex+1);
			m_pngAnimalLight.DrawImage(pDC ,m_CarRect[m_ShaYuIndex].left,m_CarRect[m_ShaYuIndex].top,\
				m_pngAnimalLight.GetWidth()/9 ,m_pngAnimalLight.GetHeight(),(AnimalIndex)*(m_pngAnimalLight.GetWidth()/9),0,m_pngAnimalLight.GetWidth()/9 ,m_pngAnimalLight.GetHeight());

		}
		


	}

	//if (m_cbAreaFlash==0xFF && false==m_bFlashResult) return;
	////��ע�ж�
	//if (false==m_bFlashResult)
	//{
	//	if (m_pGameClientDlg->GetGameStatus()==GS_PLACE_JETTON)		
	//		pDC->Draw3dRect(m_RectArea[m_cbAreaFlash-1].left,m_RectArea[m_cbAreaFlash-1].top,m_RectArea[m_cbAreaFlash-1].Width(),m_RectArea[m_cbAreaFlash-1].Height(),RGB(255,255,0),RGB(255,255,0));
	//}
	//else
	//{
	//		for (int i = 0;i<AREA_COUNT;i++)
	//		{
	//			if(m_bWinFlag[i])
	//				pDC->Draw3dRect(m_RectArea[i].left,m_RectArea[i].top,m_RectArea[i].Width(),m_RectArea[i].Height(),RGB(255,255,0),RGB(255,255,0));
	//		}
	//}
}

//�ƶ�Ӯ��
void CGameClientView::DeduceWinner(bool bWinMen[])
{
	BYTE bcData = m_cbTableCardArray[0];
	if(1==bcData||bcData==2||bcData==9||bcData==10||bcData==17||bcData==18||bcData==25||bcData==26)
	{
		if(bcData%2==1)
		 bWinMen[0]= true;
		else
		  bWinMen[1]= true;

	}
	else if(3==bcData||bcData==4||bcData==3+8||bcData==4+8||bcData==3+2*8||bcData==4+2*8||bcData==3+3*8||bcData==4+3*8)
	{
		if(bcData%2==1)
			bWinMen[2]= true;
		else
			bWinMen[3]= true;

		

	}else if(5==bcData||bcData==6||bcData==5+8||bcData==6+8||bcData==5+2*8||bcData==6+2*8||bcData==5+3*8||bcData==6+3*8)
	{
		if(bcData%2==1)
			bWinMen[4]= true;
		else
			bWinMen[5]= true;

	}else if(7==bcData||bcData==8||bcData==7+8||bcData==8+8||bcData==7+2*8||bcData==8+2*8||bcData==7+3*8||bcData==8+3*8)
	{
		if(bcData%2==1)
			bWinMen[6]= true;
		else
			bWinMen[7]= true;

	}
}

//�ؼ�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��ȡID
	WORD wControlID=LOWORD(wParam);

	//�ؼ��ж�
	switch (wControlID)
	{
	case IDC_JETTON_BUTTON_10:
		{
			//���ñ���
			m_lCurrentJetton=10L;

			ResetButtonState();
			m_btJetton10.SetWindowText(TEXT("��"));
			break;
		}
	case IDC_JETTON_BUTTON_100:
		{
			//���ñ���
			m_lCurrentJetton=100L;
			ResetButtonState();
			m_btJetton100.SetWindowText(TEXT("��"));
			break;
		}
	case IDC_JETTON_BUTTON_1000:
		{
			//���ñ���
			m_lCurrentJetton=1000L;
			ResetButtonState();
			m_btJetton1000.SetWindowText(TEXT("��"));
			break;
		}
	case IDC_JETTON_BUTTON_10000:
		{
			//���ñ���
			m_lCurrentJetton=10000L;
			ResetButtonState();
			m_btJetton10000.SetWindowText(TEXT("��"));
			break;
		}
	case IDC_JETTON_BUTTON_100000:
		{
			//���ñ���
			m_lCurrentJetton=100000L;
			ResetButtonState();
			m_btJetton100000.SetWindowText(TEXT("��"));
			break;
		}
	case IDC_AUTO_OPEN_CARD:
		{
			break;
		}
	case IDC_OPEN_CARD:
		{
			break;
		}
	case IDC_BANK:
		{
			break;
		}	
	case IDC_BANK_DRAW:
		{
			OnBankDraw();
			break;
		}
	case IDC_BANK_STORAGE:
		{
			OnBankStorage();
			break;
		}
	case IDC_COMMIT_PLACE_JETTON:
		{
			OnPlaceJetton();
		}

	}

	return CGameFrameView::OnCommand(wParam, lParam);
}

//����״̬
void CGameClientView::ResetButtonState()
{
	m_btJetton10.SetWindowText(TEXT(""));									
	m_btJetton100.SetWindowText(TEXT(""));									
	m_btJetton1000.SetWindowText(TEXT(""));									
	m_btJetton10000.SetWindowText(TEXT(""));								
	m_btJetton100000.SetWindowText(TEXT(""));	
	
}

//�ҵ�λ��
void CGameClientView::SetMeChairID(DWORD dwMeUserID)
{
	//�������Ӻ�
	for (WORD wChairID=0; wChairID<MAX_CHAIR; ++wChairID)
	{
		//tagUserData const *pUserData=GetUserInfo(wChairID);
		IClientUserItem * pClientUserItem=GetClientUserItem(wChairID);
		if (NULL!=pClientUserItem && dwMeUserID==pClientUserItem->GetUserID())
		{
			m_wMeChairID=wChairID;
			break;
		}
	}
}

//�滭ׯ��
void CGameClientView::DrawBankerInfo(CDC *pDC,int nWidth,int nHeight)
{
	CBrush brush;
	brush.CreateSolidBrush(RGB(255,0,0));
	//ׯ����Ϣ																											
	pDC->SetTextColor(RGB(255,255,0));
	//��ȡ���
	

	IClientUserItem * pClientUserItem=m_wBankerUser==INVALID_CHAIR ? NULL :GetClientUserItem(m_wBankerUser);

	CRect  StrRect;
	StrRect.left = m_LifeWidth+89;
	StrRect.top  = m_TopHeight+55;
	StrRect.right = StrRect.left + 90;
	StrRect.bottom = StrRect.top + 15;

	tagUserInfo* pUserInfo=NULL;

	if (pClientUserItem!=NULL)
	{
		pUserInfo =  pClientUserItem->GetUserInfo();
	}
	//ׯ������
	TCHAR Text[256];
	if(m_bEnableSysBanker)
	{
		myprintf(Text,256,_TEXT("%s"),pClientUserItem==NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("ϵͳ��ׯ")):pUserInfo->szNickName);
		pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE );
	}
	else
	{
		myprintf(Text,256,_TEXT("%s"),pClientUserItem==NULL?(m_bEnableSysBanker==false?TEXT("������ׯ"):TEXT("������ׯ")):pUserInfo->szNickName);
		pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE );
	}
	
	StrRect.left = StrRect.left+150;
	StrRect.right = StrRect.left +30;
	myprintf(Text,256,_TEXT("%d"),m_wBankerTime);
	pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_CENTER | DT_TOP|DT_SINGLELINE );

	StrRect.left = StrRect.left+90;
	StrRect.right = StrRect.left +80;
	DrawNumStrWithSpace(pDC,pClientUserItem==NULL?0:pClientUserItem->GetUserScore(),StrRect);

	StrRect.left = StrRect.left+96+45;
	StrRect.right = StrRect.left +85;
	DrawNumStrWithSpace(pDC,m_lBankerWinScore,StrRect);
}

//�滭���
void CGameClientView::DrawMeInfo(CDC *pDC,int nWidth,int nHeight)
{
	if (INVALID_CHAIR==m_wMeChairID)
		return;
	//ׯ����Ϣ																											
	pDC->SetTextColor(RGB(255,255,0));
	//��ȡ���
	//tagUserData const *pUserData =  GetUserInfo(m_wMeChairID);

	IClientUserItem * pClientUserItem=GetClientUserItem(m_wMeChairID);

	CRect static StrRect;
	StrRect.left = m_LifeWidth+109;
	StrRect.top = m_TopHeight+540;
	StrRect.right =StrRect.left+87;
	StrRect.bottom=StrRect.top+20;

	
	pDC->SetTextColor(RGB(118,224,255));
	
	//����
	TCHAR Text[256];
	tagUserInfo* pUserInfo =  pClientUserItem->GetUserInfo();
	myprintf(Text,256,_TEXT("%s"),pUserInfo->szNickName);
	pDC->DrawText(Text, StrRect, DT_END_ELLIPSIS | DT_LEFT | DT_TOP|DT_SINGLELINE );

	pDC->SetTextColor(RGB(180,255,119));

	StrRect.bottom-=4;
	//��Ӯ
	StrRect.top = StrRect.bottom;
	StrRect.right = StrRect.left + 87;
	StrRect.bottom = StrRect.top + 19;
	DrawNumStrWithSpace(pDC,m_lMeStatisticScore,StrRect);
	

	//����
	StrRect.top = StrRect.bottom;
	StrRect.right = StrRect.left + 87;
	StrRect.bottom = StrRect.top + 19;
	DrawNumStrWithSpace(pDC,m_nPlayGameTimes,StrRect);

	//�ܽ��
	StrRect.top = StrRect.bottom;
	StrRect.right = StrRect.left + 87;
	StrRect.bottom = StrRect.top + 19;
	
	DrawNumStrWithSpace(pDC,pClientUserItem->GetUserScore(),StrRect);

	//���ý��
	StrRect.top = StrRect.bottom;
	StrRect.right = StrRect.left + 87;
	StrRect.bottom = StrRect.top + 19;
	LONGLONG lMeJetton=0L;
	for (int nAreaIndex=1; nAreaIndex<AREA_COUNT+1; ++nAreaIndex) 
		lMeJetton += m_lUserJettonScore[nAreaIndex];

	DrawNumStrWithSpace(pDC,pClientUserItem->GetUserScore()-lMeJetton,StrRect);

	
	
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//���д��
void CGameClientView::OnBankStorage()
{
	//��ȡ�ӿ�
	CGameClientEngine *pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientEngine->m_pIClientKernel;

	if(NULL!=pIClientKernel)
	{
		CRect btRect;
		m_btBankStorage.GetWindowRect(&btRect);
		//ShowInsureSave(pIClientKernel,CPoint(btRect.right,btRect.top));
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
		m_btBankDraw.GetWindowRect(&btRect);
		//ShowInsureGet(pIClientKernel,CPoint(btRect.right,btRect.top));
	}
}


void CGameClientView::OnUp()
{
	

}
void CGameClientView::OnDown()
{

}

//ButtonAddScore(ID_YAN_ZI-1);  0
//ButtonAddScore(ID_GE_ZI-1); 1
//ButtonAddScore(ID_KONG_QUE-1); 2
//ButtonAddScore(ID_LAO_YING-1); 3
//ButtonAddScore(ID_SHI_ZI-1); 4
//ButtonAddScore(ID_HOU_ZI-1); 5
//ButtonAddScore(ID_XIONG_MAO-1); 6
//ButtonAddScore(ID_TU_ZI-1); 7
//ButtonAddScore(ID_FEI_QIN-1); 8
//ButtonAddScore(ID_SHA_YU-1); 9
//ButtonAddScore(ID_ZOU_SHOU-1); 10

void CGameClientView::OnAddScore0()
{
	ButtonAddScore(ID_YAN_ZI); 

}

void CGameClientView::OnAddScore1()
{
	ButtonAddScore(ID_GE_ZI); 

}

void CGameClientView::OnAddScore2()
{
	ButtonAddScore(ID_KONG_QUE);


}

void CGameClientView::OnAddScore3()
{
	ButtonAddScore(ID_LAO_YING);
}

void CGameClientView::OnAddScore4()
{
	ButtonAddScore(ID_SHI_ZI); 
}

void CGameClientView::OnAddScore5()
{
	ButtonAddScore(ID_HOU_ZI);
}

void CGameClientView::OnAddScore6()
{
	ButtonAddScore(ID_XIONG_MAO);
}

void CGameClientView::OnAddScore7()
{
	ButtonAddScore(ID_TU_ZI);
}

void CGameClientView::OnAddScore8()
{
	ButtonAddScore(ID_FEI_QIN);
}

void CGameClientView::OnAddScore9()
{
	ButtonAddScore(ID_SHA_YU); 
	
}

void CGameClientView::OnAddScore10()
{
	ButtonAddScore(ID_ZOU_SHOU);
	
	
}

void CGameClientView::ButtonAddScore(BYTE cbJettonArea)
{
	if( cbJettonArea > AREA_COUNT)
	{
		ASSERT(FALSE);
		return;
	}

	if (m_lCurrentJetton==0)
	{
		return;
	}
	//�����ע
	LONGLONG lMaxJettonScore=GetUserMaxJetton(cbJettonArea);

	if((m_lAllJettonScore[cbJettonArea]+m_lCurrentJetton)>m_lAreaLimitScore)
	{		
		return ;
	}

	//�Ϸ��ж�
	if (lMaxJettonScore < m_lCurrentJetton)
	{
		//SetJettonHide(0);
		ReduceJettonNumber();
		return ;
	}

	SendEngineMessage(IDM_PLACE_JETTON,cbJettonArea,(LPARAM)(&m_lCurrentJetton));

}

//���ͳ���
void CGameClientView::ReduceJettonNumber()
{
	if (m_lCurrentJetton==100000L)
	{
		m_lCurrentJetton=10000;
		ResetButtonState();
		m_btJetton10000.SetWindowText(TEXT("��"));
	}
	else if (m_lCurrentJetton==10000L)
	{
		m_lCurrentJetton=1000;
		ResetButtonState();
		m_btJetton1000.SetWindowText(TEXT("��"));
	}
	else if (m_lCurrentJetton==1000L)
	{
		m_lCurrentJetton=100;
		ResetButtonState();
		m_btJetton100.SetWindowText(TEXT("��"));
	}
	else if (m_lCurrentJetton==100L)
	{
		m_lCurrentJetton=10;
		ResetButtonState();
		m_btJetton10.SetWindowText(TEXT("��"));
	}
	RefreshGameView();

}

//�ύ��ע
void CGameClientView::OnPlaceJetton()
{

	return;

	/*LONGLONG lAllScore = 0l;
	for ( int i = 0 ; i < AREA_COUNT; ++i)
	{
		lAllScore += m_lUserAddScore[i];
	}
	if ( lAllScore > m_lMeMaxScore )
	{
		CDialogMessage Message;
		Message.SetMessage(TEXT("�������㣡"));
		Message.DoModal();
		return;
	}*/

	//if ( lAllScore > 0 )
	{
		//MD_C_PlaceJetton PlaceJetton;


		/*PlaceJetton.lBetScore[ID_YAN_ZI-1] = m_lUserAddScore[0];
		PlaceJetton.lBetScore[ID_GE_ZI-1] = m_lUserAddScore[1];
		PlaceJetton.lBetScore[ID_KONG_QUE-1] = m_lUserAddScore[2];
		PlaceJetton.lBetScore[ID_LAO_YING-1] = m_lUserAddScore[3];
		PlaceJetton.lBetScore[ID_SHI_ZI-1] = m_lUserAddScore[4];
		PlaceJetton.lBetScore[ID_HOU_ZI-1] = m_lUserAddScore[5];
		PlaceJetton.lBetScore[ID_XIONG_MAO-1] = m_lUserAddScore[6];
		PlaceJetton.lBetScore[ID_TU_ZI-1] = m_lUserAddScore[7];
		PlaceJetton.lBetScore[ID_FEI_QIN-1] = m_lUserAddScore[8];
		PlaceJetton.lBetScore[ID_SHA_YU-1] = m_lUserAddScore[9];
		PlaceJetton.lBetScore[ID_ZOU_SHOU-1] = m_lUserAddScore[10];*/

		//memcpy(PlaceJetton.lBetScore, m_lUserAddScore, sizeof(m_lUserAddScore));



		//SendEngineMessage(IDM_PLACE_JETTON,0,(LPARAM)(&PlaceJetton));
	}

}

void CGameClientView::StartHandle_Leave()
{
	
	SetTimer(IDI_HANDLELEAVE_ANIMATION,400,NULL);

	SendEngineMessage(IDM_SOUND,0,1);
	

}

//����Ա����
void CGameClientView::OpenAdminWnd()
{
	//��Ȩ��
	//if(m_pAdminControl!=NULL&&(GetUserInfo(m_wMeChairID)-> &UR_GAME_CONTROL)!=0)
	{
		

		if(!m_pAdminControl->IsWindowVisible()) 
			m_pAdminControl->ShowWindow(SW_SHOW);
		else 
			m_pAdminControl->ShowWindow(SW_HIDE);
	}
}

//ִ��ʣ�����еĻ��嶯��
void CGameClientView::PerformAllBetAnimation()
{
	KillTimer(IDI_ANDROID_BET);
	for ( int i = 0 ; i < m_ArrayAndroid.GetCount(); ++i)
	{
		KillTimer(IDI_ANDROID_BET + i + 1);
		PlaceUserJetton(m_ArrayAndroid[i].cbJettonArea, m_ArrayAndroid[i].lJettonScore);
	}
	m_ArrayAndroid.RemoveAll();
}

//������ͼ
void CGameClientView::RefreshGameView()
{
	CRect rect;
	GetClientRect(&rect);
	InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

	return;
}

//���ü���
void CGameClientView::SetAnimalPercent( INT nAnimalPercent[AREA_ALL] )
{
	memcpy(m_nAnimalPercent, nAnimalPercent, sizeof(m_nAnimalPercent));
	int nFirstValue=m_nAnimalPercent[0];
	for (int i=0;i<AREA_ALL-1;i++)
	{
		m_nAnimalPercent[i]=m_nAnimalPercent[i+1];
	}

	m_nAnimalPercent[AREA_ALL-2]=nFirstValue;

}
//�������㽱��
void CGameClientView::SetShaYuAddMulti(BYTE cbShaYuAddMulti)
{
	m_cbShaYuAddMulti = cbShaYuAddMulti; 

	m_cbOneDigit = m_cbShaYuAddMulti/10;						
	m_cbTwoDigit = m_cbShaYuAddMulti%10;						
	m_cbRandOneDigit =	0;					
	m_cbRandTwoDigit =  0;

} 


//��ȡ����
CString CGameClientView::GetAnimalStrInfo(BYTE cbAllIndex)
{
	//����
	if(1==cbAllIndex||cbAllIndex==2||cbAllIndex==3)
	{
		return TEXT("����");
	}
	//����
	else if(5==cbAllIndex||cbAllIndex==6||cbAllIndex==7)
	{
		return TEXT("��");

	}
	//����
	else if(8==cbAllIndex||cbAllIndex==9||cbAllIndex==10)
	{
		return TEXT("����");
	}
	//��ȸ
	else if(12==cbAllIndex||cbAllIndex==13||cbAllIndex==14)
	{
		return TEXT("��ȸ");
	}
	//��ӥ
	else if(15==cbAllIndex||cbAllIndex==16||cbAllIndex==17)
	{
		return TEXT("��ӥ");
	}
	//ʨ��
	else if(19==cbAllIndex||cbAllIndex==20||cbAllIndex==21)
	{
		return TEXT("ʨ��");
	}
	//��è
	else if(22==cbAllIndex||cbAllIndex==23||cbAllIndex==24)
	{
		return TEXT("��è");
	}
	//����
	else if(26==cbAllIndex||cbAllIndex==27||cbAllIndex==28)
	{
		return TEXT("����");
	}
	//����
	else
	{
		return TEXT("����");
	}
	return TEXT("");
}