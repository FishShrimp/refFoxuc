#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "cassert"
//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸ��ʱ��
#define IDI_OUT_CARD					200								//���ƶ�ʱ��
#define IDI_START_GAME					201								//��ʼ��ʱ��
#define IDI_PASS_CARD					202								//���ƶ�ʱ��
#ifdef _DEBUG
#define TIME_OUT_CARD				30
#define TIME_PASS_CARD				2
#define TIME_START_GAME				15
#else
#define TIME_OUT_CARD				30
#define TIME_PASS_CARD				2
#define TIME_START_GAME				30
#endif


#define MAX_TIMEOUT_TIMES				3								//���ʱ����

//��Ϸ��ʱ��
#define IDI_WAIT_CLEAR					300								//����ȴ�
#define IDI_DISPATCH_CARD				301								//���ƶ�ʱ��
#define IDI_LAST_TURN_CARD				302								//���ֶ�ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnPassCard)
	ON_MESSAGE(IDM_CARD_TYPE,OnCardType)
	ON_MESSAGE(IDM_REQ_HUNTER,OnReqHunter)
	ON_MESSAGE(IDM_OUT_PROMPT,OnOutPrompt)
	ON_MESSAGE(IDM_SORT_BY_COUNT,OnSortByCount)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
	ON_MESSAGE(IDM_LAST_TURN_CARD,OnLastTurnCard)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
	//��Ϸ����
	m_bReqHumter=false;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	//ʤ����Ϣ

	m_wWinOrder=0xFFFF;

	//���ñ���
	m_bStustee=false;
	m_bSortCount=false;
	m_dwCardHSpace=DEFAULT_PELS;

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));
	//���Ʊ���
	m_cbTurnCardCount=0;
	ZeroMemory(&m_LastOutCard,sizeof(tagAnalyseResult));
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//�����˿�
	m_bLastTurn=false;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	memset(m_cbOutCardCount,255,sizeof(m_cbOutCardCount));

	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	m_wMostUser=INVALID_CHAIR;
	m_wTimeOutCount=0;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	//��ʼ��Ϣ
	m_wGetRandCardID=INVALID_CHAIR;
	m_cbGetCardPosition=0xFF;
	m_cbRandCardData=0xFF;
	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{

	//ȫ�ֶ���
	//CGlobalUnits * m_pGlobalUnits=(CGlobalUnits *)CGlobalUnits::GetInstance();
	//ASSERT(m_pGlobalUnits!=NULL);

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//��ȡ����
	m_dwCardHSpace=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("CardSpace"),DEFAULT_PELS);

	//��������
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;


	return true;
}


//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;

	//ɾ����ʱ
	KillGameClock(0);
	KillTimer(IDI_WAIT_CLEAR);
	KillTimer(IDI_DISPATCH_CARD);
	
	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;

	//���ñ���
	m_bStustee=false;
	m_bSortCount=false;

	//ʤ����Ϣ
	m_wWinOrder=0xFFFF;

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));
	//���Ʊ���
	m_cbTurnCardCount=0;
	ZeroMemory(&m_LastOutCard,sizeof(tagAnalyseResult));
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//�����˿�
	m_bLastTurn=false;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	memset(m_cbOutCardCount,255,sizeof(m_cbOutCardCount));

	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	//��ʼ��Ϣ
	m_wGetRandCardID=INVALID_CHAIR;
	m_cbGetCardPosition=0xFF;
	m_cbRandCardData=0xFF;

	return true;
}

//��Ϸ����
void CGameClientEngine::OnGameOptionSet()
{
	////��������
	//CGameOption GameOption;
	//GameOption.m_dwCardHSpace=m_dwCardHSpace;
	//GameOption.m_bHaveVoiceCard=m_bHaveVoiceCard;
	//GameOption.m_bEnableSound=m_pGlobalUnits->m_bAllowSound;

	////��������
	//if (GameOption.DoModal()==IDOK)
	//{
	//	//��ȡ����
	//	m_dwCardHSpace=GameOption.m_dwCardHSpace;

	//	//���ÿؼ�
	//	if(m_bHaveVoiceCard)
	//	   m_pGlobalUnits->m_bAllowSound=GameOption.m_bEnableSound;
	//	
	//	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardSpace(m_dwCardHSpace,0,20);
	//	//��������
	//	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
	//}

	return;
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}
//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	switch (wClockID)
	{
	case IDI_OUT_CARD:			//�û�����
		{
			//�����й�
			if((IsLookonMode()==false)&&(wChairID==GetMeChairID()))
			{
				//���ʱ��ľ������Ϊ���й�״̬
				if(nElapse==0||(m_bStustee&&nElapse<TIME_OUT_CARD-2))
				{
					KillGameClock(IDI_OUT_CARD);
					m_wTimeOutCount++;
					AutomatismOutCard();
				}
				//�йܴ���
				if (m_wTimeOutCount>=MAX_TIMEOUT_TIMES&&!m_bStustee)
				{
					OnStusteeControl(0,0);
					if(m_pIStringMessage != NULL)
						m_pIStringMessage->InsertSystemString(TEXT("��������γ�ʱ���л�Ϊϵͳ�йܡ�ģʽ."));
					m_bStustee=true;
				}
				
			}
			//��������
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)/*&&m_pGlobalUnits->m_bAllowSound*/) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			return true;
		}
	case IDI_PASS_CARD:
		{
			if(nElapse==0)
				OnPassCard(0,0);
			return true;
		}
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID()))
						AfxGetMainWnd()->PostMessage(WM_CLOSE);
				return false;
			}
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}

	}

	return false;
}

//�Թ�״̬
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	//�Թ�����
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));

	return true;
}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_PASS_CARD:		//��������
		{
			return OnSubPassCard(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_TRUSTEE:			//�û��й�
		{
			return OnSubTrustee(pData,wDataSize);
		}
	}

	//�Ƿ���Ϣ
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_SK_FREE:	//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree))return false;

			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			
			if (IsLookonMode()==false)
			{
				if(GetMeUserItem()->GetUserStatus()!=US_READY)
				{
					//���ÿؼ�
					m_GameClientView.m_btStart.ShowWindow(TRUE);
					m_GameClientView.m_btStart.SetFocus();

					//����ʱ��
					SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
				}
				//�����˿�
				m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag(true);

				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
			}
			
			m_GameClientView.m_lLastTurnScore[ME_VIEW_CHAIR]=pStatusFree->lLastTurnScore;
			m_GameClientView.m_lAllTurnScore[ME_VIEW_CHAIR]=pStatusFree->lAllTurnScore;

			//���ý���
			m_GameClientView.m_btScore.ShowWindow(SW_SHOW);
			m_GameClientView.RefreshGameView();

			if(m_pIStringMessage != NULL)
				m_pIStringMessage->InsertSystemString(TEXT("��Ϸ����,�Է�����ʣ��1��6��Ϊƽ�ۣ�7��11��Ϊ���ۣ�12������Ϊ˫�ۣ�"));

			return true;
		}
	case GS_SK_PLAYING:		//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;
			//��������
			WORD wMeChairID=GetMeChairID();
			//��ʼ����
			m_bLastTurn=false;
			ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
			memset(m_cbOutCardCount,255,sizeof(m_cbOutCardCount));
			//���ñ���
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			//���Ʊ���
			m_cbTurnCardCount=pStatusPlay->cbTurnCardCount;
			CopyMemory(m_cbTurnCardData,pStatusPlay->cbTurnCardData,m_cbTurnCardCount*sizeof(BYTE));

			//�˿�����
			CopyMemory(m_cbHandCardCount,pStatusPlay->cbHandCardCount,sizeof(m_cbHandCardCount));
			CopyMemory(&m_LastOutCard,&(pStatusPlay->LastOutCard),sizeof(tagAnalyseResult));
			CopyMemory(m_cbHandCardData,pStatusPlay->cbHandCardData,sizeof(BYTE)*m_cbHandCardCount[wMeChairID]);
			
			//���ƽ���
			m_GameClientView.m_btScore.ShowWindow(SW_SHOW);
			m_GameClientView.SetUserWinOrder(INVALID_CHAIR,0xFFFF);
	
			//�����˿�
			BYTE cbCardData[MAX_COUNT];
			ZeroMemory(cbCardData,sizeof(cbCardData));
			
			m_wTimeOutCount=0;
			m_bStustee=pStatusPlay->bTrustee[GetMeChairID()];
			
			//�����˿�
			for (BYTE cbIndex=0;cbIndex<GAME_PLAYER;cbIndex++)
			{
				//��������
				WORD wViewChairID=SwitchViewChairID(cbIndex);
				//��������
				m_GameClientView.SetCardCount(wViewChairID,pStatusPlay->cbHandCardCount[cbIndex]);
				//��������
				m_GameClientView.m_lAllTurnScore[wViewChairID]=pStatusPlay->lAllTurnScore[cbIndex];
				m_GameClientView.m_lLastTurnScore[wViewChairID]=pStatusPlay->lLastTurnScore[cbIndex];	

				if (wViewChairID!=ME_VIEW_CHAIR || !IsAllowLookon())
					m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardData,m_cbHandCardCount[cbIndex]);
				else 
					m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData,m_cbHandCardCount[cbIndex]);
				//�й�����
				m_GameClientView.SetTrustee(SwitchViewChairID(cbIndex),pStatusPlay->bTrustee[cbIndex]);
				//�����б�
				m_cbOutCardCount[cbIndex][0]=pStatusPlay->cbOutCardCount[cbIndex];
				CopyMemory(m_cbOutCardData[cbIndex][0],pStatusPlay->cbOutCardData[cbIndex],sizeof(BYTE)*m_cbOutCardCount[cbIndex][0]);
			}
			//�������
			if (!IsLookonMode())
			{
				if(pStatusPlay->wCurrentUser==GetMeChairID())
				{
					m_GameClientView.m_btOutCard.EnableWindow(FALSE);
					m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
					m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
					m_GameClientView.m_btPassCard.EnableWindow((m_cbTurnCardCount!=0)?TRUE:FALSE);
				}
					//�йܰ�ť
				m_GameClientView.m_btStusteeControl.SetButtonImage((m_bStustee==true)?IDB_BT_STOP_TRUSTEE:IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetPositively(true);
				m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag(true);
			}
			//��������
			if (m_cbTurnCardCount!=0)
			{
				WORD wViewChairID=SwitchViewChairID(pStatusPlay->wTurnWiner);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbTurnCardData,m_cbTurnCardCount);
			}
			//���ö�ʱ��
			SetGameClock(pStatusPlay->wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

			//��ʾ��ʾ��Ϣ
			CString strTemp;
			strTemp.Format(_T("��ǰ��Ԫ����:ƽ��:%I64d--����:%I64d--˫��:%I64d"),pStatusPlay->lCellScore[0],pStatusPlay->lCellScore[1],pStatusPlay->lCellScore[2]);
			if(m_pIStringMessage != NULL)
				m_pIStringMessage->InsertSystemString(strTemp);

			return true;
		}
	}
	return true;
}


bool  CGameClientEngine::OnTimerDispatchCard()
{
	//�ɷ�����
	if (m_cbRemnantCardCount>0)
	{
		//�˿�����
		BYTE cbUserCard[MAX_COUNT];
		ZeroMemory(cbUserCard,sizeof(cbUserCard));

		m_cbRemnantCardCount--;
		m_cbDispatchCardCount++;
		//���ý���
		WORD wViewChairID=0;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			wViewChairID=SwitchViewChairID(i);
			if((m_wGetRandCardID==i)&&(m_cbGetCardPosition==m_cbDispatchCardCount))
			{	
				BYTE cbRandCard[]={m_cbRandCardData};
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(cbRandCard,1);
			}
			if (wViewChairID==ME_VIEW_CHAIR)
				m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbDispatchCardData,m_cbDispatchCardCount); 
			else
				m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbUserCard,m_cbDispatchCardCount);				
		}
		//��������
		PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	}

	//��ֹ�ж�
	if (m_cbRemnantCardCount==0)
	{
		//ɾ����ʱ��
		KillTimer(IDI_DISPATCH_CARD);
		//�˿���Ŀ
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.SetCardCount(SwitchViewChairID(i),m_cbHandCardCount[i]);
		}
		//��ǰ���
		if (!IsLookonMode())
		{
			if(m_wCurrentUser==GetMeChairID())
			{
				ActiveGameFrame();
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);
				m_GameClientView.m_btPassCard.EnableWindow(FALSE);
				m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btHunter.ShowWindow(m_bReqHumter?SW_SHOW:SW_HIDE);
			}
			SetGameClock(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
			m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_cbHandCardData,MAX_COUNT);
			m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetPositively(IsLookonMode()==false);
		}
		//����ʣ��
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
	}
	//���½���
	m_GameClientView.RefreshGameView();
	return true;
}


bool CGameClientEngine::OnCanReqHunter(const BYTE cbCardData[],BYTE cbCardCount)
{
	bool bReturn=false;
	tagAnalyseResult AnalyseResult;
	ZeroMemory(&AnalyseResult,sizeof(tagAnalyseResult));
	m_GameLogic.AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

	bool bHaveBomb=false;
	for(BYTE cbIndex=0;cbIndex<13;cbIndex++)
	{
		if(AnalyseResult.m_CardStyle[cbIndex].m_cbCount>=4)
		{
			bHaveBomb=true;
			break;
		}
	}
	//�����ը�������ĸ������ڵ�����������Ȩ���г���
	if(!bHaveBomb&&AnalyseResult.m_nJockerCount>=3) bReturn=true;
	return bReturn;
}
//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	m_wBankerUser=pGameStart->wCurrentUser;
	//��Ϸ����
	m_wCurrentUser=pGameStart->wCurrentUser;
	//��Ϸ����
	m_wTimeOutCount=0;
	m_cbTurnCardCount=0;
	//��ʼ��Ϣ
	m_cbRandCardData=pGameStart->cbStartRandCard;
	m_wGetRandCardID=pGameStart->wGetRandCardID;
	m_cbGetCardPosition=pGameStart->cbGetCardPosition;
	//ʤ����Ϣ
	m_wWinOrder=0xFFFF;
	//���Ʊ���
	m_cbTurnCardCount=0;
	ZeroMemory(&m_LastOutCard,sizeof(tagAnalyseResult));
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//�����˿�
	m_bLastTurn=false;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	memset(m_cbOutCardCount,255,sizeof(m_cbOutCardCount));


	//����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡλ��
		WORD wViewChairID=SwitchViewChairID(i);

		//�����˿�
		if (wViewChairID==ME_VIEW_CHAIR)
		{
			bool bShowCard=((IsLookonMode()==false)||(IsAllowLookon()==true));
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetPositively(false);
			m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(bShowCard);
		}
		else
		{
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(false);
		}
	}


	//�����˿�
	WORD wMeChairID=GetMeChairID();
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_cbHandCardCount[i]=CountArray(pGameStart->cbCardData);
		m_GameClientView.SetCardCount(i,m_cbHandCardCount[i]);
	}
	CopyMemory(m_cbHandCardData,pGameStart->cbCardData,sizeof(BYTE)*m_cbHandCardCount[wMeChairID]);
	//�Ƿ���Գ���
	m_bReqHumter=OnCanReqHunter(m_cbHandCardData,m_cbHandCardCount[wMeChairID]);
	//�ɷ��˿�
	DispatchUserCard(m_cbHandCardData,m_cbHandCardCount[wMeChairID]);

	//�����˿�
	m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount[wMeChairID]);
	
	//�й�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetTrustee(SwitchViewChairID(i),pGameStart->bTrustee[i]);
	}
	//���½���
	m_GameClientView.SetUserWinOrder(INVALID_CHAIR,0xFFFF);
	//���½���
	m_GameClientView.RefreshGameView();
	//��������
	if ((IsLookonMode()==false)) ActiveGameFrame(); 
	//�������� 
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
 	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//��ʾ��ʾ��Ϣ
	CString strTemp;
	strTemp.Format(_T("��ǰ��Ԫ����:ƽ��:%I64d--����:%I64d--˫��:%I64d"),pGameStart->lCellScore[0],pGameStart->lCellScore[1],pGameStart->lCellScore[2]);
	if(m_pIStringMessage != NULL)
		m_pIStringMessage->InsertSystemString(strTemp);

	return true;
}

//�û�����
bool CGameClientEngine::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if(wDataSize!=sizeof(CMD_S_OutCard)) return false;
 	//��������
 	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;
	//ֹͣ����
	DoncludeDispatchCard();

 	//ɾ����ʱ��
 	KillTimer(IDI_WAIT_CLEAR);
 	KillGameClock(IDI_OUT_CARD);

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pOutCard->wOutCardUser); 
	bool bPlayGameSound=((IsLookonMode()==false)&&(wMeChairID==pOutCard->wOutCardUser));


	//��ʷ����
	if (m_cbTurnCardCount==0)
	{
		//�û��˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��������
			if (i!=pOutCard->wOutCardUser)
			{
				if(m_cbOutCardCount[i][0]!=255)
				{
					//�����˿�
					m_cbOutCardCount[i][1]=m_cbOutCardCount[i][0];
					CopyMemory(m_cbOutCardData[i][1],m_cbOutCardData[i][0],sizeof(BYTE)*m_cbOutCardCount[i][0]);
				}
				//�����˿�
				m_cbOutCardCount[i][0]=255;
				ZeroMemory(m_cbOutCardData[i][0],sizeof(BYTE)*MAX_COUNT);

				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			}
		}
		//������־
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	}

	//��ǰ����
	SwitchToCurrentCard();
	
	//���Ʊ���
	m_wCurrentUser=pOutCard->wCurrentUser;
	m_cbTurnCardCount=pOutCard->cbCardCount;
 	CopyMemory(m_cbTurnCardData,pOutCard->cbCardData,sizeof(BYTE)*pOutCard->cbCardCount);	

	//��ȡ����
	tagAnalyseResult analyseResult;
	memset(&analyseResult,0,sizeof(analyseResult));
	m_GameLogic.AnalysebCardData(pOutCard->cbCardData,pOutCard->cbCardCount,analyseResult);
	m_GameLogic.FindValidCard(analyseResult,m_LastOutCard);
	memcpy(&m_LastOutCard,&analyseResult,sizeof(tagAnalyseResult));

	//ը���ж�
	if(m_LastOutCard.m_cbCardType>=CT_BOMB)
	{
		//���Ŷ���
		m_GameClientView.SetBombEffect(true);
	}
	
	//��������
	if ((bPlayGameSound==false)&&((IsLookonMode()==true)||(wMeChairID!=pOutCard->wOutCardUser)))
	{
		if (m_LastOutCard.m_cbCardType>=CT_BOMB)
		{
			//��������
			if (bPlayGameSound==false)
			{
				bPlayGameSound=true;
				PlayBombSound(pOutCard->wOutCardUser,m_LastOutCard.m_cbBombGrade);
			}
		}
		else
		{
			if (bPlayGameSound==false)
			{
				bPlayGameSound=true;
				PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
			}
		}
	}

 
	//�����˿�
	if(m_cbOutCardCount[pOutCard->wOutCardUser][0]!=255)
	{
		//���Ƽ�¼
		m_cbOutCardCount[pOutCard->wOutCardUser][1]=m_cbOutCardCount[pOutCard->wOutCardUser][0];
		if(m_cbOutCardCount[pOutCard->wOutCardUser][0]!=0)
			CopyMemory(m_cbOutCardData[pOutCard->wOutCardUser][1],m_cbOutCardData[pOutCard->wOutCardUser][0],sizeof(BYTE)*m_cbOutCardCount[pOutCard->wOutCardUser][0]);
		else
			ZeroMemory(m_cbOutCardData[pOutCard->wOutCardUser][1],MAX_COUNT*sizeof(BYTE));

	}
	m_cbOutCardCount[pOutCard->wOutCardUser][0]=pOutCard->cbCardCount;
	CopyMemory(m_cbOutCardData[pOutCard->wOutCardUser][0],pOutCard->cbCardData,pOutCard->cbCardCount*sizeof(BYTE));
	

	//��������
	if ((IsLookonMode()==true)||(wMeChairID!=pOutCard->wOutCardUser))
	{
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(pOutCard->cbCardData,pOutCard->cbCardCount);
		//ɾ���˿�
		if (wViewChairID==ME_VIEW_CHAIR)
		{
			//ɾ���˿�
			BYTE cbSourceCount=m_cbHandCardCount[wMeChairID];
			m_cbHandCardCount[pOutCard->wOutCardUser]-=pOutCard->cbCardCount;
			m_GameLogic.RemoveCard(pOutCard->cbCardData,pOutCard->cbCardCount,m_cbHandCardData,cbSourceCount);

			//�����˿�
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData,m_cbHandCardCount[wMeChairID]);
		}
		else
		{
			m_cbHandCardCount[pOutCard->wOutCardUser]-=pOutCard->cbCardCount;
			//�����˿�
			BYTE cbCardCount[MAX_COUNT];
			ZeroMemory(cbCardCount,sizeof(cbCardCount));
			m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardCount,m_cbHandCardCount[pOutCard->wOutCardUser]);
		}
	}

	m_GameClientView.SetCardCount(wViewChairID,m_cbHandCardCount[pOutCard->wOutCardUser]);

	//����ж�
	if (m_wCurrentUser==pOutCard->wOutCardUser)
	{
		//���ñ���
		m_cbTurnCardCount=0;
		m_wCurrentUser=INVALID_CHAIR;
		m_wMostUser=pOutCard->wCurrentUser;
		ZeroMemory(&m_LastOutCard,sizeof(tagAnalyseResult));
		ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
		//��������
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//�û�����
			if (pOutCard->wOutCardUser==i) continue;
			//��������
			WORD wViewChairID=SwitchViewChairID(i);
			m_GameClientView.SetPassFlag(wViewChairID,true);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
		}

		//��������
		if (bPlayGameSound==false)
		{
			bPlayGameSound=true;
			PlayGameSound(AfxGetInstanceHandle(),TEXT("MOST_CARD"));
		}
		//���ö�ʱ��
		SetTimer(IDI_WAIT_CLEAR,3000,NULL);
		return true;
	}

	//�������
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		m_GameClientView.SetPassFlag(wViewChairID,false);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);

		//�����˿�
		if(m_cbOutCardCount[m_wCurrentUser][0]!=255)
		{
			//���Ƽ�¼
			m_cbOutCardCount[m_wCurrentUser][1]=m_cbOutCardCount[m_wCurrentUser][0];
			if(m_cbOutCardCount[m_wCurrentUser][0]!=0)
				CopyMemory(m_cbOutCardData[m_wCurrentUser][1],m_cbOutCardData[m_wCurrentUser][0],sizeof(BYTE)*m_cbOutCardCount[m_wCurrentUser][0]);
			else
				ZeroMemory(m_cbOutCardData[m_wCurrentUser][1],MAX_COUNT*sizeof(BYTE));
		}
		m_cbOutCardCount[m_wCurrentUser][0]=255;
	}

	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		//������
		if (m_bStustee==false) ActiveGameFrame();
		BYTE cbCardCount=m_cbHandCardCount[pOutCard->wCurrentUser];
		if(cbCardCount==1&&pOutCard->cbCardCount>=2)
		{
			m_GameClientView.m_btPassCard.EnableWindow(TRUE);
			m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btOutCard.EnableWindow(FALSE);
			SetGameClock(m_wCurrentUser,IDI_PASS_CARD,TIME_PASS_CARD);
		}
		else
		{
			m_GameClientView.m_btPassCard.EnableWindow(TRUE);
			m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
			m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
			SetGameClock(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
		}
	}
	else
	{
		if (pOutCard->wCurrentUser!=INVALID_CHAIR)
			SetGameClock(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
	}

	//���ư�ť
	m_GameClientView.m_btLastTurnCard.EnableWindow(TRUE);

	return true;
}

//��������
bool CGameClientEngine::OnSubPassCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PassCard));
	if (wDataSize!=sizeof(CMD_S_PassCard)) return false;

	//��������
	CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pBuffer;

	//ɾ����ʱ��
	KillGameClock(IDI_OUT_CARD);

	//���ñ���
	m_wCurrentUser=pPassCard->wCurrentUser;

	//��������
	if ((IsLookonMode()==true)||(pPassCard->wPassCardUser!=GetMeChairID()))
	{
		WORD wPassCardUser=pPassCard->wPassCardUser;
		m_GameClientView.SetPassFlag(SwitchViewChairID(wPassCardUser),true);
	}

	//�����˿�
	if(m_cbOutCardCount[pPassCard->wPassCardUser][0]!=255)
	{
		//���Ƽ�¼
		m_cbOutCardCount[pPassCard->wPassCardUser][1]=m_cbOutCardCount[pPassCard->wPassCardUser][0];
		if(m_cbOutCardCount[pPassCard->wPassCardUser][0]!=0)
			CopyMemory(m_cbOutCardData[pPassCard->wPassCardUser][1],m_cbOutCardData[pPassCard->wPassCardUser][0],sizeof(BYTE)*m_cbOutCardCount[pPassCard->wPassCardUser][0]);
		else
			ZeroMemory(m_cbOutCardData[pPassCard->wPassCardUser][1],MAX_COUNT*sizeof(BYTE));
	}
	m_cbOutCardCount[pPassCard->wPassCardUser][0]=0;
	ZeroMemory(m_cbOutCardData[pPassCard->wPassCardUser][0],MAX_COUNT*sizeof(BYTE));
	if(m_wCurrentUser != INVALID_CHAIR)
	{
		//��ͼλ��
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		//���ý���
		m_GameClientView.SetPassFlag(wViewChairID,false);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
		//�����˿�
		if(m_cbOutCardCount[m_wCurrentUser][0]!=255)
		{
			//���Ƽ�¼
			m_cbOutCardCount[m_wCurrentUser][1]=m_cbOutCardCount[m_wCurrentUser][0];
			if(m_cbOutCardCount[m_wCurrentUser][0]!=0)
				CopyMemory(m_cbOutCardData[m_wCurrentUser][1],m_cbOutCardData[m_wCurrentUser][0],sizeof(BYTE)*m_cbOutCardCount[m_wCurrentUser][0]);
			else
				ZeroMemory(m_cbOutCardData[m_wCurrentUser][1],MAX_COUNT*sizeof(BYTE));
		}
		m_cbOutCardCount[m_wCurrentUser][0]=255;
	}

	//һ���ж�
	if (pPassCard->cbTurnOver==TRUE)
	{
		//���Ʊ���
		m_cbTurnCardCount=0;
		ZeroMemory(&m_LastOutCard,sizeof(tagAnalyseResult));
		ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));
		//�������
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	}

	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		//������
		ActiveGameFrame();
		//���ð�ť
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btHunter.ShowWindow(m_bReqHumter?SW_SHOW:SW_HIDE);
	}
	//��������
	if ((IsLookonMode()==true)||(pPassCard->wPassCardUser!=GetMeChairID()))
		PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));
	SetGameClock(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	//����״̬
	SetGameStatus(GS_SK_FREE);
	//ֹͣ����
	DoncludeDispatchCard();
	//ɾ����ʱ��
	KillTimer(IDI_WAIT_CLEAR);
	KillGameClock(IDI_OUT_CARD);
	KillTimer(IDI_DISPATCH_CARD);
	
	//���ؿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btHunter.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btLastTurnCard.EnableWindow(FALSE);

	//��ǰ����
	SwitchToCurrentCard();

	//���û���
	CString strTemp=_T("���ͳ��:");
	//���û���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IClientUserItem * pClientUserItem=GetTableUserItem(i);
		m_GameClientView.m_ScoreView.SetGameScore(i,pClientUserItem->GetNickName(),pGameEnd->lGameScore[i]);

		if(pGameEnd->lGameScore[i]>0)
			strTemp.AppendFormat(_T("\n--%s: %+I64d\n"),pClientUserItem->GetNickName(),pGameEnd->lGameScore[i]);
		else
			strTemp.AppendFormat(_T("\n--%s: %I64d\n"),pClientUserItem->GetNickName(),pGameEnd->lGameScore[i]);

		if(pGameEnd->lBombScore[i]>0)
			strTemp.AppendFormat(_T("----��ù��׵÷�Ϊ:%+I64d\n"),pGameEnd->lBombScore[i]);
		else
			strTemp.AppendFormat(_T("----��ù��׵÷�Ϊ:%I64d\n"),pGameEnd->lBombScore[i]);

		strTemp+=_T("----����ը���������:\n");
		for(BYTE cbIndex=0;cbIndex<BOMB_TYPE_COUNT;cbIndex++)
		{
			if(pGameEnd->cbBombList[i][cbIndex]>0)
				strTemp.AppendFormat(_T("----[%d��]X%d\n"),cbIndex+4,pGameEnd->cbBombList[i][cbIndex]);
		}
	}

	//��Ϣ����
 	if(m_pIStringMessage != NULL)
		m_pIStringMessage->InsertSystemString((LPCTSTR)strTemp);
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
		m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
	}

	//��ʾ�˿�
	if (IsLookonMode()==true) m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag(true);

	//��������
	WORD wMeChairID=GetMeChairID();
	LONGLONG lMeScore=pGameEnd->lGameScore[GetMeChairID()];	
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetPositively(false);

	//ȡ���й�
	if(m_bStustee)
		OnStusteeControl(0,0);
	
	//ȡ������
	m_bSortCount=false;

	//���ý���
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		m_GameClientView.m_lAllTurnScore[wViewChairID]=pGameEnd->lAllTurnScore[i];
		m_GameClientView.m_lLastTurnScore[wViewChairID]=pGameEnd->lLastTurnScore[i];
	}
	//��ʼ��Ϣ
	m_wGetRandCardID=INVALID_CHAIR;
	m_cbGetCardPosition=0xFF;
	m_cbRandCardData=0xFF;

	return true;
}

//�û��й�
bool CGameClientEngine::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) return false;
	//��Ϣ����
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
	m_GameClientView.SetTrustee(SwitchViewChairID(pTrustee->wChairID),pTrustee->bTrustee);
	if ((IsLookonMode()==true)||(pTrustee->wChairID!=GetMeChairID()))
	{
		IClientUserItem * pClientUserItem=GetTableUserItem(pTrustee->wChairID);
		TCHAR szBuffer[256];
		if(pTrustee->bTrustee==true)
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ѡ�����йܹ���."),pClientUserItem->GetNickName());
		else
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("���[%s]ȡ�����йܹ���."),pClientUserItem->GetNickName());
		if(m_pIStringMessage != NULL)
			m_pIStringMessage->InsertSystemString(szBuffer);
	}
	return true;
}


//�����ж�
bool CGameClientEngine::VerdictOutCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;
	if (m_wCurrentUser!=GetMeChairID()) return false;

	//��ȡ�˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbShootCount=(BYTE)m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].GetShootCard(cbCardData,CountArray(cbCardData));
	//�����ж�
	if (cbShootCount>0L)
	{
		memset(&m_OutCardInfo,0,sizeof(m_OutCardInfo));
		m_GameLogic.AnalysebCardData(cbCardData,cbShootCount,m_OutCardInfo);
		return m_GameLogic.CompareCard(m_OutCardInfo,m_LastOutCard);
	}
	return false;
}

//�����ж�
bool CGameClientEngine::VerdictPassCard()
{
	//�׳��ж�
	if (m_cbTurnCardCount==0) return false;


	return true;
}

//�Զ�����
bool CGameClientEngine::AutomatismOutCard()
{
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID())) return false;

	//��ǰ����
	CSkinButton & btOutCard=m_GameClientView.m_btOutCard;
	if ((btOutCard.IsWindowEnabled())&&(btOutCard.IsWindowVisible()))
	{
		OnOutCard(0,0);
		return true;
	}

	//���ƴ���
	if ((m_cbTurnCardCount==0))
	{
		//�������
		tagOutCardResult OutCardResult;
		m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount[GetMeChairID()],m_cbTurnCardData,m_cbTurnCardCount,OutCardResult);

		//���ý���
		if (OutCardResult.cbCardCount>0)
		{
			//���ý���
			m_GameClientView.m_btOutCard.EnableWindow(TRUE);
			m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);

			//���ƶ���
			OnOutCard(0,0);

			return true;
		}
	}

	//��������
	if (m_cbTurnCardCount>0) 
		OnPassCard(0,0);

	return true;
}

//ֹͣ����
bool CGameClientEngine::DoncludeDispatchCard()
{
	//ֹͣ����
	if (m_cbRemnantCardCount>0)
	{
		//ɾ��ʱ��
		KillTimer(IDI_DISPATCH_CARD);

		//�˿�����
		BYTE cbUserCard[MAX_COUNT];
		ZeroMemory(cbUserCard,sizeof(cbUserCard));

		//���ý���
		WORD wViewChairID=0;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			wViewChairID=SwitchViewChairID(i);
			if (wViewChairID ==ME_VIEW_CHAIR)
				m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_cbHandCardData,m_cbHandCardCount[i]);
			else
				m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbUserCard,MAX_COUNT);
		}
		//���ñ���
		m_cbRemnantCardCount=0;
		m_cbDispatchCardCount=0;
		ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));
	}
	return true;
}

//�ɷ��˿�
bool CGameClientEngine::DispatchUserCard(BYTE cbCardData[], BYTE cbCardCount)
{
	//���ñ���
	m_cbDispatchCardCount=0;
	m_cbRemnantCardCount=cbCardCount;
	CopyMemory(m_cbDispatchCardData,cbCardData,sizeof(BYTE)*cbCardCount);
	//���ö�ʱ��
	SetTimer(IDI_DISPATCH_CARD,100,NULL);
	return true;
}

//���ų�������
void CGameClientEngine::PlayBombSound(WORD wChairID, BYTE cbStarLevel)
{
    	//�ж��Ա�
		IClientUserItem * pClientUserItem=GetTableUserItem(wChairID);
		if(pClientUserItem==0||cbStarLevel<4||cbStarLevel>12)	return;
		CString strSoundName;
		strSoundName.Format(_T("%d"),cbStarLevel);
		if(pClientUserItem->GetGender()!=1)
			strSoundName = _T("BOY_") +strSoundName;
		else
			strSoundName = _T("GIRL_") + strSoundName;
		PlayGameSound(AfxGetInstanceHandle(), strSoundName);
	
}

//��ǰ�˿�
bool CGameClientEngine::SwitchToCurrentCard()
{
	//�����ж�
	if (m_bLastTurn==false) return false;

	//��������
	m_bLastTurn=false;
	KillTimer(IDI_LAST_TURN_CARD);
	m_GameClientView.SetLastTurnCard(false);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if(m_cbOutCardCount[i][0]!=255)
		{
			if(m_cbOutCardCount[i][0]==0)
			{
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
				m_GameClientView.SetPassFlag(wViewChairID,true);
			}
			else
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbOutCardData[i][0],m_cbOutCardCount[i][0]);
		}	
		else
		{
			m_GameClientView.SetPassFlag(wViewChairID,false);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
		}
	}

	return true;
}

//��ʱ����Ϣ
void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
		case IDI_WAIT_CLEAR:
			{
				//��������
				WORD wCurrentUser=m_wMostUser;
				m_wMostUser=INVALID_CHAIR;
				//ɾ����ʱ��
				KillTimer(IDI_WAIT_CLEAR);
				//���ý���
				m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
				for (WORD i=0;i<GAME_PLAYER;i++) 
					m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);

				//�������
				if ((IsLookonMode()==false)&&(wCurrentUser==GetMeChairID()))
				{
					ActiveGameFrame();
					m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
					m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
					m_GameClientView.m_btPassCard.EnableWindow(FALSE);
					m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
					m_GameClientView.m_btPassCard.EnableWindow((VerdictPassCard()==true)?TRUE:FALSE);
				}
				//����ʱ��
				SetGameClock(wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
				break;
			}
		case IDI_DISPATCH_CARD: OnTimerDispatchCard();break;
		case IDI_LAST_TURN_CARD:SwitchToCurrentCard();break;
		default:break;
	}
	__super::OnTimer(nIDEvent);
}

//��ʼ��ť
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{   

	//ʤ����Ϣ
	m_wWinOrder=0xFFFF;
	//���ñ���
	m_dwCardHSpace=DEFAULT_PELS;
	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	//���Ʊ���
	m_cbTurnCardCount=0;
	ZeroMemory(&m_LastOutCard,sizeof(tagAnalyseResult));
	ZeroMemory(m_cbTurnCardData,sizeof(m_cbTurnCardData));

	//���ý���
	KillGameClock(IDI_START_GAME);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	m_GameClientView.SetUserWinOrder(INVALID_CHAIR,0xFFFF);

	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

	//���ؿؼ�
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
	}
	m_GameClientView.m_HandCardControl[i].SetPositively(false);

	//������Ϣ
	SendUserReady(NULL,0);
	return 0;
}

//������Ϣ
LRESULT CGameClientEngine::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	m_bReqHumter=false;
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return 0;
	if (m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE) return 0;

	//���ý���
	KillGameClock(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btHunter.ShowWindow(SW_HIDE);
	
	//��ȡ�˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=(BYTE)m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].GetShootCard(cbCardData,CountArray(cbCardData));

	//�����˿�
	m_GameLogic.SortCardList(cbCardData,cbCardCount);

	//��������
	WORD wMeChairID=GetMeChairID();
	BYTE cbSourceCount=m_cbHandCardCount[wMeChairID];


	//ը���ж�
	if (m_OutCardInfo.m_cbCardType>=CT_BOMB)
		PlayBombSound(GetMeChairID(),m_OutCardInfo.m_cbBombGrade);
	else  
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetCardData(cbCardData,cbCardCount);


	//ɾ���˿�
	m_cbHandCardCount[wMeChairID]-=cbCardCount;
	m_GameLogic.RemoveCard(cbCardData,cbCardCount,m_cbHandCardData,cbSourceCount);
	BYTE cbHandCardData[MAX_COUNT];
	CopyMemory(cbHandCardData,m_cbHandCardData,sizeof(BYTE)*m_cbHandCardCount[wMeChairID]);
	m_GameLogic.SortCardList(cbHandCardData,m_cbHandCardCount[wMeChairID]);
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(cbHandCardData,m_cbHandCardCount[wMeChairID]);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardCount=cbCardCount;
	CopyMemory(OutCard.cbCardData,cbCardData,cbCardCount);
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(CMD_C_OutCard));

	//���½���
	m_GameClientView.RefreshGameView();
	return 0;
}

//��������
LRESULT CGameClientEngine::OnPassCard(WPARAM wParam, LPARAM lParam)
{

	m_bReqHumter=false;
	//״̬�ж�
	if (m_GameClientView.m_btPassCard.IsWindowVisible()==FALSE) return 0;
	if (m_GameClientView.m_btPassCard.IsWindowEnabled()==FALSE) return 0;

	//���ý���
	KillGameClock(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btHunter.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//��������
	SendSocketData(SUB_C_PASS_CARD);

	//Ԥ����ʾ
	m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetCardData(NULL,0);
	m_GameClientView.SetPassFlag(ME_VIEW_CHAIR,true);

	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));

	return 0;
}

//������ʾ
LRESULT CGameClientEngine::OnOutPrompt(WPARAM wParam, LPARAM lParam)
{
	//�������
	tagOutCardResult OutCardResult={0};
//	m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount[GetMeChairID()],m_cbMagicCardData,m_cbTurnCardCount,OutCardResult);

	//���ý���
	if (OutCardResult.cbCardCount>0)
	{
		//���ý���
		m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);

		//���ÿؼ�
		bool bOutCard=VerdictOutCard();
		m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);

		//���½���
		m_GameClientView.RefreshGameView();
	}
	else 
	{
		//��������
		OnPassCard(0,0);
	}
	return 0;
}


LRESULT CGameClientEngine::OnReqHunter(WPARAM wParam, LPARAM lParam)
{
	SendSocketData(SUB_C_REQ_HUNTERS,NULL,0);
	return 1;
}

//��Ŀ����
LRESULT CGameClientEngine::OnSortByCount(WPARAM wParam, LPARAM lParam)
{
	//����״̬
	if (m_bSortCount==true)
	{
		m_bSortCount=false;
//		m_GameClientView.m_btSortByCount.SetButtonImage(IDB_BT_SORT_BY_COUNT,AfxGetInstanceHandle(),false);
	}
	else
	{
		m_bSortCount=true;
//		m_GameClientView.m_btSortByCount.SetButtonImage(IDB_BT_SORT_BY_ORDER,AfxGetInstanceHandle(),false);
	}
	if (m_cbHandCardCount[GetMeChairID()]>0)
	{
		BYTE cbHandCardData[MAX_COUNT];
		CopyMemory(cbHandCardData,m_cbHandCardData,sizeof(m_cbHandCardData));
		if(m_bSortCount)
			m_GameLogic.SortCardList(cbHandCardData,m_cbHandCardCount[GetMeChairID()]);
		else
			m_GameLogic.SortCardList(cbHandCardData,m_cbHandCardCount[GetMeChairID()]);
		m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(cbHandCardData,m_cbHandCardCount[GetMeChairID()]);
		
		PlayGameSound(AfxGetInstanceHandle(),TEXT("SORT_CARD"));
	}
	return 0;
}


//�Ҽ��˿�
LRESULT CGameClientEngine::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	//���ÿؼ�
	bool bOutCard=VerdictOutCard();
	m_GameClientView.m_btOutCard.EnableWindow(bOutCard?TRUE:FALSE);
	//BYTE cbCardData[MAX_COUNT];
	//ZeroMemory(cbCardData,sizeof(cbCardData));
	//WORD byCardCount = m_GameClientView.m_HandCardControl[2].GetShootCard(cbCardData,MAX_COUNT);
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("SELECT"));

	////����˳��
	//if( byCardCount == 2 )
	//{
	//	tagOutCardResult OutCardResult;
	//	if( m_GameLogic.SearchLinkCard(m_cbHandCardData,m_cbHandCardCount[GetMeChairID()],cbCardData[0],cbCardData[1],OutCardResult))
	//	{
	//		m_GameClientView.m_HandCardControl[2].ShootAllCard(false);
	//		m_GameClientView.m_HandCardControl[2].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
	//	}
	//}

	////���ÿؼ�
	//bool bOutCard=VerdictOutCard();
	//m_GameClientView.m_btOutCard.EnableWindow(bOutCard?TRUE:FALSE);
 //   	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	return 0;
}

//����˿�
LRESULT CGameClientEngine::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{
	//�û�����
	OnOutCard(0,0);
	return 0;
}
//�Ϲܿ���
LRESULT CGameClientEngine::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
	//����״̬
	if (m_bStustee==true)
	{
		m_bStustee=false;
		m_wTimeOutCount=0;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);
		if(m_pIStringMessage != NULL)
			m_pIStringMessage->InsertSystemString(_T("��ȡ�����йܹ���."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = false;
		SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));

	}
	else
	{
		m_bStustee=true;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false,false);
		if(m_pIStringMessage != NULL)
			m_pIStringMessage->InsertSystemString(_T("��ѡ�����йܹ���."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = true;
		SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));
	}
	return 0;
}


//������ʾ
LRESULT CGameClientEngine::OnCardType(WPARAM wParam,LPARAM lParam)
{
	
	return 0;
}
//�����˿�
LRESULT CGameClientEngine::OnLastTurnCard(WPARAM wParam, LPARAM lParam)
{
	if (m_bLastTurn==false)
	{
		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			WORD wViewChairID=SwitchViewChairID(i);
			if(m_cbOutCardCount[i][1]!=255)
			{
				if(m_cbOutCardCount[i][1]==0)
				{
					m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
					m_GameClientView.SetPassFlag(wViewChairID,true);
				}
				else
					m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbOutCardData[i][1],m_cbOutCardCount[i][1]);
			}
			else
			{
				m_GameClientView.SetPassFlag(wViewChairID,false);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			}
		}

		//��������
		m_bLastTurn=true;
		m_GameClientView.SetLastTurnCard(true);
		m_GameClientView.RefreshGameView();
		SetTimer(IDI_LAST_TURN_CARD,1000,NULL);
	}
	else SwitchToCurrentCard();
	return 0;
}


//LRESULT CGameClientEngine::OnVoiceCreateFail(WPARAM wParam, LPARAM lParam)
//{
//	//m_pGlobalUnits->m_bAllowSound=false;
//	m_bHaveVoiceCard=false;
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
