#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////////////

//����ʱ��
#define IDI_FREE					1									//����ʱ��
#define TIME_FREE					1									//����ʱ��

//��עʱ��
#define IDI_BET						2									//��עʱ��
#ifdef _DEBUG
	#define TIME_BET				20									//��עʱ��
#else
	#define TIME_BET				30									//��עʱ��
#endif 


//����ʱ��
#define IDI_END						3									//����ʱ��
#define TIME_END					17									//����ʱ��


LONGLONG		CTableFrameSink::m_lStorageStart = 0L;
LONGLONG		CTableFrameSink::m_lStorageDeduct = 1L;

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��ע��
	m_lCellScore = 0;
	ZeroMemory(m_lAllBet, sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(m_HPlayBet,sizeof(m_HPlayBet));
	ZeroMemory(m_FPlayBet,sizeof(m_FPlayBet));
	ZeroMemory(m_APlayBet,sizeof(m_APlayBet));
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

	//��ҷ���
	ZeroMemory(m_dwDrawStartTime, sizeof(m_dwDrawStartTime));
	ZeroMemory(m_lPlayScore, sizeof(m_lPlayScore));
	ZeroMemory(m_lPlayOriginalScore, sizeof(m_lPlayOriginalScore));
	ZeroMemory(m_lPlayChip, sizeof(m_lPlayChip));

	//����
	ZeroMemory(m_lPlayWinAnimal, sizeof(m_lPlayWinAnimal));
	ZeroMemory(m_lPlayWinChip, sizeof(m_lPlayWinChip));
	ZeroMemory(m_lPlayWinScore, sizeof(m_lPlayWinScore));
	ZeroMemory(m_lPlayWinPrizes, sizeof(m_lPlayWinPrizes));

	//ת��·��
	m_bTurnTwoTime = FALSE;
	ZeroMemory(m_nTurnTableTarget, sizeof(m_nTurnTableTarget));

	//ת�̱���
	m_nLastMultipleIndex = 0;
	ZeroMemory(m_nAnimalMultiple, sizeof(m_nAnimalMultiple));
	
	//���ɼ���
	ZeroMemory(m_nGenerationChance, sizeof(m_nGenerationChance));
	m_nGenerationChance[ANIMAL_LION] = 20;
	m_nGenerationChance[ANIMAL_PANDA] = 20;
	m_nGenerationChance[ANIMAL_MONKEY] = 40;
	m_nGenerationChance[ANIMAL_RABBIT] = 40;
	m_nGenerationChance[ANIMAL_EAGLE] = 20;
	m_nGenerationChance[ANIMAL_PEACOCK] = 20;
	m_nGenerationChance[ANIMAL_PIGEON] = 40;
	m_nGenerationChance[ANIMAL_SWALLOW] = 40;
	m_nGenerationChance[ANIMAL_SLIVER_SHARK] = 5;
	m_nGenerationChance[ANIMAL_GOLD_SHARK] = 5;

	//״̬����
	m_dwBetTime=0L;

	//ʱ������
	m_cbFreeTime = TIME_FREE;
	m_cbBetTime = TIME_BET;
	m_cbEndTime = TIME_END;

	//���ñ���
	m_lAreaLimitScore = 0;
	m_lPlayLimitScore = 0;

	//�������
	m_hInst = NULL;
	m_pServerContro = NULL;
	m_hInst = LoadLibrary(TEXT("SharkBattleServerControl.dll"));
	if ( m_hInst )
	{
		typedef void * (*CREATE)(); 
		CREATE ServerControl = (CREATE)GetProcAddress(m_hInst,"CreateServerControl"); 
		if ( ServerControl )
		{
			m_pServerContro = static_cast<IServerControl*>(ServerControl());
		}
	}

	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
{
}
//�ͷŶ���
VOID  CTableFrameSink::Release()
{
	if( m_pServerContro )
	{
		delete m_pServerContro;
		m_pServerContro = NULL;
	}

	if( m_hInst )
	{
		FreeLibrary(m_hInst);
		m_hInst = NULL;
	}

	delete this; 

}
//�ӿڲ�ѯ
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��������
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);

	//�����ж�
	if (m_pITableFrame==NULL)
	{
		CTraceService::TraceString(TEXT("��Ϸ���� CTableFrameSink ��ѯ ITableFrame �ӿ�ʧ��"),TraceLevel_Exception);
		return false;
	}

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);

	//�����ļ���
	TCHAR szRoomName[32];
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(szConfigFileName,CountArray(szConfigFileName),TEXT("%s\\SharkBattleConfig.ini"),szPath);
	memcpy(szRoomName, m_pGameServiceOption->szServerName, sizeof(m_pGameServiceOption->szServerName));

	//��������
	TCHAR OutBuf[255] = {};

	//��λ����
	m_lCellScore = GetPrivateProfileInt(szRoomName, TEXT("CellScore"), 1, szConfigFileName);
	if ( m_lCellScore < 1 )
		m_lCellScore = 1;
	
	//��������
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("AreaLimitScore"), _T("0"), OutBuf, 255, szConfigFileName);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%I64d"), &m_lAreaLimitScore);

	//�����������
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("PlayLimitScore"), _T("0"), OutBuf, 255, szConfigFileName);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%I64d"), &m_lPlayLimitScore);

	//���
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("StorageStart"), _T("0"), OutBuf, 255, szConfigFileName);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%I64d"), &m_lStorageStart);

	//˥��ֵ
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("StorageDeduct"), _T("1"), OutBuf, 255, szConfigFileName);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%I64d"), &m_lStorageDeduct);

	//��ע��������Ŀ
	m_nMaxChipRobot = GetPrivateProfileInt(m_szGameRoomName, TEXT("RobotBetCount"), 10, szConfigFileName);
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 10;

	//������
	m_nRobotListMaxCount = GetPrivateProfileInt(m_szGameRoomName, _T("RobotListMaxCount"), 5, szConfigFileName);	

	//��������ע����
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szGameRoomName, TEXT("RobotAreaLimit"), TEXT("50000000"), OutBuf, 255*sizeof(TCHAR), szConfigFileName);
	_snwscanf(OutBuf, wcslen(OutBuf), TEXT("%I64d"), &m_lRobotAreaLimit);


	//ʱ�����
	m_cbFreeTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("FreeTime"), TIME_FREE, szConfigFileName);

	m_cbBetTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("BetTime"), TIME_BET, szConfigFileName);

	m_cbEndTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("EndTime"), TIME_END, szConfigFileName);

	if (m_cbFreeTime <= 0 || m_cbFreeTime > 99)			
		m_cbFreeTime = TIME_FREE;
	if (m_cbBetTime <= 0 || m_cbBetTime > 99)		
		m_cbBetTime = TIME_BET;
	if (m_cbEndTime <= 0 || m_cbEndTime > 99)			
		m_cbEndTime = TIME_END;


	if( m_lStorageDeduct < 1 )
	{
		m_lStorageDeduct = 1;
	}
	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	//��ע��
	ZeroMemory(m_lAllBet, sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(m_HPlayBet,sizeof(m_HPlayBet));
	ZeroMemory(m_FPlayBet,sizeof(m_FPlayBet));
	ZeroMemory(m_APlayBet,sizeof(m_APlayBet));
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

	//����
	ZeroMemory(m_lPlayWinAnimal, sizeof(m_lPlayWinAnimal));
	ZeroMemory(m_lPlayWinChip, sizeof(m_lPlayWinChip));
	ZeroMemory(m_lPlayWinScore, sizeof(m_lPlayWinScore));
	ZeroMemory(m_lPlayWinPrizes, sizeof(m_lPlayWinPrizes));

	//ת��·��
	m_bTurnTwoTime = FALSE;
	ZeroMemory(m_nTurnTableTarget, sizeof(m_nTurnTableTarget));

	//ת�̱���
	m_nLastMultipleIndex = 0;
	ZeroMemory(m_nAnimalMultiple, sizeof(m_nAnimalMultiple));

	return;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NETWORK_ERROR:		//�����ж�
	case GER_USER_LEAVE:		//�û��뿪
		{
			//�ǽ���״̬�� ������ע���
			if (m_pITableFrame->GetGameStatus() != GAME_SCENE_END)
			{
				//������Ϣ
				CMD_S_BetClear CMDBetClear;
				CMDBetClear.wChairID = wChairID;
				CopyMemory(CMDBetClear.lPlayBet, m_lPlayBet[wChairID], sizeof(CMDBetClear.lPlayBet));

				//�����ע
				BOOL bSendMessage = FALSE;
				for ( int i = 0; i < ANIMAL_MAX; ++i )
				{
					if ( m_lPlayBet[wChairID][i] > 0 )
					{
						bSendMessage = TRUE;
						m_lAllBet[i] -= m_lPlayBet[wChairID][i];
						m_lPlayBet[wChairID][i] = 0;
					}
				}

				//������Ϣ
				if ( bSendMessage )
				{
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BET_CLEAR, &CMDBetClear, sizeof(CMDBetClear));
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BET_CLEAR, &CMDBetClear, sizeof(CMDBetClear));
				}
			}

			//д��
			LONGLONG lScore = (m_lPlayScore[wChairID] - m_lPlayOriginalScore[wChairID]) + (m_lPlayChip[wChairID] * m_lCellScore);
			if ( lScore != 0 )
			{
				//��ֹ����
				if ( lScore < 0 && (-lScore) > pIServerUserItem->GetUserScore() )
				{
					lScore = -pIServerUserItem->GetUserScore();
				}

				//����˰��
				LONGLONG lRevenue = 0;
				if ( lScore > 0 )
				{
					lRevenue = m_pITableFrame->CalculateRevenue(wChairID, lScore);
					lScore -= lRevenue;
				}

				//д��
				tagScoreInfo ScoreInfo;
				ZeroMemory(&ScoreInfo, sizeof(ScoreInfo));
				ScoreInfo.lScore = lScore;
				ScoreInfo.lRevenue = lRevenue;
				if ( lScore > 0 )
					ScoreInfo.cbType = SCORE_TYPE_WIN;
				else if ( lScore < 0 )
					ScoreInfo.cbType = SCORE_TYPE_LOSE;
				else
					ScoreInfo.cbType = SCORE_TYPE_DRAW;

				m_pITableFrame->WriteUserScore(wChairID, ScoreInfo);
				//m_pITableFrame->WriteUserScore(wChairID, ScoreInfo, INVALID_DWORD,  (DWORD)time(NULL) - m_dwDrawStartTime[wChairID]);//ԭ����
				//pIServerUserItem->WriteUserScore(ScoreInfo.lScore, 0, ScoreInfo.lRevenue, 0, ScoreInfo.cbType, );
			}

			return true;
		}
	}
	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch( cbGameStatus )
	{
	case GAME_SCENE_FREE:
		{
			//��������
			CMD_S_StatusFree CMDStatusFree;
			ZeroMemory(&CMDStatusFree,sizeof(CMDStatusFree));

			//ʱ����Ϣ
			DWORD dwPassTime = (DWORD)time(NULL)-m_dwBetTime;
			CMDStatusFree.cbTimeLeave = (BYTE)(m_cbFreeTime - __min(dwPassTime, m_cbFreeTime));

			//������Ϣ
			CMDStatusFree.lCellScore = m_lCellScore;
			CMDStatusFree.lPlayScore = pIServerUserItem->GetUserScore();
			CMDStatusFree.lStorageStart = m_lStorageStart;

			CMDStatusFree.lAreaLimitScore = m_lAreaLimitScore;
			CMDStatusFree.lPlayLimitScore = m_lPlayLimitScore;

			//��Ϸ��¼
			for ( int i = 0; i < RECORD_COUNT_MAX; ++i )
			{
				CMDStatusFree.nTurnTableRecord[i] = INT_MAX;
				if( i < m_ArrayTurnTableRecord.GetCount() )
					CMDStatusFree.nTurnTableRecord[i] = m_ArrayTurnTableRecord[i];
			}

			//���ͳ���
			bool bReturn = m_pITableFrame->SendGameScene(pIServerUserItem,&CMDStatusFree,sizeof(CMDStatusFree));
			return bReturn;
		}
		break;
	case GAME_SCENE_BET:
		{
			//��������
			CMD_S_StatusPlay CMDStatusPlay;
			ZeroMemory(&CMDStatusPlay,sizeof(CMDStatusPlay));

			//ʱ����Ϣ
			DWORD dwPassTime = (DWORD)time(NULL)-m_dwBetTime;
			CMDStatusPlay.cbTimeLeave = (BYTE)(m_cbBetTime - __min(dwPassTime, m_cbBetTime));

			//������Ϣ
			CMDStatusPlay.lCellScore = m_lCellScore;
			CMDStatusPlay.lPlayScore = m_lPlayScore[wChairID];
			CMDStatusPlay.lPlayChip = m_lPlayChip[wChairID];
			CMDStatusPlay.lStorageStart = m_lStorageStart;

			CMDStatusPlay.lAreaLimitScore = m_lAreaLimitScore;
			CMDStatusPlay.lPlayLimitScore = m_lPlayLimitScore;

			CopyMemory( CMDStatusPlay.nAnimalMultiple, m_nAnimalMultiple, sizeof(CMDStatusPlay.nAnimalMultiple));
			CopyMemory( CMDStatusPlay.lAllBet, m_lAllBet, sizeof(CMDStatusPlay.lAllBet));
			CopyMemory( CMDStatusPlay.lPlayBet, m_lPlayBet[wChairID], sizeof(CMDStatusPlay.lPlayBet));

			//��Ϸ��¼
			for ( int i = 0; i < RECORD_COUNT_MAX; ++i )
			{
				CMDStatusPlay.nTurnTableRecord[i] = INT_MAX;
				if( i < m_ArrayTurnTableRecord.GetCount() )
					CMDStatusPlay.nTurnTableRecord[i] = m_ArrayTurnTableRecord[i];
			}

			//���ͳ���
			bool bReturn = m_pITableFrame->SendGameScene(pIServerUserItem,&CMDStatusPlay,sizeof(CMDStatusPlay));
			return bReturn;
		}
		break;
	case GAME_SCENE_END:
		{
			//��������
			CMD_S_StatusEnd CMDStatusEnd;
			ZeroMemory(&CMDStatusEnd,sizeof(CMDStatusEnd));

			//ʱ����Ϣ
			DWORD dwPassTime = (DWORD)time(NULL)-m_dwBetTime;
			CMDStatusEnd.cbTimeLeave = (BYTE)(m_cbEndTime - __min(dwPassTime, m_cbEndTime));

			//������Ϣ
			CMDStatusEnd.lCellScore = m_lCellScore;
			CMDStatusEnd.lPlayScore = m_lPlayScore[wChairID];
			CMDStatusEnd.lPlayChip = m_lPlayChip[wChairID];
			CMDStatusEnd.lStorageStart = m_lStorageStart;

			CMDStatusEnd.lAreaLimitScore = m_lAreaLimitScore;
			CMDStatusEnd.lPlayLimitScore = m_lPlayLimitScore;

			CopyMemory( CMDStatusEnd.nAnimalMultiple, m_nAnimalMultiple, sizeof(CMDStatusEnd.nAnimalMultiple));
			CopyMemory( CMDStatusEnd.lAllBet, m_lAllBet, sizeof(CMDStatusEnd.lAllBet));
			CopyMemory( CMDStatusEnd.lPlayBet, m_lPlayBet[wChairID], sizeof(CMDStatusEnd.lPlayBet));

			//��Ϸ��¼
			for ( int i = 0; i < RECORD_COUNT_MAX; ++i )
			{
				CMDStatusEnd.nTurnTableRecord[i] = INT_MAX;
				if( i < m_ArrayTurnTableRecord.GetCount() )
					CMDStatusEnd.nTurnTableRecord[i] = m_ArrayTurnTableRecord[i];
			}

			//���ͳ���
			bool bReturn = m_pITableFrame->SendGameScene(pIServerUserItem,&CMDStatusEnd,sizeof(CMDStatusEnd));
			return bReturn;
		}
		break;
	}
	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_BET:		//��עʱ��
		{
			//״̬�ж�(��ֹǿ���ظ�����)
			if ( m_pITableFrame->GetGameStatus() != GAME_SCENE_END )
			{
				//����״̬
				m_pITableFrame->SetGameStatus(GAME_SCENE_END);

				//������Ϸ
				int nPrizesMultiple = GameOver();

				//���ͽ�������Ϣ
				CMD_S_GameEnd CMDGameEnd;
				ZeroMemory(&CMDGameEnd, sizeof(CMDGameEnd));
				CMDGameEnd.cbTimeLeave = m_cbEndTime;
				CMDGameEnd.bTurnTwoTime = m_bTurnTwoTime;
				CMDGameEnd.nPrizesMultiple = nPrizesMultiple;
				CMDGameEnd.lPlayShowPrizes = (rand()%70000)/1000*1000;
				CopyMemory(CMDGameEnd.nTurnTableTarget, m_nTurnTableTarget, sizeof(CMDGameEnd.nTurnTableTarget));
				for ( int i = 0; i < GAME_PLAYER; ++i )
				{
					IServerUserItem* pServerUserItem = m_pITableFrame->GetTableUserItem(i);
					if ( pServerUserItem == NULL || !pServerUserItem->IsClientReady() )
						continue;

					CMDGameEnd.lPlayWin[0] = m_lPlayWinScore[i][0];
					CMDGameEnd.lPlayWin[1] = m_lPlayWinScore[i][1];
					CMDGameEnd.lPlayPrizes = m_lPlayWinPrizes[i];
					m_pITableFrame->SendUserItemData(pServerUserItem, SUB_S_GAME_END, &CMDGameEnd, sizeof(CMDGameEnd));
				}
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END, &CMDGameEnd, sizeof(CMDGameEnd));

				//д��
				GameOverWriteScore();

				//����ʱ��
				m_dwBetTime = (DWORD)time(NULL);
				if ( CGameLogic::GetInstance()->TurnTableAnimal(m_nTurnTableTarget[0]) == ANIMAL_GOLD_SHARK )
				{
					m_pITableFrame->SetGameTimer(IDI_END, (m_cbEndTime + 30)*1000, 1, NULL);	
				}
				else if( CGameLogic::GetInstance()->TurnTableAnimal(m_nTurnTableTarget[0]) == ANIMAL_SLIVER_SHARK )
				{
					m_pITableFrame->SetGameTimer(IDI_END, (m_cbEndTime + 17)*1000, 1, NULL);	
				}
				else
				{
					m_pITableFrame->SetGameTimer(IDI_END, m_cbEndTime*1000, 1, NULL);	
				}	
			}
			return true;
		}
	case IDI_END:				//������Ϸ
		{
			//��ʼ��Ϸ
			//-------------------------------------------------------

			//����ʱ��
			m_dwBetTime = (DWORD)time(NULL);

			//��ʾ���
			static int	nCurrentHour = 0;							//��ʾ���
			CTime time = CTime::GetCurrentTime();					//��ȡ��ǰʱ��. 
			if ( time.GetHour() != nCurrentHour )
			{
				TCHAR szServiceInfo[128] = {0};
				_sntprintf(szServiceInfo,sizeof(szServiceInfo), TEXT("��ǰ�������%I64d"), m_lStorageStart);
				CTraceService::TraceString(szServiceInfo,TraceLevel_Info);
				nCurrentHour = time.GetHour();
			}

			//��Ϣ����
			CMD_S_GameStart CMDGameStart;
			ZeroMemory(&CMDGameStart,sizeof(CMDGameStart));

			//���ñ���
			CMDGameStart.cbTimeLeave = m_cbBetTime;
			CMDGameStart.lStorageStart = m_lStorageStart;

			//��������
			int nAnimalMultiple[5][ANIMAL_MAX] = 
			{
				{ 12,  8,  8,  6, 12,  8,  8,  6, 24, 24,  2,  2},
				{ 24, 24, 12,  3, 24, 24, 12,  3, 24, 24,  2,  2},
				{ 24,  8,  6,  6, 24,  8,  6,  6, 24, 24,  2,  2},
				{ 24, 12,  8,  4, 24, 12,  8,  4, 24, 24,  2,  2},
				{ 12, 12,  6,  6, 12, 12,  6,  6, 24, 24,  2,  2},
			};

			int nLastMultipleIndex = m_nLastMultipleIndex;
			do 
			{
				m_nLastMultipleIndex = rand()%5;
			} while ( nLastMultipleIndex == m_nLastMultipleIndex );
			CopyMemory(m_nAnimalMultiple, nAnimalMultiple[m_nLastMultipleIndex], sizeof(m_nAnimalMultiple));
			CopyMemory(CMDGameStart.nAnimalMultiple, m_nAnimalMultiple, sizeof(CMDGameStart.nAnimalMultiple));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_START, &CMDGameStart, sizeof(CMDGameStart));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START, &CMDGameStart, sizeof(CMDGameStart));

			//������ʱ��
			m_pITableFrame->SetGameTimer(IDI_BET, m_cbBetTime*1000, 1, NULL);

			//����״̬
			m_pITableFrame->SetGameStatus(GAME_SCENE_BET);

			return true;
		}
	}

	return false;
}

//�����¼�
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return true;
}

//�����¼�
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return false;
}

//��Ϸ��Ϣ
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch(wSubCmdID)
	{
	case SUB_C_EXCHANGE_CHIP:		//�һ�����
		return OnSubChip( pData, wDataSize, pIServerUserItem );
	case SUB_C_PLAY_BET:			//�û���ע
		return OnSubPlayBet( pData, wDataSize, pIServerUserItem );
	case SUB_C_BET_CLEAR:			//�����ע
		return OnSubBetClear( pData, wDataSize, pIServerUserItem );
	case SUB_C_AMDIN_COMMAND:
		{
			
			ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
			if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;

			//Ȩ���ж�
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
				return false;

			
			if ( m_pServerContro == NULL)
				return true;

			
			return m_pServerContro->ServerControl(wSubCmdID, pData, wDataSize, pIServerUserItem, m_pITableFrame);
		}
	}
	return false;
}

//�����Ϣ
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if ( bLookonUser )
		return true;

	if ( m_dwBetTime == 0 )
	{
		//��ʼ��Ϸ
		m_pITableFrame->StartGame();
	}

	//ǿ�����ó���Ϸ״̬
	pIServerUserItem->SetUserStatus(US_PLAYING, pIServerUserItem->GetTableID(), wChairID);

	if ( m_dwBetTime == 0 )
	{
		//����ʱ��
		m_dwBetTime=(DWORD)time(NULL);

		//��ʾ���
		static int	nCurrentHour = 0;							//��ʾ���
		CTime time = CTime::GetCurrentTime();					//��ȡ��ǰʱ��. 
		if ( time.GetHour() != nCurrentHour )
		{
			TCHAR szServiceInfo[128] = {0};
			_sntprintf(szServiceInfo,sizeof(szServiceInfo), TEXT("��ǰ�������%I64d"), m_lStorageStart);
			CTraceService::TraceString(szServiceInfo,TraceLevel_Info);
			nCurrentHour = time.GetHour();
		}

		//��Ϣ����
		CMD_S_GameStart CMDGameStart;
		ZeroMemory(&CMDGameStart,sizeof(CMDGameStart));

		//���ñ���
		CMDGameStart.cbTimeLeave = m_cbBetTime;
		CMDGameStart.lStorageStart = m_lStorageStart;

		//��������
		int nAnimalMultiple[5][ANIMAL_MAX] = 
		{
			{ 12,  8,  8,  6, 12,  8,  8,  6, 24, 24,  2,  2},
			{ 24, 24, 12,  3, 24, 24, 12,  3, 24, 24,  2,  2},
			{ 24,  8,  6,  6, 24,  8,  6,  6, 24, 24,  2,  2},
			{ 24, 12,  8,  4, 24, 12,  8,  4, 24, 24,  2,  2},
			{ 12, 12,  6,  6, 12, 12,  6,  6, 24, 24,  2,  2},
		};

		int nLastMultipleIndex = m_nLastMultipleIndex;
		do 
		{
			m_nLastMultipleIndex = rand()%5;
		} while ( nLastMultipleIndex == m_nLastMultipleIndex );
		CopyMemory(m_nAnimalMultiple, nAnimalMultiple[m_nLastMultipleIndex], sizeof(m_nAnimalMultiple));
		CopyMemory(CMDGameStart.nAnimalMultiple, m_nAnimalMultiple, sizeof(CMDGameStart.nAnimalMultiple));

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_START, &CMDGameStart, sizeof(CMDGameStart));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START, &CMDGameStart, sizeof(CMDGameStart));

		//����ʱ��
		m_pITableFrame->SetGameTimer(IDI_BET, m_cbBetTime*1000, 1, NULL);

		//����״̬
		m_pITableFrame->SetGameStatus(GAME_SCENE_BET);
	}

	//���ñ���
	m_dwDrawStartTime[wChairID] = (DWORD)time(NULL);
	m_lPlayOriginalScore[wChairID] = pIServerUserItem->GetUserScore();
	m_lPlayScore[wChairID] = pIServerUserItem->GetUserScore();
	m_lPlayChip[wChairID] = 0;
	m_lPlayWinChip[wChairID] = 0;
	m_lPlayWinPrizes[wChairID] = 0;
	ZeroMemory(m_lPlayWinScore[wChairID], sizeof(LONGLONG)*2);
	ZeroMemory(m_lPlayWinAnimal[wChairID], sizeof(LONGLONG)*ANIMAL_MAX);

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if ( bLookonUser )
		return true;

	//���ñ���
	m_dwDrawStartTime[wChairID] = 0;
	m_lPlayOriginalScore[wChairID] = 0;
	m_lPlayScore[wChairID] = 0;
	m_lPlayChip[wChairID] = 0;
	m_lPlayWinChip[wChairID] = 0;
	m_lPlayWinPrizes[wChairID] = 0;
	ZeroMemory(m_lPlayWinScore[wChairID], sizeof(LONGLONG)*2);
	ZeroMemory(m_lPlayWinAnimal[wChairID], sizeof(LONGLONG)*ANIMAL_MAX);

	////����Ƿ�����
	//for ( WORD i = 0; i < GAME_PLAYER; ++i )
	//{
	//	IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem( i );
	//	if (pIServerUserItem == NULL) continue;

	//	return true;
	//}

	//������Ϸ
	//m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

	return true;
}

//����һ���Ϣ
bool CTableFrameSink::OnSubChip( VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem )
{
	//Ч������
	ASSERT( wDataSize == sizeof(CMD_C_Chip) );
	if ( wDataSize != sizeof(CMD_C_Chip) ) return false;

	//��Ϣ����
	CMD_C_Chip * pChip = (CMD_C_Chip *)pData;
	WORD wChairID = pIServerUserItem->GetChairID();

	//Ч����Ϣ
	if ( pChip->lChip * m_lCellScore > m_lPlayScore[wChairID] )
	{
		ASSERT(FALSE);
		return false;
	}

	//�޸ı���
	m_lPlayScore[wChairID] -= pChip->lChip * m_lCellScore;
	m_lPlayChip[wChairID] += pChip->lChip;

	return true;
}

//�û���ע��Ϣ
bool CTableFrameSink::OnSubPlayBet(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//Ч������
	ASSERT( wDataSize == sizeof(CMD_C_PlayBet) );
	if ( wDataSize != sizeof(CMD_C_PlayBet) ) return false;

	//��Ϣ����
	CMD_C_PlayBet * pPlayBet = (CMD_C_PlayBet *)pData;
	WORD wChairID = pIServerUserItem->GetChairID();

	//��ע�쳣
	if ( pPlayBet->nAnimalIndex < 0 || pPlayBet->nAnimalIndex >= ANIMAL_MAX || pPlayBet->lBetChip > m_lPlayChip[wChairID] || pPlayBet->lBetChip < 0 )
	{
		ASSERT(FALSE);
		return false;
	}

	//��עΪ0
	if( pPlayBet->lBetChip == 0 ) 
	{
		ASSERT(FALSE);
		return true;
	}

	//�жϳ���
	if ( (m_lAreaLimitScore != 0 && m_lAllBet[pPlayBet->nAnimalIndex] + pPlayBet->lBetChip > m_lAreaLimitScore)
		|| (m_lPlayLimitScore != 0 && m_lPlayBet[wChairID][pPlayBet->nAnimalIndex] + pPlayBet->lBetChip > m_lPlayLimitScore)
		|| ( m_pITableFrame->GetGameStatus() != GAME_SCENE_BET ) )
	{
		//��עʧ��
		CMD_S_PlayBetFail CMDPlayBetFail;
		CMDPlayBetFail.wChairID = wChairID;
		CMDPlayBetFail.nAnimalIndex = pPlayBet->nAnimalIndex;
		CMDPlayBetFail.lBetChip = pPlayBet->lBetChip;
		m_pITableFrame->SendUserItemData(pIServerUserItem, SUB_S_PLAY_BET_FAIL, &CMDPlayBetFail, sizeof(CMDPlayBetFail));
		return true;
	}

	//��ע�ɹ�
	m_lPlayChip[wChairID] -= pPlayBet->lBetChip;
///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
	//if (pIServerUserItem && pIServerUserItem->GetUserInfo()->cbMemberLevel>0)//��֮��ע��
		m_HPlayBet[pPlayBet->nAnimalIndex]+=pPlayBet->lBetChip;
	if (pIServerUserItem && pIServerUserItem->IsAndroidUser())
		m_APlayBet[pPlayBet->nAnimalIndex]+=pPlayBet->lBetChip;
	//if (pIServerUserItem && pIServerUserItem->GetUserInfo()->cbMemberLevel==0)//��֮��ע��
		m_FPlayBet[pPlayBet->nAnimalIndex]+=pPlayBet->lBetChip;
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
	m_lPlayBet[wChairID][pPlayBet->nAnimalIndex] += pPlayBet->lBetChip;
	m_lAllBet[pPlayBet->nAnimalIndex] += pPlayBet->lBetChip;

	//������Ϣ
	CMD_S_PlayBet CMDPlayBet;
	CMDPlayBet.wChairID = wChairID;
	CMDPlayBet.nAnimalIndex = pPlayBet->nAnimalIndex;
	CMDPlayBet.lBetChip = pPlayBet->lBetChip;
	CMDPlayBet.mHBet=0;
	//if (pIServerUserItem && pIServerUserItem->GetUserInfo()->cbMemberLevel>0)  //��֮��ע��
		CMDPlayBet.mHBet=pPlayBet->lBetChip;//��֮��ע��
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_PLAY_BET, &CMDPlayBet, sizeof(CMDPlayBet));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLAY_BET, &CMDPlayBet, sizeof(CMDPlayBet));

	return true;
}

//�����ע��Ϣ
bool CTableFrameSink::OnSubBetClear(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//Ч������
	ASSERT( wDataSize == NULL );
	if ( wDataSize != NULL ) return false;

	//��Ϣ����
	WORD wChairID = pIServerUserItem->GetChairID();

	//������Ϣ
	CMD_S_BetClear CMDBetClear;
	CMDBetClear.wChairID = wChairID;
	CopyMemory(CMDBetClear.lPlayBet, m_lPlayBet[wChairID], sizeof(CMDBetClear.lPlayBet));

	//�����ע
	BOOL bSendMessage = FALSE;
	for ( int i = 0; i < ANIMAL_MAX; ++i )
	{
		if ( m_lPlayBet[wChairID][i] > 0 )
		{
			bSendMessage = TRUE;
			m_lPlayChip[wChairID] += m_lPlayBet[wChairID][i];
			m_lAllBet[i] -= m_lPlayBet[wChairID][i];
			m_lPlayBet[wChairID][i] = 0;
		}
	}
	
	//������Ϣ
	if ( bSendMessage )
	{
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_BET_CLEAR, &CMDBetClear, sizeof(CMDBetClear));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BET_CLEAR, &CMDBetClear, sizeof(CMDBetClear));
	}
	
	return true;
}

//����ת��
bool CTableFrameSink::CreateTableTable()
 {
	//���ñ���
	m_bTurnTwoTime = FALSE;

	//������һ��ͼ
	int nWinTable = CGameLogic::GetInstance()->RandomArea(TURAN_TABLE_MAX, 
															m_nGenerationChance[ANIMAL_SLIVER_SHARK]/4, m_nGenerationChance[ANIMAL_EAGLE]/3, m_nGenerationChance[ANIMAL_EAGLE]/3, m_nGenerationChance[ANIMAL_EAGLE]/3,
															m_nGenerationChance[ANIMAL_GOLD_SHARK]/4, m_nGenerationChance[ANIMAL_LION]/3, m_nGenerationChance[ANIMAL_LION]/3, m_nGenerationChance[ANIMAL_LION]/3,
															m_nGenerationChance[ANIMAL_SLIVER_SHARK]/4, m_nGenerationChance[ANIMAL_PANDA]/2, m_nGenerationChance[ANIMAL_PANDA]/2, m_nGenerationChance[ANIMAL_GOLD_SHARK]/4,
															m_nGenerationChance[ANIMAL_MONKEY]/2, m_nGenerationChance[ANIMAL_MONKEY]/2, m_nGenerationChance[ANIMAL_SLIVER_SHARK]/4, m_nGenerationChance[ANIMAL_RABBIT]/3,
															m_nGenerationChance[ANIMAL_RABBIT]/3, m_nGenerationChance[ANIMAL_RABBIT]/3, m_nGenerationChance[ANIMAL_GOLD_SHARK]/4, m_nGenerationChance[ANIMAL_SWALLOW]/3,
															m_nGenerationChance[ANIMAL_SWALLOW]/3, m_nGenerationChance[ANIMAL_SWALLOW]/3, m_nGenerationChance[ANIMAL_SLIVER_SHARK]/4, m_nGenerationChance[ANIMAL_PIGEON]/2,
															m_nGenerationChance[ANIMAL_PIGEON]/2, m_nGenerationChance[ANIMAL_GOLD_SHARK]/4, m_nGenerationChance[ANIMAL_PEACOCK]/2, m_nGenerationChance[ANIMAL_PEACOCK]/2);

	
	////int nTemp[] = { 0, 4 };
	//static nWin = 0;
	//nWin = (nWin + 1)%CountArray(nTemp);
	//nWinTable = nTemp[nWin];
	//nWinTable = 4;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//m_nTurnTableTarget[0]//���Ƶ�һ���ֵö���
	m_nTurnTableTarget[0] = nWinTable;
	BYTE ProbilityRand=GetProbility();
	bool bControl = false;
	if( m_pServerContro != NULL && m_pServerContro->NeedControl() )//�ڴ˴��������
	{
		if(m_pServerContro->ControlResult(&m_nTurnTableTarget[0]))
		{
			bControl=true;
			m_pServerContro->CompleteControl();
			nWinTable=m_nTurnTableTarget[0];
			if ( CGameLogic::GetInstance()->TurnTableAnimal(nWinTable) == ANIMAL_GOLD_SHARK || CGameLogic::GetInstance()->TurnTableAnimal(nWinTable) == ANIMAL_SLIVER_SHARK ) //��֮��QQ1090075705 ����
			m_bTurnTwoTime = TRUE; //��֮��QQ1090075705 ����
		}			
	}
	else if(rand()%100<ProbilityRand)
	{
		int minArea1=0;
		int minArea2=0;
		int minArea3=0;
		int minArea4=0;
		LONGLONG AreaBet[8];
		AreaBet[0]=m_HPlayBet[ANIMAL_BEAST]*2+m_HPlayBet[ANIMAL_LION]*m_nAnimalMultiple[ANIMAL_LION];
		AreaBet[1]=m_HPlayBet[ANIMAL_BEAST]*2+m_HPlayBet[ANIMAL_PANDA]*m_nAnimalMultiple[ANIMAL_PANDA];
		AreaBet[2]=m_HPlayBet[ANIMAL_BEAST]*2+m_HPlayBet[ANIMAL_MONKEY]*m_nAnimalMultiple[ANIMAL_MONKEY];
		AreaBet[3]=m_HPlayBet[ANIMAL_BEAST]*2+m_HPlayBet[ANIMAL_RABBIT]*m_nAnimalMultiple[ANIMAL_RABBIT];
		AreaBet[4]=m_HPlayBet[ANIMAL_BIRD]*2+m_HPlayBet[ANIMAL_EAGLE]*m_nAnimalMultiple[ANIMAL_EAGLE];
		AreaBet[5]=m_HPlayBet[ANIMAL_BIRD]*2+m_HPlayBet[ANIMAL_PEACOCK]*m_nAnimalMultiple[ANIMAL_PEACOCK];
		AreaBet[6]=m_HPlayBet[ANIMAL_BIRD]*2+m_HPlayBet[ANIMAL_PIGEON]*m_nAnimalMultiple[ANIMAL_PIGEON];
		AreaBet[7]=m_HPlayBet[ANIMAL_BIRD]*2+m_HPlayBet[ANIMAL_SWALLOW]*m_nAnimalMultiple[ANIMAL_SWALLOW];
		LONGLONG maxAreaBet=0;
		int maxArea=0;
		for	 (int i=0;i<8;i++)
		{
			if (maxAreaBet<=AreaBet[i])
			{
				maxAreaBet=AreaBet[i];
				maxArea=i;
			}
		}
		if (maxAreaBet!=0)
		{
			LONGLONG AreaBet1=AreaBet[maxArea];
			LONGLONG AreaBet2=AreaBet[maxArea];
			LONGLONG AreaBet3=AreaBet[maxArea];
			LONGLONG AreaBet4=AreaBet[maxArea];
			for	 (int i=0;i<8;i++)
			{
				if (AreaBet1>=AreaBet[i])
				{
					AreaBet1=AreaBet[i];
					minArea1=i;
				}
			}
			for (int i=0;i<8;i++)
			{
				if (AreaBet2>=AreaBet[i]&&i!=minArea1)
				{
					AreaBet2=AreaBet[i];
					minArea2=i;
				}
			}
			for (int i=0;i<8;i++)
			{
				if (AreaBet3>=AreaBet[i]&&i!=minArea1&&i!=minArea2)
				{
					AreaBet3=AreaBet[i];
					minArea3=i;
				}
			}
			for (int i=0;i<8;i++)
			{
				if (AreaBet4>=AreaBet[i]&&i!=minArea1&&i!=minArea2&&i!=minArea3)
				{
					AreaBet4=AreaBet[i];
					minArea4=i;
				}
			}
		/*	switch(rand()%4)
			{
			case 0:
				nWinTable=minArea1;
				break;
			case 1:
				nWinTable=minArea2;
				break;
			case 2:
				nWinTable=minArea3;
				break;
			case 3:
				nWinTable=minArea4;
				break;
			}*/
			nWinTable=minArea1;
			switch(nWinTable)
			{
			case 0:
				{
					switch(rand()%3)
					{
					case 0:
						nWinTable=5;
						break;
					case 1:
						nWinTable=6;
						break;
					case 2:
						nWinTable=7;
						break;
					}
				}
				break;
			case 1:
				{
					switch(rand()%2)
					{
					case 0:
						nWinTable=9;
						break;
					case 1:
						nWinTable=10;
						break;
					}
				}
				break;
			case 2:
				{
					switch(rand()%2)
					{
					case 0:
						nWinTable=12;
						break;
					case 1:
						nWinTable=13;
						break;
					}
				}
				break;
			case 3:
				{
					switch(rand()%3)
					{
					case 0:
						nWinTable=15;
						break;
					case 1:
						nWinTable=16;
						break;
					case 2:
						nWinTable=17;
						break;
					}
				}
				break;
			case 4:
				{
					switch(rand()%3)
					{
					case 0:
						nWinTable=1;
						break;
					case 1:
						nWinTable=2;
						break;
					case 2:
						nWinTable=3;
						break;
					}
				}
				break;
			case 5:
				{
					switch(rand()%2)
					{
					case 0:
						nWinTable=26;
						break;
					case 1:
						nWinTable=27;
						break;
					}
				}
				break;
			case 6:
				{
					switch(rand()%2)
					{
					case 0:
						nWinTable=23;
						break;
					case 1:
						nWinTable=24;
						break;
					}
				}
				break;
			case 7:
				{
					switch(rand()%3)
					{
					case 0:
						nWinTable=19;
						break;
					case 1:
						nWinTable=20;
						break;
					case 2:
						nWinTable=21;
						break;
					}
				}
				break;
			}
		}
		m_nTurnTableTarget[0] = nWinTable;
		//��������㣬������һ��ͼ
		if ( CGameLogic::GetInstance()->TurnTableAnimal(nWinTable) == ANIMAL_GOLD_SHARK || CGameLogic::GetInstance()->TurnTableAnimal(nWinTable) == ANIMAL_SLIVER_SHARK )
		{
			m_bTurnTwoTime = TRUE;


			//�����ڶ���ͼ
			nWinTable = CGameLogic::GetInstance()->RandomArea(TURAN_TABLE_MAX, 
				0, m_nGenerationChance[ANIMAL_EAGLE]/3, m_nGenerationChance[ANIMAL_EAGLE]/3, m_nGenerationChance[ANIMAL_EAGLE]/3,
				0, m_nGenerationChance[ANIMAL_LION]/3, m_nGenerationChance[ANIMAL_LION]/3, m_nGenerationChance[ANIMAL_LION]/3,
				0, m_nGenerationChance[ANIMAL_PANDA]/2, m_nGenerationChance[ANIMAL_PANDA]/2, 0,
				m_nGenerationChance[ANIMAL_MONKEY]/2, m_nGenerationChance[ANIMAL_MONKEY]/2, 0, m_nGenerationChance[ANIMAL_RABBIT]/3,
				m_nGenerationChance[ANIMAL_RABBIT]/3, m_nGenerationChance[ANIMAL_RABBIT]/3, 0, m_nGenerationChance[ANIMAL_SWALLOW]/3,
				m_nGenerationChance[ANIMAL_SWALLOW]/3, m_nGenerationChance[ANIMAL_SWALLOW]/3, 0, m_nGenerationChance[ANIMAL_PIGEON]/2,
				m_nGenerationChance[ANIMAL_PIGEON]/2, 0, m_nGenerationChance[ANIMAL_PEACOCK]/2, m_nGenerationChance[ANIMAL_PEACOCK]/2);

			////nWinTable=3;
			//m_nTurnTableTarget[1] //���Ƶ����ֽ�������֮�󣬵ڶ��γ��ֵĶ���
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			m_nTurnTableTarget[1] = nWinTable;
			int minArea1=0;
			int minArea2=0;
			int minArea3=0;
			int minArea4=0;
			LONGLONG AreaBet[8];
			AreaBet[0]=m_HPlayBet[ANIMAL_BEAST]*2+m_HPlayBet[ANIMAL_LION]*m_nAnimalMultiple[ANIMAL_LION];
			AreaBet[1]=m_HPlayBet[ANIMAL_BEAST]*2+m_HPlayBet[ANIMAL_PANDA]*m_nAnimalMultiple[ANIMAL_PANDA];
			AreaBet[2]=m_HPlayBet[ANIMAL_BEAST]*2+m_HPlayBet[ANIMAL_MONKEY]*m_nAnimalMultiple[ANIMAL_MONKEY];
			AreaBet[3]=m_HPlayBet[ANIMAL_BEAST]*2+m_HPlayBet[ANIMAL_RABBIT]*m_nAnimalMultiple[ANIMAL_RABBIT];
			AreaBet[4]=m_HPlayBet[ANIMAL_BIRD]*2+m_HPlayBet[ANIMAL_EAGLE]*m_nAnimalMultiple[ANIMAL_EAGLE];
			AreaBet[5]=m_HPlayBet[ANIMAL_BIRD]*2+m_HPlayBet[ANIMAL_PEACOCK]*m_nAnimalMultiple[ANIMAL_PEACOCK];
			AreaBet[6]=m_HPlayBet[ANIMAL_BIRD]*2+m_HPlayBet[ANIMAL_PIGEON]*m_nAnimalMultiple[ANIMAL_PIGEON];
			AreaBet[7]=m_HPlayBet[ANIMAL_BIRD]*2+m_HPlayBet[ANIMAL_SWALLOW]*m_nAnimalMultiple[ANIMAL_SWALLOW];
			LONGLONG maxAreaBet=0;
			int maxArea=0;
			for	 (int i=0;i<8;i++)
			{
				if (maxAreaBet<=AreaBet[i])
				{
					maxAreaBet=AreaBet[i];
					maxArea=i;
				}
			}
			if (maxAreaBet!=0)
			{
				LONGLONG AreaBet1=AreaBet[maxArea];
				LONGLONG AreaBet2=AreaBet[maxArea];
				LONGLONG AreaBet3=AreaBet[maxArea];
				LONGLONG AreaBet4=AreaBet[maxArea];
			for	 (int i=0;i<8;i++)
			{
				if (AreaBet1>=AreaBet[i])
				{
					AreaBet1=AreaBet[i];
					minArea1=i;
				}
			}
			for (int i=0;i<8;i++)
			{
				if (AreaBet2>=AreaBet[i]&&i!=minArea1)
				{
					AreaBet2=AreaBet[i];
					minArea2=i;
				}
			}
			for (int i=0;i<8;i++)
			{
				if (AreaBet3>=AreaBet[i]&&i!=minArea1&&i!=minArea2)
				{
					AreaBet3=AreaBet[i];
					minArea3=i;
				}
			}
			for (int i=0;i<8;i++)
			{
				if (AreaBet4>=AreaBet[i]&&i!=minArea1&&i!=minArea2&&i!=minArea3)
				{
					AreaBet4=AreaBet[i];
					minArea4=i;
				}
			}
			/*switch(rand()%4)
			{
			case 0:
				nWinTable=minArea1;
				break;
			case 1:
				nWinTable=minArea2;
				break;
			case 2:
				nWinTable=minArea3;
				break;
			case 3:
				nWinTable=minArea4;
				break;
			}*/
			nWinTable=minArea1;
			switch(nWinTable)
			{
			case 0:
				{
					switch(rand()%3)
					{
					case 0:
						nWinTable=5;
						break;
					case 1:
						nWinTable=6;
						break;
					case 2:
						nWinTable=7;
						break;
					}
				}
				break;
			case 1:
				{
					switch(rand()%2)
					{
					case 0:
						nWinTable=9;
						break;
					case 1:
						nWinTable=10;
						break;
					}
				}
				break;
			case 2:
				{
					switch(rand()%2)
					{
					case 0:
						nWinTable=12;
						break;
					case 1:
						nWinTable=13;
						break;
					}
				}
				break;
			case 3:
				{
					switch(rand()%3)
					{
					case 0:
						nWinTable=15;
						break;
					case 1:
						nWinTable=16;
						break;
					case 2:
						nWinTable=17;
						break;
					}
				}
				break;
			case 4:
				{
					switch(rand()%3)
					{
					case 0:
						nWinTable=1;
						break;
					case 1:
						nWinTable=2;
						break;
					case 2:
						nWinTable=3;
						break;
					}
				}
				break;
			case 5:
				{
					switch(rand()%2)
					{
					case 0:
						nWinTable=26;
						break;
					case 1:
						nWinTable=27;
						break;
					}
				}
				break;
			case 6:
				{
					switch(rand()%2)
					{
					case 0:
						nWinTable=23;
						break;
					case 1:
						nWinTable=24;
						break;
					}
				}
				break;
			case 7:
				{
					switch(rand()%3)
					{
					case 0:
						nWinTable=19;
						break;
					case 1:
						nWinTable=20;
						break;
					case 2:
						nWinTable=21;
						break;
					}
				}
				break;
			}
			m_nTurnTableTarget[1] = nWinTable;
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
	else
	{
		//�����ڶ���ͼ
		nWinTable = CGameLogic::GetInstance()->RandomArea(TURAN_TABLE_MAX, 
			0, m_nGenerationChance[ANIMAL_EAGLE]/3, m_nGenerationChance[ANIMAL_EAGLE]/3, m_nGenerationChance[ANIMAL_EAGLE]/3,
			0, m_nGenerationChance[ANIMAL_LION]/3, m_nGenerationChance[ANIMAL_LION]/3, m_nGenerationChance[ANIMAL_LION]/3,
			0, m_nGenerationChance[ANIMAL_PANDA]/2, m_nGenerationChance[ANIMAL_PANDA]/2, 0,
			m_nGenerationChance[ANIMAL_MONKEY]/2, m_nGenerationChance[ANIMAL_MONKEY]/2, 0, m_nGenerationChance[ANIMAL_RABBIT]/3,
			m_nGenerationChance[ANIMAL_RABBIT]/3, m_nGenerationChance[ANIMAL_RABBIT]/3, 0, m_nGenerationChance[ANIMAL_SWALLOW]/3,
			m_nGenerationChance[ANIMAL_SWALLOW]/3, m_nGenerationChance[ANIMAL_SWALLOW]/3, 0, m_nGenerationChance[ANIMAL_PIGEON]/2,
			m_nGenerationChance[ANIMAL_PIGEON]/2, 0, m_nGenerationChance[ANIMAL_PEACOCK]/2, m_nGenerationChance[ANIMAL_PEACOCK]/2);

		////nWinTable=3;
		//m_nTurnTableTarget[1] //���Ƶ����ֽ�������֮�󣬵ڶ��γ��ֵĶ���
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_nTurnTableTarget[1] = nWinTable;
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return true;
}

//��Ϸ��������
int CTableFrameSink::GameOver()
{
	//�������
	int nPrizesMultiple = 0;
	bool bSuccess = false;
	LONGLONG lSystemScore = 0l;

	LONGLONG lTempCalculateScore = 0l;
	LONGLONG lMaxSystemScore = LLONG_MIN;
	int nMaxTurnTableTarget[2] = {0};
	int nWhileCount = 0;

	//�������
	do 
	{
		//��Ӯ����
		ZeroMemory(m_lPlayWinAnimal, sizeof(m_lPlayWinAnimal));
		ZeroMemory(m_lPlayWinChip, sizeof(m_lPlayWinChip));
		ZeroMemory(m_lPlayWinScore, sizeof(m_lPlayWinScore));
		ZeroMemory(m_lPlayWinPrizes, sizeof(m_lPlayWinPrizes));

		//ϵͳ��Ӯ
		lSystemScore = 0l;

		//�ɷ��˿�
		CreateTableTable();

		//��̽���ж�
		lSystemScore += CalculateScore( true, CGameLogic::GetInstance()->TurnTableAnimal(m_nTurnTableTarget[0]), nPrizesMultiple );
//CHAR sztmp1[32] ;
//_snprintf(sztmp1, sizeof(sztmp1), "1����%ld " ,lSystemScore ) ;
//MessageBoxA(NULL,sztmp1,"����1", MB_OK );

		if ( m_bTurnTwoTime)
			lSystemScore += CalculateScore( false, CGameLogic::GetInstance()->TurnTableAnimal(m_nTurnTableTarget[1]), nPrizesMultiple );
//CHAR sztmp2[32] ;
//_snprintf(sztmp2, sizeof(sztmp2), "�÷֣�%s,%ld,%ld " ,sztmp1,lSystemScore,m_lStorageStart ) ;
//MessageBoxA(NULL,sztmp2,"����2", MB_OK );

		//ϵͳ��ֵ����
		if ((lSystemScore + m_lStorageStart) < 0l)
		{
			bSuccess =  true;

			nWhileCount++;
			if( lSystemScore > lMaxSystemScore )
			{
				lMaxSystemScore = lSystemScore;
				CopyMemory(nMaxTurnTableTarget, m_nTurnTableTarget, sizeof(nMaxTurnTableTarget));
			}

			if( nWhileCount >= 100000 && lMaxSystemScore != LLONG_MIN )
			{
				//ǿ�ƽ���
				m_lStorageStart += lMaxSystemScore;
				CopyMemory(m_nTurnTableTarget, nMaxTurnTableTarget, sizeof(m_nTurnTableTarget));
				break;
			}
		}
		else
		{
			m_lStorageStart += lSystemScore;
			
			//���˲���������ע��˥�����
			for( int i = 0; i < GAME_PLAYER; ++i )
			{
				if( m_pITableFrame->GetTableUserItem(i) != NULL && lSystemScore != 0 )
				{
					m_lStorageStart = m_lStorageStart - (m_lStorageStart * m_lStorageDeduct / 1000);
					break;
				}
			}

			bSuccess = true;
		}

	} while (!bSuccess);

	//��Ӽ�¼
	m_ArrayTurnTableRecord.Add( m_nTurnTableTarget[0] );
	if( m_ArrayTurnTableRecord.GetCount() > RECORD_COUNT_MAX )
	{
		m_ArrayTurnTableRecord.RemoveAt(0);
	}
////////��֮��QQ1090075705 ������
/*
CHAR sztmp2[32] ;
_snprintf(sztmp2, sizeof(sztmp2), "��һ����%d �ڶ�����%d" ,CGameLogic::GetInstance()->TurnTableAnimal(m_nTurnTableTarget[0]),CGameLogic::GetInstance()->TurnTableAnimal(m_nTurnTableTarget[1]) ) ;
MessageBoxA(NULL,sztmp2,"����", MB_OK );
*/
/////////////��֮��QQ1090075705 ������


return nPrizesMultiple;
}

//����÷�
LONGLONG CTableFrameSink::CalculateScore( bool bFirst, int nWinAnimal, int& nPrizesMultiple )
{

///��֮��QQ1090075705 ������
/*
CHAR sztmp3[32] ;
_snprintf(sztmp3, sizeof(sztmp3), "��%d�ε���,������%d" ,bFirst,nWinAnimal ) ;
MessageBoxA(NULL,sztmp3,"����", MB_OK );
*/
///��֮��QQ1090075705 ������

	//ϵͳ��Ӯ
	LONGLONG lSystemScore = 0l;
	
	//�ʽ�
	if ( bFirst && nWinAnimal == ANIMAL_GOLD_SHARK )
	{
		nPrizesMultiple = (rand()%94) + 6;
	}

	//�������
	for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		//�Ƿ������
		bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();

		for ( int nAnimalIndex = 0; nAnimalIndex < ANIMAL_MAX; ++nAnimalIndex )
		{
			if ( nWinAnimal == nAnimalIndex || 
				(nAnimalIndex == ANIMAL_BIRD && CGameLogic::GetInstance()->AnimalType(nWinAnimal) == ANIMAL_TYPE_BIRD ) ||
				(nAnimalIndex == ANIMAL_BEAST && CGameLogic::GetInstance()->AnimalType(nWinAnimal) == ANIMAL_TYPE_BEAST ) )
			{
				//���Ӯ��
				LONGLONG lWinScore = m_lPlayBet[wChairID][nAnimalIndex] * m_nAnimalMultiple[nAnimalIndex];
				m_lPlayWinChip[wChairID] += lWinScore;
				m_lPlayWinAnimal[wChairID][nAnimalIndex] += lWinScore;

				if ( bFirst )
					m_lPlayWinScore[wChairID][0] += lWinScore;
				else
					m_lPlayWinScore[wChairID][1] += lWinScore;

				//ϵͳ���
				if ( !bIsAndroidUser )
					lSystemScore -= (lWinScore - m_lPlayBet[wChairID][nAnimalIndex]);
			}
			else if( bFirst )
			{
				//������
				LONGLONG lLoseScore = m_lPlayBet[wChairID][nAnimalIndex];

				//ϵͳӮ��
				if ( !bIsAndroidUser )
					lSystemScore += lLoseScore;
			}
		}

/////////��֮��QQ1090075705 ������
		/*
if ( !bIsAndroidUser )
{
CHAR sztmp[32] ;
ZeroMemory(sztmp, sizeof(sztmp));
_snprintf(sztmp, sizeof(sztmp), "�ɼ�:%ld,��һ����%ld �ڶ�����%ld" ,m_lPlayWinChip[wChairID],m_lPlayWinScore[wChairID][0],m_lPlayWinScore[wChairID][1] ) ;
MessageBoxA(NULL,sztmp,"����", MB_OK );
}
*/
/////////��֮��QQ1090075705 ������




		//����ʽ�
		if ( nPrizesMultiple != 0 && bFirst )
		{
			//���Ӯ��
			LONGLONG lWinScore = (m_lPlayBet[wChairID][nWinAnimal] * nPrizesMultiple);
			m_lPlayWinChip[wChairID] += lWinScore;
			m_lPlayWinAnimal[wChairID][nWinAnimal] += lWinScore;
			m_lPlayWinPrizes[wChairID] += lWinScore;

			//ϵͳ���
			if ( !bIsAndroidUser )
				lSystemScore -= lWinScore;
		}

	}

	return lSystemScore;
}

//����д��
void CTableFrameSink::GameOverWriteScore()
{
	//д�����
	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	{
		//�۳�����
		m_lPlayChip[wChairID] += m_lPlayWinChip[wChairID];
		
		if ( m_lPlayChip[wChairID] < 0 )
		{
			ASSERT(FALSE);
			m_lPlayChip[wChairID] = 0;
		}

////��֮��QQ1090075705 ������
/*
IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
if ((pIServerUserItem != NULL) &&(!pIServerUserItem->IsAndroidUser()))
{
CHAR sztmp[32] ;
ZeroMemory(sztmp, sizeof(sztmp));
_snprintf(sztmp, sizeof(sztmp), "�ɼ�:%ld,��һ����%ld �ڶ�����%ld" ,m_lPlayWinChip[wChairID],m_lPlayWinScore[wChairID][0],m_lPlayWinScore[wChairID][1] ) ;
MessageBoxA(NULL,sztmp,"����", MB_OK );
}
*/
/////��֮��QQ1090075705 ������


	}

	//�����ע
	ZeroMemory(m_lPlayWinAnimal, sizeof(m_lPlayWinAnimal));
	ZeroMemory(m_lPlayWinChip, sizeof(m_lPlayWinChip));
	ZeroMemory(m_lPlayWinScore, sizeof(m_lPlayWinScore));
	ZeroMemory(m_lPlayWinPrizes, sizeof(m_lPlayWinPrizes));

	//��ע��
	ZeroMemory(m_lAllBet, sizeof(m_lAllBet));
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(m_HPlayBet,sizeof(m_HPlayBet));
	ZeroMemory(m_FPlayBet,sizeof(m_FPlayBet));
	ZeroMemory(m_APlayBet,sizeof(m_APlayBet));
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
}

//��ȡ�����ļ�
BYTE CTableFrameSink::GetProbility()
{
	return ::GetPrivateProfileInt(m_pGameServiceOption->szServerName, _T("Problility"), 30, szConfigFileName);
}

//////////////////////////////////////////////////////////////////////////////////
