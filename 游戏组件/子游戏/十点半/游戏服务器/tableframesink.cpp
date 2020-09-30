#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define		IDI_GAME_END			1						//��Ϸ������ʱ��

#define		TIME_GAME_END			2000					//��Ϸ����ʱ��


//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const BYTE			CTableFrameSink::m_GameStartMode=START_MODE_ALL_READY;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_lCellScore = 0L;
	m_wCurrentUser = INVALID_CHAIR;
	m_wBankerUser = INVALID_CHAIR;
	ZeroMemory( m_lGameScore,sizeof(m_lGameScore) );

	//״̬����
	ZeroMemory(m_byUserStatus,sizeof(m_byUserStatus));
	ZeroMemory(m_bAddScore,sizeof(m_bAddScore));

	//��ע����
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//�˿˱���
	m_bySendCardCount = 0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	ZeroMemory(m_byCardCount,sizeof(m_byCardCount));
	ZeroMemory(m_byHandCardData,sizeof(m_byHandCardData));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameCustomRule=NULL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;
	
	//��ȡ����
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//��Ԫ����
	m_lCellScore=m_pGameServiceOption->lCellScore;

	//�Զ�����
	ASSERT(m_pITableFrame->GetCustomRule()!=NULL);
	m_pGameCustomRule=(tagCustomRule *)m_pITableFrame->GetCustomRule();

	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	//��Ԫ����
	m_lCellScore=0L;
	
	//��Ϸ����
	m_wCurrentUser = INVALID_CHAIR;
	ZeroMemory( m_lGameScore,sizeof(m_lGameScore) );

	//״̬����
	ZeroMemory(m_byUserStatus,sizeof(m_byUserStatus));
	ZeroMemory(m_bAddScore,sizeof(m_bAddScore));

	//��ע����
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//�˿˱���
	m_bySendCardCount = 0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	ZeroMemory(m_byCardCount,sizeof(m_byCardCount));
	ZeroMemory(m_byHandCardData,sizeof(m_byHandCardData));

	return;
}

//��������
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	//Ч��״̬
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->GetTableID()==m_pITableFrame->GetTableID()));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetTableID()!=m_pITableFrame->GetTableID())) return 0;

	//�û��ж�
	switch (pIServerUserItem->GetUserStatus())
	{
	case US_SIT:		//����״̬
	case US_READY:		//׼��״̬
		{
			return pIServerUserItem->GetUserScore()-m_pGameServiceOption->lMinTableScore;
		}
	case US_PLAYING:	//��Ϸ״̬
	case US_OFFLINE:	//����״̬
		{
			WORD wMaxTimes=m_pGameCustomRule->wMaxScoreTimes;
			return pIServerUserItem->GetUserScore()-m_pGameServiceOption->lCellScore*wMaxTimes;
		}
	case US_LOOKON:		//�Թ�״̬
		{
			return pIServerUserItem->GetUserScore();
		}
	}

	return 0L;
}

//���ٻ���
SCORE CTableFrameSink::QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return 0L;
}

//��ʼģʽ
BYTE  CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//��Ϸ״̬
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_TH_SCORE);

	//���õ�ע
	LONGLONG lMinUserScore = 0L;
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		IServerUserItem *pUserItem = m_pITableFrame->GetTableUserItem(i);
		if( pUserItem == NULL )
		{
			m_byUserStatus[i] = FALSE;
			continue;
		}

		m_byUserStatus[i] = TRUE;

		if( i == m_wBankerUser )
			m_lCellScore = __max(m_pGameServiceOption->lCellScore,pUserItem->GetUserScore()/240L);

		if( lMinUserScore == 0L || lMinUserScore > pUserItem->GetUserScore() )
			lMinUserScore = pUserItem->GetUserScore();
	}
	m_lCellScore = __min( lMinUserScore/8,m_lCellScore );

	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	GameStart.lCellScore = m_lCellScore;
	GameStart.wBankerUser = m_wBankerUser;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//��ҵ���
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				if( !m_byUserStatus[i] ) continue;

				GameEnd.byCardData[i] = m_byHandCardData[i][0];
			}

			//�Ƶ�Ƚ�
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				GameEnd.lGameScore[i] -= m_lGameScore[i];

				if( !m_byUserStatus[i] || i == m_wBankerUser ) continue;

				//ׯ�Ҵ�
				if( m_GameLogic.CompareCard(m_byHandCardData[i],m_byCardCount[i],m_byHandCardData[m_wBankerUser],m_byCardCount[m_wBankerUser]) )
				{
					GameEnd.lGameScore[i] -= m_lTableScore[i];
					GameEnd.lGameScore[m_wBankerUser] += m_lTableScore[i];
				}
				else
				{
					//�������
					FLOAT fCardScore = m_GameLogic.GetCardGenre(m_byHandCardData[i],m_byCardCount[i]);

					BYTE cbTimes = 1;
					//��������10��
					if( fCardScore == CT_DA_TIAN_WANG )
						cbTimes = 10;
					//��С��9��
					else if( fCardScore == CT_JIU_XIAO )
						cbTimes = 9;
					//��С��8��
					else if( fCardScore == CT_BA_XIAO )
						cbTimes = 8;
					//��С��7��
					else if( fCardScore == CT_QI_XIAO )
						cbTimes = 7;
					//��С��6��
					else if( fCardScore == CT_LIU_XIAO )
						cbTimes = 6;
					//������5��
					else if( fCardScore == CT_TIAN_WANG )
						cbTimes = 5;
					//����С��4��
					else if( fCardScore == CT_REN_WU_XIAO )
						cbTimes = 4;
					//��С��3��
					else if( fCardScore == CT_WU_XIAO )
						cbTimes = 3;
					//ʮ��룺2��
					else if( fCardScore == 10.5 )
						cbTimes = 2;

					GameEnd.lGameScore[i] = m_lTableScore[i]*cbTimes;
					GameEnd.lGameScore[m_wBankerUser] -= GameEnd.lGameScore[i];
				}
			}
			

			//ͳ�ƻ���
			LONGLONG lGameTax[GAME_PLAYER];
			ZeroMemory( lGameTax,sizeof(lGameTax) );
			if( GAME_GENRE_SCORE != m_pGameServiceOption->wServerType )
			{
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					if( !m_byUserStatus[i] ) continue;

					if( GameEnd.lGameScore[i] > 0L )
						lGameTax[i] = m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);

					GameEnd.lGameScore[i]-=lGameTax[i];
				}
			}

			//��������
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//д�����
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				if( !m_byUserStatus[i] ) continue;

				if( GameEnd.lGameScore[i] > 0L ) ScoreInfoArray[i].cbType = SCORE_TYPE_WIN;
				else if( GameEnd.lGameScore[i] < 0L )  ScoreInfoArray[i].cbType = SCORE_TYPE_LOSE;
				else  ScoreInfoArray[i].cbType = SCORE_TYPE_DRAW;

				 ScoreInfoArray[i].lScore=GameEnd.lGameScore[i];
				 ScoreInfoArray[i].lRevenue=lGameTax[i];  
			}
			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			//ׯ���л�
			if( GameEnd.lGameScore[m_wBankerUser] < 0 )
			{
				m_wBankerUser = (m_wBankerUser+1)%GAME_PLAYER;
				while( !m_byUserStatus[m_wBankerUser] ) m_wBankerUser = (m_wBankerUser+1)%GAME_PLAYER;
			}

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

			return true;
		}
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

			return true;
		}
	case GER_USER_LEAVE:		//�û�ǿ��
		{
			//ׯ��ǿ��
			if( wChairID == m_wBankerUser )
			{
				//��������ҳ�ʼ��ע��֮��
				LONGLONG lScore[GAME_PLAYER] = {0};
				LONGLONG lGameTax[GAME_PLAYER] = {0};
				for( WORD i = 0; i < GAME_PLAYER; i++ )
				{
					if( !m_byUserStatus[i] || i == wChairID ) continue;

					lScore[i] = m_lTableScore[i];

					if( GAME_GENRE_SCORE != m_pGameServiceOption->wServerType )
					{
						lGameTax[i] = m_pITableFrame->CalculateRevenue(i,lScore[i]);
						lScore[i] -= lGameTax[i];
					}

					lScore[m_wBankerUser] -= m_lTableScore[i];
				}

				//������Ϣ
				CMD_S_GameEnd ge = {};
				CopyMemory( ge.lGameScore,lScore,sizeof(ge.lGameScore) );
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&ge,sizeof(ge));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&ge,sizeof(ge));

				//д��
				tagScoreInfo ScoreInfoArray[GAME_PLAYER];
				ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

				for( WORD i = 0; i < GAME_PLAYER; i++ )
				{
					if( !m_byUserStatus[i] ) continue;

					if( ge.lGameScore[i] > 0L ) ScoreInfoArray[i].cbType = SCORE_TYPE_WIN;
					else if( ge.lGameScore[i] < 0L ) ScoreInfoArray[i].cbType = SCORE_TYPE_FLEE;
					else ScoreInfoArray[i].cbType = SCORE_TYPE_DRAW;

					ScoreInfoArray[i].lRevenue=lGameTax[i];                                  
					ScoreInfoArray[i].lScore=ge.lGameScore[i];

				}
				m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

				//֪ͨ��Ϣ
				TCHAR szMessage[128]=TEXT("");
				_sntprintf(szMessage,CountArray(szMessage),TEXT("����ׯ�� [ %s ] ���ܣ���Ϸ������"),pIServerUserItem->GetNickName());
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					IServerUserItem * pISendUserItem=m_pITableFrame->GetTableUserItem(i);
					if (pISendUserItem!=NULL) 
						m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_PROMPT);
				}

				WORD wIndex=0;
				do
				{
					IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
					if (pISendUserItem==NULL)
						break;
					m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_PROMPT);
				} while (true);

				//������Ϸ
				m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);
			}
			else
			{
				//����ҷ�
				tagScoreInfo ScoreInfoArray[GAME_PLAYER];
				ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

				m_lGameScore[wChairID] = m_lTableScore[wChairID];
				ScoreInfoArray[wChairID].lScore=-m_lTableScore[wChairID];
				ScoreInfoArray[wChairID].cbType=SCORE_TYPE_FLEE;

				m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

				//�۷ָ�ׯ��
				LONGLONG lScore = m_lTableScore[wChairID];
				LONGLONG lTax = 0L;
				if( GAME_GENRE_SCORE != m_pGameServiceOption->wServerType )
				{
					lTax = m_pITableFrame->CalculateRevenue(wChairID,lScore);
					lScore -= lTax;
				}

				ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

				ScoreInfoArray[m_wBankerUser].cbType =SCORE_TYPE_WIN;                           
				ScoreInfoArray[m_wBankerUser].lScore=lScore;

				m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

				//ʣ�����
				BYTE cbUserCount = 0;
				for( WORD i = 0; i < GAME_PLAYER; i++ )
				{
					if( m_byUserStatus[i] ) cbUserCount++;
				}
				bool bGameEnd = cbUserCount==2?true:false;

				//���ֻʣׯ��
				if( bGameEnd )
				{
					//ׯ�ҵ÷�
					LONGLONG lScore[GAME_PLAYER] = {0};
					LONGLONG lGameTax[GAME_PLAYER] = {0};
					for( WORD i = 0; i < GAME_PLAYER; i++ )
					{
						if( i == m_wBankerUser || !m_byUserStatus[i] ) continue;

						lScore[i] -= m_lTableScore[i];

						lScore[m_wBankerUser] += m_lTableScore[i];
					}

					m_byUserStatus[wChairID] = FALSE;

					if( GAME_GENRE_SCORE != m_pGameServiceOption->wServerType )
					{
						lGameTax[m_wBankerUser] = m_pITableFrame->CalculateRevenue(m_wBankerUser,lScore[m_wBankerUser]);
						lScore[m_wBankerUser] -= lGameTax[m_wBankerUser];
					}
					
					//������Ϣ
					CMD_S_GameEnd ge = {};
					CopyMemory( ge.lGameScore,lScore,sizeof(ge.lGameScore) );
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&ge,sizeof(ge));
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&ge,sizeof(ge));
				}
				else
				{
					m_byUserStatus[wChairID] = FALSE;

					if( wChairID == m_wCurrentUser )
					{
						m_wCurrentUser = (m_wCurrentUser+1)%GAME_PLAYER;
						while( !m_byUserStatus[m_wCurrentUser] ) m_wCurrentUser = (m_wCurrentUser+1)%GAME_PLAYER;
					}

					//������Ϣ
					CMD_S_UserLeft ul = {};
					ul.wLeftUser = wChairID;
					ul.wCurrentUser = m_wCurrentUser;
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_LEFT,&ul,sizeof(ul));
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_LEFT,&ul,sizeof(ul));
				}

				//֪ͨ��Ϣ
				TCHAR szMessage[128]=TEXT("");
				_sntprintf(szMessage,CountArray(szMessage),TEXT("�м� [ %s ] ���ܣ��۳�%ld�֣�"),pIServerUserItem->GetNickName(),m_lTableScore[wChairID]);
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					IServerUserItem * pISendUserItem=m_pITableFrame->GetTableUserItem(i);
					if (pISendUserItem!=NULL) 
						m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_PROMPT);
				}

				WORD wIndex=0;
				do
				{
					IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
					if (pISendUserItem==NULL)
						break;
					m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_PROMPT);
				} while (true);

				//��ע״̬
				if( !bGameEnd && m_pITableFrame->GetGameStatus() == GS_TH_SCORE )
				{
					bool bScoreFinish = true;
					for( WORD i = 0; i < GAME_PLAYER; i++ )
					{
						if( !m_byUserStatus[i] || i == m_wBankerUser ) continue;

						if( m_lTableScore[i] == 0 )
						{
							bScoreFinish = false;
							break;
						}
					}

					if( bScoreFinish )
					{
						SendGamePlay();
						return true;
					}
				}
				
				//������Ϸ
				if( bGameEnd )
					m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);
			}
			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_TH_FREE:	//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_TH_SCORE:	//��ע״̬
		{
			//��������
			CMD_S_StatusScore StatusScore = {};

			//���޻���
			StatusScore.lCellScore = m_lCellScore;

			//״̬����
			StatusScore.wBankerUser = m_wBankerUser;
			CopyMemory(StatusScore.bUserStatus,m_byUserStatus,sizeof(m_byUserStatus));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
		}
	case GS_TH_PLAY:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//���޻���
			StatusPlay.lCellScore = m_lCellScore;

			//״̬����
			StatusPlay.wCurrentUser = m_wCurrentUser;
			StatusPlay.wBankerUser = m_wBankerUser;
			CopyMemory( StatusPlay.byUserStatus,m_byUserStatus,sizeof(m_byUserStatus) );

			//��ע����
			CopyMemory( StatusPlay.lTableScore,m_lTableScore,sizeof(m_lTableScore) );

			//�˿˱���
			CopyMemory( StatusPlay.cbCardCount,m_byCardCount,sizeof(m_byCardCount) );
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				if( m_byCardCount[i] == 0 ) continue;
				CopyMemory( &StatusPlay.cbHandCardData[i][1],&m_byHandCardData[i][2],
					(m_byCardCount[i]-1)*sizeof(BYTE) );
				if( m_byUserStatus[i] && i == wChairID && bSendSecret )
					StatusPlay.cbHandCardData[i][0] = m_byHandCardData[i][0];
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	return false;
}
//�����¼�
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//�����¼�
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return true;
}
//��Ϸ��Ϣ����
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_ADD_SCORE:	//��ע
		{
			//״̬Ч��
			ASSERT(m_pITableFrame->GetGameStatus()==GS_TH_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GS_TH_PLAY) return true;

			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			return OnSubUserAddScore( pIServerUserItem->GetChairID() );
		}
	case SUB_C_GIVE_UP:
		{
			//״̬Ч��
			ASSERT(m_pITableFrame->GetGameStatus()==GS_TH_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GS_TH_PLAY) return true;

			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			return OnSubUserGiveUp( pIServerUserItem->GetChairID() );
		}
	case SUB_C_GIVE_CARD:
		{
			//״̬Ч��
			ASSERT(m_pITableFrame->GetGameStatus()==GS_TH_PLAY);
			if (m_pITableFrame->GetGameStatus()!=GS_TH_PLAY) return true;

			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			return OnSubUserGiveCard( pIServerUserItem->GetChairID() );
		}
	case SUB_C_SCORE:
		{
			ASSERT( wDataSize == sizeof(CMD_C_Score) );
			if( wDataSize != sizeof(CMD_C_Score) ) return false;

			//״̬Ч��
			ASSERT(m_pITableFrame->GetGameStatus()==GS_TH_SCORE);
			if (m_pITableFrame->GetGameStatus()!=GS_TH_SCORE) return true;

			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_Score *pScore = (CMD_C_Score *)pData;
			return OnSubUserScore(pIServerUserItem->GetChairID(),pScore->lScore);
		}
	}

	return false;
}

//�����Ϣ����
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�����ע
bool CTableFrameSink::OnSubUserAddScore( WORD wChairId )
{
	//У��
	ASSERT( wChairId == m_wCurrentUser );
	if( wChairId != m_wCurrentUser ) return false;
	ASSERT( !m_bAddScore[wChairId] );
	if( m_bAddScore[wChairId] ) return true;

	ASSERT( m_byCardCount[wChairId] < MAX_COUNT );
	if( m_byCardCount[wChairId] >= MAX_COUNT ) return false;

	FLOAT fCardScore = m_GameLogic.GetCardGenre(m_byHandCardData[wChairId],m_byCardCount[wChairId]);
	ASSERT( fCardScore != CT_ERROR );
	if( fCardScore == CT_ERROR ) return false;

	m_bAddScore[wChairId] = true;
	//����������ע
	m_lTableScore[wChairId] *= 2;

	//����
	BYTE cbSendCardData = m_cbRepertoryCard[m_bySendCardCount++];
	m_byHandCardData[wChairId][m_byCardCount[wChairId]++] = cbSendCardData;

	//�л���һλ���
	m_wCurrentUser = (m_wCurrentUser+1)%GAME_PLAYER;
	while( !m_byUserStatus[m_wCurrentUser] ) m_wCurrentUser = (m_wCurrentUser+1)%GAME_PLAYER;

	//������ע��Ϣ
	CMD_S_AddScore AddScore;
	AddScore.wAddScoreUser = wChairId;
	AddScore.wCurrentUser = m_wCurrentUser;
	AddScore.cbCardData = cbSendCardData;
	m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore) );
	m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore) );

	return true;
}

//��ҷ���
bool CTableFrameSink::OnSubUserGiveUp( WORD wChairId )
{
	//Ч��
	ASSERT( m_wCurrentUser == wChairId );
	if( m_wCurrentUser != wChairId ) return false;

	//�ж��Ƿ����
	bool bFinish = wChairId==m_wBankerUser?true:false;

	//�����Ϸ����,�������޵�ǰ���
	if( bFinish ) 
		m_wCurrentUser = INVALID_CHAIR;
	else
	{
		//Ѱ����һ�����
		m_wCurrentUser = (m_wCurrentUser+1)%GAME_PLAYER;
		while( !m_byUserStatus[m_wCurrentUser] ) m_wCurrentUser = (m_wCurrentUser+1)%GAME_PLAYER;
	}

	//����ͣ����Ϣ
	CMD_S_GiveUp GiveUp;
	GiveUp.wGiveUpUser = wChairId;
	GiveUp.wCurrentUser = m_wCurrentUser;
	m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp) );
	m_pITableFrame->SendLookonData( INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp) );

	//�������
	if( bFinish )
	{
		OnEventGameConclude( INVALID_CHAIR,NULL,GER_NORMAL );
	}

	return true;
}

//���Ҫ��
bool CTableFrameSink::OnSubUserGiveCard( WORD wChairId )
{
	//Ч��
	ASSERT( m_wCurrentUser == wChairId );
	if( m_wCurrentUser != wChairId ) return false;

	ASSERT( m_byCardCount[wChairId] < MAX_COUNT );
	if( m_byCardCount[wChairId] >= MAX_COUNT ) return false;

	FLOAT fCardScore = m_GameLogic.GetCardGenre(m_byHandCardData[wChairId],m_byCardCount[wChairId]);
	ASSERT( fCardScore != CT_ERROR );
	if( fCardScore == CT_ERROR ) return false;

	//����
	BYTE cbSendCardData = m_cbRepertoryCard[m_bySendCardCount++];
	m_byHandCardData[wChairId][m_byCardCount[wChairId]++] = cbSendCardData;

	//��ǰ���
	BYTE cbCardData[MAX_COUNT] = {};
	BYTE cbCardCount = wChairId==m_wBankerUser?m_byCardCount[wChairId]:m_byCardCount[wChairId]-1;
	CopyMemory( cbCardData,&m_byHandCardData[wChairId][wChairId==m_wBankerUser?0:1],sizeof(BYTE)*cbCardCount );
	FLOAT fCurrentScore = m_GameLogic.GetCardGenre(cbCardData,cbCardCount);
	//�������
	if( fCurrentScore == CT_ERROR )
	{
		if( m_wBankerUser == wChairId )
			m_wCurrentUser = INVALID_CHAIR;
		else
		{
			m_wCurrentUser = (m_wCurrentUser+1)%GAME_PLAYER;
			while( !m_byUserStatus[m_wCurrentUser] ) m_wCurrentUser = (m_wCurrentUser+1)%GAME_PLAYER;
		}
	}

	//������Ϣ
	CMD_S_SendCard sc = {};
	sc.wCurrentUser = m_wCurrentUser;
	sc.wSendCardUser = wChairId;
	sc.cbCardData = cbSendCardData;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&sc,sizeof(sc));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&sc,sizeof(sc));

	//�����ׯ�ұ��ƣ���ֱ�ӽ���
	if( fCurrentScore == CT_ERROR && m_wCurrentUser == INVALID_CHAIR )
	{
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
	}

	return true;
}

//�����ע
bool CTableFrameSink::OnSubUserScore(WORD wChairId,LONGLONG lScore)
{
	//Ч��
	ASSERT( m_pITableFrame->GetGameStatus() == GS_TH_SCORE );
	if( m_pITableFrame->GetGameStatus() != GS_TH_SCORE ) return false;
	ASSERT( lScore%m_lCellScore==0 && lScore/m_lCellScore >= 1 && lScore/m_lCellScore <= 4 );
	if( lScore%m_lCellScore != 0 || lScore/m_lCellScore == 0 || lScore/m_lCellScore > 4 ) return false;
	ASSERT( m_lTableScore[wChairId] == 0 );
	if( m_lTableScore[wChairId] != 0 ) return true;

	//������ע
	m_lTableScore[wChairId] = lScore;

	//�鿴�Ƿ�ȫ����ע
	bool bFinishScore = true;
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if( !m_byUserStatus[i] || i == m_wBankerUser ) continue;
		if( m_lTableScore[i] == 0 )
		{
			bFinishScore = false;
			break;
		}
	}
	if( !bFinishScore ) return true;

	SendGamePlay();

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if( !bLookonUser && m_wBankerUser == INVALID_CHAIR )
	{
		m_wBankerUser = wChairID;
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if( !bLookonUser && m_wBankerUser == wChairID )
	{
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			IServerUserItem *pUserItem = m_pITableFrame->GetTableUserItem(i);
			if( pUserItem )
			{
				m_wBankerUser = i;
				break;
			}
		}
		if( i == GAME_PLAYER ) m_wBankerUser = INVALID_CHAIR;
	}

	return true;
}

//��ʼ����
VOID CTableFrameSink::SendGamePlay()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_TH_PLAY);

	//���ȡ��
	m_GameLogic.RandCardList(m_cbRepertoryCard,sizeof(m_byHandCardData));

	//ȷ����ǰ���
	m_wCurrentUser = (m_wBankerUser+1)%GAME_PLAYER;
	while( !m_byUserStatus[m_wCurrentUser] ) m_wCurrentUser = (m_wCurrentUser+1)%GAME_PLAYER;

	m_bySendCardCount = 0;
	//���������
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if( !m_byUserStatus[i] ) continue;
		m_byHandCardData[i][0] = m_cbRepertoryCard[m_bySendCardCount++];
		m_byCardCount[i] = 1;
	}

	//��������
	CMD_S_GamePlay GamePlay = {};
	GamePlay.wCurrentUser = m_wCurrentUser;
	for( WORD i = 0; i < GAME_PLAYER; i++ )
		GamePlay.lTableScore[i] = m_lTableScore[i];

	//��������
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if( m_byUserStatus[i] )
		{
			GamePlay.byCardData[i] = m_byHandCardData[i][0];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
			GamePlay.byCardData[i] = 0;
			m_pITableFrame->SendLookonData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
