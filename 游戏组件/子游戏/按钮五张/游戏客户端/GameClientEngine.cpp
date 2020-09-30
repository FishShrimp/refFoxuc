#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_USER_ADD_SCORE			201									//��ע��ʱ��

//ʱ���ʶ
#define TIME_START_GAME				30									//��ʼ��ʱ��
#define TIME_USER_ADD_SCORE			30									//������ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine,CGameFrameEngine)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_FOLLOW,OnFollow)
	ON_MESSAGE(IDM_GIVE_UP,OnGiveUp)
	ON_MESSAGE(IDM_ADD_SCORE,OnAddScore)
	ON_MESSAGE(IDM_SHOW_HAND,OnShowHand)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
	//��ע��Ϣ
	m_lCellScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	m_lShowHandScore = 0L;
	ZeroMemory( m_lUserScore,sizeof(m_lUserScore) );

	//״̬����
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{
	//����ͼ��
	//��������
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

	return true;
}

//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
	//��ע��Ϣ
	m_lCellScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	m_lShowHandScore = 0L;
	ZeroMemory( m_lUserScore,sizeof(m_lUserScore) );

	//״̬����
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	return true;
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
			return true;
		}
	case IDI_USER_ADD_SCORE:	//��ע��ʱ��
		{
			//��ȡλ��
			WORD wViewChairID=SwitchViewChairID(wChairID);

			//��ֹ�ж�
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wViewChairID==MYSELF_VIEW_ID)) OnGiveUp(1,1);
				return false;
			}
			return true;
		}
	}

	return false;
}


bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}
//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			//��������
			m_GameClientView.FinishDispatchCard();

			//��Ϣ����
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_ADD_SCORE:	//�û���ע
		{
			//��������
			m_GameClientView.FinishDispatchCard();

			//��Ϣ����
			return OnSubAddScore(pData,wDataSize);
		}
	case SUB_S_GIVE_UP:		//�û�����
		{
			//��������
			m_GameClientView.FinishDispatchCard();

			//��Ϣ����
			return OnSubGiveUp(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:	//������Ϣ
		{
			//��������
			m_GameClientView.FinishDispatchCard();

			//��Ϣ����
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			//��������
			m_GameClientView.FinishDispatchCard();

			//��Ϣ����
			return OnSubGameEnd(pData,wDataSize);
		}
	}

	return false;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//���ÿؼ�
			if (IsLookonMode()==false&& GetMeUserItem()->GetUserStatus()!=US_READY)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
				SetGameClock(GetMeChairID(),IDI_START_GAME,30);
			}

			return true;
		}
		
	case GAME_STATUS_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//��ע��Ϣ
			m_lCellScore=pStatusPlay->lCellScore;
			m_lTurnMaxScore=pStatusPlay->lTurnMaxScore;
			m_lTurnLessScore=pStatusPlay->lTurnLessScore;
			m_lShowHandScore = pStatusPlay->lShowHandScore;
			CopyMemory(m_lTableScore,pStatusPlay->lTableScore,sizeof(m_lTableScore));

			//״̬����
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_bShowHand=(pStatusPlay->bShowHand==TRUE)?true:false;
			CopyMemory(m_cbPlayStatus,pStatusPlay->cbPlayStatus,sizeof(m_cbPlayStatus));

			//�ʺ�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IClientUserItem * pIClientUserItem=GetTableUserItem(i);
				if (pIClientUserItem!=NULL)
				{
					const tagUserInfo *pUserData=pIClientUserItem->GetUserInfo();
					m_lUserScore[i] = pUserData->lScore;
					lstrcpyn(m_szAccounts[i],pUserData->szNickName,CountArray(m_szAccounts[i]));
					m_GameClientView.SetUserScore( SwitchViewChairID(i),pUserData->lScore );
				}
			}

			//���ý���
			LONGLONG lTableScore=0L;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//����λ��
				WORD wViewChairID=SwitchViewChairID(i);

				//�����˿�
				if (m_cbPlayStatus[i]==TRUE) 
				{
					BYTE cbCardCount=pStatusPlay->cbCardCount[i];
					m_GameClientView.m_CardControl[wViewChairID].SetCardData(pStatusPlay->cbHandCardData[i],cbCardCount);
				}
				lTableScore += m_lTableScore[2*i+1];
				//��������
				m_GameClientView.m_PlayerJeton[wViewChairID].SetScore(m_lTableScore[2*i]);
				//������ע
				m_GameClientView.SetUserTableScore(wViewChairID,m_lTableScore[2*i]+m_lTableScore[2*i+1]);
			}
			m_GameClientView.m_PlayerJeton[GAME_PLAYER].SetScore(lTableScore);

			//
			m_GameClientView.SetCellScore(m_lCellScore);

			//�������
			if (IsLookonMode()==false) 
			{
				//��������
				m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetPositively(true);
				if (m_wCurrentUser==GetMeChairID()) UpdateScoreControl();
			}

			//���ö�ʱ��
			SetGameClock(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);

			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//����״̬
	SetGameStatus(GAME_STATUS_PLAY);

	//��ע����
	m_lCellScore=pGameStart->lCellScore;
	m_lTurnMaxScore=pGameStart->lTurnMaxScore;
	m_lTurnLessScore=pGameStart->lTurnLessScore;
	m_lShowHandScore = pGameStart->lShowHandScore;
	ZeroMemory( m_lUserScore,sizeof(m_lUserScore) );

	//״̬����
	m_wCurrentUser=pGameStart->wCurrentUser;

	//���ñ���
	LONGLONG lTableScore=0L;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IClientUserItem* pUserItem=GetTableUserItem(i);
		if (pUserItem==NULL) continue;

		//��Ϸ��Ϣ
		m_cbPlayStatus[i]=TRUE;
		m_lUserScore[i] = pUserItem->GetUserScore();
		lTableScore+=m_lCellScore;
		m_lTableScore[2*i+1]=m_lCellScore;

		//�û�����
		lstrcpyn(m_szAccounts[i],pUserItem->GetNickName(),CountArray(m_szAccounts[i]));

		m_GameClientView.SetUserScore( SwitchViewChairID(i),pUserItem->GetUserScore());
	}

	//���ý���
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L);
	m_GameClientView.SetCellScore(m_lCellScore);

	//���ý���
	lTableScore = 0L;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);

		//�����˿�
		m_GameClientView.m_CardControl[wViewChairID].SetCardData(NULL,0); 
		m_GameClientView.m_CardControl[wViewChairID].SetDisplayHead(false);
		lTableScore += m_lTableScore[2*i+1];
		//���ó���
		m_GameClientView.m_PlayerJeton[wViewChairID].SetScore(m_lTableScore[2*i]);
		m_GameClientView.SetUserTableScore(wViewChairID,m_lTableScore[i*2]+m_lTableScore[i*2+1]);
	}
	m_GameClientView.m_PlayerJeton[GAME_PLAYER].SetScore(lTableScore);

	//�ɷ��˿�
	for (BYTE cbIndex=0;cbIndex<2;cbIndex++)
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (m_cbPlayStatus[i]==TRUE)
			{
				//��������
				WORD wViewChairID=SwitchViewChairID(i);
				BYTE cbCardData[2]={0,pGameStart->cbCardData[i]};

				//�ɷ��˿�
				cbCardData[0]=(GetMeChairID()==i)?pGameStart->cbObscureCard:0;
				m_GameClientView.DispatchUserCard(wViewChairID,cbCardData[cbIndex]);
			}
		}
	}

	//�������
	if (IsLookonMode()==false) 
	{
		//��������
		m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetPositively(true);
	}

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//������
	if (IsLookonMode()==false) ActiveGameFrame();

	return true;
}

//�û���ע
bool CGameClientEngine::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wAddScoreUser=pAddScore->wAddScoreUser;
	WORD wViewChairID=SwitchViewChairID(wAddScoreUser);

	//����ж�
	m_bShowHand = pAddScore->bShowHand;
	if( m_bShowHand && m_cbPlayStatus[wAddScoreUser] ) m_GameClientView.SetUserShowHand( true );

	//��ע����
	if ((IsLookonMode()==true)||(pAddScore->wAddScoreUser!=wMeChairID))
	{
		if (m_cbPlayStatus[wAddScoreUser]==TRUE)
		{
			//��ע����
			m_GameClientView.m_PlayerJeton[wViewChairID].SetScore(pAddScore->lAddScoreCount);

			//��������
			//��ȡ����
			LONGLONG lTableScore=m_lTableScore[wAddScoreUser*2+1];
			LONGLONG lTurnAddScore=m_lTableScore[wAddScoreUser*2];

			//��������
			if ((pAddScore->lAddScoreCount-lTurnAddScore)==0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("NO_ADD"));
			else if (m_bShowHand) PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));
			else if ((pAddScore->lAddScoreCount+lTableScore)==m_lTurnLessScore) PlayGameSound(AfxGetInstanceHandle(),TEXT("FOLLOW"));
			else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));
		}
	}

	//���ñ���
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTurnLessScore=pAddScore->lTurnLessScore;
	if( pAddScore->lAddScoreCount > 0L )
		m_lTableScore[wAddScoreUser*2]=pAddScore->lAddScoreCount;

	//���ó���
	m_GameClientView.SetUserTableScore(wViewChairID,m_lTableScore[wAddScoreUser*2+1]+m_lTableScore[wAddScoreUser*2]);
	m_GameClientView.InvalidGameView(0,0,0,0);

	//���ƽ���
	if ((IsLookonMode()==false)&&(pAddScore->wCurrentUser==wMeChairID))
	{
		ActiveGameFrame();
		UpdateScoreControl();
	}

	//����ʱ��
	if (m_wCurrentUser==INVALID_CHAIR) KillGameClock(IDI_USER_ADD_SCORE);
	else SetGameClock(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);

	return true;
}

//�û�����
bool CGameClientEngine::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;

	//���ñ���
	m_cbPlayStatus[pGiveUp->wGiveUpUser]=FALSE;

	//��������
	WORD wGiveUpUser=pGiveUp->wGiveUpUser;
	BYTE cbCardData[MAX_COUNT]={0,0,0,0,0};
	WORD wViewChairID=SwitchViewChairID(wGiveUpUser);
	BYTE cbCardCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetCardCount();

	//�����˿�
	m_GameClientView.m_CardControl[wViewChairID].SetPositively(false);
	m_GameClientView.m_CardControl[wViewChairID].SetDisplayHead(false);
	m_GameClientView.m_CardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);

	//״̬����
	if ((IsLookonMode()==false)&&(pGiveUp->wGiveUpUser==GetMeChairID())) 
	{
		SetGameStatus(GAME_STATUS_FREE);
	}

	//��������
	if( m_lTurnMaxScore != pGiveUp->lTurnMaxScore )
	{
		//��������
		m_lTurnMaxScore = pGiveUp->lTurnMaxScore;

		if( !IsLookonMode() && m_wCurrentUser==GetMeChairID() && wGiveUpUser != m_wCurrentUser )
			UpdateScoreControl();
	}

	//��������
	if (wGiveUpUser==GetClockChairID()) KillGameClock(IDI_USER_ADD_SCORE);
	if ((IsLookonMode()==true)||(wGiveUpUser!=GetMeChairID())) PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));

	return true;
}

//������Ϣ
bool CGameClientEngine::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//���ñ���
	m_wCurrentUser=pSendCard->wCurrentUser;
	m_lTurnMaxScore=pSendCard->lTurnMaxScore;
	m_lTurnLessScore=pSendCard->lTurnLessScore;

	//���ó���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		m_GameClientView.m_PlayerJeton[wViewChairID].SetScore(0L);
		m_GameClientView.m_PlayerJeton[GAME_PLAYER].AddScore(m_lTableScore[i*2]);
	}

	//�ۼƽ��
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_lTableScore[i*2+1]+=m_lTableScore[i*2];
		m_lTableScore[i*2]=0L;
	}

	//�ɷ��˿�,���ϴ������ҿ�ʼ����
	WORD wLastMostUser = pSendCard->wLastMostUser;
	ASSERT( wLastMostUser != INVALID_CHAIR );
	for (BYTE i=0;i<pSendCard->cbSendCardCount;i++)
	{
		for (WORD j=0;j<GAME_PLAYER;j++)
		{
			WORD wChairId = (wLastMostUser+j)%GAME_PLAYER;
			if (m_cbPlayStatus[wChairId]==TRUE)
			{
				WORD wViewChairID=SwitchViewChairID(wChairId);
				m_GameClientView.DispatchUserCard(wViewChairID,pSendCard->cbCardData[wChairId][i]);
			}
		}
	}

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//״̬����
	HideScoreControl();
	SetGameStatus(GAME_STATUS_FREE);
	KillGameClock(IDI_USER_ADD_SCORE);
	m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L);

	//��������
	if (IsLookonMode()==false)
	{
		if (pGameEnd->lGameScore[GetMeChairID()]>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
		else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	}
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//��������
	m_GameClientView.m_ScoreView.ResetScore();
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ϣ
		if (pGameEnd->lGameScore[i]!=0L)
		{
			m_GameClientView.m_ScoreView.SetGameScore(i,m_szAccounts[i],pGameEnd->lGameScore[i]);
			//��˰
			m_GameClientView.m_ScoreView.SetGameTax(i,pGameEnd->lGameTax[i]);
			if (pGameEnd->cbCardData[i]!=0)
			{
				//��ȡ�˿�
				BYTE cbCardData[5];
				WORD wViewChairID=SwitchViewChairID(i);
				BYTE cbCardCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetCardData(cbCardData,CountArray(cbCardData));

				//�����˿�
				cbCardData[0]=pGameEnd->cbCardData[i];
				m_GameClientView.m_CardControl[wViewChairID].SetDisplayHead(true);
				m_GameClientView.m_CardControl[wViewChairID].SetCardData(cbCardData,cbCardCount);
			}
		}
	}
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//���û���
	TCHAR szBuffer[128] = TEXT("");
	if(m_pIStringMessage != NULL)
		m_pIStringMessage->InsertSystemString(TEXT("\n���ֽ���,�ɼ�ͳ��:\n"));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if( pGameEnd->lGameScore[i] != 0L && m_szAccounts[i][0]!=0 )
		{
			//���������ʾ�ɼ�
			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%s: %+I64d��\n"),m_szAccounts[i],pGameEnd->lGameScore[i]);
			if(m_pIStringMessage != NULL)
				m_pIStringMessage->InsertNormalString(szBuffer);
		}
	}

	//��ע��Ϣ
	m_lCellScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//״̬����
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));

	//��ʼ��ť
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}

	return true;
}

//���ؿ���
void CGameClientEngine::HideScoreControl()
{
	//���ư�ť
	m_GameClientView.m_btFollow.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUp.ShowWindow(SW_HIDE);
	m_GameClientView.m_btShowHand.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAddTimes1.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAddTimes2.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAddTimes3.ShowWindow(SW_HIDE);
	return;
}

//���¿���
void CGameClientEngine::UpdateScoreControl()
{
	//��������
	WORD wMeChairID=GetMeChairID();
	BYTE cbCardCount=(BYTE)m_GameClientView.m_CardControl[MYSELF_VIEW_ID].GetCardCount();
	LONGLONG lDrawAddScore=m_lTableScore[wMeChairID*2]+m_lTableScore[wMeChairID*2+1];

	//��ע��ť
	m_GameClientView.m_btAddTimes1.ShowWindow(SW_SHOW);
	m_GameClientView.m_btAddTimes2.ShowWindow(SW_SHOW);
	m_GameClientView.m_btAddTimes3.ShowWindow(SW_SHOW);
	LONGLONG lLeaveScore=__max(m_lTurnMaxScore-m_lTurnLessScore,0L);
	m_GameClientView.m_btAddTimes1.EnableWindow(lLeaveScore>=m_lCellScore?TRUE:FALSE);
	m_GameClientView.m_btAddTimes2.EnableWindow(lLeaveScore>=m_lCellScore*2?TRUE:FALSE);
	m_GameClientView.m_btAddTimes3.EnableWindow(lLeaveScore>=m_lCellScore*3?TRUE:FALSE);

	//��ʾ��ť
	LONGLONG lUserScore = m_lUserScore[wMeChairID];
	m_GameClientView.m_btFollow.ShowWindow(SW_SHOW);
	if( !m_bShowHand && lUserScore > m_lTurnLessScore )
		m_GameClientView.m_btFollow.EnableWindow();
	else
		m_GameClientView.m_btFollow.EnableWindow(FALSE);

	m_GameClientView.m_btGiveUp.ShowWindow(SW_SHOW);
	m_GameClientView.m_btShowHand.ShowWindow(SW_SHOW);

	//���ư�ť
	lLeaveScore = m_lTurnMaxScore-m_lTableScore[wMeChairID*2+1]-m_lTableScore[wMeChairID*2];
	m_GameClientView.m_btShowHand.EnableWindow((cbCardCount>=3&&(m_bShowHand||lLeaveScore>0L))?TRUE:FALSE);

	return;
}

//��ʼ��ť
LRESULT	CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
	//ɾ����ʱ��
	KillGameClock(IDI_START_GAME);

	//���ÿؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_PlayerJeton[i].SetScore(0L);
		m_GameClientView.m_CardControl[i].SetCardData(NULL,0);
	}
	m_GameClientView.m_PlayerJeton[GAME_PLAYER].SetScore(0L);

	//���ý���
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserTableScore(INVALID_CHAIR,0L);
	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//������ť
LRESULT CGameClientEngine::OnGiveUp(WPARAM wParam, LPARAM lParam)
{
	//��������
	HideScoreControl();

	//��������
	KillGameClock(IDI_USER_ADD_SCORE);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GIVE_UP"));

	//������Ϣ
	SendSocketData(SUB_C_GIVE_UP,0,0);

	return 0;
}

//��ע��ť
LRESULT CGameClientEngine::OnFollow(WPARAM wParam, LPARAM lParam)
{
	//��ȡ����
	WORD wMeChairID=GetMeChairID();
	LONGLONG lCurrentScore=m_lTurnLessScore-m_lTableScore[wMeChairID*2+1];

	//��ע����
	HideScoreControl();
	m_GameClientView.m_PlayerJeton[MYSELF_VIEW_ID].SetScore(lCurrentScore);

	//��������
	KillGameClock(IDI_USER_ADD_SCORE);

	PlayGameSound(AfxGetInstanceHandle(),0L==lCurrentScore?TEXT("NO_ADD"):TEXT("FOLLOW"));

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentScore;
	SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	return 0;
}

//��ע��ť 
LRESULT	CGameClientEngine::OnAddScore(WPARAM wParam, LPARAM lParam)
{
	//��ȡ����
	WORD wMeChairID=GetMeChairID();
	LONGLONG lCurrentScore=m_lTurnLessScore-m_lTableScore[wMeChairID*2+1]+m_lCellScore*(lParam);

	//��ע����
	HideScoreControl();
	m_GameClientView.m_PlayerJeton[MYSELF_VIEW_ID].SetScore(lCurrentScore);

	//��������
	KillGameClock(IDI_USER_ADD_SCORE);
	m_GameClientView.InvalidGameView(0,0,0,0);

	LONGLONG lShowHandScore = __min(m_lUserScore[wMeChairID],m_lShowHandScore);
	if( lCurrentScore + m_lTableScore[wMeChairID*2+1] == lShowHandScore )
		PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_SCORE"));

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentScore;
	SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	return 0;
}

//�����ť
LRESULT CGameClientEngine::OnShowHand(WPARAM wParam, LPARAM lParam)
{
	//��������
	WORD wMeChairID=GetMeChairID();
	const tagUserInfo* pUserData=NULL;

	//��ȡ�û�
	if (wMeChairID!=INVALID_CHAIR)
	{
		pUserData=GetMeUserItem()->GetUserInfo();
		if (pUserData==NULL) return 0;
	}

	//��֤
	BYTE cbCardCount=(BYTE)m_GameClientView.m_CardControl[MYSELF_VIEW_ID].GetCardCount();
	if( cbCardCount < 3 || m_wCurrentUser!=wMeChairID ) return 0;

	//���ͳ��
	LONGLONG lShowHandScore = __min(m_lUserScore[wMeChairID],m_lShowHandScore);
	LONGLONG lCurrentScore=lShowHandScore-m_lTableScore[wMeChairID*2+1];

	//��ע����
	HideScoreControl();
	m_GameClientView.m_PlayerJeton[MYSELF_VIEW_ID].SetScore(lCurrentScore);

	//��������
	KillGameClock(TIME_USER_ADD_SCORE);
	m_GameClientView.InvalidGameView(0,0,0,0);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("SHOW_HAND"));

	//������Ϣ
	CMD_C_AddScore AddScore;
	AddScore.lScore=lCurrentScore;
	SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

	return 0;
}

//�������
LRESULT CGameClientEngine::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_wCurrentUser==INVALID_CHAIR) return 0;

	//��ע״̬
	if ((IsLookonMode()==false)&&(GetMeChairID()==m_wCurrentUser))
	{
		//��������
		ActiveGameFrame();
		UpdateScoreControl();
	}

	//����ʱ��
	SetGameClock(m_wCurrentUser,IDI_USER_ADD_SCORE,TIME_USER_ADD_SCORE);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
