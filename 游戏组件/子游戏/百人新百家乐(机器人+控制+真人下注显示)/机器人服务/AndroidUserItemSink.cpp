#include "Stdafx.h"
#include "AndroidUserItemSink.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_BANK_OPERATE			3									//���ж�ʱ
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

int CAndroidUserItemSink::m_stlApplyBanker = 0L;
int CAndroidUserItemSink::m_stnApplyCount = 0L;

int g_Count = 0;
int g_CountOne = 0;

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//��Ϸ����
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_lAndroidBet = 0;
	m_lBankerScore = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_cbTimeLeave = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	ZeroMemory(m_lAllBet, sizeof(m_lAllBet));

	//��ׯ����
	m_bMeApplyBanker = false;
	m_nWaitBanker=0;
	m_nBankerCount=0;
	m_nListUserCount= 0;
	
	// ��ȡ������ʼ��
	m_lRobotBetLimit[1] = 10000000;
	m_lRobotBetLimit[0] = 100;
	if (m_lRobotBetLimit[1] > 10000000)					m_lRobotBetLimit[1] = 10000000;
	if (m_lRobotBetLimit[0] < 100)						m_lRobotBetLimit[0] = 100;
	if (m_lRobotBetLimit[1] < m_lRobotBetLimit[0])	m_lRobotBetLimit[1] = m_lRobotBetLimit[0];

	//��������
	m_nRobotBetTimeLimit[0] = 4;
	m_nRobotBetTimeLimit[1] = 8;

	if (m_nRobotBetTimeLimit[0] < 0)							m_nRobotBetTimeLimit[0] = 0;
	if (m_nRobotBetTimeLimit[1] < m_nRobotBetTimeLimit[0])		m_nRobotBetTimeLimit[1] = m_nRobotBetTimeLimit[0];

	//�Ƿ���ׯ
	m_bRobotBanker = 0;

	//��ׯ����
	m_nRobotBankerCount = 3;
	m_nMinBankerTimes=2;
	m_nMaxBankerTimes=5;

	//��������
	m_nRobotWaitBanker = 3;

	//��ׯ����
	m_nRobotListMaxCount = 5;
	m_nRobotListMinCount = 2;

	//��������
	m_bReduceBetLimit = 0;

	//�������
	m_RobotInfo.nAreaChance[0] = 5;
	m_RobotInfo.nAreaChance[1] = 0;
	m_RobotInfo.nAreaChance[2] = 5;
	m_RobotInfo.nAreaChance[3] = 1;
	m_RobotInfo.nAreaChance[4] = 1;
	m_RobotInfo.nAreaChance[5] = 0;
	m_RobotInfo.nAreaChance[6] = 0;
	m_RobotInfo.nAreaChance[7] = 0;

	srand((unsigned)time(NULL));  

	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//���ýӿ�
bool CAndroidUserItemSink::RepositionSink()
{
	//��Ϸ����
	m_lMaxChipBanker = 0;
	m_lMaxChipUser = 0;
	m_lAndroidBet = 0;
	m_lBankerScore = 0;
	m_wCurrentBanker = 0;
	m_nChipTime = 0;
	m_nChipTimeCount = 0;
	m_cbTimeLeave = 0;
	ZeroMemory(m_lAreaChip, sizeof(m_lAreaChip));
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	ZeroMemory(m_lAllBet,sizeof(m_lAllBet));

	//��ׯ����
	m_bMeApplyBanker = false;
	m_nWaitBanker=0;
	m_nBankerCount=0;

	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_CHECK_BANKER:		//�����ׯ
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
				return false;

			int nMinCount = m_nRobotListMaxCount;
			if ( m_nRobotListMaxCount > m_nRobotListMinCount)
				nMinCount = (rand()%(m_nRobotListMaxCount - m_nRobotListMinCount)) + m_nRobotListMinCount;

			//��ׯ
			m_nWaitBanker++;

			//��������ׯ
			if ( m_bRobotBanker 
				&& !m_bMeApplyBanker 
				&& m_nWaitBanker >= m_nRobotWaitBanker 
				&& m_nListUserCount < m_nRobotListMaxCount 
				&& m_stlApplyBanker < m_nRobotListMaxCount 
				&& m_stlApplyBanker < nMinCount 
				&& m_stnApplyCount < 2 )
			{
				m_nWaitBanker = 0;

				//�Ϸ��ж�
				IServerUserItem *pIUserItemBanker = m_pIAndroidUserItem->GetMeUserItem();
				if (pIUserItemBanker->GetUserScore() > m_lBankerCondition ) 
				{
					//��������ׯ
					m_nBankerCount = 0;
					m_stlApplyBanker++;
					m_stnApplyCount++;
					m_pIAndroidUserItem->SetGameTimer(IDI_REQUEST_BANKER, (rand() % m_cbTimeLeave) + 1);
				}
			}


			return false;
		}
	case IDI_REQUEST_BANKER:	//������ׯ
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			m_pIAndroidUserItem->SendSocketData(SUB_C_APPLY_BANKER);

			return false;
		}
	case IDI_GIVEUP_BANKER:		//������ׯ
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			m_pIAndroidUserItem->SendSocketData(SUB_C_CANCEL_BANKER);

			return false;
		}
	case IDI_BANK_OPERATE:		//���в���
		{
			m_pIAndroidUserItem->KillGameTimer(nTimerID);

			//��������
			IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
			LONGLONG lRobotScore = pUserItem->GetUserScore();			
			{

				//�жϴ�ȡ
				if (lRobotScore > m_lRobotScoreRange[1])
				{
					LONGLONG lSaveScore=0L;

					lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);
					if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

					if (lSaveScore > 0 && m_wCurrentBanker != m_pIAndroidUserItem->GetChairID())
						m_pIAndroidUserItem->PerformSaveScore(lSaveScore);

				}
				else if (lRobotScore < m_lRobotScoreRange[0])
				{
					SCORE lScore=rand()%m_lRobotBankGetScoreBanker+m_lRobotBankGetScore;
					if (lScore > 0)
						m_pIAndroidUserItem->PerformTakeScore(lScore);								
				}
			}
			return false;
		}
	default:
		{
			if (nTimerID >= IDI_PLACE_JETTON && nTimerID <= IDI_PLACE_JETTON+MAX_CHIP_TIME)
			{
				////��������
				//int nRandNum = 0, nChipArea = 0, nCurChip = 0, nACTotal = 0, nCurJetLmt[2] = {};
				//LONGLONG lMaxChipLmt = __min(m_lMaxChipBanker, m_lMaxChipUser);			//������עֵ
				//WORD wMyID = m_pIAndroidUserItem->GetChairID();
				//for (int i = 0; i < AREA_COUNT; i++)
				//	nACTotal += m_RobotInfo.nAreaChance[i];

				////ͳ�ƴ���
				//m_nChipTimeCount++;

				////����˳�
				//if (lMaxChipLmt < m_RobotInfo.nChip[m_nChipLimit[0]])	return false;
				//for (int i = 0; i < AREA_COUNT; i++)
				//{
				//	if ((max(m_lAreaChip[i], GetMaxPlayerScore(i))) >= m_RobotInfo.nChip[m_nChipLimit[0]])	break;
				//	if (i == AREA_COUNT-1)	return false;
				//}

				////��ע����
				//ASSERT(nACTotal>0);
				//static int nStFluc = 1;				//�������
				//if (nACTotal <= 0)	return false;
				//do {
				//	nRandNum = (rand()+wMyID+nStFluc*3) % nACTotal;
				//	for (int i = 0; i < AREA_COUNT; i++)
				//	{
				//		nRandNum -= m_RobotInfo.nAreaChance[i];
				//		if (nRandNum < 0)
				//		{
				//			nChipArea = i;
				//			break;
				//		}
				//	}
				//}
				//while ((max(m_lAreaChip[nChipArea], GetMaxPlayerScore(nChipArea))) < m_RobotInfo.nChip[m_nChipLimit[0]]);
				//nStFluc = nStFluc%3 + 1;

				////��ע��С
				//if (m_nChipLimit[0] == m_nChipLimit[1])
				//	nCurChip = m_nChipLimit[0];
				//else
				//{
				//	//���ñ���
				//	lMaxChipLmt = __min(lMaxChipLmt, (max(m_lAreaChip[nChipArea], GetMaxPlayerScore(nChipArea))));
				//	nCurJetLmt[0] = m_nChipLimit[0];
				//	nCurJetLmt[1] = m_nChipLimit[0];

				//	//���㵱ǰ������
				//	for (int i = m_nChipLimit[1]; i > m_nChipLimit[0]; i--)
				//	{
				//		if (lMaxChipLmt > m_RobotInfo.nChip[i]) 
				//		{
				//			nCurJetLmt[1] = i;
				//			break;
				//		}
				//	}

				//	//�����ע
				//	nRandNum = (rand()+wMyID) % (nCurJetLmt[1]-nCurJetLmt[0]+1);
				//	nCurChip = nCurJetLmt[0] + nRandNum;

				//	//���¿��� (��ׯ�ҽ�ҽ���ʱ�ᾡ����֤�������)
				//	if (m_nChipTimeCount < m_nChipTime)
				//	{
				//		LONGLONG lLeftBet = LONGLONG( (lMaxChipLmt-m_RobotInfo.nChip[nCurChip])/(m_nChipTime-m_nChipTimeCount) );

				//		//�������� (��ȫ����С���Ƴ�����עҲ����)
				//		if (lLeftBet < m_RobotInfo.nChip[m_nChipLimit[0]] && nCurChip > m_nChipLimit[0])
				//			nCurChip--;
				//	}
				//}

				///*ASSERT( MyDebug(TEXT("��������ע %d ��ע���� [%d/%d] ��ע [%d %d] ��Χ [%d %d] ���� [%I64d %I64d %I64d]"), wMyID, nTimerID-IDI_PLACE_JETTON, m_nChipTime, 
				//	nChipArea, m_RobotInfo.nChip[nCurChip], m_nChipLimit[0], m_nChipLimit[1], m_lMaxChipBanker, m_lMaxChipUser, lMaxChipLmt) );*/
				//

				//�Գ���ע
				int	nAreaChance[AREA_COUNT] = {0};	//������
				LONGLONG nMaxBet[AREA_COUNT] = {0};	//���������ע
				CopyMemory( nAreaChance, m_RobotInfo.nAreaChance, sizeof(nAreaChance));

				//�ж������ע
				for( int i = 0 ; i < AREA_COUNT; ++i )
				{
					nMaxBet[i] = GetMaxPlayerScore(i);
					if( nMaxBet[i] < 100 )
					{
						nAreaChance[i] = 0;
					}
				}

				//�������
				int nACTotal = 0;
				int nRandNum = 0;
				int nChipArea = 0;
				for (int i = 0; i < AREA_COUNT; i++)
					nACTotal += nAreaChance[i];
				static int nStFluc = 1;				//�������
				if (nACTotal > 0)	
				{
					nRandNum = (rand()+nStFluc*3) % nACTotal;
					for (int i = 0; i < AREA_COUNT; i++)
					{
						nRandNum -= nAreaChance[i];
						if (nRandNum <= 0)
						{
							nChipArea = i;
							break;
						}
					}
					nStFluc = nStFluc + 2 + rand()%3;

					//�����ע
					LONGLONG lBetScore = 0;

					if( nMaxBet[nChipArea] < m_lRobotBetLimit[0] )
					{
						for( int i = CountArray(m_RobotInfo.nChip) - 1; i >= 0 ; i-- )
						{
							if( nMaxBet[nChipArea] >= m_RobotInfo.nChip[i] )
							{
								lBetScore = m_RobotInfo.nChip[i];
								break;
							}
						}
					}
					else if( nMaxBet[nChipArea] == m_lRobotBetLimit[0] )
					{
						lBetScore = nMaxBet[nChipArea];
					}
					else
					{
						int nCurChip[2] = {0 , CountArray(m_RobotInfo.nChip) - 1};
						for( int i = 0 ; i < CountArray(m_RobotInfo.nChip); ++i )
						{
							if( m_lRobotBetLimit[0] == m_RobotInfo.nChip[i] )
								nCurChip[0] = i;
							if( m_lRobotBetLimit[1] == m_RobotInfo.nChip[i] )
								nCurChip[1] = i;
						}
						if( nCurChip[1] == nCurChip[0] )
							lBetScore = m_RobotInfo.nChip[nCurChip[0]];
						else
							lBetScore = m_RobotInfo.nChip[ (rand()%(nCurChip[1] - nCurChip[0]+1)) + nCurChip[0]];
					}

					if( lBetScore > 0 )
					{
						//��������
						CMD_C_PlaceBet PlaceBet = {};

						//�������
						PlaceBet.cbBetArea = nChipArea;	
						PlaceBet.lBetScore = lBetScore;

						//������Ϣ
						m_pIAndroidUserItem->SendSocketData(SUB_C_PLACE_JETTON, &PlaceBet, sizeof(PlaceBet));

						
					}
				}
				g_CountOne++;
			}

			m_pIAndroidUserItem->KillGameTimer(nTimerID);
			return false;
		}
	}
	return false;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:			//��Ϸ���� 
		{
			return OnSubGameFree(pBuffer, wDataSize);
		}
	case SUB_S_GAME_START:			//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer, wDataSize);
		}
	case SUB_S_PLACE_JETTON:		//�û���ע
		{
			return OnSubPlaceBet(pBuffer, wDataSize);
		}
	case SUB_S_APPLY_BANKER:		//������ׯ 
		{
			return OnSubUserApplyBanker(pBuffer,wDataSize);
		}
	case SUB_S_CANCEL_BANKER:		//ȡ����ׯ 
		{
			return OnSubUserCancelBanker(pBuffer,wDataSize);
		}
	case SUB_S_CHANGE_BANKER:		//�л�ׯ�� 
		{
			return OnSubChangeBanker(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:			//��Ϸ���� 
		{
			return OnSubGameEnd(pBuffer, wDataSize);
		}
	case SUB_S_SEND_RECORD:			//��Ϸ��¼ (����)
		{
			return true;
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ�� (����)
		{
			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			m_lUserLimitScore = pStatusFree->lPlayFreeSocre;
			m_lAreaLimitScore = pStatusFree->lAreaLimitScore;
			m_lBankerCondition = pStatusFree->lApplyBankerCondition;

			memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));

			ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, true);

			return true;
		}
	case GAME_SCENE_PLAY:		//��Ϸ״̬
	case GAME_SCENE_END:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//ׯ����Ϣ
			m_wCurrentBanker = pStatusPlay->wBankerUser;

			m_lUserLimitScore = pStatusPlay->lPlayBetScore;
			m_lAreaLimitScore = pStatusPlay->lAreaLimitScore;
			m_lBankerCondition = pStatusPlay->lApplyBankerCondition;

			memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));

			ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, true);

			return true;
		}
	}

	return true;
}

//�û�����
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}



//��Ϸ����
bool CAndroidUserItemSink::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{

	if( g_Count != 0 )
	{
		TRACE(TEXT("time %d, bet %d \n"), g_Count, g_CountOne);
		g_Count = 0;
		g_CountOne = 0;
	}


	//��ȡ����
	if (m_bRefreshCfg)
		ReadConfigInformation(m_RobotInfo.szCfgFileName, m_szRoomName, false);

	//��Ϣ����
	CMD_S_GameFree* pGameFree=(CMD_S_GameFree *)pBuffer;

	m_cbTimeLeave = pGameFree->cbTimeLeave;

	bool bMeGiveUp = false;
	if (m_wCurrentBanker == m_pIAndroidUserItem->GetChairID())
	{
		m_nBankerCount++;
		if ( m_nBankerCount >= m_nRobotBankerCount )
		{
			//��������ׯ
			m_nBankerCount = 0;
			m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP_BANKER, rand()%2 + 1);

			bMeGiveUp = true;
		}
	}

	//�����ׯ
	if (m_wCurrentBanker != m_pIAndroidUserItem->GetChairID() || bMeGiveUp)
	{
		m_cbTimeLeave = pGameFree->cbTimeLeave - 3;
		m_pIAndroidUserItem->SetGameTimer(IDI_CHECK_BANKER, 3);
	}


	//���в���
	if (pGameFree->cbTimeLeave > 2)
		m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATE, (rand() % (pGameFree->cbTimeLeave-1)) + 1);

	return true;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{	
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//�Լ���ׯ������ע������
	if (pGameStart->wBankerUser == m_pIAndroidUserItem->GetChairID() || pGameStart->nChipRobotCount <= 0)
		return true;

	//���ñ���
	m_lAndroidBet = 0;
	m_nListUserCount = pGameStart->nListUserCount;
	m_lMaxChipBanker = pGameStart->lBankerScore/m_RobotInfo.nMaxTime;

	CString strO = m_pIAndroidUserItem->GetMeUserItem()->GetNickName();
	m_lMaxChipUser = pGameStart->lPlayBetScore/m_RobotInfo.nMaxTime;
	m_wCurrentBanker = pGameStart->wBankerUser;
	m_lBankerScore = pGameStart->lBankerScore;
	m_nChipTimeCount = 0;
	ZeroMemory(m_nChipLimit, sizeof(m_nChipLimit));
	ZeroMemory(m_lAllBet,sizeof(m_lAllBet));

	if(pGameStart->nAndriodCount > 0)
		m_stlApplyBanker=pGameStart->nAndriodCount;
	else
		m_stlApplyBanker=0;

	for (int i = 0; i < AREA_COUNT; i++)
		m_lAreaChip[i] = m_lAreaLimitScore;

	//�ϴ����� ���
	//m_stlApplyBanker = 0;

	ASSERT(m_lMaxChipUser >= 0);

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
	//if (!CalcBetRange(__min(m_lMaxChipBanker, m_lMaxChipUser), m_lRobotBetLimit, m_nChipTime, m_nChipLimit))
	//	return true;

	//����ʱ��
	int nTimeGrid = int(pGameStart->cbTimeLeave-2)*800/m_nChipTime;		//ʱ���,ǰ2�벻��ע,����-2,800��ʾ��������עʱ�䷶Χǧ�ֱ�
	for (int i = 0; i < m_nChipTime; i++)
	{
		int nRandRage = int( nTimeGrid * i / (1500*sqrt((double)m_nChipTime)) ) + 1;		//������Χ
		nElapse = 2 + (nTimeGrid*i)/1000 + ( (rand()+wMyID)%(nRandRage*2) - (nRandRage-1) );
		ASSERT(nElapse>=2&&nElapse<=pGameStart->cbTimeLeave);
		if (nElapse < 2 || nElapse > pGameStart->cbTimeLeave)	continue;
		
		m_pIAndroidUserItem->SetGameTimer(IDI_PLACE_JETTON+i+1, nElapse);

		g_Count++;
	}

	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubPlaceBet(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceBet));
	if (wDataSize!=sizeof(CMD_S_PlaceBet)) return false;

	//��Ϣ����
	CMD_S_PlaceBet * pPlaceBet=(CMD_S_PlaceBet *)pBuffer;

	//��ӳ���
	if( pPlaceBet->wChairID == m_pIAndroidUserItem->GetChairID())
		m_lAndroidBet += pPlaceBet->lBetScore;

	m_lAllBet[pPlaceBet->cbBetArea] += pPlaceBet->lBetScore;

	//���ñ���
	m_lMaxChipBanker -= pPlaceBet->lBetScore;

	//�Գ����
	m_lAreaChip[pPlaceBet->cbBetArea] -= pPlaceBet->lBetScore;

	//if (pPlaceBet->wChairID == m_pIAndroidUserItem->GetChairID())
	//	m_lMaxChipUser -= pPlaceBet->lBetScore;

	ASSERT(m_lMaxChipUser >= 0);

	return true;
}

//��עʧ��
bool CAndroidUserItemSink::OnSubPlaceBetFail(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	m_stnApplyCount = 0;

	return true;
}

//������ׯ
bool CAndroidUserItemSink::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) return false;

	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;

	//�Լ��ж�
	if (m_pIAndroidUserItem->GetChairID()==pApplyBanker->wApplyUser) 
		m_bMeApplyBanker = true;

	return true;
}

//ȡ����ׯ
bool CAndroidUserItemSink::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) return false;

	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;

	//�Լ��ж�
	if ( m_pIAndroidUserItem->GetMeUserItem()->GetChairID() == pCancelBanker->wCancelUser ) 
		m_bMeApplyBanker = false;

	return true;
}

//�л�ׯ��
bool CAndroidUserItemSink::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker = (CMD_S_ChangeBanker *)pBuffer;

	if ( m_wCurrentBanker == m_pIAndroidUserItem->GetChairID() && m_wCurrentBanker != pChangeBanker->wBankerUser )
	{
		//m_stlApplyBanker--;
		m_nWaitBanker = 0;
		m_bMeApplyBanker = false;
	}
	m_wCurrentBanker = pChangeBanker->wBankerUser;

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
	_sntprintf(szConfigFileName, sizeof(szConfigFileName), TEXT("%s\\%s"), szPath, szFileName);

	//ÿ��ˢ��
	if (bReadFresh)
	{
		//ÿ��ˢ��
		BYTE cbRefreshCfg = GetPrivateProfileInt(szRoomName, TEXT("Refresh"), 0, szConfigFileName);
		m_bRefreshCfg = cbRefreshCfg?true:false;
	}

	//��������
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotMaxBet"), TEXT("10000000"), OutBuf, 255, szConfigFileName);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%I64d"), &m_lRobotBetLimit[1]);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(szRoomName, TEXT("RobotMinBet"), TEXT("100"), OutBuf, 255, szConfigFileName);
	_sntscanf(OutBuf, lstrlen(OutBuf), TEXT("%I64d"), &m_lRobotBetLimit[0]);

	if (m_lRobotBetLimit[1] > 10000000)					m_lRobotBetLimit[1] = 10000000;
	if (m_lRobotBetLimit[0] < 100)						m_lRobotBetLimit[0] = 100;
	if (m_lRobotBetLimit[1] < m_lRobotBetLimit[0])	m_lRobotBetLimit[1] = m_lRobotBetLimit[0];

	//��������
	m_nRobotBetTimeLimit[0] = GetPrivateProfileInt(szRoomName, TEXT("RobotMinBetTime"), 4, szConfigFileName);;
	m_nRobotBetTimeLimit[1] = GetPrivateProfileInt(szRoomName, TEXT("RobotMaxBetTime"), 8, szConfigFileName);;

	if (m_nRobotBetTimeLimit[0] < 0)							m_nRobotBetTimeLimit[0] = 0;
	if (m_nRobotBetTimeLimit[1] < m_nRobotBetTimeLimit[0])		m_nRobotBetTimeLimit[1] = m_nRobotBetTimeLimit[0];

	//�Ƿ���ׯ
	m_bRobotBanker = (GetPrivateProfileInt(szRoomName, TEXT("RobotBanker"), 0, szConfigFileName) == 1);

	//��ׯ����
	//m_nRobotBankerCount = GetPrivateProfileInt(szRoomName, TEXT("RobotBankerCount"), 3, szConfigFileName);

	//��ׯ����
	m_nMinBankerTimes = GetPrivateProfileInt(szRoomName, _T("RobotBankerMinCount"), 2, szConfigFileName);

	//��ׯ����
	m_nMaxBankerTimes = GetPrivateProfileInt(szRoomName, _T("RobotBankerMaxCount"), 5, szConfigFileName);

	if(m_nMinBankerTimes<0)m_nMinBankerTimes=2;
	if(m_nMaxBankerTimes<m_nMinBankerTimes)m_nMaxBankerTimes=m_nMinBankerTimes+2;

	m_nRobotBankerCount=m_nMinBankerTimes+rand()%(m_nMaxBankerTimes-m_nMinBankerTimes);


	//��������
	m_nRobotWaitBanker = GetPrivateProfileInt(szRoomName, TEXT("RobotWaitBanker"), 3, szConfigFileName);

	//������
	m_nRobotListMaxCount = GetPrivateProfileInt(szRoomName, TEXT("RobotListMaxCount"), 5, szConfigFileName);

	//���ٸ���
	m_nRobotListMinCount = GetPrivateProfileInt(szRoomName, TEXT("RobotListMinCount"), 2, szConfigFileName);

	//��������
	m_bReduceBetLimit = (GetPrivateProfileInt(szRoomName, TEXT("RobotReduceLimit"), 0, szConfigFileName)!=0);

	//��������
	m_lRobotScoreRange[0] = GetPrivateProfileInt(szRoomName, _T("RobotScoreMin"), 100, szConfigFileName);
	m_lRobotScoreRange[1] = GetPrivateProfileInt(szRoomName, _T("RobotScoreMax"), 100000, szConfigFileName);

	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//�������
	m_lRobotBankGetScore = GetPrivateProfileInt(szRoomName, _T("RobotBankGet"), 20000000, szConfigFileName);

	//������� (ׯ��)
	m_lRobotBankGetScoreBanker = GetPrivateProfileInt(szRoomName, _T("RobotBankGetBanker"), 30000000, szConfigFileName);

	//����
	m_nRobotBankStorageMul = GetPrivateProfileInt(szRoomName, _T("RobotBankStoMul"), 20, szConfigFileName);

	//�������
	m_RobotInfo.nAreaChance[0] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance1"), 3, szConfigFileName);
	m_RobotInfo.nAreaChance[1] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance2"), 0, szConfigFileName);
	m_RobotInfo.nAreaChance[2] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance3"), 3, szConfigFileName);
	m_RobotInfo.nAreaChance[3] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance4"), 1, szConfigFileName);
	m_RobotInfo.nAreaChance[4] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance5"), 1, szConfigFileName);
	m_RobotInfo.nAreaChance[5] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance6"), 0, szConfigFileName);
	m_RobotInfo.nAreaChance[6] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance7"), 0, szConfigFileName);
	m_RobotInfo.nAreaChance[7] = GetPrivateProfileInt(szRoomName, TEXT("AreaChance8"), 0, szConfigFileName);
}

//���㷶Χ	(����ֵ��ʾ�Ƿ����ͨ���½����޴ﵽ��ע)
bool CAndroidUserItemSink::CalcBetRange(LONGLONG lMaxScore, LONGLONG lChipLmt[], int & nChipTime, int lJetLmt[])
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

	//�Ƿ񽵵�����
	if (m_bReduceBetLimit)
	{
		if (nChipTime * m_RobotInfo.nChip[lJetLmt[0]] > lMaxScore)
		{
			//�Ƿ񽵵���ע����
			if (nChipTime * m_RobotInfo.nChip[0] > lMaxScore)
			{
				nChipTime = int(lMaxScore/m_RobotInfo.nChip[0]);
				lJetLmt[0] = 0;
				lJetLmt[1] = 0;
			}
			else
			{
				//���͵���������
				while (nChipTime * m_RobotInfo.nChip[lJetLmt[0]] > lMaxScore)
				{
					lJetLmt[0]--;
					ASSERT(lJetLmt[0]>=0);
				}
			}
		}
	}

	return true;
}


//�����ע
LONGLONG CAndroidUserItemSink::GetMaxPlayerScore( BYTE cbBetArea )
{	
	if( cbBetArea >= AREA_MAX )
		return 0L;

	//����ע��
	LONGLONG lNowBet = m_lAndroidBet;

	//ׯ�ҽ��
	LONGLONG lBankerScore = -1;

	//������
	BYTE cbMultiple[AREA_MAX] = {MULTIPLE_XIAN, MULTIPLE_PING, MULTIPLE_ZHUANG, 
		MULTIPLE_XIAN_TIAN, MULTIPLE_ZHUANG_TIAN, MULTIPLE_TONG_DIAN, 
		MULTIPLE_XIAN_PING, MULTIPLE_ZHUANG_PING};

	//������Ӯ
	BYTE cbArae[4][4] = {	{ AREA_XIAN_DUI,	255,			AREA_MAX,			AREA_MAX }, 
	{ AREA_ZHUANG_DUI,	255,			AREA_MAX,			AREA_MAX }, 
	{ AREA_XIAN,		AREA_PING,		AREA_ZHUANG,		AREA_MAX },  
	{ AREA_XIAN_TIAN,	AREA_TONG_DUI,	AREA_ZHUANG_TIAN,	255 }};
	//�����趨
	for ( int nTopL = 0; nTopL < 4; ++nTopL )
	{
		if( cbArae[0][nTopL] == AREA_MAX )
			continue;

		for ( int nTopR = 0; nTopR < 4; ++nTopR )
		{
			if( cbArae[1][nTopR] == AREA_MAX )
				continue;

			for ( int nCentral = 0; nCentral < 4; ++nCentral )
			{
				if( cbArae[2][nCentral] == AREA_MAX )
					continue;

				for ( int nBottom = 0; nBottom < 4; ++nBottom )
				{
					if( cbArae[3][nBottom] == AREA_MAX )
						continue;

					BYTE cbWinArea[AREA_MAX] = {FALSE};

					//ָ����ʤ����
					if ( cbArae[0][nTopL] != 255 && cbArae[0][nTopL] != AREA_MAX )
						cbWinArea[cbArae[0][nTopL]] = TRUE;

					if ( cbArae[1][nTopR] != 255 && cbArae[1][nTopR] != AREA_MAX )
						cbWinArea[cbArae[1][nTopR]] = TRUE;

					if ( cbArae[2][nCentral] != 255 && cbArae[2][nCentral] != AREA_MAX )
						cbWinArea[cbArae[2][nCentral]] = TRUE;

					if ( cbArae[3][nBottom] != 255 && cbArae[3][nBottom] != AREA_MAX )
						cbWinArea[cbArae[3][nBottom]] = TRUE;

					//ѡ������Ϊ���ʤ����ͬ�ȼ�������������Ϊ����䡣�Եó������עֵ
					for ( int i = 0; i < 4; i++ )
					{
						for ( int j = 0; j < 4; j++ )
						{
							if ( cbArae[i][j] == cbBetArea )
							{
								for ( int n = 0; n < 4; ++n )
								{
									if ( cbArae[i][n] != 255 && cbArae[i][n] != AREA_MAX )
									{
										cbWinArea[cbArae[i][n]] = FALSE;
									}
								}
								cbWinArea[cbArae[i][j]] = TRUE;
							}
						}
					}

					LONGLONG lScore = m_lBankerScore;
					for (int nAreaIndex = 0; nAreaIndex < AREA_MAX; ++nAreaIndex ) 
					{
						if ( cbWinArea[nAreaIndex] == TRUE )
						{
							lScore -= m_lAllBet[nAreaIndex]*(cbMultiple[nAreaIndex] - 1);
						}
						else if ( cbWinArea[AREA_PING] == TRUE && ( nAreaIndex == AREA_XIAN || nAreaIndex == AREA_ZHUANG ) )
						{

						}
						else
						{
							lScore += m_lAllBet[nAreaIndex];
						}
					}
					if ( lBankerScore == -1 )
						lBankerScore = lScore;
					else
						lBankerScore = min(lBankerScore, lScore);
				}
			}
		}
	}

	//�����ע
	LONGLONG lMaxBet = 0L;

	ASSERT(m_lMaxChipUser >= 0);

	//�����ע
	lMaxBet = min( m_lMaxChipUser - lNowBet, m_lAreaLimitScore - m_lAndroidBet);

	lMaxBet = min( m_lMaxChipUser - lNowBet, m_lAreaLimitScore - m_lAllBet[cbBetArea]);

	lMaxBet = min( lMaxBet, lBankerScore / (cbMultiple[cbBetArea] - 1));

	//��������
	lMaxBet = max(lMaxBet, 0);

	return lMaxBet;
}
//////////////////////////////////////////////////////////////////////////
