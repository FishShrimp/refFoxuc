#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientEngine.h"
#include ".\GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_NULLITY					200									//��Ч��ʱ��
#define IDI_START_GAME				201									//��ʼ��ʱ��
#define IDI_USER_ADD_SCORE			202									//��ע��ʱ��

//��ʱ����ʶ
#define IDI_TIME_USER_INVEST		0									//�±���ʱ��
#define IDI_TIME_OPEN_CARD			1									//���ƶ�ʱ��
#define IDI_DELAY_SHOW_BUTTON		2									//��ʱ��ʱ��
#define IDI_DELAY_EXIT_WIN			3									//��ʱ��ʱ��

//ʱ���ʶ
#ifdef _DEBUG
#define TIME_USER_INVEST			99									//�±���ʱ��
#define TIME_USER_START_GAME		99									//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			99									//��ע��ʱ��
#define TIME_USER_OPEN_CARD			99									//̯�ƶ�ʱ��

#else
#define TIME_USER_INVEST			30									//�±���ʱ��
#define TIME_USER_START_GAME		30									//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			30									//��ע��ʱ��
#define TIME_USER_OPEN_CARD			30									//̯�ƶ�ʱ��

#endif

#define TIME_DELAY_SHOW_BUTTON		1/10								//��ʱ��ʱ��
#define TIME_DELAY_EXIT_WIN			1									//��ʱ��ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)

	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_ADD_SCORE,OnAddScore)
	ON_MESSAGE(IDM_CLEAR_SCORE,OnClearScore)
	ON_MESSAGE(IDM_OPEN_CARD,OnOpenCard)
	ON_MESSAGE(IDM_DECREASE,OnDecreaseJetton)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
	ON_MESSAGE(IDM_OPEN_CARD_FINISH,OnOpenCardFinish)
	ON_WM_TIMER()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine() 
{
	//��Ϸ����
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		m_wViewChairID[i]=INVALID_CHAIR;
	}		
	m_wBankerUser=INVALID_CHAIR;							
	m_wCurrentUser=INVALID_CHAIR;				
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lUserGetScore,sizeof(m_lUserGetScore));
	ZeroMemory(m_lUserTax,sizeof(m_lUserTax));

	//��ע����
	m_lCellScore=0;
	m_lTurnMinScore=0;
	m_lTurnMaxScore=0;	
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));

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
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		m_wViewChairID[i]=INVALID_CHAIR;
	}		

	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lUserGetScore,sizeof(m_lUserGetScore));
	ZeroMemory(m_lUserTax,sizeof(m_lUserTax));

	//��ע����
	m_lCellScore=0;
	m_lTurnMinScore=0;
	m_lTurnMaxScore=0;	
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));

	//ɾ��ʱ��
	KillTimer( IDI_DELAY_SHOW_BUTTON	);
	KillTimer( IDI_DELAY_EXIT_WIN	);

	return true;
}

//��Ϸ����
void CGameClientEngine::OnGameOptionSet()
{
	
	CGlobalUnits * m_pGlobalUnits=(CGlobalUnits *)CGlobalUnits::GetInstance();

	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound=m_pGlobalUnits->IsAllowGameSound();
	GameOption.m_bAllowLookon=m_pGlobalUnits->m_bAllowFriendLookon;

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//��������
		//EnableSound(GameOption.m_bEnableSound);
		//AllowUserLookon(0,GameOption.m_bAllowLookon);

		m_pGlobalUnits->m_bAllowSound = GameOption.m_bEnableSound;
		m_pGlobalUnits->m_bAllowFriendLookon =GameOption.m_bAllowLookon ;
	}

	return;
}

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//��ʼ��ʱ��
		{
			//��ֹ�ж�
			if (nElapse==0)
			{
				AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
				return false;
			}

			//����֪ͨ
			if (nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_NULLITY:			//����ʱ��
		{
			//��ֹ�ж�
			if (nElapse==0)
			{
				KillGameClock(nTimerID);
				return false;
			}

			//����֪ͨ
			if (nElapse<=5) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_USER_ADD_SCORE:	//��עʱ��
		{
			//��ֹ�ж�
			if (nElapse==0)
			{
				KillGameClock(nTimerID);

				if(!IsLookonMode() && m_wCurrentUser==GetMeChairID())
				{
					OnAddScore(IDC_GIVE_UP,0);
				}

				return false;
			}

			//����֪ͨ
			if (nElapse<=5 && !IsLookonMode() && m_wCurrentUser==GetMeChairID()) 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	}

	return false;
}

//�Թ�״̬
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	//�ؼ�����
	//if(bLookonUser)
	{
		if(IsAllowLookon() )
			m_GameClientView.m_CardControl[m_wViewChairID[GetMeChairID()]].SetDisplayHead(true);
		else m_GameClientView.m_CardControl[m_wViewChairID[GetMeChairID()]].SetDisplayHead(false);
	}
	return true;
}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			//��Ϣ����
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_USER_INVEST:	//�û��±�
		{
			//��Ϣ����
			return OnSubUserInvest(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:	//������Ϣ
		{
			//��Ϣ����
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_GIVE_UP:		//�û�����
		{
			//��Ϣ����
			return OnSubUserGiveUp(pBuffer,wDataSize);
		}
	case SUB_S_ADD_SCORE:	//�û���ע
		{
			//��Ϣ����
			return OnSubAddScore(pBuffer,wDataSize);
		}
	case SUB_S_OPEN_START:	//��ʼ����
		{
			//��Ϣ����
			return OnSubStartOpen(pBuffer,wDataSize);
		}
	case SUB_S_OPEN_CARD:	//�û�̯��
		{
			//��Ϣ����
			return OnSubOpenCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			//��Ϣ����
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	}

	return false;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, VOID * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_TK_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//���ÿؼ�
			if (!IsLookonMode())
			{
				//��ʼ��ť
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
				SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_USER_START_GAME);

				//���õ�Ԫ
				m_lCellScore = pStatusFree->lCellScore;
				m_GameClientView.SetCellScore(pStatusFree->lCellScore);
				m_GameClientView.m_JetonControl.SetCellJetton(pStatusFree->lCellScore);
			}

			return true;
		}
	case GS_TK_INVEST:	//�±�״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusInvest)) return false;
			CMD_S_StatusInvest * pStatusInvest=(CMD_S_StatusInvest *)pBuffer;

			//���ñ���
			m_wBankerUser=pStatusInvest->wBankerUser;
			m_lCellScore = pStatusInvest->lCellScore;
			CopyMemory(m_lUserScore,pStatusInvest->lUserScore,sizeof(m_lUserScore));
			CopyMemory(m_cbPlayStatus,pStatusInvest->cbPlayStatus,sizeof(m_cbPlayStatus));
			m_GameClientView.m_JetonControl.SetCellJetton(m_lCellScore);

			//���ý���
			m_GameClientView.SetWaitInvest(true);
			m_GameClientView.SetCellScore(m_lCellScore);
			m_GameClientView.SetBankerUser(GetViewChairID(m_wBankerUser));
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;

				//��������
				if(!pStatusInvest->bInvestFinish[i])
				{
					m_GameClientView.SetUserOperate(GetViewChairID(i),true);
				}

				//�û�����
				m_GameClientView.SetUserJetton(GetViewChairID(i),m_lUserScore[i]);

				//��ȡ�û�
				IClientUserItem* pUserData=GetTableUserItem(i);

				if (pUserData==NULL) continue;

				//�û�����
				lstrcpyn(m_szAccounts[i],pUserData->GetNickName(),CountArray(m_szAccounts[i]));
			}

			//���ÿؼ�
			WORD wMeChairID=GetMeChairID();
			if(!pStatusInvest->bInvestFinish[wMeChairID] && !IsLookonMode())
			{
				//�û��ؼ�
				UpdateInvestControl();

				//�Զ�����
				if(m_GameClientView.m_btClearScore.IsWindowVisible()==FALSE)
				{
					OnAddScore(IDC_INVEST,0);

					if(m_pIStringMessage!=NULL)
						m_pIStringMessage->InsertCustomString(TEXT("\n�������ڵ�Ԫ��,�����ټ�."),RGB(255,0,0),RGB(255,255,255));

					//InsertGeneralString(TEXT("\n�������ڵ�Ԫ��,�����ټ�."),RGB(255,0,0),true);
				}
				//���ö�ʱ
				else SetTimer(IDI_TIME_USER_INVEST,TIME_USER_INVEST*1000,NULL);
			}

			//������ʱ
			SetGameClock(wMeChairID,IDI_NULLITY,TIME_USER_INVEST);

			return true;
		}
	case GS_TK_SCORE:	//��ע״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;
			CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pBuffer;

			//���ñ���
			m_wBankerUser=pStatusScore->wBankerUser;
			m_wCurrentUser=pStatusScore->wCurrentUser;
			m_lTurnMaxScore=pStatusScore->lTurnMaxScore;
			m_lTurnMinScore=pStatusScore->lTurnMinScore;	
			m_lCellScore=pStatusScore->lCellScore;
			CopyMemory(m_lUserScore,pStatusScore->lUserScore,sizeof(m_lUserScore));
			CopyMemory(m_lTotalScore,pStatusScore->lTotalScore,sizeof(m_lTotalScore));
			CopyMemory(m_cbPlayStatus,pStatusScore->cbPlayStatus,sizeof(m_cbPlayStatus));
			m_GameClientView.m_JetonControl.SetCellJetton(m_lCellScore);

			//������Ŀ
			BYTE cbSpeCount=2,cbMingCount=cbSpeCount;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;

				for(WORD j=0;j<cbSpeCount;j++)
				{
					if(pStatusScore->cbMingCard[i][j]!=0)cbMingCount++;
					else break;
				}
				break;
			}

			//�����˿�
			WORD wMeChairID=GetMeChairID();
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;

				//�û��˿�
				CopyMemory(&m_cbHandCardData[i][cbSpeCount],pStatusScore->cbMingCard[i],sizeof(BYTE)*cbMingCount);
			}
			CopyMemory(m_cbHandCardData[wMeChairID],pStatusScore->cbHandCard,sizeof(BYTE)*cbSpeCount);

			//���ý���
			m_GameClientView.SetCellScore(m_lCellScore);
			m_GameClientView.SetBankerUser(GetViewChairID(m_wBankerUser));
			m_GameClientView.SetUserOperate(GetViewChairID(m_wCurrentUser),true);
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;

				//��ע����
				m_GameClientView.SetUserTableScore(GetViewChairID(i),m_lTotalScore[i]);

				//�û�����
				m_GameClientView.SetUserJetton(GetViewChairID(i),m_lUserScore[i]-m_lTotalScore[i]);

				//�û��˿�
				m_GameClientView.m_CardControl[GetViewChairID(i)].SetCardData(m_cbHandCardData[i],cbMingCount);
			}

			//�û�����
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_lTotalScore[i]==0)continue;

				//��ȡ�û�
				IClientUserItem * pUserData=GetTableUserItem(i);
				if (pUserData!=NULL) lstrcpyn(m_szAccounts[i],pUserData->GetNickName(),CountArray(m_szAccounts[i]));
				else lstrcpyn(m_szAccounts[i],TEXT("�û����뿪"),CountArray(m_szAccounts[i]));
			}

			//��Ӧ�ؼ�
			if(!IsLookonMode())m_GameClientView.m_CardControl[MY_VIEW_CHAIR_ID].SetPositively(true);

			//�û��ؼ�
			if(m_wCurrentUser==wMeChairID && !IsLookonMode())
			{
				//���¿ؼ�
				UpdateScoreControl();
			}

			//����ʱ��
			SetGameClock(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);

			return true;
		}
	case GS_TK_OPEN_CARD:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusOpenCard)) return false;
			CMD_S_StatusOpenCard * pStatusOpenCard=(CMD_S_StatusOpenCard *)pBuffer;

			//���ñ���
			WORD wMeChairID=GetMeChairID();
			m_wBankerUser=pStatusOpenCard->wBankerUser;
			m_lCellScore = pStatusOpenCard->lCellScore;
			CopyMemory(m_lUserScore,pStatusOpenCard->lUserScore,sizeof(m_lUserScore));
			CopyMemory(m_lTotalScore,pStatusOpenCard->lTotalScore,sizeof(m_lTotalScore));
			CopyMemory(m_cbPlayStatus,pStatusOpenCard->cbPlayStatus,sizeof(m_cbPlayStatus));
			m_GameClientView.m_JetonControl.SetCellJetton(m_lCellScore);

			//��������
			BYTE cbSpeCount=2;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;

				//�û��˿�
				CopyMemory(&m_cbHandCardData[i][cbSpeCount],pStatusOpenCard->cbMingCard[i],sizeof(BYTE)*cbSpeCount);
			}
			CopyMemory(m_cbHandCardData[wMeChairID],pStatusOpenCard->cbHandCard,sizeof(BYTE)*cbSpeCount);

			//�û�����
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_lTotalScore[i]==0)continue;

				//��ȡ�û�
				IClientUserItem * pUserData=GetTableUserItem(i);
				if (pUserData!=NULL) lstrcpyn(m_szAccounts[i],pUserData->GetNickName(),CountArray(m_szAccounts[i]));
				else lstrcpyn(m_szAccounts[i],TEXT("�û����뿪"),CountArray(m_szAccounts[i]));
			}

			//���ý���
			m_GameClientView.SetWaitOpenCard(true);
			m_GameClientView.SetCellScore(m_lCellScore);
			m_GameClientView.SetBankerUser(GetViewChairID(m_wBankerUser));
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;

				//��������
				if(pStatusOpenCard->cbOpenFinish[i]==FALSE)
				{
					m_GameClientView.SetUserOperate(GetViewChairID(i),true);
					m_GameClientView.m_CardControl[GetViewChairID(i)].SetCardData(m_cbHandCardData[i],MAX_COUNT);
				}
				else 
				{
					BYTE cbCardData[MAX_COUNT];
					ZeroMemory(cbCardData,sizeof(cbCardData));
					m_GameClientView.m_CardControl[GetViewChairID(i)].SetCardData(cbCardData,MAX_COUNT);
				}

				//�û�����
				m_GameClientView.SetUserJetton(GetViewChairID(i),m_lUserScore[i]-m_lTotalScore[i]);
			}

			//�ܳ�����
			LONGLONG lAllScore=0;
			for(WORD i=0;i<GAME_PLAYER;i++)if(m_lTotalScore[i]>0)lAllScore+=m_lTotalScore[i];
			m_GameClientView.SetCenterScore(lAllScore);

			//��ע��Ϣ
			TCHAR szBuffer[512]=TEXT("");
			myprintf(szBuffer,CountArray(szBuffer),TEXT("\n��ע��Ϣ:"));
			//InsertGeneralString(szBuffer,RGB(0,0,0),true);
			
			if(m_pIStringMessage!=NULL)
				m_pIStringMessage->InsertCustomString(TEXT("\n��ע��Ϣ:"),RGB(0,0,0),RGB(255,255,255));

			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_lTotalScore[i]==0)continue;

				//�ɼ����
				myprintf(szBuffer,CountArray(szBuffer),TEXT("[%s]����ע:%I64d"),
					m_szAccounts[i],m_lTotalScore[i]);
				//InsertGeneralString(szBuffer,RGB(0,0,0),true);

				if(m_pIStringMessage!=NULL)
					m_pIStringMessage->InsertCustomString(szBuffer,RGB(0,0,0),RGB(255,255,255));
			}

			//���洦��
			SetGameClock(GetMeChairID(),IDI_NULLITY,TIME_USER_OPEN_CARD);

			//��ʾ�ؼ�
			if(pStatusOpenCard->cbOpenFinish[wMeChairID]==FALSE && !IsLookonMode())
			{
				//���ý���
				m_GameClientView.SetHitPositively(true);
				m_GameClientView.SetHitCardData(m_cbHandCardData[GetMeChairID()],MAX_COUNT);
				m_GameClientView.m_CardControl[MY_VIEW_CHAIR_ID].SetCardData(NULL,0);

				//���ư�ť
				m_GameClientView.m_btOpenCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOpenCard.EnableWindow(FALSE);

				//����ʱ��
				SetTimer(IDI_TIME_OPEN_CARD,TIME_USER_OPEN_CARD*1000,NULL);
			}
			else if(!IsLookonMode())
			{
				//�����˿�
				BYTE cbTempData[MAX_COUNT];
				CopyMemory(&cbTempData[cbSpeCount],m_cbHandCardData[wMeChairID],cbSpeCount);
				CopyMemory(cbTempData,&m_cbHandCardData[wMeChairID][cbSpeCount],cbSpeCount);
				m_GameClientView.m_CardControl[MY_VIEW_CHAIR_ID].SetCardData(cbTempData,MAX_COUNT);
				m_GameClientView.m_CardControl[MY_VIEW_CHAIR_ID].SetDisplayHead(true);
			}

			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//�����Թ�
	if (IsLookonMode())OnStart(0,0);

	//�û���Ϣ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IClientUserItem * pUserData=GetTableUserItem(i);
		if (pUserData==NULL) continue;

		//��Ϸ��Ϣ
		m_cbPlayStatus[i]=TRUE;

		//�û�����
		lstrcpyn(m_szAccounts[i],pUserData->GetNickName(),CountArray(m_szAccounts[i]));
	}
	CopyMemory(m_lUserScore,pGameStart->lUserScore,sizeof(m_lUserScore));

	//�û�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_cbPlayStatus[i]!=TRUE)continue;
		m_GameClientView.SetUserJetton(GetViewChairID(i),m_lUserScore[i]);

		m_GameClientView.SetUserOperate(GetViewChairID(i),true);
	}

	//����ׯ��
	m_wBankerUser=pGameStart->wBankerUser;
	m_GameClientView.SetBankerUser(GetViewChairID(m_wBankerUser));

	//�ȴ���־
	m_GameClientView.SetWaitInvest(true);

	//�ؼ�����
	WORD wMeChairID = GetMeChairID();
	if(!IsLookonMode())
	{
		//�û��ؼ�
		UpdateInvestControl();

		//�Զ�����
		if(m_GameClientView.m_btClearScore.IsWindowVisible()==FALSE)
		{
			OnAddScore(IDC_INVEST,0);
			//InsertGeneralString(TEXT("\n�������ڵ�Ԫ��,�����ټ�."),RGB(255,0,0),true);

			if(m_pIStringMessage!=NULL)
				m_pIStringMessage->InsertCustomString(TEXT("\n�������ڵ�Ԫ��,�����ټ�."),RGB(255,0,0),RGB(255,255,255));
		}
		//���ö�ʱ
		else SetTimer(IDI_TIME_USER_INVEST,TIME_USER_INVEST*1000,NULL);
	}

	//������ʱ
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
	SetGameClock(wMeChairID,IDI_NULLITY,TIME_USER_INVEST);

	return true;
}

//�û��±�
bool CGameClientEngine::OnSubUserInvest(VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_UserInvest)) return false;
	CMD_S_UserInvest * pUserInvest=(CMD_S_UserInvest *)pBuffer;

	if(GetMeChairID()!=pUserInvest->wChairID || IsLookonMode())
	{
		//������Ŀ
		WORD wChairID = pUserInvest->wChairID;
		m_lUserScore[wChairID] = pUserInvest->lScore;
		m_GameClientView.SetUserJetton(GetViewChairID(wChairID),m_lUserScore[wChairID]);

		m_GameClientView.SetUserOperate(GetViewChairID(wChairID),false);
		PlayGameSound(AfxGetInstanceHandle(),TEXT("INVEST"));
	}

	return true;
}

//������Ϣ
bool CGameClientEngine::OnSubSendCard(VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//��ǰ�û�
	m_wCurrentUser = pSendCard->wCurrentUser;
	WORD wMeChairID = GetMeChairID();

	//�µ�Ԫע
	BYTE cbSpeCount = 2;
	if(pSendCard->cbCardCount==cbSpeCount || pSendCard->wCurrentUser==INVALID_CHAIR)
	{
		//ɾ����ʱ��
		KillGameClock(IDI_NULLITY);
		KillTimer(IDI_TIME_USER_INVEST);

		//�������
		m_GameClientView.SetWaitInvest(false);

		//�����û�
		for(WORD i=0;i<GAME_PLAYER;i++)if(m_cbPlayStatus[i]==TRUE)break;

		//�µ�Ԫע
		if(m_lTotalScore[i]==0)
		{
			//��Ӧ�˿�
			if(!IsLookonMode())m_GameClientView.m_CardControl[MY_VIEW_CHAIR_ID].SetPositively(true);

			//�µ�Ԫע
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;
				m_lTableScore[i]=m_lCellScore;
				m_lTotalScore[i]=m_lCellScore;

				//��������
				m_GameClientView.SetJettonMoveInfo(GetViewChairID(i),MOVE_USER_ADD,m_lCellScore);

				//�û�����
				LONGLONG lScore = m_lUserScore[i]-m_lTotalScore[i];
				m_GameClientView.SetUserJetton(GetViewChairID(i),lScore);
			}

			//��С��ע
			if(m_wCurrentUser<GAME_PLAYER)
			{
				m_lTurnMinScore=0;
				m_lTurnMaxScore=m_lUserScore[m_wCurrentUser]-m_lTotalScore[m_wCurrentUser];
			}
		}
	}

	//�����˿�
	CopyMemory(m_cbHandCardData[wMeChairID],pSendCard->cbHandCard,cbSpeCount);

	//��������
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_cbPlayStatus[i]==FALSE)continue;
		CopyMemory(&m_cbHandCardData[i][cbSpeCount],pSendCard->cbMingCard[i],cbSpeCount);
	}

	//�˿���Ŀ
	BYTE cbCardCount=0;
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_cbPlayStatus[i]==TRUE)
		{
			cbCardCount=(BYTE)m_GameClientView.m_CardControl[GetViewChairID(i)].GetCardCount();
			break;
		}
	}

	//�ɷ��˿�
	for(WORD i=cbCardCount;i<pSendCard->cbCardCount;i++)
	{
		for (WORD j=m_wBankerUser;j<m_wBankerUser+GAME_PLAYER;j++)
		{
			WORD w=j%GAME_PLAYER;
			if (m_cbPlayStatus[w]!=TRUE)continue;

			WORD wViewChairID=GetViewChairID(w);
			m_GameClientView.DispatchUserCard(wViewChairID,m_cbHandCardData[w][i]);
		}
	}

	return true;
}

//�û�����
bool CGameClientEngine::OnSubUserGiveUp(VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;

	//���ñ���
	m_cbPlayStatus[pGiveUp->wGiveUpUser]=FALSE;
	WORD wViewID = GetViewChairID(pGiveUp->wGiveUpUser);
	m_GameClientView.m_CardControl[wViewID].SetDisableCardBack(true);
	m_GameClientView.SetUserOperate(wViewID,false);
 
	//��������
	if(pGiveUp->wGiveUpUser==m_wCurrentUser && pGiveUp->wGameStatus==GS_TK_SCORE)
	{
		m_GameClientView.SetUserAction(wViewID,IDC_GIVE_UP);
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));
	}

	return true;
}

//�û���ע
bool CGameClientEngine::OnSubAddScore(VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	//ɾ��ʱ��
	KillTimer(IDI_USER_ADD_SCORE);

	//��������
	WORD wMeChairID = GetMeChairID();
	WORD wAddUser = pAddScore->wAddScoreUser;
	LONGLONG lAddScore = pAddScore->lAddScoreCount;
	WORD wViewAddUser = GetViewChairID(wAddUser);

	//�������
	m_GameClientView.SetUserOperate(wViewAddUser,false);

	//��������
	if((wAddUser!=wMeChairID || IsLookonMode()) && m_cbPlayStatus[wAddUser]!=FALSE)
	{
		//��������
		WORD wUserAction=0;
		if(lAddScore==0)
		{
			wUserAction = IDC_PASS;
			PlayGameSound(AfxGetInstanceHandle(),TEXT("NO_ADD"));
		}
		else if(lAddScore==m_lTurnMaxScore)
		{
			wUserAction = IDC_SHOW_HAND;
			PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));
		}
		else if(lAddScore>m_lTurnMinScore)
		{
			wUserAction = IDC_ADD_SCORE;
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
		}
		else if(lAddScore==m_lTurnMinScore)
		{
			wUserAction = IDC_FOLLOW;
			PlayGameSound(AfxGetInstanceHandle(),TEXT("FOLLOW"));
		}

		//��ʾ����
		if(wUserAction>0)m_GameClientView.SetUserAction(wViewAddUser,wUserAction);
	}

	//���ñ���
	m_lTurnMaxScore =pAddScore->lTurnMaxScore;
	m_lTurnMinScore	= pAddScore->lTurnMinScore;
	m_wCurrentUser = pAddScore->wCurrentUser;

	//�������
	if(wAddUser!=wMeChairID || IsLookonMode())
	{
		//���ó���
		m_lTotalScore[wAddUser]+=lAddScore;
		m_lTableScore[wAddUser]+=lAddScore;

		//�ƶ�����
		m_GameClientView.SetJettonMoveInfo(wViewAddUser,MOVE_USER_ADD,lAddScore);

		//��ͼ����
		LONGLONG lMeGoldCount = m_lUserScore[wAddUser]-m_lTotalScore[wAddUser];
		m_GameClientView.SetUserJetton(wViewAddUser,lMeGoldCount);
	}

	//��ǰ�û�
	if(m_wCurrentUser==wMeChairID && !IsLookonMode())
	{
		//���¿ؼ�
		UpdateScoreControl();
	}

	//����ʱ��
	if(m_wCurrentUser<GAME_PLAYER)
	{
		//���ý���
		m_GameClientView.SetUserOperate(GetViewChairID(m_wCurrentUser),true);

		SetGameClock(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);
	}

	return true;
}

//��ʼ����
bool CGameClientEngine::OnSubStartOpen(VOID * pBuffer, WORD wDataSize)
{
	//����״̬
	SetGameStatus(GS_TK_OPEN_CARD);

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;

	//�ж϶���
	if(!m_GameClientView.IsCardMoveing())OnSendCardFinish(0,0);

	return true;
}

//�û�����
bool CGameClientEngine::OnSubOpenCard(VOID * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_Open_Card)) return false;
	CMD_S_Open_Card * pStatusOpenCard=(CMD_S_Open_Card *)pBuffer;

	//��������
	WORD wMeChairID = GetMeChairID();
	WORD wOpenUser = pStatusOpenCard->wChairID;
	WORD wViewOpenUser = GetViewChairID(wOpenUser);

	//���ý���
	if(wMeChairID!=wOpenUser || IsLookonMode())
	{
		BYTE cbCardData[MAX_COUNT];
		ZeroMemory(cbCardData,sizeof(cbCardData));
		m_GameClientView.m_CardControl[wViewOpenUser].SetCardData(cbCardData,MAX_COUNT);

		m_GameClientView.SetUserOperate(wViewOpenUser,false);
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OPEN_CARD"));
	}

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(VOID * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//���ذ�ť
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);	
	m_GameClientView.m_btInvest.ShowWindow(SW_HIDE);	
	m_GameClientView.m_btAddScore.ShowWindow(SW_HIDE);	
	m_GameClientView.m_btFollow.ShowWindow(SW_HIDE);		
	m_GameClientView.m_btShowHand.ShowWindow(SW_HIDE);	
	m_GameClientView.m_btPass.ShowWindow(SW_HIDE);		
	m_GameClientView.m_btGiveUp.ShowWindow(SW_HIDE);	
	m_GameClientView.m_btClearScore.ShowWindow(SW_HIDE);
	for(WORD i=0;i<CONTROL_COUNT;i++)m_GameClientView.m_btDecrease[i].ShowWindow(SW_HIDE);
	m_GameClientView.m_JetonControl.SetUserJetton(0);
	m_GameClientView.m_JetonControl.SetAddJetton(0);

	//ɾ��ʱ��
	KillTimer(IDI_TIME_USER_INVEST);	
	KillTimer(IDI_TIME_OPEN_CARD);	
	KillGameClock(IDI_USER_ADD_SCORE);
	KillGameClock(IDI_NULLITY);

	//�������
	m_GameClientView.SetWaitInvest(false);
	m_GameClientView.SetWaitOpenCard(false);
	m_GameClientView.SetHitPositively(false);
	m_GameClientView.SetHitCardData(NULL,0);
	for(WORD i=0;i<CONTROL_COUNT;i++)
	{
		m_GameClientView.m_CardControl[i].SetPositively(false);
	}
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetOffLinkUser(i,false);
		m_GameClientView.SetUserOperate(i,false);
	}

	//���ñ���
	CopyMemory(m_lUserTax,pGameEnd->lGameTax,sizeof(m_lUserTax));
	CopyMemory(m_lUserGetScore,pGameEnd->lGameScore,sizeof(m_lUserGetScore));
	CopyMemory(m_cbHandCardData,pGameEnd->cbCardData,sizeof(m_cbHandCardData));

	//����ԭ��
	switch(pGameEnd->cbEndReason)
	{
	case END_REASON_NORMAL:			//��������
		{
			//�����û�
			WORD wHeadMaxUser=INVALID_CHAIR;
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				wHeadMaxUser = (i+m_wBankerUser)%GAME_PLAYER;
				if(m_cbPlayStatus[wHeadMaxUser]!=FALSE)break;
			}
			WORD wFirstOpen = wHeadMaxUser;

			//�Ƚ�ͷ��
			BYTE cbSpeCount = 2;
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				//��ǰ�û�
				WORD wCurUser = (i+m_wBankerUser)%GAME_PLAYER;
				if(m_cbPlayStatus[wCurUser]==FALSE || wCurUser==wHeadMaxUser)continue;

				//�Ƚϴ�С
				BYTE cbResult=m_GameLogic.CompareCard(m_cbHandCardData[wCurUser],m_cbHandCardData[wHeadMaxUser],cbSpeCount);
				if(cbResult==TRUE)wHeadMaxUser=wCurUser;
			}
			ASSERT(m_cbPlayStatus[wHeadMaxUser]==TRUE);

			//��Ч�û�
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i]==TRUE)continue;
				m_GameClientView.m_CardControl[GetViewChairID(i)].SetDisableCardBack(true);
			}

			//��ͼ�˿�
			BYTE cbTempData[GAME_PLAYER][MAX_COUNT];
			ZeroMemory(cbTempData,sizeof(cbTempData));
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i]!=TRUE)continue;

				WORD wViewID = GetViewChairID(i);
				CopyMemory(cbTempData[wViewID],m_cbHandCardData[i],MAX_COUNT);

				//�˿�����
				TCHAR szHeap[LEN_NICKNAME]=TEXT("");
				TCHAR sxTail[LEN_NICKNAME]=TEXT("");
				GetHeapTailType(i,szHeap,sxTail);
				m_GameClientView.SetCardType(wViewID,szHeap,sxTail);
			}

			//���ƶ���
			WORD wViewOpen = GetViewChairID(wFirstOpen);
			WORD wViewHeadMaxUser = GetViewChairID(wHeadMaxUser);
			m_GameClientView.SetOpenCardInfo(cbTempData,wViewOpen,wViewHeadMaxUser);

			break;
		}
	case END_REASON_EXIT:			//ǿ�˽���
		{
			//���뼯��
			LONGLONG lScore=0;
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				lScore += m_lTotalScore[i];
				m_GameClientView.SetUserTableScore(i,0);
			}
			m_GameClientView.SetCenterScore(lScore);

			SetTimer(IDI_DELAY_EXIT_WIN,500*TIME_DELAY_EXIT_WIN,NULL);

			break;
		}
	case END_REASON_PASS:			//���ƽ���
		{
			//�����˿�
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_lTotalScore[i]==0)continue;
				m_GameClientView.m_CardControl[GetViewChairID(i)].SetDisableCardBack(false);

				BYTE cbCardCount=(BYTE)m_GameClientView.m_CardControl[GetViewChairID(i)].GetCardCount();
				m_GameClientView.m_CardControl[GetViewChairID(i)].SetCardData(m_cbHandCardData[i],cbCardCount);
			}
			m_GameClientView.m_CardControl[MY_VIEW_CHAIR_ID].SetDisplayHead(true);

			//�˻ؽ��
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_CardControl[i].SetPositively(false);
				m_GameClientView.SetUserTableScore(i,0);

				if(m_lUserGetScore[i]==0 || GetTableUserItem(i)==NULL)continue;
				m_lUserGetScore[i]=0;
				m_GameClientView.SetJettonMoveInfo(GetViewChairID(i),MOVE_BACK_SCORE,m_lTotalScore[i]);
			}

			SetTimer(IDI_DELAY_SHOW_BUTTON,1000*TIME_DELAY_SHOW_BUTTON,NULL);

			break;
		}
	}


	return true;
}

//��ȡ����
void CGameClientEngine::GetHeapTailType(WORD wChairID,TCHAR szHeapString[],TCHAR szTailString[])
{
	//��ȡ����
	BYTE cbHeapType=m_GameLogic.GetCardType(m_cbHandCardData[wChairID],2);
	BYTE cbTailType=m_GameLogic.GetCardType(&m_cbHandCardData[wChairID][2],2);

	//�ж�����
	for(BYTE i=0;i<2;i++)
	{
		//�������
		BYTE cbTemp = ((i==0)?(cbHeapType):(cbTailType));
		TCHAR *pszString = ((i==0)?(szHeapString):(szTailString));
		BYTE cbTempCardData[MAX_COUNT];
		if(i==1)CopyMemory(cbTempCardData,&m_cbHandCardData[wChairID][2],sizeof(BYTE)*2);
		else CopyMemory(cbTempCardData,m_cbHandCardData[wChairID],sizeof(BYTE)*2);

		//�ж�����
		switch(cbTemp)
		{
		case D2H_VALUE0:
			{
				_sntprintf(pszString,sizeof(szHeapString[0])*LEN_NICKNAME,TEXT("0��"));
				break;
			}
		case D2H_D2H:
			{
				_sntprintf(pszString,sizeof(szHeapString[0])*LEN_NICKNAME,TEXT("������"));
				break;
			}
		case D2H_TH:
			{
				_sntprintf(pszString,sizeof(szHeapString[0])*LEN_NICKNAME,TEXT("���"));
				break;
			}
		case D2H_TG:
			{
				_sntprintf(pszString,sizeof(szHeapString[0])*LEN_NICKNAME,TEXT("���"));
				break;
			}
		case D2H_DG:
			{
				_sntprintf(pszString,sizeof(szHeapString[0])*LEN_NICKNAME,TEXT("�ظ�"));
				break;
			}
		case D2H_DOBLUE_Q:
		case D2H_DOBLUE_2:
		case D2H_DOBLUE_8:
		case D2H_DOBLUE_4:
		case D2H_DOBLUE_46A:
		case D2H_DOBLUE_67AJ:
		case D2H_DOBLUE_9875:
			{
				TCHAR tChInfo[LEN_NICKNAME]=TEXT("");
				GetCardType(cbTempCardData,1,tChInfo);
				_sntprintf(pszString,sizeof(szHeapString[0])*LEN_NICKNAME,TEXT("%s��"),tChInfo);
				break;
			}
		default:
			{
				TCHAR tChInfo[LEN_NICKNAME]=TEXT("");
				GetCardType(cbTempCardData,2,tChInfo);
				_sntprintf(pszString,sizeof(szHeapString[0])*LEN_NICKNAME,TEXT("%s%d"),tChInfo,cbTemp);
			}
		}
	}

	return ;
}

//��ȡ����
void CGameClientEngine::GetCardType(BYTE cbCardData[],BYTE cbCardCount,TCHAR szResult[])
{
	BYTE cbValue=m_GameLogic.GetSpeCardValue(cbCardData,cbCardCount);

	//�ж�����
	switch(cbValue)
	{
	case 9:
		{
			_sntprintf(szResult,sizeof(szResult[0])*LEN_NICKNAME,TEXT("��"));
			break;
		}
	case 8:
		{
			_sntprintf(szResult,sizeof(szResult[0])*LEN_NICKNAME,TEXT("��"));
			break;
		}
	case 7:
		{
			_sntprintf(szResult,sizeof(szResult[0])*LEN_NICKNAME,TEXT("��"));
			break;
		}
	case 6:
		{
			_sntprintf(szResult,sizeof(szResult[0])*LEN_NICKNAME,TEXT("��"));
			break;
		}
	case 5:
		{
			_sntprintf(szResult,sizeof(szResult[0])*LEN_NICKNAME,TEXT("��"));
			break;
		}
	case 4:
		{
			_sntprintf(szResult,sizeof(szResult[0])*LEN_NICKNAME,TEXT("��"));
			break;
		}
	case 3:
		{
			_sntprintf(szResult,sizeof(szResult[0])*LEN_NICKNAME,TEXT("��"));
			break;
		}
	}

	return ;
}

//��ȡ����
void CGameClientEngine::GetViewUserName(WORD wViewChairID,TCHAR szNametring[])
{
	//�����û�
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(m_wViewChairID[i]==wViewChairID)break;
	}
	ASSERT(i<GAME_PLAYER);
	if(i>=GAME_PLAYER)return;

	_sntprintf(szNametring,sizeof(m_szAccounts[i]),TEXT("%s"),m_szAccounts[i]);

	return;
}

//�Ӵ�λ��
WORD CGameClientEngine::GetViewChairID(WORD wChairID)
{
	if(m_wViewChairID[wChairID]==INVALID_CHAIR)
	{
		m_wViewChairID[wChairID]=SwitchViewChairID(wChairID);
	}

	return m_wViewChairID[wChairID];
}

//���¿���
void CGameClientEngine::UpdateScoreControl()
{
	ActiveGameFrame();
	WORD wMeChairID = GetMeChairID();

	//�ɼ��û�
	WORD wShowHandCount=0,wUserCount=0;
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(m_cbPlayStatus[i]==FALSE)continue;
		wUserCount++;
		if(m_lUserScore[i]==m_lTotalScore[i])wShowHandCount++;
	}
	if(wUserCount<=wShowHandCount+1 && m_lUserScore[wMeChairID]>m_lTurnMaxScore)wShowHandCount=TRUE;
	else wShowHandCount=0;

	//����ؼ�
	if(m_lTurnMaxScore-m_lTurnMinScore-m_lCellScore>0 && wShowHandCount==0)
	{
		//����ť
		m_GameClientView.m_btClearScore.ShowWindow(SW_SHOW);

		//��ע��ť
		for(WORD i=0;i<CONTROL_COUNT;i++)
		{
			m_GameClientView.m_btDecrease[i].ShowWindow(SW_SHOW);
		}
 
		//�û�����
		m_GameClientView.SetUserJetton(MY_VIEW_CHAIR_ID,0);
		m_GameClientView.m_JetonControl.SetUserJetton(m_lUserScore[wMeChairID]-m_lTotalScore[wMeChairID]-m_lTurnMinScore);
		m_GameClientView.m_JetonControl.SetMinJetton(m_lTurnMinScore);
		m_GameClientView.m_JetonControl.SetUserHaveCount(0);
	}

	//���ư�ť
	BYTE cbEnable=((m_lTurnMinScore>0L)?FALSE:TRUE);
	m_GameClientView.m_btPass.ShowWindow(SW_SHOW);
	m_GameClientView.m_btPass.EnableWindow(cbEnable);

	//��ע��ť
	cbEnable=((m_lTurnMinScore==m_lTurnMaxScore||m_lTurnMinScore==0L)?FALSE:TRUE);
	m_GameClientView.m_btFollow.ShowWindow(SW_SHOW);
	m_GameClientView.m_btFollow.EnableWindow(cbEnable);

	//��ע�ж�
	//cbEnable=((m_lTurnMaxScore-m_lCellScore>=m_lTurnMinScore)?TRUE:FALSE);
	m_GameClientView.m_btAddScore.ShowWindow(SW_SHOW);
	m_GameClientView.m_btAddScore.EnableWindow(FALSE);

	//������ť
	m_GameClientView.m_btGiveUp.ShowWindow(SW_SHOW);
	m_GameClientView.m_btGiveUp.EnableWindow(TRUE);

	//�����ť
	m_GameClientView.m_btShowHand.ShowWindow(SW_SHOW);
	m_GameClientView.m_btShowHand.EnableWindow(TRUE);

	return;
}

//���¿���
void CGameClientEngine::UpdateInvestControl()
{
	//�Լ�����
	WORD wMeChairID=GetMeChairID();
	LONGLONG lScore = GetTableUserItem(wMeChairID)->GetUserScore();

	//ʣ�����
	lScore-=m_lUserScore[wMeChairID];

	//��ʾ�ж�
	if(lScore>0)
	{
		//����ע��
		if(m_lUserScore[wMeChairID]<m_lCellScore)
		{
			lScore-=m_lCellScore;
			if(lScore>0)m_GameClientView.m_JetonControl.SetMinJetton(m_lCellScore);
		}

		//��ʾ�ؼ�
		if(lScore-m_lCellScore>0)
		{
			//����ť
			m_GameClientView.m_btClearScore.ShowWindow(SW_SHOW);

			//��ע��ť
			for(WORD i=0;i<CONTROL_COUNT;i++)
			{
				m_GameClientView.m_btDecrease[i].ShowWindow(SW_SHOW);
			}

			//�û�����
			m_GameClientView.SetUserJetton(MY_VIEW_CHAIR_ID,0);
			m_GameClientView.m_JetonControl.SetUserJetton(lScore);
			m_GameClientView.m_JetonControl.SetUserHaveCount(m_lUserScore[wMeChairID]);

			//�±���ť
			m_GameClientView.m_btInvest.ShowWindow(SW_SHOW);

			ActiveGameFrame();
		}
	}

	return;
}

//��ʼ��ť
LRESULT	CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	////�ɷ��˿�
	//for(WORD i=0;i<MAX_COUNT;i++)
	//{
	//	for (WORD j=0;j<GAME_PLAYER;j++)
	//	{
	//		m_GameClientView.m_CardControl[j].SetCardData(NULL,0);
	//		m_GameClientView.m_CardControl[j].SetDisplayHead(true);
	//		WORD w=j%GAME_PLAYER;

	//		m_GameClientView.DispatchUserCard(w,m_cbHandCardData[w][i]);
	//	}
	//}
	////	//���뼯��
	////	for(WORD i=0;i<GAME_PLAYER;i++)
	////	{
	////		m_GameClientView.SetUserTableScore(i,55);
	////		LONGLONG lScore = m_GameClientView.m_AddGoldView[i].GetGold();
	////		if(lScore==0)continue;
	////		m_GameClientView.SetUserTableScore(i,0);
	////		m_GameClientView.SetJettonMoveInfo(i,MOVE_POOL_ALLSCORE,lScore);
	////	}
	//return 0;

	//�ƶ�����
	//m_GameClientView.SetJettonMoveInfo(MY_VIEW_CHAIR_ID,MOVE_USER_GETSCORE,10000);

	//BYTE cbCardListData[32]=
	//{
	//	0x02,0x22,0x23,0x04,0x14,0x24,0x34,
	//	0x25,0x35,0x06,0x16,0x26,0x36,
	//	0x07,0x17,0x27,0x37,0x08,0x18,0x28,0x38,
	//	0x29,0x39,0x0A,0x1A,0x2A,0x3A,0x1B,0x3B,0x0C,0x2C,
	//	0x4E,
	//};

	//CopyMemory(m_cbHandCardData,cbCardListData,sizeof(m_cbHandCardData));

	//TCHAR tHeap[32],tTail[32];

	//for(WORD i=0;i<GAME_PLAYER;i++)
	//{
	//	m_GameClientView.SetUserOperate(i,false);
	//	GetHeapTailType(i,tHeap,tTail);
	//	m_GameClientView.SetCardType(i,tHeap,tTail);
	//}
	//m_GameClientView.SetOpenCardInfo(m_cbHandCardData,0,0);
	//return 0;
#endif 

	//�������
	m_wBankerUser=INVALID_CHAIR;							
	m_wCurrentUser=INVALID_CHAIR;				
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lUserGetScore,sizeof(m_lUserGetScore));
	ZeroMemory(m_lUserTax,sizeof(m_lUserTax));

	//�������
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetUserOperate(i,false);
		m_GameClientView.SetBankerUser(INVALID_CHAIR);
		m_GameClientView.m_CardControl[i].SetDisableCardBack(false);
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
		m_GameClientView.SetCardType(INVALID_CHAIR,NULL,NULL);
	}
	m_GameClientView.m_CardControl[MY_VIEW_CHAIR_ID].SetPositively(false);
	m_GameClientView.m_CardControl[MY_VIEW_CHAIR_ID].SetDisplayHead(false);

	//��ע����
	m_lTurnMinScore=0;
	m_lTurnMaxScore=0;	
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));

	//ɾ��ʱ��
	KillTimer(IDI_DELAY_SHOW_BUTTON);
	KillTimer(IDI_DELAY_EXIT_WIN);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);

	//������Ϣ
	if(!IsLookonMode())
	{
		m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
		KillGameClock(IDI_START_GAME);
		SendUserReady(NULL,0);
	}

	return 0;
}

//��ע��ť
LRESULT	CGameClientEngine::OnAddScore(WPARAM wParam, LPARAM lParam)
{
	BYTE cbMsg = BYTE(wParam);
	WORD wMeChairID = GetMeChairID();
	LONGLONG lAddScore = m_GameClientView.m_JetonControl.GetAddJetton();

	//���ذ�ť
	m_GameClientView.m_btClearScore.ShowWindow(SW_HIDE);
	for(WORD i=0;i<CONTROL_COUNT;i++)m_GameClientView.m_btDecrease[i].ShowWindow(SW_HIDE);
	if(IDC_INVEST==cbMsg)
	{
		m_GameClientView.m_btInvest.ShowWindow(SW_HIDE);

		//ɾ��ʱ��
		KillTimer(IDI_TIME_USER_INVEST);	
	}
	else
	{
		m_GameClientView.m_btAddScore.ShowWindow(SW_HIDE);
		m_GameClientView.m_btFollow.ShowWindow(SW_HIDE);
		m_GameClientView.m_btShowHand.ShowWindow(SW_HIDE);
		m_GameClientView.m_btPass.ShowWindow(SW_HIDE);
		m_GameClientView.m_btGiveUp.ShowWindow(SW_HIDE);

		//ɾ��ʱ��
		KillGameClock(IDI_USER_ADD_SCORE);
	}

	//��������
	m_GameClientView.m_JetonControl.SetAddJetton(0);
	m_GameClientView.m_JetonControl.SetUserJetton(0);
	m_GameClientView.m_JetonControl.SetMinJetton(0);
	m_GameClientView.SetUserOperate(MY_VIEW_CHAIR_ID,false);

	//��ť��Ϣ
	switch(cbMsg)
	{
	case IDC_INVEST:			//�±���Ϣ
		{
			//��������
			CMD_C_UserInvest UserInvest;
			UserInvest.lInvestGold = lAddScore;
			SendSocketData(SUB_C_USER_INVEST,&UserInvest,sizeof(UserInvest));

			//��ͼ����
			m_lUserScore[wMeChairID]+=UserInvest.lInvestGold;
			m_GameClientView.SetUserJetton(MY_VIEW_CHAIR_ID,m_lUserScore[wMeChairID]);

			break;
		}
	case IDC_PASS:				//������Ϣ
		{
			//��������
			CMD_C_AddScore AddScore;
			AddScore.lScore=0;
			SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			//��ͼ����
			LONGLONG lMeGoldCount = m_lUserScore[wMeChairID]-m_lTotalScore[wMeChairID];
			m_GameClientView.SetUserJetton(MY_VIEW_CHAIR_ID,lMeGoldCount);

			break;
		}
	case IDC_ADD_SCORE:			//��ע��Ϣ
		{
			//��������
			CMD_C_AddScore AddScore;
			AddScore.lScore=lAddScore;
			SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			//���ó���
			m_lTotalScore[wMeChairID]+=AddScore.lScore;
			m_lTableScore[wMeChairID]+=AddScore.lScore;

			//��ͼ����
			LONGLONG lMeGoldCount = m_lUserScore[wMeChairID]-m_lTotalScore[wMeChairID];
			m_GameClientView.SetUserJetton(MY_VIEW_CHAIR_ID,lMeGoldCount);

			//�ƶ�����
			m_GameClientView.SetJettonMoveInfo(MY_VIEW_CHAIR_ID,MOVE_USER_ADD,AddScore.lScore);

			break;
		}
	case IDC_FOLLOW:			//��ע��Ϣ
		{
			//��������
			CMD_C_AddScore AddScore;
			AddScore.lScore=m_lTurnMinScore;
			SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			//���ó���
			m_lTotalScore[wMeChairID]+=AddScore.lScore;
			m_lTableScore[wMeChairID]+=AddScore.lScore;

			//��ͼ����
			LONGLONG lMeGoldCount = m_lUserScore[wMeChairID]-m_lTotalScore[wMeChairID];
			m_GameClientView.SetUserJetton(MY_VIEW_CHAIR_ID,lMeGoldCount);

			//�ƶ�����
			m_GameClientView.SetJettonMoveInfo(MY_VIEW_CHAIR_ID,MOVE_USER_ADD,AddScore.lScore);

			break;
		}
	case IDC_SHOW_HAND:			//ȫѹ��Ϣ
		{
			//��������
			CMD_C_AddScore AddScore;
			AddScore.lScore=m_lTurnMaxScore;
			SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			//���ó���
			m_lTotalScore[wMeChairID]+=AddScore.lScore;
			m_lTableScore[wMeChairID]+=AddScore.lScore;

			//��ͼ����
			m_GameClientView.SetUserJetton(MY_VIEW_CHAIR_ID,0);

			//�ƶ�����
			m_GameClientView.SetJettonMoveInfo(MY_VIEW_CHAIR_ID,MOVE_USER_ADD,AddScore.lScore);

			break;
		}
	case IDC_GIVE_UP:			//������Ϣ
		{
			//��������
			SendSocketData(SUB_C_GIVE_UP);

			//��ͼ����
			LONGLONG lMeGoldCount = m_lUserScore[wMeChairID]-m_lTotalScore[wMeChairID];
			m_GameClientView.SetUserJetton(MY_VIEW_CHAIR_ID,lMeGoldCount);

			//�˿���Ч
			m_GameClientView.m_CardControl[MY_VIEW_CHAIR_ID].SetDisableCardBack(true);

			break;
		}
	}

	//��������
	switch(cbMsg)
	{
	case IDC_INVEST:			//�±���Ϣ
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("INVEST"));
			break;
		}
	case IDC_PASS:				//������Ϣ
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("NO_ADD"));
			break;
		}
	case IDC_ADD_SCORE:			//��ע��Ϣ
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
			break;
		}
	case IDC_FOLLOW:			//��ע��Ϣ
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("FOLLOW"));
			break;
		}
	case IDC_SHOW_HAND:			//ȫѹ��Ϣ
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));
			break;
		}
	case IDC_GIVE_UP:			//������Ϣ
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));
			break;
		}
	}

	//��ʾ����
	m_GameClientView.SetUserAction(MY_VIEW_CHAIR_ID,cbMsg);

	return 0;
}

//����ť
LRESULT	CGameClientEngine::OnClearScore(WPARAM wParam, LPARAM lParam)
{
	m_GameClientView.m_JetonControl.ClearAddJetton();

	PlayGameSound(AfxGetInstanceHandle(),TEXT("CLEAR"));

	//���½���
	m_GameClientView.RefreshGameView();

	return 0;
}

//��ע��ť
LRESULT	CGameClientEngine::OnDecreaseJetton(WPARAM wParam, LPARAM lParam)
{
	if(m_GameClientView.m_JetonControl.DecreaseJetton(WORD(wParam)-1))
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("HIT_JETTON"));

		//���½���
		m_GameClientView.RefreshGameView();
	}

	return 0;
}

//���ư�ť
LRESULT	CGameClientEngine::OnOpenCard(WPARAM wParam, LPARAM lParam)
{
	//��ȡ����
	WORD wMeChairID=GetMeChairID();
	BYTE cbCardData[MAX_COUNT];
	m_GameClientView.GetHitCardData(cbCardData,MAX_COUNT);

	//�Ƚϴ�С
	BYTE cbSpeCount = 2;
	BYTE cbValue=m_GameLogic.CompareCard(cbCardData,&cbCardData[cbSpeCount],cbSpeCount);
	if(cbValue==TRUE)
	{
		CopyMemory(&m_cbHandCardData[wMeChairID][cbSpeCount],cbCardData,cbSpeCount);
		CopyMemory(m_cbHandCardData[wMeChairID],&cbCardData[cbSpeCount],cbSpeCount);
	}
	else 
	{
		CopyMemory(m_cbHandCardData[wMeChairID],cbCardData,MAX_COUNT);
		m_GameClientView.m_CardControl[MY_VIEW_CHAIR_ID].SetCardData(cbCardData,MAX_COUNT);
	}

	//������Ϣ
	CMD_C_OpenCard OpenCard;
	CopyMemory(OpenCard.cbCardData,m_cbHandCardData[wMeChairID],sizeof(OpenCard.cbCardData));
	SendSocketData(SUB_C_OPEN_CARD,&OpenCard,sizeof(OpenCard));

	//�����˿�
	BYTE cbTempData[MAX_COUNT];
	if(cbValue==TRUE || cbValue==FALSE)
	{
		CopyMemory(cbTempData,cbCardData,MAX_COUNT);
	}
	else 
	{
		CopyMemory(&cbTempData[cbSpeCount],cbCardData,cbSpeCount);
		CopyMemory(cbTempData,&cbCardData[cbSpeCount],cbSpeCount);
	}
	m_GameClientView.m_CardControl[MY_VIEW_CHAIR_ID].SetCardData(cbTempData,MAX_COUNT);
	m_GameClientView.m_CardControl[MY_VIEW_CHAIR_ID].SetDisplayHead(true);

	//���ý���
	m_GameClientView.SetUserOperate(MY_VIEW_CHAIR_ID,false);
	m_GameClientView.SetHitPositively(false);
	m_GameClientView.SetHitCardData(NULL,0);

	//���ư�ť
	m_GameClientView.m_btOpenCard.ShowWindow(SW_HIDE);

	//����ʱ��
	KillTimer(IDI_TIME_OPEN_CARD);

	PlayGameSound(AfxGetInstanceHandle(),TEXT("OPEN_CARD"));

	return 0;
}

//�������
LRESULT CGameClientEngine::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
	//����״̬
	if(GetGameStatus()==GS_TK_OPEN_CARD)
	{
		//���뼯��
		for(WORD i=0;i<GAME_PLAYER;i++)
		{
			LONGLONG lScore = m_GameClientView.m_AddGoldView[i].GetGold();
			if(lScore==0)continue;
			m_GameClientView.SetUserTableScore(i,0);
			m_GameClientView.SetJettonMoveInfo(i,MOVE_POOL_ALLSCORE,lScore);
		}

		//��ע��Ϣ
		TCHAR szBuffer[512]=TEXT("");
		myprintf(szBuffer,CountArray(szBuffer),TEXT("\n��ע��Ϣ:"));
		//InsertGeneralString(szBuffer,RGB(0,0,0),true);
		if(m_pIStringMessage!=NULL)
			m_pIStringMessage->InsertCustomString(TEXT("\n��ע��Ϣ:"),RGB(255,0,0),RGB(255,255,255));
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if(m_lTotalScore[i]==0)continue;

			//�ɼ����
			myprintf(szBuffer,CountArray(szBuffer),TEXT("[%s]����ע:%I64d"),
				m_szAccounts[i],m_lTotalScore[i]);
			//InsertGeneralString(szBuffer,RGB(0,0,0),true);

			if(m_pIStringMessage!=NULL)
				m_pIStringMessage->InsertCustomString(szBuffer,RGB(0,0,0),RGB(255,255,255));
		}

		//��������
		for(WORD i=0;i<GAME_PLAYER;i++)
		{
			if(m_cbPlayStatus[i]==FALSE)continue;
			m_GameClientView.SetUserOperate(GetViewChairID(i),true);
		}

		//���洦��
		SetGameClock(GetMeChairID(),IDI_NULLITY,TIME_USER_OPEN_CARD);
		m_GameClientView.SetWaitOpenCard(true);

		//�����û�
		WORD wMeChairID=GetMeChairID();
		if(m_cbPlayStatus[wMeChairID]!=TRUE || IsLookonMode())return 0;

		//���ý���
		m_GameClientView.SetHitPositively(true);
		m_GameClientView.SetHitCardData(m_cbHandCardData[GetMeChairID()],MAX_COUNT);
		m_GameClientView.m_CardControl[MY_VIEW_CHAIR_ID].SetCardData(NULL,0);

		//���ư�ť
		m_GameClientView.m_btOpenCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOpenCard.EnableWindow(FALSE);

		//����ʱ��
		SetTimer(IDI_TIME_OPEN_CARD,TIME_USER_OPEN_CARD*1000,NULL);

	}
	//������ע
	else if(m_wCurrentUser<GAME_PLAYER)
	{
		//�û�����
		WORD wUserViewID = GetViewChairID(m_wCurrentUser);

		//�û��ؼ�
		if(m_wCurrentUser==GetMeChairID() && !IsLookonMode())
		{
			//���¿ؼ�
			UpdateScoreControl();
		}

		//���ý���
		m_GameClientView.SetUserOperate(wUserViewID,true);

		//����ʱ��
		SetGameClock(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);
	}

	return 0;
}

//�������
LRESULT CGameClientEngine::OnOpenCardFinish(WPARAM wParam, LPARAM lParam)
{
	//�û��÷�
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		LONGLONG lScore = m_lUserGetScore[i]+m_lTotalScore[i];
		if(lScore<=0 || GetTableUserItem(i)==NULL)continue;
		m_GameClientView.SetJettonMoveInfo(GetViewChairID(i),MOVE_USER_GETSCORE,lScore);
	}
	m_GameClientView.SetCenterScore(0);

	SetTimer(IDI_DELAY_SHOW_BUTTON,1000*TIME_DELAY_SHOW_BUTTON,NULL);

	return 0;
}

//��ʱ����
void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case IDI_TIME_USER_INVEST:			//�û��±�
		{
			OnAddScore(IDC_INVEST,0);

			break;
		}
	case IDI_TIME_OPEN_CARD:			//�û�����
		{
			OnOpenCard(0,0);

			break;
		}
	case IDI_DELAY_SHOW_BUTTON:			//��Ǯ����
		{
			if(!m_GameClientView.IsJettonMoveing())
			{
				//����״̬
				SetGameStatus(GS_TK_FREE);

				//ɾ��ʱ��
				KillTimer(nIDEvent);

				//��ע����
				for(WORD i=0;i<GAME_PLAYER;i++)if(m_lTotalScore[i]>0)break;

				//������Ϣ
				if(i<GAME_PLAYER)
				{
					//���û���
					m_GameClientView.m_ScoreView.ResetScore();
					for(WORD i=0;i<GAME_PLAYER;i++)
					{
						if(m_lTotalScore[i]==0)continue;
						m_GameClientView.m_ScoreView.SetGameScore(i,m_szAccounts[i],m_lUserGetScore[i],m_lTotalScore[i]);
						m_GameClientView.m_ScoreView.SetGameTax(m_lUserTax[i],i);
					}
					m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

					//�ɼ���ʾ
					TCHAR szBuffer[512]=TEXT("");
					myprintf(szBuffer,CountArray(szBuffer),TEXT("\n���ֽ���,�ɼ�ͳ��:"));
					//InsertGeneralString(szBuffer,RGB(0,0,255),true);

					if(m_pIStringMessage!=NULL)
						m_pIStringMessage->InsertCustomString(szBuffer,RGB(0,0,255),RGB(255,255,255));

					for (WORD i=0;i<GAME_PLAYER;i++)
					{
						if(m_lTotalScore[i]==0)continue;

						//�ɼ����
						myprintf(szBuffer,CountArray(szBuffer),
							TEXT("[%s]�÷�:%+I64d;"),/*��ע:%I64d,	*/
							m_szAccounts[i],/*m_lTotalScore[i],*/m_lUserGetScore[i]);
						//InsertGeneralString(szBuffer,RGB(0,0,255),true);
						if(m_pIStringMessage!=NULL)
							m_pIStringMessage->InsertCustomString(szBuffer,RGB(0,0,255),RGB(255,255,255));
					}
				}

				WORD wMeChairID = GetMeChairID();
				if(!IsLookonMode())
				{
					//��ʼ��ť
					m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
					m_GameClientView.m_btStart.SetFocus();
					SetGameClock(wMeChairID,IDI_START_GAME,TIME_USER_START_GAME);
				}

				//��������
				if(m_lUserGetScore[wMeChairID]<0)
					PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
				else if(m_lUserGetScore[wMeChairID]>0)PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
				else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));
			}
			break;
		}
	case IDI_DELAY_EXIT_WIN:				//��ʱ����
		{
			//ɾ��ʱ��
			KillTimer(nIDEvent);

			OnOpenCardFinish(0,0);

			break;
		}
	}

	CGameFrameEngine::OnTimer(nIDEvent);
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}
//////////////////////////////////////////////////////////////////////////

