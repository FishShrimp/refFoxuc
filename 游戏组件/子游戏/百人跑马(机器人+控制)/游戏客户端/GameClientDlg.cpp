#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "DialogMessage.h"
#include ".\gameclientdlg.h"

//////////////////////////////////////////////////////////////////////////
//��ʱ��ʶ
#define IDI_FREE					100									//��Ϣʱ��
#define IDI_BET_START				101									//��ʼ��ע
#define IDI_BET_END					102									//��ע����
#define IDI_HORSES_START			103									//����ʼ

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_MESSAGE(IDM_PLAYER_BET,OnPlayerBet)
	ON_MESSAGE(IDM_ADMIN_COMMDN, OnAdminControl)	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{

	//������
	for ( int i = 0; i < CountArray(m_nMultiple); ++i)
		m_nMultiple[i] = 1;

	//��Ϸ����
	memset(m_lPlayerBet, 0, sizeof(m_lPlayerBet));
	memset(m_lPlayerBetAll, 0, sizeof(m_lPlayerBetAll));

	//���Ʊ���
	m_lAreaLimitScore = 0l;
	m_lUserLimitScore = 0l;

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
	SetWindowText(TEXT("��������  --  Ver��6.6.1.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

	return true;
}

//���ú���
bool CGameClientEngine::OnResetGameEngine()
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

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}
//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
	switch(nTimerID)
	{
	case IDI_FREE:			//��Ϸ����
		{
			if( nElapse == 0 && wChairID == GetMeChairID() )
			{
				m_GameClientView.m_DlgPlayBet.SetCanBet(false);
				return false;
			}
		}
		return true;
	case IDI_BET_START:		//��ע��ʼ
		{
			if( nElapse == 0 && wChairID == GetMeChairID() )
			{
				m_GameClientView.m_DlgPlayBet.SetCanBet(false);
				return false;
			}
		}
		return true;
	case IDI_BET_END:		//��ע����
		{
			if( nElapse == 0 && wChairID == GetMeChairID() )
			{
				m_GameClientView.m_DlgPlayBet.SetCanBet(false);
				return false;
			}
		}
		return true;
	case IDI_HORSES_START:	//����ʼ
		{
			if( nElapse == 0 && wChairID == GetMeChairID() )
			{
				m_GameClientView.m_DlgPlayBet.SetCanBet(false);
				return false;
			}
		}
		return true;
	}
	return true;
}

//�Թ�״̬
bool CGameClientEngine:: OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}


//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch(wSubCmdID)
	{
	case SUB_S_BET_START:			//��ʼ��ע
		return OnSubBetStart(pData, wDataSize);

	case SUB_S_BET_END:				//��ע����
		return OnSubBetEnd(pData, wDataSize);

	case SUB_S_HORSES_START:		//����ʼ
		return OnSubHorsesStart(pData, wDataSize);

	case SUB_S_HORSES_END:			//�������
		return OnSubHorsesEnd(pData, wDataSize);

	case SUB_S_PLAYER_BET:			//�û���ע
		return OnSubPlayerBet(pData, wDataSize);

	case SUB_S_PLAYER_BET_FAIL:		//��עʧ��
		return OnSubPlayerBetFail(pData, wDataSize);

	case SUB_S_NAMED_HORSES:		//��ƨ����
		return true;

	case SUB_S_MANDATOY_END:		//ǿ�ƽ���
		return OnSubMandatoryEnd(pData, wDataSize);

	case SUB_S_ADMIN_COMMDN:		//����
		return OnSubAdminControl(pData,wDataSize);

	}
	//�������
	ASSERT(FALSE);
	return true;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStation, bool bLookonUser, VOID * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_FREE:	
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_SceneFree));
			if (wDataSize!=sizeof(CMD_S_SceneFree)) return false;

			//��Ϣ����
			CMD_S_SceneFree * pSceneFree = (CMD_S_SceneFree *)pBuffer;

			const tagServerAttribute * pServerAttribute = (m_pIClientKernel!=NULL)?m_pIClientKernel->GetServerAttribute():NULL;
			if ( pServerAttribute != NULL )
				m_GameClientView.SetKindID(pServerAttribute->wKindID);


			//����״̬
			SetGameStatus(GS_FREE);
			m_GameClientView.SetGameStatus(GS_FREE);
			m_GameClientView.m_DlgPlayBet.SetCanBet(false);

			//����λ��
			WORD wMeChairID = GetMeChairID();
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));

			//����ʱ��
			SetGameClock(GetMeChairID(), IDI_FREE, pSceneFree->nTimeLeave);

			//���ó���
			m_GameClientView.SetStreak(pSceneFree->nStreak);

			//���ñ���
			memcpy(m_nMultiple, pSceneFree->nMultiple, sizeof(m_nMultiple));
			m_GameClientView.m_DlgPlayBet.SetMultiple(m_nMultiple);

			//��������
			for (int i = 0; i < HORSES_ALL; ++i)
				m_GameClientView.SetHorsesName(i, pSceneFree->szHorsesName[i]);

			//��ʷ��¼
			m_GameClientView.m_DlgRecord.m_GameRecords.RemoveAll();
			for ( int i = 0; i < MAX_SCORE_HISTORY; ++i )
			{
				if ( pSceneFree->GameRecords[i].nStreak != 0)
				{
					m_GameClientView.m_DlgRecord.m_GameRecords.Add(pSceneFree->GameRecords[i]);
				}
			}

			//ȫ��Ӯ�ĳ���
			m_GameClientView.m_DlgStatistics.SetWinCount(pSceneFree->nWinCount);

			//���Ʊ���
			m_lAreaLimitScore = pSceneFree->lAreaLimitScore;						//����������
			m_lUserLimitScore = pSceneFree->lUserLimitScore;						//������������

			//������ƥ
			m_GameClientView.NweHorses();

			//��Ϸ����
			memset(m_lPlayerBet, 0, sizeof(m_lPlayerBet));
			memset(m_lPlayerBetAll, 0, sizeof(m_lPlayerBetAll));
			
			//����
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.GetSafeHwnd())
				m_GameClientView.m_btOpenControl.ShowWindow(SW_SHOW);

			//���¿���
			UpdateControls();
			return true;
		}
	case GS_BET:	
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_SceneBet));
			if (wDataSize!=sizeof(CMD_S_SceneBet)) return false;

			//��Ϣ����
			CMD_S_SceneBet * pSceneBet = (CMD_S_SceneBet *)pBuffer;

			//����ID
			const tagServerAttribute * pServerAttribute = (m_pIClientKernel!=NULL)?m_pIClientKernel->GetServerAttribute():NULL;
			if ( pServerAttribute != NULL )
				m_GameClientView.SetKindID(pServerAttribute->wKindID);

			
			//����״̬
			SetGameStatus(GS_BET);
			m_GameClientView.SetGameStatus(GS_BET);
			m_GameClientView.m_DlgPlayBet.SetCanBet(true);

			//����λ��
			WORD wMeChairID = GetMeChairID();
			WORD wMeViewChairID = SwitchViewChairID(wMeChairID);
			m_GameClientView.SetMeChairID(wMeViewChairID);

			//����ʱ��
			SetGameClock(GetMeChairID(), IDI_BET_START, pSceneBet->nTimeLeave);

			//���ó���
			m_GameClientView.SetStreak(pSceneBet->nStreak);

			//���ñ���
			memcpy(m_nMultiple, pSceneBet->nMultiple, sizeof(m_nMultiple));
			m_GameClientView.m_DlgPlayBet.SetMultiple(m_nMultiple);

			//��������
			for (int i = 0; i < HORSES_ALL; ++i)
				m_GameClientView.SetHorsesName(i, pSceneBet->szHorsesName[i]);

			//��ʷ��¼
			m_GameClientView.m_DlgRecord.m_GameRecords.RemoveAll();
			for ( int i = 0; i < MAX_SCORE_HISTORY; ++i )
			{
				if ( pSceneBet->GameRecords[i].nStreak != 0)
				{
					m_GameClientView.m_DlgRecord.m_GameRecords.Add(pSceneBet->GameRecords[i]);
				}
			}

			//ȫ��Ӯ�ĳ���
			m_GameClientView.m_DlgStatistics.SetWinCount(pSceneBet->nWinCount);

			//������ע����
			m_GameClientView.SetBetPlayerCount(pSceneBet->nBetPlayerCount);

			//���������ע
			pSceneBet->lUserMaxScore;

			//������ע
			memcpy(m_lPlayerBet, pSceneBet->lPlayerBet, sizeof(m_lPlayerBet));
			for ( int i = 0; i < AREA_ALL; ++i)
				m_GameClientView.SetPlayerBet(wMeViewChairID, i, m_lPlayerBet[i]);

			//������ע
			memcpy(m_lPlayerBetAll, pSceneBet->lPlayerBetAll, sizeof(m_lPlayerBetAll));
			for ( int i = 0; i < AREA_ALL; ++i)
				m_GameClientView.SetAllBet(i, m_lPlayerBetAll[i]);

			//���Ʊ���
			m_lAreaLimitScore = pSceneBet->lAreaLimitScore;						//����������
			m_lUserLimitScore = pSceneBet->lUserLimitScore;						//������������

			//����
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.GetSafeHwnd())
				m_GameClientView.m_btOpenControl.ShowWindow(SW_SHOW);

			//���¿���
			UpdateControls();
			return true;
		}
	case GS_BET_END:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_SceneBetEnd));
			if (wDataSize!=sizeof(CMD_S_SceneBetEnd)) return false;

			//��Ϣ����
			CMD_S_SceneBetEnd * pSceneBetEnd = (CMD_S_SceneBetEnd *)pBuffer;

			const tagServerAttribute * pServerAttribute = (m_pIClientKernel!=NULL)?m_pIClientKernel->GetServerAttribute():NULL;
			if ( pServerAttribute != NULL )
				m_GameClientView.SetKindID(pServerAttribute->wKindID);


			//����״̬
			SetGameStatus(GS_BET_END);
			m_GameClientView.SetGameStatus(GS_BET_END);
			m_GameClientView.m_DlgPlayBet.SetCanBet(false);

			//����λ��
			WORD wMeChairID = GetMeChairID();
			WORD wMeViewChairID = SwitchViewChairID(wMeChairID);
			m_GameClientView.SetMeChairID(wMeViewChairID);

			//����ʱ��
			SetGameClock(GetMeChairID(), IDI_BET_START, pSceneBetEnd->nTimeLeave);

			//���ó���
			m_GameClientView.SetStreak(pSceneBetEnd->nStreak);

			//���ñ���
			memcpy(m_nMultiple, pSceneBetEnd->nMultiple, sizeof(m_nMultiple));
			m_GameClientView.m_DlgPlayBet.SetMultiple(m_nMultiple);

			//��������
			for (int i = 0; i < HORSES_ALL; ++i)
				m_GameClientView.SetHorsesName(i, pSceneBetEnd->szHorsesName[i]);

			//��ʷ��¼
			m_GameClientView.m_DlgRecord.m_GameRecords.RemoveAll();
			for ( int i = 0; i < MAX_SCORE_HISTORY; ++i )
			{
				if ( pSceneBetEnd->GameRecords[i].nStreak != 0)
				{
					m_GameClientView.m_DlgRecord.m_GameRecords.Add(pSceneBetEnd->GameRecords[i]);
				}
			}

			//ȫ��Ӯ�ĳ���
			m_GameClientView.m_DlgStatistics.SetWinCount(pSceneBetEnd->nWinCount);

			//������ע����
			m_GameClientView.SetBetPlayerCount(pSceneBetEnd->nBetPlayerCount);

			//������ע
			memcpy(m_lPlayerBet, pSceneBetEnd->lPlayerBet, sizeof(m_lPlayerBet));
			for ( int i = 0; i < AREA_ALL; ++i)
				m_GameClientView.SetPlayerBet(wMeViewChairID, i, m_lPlayerBet[i]);

			//������ע
			memcpy(m_lPlayerBetAll, pSceneBetEnd->lPlayerBetAll, sizeof(m_lPlayerBetAll));
			for ( int i = 0; i < AREA_ALL; ++i)
				m_GameClientView.SetAllBet(i, m_lPlayerBetAll[i]);

			//���Ʊ���
			m_lAreaLimitScore = pSceneBetEnd->lAreaLimitScore;						//����������
			m_lUserLimitScore = pSceneBetEnd->lUserLimitScore;						//������������

			//����
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.GetSafeHwnd())
				m_GameClientView.m_btOpenControl.ShowWindow(SW_SHOW);

			//���¿���
			UpdateControls();
			return true;
		}
	case GS_HORSES:			
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_SceneHorses));
			if (wDataSize!=sizeof(CMD_S_SceneHorses)) return false;

			//��Ϣ����
			CMD_S_SceneHorses * pSceneHorses = (CMD_S_SceneHorses *)pBuffer;

			const tagServerAttribute * pServerAttribute = (m_pIClientKernel!=NULL)?m_pIClientKernel->GetServerAttribute():NULL;
			if ( pServerAttribute != NULL )
				m_GameClientView.SetKindID(pServerAttribute->wKindID);

			//����״̬
			SetGameStatus(GS_HORSES);
			m_GameClientView.SetGameStatus(GS_HORSES);
			m_GameClientView.m_DlgPlayBet.SetCanBet(false);

			//����λ��
			WORD wMeChairID = GetMeChairID();
			WORD wMeViewChairID = SwitchViewChairID(wMeChairID);
			m_GameClientView.SetMeChairID(wMeViewChairID);

			//����ʱ��
			SetGameClock(GetMeChairID(), IDI_HORSES_START, pSceneHorses->nTimeLeave);

			//���ó���
			m_GameClientView.SetStreak(pSceneHorses->nStreak);

			//���ñ���
			memcpy(m_nMultiple, pSceneHorses->nMultiple, sizeof(m_nMultiple));
			m_GameClientView.m_DlgPlayBet.SetMultiple(m_nMultiple);

			//��������
			for (int i = 0; i < HORSES_ALL; ++i)
				m_GameClientView.SetHorsesName(i, pSceneHorses->szHorsesName[i]);

			//��ʷ��¼
			m_GameClientView.m_DlgRecord.m_GameRecords.RemoveAll();
			for ( int i = 0; i < MAX_SCORE_HISTORY; ++i )
			{
				if ( pSceneHorses->GameRecords[i].nStreak != 0)
				{
					m_GameClientView.m_DlgRecord.m_GameRecords.Add(pSceneHorses->GameRecords[i]);
				}
			}

			//ȫ��Ӯ�ĳ���
			m_GameClientView.m_DlgStatistics.SetWinCount(pSceneHorses->nWinCount);

			//������ע����
			m_GameClientView.SetBetPlayerCount(pSceneHorses->nBetPlayerCount);

			//������ע
			memcpy(m_lPlayerBet, pSceneHorses->lPlayerBet, sizeof(m_lPlayerBet));
			for ( int i = 0; i < AREA_ALL; ++i)
				m_GameClientView.SetPlayerBet(wMeViewChairID, i, m_lPlayerBet[i]);

			//������ע
			memcpy(m_lPlayerBetAll, pSceneHorses->lPlayerBetAll, sizeof(m_lPlayerBetAll));
			for ( int i = 0; i < AREA_ALL; ++i)
				m_GameClientView.SetAllBet(i, m_lPlayerBetAll[i]);

			//���������Ӯ
			m_GameClientView.SetPlayerWinning(pSceneHorses->lPlayerWinning);

			//���Ʊ���
			m_lAreaLimitScore = pSceneHorses->lAreaLimitScore;						//����������
			m_lUserLimitScore = pSceneHorses->lUserLimitScore;						//������������

			//����
			if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.GetSafeHwnd())
				m_GameClientView.m_btOpenControl.ShowWindow(SW_SHOW);

			//���¿���
			UpdateControls();
			return true;
		}
	}
	return false;
}


//���¿ؼ�
void CGameClientEngine::UpdateControls()
{

}

//��ע��Ϣ
LRESULT CGameClientEngine::OnPlayerBet( WPARAM wParam, LPARAM lParam )
{
	//�������
	CMD_C_PlayerBet* pPlayerBet = (CMD_C_PlayerBet*)lParam;

	//��֤��ע
	for ( int i = 0 ; i < AREA_ALL ; ++i)
	{
		if ( pPlayerBet->lBetScore[i] > GetMeMaxBet(i) )
		{
			//��עʧ����Ϣ
			CDialogMessage Message;
			Message.SetMessage(TEXT("��ע����ϵͳ���ƽ���������ע��"));
			Message.DoModal();
			return 0;
		}
	}
	
	//������Ϣ
	SendSocketData( SUB_C_PLAYER_BET, pPlayerBet, sizeof(CMD_C_PlayerBet));

	//������ע
	WORD wMeChairId = GetMeChairID();
	WORD wViewChairID = SwitchViewChairID(wMeChairId);
	for( int i = 0 ; i < AREA_ALL ; ++i )
	{
		m_GameClientView.SetPlayerBet(wViewChairID, i, pPlayerBet->lBetScore[i]);
		m_lPlayerBet[i] += pPlayerBet->lBetScore[i];
		m_lPlayerBetAll[i] += pPlayerBet->lBetScore[i];
	}
	return 0;
}
//����
LRESULT CGameClientEngine::OnAdminControl( WPARAM wParam, LPARAM lParam )
{
	if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.GetSafeHwnd())
		SendSocketData(SUB_C_ADMIN_COMMDN,(CMD_C_ControlApplication*)wParam,sizeof(CMD_C_ControlApplication));

	return true;
}


//��ע��ʼ
bool CGameClientEngine::OnSubBetStart( const void * pBuffer, WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_BetStart));
	if (wDataSize!=sizeof(CMD_S_BetStart)) return false;

	if (m_GameClientView.m_DlgControl.GetSafeHwnd())
	{
		m_GameClientView.m_DlgControl.ResetUserBet();
	}

	//��Ϣ����
	CMD_S_BetStart * pBetStart = (CMD_S_BetStart *)pBuffer;

	//����ʱ��
	SetGameClock(GetMeChairID(), IDI_BET_START, pBetStart->nTimeLeave);
	m_GameClientView.SetBetEndTime(pBetStart->nTimeBetEnd);

	//����״̬
	SetGameStatus(GS_BET);
	m_GameClientView.SetGameStatus(GS_BET);
	m_GameClientView.m_DlgPlayBet.SetCanBet(true);

	//����λ��
	m_GameClientView.SetMeChairID(SwitchViewChairID(GetMeChairID()));
	return true;
}

//��ע����
bool CGameClientEngine::OnSubBetEnd( const void * pBuffer, WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_BetEnd));
	if (wDataSize!=sizeof(CMD_S_BetEnd)) return false;

	//��Ϣ����
	CMD_S_BetEnd * pBetEnd = (CMD_S_BetEnd *)pBuffer;

	//����ʱ��
	SetGameClock(GetMeChairID(), IDI_BET_END, pBetEnd->nTimeLeave);

	//����״̬
	SetGameStatus(GS_BET_END);
	m_GameClientView.SetGameStatus(GS_BET_END);
	m_GameClientView.m_DlgPlayBet.SetCanBet(false);
	return true;
}

//����ʼ
bool CGameClientEngine::OnSubHorsesStart( const void * pBuffer, WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_HorsesStart));
	if (wDataSize!=sizeof(CMD_S_HorsesStart)) return false;

	//��Ϣ����
	CMD_S_HorsesStart * pHorsesStart = (CMD_S_HorsesStart *)pBuffer;

	//����ʱ��
	SetGameClock(GetMeChairID(), IDI_HORSES_START, pHorsesStart->nTimeLeave);

	//����״̬
	SetGameStatus(GS_HORSES);
	m_GameClientView.SetGameStatus(GS_HORSES);
	m_GameClientView.m_DlgPlayBet.SetCanBet(false);

	//��������
	m_GameClientView.SetRanking(pHorsesStart->cbHorsesRanking);

	//������Ӯ
	m_GameClientView.SetPlayerWinning(pHorsesStart->lPlayerWinning + pHorsesStart->lPlayerReturnBet);

	//��ʼ����
	m_GameClientView.HorsesStart(pHorsesStart->nHorsesSpeed);
	return true;
}

//�������
bool CGameClientEngine::OnSubHorsesEnd( const void * pBuffer, WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_HorsesEnd));
	if (wDataSize!=sizeof(CMD_S_HorsesEnd)) return false;

	//��Ϣ����
	CMD_S_HorsesEnd * pHorsesEnd = (CMD_S_HorsesEnd *)pBuffer;

	//����ʱ��
	SetGameClock(GetMeChairID(), IDI_FREE, pHorsesEnd->nTimeLeave);

	//�����¼
	m_GameClientView.m_DlgRecord.m_GameRecords.Add(pHorsesEnd->RecordRecord);
	if ( m_GameClientView.m_DlgRecord.m_GameRecords.GetCount() > MAX_SCORE_HISTORY )
	{
		m_GameClientView.m_DlgRecord.m_GameRecords.RemoveAt(0);
	}
	if ( m_GameClientView.m_DlgRecord.GetSafeHwnd() && m_GameClientView.m_DlgRecord.IsWindowVisible())
	{
		m_GameClientView.m_DlgRecord.Invalidate(FALSE);
	}
	

	//������ע��¼
	for ( int i = 0 ; i < AREA_ALL; ++i )
	{
		if ( pHorsesEnd->lPlayerBet[i] > 0 )
		{
			BetRecordInfo Info;
			Info.nStreak = pHorsesEnd->RecordRecord.nStreak;
			Info.nHours = pHorsesEnd->RecordRecord.nHours;
			Info.nMinutes = pHorsesEnd->RecordRecord.nMinutes;
			Info.nRanking = i;
			Info.lBet = pHorsesEnd->lPlayerBet[i];
			Info.lWin = pHorsesEnd->lPlayerWinning[i];
			m_GameClientView.m_DlgBetRecord.AddInfo(&Info);
		}
	}

	//���ó���
	m_GameClientView.SetStreak(pHorsesEnd->RecordRecord.nStreak);

	//ȫ��Ӯ�ĳ���
	m_GameClientView.m_DlgStatistics.SetWinCount(pHorsesEnd->nWinCount);

	//���汶��
	memcpy(m_nMultiple, pHorsesEnd->nMultiple, sizeof(m_nMultiple));
	m_GameClientView.m_DlgPlayBet.SetMultiple(m_nMultiple);

	//����״̬
	SetGameStatus(GS_FREE);
	m_GameClientView.SetGameStatus(GS_FREE);
	m_GameClientView.m_DlgPlayBet.SetCanBet(false);

	//������ƥ
	m_GameClientView.NweHorses();

	//��Ϸ����
	memset(m_lPlayerBet, 0, sizeof(m_lPlayerBet));
	memset(m_lPlayerBetAll, 0, sizeof(m_lPlayerBetAll));

	return true;
}

//�����ע
bool CGameClientEngine::OnSubPlayerBet(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlayerBet));
	if (wDataSize!=sizeof(CMD_S_PlayerBet)) return false;

	//��Ϣ����
	CMD_S_PlayerBet * pPlayerBet = (CMD_S_PlayerBet *)pBuffer;

	if (pPlayerBet->bIsAndroid==false )
	{
		m_GameClientView.m_DlgControl.SetUserBetScore(pPlayerBet->lBetScore);
	}
	//������ע����
	m_GameClientView.SetBetPlayerCount(pPlayerBet->nBetPlayerCount);

	//���ע��
	if ( pPlayerBet->wChairID != GetMeChairID() )
	{
		WORD wViewChairID = SwitchViewChairID(pPlayerBet->wChairID);

		for( int i = 0 ; i < AREA_ALL ; ++i )
		{
			m_GameClientView.SetPlayerBet(wViewChairID, i, pPlayerBet->lBetScore[i]);
			m_lPlayerBetAll[i] += pPlayerBet->lBetScore[i];
		}
	}
	return true;
}

//��עʧ��
bool CGameClientEngine::OnSubPlayerBetFail( const void * pBuffer, WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlayerBetFail));
	if (wDataSize!=sizeof(CMD_S_PlayerBetFail)) return false;

	//��Ϣ����
	CMD_S_PlayerBetFail * pPlayerBetFail = (CMD_S_PlayerBetFail *)pBuffer;

	//�Լ���עɾ��
	if (pPlayerBetFail->wChairID == GetMeChairID())
	{
		for( int i = 0 ; i < AREA_ALL ; ++i )
		{
			m_lPlayerBet[i] -= pPlayerBetFail->lBetScore[i];
			m_lPlayerBetAll[i] -= pPlayerBetFail->lBetScore[i];
		}
	}

	//��עʧ��
	WORD wViewChairID = SwitchViewChairID(pPlayerBetFail->wChairID);
	for( int i = 0 ; i < AREA_ALL ; ++i )
	{
		m_GameClientView.SetPlayerBet(wViewChairID, i, -pPlayerBetFail->lBetScore[i]);
	}

	//��ʾʧ����Ϣ
	if ( pPlayerBetFail->wChairID == GetMeChairID() )
	{
		CDialogMessage Message;

		if ( pPlayerBetFail->cbFailType == FAIL_TYPE_TIME_OVER )
		{
			Message.SetMessage(TEXT("������עʱ�������ע��"));	
		}
		else
		{
			Message.SetMessage(TEXT("��ע����ϵͳ���ƽ���������ע��"));
		}
		Message.DoModal();
	}
	return true;
}

//ǿ�ƽ���
bool CGameClientEngine::OnSubMandatoryEnd( const void * pBuffer, WORD wDataSize )
{
	if ( wDataSize != 0 )
		return false;

	//�������
	m_GameClientView.HorsesEnd();
	return true;
}

//����
bool CGameClientEngine::OnSubAdminControl( const void * pBuffer, WORD wDataSize )
{
	ASSERT(wDataSize==sizeof(CMD_S_ControlReturns));
	if(wDataSize!=sizeof(CMD_S_ControlReturns)) return false;

	if( NULL != m_GameClientView.m_DlgControl.m_hWnd )
	{
		CMD_S_ControlReturns* pResult = (CMD_S_ControlReturns*)pBuffer;
		m_GameClientView.m_DlgControl.UpdateControl(pResult);
	}
	return true;
}

//�Լ�����·�
LONGLONG CGameClientEngine::GetMeMaxBet( BYTE cbArea /*= AREA_ALL */ )
{
	//�ҳ����
	IClientUserItem * pUserData = GetTableUserItem(GetMeChairID());
	if ( pUserData == NULL || cbArea > AREA_ALL ) 
		return 0l;

	//��ҷ���
	LONGLONG lUserScore = pUserData->GetUserScore();

	//��ȥ�Լ�����ע
	for(int i = 0; i < AREA_ALL; ++i)
	{
		lUserScore -= m_lPlayerBet[i];
	}

	//�����������ע. ��Ҫ�ж���������
	if ( cbArea < AREA_ALL )
	{
		//������������
		LONGLONG lUserLimit = m_lUserLimitScore - m_lPlayerBet[cbArea];
		lUserScore = __min( lUserScore, lUserLimit);

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