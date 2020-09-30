#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

#define IDI_GAME_END				1										//������ʶ
#define IDI_PASS_CARD				2										//������ʶ

#define TIME_PASS_CARD				50000									//

#define SHOW_HAND_SCORE				0x7FFFFFFFFFFFFFFF								//

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wOperaCount=0;
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	m_wLastMostUser = INVALID_CHAIR;
	m_bGameEnd = false;

	//�û�״̬
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));
	ZeroMemory(m_lLostScore,sizeof(m_lLostScore));

	//�˿˱���
	m_cbSendCardCount=0;
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע��Ϣ
	m_lMaxScore = 0L;
	m_lCellScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;
	m_lShowHandScore = 0L;

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ
void *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);
	//��Ϸ����
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	m_wOperaCount=0;
	m_bShowHand=false;
	m_wCurrentUser=INVALID_CHAIR;
	m_wLastMostUser = INVALID_CHAIR;
	m_bGameEnd = false;

	//�û�״̬
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));
	ZeroMemory(m_lLostScore,sizeof(m_lLostScore));

	//�˿˱���
	m_cbSendCardCount=0;
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע��Ϣ
	m_lMaxScore = 0L;
	m_lCellScore=0L;
	m_lTurnMaxScore=0L;
	m_lTurnLessScore=0L;
	m_lShowHandScore=0L;

	return;
}

//��Ϸ״̬
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	ASSERT(wChairID<m_wPlayerCount);
	return (m_cbPlayStatus[wChairID]==TRUE)?true:false;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GAME_STATUS_PLAY);

	//�����ע
	m_lShowHandScore = m_pGameServiceOption->lRestrictScore==0?SHOW_HAND_SCORE:m_pGameServiceOption->lRestrictScore;

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;

		//��ȡ����
		ASSERT(pIServerUserItem->GetUserScore()>=m_pGameServiceOption->lCellScore);

		//���ñ���
		m_cbPlayStatus[i]=TRUE;
		m_lUserMaxScore[i]=__min(pIServerUserItem->GetUserScore(),m_lShowHandScore);
		if ((m_lMaxScore==0L)||(m_lUserMaxScore[i]<m_lMaxScore)) m_lMaxScore=m_lUserMaxScore[i];
	}
	//��ע����
	m_lTurnMaxScore=m_lMaxScore/2L;
	m_lCellScore=__max(m_lMaxScore/100L,m_pGameServiceOption->lCellScore);
	m_lTurnLessScore=m_lCellScore;
	

	//�ַ��˿�
	m_cbSendCardCount=2;
	m_GameLogic.RandCardList(m_cbHandCardData[0],sizeof(m_cbHandCardData)/sizeof(m_cbHandCardData[0][0]));

	//�û�����
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			m_lTableScore[2*i+1]=m_lCellScore;
			m_cbCardCount[i]=m_cbSendCardCount;
		}
	}

	//�����û�
	m_wOperaCount=0;
	m_wCurrentUser=EstimateWinner(1,1);
	m_wLastMostUser = m_wCurrentUser;

	//��������
	CMD_S_GameStart GameStart;
	GameStart.lCellScore=m_lCellScore;
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.lTurnMaxScore=m_lTurnMaxScore;
	GameStart.lTurnLessScore=m_lTurnLessScore;
	GameStart.lShowHandScore = m_lShowHandScore;

	//�����˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==FALSE) GameStart.cbCardData[i]=0;
		else GameStart.cbCardData[i]=m_cbHandCardData[i][1];
	}

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		GameStart.cbObscureCard=0;
		if (m_cbPlayStatus[i]==TRUE)
		{
			GameStart.cbObscureCard=m_cbHandCardData[i][0];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		GameStart.cbObscureCard=0;
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	//����ʱ��
	m_pITableFrame->SetGameTimer(IDI_PASS_CARD,TIME_PASS_CARD,1,m_wCurrentUser);

	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
	case GER_NO_PLAYER:		//û�����
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//��������
			WORD wWinerUser=EstimateWinner(0,4);
			LONGLONG lMaxLostScore=m_lTableScore[wWinerUser*2]+m_lTableScore[wWinerUser*2+1];

			//ͳ����Ϣ
			LONGLONG lWinnerScore=0L;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//������Ϣ
				if ((cbReason==GER_NO_PLAYER)||(m_cbPlayStatus[i]==FALSE)) GameEnd.cbCardData[i]=0;
				else GameEnd.cbCardData[i]=m_cbHandCardData[i][0];

				//������Ϣ
				if (i!=wWinerUser)
				{
					LONGLONG lLostScore=m_lTableScore[i*2]+m_lTableScore[i*2+1];
					if (m_cbPlayStatus[i]==TRUE)
					{
						lWinnerScore+=__min(lLostScore,lMaxLostScore);
						GameEnd.lGameScore[i]=-__min(lLostScore,lMaxLostScore);
					}
					else
					{
						lWinnerScore+=-m_lLostScore[i];
						GameEnd.lGameScore[i]=m_lLostScore[i];
					}
				}
			}

			//ʤ�ߵ÷�
			GameEnd.lGameScore[wWinerUser]=lWinnerScore;
			
			//��˰
			if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0)
			{
				GameEnd.lGameTax[wWinerUser]=m_pITableFrame->CalculateRevenue(wWinerUser,GameEnd.lGameScore[wWinerUser]);
				GameEnd.lGameScore[wWinerUser] -= GameEnd.lGameTax[wWinerUser];
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));


			tagScoreInfo UserScore[GAME_PLAYER];
			ZeroMemory(UserScore,sizeof(UserScore));

			//�޸Ļ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if( m_cbPlayStatus[i] == TRUE )
				{	
					UserScore[i].lScore=GameEnd.lGameScore[i];
					UserScore[i].lRevenue=GameEnd.lGameTax[i];
					UserScore[i].cbType=SCORE_TYPE_DRAW;
					if(GameEnd.lGameScore[i]>0) UserScore[i].cbType=SCORE_TYPE_WIN;
					else UserScore[i].cbType=SCORE_TYPE_LOSE;
				}
			}

			m_pITableFrame->WriteTableScore(UserScore,CountArray(UserScore));
			
			//ɾ��ʱ��
			m_pITableFrame->KillGameTimer(IDI_GAME_END);
			m_pITableFrame->KillGameTimer(IDI_PASS_CARD);

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

			return true;
		}
	case GER_USER_LEAVE:		//�û�ǿ��
	case GER_NETWORK_ERROR:		//�����ж�
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			if( m_cbPlayStatus[wChairID] )
				OnUserGiveUp(wChairID,true);

			return true;
		}
	case GER_DISMISS:
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//ɾ��ʱ��
			m_pITableFrame->KillGameTimer(IDI_GAME_END);
			m_pITableFrame->KillGameTimer(IDI_PASS_CARD);

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

			return true;
		}
	}

	return false;
}

//���ͳ���
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
			StatusFree.lCellScore=0L;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GAME_STATUS_PLAY:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//��־����
			StatusPlay.bShowHand=(m_bShowHand==true)?TRUE:FALSE;

			//��ע��Ϣ
			StatusPlay.lCellScore=m_lCellScore;
			StatusPlay.lTurnMaxScore=m_lTurnMaxScore;
			StatusPlay.lTurnLessScore=m_lTurnLessScore;
			StatusPlay.lShowHandScore = m_lShowHandScore;

			//���ñ���
			StatusPlay.wCurrentUser=m_wCurrentUser;
			CopyMemory(StatusPlay.lTableScore,m_lTableScore,sizeof(m_lTableScore));
			CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));

			//�����˿�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (m_cbPlayStatus[i]==TRUE)
				{
					StatusPlay.cbCardCount[i]=m_cbCardCount[i];
					if ((i==wChiarID)&&(pIServerUserItem->GetUserStatus()!=US_LOOKON)) StatusPlay.cbHandCardData[i][0]=m_cbHandCardData[i][0];
					else StatusPlay.cbHandCardData[i][0]=0;
					CopyMemory(&StatusPlay.cbHandCardData[i][1],&m_cbHandCardData[i][1],(m_cbCardCount[i]-1)*sizeof(BYTE));
				}
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	//Ч�����
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParam)
{
	switch (dwTimerID)
	{
	case IDI_GAME_END:	//��Ϸ����
		{
			if (m_pITableFrame->GetGameStatus()==GAME_STATUS_PLAY)
			{
				OnEventGameConclude(INVALID_CHAIR,NULL,(BYTE)dwBindParam);
				return true;
			}
		}
	case IDI_PASS_CARD:	//������ע
		{
			if ((dwBindParam==m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR))
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentUser);
				
				//�û��ж�
				if ((pIServerUserItem!=NULL)&&(pIServerUserItem->GetUserStatus()!=US_OFFLINE))
				{
					OnUserGiveUp(m_wCurrentUser);
				}
				else m_pITableFrame->SetGameTimer(IDI_PASS_CARD,TIME_PASS_CARD,1,m_wCurrentUser);

				return true;
			}
		}
	}

	return false;
}

//��Ϸ��Ϣ����
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_ADD_SCORE:			//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) return false;

			//��������
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pDataBuffer;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return false;

			//��Ϣ����
			return OnUserAddScore(pUserData->wChairID,pAddScore->lScore,false);
		}
	case SUB_C_GIVE_UP:				//�û�����
		{
			//�û�Ч��
			tagUserInfo* pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return false;

			//��Ϣ����
			return OnUserGiveUp(pUserData->wChairID);
		}
	}

	return false;
}

//�����Ϣ����
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID,VOID* pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool  CTableFrameSink::OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	if (pIServerUserItem->GetChairID()==m_wCurrentUser)
	{
		m_pITableFrame->KillGameTimer(IDI_PASS_CARD);
	}

	return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	if (pIServerUserItem->GetChairID()==m_wCurrentUser)
	{
		m_pITableFrame->SetGameTimer(IDI_PASS_CARD,TIME_PASS_CARD,1,m_wCurrentUser);
	}

	return true;
}

//�����¼�
bool CTableFrameSink::OnUserGiveUp(WORD wChairID,bool bLeave)
{
	//Ч��
	ASSERT( m_cbPlayStatus[wChairID] );
	if( !m_cbPlayStatus[wChairID] ) return false;

	if( m_bGameEnd )
	{
		m_pITableFrame->KillGameTimer( IDI_GAME_END );
		return OnEventGameConclude( INVALID_CHAIR,NULL,GER_NORMAL );
	}

	//��������
	m_cbCardCount[wChairID]=0;
	m_cbPlayStatus[wChairID]=FALSE;

	//����۷�
	LONGLONG lScore = m_lTableScore[wChairID*2]+m_lTableScore[wChairID*2+1];
	lScore = -__min( m_lMaxScore,lScore );

	m_lLostScore[wChairID] = lScore;

	//���»�ȡ�����ע
	if( m_lUserMaxScore[wChairID] == m_lMaxScore )
	{
		for( WORD i = 0; i < GAME_PLAYER; i ++ )
		{
			if( !m_cbPlayStatus[i] ) continue;
			m_lMaxScore = m_lUserMaxScore[i];
			break;
		}
		for( ; i < GAME_PLAYER; i++ )
		{
			if( m_cbPlayStatus[i] )
				m_lMaxScore = __min(m_lMaxScore,m_lUserMaxScore[i]);
		}
		m_lTurnMaxScore = m_cbSendCardCount>=3?m_lMaxScore:m_lMaxScore/2;
	}

	//������Ϣ
	CMD_S_GiveUp GiveUp;
	GiveUp.wGiveUpUser=wChairID;
	GiveUp.lTurnMaxScore = m_lTurnMaxScore;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	//д�����
	tagScoreInfo ScoreInfo[GAME_PLAYER];
	ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
	ScoreInfo[wChairID].cbType=SCORE_TYPE_FLEE;
	ScoreInfo[wChairID].lScore=m_lLostScore[wChairID];
	m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

	//����ͳ��
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}

	//�жϽ���
	if (wPlayerCount>=2)
	{
		if (m_wCurrentUser==wChairID) OnUserAddScore(wChairID,0L,true);
	}
	else
	{
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NO_PLAYER);
	}

	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserAddScore(WORD wChairID, LONGLONG lScore, bool bGiveUp)
{
	//Ч��
	if( m_bGameEnd )
	{
		m_pITableFrame->KillGameTimer( IDI_GAME_END );
		return OnEventGameConclude( INVALID_CHAIR,NULL,GER_NORMAL );
	}

	m_pITableFrame->KillGameTimer(IDI_PASS_CARD);

	//��������
	if (bGiveUp==false)
	{
		//״̬Ч��
		ASSERT(m_wCurrentUser==wChairID);
		if (m_wCurrentUser!=wChairID) return false;

		//���Ч��
		ASSERT( !m_bShowHand && lScore+m_lTableScore[wChairID*2+1] >= m_lTurnLessScore && lScore+m_lTableScore[wChairID*2+1]<=m_lTurnMaxScore
			|| lScore+m_lTableScore[wChairID*2+1]==m_lUserMaxScore[wChairID]);
		if ( (m_bShowHand || lScore+m_lTableScore[wChairID*2+1] < m_lTurnLessScore || lScore+m_lTableScore[wChairID*2+1] > m_lTurnMaxScore)
			&& lScore+m_lTableScore[wChairID*2+1]!=m_lUserMaxScore[wChairID]) 
			return false;

		m_lTableScore[wChairID*2] = lScore;

		//���ñ���
		if( !m_bShowHand && m_lTableScore[wChairID*2]+m_lTableScore[wChairID*2+1] == m_lUserMaxScore[wChairID] ) 
			m_bShowHand = true;

		m_lTurnLessScore=__max(m_lTableScore[wChairID*2]+m_lTableScore[wChairID*2+1],m_lTurnLessScore);
	}

	//�û��л�
	WORD wNextPlayer=INVALID_CHAIR;
	for (WORD i=0;i<m_wPlayerCount-1;i++)
	{
		//���ñ���
		m_wOperaCount++;
		wNextPlayer=(m_wCurrentUser+i+1)%m_wPlayerCount;

		//�����ж�
		if (m_cbPlayStatus[wNextPlayer]==TRUE) break;
	}
	
	//����ж�
	bool bFinishTurn=false;
	if (m_wOperaCount>=m_wPlayerCount)
	{
		LONGLONG lTableScore=0L;
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if (m_cbPlayStatus[i]==TRUE)
			{
				lTableScore=m_lTableScore[i*2];
				break;
			}
		}
		while (i<m_wPlayerCount)
		{
			if (m_cbPlayStatus[i]==TRUE) 
			{
				if( !m_bShowHand&&m_lTableScore[i*2]!=lTableScore ||
					m_bShowHand&&m_lTableScore[i*2]+m_lTableScore[i*2+1]<m_lUserMaxScore[i] )
					break;
			}
			i++;
		}
		if (i==m_wPlayerCount) bFinishTurn=true;
	}

	//������ע
	if (bFinishTurn==false)
	{
		//�����û�
		m_wCurrentUser=wNextPlayer;

		//��������
		CMD_S_AddScore AddScore;
		AddScore.wAddScoreUser=wChairID;
		AddScore.wCurrentUser=m_wCurrentUser;
		AddScore.lTurnLessScore=m_lTurnLessScore;
		AddScore.lAddScoreCount=m_lTableScore[wChairID*2];
		AddScore.bShowHand = m_bShowHand;

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));

		//����ʱ��
		m_pITableFrame->SetGameTimer(IDI_PASS_CARD,40000L,1,m_wCurrentUser);

		return true;
	}
	else
	{
		//��������
		CMD_S_AddScore AddScore;
		AddScore.wAddScoreUser=wChairID;
		AddScore.wCurrentUser=INVALID_CHAIR;
		AddScore.lTurnLessScore=m_lTurnLessScore;
		AddScore.lAddScoreCount=m_lTableScore[wChairID*2];
		AddScore.bShowHand = m_bShowHand;

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
	}

	//�����ж�
	if (m_cbSendCardCount==MAX_COUNT) 
	{
		m_bGameEnd = true;
		m_pITableFrame->SetGameTimer( IDI_GAME_END,2000,1,GER_NORMAL );
		return true;
	}

	//�ۼƽ��
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_lTableScore[i*2+1]+=m_lTableScore[i*2];
		m_lTableScore[i*2]=0L;
	}

	//���ñ���
	m_wOperaCount=0;

	//�ɷ��˿�
	BYTE cbSourceCount=m_cbSendCardCount;
	if (m_bShowHand==false) m_cbSendCardCount++;
	else m_cbSendCardCount=MAX_COUNT;

	//�ϴ�����û�
	WORD wLastMostUser = m_wLastMostUser;
	//��ǰ�û�
	if (m_bShowHand==false)
	{
		//�û�����
		m_wCurrentUser=EstimateWinner(1,m_cbSendCardCount-1);
		m_wLastMostUser = m_wCurrentUser;
		
		//��ע����
		m_lTurnMaxScore=(m_cbSendCardCount>=3)?m_lMaxScore:m_lMaxScore/2L;
	}
	else m_wCurrentUser=INVALID_CHAIR; 

	//��������
	CMD_S_SendCard SendCard;
	SendCard.wCurrentUser=m_wCurrentUser;
	SendCard.wLastMostUser = wLastMostUser;
	SendCard.lTurnMaxScore=m_lTurnMaxScore;
	SendCard.lTurnLessScore=m_lTurnLessScore;
	SendCard.cbSendCardCount=m_cbSendCardCount-cbSourceCount;

	//�����˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�˿���Ŀ
		if (!m_cbPlayStatus[i]) continue;

		m_cbCardCount[i]=m_cbSendCardCount;

		//�ɷ��˿�
		for (BYTE j=0;j<(m_cbSendCardCount-cbSourceCount);j++)
		{
			ASSERT(j<CountArray(SendCard.cbCardData[i]));
			SendCard.cbCardData[i][j]=m_cbHandCardData[i][cbSourceCount+j];
		}
	}

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	//��������
	if (m_wCurrentUser==INVALID_CHAIR) 
	{
		m_pITableFrame->KillGameTimer(IDI_PASS_CARD);
		m_bGameEnd = true;
		m_pITableFrame->SetGameTimer(IDI_GAME_END,3000,1,0);
	}
	else m_pITableFrame->SetGameTimer(IDI_PASS_CARD,TIME_PASS_CARD,1,m_wCurrentUser);

	return true;
}

//�ƶ�ʤ��
WORD CTableFrameSink::EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos)
{
	//�����˿�
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	//Ѱ�����
	for (WORD wWinner=0;wWinner<m_wPlayerCount;wWinner++)
	{
		if (m_cbPlayStatus[wWinner]==TRUE) 
		{
			//�����˿�
			m_GameLogic.SortCardList(cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1);
			break;
		}
	}

	//�Ա����
	WORD wId = wWinner;
	for (WORD i=0;i<GAME_PLAYER-1;i++)
	{
		wId = (wId+1)%GAME_PLAYER;

		//�û�����
		if (m_cbPlayStatus[wId]==FALSE) continue;

		//�����˿�
		m_GameLogic.SortCardList(cbUserCardData[wId]+cbStartPos,cbConcludePos-cbStartPos+1);

		//�Ա��˿�
		if (m_GameLogic.CompareCard(cbUserCardData[wId]+cbStartPos,cbUserCardData[wWinner]+cbStartPos,cbConcludePos-cbStartPos+1)==true) 
		{
			wWinner=wId;
		}
	}

	return wWinner;
}

//////////////////////////////////////////////////////////////////////////
