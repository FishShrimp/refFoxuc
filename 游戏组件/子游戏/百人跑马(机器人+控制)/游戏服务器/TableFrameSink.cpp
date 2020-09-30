#include "StdAfx.h"
#include "TableFrameSink.h"


//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define IDI_FREE					1									//����ʱ��
#define TIME_FREE					1									//����ʱ��

//��עʱ��
#define IDI_BET_START				2									//��ʼ��ע
#define TIME_BET_START				10									//��עʱ��

//��ע����ʱ��
#define IDI_BET_END					3									//��ʼ��ע
#define TIME_BET_END				1									//��ע����ʱ��

//����ʱ��
#define IDI_HORSES_START			4									//����ʼ
#define TIME_HORSES_START			50									//����ʱ��

//����ʱ��
#define IDI_HORSES_END				5									//�������(ǿ�ƿͻ��˽���)
#define TIME_HORSES_END				(TIME_HORSES_START-5)				//����ʱ��

//����ʱ��
#define IDI_STORAGE_INTERVAL		6									//����ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_nStreak = 0;
	m_dwGameTime = 0;
	m_nFreeTime = TIME_FREE;
	m_nBetTime = TIME_BET_START;
	m_nBetEndTime = TIME_BET_END;
	m_nHorsesTime = TIME_HORSES_START;
	m_nHorsesEndTime = TIME_HORSES_END;
	CTime time = CTime::GetCurrentTime();					//��ȡ��ǰʱ��. 
	m_nDay = time.GetDay();									//��

	//���Ʊ���
	m_cbCLTimes = 0;
	m_bControl = false;
	m_cbCLArea = 255;
	for (int i = 0 ; i < AREA_ALL; ++i)
		m_nCLMultiple[i] = -1;


	//���Ʊ���
	m_lAreaLimitScore = 0l;
	m_lUserLimitScore = 0l;

	//���
	m_StorageStart = 0l;
	m_StorageStart = 1;

	//��Ϸ���
	m_cbGameResults = AREA_ALL;

	//����
	m_nBetPlayerCount = 0;
	ZeroMemory(m_lPlayerBet,sizeof(m_lPlayerBet));
	ZeroMemory(m_lPlayerBetWin,sizeof(m_lPlayerBetWin));
	ZeroMemory(m_lPlayerBetAll,sizeof(m_lPlayerBetAll));
	ZeroMemory(m_lPlayerWinning,sizeof(m_lPlayerWinning));
	ZeroMemory(m_lPlayerReturnBet,sizeof(m_lPlayerReturnBet));
	ZeroMemory(m_lPlayerRevenue,sizeof(m_lPlayerRevenue));

	//������
	m_bMultipleControl = FALSE;
	for ( int i = 0; i < CountArray(m_nMultiple); ++i)
		m_nMultiple[i] = 1;

	//��ƥ��Ϣ
	ZeroMemory(m_nHorsesSpeed,sizeof(m_nHorsesSpeed));
	for ( int i = 0; i < CountArray(m_cbHorsesRanking); ++i)
		m_cbHorsesRanking[i] = HORSES_ALL;

	//��ƥ����
	_sntprintf(m_szHorsesName[HORSES_ONE],sizeof(m_szHorsesName[HORSES_ONE]),TEXT("һ����"));
	_sntprintf(m_szHorsesName[HORSES_TWO],sizeof(m_szHorsesName[HORSES_TWO]),TEXT("������"));
	_sntprintf(m_szHorsesName[HORSES_THREE],sizeof(m_szHorsesName[HORSES_THREE]),TEXT("������"));
	_sntprintf(m_szHorsesName[HORSES_FOUR],sizeof(m_szHorsesName[HORSES_FOUR]),TEXT("�ĺ���"));
	_sntprintf(m_szHorsesName[HORSES_FIVE],sizeof(m_szHorsesName[HORSES_FIVE]),TEXT("�����"));
	_sntprintf(m_szHorsesName[HORSES_SIX],sizeof(m_szHorsesName[HORSES_SIX]),TEXT("������"));

	//������Ϣ
	ZeroMemory(m_szConfigFileName,sizeof(m_szConfigFileName));
	ZeroMemory(m_szGameRoomName,sizeof(m_szGameRoomName));

	//��Ϸ��¼
	m_GameRecords.RemoveAll();
	ZeroMemory(m_nWinCount,sizeof(m_nWinCount));

	//�����˿���
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));
	
	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
{
}

VOID CTableFrameSink::Release()
{
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
//��ȡ����
LONGLONG CTableFrameSink::GetPrivateProfileInt64(LPCTSTR lpAppName, LPCTSTR lpKeyName, LONGLONG lDefault, LPCTSTR lpFileName)
{
	//��������
	TCHAR OutBuf[255] = _T("");
	TCHAR DefaultBuf[255] = {};
	LONGLONG lNumber = 0;

	GetPrivateProfileString(lpAppName, lpKeyName, DefaultBuf, OutBuf, 255, lpFileName);

	if (OutBuf[0] != 0)
		_sntscanf(OutBuf, _tcslen(OutBuf), _T("%I64d"), &lNumber);
	else
		lNumber = lDefault;

	return lNumber;
}
//��ʼ��
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{

	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);

	m_pGameServiceOption = m_pITableFrame->GetGameServiceOption();
	m_pGameServiceAttrib = m_pITableFrame->GetGameServiceAttrib();

	ASSERT(m_pGameServiceOption!=NULL);
	ASSERT(m_pGameServiceAttrib!=NULL);
	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\HorseBattle.ini"),szPath);
	memcpy(m_szGameRoomName, m_pGameServiceOption->szServerName, sizeof(m_szGameRoomName));

	//���Ʊ���
	TCHAR OutBuf[255];

	//��������
	m_lRobotScoreRange[0] = GetPrivateProfileInt64(m_szGameRoomName, _T("RobotScoreMin"), 10000000, m_szConfigFileName);
	m_lRobotScoreRange[1] = GetPrivateProfileInt64(m_szGameRoomName, _T("RobotScoreMax"), 100000000, m_szConfigFileName);

	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//�������
	m_lRobotBankGetScore = GetPrivateProfileInt64(m_szGameRoomName, _T("RobotBankGet"), 20000000, m_szConfigFileName);

	//������� (ׯ��)
	m_lRobotBankGetScoreBanker = GetPrivateProfileInt64(m_szGameRoomName, _T("RobotBankGetBanker"), 30000000, m_szConfigFileName);

	//����
	m_nRobotBankStorageMul = GetPrivateProfileInt(m_szGameRoomName, _T("RobotBankStoMul"), 2, m_szConfigFileName);

	//���������ʱ��
	m_nStorageIntervalTime = GetPrivateProfileInt(m_szGameRoomName, _T("StorageIntervalTime"), 1, m_szConfigFileName);
	if ( m_nStorageIntervalTime<=0 ) m_nStorageIntervalTime=1;

	//�����Ŀ
	m_nStorageCount =  GetPrivateProfileInt(m_szGameRoomName, _T("StorageCount"), 1, m_szConfigFileName);
	if ( m_nStorageCount<=0 ) m_nStorageCount=1;
	if ( m_nStorageCount==1 ) m_nStorageIntervalTime=0;//ȡ��ת��

	memset(m_StorageArray,0,sizeof(m_StorageArray));
	CString strStorageName=TEXT("");
	for (size_t begin=1; begin<=m_nStorageCount; ++begin)
	{
		strStorageName.Format(TEXT("%dStorage"),begin);
		memset(OutBuf,0,255);
		GetPrivateProfileString(m_szGameRoomName,strStorageName.GetBuffer(),TEXT("0"),OutBuf,255,m_szConfigFileName);
		_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%I64d"),&m_StorageArray[begin-1]);	
	}
	//��潵ֵ
	m_StorageDeduct = GetPrivateProfileInt(m_szGameRoomName, TEXT("StorageDeduct"), 1, m_szConfigFileName);
	if ( m_StorageDeduct > 1000 )
		m_StorageDeduct = 900;

	//���ƿ���
	//----------------------------
	//��������
	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("AreaLimitScore"),TEXT("1000000000"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%I64d"),&m_lAreaLimitScore);
	//��������
	memset(OutBuf,0,255);
	GetPrivateProfileString(m_szGameRoomName,TEXT("UserLimitScore"),TEXT("10000000"),OutBuf,255,m_szConfigFileName);
	_sntscanf(OutBuf,_tcslen(OutBuf),TEXT("%I64d"),&m_lUserLimitScore);

	//ʱ�����
	//----------------------------
	//����ʱ��
	m_nFreeTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("FreeTime"), TIME_FREE, m_szConfigFileName);
	//��עʱ��
	m_nBetTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("BetTime"), TIME_BET_START, m_szConfigFileName);
	//��ע����ʱ��
	m_nBetEndTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("BetEndTime"), TIME_BET_END, m_szConfigFileName);
	//����ʱ��
	m_nHorsesTime = GetPrivateProfileInt(m_szGameRoomName, TEXT("HorsesTime"), TIME_HORSES_START, m_szConfigFileName);

	if ( m_nFreeTime < 10 )
		m_nFreeTime = 10;

	if ( m_nBetTime < 10 )
		m_nBetTime = 10;

	if ( m_nBetEndTime < 10 )
		m_nBetEndTime = 10;

	if ( m_nHorsesTime < 45 )
		m_nHorsesTime = 45;
	
	m_nHorsesEndTime = m_nHorsesTime - 5;

	//�����˿���
	//��ע��������Ŀ
	m_nMaxChipRobot = GetPrivateProfileInt(m_szGameRoomName, TEXT("RobotBetCount"), 10, m_szConfigFileName);
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 10;

	//��������ע����
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szGameRoomName, TEXT("RobotAreaLimit"), TEXT("50000000"), OutBuf, 255, m_szConfigFileName);
	_sntscanf(OutBuf, _tcslen(OutBuf), _T("%I64d"), &m_lRobotAreaLimit);
	
	m_nStorageNowNode=rand()%m_nStorageCount;
	m_StorageStart=m_StorageArray[m_nStorageNowNode];

	if ( m_nStorageIntervalTime>0 )
	{
		m_pITableFrame->SetGameTimer(IDI_STORAGE_INTERVAL,m_nStorageIntervalTime*60000,1,0L);
	}
	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	//��Ϸ���
	m_cbGameResults = AREA_ALL;

	//����
	m_nBetPlayerCount = 0;
	ZeroMemory(m_lPlayerBet,sizeof(m_lPlayerBet));
	ZeroMemory(m_lPlayerBetWin,sizeof(m_lPlayerBetWin));
	ZeroMemory(m_lPlayerBetAll,sizeof(m_lPlayerBetAll));
	ZeroMemory(m_lPlayerWinning,sizeof(m_lPlayerWinning));
	ZeroMemory(m_lPlayerReturnBet,sizeof(m_lPlayerReturnBet));
	ZeroMemory(m_lPlayerRevenue,sizeof(m_lPlayerRevenue));

	//��ƥ��Ϣ
	ZeroMemory(m_nHorsesSpeed,sizeof(m_nHorsesSpeed));
	for ( int i = 0; i < CountArray(m_cbHorsesRanking); ++i)
		m_cbHorsesRanking[i] = HORSES_ALL;

	//�����˿���
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	return;
}

//��ѯ�޶�
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	if(pIServerUserItem->GetUserStatus() == US_PLAYING)
	{
		return 0L;
	}
	else
	{
		return __max(pIServerUserItem->GetUserScore()-m_pGameServiceOption->lMinTableScore, 0L);
	}
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{

	//�������
	CMD_S_BetStart stBetStart;
	ZeroMemory(&stBetStart, sizeof(stBetStart));
	stBetStart.nTimeLeave = m_nBetTime;
	stBetStart.nTimeBetEnd = m_nBetEndTime;

	//��ע����������
	int nChipRobotCount = 0;
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}
	stBetStart.nChipRobotCount = min(nChipRobotCount, m_nMaxChipRobot);

	//�����˿���
	m_nChipRobotCount = 0;

	//������Ϣ 
	//----------------------------
	//�Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BET_START, &stBetStart, sizeof(stBetStart));	

	//��Ϸ���
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		//���û���
		stBetStart.lUserMaxScore = min(pIServerUserItem->GetUserScore(), m_lUserLimitScore);

		m_pITableFrame->SendTableData(wChairID,SUB_S_BET_START,&stBetStart,sizeof(stBetStart));	
	}
	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������	
		{
			//����
			HorsesProcess();

			CMD_S_HorsesStart stHorsesStart;
			ZeroMemory(&stHorsesStart, sizeof(stHorsesStart));
			stHorsesStart.nTimeLeave = m_nHorsesTime;
			memcpy(stHorsesStart.nHorsesSpeed, m_nHorsesSpeed, sizeof(stHorsesStart.nHorsesSpeed));
			memcpy(stHorsesStart.cbHorsesRanking, m_cbHorsesRanking, sizeof(stHorsesStart.cbHorsesRanking));

			//������Ϣ
			//----------------------------
			//�Թ����
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_HORSES_START, &stHorsesStart, sizeof(stHorsesStart));	

			//��Ϸ���
			for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
				if (pIServerUserItem==NULL) continue;

				//���û���
				stHorsesStart.lPlayerWinning = m_lPlayerWinning[wChairID];
				stHorsesStart.lPlayerReturnBet = m_lPlayerReturnBet[wChairID];

				m_pITableFrame->SendTableData(wChairID,SUB_S_HORSES_START,&stHorsesStart,sizeof(stHorsesStart));	
			}
			
			return true;
		}
	case GER_USER_LEAVE:		//�û��뿪
		{

			//��������
			LONGLONG lScore=0l;

			//ͳ�Ƴɼ�
			for (int i = 0; i < AREA_ALL; ++i) 
			{
				lScore -= m_lPlayerBet[wChairID][i];
				m_lPlayerBet[wChairID][i] = 0;
				m_lPlayerBetWin[wChairID][i] = 0;
			}
			m_lPlayerWinning[wChairID] = 0;
			m_lPlayerReturnBet[wChairID] = 0;
			m_lPlayerRevenue[wChairID] = 0;

			//д�����
			if ( lScore != 0 )
			{

				tagScoreInfo ScoreInfo;
				ZeroMemory(&ScoreInfo,sizeof(tagScoreInfo));
				ScoreInfo.lScore=lScore;
				ScoreInfo.cbType=SCORE_TYPE_LOSE;

				m_pITableFrame->WriteUserScore(pIServerUserItem->GetChairID(), ScoreInfo);
			}
			return true;
		}
	}

	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_FREE:		
		{
			//��������
			CMD_S_SceneFree SceneFree;
			ZeroMemory(&SceneFree,sizeof(SceneFree));	

			//ʱ��
			INT dwPassTime = (INT)time(NULL) - m_dwGameTime;
			SceneFree.nTimeLeave = (m_nFreeTime - __min(dwPassTime,m_nFreeTime));

			//����
			SceneFree.nStreak = m_nStreak;

			//����
			memcpy(SceneFree.nMultiple, m_nMultiple, sizeof(SceneFree.nMultiple));

			//��ƥ����
			memcpy(SceneFree.szHorsesName, m_szHorsesName, sizeof(SceneFree.szHorsesName));

			//��ʷ��¼
			for ( int i = 0; i < m_GameRecords.GetCount(); ++i)
				SceneFree.GameRecords[i] = m_GameRecords[i];

			//ȫ��Ӯ����
			memcpy(SceneFree.nWinCount, m_nWinCount, sizeof(SceneFree.nWinCount));

			//���Ʊ���
			SceneFree.lAreaLimitScore = m_lAreaLimitScore;						//����������
			SceneFree.lUserLimitScore = m_lUserLimitScore;						//������������

			//��������
			CopyMemory(SceneFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(SceneFree.szGameRoomName));
			
			//������Ϣ
			return m_pITableFrame->SendGameScene(pIServerUserItem,&SceneFree,sizeof(SceneFree));;
		}
	case GS_BET:	
		{
			//��������
			CMD_S_SceneBet SceneBet;
			ZeroMemory(&SceneBet,sizeof(SceneBet));	

			//ʱ��
			INT dwPassTime = (INT)time(NULL) - m_dwGameTime;
			SceneBet.nTimeLeave = (m_nBetTime - __min(dwPassTime,m_nBetTime));
			SceneBet.nTimeLeave += m_nBetEndTime;

			//����
			SceneBet.nStreak = m_nStreak;

			//����
			memcpy(SceneBet.nMultiple, m_nMultiple, sizeof(SceneBet.nMultiple));

			//��ƥ����
			memcpy(SceneBet.szHorsesName, m_szHorsesName, sizeof(SceneBet.szHorsesName));

			//��ʷ��¼
			for ( int i = 0; i < m_GameRecords.GetCount(); ++i)
				SceneBet.GameRecords[i] = m_GameRecords[i];

			//ȫ��Ӯ����
			memcpy(SceneBet.nWinCount, m_nWinCount, sizeof(SceneBet.nWinCount));

			//������ע
			SceneBet.lUserMaxScore = GetPlayersMaxBet(wChiarID);

			//��ע����
			SceneBet.nBetPlayerCount = m_nBetPlayerCount;

			//�����ע
			memcpy(SceneBet.lPlayerBet, m_lPlayerBet[wChiarID], sizeof(SceneBet.lPlayerBet));

			//������ע
			memcpy(SceneBet.lPlayerBetAll, m_lPlayerBetAll, sizeof(SceneBet.lPlayerBetAll));

			//���Ʊ���
			SceneBet.lAreaLimitScore = m_lAreaLimitScore;						//����������
			SceneBet.lUserLimitScore = m_lUserLimitScore;						//������������

			//��������
			CopyMemory(SceneBet.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(SceneBet.szGameRoomName));

			//������Ϣ
			return m_pITableFrame->SendGameScene(pIServerUserItem,&SceneBet,sizeof(SceneBet));;
		}
	case GS_BET_END:			//��ע����	
		{
			//��������
			CMD_S_SceneBetEnd SceneBetEnd;
			ZeroMemory(&SceneBetEnd,sizeof(SceneBetEnd));	

			//ʱ��
			INT dwPassTime = (INT)time(NULL) - m_dwGameTime;
			SceneBetEnd.nTimeLeave = (m_nBetEndTime - __min(dwPassTime,m_nBetEndTime));

			//����
			SceneBetEnd.nStreak = m_nStreak;

			//����
			memcpy(SceneBetEnd.nMultiple, m_nMultiple, sizeof(SceneBetEnd.nMultiple));

			//��ƥ����
			memcpy(SceneBetEnd.szHorsesName, m_szHorsesName, sizeof(SceneBetEnd.szHorsesName));

			//��ʷ��¼
			for ( int i = 0; i < m_GameRecords.GetCount(); ++i)
				SceneBetEnd.GameRecords[i] = m_GameRecords[i];

			//ȫ��Ӯ����
			memcpy(SceneBetEnd.nWinCount, m_nWinCount, sizeof(SceneBetEnd.nWinCount));

			//��ע����
			SceneBetEnd.nBetPlayerCount = m_nBetPlayerCount;

			//�����ע
			memcpy(SceneBetEnd.lPlayerBet, m_lPlayerBet[wChiarID], sizeof(SceneBetEnd.lPlayerBet));

			//������ע
			memcpy(SceneBetEnd.lPlayerBetAll, m_lPlayerBetAll, sizeof(SceneBetEnd.lPlayerBetAll));

			//���Ʊ���
			SceneBetEnd.lAreaLimitScore = m_lAreaLimitScore;						//����������
			SceneBetEnd.lUserLimitScore = m_lUserLimitScore;						//������������

			//��������
			CopyMemory(SceneBetEnd.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(SceneBetEnd.szGameRoomName));

			//������Ϣ
			return m_pITableFrame->SendGameScene(pIServerUserItem,&SceneBetEnd,sizeof(SceneBetEnd));;
		}
	case GS_HORSES:			
		{
			//��������
			CMD_S_SceneHorses SceneHorses;
			ZeroMemory(&SceneHorses,sizeof(SceneHorses));	

			//ʱ��
			INT dwPassTime = (INT)time(NULL) - m_dwGameTime;
			SceneHorses.nTimeLeave = (m_nHorsesTime - __min(dwPassTime,m_nHorsesTime));

			//����
			SceneHorses.nStreak = m_nStreak;

			//����
			memcpy(SceneHorses.nMultiple, m_nMultiple, sizeof(SceneHorses.nMultiple));

			//��ƥ����
			memcpy(SceneHorses.szHorsesName, m_szHorsesName, sizeof(SceneHorses.szHorsesName));

			//��ʷ��¼
			for ( int i = 0; i < m_GameRecords.GetCount(); ++i)
				SceneHorses.GameRecords[i] = m_GameRecords[i];

			//ȫ��Ӯ����
			memcpy(SceneHorses.nWinCount, m_nWinCount, sizeof(SceneHorses.nWinCount));

			//��ע����
			SceneHorses.nBetPlayerCount = m_nBetPlayerCount;

			//�����ע
			memcpy(SceneHorses.lPlayerBet, m_lPlayerBet[wChiarID], sizeof(SceneHorses.lPlayerBet));

			//������ע
			memcpy(SceneHorses.lPlayerBetAll, m_lPlayerBetAll, sizeof(SceneHorses.lPlayerBetAll));

			//������Ӯ
			SceneHorses.lPlayerWinning = m_lPlayerWinning[wChiarID];

			//���Ʊ���
			SceneHorses.lAreaLimitScore = m_lAreaLimitScore;						//����������
			SceneHorses.lUserLimitScore = m_lUserLimitScore;						//������������

			//��������
			CopyMemory(SceneHorses.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(SceneHorses.szGameRoomName));

			//������Ϣ
			return m_pITableFrame->SendGameScene(pIServerUserItem,&SceneHorses,sizeof(SceneHorses));;
		}
	}

	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_STORAGE_INTERVAL:
		{
			m_pITableFrame->KillGameTimer(IDI_STORAGE_INTERVAL);


			m_StorageArray[m_nStorageNowNode] = m_StorageStart;
			m_nStorageNowNode				  = rand()%m_nStorageCount;
			m_StorageStart					  = m_StorageArray[m_nStorageNowNode];

			if ( m_nStorageIntervalTime>0 )
			{
				m_pITableFrame->SetGameTimer(IDI_STORAGE_INTERVAL,m_nStorageIntervalTime*60*1000,1,0L);
			}
			return true;
		}
		case IDI_FREE:
			{
				for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
				{
					//��������
					IServerUserItem *pUserItem = m_pITableFrame->GetTableUserItem(wChairID);

					if ( (pUserItem==NULL) || (!pUserItem->IsAndroidUser())) 
					{
						continue;
					}

					LONGLONG lRobotScore = pUserItem->GetUserScore();
					LONGLONG lInsureScore = pUserItem->GetUserInsure();

					//�жϴ�ȡ
					if (lRobotScore > m_lRobotScoreRange[1])
					{
						//������Ϣ
						LONGLONG lStorageValue = lRobotScore - LONGLONG(lRobotScore/m_nRobotBankStorageMul);

						if (lStorageValue > 0 )
						{
							tagScoreInfo ScoreInfo;
							ZeroMemory(&ScoreInfo,sizeof(tagScoreInfo));
							ScoreInfo.lScore=lStorageValue*-1;
							ScoreInfo.cbType=SCORE_TYPE_LOSE;

							m_pITableFrame->WriteUserScore(pUserItem->GetChairID(),ScoreInfo);//��
						}
					}
					else if (lRobotScore < m_lRobotScoreRange[0])
					{
						//������Ϣ
						LONGLONG lGetValue = m_lRobotBankGetScore;

						if (lGetValue > 0)
						{
							tagScoreInfo ScoreInfo;
							ZeroMemory(&ScoreInfo,sizeof(tagScoreInfo));
							ScoreInfo.lScore=lGetValue;
							ScoreInfo.cbType=SCORE_TYPE_WIN;

							m_pITableFrame->WriteUserScore(pUserItem->GetChairID(),ScoreInfo);//ȡ
						}
					}
				}

				//��ʼ��Ϸ
				m_pITableFrame->StartGame();

				//����ʱ��
				m_dwGameTime = (DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_BET_START,m_nBetTime*1000,1,0L);

				//����״̬
				m_pITableFrame->SetGameStatus(GS_BET);
			}
			return true;
		case IDI_BET_START:			//��עʱ��
			{
				//����ʱ��
				m_dwGameTime = (DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_BET_END,m_nBetEndTime*1000,1,0L);

				//����״̬
				m_pITableFrame->SetGameStatus(GS_BET_END);	

				//������ע������Ϣ
				CMD_S_BetEnd stBetEnd;
				stBetEnd.nTimeLeave = m_nBetEndTime;
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BET_END,&stBetEnd,sizeof(stBetEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BET_END,&stBetEnd,sizeof(stBetEnd));
			}
			return true;
		case IDI_BET_END:			//��ע����
			{
				//״̬�ж�(��ֹǿ���ظ�����)
				if (m_pITableFrame->GetGameStatus()!=GS_HORSES)
				{
					//����״̬
					m_pITableFrame->SetGameStatus(GS_HORSES);			

					//��ʼ����(�Ƚ�����Ϸ)
					OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

					//����ʱ��
					m_dwGameTime = (DWORD)time(NULL);
					m_pITableFrame->SetGameTimer(IDI_HORSES_START, m_nHorsesTime*1000, 1, 0L);
					m_pITableFrame->SetGameTimer(IDI_HORSES_END, m_nHorsesEndTime*1000, 1, 0L);		

				}	
			}
			return true;
		case IDI_HORSES_START:		//����ʱ��
			{

				//д��
				for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
					if ( pIServerUserItem == NULL ) 
						continue;

					if ( m_lPlayerWinning[wChairID] != 0 )
					{
						tagScoreInfo ScoreInfo;
						ZeroMemory(&ScoreInfo,sizeof(tagScoreInfo));
						ScoreInfo.cbType=( m_lPlayerWinning[wChairID] > 0L ) ? SCORE_TYPE_WIN : SCORE_TYPE_LOSE;
						ScoreInfo.lScore=m_lPlayerWinning[wChairID];
						ScoreInfo.lRevenue=m_lPlayerRevenue[wChairID];
						
						//д�����
						m_pITableFrame->WriteUserScore(wChairID,ScoreInfo);
					}
				}



				//���п��ƿ�������
				if( m_cbCLTimes > 0 )
					m_bControl = true;

				//�������
				MultipleControl();

				//����ʱ��
				m_dwGameTime = (DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_FREE, m_nFreeTime*1000, 1, 0L);

				//������Ϣ
				CMD_S_HorsesEnd stHorsesEnd;
				ZeroMemory(&stHorsesEnd,sizeof(stHorsesEnd));
				stHorsesEnd.nTimeLeave = m_nFreeTime;
				stHorsesEnd.RecordRecord = m_GameRecords[m_GameRecords.GetCount() - 1];
				memcpy(stHorsesEnd.nWinCount, m_nWinCount, sizeof(stHorsesEnd.nWinCount));
				memcpy(stHorsesEnd.nMultiple, m_nMultiple, sizeof(stHorsesEnd.nMultiple));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_HORSES_END, &stHorsesEnd, sizeof(stHorsesEnd));	

				//��Ϸ���
				for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
				{
					IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
					if (pIServerUserItem==NULL) continue;

					//���û���
					memcpy(stHorsesEnd.lPlayerBet, m_lPlayerBet[wChairID], sizeof(stHorsesEnd.lPlayerBet));
					memcpy(stHorsesEnd.lPlayerWinning, m_lPlayerBetWin[wChairID], sizeof(stHorsesEnd.lPlayerWinning));

					m_pITableFrame->SendTableData(wChairID,SUB_S_HORSES_END,&stHorsesEnd,sizeof(stHorsesEnd));	
				}

				//������Ϸ
			m_pITableFrame->ConcludeGame(GS_FREE);

				//����״̬
			//	m_pITableFrame->SetGameStatus(GS_FREE);
			}
			return true;
		case IDI_HORSES_END:		//����ǿ�ƽ���
			{
				//��Ϸ���
				for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
				{
					IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
					if (pIServerUserItem==NULL) 
						continue;

					m_pITableFrame->SendTableData(wChairID,SUB_S_MANDATOY_END, NULL ,0);	
				}
			}
			return true;
	}
	return false;
}

//��Ϸ��Ϣ����
bool CTableFrameSink:: OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch(wSubCmdID)
	{
	case SUB_C_PLAYER_BET:
			return OnSubPlayBet(pData, wDataSize, pIServerUserItem);

	case SUB_C_ADMIN_COMMDN:
			return OnAdminControl(pData, wDataSize, pIServerUserItem);
	}
	return false;
}

//�����Ϣ
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if( bLookonUser == false && m_dwGameTime == 0)
	{
		//��������
		srand(GetTickCount());

		//�������
		MultipleControl();

		//��ȡʱ��
		m_dwGameTime = (DWORD)time(NULL);

		//������ʱ��
		m_pITableFrame->SetGameTimer(IDI_FREE, m_nFreeTime*1000, 1, NULL);

		//����״̬
		m_pITableFrame->SetGameStatus(GS_FREE);
	}
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	return true;
}

//��ע�¼�
bool CTableFrameSink::OnSubPlayBet(const void * pBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_C_PlayerBet));
	if (wDataSize!=sizeof(CMD_C_PlayerBet)) return false;

	//�û�Ч��
	if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

	//��Ϣ����
	CMD_C_PlayerBet * pPlayerBet = (CMD_C_PlayerBet *)pBuffer;

	//״̬��֤
	if ( m_pITableFrame->GetGameStatus() != GS_BET)
	{
		//������עʧ����Ϣ
		CMD_S_PlayerBetFail stPlayerBetFail;
		stPlayerBetFail.cbFailType = FAIL_TYPE_TIME_OVER;
		stPlayerBetFail.wChairID = pIServerUserItem->GetChairID();
		memcpy(stPlayerBetFail.lBetScore, pPlayerBet->lBetScore, sizeof(stPlayerBetFail.lBetScore));
		m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(),SUB_S_PLAYER_BET_FAIL,&stPlayerBetFail,sizeof(stPlayerBetFail));
		return true;
	}

	//�ж���ע�Ƿ�Ϸ�
	for ( int i = 0 ; i < AREA_ALL ; ++i)
	{
		if ( pPlayerBet->lBetScore[i] > GetPlayersMaxBet(pIServerUserItem->GetChairID(), i) )
		{
			//������עʧ����Ϣ
			CMD_S_PlayerBetFail stPlayerBetFail;
			stPlayerBetFail.cbFailType = FAIL_TYPE_OVERTOP;
			stPlayerBetFail.wChairID = pIServerUserItem->GetChairID();
			memcpy(stPlayerBetFail.lBetScore, pPlayerBet->lBetScore, sizeof(stPlayerBetFail.lBetScore));
			m_pITableFrame->SendTableData(pIServerUserItem->GetChairID(),SUB_S_PLAYER_BET_FAIL,&stPlayerBetFail,sizeof(stPlayerBetFail));
			return true;
		}
	}

	//��������֤
	if(pIServerUserItem->IsAndroidUser())
	{
		for ( int i = 0 ; i < AREA_ALL; ++i )
		{
			if ( m_lRobotAreaScore[i] + pPlayerBet->lBetScore[i] > m_lRobotAreaLimit )
				return true;
		}

		//��Ŀ����
		bool bHaveChip = false;
		for (int i = 0; i < AREA_ALL; i++)
		{
			if (m_lPlayerBet[pIServerUserItem->GetChairID()][i] != 0)
				bHaveChip = true;
		}

		if (!bHaveChip)
		{
			if ( m_nChipRobotCount + 1 > m_nMaxChipRobot )
				return true;
			else
				m_nChipRobotCount++;
		}
	}

	//�������
	LONGLONG lTempPlayerAllBet = 0l;
	for(int i = 0 ; i < AREA_ALL ; ++i)
		lTempPlayerAllBet += m_lPlayerBet[pIServerUserItem->GetChairID()][i];

	//��ҵ�һ����ע,���������
	if ( lTempPlayerAllBet == 0l )
		m_nBetPlayerCount++;

	//���ע
	for ( int i = 0 ; i < AREA_ALL ; ++i)
	{
		m_lPlayerBet[pIServerUserItem->GetChairID()][i] += pPlayerBet->lBetScore[i];
		m_lPlayerBetAll[i] += pPlayerBet->lBetScore[i];
	}

	//������Ϣ
	CMD_S_PlayerBet stPlayerBet;
	memcpy(stPlayerBet.lBetScore, pPlayerBet->lBetScore, sizeof(stPlayerBet.lBetScore));
	stPlayerBet.wChairID = pIServerUserItem->GetChairID();
	stPlayerBet.nBetPlayerCount = m_nBetPlayerCount;
	stPlayerBet.bIsAndroid=pIServerUserItem->IsAndroidUser();
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PLAYER_BET,&stPlayerBet,sizeof(stPlayerBet));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLAYER_BET,&stPlayerBet,sizeof(stPlayerBet));

	return true;
}

//����
bool CTableFrameSink::OnAdminControl(const void * pBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_C_ControlApplication));
	if(wDataSize!=sizeof(CMD_C_ControlApplication)) 
		return false;

	//�û�Ч��
	if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
		return false;

	//��Ϣ����
	CMD_C_ControlApplication * pControlApplication=(CMD_C_ControlApplication *)pBuffer;

	switch(pControlApplication->cbControlAppType)
	{
	case C_CA_UPDATE:	//����
		{
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_UPDATE_SUCCES;
			ControlReturns.cbControlTimes = m_cbCLTimes;
			ControlReturns.cbControlArea = m_cbCLArea;
			for (int i = 0 ; i < AREA_ALL; ++i)
				ControlReturns.nControlMultiple[i] = m_nCLMultiple[i];
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	case C_CA_SET:		//����
		{
			//���ñ���
			m_cbCLTimes = pControlApplication->cbControlTimes;
			m_cbCLArea = pControlApplication->cbControlArea;
			for (int i = 0 ; i < AREA_ALL; ++i)
				m_nCLMultiple[i] = pControlApplication->nControlMultiple[i];

			m_bControl = pControlApplication->bAuthoritiesExecuted == TRUE;

			//�������
			CMD_S_ControlReturns ControlReturns;

			//���ñ���
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_SET_SUCCESS;
			ControlReturns.cbControlTimes = pControlApplication->cbControlTimes;
			ControlReturns.cbControlArea = pControlApplication->cbControlArea;
			ControlReturns.bAuthoritiesExecuted = pControlApplication->cbControlArea;
			for (int i = 0 ; i < AREA_ALL; ++i)
				ControlReturns.nControlMultiple[i] = pControlApplication->nControlMultiple[i];

			//������Ϣ
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	case C_CA_CANCELS:	//ȡ��
		{
			//���ñ���
			m_cbCLTimes = 0;
			m_bControl = false;
			m_cbCLArea = 255;
			for (int i = 0 ; i < AREA_ALL; ++i)
				m_nCLMultiple[i] = -1;

			//�������
			CMD_S_ControlReturns ControlReturns;

			//���ñ���
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_CANCEL_SUCCESS;
			ControlReturns.cbControlArea = 255;
			ControlReturns.cbControlTimes = 0;
			for ( int i = 0; i < AREA_ALL; ++i)
				ControlReturns.nControlMultiple[i] = -1;

			//������Ϣ
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	}
	return false;

}

//�������
void CTableFrameSink::HorsesProcess()
{
	while(true)
	{
		HorsesStart();

		//�ȿ�����Ƿ���ϳ������������
		//�ٿ���������Ƿ���ȷ
		if( HorsesEnd() && CalculateScore() )
			break ;
	}

	//��¼
	m_nStreak++;
	CTime time = CTime::GetCurrentTime();					//��ȡ��ǰʱ��. 
	int nDay = time.GetDay();								//��
	if( m_nStreak == INT_MAX || m_nDay != nDay )
	{
		m_nStreak = 1;
		m_nDay = nDay;
		ZeroMemory(m_nWinCount,sizeof(m_nWinCount));
	}

	tagHistoryRecord HistoryRecord;
	HistoryRecord.nStreak = m_nStreak;
	HistoryRecord.nRanking = m_cbGameResults;
	HistoryRecord.nRiskCompensate = m_nMultiple[m_cbGameResults];
	HistoryRecord.nHours = time.GetHour();
	HistoryRecord.nMinutes = time.GetMinute();
	HistoryRecord.nSeconds = time.GetSecond();
	m_nWinCount[m_cbHorsesRanking[RANKING_FIRST]]++;

	m_GameRecords.Add(HistoryRecord);
	if ( m_GameRecords.GetCount() > MAX_SCORE_HISTORY )
		m_GameRecords.RemoveAt(0);
}


//����ʼ
void CTableFrameSink::HorsesStart()
{

	//��ƥ��Ϣ
	ZeroMemory(m_nHorsesSpeed,sizeof(m_nHorsesSpeed));

	//------------------------------------------------------
	//���ǵõ�ÿ����ٶ�.. ģ���ٶ�.. ��������С.
	for ( int nHorses = 0 ; nHorses < HORSES_ALL; ++nHorses )
	{
		for ( int i = 0 ; i < COMPLETION_TIME; ++i)
		{
			//���� 
			if ( i % 2 == 0 )
			{
				m_nHorsesSpeed[nHorses][i] = (rand() + GetTickCount() + nHorses) % HIGH_SPEED + 10;
			}
			//����
			else
			{
				m_nHorsesSpeed[nHorses][i] = (rand() + GetTickCount() + nHorses) % LOW_SPEED;
				m_nHorsesSpeed[nHorses][i] = -m_nHorsesSpeed[nHorses][i];
			}
		}
		m_nHorsesSpeed[nHorses][COMPLETION_TIME - 1] = m_nHorsesSpeed[nHorses][COMPLETION_TIME - 1] / 20 * 20;
	}

	//------------------------------------------------------
	//���ǵõ�ÿ30������ٶ�.. ��ȷ�ٶ�.. ������������̫��. ��������.. ����.
	//for ( int nHorses = 0 ; nHorses < HORSES_ALL; ++nHorses )
	//{
	//	//�ٲ�����
	//	int nSpeedIndex = 0;	
	//	//�ٶ�
	//	int nVelocity = 1;	
	//	//���ٶ�
	//	int nAcceleration = ACCELERATION;
	//	//����ά��
	//	int nMaintenance = 0;
	//	//Ƶ��
	//	int nFrequency = (rand() + GetTickCount()*2 ) % FREQUENCY + 1;
	//	//����ٶ�
	//	int nHighestSpeed = (rand() + GetTickCount() + nHorses) % HIGH_SPEED;
	//	//����ٶ�
	//	int nMinimumSpeed = (rand() + GetTickCount() + nHorses) % LOW_SPEED;
	//	nMinimumSpeed = -nMinimumSpeed;

	//	if ( nHighestSpeed == nMinimumSpeed )
	//		nHighestSpeed = nMinimumSpeed + rand()%HIGH_SPEED + 1;

	//	//����ѭ��
	//	bool bCycle = false;
	//	bool bHighSpeedAppeared = false;
	//	while( nSpeedIndex < STEP_SPEED )
	//	{
	//		m_nHorsesSpeed[nHorses][nSpeedIndex] = nVelocity;

	//		//���ü��ٶ�
	//		if ( nVelocity >= nHighestSpeed )
	//		{
	//			bHighSpeedAppeared = true;
	//			nAcceleration = ((rand() + GetTickCount())%ACCELERATION + 1);
	//			nAcceleration = -nAcceleration;
	//		}
	//		else if ( nVelocity <= nMinimumSpeed )
	//		{
	//			nAcceleration = ((rand() + GetTickCount())%ACCELERATION + 2);
	//		}

	//		//������һ���ٶ�
	//		if( nMaintenance < nFrequency )
	//		{
	//			nMaintenance++;
	//		}
	//		else
	//		{
	//			nFrequency = (rand() + GetTickCount()*2 ) % (FREQUENCY/2) + 1;
	//			nMaintenance = 0;
	//			nVelocity += nAcceleration;
	//		}

	//		//һ�������
	//		if ( nVelocity == nMinimumSpeed && bHighSpeedAppeared)
	//			bCycle = true;

	//		if ( bCycle )
	//		{
	//			bCycle = false;
	//			bHighSpeedAppeared = false;
	//			nAcceleration = 1;
	//			nMaintenance = 0;
	//			nFrequency = (rand() + GetTickCount()*2 ) % FREQUENCY + 1;
	//			nHighestSpeed = (rand() + GetTickCount() + nHorses) % HIGH_SPEED;
	//			nMinimumSpeed = (rand() + GetTickCount() + nHorses) % LOW_SPEED;
	//			nMinimumSpeed = -nMinimumSpeed;

	//			if ( nHighestSpeed == nMinimumSpeed )
	//				nHighestSpeed = nMinimumSpeed + rand()%HIGH_SPEED + 1;
	//		}

	//		nSpeedIndex++;
	//	}
	//}
}

//�������
bool CTableFrameSink::HorsesEnd()
{
	//���ٶȺ�
	INT nSpeedTotal[HORSES_ALL] = {0};

	for ( int nHorses = 0; nHorses < HORSES_ALL; ++nHorses )
	{
		nSpeedTotal[nHorses] += m_nHorsesSpeed[nHorses][COMPLETION_TIME - 1];
	}

	//�����ж�
	for ( int nHorsesX = 0; nHorsesX < HORSES_ALL; ++nHorsesX )
	{
		for (int nHorsesY = 0; nHorsesY < HORSES_ALL; ++nHorsesY)
		{
			//�������ȵľʹ���
			if ( nSpeedTotal[nHorsesX] == nSpeedTotal[nHorsesY] && nHorsesX != nHorsesY )
			{
				return false;
			}
		}
	}

	//�ҳ�����
	INT nFirst = HORSES_ALL;
	INT nSecond = HORSES_ALL;
	INT nThird = HORSES_ALL;
	INT nFourth = HORSES_ALL;
	INT nFifth = HORSES_ALL;
	INT nSixth = HORSES_ALL;
	
	//��һ��
	INT nTemp = 0;
	for ( int nHorses = 0; nHorses < HORSES_ALL; ++nHorses )
	{
		if ( nTemp == 0 || nTemp < nSpeedTotal[nHorses] )
		{
			nTemp = nSpeedTotal[nHorses];
			nFirst = nHorses;
		}
	}

	//�ڶ���
	nTemp = 0;
	for ( int nHorses = 0; nHorses < HORSES_ALL; ++nHorses )
	{
		if ( nHorses != nFirst && ( nTemp == 0 || nTemp < nSpeedTotal[nHorses] ))
		{
			nTemp = nSpeedTotal[nHorses];
			nSecond = nHorses;
		}
	}

	//������
	nTemp = 0;
	for ( int nHorses = 0; nHorses < HORSES_ALL; ++nHorses )
	{
		if ( nHorses != nFirst && nHorses != nSecond && ( nTemp == 0 || nTemp < nSpeedTotal[nHorses] ))
		{
			nTemp = nSpeedTotal[nHorses];
			nThird = nHorses;
		}
	}

	//������
	nTemp = 0;
	for ( int nHorses = 0; nHorses < HORSES_ALL; ++nHorses )
	{
		if ( nHorses != nFirst &&  nHorses != nSecond && nHorses != nThird && ( nTemp == 0 || nTemp < nSpeedTotal[nHorses] ))
		{
			nTemp = nSpeedTotal[nHorses];
			nFourth = nHorses;
		}
	}

	//������
	nTemp = 0;
	for ( int nHorses = 0; nHorses < HORSES_ALL; ++nHorses )
	{
		if ( nHorses != nFirst && nHorses != nSecond && nHorses != nThird && nHorses != nFourth && ( nTemp == 0 || nTemp < nSpeedTotal[nHorses] ))
		{
			nTemp = nSpeedTotal[nHorses];
			nFifth = nHorses;
		}
	}

	//������
	nTemp = 0;
	for ( int nHorses = 0; nHorses < HORSES_ALL; ++nHorses )
	{
		if ( nHorses != nFirst && nHorses != nSecond && nHorses != nThird && nHorses != nFourth && nHorses != nFifth && ( nTemp == 0 || nTemp < nSpeedTotal[nHorses] ))
		{
			nTemp = nSpeedTotal[nHorses];
			nSixth = nHorses;
		}
	}

	//�����ж�
	if( nFirst == HORSES_ALL || nSecond == HORSES_ALL || nThird == HORSES_ALL 
		|| nFourth == HORSES_ALL || nFifth == HORSES_ALL || nSixth == HORSES_ALL)
	{
		ASSERT(FALSE);
		return false;
	}

	//��ƥ��Ϣ
	for ( int i = 0; i < CountArray(m_cbHorsesRanking); ++i)
		m_cbHorsesRanking[i] = HORSES_ALL;

	m_cbHorsesRanking[RANKING_FIRST] = nFirst;
	m_cbHorsesRanking[RANKING_SECOND] = nSecond;
	m_cbHorsesRanking[RANKING_THIRD] = nThird;
	m_cbHorsesRanking[RANKING_FOURTH] = nFourth;
	m_cbHorsesRanking[RANKING_FIFTH] = nFifth;
	m_cbHorsesRanking[RANKING_SIXTH] = nSixth;

	//������Ƿ����
	return FinalResults();
}


//�����
bool CTableFrameSink::FinalResults()
{
	//��Ϸ���
	if ( (m_cbHorsesRanking[RANKING_FIRST] == HORSES_ONE && m_cbHorsesRanking[RANKING_SECOND] == HORSES_SIX)
		||(m_cbHorsesRanking[RANKING_SECOND] == HORSES_ONE&& m_cbHorsesRanking[RANKING_FIRST] == HORSES_SIX)  )
	{
		m_cbGameResults = AREA_1_6;
	}
	else if ( (m_cbHorsesRanking[RANKING_FIRST] == HORSES_ONE && m_cbHorsesRanking[RANKING_SECOND] == HORSES_FIVE)
		||(m_cbHorsesRanking[RANKING_SECOND] == HORSES_ONE && m_cbHorsesRanking[RANKING_FIRST] == HORSES_FIVE)  )
	{
		m_cbGameResults = AREA_1_5;
	}
	else if ( (m_cbHorsesRanking[RANKING_FIRST] == HORSES_ONE && m_cbHorsesRanking[RANKING_SECOND] == HORSES_FOUR)
		||(m_cbHorsesRanking[RANKING_SECOND] == HORSES_ONE && m_cbHorsesRanking[RANKING_FIRST] == HORSES_FOUR)  )
	{
		m_cbGameResults = AREA_1_4;
	}
	else if ( (m_cbHorsesRanking[RANKING_FIRST] == HORSES_ONE && m_cbHorsesRanking[RANKING_SECOND] == HORSES_THREE)
		||(m_cbHorsesRanking[RANKING_SECOND] == HORSES_ONE && m_cbHorsesRanking[RANKING_FIRST] == HORSES_THREE)  )
	{
		m_cbGameResults = AREA_1_3;
	}
	else if ( (m_cbHorsesRanking[RANKING_FIRST] == HORSES_ONE && m_cbHorsesRanking[RANKING_SECOND] == HORSES_TWO)
		||(m_cbHorsesRanking[RANKING_SECOND] == HORSES_ONE && m_cbHorsesRanking[RANKING_FIRST] == HORSES_TWO)  )
	{
		m_cbGameResults = AREA_1_2;
	}
	else if ( (m_cbHorsesRanking[RANKING_FIRST] == HORSES_TWO && m_cbHorsesRanking[RANKING_SECOND] == HORSES_SIX)
		||(m_cbHorsesRanking[RANKING_SECOND] == HORSES_TWO && m_cbHorsesRanking[RANKING_FIRST] == HORSES_SIX)  )
	{
		m_cbGameResults = AREA_2_6;
	}
	else if ( (m_cbHorsesRanking[RANKING_FIRST] == HORSES_TWO && m_cbHorsesRanking[RANKING_SECOND] == HORSES_FIVE)
		||(m_cbHorsesRanking[RANKING_SECOND] == HORSES_TWO && m_cbHorsesRanking[RANKING_FIRST] == HORSES_FIVE)  )
	{
		m_cbGameResults = AREA_2_5;
	}
	else if ( (m_cbHorsesRanking[RANKING_FIRST] == HORSES_TWO && m_cbHorsesRanking[RANKING_SECOND] == HORSES_FOUR)
		||(m_cbHorsesRanking[RANKING_SECOND] == HORSES_TWO && m_cbHorsesRanking[RANKING_FIRST] == HORSES_FOUR)  )
	{
		m_cbGameResults = AREA_2_4;
	}
	else if ( (m_cbHorsesRanking[RANKING_FIRST] == HORSES_TWO && m_cbHorsesRanking[RANKING_SECOND] == HORSES_THREE)
		||(m_cbHorsesRanking[RANKING_SECOND] == HORSES_TWO && m_cbHorsesRanking[RANKING_FIRST] == HORSES_THREE)  )	
	{
		m_cbGameResults = AREA_2_3;
	}
	else if ( (m_cbHorsesRanking[RANKING_FIRST] == HORSES_THREE && m_cbHorsesRanking[RANKING_SECOND] == HORSES_SIX)
		||(m_cbHorsesRanking[RANKING_SECOND] == HORSES_THREE && m_cbHorsesRanking[RANKING_FIRST] == HORSES_SIX)  )	
	{
		m_cbGameResults = AREA_3_6;
	}
	else if ( (m_cbHorsesRanking[RANKING_FIRST] == HORSES_THREE && m_cbHorsesRanking[RANKING_SECOND] == HORSES_FIVE)
		||(m_cbHorsesRanking[RANKING_SECOND] == HORSES_THREE && m_cbHorsesRanking[RANKING_FIRST] == HORSES_FIVE)  )	
	{
		m_cbGameResults = AREA_3_5;
	}
	else if ( (m_cbHorsesRanking[RANKING_FIRST] == HORSES_THREE && m_cbHorsesRanking[RANKING_SECOND] == HORSES_FOUR)
		||(m_cbHorsesRanking[RANKING_SECOND] == HORSES_THREE && m_cbHorsesRanking[RANKING_FIRST] == HORSES_FOUR)  )	
	{
		m_cbGameResults = AREA_3_4;
	}
	else if ( (m_cbHorsesRanking[RANKING_FIRST] == HORSES_FOUR && m_cbHorsesRanking[RANKING_SECOND] == HORSES_SIX)
		||(m_cbHorsesRanking[RANKING_SECOND] == HORSES_FOUR && m_cbHorsesRanking[RANKING_FIRST] == HORSES_SIX)  )	
	{
		m_cbGameResults = AREA_4_6;
	}
	else if ( (m_cbHorsesRanking[RANKING_FIRST] == HORSES_FOUR && m_cbHorsesRanking[RANKING_SECOND] == HORSES_FIVE)
		||(m_cbHorsesRanking[RANKING_SECOND] == HORSES_FOUR && m_cbHorsesRanking[RANKING_FIRST] == HORSES_FIVE)  )	
	{
		m_cbGameResults = AREA_4_5;
	}
	else if ( (m_cbHorsesRanking[RANKING_FIRST] == HORSES_FIVE && m_cbHorsesRanking[RANKING_SECOND] == HORSES_SIX)
		||(m_cbHorsesRanking[RANKING_SECOND] == HORSES_FIVE && m_cbHorsesRanking[RANKING_FIRST] == HORSES_SIX)  )	
	{
		m_cbGameResults = AREA_5_6;
	}
	else
	{
		ASSERT(FALSE);
		return false;
	}

	if (NeedControl())
	{
		return MeetControl();
	}

	return true;
}

//�Ƿ���Ҫ����
bool CTableFrameSink::NeedControl()
{
	if ( m_cbCLTimes > 0 && m_bControl )
	{
		return true;
	}
	return false;
}

//�Ƿ��������
bool CTableFrameSink::MeetControl()
{
	if ( m_cbCLArea != 255 && m_cbCLArea >= AREA_1_6 && m_cbCLArea < AREA_ALL && m_cbGameResults != m_cbCLArea )
	{
		return false;
	}

	return true;
}

//��ɿ���
bool CTableFrameSink::CompleteControl()
{
	m_cbCLTimes--;
	if ( m_cbCLTimes == 0 )
	{
		m_cbCLTimes = 0;
		m_bControl = false;
		m_cbCLArea = 255;
		for (int i = 0 ; i < AREA_ALL; ++i)
			m_nCLMultiple[i] = -1;
	}
	return true;
}

//�������
bool CTableFrameSink::CalculateScore()
{
	//�����Ӯ
	LONGLONG lPlayerLost[GAME_PLAYER];
	ZeroMemory(lPlayerLost, sizeof(lPlayerLost));
	ZeroMemory(m_lPlayerWinning,sizeof(m_lPlayerWinning));
	ZeroMemory(m_lPlayerReturnBet,sizeof(m_lPlayerReturnBet));
	ZeroMemory(m_lPlayerRevenue,sizeof(m_lPlayerRevenue));
	ZeroMemory(m_lPlayerBetWin,sizeof(m_lPlayerBetWin));
	

	//ϵͳ��Ӯ
	LONGLONG lSystemScore = 0l;

	//�������
	for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;

		bool bIsAndroidUser = pIServerUserItem->IsAndroidUser();

		//ÿ���˶�ע��
		for (WORD wAreaIndex = 0; wAreaIndex < AREA_ALL; ++wAreaIndex)
		{
			if ( m_cbGameResults == wAreaIndex )
			{
				m_lPlayerWinning[wChairID] += m_lPlayerBet[wChairID][wAreaIndex] * (m_nMultiple[wAreaIndex] - 1);
				m_lPlayerReturnBet[wChairID] += m_lPlayerBet[wChairID][wAreaIndex];
				m_lPlayerBetWin[wChairID][wAreaIndex] = m_lPlayerBet[wChairID][wAreaIndex] * m_nMultiple[wAreaIndex];

				//ϵͳ�÷�
				if(!bIsAndroidUser)
					lSystemScore -= (m_lPlayerBet[wChairID][wAreaIndex] * (m_nMultiple[wAreaIndex] - 1));
			}
			else
			{
				lPlayerLost[wChairID] -= m_lPlayerBet[wChairID][wAreaIndex];
				m_lPlayerBetWin[wChairID][wAreaIndex] = -m_lPlayerBet[wChairID][wAreaIndex];

				//ϵͳ�÷�
				if(!bIsAndroidUser)
					lSystemScore += m_lPlayerBet[wChairID][wAreaIndex];
			}
		}

		//�ܵķ���
		m_lPlayerWinning[wChairID] += lPlayerLost[wChairID];

		//����˰��
		if (0 < m_lPlayerWinning[wChairID])
		{
			DOUBLE fRevenuePer = DOUBLE( (DOUBLE)m_pGameServiceOption->wRevenueRatio / (DOUBLE)1000.000000 );
			m_lPlayerRevenue[wChairID]  = LONGLONG(m_lPlayerWinning[wChairID] * fRevenuePer);
			m_lPlayerWinning[wChairID] -= m_lPlayerRevenue[wChairID];
		}
	}

	if ( NeedControl() )
	{
		return CompleteControl();
	}


	//ϵͳ��ֵ����
	if ((lSystemScore + m_StorageStart) < 0l)
	{
		return false;
	}
	else
	{
		m_StorageStart += lSystemScore;
		m_StorageStart = m_StorageStart - (m_StorageStart * m_StorageDeduct / 1000);
		return true;
	}
}

//�������·�
LONGLONG CTableFrameSink::GetPlayersMaxBet( WORD wChiarID, BYTE cbArea /*= AREA_ALL*/)
{
	//�ҳ����
	IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wChiarID);
	if ( pIServerUserItem == NULL || cbArea > AREA_ALL ) 
		return 0l;

	//��ҷ���
	LONGLONG lUserScore = pIServerUserItem->GetUserScore();

	//��ȥ�Լ�����ע
	for(int i = 0; i < AREA_ALL; ++i)
	{
		lUserScore -= m_lPlayerBet[wChiarID][i];
	}

	//�����������ע. ��Ҫ�ж���������
	if ( cbArea < AREA_ALL )
	{
		//������������
		LONGLONG lUserLimit = m_lUserLimitScore - m_lPlayerBet[wChiarID][cbArea];
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

//��������
void CTableFrameSink::MultipleControl()
{
	//��������
	//----------------------------
	INT nControl = GetPrivateProfileInt(m_szGameRoomName, TEXT("MultipleControl"), 0, m_szConfigFileName);
	m_bMultipleControl == nControl == 1 ? TRUE : FALSE;

	//��Ҫ����
	if(m_bMultipleControl)
	{
		m_nMultiple[AREA_1_6] = GetPrivateProfileInt(m_szGameRoomName, TEXT("AREA_1_6"), 1, m_szConfigFileName);
		m_nMultiple[AREA_1_5] = GetPrivateProfileInt(m_szGameRoomName, TEXT("AREA_1_5"), 1, m_szConfigFileName);
		m_nMultiple[AREA_1_4] = GetPrivateProfileInt(m_szGameRoomName, TEXT("AREA_1_4"), 1, m_szConfigFileName);
		m_nMultiple[AREA_1_3] = GetPrivateProfileInt(m_szGameRoomName, TEXT("AREA_1_3"), 1, m_szConfigFileName);
		m_nMultiple[AREA_1_2] = GetPrivateProfileInt(m_szGameRoomName, TEXT("AREA_1_2"), 1, m_szConfigFileName);
		m_nMultiple[AREA_2_6] = GetPrivateProfileInt(m_szGameRoomName, TEXT("AREA_2_6"), 1, m_szConfigFileName);
		m_nMultiple[AREA_2_5] = GetPrivateProfileInt(m_szGameRoomName, TEXT("AREA_2_5"), 1, m_szConfigFileName);
		m_nMultiple[AREA_2_4] = GetPrivateProfileInt(m_szGameRoomName, TEXT("AREA_2_4"), 1, m_szConfigFileName);
		m_nMultiple[AREA_2_3] = GetPrivateProfileInt(m_szGameRoomName, TEXT("AREA_2_3"), 1, m_szConfigFileName);
		m_nMultiple[AREA_3_6] = GetPrivateProfileInt(m_szGameRoomName, TEXT("AREA_3_6"), 1, m_szConfigFileName);
		m_nMultiple[AREA_3_5] = GetPrivateProfileInt(m_szGameRoomName, TEXT("AREA_3_5"), 1, m_szConfigFileName);
		m_nMultiple[AREA_3_4] = GetPrivateProfileInt(m_szGameRoomName, TEXT("AREA_3_4"), 1, m_szConfigFileName);
		m_nMultiple[AREA_4_6] = GetPrivateProfileInt(m_szGameRoomName, TEXT("AREA_4_6"), 1, m_szConfigFileName);
		m_nMultiple[AREA_4_5] = GetPrivateProfileInt(m_szGameRoomName, TEXT("AREA_4_5"), 1, m_szConfigFileName);
		m_nMultiple[AREA_5_6] = GetPrivateProfileInt(m_szGameRoomName, TEXT("AREA_5_6"), 1, m_szConfigFileName);

		for ( int i = 0;  i < AREA_ALL; ++i)
		{
			if ( m_nMultiple[i] < 1 )
			{
				m_nMultiple[i] = 1;
			}
		}
	}
	else
	{
		//���������
		RandomMultiples();
	}

	//����Ա����
	if ( NeedControl() )
	{
		for ( int i = 0; i < AREA_ALL; ++i)
		{
			if ( m_nCLMultiple[i] > 0 )
			{
				m_nMultiple[i] = m_nCLMultiple[i];
			}
		}
	}
}

//�����ñ���
void CTableFrameSink::RandomMultiples()
{
	DWORD wTick = GetTickCount();
	INT nMultiples[]	=					{ 3, 4, 5, 6, 7, 8, 9, 10, 18, 20, 25, 28, 30, 35, 38, 40, 45 };//����
	INT nChance[CountArray(nMultiples)] =	{ 6, 6, 6, 6, 6, 6, 6, 4,  4,  3,  3,  3,  2,  2,  2,  1,  1 };	//����
	INT nBigAreaMultiples[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };							//�󱶼���

	//����
	ChaosArray(nBigAreaMultiples, CountArray(nBigAreaMultiples));
	ChaosArray(nMultiples, CountArray(nMultiples), nChance, CountArray(nChance));

	//����
	INT nBigArea = nBigAreaMultiples[(rand() + wTick)%CountArray(nBigAreaMultiples)];
	m_nMultiple[nBigArea] = 88;

	//�������
	for ( int i = 0; i < AREA_ALL; ++i )
	{
		if ( i == nBigArea )
			continue;

		//���ʺ�ֵ
		INT nChanceAndValue = 0;
		for ( int n = 0; n < CountArray(nChance); ++n )
			nChanceAndValue += nChance[n];

		INT nMuIndex = 0;
		int nRandNum = 0;					//�������
		static int nStFluc = 1;				
		nRandNum = (rand() + wTick + nStFluc*3 + i) % nChanceAndValue;
		for (int j = 0; j < CountArray(nChance); j++)
		{
			nRandNum -= nChance[j];
			if (nRandNum < 0)
			{
				nMuIndex = j;
				break;
			}
		}
		nStFluc = nStFluc%3 + 1;

		m_nMultiple[i] = nMultiples[nMuIndex];
		nChance[nMuIndex] = 0;
	}
}

//��������
void CTableFrameSink::ChaosArray( INT nArray[], INT nCount )
{
	DWORD wTick = GetTickCount();
	for (int i = 0; i < nCount; ++i)
	{
		int nTempIndex = (rand()+wTick)%nCount;
		int nTempValue = nArray[i];
		nArray[i] = nArray[nTempIndex];
		nArray[nTempIndex] = nTempValue;
	}
}

//��������
void CTableFrameSink::ChaosArray( INT nArrayOne[], INT nCountOne, INT nArrayTwo[], INT nCountTwo )
{
	ASSERT( nCountOne == nCountTwo );
	if( nCountTwo != nCountOne )
		return;

	DWORD wTick = GetTickCount();
	for (int i = 1; i < nCountOne; ++i)
	{
		int nTempIndex = (rand()+wTick)%nCountOne;

		int nTempValueOne = nArrayOne[i];
		nArrayOne[i] = nArrayOne[nTempIndex];
		nArrayOne[nTempIndex] = nTempValueOne;

		int nTempValueTwo = nArrayTwo[i];
		nArrayTwo[i] = nArrayTwo[nTempIndex];
		nArrayTwo[nTempIndex] = nTempValueTwo;
	}
}

//////////////////////////////////////////////////////////////////////////

//��ѯ�Ƿ�۷����
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{

	return true;
}
//////////////////////////////////////////////////////////////////////////