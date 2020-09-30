#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
//const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_AllReady;	//��ʼģʽ

#define	STOCK_TAX						2									//�ؿۺ���

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbShowHand,sizeof(m_cbShowHand));
	ZeroMemory(m_cbOpenFinish,sizeof(m_cbOpenFinish));
	ZeroMemory(m_bInvestFinish,sizeof(m_bInvestFinish));

	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	m_wOperaCount=0;
	m_cbBalanceCount=0;
	m_lBalanceScore=0;
	m_lTurnMaxScore = 0;
	m_lTurnMinScore = 0;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lLoseScore,sizeof(m_lLoseScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ--��������Ϣ�汾
void *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
#ifdef __SPECIAL___
	QUERYINTERFACE(ITableUserActionEX,Guid,dwQueryVer);	
#endif
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

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);
	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	return true;
}

//��λ����
void  CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbShowHand,sizeof(m_cbShowHand));
	ZeroMemory(m_cbOpenFinish,sizeof(m_cbOpenFinish));
	ZeroMemory(m_bInvestFinish,sizeof(m_bInvestFinish));

	m_wBankerUser=INVALID_CHAIR;				
	m_wCurrentUser=INVALID_CHAIR;				

	m_wOperaCount=0;
	m_cbBalanceCount=0;
	m_lBalanceScore=0;
	m_lTurnMaxScore = 0;
	m_lTurnMinScore = 0;
	ZeroMemory(m_lLoseScore,sizeof(m_lLoseScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lTotalScore,sizeof(m_lTotalScore));

	return;
}

////��ʼģʽ
//enStartMode  CTableFrameSink::GetGameStartMode()
//{
//	return m_GameStartMode;
//}

//��Ϸ״̬
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	ASSERT(wChairID<m_wPlayerCount && (m_cbPlayStatus[wChairID]==TRUE || m_lLoseScore[wChairID]<0));
	if(wChairID<m_wPlayerCount && (m_cbPlayStatus[wChairID]==TRUE || m_lLoseScore[wChairID]<0))return true;

	return false;
}

//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_TK_INVEST);

	//�û�״̬
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��ȡ�û�
		IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(i);

		if(pIServerUser==NULL)
		{
			m_cbPlayStatus[i]=FALSE;
		}
		else
		{
			m_cbPlayStatus[i]=TRUE;
		}
	}

	//�׾����
	if(m_wBankerUser==INVALID_CHAIR)
	{
		m_wBankerUser = rand()%m_wPlayerCount;
	}

	//ȷ��ׯ��
	do
	{
		m_wBankerUser =(m_wBankerUser+1)%m_wPlayerCount;
	}while(m_cbPlayStatus[m_wBankerUser]!=TRUE);

	//��ǰ�û�
	m_wCurrentUser=m_wBankerUser;

	//���ñ���
	CMD_S_GameStart GameStart;
	GameStart.wBankerUser=m_wBankerUser;
	CopyMemory(GameStart.lUserScore,m_lUserScore,sizeof(m_lUserScore));

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_pITableFrame->GetTableUserItem(i)==NULL)continue;
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

	return true;
}

//��Ϸ����
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.cbEndReason=END_REASON_NORMAL;
			CopyMemory(GameEnd.cbCardData,m_cbHandCardData,sizeof(GameEnd.cbCardData));

			//�ж�ǿ��
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i]==FALSE && m_lTotalScore[i]>0)
				{
					m_cbPlayStatus[i]=TRUE;
					ZeroMemory(m_cbHandCardData[i],sizeof(BYTE)*MAX_COUNT);
				}
			}

			//�����÷�
			BYTE cbPlayCount=0;
			do
			{
				//��������
				BYTE cbSpeCount = 2;
				struct LoseInfo
				{
					WORD wPlyObjUser[GAME_PLAYER];
					WORD wPlyObjCount;
				}LoseUserInof[GAME_PLAYER];
				ZeroMemory(LoseUserInof,sizeof(LoseUserInof));

				//�ж����
				BYTE cbLoseUser[GAME_PLAYER];
				ZeroMemory(cbLoseUser,sizeof(cbLoseUser));
				for(WORD i=0;i<m_wPlayerCount;i++)
				{
					if(m_cbPlayStatus[i]==FALSE)continue;

					for(WORD j=0;j<m_wPlayerCount;j++)
					{
						if(m_cbPlayStatus[j]==FALSE || i==j)continue;

						//�Ƚϴ�С
						INT nHeadResult=m_GameLogic.CompareCard(m_cbHandCardData[i],m_cbHandCardData[j],cbSpeCount);
						INT nTailResult=m_GameLogic.CompareCard(&m_cbHandCardData[i][cbSpeCount],&m_cbHandCardData[j][cbSpeCount],cbSpeCount);
						if(nHeadResult==0xff)nHeadResult=-1;
						if(nTailResult==0xff)nTailResult=-1;

						if(nHeadResult+nTailResult<0)
						{
							cbLoseUser[i]=TRUE;
							break;
						}
					}
				}

				//�������
				for(WORD i=0;i<m_wPlayerCount;i++)
				{
					if(m_cbPlayStatus[i]==FALSE || cbLoseUser[i]==FALSE)continue;

					for(WORD t=0;t<m_wPlayerCount;t++)
					{
						//ׯ������
						WORD j=(t+m_wBankerUser)%GAME_PLAYER;
						if(m_cbPlayStatus[j]==FALSE || i==j || cbLoseUser[j]==TRUE)continue;

						//�Ƚϴ�С
						INT nHeadResult=m_GameLogic.CompareCard(m_cbHandCardData[i],m_cbHandCardData[j],cbSpeCount);
						INT nTailResult=m_GameLogic.CompareCard(&m_cbHandCardData[i][cbSpeCount],&m_cbHandCardData[j][cbSpeCount],cbSpeCount);
						if(nHeadResult==0xff)nHeadResult=-1;
						if(nTailResult==0xff)nTailResult=-1;

						if(nHeadResult+nTailResult<0)
						{
							LoseUserInof[i].wPlyObjUser[LoseUserInof[i].wPlyObjCount++] = j;
						}
					}
				}

				//��ҵ÷�
				LONGLONG lUserGetScore[GAME_PLAYER];
				ZeroMemory(lUserGetScore,sizeof(lUserGetScore));
				for(WORD i=0;i<m_wPlayerCount;i++)
				{
					LoseInfo *pLoseInfo = &LoseUserInof[i];
					WORD wPlyCount=pLoseInfo->wPlyObjCount;

					//����÷�
					while(wPlyCount>0 && m_lTotalScore[i]>0)
					{
						//����ͷ��
						WORD wIndex = 0;
						WORD wHeadMaxUser = pLoseInfo->wPlyObjUser[0];
						for(WORD j=1;j<wPlyCount;j++)
						{
							//��ǰ�û�
							WORD wCurUser = pLoseInfo->wPlyObjUser[j];
							if(m_cbPlayStatus[wCurUser]==FALSE)continue;

							//�Ƚϴ�С
							BYTE cbResult=m_GameLogic.CompareCard(m_cbHandCardData[wCurUser],m_cbHandCardData[wHeadMaxUser],cbSpeCount);
							if(cbResult==TRUE)
							{
								wIndex=j;
								wHeadMaxUser=wCurUser;
							}
						}

						//����÷�
						LONGLONG lTempScore=__min(m_lTotalScore[wHeadMaxUser],m_lTotalScore[i]);
						lUserGetScore[wHeadMaxUser]+=lTempScore;
						lUserGetScore[i]-=lTempScore;
						m_lTotalScore[i]-=lTempScore;

						//�жϽ���
						if(m_lTotalScore[i]==0 || wPlyCount==1)break;

						//�����û�
						WORD wTempUser[GAME_PLAYER],wTempCount=0;
						CopyMemory(wTempUser,pLoseInfo->wPlyObjUser,sizeof(WORD)*wPlyCount);
						wTempUser[wIndex]=INVALID_CHAIR;
						for(WORD j=0;j<wPlyCount;j++)
						{
							if(wTempUser[j]!=INVALID_CHAIR)
								pLoseInfo->wPlyObjUser[wTempCount++]=wTempUser[j];
						}
						ASSERT(wTempCount==wPlyCount-1);
						wPlyCount=wTempCount;
					}
				}

				//������
				for(WORD i=0;i<GAME_PLAYER;i++)
				{
					GameEnd.lGameScore[i]+=lUserGetScore[i];
					if(lUserGetScore[i]>=0 || m_lTotalScore[i]==0)
					{
						m_lTotalScore[i]=0;
						m_cbPlayStatus[i]=FALSE;
					}
				}

				//�жϽ���
				cbPlayCount=0;
				for(WORD i=0;i<GAME_PLAYER;i++)if(m_cbPlayStatus[i]!=FALSE)cbPlayCount++;

			}while(cbPlayCount>=2);

			//����˰��
			LONGLONG lRevenue=m_pGameServiceOption->wRevenueRatio;
			for(WORD i=0;i<m_wPlayerCount;i++)
			{
				if(GameEnd.lGameScore[i]>=100)
				{
					GameEnd.lGameTax[i]=GameEnd.lGameScore[i]*lRevenue/1000;
					GameEnd.lGameScore[i]-=GameEnd.lGameTax[i];
				}

				//�û��ܱ�
				if(m_lUserScore[i]>0)m_lUserScore[i]+=GameEnd.lGameScore[i];
			}

			//������Ϣ
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_pITableFrame->GetTableUserItem(i)==NULL)continue;
				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));


			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));

			//д�����
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//�������
				if(m_lLoseScore[i]<0)
				{
					//�۳���ע
					ASSERT(m_pITableFrame->GetTableUserItem(i)!=NULL);
					//m_pITableFrame->WriteUserScore(i,m_lLoseScore[i],0,enScoreKind_Lost);

					ScoreInfo[i].lScore=m_lLoseScore[i];
					ScoreInfo[i].cbType=SCORE_TYPE_LOSE;
					m_lUserScore[i]+=m_lLoseScore[i];
				}

				if(m_pITableFrame->GetTableUserItem(i)==NULL)continue;
				//enScoreKind nScoreKind=enScoreKind_Draw;
				ScoreInfo[i].cbType=SCORE_TYPE_DRAW;
				if(GameEnd.lGameScore[i]!=0)
					ScoreInfo[i].cbType=(GameEnd.lGameScore[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;

				ScoreInfo[i].lScore=GameEnd.lGameScore[i];
				ScoreInfo[i].lRevenue = GameEnd.lGameTax[i];
				//m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],GameEnd.lGameTax[i],nScoreKind);
			}
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

			return true;
		}
	case GER_USER_LEAVE:		//�û�ǿ��
		{
			//��Ϸ״̬
			WORD wGameStatus=m_pITableFrame->GetGameStatus();

			//�û�����
			bool bGiveUp=false;
			if(WORD(wChairID-GAME_PLAYER)<WORD(GAME_PLAYER) && pIServerUserItem!=NULL)
			{
				bGiveUp=true;
				wChairID-=GAME_PLAYER;
			}

			//����ǿ��
			bool bHaveGiveUp=false;
			if(m_cbPlayStatus[wChairID]==FALSE)bHaveGiveUp=true;

			//��������
			m_cbShowHand[wChairID]=FALSE;
			m_cbPlayStatus[wChairID]=FALSE;

			//�û�����
			WORD wUserCount =0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]!=FALSE)wUserCount++;
			}

			//�۳�����
			if(!bGiveUp || wUserCount<2)
			{
				//�����ע
				LONGLONG lMaxScore=0;
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if(m_cbPlayStatus[i]==FALSE || i==wChairID)continue;

					if(lMaxScore<m_lUserScore[i])lMaxScore=m_lUserScore[i];
				}

				//������
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if(m_lLoseScore[i]==0)continue;
					LONGLONG lLose = __max(m_lLoseScore[i],-m_lLoseScore[i]);
					if(lMaxScore<lLose)lMaxScore=lLose;
				}

				//ʵ����ע
				m_lLoseScore[wChairID]=__min(lMaxScore,m_lTotalScore[wChairID]);

				tagScoreInfo ScoreInfo[GAME_PLAYER];
				ZeroMemory(ScoreInfo,sizeof(ScoreInfo));

				//�۳���ע
				LONGLONG lLoseScore=-m_lLoseScore[wChairID];

				ScoreInfo[wChairID].lScore = lLoseScore;
				ScoreInfo[wChairID].cbType = SCORE_TYPE_LOSE;
				//m_pITableFrame->WriteUserScore(wChairID,lLoseScore,0,enScoreKind_Lost);
				m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
				m_lUserScore[wChairID]-=m_lLoseScore[wChairID];
			}
			else 
			{
				m_lLoseScore[wChairID]=-m_lTotalScore[wChairID];
			}

			//������Ϣ
			if(!bHaveGiveUp)
			{
				CMD_S_GiveUp GiveUp;
				ZeroMemory(&GiveUp,sizeof(GiveUp));
				GiveUp.wGiveUpUser=wChairID;
				GiveUp.wGameStatus= wGameStatus;
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
			}

			//������Ϸ
			if(wUserCount<2)
			{
				//��������
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));
				GameEnd.cbEndReason=END_REASON_EXIT;

				//ʤ�����
				WORD wWinner=0;
				for(WORD i=0;i<m_wPlayerCount;i++)
				{
					if(m_cbPlayStatus[i]!=FALSE)
					{
						wWinner=i;
						break;
					}
				}

				
				tagScoreInfo ScoreInfo[GAME_PLAYER];
				ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
				//��ע��Ŀ
				LONGLONG lAllGold=0,lCount=0;
				for(WORD i=0;i<m_wPlayerCount;i++)
				{
					if(m_lLoseScore[i]!=0 && i!=wWinner)
					{
						lCount++;
						LONGLONG lLose = __max(m_lLoseScore[i],-m_lLoseScore[i]);
						lAllGold+=lLose;
						GameEnd.lGameScore[i]=-lLose;

						//�������
						if(m_lLoseScore[i]<0)
						{
							
							ScoreInfo[i].lScore = m_lLoseScore[i];
							ScoreInfo[i].cbType = SCORE_TYPE_LOSE;
							
							//�۳���ע
							//m_pITableFrame->WriteUserScore(i,m_lLoseScore[i],0,enScoreKind_Lost);
							m_lUserScore[i]+=m_lLoseScore[i];
							m_lLoseScore[i]*=-1;
						}
					}
				}
				//m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

				//ʵ��Ӯע
				lAllGold = __min(lAllGold,m_lUserScore[wWinner]*lCount);

				//ʤ���÷�
				LONGLONG lRevenue=m_pGameServiceOption->wRevenueRatio;
				GameEnd.lGameTax[wWinner]=lAllGold*lRevenue/1000;
				GameEnd.lGameScore[wWinner]=lAllGold-GameEnd.lGameTax[wWinner];
				
				
				ScoreInfo[wWinner].lScore = GameEnd.lGameScore[wWinner];
				ScoreInfo[wWinner].cbType = SCORE_TYPE_WIN;
				ScoreInfo[wWinner].lRevenue = GameEnd.lGameTax[wWinner];

				//m_pITableFrame->WriteUserScore(wWinner,GameEnd.lGameScore[wWinner],GameEnd.lGameTax[wWinner],enScoreKind_Win);
				m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				//������Ϸ
				m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);
			}
			//�û�����
			else if(wGameStatus==GS_TK_SCORE)
			{
				if(m_wCurrentUser==wChairID)OnUserAddScore(wChairID,0,true);
			}
			//�±�����
			else if(wGameStatus==GS_TK_INVEST)
			{
				OnUserInvest(wChairID,0);
			}
			//���Ʒ���
			else OnUserOpenCard(wChairID,NULL);

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
	case GAME_STATUS_FREE:			//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_TK_INVEST:		//�±�״̬
		{
			//��������
			CMD_S_StatusInvest StatusInvest;
			ZeroMemory(&StatusInvest,sizeof(StatusInvest));

			//��������
			StatusInvest.wBankerUser=m_wBankerUser;
			StatusInvest.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusInvest.lUserScore,m_lUserScore,sizeof(m_lUserScore));
			CopyMemory(StatusInvest.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));
			CopyMemory(StatusInvest.bInvestFinish,m_bInvestFinish,sizeof(m_bInvestFinish));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusInvest,sizeof(StatusInvest));
		}
	case GS_TK_SCORE:		//��ע״̬
		{
			//��������
			CMD_S_StatusScore StatusScore;
			ZeroMemory(&StatusScore,sizeof(StatusScore));

			//��������
			StatusScore.wBankerUser=m_wBankerUser;
			StatusScore.wCurrentUser=m_wCurrentUser;
			StatusScore.lTurnMinScore=m_lTurnMinScore;	
			StatusScore.lTurnMaxScore=m_lUserScore[m_wCurrentUser]-m_lTotalScore[m_wCurrentUser];
			StatusScore.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusScore.lUserScore,m_lUserScore,sizeof(m_lUserScore));
			CopyMemory(StatusScore.lTotalScore,m_lTotalScore,sizeof(m_lTotalScore));
			CopyMemory(StatusScore.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));

			//��������
			BYTE cbSpeCount=2;
			if(m_cbBalanceCount>0)
			{
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if(m_cbPlayStatus[i]==FALSE)continue;

					//�û��˿�
					CopyMemory(StatusScore.cbMingCard[i],&m_cbHandCardData[i][cbSpeCount],sizeof(BYTE)*m_cbBalanceCount);
				}
			}

			//�����˿�
			if(pIServerUserItem->GetUserInfo()->cbUserStatus!=US_LOOKON)
			{
				CopyMemory(StatusScore.cbHandCard,m_cbHandCardData[wChiarID],sizeof(BYTE)*cbSpeCount);
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
		}
	case GS_TK_OPEN_CARD:	//����״̬
		{
			//��������
			CMD_S_StatusOpenCard StatusOpenCard;
			ZeroMemory(&StatusOpenCard,sizeof(StatusOpenCard));

			//��������
			StatusOpenCard.wBankerUser=m_wBankerUser;
			StatusOpenCard.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusOpenCard.lUserScore,m_lUserScore,sizeof(m_lUserScore));
			CopyMemory(StatusOpenCard.lTotalScore,m_lTotalScore,sizeof(m_lTotalScore));
			CopyMemory(StatusOpenCard.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));
			CopyMemory(StatusOpenCard.cbOpenFinish,m_cbOpenFinish,sizeof(m_cbOpenFinish));

			//��������
			BYTE cbSpeCount=2;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;

				//�û��˿�
				CopyMemory(StatusOpenCard.cbMingCard[i],&m_cbHandCardData[i][cbSpeCount],sizeof(BYTE)*cbSpeCount);
			}

			//�����˿�
			if(pIServerUserItem->GetUserInfo()->cbUserStatus!=US_LOOKON)
			{
				CopyMemory(StatusOpenCard.cbHandCard,m_cbHandCardData[wChiarID],sizeof(BYTE)*cbSpeCount);
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusOpenCard,sizeof(StatusOpenCard));
		}
	}
	//Ч�����
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//��Ϸ��Ϣ���� 
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_USER_INVEST:			//�û��±�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_UserInvest));
			if (wDataSize!=sizeof(CMD_C_UserInvest)) return false;

			//��������
			CMD_C_UserInvest * pUserInvest=(CMD_C_UserInvest *)pDataBuffer;

			//�û�Ч��
			//tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//״̬�ж�
			ASSERT(IsUserPlaying(pUserData->wChairID));
			if (!IsUserPlaying(pUserData->wChairID)) return false;

			//��Ϣ����
			return OnUserInvest(pUserData->wChairID,pUserInvest->lInvestGold);
		}
	case SUB_C_GIVE_UP:		//�û�����
		{
			//״̬Ч��
			ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_SCORE);
			if (m_pITableFrame->GetGameStatus()!=GS_TK_SCORE) return true;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();

			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return false;

			//��Ϣ����
			return OnUserGiveUp(pUserData->wChairID);
		}
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
			ASSERT(IsUserPlaying(pUserData->wChairID));
			if (!IsUserPlaying(pUserData->wChairID)) return false;

			//��Ϣ����
			return OnUserAddScore(pUserData->wChairID,pAddScore->lScore);
		}
	case SUB_C_OPEN_CARD:			//�û�̯��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_OpenCard));
			if (wDataSize!=sizeof(CMD_C_OpenCard)) return false;

			//��������
			CMD_C_OpenCard * pOpenCard=(CMD_C_OpenCard *)pDataBuffer;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]!=FALSE);
			if(m_cbPlayStatus[pUserData->wChairID]==FALSE)return false;

			//��Ϣ����
			return OnUserOpenCard(pUserData->wChairID,pOpenCard->cbCardData);
		}
	}

	return false;
}

//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(!bLookonUser)
	{
		//������
		if(m_lUserScore[wChairID]!=0)m_lUserScore[wChairID]=0;

		//�������
		BYTE cbHaveGoldCount=0;
		for(WORD i=0;i<GAME_PLAYER;i++)
		{
			if(m_lUserScore[i]>0)cbHaveGoldCount++;
		}
		if(cbHaveGoldCount==1)ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	}

	return true;
}

//��ע��Ϣ
void CTableFrameSink::SendUserAddInfo(WORD wAddUser,LONGLONG lScore,WORD wNextUser,LONGLONG lMinScore,LONGLONG lMaxScore)
{
	//��������
	CMD_S_AddScore AddScore;
	ZeroMemory(&AddScore,sizeof(AddScore));
	AddScore.lAddScoreCount=lScore;
	AddScore.wAddScoreUser=wAddUser;
	AddScore.wCurrentUser=wNextUser;
	AddScore.lTurnMinScore=lMinScore;
	AddScore.lTurnMaxScore=lMaxScore;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));

	return;
}

//������Ϣ
void CTableFrameSink::SendSendCardInfo(BYTE cbCardCount,WORD wCurrentUser)
{
	ASSERT(cbCardCount>=2 && cbCardCount<=MAX_COUNT);

	//��������
	CMD_S_SendCard SendCard;
	ZeroMemory(&SendCard,sizeof(SendCard));
	SendCard.cbCardCount=cbCardCount;
	SendCard.wCurrentUser=wCurrentUser;
	//SendCard.bStartOpen = bStartOpen;

	//��������
	BYTE cbMingCount = cbCardCount-2;
	if(cbMingCount>0)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE)continue;

			//�û��˿�
			CopyMemory(SendCard.cbMingCard[i],&m_cbHandCardData[i][2],cbMingCount);
		}
	}

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_pITableFrame->GetTableUserItem(i)==NULL)continue;

		//�û��˿�
		CopyMemory(SendCard.cbHandCard,m_cbHandCardData[i],sizeof(SendCard.cbHandCard));
		m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return;
}

//�±��¼�
bool CTableFrameSink::OnUserInvest(WORD wChairID, LONGLONG lInvestCount)
{
	//״̬Ч��
	ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_INVEST);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_INVEST) return false;

	//�û�����
	IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if(pIServerUserItem==NULL)return false;
	LONGLONG lUserGold=pIServerUserItem->GetUserScore();

	if(m_cbPlayStatus[wChairID]==TRUE)
	{
		//Ч����Ϣ
		ASSERT(lInvestCount>=0l && (lInvestCount+m_lUserScore[wChairID])<=lUserGold);
		if(!(lInvestCount>=0l && (lInvestCount+m_lUserScore[wChairID])<=lUserGold))return false;
		if(m_lUserScore[wChairID]==0)
		{
			ASSERT(lInvestCount>=m_pGameServiceOption->lCellScore);
			if(lInvestCount<m_pGameServiceOption->lCellScore)return false;
		}

		//���ñ���
		m_bInvestFinish[wChairID]=true;
		m_lUserScore[wChairID]+=lInvestCount;

		//��������
		CMD_S_UserInvest UserInvest;
		ZeroMemory(&UserInvest,sizeof(UserInvest));
		UserInvest.wChairID=wChairID;
		UserInvest.lScore=m_lUserScore[wChairID];

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_pITableFrame->GetTableUserItem(i)==NULL)continue;
			m_pITableFrame->SendTableData(i,SUB_S_USER_INVEST,&UserInvest,sizeof(UserInvest));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_INVEST,&UserInvest,sizeof(UserInvest));
	}

	//�±�����
	WORD wInvestFinishCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_cbPlayStatus[i]==TRUE && m_bInvestFinish[i]) wInvestFinishCount++;
		else if(m_cbPlayStatus[i]!=TRUE) wInvestFinishCount++;
	}

	//�¸�����
	if(wInvestFinishCount==m_wPlayerCount)
	{
		//�µ�ע��
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE)continue;
			m_lTotalScore[i] = m_pGameServiceOption->lCellScore;
		}

		//����˿�
		BYTE bTempArray[GAME_PLAYER*MAX_COUNT];
		m_GameLogic.RandCardList(bTempArray,sizeof(bTempArray));
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE)continue;
			CopyMemory(m_cbHandCardData[i],&bTempArray[i*MAX_COUNT],MAX_COUNT);
		}

		//�ж����
		WORD wPlayCount = 0,wShowCount = 0;
		for (BYTE i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]!=TRUE)continue;

			if(m_lTotalScore[i]==m_lUserScore[i])wShowCount++;
			wPlayCount++;
		}

		//����״̬
		BYTE cbStatus = ((wPlayCount>wShowCount+1)?GS_TK_SCORE:GS_TK_OPEN_CARD);
		m_pITableFrame->SetGameStatus(cbStatus);

		//��ע״̬
		if(wPlayCount>wShowCount+1)
		{
			//����û�
			for(WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;

				if(m_lTotalScore[i]==m_lUserScore[i])m_cbShowHand[i]=TRUE;
			}

			//������λ
			for(WORD i=0;i<m_wPlayerCount;i++)
			{
				m_wCurrentUser=(m_wBankerUser+i)%m_wPlayerCount;
				if(m_cbPlayStatus[m_wCurrentUser]==FALSE)continue;

				if(m_lTotalScore[m_wCurrentUser]<m_lUserScore[m_wCurrentUser])break;
			}

			SendSendCardInfo(2,m_wCurrentUser);
		}
		//����״̬
		else 
		{
			SendSendCardInfo(MAX_COUNT,INVALID_CHAIR);

			//��������
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPEN_START);
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPEN_START);
		}
	}

	return true;
}

//�����¼�
bool CTableFrameSink::OnUserGiveUp(WORD wChairID)
{
	OnEventGameConclude(wChairID+GAME_PLAYER,m_pITableFrame->GetTableUserItem(wChairID),GER_USER_LEAVE);

	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserAddScore(WORD wChairID, LONGLONG lScore,bool bGiveUp)
{
	//״̬Ч��
	ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_SCORE);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_SCORE) return false;

	//Ч���û�
	ASSERT(m_wCurrentUser==wChairID);
	if(m_wCurrentUser!=wChairID)return false;

	//�û�����
	if(!bGiveUp)
	{
		//У����
		ASSERT(lScore>=m_lTurnMinScore && (lScore+m_lTotalScore[wChairID])<=m_lUserScore[wChairID]);
		if (lScore<m_lTurnMinScore || (lScore+m_lTotalScore[wChairID])>m_lUserScore[wChairID])return false;

		//�ۼƽ��
		m_lTableScore[wChairID] += lScore;
		m_lTotalScore[wChairID] += lScore;

		//ƽ����ע
		if(m_lTableScore[wChairID] > m_lBalanceScore)
		{
			m_lBalanceScore = m_lTableScore[wChairID];
		}

		//����ж�
		if(m_lTotalScore[wChairID]==m_lUserScore[wChairID])m_cbShowHand[wChairID] = TRUE;
	}

	//�û��л�
	WORD wNextPlayer=INVALID_CHAIR;
	for (WORD i=1;i<m_wPlayerCount;i++)
	{
		//���ñ���
		m_wOperaCount++;
		wNextPlayer=(m_wCurrentUser+i)%m_wPlayerCount;

		//�����ж�
		if((m_cbPlayStatus[wNextPlayer]==TRUE)&&(m_cbShowHand[wNextPlayer]==FALSE))break;
	}

	//�жϵ�ע
	bool bFinishTurn=false;
	if (m_wOperaCount>=m_wPlayerCount)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//�жϵ�ע
			if ((m_cbPlayStatus[i]==TRUE)&&(m_lTableScore[i]<m_lBalanceScore)&&(m_cbShowHand[i]==FALSE)) 
				break;
		}

		//���ֽ���
		if (i==m_wPlayerCount) bFinishTurn=true;
	}

	//�ж����
	WORD wPlayCount = 0,wShowCount = 0;
	for (BYTE i=0;i<m_wPlayerCount;i++)
	{
		if(m_cbPlayStatus[i]!=TRUE)continue;

		if(m_cbShowHand[i]==TRUE)wShowCount++;
		wPlayCount++;
	}
	//ֻʣһ���û��(��Ϊ����ע�����������)����ȫ��
	if(!bFinishTurn && wPlayCount<=wShowCount+1 && m_lTableScore[wNextPlayer]>=m_lBalanceScore) bFinishTurn=true;

	//�����ж�
	if(bFinishTurn && wShowCount==0)
	{
		//��������
		WORD wNoAddCount=0;
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if (m_cbPlayStatus[i]==TRUE && m_lTableScore[i]==0)wNoAddCount++;
		}

		//���ƽ���
		if (wPlayCount==wNoAddCount)
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.cbEndReason=END_REASON_PASS;
			CopyMemory(GameEnd.cbCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

			//�������
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_lLoseScore[i]<0 || m_cbPlayStatus[i]==TRUE)GameEnd.lGameScore[i]=1;
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

			return true;
		}
	}

	//������ע
	if (!bFinishTurn)
	{
		//��ǰ�û�
		m_wCurrentUser=wNextPlayer;

		//С����ע
		m_lTurnMinScore = min(m_lBalanceScore-m_lTableScore[m_wCurrentUser],m_lUserScore[m_wCurrentUser]-m_lTotalScore[m_wCurrentUser]);
		m_lTurnMaxScore = m_lUserScore[m_wCurrentUser]-m_lTotalScore[m_wCurrentUser];

		//������Ϣ
		SendUserAddInfo(wChairID,lScore,m_wCurrentUser,m_lTurnMinScore,m_lTurnMaxScore);

		return true;
	}

	//��������
	m_wOperaCount=0;
	m_lBalanceScore = 0L;
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//ƽ�����
	m_cbBalanceCount++;

	//�жϿ���
	bool bOpenStatus = (m_cbBalanceCount!=3)?false:true;

	//������Ϣ
	if(bOpenStatus)SendUserAddInfo(wChairID,lScore,INVALID_CHAIR,0,0);

	//��������
	if(!bOpenStatus)
	{
		//����ж�
		BYTE cbSendCount=0;
		if(wPlayCount>0 && wShowCount>=wPlayCount-1)
		{
			//���ñ���
			bOpenStatus = true;
			cbSendCount = MAX_COUNT;
		}
		else cbSendCount = m_cbBalanceCount+2;

		//��ǰ�û�
		if(!bOpenStatus)
		{
			//�����С
			WORD wHeadMaxUser=INVALID_CHAIR;
			BYTE cbSingleCard[m_wPlayerCount];
			ZeroMemory(cbSingleCard,sizeof(cbSingleCard));
			for(WORD i=0;i<m_wPlayerCount;i++)
			{
				WORD wCurUser = (i+m_wBankerUser)%GAME_PLAYER;
				if(m_cbPlayStatus[wCurUser]!=FALSE)
				{
					if(wHeadMaxUser==INVALID_CHAIR)wHeadMaxUser=wCurUser;
					cbSingleCard[wCurUser]=m_GameLogic.GetSpeCardValue(&m_cbHandCardData[wCurUser][cbSendCount-1],1);
				}
			}

			//�Ƚϴ�С
			for(WORD i=0;i<m_wPlayerCount;i++)
			{
				WORD wCurUser = (i+m_wBankerUser)%GAME_PLAYER;
				if(m_cbPlayStatus[wCurUser]==FALSE || wCurUser==wHeadMaxUser || m_cbShowHand[wCurUser]==TRUE)continue;
				if(cbSingleCard[wCurUser]>cbSingleCard[wHeadMaxUser])wHeadMaxUser=wCurUser;
			}

			//��ǰ�û�
			m_wCurrentUser = wHeadMaxUser;

			//С����ע
			m_lTurnMinScore = 0L;
			m_lTurnMaxScore = m_lUserScore[m_wCurrentUser]-m_lTotalScore[m_wCurrentUser];

			//������Ϣ
			SendUserAddInfo(wChairID,lScore,INVALID_CHAIR,m_lTurnMinScore,m_lTurnMaxScore);
		}
		//�������
		else SendUserAddInfo(wChairID,lScore,INVALID_CHAIR,0,0);

		//������Ϣ
		SendSendCardInfo(cbSendCount,((!bOpenStatus)?m_wCurrentUser:INVALID_CHAIR));
	}

	//�û�����
	if(bOpenStatus)
	{	
		//����״̬
		m_pITableFrame->SetGameStatus(GS_TK_OPEN_CARD);

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPEN_START);
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPEN_START);
	}

	return true;
}

//�����¼�
bool CTableFrameSink::OnUserOpenCard(WORD wChairID, BYTE cbCardData[])
{
	//״̬Ч��
	ASSERT (m_pITableFrame->GetGameStatus()==GS_TK_OPEN_CARD);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_OPEN_CARD) return false;

	//��������
	if(cbCardData!=NULL)
	{
		//Ч������
		BYTE cbTempData[MAX_COUNT];
		CopyMemory(cbTempData,cbCardData,sizeof(cbTempData));
		bool bRemove = m_GameLogic.RemoveCard(m_cbHandCardData[wChairID],MAX_COUNT,cbTempData,MAX_COUNT);
		ASSERT(bRemove);
		if(!bRemove)return false;

		//ͷβ��С
		BYTE cbSpeCount = 2;
		BYTE cbValue=m_GameLogic.CompareCard(cbCardData,&cbCardData[cbSpeCount],cbSpeCount);
		ASSERT(cbValue!=TRUE);
		if(cbValue==TRUE)return false;

		//���⴦��
		m_GameLogic.ChangeSpeCard(cbCardData,MAX_COUNT);//!

		//�û��˿�
		CopyMemory(m_cbHandCardData[wChairID],cbCardData,sizeof(BYTE)*MAX_COUNT);
		m_cbOpenFinish[wChairID] = TRUE;

		//�������
		CMD_S_Open_Card OpenCard;
		ZeroMemory(&OpenCard,sizeof(OpenCard));
		OpenCard.wChairID=wChairID;

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_pITableFrame->GetTableUserItem(i)==NULL)continue;
			m_pITableFrame->SendTableData(i,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));	
	}

	//��������
	BYTE bUserCount=0;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_cbOpenFinish[i]==TRUE && m_cbPlayStatus[i]==TRUE)bUserCount++;
		else if(m_cbPlayStatus[i]==FALSE)bUserCount++;
	}

	//������Ϸ
	if(bUserCount == m_wPlayerCount)
	{
		return OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
	}

	return true;
}

//�����¼�
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
