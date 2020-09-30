#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_PLACE_JETTON1			103									//��ע��ʱ
#define IDI_PLACE_JETTON2			104									//��ע��ʱ
#define IDI_PLACE_JETTON3			105									//��ע��ʱ
#define IDI_PLACE_JETTON4			106									//��ע��ʱ
#define IDI_PLACE_JETTON5			107									//��ע��ʱ
#define IDI_CHECK_BANKER			108									//�����ׯ
#define IDI_REQUEST_BANKER			101									//���붨ʱ
#define IDI_GIVEUP_BANKER			102									//��ׯ��ʱ
#define IDI_PLACE_JETTON			110									//��ע���� (Ԥ��110-160)

//////////////////////////////////////////////////////////////////////////


//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//��Ϸ����
	m_lMaxChipUser = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_cbTimeLeave = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));

	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
VOID *  CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool  CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//���ýӿ�
bool  CAndroidUserItemSink::RepositionSink()
{
	//��Ϸ����
	m_lMaxChipUser = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_cbTimeLeave = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));

	return true;
}

//ʱ����Ϣ
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	if (nTimerID >= IDI_PLACE_JETTON && nTimerID <= IDI_PLACE_JETTON+MAX_CHIP_TIME)
	{
		srand(GetTickCount());

		//��������
		int nRandNum = 0, nChipArea = 0, nCurChip = 0, nACTotal = 0, nCurJetLmt[2] = {};
		LONGLONG lMaxChipLmt = m_lMaxChipUser;			//������עֵ
		WORD wMyID = m_pIAndroidUserItem->GetChairID();
		for (int i = 0; i < AREA_ALL; i++)
			nACTotal += m_RobotInfo.nAreaChance[i];

		//ͳ�ƴ���
		m_nChipTimeCount++;

		//����˳�
		if (lMaxChipLmt < m_RobotInfo.nChip[m_nChipLimit[0]])	return false;
		for (int i = 0; i < AREA_ALL; i++)
		{
			if (m_lAreaChip[i] >= m_RobotInfo.nChip[m_nChipLimit[0]])	break;
			if (i == AREA_ALL - 1)	return false;
		}

		//��ע����
		ASSERT(nACTotal>0);
		static int nStFluc = 1;				//�������
		if (nACTotal <= 0)	return false;
		do {
			nRandNum = (rand()+wMyID+nStFluc*3) % nACTotal;
			for (int i = 0; i < AREA_ALL; i++)
			{
				nRandNum -= m_RobotInfo.nAreaChance[i];
				if (nRandNum < 0)
				{
					nChipArea = i;
					break;
				}
			}
		}
		while (m_lAreaChip[nChipArea] < m_RobotInfo.nChip[m_nChipLimit[0]]);
		nStFluc = nStFluc%3 + 1;

		//��ע��С
		if (m_nChipLimit[0] == m_nChipLimit[1])
			nCurChip = m_nChipLimit[0];
		else
		{
			//���ñ���
			lMaxChipLmt = __min(lMaxChipLmt, m_lAreaChip[nChipArea]);
			nCurJetLmt[0] = m_nChipLimit[0];
			nCurJetLmt[1] = m_nChipLimit[0];

			//���㵱ǰ������
			for (int i = m_nChipLimit[1]; i > m_nChipLimit[0]; i--)
			{
				if (lMaxChipLmt > m_RobotInfo.nChip[i]) 
				{
					nCurJetLmt[1] = i;
					break;
				}
			}
	
			//�����ע
			nRandNum = (rand()+wMyID) % (nCurJetLmt[1]-nCurJetLmt[0]+1);
			nCurChip = nCurJetLmt[0] + nRandNum;

			//���¿��� (��ׯ�ҽ�ҽ���ʱ�ᾡ����֤�������)
			if (m_nChipTimeCount < m_nChipTime)
			{
				LONGLONG lLeftJetton = LONGLONG( (lMaxChipLmt-m_RobotInfo.nChip[nCurChip])/(m_nChipTime-m_nChipTimeCount) );

				//�������� (��ȫ����С���Ƴ�����עҲ����)
				if (lLeftJetton < m_RobotInfo.nChip[m_nChipLimit[0]] && nCurChip > m_nChipLimit[0])
					nCurChip--;
			}
		}

		LONGLONG Gold = m_pIAndroidUserItem->GetMeUserItem()->GetUserScore();		

		int nMaxIndex=0;			

		for (int i=0; i < CountArray(m_RobotInfo.nChip); i++)
			if (Gold>m_RobotInfo.nChip[i]) nMaxIndex=i;//

		nCurChip=rand()%(nMaxIndex+1);
		//��������
		CMD_C_PlayerBet stPlayerBet;
		ZeroMemory(&stPlayerBet, sizeof(stPlayerBet));
		stPlayerBet.lBetScore[nChipArea] = m_RobotInfo.nChip[nCurChip];

		//������Ϣ
		m_pIAndroidUserItem->SendSocketData(SUB_C_PLAYER_BET, &stPlayerBet, sizeof(stPlayerBet));
	}

	m_pIAndroidUserItem->KillGameTimer(nTimerID);
	return false;
}

//��Ϸ��Ϣ
bool  CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	switch(wSubCmdID)
	{
	case SUB_S_BET_START:			//��ʼ��ע
		return OnSubBetStart(pBuffer, wDataSize);

	case SUB_S_BET_END:				//��ע����
		return true;

	case SUB_S_HORSES_START:		//����ʼ
		return true;

	case SUB_S_HORSES_END:			//�������
		return true;

	case SUB_S_PLAYER_BET:			//�û���ע
		return OnSubPlayerBet(pBuffer, wDataSize);

	case SUB_S_PLAYER_BET_FAIL:		//��עʧ��
		return true;

	case SUB_S_NAMED_HORSES:		//��ƨ����
		return true;

	case SUB_S_MANDATOY_END:		//ǿ�ƽ���
		return true;

	case SUB_S_ADMIN_COMMDN:		//����
		return true;

	}
	//�������
	ASSERT(FALSE);
	return true;
}

//��Ϸ��Ϣ
bool  CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool  CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_SceneFree));
			if (wDataSize!=sizeof(CMD_S_SceneFree)) return false;

			//��Ϣ����
			CMD_S_SceneFree * pSceneFree=(CMD_S_SceneFree *)pData;

			m_lUserLimitScore = pSceneFree->lUserLimitScore;
			m_lAreaLimitScore = pSceneFree->lAreaLimitScore;

			memcpy(m_szRoomName, pSceneFree->szGameRoomName, sizeof(m_szRoomName));
			ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, true);
			return true;
		}
	case GS_BET:	
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_SceneBet));
			if (wDataSize!=sizeof(CMD_S_SceneBet)) return false;

			//��Ϣ����
			CMD_S_SceneBet * pSceneBet=(CMD_S_SceneBet *)pData;

			m_lUserLimitScore = pSceneBet->lUserLimitScore;
			m_lAreaLimitScore = pSceneBet->lAreaLimitScore;
			memcpy(m_szRoomName, pSceneBet->szGameRoomName, sizeof(m_szRoomName));

			ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, true);
			return true;
		}
	case GS_BET_END:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_SceneBetEnd));
			if (wDataSize!=sizeof(CMD_S_SceneBetEnd)) return false;

			//��Ϣ����
			CMD_S_SceneBetEnd * pSceneBetEnd=(CMD_S_SceneBetEnd *)pData;

			m_lUserLimitScore = pSceneBetEnd->lUserLimitScore;
			m_lAreaLimitScore = pSceneBetEnd->lAreaLimitScore;
			memcpy(m_szRoomName, pSceneBetEnd->szGameRoomName, sizeof(m_szRoomName));

			ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, true);
			return true;
		}
	case GS_HORSES:	
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_SceneHorses));
			if (wDataSize!=sizeof(CMD_S_SceneHorses)) return false;

			//��Ϣ����
			CMD_S_SceneHorses * pSceneHorses=(CMD_S_SceneHorses *)pData;

			m_lUserLimitScore = pSceneHorses->lUserLimitScore;
			m_lAreaLimitScore = pSceneHorses->lAreaLimitScore;
			memcpy(m_szRoomName, pSceneHorses->szGameRoomName, sizeof(m_szRoomName));

			ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, true);
			return true;
		}
	}

	return true;
}

//�û�����
VOID  CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
VOID  CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
VOID  CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
VOID  CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}


//��ʼ��ע
bool CAndroidUserItemSink::OnSubBetStart(const void * pBuffer, WORD wDataSize)
{	
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_BetStart));
	if (wDataSize!=sizeof(CMD_S_BetStart)) return false;

	//��Ϣ����
	CMD_S_BetStart * pGameStart=(CMD_S_BetStart *)pBuffer;

	srand(GetTickCount());

	//�Լ���ׯ������ע������
	if ( pGameStart->nChipRobotCount <= 0 )
		return true;

	//���ñ���
	m_lMaxChipUser = __min(m_lUserLimitScore, m_pIAndroidUserItem->GetMeUserItem()->GetUserScore());
	m_nChipTimeCount = 0;
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	for (int i = 0; i < AREA_ALL; i++)
		m_lAreaChip[i] = m_lAreaLimitScore;

	//������ע����
	int nElapse = 0;												
	WORD wMyID = m_pIAndroidUserItem->GetChairID();

	if (m_nRobotBetTimeLimit[0] == m_nRobotBetTimeLimit[1])
		m_nChipTime = m_nRobotBetTimeLimit[0];
	else
		m_nChipTime = (rand()+wMyID)%(m_nRobotBetTimeLimit[1]-m_nRobotBetTimeLimit[0]+1) + m_nRobotBetTimeLimit[0];
	ASSERT(m_nChipTime>=0);		
	if (m_nChipTime <= 0)	return false;								//��ȷ,2����������
	if (m_nChipTime > MAX_CHIP_TIME)	m_nChipTime = MAX_CHIP_TIME;	//�޶�MAX_CHIP����ע

	//���㷶Χ
	if (!CalcJettonRange(m_lMaxChipUser, m_lRobotJettonLimit, m_nChipTime, m_nChipLimit))
		return true;

	//����ʱ��
	int nTimeGrid = int(pGameStart->nTimeLeave-2)*800/m_nChipTime;		//ʱ���,ǰ2�벻��ע,����-2,800��ʾ��������עʱ�䷶Χǧ�ֱ�
	for (int i = 0; i < m_nChipTime; i++)
	{
		int nRandRage = int( nTimeGrid * i / (1500*sqrt((double)m_nChipTime)) ) + 1;		//������Χ
		nElapse = 2 + (nTimeGrid*i)/1000 + ( (rand()+wMyID)%(nRandRage*2) - (nRandRage-1) );
		ASSERT(nElapse>=2&&nElapse<=pGameStart->nTimeLeave);
		if (nElapse < 2 || nElapse > pGameStart->nTimeLeave)	continue;
		
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON+i+1, nElapse);
	}
	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubPlayerBet(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlayerBet));
	if (wDataSize!=sizeof(CMD_S_PlayerBet)) return false;

	//��Ϣ����
	CMD_S_PlayerBet * pPlayerBet=(CMD_S_PlayerBet *)pBuffer;

	//���ñ���
	for(int i = 0; i < AREA_ALL; ++i)
	{
		m_lAreaChip[i] -= pPlayerBet->lBetScore[i];
	}
	if (pPlayerBet->wChairID == m_pIAndroidUserItem->GetChairID())
	{
		for(int i = 0; i < AREA_ALL; ++i)
		{
			m_lMaxChipUser -= pPlayerBet->lBetScore[i];
		}
	}

	return true;
}


//��ȡ����
void CAndroidUserItemSink::ReadConfigInformation(TCHAR szFileName[], TCHAR szRoomName[], bool bReadFresh)
{
	//�����ļ���
	TCHAR szPath[MAX_PATH] = TEXT("");
	TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	TCHAR OutBuf[255] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	_sntprintf(szConfigFileName, sizeof(szConfigFileName), _T("%s\\%s"), szPath, szFileName);

	//��������
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotMaxJetton"), _T("100000"), OutBuf, 255, szConfigFileName);
	_sntscanf(OutBuf, _tcslen(OutBuf), _T("%I64d"), &m_lRobotJettonLimit[1]);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotMinJetton"), _T("100"), OutBuf, 255, szConfigFileName);
	_sntscanf(OutBuf, _tcslen(OutBuf), _T("%I64d"), &m_lRobotJettonLimit[0]);

	if (m_lRobotJettonLimit[1] > 5000000)					m_lRobotJettonLimit[1] = 5000000;
	if (m_lRobotJettonLimit[0] < 1000)						m_lRobotJettonLimit[0] = 100;
	if (m_lRobotJettonLimit[1] < m_lRobotJettonLimit[0])	m_lRobotJettonLimit[1] = m_lRobotJettonLimit[0];

	//��������
	m_nRobotBetTimeLimit[0] = GetPrivateProfileInt(szRoomName, _T("RobotMinBetTime"), 4, szConfigFileName);;
	m_nRobotBetTimeLimit[1] = GetPrivateProfileInt(szRoomName, _T("RobotMaxBetTime"), 8, szConfigFileName);;

	if (m_nRobotBetTimeLimit[0] < 0)							m_nRobotBetTimeLimit[0] = 0;
	if (m_nRobotBetTimeLimit[1] < m_nRobotBetTimeLimit[0])		m_nRobotBetTimeLimit[1] = m_nRobotBetTimeLimit[0];
}

//���㷶Χ	(����ֵ��ʾ�Ƿ����ͨ���½����޴ﵽ��ע)
bool CAndroidUserItemSink::CalcJettonRange(LONGLONG lMaxScore, LONGLONG lChipLmt[], int & nChipTime, int lJetLmt[])
{
	//�������
	bool bHaveSetMinChip = false;

	//����һע
	if (lMaxScore < m_RobotInfo.nChip[0])	return false;

	//���÷�Χ
	for (int i = 0; i < CountArray(m_RobotInfo.nChip); i++)
	{
		if (!bHaveSetMinChip && m_RobotInfo.nChip[i] >= lChipLmt[0])
		{ 
			lJetLmt[0] = i;
			bHaveSetMinChip = true;
		}
		if (m_RobotInfo.nChip[i] <= lChipLmt[1])
			lJetLmt[1] = i;
	}
	if (lJetLmt[0] > lJetLmt[1])	lJetLmt[0] = lJetLmt[1];

	return true;
}

//�����������
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
