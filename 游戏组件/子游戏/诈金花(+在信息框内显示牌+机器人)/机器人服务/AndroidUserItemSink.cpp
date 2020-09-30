#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS					1									//����ʱ��
#define TIME_DISPATCH				5									//����ʱ��

//��Ϸʱ��
#define TIME_START_GAME				8									//��ʼʱ��
#define TIME_CALL_SCORE				6									//�з�ʱ��
#define TIME_GAME_OPENEND			4
#define TIME_GAME_COMPAREEND		8


//��ʱ����ʶ
#define IDI_USER_ADD_SCORE		(0)									//��ע��ʱ��
#define IDI_START_GAME			(1)									//��ʼ��ʱ��
#define IDI_USER_COMPARE_CARD	(2)									//ѡ�����û���ʱ��
#define IDI_GIVE_UP				(3)
#define IDI_LOOK_CARD			(4)
#define IDI_OPEN_CARD			(5)
//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;

	m_lCurrentTimes=1L;
	m_lMaxScore=0L;												//�ⶥ��Ŀ
	m_lMaxCellScore=0L;											//��Ԫ����
	m_lCellScore=0L;											//��Ԫ��ע
	m_lUserMaxScore=0L;											//������
	m_cbPlayerCount=0;
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));			//��Ϸ���
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));			//�����ע
	
	//�����˿�
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbAllUserCardData,sizeof(m_cbAllUserCardData));
	ZeroMemory(m_bMingZhu,sizeof(m_bMingZhu));
	//�ӿڱ���
	m_pIAndroidUserItem=NULL;;

	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
VOID * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
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
	m_wBankerUser=INVALID_CHAIR;

	m_lCurrentTimes=1L;
	m_lMaxScore=0L;												//�ⶥ��Ŀ
	m_lMaxCellScore=0L;											//��Ԫ����
	m_lCellScore=0L;											//��Ԫ��ע
	m_lUserMaxScore=0L;											//������
	m_cbPlayerCount=0;
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));			//��Ϸ���
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));			//�����ע
	
	//�����˿�
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbAllUserCardData,sizeof(m_cbAllUserCardData));
	ZeroMemory(m_bMingZhu,sizeof(m_bMingZhu));

	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:	//��ʼ��Ϸ
		{
			//��ʼ�ж�
			m_pIAndroidUserItem->SendUserReady(NULL,0);
			return true;
		}
	case IDI_USER_ADD_SCORE:	//�û���ע
		{
			//�����ϼ�
			for (LONGLONG j=m_pIAndroidUserItem->GetChairID()-1;;j--)
			{
				if(j==-1)j=GAME_PLAYER-1;
				if(m_cbPlayStatus[j]==TRUE)break;
			}
			//һΪ��ע
			WORD wTemp=(m_lTableScore[j]==m_lCellScore)?0:1;
			//��ȡ����
			LONGLONG lCurrentScore=(wTemp==0)?m_lCellScore:(m_lCellScore*m_lCurrentTimes);

			WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
			//��ע�ӱ�
			if(m_bMingZhu[wMeChairID])lCurrentScore*=2;
			//������Ϣ
			CMD_C_AddScore AddScore;
			AddScore.wState=0;
			AddScore.lScore=lCurrentScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
			
			return true;
		}
	case IDI_GIVE_UP:
		{
			m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP,NULL,0);
			return true;
		}
	case IDI_LOOK_CARD:
		{
			m_pIAndroidUserItem->SendSocketData(SUB_C_LOOK_CARD,NULL,0);
			return true;
		}
	case IDI_OPEN_CARD:
		{


			WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
			//�ж���ע
			LONGLONG lCurrentScore=(m_bMingZhu[wMeChairID])?(m_lCurrentTimes*m_lCellScore*4):(m_lCurrentTimes*m_lCellScore*2);

			//������Ϣ
			CMD_C_AddScore AddScore;
			AddScore.wState=2;
			AddScore.lScore=lCurrentScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			if(m_cbPlayerCount==2)
			{
				CMD_C_CompareCard CompareCard;
				//�����ϼ�
				for (LONGLONG i=(LONGLONG)wMeChairID-1;;i--)
				{
					if(i==-1)i=GAME_PLAYER-1;
					if(m_cbPlayStatus[i]==TRUE)
					{
						CompareCard.wCompareUser=(WORD)i;
						break;
					}
				}
				m_pIAndroidUserItem->SendSocketData(SUB_C_COMPARE_CARD,&CompareCard,sizeof(CompareCard));
				return true;
			}
			//������Ϣ
			m_pIAndroidUserItem->SendSocketData(SUB_C_OPEN_CARD,NULL,0);
			return true;
		}
	case IDI_USER_COMPARE_CARD:		//�û�����
		{
			//�ж���ע
			WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
			LONGLONG lCurrentScore=(m_bMingZhu[wMeChairID])?(m_lCurrentTimes*m_lCellScore*4):(m_lCurrentTimes*m_lCellScore*2);

			//������Ϣ
			CMD_C_AddScore AddScore;
			AddScore.wState=TRUE;
			AddScore.lScore=lCurrentScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			//�������
			CMD_C_CompareCard CompareCard;
			//ׯ���ڵ�һ��û��עֻ�ܸ��ϼұ��� �� ֻʣ������
			if(m_wBankerUser==wMeChairID && (m_lTableScore[wMeChairID]-lCurrentScore)==m_lCellScore || m_cbPlayerCount==2)
			{
				//�����ϼ�
				for (LONGLONG i=(LONGLONG)wMeChairID-1;;i--)
				{
					if(i==-1)i=GAME_PLAYER-1;
					if(m_cbPlayStatus[i]==TRUE)
					{
						CompareCard.wCompareUser=(WORD)i;
						break;
					}
				}
			}
			else	//ѡ����ұ���
			{
				m_pIAndroidUserItem->SendSocketData(SUB_C_WAIT_COMPARE,NULL,0);

				WORD wCompare=INVALID_CHAIR;
				for(int i=0;i<GAME_PLAYER;i++)
				{
					if(m_cbPlayStatus[i]==TRUE && wMeChairID!=i&& m_GameLogic.CompareMyCard(wMeChairID,i))
					{
						wCompare=(WORD)i;
						break;
					}
				}
				if(wCompare==INVALID_CHAIR)
				{
					for(int i=0;i<GAME_PLAYER;i++)
					{
						if(m_cbPlayStatus[i]==TRUE && wMeChairID!=i)
						{
							wCompare=(WORD)i;
							break;
						}
					}
				}

				CompareCard.wCompareUser=wCompare;
			}
			//������Ϣ
			m_pIAndroidUserItem->SendSocketData(SUB_C_COMPARE_CARD,&CompareCard,sizeof(CompareCard));
			return true;
		}
	}

	return false;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			//��Ϣ����
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_LOOK_CARD:		//������Ϣ
		{
			return OnSubLookCard(pData,wDataSize);
		}
	case SUB_S_ADD_SCORE:		//�û���ע
		{
			//��Ϣ����
			return OnSubAddScore(pData,wDataSize);
		}
	case SUB_S_COMPARE_CARD:	//������Ϣ
		{
			//��Ϣ����
			return OnSubCompareCard(pData,wDataSize);
		}
	case SUB_S_GIVE_UP:			//�û�����
		{
			//��Ϣ����
			return OnSubGiveUp(pData,wDataSize);
		}
	case SUB_S_PLAYER_EXIT:		//�û�ǿ��
		{
			//��Ϣ����
			return OnSubPlayerExit(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			//��Ϣ����
			return OnSubGameEnd(pData,wDataSize);
		}
	}
	//�������
//	ASSERT(FALSE);

	return true;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			IServerUserItem * pIServerUserItem=m_pIAndroidUserItem->GetMeUserItem();

			//�������
			if (pIServerUserItem->GetUserStatus()!=US_READY)
			{
				UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
			}

			return true;
		}
	case GAME_STATUS_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

						//�˿�����
			WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
			CopyMemory(m_cbHandCardData,pStatusPlay->cbHandCardData,sizeof(BYTE)*3);

			//�������
			if (pStatusPlay->wCurrentUser==m_pIAndroidUserItem->GetChairID())
			{
				UINT nElapse=rand()%TIME_CALL_SCORE+TIME_LESS+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
			}

			return true;
		}
	}

	return false;
}

//�������
void CAndroidUserItemSink::UpdataControl(WORD wCurrentUser)
{
	BYTE cbMyCardLevel=0;
	bool bMingZhu=false;
	bool bGiveUp=false;
	bool bAddScore=false;
	bool bCompare=false;

	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if((!m_cbPlayStatus[i])||(i==wCurrentUser)) continue;
		if(m_GameLogic.CompareMyCard(wCurrentUser,i)) cbMyCardLevel++;
	}

	LONGLONG m_lCurrentScore=m_bMingZhu[wCurrentUser]?(2*m_lCellScore*m_lCurrentTimes):(m_lCellScore*m_lCurrentTimes);
	if(cbMyCardLevel==0)
	{
		if((m_cbPlayerCount>2)&&(m_lTableScore[wCurrentUser]==m_lCellScore))
		{
			bMingZhu=true;
			bGiveUp=true;
		}
		else
		{
			bAddScore=((m_lTableScore[wCurrentUser]+m_lCurrentScore)<2*m_lCellScore)?true:false;
			bMingZhu=true;
			if((!bAddScore)&&((m_lTableScore[wCurrentUser])>5*m_lCellScore))
			{
				if(wCurrentUser==m_wBankerUser || m_lTableScore[wCurrentUser]>=2*m_lCellScore)
				{
					bCompare=true;
				}else{
					bAddScore=true;
				}
			}else{
				bGiveUp=true;
			}
		}
	}
	else if(1==(m_cbPlayerCount-cbMyCardLevel))
	{
		bMingZhu=(m_lTableScore[wCurrentUser]>=m_lMaxCellScore)?true:false;
		bAddScore=((m_lTableScore[wCurrentUser]+m_lCurrentScore) <= m_lUserMaxScore)?true:false;
		if(!bAddScore)	bCompare=true;
	}
	else
	{
		bMingZhu=(m_lTableScore[wCurrentUser]>=3*m_lMaxCellScore)?true:false;
		bAddScore=((m_lTableScore[wCurrentUser]+m_lCurrentScore) < m_lUserMaxScore/10)?true:false;
		if(!bAddScore) bCompare=true;
	}

	//���ư�ť
	if((!m_bMingZhu[wCurrentUser])&&bMingZhu)
	{
		UINT nElapse=rand()%TIME_GAME_OPENEND+TIME_LESS+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_LOOK_CARD,nElapse);
		return;
	}
	if(bGiveUp)
	{
		UINT nElapse=rand()%TIME_CALL_SCORE+TIME_LESS+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_GIVE_UP,nElapse);
		return;
	}
	//�жϿ���
	LONGLONG lTemp=(m_bMingZhu[wCurrentUser])?6:5;
	if((m_lUserMaxScore-m_lTableScore[wCurrentUser]) >= (m_lMaxCellScore*lTemp))
	{
		//��ע��ť
		if(bAddScore)
		{
			UINT nElapse=rand()%TIME_CALL_SCORE+TIME_LESS+TIME_LESS;
			m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
			return;
		}
		//���ư�ť
		if(bCompare)
		{
			if(wCurrentUser==m_wBankerUser || m_lTableScore[wCurrentUser]>=2*m_lCellScore)
			{
				UINT nElapse=rand()%TIME_GAME_COMPAREEND+TIME_LESS+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_USER_COMPARE_CARD,nElapse);
				return;
			}
		}
	}
	else 
	{
		UINT nElapse=rand()%TIME_CALL_SCORE+TIME_LESS+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);
	}

	return;
}
//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_S_AndroidCard));
	if (wDataSize!=sizeof(CMD_S_AndroidCard)) return false;

	//��������
	CMD_S_AndroidCard * pAndroidCard=(CMD_S_AndroidCard *)pData;

	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GAME_STATUS_PLAY);

	//�����˿�
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();

	for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		if(wMeChairID==wChairID)
			CopyMemory(m_cbHandCardData,pAndroidCard->cbHandCard[wChairID],sizeof(BYTE)*MAX_COUNT);
		m_GameLogic.SetUserCard(wChairID, pAndroidCard->cbHandCard[wChairID]) ;
	}
	//�����˿�
	m_GameLogic.SortCardList(m_cbHandCardData,3);

	//������Ϣ
	m_lCellScore=pAndroidCard->lCellScore;
	m_lMaxCellScore=pAndroidCard->lMaxScore;
	m_lCurrentTimes=pAndroidCard->lCurrentTimes;
	m_wBankerUser=pAndroidCard->m_wBankerUser;
	m_lUserMaxScore=pAndroidCard->lUserMaxScore;

	//���ñ���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		if (!pAndroidCard->cbPlayStatus[i]) continue;
		//��Ϸ��Ϣ		
		m_cbPlayStatus[i]=TRUE;
		m_cbPlayerCount++;
		m_lTableScore[i]=m_lCellScore;
	}

	//��Ҵ���
	if (m_pIAndroidUserItem->GetChairID()==pAndroidCard->wCurrentUser)
	{
		UpdataControl(pAndroidCard->wCurrentUser);
	}

	return true;
}

//�û��з�
bool CAndroidUserItemSink::OnSubAddScore(VOID * pData, WORD wDataSize)
{
//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_AddScore));
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pData;
	ASSERT(pAddScore->wCurrentUser < GAME_PLAYER);

	m_lCurrentTimes=pAddScore->lCurrentTimes;
	//��ע���
	m_lTableScore[pAddScore->wAddScoreUser]+=pAddScore->lAddScoreCount;
	if(m_pIAndroidUserItem->GetChairID()==pAddScore->wCurrentUser)UpdataControl(m_pIAndroidUserItem->GetChairID());

	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnSubCompareCard(VOID * pData, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_CompareCard)) return false;
	CMD_S_CompareCard * pCompareCard=(CMD_S_CompareCard *)pData;

	//���ñ���
	m_cbPlayStatus[pCompareCard->wLostUser]=FALSE;
	m_cbPlayerCount--;
	//״̬����
	if (pCompareCard->wLostUser==m_pIAndroidUserItem->GetChairID())	
	{
		m_pIAndroidUserItem->SetGameStatus(GAME_STATUS_FREE);
		RepositionSink();
	}
	//��ǰ�û�
	if(m_pIAndroidUserItem->GetChairID()==pCompareCard->wCurrentUser)UpdataControl(pCompareCard->wCurrentUser);
	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubGiveUp(VOID * pData, WORD wDataSize)
{
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pData;

	//���ñ���
	m_cbPlayStatus[pGiveUp->wGiveUpUser]=FALSE;
	m_cbPlayerCount--;
	//��������
	WORD wGiveUpUser=pGiveUp->wGiveUpUser;
	//״̬����
	if (pGiveUp->wGiveUpUser==m_pIAndroidUserItem->GetChairID()) 
	{
		m_pIAndroidUserItem->SetGameStatus(GAME_STATUS_FREE);
		RepositionSink();
	}
	return true;
}
//�û�����
bool CAndroidUserItemSink::OnSubLookCard(VOID * pData, WORD wDataSize)
{
	if (wDataSize!=sizeof(CMD_S_LookCard)) return false;
	CMD_S_LookCard * pLookCard=(CMD_S_LookCard *)pData;
	m_bMingZhu[pLookCard->wLookCardUser]=TRUE;
	if(m_pIAndroidUserItem->GetChairID()==pLookCard->wLookCardUser)
	UpdataControl(pLookCard->wLookCardUser);
	return true;
}
bool CAndroidUserItemSink::OnSubPlayerExit(VOID * pData, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_PlayerExit)) return false;
	CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pData;

	WORD wID=pPlayerExit->wPlayerID;

	//��Ϸ��Ϣ
	ASSERT(m_cbPlayStatus[wID]==TRUE);
	m_cbPlayStatus[wID]=FALSE;
	m_cbPlayerCount--;
	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(VOID * pData, WORD wDataSize)
{
	//����״̬
	m_pIAndroidUserItem->SetGameStatus(GAME_STATUS_FREE);
	
	//���ñ���
	RepositionSink();

	//��ʼ����
	UINT nElapse=rand()%TIME_START_GAME+TIME_LESS;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
//�û�����
VOID CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
VOID CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
VOID CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
VOID CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���λ
VOID CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}
