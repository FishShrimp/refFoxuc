#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wBombTime=1;
	m_bLandScore=0;
	m_wFirstUser=INVALID_CHAIR;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_bScoreInfo,sizeof(m_bScoreInfo));

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));
	ZeroMemory(m_bUserTrustee,  sizeof( m_bUserTrustee ) );
	ZeroMemory(m_bUserActive,sizeof(m_bUserActive));
	ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));

	//�˿���Ϣ
	ZeroMemory(m_bBackCard,sizeof(m_bBackCard));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bOutCardCount,sizeof(m_bOutCardCount));
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
	m_bLeftCardCount=0;
	ZeroMemory(m_bAllCardData,sizeof(m_bAllCardData));

	m_pITableFrame=NULL;							
	m_pGameServiceOption=NULL;					
	m_pGameServiceAttrib=NULL;					
	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
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
						
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();					
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();	

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{

	//��Ϸ����
	m_wBombTime=1;
	m_bLandScore=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_bScoreInfo,sizeof(m_bScoreInfo));

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));
	ZeroMemory(m_bUserTrustee,  sizeof( m_bUserTrustee ) );

	//�˿���Ϣ
	ZeroMemory(m_bBackCard,sizeof(m_bBackCard));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bOutCardCount,sizeof(m_bOutCardCount));
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
	m_bLeftCardCount=0;
	ZeroMemory(m_bAllCardData,sizeof(m_bAllCardData));
	ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));
	return;
}

//��������
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{

	return (SCORE)0;
}

//���ٻ���
SCORE CTableFrameSink::QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return (SCORE)0;
}
//��ѯ�Ƿ�۷����
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{

	return true;
}
//�����¼�
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return true;
}
//�����¼�
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return true;
}
//���û���
void CTableFrameSink::SetGameBaseScore(LONG lBaseScore)
{

}

//�û�����
bool CTableFrameSink::OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem)
{

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false) m_HistoryScore.OnEventUserEnter(wChairID);
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false) m_HistoryScore.OnEventUserLeave(wChairID);
	return true;
}
//�û�����
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{

	return true;
}

//�û�ͬ��
bool CTableFrameSink::OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) 
{
	return true;

}
//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_WK_PLAYING);

	m_GameLogic.SetCanOutSingleKing(false);

	//����û�
	ZeroMemory(m_bUserActive,sizeof(m_bUserActive));
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_pITableFrame->GetTableUserItem(i)!=NULL)	m_bUserActive[i]=true;
	}

	//�����û�
	if(m_wFirstUser==INVALID_CHAIR)
	{
		do
		{
			m_wFirstUser = rand() % GAME_PLAYER;
		}while(m_bUserActive[m_wFirstUser]!=true);
	}
	else
	{
		do
		{
			m_wFirstUser = (m_wFirstUser+1) % GAME_PLAYER;
		}while(m_bUserActive[m_wFirstUser]!=true);
	}
	m_wCurrentUser=m_wFirstUser;
	m_wBankerUser=m_wCurrentUser;

	//��������
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_bUserActive[i]==true)
		{
			m_bCardCount[i] = 5;
			if(i==m_wBankerUser) m_bCardCount[i] = 6;
		}
	}

	//�����˿�
	BYTE bRandCard[ 54 ];
	m_GameLogic.RandCardList( bRandCard, sizeof( bRandCard ) / sizeof( bRandCard[ 0 ] ) );

	//�ַ��˿�
	BYTE bSendCount=0;		//��������
	for ( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if(m_bUserActive[i]!=true) continue;
		CopyMemory( &m_bHandCardData[i], &bRandCard[bSendCount], sizeof(BYTE)*m_bCardCount[i]);
		bSendCount+=m_bCardCount[i];
	}
	CopyMemory( m_bAllCardData, &bRandCard[bSendCount], sizeof(m_bAllCardData));
	m_bLeftCardCount=54-bSendCount;

	//�����˿�
	for ( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		m_GameLogic.SortCardList( m_bHandCardData[ i ], m_bCardCount[ i ], ST_ORDER );
	}

	CMD_S_GameStart GameStart;
	GameStart.wLandUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.bLeftCardCount=m_bLeftCardCount;
	CopyMemory(GameStart.bUserCardCount,m_bCardCount,sizeof(GameStart.bUserCardCount));

	for ( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if(m_bUserActive[i]!=true) continue;
		GameStart.bCardCount=m_bCardCount[i];
		CopyMemory(&GameStart.bCardData,m_bHandCardData[i],m_bCardCount[i]);
		m_pITableFrame->SendTableData( i, SUB_S_GAME_START, &GameStart, sizeof( GameStart ) );
		m_pITableFrame->SendLookonData( i, SUB_S_GAME_START, &GameStart, sizeof( GameStart ) );
		ZeroMemory(GameStart.bCardData,sizeof(GameStart.bCardData));
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
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory( &GameEnd, sizeof( GameEnd ) );

			//ʣ���˿�
			BYTE bCardPos = 0;
			for ( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				GameEnd.bCardCount[ i ] = m_bCardCount[ i ];
				CopyMemory( &GameEnd.bCardData[ bCardPos ], m_bHandCardData[ i ], m_bCardCount[ i ] * sizeof( BYTE ) );
				bCardPos += m_bCardCount[ i ];
			}

			//��������
			LONGLONG lCellScore = m_pGameServiceOption->lCellScore;

			//�����ж�
			bool bHaoChun=false;
			bool bChunTian[GAME_PLAYER];
			ZeroMemory(bChunTian,sizeof(bChunTian));
			BYTE bActUser=0;
			BYTE bChunTianUser=0;
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_bUserActive[i]==true) bActUser++;
				if(m_bCardCount[i]==5)
				{
					bChunTianUser++;
					bChunTian[i]=true;
				}
			}

			//ׯ��һ�Ѿͳ��꣬�����
			if(  m_bLeftCardCount==(54-(bActUser*5+1))) bHaoChun=true;
			if(bHaoChun==true) m_wBombTime*=8;

			//���
			for(int i=0;i<GAME_PLAYER;i++)
			{
				if(m_bUserActive[i]!=true||i==wChairID) continue;
				if(bChunTian[i]==true && bHaoChun==false) m_wBombTime*=2;
				GameEnd.lGameScore[i]= - m_wBombTime*m_bCardCount[i]*lCellScore;
				GameEnd.lGameScore[wChairID]+= - GameEnd.lGameScore[i];
			}

			//д��
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			for(int i=0;i<GAME_PLAYER;i++)
			{
				if(m_bUserActive[i]!=true) continue;

				ScoreInfo[i].lScore=GameEnd.lGameScore[i];
				ScoreInfo[i].lRevenue=0;
				if(ScoreInfo[i].lScore>0)
				{
					ScoreInfo[i].cbType=SCORE_TYPE_WIN;
					if(m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
						ScoreInfo[i].lRevenue=ScoreInfo[i].lScore*m_pGameServiceOption->wRevenueRatio/1000;
					ScoreInfo[i].lScore-=ScoreInfo[i].lRevenue;
				}
				else if(ScoreInfo[i].lScore==0) 	ScoreInfo[i].cbType=SCORE_TYPE_DRAW;
				else				ScoreInfo[i].cbType=SCORE_TYPE_LOSE;

				GameEnd.lGameScore[i]=ScoreInfo[i].lScore;
				GameEnd.lGameTax+=ScoreInfo[i].lRevenue;
				//��ʷ����
				m_HistoryScore.OnEventUserScore(i,GameEnd.lGameScore[i]);
			}

			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�л��û�
			m_wFirstUser=wChairID;

			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_WK_FREE);
			return true;
		}
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//Ч�����
			ASSERT( pIServerUserItem != NULL );
			ASSERT( wChairID < GAME_PLAYER );

			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory( &GameEnd,  sizeof( GameEnd ) );

			//ʣ���˿�
			BYTE bCardPos = 0;
			for ( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				GameEnd.bCardCount[ i ] = m_bCardCount[ i ];
				CopyMemory( &GameEnd.bCardData[ bCardPos ], m_bHandCardData[ i ], m_bCardCount[ i ] * sizeof( BYTE ) );
				bCardPos += m_bCardCount[ i ];
			}

			//������Ϣ
			m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof( GameEnd ) );
			m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof( GameEnd ) );

			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_WK_FREE);
			return true;
		}
	case GER_USER_LEAVE:	//�û�ǿ��
	case GER_NETWORK_ERROR:	//�����ж�
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<GAME_PLAYER);

			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//ը������
			//m_wBombTime=__min(m_wBombTime,16);
			//m_bLandScore=__max(m_bLandScore,1);
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));

			GameEnd.lGameScore[wChairID]=-3*m_wBombTime*m_bCardCount[wChairID]*m_pGameServiceOption->lCellScore;

			//�޸Ļ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(i==wChairID)
				{
					ScoreInfo[i].cbType=SCORE_TYPE_FLEE;
					ScoreInfo[i].lScore=GameEnd.lGameScore[wChairID];
				}
				else
					ScoreInfo[i].cbType =SCORE_TYPE_DRAW;
			}			

			//��ʷ����
			m_HistoryScore.OnEventUserScore(wChairID,GameEnd.lGameScore[wChairID]);
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_WK_FREE);
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
	case GS_WK_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
			StatusFree.lBaseScore=m_pGameServiceOption->lCellScore;
			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusFree.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusFree.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_WK_SCORE:		//�з�״̬
		{
			//��������
			CMD_S_StatusScore StatusScore;
			ZeroMemory(&StatusScore,sizeof(StatusScore));

			//���ñ���
			StatusScore.bLandScore=m_bLandScore;
			StatusScore.wCurrentUser=m_wCurrentUser;
			StatusScore.lBaseScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusScore.bScoreInfo,m_bScoreInfo,sizeof(m_bScoreInfo));
			//		CopyMemory(StatusScore.bCardData,m_bHandCardData[wChiarID],m_bCardCount[wChiarID]);
			CopyMemory(StatusScore.bCardData,m_bHandCardData, sizeof(m_bHandCardData)*sizeof(BYTE));
			CopyMemory( StatusScore.bUserTrustee, m_bUserTrustee, sizeof( m_bUserTrustee ) );

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusScore.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusScore.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
		}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//���ñ���
			StatusPlay.wBombTime=m_wBombTime;
			StatusPlay.wLandUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.bLandScore=m_bLandScore;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
			StatusPlay.bLeftCardCount=m_bLeftCardCount;
			StatusPlay.lBaseScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.bBackCard,m_bBackCard,sizeof(m_bBackCard));
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
			//		CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);
			CopyMemory(StatusPlay.bCardData,m_bHandCardData,sizeof(BYTE)*sizeof(m_bHandCardData));
			CopyMemory( StatusPlay.bUserTrustee, m_bUserTrustee, sizeof( m_bUserTrustee ) );
			CopyMemory(StatusPlay.bMagicData,m_cbMagicCardData,sizeof(StatusPlay.bMagicData));

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusPlay.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusPlay.lCollectScore[i]=pHistoryScore->lCollectScore;
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

//��Ϸ��Ϣ
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CART:		//�û�����
		{
			//��������
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pData;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->bCardData);

			//Ч������
			ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
			ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0])));
			if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0]))) return false;

			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			return OnUserOutCard(pIServerUserItem->GetChairID(),pOutCard->bCardData,pOutCard->bCardCount);
		}
	case SUB_C_PASS_CARD:
		{
			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;
			//��Ϣ����
			return OnUserPassCard(pIServerUserItem->GetChairID());
		}
	case SUB_C_TRUSTEE:			//����й�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_UserTrustee));
			if (wDataSize != sizeof(CMD_C_UserTrustee)) return false;

			CMD_C_UserTrustee *pUserTrustee = ( CMD_C_UserTrustee * )pData;

			m_bUserTrustee[ pUserTrustee->wUserChairID ] = pUserTrustee->bTrustee;

			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_C_TRUSTEE, (void*)pData,wDataSize);
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_C_TRUSTEE, (void*)pData,wDataSize);

			return true;
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
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//�����˿�
	m_GameLogic.SortCardList(bCardData, bCardCount, ST_ORDER);

	//����˿�
	BYTE cbMagicCardData[MAX_COUNT];
	m_GameLogic.MagicCardData(bCardData,bCardCount,cbMagicCardData,m_cbMagicCardData,m_bTurnCardCount);

	//�����ж�
	BYTE bCardType=m_GameLogic.GetCardType(cbMagicCardData,bCardCount);
	if (bCardType==CT_ERROR) return false;

	//�������
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;
	else if (m_GameLogic.CompareCard(m_cbMagicCardData,cbMagicCardData,m_bTurnCardCount,bCardCount)==false) return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;

	//���Ƽ�¼
	m_bTurnCardCount=bCardCount;
	m_bOutCardCount[wChairID]++;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);
	CopyMemory(m_cbMagicCardData,cbMagicCardData,sizeof(BYTE)*bCardCount);

	//ը���ж�
	if ((bCardType==CT_BOMB_3)||(bCardType==CT_BOMB_3A)) m_wBombTime*=2;
	else if((bCardType==CT_BOMB_KING)&&bCardCount==5)  m_wBombTime*=8;
	else if((bCardType==CT_BOMB_KING)&&bCardCount==6)  m_wBombTime*=16;
	else if ((bCardType==CT_BOMB_4)||(bCardType==CT_BOMB_4A)||(bCardType==CT_BOMB_KING)) m_wBombTime*=4;

	//�л��û�
	m_wTurnWiner=wChairID;
	if (m_bCardCount[wChairID]!=0)
	{
		do
		{
			m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;
		}while(m_bUserActive[m_wCurrentUser]!=true);
	}
	else m_wCurrentUser=INVALID_CHAIR;

	//��������
	CMD_S_OutCard OutCard;
	OutCard.bCardCount=bCardCount;
	OutCard.wOutCardUser=wChairID;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));
	CopyMemory(OutCard.bMagicData,m_cbMagicCardData,m_bTurnCardCount*sizeof(BYTE));
	CopyMemory(OutCard.bUserCardCount,m_bCardCount,sizeof(OutCard.bUserCardCount));

	//��������
	WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameConclude(wChairID,NULL,GER_NORMAL);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;
	if ((wChairID!=m_wCurrentUser)||(m_bTurnCardCount==0)) return false;

	//���ñ���	
	do
	{
		m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;
	}while(m_bUserActive[m_wCurrentUser]!=true);
	if (m_wCurrentUser==m_wTurnWiner) m_bTurnCardCount=0;

	//��������
	CMD_S_PassCard PassCard;
	PassCard.wPassUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;

	//��һ��
	bool bNewTurn=(m_bTurnCardCount==0)?true:false;
	//���һ��û���Ʒ���ʱ�������Ե���
	if(bNewTurn&&m_bLeftCardCount==0) 
	{
		m_GameLogic.SetCanOutSingleKing(true);
		PassCard.bCanOutSingleKing=true;
	}
	if(bNewTurn&&m_bLeftCardCount>0)
	{
		ASSERT(m_bLeftCardCount>0);
		if(m_bLeftCardCount==0) goto pass;

		//����һ����
		m_bCardCount[m_wTurnWiner]+=1;
		m_bHandCardData[m_wTurnWiner][m_bCardCount[m_wTurnWiner]-1]=m_bAllCardData[m_bLeftCardCount-1];
		m_GameLogic.SortCardList(m_bHandCardData[m_wTurnWiner],m_bCardCount[m_wTurnWiner],ST_ORDER);
		m_bLeftCardCount--;
	}
pass:
	PassCard.bLeftCardCount=m_bLeftCardCount;
	PassCard.bNewTurn=bNewTurn;
	CopyMemory(PassCard.bUserCardCount,m_bCardCount,sizeof(m_bCardCount));
	CopyMemory(PassCard.bCardData,m_bHandCardData,sizeof(m_bHandCardData));
	CopyMemory(PassCard.bUserCardCount,m_bCardCount,sizeof(PassCard.bUserCardCount));
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	return true;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
