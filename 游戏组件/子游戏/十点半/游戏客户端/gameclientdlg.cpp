#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "DlgInfomation.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸʱ��
#define IDI_OPERATE_SCORE			201									//��ע��ʱ��
#define IDI_START_GAME				202									//��ʼ��ʱ��
#define IDI_USER_SCORE				203									//��ע��ʱ��

//��Ϸʱ��
#define TIME_OPERATE_SCORE			30									//��ע��ʱ��
#define TIME_START_GAME				30									//��ʼ��ʱ��
#define TIME_USER_SCORE				30									//��ע��ʱ��

#define IDI_PERFORM_START			203									//ִ�п�ʼ
#define IDI_PERFORM_END				204									//ִ�н���

#define TIME_PERFORM_START			1000								//ִ�п�ʼʱ��
#define TIME_PERFORM_END			1000								//ִ�н���ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_ADD_SCORE,OnAddScore)
	ON_MESSAGE(IDM_GIVE_CARD,OnGiveCard)
	ON_MESSAGE(IDM_STOP_CARD,OnStopCard)
	ON_MESSAGE(IDM_USER_SCORE,OnUserScore)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
	ON_MESSAGE(IDM_MOVE_JET_FINISH,OnMoveJetFinish)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
	//��Ϸ����
	m_lCellScore = 0L;
	ZeroMemory( m_lTableScore,sizeof(m_lTableScore) );
	m_bAddScore = false;
	m_bGameStart = false;

	//��ұ���
	m_wCurrentUser = INVALID_CHAIR;
	m_wBankerUser = INVALID_CHAIR;
	ZeroMemory( m_byUserStatus,sizeof(m_byUserStatus) );

	ZeroMemory( m_szAccounts,sizeof(m_szAccounts));

	m_bySendCardCount = 0;
	m_wStartChairId = INVALID_CHAIR;
	ZeroMemory( m_bySendCardData,sizeof(m_bySendCardData) );

	ZeroMemory( &m_GameStart,sizeof(m_GameStart) );
	ZeroMemory( &m_GameEnd,sizeof(m_GameEnd) );

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
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	return true;
}

//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
	//��Ϸ����
	m_lCellScore = 0L;
	ZeroMemory( m_lTableScore,sizeof(m_lTableScore) );
	m_bAddScore = false;
	m_bGameStart = false;

	//��ұ���
	m_wCurrentUser = INVALID_CHAIR;
	m_wBankerUser = INVALID_CHAIR;
	ZeroMemory( m_byUserStatus,sizeof(m_byUserStatus) );

	ZeroMemory( m_szAccounts,sizeof(m_szAccounts));

	m_bySendCardCount = 0;
	m_wStartChairId = INVALID_CHAIR;
	ZeroMemory( m_bySendCardData,sizeof(m_bySendCardData) );

	ZeroMemory( &m_GameStart,sizeof(m_GameStart) );
	ZeroMemory( &m_GameEnd,sizeof(m_GameEnd) );

	return true;
}

//��Ϸ����
void CGameClientEngine::OnGameOptionSet()
{
	//��������
	//CGameOption GameOption;
	//GameOption.m_bEnableSound=IsEnableSound();
	//GameOption.m_bAllowLookon = IsAllowUserLookon();

	////��������
	//if (GameOption.DoModal()==IDOK)
	//{
	//	//���ÿؼ�
	//	EnableSound(GameOption.m_bEnableSound);
	//	AllowUserLookon(0,GameOption.m_bAllowLookon);
	//}

	return;
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	switch (wClockID)
	{
	case IDI_START_GAME:	//��Ϸ��ʼ
		{
			//�ر���Ϸ
			if ((nElapse==0)&&(IsLookonMode()==false)&&(wChairID==GetMeChairID()))
			{
				AfxGetMainWnd()->PostMessage(WM_CLOSE);
				return true;
			}

			//��ʱ����
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_OPERATE_SCORE:		//�û���ע
		{
			//�Զ�����
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnStopCard(0,0);
				return true;
			}

			//��ʱ����
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_USER_SCORE:		//�����ע
		{
			//�Զ�����
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnUserScore(1,0);
				return true;
			}

			//��ʱ����
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	}
	return true;
}


//�Թ�״̬
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			PerformGameConclude();
			//��������
			m_GameClientView.StopMoveJettons();
			m_GameClientView.ConcludeDispatch();
			m_GameClientView.StopMoveNumber();
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:
		{
			//��������
			m_GameClientView.StopMoveJettons();
			m_GameClientView.ConcludeDispatch();
			return OnSubSendCard( pData,wDataSize );
		}
	case SUB_S_GAME_END:	//��Ϸ����
		{
			//��������
			m_GameClientView.StopMoveJettons();
			m_GameClientView.ConcludeDispatch();
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_GIVE_UP:
		{
			//��������
			m_GameClientView.StopMoveJettons();
			m_GameClientView.ConcludeDispatch();
			return OnSubUserStopCard( pData,wDataSize );
		}
	case SUB_S_ADD_SCORE:
		{
			//��������
			m_GameClientView.ConcludeDispatch();
			m_GameClientView.StopMoveJettons();
			return OnSubUserAddScore( pData,wDataSize );
		}
	case SUB_S_USER_LEFT:
		{
			return OnSubUserLeft( pData,wDataSize );
		}
	case SUB_S_GAME_PLAY:
		{
			PerformGameStart();
			return OnSubGamePlay(pData,wDataSize);
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_TH_FREE:	//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��������
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//��Ԫ����
			m_lCellScore = pStatusFree->lCellScore;

			//��������
			m_GameClientView.SetCellScore( m_lCellScore );

			//�������
			if (IsLookonMode()==false && GetMeUserItem()->GetUserStatus()!=US_READY)
			{
				m_GameClientView.m_btStart.ShowWindow(TRUE);
				m_GameClientView.m_btStart.SetFocus();

				//����ʱ��
				SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
			}

			return true;
		}
	case GS_TH_SCORE:	//��Ϸ״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusScore));
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;

			//��������
			CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pData;

			//���ñ���
			m_wBankerUser = pStatusScore->wBankerUser;
			CopyMemory(m_byUserStatus,pStatusScore->bUserStatus,sizeof(m_byUserStatus));

			//��ұ���
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				//�û�����
				IClientUserItem * pClientUserItem=GetTableUserItem(i);
				if( pClientUserItem == NULL ) continue;
				lstrcpy( m_szAccounts[i],pClientUserItem->GetNickName() );
				
				//m_byUserStatus[i] = TRUE;
				m_bGameStart = true;

				m_GameClientView.SetScoreInfo( SwitchViewChairID(i),pClientUserItem->GetUserScore() );
			}

			//��Ԫ����
			m_lCellScore = pStatusScore->lCellScore;

			//���õ�Ԫ����
			m_GameClientView.SetCellScore( m_lCellScore );

			m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

			//��ǰ�û�
			if( !IsLookonMode() && !pStatusScore->cbHadScore && GetMeChairID() != m_wBankerUser )
			{
				//������
				ActiveGameFrame();

				//���¿ؼ�
				UpdateScoreControl();

				//���ö�ʱ��
				SetGameClock( GetMeChairID(),IDI_USER_SCORE,TIME_USER_SCORE );
			}

			m_GameClientView.RefreshGameView( NULL );

			return true;
		}
	case GS_TH_PLAY:	//��Ϸ״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��������
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//��ұ���
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				//�û�����
				IClientUserItem * pClientUserItem=GetTableUserItem(i);
				if( pClientUserItem == NULL ) continue;
				lstrcpy( m_szAccounts[i],pClientUserItem->GetNickName() );
				m_bGameStart = true;
				
				m_GameClientView.SetScoreInfo( SwitchViewChairID(i),pClientUserItem->GetUserScore() );
			}

			//��Ԫ����
			m_lCellScore = pStatusPlay->lCellScore;
			m_wBankerUser = pStatusPlay->wBankerUser;

			//״̬����
			m_wCurrentUser = pStatusPlay->wCurrentUser;
			CopyMemory( m_byUserStatus,pStatusPlay->byUserStatus,sizeof(m_byUserStatus) );

			//��ע����
			CopyMemory( m_lTableScore,pStatusPlay->lTableScore,sizeof(m_lTableScore) );

			//���õ�Ԫ����
			m_GameClientView.SetCellScore( m_lCellScore );

			m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

			//������Թ��û�
			WORD wMeChairId = GetMeChairID();
			if( !IsLookonMode() )
			{
				m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetPositively( true );
			}

			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				WORD wViewId = SwitchViewChairID( i );

				if( pStatusPlay->cbCardCount[i] == 0 || !pStatusPlay->byUserStatus[i] ) continue;

				//�����˿�
				m_GameClientView.m_CardControl[wViewId].SetCardData( pStatusPlay->cbHandCardData[i],
					pStatusPlay->cbCardCount[i] );

				//���������
				if( wViewId == MYSELF_VIEW_ID && !IsLookonMode() )
				{
					FLOAT fCardScore = m_GameLogic.GetCardGenre( pStatusPlay->cbHandCardData[i],
						pStatusPlay->cbCardCount[i] );
					m_GameClientView.SetCardScore( wViewId,fCardScore );
				}
				else
				{
					FLOAT fCardScore = m_GameLogic.GetCardGenre( &pStatusPlay->cbHandCardData[i][1],
						pStatusPlay->cbCardCount[i]-1 );
					m_GameClientView.SetCardScore( wViewId,fCardScore );
				}

				//�����������
				m_GameClientView.OnUserAddJettons( wViewId,m_lTableScore[i] );
			}
			
			//ֹͣ���붯��
			m_GameClientView.StopMoveJettons();

			//��ǰ�û�
			if( !IsLookonMode() && wMeChairId == m_wCurrentUser )
			{
				//������
				ActiveGameFrame();

				//���¿ؼ�
				UpdateScoreControl();
			}

			//���ö�ʱ��
			if( m_wCurrentUser != INVALID_CHAIR )
				SetGameClock( m_wCurrentUser,IDI_OPERATE_SCORE,TIME_OPERATE_SCORE );

			m_GameClientView.RefreshGameView( NULL );

			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч��
	ASSERT( wDataSize == sizeof(CMD_S_GameStart) );
	if( wDataSize != sizeof(CMD_S_GameStart) ) return false;

	CMD_S_GameStart *pGameStart = (CMD_S_GameStart *)pBuffer;

	//����״̬
	SetGameStatus(GS_TH_SCORE);

	if( IsLookonMode() ) KillGameClock( IDI_START_GAME );

	//��������
	CopyMemory( &m_GameStart,pGameStart,sizeof(m_GameStart) );

	//��������
	PlayGameSound( AfxGetInstanceHandle(),TEXT("GAME_START") );

	SetTimer( IDI_PERFORM_START,TIME_PERFORM_START,NULL );

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd( const void *pData, WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��������
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pData;

	//����״̬
	SetGameStatus(GS_TH_FREE);
	KillGameClock( IDI_OPERATE_SCORE );
	KillGameClock( IDI_USER_SCORE );

	//���ؿؼ�
	HideScoreControl();

	//��������
	CopyMemory( &m_GameEnd,pGameEnd,sizeof(m_GameEnd) );

	//�û��˿�
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if( m_GameEnd.byCardData[i] != 0 )
		{
			//��ȡ�˿�
			BYTE byCardData[MAX_COUNT];
			WORD wViewChairID=SwitchViewChairID(i);
			BYTE wCardCount=(BYTE)m_GameClientView.m_CardControl[wViewChairID].GetCardData(byCardData,CountArray(byCardData));

			//�����˿�
			byCardData[0] = m_GameEnd.byCardData[i];
			FLOAT fCardScore;
			if( wCardCount > 1 )
				fCardScore = m_GameLogic.GetCardGenre(&byCardData[1],wCardCount-1);
			else fCardScore = m_GameLogic.GetCardGenre(byCardData,wCardCount);
			if( fCardScore != CT_ERROR )
			{
				m_GameClientView.m_CardControl[wViewChairID].SetDisplayHead(true);
				m_GameClientView.m_CardControl[wViewChairID].SetCardData(byCardData,wCardCount);
				fCardScore = m_GameLogic.GetCardGenre( byCardData,(BYTE)wCardCount );
				m_GameClientView.SetCardScore( wViewChairID,fCardScore );
			}
		}
	}

	//���ÿؼ�
	m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetPositively( false );

	m_GameClientView.SetWaitUserScore(false);

	//��������
	LONGLONG lMeScore=m_GameEnd.lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOSE"));
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));

	//���ö�ʱ
	SetTimer( IDI_PERFORM_END,TIME_PERFORM_END,NULL );

	return true;
}

//����
bool CGameClientEngine::OnSubSendCard( const void *pData, WORD wDataSize )
{
	//Ч��
	ASSERT( wDataSize == sizeof(CMD_S_SendCard) );
	if( wDataSize != sizeof(CMD_S_SendCard) ) return false;

	//�������
	CMD_S_SendCard *pSendCard = (CMD_S_SendCard *)pData;

	//���õ�ǰ���
	m_wCurrentUser = pSendCard->wCurrentUser;

	//����
	if( !m_GameClientView.m_JettonControl.IsPlayMoving() )
	{
		m_GameClientView.DispatchUserCard( SwitchViewChairID(pSendCard->wSendCardUser),
			pSendCard->cbCardData );
	}
	else
	{
		m_bySendCardCount = 1;
		if( m_wStartChairId == INVALID_CHAIR )
			m_wStartChairId = pSendCard->wSendCardUser;
		m_bySendCardData[pSendCard->wSendCardUser][0] = pSendCard->cbCardData;
	}

	return true;
}

//��ʼ��Ϣ
LRESULT CGameClientEngine::OnStart( WPARAM wParam, LPARAM lParam )
{
	KillGameClock( IDI_START_GAME );

	//��Ϸ����
	m_lCellScore = 0L;
	ZeroMemory( m_lTableScore,sizeof(m_lTableScore) );
	m_bAddScore = false;

	//���õ�ǰ���
	m_wCurrentUser = INVALID_CHAIR;
	m_wBankerUser = INVALID_CHAIR;
	ZeroMemory( m_byUserStatus,sizeof(m_byUserStatus) );
	ZeroMemory(m_szAccounts,sizeof(m_szAccounts));

	//���ذ�ť
	m_GameClientView.m_btStart.ShowWindow( SW_HIDE );

	//ֹͣ����
	m_GameClientView.StopMoveJettons();
	m_GameClientView.StopMoveNumber();

	m_GameClientView.m_JettonControl.ResetControl();
	//���ÿؼ�
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		//���ֿؼ�
		m_GameClientView.m_NumberControl[i].ResetControl();
		//�˿˿ؼ�
		m_GameClientView.m_CardControl[i].SetCardData( NULL,0 );	
		m_GameClientView.m_CardControl[i].SetDisplayHead( false );
	}

	//���������
	m_GameClientView.SetCardScore( INVALID_CHAIR,0 );

	//�����û�����
	m_GameClientView.SetUserAction( INVALID_CHAIR,0 );

	//���½���
	m_GameClientView.RefreshGameView( NULL );

	//����׼��
	SendUserReady( NULL,0 );

	return 0;
}

//�ӱ���Ϣ
LRESULT CGameClientEngine::OnAddScore( WPARAM wParam, LPARAM lParam )
{
	//Ч��
	ASSERT( !m_bAddScore );
	if( m_bAddScore ) return 0;

	WORD wMeChairId = GetMeChairID();

	//ɾ����ʱ��
	KillGameClock( IDI_OPERATE_SCORE );

	//������ע�ؼ�
	HideScoreControl();

	WORD wViewId = SwitchViewChairID( wMeChairId );

	//������Ҷ���
	m_GameClientView.SetUserAction( wViewId,AC_ADD_SCORE );

	//��������	
	PlayActionSound( wMeChairId,AC_ADD_SCORE );

	//��ע����
	m_GameClientView.OnUserAddJettons( wViewId,m_lTableScore[wMeChairId] );
	m_GameClientView.BeginMoveJettons();

	//���ñ���
	m_bAddScore = true;
	m_lTableScore[wMeChairId] *= 2;

	//��������
	SendSocketData( SUB_C_ADD_SCORE );

	//���½���
	m_GameClientView.RefreshGameView( NULL );

	return 0;
}

//Ҫ����Ϣ
LRESULT CGameClientEngine::OnGiveCard( WPARAM wParam, LPARAM lParam )
{
	//Ч��
	WORD wMeChairId = GetMeChairID();
	BYTE cbCardData[MAX_COUNT] = {};
	BYTE cbCardCount = (BYTE)m_GameClientView.m_CardControl[MYSELF_VIEW_ID].GetCardData(cbCardData,MAX_COUNT);
	FLOAT fCardScore = m_GameLogic.GetCardGenre(cbCardData,cbCardCount);
	ASSERT( cbCardCount < MAX_COUNT );
	if( cbCardCount == MAX_COUNT ) return 0;
	ASSERT( fCardScore != CT_ERROR );
	if( fCardScore == CT_ERROR ) return 0;
	ASSERT( !m_bAddScore );
	if( m_bAddScore ) return 0;

	//ɾ����ʱ��
	KillGameClock( IDI_OPERATE_SCORE );

	//���ؿؼ�
	HideScoreControl();

	WORD wViewId = SwitchViewChairID( wMeChairId );

	//������Ҳ���
	m_GameClientView.SetUserAction( wViewId,AC_GIVE_CARD );

	//��������	
	PlayActionSound( wMeChairId,AC_GIVE_CARD );
		
	//��������
	SendSocketData( SUB_C_GIVE_CARD );

	//���½���
	m_GameClientView.RefreshGameView( NULL );

	return 0;
}

//������Ϣ
LRESULT CGameClientEngine::OnStopCard( WPARAM wParam, LPARAM lParam )
{
	WORD wMeChairId = GetMeChairID();
	ASSERT( m_byUserStatus[wMeChairId] );
	if( !m_byUserStatus[wMeChairId] ) return 0;

	//ɾ����ʱ��
	KillGameClock( IDI_OPERATE_SCORE );

	//���ؿؼ�
	HideScoreControl();

	WORD wViewId = SwitchViewChairID( wMeChairId );
	m_byUserStatus[wMeChairId] = FALSE;

	//������Ϸ״̬
	m_GameClientView.SetUserAction( wViewId,AC_STOP_CARD );

	//��������
	SendSocketData( SUB_C_GIVE_UP );

	//��������	
	PlayActionSound( wMeChairId,AC_STOP_CARD );

	//���½���
	m_GameClientView.RefreshGameView( NULL );

	return 0;
}

//��ע��Ϣ
LRESULT CGameClientEngine::OnUserScore( WPARAM wParam, LPARAM lParam )
{
	ASSERT( GetGameStatus() == GS_TH_SCORE );
	if( GetGameStatus() != GS_TH_SCORE ) return 0;
	BYTE cbTime = (BYTE)wParam;
	ASSERT( cbTime <= 4 );
	if( cbTime > 4 ) return 0;

	//ɾ����ʱ��
	KillGameClock( IDI_USER_SCORE );

	//���ؿؼ�
	HideScoreControl();

	//��������
	CMD_C_Score Score = {};
	Score.lScore = m_lCellScore*cbTime;
	SendSocketData( SUB_C_SCORE,&Score,sizeof(Score) );

	return 0;
}

//��ʱ����Ϣ
void CGameClientEngine::OnTimer( UINT nIDEvent )
{
	switch( nIDEvent )
	{
	case IDI_PERFORM_START:
		{
			KillTimer( IDI_PERFORM_START );
			PerformGameStart();
			return;
		}
	case IDI_PERFORM_END:
		{
			KillTimer( IDI_PERFORM_END );
			PerformGameConclude();
			return;
		}
	}
	__super::OnTimer( nIDEvent );
}

//���ؿؼ�
VOID CGameClientEngine::HideScoreControl()
{
	//���ذ�ť
	m_GameClientView.m_btGiveCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btStopCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAddScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOneScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btTwoScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btThreeScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btFourScore.ShowWindow(SW_HIDE);
}

//���¿ؼ�
VOID CGameClientEngine::UpdateScoreControl()
{
	if( GetGameStatus() == GS_TH_PLAY  && m_byUserStatus[GetMeChairID()] == TRUE)
	{
		WORD wMeChairID = GetMeChairID();
		if ((m_wCurrentUser==wMeChairID)&&(m_byUserStatus[wMeChairID]==TRUE)&&!m_bAddScore)
		{
			BYTE cbCardData[MAX_COUNT] = {};
			BYTE cbCardCount = (BYTE)m_GameClientView.m_CardControl[MYSELF_VIEW_ID].GetCardData(cbCardData,MAX_COUNT);
			FLOAT fCardScore = m_GameLogic.GetCardGenre(cbCardData,cbCardCount);

			//��ʾ��ť
			m_GameClientView.m_btGiveCard.ShowWindow( SW_SHOW );
			m_GameClientView.m_btStopCard.ShowWindow( SW_SHOW );
			if( wMeChairID != m_wBankerUser )
				m_GameClientView.m_btAddScore.ShowWindow( SW_SHOW );
			else m_GameClientView.m_btAddScore.ShowWindow( SW_HIDE );

			//ʹ�ܰ�ť
			if( fCardScore != CT_ERROR && fCardScore != 10.5 && cbCardCount < MAX_COUNT )
				m_GameClientView.m_btGiveCard.EnableWindow(TRUE);
			else m_GameClientView.m_btGiveCard.EnableWindow(FALSE);
			if( wMeChairID != m_wBankerUser )
			{
				if( !m_bAddScore && fCardScore != CT_ERROR && fCardScore != 10.5 && cbCardCount < MAX_COUNT )
					m_GameClientView.m_btAddScore.EnableWindow(TRUE);
				else m_GameClientView.m_btAddScore.EnableWindow(FALSE);
			}
		}
	}
	else if( GetGameStatus() == GS_TH_SCORE  && m_byUserStatus[GetMeChairID()] == TRUE)
	{
		TCHAR szTitle[4][32] = {};
		for( WORD i = 0; i < 4; i++ )
		{
			_sntprintf( szTitle[i],CountArray(szTitle[i]),TEXT("%I64d"),m_lCellScore*(i+1) );
		}
		m_GameClientView.m_btOneScore.SetWindowText(szTitle[0]);
		m_GameClientView.m_btTwoScore.SetWindowText(szTitle[1]);
		m_GameClientView.m_btThreeScore.SetWindowText(szTitle[2]);
		m_GameClientView.m_btFourScore.SetWindowText(szTitle[3]);

		m_GameClientView.m_btOneScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_btTwoScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_btThreeScore.ShowWindow(SW_SHOW);
		m_GameClientView.m_btFourScore.ShowWindow(SW_SHOW);
	}
}

//�������
LRESULT CGameClientEngine::OnSendCardFinish( WPARAM wParam, LPARAM lParam )
{
	if( !IsLookonMode() )
		m_GameClientView.m_CardControl[MYSELF_VIEW_ID].SetPositively( true );

	//���������
	BYTE byCardCount = 0;
	BYTE byCardData[MAX_COUNT];
	WORD wMeChairId = GetMeChairID();
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if( !m_byUserStatus[i] ) continue;
		WORD wViewId = SwitchViewChairID( i );
		byCardCount = (BYTE)m_GameClientView.m_CardControl[wViewId].GetCardData( byCardData,MAX_COUNT );
		if( byCardCount > 1 )
		{
			FLOAT fCardScore = CT_ERROR;
			if( !IsLookonMode() && i == wMeChairId )
				fCardScore = m_GameLogic.GetCardGenre( byCardData,byCardCount );
			else
				fCardScore = m_GameLogic.GetCardGenre( &byCardData[1],byCardCount-1 );
			m_GameClientView.SetCardScore( wViewId,fCardScore );
		}
	}

	//���ÿ���
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();

		UpdateScoreControl();
	}

	m_GameClientView.RefreshGameView( NULL );

	//����ʱ��
	if( m_wCurrentUser != INVALID_CHAIR )
		SetGameClock(m_wCurrentUser,IDI_OPERATE_SCORE,TIME_OPERATE_SCORE);

	return 0;
}

//�����ƶ����
LRESULT CGameClientEngine::OnMoveJetFinish( WPARAM wParam, LPARAM lParam )
{
	if( m_bySendCardCount > 0 && GetGameStatus()==GS_TH_PLAY )
	{
		bool bGameStart = m_GameClientView.m_CardControl[MYSELF_VIEW_ID].GetCardCount()==0?true:false;
		//����
		for( BYTE i = 0; i < m_bySendCardCount; i++ )
		{
			WORD wId = m_wStartChairId;
			if( wId == INVALID_CHAIR ) wId = 0;
			for( WORD j = 0; j < GAME_PLAYER; j++ )
			{
				WORD wChairId = (wId+j)%GAME_PLAYER;
				if( m_byUserStatus[wChairId] && (m_bySendCardData[wChairId][i] != 0 || bGameStart) )
				{
					WORD wViewId = SwitchViewChairID( wChairId );
					m_GameClientView.DispatchUserCard( wViewId,m_bySendCardData[wChairId][i] );
				}
			}
		}
		m_bySendCardCount = 0;
		m_wStartChairId = INVALID_CHAIR;
		ZeroMemory( m_bySendCardData,sizeof(m_bySendCardData) );
	}
	return 0;
}

//���Ų�������
VOID CGameClientEngine::PlayActionSound( WORD wChairId, BYTE byAction )
{
	//Ч��
	ASSERT( wChairId != INVALID_CHAIR );
	IClientUserItem * pClientUserItem=GetTableUserItem(wChairId);
	if( pClientUserItem == NULL ) return ;

	if( byAction != AC_GIVE_CARD )
	{
		TCHAR szSoundSrc[64] = TEXT("");

		switch( byAction )
		{
		case AC_STOP_CARD:
			lstrcpy( szSoundSrc,TEXT("STOP_CARD") );
			break;
		case AC_ADD_SCORE:
			lstrcat( szSoundSrc,TEXT("ADD_SCORE") );
			break;
		default:
			ASSERT( FALSE );
			return;
		}

		//����
		PlayGameSound( AfxGetInstanceHandle(),szSoundSrc );
	}
}

//ִ�н���
VOID CGameClientEngine::PerformGameConclude() 
{
	if( m_bGameStart == false ) return;

	KillTimer( IDI_PERFORM_END );

	m_GameClientView.ShowAddJettonInfo(false);
	//���ö���
	if( m_GameEnd.lGameScore[m_wBankerUser] < 0 )
		m_GameClientView.OnUserAddJettons(SwitchViewChairID(m_wBankerUser),-m_GameEnd.lGameScore[m_wBankerUser]);
	m_GameClientView.OnUserAddJettons(INVALID_CHAIR,0);
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if( m_GameEnd.lGameScore[i] > 0 )
			m_GameClientView.OnUserRemoveJettons(SwitchViewChairID(i),m_GameEnd.lGameScore[i]+(i!=m_wBankerUser?m_lTableScore[i]:0));
	}	
	//��ʼ����
	m_GameClientView.BeginMoveJettons();

	//���ֹ�������
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if( m_GameEnd.lGameScore[i] == 0 ) continue;
		WORD wViewId = SwitchViewChairID(i);	
		m_GameClientView.m_NumberControl[wViewId].SetScore( m_GameEnd.lGameScore[i] );
	}
	m_GameClientView.BeginMoveNumber();

	//��Ϸ����
	m_lCellScore = 0L;
	ZeroMemory( m_lTableScore,sizeof(m_lTableScore) );
	m_bGameStart = false;

	//��ұ���
	m_wCurrentUser = INVALID_CHAIR;
	ZeroMemory( m_byUserStatus,sizeof(m_byUserStatus) );

	//�÷���Ϣ
	TCHAR szBuffer[128] = TEXT("");
	if(m_pIStringMessage != NULL)
		m_pIStringMessage->InsertSystemString(TEXT("���ֽ���,�ɼ�ͳ��:"));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if( m_GameEnd.lGameScore[i] != 0L && m_szAccounts[i][0]!=0 )
		{
			//���������ʾ�ɼ�
			_sntprintf(szBuffer,CountArray(szBuffer),TEXT(" \n%s: %+I64d��"),m_szAccounts[i],m_GameEnd.lGameScore[i]);
			if(m_pIStringMessage != NULL)
				m_pIStringMessage->InsertNormalString(szBuffer);
		}
	}

	//��ʼ����
	if ((IsLookonMode()==false)&&(GetMeChairID()!=INVALID_CHAIR))
	{
		SetGameClock( GetMeChairID(),IDI_START_GAME,TIME_START_GAME );
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
	}

	//���½���
	m_GameClientView.RefreshGameView(NULL);

	return;
}

//ִ�п�ʼ
VOID CGameClientEngine::PerformGameStart()
{
	if( m_bGameStart ) return;

	KillTimer( IDI_PERFORM_START );

	//���ñ���
	m_lCellScore = m_GameStart.lCellScore;
	m_wBankerUser = m_GameStart.wBankerUser;
	ZeroMemory( m_lTableScore,sizeof(m_lTableScore) );
	m_GameClientView.SetCellScore( m_lCellScore );
	m_bGameStart = true;

	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);
		IClientUserItem* pClientUserItem=GetTableUserItem(i);
		if( pClientUserItem == NULL ) 
		{
			m_byUserStatus[i] = FALSE;
			continue;
		}

		m_byUserStatus[i] = TRUE;

		//���������
		lstrcpy( m_szAccounts[i],pClientUserItem->GetNickName() );
	}

	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));
	//���ý���
	if( IsLookonMode() )
	{
		//���ó���ؼ�
		m_GameClientView.m_JettonControl.ResetControl();
		//�����˿�
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			m_GameClientView.m_CardControl[i].SetCardData( NULL,0 );
			m_GameClientView.m_CardControl[i].SetDisplayHead( false );
			m_GameClientView.m_CardControl[i].SetPositively( false );
			m_GameClientView.m_NumberControl[i].ResetControl();
		}
		//���������
		m_GameClientView.SetCardScore( INVALID_CHAIR,0 );
	}
	m_GameClientView.SetWaitUserScore(true);
	m_GameClientView.ShowAddJettonInfo(true);

	//���ö�ʱ��
	if( IsLookonMode() == false && GetMeChairID() != m_wBankerUser )
	{
		ActiveGameFrame();

		UpdateScoreControl();
	}

	if( GetMeChairID() != m_wBankerUser )
		SetGameClock(GetMeChairID(),IDI_USER_SCORE,TIME_USER_SCORE);

	return;
}

//�����ע
bool CGameClientEngine::OnSubUserAddScore( const void *pData, WORD wDataSize )
{
	//Ч��
	ASSERT( wDataSize == sizeof(CMD_S_AddScore) );
	if( wDataSize != sizeof(CMD_S_AddScore) ) return false;

	//�������
	CMD_S_AddScore *pAddScore = (CMD_S_AddScore *)pData;

	ASSERT( m_wCurrentUser == pAddScore->wAddScoreUser );
	if( pAddScore->wAddScoreUser != m_wCurrentUser ) return false;

	//ɾ����ʱ��
	KillGameClock(IDI_OPERATE_SCORE);
	
	//ֹ֮ͣǰ���붯��
	if( IsLookonMode() || pAddScore->wAddScoreUser != GetMeChairID() )
		m_GameClientView.StopMoveJettons();

	//���ñ���
	m_wCurrentUser = pAddScore->wCurrentUser;
	BYTE cbSendCardData = pAddScore->cbCardData;

	WORD wMeChairId = GetMeChairID();
	//������Թ��߻���Լ�
	if( IsLookonMode() || wMeChairId != pAddScore->wAddScoreUser )
	{
		//��������
		PlayActionSound( pAddScore->wAddScoreUser,AC_ADD_SCORE );

		WORD wViewId = SwitchViewChairID(pAddScore->wAddScoreUser);

		//������Ҷ���
		m_GameClientView.SetUserAction( wViewId,AC_ADD_SCORE );

		//��ע����
		m_GameClientView.OnUserAddJettons( wViewId,m_lTableScore[pAddScore->wAddScoreUser] );
		m_GameClientView.BeginMoveJettons();

		//���û���
		m_lTableScore[pAddScore->wAddScoreUser] *= 2;
	}

	//����
	if( !m_GameClientView.m_JettonControl.IsPlayMoving() )
	{
		m_GameClientView.DispatchUserCard( SwitchViewChairID(pAddScore->wAddScoreUser),
			cbSendCardData );
	}
	else
	{
		m_bySendCardCount = 1;
		if( m_wStartChairId == INVALID_CHAIR )
			m_wStartChairId = pAddScore->wAddScoreUser;
		m_bySendCardData[pAddScore->wAddScoreUser][0] = cbSendCardData;
	}

	//���½���
	m_GameClientView.RefreshGameView( NULL );

	return true;
}

//��ҷ���
bool CGameClientEngine::OnSubUserStopCard( const void *pData, WORD wDataSize )
{
	//Ч��
	ASSERT( wDataSize == sizeof(CMD_S_GiveUp) );
	if( wDataSize != sizeof(CMD_S_GiveUp) ) return false;

	//�������
	CMD_S_GiveUp *pGiveUp = (CMD_S_GiveUp *)pData;

	WORD wMeChairId = GetMeChairID();

	//ɾ����ʱ��
	KillGameClock( IDI_OPERATE_SCORE );

	//������Թ��߻���Լ�
	if( IsLookonMode() || wMeChairId != pGiveUp->wGiveUpUser )
	{
		//��������
		PlayActionSound( pGiveUp->wGiveUpUser,AC_STOP_CARD );

		WORD wViewId = SwitchViewChairID(pGiveUp->wGiveUpUser);
		//������Ҷ���
		m_GameClientView.SetUserAction( wViewId,AC_STOP_CARD );
	}

	//���õ�ǰ���
	m_wCurrentUser = pGiveUp->wCurrentUser;

	if( !IsLookonMode() && m_wCurrentUser == wMeChairId )
	{
		ActiveGameFrame();

		UpdateScoreControl();
	}

	//���ö�ʱ��
	if( m_wCurrentUser != INVALID_CHAIR )
		SetGameClock( m_wCurrentUser,IDI_OPERATE_SCORE,TIME_OPERATE_SCORE );

	//���½���
	m_GameClientView.RefreshGameView( NULL );
	
	return true;
}

//�������
bool CGameClientEngine::OnSubUserLeft( const void *pData, WORD wDataSize )
{
	//Ч��
	ASSERT( wDataSize == sizeof(CMD_S_UserLeft) );
	if( wDataSize != sizeof(CMD_S_UserLeft) ) return false;

	//�������
	CMD_S_UserLeft *pUserLeft = (CMD_S_UserLeft *)pData;

	m_byUserStatus[pUserLeft->wLeftUser] = FALSE;

	//��������ǵ�ǰ���
	if( pUserLeft->wLeftUser == m_wCurrentUser )
	{
		KillGameClock( IDI_OPERATE_SCORE );

		m_wCurrentUser = pUserLeft->wCurrentUser;

		if( !IsLookonMode() && m_wCurrentUser == GetMeChairID() )
		{
			ActiveGameFrame();

			UpdateScoreControl();
		}

		if( m_wCurrentUser != INVALID_CHAIR )
			SetGameClock( m_wCurrentUser,IDI_OPERATE_SCORE,TIME_OPERATE_SCORE );
	}

	return true;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGamePlay( const void *pData, WORD wDataSize )
{
	//Ч��
	ASSERT( wDataSize == sizeof(CMD_S_GamePlay) );
	if( wDataSize != sizeof(CMD_S_GamePlay) ) return false;

	//�������
	CMD_S_GamePlay *pGamePlay = (CMD_S_GamePlay *)pData;

	//����״̬
	SetGameStatus(GS_TH_PLAY);

	//���ñ���
	m_wCurrentUser = pGamePlay->wCurrentUser;

	CopyMemory( m_lTableScore,pGamePlay->lTableScore,sizeof(pGamePlay->lTableScore) );

	m_GameClientView.SetWaitUserScore(false);

	//����������
	WORD wMeChairId = GetMeChairID();
	m_wStartChairId = m_wBankerUser;
	m_bySendCardCount = 1;
	for( WORD i = 0; i < GAME_PLAYER; i++ )
		m_bySendCardData[i][0] = pGamePlay->byCardData[i];

	//�³ﶯ��
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if( !m_byUserStatus[i] ) continue;
		WORD wViewId = SwitchViewChairID(i);
		if( i != m_wBankerUser )
			m_GameClientView.OnUserAddJettons( wViewId,m_lTableScore[i] );
	}
	m_GameClientView.BeginMoveJettons();


	return true;
}

//////////////////////////////////////////////////////////////////////////

