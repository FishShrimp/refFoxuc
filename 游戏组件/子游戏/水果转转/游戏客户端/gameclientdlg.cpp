#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//��ʱ����ʶ
#define IDI_FREE					99									//����ʱ��
#define IDI_PLACE_JETTON			100									//��עʱ��
#define IDI_DISPATCH_CARD			301									//����ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameEngine)
	ON_MESSAGE(IDM_APPLY_BANKER, OnApplyBanker)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg()
{
	//������Ϣ
	m_iMeMaxScore=0L;			
	m_iApplyBankerCondition=0L;	

	//������ע
	ZeroMemory(m_iMeAreaScoreArray, sizeof(m_iMeAreaScoreArray));

	//ׯ����Ϣ
	m_iBankerScore=0L;
	m_wCurrentBanker=0L;

	//״̬����
	m_bMeApplyBanker=false;


	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}

//��ʼ����
bool CGameClientDlg::OnInitGameEngine()
{
	//���ñ���
	SetWindowText(TEXT("ˮ��תת��Ϸ"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//��������
	VERIFY(m_DTSDBackground.Create(TEXT("BACK_GROUND")));
	VERIFY(m_DTSDCheer[0].Create(TEXT("CHEER1")));
	VERIFY(m_DTSDCheer[1].Create(TEXT("CHEER2")));
	VERIFY(m_DTSDCheer[2].Create(TEXT("CHEER3")));
	VERIFY(m_DTStopJetton.Create("STOP_JETTON"));
	return true;
}

//���ÿ��
bool CGameClientDlg::OnResetGameEngine()
{
	//������Ϣ
	m_iMeMaxScore=0L;			

	//������ע
	ZeroMemory(m_iMeAreaScoreArray, sizeof(m_iMeAreaScoreArray));

	//ׯ����Ϣ
	m_iBankerScore=0L;
	m_wCurrentBanker=0L;

	//״̬����
	m_bMeApplyBanker=false;

	return true;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	if ((wClockID==IDI_PLACE_JETTON)&&(nElapse==0))
	{
		//ׯ�Ұ�ť
		m_GameClientView.m_btApplyBanker.EnableWindow( FALSE );
		m_GameClientView.m_btCancelBanker.EnableWindow( FALSE );
	}

	if (1) 
	{
		if (wClockID==IDI_PLACE_JETTON&&nElapse<=5) 
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("TIME_WARIMG"));
			if( nElapse <= 0 )
			{
				PlayGameSound(AfxGetInstanceHandle(), TEXT("STOP_JETTON"));
			}
		}
	}

	return true;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
}

//������Ϣ
bool CGameClientDlg::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_FREE:		//��Ϸ����
		{
			return OnSubGameFree(pData,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_PLACE_JETTON:	//�û���ע
		{
			return OnSubPlaceJetton(pData,wDataSize);
		}
	case SUB_S_APPLY_BANKER:	//������ׯ
		{
			return OnSubUserApplyBanker(pData, wDataSize);
		}
	case SUB_S_CANCEL_BANKER:	//ȡ����ׯ
		{
			return OnSubUserCancelBanker(pData, wDataSize);
		}
	case SUB_S_CHANGE_BANKER:	//�л�ׯ��
		{
			return OnSubChangeBanker(pData, wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_SEND_RECORD:		//��Ϸ��¼
		{
			return OnSubGameRecord(pData,wDataSize);
		}
	case SUB_S_PLACE_JETTON_FAIL:	//��עʧ��
		{
			return OnSubPlaceJettonFail(pData,wDataSize);
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GAME_STATUS_FREE:			//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) 
				return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;
			if( pStatusFree == NULL )
				return FALSE;

			//����ʱ��
			SetGameClock(GetMeChairID(),IDI_FREE,pStatusFree->cbTimeLeave);

            //�����Ϣ
			m_iMeMaxScore=pStatusFree->iUserMaxScore;

			m_GameClientView.SetMeMaxScore(m_iMeMaxScore);
			WORD wMeChairID=GetMeChairID();
		
			WORD wSwitchViewChairID=SwitchViewChairID(wMeChairID);
			m_GameClientView.SetMeChairID(wSwitchViewChairID);
			
            //ׯ����Ϣ
			SetBankerInfo(pStatusFree->wBankerUser,pStatusFree->iBankerScore);
			m_GameClientView.SetBankerScore(pStatusFree->cbBankerTime,pStatusFree->iBankerWinScore);
			m_bEnableSysBanker=pStatusFree->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

            //������Ϣ
			m_iApplyBankerCondition=pStatusFree->iApplyBankerCondition;
			m_GameClientView.SetAreaLimitScore(pStatusFree->iAreaLimitScore, JETTON_AREA_COUNT);

			//����״̬
			SetGameStatus(GAME_STATUS_FREE);
			
			//��������
			if (1)
			{
				m_DTSDBackground.Play(1,true);
			}
			
			m_GameClientView.m_btAddStep1_H.ShowWindow(SW_SHOW);
			m_GameClientView.m_btAddStep1_H.EnableWindow(true);
			m_GameClientView.m_btAddStep1.ShowWindow(SW_HIDE);

			m_GameClientView.m_btAddStep10.ShowWindow(SW_SHOW);
			m_GameClientView.m_btAddStep10.EnableWindow(true);
			m_GameClientView.m_btAddStep10_H.ShowWindow(SW_HIDE);

			m_GameClientView.m_btAddStep100.ShowWindow(SW_SHOW);
			m_GameClientView.m_btAddStep100.EnableWindow(true);
			m_GameClientView.m_btAddStep100_H.ShowWindow(SW_HIDE);


			m_GameClientView.m_btBetLast.ShowWindow(SW_SHOW);
			m_GameClientView.m_btBetSmall.ShowWindow(SW_SHOW);
			m_GameClientView.m_btBetBig.ShowWindow(SW_SHOW);
			m_GameClientView.m_btBetLast.EnableWindow(true);
			m_GameClientView.m_btBetSmall.EnableWindow(true);
			m_GameClientView.m_btBetBig.EnableWindow(true);

			//���¿���
			UpdateButtonContron();
			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	case GAME_STATUS_PLAY:		//��Ϸ״̬
	case GS_GAME_END:		//����״̬
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) 
				return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;
			if( pStatusPlay == NULL )
				return FALSE;

			if (1)
			{
				m_DTSDBackground.Play(1,true);
			}

			//ȫ����ע
			for( int i=0; i<JETTON_AREA_COUNT; i++)
				m_GameClientView.PlaceUserJetton(i, pStatusPlay->iTotalAreaScore[i]);

			//�����ע
			for( int i=0; i<JETTON_AREA_COUNT; i++ )
				SetMePlaceJetton(i, pStatusPlay->iUserAreaScore[i]);

			//��һ���
			m_iMeMaxScore=pStatusPlay->iUserMaxScore;			
			m_GameClientView.SetMeMaxScore(m_iMeMaxScore);
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetMeChairID(SwitchViewChairID(wMeChairID));

			//������Ϣ
			m_iApplyBankerCondition=pStatusPlay->iApplyBankerCondition;
			m_GameClientView.SetAreaLimitScore(pStatusPlay->iAreaScoreLimit, JETTON_AREA_COUNT);

			if (pStatusPlay->cbGameStatus==GS_GAME_END)
			{
				//���óɼ�
				m_GameClientView.SetCurGameScore(pStatusPlay->iEndUserScore,pStatusPlay->iEndUserReturnScore,pStatusPlay->iEndBankerScore,pStatusPlay->iEndRevenue);
			}
			else
			{
				//��������
				if (1) m_DTSDBackground.Play(0,true);
			}

			//ׯ����Ϣ
			SetBankerInfo(pStatusPlay->wBankerUser,pStatusPlay->iBankerScore);
			m_GameClientView.SetBankerScore(pStatusPlay->cbBankerTime,pStatusPlay->iBankerWinScore);
			m_bEnableSysBanker=pStatusPlay->bEnableSysBanker;
			m_GameClientView.EnableSysBanker(m_bEnableSysBanker);

			//����״̬
			SetGameStatus(pStatusPlay->cbGameStatus);

			//����ʱ��
			SetGameClock(GetMeChairID(),pStatusPlay->cbGameStatus==GS_GAME_END?IDI_DISPATCH_CARD:IDI_PLACE_JETTON,pStatusPlay->cbTimeLeave);

			//���°�ť
			UpdateButtonContron();
			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	}

	return false;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	if( pGameStart == NULL )
		return FALSE;

	//ׯ����Ϣ
	SetBankerInfo(pGameStart->wBankerUser,pGameStart->iBankerScore);

	//�����Ϣ
	m_iMeMaxScore=pGameStart->iUserMaxScore;
	m_GameClientView.SetMeMaxScore(m_iMeMaxScore);

	//��ʾ��Ͷ��Ϣ
	//m_GameClientView.m_btAddStep1_H.ShowWindow(SW_SHOW);
	//m_GameClientView.m_btAddStep1_H.EnableWindow(true);
	//m_GameClientView.m_btAddStep1.ShowWindow(SW_HIDE);

	//m_GameClientView.m_btAddStep10.ShowWindow(SW_SHOW);
	//m_GameClientView.m_btAddStep10.EnableWindow(true);
	//m_GameClientView.m_btAddStep10_H.ShowWindow(SW_HIDE);

	//m_GameClientView.m_btAddStep100.ShowWindow(SW_SHOW);
	//m_GameClientView.m_btAddStep100.EnableWindow(true);
	//m_GameClientView.m_btAddStep100_H.ShowWindow(SW_HIDE);

	//���ø���ע�����ʼ���·�
	m_GameClientView.SetAreaLimitScore(pGameStart->iAreaLimitScore, JETTON_AREA_COUNT);

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_PLACE_JETTON,pGameStart->cbTimeLeave);

	//����״̬
	SetGameStatus(GS_PLACE_JETTON);

	//���¿���
	UpdateButtonContron();

	//���½���
	m_GameClientView.InvalidGameView(0,0,0,0);

	//��������
	//if (1) 
	//{
	//	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
	//	m_DTSDBackground.Play(0,true);
	//}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) 
		return false;

	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;
	if( pGameFree == NULL )
		return false;

	//����ʱ��
	SetGameClock(GetMeChairID(),IDI_FREE,pGameFree->cbTimeLeave);

	tagGameRecord GameRecord;
	ZeroMemory(&GameRecord, sizeof(GameRecord));
	GameRecord.cbGameRecord = pGameFree->cbGameRecord;
	//CString str;
	//str.Format("----%d",GameRecord.cbGameRecord);
	//AfxMessageBox(str);
	GameRecord.cbGameTimes = pGameFree->iGameTimes;

	m_GameClientView.m_GameRecord.FillGameRecord(&GameRecord, 1);

	//����״̬
	SetGameStatus(GAME_STATUS_FREE);

	//��������
	m_GameClientView.SetWinnerSide(0xFF);
	m_GameClientView.CleanUserJetton();
	for (int nAreaIndex=ID_BIG_TIGER; nAreaIndex<=ID_SMALL_SNAKE; ++nAreaIndex) 
		SetMePlaceJetton(nAreaIndex,0);

	//���¿ؼ�
	UpdateButtonContron();

	return true;
}

//�û���ע
bool CGameClientDlg::OnSubPlaceJetton(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJetton));
	if (wDataSize!=sizeof(CMD_S_PlaceJetton)) 
		return false;

	//��Ϣ����
	CMD_S_PlaceJetton * pPlaceJetton=(CMD_S_PlaceJetton *)pBuffer;
	if( pPlaceJetton == NULL )
		return false;
	if( pPlaceJetton->cbJettonArea>ID_SMALL_SNAKE || pPlaceJetton->cbJettonArea<ID_BIG_TIGER )
		return false;

	//��ע����
	m_GameClientView.PlaceUserJetton(pPlaceJetton->cbJettonArea,pPlaceJetton->iJettonScore);

	//��������
	if (1) 
	{
		if (pPlaceJetton->wChairID!=GetMeChairID() || IsLookonMode())
		{
			if (pPlaceJetton->iJettonScore==5000000) 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
			else 
				PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
			//m_DTSDCheer[rand()%3].Play();
		}
	}
    
	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) 
		return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	if( pGameEnd == NULL )
		return FALSE;

	//ȡ������
	for (WORD wAreaIndex=ID_BIG_TIGER; wAreaIndex<=ID_SMALL_SNAKE; ++wAreaIndex) 
		m_GameClientView.SetBombEffect(false,wAreaIndex);

	//CString str;
	//str.Format("%d",pGameEnd->cbGoalAnimal);
	//AfxMessageBox(str);

	m_GameClientView.UpdateCartoonTimeElapse(pGameEnd->cbGoalAnimal);	//���쿨ͨ��ʱ���������

	//���÷��Ƽ�ʱ��
	SetGameClock(GetMeChairID(),IDI_DISPATCH_CARD, pGameEnd->cbTimeLeave);

	//ׯ����Ϣ
	m_GameClientView.SetBankerScore(pGameEnd->nBankerTime, pGameEnd->iBankerTotallScore);

	//�ɼ���Ϣ
	m_GameClientView.SetCurGameScore(pGameEnd->iUserScore,pGameEnd->iUserReturnScore,pGameEnd->iBankerScore,pGameEnd->iRevenue);

	//����״̬
	SetGameStatus(GS_GAME_END);

	//���³ɼ�
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; wUserIndex++)
	{
		IClientUserItem *pUserItem=GetTableUserItem(wUserIndex);
		tagUserInfo *pUserData = NULL;
		if(pUserItem!=NULL)
			pUserData = pUserItem->GetUserInfo();

		if ( pUserData == NULL ) 
			continue;

		tagApplyUser ApplyUser ;
		ZeroMemory(&ApplyUser, sizeof(ApplyUser));

		//������Ϣ
		ApplyUser.iUserScore = pUserData->lScore;
		lstrcpyn(ApplyUser.szUserName, pUserData->szNickName, lstrlen(pUserData->szNickName));
		ApplyUser.dwUserID = pUserData->dwUserID;
		m_GameClientView.m_ApplyUser.UpdateUser(ApplyUser);
	}

	//���¿ؼ�
	UpdateButtonContron();

	//ֹͣ����
	for (int i=0; i<CountArray(m_DTSDCheer); ++i) 
		m_DTSDCheer[i].Stop();

	return true;
}

//���¿���
void CGameClientDlg::UpdateButtonContron()
{
	//�����ж�
	bool bEnablePlaceJetton=true;
	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) 
		bEnablePlaceJetton=false;

	if (GetGameStatus()!=GS_PLACE_JETTON) 
		bEnablePlaceJetton=false;

	if (m_wCurrentBanker==GetMeChairID()) 
		bEnablePlaceJetton=false;

	if (IsLookonMode())
		bEnablePlaceJetton=false;

	//��ע��ť
	if (bEnablePlaceJetton==true)
	{
		//�������
		__int64 iCurrentJetton=m_GameClientView.GetCurrentJetton();
		__int64 iLeaveScore= 0;
		__int64	iTemp = 0;
		for( int i=0; i<JETTON_AREA_COUNT; i++ )
			iTemp += m_iMeAreaScoreArray[i];
		iLeaveScore = m_iMeMaxScore - iTemp;

	}
	//ׯ�Ұ�ť
	if (!IsLookonMode())
	{
		//��ȡ��Ϣ
		IClientUserItem *pUserItem=GetTableUserItem(GetMeChairID());
		tagUserInfo *pMeUserData = NULL;
		if(pUserItem!=NULL)
			pMeUserData = pUserItem->GetUserInfo();

		//���밴ť
		bool bEnableApply=true;
		if (m_wCurrentBanker==GetMeChairID()) 
			bEnableApply=false;

		if (m_bMeApplyBanker) 
			bEnableApply=false;

		if (pMeUserData && pMeUserData->lScore<m_iApplyBankerCondition) 
			bEnableApply=false;

		m_GameClientView.m_btApplyBanker.EnableWindow(bEnableApply?TRUE:FALSE);

		//ȡ����ť
		bool bEnableCancel=true;
		if (m_wCurrentBanker==GetMeChairID() && GetGameStatus()!=GAME_STATUS_FREE) 
			bEnableCancel=false;

		if (m_bMeApplyBanker==false) 
			bEnableCancel=false;

		m_GameClientView.m_btCancelBanker.EnableWindow(bEnableCancel?TRUE:FALSE);
		m_GameClientView.m_btCancelBanker.SetButtonImage(m_wCurrentBanker==GetMeChairID()?IDB_BT_CANCEL_BANKER:IDB_BT_CANCEL_APPLY,AfxGetInstanceHandle(),false,false);

		//��ʾ�ж�
		if (m_bMeApplyBanker)
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_SHOW);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_HIDE);
		}
		else
		{
			m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
			m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		}
	}
	else
	{
		m_GameClientView.m_btCancelBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.EnableWindow(FALSE);
		m_GameClientView.m_btApplyBanker.ShowWindow(SW_SHOW);
		m_GameClientView.m_btCancelBanker.ShowWindow(SW_HIDE);
	}

	//����������ׯ����б�
	int nApplayCount = m_GameClientView.m_ApplyUser.GetApplayBankerUserCount();
	if( nApplayCount <= MAX_SCORE_BANKER )
	{
		/////////////////////////////
		m_GameClientView.m_btBankerMoveUp.ShowWindow(SW_HIDE);
		m_GameClientView.m_btBankerMoveDown.ShowWindow(SW_HIDE);
		/////////////////////////////
		m_GameClientView.m_btBankerMoveUp.EnableWindow(FALSE);
		m_GameClientView.m_btBankerMoveDown.EnableWindow(FALSE);
	}
	else
	{
		/////////////////////////////
		m_GameClientView.m_btBankerMoveUp.ShowWindow(SW_HIDE);
		m_GameClientView.m_btBankerMoveDown.ShowWindow(SW_HIDE);
		/////////////////////////////
		m_GameClientView.m_btBankerMoveUp.EnableWindow(TRUE);
		m_GameClientView.m_btBankerMoveDown.EnableWindow(TRUE);
	}

	//������Ϸ��¼��ť�ؼ�
	int nRecord = m_GameClientView.m_GameRecord.GetRecordCount();
	if( nRecord <= MAX_SHOW_HISTORY )
	{
		m_GameClientView.m_btScoreMoveL.EnableWindow(FALSE);
		m_GameClientView.m_btScoreMoveR.EnableWindow(FALSE);
	}
	else
	{
		int nReadPos = m_GameClientView.m_GameRecord.GetCurReadPos();
		int nWritePos = m_GameClientView.m_GameRecord.GetCurWritePos();
		m_GameClientView.m_btScoreMoveL.EnableWindow(TRUE);
		if( nReadPos != nWritePos-1 )
			m_GameClientView.m_btScoreMoveR.EnableWindow(TRUE);
	}
	return;
}

//��ע��Ϣ
void CGameClientDlg::OnPlaceJetton(BYTE iJettonArea, __int64 iJettonNum)
{
	//��������
	BYTE cbJettonArea=iJettonArea;			//��ע����
	__int64 iJettonScore=iJettonNum;		//��������

	if( cbJettonArea > ID_SMALL_SNAKE || cbJettonArea < ID_BIG_TIGER )
		return;

	//ׯ�Ҳ�����ע
	if ( GetMeChairID() == m_wCurrentBanker )
		return;

	//����ע״̬��ֱ�ӷ���
	if (GetGameStatus()!=GS_PLACE_JETTON)
	{
		UpdateButtonContron();
		return;
	}

	//������ע�������ñ�����ע����
	m_iMeAreaScoreArray[cbJettonArea] += iJettonScore;
	m_GameClientView.SetMePlaceJetton(cbJettonArea, m_iMeAreaScoreArray[cbJettonArea]);

	//��������
	CMD_C_PlaceJetton PlaceJetton;
	ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

	//�������
	PlaceJetton.cbJettonArea=cbJettonArea;
	PlaceJetton.iJettonScore=iJettonScore;

	//����������Ϣ
	SendSocketData(SUB_C_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));

	//���°�ť
	UpdateButtonContron();

	//��������
	if (1) 
	{
		if (iJettonScore==5000000) 
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD_EX"));
		else 
			PlayGameSound(AfxGetInstanceHandle(),TEXT("ADD_GOLD"));
		//m_DTSDCheer[rand()%3].Play();
	}

	return;
}

void CGameClientDlg::OpenBank()
{
	//OnEventOpenBank();
	return;
}
//������Ϣ
LRESULT CGameClientDlg::OnApplyBanker(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	IClientUserItem *pUserItem=GetTableUserItem(GetMeChairID());
	tagUserInfo *pMeUserData = NULL;
	if(pUserItem!=NULL)
		pMeUserData = pUserItem->GetUserInfo();
	if( pMeUserData == NULL )
		return true;

	if (pMeUserData->lScore < m_iApplyBankerCondition) 
		return true;

	//�Թ��ж�
	if (IsLookonMode()) 
		return true;

	//ת������
	bool bApplyBanker = wParam ? true:false;

	//�����ǰ����ׯ�ң��������ٴ�����
	if (m_wCurrentBanker == GetMeChairID() && bApplyBanker) 
		return true;

	if (bApplyBanker)
	{
		//����������ׯ��Ϣ
		SendSocketData(SUB_C_APPLY_BANKER, NULL, 0);
		m_bMeApplyBanker=true;
	}
	else
	{
		//����ȡ����ׯ��Ϣ
		SendSocketData(SUB_C_CANCEL_BANKER, NULL, 0);
		m_bMeApplyBanker=false;
	}

	//���ð�ť
	UpdateButtonContron();

	return true;
}

//������ׯ
bool CGameClientDlg::OnSubUserApplyBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ApplyBanker));
	if (wDataSize!=sizeof(CMD_S_ApplyBanker)) 
		return false;

	//��Ϣ����
	CMD_S_ApplyBanker * pApplyBanker=(CMD_S_ApplyBanker *)pBuffer;
	if( pApplyBanker == NULL )
		return FALSE;

	//��ȡ���
	IClientUserItem *pUserItem=GetTableUserItem(pApplyBanker->wApplyUser);
	tagUserInfo *pUserData = NULL;
	if(pUserItem!=NULL)
		pUserData = pUserItem->GetUserInfo();

	if( pUserData == NULL )
		return FALSE;

	//�������
	if (m_wCurrentBanker != pApplyBanker->wApplyUser)
	{
		tagApplyUser ApplyUser;
		::ZeroMemory(&ApplyUser, sizeof(ApplyUser));

		lstrcpyn(ApplyUser.szUserName, pUserData->szNickName, sizeof(ApplyUser.szUserName));

		ApplyUser.iUserScore=pUserData->lScore;
		ApplyUser.dwUserID = pUserData->dwUserID;
		m_GameClientView.m_ApplyUser.InserUser(ApplyUser);
	}

	//�Լ��ж�
	if (IsLookonMode()==false && GetMeChairID()==pApplyBanker->wApplyUser) 
		m_bMeApplyBanker=true;

	//���¿ؼ�
	UpdateButtonContron();

	return true;
}

//ȡ����ׯ
bool CGameClientDlg::OnSubUserCancelBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CancelBanker));
	if (wDataSize!=sizeof(CMD_S_CancelBanker)) 
		return false;

	//��Ϣ����
	CMD_S_CancelBanker * pCancelBanker=(CMD_S_CancelBanker *)pBuffer;
	if( pCancelBanker == NULL )
		return FALSE;

	//ɾ�����
	tagApplyUser ApplyUser;
	::ZeroMemory(&ApplyUser, sizeof(ApplyUser));
	lstrcpyn(ApplyUser.szUserName, pCancelBanker->szCancelUser, lstrlen(pCancelBanker->szCancelUser));
	ApplyUser.iUserScore=0;
	ApplyUser.dwUserID = 0xFF;
	for (WORD wUserIndex = 0; wUserIndex < MAX_CHAIR; ++wUserIndex)
	{
		IClientUserItem *pUserItem=GetTableUserItem(wUserIndex);
		tagUserInfo *pUserData = NULL;
		if(pUserItem!=NULL)
			pUserData = pUserItem->GetUserInfo();

		if ( pUserData == NULL ) 
			continue;
		if(!lstrcmpi(pUserData->szNickName, pCancelBanker->szCancelUser))
		{
			ApplyUser.dwUserID = pUserData->dwUserID;
			break;
		}
	}
	if( ApplyUser.dwUserID != 0xFF)
		m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);

	//�Լ��ж�
	IClientUserItem *pUserItem=GetTableUserItem(GetMeChairID());
	tagUserInfo *pMeUserData = NULL;
	if(pUserItem!=NULL)
		pMeUserData = pUserItem->GetUserInfo();
	if( pMeUserData == NULL )
		return FALSE;

	//����ȡ����ׯ
	if (IsLookonMode()==false && !lstrcmp(pMeUserData->szNickName,pCancelBanker->szCancelUser)) 
		m_bMeApplyBanker=false;

	//���¿ؼ�
	UpdateButtonContron();

	return true;
}

//�л�ׯ��
bool CGameClientDlg::OnSubChangeBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_ChangeBanker));
	if (wDataSize!=sizeof(CMD_S_ChangeBanker)) 
		return false;

	//��Ϣ����
	CMD_S_ChangeBanker * pChangeBanker=(CMD_S_ChangeBanker *)pBuffer;
	if( pChangeBanker == NULL )
		return FALSE;

	//��ʾͼƬ
	m_GameClientView.ShowChangeBanker(m_wCurrentBanker!=pChangeBanker->wBankerUser);

	//�Լ��ж�
	if (m_wCurrentBanker==GetMeChairID() && IsLookonMode() == false && pChangeBanker->wBankerUser!=GetMeChairID()) 
	{
		m_bMeApplyBanker=false;
	}
	else if (IsLookonMode() == false && pChangeBanker->wBankerUser==GetMeChairID())
	{
		m_bMeApplyBanker=true;
	}

	//ׯ����
	SetBankerInfo(pChangeBanker->wBankerUser,pChangeBanker->iBankerScore);
	m_GameClientView.SetBankerScore(0,0);

	//ɾ�����
	if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		IClientUserItem *pUserItem=GetTableUserItem(m_wCurrentBanker);
		tagUserInfo *pBankerUserData = NULL;
		if(pUserItem!=NULL)
			pBankerUserData = pUserItem->GetUserInfo();

		if (pBankerUserData != NULL)
		{
			tagApplyUser ApplyUser;
			::ZeroMemory(&ApplyUser, sizeof(ApplyUser));
			lstrcpyn(ApplyUser.szUserName, pBankerUserData->szNickName, lstrlen(pBankerUserData->szNickName));
			ApplyUser.dwUserID = pBankerUserData->dwUserID;
			m_GameClientView.m_ApplyUser.DeleteUser(ApplyUser);
		}
	}

	//���½���
	UpdateButtonContron();
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//��Ϸ��¼
bool CGameClientDlg::OnSubGameRecord(const void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(tagServerGameRecord)==0);
	if (wDataSize%sizeof(tagServerGameRecord)!=0) 
		return false;

	if( pBuffer == NULL )
		return FALSE;

	//�������
	tagGameRecord GameRecord;
	ZeroMemory(&GameRecord,sizeof(GameRecord));

	//���ü�¼
	WORD wRecordCount=wDataSize/sizeof(tagServerGameRecord);
	for (WORD wIndex=0;wIndex<wRecordCount;wIndex++) 
	{
		tagServerGameRecord * pServerGameRecord=(((tagServerGameRecord *)pBuffer)+wIndex);
		if( pServerGameRecord == NULL )
			continue;

		GameRecord.cbGameTimes = pServerGameRecord->cbGameTimes;
		GameRecord.cbGameRecord = pServerGameRecord->cbRecord;
		m_GameClientView.SetGameHistory(&GameRecord, 1);
	}

	return true;
}

//��עʧ��
bool CGameClientDlg::OnSubPlaceJettonFail(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlaceJettonFail));
	if (wDataSize!=sizeof(CMD_S_PlaceJettonFail)) 
		return false;

	//��Ϣ����
	CMD_S_PlaceJettonFail * pPlaceJettonFail=(CMD_S_PlaceJettonFail *)pBuffer;
	if( pPlaceJettonFail == NULL )
		return FALSE;

	//Ч�����
	BYTE cbViewIndex=pPlaceJettonFail->lJettonArea;	//ȡ����ע����
	ASSERT(cbViewIndex<=ID_SMALL_SNAKE);
	if (cbViewIndex>ID_SMALL_SNAKE) 
		return false;

	//��ע����
	__int64 iJettonCount=pPlaceJettonFail->iPlaceScore;
	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		if( cbViewIndex == i )
		{
			m_iMeAreaScoreArray[i] -= iJettonCount;
			m_GameClientView.SetMePlaceJetton(cbViewIndex, m_iMeAreaScoreArray[i]);
		}
	}
	return true;
}

//����ׯ��
void CGameClientDlg::SetBankerInfo(WORD wBanker,__int64 iScore)
{
	m_wCurrentBanker=wBanker;
	m_iBankerScore=iScore;
	WORD wBankerViewChairID=m_wCurrentBanker==INVALID_CHAIR ? INVALID_CHAIR:SwitchViewChairID(m_wCurrentBanker);
	m_GameClientView.SetBankerInfo(wBankerViewChairID,m_iBankerScore);
}

//������ע
void CGameClientDlg::SetMePlaceJetton(BYTE cbViewIndex, __int64 iJettonCount)
{
	//Ч�����
	ASSERT(cbViewIndex<= ID_SMALL_SNAKE);
	if (cbViewIndex>ID_SMALL_SNAKE) 
		return;

	for( int i=0; i<JETTON_AREA_COUNT; i++ )
	{
		if( cbViewIndex == i )
			m_iMeAreaScoreArray[i] = iJettonCount;
	}

	//���ý���
	m_GameClientView.SetMePlaceJetton(cbViewIndex,iJettonCount);
}
//////////////////////////////////////////////////////////////////////////
