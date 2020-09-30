#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����

//�궨��
#define	GOOD_CARD_PERCENT				4									//���Ƹ���

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wBombTime=1;
	m_bLandScore=0;
	m_wFirstUser=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_bScoreInfo,sizeof(m_bScoreInfo));

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));
	ZeroMemory(m_bUserTrustee,  sizeof( m_bUserTrustee ) );
	ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));

	//�˿���Ϣ
	ZeroMemory(m_bBackCard,sizeof(m_bBackCard));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bOutCardCount,sizeof(m_bOutCardCount));
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
	
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
VOID *  CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
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

	//��ѯ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	return true;
}

//��λ����
VOID  CTableFrameSink::RepositionSink()
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
	ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));

	//�˿���Ϣ
	ZeroMemory(m_bBackCard,sizeof(m_bBackCard));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bOutCardCount,sizeof(m_bOutCardCount));
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));

	return;
}

//�û�����
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false) m_HistoryScore.OnEventUserEnter(wChairID);

	return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false) m_HistoryScore.OnEventUserLeave(wChairID);

	return true;
}
//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_WK_SCORE);

	BYTE cbSendGoodCard = rand () % GOOD_CARD_PERCENT ;
	BYTE cbAndroidUserCount = 0 ;
	WORD wAndroidUser[ GAME_PLAYER ] = {0};

	for ( BYTE i = 0; i < GAME_PLAYER; ++i )
	{
		wAndroidUser[i]=INVALID_CHAIR;
	}
	//�����˸���
	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	{
		IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem( wChairID ) ;
		if ( pServerUserItem->IsAndroidUser() )
		{
			wAndroidUser[ cbAndroidUserCount ] = ( wChairID ) ;
			++cbAndroidUserCount ;
		}
	}

	WORD wHaveGoodCardAndroidUser = INVALID_CHAIR ;
	if ( 0 < cbAndroidUserCount ) wHaveGoodCardAndroidUser = wAndroidUser[ rand() % cbAndroidUserCount ] ;

	//���ͺ���
	if ( 1 == cbSendGoodCard && wHaveGoodCardAndroidUser != INVALID_CHAIR )
	{
		//�����˿�
		BYTE bRandCard[FULL_COUNT];
		m_GameLogic.RandCardList( bRandCard, sizeof( bRandCard ) / sizeof( bRandCard[ 0 ] ) );

		BYTE cbGoodCard[ NORMAL_COUNT ] ;
		m_GameLogic.GetGoodCardData( cbGoodCard ) ;

		//��ȡ����
		m_GameLogic.RemoveGoodCardData( cbGoodCard, NORMAL_COUNT, bRandCard, FULL_COUNT ) ;

		//�ַ��˿�
		CopyMemory( m_bHandCardData[ wHaveGoodCardAndroidUser ], cbGoodCard, NORMAL_COUNT ) ;
		m_bCardCount[ wHaveGoodCardAndroidUser ] = NORMAL_COUNT;
		for ( WORD i = 0, j = 0; i < m_wPlayerCount; i++ )
		{
			if ( i != wHaveGoodCardAndroidUser )
			{
				m_bCardCount[ i ] = NORMAL_COUNT;
				CopyMemory( &m_bHandCardData[ i ], &bRandCard[ j * NORMAL_COUNT ], sizeof( BYTE ) * NORMAL_COUNT );
				++j ;
			}
		}
		CopyMemory( m_bBackCard, &bRandCard[ 75 ], sizeof( m_bBackCard ) );
		m_GameLogic.SortCardList( m_bBackCard, 8, ST_ORDER );
	}
	else
	{
		//�����˿�
		BYTE bRandCard[ 108 ];
		m_GameLogic.RandCardList( bRandCard, sizeof( bRandCard ) / sizeof( bRandCard[ 0 ] ) );

		//�ַ��˿�
		for ( WORD i = 0; i < m_wPlayerCount; i++ )
		{
			m_bCardCount[ i ] = NORMAL_COUNT;
			CopyMemory( &m_bHandCardData[ i ], &bRandCard[ i * m_bCardCount[ i ] ], sizeof( BYTE ) * m_bCardCount[ i ] );
		}
		CopyMemory( m_bBackCard, &bRandCard[ 100 ], sizeof( m_bBackCard ) );
		m_GameLogic.SortCardList( m_bBackCard, 8, ST_ORDER );
	}

	//�����û�
	m_wCurrentUser = rand() % GAME_PLAYER;
	m_wFirstUser=m_wCurrentUser;

	//�����˿�
	CMD_S_SendAllCard SendAllCard ;
	SendAllCard.wCurrentUser = m_wCurrentUser ;

	//�����˿�
	CopyMemory( SendAllCard.bCardData, m_bHandCardData, sizeof( m_bHandCardData ) ) ;
	//�����˿�
	CopyMemory( SendAllCard.bBackCardData, m_bBackCard, 8 ) ;
	for ( WORD i = 0; i < m_wPlayerCount; i++ )
	{
		m_pITableFrame->SendTableData( i, SUB_S_SEND_CARD, &SendAllCard, sizeof( SendAllCard ) );
		m_pITableFrame->SendLookonData( i, SUB_S_SEND_CARD, &SendAllCard, sizeof( SendAllCard ) );
		//ZeroMemory(SendAllCard.bCardData[i],sizeof(SendAllCard.bCardData[i]));
	}

	//�����˿�
	for ( WORD i = 0; i < m_wPlayerCount; i++ )
	{
		m_GameLogic.SortCardList( m_bHandCardData[ i ], m_bCardCount[ i ], ST_ORDER );
	}

	return true;
}

//��Ϸ����
bool  CTableFrameSink::OnEventGameConclude( WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason )
{
	switch ( cbReason )
	{
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//Ч�����
			ASSERT( pIServerUserItem != NULL );
			ASSERT( wChairID < m_wPlayerCount );

			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory( &GameEnd,sizeof( GameEnd ) );

			//ʣ���˿�
			BYTE bCardPos = 0;
			for ( WORD i = 0; i < m_wPlayerCount; i++ )
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
	case GER_NORMAL:		//�������
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory( &GameEnd, sizeof( GameEnd ) );

			//ʣ���˿�
			BYTE bCardPos = 0;
			for ( WORD i = 0; i < m_wPlayerCount; i++ )
			{
				GameEnd.bCardCount[ i ] = m_bCardCount[ i ];
				CopyMemory( &GameEnd.bCardData[ bCardPos ], m_bHandCardData[ i ], m_bCardCount[ i ] * sizeof( BYTE ) );
				bCardPos += m_bCardCount[ i ];
			}

			//��������
			LONGLONG lCellScore = m_pGameServiceOption->lCellScore;
			bool bLandWin = ( m_bCardCount[ m_wBankerUser ] == 0 ) ? true : false;

			//�����ж�
			if ( wChairID == m_wBankerUser )
			{
				WORD wUser1 = ( m_wBankerUser + 1 ) % GAME_PLAYER;
				WORD wUser2 = ( m_wBankerUser + 2 ) % GAME_PLAYER;
				WORD wUser3 = ( m_wBankerUser + 3 ) % GAME_PLAYER;
				if ( ( m_bOutCardCount[ wUser1 ] == 0 ) && ( m_bOutCardCount[ wUser2 ] == 0 ) && ( m_bOutCardCount[ wUser3 ] == 0 ) ) m_wBombTime *= 2;
			}
			else
			{
				if ( m_bOutCardCount[ m_wBankerUser ] == 1 ) m_wBombTime *= 2;
			}

			//ը������
			m_wBombTime = __min( m_wBombTime, 16 );

			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			//ͳ�ƻ���
			for ( WORD i = 0; i < m_wPlayerCount; i++ )
			{
				LONGLONG lScore=0;
				LONGLONG lRevenue=0;


				//ͳ�ƻ���
				if ( i == m_wBankerUser )
				{
					lScore = m_wBombTime * m_bLandScore * lCellScore * ( ( bLandWin == true ) ? 3 : -3 );
					GameEnd.lGameScore[ i ] = m_wBombTime * m_bLandScore * lCellScore * ( ( bLandWin == true ) ? 3 : -3 );
				}
				else 
				{
					lScore = m_wBombTime * m_bLandScore * lCellScore * ( ( bLandWin == true ) ? -1 : 1 );
					GameEnd.lGameScore[ i ] = m_wBombTime * m_bLandScore * lCellScore * ( ( bLandWin == true ) ? -1 : 1 );
				}


				//����˰��
				if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
				{
					if (GameEnd.lGameScore[i]>=100L)
					{
						//����˰��
						GameEnd.lGameTax+=(GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenueRatio/1000L);
						lRevenue = (GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenueRatio/1000L);

						//���ֵ���
						lScore=lScore-lRevenue ;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]-lRevenue ;
					}
				}
					
				ScoreInfo[i].cbType=GameEnd.lGameScore[i]>0?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				ScoreInfo[i].lRevenue=lRevenue;
				ScoreInfo[i].lScore=GameEnd.lGameScore[i];
				//��ʷ����
				m_HistoryScore.OnEventUserScore(i,GameEnd.lGameScore[i]);
			}

			//�޸ķ���
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
	case GER_USER_LEAVE:	//�û�ǿ��
	case GER_NETWORK_ERROR:	//�����ж�
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//ը������
			m_wBombTime=__min(m_wBombTime,16);
			m_bLandScore=__max(m_bLandScore,1);

			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			//�޸Ļ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//��������
				LONGLONG lScore=0;
				//�������
				if (i==wChairID)
				{

					lScore	 =-m_wBombTime*m_bLandScore*m_pGameServiceOption->lCellScore*4;
					GameEnd.lGameScore[i]=-m_wBombTime*m_bLandScore*m_pGameServiceOption->lCellScore*4;
					ScoreInfo[i].cbType=SCORE_TYPE_FLEE;
					ScoreInfo[i].lScore=GameEnd.lGameScore[i];
				}				
				else
				{
					ScoreInfo[i].cbType=SCORE_TYPE_DRAW;
				}

				//��ʷ����
				m_HistoryScore.OnEventUserScore(i,GameEnd.lGameScore[i]);

			}
			//д�����
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

			//������Ϣ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(i!=wChairID)
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_WK_FREE);

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
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
			StatusPlay.lBaseScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.bBackCard,m_bBackCard,sizeof(m_bBackCard));
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
			CopyMemory(StatusPlay.cbMagicCardData,m_cbMagicCardData,m_bTurnCardCount*sizeof(BYTE));
	//		CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);
			CopyMemory(StatusPlay.bCardData,m_bHandCardData,sizeof(BYTE)*sizeof(m_bHandCardData));
			CopyMemory( StatusPlay.bUserTrustee, m_bUserTrustee, sizeof( m_bUserTrustee ) );
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

	return false;
}

//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter)
{
	return false;
}

//��Ϸ��Ϣ����
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_LAND_SCORE:	//�û��з�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_LandScore));
			if (wDataSize!=sizeof(CMD_C_LandScore)) return false;

			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_LandScore * pLandScore=(CMD_C_LandScore *)pData;
			return OnUserLandScore(pIServerUserItem->GetChairID(),pLandScore->bLandScore);
		}
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
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			return OnUserOutCard(pIServerUserItem->GetChairID(),pOutCard->bCardData,pOutCard->bCardCount);
		}
	case SUB_C_PASS_CARD:
		{
			//�û�Ч��
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
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

	return true;
}

//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�з��¼�
bool CTableFrameSink::OnUserLandScore(WORD wChairID, BYTE bLandScore)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_SCORE) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//Ч�����
	if (((bLandScore>3)&&(bLandScore!=255))||(bLandScore<=m_bLandScore)) return false;

	//���ñ���
	if (bLandScore!=255)
	{
		m_bLandScore=bLandScore;
		m_wBankerUser=m_wCurrentUser;
	}
	m_bScoreInfo[wChairID]=bLandScore;

	//��ʼ�ж�
	if ((m_bLandScore==3)||(m_wFirstUser==(wChairID+1)%m_wPlayerCount))
	{
		//���·���
		if (m_bLandScore==0) 
		{
			BYTE cbSendGoodCard = rand() % GOOD_CARD_PERCENT ;
			BYTE cbAndroidUserCount=0 ;
			WORD wAndroidUser[GAME_PLAYER]={0};

			//���ñ���
			ZeroMemory(m_bScoreInfo, sizeof(m_bScoreInfo)) ;

			//�����˸���
			for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
			{
				IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(wChairID) ;
				if(pServerUserItem->IsAndroidUser())
				{
					wAndroidUser[cbAndroidUserCount] = (wChairID) ;
					++cbAndroidUserCount ;

				}
			}

			WORD wHaveGoodCardAndroidUser = INVALID_CHAIR ;
			if(cbAndroidUserCount>0) wHaveGoodCardAndroidUser = wAndroidUser[rand()%cbAndroidUserCount] ;

			//���ͺ���
			if( 0 == cbSendGoodCard && wHaveGoodCardAndroidUser!=INVALID_CHAIR)
			{
				//�����˿�
				BYTE bRandCard[FULL_COUNT];
				m_GameLogic.RandCardList(bRandCard,sizeof(bRandCard)/sizeof(bRandCard[0]));

				BYTE cbGoodCard[NORMAL_COUNT] ;
				m_GameLogic.GetGoodCardData(cbGoodCard) ;

				//��ȡ����
				m_GameLogic.RemoveGoodCardData(cbGoodCard, NORMAL_COUNT, bRandCard, FULL_COUNT ) ;

				//�ַ��˿�
				CopyMemory(m_bHandCardData[wHaveGoodCardAndroidUser], cbGoodCard, NORMAL_COUNT) ;
				m_bCardCount[wHaveGoodCardAndroidUser]=NORMAL_COUNT;
				for (WORD i=0, j=0;i<m_wPlayerCount;i++)
				{
					if(i!=wHaveGoodCardAndroidUser)
					{
						m_bCardCount[i]=NORMAL_COUNT;
						CopyMemory(&m_bHandCardData[i],&bRandCard[j*NORMAL_COUNT],sizeof(BYTE)*NORMAL_COUNT);
						++j ;
					}
				}
				CopyMemory(m_bBackCard,&bRandCard[75],sizeof(m_bBackCard));
				m_GameLogic.SortCardList(m_bBackCard,8,ST_ORDER);
			}
			else
			{
				//�����˿�
				BYTE bRandCard[FULL_COUNT];
				m_GameLogic.RandCardList(bRandCard,sizeof(bRandCard)/sizeof(bRandCard[0]));

				//�ַ��˿�
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					m_bCardCount[i]=NORMAL_COUNT;
					CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
				}
				CopyMemory(m_bBackCard,&bRandCard[100],sizeof(m_bBackCard));
				m_GameLogic.SortCardList(m_bBackCard,8,ST_ORDER);
			}

			//�����û�
			m_wCurrentUser = rand() % GAME_PLAYER;
			m_wFirstUser=m_wCurrentUser;

			//�����˿�
			CMD_S_SendAllCard SendAllCard ;
			SendAllCard.wCurrentUser = m_wCurrentUser ;

			//�����˿�
			CopyMemory(SendAllCard.bCardData, m_bHandCardData, sizeof(m_bHandCardData)) ;
			//�����˿�
			CopyMemory(SendAllCard.bBackCardData, m_bBackCard, 8) ;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendAllCard,sizeof(SendAllCard));
				m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendAllCard,sizeof(SendAllCard));
				//ZeroMemory(SendAllCard.bCardData[i],sizeof(SendAllCard.bCardData[i]));
			}

			//�����˿�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i],ST_ORDER);
			}

			return true;
		}

		//���ñ���
		if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wFirstUser;

		//����״̬
		m_pITableFrame->SetGameStatus(GS_WK_PLAYING);

		//���͵���
		m_bCardCount[m_wBankerUser]=MAX_COUNT;
		CopyMemory(&m_bHandCardData[m_wBankerUser][NORMAL_COUNT],m_bBackCard,sizeof(m_bBackCard));
		m_GameLogic.SortCardList(m_bHandCardData[m_wBankerUser],m_bCardCount[m_wBankerUser],ST_ORDER);

		//������Ϣ
		m_bTurnCardCount=0;
		m_wTurnWiner=m_wBankerUser;
		m_wCurrentUser=m_wBankerUser;

		//������Ϣ
		CMD_S_GameStart GameStart;
		GameStart.wLandUser=m_wBankerUser;
		GameStart.bLandScore=m_bLandScore;
		GameStart.wCurrentUser=m_wCurrentUser;
		CopyMemory(GameStart.bBackCard,m_bBackCard,sizeof(m_bBackCard));
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

		return true;
	}

	//�����û�
	m_wCurrentUser=(wChairID+1)%m_wPlayerCount;

	//������Ϣ
	CMD_S_LandScore LandScore;
	LandScore.bLandUser=wChairID;
	LandScore.bLandScore=bLandScore;
	LandScore.wCurrentUser=m_wCurrentUser;
	LandScore.bCurrentScore=m_bLandScore;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//�����˿�
	m_GameLogic.SortCardList( bCardData, bCardCount, ST_ORDER );

	//����˿�
	BYTE cbMagicCardData[MAX_COUNT];
	m_GameLogic.MagicCardData(bCardData,bCardCount,cbMagicCardData,0);

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
	if ((bCardType==CT_BOMB_CARD) && ( bCardCount == 6 || bCardCount == 7 )  ) m_wBombTime*=2;
	if ((bCardType==CT_BOMB_CARD) && bCardCount == 8 || bCardType == CT_MISSILE_CARD ) m_wBombTime*=3;
	
	//�л��û�
	m_wTurnWiner=wChairID;
	if (m_bCardCount[wChairID]!=0)
	{
		if (bCardType!=CT_MISSILE_CARD) m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	}
	else m_wCurrentUser=INVALID_CHAIR;

	//��������
	CMD_S_OutCard OutCard;
	OutCard.bCardCount=bCardCount;
	OutCard.wOutCardUser=wChairID;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));

	//��������
	WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//�������
	if (bCardType==CT_MISSILE_CARD) m_bTurnCardCount=0;

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
	m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	if (m_wCurrentUser==m_wTurnWiner) m_bTurnCardCount=0;

	//��������
	CMD_S_PassCard PassCard;
	PassCard.wPassUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.bNewTurn=(m_bTurnCardCount==0)?TRUE:FALSE;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	return true;
}

//////////////////////////////////////////////////////////////////////////
