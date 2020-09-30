#include "StdAfx.h"
#include "TableFrameSink.h"
#include <conio.h>

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
bool				CTableFrameSink::m_bFirstInit=true;
//////////////////////////////////////////////////////////////////////////

#define	IDI_SO_OPERATE							2							//����ʱ��
#define	TIME_SO_OPERATE							80000						//����ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����	
	m_lExitScore=0;	
	m_lDynamicScore=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wFisrtCallUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//�û�״̬
	ZeroMemory(m_cbDynamicJoin,sizeof(m_cbDynamicJoin));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbCallStatus,sizeof(m_cbCallStatus));
	ZeroMemory(m_bSpecialClient,sizeof(m_bSpecialClient));
	for(BYTE i=0;i<m_wPlayerCount;i++)m_cbOxCard[i]=0xff;

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע��Ϣ
	ZeroMemory(m_lTurnMaxScore,sizeof(m_lTurnMaxScore));

	ZeroMemory(m_bBuckleServiceCharge,sizeof(m_bBuckleServiceCharge));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	//�������
	m_hInst = NULL;
	m_pServerContro = NULL;
	m_hInst = LoadLibrary(TEXT("OxNewServerControl.dll"));
	if ( m_hInst )
	{
		typedef void * (*CREATE)(); 
		CREATE ServerControl = (CREATE)GetProcAddress(m_hInst,"CreateServerControl"); 
		if ( ServerControl )
		{
			m_pServerContro = static_cast<IServerControl*>(ServerControl());
		}
	}


	if(m_bFirstInit)
	{
		CString strName = GetFileDialogPath()+	"\\OxNewBattle.log";
		CFileFind findLogFile;
		if(findLogFile.FindFile(strName))
		{
			::DeleteFile(strName);
		}

		m_bFirstInit=false;
	}
	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
	if( m_pServerContro )
	{
		delete m_pServerContro;
		m_pServerContro = NULL;
	}

	if( m_hInst )
	{
		FreeLibrary(m_hInst);
		m_hInst = NULL;
	}
}

//�ӿڲ�ѯ--��������Ϣ�汾
void * CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
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

	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	//��Ϸ����
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();

	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//��ȡ����
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\OxConfig.ini"),szPath);

	//��ʼ�洢��Ŀ
	TCHAR szServerName[MAX_PATH]=TEXT("");
	_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);
	m_lStockScore=GetPrivateProfileInt(szServerName,TEXT("StorageStart"),0,szFileName);
	m_lStorageDeduct=GetPrivateProfileInt(szServerName,TEXT("StorageDeduct"),0,szFileName);
	m_lStockLimit=0;

	if(m_lStockScore>=m_lStockLimit)m_lStockLimit=0;

	if( m_lStorageDeduct < 0 || m_lStorageDeduct > 1000 )
		m_lStorageDeduct = 0;
	if ( m_lStorageDeduct > 1000 )
		m_lStorageDeduct = 1000;

	return true;
}

//��λ����
void CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	m_lExitScore=0;	
	m_wCurrentUser=INVALID_CHAIR;
	m_lDynamicScore=0;
	//�û�״̬
	ZeroMemory(m_cbDynamicJoin,sizeof(m_cbDynamicJoin));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_cbCallStatus,sizeof(m_cbCallStatus));
	ZeroMemory(m_bSpecialClient,sizeof(m_bSpecialClient));
	for(BYTE i=0;i<m_wPlayerCount;i++)m_cbOxCard[i]=0xff;

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע��Ϣ
	ZeroMemory(m_lTurnMaxScore,sizeof(m_lTurnMaxScore));

	return;
}

//��Ϸ״̬
bool CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	ASSERT(wChairID<m_wPlayerCount && m_cbPlayStatus[wChairID]==TRUE);
	if(wChairID<m_wPlayerCount && m_cbPlayStatus[wChairID]==TRUE)return true;
	return false;
}


//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false) m_HistoryScore.OnEventUserEnter(pIServerUserItem->GetChairID());

	if(m_pITableFrame->GetGameStatus()!=GS_TK_FREE)
		m_cbDynamicJoin[pIServerUserItem->GetChairID()]=true;

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false) 
	{
		m_HistoryScore.OnEventUserLeave(pIServerUserItem->GetChairID());
		m_cbDynamicJoin[pIServerUserItem->GetChairID()]=false;
		m_bSpecialClient[pIServerUserItem->GetChairID()]=false;
	}

	return true;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{

	//���
	if(m_lStockScore>0)m_lStockScore=m_lStockScore-m_lStockScore*m_lStorageDeduct/1000;

	CString strInfo;
	strInfo.Format(TEXT("[��%d��] ��ǰ��棺%I64d"),m_pITableFrame->GetTableID()+1, m_lStockScore);
	NcaTextOut(strInfo);

	//����״̬
	m_pITableFrame->SetGameStatus(GS_TK_CALL);

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
		if(pIServerUser!=NULL&&pIServerUser->IsAndroidUser())
		{
			m_pITableFrame->SendTableData(i,SUB_S_BANKER_OPERATE);
		}
	}

	ZeroMemory(m_bBuckleServiceCharge,sizeof(m_bBuckleServiceCharge));

	//�׾����ʼ��
	if(m_wFisrtCallUser==INVALID_CHAIR)
	{
		m_wFisrtCallUser=rand()%m_wPlayerCount;
	}
	else
	{
		m_wFisrtCallUser=(m_wFisrtCallUser+1)%m_wPlayerCount;
	}

	//ʼ���û�
	while(m_cbPlayStatus[m_wFisrtCallUser]!=TRUE)
	{
		m_wFisrtCallUser=(m_wFisrtCallUser+1)%m_wPlayerCount;
	}

	//��ǰ�û�
	m_wCurrentUser=m_wFisrtCallUser;

	//���ñ���
	CMD_S_CallBanker CallBanker;
	CallBanker.wCallBanker=m_wCurrentUser;
	CallBanker.bFirstTimes=true;

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_cbPlayStatus[i]!=TRUE) continue;
		m_pITableFrame->SendTableData(i,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
	m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);

	//����˿�
	CMD_S_AllCard AllCard={0};
	BYTE bTempArray[GAME_PLAYER*MAX_COUNT];
	m_GameLogic.RandCardList(bTempArray,sizeof(bTempArray));
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(i);	
		if(pIServerUser==NULL)continue;
		if(pIServerUser->IsAndroidUser())AllCard.bAICount[i] =true;

		//�ɷ��˿�
		CopyMemory(m_cbHandCardData[i],&bTempArray[i*MAX_COUNT],MAX_COUNT);
		CopyMemory(AllCard.cbCardData[i],&bTempArray[i*MAX_COUNT],MAX_COUNT);
	}

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(i);	
		if(pIServerUser==NULL)continue;
#ifndef _DEBUG
		if(CUserRight::IsGameCheatUser(pIServerUser->GetUserRight())==false || m_bSpecialClient[i]==false)continue;
#endif 
		m_pITableFrame->SendTableData(i,SUB_S_ALL_CARD,&AllCard,sizeof(AllCard));
	}
	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<GAME_PLAYER);

			//��������
			CMD_S_GameEnd GameEnd = {0};

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

			return true;
		}
	case GER_NORMAL:		//�������
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			WORD wWinTimes[GAME_PLAYER],wWinCount[GAME_PLAYER];
			ZeroMemory(wWinCount,sizeof(wWinCount));
			ZeroMemory(wWinTimes,sizeof(wWinTimes));

			//�����˿�
			BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
			CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

			//ׯ�ұ���
			ASSERT(m_cbOxCard[m_wBankerUser]<2);
			if(m_cbOxCard[m_wBankerUser]==TRUE)
				wWinTimes[m_wBankerUser]=m_GameLogic.GetTimes(cbUserCardData[m_wBankerUser],MAX_COUNT);
			else wWinTimes[m_wBankerUser]=1;

			//�Ա����
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==m_wBankerUser || m_cbPlayStatus[i]==FALSE)continue;

				ASSERT(m_cbOxCard[i]<2);

				//�Ա��˿�
				if (m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[m_wBankerUser],MAX_COUNT,m_cbOxCard[i],m_cbOxCard[m_wBankerUser])) 
				{
					wWinCount[i]++;
					//��ȡ����
					if(m_cbOxCard[i]==TRUE)
						wWinTimes[i]=m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT);
					else wWinTimes[i]=1;
				}
				else
				{
					wWinCount[m_wBankerUser]++;
				}
			}

			//ͳ�Ƶ÷�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==m_wBankerUser || m_cbPlayStatus[i]==FALSE)continue;

				if(wWinCount[i]>0)	//�м�ʤ��
				{
					GameEnd.lGameScore[i]=m_lTableScore[i]*wWinTimes[i];
					GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameScore[i];
					m_lTableScore[i]=0;
				}
				else					//ׯ��ʤ��
				{
					GameEnd.lGameScore[i]=(-1)*m_lTableScore[i]*wWinTimes[m_wBankerUser];
					GameEnd.lGameScore[m_wBankerUser]+=(-1)*GameEnd.lGameScore[i];
					m_lTableScore[i]=0;
				}
			}

			//�м�ǿ�˷���	
			GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;

			//�뿪�û�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_lTableScore[i]>0)GameEnd.lGameScore[i]=-m_lTableScore[i];
			}

			//�޸Ļ���
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

			//����˰��
			for(WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;

				if(GameEnd.lGameScore[i]>0L)
				{
					GameEnd.lGameTax[i] = m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);//GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenueRatio/1000l;
					if(GameEnd.lGameTax[i]>0)
						GameEnd.lGameScore[i] -= GameEnd.lGameTax[i];
				}

				//��ʷ����
				m_HistoryScore.OnEventUserScore(i,GameEnd.lGameScore[i]);

				//�������
				ScoreInfoArray[i].lScore = GameEnd.lGameScore[i];
				ScoreInfoArray[i].lRevenue = GameEnd.lGameTax[i];
				ScoreInfoArray[i].cbType = (GameEnd.lGameScore[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
			}

			//������Ϣ
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE) continue;
				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			
			TryWriteTableScore(ScoreInfoArray);
			
			//���ͳ��
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserIte==NULL) continue;

				//����ۼ�
				if(!pIServerUserIte->IsAndroidUser())
					m_lStockScore-=GameEnd.lGameScore[i];

			}

			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_TK_FREE);

			return true;
		}
	case GER_USER_LEAVE:		//�û�ǿ��
	case GER_NETWORK_ERROR:
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount && (m_cbPlayStatus[wChairID]==TRUE||m_cbDynamicJoin[wChairID]==FALSE));

			//����״̬
			m_cbPlayStatus[wChairID]=FALSE;
			m_cbDynamicJoin[wChairID]=FALSE;

			//�������
			CMD_S_PlayerExit PlayerExit;
			PlayerExit.wPlayerID=wChairID;

			//������Ϣ
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==wChairID || (m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE))continue;
				m_pITableFrame->SendTableData(i,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));


			WORD wWinTimes[GAME_PLAYER];
			
			ZeroMemory(wWinTimes,sizeof(wWinTimes));

			if (m_pITableFrame->GetGameStatus()>GS_TK_CALL)
			{
				if (wChairID==m_wBankerUser)	//ׯ��ǿ��
				{
					//�������
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd,sizeof(GameEnd));
					ZeroMemory(wWinTimes,sizeof(wWinTimes));

					BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
					CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

					//�÷ֱ���
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_cbPlayStatus[i]==FALSE)continue;
						wWinTimes[i]=(m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING)?(1):(m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT));
					}

					//ͳ�Ƶ÷� ���»�û��
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_cbPlayStatus[i]==FALSE)continue;
						GameEnd.lGameScore[i]=m_lTableScore[i]*wWinTimes[i];
						GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameScore[i];
						m_lTableScore[i]=0;
					}

					//�м�ǿ�˷��� 
					GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;

					//�뿪�û�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_lTableScore[i]>0)GameEnd.lGameScore[i]=-m_lTableScore[i];
					}

					//�޸Ļ���
					tagScoreInfo ScoreInfoArray[GAME_PLAYER];
					ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

					//����˰��
					for(WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_cbPlayStatus[i]==FALSE && i!=m_wBankerUser)continue;
						
						if(GameEnd.lGameScore[i]>0L)
						{
							GameEnd.lGameTax[i]=m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);//GameEnd.lGameScore[i]*m_pGameServiceOption->wRevenueRatio/1000l;
							if(GameEnd.lGameTax[i]>0)
								GameEnd.lGameScore[i]-=GameEnd.lGameTax[i];
						}

						//�������
						ScoreInfoArray[i].lRevenue = GameEnd.lGameTax[i];
						ScoreInfoArray[i].lScore = GameEnd.lGameScore[i];

						if(i==m_wBankerUser)
							ScoreInfoArray[i].cbType =SCORE_TYPE_FLEE;
						else if(m_cbPlayStatus[i]==TRUE)
							ScoreInfoArray[i].cbType =(GameEnd.lGameScore[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
					}
					
					TryWriteTableScore(ScoreInfoArray);

					//������Ϣ
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || (m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE))continue;
						m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					}
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
						
					//д����
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_cbPlayStatus[i]==FALSE && i!=m_wBankerUser)continue;

						//��ȡ�û�
						IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(i);

						//����ۼ�
						if ((pIServerUserIte!=NULL)&&(!pIServerUserIte->IsAndroidUser())) 
							m_lStockScore-=GameEnd.lGameScore[i];

					}
					//������Ϸ
					m_pITableFrame->ConcludeGame(GS_TK_FREE);
				}
				else						//�м�ǿ��
				{
					//�Ѿ���ע
					if (m_lTableScore[wChairID]>0L)
					{
						ZeroMemory(wWinTimes,sizeof(wWinTimes));

						//�û��˿�
						BYTE cbUserCardData[MAX_COUNT];
						CopyMemory(cbUserCardData,m_cbHandCardData[m_wBankerUser],MAX_COUNT);

						//�û�����
						wWinTimes[m_wBankerUser]=(m_pITableFrame->GetGameStatus()==GS_TK_SCORE)?(1):(m_GameLogic.GetTimes(cbUserCardData,MAX_COUNT));

						//�޸Ļ���
						LONGLONG lScore=-m_lTableScore[wChairID]*wWinTimes[m_wBankerUser];
						m_lExitScore+=(-1*lScore);
						m_lTableScore[wChairID]=(-1*lScore);
						
						tagScoreInfo ScoreInfoArray[GAME_PLAYER];
						ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));
						ScoreInfoArray[wChairID].lScore = lScore;
						ScoreInfoArray[wChairID].cbType = SCORE_TYPE_FLEE;		
						
						TryWriteTableScore(ScoreInfoArray);

						//��ȡ�û�
						IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(wChairID);
						
						//����ۼ�
						if ((pIServerUserIte!=NULL)&&(!pIServerUserIte->IsAndroidUser())) 
							m_lStockScore-=lScore;


					}

					//�������
					WORD wUserCount=0;
					for (WORD i=0;i<m_wPlayerCount;i++)if(m_cbPlayStatus[i]==TRUE)wUserCount++;

					//������Ϸ
					if(wUserCount==1)
					{
						//�������
						CMD_S_GameEnd GameEnd;
						ZeroMemory(&GameEnd,sizeof(GameEnd));
						ASSERT(m_lExitScore>=0L); 

						
						//ͳ�Ƶ÷�
						GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;
						GameEnd.lGameTax[m_wBankerUser]=m_pITableFrame->CalculateRevenue(m_wBankerUser,GameEnd.lGameScore[m_wBankerUser]);//GameEnd.lGameScore[m_wBankerUser]*m_pGameServiceOption->wRevenueRatio/1000l;
						GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameTax[m_wBankerUser];

						//�뿪�û�
						for (WORD i=0;i<m_wPlayerCount;i++)
						{
							if(m_lTableScore[i]>0)GameEnd.lGameScore[i]=-m_lTableScore[i];
						}

						//������Ϣ
						m_pITableFrame->SendTableData(m_wBankerUser,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
						m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

						for (WORD Zero=0;Zero<m_wPlayerCount;Zero++)if(m_lTableScore[Zero]!=0)break;
						if(Zero!=m_wPlayerCount)
						{
							//�޸Ļ���
							tagScoreInfo ScoreInfoArray[GAME_PLAYER];
							ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
							ScoreInfoArray[m_wBankerUser].lScore=GameEnd.lGameScore[m_wBankerUser];
							ScoreInfoArray[m_wBankerUser].lRevenue = GameEnd.lGameTax[m_wBankerUser];
							ScoreInfoArray[m_wBankerUser].cbType = SCORE_TYPE_WIN;

							TryWriteTableScore(ScoreInfoArray);

							//��ȡ�û�
							IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(wChairID);
							
							//����ۼ�
							if ((pIServerUserIte!=NULL)&&(!pIServerUserIte->IsAndroidUser())) 
								m_lStockScore-=GameEnd.lGameScore[m_wBankerUser];

						}

						//������Ϸ
						m_pITableFrame->ConcludeGame(GS_TK_FREE);		
					}
					else if	(m_pITableFrame->GetGameStatus()==GS_TK_SCORE && m_lTableScore[wChairID]==0L)
					{
						OnUserAddScore(wChairID,0);
					}
					else if (m_pITableFrame->GetGameStatus()==GS_TK_PLAYING && m_cbOxCard[wChairID]==0xff)
					{
						OnUserOpenCard(wChairID,0);
					}
				}
			}
			else 
			{
				//�������
				WORD wUserCount=0;
				for (WORD i=0;i<m_wPlayerCount;i++)if(m_cbPlayStatus[i]==TRUE)wUserCount++;

				//������Ϸ
				if(wUserCount==1)
				{
					//�������
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd,sizeof(GameEnd));

					//������Ϣ
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
						m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					}
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

					//������Ϸ
					m_pITableFrame->ConcludeGame(GS_TK_FREE);			
				}
				else if(m_wCurrentUser==wChairID)OnUserCallBanker(wChairID,0);
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
	case GAME_STATUS_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
			StatusFree.lCellScore=0L;

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusFree.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusFree.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//��������
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));


			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_TK_CALL:	//��ׯ״̬
		{
			//��������
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusCall.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusCall.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//���ñ���
			StatusCall.wCallBanker=m_wCurrentUser;
			StatusCall.cbDynamicJoin=m_cbDynamicJoin[wChiarID];
			CopyMemory(StatusCall.cbPlayStatus,m_cbPlayStatus,sizeof(StatusCall.cbPlayStatus));

			//��������
			CopyMemory(StatusCall.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusCall.szGameRoomName));


			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusCall,sizeof(StatusCall));
		}
	case GS_TK_SCORE:	//��ע״̬
		{
			//��������
			CMD_S_StatusScore StatusScore;
			memset(&StatusScore,0,sizeof(StatusScore));

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusScore.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusScore.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//��ע��Ϣ
			StatusScore.lTurnMaxScore=GetUserMaxTurnScore(wChiarID);
			StatusScore.wBankerUser=m_wBankerUser;
			StatusScore.cbDynamicJoin=m_cbDynamicJoin[wChiarID];
			CopyMemory(StatusScore.cbPlayStatus,m_cbPlayStatus,sizeof(StatusScore.cbPlayStatus));

			//���û���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;
				StatusScore.lTableScore[i]=m_lTableScore[i];
			}

			//��������
			CopyMemory(StatusScore.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusScore.szGameRoomName));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
		}
	case GS_TK_PLAYING:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusPlay.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusPlay.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//������Ϣ
			StatusPlay.lTurnMaxScore=GetUserMaxTurnScore(wChiarID);
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.cbDynamicJoin=m_cbDynamicJoin[wChiarID];
			CopyMemory(StatusPlay.bOxCard,m_cbOxCard,sizeof(StatusPlay.bOxCard));
			CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(StatusPlay.cbPlayStatus));

			//��������
			CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));

			//�����˿�
			for (WORD i=0;i< m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;
				WORD j= i;
				StatusPlay.lTableScore[j]=m_lTableScore[j];
				CopyMemory(StatusPlay.cbHandCardData[j],m_cbHandCardData[j],MAX_COUNT);
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
bool CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam)
{
	switch(dwTimerID)
	{
	case IDI_SO_OPERATE:
		{
			//ɾ��ʱ��
			m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

			//��Ϸ״̬
			switch( m_pITableFrame->GetGameStatus() )
			{
			case GS_TK_CALL:			//�û���ׯ
				{
					OnUserCallBanker(m_wCurrentUser, 0);
					break;
				}
			case GS_TK_SCORE:			//��ע����
				{
					for(WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_lTableScore[i]>0L || m_cbPlayStatus[i]==FALSE || i==m_wBankerUser)
							continue;

						if ( m_lTurnMaxScore[i] > 0 )
						{
							OnUserAddScore(i,m_lTurnMaxScore[i]/8);
						}
						else
						{
							OnUserAddScore(i,1);
						}
					}

					break;
				}
			case GS_TK_PLAYING:			//�û�����
				{
					for(WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_cbOxCard[i]<2 || m_cbPlayStatus[i]==FALSE)continue;
						OnUserOpenCard(i, 0);
					}

					break;
				}
			default:
				{
					break;
				}
			}

			if(m_pITableFrame->GetGameStatus()!=GS_TK_FREE)
				m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);
			return true;
		}
	}
	return false;
}

//��Ϸ��Ϣ���� 
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	bool bResult=false;
	switch (wSubCmdID)
	{
	case SUB_C_CALL_BANKER:			//�û���ׯ
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_CallBanker));
			if (wDataSize!=sizeof(CMD_C_CallBanker)) return false;

			//��������
			CMD_C_CallBanker * pCallBanker=(CMD_C_CallBanker *)pDataBuffer;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//״̬�ж�
			ASSERT(IsUserPlaying(pUserData->wChairID));
			if (!IsUserPlaying(pUserData->wChairID)) return false;

			//��Ϣ����
			bResult=OnUserCallBanker(pUserData->wChairID,pCallBanker->bBanker);
			break;
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
			bResult=OnUserAddScore(pUserData->wChairID,pAddScore->lScore);
			break;
		}
	case SUB_C_OPEN_CARD:			//�û�̯��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_OxCard));
			if (wDataSize!=sizeof(CMD_C_OxCard)) return false;

			//��������
			CMD_C_OxCard * pOxCard=(CMD_C_OxCard *)pDataBuffer;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]!=FALSE);
			if(m_cbPlayStatus[pUserData->wChairID]==FALSE)return false;

			//��Ϣ����
			bResult=OnUserOpenCard(pUserData->wChairID,pOxCard->bOX);
			break;
		}
	case SUB_C_SPECIAL_CLIENT_REPORT:  //����ͻ���
		{
			////Ч������
			//ASSERT(wDataSize==sizeof(CMD_C_SPECIAL_CLIENT_REPORT));
			//if (wDataSize!=sizeof(CMD_C_SPECIAL_CLIENT_REPORT)) return false;

			////��������
			//CMD_C_SPECIAL_CLIENT_REPORT * pSpecailClient=(CMD_C_SPECIAL_CLIENT_REPORT *)pDataBuffer;
			//if(pSpecailClient==NULL) return false;

			////���ñ���
			//m_bSpecialClient[pSpecailClient->wUserChairID]=true;

			return true;
		}
	case SUB_C_AMDIN_COMMAND:
		{
			ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
			if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;

			ASSERT(pIServerUserItem->GetUserStatus()!=GS_TK_PLAYING);
			if (pIServerUserItem->GetUserStatus()==GS_TK_PLAYING) return true;

			if ( m_pServerContro == NULL)
				return true;

			return m_pServerContro->ServerControl(wSubCmdID, pDataBuffer, wDataSize, pIServerUserItem, m_pITableFrame);
		}
	}

	//������ʱ��
	if(bResult)
	{
		m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);
		return true;
	}

	return false;
}

//�����Ϣ����
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}


//��ׯ�¼�
bool CTableFrameSink::OnUserCallBanker(WORD wChairID, BYTE bBanker)
{
	//״̬Ч��
	ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_CALL);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_CALL) return true;
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) return true;

	//���ñ���
	m_cbCallStatus[wChairID]=TRUE;

	//��ׯ����
	WORD wCallUserCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_cbPlayStatus[i]==TRUE && m_cbCallStatus[i]==TRUE) wCallUserCount++;
		else if(m_cbPlayStatus[i]!=TRUE) wCallUserCount++;
	}

	if(bBanker==FALSE && wCallUserCount==m_wPlayerCount)
	{
		m_wBankerUser=m_wFisrtCallUser;
	}

	

	//��ע��ʼ
	if(bBanker==TRUE || wCallUserCount==m_wPlayerCount)
	{
		//ʼ���û�
		if(bBanker==TRUE)
		{
			m_wBankerUser=wChairID;
		}
		m_wCurrentUser=INVALID_CHAIR;
		
		//�������һ����ׯ�û�ǿ�����
		while(m_cbPlayStatus[m_wBankerUser]==FALSE)
		{
			m_wBankerUser=(m_wBankerUser+1)%GAME_PLAYER;
		}
		m_bBuckleServiceCharge[m_wBankerUser]=true;
		//����״̬
		m_pITableFrame->SetGameStatus(GS_TK_SCORE);

		//ׯ�һ���
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wBankerUser);
		LONGLONG lBankerScore=pIServerUserItem->GetUserScore();

		//�����ע
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]!=TRUE || i==m_wBankerUser)continue;

			//��ע���� �ͻ�Ҫ��
			m_lTurnMaxScore[i]=GetUserMaxTurnScore(i);
		}

		//���ñ���
		CMD_S_GameStart GameStart;
		GameStart.wBankerUser=m_wBankerUser;
		GameStart.lTurnMaxScore=0;

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
			GameStart.lTurnMaxScore=m_lTurnMaxScore[i];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	else		 //�û���ׯ
	{
		//�����¸����
		do{
			m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		}while(m_cbPlayStatus[m_wCurrentUser]==FALSE);

		//���ñ���
		CMD_S_CallBanker CallBanker;
		CallBanker.wCallBanker=m_wCurrentUser;
		CallBanker.bFirstTimes=false;

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
	}
	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserAddScore(WORD wChairID, LONGLONG lScore)
{
	//״̬Ч��
	ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_SCORE);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_SCORE) return true;

	//���Ч��
	if(m_cbPlayStatus[wChairID]==TRUE)
	{
		ASSERT(lScore>0 && lScore<=m_lTurnMaxScore[wChairID]);
		if (lScore<=0 || lScore>m_lTurnMaxScore[wChairID]) return false;
	}
	else //û��ע���ǿ��
	{
		ASSERT(lScore==0);
		if (lScore!=0) return false;
	}

	if(!UserCanAddScore(wChairID,lScore)) return false;

	if(lScore>0L)
	{
		//��ע���
		m_lTableScore[wChairID]=lScore;
		m_bBuckleServiceCharge[wChairID]=true;
		//��������
		CMD_S_AddScore AddScore;
		AddScore.wAddScoreUser=wChairID;
		AddScore.lAddScoreCount=m_lTableScore[wChairID];

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
	}

	//��ע����
	BYTE bUserCount=0;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_lTableScore[i]>0L && m_cbPlayStatus[i]==TRUE)bUserCount++;
		else if(m_cbPlayStatus[i]==FALSE || i==m_wBankerUser)bUserCount++;
	}

	//�м�ȫ��
	if(bUserCount==m_wPlayerCount)
	{
		//����״̬
		m_pITableFrame->SetGameStatus(GS_TK_PLAYING);

		//��������
		CMD_S_SendCard SendCard;
		ZeroMemory(SendCard.cbCardData,sizeof(SendCard.cbCardData));

		//�����˿�
		AnalyseCard();

		//����
		bool bControl = false;
		BYTE bMaxCount=0;
		if( m_pServerContro != NULL && m_pServerContro->NeedControl() )//�ڴ˴��������
		{
			if(m_pServerContro->ControlResult(m_cbHandCardData))
			{
				bControl=true;
				m_pServerContro->CompleteControl();

			}			
		}

		//�����˿�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE)continue;

			//�ɷ��˿�
			CopyMemory(SendCard.cbCardData[i],m_cbHandCardData[i],MAX_COUNT);
		}

		//��������
		for (WORD i=0;i< m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	}

	return true;
}

//̯���¼�
bool CTableFrameSink::OnUserOpenCard(WORD wChairID, BYTE bOx)
{
	//״̬Ч��
	ASSERT (m_pITableFrame->GetGameStatus()==GS_TK_PLAYING);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING) return true;
	if (m_cbOxCard[wChairID]!=0xff) return true;

	//Ч������
	ASSERT(bOx<=TRUE);
	if(bOx>TRUE)return false;

	//Ч������
	if(bOx)
	{
		ASSERT(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>0);
		if(!(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>0))return false;
	}
	else if(m_cbPlayStatus[wChairID]==TRUE)
	{
		if(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>=OX_THREE_SAME)bOx=TRUE;
	}

	//ţţ����
	m_cbOxCard[wChairID] = bOx;

	//̯������
	BYTE bUserCount=0;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_cbOxCard[i]<2 && m_cbPlayStatus[i]==TRUE)bUserCount++;
		else if(m_cbPlayStatus[i]==FALSE)bUserCount++;
	}

	 //�������
	CMD_S_Open_Card OpenCard;
	ZeroMemory(&OpenCard,sizeof(OpenCard));

	//���ñ���
	OpenCard.bOpen=bOx;
	OpenCard.wPlayerID=wChairID;

	//��������
	for (WORD i=0;i< m_wPlayerCount;i++)
	{
		if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
		m_pITableFrame->SendTableData(i,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));	

	//������Ϸ
	if(bUserCount == m_wPlayerCount)
	{
		return OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
	}

	return true;
}

//�˿˷���
void CTableFrameSink::AnalyseCard()
{
	//��������
	bool bIsAiBanker = false;
	WORD wAiCount = 0;
	WORD wPlayerCount = 0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem!=NULL)
		{
			if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
			if(pIServerUserItem->IsAndroidUser()) 
			{
				wAiCount++ ;
				if(!bIsAiBanker && i==m_wBankerUser)bIsAiBanker = true;
			}
			wPlayerCount++; 
		}
	}

	//ȫ������
	if(wPlayerCount == wAiCount || wAiCount==0)return;

	//�˿˱���
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

	//ţţ����
	BOOL bUserOxData[GAME_PLAYER];
	ZeroMemory(bUserOxData,sizeof(bUserOxData));
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_cbPlayStatus[i]==FALSE)continue;
		bUserOxData[i] = (m_GameLogic.GetCardType(cbUserCardData[i],MAX_COUNT)>0)?TRUE:FALSE;
	}

	//�����˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_GameLogic.SortCardList(cbUserCardData[i],MAX_COUNT);
	}

	//��������
	LONGLONG lAndroidScore=0;

	//��������
	BYTE cbCardTimes[GAME_PLAYER];
	ZeroMemory(cbCardTimes,sizeof(cbCardTimes));

	//���ұ���
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			cbCardTimes[i]=m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT);
		}
	}

	//����ׯ��
	if(bIsAiBanker)
	{
		//�Ա��˿�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//�û�����
			if ((i==m_wBankerUser)||(m_cbPlayStatus[i]==FALSE)) continue;

			//��ȡ�û�
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

			//��������
			if ((pIServerUserItem!=NULL)&&(pIServerUserItem->IsAndroidUser())) continue;

			//�Ա��˿�
			if (m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[m_wBankerUser],MAX_COUNT,bUserOxData[i],bUserOxData[m_wBankerUser])==true)
			{
				lAndroidScore-=cbCardTimes[i]*m_lTableScore[i];
			}
			else
			{
				lAndroidScore+=cbCardTimes[m_wBankerUser]*m_lTableScore[i];
			}
		}
	}
	else//�û�ׯ��
	{
		//�Ա��˿�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

			//�û�����
			if ((i==m_wBankerUser)||(pIServerUserItem==NULL)||!(pIServerUserItem->IsAndroidUser())) continue;

			//�Ա��˿�
			if (m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[m_wBankerUser],MAX_COUNT,bUserOxData[i],bUserOxData[m_wBankerUser])==true)
			{
				lAndroidScore+=cbCardTimes[i]*m_lTableScore[i];
			}
			else
			{
				lAndroidScore-=cbCardTimes[m_wBankerUser]*m_lTableScore[i];
			}
		}
	}

	//����ж�
	bool bCon[]={((m_lStockScore+lAndroidScore)<0),false};
	bool bMaxOrMin[]={bIsAiBanker,!bIsAiBanker};
	for(WORD j=0;j<CountArray(bCon);j++)
	{
		if(!bCon[j])continue;
		//��������
		WORD wWinUser=INVALID_CHAIR;

		//�ж����
		bool bCardMax = bMaxOrMin[j];

		//��������
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//�û�����
			if (m_cbPlayStatus[i]==FALSE) continue;

			//�����û�
			if (wWinUser==INVALID_CHAIR)
			{
				wWinUser=i;
				continue;
			}

			//�Ա��˿�
			if (m_GameLogic.CompareCard(cbUserCardData[i],m_cbHandCardData[wWinUser],MAX_COUNT,bUserOxData[i],bUserOxData[wWinUser])==bCardMax)
			{
				wWinUser=i;
			}
		}

		//��������
		BYTE cbTempData[MAX_COUNT];
		CopyMemory(cbTempData,m_cbHandCardData[m_wBankerUser],MAX_COUNT);
		CopyMemory(m_cbHandCardData[m_wBankerUser],m_cbHandCardData[wWinUser],MAX_COUNT);
		CopyMemory(m_cbHandCardData[wWinUser],cbTempData,MAX_COUNT);
		if(bCon[j])break;
	}
	return;
}


//��ѯ�Ƿ�۷����
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pUserItem=m_pITableFrame->GetTableUserItem(i);
		if(pUserItem==NULL) continue;
		
		if (m_bBuckleServiceCharge[i]&&i==wChairID)
		{
			return true;
		}
		
	}
	return false;
}


bool CTableFrameSink::TryWriteTableScore(tagScoreInfo ScoreInfoArray[])
{
	//�޸Ļ���
	tagScoreInfo ScoreInfo[GAME_PLAYER];
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	memcpy(&ScoreInfo,ScoreInfoArray,sizeof(ScoreInfo));
	//��¼�쳣
	LONGLONG beforeScore[GAME_PLAYER];
	ZeroMemory(beforeScore,sizeof(beforeScore));
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		IServerUserItem *pItem=m_pITableFrame->GetTableUserItem(i);
		if(pItem!=NULL)
		{
			beforeScore[i]=pItem->GetUserScore();

		}
	}
	m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
	LONGLONG afterScore[GAME_PLAYER];
	ZeroMemory(afterScore,sizeof(afterScore));
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		IServerUserItem *pItem=m_pITableFrame->GetTableUserItem(i);
		if(pItem!=NULL)
		{
			afterScore[i]=pItem->GetUserScore();

			if(afterScore[i]<0)
			{
				//�쳣д����־

				CString strInfo;
				strInfo.Format(TEXT("[%s] ���ָ���"),pItem->GetNickName());
				NcaTextOut(strInfo);

				strInfo.Format(TEXT("д��ǰ������%I64d"),beforeScore[i]);
				NcaTextOut(strInfo);

				strInfo.Format(TEXT("д����Ϣ��д����� %I64d��˰�� %I64d"),ScoreInfoArray[i].lScore,ScoreInfoArray[i].lRevenue);
				NcaTextOut(strInfo);

				strInfo.Format(TEXT("д�ֺ������%I64d"),afterScore[i]);
				NcaTextOut(strInfo);

			}

		}
	}
	return true;
}

//����·�
SCORE CTableFrameSink::GetUserMaxTurnScore(WORD wChairID)
{

	SCORE lMaxTurnScore=0L;
	if(wChairID==m_wBankerUser)  return 0;
	//ׯ�һ���
	IServerUserItem *pIBankerItem=m_pITableFrame->GetTableUserItem(m_wBankerUser);
	LONGLONG lBankerScore=0L;
	if(pIBankerItem!=NULL)
		lBankerScore=pIBankerItem->GetUserScore();

	//�������
	WORD wUserCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
		if(m_cbPlayStatus[i]==TRUE )wUserCount++;

	//��ȡ�û�
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

	if(pIServerUserItem!=NULL)
	{
		//��ȡ����
		LONGLONG lScore=pIServerUserItem->GetUserScore();

		lMaxTurnScore=__min(lBankerScore/(wUserCount-1)/MAX_TIMES,lScore/MAX_TIMES);
	}

	return lMaxTurnScore;
	

}

//�Ƿ�ɼ�
bool CTableFrameSink::UserCanAddScore(WORD wChairID, LONGLONG lAddScore)
{

	//��ȡ�û�
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

	if(pIServerUserItem!=NULL)
	{
		//��ȡ����
		LONGLONG lScore=pIServerUserItem->GetUserScore();

		if(lAddScore>lScore/MAX_TIMES)
		{
			return false;
		}
	}
	return true;

}

//��ѯ�޶�
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	SCORE consumeQuota=0L;
	/*SCORE lMinTableScore=m_pGameServiceOption->lMinTableScore;
	if(m_pITableFrame->GetGameStatus()==GAME_STATUS_FREE)
	{
		consumeQuota=pIServerUserItem->GetUserScore()-lMinTableScore;

	}*/
	return consumeQuota;
}


//////////////////////////////////////////////////////////////////////////
