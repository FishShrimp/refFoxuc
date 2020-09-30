#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////////////
//ʱ���ʶ

#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��
#define IDI_ANDROID_BET				1000	

//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_ADMIN_COMMDN, OnAdminControl)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
	//������Ϣ
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;		
	m_lApplyBankerCondition=0L;	

	//������ע
	ZeroMemory(m_lMeInAllScore,sizeof(m_lMeInAllScore));	

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;	

	//״̬����
	m_bMeApplyBanker=false;

	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��������
bool CGameClientEngine::OnInitGameEngine()
{
	//��������
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

	return true;
}

//���ú���
bool CGameClientEngine::OnResetGameEngine()
{
	//������Ϣ
	m_lMeMaxScore=0L;			

	//������ע
	ZeroMemory(m_lMeInAllScore,sizeof(m_lMeInAllScore));
	
	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;	
	
	return true;
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//��������
bool CGameClientEngine::AllowBackGroundSound(bool bAllowSound)
{
	if(bAllowSound)
		PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
	else
		StopSound();

	return true;
}

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{	
	switch (wClockID){
		case IDI_PLACE_JETTON:
			{
				if(nElapse==0)
				{
					//���ù��
					m_GameClientView.SetCurrentJetton(0L);

					//��ֹ��ť
					m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
					m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
					m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
					m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
					m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
					m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);


					//ׯ�Ұ�ť
					m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
					m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );
					
				}
				if (nElapse<=5) 
					PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
				return true;
			}
		case IDI_FREE:
			return true;
		case IDI_DISPATCH_CARD:
			return true;
	}
	return false;
}

//�Թ���Ϣ
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//��Ϸ��Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//��Ϸ����
		{
			return OnSubGameFree(pData,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//�û���ע
		{
			return OnSubPlaceJetton(pData,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//������ׯ
		{
			return OnSubUserApplyBanker(pData, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//ȡ����ׯ
		{
			return OnSubUserCancelBanker(pData, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	//�л�ׯ��
		{
			return OnSubChangeBanker(pData, wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//��Ϸ��¼
		{
			return OnSubGameRecord(pData,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			return OnSubPlaceJettonFail(pData,wDataSize);
		}
	case SUB_S_AMDIN_COMMAND:		//����
		{
			return OnSubAdminControl(pData,wDataSize);
		}	
	}

	//�������
	ASSERT(FALSE);

	return true;
}

bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
	SetGameStatus(GAME_STATUS_FREE);
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;



			//�����Ϣ
			m_lMeMaxScore=pStatusFree->lUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			WORD wMeChairID=GetMeChairID();
			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));

			//ׯ����Ϣ
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//������Ϣ
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);			

			//����
			PlayGameSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));

			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

			//����״̬

			ReSetGameCtr();

			//����ʱ��
			SetGameClock(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);

			if(IsLookonMode()==false && GetMeChairID() == m_wCurrentBanker)
			{
				m_bMeApplyBanker =true;
			}

			//���¿���
			UpdateButtonContron();
			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	case GAME_SCENE_GAME_END:	//��Ϸ״̬
	case GAME_SCENE_PLACE_JETTON:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			WORD wMeChairID=GetMeChairID();
			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

			//ȫ����ע
			m_GameClientView.PlaceUserJetton(AREA_LONG,pStatusPlay->lAreaInAllScore[AREA_LONG]);
			m_GameClientView.PlaceUserJetton(AREA_PING,pStatusPlay->lAreaInAllScore[AREA_PING]);
			m_GameClientView.PlaceUserJetton(AREA_HU,pStatusPlay->lAreaInAllScore[AREA_HU]);
			m_GameClientView.PlaceUserJetton(AREA_2_13,pStatusPlay->lAreaInAllScore[AREA_2_13]);
			m_GameClientView.PlaceUserJetton(AREA_14,pStatusPlay->lAreaInAllScore[AREA_14]);
			m_GameClientView.PlaceUserJetton(AREA_15_26,pStatusPlay->lAreaInAllScore[AREA_15_26]);
			m_GameClientView.PlaceUserJetton(AREA_2_6,pStatusPlay->lAreaInAllScore[AREA_2_6]);
			m_GameClientView.PlaceUserJetton(AREA_7_11,pStatusPlay->lAreaInAllScore[AREA_7_11]);
			m_GameClientView.PlaceUserJetton(AREA_12_16,pStatusPlay->lAreaInAllScore[AREA_12_16]);
			m_GameClientView.PlaceUserJetton(AREA_17_21,pStatusPlay->lAreaInAllScore[AREA_17_21]);
			m_GameClientView.PlaceUserJetton(AREA_22_26,pStatusPlay->lAreaInAllScore[AREA_22_26]);

			//�����ע
			SetMePlaceJetton(AREA_LONG,pStatusPlay->lUserInAllScore[AREA_LONG]);
			SetMePlaceJetton(AREA_PING,pStatusPlay->lUserInAllScore[AREA_PING]);
			SetMePlaceJetton(AREA_HU,pStatusPlay->lUserInAllScore[AREA_HU]);
			SetMePlaceJetton(AREA_2_13,pStatusPlay->lUserInAllScore[AREA_2_13]);
			SetMePlaceJetton(AREA_14,pStatusPlay->lUserInAllScore[AREA_14]);
			SetMePlaceJetton(AREA_15_26,pStatusPlay->lUserInAllScore[AREA_15_26]);
			SetMePlaceJetton(AREA_2_6,pStatusPlay->lUserInAllScore[AREA_2_6]);
			SetMePlaceJetton(AREA_7_11,pStatusPlay->lUserInAllScore[AREA_7_11]);
			SetMePlaceJetton(AREA_12_16,pStatusPlay->lUserInAllScore[AREA_12_16]);
			SetMePlaceJetton(AREA_17_21,pStatusPlay->lUserInAllScore[AREA_17_21]);
			SetMePlaceJetton(AREA_22_26,pStatusPlay->lUserInAllScore[AREA_22_26]);

			//��һ���
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));

			//������Ϣ
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			if (pStatusPlay->cbGameStatus==GAME_SCENE_GAME_END)
			{
				//�˿���Ϣ
				m_GameClientView.SetCardInfo(pStatusPlay->cbCardCount,pStatusPlay->cbTableCardArray);
				m_GameClientView.FinishDispatchCard();

				//���óɼ�
				m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
			}
			else
			{
				m_GameClientView.SetCardInfo(NULL,NULL);

				//��������
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
			}

			//����
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

			//ׯ����Ϣ
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			ReSetGameCtr();

			if(IsLookonMode()==false && GetMeChairID() == m_wCurrentBanker)
			{
				m_bMeApplyBanker =true;
			}
			//���°�ť
			UpdateButtonContron();			

			//����ʱ��
			SetGameClock(GetMeChairID(),pStatusPlay->cbGameStatus==GAME_SCENE_GAME_END?IDI_DISPATCH_CARD:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);
			//����״̬
			SetGameStatus(pStatusPlay->cbGameStatus);

			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//�����Ϣ
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	//����״̬
	SetGameStatus(GAME_SCENE_PLACE_JETTON);
	ReSetGameCtr();

	SetTimer(IDI_ANDROID_BET, 100, NULL);

	//���¿���
	UpdateButtonContron();

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);


	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	//����״̬
	SetGameStatus(GAME_STATUS_FREE);
	ReSetGameCtr();

	//��������
	m_GameClientView.SetWinnerSide(0xFF);
	m_GameClientView.CleanUserJetton();
	for (int nAreaIndex=0; nAreaIndex<AREA_ALL; ++nAreaIndex) 
		SetMePlaceJetton(nAreaIndex,0);

	//���¿ؼ�
	UpdateButtonContron();

	//��ɷ���
	m_GameClientView.FinishDispatchCard();

	//���³ɼ�
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; ++wUserIndex)
	{
		IClientUserItem *pUserItem=GetTableUserItem(wUserIndex);
		if(pUserItem == NULL) continue;
		tagUserInfo  *pUserData = pUserItem->GetUserInfo();
		if ( pUserData == NULL ) continue;
		tagApplyUser ApplyUser ;

		//������Ϣ
		ApplyUser.lUserScore = pUserData->lScore;
		ApplyUser.strUserName = pUserData->szNickName;
		m_GameClientView.m_ApplyUser.UpdateUser(ApplyUser);
	}

	return true;
}

//�û���ע
bool CGameClientEngine::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) return false;

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;

	if (GetMeChairID()!=pPlaceJetton->wChairID || IsLookonMode())
	{

		//��ע����
		//if(pPlaceJetton->cbAndroid == TRUE)
		//{
		//	//����
		//	static WORD wStFluc = 1;	//�������
		//	tagAndroidBet androidBet = {};
		//	androidBet.cbJettonArea = pPlaceJetton->cbJettonArea;
		//	androidBet.lJettonScore = pPlaceJetton->lJettonScore;
		//	androidBet.wChairID = pPlaceJetton->wChairID;
		//	androidBet.nLeftTime = ((rand()+androidBet.wChairID+wStFluc*3)%10+1)*100;
		//	wStFluc = wStFluc%3 + 1;

		//	m_ListAndroid.AddTail(androidBet);
		//}
		//else
		{
			if( CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&pPlaceJetton->cbAndroid)
			{
				m_GameClientView.PlaceUserJetton(1,0);
			}
			else
			{
				m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
			}			

			//��������

				if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
				{
					if (pPlaceJetton->lJettonScore==5000000L) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
					else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
					if(rand()%100 > 80)
					{
						switch (rand()%3)
						{
						case 0:
							PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER1"));
							break;
						case 1:
							PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER2"));
							break;
						case 2:
							PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER3"));
							break;
						}

					}	

			}
		}
	}

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

	//ȡ������
	for (WORD wAreaIndex=0; wAreaIndex<AREA_ALL; ++wAreaIndex) 
		m_GameClientView.SetBombEffect(false,wAreaIndex);

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

	//�˿���Ϣ
	m_GameClientView.SetCardInfo(pGameEnd->cbCardCount,pGameEnd->cbTableCardArray);

	//ׯ����Ϣ
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);

	//�ɼ���Ϣ
	m_GameClientView.SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);

	//����״̬
	SetGameStatus(GAME_SCENE_GAME_END);
	ReSetGameCtr();


	//����ʱ��
	KillTimer(IDI_ANDROID_BET);

	//���¿ؼ�
	UpdateButtonContron();

	//ֹͣ����
	//for (int i=0; i<CountArray(m_DTSDCheer); ++i) m_DTSDCheer[i].Stop();

	return true;
}

//���¿���
void CGameClientEngine::UpdateButtonContron()
{
	//�����ж�
	bool bEnablePlaceJetton=true;
	if (m_bEnableSysBanker==false&&m_wCurrentBanker==INVALID_CHAIR) bEnablePlaceJetton=false;
	if (GetGameStatus()!=GAME_SCENE_PLACE_JETTON) bEnablePlaceJetton=false;
	if (m_wCurrentBanker==GetMeChairID()) bEnablePlaceJetton=false;
	if (IsLookonMode()) bEnablePlaceJetton=false;

	//��ע��ť
	if (bEnablePlaceJetton==true)
	{
		//�������
		LONGLONG lCurrentJetton=m_GameClientView.GetCurrentJetton();
		LONGLONG lLeaveScore=m_lMeMaxScore;

		for ( int i = 0; i < AREA_ALL; ++i)
		{
			lLeaveScore -= m_lMeInAllScore[i];
		}

		//���ù��
		if (lCurrentJetton>lLeaveScore)
		{
			if (lLeaveScore>=5000000L) m_GameClientView.SetCurrentJetton(5000000L);
			else if (lLeaveScore>=1000000L) m_GameClientView.SetCurrentJetton(1000000L);
			else if (lLeaveScore>=100000L) m_GameClientView.SetCurrentJetton(100000L);
			else if (lLeaveScore>=10000L) m_GameClientView.SetCurrentJetton(10000L);
			else if (lLeaveScore>=1000L) m_GameClientView.SetCurrentJetton(1000L);
			else if (lLeaveScore>=100L) m_GameClientView.SetCurrentJetton(100L);
			else m_GameClientView.SetCurrentJetton(0L);
		}

		//���ư�ť
		m_GameClientView.m_btJetton100.EnableWindow((lLeaveScore>=100)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow((lLeaveScore>=1000)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow((lLeaveScore>=10000)?TRUE:FALSE);
		m_GameClientView.m_btJetton100000.EnableWindow((lLeaveScore>=100000)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow((lLeaveScore>=1000000)?TRUE:FALSE);		
		m_GameClientView.m_btJetton5000000.EnableWindow((lLeaveScore>=5000000)?TRUE:FALSE);
	}
	else
	{
		//���ù��
		m_GameClientView.SetCurrentJetton(0L);

		//��ֹ��ť
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton5000000.EnableWindow(FALSE);
	}

	//ׯ�Ұ�ť
	if (!IsLookonMode())
	{
		//��ȡ��Ϣ
		const tagUserInfo *pMeUserData=GetMeUserItem()->GetUserInfo();

		//���밴ť
		bool bEnableApply=true;
		if (m_wCurrentBanker==GetMeChairID()) bEnableApply=false;
		if (m_bMeApplyBanker) bEnableApply=false;
		if (pMeUserData->lScore<m_lApplyBankerCondition) bEnableApply=false;
		m_GameClientView.m_btApplyBanker.EnableWindow(bEnableApply?TRUE:FALSE);

		//ȡ����ť
		bool bEnableCancel=true;
		if (m_wCurrentBanker==GetMeChairID() && GetGameStatus()!=GAME_STATUS_FREE) bEnableCancel=false;
		if (m_bMeApplyBanker==false) bEnableCancel=false;
		m_GameClientView.m_btCancelBanker.EnableWindow(bEnableCancel?TRUE:FALSE);

		m_GameClientView.m_btCancelBanker.SetButtonImage(m_wCurrentBanker==GetMeChairID()?IDB_BT_CANCEL_BANKER:IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false,false);

		//��ʾ�ж�
		if (m_bMeApplyBanker)
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_SHOW);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);
			
		}
		else
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);			
		}
	}
	else
	{
		m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
	}

	//��ȡ��Ϣ
	const tagUserInfo *pMeUserData=GetMeUserItem()->GetUserInfo();

	//���а�ť
	m_GameClientView.m_btBankerDraw.EnableWindow(TRUE);
	m_GameClientView.m_btBankerStorage.EnableWindow(FALSE);
	m_GameClientView.m_btBankerStorage.ShowWindow(SW_SHOW);
	m_GameClientView.m_btBankerStorage.EnableWindow((GetGameStatus()==GAME_STATUS_FREE || IsLookonMode())?TRUE:FALSE);
	if(m_wCurrentBanker==GetMeChairID()&&IsLookonMode()==false)
	{
		m_GameClientView.m_btBankerStorage.EnableWindow(FALSE);
	}
	if (m_wCurrentBanker==GetMeChairID()) m_GameClientView.m_blCanStore = false;
	else  m_GameClientView.m_blCanStore = true;
	m_GameClientView.SetInsureOption(false, m_GameClientView.m_blCanStore);
	ReSetGameCtr();
	return;
}

//��ע��Ϣ
LRESULT CGameClientEngine::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbJettonArea=(BYTE)wParam;
	LONGLONG lJettonScore=(LONGLONG)(*((LONGLONG*)lParam));

	//ׯ���ж�
	if ( GetMeChairID() == m_wCurrentBanker ) return true;

	//״̬�ж�
	if (GetGameStatus()!=GAME_SCENE_PLACE_JETTON)
	{
		UpdateButtonContron();
		return true;
	}

	m_lMeInAllScore[cbJettonArea] += lJettonScore;
	m_GameClientView.SetMePlaceJetton(cbJettonArea,m_lMeInAllScore[cbJettonArea]);

	//��������
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//�������
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=lJettonScore;

	//������Ϣ
	SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	//���°�ť
	UpdateButtonContron();

	//Ԥ����ʾ
	m_GameClientView.PlaceUserJetton(cbJettonArea,lJettonScore);

	//��������
	if (lJettonScore==5000000L) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
	if(rand()%100 > 80)
	{
		switch (rand()%3)
		{
		case 0:
			PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER1"));
			break;
		case 1:
			PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER2"));
			break;
		case 2:
			PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER3"));
			break;
		}
	}	
	

	return 0;
}

//������Ϣ
LRESULT CGameClientEngine::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	tagUserInfo const *pMeUserData = GetMeUserItem()->GetUserInfo();
	if (pMeUserData->lScore < m_lApplyBankerCondition) return true;	

	//�Թ��ж�
	if (IsLookonMode()) return true;

	//ת������
	bool bApplyBanker = wParam ? true:false;
	
	//��ǰ�ж�
	if (m_wCurrentBanker == GetMeChairID() && bApplyBanker) return true;	

	if (bApplyBanker)
	{
		//������Ϣ
		SendSocketData(SUB_C_APPLY_BANKER, NULL, 0);		

		m_bMeApplyBanker=true;
	}
	else
	{		
		//������Ϣ
		SendSocketData(SUB_C_CANCEL_BANKER, NULL, 0);		

		m_bMeApplyBanker=false;
	}

	//���ð�ť
	UpdateButtonContron();
	m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
	m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);

	return true;
}

//������ׯ
bool CGameClientEngine::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	IClientUserItem *pUserItem=GetTableUserItem(pApplyBanker->wApplyUser);

	//��ȡ���
	tagUserInfo  *pUserData=NULL;
	if(pUserItem!=NULL)
		pUserData=pUserItem->GetUserInfo();

	//�������
	if (m_wCurrentBanker!=pApplyBanker->wApplyUser && pUserData!=NULL)
	{
		tagApplyUser ApplyUser;
		ApplyUser.strUserName=pUserData->szNickName;
		ApplyUser.lUserScore=pUserData->lScore;
		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
		m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);
	}

	//�Լ��ж�
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) m_bMeApplyBanker=true;	

	//���¿ؼ�
	UpdateButtonContron();
	m_GameClientView.m_btCancelBanker.EnableWindow(TRUE);
	return true;
}

//ȡ����ׯ
bool CGameClientEngine::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	//ɾ�����
	tagApplyUser ApplyUser;
	ApplyUser.strUserName=pCancelBanker->szCancelUser;
	ApplyUser.lUserScore=0;
	m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
	m_GameClientView.m_ApplyUser.m_AppyUserList.Invalidate(TRUE);

	//�Լ��ж�
	const tagUserInfo *pMeUserData=GetMeUserItem()->GetUserInfo();
	if (IsLookonMode()==false && lstrcmp(pMeUserData->szNickName,pCancelBanker->szCancelUser)==0) m_bMeApplyBanker=false;	

	//���¿ؼ�
	UpdateButtonContron();
	m_GameClientView.m_btApplyBanker.EnableWindow(TRUE);
	return true;
}

//�л�ׯ��
bool CGameClientEngine::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;

	//��ʾͼƬ
	m_GameClientView.ShowChangeBanker(m_wCurrentBanker!=pChangeBanker->wBankerUser);

	//�Լ��ж�
	if (m_wCurrentBanker==GetMeChairID() && IsLookonMode() == false && pChangeBanker->wBankerUser!=GetMeChairID()) 
	{
		m_bMeApplyBanker=false;
	}
	else if (IsLookonMode() == false && pChangeBanker->wBankerUser==GetMeChairID())
	{
		m_bMeApplyBanker=true;
	}

	//ׯ����
	SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->lBankerScore);
	m_GameClientView.SetBankerScore(0,0);

	//ɾ�����
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		tagUserInfo const *pBankerUserData=GetTableUserItem(m_wCurrentBanker)->GetUserInfo();
		if (pBankerUserData != NULL)
		{
			tagApplyUser ApplyUser;
			ApplyUser.strUserName = pBankerUserData->szNickName;
			m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
		}
	}

	//���½���
	UpdateButtonContron();

	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//��Ϸ��¼
bool CGameClientEngine::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize%sizeof(tagServerGameRecord)!=0) return false;

	//�������
	tagGameRecord GameRecord;
	ZeroMemory(&GameRecord,sizeof(GameRecord));

	//���ü�¼
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);

		m_GameClientView.SetGameHistory(enOperateResult_NULL, pServerGameRecord->cbResult,pServerGameRecord->cbLong,pServerGameRecord->cbHu);
	}

	return true;
}

//��עʧ��
bool CGameClientEngine::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) return false;

	//��Ϣ����
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;

	//Ч�����
	BYTE cbViewIndex=pPlaceJettonFail->lJettonArea;
	ASSERT(cbViewIndex < AREA_ALL);
	if (cbViewIndex >= AREA_ALL) return false;

	//��ע����
	m_GameClientView.PlaceUserJetton(pPlaceJettonFail->lJettonArea,-pPlaceJettonFail->lPlaceScore);

	//�Լ��ж�
	if (GetMeChairID()==pPlaceJettonFail->wPlaceUser && false==IsLookonMode())
	{

		LONGLONG lJettonCount=pPlaceJettonFail->lPlaceScore;

		ASSERT(m_lMeInAllScore[cbViewIndex]>=lJettonCount);
		if (lJettonCount>m_lMeInAllScore[cbViewIndex]) 
			return false;

		m_lMeInAllScore[cbViewIndex] -= lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lMeInAllScore[cbViewIndex]);
	}
	return true;
}

//����ׯ��
void CGameClientEngine::SetBankerInfo(WORD wBanker,LONGLONG lScore)
{
	m_wCurrentBanker = wBanker;
	m_lBankerScore = lScore;
	WORD wBankerViewChairID = m_wCurrentBanker == INVALID_CHAIR ? INVALID_CHAIR:SwitchViewChairID(m_wCurrentBanker);
	m_GameClientView.SetBankerInfo(wBankerViewChairID,m_lBankerScore);
}

//������ע
void CGameClientEngine::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex < AREA_ALL);
	if (cbViewIndex >= AREA_ALL) return;

	m_lMeInAllScore[cbViewIndex] = lJettonCount;

	//���ý���
	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}
//////////////////////////////////////////////////////////////////////////


void CGameClientEngine::ReSetGameCtr()
{
#ifdef __BANKER___
	if(!IsLookonMode())
	{
		switch (GetGameStatus())
		{
		case GAME_STATUS_FREE:
			{
				if(GetMeChairID() != m_wCurrentBanker)
				{					
					m_GameClientView.m_btBankerDraw.EnableWindow(TRUE);
					m_GameClientView.m_btBankerStorage.EnableWindow(TRUE);
				}
				else
				{					
					m_GameClientView.m_btBankerDraw.EnableWindow(TRUE);
					m_GameClientView.m_btBankerStorage.EnableWindow(FALSE);
				}
				break;
			}
		case GAME_SCENE_GAME_END:
		case GAME_SCENE_PLACE_JETTON:
			{
				if(GetMeChairID() != m_wCurrentBanker)
				{					
					m_GameClientView.m_btBankerDraw.EnableWindow(TRUE);
					m_GameClientView.m_btBankerStorage.EnableWindow(FALSE);
				}
				else
				{					
					m_GameClientView.m_btBankerDraw.EnableWindow(FALSE);
					m_GameClientView.m_btBankerStorage.EnableWindow(FALSE);
				}
				break;
			}
		default:break;
		}
	}
	else
	{
		m_GameClientView.m_btBankerDraw.EnableWindow(TRUE);
		m_GameClientView.m_btBankerStorage.EnableWindow(TRUE);
	}
#endif
}

//����
bool CGameClientEngine::OnSubAdminControl(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_CommandResult));
	if(wDataSize!=sizeof(CMD_S_CommandResult)) return false;

	m_GameClientView.m_pClientControlDlg->ReqResult(pBuffer);
	return true;
}

//����
LRESULT CGameClientEngine::OnAdminControl( WPARAM wParam, LPARAM lParam )
{
	SendSocketData(SUB_C_AMDIN_COMMAND,(CMD_C_AdminReq*)wParam,sizeof(CMD_C_AdminReq));
	return true;
}


void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == IDI_ANDROID_BET)
	{
		if (m_ListAndroid.GetCount() != 0)
		{
			POSITION pos = m_ListAndroid.GetTailPosition();
			POSITION posTmp = pos;

			//������ע����
			while (true)
			{
				if (pos == 0)
					break;

				posTmp = pos;
				tagAndroidBet & androidBet = m_ListAndroid.GetPrev(pos);

				androidBet.nLeftTime -= 100;
				if (androidBet.nLeftTime <= 0)
				{		
					//ģ����Ϣ
					CMD_S_PlaceJetton placeJetton = {};
					placeJetton.cbAndroid = FALSE;
					placeJetton.cbJettonArea = androidBet.cbJettonArea;
					placeJetton.lJettonScore = androidBet.lJettonScore;
					placeJetton.wChairID = androidBet.wChairID;

					OnSubPlaceJetton((void*)&placeJetton, sizeof(placeJetton));

					//ɾ��Ԫ��
					m_ListAndroid.RemoveAt(posTmp);
				}
			}
		}
	}
	CGameFrameEngine::OnTimer(nIDEvent);
}
//////////////////////////////////////////////////////////////////////////////////
