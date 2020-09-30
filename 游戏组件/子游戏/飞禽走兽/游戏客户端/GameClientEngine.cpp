 #include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientEngine.h"
#include ".\GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��
#define IDI_OPEN_CARD				302								    //����ʱ��

#define IDI_ANDROID_BET				1000	


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_MESSAGE(IDM_PLACE_JETTON,OnPlaceJetton)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
	ON_MESSAGE(IDM_SOUND,OnPlaySound)
	ON_MESSAGE(IDM_ADMIN_COMMDN, OnAdminControl)	
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//���캯��
CGameClientEngine::CGameClientEngine() 
{
	//������Ϣ
	m_lMeMaxScore=0L;			
	m_lAreaLimitScore=0L;		
	m_lApplyBankerCondition=0L;	
	m_GameEndTime = 0;

	m_blUsing = false;



	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lPlayerBetAll,sizeof(m_lPlayerBetAll));

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	
	//״̬����
	m_bMeApplyBanker=false;

	//������
	for ( int i = 0; i < CountArray(m_nAnimalPercent); ++i)
		m_nAnimalPercent[i] = 1;

	m_bPlaceEnd = false;
	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{
	
	CGlobalUnits * m_pGlobalUnits=(CGlobalUnits *)CGlobalUnits::GetInstance();
	ASSERT(m_pGlobalUnits!=NULL);

	//���ñ���
	//SetWindowText(TEXT("����������Ϸ  --  Ver��6.6.0.3"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);
	return true;
}

//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
	//������Ϣ
	m_lMeMaxScore=0L;			

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));
	ZeroMemory(m_lPlayerBetAll,sizeof(m_lPlayerBetAll));

	//ׯ����Ϣ
	m_lBankerScore=0L;
	m_wCurrentBanker=0L;
	//״̬����
	m_bMeApplyBanker=false;

	return true;
}

//��Ϸ����
void CGameClientEngine::OnGameOptionSet()
{

	return;
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//�Թ�״̬
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
	if ((nTimerID==IDI_PLACE_JETTON)&&(nElapse==0))
	{
		//��ֹ��ť
		/*m_GameClientView.m_btJetton10.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		*/


		//ׯ�Ұ�ť
		//m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
		//m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );
	}
	//if(IDI_DISPATCH_CARD == nTimerID)
	//{
		/*m_GameClientView.StartHandle_Leave();
		KillTimer(IDI_DISPATCH_CARD);

		if ( m_GameEndTime <= 0 || m_GameEndTime > 100)
			m_GameEndTime = 2;
		
		SetGameClock(GetMeChairID(),IDI_OPEN_CARD ,m_GameEndTime-1);*/
	//	return false;
	//}
	if (IDI_DISPATCH_CARD == nTimerID&&(nElapse==0))
	{
		m_GameClientView.SetAnimalPercent(m_nAnimalPercent);
	}

	
	
	if (nTimerID==IDI_PLACE_JETTON&&nElapse<=5) 
		PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
	

	
	
	if (nTimerID==IDI_PLACE_JETTON)
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
	

	return true;
}

////�Թ�״̬
//void CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
//{
//}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//��Ϸ����
		{
			return OnSubGameFree(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//�û���ע
		{
			return OnSubPlaceJetton(pBuffer,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//������ׯ
		{
			return OnSubUserApplyBanker(pBuffer, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//ȡ����ׯ
		{
			return OnSubUserCancelBanker(pBuffer, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	//�л�ׯ��
		{
			return OnSubChangeBanker(pBuffer, wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//��Ϸ��¼
		{
			return OnSubGameRecord(pBuffer,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			return OnSubPlaceJettonFail(pBuffer,wDataSize);
		}
	/*case SUB_S_CHECK_IMAGE:
		{
			return OnSubCheckImageIndex(pBuffer,wDataSize);
		}*/
	case SUB_S_ADMIN_COMMDN:		//����
		{
			return OnSubAdminControl(pBuffer,wDataSize);
		}	
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, VOID * pBuffer, WORD wDataSize)
{

	switch (cbGameStation)
	{
	case GAME_STATUS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;

            //�����Ϣ
			m_lMeMaxScore=pStatusFree->lUserMaxScore;
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			//IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());

			m_GameClientView.SetMeChairID(pMeUserData->GetUserID());

            //ׯ����Ϣ
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->lBankerWinScore);
		
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //������Ϣ
			m_lApplyBankerCondition=pStatusFree->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusFree->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);


			//���ü���
			memcpy(m_nAnimalPercent, pStatusFree->nAnimalPercent, sizeof(m_nAnimalPercent));
			m_GameClientView.SetAnimalPercent(m_nAnimalPercent);

			//����
			//if((GetTableUserItem(GetMeChairID())->dwUserRight&UR_GAME_CONTROL)!=0&&m_GameClientView.m_pAdminControl!=NULL)
			//	m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

			//����״̬
			SetGameStatus(GAME_STATUS_FREE);

			//���¿���
			UpdateButtonContron();
			m_GameClientView.RefreshGameView();

			//����ʱ��
			SetGameClock(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);
			
			//��������
			//PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));

			return true;
		}
	case GS_PLACE_JETTON:	//��Ϸ״̬
	case GS_GAME_END:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			
			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//��ע��Ϣ
			for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
			{
				m_GameClientView.PlaceUserJetton(nAreaIndex,pStatusPlay->lAllJettonScore[nAreaIndex]);
				SetMePlaceJetton(nAreaIndex,pStatusPlay->lUserJettonScore[nAreaIndex]);
			}

			//��һ���
			m_lMeMaxScore=pStatusPlay->lUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_lMeMaxScore);
			IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
			m_GameClientView.SetMeChairID(pMeUserData->GetUserID());

			//������Ϣ
			m_lApplyBankerCondition=pStatusPlay->lApplyBankerCondition;
			m_lAreaLimitScore=pStatusPlay->lAreaLimitScore;
			m_GameClientView.SetAreaLimitScore(m_lAreaLimitScore);

			if (pStatusPlay->cbGameStatus==GS_GAME_END)
			{
				//�˿���Ϣ
				m_GameClientView.SetCardInfo(pStatusPlay->cbTableCardArray);
				m_GameClientView.FinishDispatchCard(false);

				//���óɼ�
				m_GameClientView.SetCurGameScore(pStatusPlay->lEndUserScore,pStatusPlay->lEndUserReturnScore,pStatusPlay->lEndBankerScore,pStatusPlay->lEndRevenue);
			}
			else
			{
				m_GameClientView.SetCardInfo(NULL);
				m_blUsing = true;
				
			}

			//��������
			//PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));

			//ׯ����Ϣ
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->lBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->lBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//���ü���
			memcpy(m_nAnimalPercent, pStatusPlay->nAnimalPercent, sizeof(m_nAnimalPercent));
			m_GameClientView.SetAnimalPercent(m_nAnimalPercent);

			//����
			//if((GetTableUserItem(GetMeChairID())->dwUserRight&UR_GAME_CONTROL)!=0&&m_GameClientView.m_pAdminControl!=NULL)
			//	m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

			//����״̬
			SetGameStatus(pStatusPlay->cbGameStatus);

			//����ʱ��
			UINT nTimerID = pStatusPlay->cbGameStatus==GS_GAME_END ? IDI_OPEN_CARD : IDI_PLACE_JETTON;
			SetGameClock(GetMeChairID(), nTimerID, pStatusPlay->cbTimeLeave);

			m_GameClientView.StartRandShowSide();

			//���°�ť
			UpdateButtonContron();
			m_GameClientView.RefreshGameView();

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

	m_bPlaceEnd = false;
	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	m_GameClientView.SwithToNormalView();

	m_GameClientView.KillCardTime();

	m_blUsing = true;

	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->lBankerScore);

	//�����Ϣ
	m_lMeMaxScore=pGameStart->lUserMaxScore;
	m_GameClientView.SetMeMaxScore(m_lMeMaxScore);

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	

	//����״̬
	SetGameStatus(GS_PLACE_JETTON);

	m_GameClientView.StartRandShowSide();

	SetTimer(IDI_ANDROID_BET, 100, NULL);

	//���¿���
	UpdateButtonContron();

	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	OnPlaySound(0,0);
	
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

	m_blUsing = true;

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	//m_GameClientView.StartMove();

	//����״̬
	SetGameStatus(GAME_STATUS_FREE);

	//����ʱ��
	KillTimer(IDI_ANDROID_BET);

	////��������
	bool blWin[AREA_COUNT];
	for (int i = 0;i<AREA_COUNT;i++){

			blWin[i]=false;
	}

	m_GameClientView.FinishDispatchCard();

	m_GameClientView.SetWinnerSide(blWin, false);



	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) SetMePlaceJetton(nAreaIndex,0);

	m_GameClientView.CleanUserJetton();
	
	
	m_GameClientView.SwitchToCheck();

	//���¿ؼ�
	UpdateButtonContron();

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


		////��ע����
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
			m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);

			//��������
			//if (IsEnableSound()) 
			{
				if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
				{
					if (pPlaceJetton->lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
					else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));

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
	UpdateButtonContron();
    
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

	m_GameClientView.m_blMoveFinish = false;

	
	m_GameEndTime = pGameEnd->cbTimeLeave;

	m_blUsing = true;

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

	//�˿���Ϣ
	m_GameClientView.SetCardInfo(pGameEnd->cbTableCardArray);
	m_GameClientView.ClearAreaFlash();

	m_GameClientView.SetShaYuAddMulti(pGameEnd->cbShaYuAddMulti);
	
	m_GameClientView.StartRunCar(20);

	//���漸��
	memcpy(m_nAnimalPercent, pGameEnd->nAnimalPercent, sizeof(m_nAnimalPercent));

	

	//����״̬
	SetGameStatus(GS_GAME_END);

	//ׯ����Ϣ
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->lBankerTotallScore);
	//�ɼ���Ϣ
	m_GameClientView.SetCurGameScore(pGameEnd->lUserScore,pGameEnd->lUserReturnScore,pGameEnd->lBankerScore,pGameEnd->lRevenue);

	m_GameClientView.PerformAllBetAnimation();
	
	//���¿ؼ�
	UpdateButtonContron();

	////ֹͣ����
	//for (int i=0; i<CountArray(m_DTSDCheer); ++i) m_DTSDCheer[i].Stop();

	return true;
}

//���¿���
void CGameClientEngine::UpdateButtonContron()
{
	
	//�����ж�
	bool bEnablePlaceJetton=true;

	if(m_wCurrentBanker==INVALID_CHAIR)
	{
		bEnablePlaceJetton = true;

	}
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		bEnablePlaceJetton=false;

	}
	if (m_wCurrentBanker==GetMeChairID()) 
	{
		bEnablePlaceJetton=false;
	}
	if (IsLookonMode())
	{
		bEnablePlaceJetton=false;
	}
	if (m_bEnableSysBanker==false&&m_wCurrentBanker==INVALID_CHAIR) 
	{
		bEnablePlaceJetton=false;
	}

	//��ע��ť
	if (bEnablePlaceJetton==true)
	{
		
		//�������
		LONGLONG lCurrentJetton=m_GameClientView.GetCurrentJetton();
		LONGLONG lLeaveScore=m_lMeMaxScore;
		for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lLeaveScore -= m_lUserJettonScore[nAreaIndex];

		//�����ע
		LONGLONG lUserMaxJetton = 0;

		for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
		{
			if(lUserMaxJetton==0&&nAreaIndex == 1)
			{
				lUserMaxJetton = m_GameClientView.GetUserMaxJetton(nAreaIndex);

			}else
			{
				if(m_GameClientView.GetUserMaxJetton(nAreaIndex)>lUserMaxJetton)
				{
					lUserMaxJetton = m_GameClientView.GetUserMaxJetton(nAreaIndex);
				}
			}
		}
		lLeaveScore = min((lLeaveScore),lUserMaxJetton); //�û����·� �����ֱȽ� �������屶 

		//���ư�ť
		int iTimer = 1;

		if(m_blUsing==false)
		{
			lLeaveScore = 0;
			lUserMaxJetton = 0;

		}
		m_GameClientView.m_btJetton10.EnableWindow((lLeaveScore>=10*iTimer && lUserMaxJetton>=10*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton100.EnableWindow((lLeaveScore>=100*iTimer && lUserMaxJetton>=100*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow((lLeaveScore>=1000*iTimer && lUserMaxJetton>=1000*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow((lLeaveScore>=10000*iTimer && lUserMaxJetton>=10000*iTimer)?TRUE:FALSE);
		m_GameClientView.m_btJetton100000.EnableWindow((lLeaveScore>=100000*iTimer && lUserMaxJetton>=100000*iTimer)?TRUE:FALSE);	


		//���ٳ���
		if (lCurrentJetton>lLeaveScore)
		{
			m_GameClientView.ReduceJettonNumber();
		}		
		
	}
	else
	{
		
		//��ֹ��ť
		
		m_GameClientView.m_btJetton10.EnableWindow(FALSE);		
		m_GameClientView.m_btJetton100.EnableWindow(FALSE);
		m_GameClientView.m_btJetton1000.EnableWindow(FALSE);
		m_GameClientView.m_btJetton10000.EnableWindow(FALSE);
	
		m_GameClientView.m_btJetton100000.EnableWindow(FALSE);
		

		
	}

	//��ȡ��Ϣ
	IClientUserItem *pMeUserData=GetTableUserItem(GetMeChairID());
	//���а�ť
	m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
	m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
	m_GameClientView.m_btBankStorage.ShowWindow(SW_SHOW);
	m_GameClientView.m_btBankStorage.EnableWindow((GetGameStatus()==GAME_STATUS_FREE || IsLookonMode())?TRUE:FALSE);
	if(m_wCurrentBanker==GetMeChairID()&&IsLookonMode()==false)
	{
		m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
	}
	
	if (m_wCurrentBanker==GetMeChairID()) m_GameClientView.m_blCanStore = false;
	else  m_GameClientView.m_blCanStore = true;
	//m_GameClientView.SetInsureOption(false, m_GameClientView.m_blCanStore);

	ReSetBankCtrol(GetGameStatus());

	return;
}

//��ע��Ϣ
LRESULT CGameClientEngine::OnPlaceJetton(WPARAM wParam, LPARAM lParam)
{
	


	//��������
	BYTE cbJettonArea=(BYTE)wParam;
	LONGLONG lJettonScore=(LONGLONG)(*((LONGLONG*)lParam));

	//�Ϸ��ж�
	ASSERT(cbJettonArea>=1 && cbJettonArea<=AREA_COUNT);
	if (!(cbJettonArea>=1 && cbJettonArea<=AREA_COUNT)) return 0;

	//ׯ���ж�
	if ( GetMeChairID() == m_wCurrentBanker ) return true;


	//״̬�ж�
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		UpdateButtonContron();
		return true;
	}

	//���ñ���
	m_lUserJettonScore[cbJettonArea] += lJettonScore;
	m_GameClientView.SetMePlaceJetton(cbJettonArea, m_lUserJettonScore[cbJettonArea]);



	//��������
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//�������
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.lJettonScore=lJettonScore;

	//������Ϣ
	if(!SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton)))
	{

		return 0;
	};

	//���°�ť
	UpdateButtonContron();

	//Ԥ����ʾ
	m_GameClientView.PlaceUserJetton(cbJettonArea,lJettonScore);

	//��������
	//if (IsEnableSound()) 
	{
		if (lJettonScore==5000000) PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
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



	//ׯ���ж�
	//if ( GetMeChairID() == m_wCurrentBanker ) return true;

	////״̬�ж�
	//if (GetGameStatus()!=GS_PLACE_JETTON)
	//{
	//	UpdateButtonContron();
	//	return true;
	//}
	//

	////�������
	//CMD_C_PlaceJetton* pPlayerBet = (CMD_C_PlaceJetton*)lParam;

	////��֤��ע
	//for ( int i = 0 ; i < AREA_COUNT ; ++i)
	//{
	//	if ( pPlayerBet->lBetScore[i] > GetMeMaxBet(i) )
	//	{
	//		//��עʧ����Ϣ
	//		CDialogMessage Message;
	//		Message.SetMessage(TEXT("��ע����ϵͳ���ƽ���������ע��"));
	//		Message.DoModal();
	//		return 0;
	//	}
	//}

	//
	////������Ϣ
	//SendSocketData( SUB_C_PLACE_JETTON, pPlayerBet, sizeof(CMD_C_PlaceJetton));


	////������ע
	//WORD wMeChairId = GetMeChairID();
	//WORD wViewChairID = SwitchViewChairID(wMeChairId);
	//for( int i = 0 ; i < AREA_COUNT ; ++i )
	//{
	//	
	//	m_lUserJettonScore[i] += pPlayerBet->lBetScore[i];
	//	m_lPlayerBetAll[i] += pPlayerBet->lBetScore[i];
	//}

	////���°�ť
	//UpdateButtonContron();

	//m_GameClientView.RefreshGameView();

	//
	//switch (rand()%3)
	//{
	//case 0:
	//	PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER1"));
	//	break;
	//case 1:
	//	PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER2"));
	//	break;
	//case 2:
	//	PlayGameSound(AfxGetInstanceHandle(),TEXT("CHEER3"));
	//	break;
	//}
	//

	//
	//m_bPlaceEnd = true;

	////CDialogMessage Message;
	////Message.SetMessage(TEXT("��ע��Ϣ���ύ����ȴ���Ϸ������"));
	////Message.DoModal();
	//m_pIStringMessage->InsertCustomString(TEXT("��ע��Ϣ���ύ����ȴ���"),RGB(255,0,0));

	return 0;

}


//�Լ�����·�
LONGLONG CGameClientEngine::GetMeMaxBet( BYTE cbArea )
{
	//�ҳ����
	IClientUserItem * pUserData = GetTableUserItem(GetMeChairID());
	if ( pUserData == NULL || cbArea > AREA_COUNT ) 
		return 0l;

	//��ҷ���
	LONGLONG lUserScore = pUserData->GetUserScore();

	//��ȥ�Լ�����ע
	for(int i = 0; i < AREA_COUNT; ++i)
	{
		lUserScore -= m_lUserJettonScore[i];
	}

	//�����������ע. ��Ҫ�ж���������
	if ( cbArea < AREA_COUNT )
	{
		//����������
		LONGLONG lAreaLimit = m_lAreaLimitScore - m_lPlayerBetAll[cbArea];
		lUserScore = __min( lUserScore, lAreaLimit);
	}

	//�쳣��������
	ASSERT( lUserScore >= 0l );
	if( lUserScore < 0 )
		lUserScore = 0;

	return lUserScore;
}

LRESULT  CGameClientEngine::OnPlaySound(WPARAM wParam, LPARAM lParam)
{
	
	if(lParam == 0)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("PLEASEJETTONWAV"));
	}
	else if(lParam==1)
	{
		//PlayGameSound(AfxGetInstanceHandle(),TEXT("LEAVEHANDLEWAV"));
	}
	else if(lParam==3)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("IDC_SNDWAV"));
	}
	else if(lParam==4)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("END_LOST"));
	}
	else if(lParam==5)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("END_WIN"));
	}
	else if(lParam==6)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("END_DRAW"));
	}
	else if(lParam==7)
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
	}
	
	

	return 1;
		
}

//������Ϣ
LRESULT CGameClientEngine::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	IClientUserItem * pClientUserItem=GetTableUserItem(GetMeChairID());

	if (pClientUserItem->GetUserScore() < m_lApplyBankerCondition) return true;

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

		//m_bMeApplyBanker=true;
	}
	else
	{
		//������Ϣ
		SendSocketData(SUB_C_CANCEL_BANKER, NULL, 0);

		//m_bMeApplyBanker=false;
	}

	//���ð�ť
	UpdateButtonContron();

	return true;
}

//������ׯ
bool CGameClientEngine::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	return true;
}

//ȡ����ׯ
bool CGameClientEngine::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	return true;
}

//�л�ׯ��
bool CGameClientEngine::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

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

		m_GameClientView.SetGameHistory(pServerGameRecord->bWinMen);
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
	ASSERT(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT);
	if (!(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT)) return false;

	//��ע����
	m_GameClientView.PlaceUserJetton(pPlaceJettonFail->lJettonArea,-pPlaceJettonFail->lPlaceScore);

	//�Լ��ж�
	if (GetMeChairID()==pPlaceJettonFail->wPlaceUser && false==IsLookonMode())
	{
		LONGLONG lJettonCount=pPlaceJettonFail->lPlaceScore;
		//�Ϸ�У��
		ASSERT(m_lUserJettonScore[cbViewIndex]>=lJettonCount);
		if (lJettonCount>m_lUserJettonScore[cbViewIndex]) return false;

		//������ע
		m_lUserJettonScore[cbViewIndex]-=lJettonCount;
		m_GameClientView.SetMePlaceJetton(cbViewIndex,m_lUserJettonScore[cbViewIndex]);
	}


	return true;
}

//����ׯ��
void CGameClientEngine::SetBankerInfo(WORD wBanker,LONGLONG lScore)
{
	m_wCurrentBanker=wBanker;
	m_lBankerScore=lScore;
	//tagUserData const *pUserData=m_wCurrentBanker==INVALID_CHAIR ? NULL : GetTableUserItem(m_wCurrentBanker);
	IClientUserItem * pClientUserItem=m_wCurrentBanker==INVALID_CHAIR ? NULL : GetTableUserItem(m_wCurrentBanker);
	DWORD dwBankerUserID = (NULL==pClientUserItem) ? 0 : pClientUserItem->GetUserID();
	m_GameClientView.SetBankerInfo(dwBankerUserID,m_lBankerScore);

	//if (IsLookonMode()==false && GetMeChairID()==m_wCurrentBanker) m_bMeApplyBanker=true;
	//else m_bMeApplyBanker = false;
	//UpdateButtonContron();

}

//������ע
void CGameClientEngine::SetMePlaceJetton(BYTE cbViewIndex, LONGLONG lJettonCount)
{
	//�Ϸ��ж�
	ASSERT(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT);
	if (!(cbViewIndex>=1 && cbViewIndex<=AREA_COUNT)) return;

	//���ñ���
	m_lUserJettonScore[cbViewIndex]=lJettonCount;

	//���ý���
	m_GameClientView.SetMePlaceJetton(cbViewIndex,lJettonCount);
}
//////////////////////////////////////////////////////////////////////////
//����
LRESULT CGameClientEngine::OnAdminControl( WPARAM wParam, LPARAM lParam )
{
	SendSocketData(SUB_C_ADMIN_COMMDN,(CMD_C_ControlApplication*)wParam,sizeof(CMD_C_ControlApplication));
	return true;
}

//����
bool CGameClientEngine::OnSubAdminControl( const void * pBuffer, WORD wDataSize )
{
	ASSERT(wDataSize==sizeof(CMD_S_ControlReturns));
	if(wDataSize!=sizeof(CMD_S_ControlReturns)) return false;

	if(m_GameClientView.m_pAdminControl!=NULL)
	{
		CMD_S_ControlReturns* pResult = (CMD_S_ControlReturns*)pBuffer;
		m_GameClientView.m_pAdminControl->UpdateControl(pResult);
	}
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
					//placeJetton.cbJettonArea = androidBet.cbJettonArea;
					//placeJetton.lJettonScore = androidBet.lJettonScore;
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


void CGameClientEngine::ReSetBankCtrol(int nGameState)
{

	if(!IsLookonMode())
	{
		switch (GetGameStatus())
		{
		case GAME_STATUS_FREE:
			{
				if(GetMeChairID()!=m_wCurrentBanker)
				{					
					m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
					m_GameClientView.m_btBankStorage.EnableWindow(TRUE);
				}
				else
				{					
					m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
					m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
				}
				break;
			}
		case GS_GAME_END:
		case GS_PLACE_JETTON:
		case GS_MOVECARD_END:
			{
				if(GetMeChairID()!=m_wCurrentBanker)
				{
					m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
					m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
				}
				else
				{
					m_GameClientView.m_btBankDraw.EnableWindow(FALSE);
					m_GameClientView.m_btBankStorage.EnableWindow(FALSE);
				}				
				break;
			}
		default:break;
		}
	}
	else
	{
		m_GameClientView.m_btBankDraw.EnableWindow(TRUE);
		m_GameClientView.m_btBankStorage.EnableWindow(TRUE);
	}

}

//�û�����
VOID  CGameClientEngine::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//���óɼ�
	if (bLookonUser==false)
	{
		
		//���½���
		m_GameClientView.RefreshGameView();
	}

	return;
}

//�û��뿪
VOID  CGameClientEngine::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//���óɼ�
	if (bLookonUser==false)
	{
		

		WORD wViewChairId = SwitchViewChairID(pIClientUserItem->GetChairID());
		
		//���½���
		m_GameClientView.RefreshGameView();
	}

	return;
}

//�û�״̬
VOID  CGameClientEngine::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//��������
	IClientUserItem * pIMySelfUserItem=GetMeUserItem();
	BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
	return;
}

//ѡ���¼�
VOID  CGameClientEngine::OnEventPhraseSelect(INT nPhraseIndex)
{
	
}

//��������
bool CGameClientEngine::AllowBackGroundSound(bool bAllowSound)
{
	//if(bAllowSound)
	//	PlayBackGroundSound(AfxGetInstanceHandle(),TEXT("BACK_GROUND"));
	//else
	//	StopSound();

	return true;
}


