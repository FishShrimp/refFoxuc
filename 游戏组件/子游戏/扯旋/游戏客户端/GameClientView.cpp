#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientEngine.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��ʱ����ʶ
#define IDI_USER_ACTION					101								//������ʶ
#define IDI_CARTOON_ACTION				99								//���Ʊ�ʶ

//�ƶ��ٶ�
#define	SEND_PELS						70								//�����ٶ�	


//��ʱ��ʱ��
#ifdef _DEBUG
#define TIME_CARTOON_ACTION				50								//������ʱ��
#define TIME_USER_ACTION				2200							//������ʶ
#define DELAY_OPEN_TIME					1000							//�ӳٿ���
#else 
#define TIME_CARTOON_ACTION				50								//������ʱ��
#define TIME_USER_ACTION				2200							//������ʶ
#define DELAY_OPEN_TIME					1000							//�ӳٿ���
#endif
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)							  
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��Ϸ����
	m_bWaitCompareHeap=false;
	m_bWaitCompareTail=false;
	m_bWaitInvest=false;				
	m_bWaitOpenCard=false;			
	m_bHitPositively=false;			
	ZeroMemory(m_bUserOperate,sizeof(m_bUserOperate));
	ZeroMemory(m_bUserOffLine,sizeof(m_bUserOffLine));
	m_wBankerUser=INVALID_CHAIR;				
	ZeroMemory(m_cbHitCardData,sizeof(m_cbHitCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	m_lCellScore=0l;	
	m_lCenterScore=0l;				
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_szHeapCard,sizeof(m_szHeapCard));
	ZeroMemory(m_szTailCard,sizeof(m_szTailCard));

	//��������
	m_wActionUser=INVALID_CHAIR;	
	m_wUserAction=0;

	//��������
	m_wSendCount=0;				
	m_wSendIndex=0;					
	m_SendCardItemArray.RemoveAll();		
	m_bJettonAction=false;		
	ZeroMemory(m_JettonStatus,sizeof(m_JettonStatus));	
	m_bFlashCard=false;		
	m_wDelayCount=0;		
	m_wDelayIndex=0;		
	m_wHeapMaxUser=INVALID_CHAIR;
	m_wCurrentOpenUser=INVALID_CHAIR;

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageCard.LoadFromResource(hInstance,IDB_CARD);
	m_ImageViewBack.LoadFromResource(hInstance,IDB_VIEW_BACK);
	m_ImageBanker.LoadFromResource(hInstance,IDB_BANKER_FLAG);
	m_ImageControlBack.LoadFromResource(hInstance,IDB_CONTROL_BACK);

	m_nXFace=32;
	m_nYFace=32;

	m_nXBorder=0;
	m_nYBorder=0;

	m_nYTimer=48;
	m_nXTimer=48;

	return;
}

//��������
CGameClientView::~CGameClientView()
{
}

//��Ϣ����
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	m_ToolTipCtrl.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,10);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,rcCreate,this,		IDC_START);
	m_btInvest.Create(TEXT(""),WS_CHILD,rcCreate,this,		IDC_INVEST);					
	m_btAddScore.Create(TEXT(""),WS_CHILD,rcCreate,this,	IDC_ADD_SCORE);				
	m_btFollow.Create(TEXT(""),WS_CHILD,rcCreate,this,		IDC_FOLLOW);					
	m_btShowHand.Create(TEXT(""),WS_CHILD,rcCreate,this,	IDC_SHOW_HAND);				
	m_btPass.Create(TEXT(""),WS_CHILD,rcCreate,this,		IDC_PASS);					
	m_btGiveUp.Create(TEXT(""),WS_CHILD,rcCreate,this,		IDC_GIVE_UP);					
	m_btClearScore.Create(TEXT(""),WS_CHILD,rcCreate,this,	IDC_CLEAR_SCORE);				
	m_btOpenCard.Create(TEXT(""),WS_CHILD,rcCreate,this,	IDC_OPEN_CARD);				
	m_btDecrease[0].Create(TEXT(""),WS_CHILD,rcCreate,this,	IDC_DECREASE1);	
	m_btDecrease[1].Create(TEXT(""),WS_CHILD,rcCreate,this,	IDC_DECREASE2);	
	m_btDecrease[2].Create(TEXT(""),WS_CHILD,rcCreate,this,	IDC_DECREASE3);	
	m_btDecrease[3].Create(TEXT(""),WS_CHILD,rcCreate,this,	IDC_DECREASE4);	

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_BT_START,hInstance,false,false);
	m_btInvest.SetButtonImage(IDB_BT_INVEST,hInstance,false,false);
	m_btAddScore.SetButtonImage(IDB_BT_ADD,hInstance,false,false);
	m_btFollow.SetButtonImage(IDB_BT_FOLLOW,hInstance,false,false);
	m_btShowHand.SetButtonImage(IDB_BT_SHOWHAND,hInstance,false,false);
	m_btPass.SetButtonImage(IDB_BT_PASS,hInstance,false,false);
	m_btGiveUp.SetButtonImage(IDB_BT_GIVEUP,hInstance,false,false);
	m_btClearScore.SetButtonImage(IDB_BT_CLEAR_JETTON,hInstance,false,false);
	m_btOpenCard.SetButtonImage(IDB_BT_OpenCard,hInstance,false,false);
	m_btDecrease[0].SetButtonImage(IDB_BT_DECREASE_JETTON,hInstance,false,false);
	m_btDecrease[1].SetButtonImage(IDB_BT_DECREASE_JETTON,hInstance,false,false);
	m_btDecrease[2].SetButtonImage(IDB_BT_DECREASE_JETTON,hInstance,false,false);
	m_btDecrease[3].SetButtonImage(IDB_BT_DECREASE_JETTON,hInstance,false,false);

	//�����ؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_CardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,20+i);
	}

	//���ÿؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)m_UserGoldView[i].SetBeelinePut(true);
	m_JetonControl.SetButton(m_btDecrease,&m_btClearScore,&m_btAddScore);

    //��ť��ʾ
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btAddScore,TEXT("ȷ������"));
	m_ToolTipCtrl.AddTool(&m_btFollow,TEXT("����"));
	m_ToolTipCtrl.AddTool(&m_btPass,TEXT("����"));
	m_ToolTipCtrl.AddTool(&m_btShowHand,TEXT("ȫѹ"));
	m_ToolTipCtrl.AddTool(&m_btGiveUp,TEXT("����"));
	m_ToolTipCtrl.AddTool(&m_btInvest,TEXT("ȷ������"));
	m_ToolTipCtrl.AddTool(&m_btDecrease[0],TEXT("����"));
	m_ToolTipCtrl.AddTool(&m_btDecrease[1],TEXT("����"));
	m_ToolTipCtrl.AddTool(&m_btDecrease[2],TEXT("����"));
	m_ToolTipCtrl.AddTool(&m_btDecrease[3],TEXT("����"));

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
	case IDC_INVEST:			//�±���ť
		{
			//������Ϣ
			SendEngineMessage(IDM_ADD_SCORE,IDC_INVEST,0);
			return TRUE;
		}
	case IDC_PASS:				//���Ӱ�ť
		{
			//������Ϣ
			SendEngineMessage(IDM_ADD_SCORE,IDC_PASS,0);
			return TRUE;
		}
	case IDC_ADD_SCORE:			//��ע��ť
		{
			//������Ϣ
			SendEngineMessage(IDM_ADD_SCORE,IDC_ADD_SCORE,0);
			return TRUE;
		}
	case IDC_FOLLOW:			//��ע��ť
		{
			//������Ϣ
			SendEngineMessage(IDM_ADD_SCORE,IDC_FOLLOW,0);
			return TRUE;
		}
	case IDC_SHOW_HAND:			//ȫѹ��ť
		{
			//������Ϣ
			SendEngineMessage(IDM_ADD_SCORE,IDC_SHOW_HAND,0);
			return TRUE;
		}
	case IDC_GIVE_UP:			//������ť
		{
			//������Ϣ
			SendEngineMessage(IDM_ADD_SCORE,IDC_GIVE_UP,0);
			return TRUE;
		}
	case IDC_CLEAR_SCORE:		//���㰴ť
		{
			//������Ϣ
			SendEngineMessage(IDM_CLEAR_SCORE,0,0);
			return TRUE;
		}
	case IDC_OPEN_CARD:			//���ư�ť
		{
			//������Ϣ
			SendEngineMessage(IDM_OPEN_CARD,0,0);
			return TRUE;
		}
	case IDC_DECREASE1:			//��ע��ť
		{
			//������Ϣ
			SendEngineMessage(IDM_DECREASE,1,1);
			return TRUE;
		}
	case IDC_DECREASE2:			//��ע��ť
		{
			//������Ϣ
			SendEngineMessage(IDM_DECREASE,2,2);
			return TRUE;
		}
	case IDC_DECREASE3:			//��ע��ť
		{
			//������Ϣ
			SendEngineMessage(IDM_DECREASE,3,3);
			return TRUE;
		}
	case IDC_DECREASE4:			//��ע��ť
		{
			//������Ϣ
			SendEngineMessage(IDM_DECREASE,4,4);
			return TRUE;
		}
	}

	return __super::OnCommand(wParam, lParam);
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��Ϸ����
	m_bWaitCompareHeap=false;
	m_bWaitCompareTail=false;
	m_bWaitInvest=false;				
	m_bWaitOpenCard=false;			
	m_bHitPositively=false;			
	ZeroMemory(m_bUserOperate,sizeof(m_bUserOperate));
	ZeroMemory(m_bUserOffLine,sizeof(m_bUserOffLine));
	m_wBankerUser=INVALID_CHAIR;				
	ZeroMemory(m_cbHitCardData,sizeof(m_cbHitCardData));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	m_lCellScore=0l;	
	m_lCenterScore=0l;				
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_szHeapCard,sizeof(m_szHeapCard));
	ZeroMemory(m_szTailCard,sizeof(m_szTailCard));

	//��������
	m_wActionUser=INVALID_CHAIR;	
	m_wUserAction=0;

	//��������
	m_wSendCount=0;				
	m_wSendIndex=0;					
	m_SendCardItemArray.RemoveAll();		
	m_bJettonAction=false;		
	ZeroMemory(m_JettonStatus,sizeof(m_JettonStatus));	
	m_bFlashCard=false;		
	m_wDelayCount=0;		
	m_wDelayIndex=0;		
	m_wHeapMaxUser=INVALID_CHAIR;
	m_wCurrentOpenUser=INVALID_CHAIR;

	//ɾ����ʱ��
	KillTimer(IDI_CARTOON_ACTION);
	KillTimer(IDI_USER_ACTION);

	//���ذ�ť
	m_btStart.ShowWindow(SW_HIDE); 
	m_btInvest.ShowWindow(SW_HIDE);
	m_btAddScore.ShowWindow(SW_HIDE);
	m_btFollow.ShowWindow(SW_HIDE);
	m_btShowHand.ShowWindow(SW_HIDE);
	m_btPass.ShowWindow(SW_HIDE);
	m_btGiveUp.ShowWindow(SW_HIDE);
	m_btClearScore.ShowWindow(SW_HIDE);	
	m_btOpenCard.ShowWindow(SW_HIDE);	
	for(WORD i=0;i<CONTROL_COUNT;i++)
		m_btDecrease[i].ShowWindow(SW_HIDE);

	//���ؿؼ�
	m_ScoreView.ShowWindow(SW_HIDE);
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_CardControl[i].SetDisplayHead(false);
		m_CardControl[i].SetCardData(NULL,0);
		m_MoveGoldView[i].SetGold(0l);
		m_AddGoldView[i].SetGold(0l);
		m_UserGoldView[i].SetGold(0l);
	}
	m_CenterGoldView.SetGold(0l);

	return;
}

//�����ؼ�
void CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//CImageHandle ImageHandleBanker(&m_ImageBanker);

	//��������
	m_ptAvatar[4].x=nWidth/2+343;
	m_ptAvatar[4].y=nHeight/2-201;
	m_ptAvatar[3].x=nWidth/2+343;
	m_ptAvatar[3].y=nHeight/2;
	m_ptAvatar[2].x=nWidth/2-m_nXFace;
	m_ptAvatar[2].y=nHeight/2+234;
	m_ptAvatar[1].x=nWidth/2-375;
	m_ptAvatar[1].y=nHeight/2;
	m_ptAvatar[0].x=nWidth/2-375;
	m_ptAvatar[0].y=nHeight/2-201;

	//����λ��
	m_ptNickName[4].x=nWidth/2+375;
	m_ptNickName[4].y=nHeight/2-201+m_nYFace+20;
	m_ptNickName[3].x=nWidth/2+375;
	m_ptNickName[3].y=nHeight/2+m_nYFace+20;
	m_ptNickName[2].x=nWidth/2+22;
	m_ptNickName[2].y=nHeight/2+245;
	m_ptNickName[1].x=nWidth/2-375;
	m_ptNickName[1].y=nHeight/2+m_nYFace+20;
	m_ptNickName[0].x=nWidth/2-375;
	m_ptNickName[0].y=nHeight/2-201+m_nYFace+20;

	//ʱ��λ��
	m_ptClock[4].x=nWidth/2+165;
	m_ptClock[4].y=nHeight/2-223;
	m_ptClock[3].x=nWidth/2+165;
	m_ptClock[3].y=nHeight/2-15;
	m_ptClock[2].x=nWidth/2-60;
	m_ptClock[2].y=nHeight/2+85;
	m_ptClock[1].x=nWidth/2-160;
	m_ptClock[1].y=nHeight/2-15;
	m_ptClock[0].x=nWidth/2-160;
	m_ptClock[0].y=nHeight/2-223;

	//׼��λ��
	m_ptReady[4].x=nWidth/2+140;
	m_ptReady[4].y=nHeight/2-160;
	m_ptReady[3].x=nWidth/2+140;
	m_ptReady[3].y=nHeight/2-15;
	m_ptReady[2].x=nWidth/2+10;
	m_ptReady[2].y=nHeight/2+46;
	m_ptReady[1].x=nWidth/2-135;
	m_ptReady[1].y=nHeight/2-15;
	m_ptReady[0].x=nWidth/2-135;
	m_ptReady[0].y=nHeight/2-160;

	//ׯ�ұ�־
	m_ptBanker[4].x=m_ptAvatar[4].x-5;
	m_ptBanker[4].y=m_ptAvatar[4].y-m_nYFace-10;
	m_ptBanker[3].x=m_ptAvatar[3].x-5;
	m_ptBanker[3].y=m_ptAvatar[3].y-m_nYFace-10;
	m_ptBanker[2].x=m_ptAvatar[2].x-m_nXFace-10;
	m_ptBanker[2].y=m_ptAvatar[2].y-10;
	m_ptBanker[1].x=m_ptAvatar[1].x;
	m_ptBanker[1].y=m_ptAvatar[1].y-m_nYFace-10;
	m_ptBanker[0].x=m_ptAvatar[0].x;
	m_ptBanker[0].y=m_ptAvatar[0].y-m_nYFace-10;

	//�û��˿�
	m_CardControl[4].SetBenchmarkPos(nWidth/2+245,nHeight/2-165,enXCenter,enYCenter);
	m_CardControl[3].SetBenchmarkPos(nWidth/2+245,nHeight/2+20,enXCenter,enYCenter);
	m_CardControl[2].SetBenchmarkPos(nWidth/2-60,nHeight/2+221,enXCenter,enYBottom);
	m_CardControl[1].SetBenchmarkPos(nWidth/2-240,nHeight/2+20,enXCenter,enYCenter);
	m_CardControl[0].SetBenchmarkPos(nWidth/2-240,nHeight/2-165,enXCenter,enYCenter);

	//����λ��
	m_SendEndingPos[4].x=m_CardControl[4].GetCardPos(false).x;
	m_SendEndingPos[4].y=m_CardControl[4].GetCardPos(false).y;
	m_SendEndingPos[3].x=m_CardControl[3].GetCardPos(false).x;
	m_SendEndingPos[3].y=m_CardControl[3].GetCardPos(false).y;
	m_SendEndingPos[2].x=m_CardControl[2].GetCardPos(false).x;
	m_SendEndingPos[2].y=m_CardControl[2].GetCardPos(false).y;
	m_SendEndingPos[1].x=m_CardControl[1].GetCardPos(false).x;
	m_SendEndingPos[1].y=m_CardControl[1].GetCardPos(false).y;
	m_SendEndingPos[0].x=m_CardControl[0].GetCardPos(false).x;
	m_SendEndingPos[0].y=m_CardControl[0].GetCardPos(false).y;

	//����ؼ�
	CPoint ptCotrol,ptAdd;
	ptCotrol.x=nWidth/2+35;
	ptCotrol.y=nHeight/2+189;
	ptAdd.x=nWidth/2+15;
	ptAdd.y=nHeight/2+101;
	m_JetonControl.SetControlPost(ptCotrol,ptAdd);
	m_ptMyJetton.x=ptAdd.x+50;
	m_ptMyJetton.y=ptAdd.y-5;

	//�ؼ�����
	m_ptControlBack.SetPoint(nWidth/2+100,nHeight/2+240);

	//��ť�ؼ�
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//��ʼ��ť
	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,nWidth/2-31,nHeight/2+271,0,0,uFlags);

	//���ư�ť
	m_btAddScore.GetWindowRect(&rcButton);
	INT nXTemp = m_ptControlBack.x+12;
	INT nYTemp = m_ptControlBack.y+14;
	DeferWindowPos(hDwp,m_btAddScore,NULL,nXTemp,nYTemp,0,0,uFlags);
	DeferWindowPos(hDwp,m_btFollow,NULL,nXTemp+(rcButton.Width()+2),nYTemp,0,0,uFlags);
	DeferWindowPos(hDwp,m_btShowHand,NULL,nXTemp+(rcButton.Width()+2)*2,nYTemp,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPass,NULL,nXTemp+(rcButton.Width()+2)*3,nYTemp,0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUp,NULL,nXTemp+(rcButton.Width()+2)*4,nYTemp,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOpenCard,NULL,nWidth/2+130,nHeight/2+160,0,0,uFlags);

	m_btDecrease[0].GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btDecrease[3],NULL,ptCotrol.x-1,ptCotrol.y+35,0,0,uFlags);
	DeferWindowPos(hDwp,m_btDecrease[2],NULL,ptCotrol.x+rcButton.Width(),ptCotrol.y+35,0,0,uFlags);
	DeferWindowPos(hDwp,m_btDecrease[1],NULL,ptCotrol.x+rcButton.Width()*2+1,ptCotrol.y+35,0,0,uFlags);
	DeferWindowPos(hDwp,m_btDecrease[0],NULL,ptCotrol.x+rcButton.Width()*3+2,ptCotrol.y+35,0,0,uFlags);
	DeferWindowPos(hDwp,m_btClearScore,NULL,ptCotrol.x+rcButton.Width()*4+5,ptCotrol.y+26,0,0,uFlags);
	DeferWindowPos(hDwp,m_btInvest,NULL,ptCotrol.x+rcButton.Width()*6-20,ptCotrol.y+26,0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	//������ͼ
	CRect rcControl;
	m_ScoreView.GetWindowRect(&rcControl);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcControl.Width())/2,nHeight/2-190,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//��������
	m_ptOperateBack[0].x=nWidth/2-336;
	m_ptOperateBack[0].y=nHeight/2-258;
	m_ptOperateBack[1].x=nWidth/2-336;
	m_ptOperateBack[1].y=nHeight/2-37;
	m_ptOperateBack[2].x=nWidth/2-212;
	m_ptOperateBack[2].y=nHeight/2+62;
	m_ptOperateBack[3].x=nWidth/2+117;
	m_ptOperateBack[3].y=nHeight/2-37;
	m_ptOperateBack[4].x=nWidth/2+164;
	m_ptOperateBack[4].y=nHeight/2-262;

	//���뱳��
	m_ptJettonBack[0].x=nWidth/2-155;
	m_ptJettonBack[0].y=nHeight/2-165;
	m_ptJettonBack[1].x=nWidth/2-136;
	m_ptJettonBack[1].y=nHeight/2-65;
	m_ptJettonBack[2].x=nWidth/2-40;
	m_ptJettonBack[2].y=nHeight/2-20;
	m_ptJettonBack[3].x=nWidth/2+58;
	m_ptJettonBack[3].y=nHeight/2-65;
	m_ptJettonBack[4].x=nWidth/2+80;
	m_ptJettonBack[4].y=nHeight/2-165;

	//����λ��
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		m_ptAddJetton[i].x=m_ptJettonBack[i].x+27;
		m_ptAddJetton[i].y=m_ptJettonBack[i].y+18;
	}

	//����λ��
	m_SendStartPos.SetPoint(nWidth/2+185,nHeight/2-280);

	//����λ��
	m_ptUserAction[0].x=nWidth/2-315;
	m_ptUserAction[0].y=nHeight/2-253;
	m_ptUserAction[1].x=nWidth/2-315;
	m_ptUserAction[1].y=nHeight/2-53;
	m_ptUserAction[2].x=nWidth/2-55;
	m_ptUserAction[2].y=nHeight/2+76;
	m_ptUserAction[3].x=nWidth/2+300;
	m_ptUserAction[3].y=nHeight/2-53;
	m_ptUserAction[4].x=nWidth/2+300;
	m_ptUserAction[4].y=nHeight/2-253;

	//�û�����
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(i!=MY_VIEW_CHAIR_ID)
		{
			m_ptUserJetton[i].x=m_CardControl[i].GetCardPos().x+40;
			m_ptUserJetton[i].y=m_CardControl[i].GetCardPos().y+108;
		}
		else
		{
			m_ptUserJetton[i].x=m_CardControl[i].GetCardPos().x+170;
			m_ptUserJetton[i].y=m_CardControl[i].GetCardPos().y+80;
		}
	}
	m_ptUserJetton[2]=m_ptMyJetton;

	//����λ��
	m_ptCenterJetton.SetPoint(nWidth/2-15,nHeight/2-100);

	//���λ��
	CPoint CardPost= m_CardControl[MY_VIEW_CHAIR_ID].GetCardPos();
	for(BYTE i=0;i<4;i++)
	{
		m_ptHitCard[i].x=CardPost.x+i*DEFAULT_PELS;
		m_ptHitCard[i].y=CardPost.y;
	}
	m_ptHitCard[4].x = m_ptHitCard[3].x+100;
	m_ptHitCard[4].y = m_ptHitCard[0].y;
	m_ptHitCard[5].x = m_ptHitCard[4].x+DEFAULT_PELS;
	m_ptHitCard[5].y = m_ptHitCard[0].y;

	//��������
	//SetFlowerControlInfo(0,m_ptAvatar[0].x,m_ptAvatar[0].y);
	//SetFlowerControlInfo(1,m_ptAvatar[1].x,m_ptAvatar[1].y);
	//SetFlowerControlInfo(2,m_ptAvatar[2].x-BIG_FACE_WIDTH,m_ptAvatar[2].y-40);
	//SetFlowerControlInfo(3,m_ptAvatar[3].x-BIG_FACE_WIDTH/2,m_ptAvatar[3].y);
	//SetFlowerControlInfo(4,m_ptAvatar[4].x-BIG_FACE_WIDTH/2,m_ptAvatar[4].y);
	return;
}

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//��������
	//CImageHandle HandleBack(&m_ImageViewBack);
	//CImageHandle HandleControlBack(&m_ImageControlBack);

	//�滭����
	m_ImageViewBack.TransDrawImage(pDC,nWidth/2-m_ImageViewBack.GetWidth()/2+5,
		nHeight/2-m_ImageViewBack.GetHeight()/2+5,RGB(255,0,255));

	//DrawViewImage(pDC,m_ImageViewBack,enMode_Centent);

	//��������
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(!m_bUserOperate[i])continue;

		TCHAR tcInfo[32]=TEXT("");
		_sntprintf(tcInfo,sizeof(tcInfo)/sizeof(tcInfo[0]),TEXT("USER_%d"),i+1);

		//������Դ
		CPngImage ImageOperate;
		ImageOperate.LoadImage(AfxGetInstanceHandle(),tcInfo);
		ImageOperate.DrawImage(pDC,m_ptOperateBack[i].x,m_ptOperateBack[i].y);
	}

	//���뱳��
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_lTableScore[i]>0)
		{
			CPngImage BackPng;
			BackPng.LoadImage(AfxGetInstanceHandle(),TEXT("ADD_BACK"));
			BackPng.DrawImage(pDC,m_ptJettonBack[i].x,m_ptJettonBack[i].y);
		}
	}

	//�滭����
	m_JetonControl.DrawJettonControl(pDC);

	//�滭�û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wUserTimer=GetUserClock(i);
		IClientUserItem * pUserData=GetClientUserItem(i);

#ifdef _DEBUG
		////���Դ���
		//tagUserData Obj;
		//Obj.cbUserStatus = US_OFFLINE;
		//Obj.lScore = 999999999;
		//_sntprintf( Obj.szName,sizeof(Obj.szName),TEXT("�û�������%d"),i);
		//wUserTimer = i+1;
		//pUserData = &Obj;
		////ׯ�ұ�־
		////CImageHandle ImageHandleBanker(&m_ImageBanker);
		//m_ImageBanker.TransDrawImage(pDC,m_ptBanker[i].x,m_ptBanker[i].y,m_ImageBanker.GetWidth(),m_ImageBanker.GetHeight(),0,0,RGB(255,0,255));
#endif

		//�滭�û�
		if (pUserData!=NULL)
		{
			//�û�����
			pDC->SetTextAlign((i>=3)?TA_RIGHT:TA_LEFT);
			DrawTextString(pDC,pUserData->GetNickName(),RGB(255,255,255),RGB(0,0,0),m_ptNickName[i].x,m_ptNickName[i].y);
			
			//ʱ����Ϣ
			if (wUserTimer!=0)
			{
				if(m_bWaitInvest||m_bWaitOpenCard)
				{
					DrawUserClock(pDC,nWidth/2,nHeight/2+25,wUserTimer);
				}
				else if(i==MY_VIEW_CHAIR_ID)
				{
					if(m_CardControl[i].GetCardCount()>0)
					{
						DrawUserClock(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer);
					}
					else DrawUserClock(pDC,m_ptAvatar[i].x+20,m_ptAvatar[i].y-m_nYFace-5,wUserTimer);
				}
				else DrawUserClock(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer);
			}

			//׼����־
			if (pUserData->GetUserStatus()==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);

			//ͷ���־
			DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pUserData);

			//������ʾ
			if(pUserData->GetUserStatus()==US_OFFLINE)
			{
				DrawTextString(pDC,TEXT("����-������"),RGB(255,0,0),RGB(0,0,0),m_ptNickName[i].x,m_ptNickName[i].y+16);
			}
			if(pUserData->GetUserStatus()==US_OFFLINE && !m_bUserOffLine[i])
			{
				m_bUserOffLine[i]=true;

				CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
				TCHAR szBuffer[512]=TEXT("");
				myprintf(szBuffer,CountArray(szBuffer),TEXT("%s����������,��90����û����,��ǿ�˴���,��������ĵȴ�."),pUserData->GetNickName());
				//pGameClientEngine->InsertCustomString(szBuffer,RGB(255,0,0));
			}
			else if(pUserData->GetUserStatus()!=US_OFFLINE && m_bUserOffLine[i])
			{
				m_bUserOffLine[i]=false;

				CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
				TCHAR szBuffer[512]=TEXT("");
				myprintf(szBuffer,CountArray(szBuffer),TEXT("%s���������."),pUserData->GetNickName());
				//pGameClientEngine->InsertCustomString(szBuffer,RGB(255,0,0));
			}
			//�û�����
			//TCHAR szBuffer[64]=TEXT("");
			//myprintf(szBuffer,sizeof(szBuffer),TEXT("��%I64d"),m_lUserScore[i]);
			//DrawTextString(pDC,szBuffer,RGB(255,255,255),RGB(0,0,0),m_ptNickName[i].x,m_ptNickName[i].y+16);
		}
	}

	//�ȴ���ע
	if (m_bWaitInvest)
	{
		//������Դ
		CFont InfoFont;
		InfoFont.CreateFont(-24,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("����_GB2312"));

		//���� DC
		pDC->SetTextAlign(TA_CENTER|TA_TOP);
		CFont * pOldFont=pDC->SelectObject(&InfoFont);

		//�滭��Ϣ
		TCHAR zsInfo[128]=TEXT("");
		_sntprintf(zsInfo,sizeof(zsInfo),TEXT("�ȴ��������"));
		DrawTextString(pDC,zsInfo,RGB(255,255,56),RGB(0,0,0),nWidth/2+5,nHeight/2-30);

		//������Դ
		pDC->SelectObject(pOldFont);
		InfoFont.DeleteObject();
	}

	//�ȴ�����
	if (m_bWaitOpenCard)
	{
		//������Դ
		CFont InfoFont;
		InfoFont.CreateFont(-24,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("����_GB2312"));

		//���� DC
		pDC->SetTextColor(RGB(250,200,40));
		pDC->SetTextAlign(TA_CENTER|TA_TOP);
		CFont * pOldFont=pDC->SelectObject(&InfoFont);

		//�滭��Ϣ
		TCHAR zsInfo[128]=TEXT("");
		_sntprintf(zsInfo,sizeof(zsInfo),TEXT("�ȴ������ͷβ��"));
		DrawTextString(pDC,zsInfo,RGB(255,255,56),RGB(0,0,0),nWidth/2+5,nHeight/2-30);

		//������Դ
		pDC->SelectObject(pOldFont);
		InfoFont.DeleteObject();
	}

	//�ȴ��Ƚ�
	if(m_bWaitCompareHeap || m_bWaitCompareTail)
	{
		//������Դ
		CFont InfoFont;
		InfoFont.CreateFont(-24,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("����_GB2312"));

		//���� DC
		pDC->SetTextColor(RGB(250,200,40));
		pDC->SetTextAlign(TA_CENTER|TA_TOP);
		CFont * pOldFont=pDC->SelectObject(&InfoFont);

		//�滭��Ϣ
		TCHAR zsInfo[128]=TEXT("");
		if(m_bWaitCompareHeap)_sntprintf(zsInfo,sizeof(zsInfo),TEXT("�ȴ���ұȽ�ͷ��"));
		else _sntprintf(zsInfo,sizeof(zsInfo),TEXT("�ȴ���ұȽ�β��"));
		DrawTextString(pDC,zsInfo,RGB(255,255,56),RGB(0,0,0),nWidth/2+5,nHeight/2-30);

		//������Դ
		pDC->SelectObject(pOldFont);
		InfoFont.DeleteObject();
	}

	//ׯ����Ϣ
	if (m_wBankerUser<GAME_PLAYER)
	{
		IClientUserItem * pUserData=GetClientUserItem(m_wBankerUser);
		INT nXpt = 0,nYpt = 0;
		if (pUserData!=NULL)
		{
			nXpt=m_ptBanker[m_wBankerUser].x;
			nYpt=m_ptBanker[m_wBankerUser].y;
		}
		else
		{
			nXpt=m_ptAvatar[m_wBankerUser].x;
			nYpt=m_ptAvatar[m_wBankerUser].y;
		}
		//ׯ�ұ�־
		//CImageHandle ImageHandleBanker(&m_ImageBanker);
		m_ImageBanker.TransDrawImage(pDC,nXpt,nYpt,RGB(255,0,255));
	}

	//���õ�Ԫ
	if(m_lCellScore>0)
	{
		//������Դ
		CFont InfoFont;
		InfoFont.CreateFont(-18,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("����"));

		//���� DC
		pDC->SetTextAlign(TA_CENTER|TA_TOP);
		CFont * pOldFont=pDC->SelectObject(&InfoFont);

		//�滭��Ϣ
		TCHAR zsInfo[128]=TEXT("");
		_sntprintf(zsInfo,sizeof(zsInfo),TEXT("%I64d"),m_lCellScore);
		DrawTextString(pDC,zsInfo,RGB(255,255,56),RGB(0,0,0),nWidth/2-165,nHeight/2-340);

		//������Դ
		pDC->SelectObject(pOldFont);
		InfoFont.DeleteObject();
	}

	//����˿�
	if(m_bHitPositively)
	{
		//������Դ
		CPngImage BackImage;
		BackImage.LoadImage(AfxGetInstanceHandle(),TEXT("MARKING_CARD"));

		//ͷβ����
		BackImage.DrawImage(pDC,m_ptHitCard[4].x-4,m_ptHitCard[4].y-4);

		//��������
		//CImageHandle HandleCard(&m_ImageCard);

		//��ȡ��С
		int nItemWidth=m_ImageCard.GetWidth()/13;
		int nItemHeight=m_ImageCard.GetHeight()/5;

		//�滭�˿�
		for(BYTE i=0;i<CountArray(m_cbHitCardData);i++)
		{
			BYTE cbCardData=m_cbHitCardData[i];
			if(cbCardData==0)continue;

			if(cbCardData==0x4e)cbCardData=0x42;

			DWORD dwXImagePos=((cbCardData&CARD_MASK_VALUE)-1)*nItemWidth;
			DWORD dwYImagePos=((cbCardData&CARD_MASK_COLOR)>>4)*nItemHeight;

			//�滭�˿�
			int nXPos=m_ptHitCard[i].x;
			int nYPos=m_ptHitCard[i].y;
			m_ImageCard.TransDrawImage(pDC,nXPos,nYPos,nItemWidth,nItemHeight,dwXImagePos,dwYImagePos,RGB(255,0,255));
		}
	}

	//������Ϣ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(GetClientUserItem(i)==NULL && m_UserGoldView[i].GetGold()>0)
		{
			m_lUserScore[i]=0;
			m_UserGoldView[i].SetGold(0);
		}

		//�û�����
		m_UserGoldView[i].DrawGoldView(pDC,m_ptUserJetton[i].x,m_ptUserJetton[i].y,true);

		//�����ƶ�
		m_MoveGoldView[i].DrawGoldView(pDC,m_JettonStatus[i].ptCourse.x,m_JettonStatus[i].ptCourse.y,false);

		//��ע����
		m_AddGoldView[i].DrawGoldView(pDC,m_ptAddJetton[i].x,m_ptAddJetton[i].y,true);
	}

	//���ĳ���
	m_CenterGoldView.DrawGoldView(pDC,m_ptCenterJetton.x,m_ptCenterJetton.y,true,true);

	//for(BYTE i=0;i<GAME_PLAYER;i++)
	//{
	//	m_wActionUser=i;
	//	m_wUserAction=IDC_PASS;
	//�û�����
	if (m_wActionUser!=INVALID_CHAIR)
	{
		//������Ϣ
		BYTE cbAction = 0;
		switch(m_wUserAction)
		{
		case IDC_INVEST:			//�±���Ϣ
			{
				//cbAction=6;
				break;
			}
		case IDC_PASS:				//������Ϣ
			{
				cbAction=2;
				break;
			}
		case IDC_ADD_SCORE:			//��ע��Ϣ
			{
				cbAction=1;
				break;
			}
		case IDC_FOLLOW:			//��ע��Ϣ
			{
				cbAction=3;
				break;
			}
		case IDC_SHOW_HAND:			//ȫѹ��Ϣ
			{
				cbAction=4;
				break;
			}
		case IDC_GIVE_UP:			//������Ϣ
			{
				cbAction=5;
				break;
			}
		case IDC_OPEN_CARD:			//������Ϣ
			{
				//cbAction=7;
				break;
			}
		}

		if(cbAction>0)
		{
			//������Դ
			CPngImage ImageActionBack;
			CPngImage ImageActionFont;
			ImageActionBack.LoadImage(AfxGetInstanceHandle(),TEXT("ACTION_BACK"));
			ImageActionFont.LoadImage(AfxGetInstanceHandle(),TEXT("ACTION_FONT"));

			//����λ��
			INT nXPos=m_ptUserAction[m_wActionUser].x-ImageActionBack.GetWidth()/12;
			INT nYPos=m_ptUserAction[m_wActionUser].y-ImageActionBack.GetHeight()/2;

			//�滭����
			ImageActionBack.DrawImage(pDC,nXPos,nYPos,ImageActionBack.GetWidth()/5,ImageActionBack.GetHeight(),
				m_wActionUser*ImageActionBack.GetWidth()/5,0);

			cbAction-=1;

			//�滭����
			ImageActionFont.DrawImage(pDC,nXPos+5,nYPos+32,ImageActionFont.GetWidth()/7,ImageActionFont.GetHeight(),
				cbAction*ImageActionFont.GetWidth()/7,0);
		}
	}
	//}

	//�û�����
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_szHeapCard[i]!=0 && m_CardControl[i].GetUnDisableCardCount()>=2)
		{
			//������Դ
			CFont InfoFont;
			InfoFont.CreateFont(-13,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("����"));

			//���� DC
			pDC->SetTextAlign(TA_CENTER|TA_TOP);
			CFont * pOldFont=pDC->SelectObject(&InfoFont);

			//�滭��Ϣ
			CPoint ptDraw = m_CardControl[i].GetCardPos();
			ptDraw.y-=16;
			ptDraw.x+=75;
			DrawTextString(pDC,m_szHeapCard[i],RGB(255,255,128),RGB(0,0,0),ptDraw.x,ptDraw.y);

			//������Դ
			pDC->SelectObject(pOldFont);
			InfoFont.DeleteObject();
		}
		if(m_szTailCard[i]!=0 && m_CardControl[i].GetUnDisableCardCount()==	MAX_COUNT)
		{
			//������Դ
			CFont InfoFont;
			InfoFont.CreateFont(-13,0,0,0,700,0,0,0,134,3,2,1,1,TEXT("����"));

			//���� DC
			pDC->SetTextAlign(TA_CENTER|TA_TOP);
			CFont * pOldFont=pDC->SelectObject(&InfoFont);

			//�滭��Ϣ
			CPoint ptDraw = m_CardControl[i].GetCardPos();
			ptDraw.y-=16;
			ptDraw.x+=20;
			DrawTextString(pDC,m_szTailCard[i],RGB(255,255,128),RGB(0,0,0),ptDraw.x,ptDraw.y);
 
			//������Դ
			pDC->SelectObject(pOldFont);
			InfoFont.DeleteObject();
		}
	}

	//�滭�˿�
	if (m_SendCardItemArray.GetCount()>0 && !m_bJettonAction)
	{
		//��������
		//CImageHandle HandleCard(&m_ImageCard);

		//��ȡ��С
		int nItemWidth=m_ImageCard.GetWidth()/13;
		int nItemHeight=m_ImageCard.GetHeight()/5;

		//�滭�˿�
		int nXPos=m_SendCardPos.x-nItemWidth/2;
		int nYPos=m_SendCardPos.y-nItemHeight/2+10;
		m_ImageCard.TransDrawImage(pDC,nXPos,nYPos,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4,RGB(255,0,255));
	}

	//�ؼ�����
	if(m_btGiveUp.IsWindowVisible()==TRUE)
	{
		m_ImageControlBack.TransDrawImage(pDC,m_ptControlBack.x,m_ptControlBack.y,RGB(255,0,255));
	}

	return;
}

//���ö���
void CGameClientView::SetOffLinkUser(WORD wChairID,bool bOffLink)
{
	m_bUserOffLine[wChairID]=bOffLink;

	//���½���
	RefreshGameView();

	return;
}

//��������
void CGameClientView::SetCardType(WORD wChairID,LPCTSTR pszHeapString,LPCTSTR pszTailString)
{
	//���ñ���
	if(wChairID<GAME_PLAYER)
	{
		_sntprintf(m_szHeapCard[wChairID],sizeof(m_szHeapCard[wChairID]),TEXT("%s"),pszHeapString);
		_sntprintf(m_szTailCard[wChairID],sizeof(m_szTailCard[wChairID]),TEXT("%s"),pszTailString);
	}
	else
	{
		ZeroMemory(m_szHeapCard,sizeof(m_szHeapCard));
		ZeroMemory(m_szTailCard,sizeof(m_szTailCard));
	}

	return;
}

//��ȡ�˿�
bool CGameClientView::GetHitCardData(BYTE cbCardData[],BYTE cbCardCount)
{
	//��ȡ����
	BYTE cbTempCount=0;
	for(BYTE i=0;i<CountArray(m_cbHitCardData);i++)
	{
		if(m_cbHitCardData[i]!=0)cbCardData[cbTempCount++]=m_cbHitCardData[i];
	}
	ASSERT(cbTempCount==cbCardCount);

	return (cbTempCount==cbCardCount);
}

//���ò���
void CGameClientView::SetUserOperate(WORD wChairID,bool bOperate)
{
	if(m_bUserOperate[wChairID]!=bOperate)
	{
		m_bUserOperate[wChairID]=bOperate;

		//���½���
		RefreshGameView();
	}

	return;
}

//�˿�����
void CGameClientView::SetHitCardData(BYTE cbCardData[],BYTE cbCardCount)
{
	if(cbCardCount>CountArray(m_cbHitCardData))return;

	if(cbCardData==NULL)
	{
		ZeroMemory(m_cbHitCardData,sizeof(m_cbHitCardData));
	}
	else
	{
		for(BYTE i=0;i<cbCardCount;i++)
		{
			m_cbHitCardData[i]=cbCardData[i];
		}
	}

	//���½���
	RefreshGameView();

	return;
}

//���õ��
void CGameClientView::SetHitPositively(bool bPositively)
{
	if(m_bHitPositively!=bPositively)
	{
		m_bHitPositively=bPositively;

		//���½���
		RefreshGameView();
	}

	return;
}

//�û�����
void CGameClientView::SetUserJetton(WORD wChairID,LONGLONG lGold)
{
	if(m_lUserScore[wChairID]!=lGold)
	{
		m_lUserScore[wChairID]=lGold;

		m_UserGoldView[wChairID].SetGold(lGold);

		//���½���
		RefreshGameView();
	}

	return ;
}

//���õ�Ԫ
void CGameClientView::SetCellScore(LONGLONG lCellScore)
{
	if(m_lCellScore != lCellScore)
	{
		m_lCellScore = lCellScore;

		//���½���
		RefreshGameView();
	}
	return ;
}

//������ע
void CGameClientView::SetUserTableScore(WORD wChairID, LONGLONG lTableScore)
{
	//��������
	if (wChairID!=INVALID_CHAIR) 
	{
		m_lTableScore[wChairID]=lTableScore;
		m_AddGoldView[wChairID].SetGold(lTableScore);
	}
	else
	{
		ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
		for (WORD i =0;i<GAME_PLAYER;i++)
		{
			m_AddGoldView[i].SetGold(0L);
		}
	}

	//���½���
	RefreshGameView();

	return;
}

//���ĳ���
void CGameClientView::SetCenterScore(LONGLONG lCenterScore)
{
	//��������
	m_lCenterScore = lCenterScore;
	m_CenterGoldView.SetGold(lCenterScore);

	//���±���
	RefreshGameView();

	return;
}

//�Ƚ���Ϣ
void CGameClientView::SetOpenCardInfo(BYTE cbCardData[GAME_PLAYER][MAX_COUNT],WORD wCurrentOpenUser,WORD wHeapMaxUser)
{
	//���õȴ�
	SetWiatCompare(true,false);

	//����ؼ�
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_CardControl[i].GetCardDisable())continue;

		BYTE cbTempCard[]={0,0,0,0};
		m_CardControl[i].SetCardData(cbTempCard,MAX_COUNT);
		m_CardControl[i].SetDisplayHead(true);
	}

	//��������
	m_bFlashCard=false;		
	m_wDelayIndex=0;
	m_wCurrentOpenUser=wCurrentOpenUser;
	m_wHeapMaxUser = wHeapMaxUser;
	m_wDelayCount=DELAY_OPEN_TIME/TIME_CARTOON_ACTION;		
	CopyMemory(m_cbHandCardData,cbCardData,sizeof(m_cbHandCardData));

	//����ʱ��
	if(m_SendCardItemArray.GetCount()==0 && !m_bJettonAction)
		SetTimer(IDI_CARTOON_ACTION,TIME_CARTOON_ACTION,NULL);

	return ;
}

//���ö���
bool CGameClientView::SetUserAction(WORD wActionUser, WORD wUserAction)
{
	//���ö���
	if ((m_wActionUser!=wActionUser)||(m_wUserAction!=wUserAction))
	{
		//���ñ���
		m_wActionUser=wActionUser;
		m_wUserAction=wUserAction;

		//���½���
		RefreshGameView();

		//����ʱ��
		if (m_wActionUser!=INVALID_CHAIR) SetTimer(IDI_USER_ACTION,TIME_USER_ACTION,NULL);
	}

	return true;
}

//������Ϣ
void CGameClientView::SetJettonMoveInfo( WORD wChairID, int iMoveType,LONGLONG lTableScore)
{
	//��������
	int nAnimeStep = 60;

	m_JettonStatus[wChairID].wMoveIndex = 0;
	m_JettonStatus[wChairID].iMoveType = iMoveType;
	m_MoveGoldView[wChairID].SetGold(lTableScore);

	switch(iMoveType)
	{
	case MOVE_USER_ADD:				// ��ע������ע
		{
			m_JettonStatus[wChairID].ptFrom = ((wChairID!=MY_VIEW_CHAIR_ID)?m_ptUserJetton[wChairID]:m_ptMyJetton);
			m_JettonStatus[wChairID].ptDest = m_ptAddJetton[wChairID];
			m_JettonStatus[wChairID].ptCourse= m_JettonStatus[wChairID].ptFrom;
			m_JettonStatus[wChairID].lGold =lTableScore;
			break;
		}
	case MOVE_USER_GETSCORE:		// �м����������ע
		{
			m_JettonStatus[wChairID].ptFrom = m_ptCenterJetton;
			m_JettonStatus[wChairID].ptDest = m_ptUserJetton[wChairID];
			m_JettonStatus[wChairID].ptCourse =m_JettonStatus[wChairID].ptFrom;
			m_JettonStatus[wChairID].lGold =lTableScore;

			//����Ч��
			CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
			pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("MOVE_GOLD"));
			break;
		}
	case MOVE_POOL_ALLSCORE:		// ��ע�ϲ����м�
		{
			m_JettonStatus[wChairID].ptFrom = m_ptAddJetton[wChairID];
			m_JettonStatus[wChairID].ptDest = m_ptCenterJetton;
			m_JettonStatus[wChairID].ptCourse= m_JettonStatus[wChairID].ptFrom;
			m_JettonStatus[wChairID].lGold =lTableScore;

			//����Ч��
			CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
			pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("MOVE_GOLD"));
			break;
		}
	case MOVE_BACK_SCORE:			// ���س���
		{
			m_JettonStatus[wChairID].ptFrom = m_ptAddJetton[wChairID];
			m_JettonStatus[wChairID].ptDest = m_ptUserJetton[wChairID];
			m_JettonStatus[wChairID].ptCourse= m_JettonStatus[wChairID].ptFrom;
			m_JettonStatus[wChairID].lGold =lTableScore;

			//����Ч��
			CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
			pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("MOVE_GOLD"));
			break;
		}

	default:
		ASSERT(FALSE);
	}

	//λ�Ƽ���
	INT nXCount=abs(m_JettonStatus[wChairID].ptDest.x-m_JettonStatus[wChairID].ptFrom.x)/nAnimeStep+rand()%8;
	INT nYCount=abs(m_JettonStatus[wChairID].ptDest.y-m_JettonStatus[wChairID].ptFrom.y)/nAnimeStep+rand()%8;
	m_JettonStatus[wChairID].wMoveCount=__max(1,__max(nXCount,nYCount));

	//����ʱ��
	if(!m_bJettonAction)
	{
		m_bJettonAction = true;
		if(m_SendCardItemArray.GetCount()==0 && m_wCurrentOpenUser==INVALID_CHAIR)
		{
			SetTimer(IDI_CARTOON_ACTION,TIME_CARTOON_ACTION,NULL);
		}
	}

	//���½���
	RefreshGameView();

	return;
}

//���ƶ���
bool CGameClientView::CartoonOpenCard()
{
	if(m_wCurrentOpenUser==INVALID_CHAIR)return false;

	m_wDelayIndex++;

	//��˸�˿�
	if(m_bFlashCard)
	{
		if(m_wDelayCount==DELAY_OPEN_TIME/TIME_CARTOON_ACTION+15 && m_wDelayIndex%2==0)
			m_bUserOperate[m_wHeapMaxUser]=!m_bUserOperate[m_wHeapMaxUser];

		//��ʼ��˸
		if(m_wDelayCount==m_wDelayIndex && m_wDelayCount<DELAY_OPEN_TIME/TIME_CARTOON_ACTION)
		{
			m_wDelayIndex=0;
			m_wDelayCount=DELAY_OPEN_TIME/TIME_CARTOON_ACTION+15;
			//bool bFlash = !m_CardControl[m_wHeapMaxUser].GetCardFlash();
			//m_CardControl[m_wHeapMaxUser].SetCardFlash(bFlash);

			//�û�����
			CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
			TCHAR tNameInfo[LEN_NICKNAME] = TEXT("");
			pGameClientEngine->GetViewUserName(m_wHeapMaxUser,tNameInfo);

			//�����Ϣ
			TCHAR tchInfo[512] = TEXT("");
			_sntprintf(tchInfo,CountArray(tchInfo),TEXT("���ͷ���û�:[%s],%s����."),tNameInfo,m_szHeapCard[m_wHeapMaxUser]);
			//pGameClientEngine->InsertCustomString(tchInfo,RGB(0,0,255),true);
		}
		else if(m_wDelayCount==DELAY_OPEN_TIME/TIME_CARTOON_ACTION+15)
		{
			////�����˸
			//if(m_wDelayIndex%5==0)
			//{
			//	bool bFlash = !m_CardControl[m_wHeapMaxUser].GetCardFlash();
			//	m_CardControl[m_wHeapMaxUser].SetCardFlash(bFlash);
			//}

			//������˸
			if(m_wDelayCount==m_wDelayIndex)
			{
				m_wDelayIndex=0;
				m_bFlashCard = false;
				m_bWaitCompareHeap=false;
				m_bWaitCompareTail=true;
				m_wDelayCount=DELAY_OPEN_TIME/TIME_CARTOON_ACTION;
				//m_CardControl[m_wHeapMaxUser].SetCardFlash(false);
				ZeroMemory(m_bUserOperate,sizeof(m_bUserOperate));
			}
		}
	}
	else if(m_wDelayCount==m_wDelayIndex)
	{
		m_wDelayIndex=0;

		//�����˿�
		BYTE cbCard[MAX_COUNT];
		ZeroMemory(cbCard,sizeof(cbCard));
		WORD wChairID = m_wCurrentOpenUser;

		//ͷ�ƿ���
		if(m_CardControl[wChairID].GetUnDisableCardCount()==0)
		{
			CopyMemory(&cbCard[2],m_cbHandCardData[wChairID],sizeof(BYTE)*2);
			m_CardControl[wChairID].SetCardData(cbCard,MAX_COUNT);
			m_bUserOperate[wChairID]=true;

			//ͷ���û�
			for(WORD i=1;i<GAME_PLAYER;i++)
			{
				WORD wUser = (m_wCurrentOpenUser+i)%GAME_PLAYER;
				if(m_CardControl[wUser].GetCardDisable())continue;
				if(m_CardControl[wUser].GetUnDisableCardCount()==2)continue;
				m_wCurrentOpenUser = wUser;
				break;
			}

			//ͷ�ƽ���
			if(i==GAME_PLAYER)
			{
				m_bFlashCard = true;
				m_wDelayCount/=4;

				//����β��
				for(WORD i=1;i<GAME_PLAYER;i++)
				{
					WORD wUser = (m_wCurrentOpenUser+i)%GAME_PLAYER;
					if(m_CardControl[wUser].GetCardDisable())continue;
					if(m_CardControl[wUser].GetUnDisableCardCount()==2)
					{
						m_wCurrentOpenUser = wUser;
						break;
					}
				}
			}
		}
		//β�ƿ���
		else if(m_CardControl[wChairID].GetUnDisableCardCount()==2)
		{
			CopyMemory(&cbCard[2],m_cbHandCardData[wChairID],sizeof(BYTE)*2);
			CopyMemory(cbCard,&m_cbHandCardData[wChairID][2],sizeof(BYTE)*2);
			m_CardControl[wChairID].SetCardData(cbCard,MAX_COUNT);
			m_bUserOperate[wChairID]=true;

			//β���û�
			for(WORD i=1;i<GAME_PLAYER;i++)
			{
				WORD wUser = (m_wCurrentOpenUser+i)%GAME_PLAYER;
				if(m_CardControl[wUser].GetCardDisable())continue;
				if(m_CardControl[wUser].GetUnDisableCardCount()==MAX_COUNT)continue;
				m_wCurrentOpenUser = wUser;
				break;
			}

			//ͷ�ƽ���
			if(i==GAME_PLAYER)
			{
				m_bWaitCompareTail=false;
				m_wDelayCount=0;
				m_wCurrentOpenUser = INVALID_CHAIR;

				//������Ϣ
				SendEngineMessage(IDM_OPEN_CARD_FINISH,0,0);
			}
		}
	}

	return true;
}

//�ƶ�����
bool CGameClientView::CartoonMoveJetton()
{
	bool bAllClean = true;

	//���ñ���
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		//�ƶ�����
		if(m_JettonStatus[i].wMoveIndex<m_JettonStatus[i].wMoveCount)
		{
			bAllClean = false;
			m_JettonStatus[i].wMoveIndex++;
			WORD wMoveIndex = m_JettonStatus[i].wMoveIndex;
			WORD wMoveCount = m_JettonStatus[i].wMoveCount;
			m_JettonStatus[i].ptCourse.x =m_JettonStatus[i].ptFrom.x + (m_JettonStatus[i].ptDest.x-m_JettonStatus[i].ptFrom.x)*wMoveIndex/wMoveCount;
			m_JettonStatus[i].ptCourse.y =m_JettonStatus[i].ptFrom.y + (m_JettonStatus[i].ptDest.y-m_JettonStatus[i].ptFrom.y)*wMoveIndex/wMoveCount;
		}
		else if(m_JettonStatus[i].wMoveCount>0)
		{
			//���봦��
			switch( m_JettonStatus[i].iMoveType )
			{
			case MOVE_USER_ADD:				// ��ע������ע
				{
					m_lTableScore[i] += m_JettonStatus[i].lGold ;
					m_AddGoldView[i].SetGold(m_lTableScore[i]);
					break;
				}

			case MOVE_POOL_ALLSCORE:		// ��ע�ϲ����м�
				{
					m_lCenterScore += m_JettonStatus[i].lGold;
					m_CenterGoldView.SetGold(m_lCenterScore);
					break;
				}
			case MOVE_USER_GETSCORE:		// �м����������ע
				{
					m_lUserScore[i] += m_JettonStatus[i].lGold;
					m_UserGoldView[i].SetGold(m_lUserScore[i]);
					m_CenterGoldView.SetGold(0);
					break;
				}
			case MOVE_BACK_SCORE:			// ���س���
				{
					m_lUserScore[i] += m_JettonStatus[i].lGold;
					m_UserGoldView[i].SetGold(m_lUserScore[i]);
					break;
				}
			default:
				ASSERT(FALSE);
			}

			//������Ϣ
			m_MoveGoldView[i].SetGold(0);
			ZeroMemory(&m_JettonStatus[i],sizeof(m_JettonStatus[i]));
		}
	}

	if(bAllClean)
	{
		m_bJettonAction = false;
	}

	//���½���
	RefreshGameView();

	return !bAllClean;
}

//�ƶ��˿�
bool CGameClientView::CartoonMoveCard()
{
	if(m_SendCardItemArray.GetCount()==0)return false;

	//��������
	m_wSendIndex++;

	//����λ��
	tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];
	m_SendCardPos.x=m_SendStartPos.x+(m_SendEndingPos[pSendCardItem->wChairID].x-m_SendStartPos.x)*m_wSendIndex/m_wSendCount;
	m_SendCardPos.y=m_SendStartPos.y+(m_SendEndingPos[pSendCardItem->wChairID].y-m_SendStartPos.y)*m_wSendIndex/m_wSendCount;

	//ֹͣ�ж�
	if (m_wSendIndex>=m_wSendCount)
	{
		//��ȡ�˿�
		BYTE cbCardData[MAX_COUNT];
		WORD wChairID=pSendCardItem->wChairID;
		BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

		//�����˿�
		cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
		m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);

		//ɾ������
		m_SendCardItemArray.RemoveAt(0);

		//��������
		if (m_SendCardItemArray.GetCount()>0)
		{
			//��������
			tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

			//����λ��
			m_SendCardPos.x=m_SendStartPos.x;
			m_SendCardPos.y=m_SendStartPos.y;

			//λ�Ƽ���
			int nXCount=abs(m_SendEndingPos[pSendCardItem->wChairID].x-m_SendStartPos.x)/SEND_PELS;
			int nYCount=abs(m_SendEndingPos[pSendCardItem->wChairID].y-m_SendStartPos.y)/SEND_PELS;

			//���ô���
			m_wSendIndex=0;
			m_wSendCount=__max(1,__max(nXCount,nYCount));

			//��������
			CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
			pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
		}
		else 
		{
			//��ɴ���
			SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);
		}
	}

	return true;
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
	if(!m_bJettonAction && m_wCurrentOpenUser==INVALID_CHAIR)KillTimer(IDI_CARTOON_ACTION);
	m_SendCardItemArray.RemoveAll();

	//���½���
	RefreshGameView();

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
	SendCardItem.cbCardData=cbCardData;
	SendCardItem.wChairID=wChairID;

	//��������
	m_SendCardItemArray.Add(SendCardItem);

	//��������
	if (m_SendCardItemArray.GetCount()==1) 
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//����λ��
		m_SendCardPos.x=m_SendStartPos.x;
		m_SendCardPos.y=m_SendStartPos.y;

		//λ�Ƽ���
		int nXCount=abs(m_SendEndingPos[SendCardItem.wChairID].x-m_SendStartPos.x)/SEND_PELS;
		int nYCount=abs(m_SendEndingPos[SendCardItem.wChairID].y-m_SendStartPos.y)/SEND_PELS;

		//���ô���
		m_wSendIndex=0;
		m_wSendCount=__max(1,__max(nXCount,nYCount));

		//���ö�ʱ��
		if(!m_bJettonAction && m_wCurrentOpenUser==INVALID_CHAIR)
		{
			SetTimer(IDI_CARTOON_ACTION,TIME_CARTOON_ACTION,NULL);
		}

		//��������
		CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
		pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	}

	return;
}

//ׯ�ұ�־
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
	m_wBankerUser=wBankerUser;

	//���½���
	RefreshGameView();

	return;
}

//�ȴ���ׯ
void CGameClientView::SetWaitInvest(bool bWaitInvest)
{
	if(bWaitInvest!=m_bWaitInvest)
	{
		m_bWaitInvest=bWaitInvest;

		//���½���
		RefreshGameView();
	}

	return ;
}

//�ȴ���־
void CGameClientView::SetWaitOpenCard(bool bWaitOpenCard)
{
	if(m_bWaitOpenCard!=bWaitOpenCard)
	{
		m_bWaitOpenCard=bWaitOpenCard;

		//���½���
		RefreshGameView();
	}
	return;
}

//�ȴ��Ƚ�
void CGameClientView::SetWiatCompare(bool bWaitCompareHeap,bool bWaitCompareTail)
{
	m_bWaitCompareHeap=bWaitCompareHeap;
	m_bWaitCompareTail=bWaitCompareTail;

	//���½���
	RefreshGameView();

	return;
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

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	CRect rcDraw;
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		rcDraw.CopyRect(lpRect);
		rcDraw.OffsetRect(nXExcursion[i],nYExcursion[i]);
		pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	//�滭����
	rcDraw.CopyRect(lpRect);
	pDC->SetTextColor(crText);
	pDC->DrawText(pszString,nStringLength,&rcDraw,DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//��ʱ����Ϣ
void CGameClientView::OnTimer(UINT nIDEvent)
{
	//���ʹ���
	switch(nIDEvent)
	{
	case IDI_CARTOON_ACTION:		//��������
		{
			bool bKillTime = true;

			//�����ƶ�
			if(m_bJettonAction && CartoonMoveJetton()) bKillTime = false;

			//�����ƶ�
			else if(CartoonMoveCard()) bKillTime = false;

			//���ƶ���
			if(CartoonOpenCard()) bKillTime = false;

			//ɾ����ʱ��
			if(bKillTime)
			{
				m_bJettonAction = false;
				KillTimer(nIDEvent);
			}

			//���½���
			RefreshGameView();

			return;
		}
	case IDI_USER_ACTION:	//�û�����
		{
			//���ñ���
			m_wUserAction=0;
			m_wActionUser=INVALID_CHAIR;
			KillTimer(nIDEvent);

			//���½���
			RefreshGameView();

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_JetonControl.EstimateHitJetton(point))
	{
		//��������
		CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
		pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("HIT_JETTON"));

		//���½���
		RefreshGameView();
	}

	if(m_bHitPositively)
	{
		//��������
		//CImageHandle HandleCard(&m_ImageCard);

		//��ȡ��С
		int nItemWidth=m_ImageCard.GetWidth()/13;
		int nItemHeight=m_ImageCard.GetHeight()/5;

		//�жϷ�Χ
		if(m_cbHitCardData[4]==0 || m_cbHitCardData[5]==0)
		{
			for(INT i=3;i>=0;i--)
			{
				if(m_cbHitCardData[i]==0)continue;

				CRect rect;
				rect.right = m_ptHitCard[i].x+nItemWidth;
				rect.left = m_ptHitCard[i].x;
				rect.top = m_ptHitCard[i].y;
				rect.bottom = m_ptHitCard[i].y+nItemHeight;
				if(rect.PtInRect(point)==TRUE)
				{
					for(BYTE j=4;j<CountArray(m_cbHitCardData);j++)
					{
						if(m_cbHitCardData[j]==0)
						{
							m_cbHitCardData[j]=m_cbHitCardData[i];
							m_cbHitCardData[i]=0;

							//�жϿؼ�
							BYTE cbEnable=((m_cbHitCardData[4]!=0 && m_cbHitCardData[5]!=0)?TRUE:FALSE);
							m_btOpenCard.EnableWindow(cbEnable);

							//��������
							CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
							pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("HIT_CARD"));

							//���½���
							RefreshGameView();

							return;
						}
					}
				}
			}
		}

		//�жϷ�Χ
		if(m_cbHitCardData[4]!=0 || m_cbHitCardData[5]!=0)
		{
			for(BYTE i=CountArray(m_cbHitCardData)-1;i>=4;i--)
			{
				if(m_cbHitCardData[i]==0)continue;

				CRect rect;
				rect.right = m_ptHitCard[i].x+nItemWidth;
				rect.left = m_ptHitCard[i].x;
				rect.top = m_ptHitCard[i].y;
				rect.bottom = m_ptHitCard[i].y+nItemHeight;
				if(rect.PtInRect(point)==TRUE)
				{
					for(BYTE j=0;j<4;j++)
					{
						if(m_cbHitCardData[j]==0)
						{
							m_cbHitCardData[j]=m_cbHitCardData[i];
							m_cbHitCardData[i]=0;

							//�жϿؼ�
							BYTE cbEnable=((m_cbHitCardData[4]!=0 && m_cbHitCardData[5]!=0)?TRUE:FALSE);
							m_btOpenCard.EnableWindow(cbEnable);

							//��������
							CGameClientEngine * pGameClientEngine=CONTAINING_RECORD(this,CGameClientEngine,m_GameClientView);
							pGameClientEngine->PlayGameSound(AfxGetInstanceHandle(),TEXT("HIT_CARD"));

							//���½���
							RefreshGameView();

							return;
						}
					}
				}
			}
		}

	}

	__super::OnLButtonDown(nFlags, point);
}

//�ƶ���Ϣ
void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_JetonControl.EstimateMove(point))
	{
		//���½���
		RefreshGameView();
	}

	if(m_CardControl[MY_VIEW_CHAIR_ID].GetPositively() && m_CardControl[MY_VIEW_CHAIR_ID].GetCardCount()>=2)
	{
		m_CardControl[MY_VIEW_CHAIR_ID].AnalyseMingCard();
	}

	__super::OnMouseMove(nFlags, point);
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
