#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸ��ʱ��
#define IDI_OUT_CARD					200								//���ƶ�ʱ��
#define IDI_MOST_CARD					201								//���ʱ��
#define IDI_START_GAME					202								//��ʼ��ʱ��
#define IDI_LAND_SCORE					203								//�зֶ�ʱ��
#define IDI_DISPATCH_CARD				204								//���ƶ�ʱ��

#ifdef DEBUG
#define TIME_OUT_CARD					30
#else
#define TIME_OUT_CARD					30
#endif
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnPassCard)
	ON_MESSAGE(IDM_LAND_SCORE,OnLandScore)
	ON_MESSAGE(IDM_AUTO_OUTCARD,OnAutoOutCard)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
	ON_MESSAGE(IDM_LAND_AUTOPLAY,OnAutoPlay)
	ON_MESSAGE(IDM_SORT_HAND_CARD,OnMessageSortCard)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine() 
{
	//��Ϸ����
	m_wBombTime=1;
	m_bHandCardCount=0;
	m_wLandUser=INVALID_CHAIR;
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
	m_bAutoPlay = false ;
	m_cbSortType=ST_COUNT;
	ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));

	//���ñ���
	m_dwCardHSpace=DEFAULT_PELS;

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));

	//��������
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wTimeOutCount=0;

	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	///////////////////////////////////////////////////////////////////
	//BYTE cbHandCardData[] = {0x03, 0x03, 0x04,0x04,0x04,0x04,0x04,0x04, 0x06, 0x07};
	//BYTE cbOutCard[] = {0x05,0x05,0x05,0x05};
	////m_GameLogic.CompareCard(cbOutCard, cbHandCardData, sizeof( cbOutCard ), sizeof( cbHandCardData ));
	//tagOutCardResult OutCardResult;
	//m_GameLogic.SearchOutCard( cbHandCardData, sizeof( cbHandCardData ), cbOutCard, sizeof( cbOutCard ), OutCardResult );
	/////////////////////////////////////////////////////////////////////

	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{
	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

	//��������
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//���ÿؼ�
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardSpace(m_dwCardHSpace,0,20);

	return true;
}

//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
	//��Ϸ����
	m_wBombTime=1;
	m_bHandCardCount=0;
	m_wLandUser=INVALID_CHAIR;
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
	m_bAutoPlay = false ;
	m_cbSortType=ST_COUNT;
	ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));

	//��������
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wTimeOutCount=0;
    
	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	//ɾ����ʱ
	KillGameClock(0);
	KillTimer(IDI_MOST_CARD);
	KillTimer(IDI_DISPATCH_CARD);

	//��ʷ����
	m_HistoryScore.ResetData();

	return true;
}

//��Ϸ����
void CGameClientEngine::OnGameOptionSet()
{
	//��������
	//CGameOption GameOption;
	//GameOption.m_dwCardHSpace=m_dwCardHSpace;
	//GameOption.m_bEnableSound=IsEnableSound();
	//GameOption.m_bDeasilOrder=m_GameClientView.IsDeasilOrder();
	//GameOption.m_bAllowLookOn=IsAllowUserLookon();

	//��������
	//if (GameOption.DoModal()==IDOK)
	//{
	//	//��ȡ����
	//	m_bDeasilOrder=GameOption.m_bDeasilOrder;
	//	m_dwCardHSpace=GameOption.m_dwCardHSpace;

	//	//���ÿؼ�
	//	EnableSound(GameOption.m_bEnableSound);
	//	m_GameClientView.SetUserOrder(GameOption.m_bDeasilOrder);
	//	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardSpace(m_dwCardHSpace,0,20);
	//	AllowUserLookon(0,GameOption.m_bAllowLookOn);

	//	//��������
	//	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
	//	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);
	//	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("AllowLookon"),m_bDeasilOrder?TRUE:FALSE);
	//	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
	//}

	return;
}
////�û�����
//VOID __cdecl CGameClientEngine::OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
//{
//	//���óɼ�
//	if (bLookonUser==false)
//	{
//		//���ñ���
//		m_HistoryScore.OnEventUserEnter(wChairID);
//
//		//���½���
//		m_GameClientView.UpdateGameView(NULL);
//	}
//
//	return;
//}
//
////�û��뿪
//VOID __cdecl CGameClientEngine::OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
//{
//	//���óɼ�
//	if (bLookonUser==false)
//	{
//		//���ñ���
//		m_HistoryScore.OnEventUserLeave(wChairID);
//
//		//���½���
//		m_GameClientView.UpdateGameView(NULL);
//	}
//
//	return;
//}
////�û�״̬
//VOID __cdecl CGameClientEngine::OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
//{
//	return;
//}
////�û�״̬
//VOID __cdecl CGameClientEngine::OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
//{
//	////��������
//	//BYTE cbUserStatus=pUserData->GetUserStatus();
//
//	////�Զ���ʼ
//	//if ((pIClientUserItem!=pIMySelfUserItem)&&(cbUserStatus==US_READY)) PerformAutoStart();
//
//	////ֹͣ��ʼ
//	//if ((pIClientUserItem==pIMySelfUserItem)&&(cbUserStatus==US_READY)) KillGameClock(IDI_START_GAME);
//
//	return;
//}
//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	switch (wClockID)
	{
	case IDI_OUT_CARD:			//�û�����
		{
			//�Զ�����
			if ((((nElapse==0)||((m_bAutoPlay==true)&&(nElapse<=29))))&&(wChairID==GetMeChairID()))
			{
				//��ʱ�ж�
				if ((IsLookonMode()==false)&&(m_bAutoPlay==false)&&((++m_wTimeOutCount)>=3)) 
				{
					//״̬�л�
					m_wTimeOutCount=0;
					
					//�йܹ���
					m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
					m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_SHOW);
#ifndef DEBUG
					if(m_pIStringMessage!=NULL)
					{
						m_pIStringMessage->InsertSystemString( TEXT( "��������γ�ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ" ) );
					}
#endif
					m_bAutoPlay = true;

					CMD_C_UserTrustee UserTrustee;
					ZeroMemory( &UserTrustee, sizeof( UserTrustee ) );
					UserTrustee.bTrustee = true;
					UserTrustee.wUserChairID = GetMeChairID();

					SendSocketData( SUB_C_TRUSTEE, &UserTrustee, sizeof( UserTrustee ) );
				}

				//�Զ���ʼ
				if ( m_GameClientView.m_btStart.IsWindowVisible() ) {
					OnStart( 0, 0 );
				}
				else {
					//�Զ�����
					OnAutoOutCard( 0, 0 ) ;
					OnOutCard( 0, 0 ) ;
				}
				return true;
			}

			//��������
			if (m_bHandCardCount<m_bTurnCardCount) return true;
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{
					AfxGetMainWnd()->PostMessage(WM_CLOSE);
				}
				return false;
			}
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_LAND_SCORE:		//�ڿӽз�
		{
			if ( nElapse==0 || ( nElapse <= 27 && m_bAutoPlay ) )
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnLandScore(255,255);
				return false;
			}
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	}

	return false;
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//�Թ�״̬
bool  CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	//�Թ�����
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag( ( IsLookonMode() == true ) && ( IsAllowLookon() == true ) );
	m_GameClientView.UpdateGameView(NULL);
	return true;
}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_SEND_CARD:		//�����˿�
		{
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_LAND_SCORE:	//�û��з�
		{
			return OnSubLandScore(pData,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_PASS_CARD:		//��������
		{
			return OnSubPassCard(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_C_TRUSTEE:			//����й�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_UserTrustee));
			if (wDataSize != sizeof(CMD_C_UserTrustee)) return false;
			
			CMD_C_UserTrustee *pUserTrustee = ( CMD_C_UserTrustee * )pData;

			m_GameClientView.SetUserTrustee( SwitchViewChairID( pUserTrustee->wUserChairID ), pUserTrustee->bTrustee );
			
			IClientUserItem *pUserItem =GetTableUserItem(pUserTrustee->wUserChairID);
			//��ʾ�й�
#ifndef DEBUG
			if (( pUserTrustee->wUserChairID != GetMeChairID() || IsLookonMode()) && m_pIStringMessage!=NULL && pUserItem!=NULL)
			{
				
				CString strMsg;
				if ( pUserTrustee->bTrustee )
					strMsg.Format( TEXT( "[ %s ] ���ѡ�����й�" ), pUserItem->GetNickName());
				else
					strMsg.Format( TEXT( "[ %s ] ���ȡ�����й�" ), pUserItem->GetNickName());
				m_pIStringMessage->InsertSystemString( strMsg );

			}
#endif
			return true;
		}
	}

	return false;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_WK_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//���ý���
			m_GameClientView.SetBaseScore(pStatusFree->lBaseScore);
			//���ƽ���
			m_GameClientView.m_btScore.ShowWindow(SW_SHOW);
			//���ÿؼ�
			if (IsLookonMode()==false && GetMeUserItem()->GetUserStatus()!=US_READY)
			{
				m_GameClientView.m_btStart.ShowWindow(TRUE);
				m_GameClientView.m_btStart.SetFocus();
				//����ʱ��
				SetGameClock(GetMeChairID(),IDI_START_GAME,30);
			}
			if (IsLookonMode())
			{
				m_GameClientView.m_btAutoPlayOff.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOn.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOff.EnableWindow( FALSE ) ;
				m_GameClientView.m_btAutoPlayOn.EnableWindow( FALSE ) ;
				m_GameClientView.m_btSortCard.ShowWindow( SW_HIDE );
			}

			//�����˿�
			m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));


			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusFree->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusFree->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			return true;
		}
	case GS_WK_SCORE:	//�з�״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;
			CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pData;

			//���ñ���
			m_bHandCardCount=NORMAL_COUNT;
			for (WORD i=0;i<GAME_PLAYER;i++) m_bCardCount[i]=NORMAL_COUNT;
			CopyMemory(m_bHandCardData,pStatusScore->bCardData[GetMeChairID()],m_bHandCardCount);
			//���ƽ���
			m_GameClientView.m_btScore.ShowWindow(SW_SHOW);
			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)	
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetCardCount(wViewChairID,m_bCardCount[i]);
				m_GameClientView.SetLandScore(wViewChairID,pStatusScore->bScoreInfo[i]);
			}
			m_GameClientView.ShowLandTitle(true);
			m_GameClientView.SetBaseScore(pStatusScore->lBaseScore);

			//��ť����
			if ((IsLookonMode()==false)&&(pStatusScore->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btGiveUpScore.ShowWindow(SW_SHOW);

				m_GameClientView.m_btOneScore.ShowWindow( SW_SHOW );
				m_GameClientView.m_btTwoScore.ShowWindow( SW_SHOW );
				m_GameClientView.m_btThreeScore.ShowWindow( SW_SHOW );

				m_GameClientView.m_btOneScore.EnableWindow(pStatusScore->bLandScore==0?TRUE:FALSE);
				m_GameClientView.m_btTwoScore.EnableWindow(pStatusScore->bLandScore<=1?TRUE:FALSE);
				m_GameClientView.m_btThreeScore.EnableWindow(pStatusScore->bLandScore<=2?TRUE:FALSE);
			}

			if ( IsLookonMode() )
			{
				m_GameClientView.m_btAutoPlayOff.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOn.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOff.EnableWindow( FALSE ) ;
				m_GameClientView.m_btAutoPlayOn.EnableWindow( FALSE ) ;
				m_GameClientView.m_btSortCard.ShowWindow( SW_HIDE );
			}
			else 
			{
				m_GameClientView.m_btSortCard.EnableWindow(TRUE);
			}

			//�����˿�
			BYTE bCardData[BACK_COUNT]={0,0,0,0,0,0,0,0};
			m_GameClientView.m_BackCardControl.SetCardData(bCardData,BACK_COUNT);
			m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);
			m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));

			//�����˿�
			BYTE cbCardData[NORMAL_COUNT];
			ZeroMemory(cbCardData,sizeof(cbCardData));
			m_GameClientView.m_HandCardControl[0].SetCardData(cbCardData,NORMAL_COUNT);
			m_GameClientView.m_HandCardControl[1].SetCardData(cbCardData,NORMAL_COUNT);
			m_GameClientView.m_HandCardControl[3].SetCardData(cbCardData,NORMAL_COUNT);

			for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
			{
				m_GameClientView.SetUserTrustee( SwitchViewChairID( wChairID ), pStatusScore->bUserTrustee[ wChairID ] );
				
				//�Լ��ж�
				if ( ! IsLookonMode() && wChairID == GetMeChairID() && pStatusScore->bUserTrustee[ wChairID ] )
				{
					//�йܹ���
					m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
					m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_SHOW);
					OnAutoPlay( true, 0 );
				}
			}

			//����ʱ��
			SetGameClock(pStatusScore->wCurrentUser,IDI_LAND_SCORE,30);

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusScore->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusScore->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			return true;
		}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//���ñ���
			m_bTurnCardCount=pStatusPlay->bTurnCardCount;
			m_bHandCardCount=pStatusPlay->bCardCount[GetMeChairID()];
			m_bTurnOutType=m_GameLogic.GetCardType(pStatusPlay->bTurnCardData,m_bTurnCardCount);
			CopyMemory(m_bHandCardData,pStatusPlay->bCardData[GetMeChairID()],m_bHandCardCount);
			CopyMemory(m_bTurnCardData,pStatusPlay->bTurnCardData,pStatusPlay->bTurnCardCount);
			CopyMemory(m_cbMagicCardData,pStatusPlay->cbMagicCardData,m_bTurnCardCount*sizeof(BYTE));

			//���ƽ���
			m_GameClientView.m_btScore.ShowWindow(SW_SHOW);
			//���ý���
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_bCardCount[i]=pStatusPlay->bCardCount[i];
				m_GameClientView.SetCardCount(wViewChairID,pStatusPlay->bCardCount[i]);
			}
			m_GameClientView.SetBombTime(pStatusPlay->wBombTime);
			m_GameClientView.SetBaseScore(pStatusPlay->lBaseScore);
			m_GameClientView.m_BackCardControl.SetCardData(pStatusPlay->bBackCard,8);
			m_GameClientView.m_BackCardControl.SetDisplayFlag( true );
			m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);
			m_GameClientView.SetLandUser(SwitchViewChairID(pStatusPlay->wLandUser),pStatusPlay->bLandScore);

			//�������
			if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.EnableWindow((m_bTurnCardCount!=0)?TRUE:FALSE);
				m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE);
			}

			//��������
			if (m_bTurnCardCount!=0)
			{
				WORD wViewChairID=SwitchViewChairID(pStatusPlay->wLastOutUser);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_bTurnCardData,m_bTurnCardCount);
			}

			if ( IsLookonMode() )
			{
				m_GameClientView.m_btAutoPlayOff.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOn.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOff.EnableWindow( FALSE ) ;
				m_GameClientView.m_btAutoPlayOn.EnableWindow( FALSE ) ;
				m_GameClientView.m_btSortCard.ShowWindow( SW_HIDE );
			}
			else
			{
				m_GameClientView.m_btSortCard.EnableWindow(TRUE);
			}

			//�����˿�
			BYTE cbCardData[20];
			ZeroMemory(cbCardData,sizeof(cbCardData));
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				if (wViewChairID!=ME_VIEW_CHAIR) m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardData,pStatusPlay->bCardCount[i]);
			}

			//���ö�ʱ��
			SetGameClock(pStatusPlay->wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

			for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
			{
				m_GameClientView.SetUserTrustee( SwitchViewChairID( wChairID ), pStatusPlay->bUserTrustee[ wChairID ] );
				
				//�Լ��ж�
				if ( ! IsLookonMode() && wChairID == GetMeChairID() &&pStatusPlay->bUserTrustee[ wChairID ] )
				{
					//�йܹ���
					m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
					m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_SHOW);
					OnAutoPlay( true, 0 );
				}
			}

			//�����˿�
			m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetPositively(IsLookonMode()==false);
			m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusPlay->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusPlay->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			return true;
		}
	}

	return false;
}

//�����˿�
bool CGameClientEngine::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendAllCard));
	if (wDataSize!=sizeof(CMD_S_SendAllCard)) return false;

	//��������
	CMD_S_SendAllCard * pSendCard=(CMD_S_SendAllCard *)pBuffer;

	//�رճɼ�
	if (m_GameClientView.m_ScoreView.m_hWnd!=NULL)
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_ScoreView.DestroyWindow();
	}

	//��������
	m_wCurrentUser=pSendCard->wCurrentUser;
	m_bHandCardCount=NORMAL_COUNT;
	CopyMemory(m_bHandCardData,pSendCard->bCardData[GetMeChairID()],sizeof(pSendCard->bCardData[GetMeChairID()]));
	for (WORD i=0;i<GAME_PLAYER;i++) m_bCardCount[i]=CountArray(pSendCard->bCardData);
	for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		m_bCardCount[wChairID] = NORMAL_COUNT ;
	}

	//���ý���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetLandScore(i,0);
		m_GameClientView.SetPassFlag(i,false);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
	}

	//���ý���
	m_GameClientView.ShowLandTitle(true);
	m_GameClientView.SetBombTime(m_wBombTime);
	m_GameClientView.SetLandUser(INVALID_CHAIR,0);

	//�����˿�
	BYTE bBackCard[BACK_COUNT]={0,0,0};
	m_GameClientView.m_BackCardControl.SetCardData(bBackCard,CountArray(bBackCard));
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetDisplayFlag(((IsLookonMode()==false)||(IsAllowLookon()==true)));

	//�����˿�
	for ( BYTE i = 0; i < GAME_PLAYER; ++i )
		if (i!=ME_VIEW_CHAIR)
			m_GameClientView.m_HandCardControl[i].SetCardSpace(8,8,0);

	//�ŷ��˿�
	DispatchUserCard(m_bHandCardData,m_bHandCardCount);
	m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount,ST_ORDER);

	return true;
}

//�û��з�
bool CGameClientEngine::OnSubLandScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_LandScore));
	if (wDataSize!=sizeof(CMD_S_LandScore)) return false;

	//��Ϣ����
	CMD_S_LandScore * pLandScore=(CMD_S_LandScore *)pBuffer;

	//ֹͣ����
	DoncludeDispatchCard();

	//���ý���
	WORD wViewChairID=SwitchViewChairID(pLandScore->bLandUser);
	m_GameClientView.SetLandScore(wViewChairID,pLandScore->bLandScore);

	//�������
	if ((IsLookonMode()==false)&&(pLandScore->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btGiveUpScore.ShowWindow(SW_SHOW);
		
		m_GameClientView.m_btOneScore.ShowWindow( SW_SHOW );
		m_GameClientView.m_btTwoScore.ShowWindow( SW_SHOW );
		m_GameClientView.m_btThreeScore.ShowWindow( SW_SHOW );

		m_GameClientView.m_btOneScore.EnableWindow(pLandScore->bCurrentScore==0?TRUE:FALSE);
		m_GameClientView.m_btTwoScore.EnableWindow(pLandScore->bCurrentScore<=1?TRUE:FALSE);
		m_GameClientView.m_btThreeScore.EnableWindow(pLandScore->bCurrentScore<=2?TRUE:FALSE);
	}

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("CALL_CARD"));

	//����ʱ��
	SetGameClock(pLandScore->wCurrentUser,IDI_LAND_SCORE,30);

	return true;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//���ñ���
	m_wBombTime=1;
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_wLandUser=pGameStart->wLandUser;
	m_bCardCount[pGameStart->wLandUser]=MAX_COUNT;
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));
	ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));

	//�رճɼ�
	if (m_GameClientView.m_ScoreView.m_hWnd!=NULL)
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_ScoreView.DestroyWindow();
	}

	//���ÿؼ�
	m_GameClientView.ShowLandTitle(false);
	m_GameClientView.m_BackCardControl.SetCardData(pGameStart->bBackCard,CountArray(pGameStart->bBackCard));
	m_GameClientView.m_BackCardControl.SetDisplayFlag( true );

	//���ý���
	m_GameClientView.SetLandScore(INVALID_CHAIR,0);
	m_GameClientView.SetCardCount(SwitchViewChairID(pGameStart->wLandUser),m_bCardCount[pGameStart->wLandUser]);

	//��������
	if (pGameStart->wLandUser==GetMeChairID())
	{
		BYTE bCardCound=m_bHandCardCount;
		m_bHandCardCount+=CountArray(pGameStart->bBackCard);
		CopyMemory(&m_bHandCardData[bCardCound],pGameStart->bBackCard,sizeof(pGameStart->bBackCard));
		m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount,ST_ORDER);
		m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	//���ý���
	m_GameClientView.SetLandUser(SwitchViewChairID(pGameStart->wLandUser),pGameStart->bLandScore);

	//�������
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetPositively( IsLookonMode()==false || IsAllowLookon() );

	//��ǰ���
	if ((IsLookonMode()==false)&&(pGameStart->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE);
	}

	//�����˿�
	for ( BYTE i = 0; i < GAME_PLAYER; ++i )
		if (i!=ME_VIEW_CHAIR)
			m_GameClientView.m_HandCardControl[i].SetCardSpace(8,8,0);

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//����ʱ��
	SetGameClock(pGameStart->wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

	return true;
}

//�û�����
bool CGameClientEngine::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//��������
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;
	WORD wHeadSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->bCardData);

	//Ч������
	ASSERT(wDataSize>=wHeadSize);
	if (wDataSize<wHeadSize) return false;
	ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0])));
	if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0]))) return false;

	//ɾ����ʱ��
	KillTimer(IDI_MOST_CARD);
	KillGameClock(IDI_OUT_CARD);
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	//��������
	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser))
	{
		m_GameClientView.m_UserCardControl[wOutViewChairID].SetCardData(pOutCard->bCardData,pOutCard->bCardCount);
	}

	//��������
	if (m_bTurnCardCount==0)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			if (i==pOutCard->wOutCardUser) continue;
			m_GameClientView.m_UserCardControl[SwitchViewChairID(i)].SetCardData(NULL,0);
		}
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	}

	
	//����˿�
	m_GameLogic.MagicCardData(pOutCard->bCardData,pOutCard->bCardCount,m_cbMagicCardData,m_bTurnOutType);

	//��¼����
	m_bTurnCardCount=pOutCard->bCardCount;
	m_bTurnOutType=m_GameLogic.GetCardType(m_cbMagicCardData,pOutCard->bCardCount);
	CopyMemory(m_bTurnCardData,pOutCard->bCardData,sizeof(BYTE)*pOutCard->bCardCount);

	//ը���ж�
	if ((m_bTurnOutType==CT_BOMB_CARD)||(m_bTurnOutType==CT_MISSILE_CARD))
	{
		//ը���ж�
		if ((m_bTurnOutType==CT_BOMB_CARD) && ( m_bTurnCardCount == 6 || m_bTurnCardCount == 7 )  ) m_wBombTime*=2;
		if ((m_bTurnOutType==CT_BOMB_CARD) && m_bTurnCardCount == 8 || m_bTurnOutType == CT_MISSILE_CARD ) m_wBombTime*=3;

		m_GameClientView.SetBombTime(m_wBombTime);

		//���Ŷ���
		m_GameClientView.SetBombEffect(true);
	}

	//�Լ��˿�
	if ((IsLookonMode()==true)&&(pOutCard->wOutCardUser==GetMeChairID()))
	{
		//ɾ���˿� 
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=pOutCard->bCardCount;
		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData,bSourceCount);

		//���ý���
		m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	//��������	
	m_bCardCount[pOutCard->wOutCardUser]-=pOutCard->bCardCount;
	m_GameClientView.SetCardCount(wOutViewChairID,m_bCardCount[pOutCard->wOutCardUser]);
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		WORD wViewID = SwitchViewChairID(wChairID) ;
		if(ME_VIEW_CHAIR!=wViewID)
		{
			BYTE cbCardData[20] ;
			ZeroMemory(cbCardData, sizeof(cbCardData)) ;
			m_GameClientView.m_HandCardControl[wViewID].SetCardData(cbCardData,m_bCardCount[wChairID]);
		}
	}

	//����ж�
	if (pOutCard->wCurrentUser==pOutCard->wOutCardUser)
	{
		//���ñ���
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_ERROR;
		m_wMostUser=pOutCard->wCurrentUser;
		ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));
		ZeroMemory(m_cbMagicCardData,sizeof(m_cbMagicCardData));

		//���ý���
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (i!=pOutCard->wOutCardUser)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetPassFlag(wViewChairID,true);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			}
		}

		//��������
		PlayGameSound(AfxGetInstanceHandle(),TEXT("MOST_CARD"));

		//���ö�ʱ��
		SetTimer(IDI_MOST_CARD,3000,NULL);

		return true;
	}

	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser))
	{
		//ը���ж�
		if ((m_bTurnOutType==CT_BOMB_CARD)||(m_bTurnOutType==CT_MISSILE_CARD))
			PlayGameSound(AfxGetInstanceHandle(),TEXT("BOMB_CARD"));
		else
			PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	}

	//�������
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		WORD wViewChairID=SwitchViewChairID(pOutCard->wCurrentUser);
		m_GameClientView.SetPassFlag(wViewChairID,false);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//�������
	if ((IsLookonMode()==false)&&(pOutCard->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btPassCard.EnableWindow(TRUE);
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE);
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
	}

	//����ʱ��
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		BYTE bCardCount=m_bCardCount[pOutCard->wCurrentUser];
		SetGameClock(pOutCard->wCurrentUser,IDI_OUT_CARD,(bCardCount<m_bTurnCardCount)?TIME_OUT_CARD:TIME_OUT_CARD);
	}

	return true;
}

//��������
bool CGameClientEngine::OnSubPassCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_PassCard)) return false;
	CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pBuffer;

	//ɾ����ʱ��
	KillGameClock(IDI_OUT_CARD);

	//�������
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=GetMeChairID()))
	{
		WORD wViewChairID=SwitchViewChairID(pPassCard->wPassUser);
		m_GameClientView.SetPassFlag(wViewChairID,true);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//һ���ж�
	if (pPassCard->bNewTurn==TRUE)
	{
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_ERROR;
		ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));
	}

	//���ý���
	WORD wViewChairID=SwitchViewChairID(pPassCard->wCurrentUser);
	m_GameClientView.SetPassFlag(wViewChairID,false);
	m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);

	//�������
	if ((IsLookonMode()==false)&&(pPassCard->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.EnableWindow((m_bTurnCardCount>0)?TRUE:FALSE);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btAutoOutCard.EnableWindow( TRUE );
	}

	//��������
#ifndef DEBUG
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=GetMeChairID()))	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));
#endif
	//����ʱ��
	if (m_bTurnCardCount!=0)
	{
		BYTE bCardCount=m_bCardCount[pPassCard->wCurrentUser];
		SetGameClock(pPassCard->wCurrentUser,IDI_OUT_CARD,(bCardCount<m_bTurnCardCount)?TIME_OUT_CARD:TIME_OUT_CARD);
	}
	else SetGameClock(pPassCard->wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//ɾ����ʱ��
	KillTimer(IDI_MOST_CARD);
	KillGameClock(IDI_OUT_CARD);
	KillGameClock(IDI_LAND_SCORE);
	KillTimer(IDI_DISPATCH_CARD);

	//ֹͣ����
	DoncludeDispatchCard();

	//���ؿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOneScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btTwoScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btThreeScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btSortCard.EnableWindow(FALSE);

	//���ÿؼ�
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);

	//�����ɼ�
	if (m_GameClientView.m_ScoreView.m_hWnd==NULL)
	{
		m_GameClientView.m_ScoreView.Create(IDD_GAME_SCORE,&m_GameClientView);
	}

	//���û���
	CString strScore;
	CString strEndInfo;
	strEndInfo+=TEXT(" ���ֽ������ɼ�ͳ�ƣ�\n");

	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IClientUserItem *pUserItem = GetTableUserItem(i);
		m_GameClientView.m_ScoreView.SetGameScore(i,pUserItem!=NULL?pUserItem->GetNickName():TEXT("�뿪�û�"),pGameEnd->lGameScore[i]);
		strScore.Format( TEXT(" %s ��%I64d \n"), pUserItem!=NULL?pUserItem->GetNickName():TEXT("�뿪�û�"),pGameEnd->lGameScore[i]);
		strEndInfo+=strScore;
	}
#ifndef DEBUG

	if(m_pIStringMessage!=NULL)
		m_pIStringMessage->InsertSystemString(strEndInfo);
	//��Ϸ����

	const tagServerAttribute *pServerAttribute  = m_pIClientKernel->GetServerAttribute();

	if ( pServerAttribute->wServerType == GAME_GENRE_GOLD && m_pIStringMessage!=NULL ) {
		strScore.Format( TEXT( "��˰��%I64d" ), pGameEnd->lGameTax);
		m_pIStringMessage->InsertSystemString(strScore);
	}
#endif
	m_GameClientView.m_ScoreView.SetGameTax(pGameEnd->lGameTax);

	//�����ɼ�
	CRect scoreRect, gameViewRect ;
	m_GameClientView.GetWindowRect( gameViewRect ) ;
	m_GameClientView.m_ScoreView.GetWindowRect( scoreRect ) ;
	m_GameClientView.m_ScoreView.MoveWindow( gameViewRect.left + ( gameViewRect.Width() - scoreRect.Width() ) / 2, 
		gameViewRect.top + ( gameViewRect.Height() - scoreRect.Height() ) / 2, scoreRect.Width(), scoreRect.Height()) ;
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//��ʾ�˿�
	BYTE cbCardPos=0 ;
	for(BYTE i=0; i<GAME_PLAYER; ++i)
	{
		WORD wViewID = SwitchViewChairID(i) ;
		m_GameClientView.m_HandCardControl[wViewID].SetCardData(pGameEnd->bCardData+cbCardPos, pGameEnd->bCardCount[i]) ;
		m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(true) ;
		cbCardPos += pGameEnd->bCardCount[i] ;

		if (i!=ME_VIEW_CHAIR)
			m_GameClientView.m_HandCardControl[i].SetCardSpace(16,16,0);
	}

	//��������
	WORD wMeChairID=GetMeChairID();
	LONGLONG lMeScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	//else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameClock(GetMeChairID(),IDI_START_GAME,30);

		//�йܹ���
		 m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
		OnAutoPlay( false, 0 );
	}
	m_GameClientView.ShowLandTitle(false);

	//�û��ɼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		if (GetTableUserItem(i)==NULL) continue;

		//���óɼ�
		m_HistoryScore.OnEventUserScore(i,pGameEnd->lGameScore[i]);
	}

	return true;
}

//�����ж�
bool CGameClientEngine::VerdictOutCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;

	//��ȡ�˿�
	BYTE bCardData[MAX_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].GetShootCard(bCardData,CountArray(bCardData));

	//����˿�
	BYTE cbMagicCardData[MAX_COUNT];
	m_GameLogic.MagicCardData(bCardData,bShootCount,cbMagicCardData,m_bTurnOutType);

	//�����ж�
	if (bShootCount>0L)
	{
		//�����˿�
		m_GameLogic.SortCardList(cbMagicCardData, bShootCount, ST_ORDER);

		//��������
		BYTE bCardType=m_GameLogic.GetCardType(cbMagicCardData,bShootCount);

		//�����ж�
		if (bCardType==CT_ERROR) return false;

		//�����ж�
		if (m_bTurnCardCount==0) return true;
		return m_GameLogic.CompareCard(m_cbMagicCardData,cbMagicCardData,m_bTurnCardCount,bShootCount);
	}

	return false;
}

//�Զ�����
bool CGameClientEngine::AutomatismOutCard()
{
	//�ȳ�����
	if (m_bTurnCardCount==0)
	{
		//���ƽ���
		KillGameClock(IDI_OUT_CARD);
		m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btPassCard.EnableWindow(FALSE);
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE);

		//��������
		CMD_C_OutCard OutCard;
		OutCard.bCardCount=1;
		OutCard.bCardData[0]=m_bHandCardData[m_bHandCardCount-1];
		SendSocketData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

		//Ԥ�ȴ���
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
		m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetCardData(OutCard.bCardData,OutCard.bCardCount);

		//Ԥ��ɾ��
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=OutCard.bCardCount;
		m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
		m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);
	}
	else OnPassCard(0,0);

	return true;
}

//ֹͣ����
bool CGameClientEngine::DoncludeDispatchCard()
{
	//ֹͣ����
	if (m_cbRemnantCardCount>0)
	{
		//ɾ��ʱ��
		KillTimer(IDI_DISPATCH_CARD);

		//���ý���
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			WORD wViewChairID = SwitchViewChairID(i) ;
			if (wViewChairID!=ME_VIEW_CHAIR)
			{
				BYTE cbCardData[20] ;
				ZeroMemory(cbCardData, sizeof(cbCardData)) ;
				m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardData,NORMAL_COUNT);
			}
			else m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount); 
		}

		//���ñ���
		m_cbRemnantCardCount=0;
		m_cbDispatchCardCount=0;
		ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));
	}

	return true;
}

//�ɷ��˿�
bool CGameClientEngine::DispatchUserCard(BYTE cbCardData[], BYTE cbCardCount)
{
	//���ñ���
	m_cbDispatchCardCount=0;
	m_cbRemnantCardCount=cbCardCount;
	CopyMemory(m_cbDispatchCardData,cbCardData,sizeof(BYTE)*cbCardCount);

	//���ö�ʱ��
	SetTimer(IDI_DISPATCH_CARD,120,NULL);

	return true;
}

//��ʱ����Ϣ
void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	if ((nIDEvent==IDI_MOST_CARD)&&(m_wMostUser!=INVALID_CHAIR))
	{
		//��������
		WORD wCurrentUser=m_wMostUser;
		m_wMostUser=INVALID_CHAIR;

		//ɾ����ʱ��
		KillTimer(IDI_MOST_CARD);

		//���ý���
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
		for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);

		//�������
		if ((IsLookonMode()==false)&&(wCurrentUser==GetMeChairID()))
		{
			ActiveGameFrame();
			m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btPassCard.EnableWindow(FALSE);
			m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE);
			m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		}

		//����ʱ��
		SetGameClock(wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

		return;
	}

	//�ɷ��˿�
	if (nIDEvent==IDI_DISPATCH_CARD)
	{
		//�ɷ�����
		if (m_cbRemnantCardCount>0)
		{
			//�˿�����
			BYTE cbUserCard[NORMAL_COUNT];
			ZeroMemory(cbUserCard,sizeof(cbUserCard));

			//�ɷ��˿�
			m_cbRemnantCardCount--;
			m_cbDispatchCardCount++;
			m_GameLogic.SortCardList(m_cbDispatchCardData,m_cbDispatchCardCount,ST_ORDER);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewID = SwitchViewChairID(i) ;
				if (wViewID!=ME_VIEW_CHAIR)
				{
					m_GameClientView.m_HandCardControl[wViewID].SetCardData(cbUserCard,m_cbDispatchCardCount);
				}
				else m_GameClientView.m_HandCardControl[wViewID].SetCardData(m_cbDispatchCardData,m_cbDispatchCardCount); 
			}

			//��������
			PlayGameSound(AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"));
		}

		//��ֹ�ж�
		if (m_cbRemnantCardCount==0)
		{
			//ɾ����ʱ��
			KillTimer(IDI_DISPATCH_CARD);

			//�˿���Ŀ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetCardCount(SwitchViewChairID(i),m_bCardCount[i]);
			}

			//��ǰ���
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				ActiveGameFrame();
				m_GameClientView.m_btOneScore.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOneScore.EnableWindow(TRUE);
				m_GameClientView.m_btTwoScore.ShowWindow(SW_SHOW);
				m_GameClientView.m_btTwoScore.EnableWindow(TRUE);
				m_GameClientView.m_btThreeScore.ShowWindow(SW_SHOW);
				m_GameClientView.m_btThreeScore.EnableWindow(SW_SHOW);
				m_GameClientView.m_btGiveUpScore.ShowWindow(SW_SHOW);
				m_GameClientView.m_btGiveUpScore.EnableWindow(TRUE);
			}
			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewID = SwitchViewChairID(i) ;
				if (wViewID!=ME_VIEW_CHAIR)
				{
					BYTE cbCardData[20] ;
					ZeroMemory(cbCardData, sizeof(cbCardData)) ;
					m_GameClientView.m_HandCardControl[wViewID].SetCardData(cbCardData,NORMAL_COUNT);
					m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(false) ;
				}
				else 
				{
					m_GameClientView.m_HandCardControl[wViewID].SetCardData(m_bHandCardData,m_bHandCardCount); 
					if(false==IsLookonMode())
					{
						m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(true) ;
						m_GameClientView.m_HandCardControl[wViewID].SetPositively(true) ;
					}
				}
			}

			if(IsLookonMode()==false) m_GameClientView.m_btSortCard.EnableWindow(TRUE);

			//��������
			PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

			//����ʱ��
			if (m_wCurrentUser!=INVALID_CHAIR) SetGameClock(m_wCurrentUser,IDI_LAND_SCORE,30);
		}

		return;
	}

	__super::OnTimer(nIDEvent);
}

//��ʼ��ť
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_wBombTime=1;
	m_wTimeOutCount=0;
	m_wTimeOutCount=0;
	m_bHandCardCount=0;
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_wMostUser=INVALID_CHAIR;
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));

	//���ý���
	KillGameClock(IDI_START_GAME);
	m_GameClientView.ShowLandTitle(false);
	m_GameClientView.SetBombTime(m_wBombTime);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	m_GameClientView.SetLandUser(INVALID_CHAIR,0);
	m_GameClientView.SetLandScore(INVALID_CHAIR,0);
	m_GameClientView.SetPassFlag(INVALID_CHAIR,false);

	//���ؿؼ�
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	if (m_GameClientView.m_ScoreView.m_hWnd!=NULL) m_GameClientView.m_ScoreView.DestroyWindow();

	//�����˿�
	m_GameClientView.m_BackCardControl.SetCardData(NULL,0);
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetPositively(false);
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
		WORD wViewID = SwitchViewChairID(i) ;
		if(wViewID==ME_VIEW_CHAIR)	m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(true);
		else m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(false);
	}

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}

//������Ϣ
LRESULT CGameClientEngine::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if ((m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE)||
		(m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE)) return 0;

	//���ñ���
	m_wTimeOutCount=0;

	//���ý���
	KillGameClock(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.bCardCount=(BYTE)m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].GetShootCard(OutCard.bCardData,CountArray(OutCard.bCardData));
	m_GameLogic.SortCardList( OutCard.bCardData, OutCard.bCardCount, ST_ORDER);
	SendSocketData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

	//����˿�
	BYTE cbMagicCardData[MAX_COUNT];
	m_GameLogic.MagicCardData(OutCard.bCardData,OutCard.bCardCount,cbMagicCardData,m_bTurnOutType);

	//Ԥ����ʾ
	BYTE cbOutCardType = m_GameLogic.GetCardType(cbMagicCardData,OutCard.bCardCount);
	if ( cbOutCardType == CT_BOMB_CARD || cbOutCardType == CT_MISSILE_CARD ) PlayGameSound(AfxGetInstanceHandle(),TEXT("BOMB_CARD"));
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[ME_VIEW_CHAIR].SetCardData(OutCard.bCardData,OutCard.bCardCount);

	//Ԥ��ɾ��
	BYTE bSourceCount=m_bHandCardCount;
	m_bHandCardCount-=OutCard.bCardCount;
	m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}

//��������
LRESULT CGameClientEngine::OnPassCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_GameClientView.m_btPassCard.IsWindowEnabled()==FALSE) return 0;

	//���ñ���
	m_wTimeOutCount=0;

	//���ý���
	KillGameClock(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE);

	//��������
	SendSocketData(SUB_C_PASS_CARD);

	//Ԥ����ʾ
	m_GameClientView.SetPassFlag(ME_VIEW_CHAIR,true);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));
	m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}

//�з���Ϣ
LRESULT CGameClientEngine::OnLandScore(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	KillGameClock(IDI_LAND_SCORE);
	m_GameClientView.m_btOneScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btTwoScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btThreeScore.ShowWindow(SW_HIDE);

	//��������
	CMD_C_LandScore LandScore;
	LandScore.bLandScore=(BYTE)wParam;
	SendSocketData(SUB_C_LAND_SCORE,&LandScore,sizeof(LandScore));

	return 0;
}

//������ʾ
LRESULT CGameClientEngine::OnAutoOutCard(WPARAM wParam, LPARAM lParam)
{
	//�������
	tagOutCardResult OutCardResult;
	m_GameLogic.SearchOutCard(m_bHandCardData,m_bHandCardCount,m_bTurnCardData,m_bTurnCardCount,OutCardResult);

	//���ý���
	if (OutCardResult.cbCardCount>0)
	{
		//���ý���
		m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);

		//���ÿؼ�
		bool bOutCard=VerdictOutCard();
		m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);
	}
	else 
	{
		//��������
		OnPassCard(0,0);
	}

	return 0;
}

//�Ҽ��˿�
LRESULT CGameClientEngine::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	//���ÿؼ�
	bool bOutCard=VerdictOutCard();
	m_GameClientView.m_btOutCard.EnableWindow(bOutCard?TRUE:FALSE);

	return 0;
}

//����˿�
LRESULT CGameClientEngine::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{
	//�û�����
	OnOutCard(0,0);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CGameClientEngine::OnAutoPlay(WPARAM wParam, LPARAM lParam)
{
    bool bAutoPlay = (wParam == 1);
    if (bAutoPlay == m_bAutoPlay)
        return 0;

    m_bAutoPlay = bAutoPlay;
#ifndef DEBUG
	if(m_pIStringMessage!=NULL)
	{
		if (m_bAutoPlay)
			m_pIStringMessage->InsertSystemString(TEXT("ʹ���йܹ���, ��������Զ�����״̬!"));
		else
			m_pIStringMessage->InsertSystemString(TEXT("ȡ���йܹ���. "));
	}
#endif
	//������Ϣ
	CMD_C_UserTrustee UserTrustee;
	ZeroMemory( &UserTrustee, sizeof( UserTrustee ) );
	UserTrustee.bTrustee = m_bAutoPlay;
	UserTrustee.wUserChairID = GetMeChairID();

	SendSocketData( SUB_C_TRUSTEE, &UserTrustee, sizeof( UserTrustee ) );

    return 0;
}


LRESULT CGameClientEngine::OnMessageSortCard(WPARAM wParam, LPARAM lParam)
{

	//�����˿�
	WORD wMeChairID=GetMeChairID();
	m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount,m_cbSortType);

	//��ȡ�˿�
	BYTE cbShootCount=(BYTE)m_GameClientView.m_HandCardControl[ME_VIEW_CHAIR].SetCardData(m_bHandCardData,m_bHandCardCount);

	//���ñ���
	m_cbSortType=(m_cbSortType==ST_ORDER)?ST_COUNT:ST_ORDER;

	//���ð�ť
	UINT uImageID=(m_cbSortType==ST_ORDER)?IDB_ORDER_SORT:IDB_COUNT_SORT;
	m_GameClientView.m_btSortCard.SetButtonImage(uImageID,AfxGetInstanceHandle(),false,false);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return 0;
}
