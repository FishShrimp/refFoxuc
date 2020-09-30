#include "Stdafx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

#define RECTIFY_TIME				3										//����ʱ��
#define RULE_STEP_TIME				30										//����ʱ
#define RULE_LIMIT_TIME				1800									//������ʱ
#define RULE_TIME_OUT_COUNT			3										//����ʱ

//////////////////////////////////////////////////////////////////////////

//�����Ϣ
void TraceMessage(LPCTSTR pszMessage)
{
	CFile File;
	if ((File.Open(TEXT("TraceData.txt"),CFile::modeWrite)==FALSE)&&
		(File.Open(TEXT("TraceData.txt"),CFile::modeWrite|CFile::modeCreate)==FALSE))
	{
		ASSERT(FALSE);
		return;
	}

	File.SeekToEnd();
	File.Write(pszMessage,lstrlen(pszMessage));
	File.Write(TEXT("\r\n"),lstrlen(TEXT("\r\n")));

	File.Flush();
	File.Close();

	return;
}

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const BYTE			CTableFrameSink::m_GameStartMode=START_MODE_FULL_READY;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//�������
	m_bGameRule=false;
	m_wRuleRoundID=0L;
	ZeroMemory(&m_GameRuleInfo,sizeof(m_GameRuleInfo));

	//�û�����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(&m_wLeaveDrawTime,sizeof(m_wLeaveDrawTime));

	//����״̬
	m_cbConcludeReason=0;
	ZeroMemory(&m_cbPeaceCount,sizeof(m_cbPeaceCount));
	ZeroMemory(&m_cbRegretCount,sizeof(m_cbRegretCount));
	ZeroMemory(&m_cbRequestStatus,sizeof(m_cbRequestStatus));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	//���ù���
	m_GameRuleInfo.cbDirections=TRUE;
	m_GameRuleInfo.cbRegretFlag=TRUE;
	m_GameRuleInfo.cbCurrentMode=MODE_SLOW;
	m_GameRuleInfo.wRuleDrawTime=MODE_SLOW_DRAW_TIME;
	m_GameRuleInfo.wRuleStepTime=MODE_SLOW_STEP_TIME;
	m_GameRuleInfo.wRuleSecondTime=MODE_SLOW_SECOND_TIME;

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


	//��Ϸ����
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	return true;
}

//��λ����
VOID  CTableFrameSink::RepositionSink()
{
	//�û�����
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(&m_wLeaveDrawTime,sizeof(m_wLeaveDrawTime));

	//����״̬
	m_cbConcludeReason=0;
	ZeroMemory(&m_cbPeaceCount,sizeof(m_cbPeaceCount));
	ZeroMemory(&m_cbRegretCount,sizeof(m_cbRegretCount));
	ZeroMemory(&m_cbRequestStatus,sizeof(m_cbRequestStatus));

	return;
}

//��Ϸ״̬
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}
//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GAME_SCENE_PLAY);

	//���ñ���
	m_wCurrentUser=m_wBankerUser;
	for (WORD i=0;i<GAME_PLAYER;i++) m_wLeaveDrawTime[i]=m_GameRuleInfo.wRuleDrawTime;

	//��������
	m_GameLogic.ResetChessBorad();

	//���Ϳ�ʼ
	CMD_S_GameStart GameStart;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.GameRuleInfo=m_GameRuleInfo;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

	return true;
}

//��Ϸ����
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	try
	{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//���ñ���
			GameEnd.cbReason=m_cbConcludeReason;

			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray)); 

			//�л��û�
			WORD wBankerUser=m_wBankerUser;
			m_wBankerUser=(m_wBankerUser+1)%GAME_PLAYER;

			//����ж�
			if (m_GameLogic.GetStepCount(CHESS_WHITE)>=5)
			{
				//��������
				LONGLONG lGameScore[GAME_PLAYER];
				ZeroMemory(&lGameScore,sizeof(lGameScore));

				//�ɼ�����
				switch (m_pGameServiceOption->wServerType)
				{
				case GAME_GENRE_GOLD:		//���ģʽ
				case GAME_GENRE_MATCH:		//����ģʽ
					{
						//��Ҽ���
						if (wChairID!=INVALID_CHAIR)
						{
							lGameScore[wChairID]=1L;
							lGameScore[(wChairID+1)%GAME_PLAYER]=-1L;
						}

						break;
					}
				default:					//����ģʽ
					{
						//���ּ���
						CalculateGameScore(wChairID,lGameScore);

						break;
					}
				}

				//ͳ�ƻ���
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					LONGLONG lScore=(LONGLONG)lGameScore[i]*m_pGameServiceOption->lCellScore;
					BYTE nScoreKind;
					if(wChairID!=INVALID_CHAIR)
					{
						if (lScore>0L) nScoreKind = SCORE_TYPE_WIN;
						else if(lScore<0L) nScoreKind = SCORE_TYPE_LOSE;
						else nScoreKind= SCORE_TYPE_DRAW;
					}

					//����˰��
					LONGLONG lRevenue=m_pITableFrame->CalculateRevenue(i,lScore);
					lScore-=lRevenue;

					GameEnd.lGameScore[i]=lScore/*-m_pITableFrame->AccountRevenue(ScoreInfoArray[i].lScore)*/;

					ScoreInfoArray[i].lScore=lScore;
					//��������
					ScoreInfoArray[i].cbType=(ScoreInfoArray[i].lScore>=0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
					//����˰��
					ScoreInfoArray[i].lRevenue=m_pITableFrame->CalculateRevenue(i,ScoreInfoArray[i].lScore);
					if (ScoreInfoArray[i].lRevenue>0L) ScoreInfoArray[i].lScore-=ScoreInfoArray[i].lRevenue;

				}
				//д�����
				m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

				//��������
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			}
			else
			{
				//��������
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			//������Ϣ
			TCHAR szEndMessage[1024]=TEXT("");
			_sntprintf(szEndMessage,CountArray(szEndMessage),TEXT("���ֽ������ɼ�ͳ�ƣ�\n %s �� %I64d ��\n %s �� %I64d ��\n"),
				m_pITableFrame->GetTableUserItem(0)->GetNickName(),GameEnd.lGameScore[0],
				m_pITableFrame->GetTableUserItem(1)->GetNickName(),GameEnd.lGameScore[1]);

			//���ͽ�����Ϣ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
				m_pITableFrame->SendGameMessage(pIServerUserItem,szEndMessage,SMT_CHAT);
			}

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

			//��������
			if ((m_bGameRule==true)&&(m_wBankerUser!=INVALID_CHAIR))
			{
				//���ñ���
				m_wRuleRoundID++;
				m_bGameRule=false;

				//��������
				CMD_S_RequestRule RequestRule;
				ZeroMemory(&RequestRule,sizeof(RequestRule));

				//�������
				RequestRule.wRuleRoundID=m_wRuleRoundID;
				RequestRule.GameRuleInfo=m_GameRuleInfo;

				//��������
				m_pITableFrame->SendTableData(m_wBankerUser,SUB_S_REQUEST_RULE,&RequestRule,sizeof(RequestRule));
			}

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

			//��������
			if ((m_bGameRule==true)&&(m_wBankerUser!=INVALID_CHAIR))
			{
				//���ñ���
				m_wRuleRoundID++;
				m_bGameRule=false;

				//��������
				CMD_S_RequestRule RequestRule;
				ZeroMemory(&RequestRule,sizeof(RequestRule));

				//�������
				RequestRule.wRuleRoundID=m_wRuleRoundID;
				RequestRule.GameRuleInfo=m_GameRuleInfo;

				//��������
				m_pITableFrame->SendTableData(m_wBankerUser,SUB_S_REQUEST_RULE,&RequestRule,sizeof(RequestRule));
			}

			return true;
		}
	case GER_USER_LEAVE:	  //�û�ǿ��
	case GER_NETWORK_ERROR:   //�����쳣
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//���ñ���
			GameEnd.cbReason=REASON_USER_EXIT;

			//�л��û�
			m_wBankerUser=(wChairID+1)%GAME_PLAYER;

			//����ж�
			if (m_GameLogic.GetStepCount(CHESS_WHITE)>=5)
			{
				//��������
				LONGLONG lGameScore[GAME_PLAYER];
				ZeroMemory(&lGameScore,sizeof(lGameScore));

				tagScoreInfo ScoreInfoArray[GAME_PLAYER];
				ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray)); 

				//�ɼ�����
				switch (m_pGameServiceOption->wServerType)
				{
				case GAME_GENRE_GOLD:		//���ģʽ
				case GAME_GENRE_MATCH:		//����ģʽ
					{
						//��Ҽ���
						lGameScore[wChairID]=-m_pGameServiceOption->lCellScore;

						break;
					}
				default:					//����ģʽ
					{
						//���ּ���
						CalculateGameScore(m_wBankerUser,lGameScore);

						//���ּ���
						//lGameScore[wChairID]=-5L;

						break;
					}
				}

				//ͳ�ƻ���
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					LONGLONG lScore=(LONGLONG)lGameScore[i]*m_pGameServiceOption->lCellScore;
					BYTE nScoreKind;
					if(i==wChairID) nScoreKind=SCORE_TYPE_FLEE;
					else nScoreKind=SCORE_TYPE_WIN;

					//����˰��
					LONGLONG lRevenue=m_pITableFrame->CalculateRevenue(i,lScore);
					lScore-=lRevenue;

					GameEnd.lGameScore[i]=lScore;


					ScoreInfoArray[i].lScore=lScore;
					//��������
					ScoreInfoArray[i].cbType=nScoreKind;
					//����˰��
					ScoreInfoArray[i].lRevenue=m_pITableFrame->CalculateRevenue(i,ScoreInfoArray[i].lScore);
					if (ScoreInfoArray[i].lRevenue>0L) ScoreInfoArray[i].lScore-=ScoreInfoArray[i].lRevenue;

				}

				//д�����
				m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

				//��������
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			}
			else
			{
				//��������
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

			//��������
			if ((m_bGameRule==true)&&(m_wBankerUser!=INVALID_CHAIR))
			{
				//���ñ���
				m_wRuleRoundID++;
				m_bGameRule=false;

				//��������
				CMD_S_RequestRule RequestRule;
				ZeroMemory(&RequestRule,sizeof(RequestRule));

				//�������
				RequestRule.wRuleRoundID=m_wRuleRoundID;
				RequestRule.GameRuleInfo=m_GameRuleInfo;

				//��������
				m_pITableFrame->SendTableData(m_wBankerUser,SUB_S_REQUEST_RULE,&RequestRule,sizeof(RequestRule));
			}

			return true;
		}
	}
	}
	catch(...)
	{
		TraceMessage(TEXT("113444444_������Ϸ"));
	}

	//�������
	ASSERT(FALSE);

	TraceMessage(TEXT("113444444_false������Ϸ"));
	return false;
}

//���ͳ���
bool  CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	try
	{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:	//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			try
			{
				//���ñ���
				StatusFree.wBankerUser=m_wBankerUser;
				StatusFree.wRuleRoundID=m_wRuleRoundID;
				StatusFree.cbGameRule=(m_bGameRule==true)?TRUE:FALSE;
				CopyMemory(&StatusFree.GameRuleInfo,&m_GameRuleInfo,sizeof(StatusFree.GameRuleInfo));
			}
			catch(...)
			{
				TraceMessage(TEXT("113444444_�������ñ����������ñ�������״̬"));
				return false;
			}

			try
			{
				//���ͳ���
				return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
			}
			catch(...)
			{
				TraceMessage(TEXT("113444444_���ͳ�������״̬"));
				return false;
			}
		}
	case GAME_SCENE_PLAY:	//��Ϸ״̬
		{
			//��ȡ����
			INT_PTR nManualCount=0;
			tagChessManual * pChessManual=NULL;

			try
			{
				nManualCount=m_GameLogic.m_ChessManualArray.GetCount();
				pChessManual=m_GameLogic.m_ChessManualArray.GetData();
			}
			catch(...)
			{
				TraceMessage(TEXT("113444444_��Ϸ״̬"));
				return false;
			}

			try
			{
				//��������
				for (WORD i=0;i<nManualCount;i+=(4080/sizeof(tagChessManual)))
				{
					WORD wSendCount=(WORD)__min(nManualCount-i,4080/sizeof(tagChessManual));
					m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_CHESS_MANUAL,pChessManual+i,wSendCount*sizeof(tagChessManual));
				}
			}
			catch(...)
			{
				TraceMessage(TEXT("113444444_��Ϸ״̬��������"));
				return false;
			}

			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			try
			{
				//��Ϸ��Ϣ
				StatusPlay.wBankerUser=m_wBankerUser;
				StatusPlay.wCurrentUser=m_wCurrentUser;
				StatusPlay.GameRuleInfo=m_GameRuleInfo;
				CopyMemory(StatusPlay.wLeaveDrawTime,m_wLeaveDrawTime,sizeof(m_wLeaveDrawTime));
				CopyMemory(StatusPlay.cbRequestStatus,m_cbRequestStatus,sizeof(m_cbRequestStatus));
			}
			catch(...)
			{
				TraceMessage(TEXT("113444444_��Ϸ����Ϸ��Ϣ23"));
				return false;
			}

			try
			{
				//���ͳ���
				return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
			}
			catch(...)
			{
				TraceMessage(TEXT("113444444_��Ϸ��Ϣ34523"));
				return false;
			}
		}
	}
	}
	catch(...)
	{
		TraceMessage(TEXT("113444444_���ͳ���"));
		return false;
	}

	//�������
	ASSERT(FALSE);

	TraceMessage(TEXT("113444444_false���ͳ���"));
	return false;
}

//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	TraceMessage(TEXT("113444444_false��ʱ���¼�"));
	return false;
}

//��Ϸ��Ϣ
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID,VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	try
	{
		switch (wSubCmdID)
		{
		case SUB_C_MOVE_CHESS:			//�ƶ�����
			{
				//Ч������
				ASSERT(wDataSize==sizeof(CMD_C_MoveChess));
				if (wDataSize!=sizeof(CMD_C_MoveChess)) 
				{
					TraceMessage(TEXT("113444444_�ƶ�����"));
					return false;
				}

				try
				{
					//״̬Ч��
					ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
					if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

					//�û�Ч��
					ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
					if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

					//��������
					CMD_C_MoveChess * pMoveChess=(CMD_C_MoveChess *)pData;

					//��Ϣ����
					WORD wChairID=pIServerUserItem->GetChairID();

					return OnUserMoveChess(wChairID,pMoveChess->cbXSourcePos,pMoveChess->cbYSourcePos,
						pMoveChess->cbXTargetPos,pMoveChess->cbYTargetPos,pMoveChess->wUsedTimeCount);
				}
				catch(...)
				{
					TraceMessage(TEXT("113444444_��Ϣ����OnUserMoveChess"));
					return false;
				}
			}
		case SUB_C_PEACE_REQUEST:		//��������
			{
				try
				{
					//״̬Ч��
					ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
					if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

					//�û�Ч��
					ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
					if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

					//��Ϣ����
					return OnUserPeaceRequest(pIServerUserItem->GetChairID());
				}
				catch(...)
				{
					TraceMessage(TEXT("113444444_��Ϣ����OnUserPeaceRequest"));
					return false;
				}
			}
		case SUB_C_PEACE_RESPOND:		//�����Ӧ
			{
				try
				{
					//Ч������
					ASSERT(wDataSize==sizeof(CMD_C_PeaceRespond));
					if (wDataSize!=sizeof(CMD_C_PeaceRespond)) 
					{
						TraceMessage(TEXT("113444444_�����Ӧ"));
						return false;
					}

					//״̬Ч��
					ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
					if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

					//�û�Ч��
					ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
					if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

					//��������
					CMD_C_PeaceRespond * pPeaceRespond=(CMD_C_PeaceRespond *)pData;

					//��Ϣ����
					WORD wChairID=pIServerUserItem->GetChairID();
					return OnUserPeaceRespond(wChairID,pPeaceRespond->cbApprove);
				}
				catch(...)
				{
					TraceMessage(TEXT("113444444_��Ϣ����OnUserPeaceRespond"));
					return false;
				}
			}
		case SUB_C_REGRET_REQUEST:		//��������
			{
				try
				{
					//״̬Ч��
					ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
					if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

					//�û�Ч��
					ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
					if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

					//��Ϣ����
					return OnUserRegretRequest(pIServerUserItem->GetChairID());
				}
				catch(...)
				{
					TraceMessage(TEXT("113444444_��Ϣ����OnUserRegretRequest"));
					return false;
				}
			}
		case SUB_C_REGRET_RESPOND:		//�����Ӧ
			{
				try
				{
					//Ч������
					ASSERT(wDataSize==sizeof(CMD_C_RegretRespond));
					if (wDataSize!=sizeof(CMD_C_RegretRespond)) 
					{
						TraceMessage(TEXT("113444444_�����Ӧ"));
						return false;
					}

					//״̬Ч��
					ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
					if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

					//�û�Ч��
					ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
					if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

					//��������
					CMD_C_RegretRespond * pRegretRespond=(CMD_C_RegretRespond *)pData;

					//��Ϣ����
					WORD wChairID=pIServerUserItem->GetChairID();
					return OnUserRegretRespond(wChairID,pRegretRespond->cbApprove);
				}
				catch(...)
				{
					TraceMessage(TEXT("113444444_��Ϣ����OnUserRegretRespond"));
					return false;
				}
			}
		case SUB_C_CONCLUDE_REQUEST:	//��������
			{
				try
				{
					//Ч������
					ASSERT(wDataSize==sizeof(CMD_C_ConcludeRequest));
					if (wDataSize!=sizeof(CMD_C_ConcludeRequest)) 
					{
						TraceMessage(TEXT("113444444_��������"));
						return false;
					}

					//״̬Ч��
					ASSERT(m_pITableFrame->GetGameStatus()==GAME_SCENE_PLAY);
					if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLAY) return true;

					//�û�Ч��
					ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
					if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

					//��������
					CMD_C_ConcludeRequest * pConcludeRequest=(CMD_C_ConcludeRequest *)pData;

					//��Ϣ����
					WORD wChairID=pIServerUserItem->GetChairID();
					return OnUserConcludeRequest(wChairID,pConcludeRequest->cbReason);
				}
				catch(...)
				{
					TraceMessage(TEXT("113444444_��Ϣ����OnUserConcludeRequest"));
					return false;
				}
			}
		}
	}
	catch(...)
	{
		TraceMessage(TEXT("113444444_��Ϸ��Ϣ"));
	}

	TraceMessage(TEXT("113444444_false��Ϸ��Ϣ"));
	return false;
}

//�����Ϣ
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID,VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	/*WORD wMsg[]={SUB_GF_INFO,SUB_GF_USER_READY,SUB_GF_USER_CHAT,SUB_GF_LOOKON_CONTROL,SUB_GF_KICK_TABLE_USER};
	for(WORD i=0;i<CountArray(wMsg);i++)
	{
		if(wSubCmdID==wMsg[i])return false;
	}

	TCHAR tchInfo[512]=TEXT("");
	_sntprintf(tchInfo,sizeof(tchInfo),TEXT("wSubCmdID=%d"),wSubCmdID);
	TraceMessage(tchInfo);*/
	return false;
}

//�����¼�
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}


//�û�����
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//ׯ������
	if ((bLookonUser==false)&&(m_wBankerUser==INVALID_CHAIR)) m_wBankerUser=pIServerUserItem->GetChairID();

	return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{

	//ׯ������
	if ((bLookonUser==false)&&(pIServerUserItem->GetChairID()==m_wBankerUser))
	{
		//����ׯ��
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if ((m_wBankerUser!=i)&&(m_pITableFrame->GetTableUserItem(i)!=NULL))
			{
				//���ñ���
				m_wBankerUser=i;
				m_wRuleRoundID++;
				m_bGameRule=false;

				//��������
				CMD_S_RequestRule RequestRule;
				ZeroMemory(&RequestRule,sizeof(RequestRule));

				//�������
				RequestRule.wRuleRoundID=m_wRuleRoundID;
				RequestRule.GameRuleInfo=m_GameRuleInfo;

				//��������
				m_pITableFrame->SendTableData(m_wBankerUser,SUB_S_REQUEST_RULE,&RequestRule,sizeof(RequestRule));

				break;
			}
		}

		//ׯ������
		if (i==GAME_PLAYER) 
		{
			//���ñ���
			m_wRuleRoundID++;
			m_bGameRule=false;
			m_wBankerUser=INVALID_CHAIR;

			//�������
			m_GameRuleInfo.cbDirections=TRUE;
			m_GameRuleInfo.cbRegretFlag=TRUE;
			m_GameRuleInfo.cbCurrentMode=MODE_SLOW;
			m_GameRuleInfo.wRuleDrawTime=MODE_SLOW_DRAW_TIME;
			m_GameRuleInfo.wRuleStepTime=MODE_SLOW_STEP_TIME;
			m_GameRuleInfo.wRuleSecondTime=MODE_SLOW_SECOND_TIME;
		}
	}

	return true;
}

//�û�ͬ��
bool  CTableFrameSink::OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	//ׯ���û�
	if (pIServerUserItem->GetChairID()==m_wBankerUser)
	{
		//Ч���С
		ASSERT(wDataSize==sizeof(CMD_C_CustomizeRult));
		if (wDataSize!=sizeof(CMD_C_CustomizeRult)) 
		{
			TraceMessage(TEXT("113444444_ׯ���û�"));
			return false;
		}

		//Ч��״̬
		ASSERT(m_bGameRule==false);
		if (m_bGameRule!=false) 
		{
			TraceMessage(TEXT("113444444_m_bGameRuleЧ��״̬"));
			return false;
		}

		//��Ϣ����
		CMD_C_CustomizeRult * pCustomizeRult=(CMD_C_CustomizeRult *)pData;

		//����Ч��
		pCustomizeRult->GameRuleInfo.wRuleDrawTime=__min(pCustomizeRult->GameRuleInfo.wRuleDrawTime,MAX_DRAW_TIME);
		pCustomizeRult->GameRuleInfo.wRuleStepTime=__min(pCustomizeRult->GameRuleInfo.wRuleStepTime,MAX_STEP_TIME);
		pCustomizeRult->GameRuleInfo.wRuleSecondTime=__min(pCustomizeRult->GameRuleInfo.wRuleSecondTime,MAX_SECOND_TIME);

		//���ù���
		m_bGameRule=true;
		m_GameRuleInfo=pCustomizeRult->GameRuleInfo;

		//����ͬ��
		WORD wReqChairID=(m_wBankerUser+1)%GAME_PLAYER;
		if (m_pITableFrame->GetTableUserItem(wReqChairID)!=NULL)
		{
			//��������
			CMD_S_RequestAgree RequestAgree;
			ZeroMemory(&RequestAgree,sizeof(RequestAgree));

			//������Ϣ
			RequestAgree.wRuleRoundID=m_wRuleRoundID;
			RequestAgree.GameRuleInfo=m_GameRuleInfo;

			//������Ϣ
			m_pITableFrame->SendTableData(wReqChairID,SUB_S_REQUEST_AGREE,&RequestAgree,sizeof(RequestAgree));
		}
	}

	//�����û�
	if ((pIServerUserItem->GetChairID()!=m_wBankerUser)&&(m_bGameRule==false))
	{
		TraceMessage(TEXT("113444444_�����û�"));
		return false;
	}

	return true;
}

//�ƶ�����
bool CTableFrameSink::OnUserMoveChess(WORD wChairID, BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos, WORD wUserTimeCount)
{
	//Ч�����
	ASSERT((cbXSourcePos<COUNT_X)&&(cbYSourcePos<COUNT_Y));
	if ((cbXSourcePos>=COUNT_X)||(cbYSourcePos>=COUNT_Y))
	{
		TraceMessage(TEXT("113444444_cbXSourcePos>=COUNT_X"));
		return false;
	}

	//Ч�����
	ASSERT((cbXTargetPos<COUNT_X)&&(cbYTargetPos<COUNT_Y));
	if ((cbXTargetPos>=COUNT_X)||(cbYTargetPos>=COUNT_Y)) 
	{
		TraceMessage(TEXT("113444444_(cbXTargetPos>=COUNT_X)||(cbYTargetPos>=COUNT_Y"));
		return false;
	}

	//Ч��ʱ��
	ASSERT(wUserTimeCount<=m_GameRuleInfo.wRuleDrawTime);
	if (wUserTimeCount>m_GameRuleInfo.wRuleDrawTime)
	{
		TraceMessage(TEXT("113444444_(wUserTimeCount>m_GameRuleInfo.wRuleDrawTime)"));
		return false;
	}

	//Ч��ʱ��
	ASSERT(wUserTimeCount>=(m_GameRuleInfo.wRuleDrawTime-m_wLeaveDrawTime[wChairID]));
	if (wUserTimeCount<(m_GameRuleInfo.wRuleDrawTime-m_wLeaveDrawTime[wChairID])) 
	{
		TraceMessage(TEXT("113444444_(Ч��ʱ��>m_GameRuleInfo.wRuleDrawTime)"));
		return false;
	}

	//Ч���û�
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) 
	{
		TraceMessage(TEXT("113444444_(m_wCurrentUsereDrawTime)"));
		return false;
	}

	//�����ж�
	BYTE cbUserColor;
	tagChessItem * pSourceChessItem=NULL;
	try
	{
		cbUserColor=(wChairID==m_wBankerUser)?CHESS_BLACK:CHESS_WHITE;
		pSourceChessItem=m_GameLogic.GetChessItem(cbXSourcePos,cbYSourcePos);
	}
	catch(...)
	{
		TraceMessage(TEXT("113444444_(m_GameLogic.GetChessItem)"));
		return false;
	}

	//Ч�����
	ASSERT((pSourceChessItem!=NULL)&&(pSourceChessItem->cbColor==cbUserColor));
	if ((pSourceChessItem==NULL)||(pSourceChessItem->cbColor!=cbUserColor)) 
	{
		if(pSourceChessItem==NULL)
		{
			TraceMessage(TEXT("113444444_(pSourceChessItem==NULL)"));
			return false;
		}
		TCHAR tchInfo[512]=TEXT("");
		_sntprintf(tchInfo,sizeof(tchInfo),TEXT("pSourceChessItem->cbColor=%d,cbUserColor=%d"),pSourceChessItem->cbColor,cbUserColor);
		TraceMessage(tchInfo);
		return false;
	}

	//�ƶ�����
	tagMoveChessResult MoveChessResult;
	bool bSucceed = false;
	try
	{
		bSucceed = m_GameLogic.IsWalkLegality(pSourceChessItem,cbXTargetPos,cbYTargetPos);
	}
	catch(...)
	{
		TraceMessage(TEXT("113444444_(Ч��pSourceChessItem,cbXTargetPos,cbYTargetPDrawTime)"));
	}
	if (bSucceed==false) return false;

	try
	{
		bSucceed = m_GameLogic.MoveChess(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos,MoveChessResult,NULL);
	}
	catch(...)
	{
		TraceMessage(TEXT("113444444_(Ч��MoveChess(cbXSourcePos,cbYSourcePos,cbXTargetPDrawTime)"));
	}
	if (bSucceed==false) return false;

	//��������
	tagConcludeInfo ConcludeInfo;
	ZeroMemory(&ConcludeInfo,sizeof(ConcludeInfo));
	try
	{
		m_GameLogic.IsGameFinish((wChairID==m_wBankerUser)?CHESS_BLACK:CHESS_WHITE,ConcludeInfo);
	}
	catch(...)
	{
		TraceMessage(TEXT("113444444_(��������,IsGameFinish)"));
		return false;
	}

	//���ñ���
	m_wLeaveDrawTime[wChairID]=m_GameRuleInfo.wRuleDrawTime-wUserTimeCount;
	m_wCurrentUser=(ConcludeInfo.bGameConclude==true)?INVALID_CHAIR:(m_wCurrentUser+1)%GAME_PLAYER;

	//��������
	CMD_S_MoveChess MoveChess;
	ZeroMemory(&MoveChess,sizeof(MoveChess));

	//�û���Ϣ
	MoveChess.wActionUser=wChairID;
	MoveChess.wCurrentUser=m_wCurrentUser;

	//������Ϣ
	MoveChess.cbXSourcePos=cbXSourcePos;
	MoveChess.cbYSourcePos=cbYSourcePos;
	MoveChess.cbXTargetPos=cbXTargetPos;
	MoveChess.cbYTargetPos=cbYTargetPos;
	MoveChess.wLeaveDrawTime=m_wLeaveDrawTime[wChairID];

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_MOVE_CHESS,&MoveChess,sizeof(MoveChess));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_MOVE_CHESS,&MoveChess,sizeof(MoveChess));

	//������Ϸ
	if (ConcludeInfo.bGameConclude==true) 
	{
		//��������
		WORD wWinnerUser=INVALID_CHAIR;
		BYTE cbWinnerColor=ConcludeInfo.cbWinnerColor;

		//ʤ������
		if (cbWinnerColor!=CHESS_NULL)
		{
			WORD wBlackUser=m_wBankerUser;
			WORD wWhiteUser=(m_wBankerUser+1)%GAME_PLAYER;
			wWinnerUser=(cbWinnerColor==CHESS_BLACK)?wBlackUser:wWhiteUser;
		}

		//������Ϸ
		m_cbConcludeReason = ConcludeInfo.cbReason;
		OnEventGameConclude(wWinnerUser,NULL,GER_NORMAL);
	}

	return true;
}

//��������
bool CTableFrameSink::OnUserPeaceRequest(WORD wChairID)
{
	//�û�״̬
	ASSERT((m_cbRequestStatus[wChairID]&GU_WAIT_PEACE)==0);
	if ((m_cbRequestStatus[wChairID]&GU_WAIT_PEACE)!=0) 
	{
		TraceMessage(TEXT("113444444_(Ч��MoveChess(�û�״̬,cbYSourcePos,cbXTargetPDrawTime)"));
		return false;
	}

	//�����ж�
	if (m_cbPeaceCount[wChairID]>=3)
	{
		//������Ϣ
		TCHAR szMessage[128]=TEXT("");
		_sntprintf(szMessage,CountChar(szMessage),TEXT("���Ѿ� [ ��� ] %d ���ˣ������ܴ���"),m_cbPeaceCount[wChairID]);

		//������Ϣ
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem!=NULL) m_pITableFrame->SendGameMessage(pIServerUserItem,szMessage,SMT_CHAT);

		return true;
	}

	//���ñ���
	m_cbPeaceCount[wChairID]++;
	m_cbRequestStatus[wChairID]|=GU_WAIT_PEACE;

	//������Ϣ
	m_pITableFrame->SendTableData((wChairID+1)%GAME_PLAYER,SUB_S_PEACE_REQUEST);

	return true;
}

//�����Ӧ
bool CTableFrameSink::OnUserPeaceRespond(WORD wChairID, BYTE cbApprove)
{
	//�û�״̬
	WORD wRequestUser=(wChairID+1)%GAME_PLAYER;
	if ((m_cbRequestStatus[wRequestUser]&GU_WAIT_PEACE)==0) return true;

	//���ñ���
	m_cbRequestStatus[wRequestUser]&=~GU_WAIT_PEACE;

	//�������
	if (cbApprove==TRUE) 
	{
		m_cbConcludeReason = REASON_AGREE_DRAW;
		OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
	}
	else m_pITableFrame->SendTableData(wRequestUser,SUB_S_PEACE_RESPOND);

	return true;
}

//��������
bool CTableFrameSink::OnUserRegretRequest(WORD wChairID)
{
	//״̬�ж�
	if ((m_cbRequestStatus[wChairID]&GU_WAIT_REGRET)!=0) 
	{
		TraceMessage(TEXT("113444444_(Ч���������(�û�״̬,cbYSourcePos,cbXTargetPDrawTime)"));
		return false;
	}
	if (m_GameLogic.GetStepCount((wChairID==m_wBankerUser)?CHESS_BLACK:CHESS_WHITE)==0) 
	{
		TraceMessage(TEXT("113444444_(wChairID==m_wBankerUser)?CHESS(�û�״̬,cbYSourcePos,cbXTargetPDrawTime)"));
		return false;
	}

	//��������
	if (m_GameRuleInfo.cbRegretFlag==FALSE)
	{
		//������Ϣ
		CMD_S_RegretFaile RegretFaile;
		RegretFaile.cbFaileReason=FR_RULE_LIMIT;
		m_pITableFrame->SendTableData(wChairID,SUB_S_REGRET_FAILE,&RegretFaile,sizeof(RegretFaile));

		//ϵͳ��Ϣ
		LPCTSTR pszMessage=TEXT("���ݹ������ã�������Ϸ���������塱��");
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem!=NULL) m_pITableFrame->SendGameMessage(pIServerUserItem,pszMessage,SMT_CHAT);

		return true;
	}

	//�жϴ���
	if (m_cbRegretCount[wChairID]==5)
	{
		//������Ϣ
		CMD_S_RegretFaile RegretFaile;
		RegretFaile.cbFaileReason=FR_COUNT_LIMIT;
		m_pITableFrame->SendTableData(wChairID,SUB_S_REGRET_FAILE,&RegretFaile,sizeof(RegretFaile));

		//������Ϣ
		TCHAR szMessage[128]=TEXT("");
		_sntprintf(szMessage,CountChar(szMessage),TEXT("���Ѿ� [ ���� ] %d ���ˣ������ٴλ��塣"),m_cbRegretCount[wChairID]);

		//������Ϣ
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem!=NULL) m_pITableFrame->SendGameMessage(pIServerUserItem,szMessage,SMT_CHAT);

		return true;
	}

	//���ñ���
	m_cbRegretCount[wChairID]++;
	m_cbRequestStatus[wChairID]|=GU_WAIT_REGRET;

	//������Ϣ
	m_pITableFrame->SendTableData((wChairID+1)%GAME_PLAYER,SUB_S_REGRET_REQUEST);

	return true;
}

//�����Ӧ
bool CTableFrameSink::OnUserRegretRespond(WORD wChairID, BYTE cbApprove)
{
	//״̬�ж�
	WORD wRequestUser=(wChairID+1)%GAME_PLAYER;
	if ((m_cbRequestStatus[wRequestUser]&GU_WAIT_REGRET)==0) 
	{
		TraceMessage(TEXT("113444444_(wChairID==m_wBankerUser)?CHESS(�û�״̬,�����Ӧ,cbXTargetPDrawTime)"));
		return false;
	}

	//���ñ���
	m_cbRequestStatus[wRequestUser]&=~GU_WAIT_REGRET;

	//�ܾ�����
	if (cbApprove==FALSE)
	{
		CMD_S_RegretFaile RegretFaile;
		RegretFaile.cbFaileReason=FR_PLAYER_OPPOSE;
		m_pITableFrame->SendTableData(wRequestUser,SUB_S_REGRET_FAILE,&RegretFaile,sizeof(RegretFaile));
		return true;
	}

	//��������
	CMD_S_RegretResult RegretResult;
	ZeroMemory(&RegretResult,sizeof(RegretResult));

	//�������
	WORD wRegretCount=(wRequestUser==m_wCurrentUser)?2:1;
	WORD wCurrentStep=(WORD)m_GameLogic.m_ChessManualArray.GetCount();
	RegretResult.wTargetStep=(wCurrentStep>wRegretCount)?(wCurrentStep-wRegretCount):0;

	//�������
	m_GameLogic.RegretChess(RegretResult.wTargetStep);

	//�����û�
	m_wCurrentUser=((RegretResult.wTargetStep%2)==0)?m_wBankerUser:(m_wBankerUser+1)%GAME_PLAYER;

	//��������
	RegretResult.wRegretUser=wRequestUser;
	RegretResult.wCurrentUser=m_wCurrentUser;
	CopyMemory(RegretResult.wLeaveDrawTime,m_wLeaveDrawTime,sizeof(RegretResult.wLeaveDrawTime));

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_REGRET_RESULT,&RegretResult,sizeof(RegretResult));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_REGRET_RESULT,&RegretResult,sizeof(RegretResult));

	return true;
}

//��������
bool CTableFrameSink::OnUserConcludeRequest(WORD wChairID, BYTE cbReason)
{
	//���ñ���
	m_cbConcludeReason=cbReason;

	//������Ϸ
	OnEventGameConclude((wChairID+1)%GAME_PLAYER,NULL,GER_NORMAL);

	return true;
}

//���ּ���
bool CTableFrameSink::CalculateGameScore(WORD wWinnerUser, LONGLONG lGameScore[GAME_PLAYER])
{
	//��������
	WORD wUserLevel[GAME_PLAYER];
	ZeroMemory(&wUserLevel,sizeof(wUserLevel));

	//��ȡ����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		LONGLONG lUserScore=0L;

		//��ȡ����
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem!=NULL) lUserScore=pIServerUserItem->GetUserScore();

		//�ȼ�����
		if (lUserScore>=0L)
		{
			//��������
			LONGLONG lLevelScore=0L;
			LONGLONG lScoreIncrease=20L;

			//�ȼ�����
			for (WORD wLevel=1;wLevel<20;wLevel++)
			{
				lScoreIncrease+=wLevel*10;
				lLevelScore+=lScoreIncrease;
				if (lUserScore<lLevelScore) break;
			}

			//���õȼ�
			wUserLevel[i]=wLevel;
		}
	}

	//����ͳ��
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		//��������
		WORD wOtherChairID=(i+1)%GAME_PLAYER;

		//ʤ�ּ���
		if (i==wWinnerUser)
		{
			if (wUserLevel[i]<=(wUserLevel[wOtherChairID]+4))
			{
				lGameScore[i]=(wUserLevel[i]>=(wUserLevel[wOtherChairID]+2))?2:5;
			}
			continue;
		}

		//��ּ���
		if (wWinnerUser==wOtherChairID)
		{
			lGameScore[i]=(wUserLevel[i]>=(wUserLevel[wOtherChairID]+4))?-3:-2;
			continue;
		}

		//ƽ�ּ���
		if (wWinnerUser==INVALID_CHAIR)
		{
			if (wUserLevel[i]<=(wUserLevel[wOtherChairID]+2)) lGameScore[i]=1;
			continue;
		}
	}

	//ʮ����Ч
	if (m_GameLogic.GetStepCount(CHESS_WHITE)<10)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			if(lGameScore[i]>0)lGameScore[i]=0;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
