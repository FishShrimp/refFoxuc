#include "StdAfx.h"
#include "TableFrameSink.h"


//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����


//////////////////////////////////////////////////////////////////////////
//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����	 	 
	m_wCurrentUser=INVALID_CHAIR; 
	m_baseScore = 0;	
	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;	
	m_bHit = false;
	m_roundCount = 0;
	m_bGameStart = false;
	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ
VOID * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
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
	//m_pITableFrame=GET_OBJECTPTR_INTERFACE(pIUnknownEx,ITableFrame);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	m_baseScore = m_pGameServiceOption->lCellScore;

	return true;
}

//��λ����
VOID  CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	//m_wDouble=1;
	//m_bLandScore=0;
	//m_wBankerUser=INVALID_CHAIR;
	//m_wCurrentUser=INVALID_CHAIR;

	//memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));
	//ZeroMemory(m_byPeasantDouble,sizeof(m_byPeasantDouble));
	////������Ϣ
	//m_bTurnCardCount=0;
	//m_wTurnWiner=INVALID_CHAIR;
	//memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	////�˿���Ϣ
	//memset(m_bBackCard,0,sizeof(m_bBackCard));
	//memset(m_bCardCount,0,sizeof(m_bCardCount));
	//memset(m_bOutCardCount,0,sizeof(m_bOutCardCount));
	//memset(m_bHandCardData,0,sizeof(m_bHandCardData));


	return;
}

 


//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
	//����״̬
 	const float yups =25*0.9160256f;  //*3BALLDIAMETER* _/-3/2
	m_ptBallPos[0].x = 222;
	m_ptBallPos[0].y = 244;
	float banjin = 12.5;

	m_currentComob = 0;
	FLOAT bp8[][3]={{0,0,0}
	,{0,0,1},
	{yups,banjin,4},{yups,-banjin,9},
	{yups*2,banjin*2,7},{yups*2,0,8},{yups*2,-banjin*2,3}
	,{yups*3,banjin*3,2},{yups*3,banjin,6},{yups*3,-banjin,5},{yups*3,-banjin*3,10}
	,{yups*4,banjin*4,15},{yups*4,banjin*2,13},{yups*4,0,11},{yups*4,-banjin*2,14},{yups*4,-banjin*4,12}};
	for(int i=1;i<BALL_NUM;i++)
	{
		m_ptBallPos[(int)bp8[i][2]].x = bp8[i][0]+577;
		m_ptBallPos[(int)bp8[i][2]].y = bp8[i][1]+222;
	}
	m_bHit = false;
	m_roundCount ++;
	ZeroMemory(m_cbPlayOutTimeNum,sizeof(m_cbPlayOutTimeNum));
	ZeroMemory(m_bWrongHit,sizeof(m_bWrongHit));
	ZeroMemory(m_ballNum,sizeof(m_ballNum));
	ZeroMemory(m_comob,sizeof(m_comob));
	ZeroMemory(m_bSendHitResult,sizeof(m_bSendHitResult));
	ZeroMemory(&m_hitBall,sizeof(m_hitBall));
	ZeroMemory(&m_hitResult,sizeof(m_hitResult));
	m_userBallType[0] = 100;
	m_userBallType[1] = 100;
	m_wCurrentUser = rand()%GAME_PLAYER;
	m_bFirstBall = true;
	ZeroMemory(m_ball,sizeof(m_ball));
	////�����û�	 	
	IServerUserItem *pServerUserItem = NULL;
	//	
	CMD_S_GameStart gameStart;
	gameStart.currentUser = m_wCurrentUser;
	gameStart.nBaseScore =  m_baseScore;
	gameStart.roundCount = m_roundCount;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		pServerUserItem = m_pITableFrame->GetTableUserItem( i ) ;		 
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&gameStart,sizeof(gameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&gameStart,sizeof(gameStart));
	}
	m_bGameStart = true;
	m_pITableFrame->SetGameTimer(IDI_SET_BASEBALL,TIME_SET_BASEBALL+5000,1,NULL);
	m_pITableFrame->SetGameStatus(GS_WK_SET_SETBASEBALL);	 
	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	m_pITableFrame->SetGameStatus(GS_WK_FREE);
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			m_pITableFrame->KillGameTimer(IDI_SET_BASEBALL);
			m_pITableFrame->KillGameTimer(IDI_HIT_BALL);
			m_pITableFrame->KillGameTimer(IDI_WAIT_SENDRESULT);
			m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
			////�������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));
			GameEnd.endReason = m_endReason;
			//�÷�==�׷�+������֮��*ϵ��(�ͷ�/10); wChairID,Ӯ��id
			int index = m_baseScore/10;
			if(index<1)
				index = 1;
			BYTE nextChair = (wChairID+1)%GAME_PLAYER;
			GameEnd.nGameScore[wChairID] = m_baseScore + (m_comob[wChairID]-m_comob[nextChair])*index;
			GameEnd.nGameScore[nextChair] = 0-GameEnd.nGameScore[wChairID];
		 		  
			//	//������Ϸ�÷�

			for(BYTE i =0; i< GAME_PLAYER; i++)
			{
				GameEnd.comobNum[i] = this->m_comob[i];
			}

			BYTE temp;
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];

			temp = GameEnd.nGameScore[wChairID]>0? SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
			ScoreInfoArray[wChairID].lScore = GameEnd.nGameScore[wChairID];
			ScoreInfoArray[wChairID].cbType = temp;
			ScoreInfoArray[wChairID].lRevenue = m_pITableFrame->CalculateRevenue(wChairID,ScoreInfoArray[wChairID].lScore);
			
			temp = GameEnd.nGameScore[nextChair]>0? SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
			ScoreInfoArray[nextChair].lScore = GameEnd.nGameScore[nextChair];
			ScoreInfoArray[nextChair].cbType = temp;
			ScoreInfoArray[nextChair].lRevenue = m_pITableFrame->CalculateRevenue(nextChair,ScoreInfoArray[nextChair].lScore);
			
			m_pITableFrame->WriteTableScore(ScoreInfoArray,GAME_PLAYER);

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);
			
			m_bGameStart = false;
			return true;
		}
	case	GER_USER_LEAVE:		//�û�ǿ��
	case	GER_DISMISS://��Ϸ��ɢ
	case	GER_NETWORK_ERROR:
		{
			
			m_pITableFrame->KillGameTimer(IDI_SET_BASEBALL);
			m_pITableFrame->KillGameTimer(IDI_HIT_BALL);
			m_pITableFrame->SetGameStatus(GAME_STATUS_FREE);
			////�������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));
			GameEnd.endReason = m_endReason;

			for(BYTE i =0; i< GAME_PLAYER; i++)
			{
				GameEnd.comobNum[i] =0;
			}

//			m_pITableFrame->WriteUserScore(WriteScoreInfo);
  	
			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);
			m_pITableFrame->SetGameStatus(GS_WK_FREE);
			m_bGameStart = false; 
			return true;
		}
	}
	ASSERT(FALSE);
	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)

{	
	//������Ϸ����
	switch (cbGameStatus)
	{
	case GS_WK_FREE:		//����״̬
		{		
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));
			//���ñ���
			StatusFree.wServerType = m_pGameServiceOption->wServerType;
			//���ͳ���
			m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
			return true;
		}
	case GS_WK_SET_SETBASEBALL:		 
	case GS_WK_WAIT_HITBALL:
		{
			CMD_S_StatusGamePlay  statusGamePlay;
			statusGamePlay.currentUser = m_wCurrentUser;
			statusGamePlay.gameStatus = m_pITableFrame->GetGameStatus();
			statusGamePlay.baseScore = m_baseScore;
			memcpy(statusGamePlay.ptBall,m_ptBallPos,sizeof(m_ptBallPos));
			for(int i=0;i<16;i++)
			{
				statusGamePlay.ballState[i] = true;
			}
			for(int i=0;i<2;i++)
			{
				for(int j=0;j<m_ballNum[i];j++)
				{
					statusGamePlay.inHoleBall[i][j] = m_ball[i][j];
					statusGamePlay.ballState[m_ball[i][j]] = false;
				}
				statusGamePlay.inHoleBall[i][7] = m_ballNum[i];
				statusGamePlay.maxComob[i] = m_comob[i];
				statusGamePlay.userBallType[i] = m_userBallType[i];
			}
			m_pITableFrame->SendGameScene(pIServerUserItem,&statusGamePlay,sizeof(statusGamePlay));
			if(cbGameStatus==GS_WK_WAIT_HITBALL&&m_bHit&&m_bSendHitResult[0]==false&&m_bSendHitResult[1]==false)
			{//�ȴ�����,�����Ѿ��������ˡ�˫����û���յ�����Ľ�������Ƿ��ͻ������Ϣ
				m_pITableFrame->SendTableData(wChairID,SUB_S_HITBALL,&m_hitBall,sizeof(CMD_S_HitBall));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_HITBALL,&m_hitBall,sizeof(CMD_S_HitBall));
			}
			return true;
		}
 	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			return true;
		}
	}

	return false;
}


//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam)
{

	switch (dwTimerID)
	{
	case IDI_SET_BASEBALL:
		{
			if(m_bGameStart==false)
				return true;
			if(m_pITableFrame->GetGameStatus()!=GS_WK_SET_SETBASEBALL)
			{
				ASSERT(0);
				return true;
			}
			m_cbPlayOutTimeNum[m_wCurrentUser]++;
			if(m_cbPlayOutTimeNum[m_wCurrentUser]>=5)
			{
				m_endReason = 4;//��ʱ
				OnEventGameConclude((m_wCurrentUser+1)%2,NULL,0);
				return true;
			}
			m_currentComob = 0;
			//ǿ������ĸ��
			CMD_S_END_SET_BASEBALL_TIMEOUT  endSetBaseBall;
			endSetBaseBall.ballPosx = 222;
			endSetBaseBall.ballPosy = 244;
			endSetBaseBall.timeOutCount = m_cbPlayOutTimeNum[m_wCurrentUser];
			m_pITableFrame->SetGameStatus(GS_WK_WAIT_HITBALL);
			m_pITableFrame->KillGameTimer(IDI_SET_BASEBALL);
			m_pITableFrame->SetGameTimer(IDI_HIT_BALL,TIME_HIT_BALL,1,0);			
			bool res = m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_END_SETBASEBALL_TIMEOUT,&endSetBaseBall,sizeof(endSetBaseBall));
			res = m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_END_SETBASEBALL_TIMEOUT,&endSetBaseBall,sizeof(endSetBaseBall));
			return true;

		}
		break;
	case IDI_HIT_BALL:
		{

			if(m_bGameStart==false)
				return true;
			m_bHit = false;
			m_pITableFrame->SetGameStatus(GS_WK_WAIT_HITBALL);
			m_pITableFrame->KillGameTimer(IDI_HIT_BALL);
			m_pITableFrame->SetGameTimer(IDI_HIT_BALL,TIME_HIT_BALL,1,0);
			if(m_pITableFrame->GetGameStatus()!=GS_WK_WAIT_HITBALL)
			{
				ASSERT(0);
				return true;
			}
			m_cbPlayOutTimeNum[m_wCurrentUser]++;
			if(m_cbPlayOutTimeNum[m_wCurrentUser]>=5)
			{
				m_endReason = 4;//��ʱ
				OnEventGameConclude((m_wCurrentUser+1)%2,NULL,0);
				return true;
			}
			CMD_S_HIT_TIMEOUT hitTimeOut;
			hitTimeOut.timeOutCount = m_cbPlayOutTimeNum[m_wCurrentUser];
			m_bWrongHit[m_wCurrentUser] = false;
			m_wCurrentUser = (m_wCurrentUser+1)%GAME_PLAYER;
			m_bWrongHit[m_wCurrentUser] = false;
			m_currentComob = 0;
			hitTimeOut.currentUser = m_wCurrentUser;			
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_HIT_TIMEOUT,&hitTimeOut,sizeof(hitTimeOut));		
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_HIT_TIMEOUT,&hitTimeOut,sizeof(hitTimeOut));
			return true;
		}
		break;
	case IDI_WAIT_SENDRESULT:
		{
			m_pITableFrame->KillGameTimer(IDI_WAIT_SENDRESULT);
			if(m_bSendHitResult[m_wCurrentUser]||m_bSendHitResult[(m_wCurrentUser+1)%2])
			{//ֻҪ��һ���˷�����������
				OnUserHitBallResult(&m_hitResult);
				return true;
			}
			else
			{//
				m_endReason = 5;//��Ϸ��ɢ
				OnEventGameConclude(0,NULL,GER_DISMISS);
			}
			return true;
		}
		break;
	}
	return true;
}

//��Ϸ��Ϣ����
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{	 
	case SUB_S_SETBASEBALL:
		{
			if(m_pITableFrame->GetGameStatus()!=GS_WK_SET_SETBASEBALL)
				return true;
			if(m_bGameStart==false)
				return true;
			CMD_S_SET_BASEBALL  *setBaseBall = (CMD_S_SET_BASEBALL*)pDataBuffer;
			if(wDataSize!=sizeof(CMD_S_SET_BASEBALL))
			{
				ASSERT(0);
				return false;
			}
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus==US_LOOKON) 
				return true;
			BYTE user = (m_wCurrentUser+1)%GAME_PLAYER;
			m_pITableFrame->SendTableData(user,SUB_S_SETBASEBALL,const_cast<void*>(pDataBuffer),sizeof(CMD_S_SET_BASEBALL));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SETBASEBALL,const_cast<void*>(pDataBuffer),sizeof(CMD_S_SET_BASEBALL));
			return true;
		}

	case SUB_S_END_SETBASEBALL:
		{
			if(m_bGameStart==false)
				return true;
			if(m_pITableFrame->GetGameStatus()!=GS_WK_SET_SETBASEBALL)
			{
				ASSERT(0);
				return true;
			}
			CMD_S_END_SET_BASEBALL  *setBaseBall = (CMD_S_END_SET_BASEBALL*)pDataBuffer;
			if(wDataSize!=sizeof(CMD_S_END_SET_BASEBALL))
			{
				ASSERT(0);
				return false;
			}
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus==US_LOOKON) 
				return true;
			//m_logger.LogMsg("chairid = %d,currentUser=%d,SUB_S_END_SETBASEBALL",pUserData->wChairID,m_wCurrentUser);

			m_pITableFrame->SetGameStatus(GS_WK_WAIT_HITBALL);
			m_pITableFrame->KillGameTimer(IDI_SET_BASEBALL);
			m_pITableFrame->SetGameTimer(IDI_HIT_BALL,TIME_HIT_BALL+2000,1,0);

			//BYTE user = (m_wCurrentUser+1)%GAME_PLAYER;
			bool res = m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_END_SETBASEBALL,const_cast<void*>(pDataBuffer),sizeof(CMD_S_SET_BASEBALL));
			res = m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_END_SETBASEBALL,const_cast<void*>(pDataBuffer),sizeof(CMD_S_SET_BASEBALL));
			return true;
		}
	case SUB_S_CLUBPOS:
		{
			if(m_bGameStart==false)
				return true;
			if(m_pITableFrame->GetGameStatus()!=GS_WK_WAIT_HITBALL)
				return true;
			CMD_S_BALLCLUB *club = (CMD_S_BALLCLUB*)pDataBuffer;
			if(wDataSize!=sizeof(CMD_S_BALLCLUB))
			{
				ASSERT(0);
				return false;
			}
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus==US_LOOKON) 
				return true;
			if(m_wCurrentUser!=pUserData->wChairID)
				return true;
			BYTE user = (m_wCurrentUser+1)%GAME_PLAYER;
			IServerUserItem *pUserItem = m_pITableFrame->GetTableUserItem(user);
			 
 			if(pUserItem->GetUserStatus()==US_OFFLINE)
			{//���߾Ͳ��÷���
				return true;
			}
			m_pITableFrame->SendTableData(user,SUB_S_CLUBPOS,const_cast<void*>(pDataBuffer),sizeof(CMD_S_BALLCLUB));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CLUBPOS,const_cast<void*>(pDataBuffer),sizeof(CMD_S_BALLCLUB));
			return true;

		}
	case SUB_S_HITBALL:
		{
			if(m_bGameStart==false)
				return true;
			if(m_pITableFrame->GetGameStatus()!=GS_WK_WAIT_HITBALL)
				return true;
			if(wDataSize!=sizeof(CMD_S_HitBall))
			{
				ASSERT(0);
				return false;
			}			
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus==US_LOOKON) 
				return true;
			//m_logger.LogMsg("chairid = %d,currentUser=%d,SUB_S_HITBALL",pUserData->wChairID,m_wCurrentUser);
			if(m_bHit==true)
			{
				return true;
			}
			if(pUserData->wChairID!=m_wCurrentUser)
			{
				return true;
			}
			memcpy(&m_hitBall,pDataBuffer,sizeof(m_hitBall));
			m_bSendHitResult[0] = false;
			m_bSendHitResult[1] = false;
			m_bHit = true;
			m_pITableFrame->KillGameTimer(IDI_HIT_BALL);
			BYTE user = (m_wCurrentUser+1)%GAME_PLAYER;
			m_pITableFrame->SendTableData(user,SUB_S_HITBALL,const_cast<void*>(pDataBuffer),sizeof(CMD_S_HitBall));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_HITBALL,const_cast<void*>(pDataBuffer),sizeof(CMD_S_HitBall));
			m_pITableFrame->SetGameTimer(IDI_WAIT_SENDRESULT,TIME_WAIT_SENDRESULT,1,0);
			return true;
		}
	case SUB_S_HIT_RESULT:
		{//������
			if(m_bGameStart==false)
				return true;
			if(!m_bHit)
			{//���û�д��򣬾��յ��������Ϣ����
				//ASSERT(0);			 	 
				return true;
			}
			if(wDataSize!=sizeof(CMD_S_HitResult))
			{
				ASSERT(0);
				return false;
			}
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
		/*	m_logger.LogMsg("chairid = %d,currentUser=%d,SUB_S_HIT_RESULT",pUserData->wChairID,m_wCurrentUser);*/					if (pUserData->cbUserStatus==US_LOOKON) 
				return true;
			m_bSendHitResult[pUserData->wChairID] = true;
			if(pUserData->wChairID==m_wCurrentUser)
			{
				memcpy(&m_hitResult,pDataBuffer,sizeof(m_hitResult));
			}
			else
			{
				if(m_bSendHitResult[m_wCurrentUser]==false)
				{//�Լ��ȷ�����Ϣ��������
					memcpy(&m_hitResult,pDataBuffer,sizeof(m_hitResult));
				}
			}
			BYTE user = (m_wCurrentUser+1)%2;
			IServerUserItem * pUserItem = m_pITableFrame->GetTableUserItem(user);
			IServerUserItem *pCurrentItem = m_pITableFrame->GetTableUserItem(m_wCurrentUser);
  
			if(m_bSendHitResult[0]&&m_bSendHitResult[1])
			{
				m_bHit = false;
				OnUserHitBallResult(&m_hitResult);
			}
			else if(m_bSendHitResult[m_wCurrentUser]&&(pUserItem->GetUserStatus()==US_OFFLINE))
			{//����Է�����
				m_bHit = false;
				OnUserHitBallResult(&m_hitResult);
			}
			else if(m_bSendHitResult[user]&&pCurrentItem->GetUserStatus()==US_OFFLINE)
			{//�����ǰ��ҵ���
				m_bHit = false;
				OnUserHitBallResult(&m_hitResult);
			}
			return true;
		}
	case SUB_S_GIVEUP:
		{
			if(m_bGameStart==false)
				return true;
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus==US_LOOKON) 
				return true;
			m_endReason = 2;//����
			OnEventGameConclude((pUserData->wChairID+1)%2,NULL,0);
			return true;
		}
	}

	return true;
}

//�����Ϣ����
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�����¼�
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}


bool CTableFrameSink::OnUserHitBallResult(const void * pDataBuffer)
{
	m_pITableFrame->KillGameTimer(IDI_WAIT_SENDRESULT);
	CMD_S_HitResult  *hitResult = (CMD_S_HitResult*)pDataBuffer;
	memcpy(m_ptBallPos,hitResult->ptBall,sizeof(m_ptBallPos));
//	static BYTE combomNum = 0;//������Ŀ

	CMD_S_CLIENT_HITRESLUT  clientHitResult;
	BYTE tempCurrentUser = m_wCurrentUser;
	bool firstHit = m_bFirstBall;
	bool bGameEnd = false;
	bool baseBallInHole = false;//�����Ƿ����
	bool exChangeUser = false;
	bool bHit8Num = false;//�Ƿ��Ǵ��8
	if(m_ballNum[m_wCurrentUser]==7)
		bHit8Num = true;
	BYTE endReason = 0;
	clientHitResult.hitResult = 0;
	if(hitResult->inHoleNum==0)
	{//���û�д��ȥ�򣬽������
		exChangeUser = true;
		clientHitResult.hitResult = noInHole;
	}
	else
	{//���������ȥ�ˣ�		
		for(int i=0;i<hitResult->inHoleNum;i++)
		{
			if(hitResult->ballInHole[i]==8)
			{//��Ϸ����			 
				m_ball[m_wCurrentUser][m_ballNum[m_wCurrentUser]++] = hitResult->ballInHole[i];
				bGameEnd = true;
				if(bHit8Num&&hitResult->firstBallNum==8)
				{//��������һ�����ˡ����Ҵ����8�����Ұ����ܽ�ȥ
					endReason = 1;//��ȷ����
					clientHitResult.hitResult = rightInHole;
					exChangeUser = false;
				}
				else
				{
					endReason = 2;//������
					clientHitResult.hitResult = wrongInHole;
					exChangeUser = true;
				}
			}
			else if(hitResult->ballInHole[i]==0)
			{//�������
				baseBallInHole = true;
				exChangeUser = true;
			}
			else if(hitResult->ballInHole[i]<8)
			{
				if(m_userBallType[m_wCurrentUser]==1)
					m_ball[(m_wCurrentUser+1)%2][m_ballNum[(m_wCurrentUser+1)%2]++] = hitResult->ballInHole[i];
				else
				{
					m_ball[m_wCurrentUser][m_ballNum[m_wCurrentUser]++] = hitResult->ballInHole[i];
					clientHitResult.hitResult = rightInHole;
				}
			}
			else
			{
				if(m_userBallType[m_wCurrentUser]==0)
					m_ball[(m_wCurrentUser+1)%2][m_ballNum[(m_wCurrentUser+1)%2]++] = hitResult->ballInHole[i];
				else
				{
					m_ball[m_wCurrentUser][m_ballNum[m_wCurrentUser]++] = hitResult->ballInHole[i];
					clientHitResult.hitResult = rightInHole;
				}
			}
		}		
		if(m_userBallType[0]==100&&m_userBallType[1] ==100)
		{
			firstHit = true;
			//�����״ν���ȷ����С��
			if(hitResult->ballInHole[0]>8&&(baseBallInHole==false||m_bFirstBall==false))
			{//�����Ǵ���
				m_bFirstBall = false;
				m_userBallType[m_wCurrentUser] = 1;
				m_userBallType[(m_wCurrentUser+1)%2] = 0;
			}
			else if(hitResult->ballInHole[0]<8&&hitResult->ballInHole[0]>0&&(baseBallInHole==false||m_bFirstBall==false))
			{
				m_bFirstBall = false;
				m_userBallType[m_wCurrentUser] = 0;
				m_userBallType[(m_wCurrentUser+1)%2] = 1;
			}
			else
			{//�������������
			}
		}
	}
	//��һ����������
	if(firstHit)
	{//����ǵ�һ�ο���
		if((baseBallInHole||hitResult->firstBallNum==17||hitResult->bHitWithWall==false)&&m_bFirstBall)
		{//�������������ʲô��û��,����û������ǽ
			exChangeUser = true;	
			clientHitResult.hitResult = openFail;
			m_bFirstBall = true;
			ZeroMemory(m_ball,sizeof(m_ball));
			ZeroMemory(m_ballNum,sizeof(m_ballNum));
			ZeroMemory(m_bWrongHit,sizeof(m_bWrongHit));
		}
		else if (hitResult->inHoleNum==0)
		{
			exChangeUser = true;	
			clientHitResult.hitResult = noInHole;
			m_bFirstBall = false;
		}
		else if(baseBallInHole)
		{
			exChangeUser = true;	
			clientHitResult.hitResult = wrongInHole;
			m_bWrongHit[m_wCurrentUser] = true;
			m_bFirstBall = false;
		}
		else if (hitResult->inHoleNum!=0)
		{
			m_bFirstBall = false;
			clientHitResult.hitResult = firtIn;
		}
		else
		{
		}
	}
	else if(baseBallInHole)
	{	
		if(!firstHit)
		{
			m_bWrongHit[m_wCurrentUser] = true;
			clientHitResult.hitResult = wrongInHole;
			exChangeUser = true;
		}
		if(bHit8Num&&hitResult->firstBallNum==8)
		{//���8��ʱ������������Ϸ����
			bGameEnd = true;
			endReason = 2;
			m_bWrongHit[m_wCurrentUser] = true;
			clientHitResult.hitResult = wrongInHole;
			exChangeUser = true;
		}		
	}
	else if(hitResult->firstBallNum==8)
	{
		if(m_ballNum[m_wCurrentUser]==7)
		{
			clientHitResult.hitResult = noInHole;
			exChangeUser = true;	
			m_bWrongHit[m_wCurrentUser] = false;
		}
		else if(m_ballNum[m_wCurrentUser]==8)
		{
			bGameEnd = true;
			endReason = 1;
		}
		else
		{
			clientHitResult.hitResult = wrongInHole;
			exChangeUser = true;
			m_bWrongHit[m_wCurrentUser] = true;
		}
	}
	else if(hitResult->firstBallNum==17)
	{//û�������κ���
		clientHitResult.hitResult = wrongInHole;
		exChangeUser = true;
		m_bWrongHit[m_wCurrentUser] = true;
	}
	else if (hitResult->firstBallNum>8&&m_userBallType[m_wCurrentUser]==0)
	{//
		clientHitResult.hitResult = wrongInHole;
		exChangeUser = true;
		m_bWrongHit[m_wCurrentUser] = true;

	}
	else if(hitResult->firstBallNum<8&&m_userBallType[m_wCurrentUser]==1)
	{
		clientHitResult.hitResult = wrongInHole;
		exChangeUser = true;
		m_bWrongHit[m_wCurrentUser] = true;
	}
	else if(clientHitResult.hitResult==0)
	{
		clientHitResult.hitResult = noInHole;
		m_bWrongHit[m_wCurrentUser] = false;
		exChangeUser = true;//û���Լ�����������������
		//m_wCurrentUser = (m_wCurrentUser+1)%GAME_PLAYER;	
	}
	if(hitResult->bHitWithWall==false&&hitResult->inHoleNum==0)
	{//û������ǽ������û�������
		if(clientHitResult.hitResult != openFail)
		{
			m_bWrongHit[m_wCurrentUser] = true;
			clientHitResult.hitResult = wrongInHole;
			exChangeUser = true;
		}
	}
	if(m_bWrongHit[m_wCurrentUser])
	{
		m_bWrongHit[(m_wCurrentUser+1)%GAME_PLAYER] = false;
	}
	
	if(exChangeUser)
	{
		m_wCurrentUser = (m_wCurrentUser+1)%GAME_PLAYER;	
		m_currentComob = 0;
		//combomNum = 0;
	}
	else
	{
		m_currentComob ++;
		//combomNum ++;
	}
	if(m_comob[m_wCurrentUser]<m_currentComob&&m_currentComob>=1)
			m_comob[m_wCurrentUser] = m_currentComob -1;

	ASSERT(clientHitResult.hitResult>0);
	if(exChangeUser&&clientHitResult.hitResult!=wrongInHole)
	{//����Է������ˣ���������
		if(m_bWrongHit[m_wCurrentUser])
		{
			m_bWrongHit[m_wCurrentUser] = false;
			m_wCurrentUser = (m_wCurrentUser+1)%GAME_PLAYER;
		}
	}
	
	//TRACE("before=%d,current=%d,exChangeUser=%d",tempCurrentUser,m_wCurrentUser,exChangeUser);

	for(int i=0;i<BALL_NUM;i++)
		clientHitResult.ballState[i] = true;
	//������Ϣ
	for(int i=0;i<GAME_PLAYER;i++)
	{
		for(int j=0;j<m_ballNum[i];j++)
		{
			if(m_ball[i][j]>0)
				clientHitResult.ballState[m_ball[i][j]] = false;
		}
	}
	m_bWrongHit[m_wCurrentUser] = false;
	if(baseBallInHole)
		clientHitResult.ballState[0] = false;//�������
	if(bGameEnd)
	{
		clientHitResult.hitResult = gameEnd;
	}
	memcpy(clientHitResult.ballInHole,hitResult->ballInHole,sizeof(clientHitResult.ballInHole));
	memcpy(clientHitResult.ptBall,hitResult->ptBall,sizeof(clientHitResult.ptBall));
	memcpy(clientHitResult.totalComobNum,this->m_comob,sizeof(m_comob));
	memcpy(clientHitResult.userBallType,this->m_userBallType,sizeof(m_userBallType));
	clientHitResult.currentUser = m_wCurrentUser;
	clientHitResult.firstBallNum = hitResult->firstBallNum;
	clientHitResult.inHoleNum = hitResult->inHoleNum;
	clientHitResult.comobNum = m_currentComob-1;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_HIT_RESULT,&clientHitResult,sizeof(clientHitResult));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_HIT_RESULT,&clientHitResult,sizeof(clientHitResult));
	if(baseBallInHole)
	{
		m_pITableFrame->SetGameStatus(GS_WK_SET_SETBASEBALL);
		m_pITableFrame->KillGameTimer(IDI_HIT_BALL);
		m_pITableFrame->SetGameTimer(IDI_SET_BASEBALL,TIME_SET_BASEBALL,1,0);
	}
	else
	{
		m_pITableFrame->SetGameStatus(GS_WK_WAIT_HITBALL);
		m_pITableFrame->KillGameTimer(IDI_HIT_BALL);
		m_pITableFrame->SetGameTimer(IDI_HIT_BALL,TIME_HIT_BALL+2000,1,0);
	}
	if(bGameEnd)
	{
		m_currentComob = 0;
		if(endReason==1)
		{//�����8����Ϸ����
			m_endReason = 1;//��������
			OnEventGameConclude(tempCurrentUser,NULL,0);
		}
		else
		{
			m_endReason = 3;//����
			OnEventGameConclude((tempCurrentUser+1)%GAME_PLAYER,NULL,0);
		}
	}
	return true;
}
//����ʧ�ܣ����¿���
void CTableFrameSink::OnFaultBegin()
{

}


	//�û�����
bool CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true;
}
	//�û�����
bool CTableFrameSink::OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true;
}
	//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{	
	if(!bLookonUser)
	{
		memset(m_nTatolScore,0,sizeof(m_nTatolScore));			//�ܵ���Ӯ����
		memset(m_nTurnScore,0,sizeof(m_nTatolScore));			//��һ����Ӯ
	}
	return true;
}
	//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if(!bLookonUser)
	{ 
		memset(m_nTatolScore,0,sizeof(m_nTatolScore));			//�ܵ���Ӯ����
		memset(m_nTurnScore,0,sizeof(m_nTatolScore));			//��һ����Ӯ
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
