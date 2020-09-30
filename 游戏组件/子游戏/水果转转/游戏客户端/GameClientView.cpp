#include "StdAfx.h"
#include "Math.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FLASH_WINNER			100									//������ʶ
#define IDI_SHOW_CHANGE_BANKER		101									//�ֻ�ׯ��
#define IDI_DISPATCH_CARD			102									//���Ʊ�ʶ
#define IDI_BOMB_EFFECT				200									//��ը��ʶ

#define IDI_END_CARTOON				105									//����������ʶ


//��ť��ʶ
#define IDC_APPY_BANKER				206									//��ť��ʶ
#define IDC_CANCEL_BANKER			207									//��ť��ʶ
#define IDC_SCORE_MOVE_L			209									//��ť��ʶ
#define IDC_SCORE_MOVE_R			210									//��ť��ʶ
#define IDC_VIEW_CHART				211									//��ť��ʶ
#define IDC_BANKER_UP				212
#define IDC_BANKER_DOWN				213
#define IDC_USER_BANK				214									//���а�ť

#define IDC_ADD_STEP1				215									//��Ͷ��ť
#define IDC_ADD_STEP10				216									//��Ͷ��ť
#define IDC_ADD_STEP100				217									//��Ͷ��ť
#define IDC_ADD_STEP1_H				218									//��Ͷ��ť
#define IDC_ADD_STEP10_H			219									//��Ͷ��ť
#define IDC_ADD_STEP100_H			220									//��Ͷ��ť

#define IDC_BET_LAST				221									//��Ѻ��ť
#define	IDC_BET_SMALL				222									//ѺС��ť
#define IDC_BET_BIG					223									//Ѻ��ť


//��ը��Ŀ
#define BOMB_EFFECT_COUNT			8									//��ը��Ŀ

//��ע�߿��С
#define FRAME_SIZE_WIDTH			71									//���
#define FRAME_SIZE_HIGHT			77									//�߶�
////��ע�߿��С
//#define FRAME_SIZE_WIDTH			128									//���
//#define FRAME_SIZE_HIGHT			138									//�߶�

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_ADD_STEP1_H,OnAddStep1_H)
	ON_BN_CLICKED(IDC_ADD_STEP10_H,OnAddStep10_H)
	ON_BN_CLICKED(IDC_ADD_STEP100_H,OnAddStep100_H)
	ON_BN_CLICKED(IDC_ADD_STEP1,OnAddStep1)
	ON_BN_CLICKED(IDC_ADD_STEP10,OnAddStep10)
	ON_BN_CLICKED(IDC_ADD_STEP100,OnAddStep100)

	ON_BN_CLICKED(IDC_APPY_BANKER, OnApplyBanker)
	ON_BN_CLICKED(IDC_CANCEL_BANKER, OnCancelBanker)
	ON_BN_CLICKED(IDC_SCORE_MOVE_L, OnScoreMoveL)
	ON_BN_CLICKED(IDC_SCORE_MOVE_R, OnScoreMoveR)
	ON_BN_CLICKED(IDC_BANKER_UP, OnBankerListMoveUp)
	ON_BN_CLICKED(IDC_BANKER_DOWN, OnBankerListMoveDown)
	ON_BN_CLICKED(IDC_USER_BANK, OnClickedBank)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��ע��Ϣ
	ZeroMemory(m_iMeAreaScore, sizeof(m_iMeAreaScore));
	
	//ȫ����ע
	ZeroMemory(m_iAllAreaScore, sizeof(m_iAllAreaScore));

	//����ע����ǰ���·�
	ZeroMemory(m_iAreaScoreLimit, sizeof(m_iAreaScoreLimit));

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_iBankerScore=0L;	
	m_iBankerWinScore=0L;

	//���ֳɼ�
	m_iMeCurGameScore=0L;	
	m_iMeCurGameReturnScore=0L;
	m_iBankerCurGameScore=0L;
	m_iGameRevenue=0L;

	//��������
	ZeroMemory(m_bBombEffect,sizeof(m_bBombEffect));
	ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));
	ZeroMemory(m_CartoonTimeElapseArray, sizeof(m_CartoonTimeElapseArray));

	//״̬��Ϣ
	m_iCurrentJetton=1L;
	m_cbWinnerSide=0xFF;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
	

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	m_nCurCartoonIndex = 0;
	m_CartoonElapsIndex = 0;
	m_bCanShowResult = FALSE;
	m_cbGoalAnimal=0xFF;

	//��ʷ�ɼ�
	m_iMeStatisticScore=0;

	//�ؼ�����
	m_pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageViewFill.LoadFromResource(hInstance,IDB_VIEW_FILL);
	m_ImageViewBack.LoadFromResource(hInstance,IDB_VIEW_BACK);
	m_ImageJettonView.LoadFromResource(hInstance,IDB_JETTOM_VIEW);
	m_ImageScoreNumber.LoadFromResource(hInstance,IDB_SCORE_NUMBER);
	m_ImageMeScoreNumber.LoadFromResource(hInstance,IDB_ME_SCORE_NUMBER);
	m_ImageWinFrame.LoadFromResource(hInstance,IDB_WIN_FRAME);


	m_ImageGameEnd.LoadFromResource( hInstance,IDB_GAME_END);

	m_ImageMeBanker.LoadFromResource( hInstance,IDB_ME_BANKER);
	m_ImageChangeBanker.LoadFromResource( hInstance,IDB_CHANGE_BANKER );
	m_ImageNoBanker.LoadFromResource( hInstance,IDB_NO_BANKER);	

	m_ImageTimeFlag.LoadFromResource(hInstance,IDB_TIME_FLAG);

	m_ImageBombEffect.LoadImage(hInstance,TEXT("FIRE_EFFECT"));
	m_ImageHistoryRecord.LoadFromResource(hInstance,IDB_HISTORY_RECORD);

	m_ImageCardCartoon.LoadImage(hInstance, TEXT("CARD_CARTOON"));

	m_ImageApple.LoadImage(hInstance,TEXT("APPLE"));
	m_ImageApple2.LoadImage(hInstance,TEXT("APPLE2"));
	m_IamgeBar.LoadImage(hInstance,TEXT("BAR"));
	m_IamgeBar2.LoadImage(hInstance,TEXT("BAR2"));
	m_IamgeBell.LoadImage(hInstance,TEXT("BELL"));
	m_IamgeBell2.LoadImage(hInstance,TEXT("BELL2"));
	m_IamgeSeven.LoadImage(hInstance,TEXT("D_SEVEN"));
	m_IamgeSeven2.LoadImage(hInstance,TEXT("D_SEVEN2"));
	m_IamgeStar.LoadImage(hInstance,TEXT("D_STAR"));
	m_IamgeStar2.LoadImage(hInstance,TEXT("D_STAR2"));
	m_IamgeOrange.LoadImage(hInstance,TEXT("ORANGE"));
	m_IamgeOrange2.LoadImage(hInstance,TEXT("ORANGE2"));
	m_IamgePaw.LoadImage(hInstance,TEXT("PAWPAW"));
	m_IamgePaw2.LoadImage(hInstance,TEXT("PAWPAW2"));
	m_ImageWaterMelon.LoadImage(hInstance,TEXT("WATERMELON"));
	m_ImageWaterMelon2.LoadImage(hInstance,TEXT("WATERMELON2"));


	m_ImageBetNumber.LoadImage(hInstance,TEXT("BET_NUMBER"));
	m_ImageBetBigNumber.LoadImage(hInstance,TEXT("BET_BIG_NUMBER"));

	m_ImageFreeTime.LoadImage(hInstance,TEXT("FREETIME"));
	m_ImageBitSmallTime.LoadImage(hInstance,TEXT("BITSMALLTIME"));
	m_ImageJettonTime.LoadImage(hInstance,TEXT("JETTONTIME"));

	m_ImageKeyPress.LoadFromResource(hInstance,IDB_BT_KEY_PRESS);


	m_bisLButtonDown = false;

	m_bEndCount = 0;
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

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
		
	//���밴ť
	m_btApplyBanker.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_APPY_BANKER);
	m_btCancelBanker.Create(NULL,WS_CHILD|WS_DISABLED,rcCreate,this,IDC_CANCEL_BANKER);

	//����Ͷע��ť
	m_btAddStep1.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_ADD_STEP1);
	m_btAddStep10.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_ADD_STEP10);
	m_btAddStep100.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_ADD_STEP100);

	m_btAddStep1_H.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_ADD_STEP1_H);
	m_btAddStep10_H.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_ADD_STEP10_H);
	m_btAddStep100_H.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_ADD_STEP100_H);

	m_btScoreMoveL.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_L);
	m_btScoreMoveR.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_SCORE_MOVE_R);

	m_btBankerMoveUp.Create(NULL, WS_CHILD|WS_VISIBLE|WS_DISABLED, rcCreate, this, IDC_BANKER_UP);
	m_btBankerMoveDown.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED, rcCreate, this, IDC_BANKER_DOWN);

	m_btBankButton.Create(NULL, WS_CHILD|WS_VISIBLE, rcCreate, this, IDC_USER_BANK);


	m_btBetLast.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BET_LAST);
	m_btBetSmall.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BET_SMALL);
	m_btBetBig.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rcCreate,this,IDC_BET_BIG);



	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();

	m_btBankButton.SetButtonImage(IDB_BANK, hResInstance, false,false);
	
	m_btApplyBanker.SetButtonImage(IDB_BT_APPLY_BANKER,hResInstance,false,false);
	m_btCancelBanker.SetButtonImage(IDB_BT_CANCEL_APPLY,hResInstance,false,false);

	m_btScoreMoveL.SetButtonImage(IDB_BT_SCORE_MOVE_L,hResInstance,false,false);
	m_btScoreMoveR.SetButtonImage(IDB_BT_SCORE_MOVE_R,hResInstance,false,false);

	m_btBankerMoveUp.SetButtonImage(IDB_BT_BANKER_UP, hResInstance, false,false);
	m_btBankerMoveDown.SetButtonImage(IDB_BT_BANKER_DOWN, hResInstance, false,false);

	m_btAddStep1.SetButtonImage(IDB_BT_ADDSTEP1,hResInstance,false,false);
	m_btAddStep10.SetButtonImage(IDB_BT_ADDSTEP10,hResInstance,false,false);
	m_btAddStep100.SetButtonImage(IDB_BT_ADDSTEP100,hResInstance,false,false);
	m_btAddStep1_H.SetButtonImage(IDB_BT_ADDSTEP1_H,hResInstance,false,false);
	m_btAddStep10_H.SetButtonImage(IDB_BT_ADDSTEP10_H,hResInstance,false,false);
	m_btAddStep100_H.SetButtonImage(IDB_BT_ADDSTEP100_H,hResInstance,false,false);

// 	m_btBetLast.SetButtonImage(IDB_BT_BET_LAST,hResInstance,false);
// 	m_btBetSmall.SetButtonImage(IDB_BT_BET_SMALL,hResInstance,false);
// 	m_btBetBig.SetButtonImage(IDB_BT_BET_BIG,hResInstance,false);

	m_fontScoreLimit.CreateFont(-16,0,0,0,500,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));

	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��ע��Ϣ
	ZeroMemory(m_iMeAreaScore, sizeof(m_iMeAreaScore));
	
	//ȫ����ע
	ZeroMemory(m_iAllAreaScore, sizeof(m_iAllAreaScore));

	//����ע����ǰ���·�
	ZeroMemory(m_iAreaScoreLimit, sizeof(m_iAreaScoreLimit));

	//ׯ����Ϣ
	m_wBankerUser=INVALID_CHAIR;		
	m_wBankerTime=0;
	m_iBankerScore=0L;	
	m_iBankerWinScore=0L;

	//���ֳɼ�
	m_iMeCurGameScore=0L;	
	m_iMeCurGameReturnScore=0L;
	m_iBankerCurGameScore=0L;
	m_iGameRevenue=0L;

	//��������
	ZeroMemory(m_bBombEffect,sizeof(m_bBombEffect));
	ZeroMemory(m_cbBombFrameIndex,sizeof(m_cbBombFrameIndex));
	ZeroMemory(m_CartoonTimeElapseArray, sizeof(m_CartoonTimeElapseArray));

	//״̬��Ϣ
	m_iCurrentJetton=1L;
	m_cbWinnerSide=0xFF;
	m_cbAreaFlash=0xFF;
	m_wMeChairID=INVALID_CHAIR;
	m_bShowChangeBanker=false;
	m_bNeedSetGameRecord=false;
		

	//λ����Ϣ
	m_nScoreHead = 0;
	m_nRecordFirst= 0;	
	m_nRecordLast= 0;	

	//��ʷ�ɼ�
	m_iMeStatisticScore=0;

	m_nCurCartoonIndex = 0;
	m_CartoonElapsIndex = 0;
	m_bCanShowResult = FALSE;
	m_cbGoalAnimal=0xFF;

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


	m_bisLButtonDown = false;

	m_bEndCount = 0;
	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//λ����Ϣ
	//����λ�û�׼��
	int nCenterX=nWidth/2, nCenterY=nHeight/2;
	
	//����ע����
	m_rcJettonArea[0].left = nCenterX - 365;
	m_rcJettonArea[0].top = nCenterY + 260;
	m_rcJettonArea[0].right = m_rcJettonArea[0].left + FRAME_SIZE_WIDTH;
	m_rcJettonArea[0].bottom = m_rcJettonArea[0].top + FRAME_SIZE_HIGHT;

	//С����ע����
	m_rcJettonArea[1].left = m_rcJettonArea[0].right ;
	m_rcJettonArea[1].top = nCenterY + 260;
	m_rcJettonArea[1].right = m_rcJettonArea[1].left + FRAME_SIZE_WIDTH;
	m_rcJettonArea[1].bottom = m_rcJettonArea[1].top + FRAME_SIZE_HIGHT;

	//����ע����
	m_rcJettonArea[2].left = m_rcJettonArea[1].right ;
	m_rcJettonArea[2].top = nCenterY + 260;
	m_rcJettonArea[2].right = m_rcJettonArea[2].left + FRAME_SIZE_WIDTH;
	m_rcJettonArea[2].bottom = m_rcJettonArea[2].top +FRAME_SIZE_HIGHT;

	//С����ע����
	m_rcJettonArea[3].left = m_rcJettonArea[2].right ;
	m_rcJettonArea[3].top = nCenterY + 260;
	m_rcJettonArea[3].right = m_rcJettonArea[3].left + FRAME_SIZE_WIDTH;
	m_rcJettonArea[3].bottom = m_rcJettonArea[3].top + FRAME_SIZE_HIGHT;

	//������ע����
	m_rcJettonArea[4].left = m_rcJettonArea[3].right + 10;
	m_rcJettonArea[4].top = nCenterY + 260;
	m_rcJettonArea[4].right = m_rcJettonArea[4].left + FRAME_SIZE_WIDTH;
	m_rcJettonArea[4].bottom = m_rcJettonArea[4].top + FRAME_SIZE_HIGHT;

	//С����ע����
	m_rcJettonArea[5].left = m_rcJettonArea[4].right -10;
	m_rcJettonArea[5].top = nCenterY + 260;
	m_rcJettonArea[5].right = m_rcJettonArea[5].left + FRAME_SIZE_WIDTH;
	m_rcJettonArea[5].bottom = m_rcJettonArea[5].top + FRAME_SIZE_HIGHT;

	//������ע����
	m_rcJettonArea[6].left = m_rcJettonArea[5].right ;
	m_rcJettonArea[6].top = nCenterY +260;
	m_rcJettonArea[6].right = m_rcJettonArea[6].left+ FRAME_SIZE_WIDTH;
	m_rcJettonArea[6].bottom =  m_rcJettonArea[6].top + FRAME_SIZE_HIGHT;

	//С����ע����
	m_rcJettonArea[7].left = m_rcJettonArea[6].right - 10;
	m_rcJettonArea[7].top = nCenterY + 260;
	m_rcJettonArea[7].right = m_rcJettonArea[7].left + FRAME_SIZE_WIDTH;
	m_rcJettonArea[7].bottom = m_rcJettonArea[7].top + FRAME_SIZE_HIGHT;

	m_rcScoreLimit[0].SetRect(nCenterX + 287, nCenterY + 195, nCenterX + 387, nCenterY + 215);
	m_rcScoreLimit[1].SetRect(nCenterX + 187, nCenterY + 195, nCenterX + 287, nCenterY + 215);
	m_rcScoreLimit[2].SetRect(nCenterX + 287, nCenterY + 136, nCenterX + 387, nCenterY + 156);
	m_rcScoreLimit[3].SetRect(nCenterX + 187, nCenterY + 136, nCenterX  + 287, nCenterY + 156);
	m_rcScoreLimit[4].SetRect(nCenterX + 287, nCenterY + 77, nCenterX + 387, nCenterY + 97);
	m_rcScoreLimit[5].SetRect(nCenterX + 187, nCenterY + 77, nCenterX + 287, nCenterY + 97);
	m_rcScoreLimit[6].SetRect(nCenterX + 287, nCenterY + 18, nCenterX + 387, nCenterY + 38);
	m_rcScoreLimit[7].SetRect(nCenterX + 187, nCenterY + 18, nCenterX + 287, nCenterY + 38);

	//������ת·����ÿһ��Ļ�׼��
	CreateTurnPathAreaBasePoint(nWidth, nHeight);

	//����������ׯ����б���Ϣ����
	CreateApplayBankerListRect(nWidth, nHeight);

	//������ʷ��¼��ʾ����
	CreateHistoryRecordRect(nWidth,nHeight);
	
	int ExcursionY=10;
	for(int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		//����������ʼλ��
		if( i == 0 )
		{
			m_PointJettonNumber[i].SetPoint( m_rcJettonArea[i].right - 1 , (m_rcJettonArea[i].bottom + m_rcJettonArea[i].top)/2-ExcursionY );
		}
		else if( i == 1 )
		{
			m_PointJettonNumber[i].SetPoint( m_rcJettonArea[i].right - 8, (m_rcJettonArea[i].bottom + m_rcJettonArea[i].top)/2-ExcursionY );
		}
		else if( i == 2 )
		{
			m_PointJettonNumber[i].SetPoint( m_rcJettonArea[i].right - 10, (m_rcJettonArea[i].bottom + m_rcJettonArea[i].top)/2-ExcursionY );
		}
		else if( i == 3 )
		{
			m_PointJettonNumber[i].SetPoint( m_rcJettonArea[i].right - 19 , (m_rcJettonArea[i].bottom + m_rcJettonArea[i].top)/2-ExcursionY );
		}
		else if( i == 4 )
		{
			m_PointJettonNumber[i].SetPoint( m_rcJettonArea[i].right - 25, (m_rcJettonArea[i].bottom + m_rcJettonArea[i].top)/2-ExcursionY );
		}
		else if( i == 5 )
		{
			m_PointJettonNumber[i].SetPoint( m_rcJettonArea[i].right - 20, (m_rcJettonArea[i].bottom + m_rcJettonArea[i].top)/2-ExcursionY );
		}
		else if( i == 6 )
		{
			m_PointJettonNumber[i].SetPoint( m_rcJettonArea[i].right - 25, (m_rcJettonArea[i].bottom + m_rcJettonArea[i].top)/2-ExcursionY );
		}
		else
		{
			m_PointJettonNumber[i].SetPoint( m_rcJettonArea[i].right - 17, (m_rcJettonArea[i].bottom + m_rcJettonArea[i].top)/2-ExcursionY );
		}

		//����ͼƬ��ʼλ��
		m_PointJetton[i].SetPoint(m_rcJettonArea[i].left, m_rcJettonArea[i].top);
	}
	
	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(24);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//��ׯ��ť
	DeferWindowPos(hDwp,m_btApplyBanker,NULL,nCenterX+331, nCenterY-248,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btCancelBanker,NULL,nCenterX+331, nCenterY-248,0,0,uFlags|SWP_NOSIZE);
	
	//��ʷ��¼���ϣ����°�ť
	DeferWindowPos(hDwp,m_btScoreMoveL,NULL,nCenterX-380,nCenterY-244,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btScoreMoveR,NULL,nCenterX-380,nCenterY+200,0,0,uFlags|SWP_NOSIZE);

	//ׯ���б�ť
	DeferWindowPos(hDwp, m_btBankerMoveUp, NULL, nCenterX+295, nCenterY+276, 0, 0, uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp, m_btBankerMoveDown, NULL, nCenterX+317, nCenterY+276, 0, 0, uFlags|SWP_NOSIZE);

	//���а�ť
	DeferWindowPos(hDwp, m_btBankButton, NULL, nCenterX-420, nCenterY+230, 0, 0, uFlags|SWP_NOSIZE);

	//��Ͷ��ť
	DeferWindowPos(hDwp,m_btAddStep1,NULL,nCenterX + 300,nCenterY + 230,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btAddStep10,NULL,nCenterX + 332,nCenterY + 230,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btAddStep100,NULL,nCenterX + 372,nCenterY + 230,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btAddStep1_H,NULL,nCenterX + 300,nCenterY + 230,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btAddStep10_H,NULL,nCenterX + 332,nCenterY + 230,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btAddStep100_H,NULL,nCenterX + 372,nCenterY + 230,0,0,uFlags|SWP_NOSIZE);

	DeferWindowPos(hDwp,m_btBetLast,NULL,nCenterX + 230,nCenterY + 270,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBetSmall,NULL,nCenterX + 292,nCenterY + 270,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btBetBig,NULL,nCenterX + 354,nCenterY + 270,0,0,uFlags|SWP_NOSIZE);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	return;
}

//������ת·���ϸ�С��������ĵ�
void CGameClientView::CreateTurnPathAreaBasePoint(int nWidth, int nHeight)
{
	int nCenterX = nWidth/2;
	int nCenterY = nHeight/2;

	//��һ����
	int BaseX = nCenterX-291;
	int BaseY = nCenterY-251;
	int w = 70;	//������
	int h = 70; //����߶�
	for( int i=0; i<7; i++ )
	{
		m_PtCardArray[i].SetPoint(BaseX+i*w, BaseY);
	}

	//�ڶ�����
	BaseX = nCenterX+129;//307
	BaseY = nCenterY-251+h;
	int j=0;
	for(int i=7; i<12; i++ )
	{
		m_PtCardArray[i].SetPoint(BaseX, BaseY+(j++)*h);
	}

	//��������
	BaseX = nCenterX+129;//307
	BaseY = nCenterY+99+h;
	j=0;
	for( int i=12; i<19; i++ )
	{
		m_PtCardArray[i].SetPoint(BaseX-(j++)*w, BaseY);
	}

	//��������
	BaseX = nCenterX-291;
	BaseY = nCenterY+29+h;
	j=0;
	for( int i=19; i<24; i++ )
	{
		m_PtCardArray[i].SetPoint(BaseX, BaseY-(j++)*h);
	}
}
void CGameClientView::CreateApplayBankerListRect(int nBaseX, int nBaseY)
{
	int nCenterX = nBaseX/2;
	int nCenterY = nBaseY/2;

	//����������ׯ����б���Ϣ����
	int vSpace = 15;
	int x1 = nCenterX+170;
	int x2 = nCenterX+200;
	int x3 = nCenterX+295;
	int x4 = nCenterX+395;
	int y = nCenterY - 125;
	for( int i=0; i<sizeof(m_rcApplayBankerNo)/sizeof(m_rcApplayBankerNo[0]); i++)
	{
		m_rcApplayBankerNo[i].SetRect(x1, y+i*vSpace, x2, y+(i+1)*vSpace);
	}
	for( int i=0; i<sizeof(m_rcApplayBankerName)/sizeof(m_rcApplayBankerName[0]); i++ )
	{
		m_rcApplayBankerName[i].SetRect(x2,y+i*vSpace,x3, y+(i+1)*vSpace);
	}
	for( int i=0; i<sizeof(m_rcApplayBankerScore)/sizeof(m_rcApplayBankerScore[0]); i++ )
	{
		m_rcApplayBankerScore[i].SetRect(x3, y+i*vSpace,x4,y+(i+1)*vSpace);
	}
}
//������ʷ��¼��ʾ��
void CGameClientView::CreateHistoryRecordRect(int nWidth, int nHeight)
{
	int nCenterX = nWidth/2;
	int nCenterY = nHeight/2;
	int nRectWidth = 39;
	int nRectHeight= 39;
	int nBaseX = nCenterX-388;
	int nBaseY = nCenterY-221;
	//int nBaseX = nCenterX-156;
	//int nBaseY = nCenterY+268;

	for( int i=0; i<sizeof(m_rcHistoryRecord)/sizeof(m_rcHistoryRecord[0]); i++ )
	{
		m_rcHistoryRecord[i].SetRect(nBaseX, nBaseY+nRectHeight*i, nBaseX+nRectWidth,nBaseY+nRectHeight*(i+1));
		//m_rcHistoryRecord[i].SetRect(nBaseX+nRectWidth*i, nBaseY, nBaseX+nRectWidth*(i+1),nBaseY+nRectHeight);
	}
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageViewBack,DRAW_MODE_CENTENT);

	int nCenterX = nWidth / 2;
	int nCenterY = nHeight / 2;

	//��ȡ״̬
	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();


	//״̬��ʾ
	CFont InfoFont;
	InfoFont.CreateFont(-13,0,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
	CFont * pOldFont=pDC->SelectObject(&InfoFont);
	pDC->SetTextColor(RGB(255,234,0));

	//ʱ����ʾ
	DrawTimeTip(pDC, nWidth, nHeight);

	//��ǰ���·�
	DrawAreaLimitScore(pDC);
	
	pDC->SelectObject(pOldFont);
	InfoFont.DeleteObject();

	//ʤ���߿�
	FlashJettonAreaFrame(nWidth,nHeight,pDC);

	//���Ƴ���
	//DrawAreaJetton(pDC);

	//ׯ����Ϣ		
	DrawBankerInfo(pDC, nWidth, nHeight);

	//�滭�û�
	//DrawUserInfo(pDC, nWidth, nHeight);

	//�滭��ǰ�û�����
	DrawMeScore(pDC,nWidth,nHeight);

	//�����û��÷�
	DrawCurrentScore(pDC,nWidth,nHeight);
	
	//�л�ׯ��
	DrawChangeBanker(pDC, nWidth, nHeight);

	//�ҵ���ע
	DrawMeJettonNumber(pDC);

	//�滭ʱ��
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		WORD wUserTimer=GetUserClock(m_wMeChairID);
		if (wUserTimer!=0) 
			DrawUserTimer(pDC,nCenterX - 42,nCenterY+72,wUserTimer,99);
	}

	//��ʾ���
	ShowGameResult(pDC, nWidth, nHeight);	

	//��ըЧ��
	DrawBombEffect(pDC);

	//����������ׯ����б�
	DrawApplyBankerUserList(pDC);
	
	//������ʷ��¼
	DrawHistoryRecord(pDC);

	//���ƶ���
	DrawCardCartoon(pDC);

	//������ע��ť
	if( m_bisLButtonDown == true )
	{
		DrawAddMoney(pDC, nWidth,nHeight );
	}
	//DrawAddMoney(pDC, nWidth,nHeight );

	return;
}

//�л�ׯ��
void CGameClientView::DrawChangeBanker(CDC * pDC, int nWidth, int nHeight)
{
	int nCenterX = nWidth/2;
	int nCenterY = nHeight/2;

	if ( m_bShowChangeBanker )
	{
		int	nXPos = nCenterX - 133;
		int	nYPos = nCenterY - 55;

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
}

//����ׯ����Ϣ
void CGameClientView::DrawBankerInfo(CDC * pDC, int nWidth, int nHeight)
{
	int nCenterX = nWidth/2;
	int nCenterY = nHeight/2;
	pDC->SetTextColor(RGB(255,234,0));

	//��ȡ���
	IClientUserItem *pUserItem = ( m_wBankerUser==INVALID_CHAIR ? NULL : GetClientUserItem(m_wBankerUser) );
	tagUserInfo *pUserData = NULL;
	if(pUserItem!=NULL)
		pUserData = pUserItem->GetUserInfo();

	//λ����Ϣ
	CRect StrRect;
	StrRect.left = nCenterX + 222;
	StrRect.top = nCenterY - 283;
	StrRect.right = StrRect.left + 105;
	StrRect.bottom = StrRect.top + 24;

	//ׯ������
	pDC->DrawText(pUserData==NULL?(m_bEnableSysBanker?TEXT("ϵͳ��ׯ"):TEXT("������ׯ")):pUserData->szNickName, StrRect, DT_END_ELLIPSIS | DT_CENTER | DT_TOP| DT_SINGLELINE );

	//ׯ�Ҿ���
	StrRect.left = nCenterX + 222;
	StrRect.top = nCenterY - 260;
	StrRect.right = StrRect.left + 105;
	StrRect.bottom = StrRect.top + 24;
	DrawNumberStringWithSpace(pDC,m_wBankerTime,StrRect);

	//ׯ�ҳɼ�
	StrRect.left = nCenterX + 222;
	StrRect.top = nCenterY - 238;
	StrRect.right = StrRect.left + 105;
	StrRect.bottom = StrRect.top + 24;
	DrawNumberStringWithSpace(pDC,m_iBankerWinScore,StrRect);

	//ׯ���ܷ�
	StrRect.left = nCenterX + 222;
	StrRect.top = nCenterY - 215;
	StrRect.right = StrRect.left + 105;
	StrRect.bottom = StrRect.top + 24;
	DrawNumberStringWithSpace(pDC,pUserData==NULL?0:pUserData->lScore, StrRect);

	
}

//�����м���Ϣ
void CGameClientView::DrawUserInfo(CDC * pDC, int nWidth, int nHeight)
{
	int nCenterX = nWidth/2;
	int nCenterY = nHeight/2;
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		IClientUserItem *pUserItem =GetClientUserItem(m_wMeChairID);
		tagUserInfo *pMeUserData = NULL;
		if(pUserItem!=NULL)
			pMeUserData = pUserItem->GetUserInfo();
		if ( pMeUserData != NULL )
		{
			//��Ϸ��Ϣ
			TCHAR szResultScore[16]=TEXT("");
			TCHAR szGameScore[16]=TEXT("");
			pDC->SetTextColor(RGB(255,255,255));

			__int64 iMeJetton = 0;
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
				iMeJetton += m_iMeAreaScore[i];

			CRect rcAccount(CPoint(nCenterX-295,nCenterY+225),CPoint(nCenterX-195,nCenterY+245));
			CRect rcGameScore(CPoint(nCenterX-295,nCenterY+245),CPoint(nCenterX-195,nCenterY+265));
			CRect rcResultScore(CPoint(nCenterX-295,nCenterY+265),CPoint(nCenterX-195,nCenterY+285));

			DrawNumberStringWithSpace(pDC,pMeUserData->lScore-iMeJetton,rcGameScore,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);
			DrawNumberStringWithSpace(pDC,m_iMeStatisticScore,rcResultScore,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

			pDC->DrawText(pMeUserData->szNickName,lstrlen(pMeUserData->szNickName),rcAccount,DT_VCENTER|DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS);

		}
	}
}

//����ʱ����ʾ
void CGameClientView::DrawTimeTip(CDC * pDC, int nWidth, int nHeight)
{
	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();
	int nCenterX = nWidth/2;
	int nCenterY = nHeight/2;
	//ʱ����ʾ
	int nTimeFlagWidth = m_ImageTimeFlag.GetWidth()/3;
	if (cbGameStatus==GAME_STATUS_FREE)
	{
		m_ImageFreeTime.DrawImage(pDC,nCenterX-210, nCenterY - 100 );
	}
	else if (cbGameStatus==GS_PLACE_JETTON)
	{
		int nJettonWidth = m_ImageJettonTime.GetWidth()/2;
		int nJettonHeight = m_ImageJettonTime.GetHeight();
		for( int i = 0; i < 2; i ++ )
		{
			m_ImageJettonTime.DrawImage(pDC,nCenterX-210,nCenterY - 100,nJettonWidth,nJettonHeight,
				i*nJettonWidth,0);
		}
		
	}
	else if (cbGameStatus==GS_GAME_END)
	{
		int nSmallWidth = m_ImageBitSmallTime.GetWidth()/2;
		int nSmallHeight = m_ImageBitSmallTime.GetHeight();
		if( m_cbGoalAnimal != 0xFF )
		{
			for( int i = 0; i < 2; i ++ )
			{
				m_ImageBitSmallTime.DrawImage(pDC,nCenterX-210,nCenterY - 100,nSmallWidth,nSmallHeight,
					i*nSmallWidth,0);
			}
		}

	}
	//m_ImageTimeFlag.AlphaDrawImage(pDC, nCenterX-100, nCenterY-186, nTimeFlagWidth, m_ImageTimeFlag.GetHeight(), nFlagIndex*nTimeFlagWidth, 0, GetPixel(pDC->GetSafeHdc(), 0, 0));

}

//������ע��ť
void CGameClientView::DrawAddMoney(CDC * pDC, int nWidth, int nHeight)
{
	int nCenterX = nWidth/2;
	int nCenterY = nHeight/2;
	int nImageWidth = m_ImageKeyPress.GetWidth()/8;
	int nImageHight = m_ImageKeyPress.GetHeight();

	//��ȡ��ע����
	//��ȡ����
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);
	BYTE cbJettonArea= GetJettonArea(MousePoint);

	if ( cbJettonArea!=0xFF ) 
	{
		m_ImageKeyPress.AlphaDrawImage(pDC,m_rcJettonArea[cbJettonArea].left,m_rcJettonArea[cbJettonArea].top,
										nImageWidth,nImageHight,
										nImageWidth*cbJettonArea,0,RGB(255,0,255));
		m_bisLButtonDown = false;
	}
	
}
//���Ƴ���
void CGameClientView::DrawAreaJetton(CDC * pDC)
{
	//������Դ
	CSize SizeJettonItem(m_ImageJettonView.GetWidth()/JETTON_COUNT,m_ImageJettonView.GetHeight());

	//�滭����
	for (INT i=0;i<JETTON_AREA_COUNT;i++)
	{
		//��������
		__int64 lScoreCount=0L;
		__int64 lScoreJetton[JETTON_COUNT]={100L,500L,1000L,10000L,50000L,100000L,500000L,1000000L,5000000L};

		//�滭����
		for (INT_PTR j=0;j<m_JettonInfoArray[i].GetCount();j++)
		{
			//��ȡ��Ϣ
			tagJettonInfo * pJettonInfo=&m_JettonInfoArray[i][j];

			//�ۼ�����
			ASSERT(pJettonInfo->cbJettonIndex<JETTON_COUNT);
			lScoreCount+=lScoreJetton[pJettonInfo->cbJettonIndex];

			//�滭����
			m_ImageJettonView.AlphaDrawImage(pDC,
				pJettonInfo->nXPos+m_PointJetton[i].x+6,pJettonInfo->nYPos+m_PointJetton[i].y+6,
				SizeJettonItem.cx,SizeJettonItem.cy,
				pJettonInfo->cbJettonIndex*SizeJettonItem.cx,0,RGB(255,0,255));
		}

		//�滭����
		if (lScoreCount>0L)	
			DrawNumberString(pDC,lScoreCount,m_PointJettonNumber[i].x,m_PointJettonNumber[i].y);
	}
}
//����������ע
void CGameClientView::DrawAreaLimitScore(CDC * pDC)
{
	CFont * pOldFont = pDC->SelectObject(&m_fontScoreLimit);

	BYTE cbGameStatus=m_pGameClientDlg->GetGameStatus();
	pDC->SetTextColor(RGB(255,255,255));
	__int64 iLimitScore=0;
	CString strMsg;
	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		iLimitScore = m_iAreaScoreLimit[i];
		if( iLimitScore > 0 && cbGameStatus == GS_PLACE_JETTON)
		{
			strMsg.Format(TEXT("%I64d"), iLimitScore);
			pDC->DrawText(strMsg, m_rcScoreLimit[i], DT_END_ELLIPSIS|DT_CENTER|DT_TOP|DT_SINGLELINE);
		}
		else
		{
			strMsg = TEXT("���·�");
			pDC->DrawText(strMsg, m_rcScoreLimit[i], DT_END_ELLIPSIS|DT_CENTER|DT_TOP|DT_SINGLELINE);
		}
	}
	pDC->SelectObject(pOldFont);
}
//������Ϣ
void CGameClientView::SetMeMaxScore(__int64 iMeMaxScore)
{
	if (m_iMeMaxScore!=iMeMaxScore)
	{
		//���ñ���
		m_iMeMaxScore=iMeMaxScore;
	}

	return;
}

//�����ע
void CGameClientView::SetAreaLimitScore(__int64 * pLimitArray, int ArraySize)
{
	if( pLimitArray == NULL || ArraySize != JETTON_AREA_COUNT )
		return;

	for( int i=0; i<ArraySize; i++ )
		m_iAreaScoreLimit[i] = pLimitArray[i];
	return;
}

//��ʷ��¼
void CGameClientView::SetGameHistory(tagGameRecord * pGameRecord, int nRecordCount)
{
	//��������
	m_GameRecord.FillGameRecord(pGameRecord, nRecordCount);
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

	//��ע��Ϣ
	ZeroMemory(m_iMeAreaScore, sizeof(m_iMeAreaScore));

	//ȫ����ע
	ZeroMemory(m_iAllAreaScore, sizeof(m_iAllAreaScore));

	m_strDispatchCardTips=TEXT("");

	m_bCanShowResult = FALSE;
	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//������ע
void CGameClientView::SetMePlaceJetton(BYTE cbViewIndex, __int64 iJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=ID_SMALL_SNAKE);
	if (cbViewIndex>ID_SMALL_SNAKE)
		return;

	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		if( i == cbViewIndex )
			m_iMeAreaScore[i] = iJettonCount;
	}

	//���½���
	InvalidGameView(0,0,0,0);
}

//���ó���
void CGameClientView::PlaceUserJetton(BYTE cbViewIndex, __int64 iScoreCount)
{
	//Ч�����
	ASSERT(cbViewIndex<=ID_SMALL_SNAKE);
	if (cbViewIndex>ID_SMALL_SNAKE)
		return;

	//����ը��
	if (iScoreCount==5000000L) 
		SetBombEffect(true,cbViewIndex);

	//��������
	bool bPlaceJetton=false;
	__int64 iScoreIndex[JETTON_COUNT]={100L,500L,1000L,10000L,50000L,100000L,500000L,1000000L,5000000L};

	//�߿���
	int nFrameWidth=0;
	int nFrameHeight=0;
	int nBorderWidth=6;

	for(int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		if( cbViewIndex == i )
		{
			m_iAllAreaScore[i] += iScoreCount;
			m_iAreaScoreLimit[i] -= iScoreCount;
			nFrameWidth = m_rcJettonArea[i].right - m_rcJettonArea[i].left;
			nFrameHeight = m_rcJettonArea[i].bottom - m_rcJettonArea[i].top;
		}
	}

	nFrameWidth -= nBorderWidth;
	nFrameHeight -= nBorderWidth;

	//���ӳ���
	for (BYTE i=0;i<CountArray(iScoreIndex);i++)
	{
		//������Ŀ
		BYTE cbScoreIndex=JETTON_COUNT-i-1;
		__int64 iCellCount=iScoreCount/iScoreIndex[cbScoreIndex];

		//�������
		if (iCellCount==0L)
			continue;

		//�������
		for (LONG j=0;j<iCellCount;j++)
		{
			//�������
			tagJettonInfo JettonInfo;
			::ZeroMemory(&JettonInfo, sizeof(JettonInfo));
			int nJettonSize=68;
			JettonInfo.cbJettonIndex=cbScoreIndex;
			JettonInfo.nXPos=rand()%(nFrameWidth-nJettonSize);
			JettonInfo.nYPos=rand()%(nFrameHeight-nJettonSize);

			//��������
			bPlaceJetton=true;
			m_JettonInfoArray[cbViewIndex].Add(JettonInfo);
		}

		//������Ŀ
		iScoreCount-=iCellCount*iScoreIndex[cbScoreIndex];
	}

	//���½���
	if (bPlaceJetton==true) 
		InvalidGameView(0,0,0,0);

	return;
}

//���ֳɼ�
void CGameClientView::SetCurGameScore(__int64 iMeCurGameScore, __int64 iMeCurGameReturnScore, __int64 iBankerCurGameScore, __int64 iGameRevenue)
{
	m_iMeCurGameScore=iMeCurGameScore;			
	m_iMeCurGameReturnScore=iMeCurGameReturnScore;			
	m_iBankerCurGameScore=iBankerCurGameScore;			
	m_iGameRevenue=iGameRevenue;					
}

//����ʤ��
void CGameClientView::SetWinnerSide(BYTE cbWinnerSide)
{
	//���ñ���
	m_cbWinnerSide=cbWinnerSide;
	m_cbAreaFlash=cbWinnerSide;

	//����ʱ��
	if (cbWinnerSide!=0xFF)
	{
		SetTimer(IDI_FLASH_WINNER,300,NULL);
		SetTimer(IDI_END_CARTOON,500,NULL);
	}
	else 
	{
		KillTimer(IDI_FLASH_WINNER);
		KillTimer(IDI_END_CARTOON);
		m_nCurCartoonIndex = m_cbGoalAnimal;
	}

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//��ȡ����
BYTE CGameClientView::GetJettonArea(CPoint MousePoint)
{
	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		if( m_rcJettonArea[i].PtInRect(MousePoint))
			return i;
	}
	return 0xFF;
}

//�滭����
void CGameClientView::DrawNumberString(CDC * pDC, __int64 iNumber, INT nXPos, INT nYPos, bool bMeScore)
{
	//������Դ
	CSize SizeScoreNumber(m_ImageScoreNumber.GetWidth()/10,m_ImageScoreNumber.GetHeight());

	if ( bMeScore ) 
		SizeScoreNumber.SetSize(m_ImageMeScoreNumber.GetWidth()/10, m_ImageMeScoreNumber.GetHeight());

	//������Ŀ
	int iNumberCount=0;
	__int64 iNumberTemp=iNumber;
	do
	{
		iNumberCount++;
		iNumberTemp/=10;
	} while (iNumberTemp>0);

	//λ�ö���
	INT nYDrawPos=nYPos-SizeScoreNumber.cy/2;
	INT nXDrawPos=nXPos - 10;//+iNumberCount*SizeScoreNumber.cx/2-SizeScoreNumber.cx;
	//INT nXDrawPos=nXPos+iNumberCount*SizeScoreNumber.cx/2-SizeScoreNumber.cx;

	//�滭����
	for (LONG i=0;i<iNumberCount;i++)
	{
		//�滭����
		int iCellNumber=(int)(iNumber%10);
		if ( bMeScore )
		{
			m_ImageMeScoreNumber.AlphaDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				iCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
		}
		else
		{
			m_ImageScoreNumber.AlphaDrawImage(pDC,nXDrawPos,nYDrawPos,SizeScoreNumber.cx,SizeScoreNumber.cy,
				iCellNumber*SizeScoreNumber.cx,0,RGB(255,0,255));
		}

		//���ñ���
		iNumber/=10;
		nXDrawPos-=SizeScoreNumber.cx;
	};

	return;
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, __int64 iNumber, INT nXPos, INT nYPos)
{
	CString strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;
	strNumber.Format(TEXT("%I64d"),iNumber>0?iNumber:-iNumber);
	int p=strNumber.GetLength()-3;
	while(p>0)
	{
		strNumber.Insert(p,TEXT(","));
		p-=3;
	}
	if(iNumber<0)
		strNumber=TEXT("-")+strNumber;
	//�������
	pDC->TextOut(nXPos,nYPos,strNumber);
}

//�滭����
void CGameClientView::DrawNumberStringWithSpace(CDC * pDC, __int64 iNumber, CRect rcRect, INT nFormat)
{
	CString strNumber=TEXT(""), strTmpNumber1,strTmpNumber2;

	strNumber.Format(TEXT("%I64d"),iNumber>0?iNumber:-iNumber);
	int p=strNumber.GetLength()-3;
	while(p>0)
	{
		strNumber.Insert(p,TEXT(","));
		p-=3;
	}
	if(iNumber<0)
		strNumber=TEXT("-")+strNumber;

	//�������
	if (nFormat==-1) 
		pDC->DrawText(strNumber,rcRect,DT_END_ELLIPSIS|DT_CENTER|DT_TOP|DT_SINGLELINE);
	else 
		pDC->DrawText(strNumber,rcRect,nFormat);
}


//��Ͷ��ť
void CGameClientView::OnAddStep1()
{
	//���ñ���
	m_iCurrentJetton=1L;

	m_btAddStep1_H.ShowWindow(SW_SHOW);
	m_btAddStep1_H.EnableWindow(true);
	m_btAddStep1.ShowWindow(SW_HIDE);

	m_btAddStep10.ShowWindow(SW_SHOW);
	m_btAddStep10.EnableWindow(true);
	m_btAddStep10_H.ShowWindow(SW_HIDE);

	m_btAddStep100.ShowWindow(SW_SHOW);
	m_btAddStep100.EnableWindow(true);
	m_btAddStep100_H.ShowWindow(SW_HIDE);
	return;
}

//��Ͷ��ť
void CGameClientView::OnAddStep10()
{
	//���ñ���
	m_iCurrentJetton=10L;

	m_btAddStep1.ShowWindow(SW_SHOW);
	m_btAddStep1.EnableWindow(true);
	m_btAddStep1_H.ShowWindow(SW_HIDE);

	m_btAddStep10_H.ShowWindow(SW_SHOW);
	m_btAddStep10_H.EnableWindow(true);
	m_btAddStep10.ShowWindow(SW_HIDE);

	m_btAddStep100.ShowWindow(SW_SHOW);
	m_btAddStep100.EnableWindow(true);
	m_btAddStep100_H.ShowWindow(SW_HIDE);

	return;
}

//��Ͷ��ť
void CGameClientView::OnAddStep100()
{
	//���ñ���
	m_iCurrentJetton=100L;

	m_btAddStep1.ShowWindow(SW_SHOW);
	m_btAddStep1.EnableWindow(true);
	m_btAddStep1_H.ShowWindow(SW_HIDE);

	m_btAddStep10.ShowWindow(SW_SHOW);
	m_btAddStep10.EnableWindow(true);
	m_btAddStep10_H.ShowWindow(SW_HIDE);

	m_btAddStep100_H.ShowWindow(SW_SHOW);
	m_btAddStep100_H.EnableWindow(true);
	m_btAddStep100.ShowWindow(SW_HIDE);

	return;
}

//��Ͷ��ť
void CGameClientView::OnAddStep1_H()
{
	//���ñ���
	m_iCurrentJetton=1L;

	m_btAddStep1_H.ShowWindow(SW_SHOW);
	m_btAddStep1_H.EnableWindow(true);
	m_btAddStep1.ShowWindow(SW_HIDE);

	m_btAddStep10.ShowWindow(SW_SHOW);
	m_btAddStep10.EnableWindow(true);
	m_btAddStep10_H.ShowWindow(SW_HIDE);

	m_btAddStep100.ShowWindow(SW_SHOW);
	m_btAddStep100.EnableWindow(true);
	m_btAddStep100_H.ShowWindow(SW_HIDE);

	return;
}

//��Ͷ��ť
void CGameClientView::OnAddStep10_H()
{
	//���ñ���
	m_iCurrentJetton=10L;

	return;
}

//��Ͷ��ť
void CGameClientView::OnAddStep100_H()
{
	//���ñ���
	m_iCurrentJetton=100L;
	return;
}
//����������ʱ��ʱ�������飬������׶������ȼ�
void CGameClientView::UpdateCartoonTimeElapse(BYTE cbStopStep)
{
	m_cbGoalAnimal=cbStopStep;		//��¼�±�����ת����ͣ����λ��
	ZeroMemory(m_CartoonTimeElapseArray, sizeof(m_CartoonTimeElapseArray));
	m_nCurCartoonIndex = 0;			//���³�ʼ����ͨ��ͼλ�������±�
	m_CartoonElapsIndex = 0;		//���³�ʼ����ͨ��ͼ��ʱ�������±�
	int nTimeElaps0=800;			//��0�͵ڰ˽׶�
	int nTimeElaps1=400;			//��һ�͵��߽׶�
	int nTimeElaps2=200;			//��һ�͵���׶�
	int nTimeElaps3=80;				//�ڶ��͵��Ľ׶�
	int nTimeElaps4=50;				//�����׶Σ�ÿ����ʱ������Ϊ500����

	//int nAllStep = 64+cbStopStep;	//�ܹ���ת��ô���ʱ������
	int nAllStep = 48+cbStopStep;	//�ܹ���ת��ô���ʱ������
	int n1 = 2;						//��һ�׶�5��ʱ������
	int n2 = 3;						//�ڶ��׶�4��ʱ������
	int n3 = 4;
	int n4 = 5;
	int n6 = 5;
	int n7 = 4;
	int n8 = 3;
	int n9 = 2;
	int n5 = nAllStep-n1-n2-n3-n4-n6-n7-n8-n9;		//����׶���ת5��ʱ������
	
	//��һ�׶�ʱ��ֵ(0-5)
	for( int i=0; i<n1; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps0;

	//�ڶ��׶�ʱ��ֵ
	for(int i=n1; i<n1+n2; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps1;

	//�����׶�ʱ��ֵ
	for( int i=n1+n2; i<n1+n2+n3; i++)
		m_CartoonTimeElapseArray[i] = nTimeElaps2;

	//���Ľ׶�ʱ��ֵ
	for( int i=n1+n2+n3; i<n1+n2+n3+n4; i++ )
		m_CartoonTimeElapseArray[i] = nTimeElaps3;

	//����׶�ʱ��ֵ
	for( int i=n1+n2+n3+n4; i<n1+n2+n3+n4+n5; i++)
		m_CartoonTimeElapseArray[i]=nTimeElaps4;
	
	//�����׶�
	for( int i=n1+n2+n3+n4+n5;i<n1+n2+n3+n4+n5+n6;i++)
		m_CartoonTimeElapseArray[i]=nTimeElaps3;

	//���߽׶�
	for( int i=n1+n2+n3+n4+n5+n6;i<n1+n2+n3+n4+n5+n6+n7;i++)
		m_CartoonTimeElapseArray[i]=nTimeElaps2;

	for( int i=n1+n2+n3+n4+n5+n6+n7;i<n1+n2+n3+n4+n5+n6+n7+n8;i++)
		m_CartoonTimeElapseArray[i]=nTimeElaps1;

	for( int i=n1+n2+n3+n4+n5+n6+n7+n8;i<=n1+n2+n3+n4+n5+n6+n7+n8+n9;i++)
		m_CartoonTimeElapseArray[i]=nTimeElaps0;

	SetTimer(IDI_DISPATCH_CARD, m_CartoonTimeElapseArray[m_CartoonElapsIndex++], NULL);	//��ʼ�������ƶ���
	m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("TURN_START"));
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

		//if(m_nCurCartoonIndex == m_cbGoalAnimal )
		//{
		//	m_nCurCartoonIndex = 0xFF;
		//}
		//else
		//{
		//	m_nCurCartoonIndex = m_cbGoalAnimal;
		//}

		//���½���
		InvalidGameView(0,0,0,0);

		return;
	}

	if( nIDEvent == IDI_END_CARTOON )
	{
		if(m_nCurCartoonIndex == m_cbGoalAnimal )
		{
			m_nCurCartoonIndex = 0xFF;
		}
		else
		{
			m_nCurCartoonIndex = m_cbGoalAnimal;
		}

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

	else if (nIDEvent==IDI_DISPATCH_CARD)	//���Ƽ�ʱ�����ڴ˸���������ʱ��
	{
		KillTimer(IDI_DISPATCH_CARD);	//��ɱ����һ����ʱ��
		PlayingCartoonEffect();
		return;
	}

	//��ը����
	if (nIDEvent<=IDI_BOMB_EFFECT+ID_SMALL_SNAKE && IDI_BOMB_EFFECT+ID_BIG_TIGER<=nIDEvent)
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
		else 
			m_cbBombFrameIndex[wIndex]++;

		//���½���
		InvalidGameView(0,0,0,0);
		return;
	}

	__super::OnTimer(nIDEvent);
}

//��ʼ��ת
void CGameClientView::PlayingCartoonEffect()
{
	InvalidGameView(0,0,0,0);			//ˢ�½��棬��ʹ��һ��ͼ������
	if( m_CartoonElapsIndex < sizeof(m_CartoonTimeElapseArray)/sizeof(m_CartoonTimeElapseArray[0]))
	{
		if( m_CartoonTimeElapseArray[m_CartoonElapsIndex]>0)	//��Ч��ʱ��ֵ
		{
			m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"));
			m_nCurCartoonIndex = (m_nCurCartoonIndex+1)%24;//32
			SetTimer(IDI_DISPATCH_CARD, m_CartoonTimeElapseArray[m_CartoonElapsIndex++],NULL);
		}
		else	//��ת�������ô�����㲿�ֵ��߼���
		{
			m_bCanShowResult = TRUE;
			EndCartoonEffect();
		}
	}
}

void CGameClientView::EndCartoonEffect()
{
	//����Ӯ��
	SetWinnerSide(m_cbGoalAnimal%JETTON_AREA_COUNT);

	//��������
	if (m_iMeCurGameScore>0) 
		m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_WIN"));
	else if (m_iMeCurGameScore<0) 
		m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_LOST"));
	else 
		m_pGameClientDlg->PlayGameSound(AfxGetInstanceHandle(),TEXT("END_DRAW"));
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	if( m_pGameClientDlg->GetGameStatus() == GS_PLACE_JETTON )
	{
		//���ʣ�µĻ��֣���ң�����������ô���ע
		__int64 iLeaveScore = GetUserCurrentLeaveScore();
		if( m_iCurrentJetton > iLeaveScore )
			return;

		//��ȡ��ע����
		BYTE cbJettonArea=GetJettonArea(Point);

		if (cbJettonArea!=0xFF) 
		{
			if( m_pGameClientDlg != NULL )
			{
				//m_iCurrentJetton = 1;
				m_bisLButtonDown = true;
				m_pGameClientDlg->OnPlaceJetton(cbJettonArea, m_iCurrentJetton);
			}

		}
	}

	__super::OnLButtonDown(nFlags,Point);
}

//��ȡ��ǰ��һ�����ע������
__int64 CGameClientView::GetUserCurrentLeaveScore()
{
	__int64 iTemp = 0;
	for( int i=0; i<JETTON_AREA_COUNT; i++ )
		iTemp += m_iMeAreaScore[i];
	__int64 iLeaveScore = m_iMeMaxScore - iTemp;
	return  __max(iLeaveScore, 0);
}

//�����Ϣ
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//���ñ���
	m_iCurrentJetton=0L;

	if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END && m_cbAreaFlash!=0xFF)
	{
		m_cbAreaFlash=0xFF;
		InvalidGameView(0,0,0,0);
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
	InvalidGameView(0,0,0,0);
}

//��ׯ��ť
void CGameClientView::OnApplyBanker()
{
	AfxGetMainWnd()->SendMessage(IDM_APPLY_BANKER,1,0);

}

//��ׯ��ť
void CGameClientView::OnCancelBanker()
{
	AfxGetMainWnd()->SendMessage(IDM_APPLY_BANKER,0,0);
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
void CGameClientView::SetBankerInfo(WORD wBankerUser, __int64 iBankerScore) 
{
	//�л��ж�
	if (m_wBankerUser!=wBankerUser)
	{
		m_wBankerUser=wBankerUser;
		m_wBankerTime=0L;
		m_iBankerWinScore=0L;	
	}
	m_iBankerScore=iBankerScore;
}

//�ƶ���ť
void CGameClientView::OnScoreMoveL()
{
	int n = m_GameRecord.ScrollLeft();
	if( n == -1 )
		m_btScoreMoveL.EnableWindow(FALSE);
	if( !m_btScoreMoveR.IsWindowEnabled())
		m_btScoreMoveR.EnableWindow(TRUE);
	//���½���
	InvalidGameView(0,0,0,0);
}

//�ƶ���ť
void CGameClientView::OnScoreMoveR()
{
	int n = m_GameRecord.ScrollRight();
	if( n == -1 )
		m_btScoreMoveR.EnableWindow(FALSE);
	if( !m_btScoreMoveL.IsWindowEnabled())
		m_btScoreMoveL.EnableWindow(TRUE);
	//���½���
	InvalidGameView(0,0,0,0);
}

void CGameClientView::OnBankerListMoveUp()
{
	int n = m_ApplyUser.ScrollUp();
	if( n == 0 )
		m_btBankerMoveUp.EnableWindow(FALSE);
	if(!m_btBankerMoveDown.IsWindowEnabled())
		m_btBankerMoveDown.EnableWindow(TRUE);
	InvalidGameView(0,0,0,0);
}
void CGameClientView::OnBankerListMoveDown()
{
	int n = m_ApplyUser.ScrollDown();
	int c = m_ApplyUser.GetApplayBankerUserCount();
	if( n == c-1 )
		m_btBankerMoveDown.EnableWindow(FALSE);
	if( !m_btBankerMoveUp.IsWindowEnabled())
		m_btBankerMoveUp.EnableWindow(TRUE);
	InvalidGameView(0,0,0,0);
}

void CGameClientView::OnClickedBank()
{
	m_pGameClientDlg->OpenBank();
}
//��ʾ���
void CGameClientView::ShowGameResult(CDC *pDC, int nWidth, int nHeight)
{
	//��ʾ�ж�
	if (m_pGameClientDlg->GetGameStatus()!=GS_GAME_END )
		return;

	if(!m_bCanShowResult )
		return;

	int	nXPos = nWidth / 2 - 129;
	int	nYPos = nHeight / 2 - 208;

	CRect rcAlpha(nXPos+2, nYPos+70, nXPos+2 + m_ImageGameEnd.GetWidth(), nYPos+70+m_ImageGameEnd.GetHeight());
	DrawAlphaRect(pDC, &rcAlpha, RGB(74,70,73), 0.8f);
	m_ImageGameEnd.AlphaDrawImage(pDC, nXPos+2, nYPos+70, RGB(255,0,255));

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
	DrawNumberStringWithSpace(pDC,m_iMeCurGameScore,rcMeWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	DrawNumberStringWithSpace(pDC,m_iMeCurGameReturnScore,rcMeReturnScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	CRect rcBankerWinScore;
	rcBankerWinScore.left = nXPos+2 + 40;
	rcBankerWinScore.top = nYPos+70 + 69;
	rcBankerWinScore.right = rcBankerWinScore.left + 111;
	rcBankerWinScore.bottom = rcBankerWinScore.top + 34;

	DrawNumberStringWithSpace(pDC,m_iBankerCurGameScore,rcBankerWinScore, DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}

//�����ҵķ���
void CGameClientView::DrawMeScore(CDC *pDC, int nWidth, int nHeight)
{
	int nImageWidth = m_ImageBetNumber.GetWidth()/10;
	int nImageHeight = m_ImageBetNumber.GetHeight();
	if (m_wMeChairID!=INVALID_CHAIR)
	{
		IClientUserItem *pUserItem = GetClientUserItem(m_wMeChairID);
		tagUserInfo *pMeUserData = NULL;
		if(pUserItem!=NULL)
			pMeUserData = pUserItem->GetUserInfo();

		if(pMeUserData==NULL) return;

		//������Ŀ
		int iNumberCount=0;
		int nNUmBerScore = 0;
		LONG iNumberTemp=pMeUserData->lScore;

		do
		{
			iNumberCount++;
			iNumberTemp/=10;
		} while (iNumberTemp>0);
		
		LONG lUserScore = pMeUserData->lScore;
		//λ�ö���
		INT nXDrawPos=nWidth/2 + 120;
		INT nYDrawPos=nHeight/2 - 330;
		


		//�滭����
		for (LONG i=0;i<iNumberCount;i++)
		{
			//�滭����
			int iCellNumber=(int)(lUserScore%10);

			m_ImageBetNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nImageWidth,nImageHeight,
										iCellNumber*nImageWidth,0);

			//���ñ���
			lUserScore/=10;
			nXDrawPos-=nImageWidth;
		}
	}
}

//���Ƶ�ǰ�ɼ�
void CGameClientView::DrawCurrentScore(CDC *pDC, int nWidth, int nHeight)
{
	int nImageWidth = m_ImageBetNumber.GetWidth()/10;
	int nImageHeight = m_ImageBetNumber.GetHeight();
	//������Ŀ
	int iNumberCount=0;
	int nNUmBerScore = 0;
	int iNumberTemp=m_iMeCurGameScore;
	int lUserScore = m_iMeCurGameScore;

	if(m_iMeCurGameScore <= 0 )
	{
		iNumberTemp = 0;
		lUserScore = 0;
	}
	else
	{
		do
		{
			iNumberCount++;
			iNumberTemp/=10;
		} while (iNumberTemp>0);
	}
	

	
	//λ�ö���
	INT nXDrawPos=nWidth/2 - 115;
	INT nYDrawPos=nHeight/2 - 330;

	if( m_iMeCurGameScore <= 0 )
	{
		m_ImageBetNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nImageWidth,nImageHeight,
			lUserScore,0);
	}
	else
	{
		//�滭����
		for (LONG i=0;i<iNumberCount;i++)
		{
			//�滭����
			int iCellNumber=(int)(lUserScore%10);

			m_ImageBetNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nImageWidth,nImageHeight,
				iCellNumber*nImageWidth,0);

			//���ñ���
			lUserScore/=10;
			nXDrawPos-=nImageWidth;
		}
	}
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

	for(LONG l=lpRect->top; l<lpRect->bottom; l++)
	{
		for(LONG k=lpRect->left; k<lpRect->right; k++)
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

//�ɼ�����
void CGameClientView::SetGameScore(__int64 iMeCurGameScore, __int64 iMeCurGameReturnScore, __int64 iBankerCurGameScore)
{
	m_iMeCurGameScore=iMeCurGameScore;
	m_iMeCurGameReturnScore=iMeCurGameReturnScore;
	m_iBankerCurGameScore=iBankerCurGameScore;	
}

//�滭����
void CGameClientView::DrawMeJettonNumber(CDC *pDC)
{
	//�滭����
	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		if( m_iMeAreaScore[i] > 0 )
		{
			DrawNumberString(pDC, m_iMeAreaScore[i], m_PointJettonNumber[i].x, m_PointJettonNumber[i].y-45, true);
		}
	}

}

//���ƶ���
void CGameClientView::DrawCardCartoon(CDC * pDC)
{
	BYTE cbGameStatus = m_pGameClientDlg->GetGameStatus();

	if( m_nCurCartoonIndex<0 || m_nCurCartoonIndex>=24 )//32 )
		return;

	int nImageWidth = m_ImageCardCartoon.GetWidth();
	//int nImageWidth = m_ImageCardCartoon.GetWidth()/JETTON_AREA_COUNT;
	int nImageHeight = m_ImageCardCartoon.GetHeight();
	m_ImageCardCartoon.DrawImage(pDC, m_PtCardArray[m_nCurCartoonIndex].x-nImageWidth/2, 
		m_PtCardArray[m_nCurCartoonIndex].y-nImageHeight/2, nImageWidth, nImageHeight, 
		0, 0);
	//m_ImageCardCartoon.DrawImage(pDC, m_PtCardArray[m_nCurCartoonIndex].x-nImageWidth/2, 
	//	m_PtCardArray[m_nCurCartoonIndex].y-nImageHeight/2, nImageWidth, nImageHeight, 
	//	(m_nCurCartoonIndex%JETTON_AREA_COUNT )*nImageWidth, 0);
}

//���ƽ�������
void CGameClientView::DrawEndCartoon(CDC * pDC,int nWidth,int nHeight,BYTE bGoalAnimal)
{
	CString str;
	m_bEndCount++;
	if( m_bEndCount > 3 )
	{
		m_bEndCount = 1;
	}
	int nImageWidth = 0;//m_ImageEnd_1.GetWidth()/3;
	int nImageHeight = 0;//m_ImageEnd_1.GetHeight();
	switch(bGoalAnimal)
	{
		case 0:
		case 12:
			{
				nImageWidth = m_IamgeOrange.GetWidth()/3;
				nImageHeight = m_IamgeOrange.GetHeight();
				m_IamgeOrange.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth *  m_bEndCount  ),0);
				break;
			}
		case 1:
		case 13:
			{
				nImageWidth = m_IamgeBell.GetWidth()/3;
				nImageHeight = m_IamgeBell.GetHeight();
				m_IamgeBell.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		case 2:
			{
				nImageWidth = m_IamgeBar2.GetWidth()/3;
				nImageHeight = m_IamgeBar2.GetHeight();
				m_IamgeBar2.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		case 3:
			{
				nImageWidth = m_IamgeBar.GetWidth()/3;
				nImageHeight = m_IamgeBar.GetHeight();
				m_IamgeBar.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		case 4:
		case 10:
		case 16:
		case 22:
			{
				nImageWidth = m_ImageApple.GetWidth()/3;
				nImageHeight = m_ImageApple.GetHeight();
				m_ImageApple.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		case 5:
			{
				nImageWidth = m_ImageApple2.GetWidth()/3;
				nImageHeight = m_ImageApple2.GetHeight();
				m_ImageApple2.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		case 6:
		case 18:
			{
				nImageWidth = m_IamgePaw.GetWidth()/3;
				nImageHeight = m_IamgePaw.GetHeight();
				m_IamgePaw.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		case 7:
			{
				nImageWidth = m_ImageWaterMelon.GetWidth()/3;
				nImageHeight = m_ImageWaterMelon.GetHeight();
				m_ImageWaterMelon.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		case 8:
			{
				nImageWidth = m_ImageWaterMelon2.GetWidth()/3;
				nImageHeight = m_ImageWaterMelon2.GetHeight();
				m_ImageWaterMelon2.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		case 9:
			{
				nImageWidth = m_ImageApple.GetWidth()/3;
				nImageHeight = m_ImageApple.GetHeight();
				m_ImageApple.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		case 11:
			{
				nImageWidth = m_IamgeOrange2.GetWidth()/3;
				nImageHeight = m_IamgeOrange2.GetHeight();
				m_IamgeOrange2.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		case 14:
			{
				nImageWidth = m_IamgeSeven2.GetWidth()/3;
				nImageHeight = m_IamgeSeven2.GetHeight();
				m_IamgeSeven2.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		case 15:
			{
				nImageWidth = m_IamgeSeven.GetWidth()/3;
				nImageHeight = m_IamgeSeven.GetHeight();
				m_IamgeSeven.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		case 17:
			{
				nImageWidth = m_IamgePaw2.GetWidth()/3;
				nImageHeight = m_IamgePaw2.GetHeight();
				m_IamgePaw2.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		case 19:
			{
				nImageWidth = m_IamgeStar.GetWidth()/3;
				nImageHeight = m_IamgeStar.GetHeight();
				m_IamgeStar.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		case 20:
			{
				nImageWidth = m_IamgeStar2.GetWidth()/3;
				nImageHeight = m_IamgeStar2.GetHeight();
				m_IamgeStar2.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		case 21:
			{
				nImageWidth = m_ImageApple.GetWidth()/3;
				nImageHeight = m_ImageApple.GetHeight();
				m_ImageApple.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		case 23:
			{
				nImageWidth = m_IamgeBell2.GetWidth()/3;
				nImageHeight = m_IamgeBell2.GetHeight();
				m_IamgeBell2.DrawImage(pDC,nWidth/2 - nImageWidth/2,nHeight/2 - 100,
					nImageWidth, nImageHeight,
					(nImageWidth * m_bEndCount  ),0);
				break;
			}
		default:
			{
				break;
			}
	}
	
	
}
//ʤ���߿�
void CGameClientView::FlashJettonAreaFrame(int nWidth, int nHeight, CDC *pDC)
{
	if (m_cbAreaFlash==0xFF) 
			return;

	//λ�ñ���
	int nXPos = 0;
	int nYPos = 0;

	//�����ж�
	if (m_pGameClientDlg->GetGameStatus()==GS_GAME_END )
	{
		//�����н�����
		if( m_cbAreaFlash != 0xFF)
		{
			DrawEndCartoon(pDC,nWidth,nHeight,m_cbGoalAnimal);
			//m_ImageWinFrame.AlphaDrawImage(pDC, m_rcJettonArea[m_cbAreaFlash].left, m_rcJettonArea[m_cbAreaFlash].top, RGB(255, 0, 255));
		}

		if( m_nCurCartoonIndex != 0xFF)
		{
			//������תֹͣ����
			//int nImageWidth = m_ImageCardCartoon.GetWidth()/JETTON_AREA_COUNT;
			int nImageWidth = m_ImageCardCartoon.GetWidth();
			int nImageHeight = m_ImageCardCartoon.GetHeight();
			m_ImageCardCartoon.DrawImage(pDC, m_PtCardArray[m_nCurCartoonIndex].x-nImageWidth/2, 
				m_PtCardArray[m_nCurCartoonIndex].y-nImageHeight/2, nImageWidth, nImageHeight, 
				0, 0);
			//m_ImageCardCartoon.DrawImage(pDC, m_PtCardArray[m_nCurCartoonIndex].x-nImageWidth/2, 
			//	m_PtCardArray[m_nCurCartoonIndex].y-nImageHeight/2, nImageWidth, nImageHeight, 
			//	(m_nCurCartoonIndex%JETTON_AREA_COUNT)*nImageWidth, 0);
		}

	}
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

	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		if( m_bBombEffect[i] )
		{
			m_ImageBombEffect.DrawImage(pDC, m_PointJettonNumber[i].x-nImageWidth/2, m_PointJettonNumber[i].y-10, nImageWidth, nImageHeight,
				nImageWidth * (m_cbBombFrameIndex[i]%BOMB_EFFECT_COUNT), 0);
		}
	}
}

//������ׯ�������
void CGameClientView::DrawApplyBankerUserList(CDC * pDC)
{
	m_ApplyUser.ShowApplayBankerUser(pDC, m_rcApplayBankerNo, m_rcApplayBankerName, m_rcApplayBankerScore);
}

void CGameClientView::DrawHistoryRecord(CDC * pDC)
{
	m_GameRecord.ShowHistory(pDC, m_ImageHistoryRecord, m_rcHistoryRecord, MAX_SHOW_HISTORY);
}

//�滭ʱ��
void CGameClientView::DrawUserTimer(CDC * pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
	//��ȡ����
	const INT nNumberHeight=m_ImageBetBigNumber.GetHeight();
	const INT nNumberWidth=m_ImageBetBigNumber.GetWidth()/10;

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
	INT nXDrawPos=nXPos-nNumberWidth;
	//INT nXDrawPos=nXPos+(lNumberCount*nNumberWidth)/2-nNumberWidth;

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		WORD wCellNumber=wTime%10;
		m_ImageBetBigNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

		//���ñ���
		wTime/=10;
		nXDrawPos-=nNumberWidth;
	};

	return;
}
//////////////////////////////////////////////////////////////////////////

