#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////////////
//��ʱ��
//����ʱ��
#define IDI_FREE					101									//����ʱ��
#define IDI_BET						102									//��עʱ��
#define IDI_END						103									//����ʱ��

#define BET_NUMBER					(10)

//////////////////////////////////////////////////////////////////////////////////

//����·��
#define SOUND_MAX_COUNT				(13)
TCHAR g_szSoundPathT[SOUND_MAX_COUNT][MAX_PATH] = { 
													TEXT("\\Sound\\����.mp3"), 
													TEXT("\\Sound\\��ȸ.mp3"), 
													TEXT("\\Sound\\����.mp3"), 
													TEXT("\\Sound\\��è.mp3"), 
													TEXT("\\Sound\\����.mp3"), 
													TEXT("\\Sound\\ʨ��.mp3"), 
													TEXT("\\Sound\\����.mp3"), 
													TEXT("\\Sound\\����.mp3"), 
													TEXT("\\Sound\\��ӥ.mp3"), 
													TEXT("\\Sound\\����.mp3"), 
													TEXT("\\Sound\\��������.mp3"), 
													TEXT("\\Sound\\����.mp3"),
													TEXT("\\Sound\\����.mp3")	};
//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)

	//��Ϸ��Ϣ
	ON_MESSAGE(IDM_UPDATE_CHIP,OnMessageUpDateChip)
	ON_MESSAGE(IDM_EXCHANGE_CHIP, OnMessageChip)
	ON_MESSAGE(IDM_PLAY_BET, OnMessagePlayBet)
	ON_MESSAGE(IDM_OPERATION_RENEWAL, OnMessageOperationRenewal)
	ON_MESSAGE(IDM_OPERATION_CANCEL, OnMessageOperationCancel)
	ON_MESSAGE(IDM_OPERATION_SWITCH, OnMessageOperationSwitch)
	ON_MESSAGE(IDM_ADMIN_COMMDN, OnAdminControl)

	//ϵͳ��Ϣ
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
	//��Ϸ����
	m_lCellScore = 0;
	m_lPlayScore = 0;
	m_lPlayChip = 0;
	m_nTurnTableRecord = INT_MAX;
	m_lHasInChip = FALSE;

	//��ע����
	m_nCurrentNote = 0;
	m_nUnitNote = 0;
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));
	ZeroMemory(m_lPlayAllBet, sizeof(m_lPlayAllBet));
	ZeroMemory(m_hPlayAllBet, sizeof(m_hPlayAllBet));
	for ( int i = 0; i < ANIMAL_MAX; ++i )
		m_lPlayLastBet[i] = -1;
	m_lPlayWin = 0;

	//���ñ���
	m_lAreaLimitScore = 0;
	m_lPlayLimitScore = 0;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
	//�������
	CD3DMusic::Delete();
}

//��������
bool CGameClientEngine::OnInitGameEngine()
{
	//��������
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);

	if ( !CGlobalUnits::GetInstance()->m_bAllowSound || CGlobalUnits::GetInstance()->m_bMuteStatuts )
		CD3DMusic::_Object()->ControlVolume(-10000);
	else
		CD3DMusic::_Object()->ControlVolume(0);

	return true;
}

//���ú���
bool CGameClientEngine::OnResetGameEngine()
{
	//��Ϸ����
	m_lCellScore = 0;
	m_lPlayScore = 0;
	m_lPlayChip = 0;
	m_nTurnTableRecord = INT_MAX;
	m_lHasInChip = FALSE;

	//��ע����
	m_nCurrentNote = 0;
	m_nUnitNote = 0;
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));
	ZeroMemory(m_lPlayAllBet, sizeof(m_lPlayAllBet));
	ZeroMemory(m_hPlayAllBet, sizeof(m_hPlayAllBet));
	for ( int i = 0; i < ANIMAL_MAX; ++i )
		m_lPlayLastBet[i] = -1;
	m_lPlayWin = 0;

	//���ñ���
	m_lAreaLimitScore = 0;
	m_lPlayLimitScore = 0;

	return true;
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	switch( wClockID )
	{
	case IDI_BET:
		{
			if ( nElapse <= 5 && nElapse > 1 )
			{
				CD3DMusic::_Object()->Start(TEXT("\\Sound\\5-2����ʱ.mp3"));
			}
			else if ( nElapse == 1 )
			{
				CD3DMusic::_Object()->Start(TEXT("\\Sound\\��󵹼�ʱ.mp3"));
			}
		}
		break;
	}
	return true;
}

//�Թ���Ϣ
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//��Ϸ��Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch ( wSubCmdID )
	{
	case SUB_S_GAME_FREE:	//����ʱ��
		return OnSubGameFree(pData, wDataSize);
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		return OnSubGameStart(pData, wDataSize);
	case SUB_S_GAME_END:	//��Ϸ����
		return OnSubGameEnd(pData, wDataSize);
	case SUB_S_PLAY_BET:	//�����ע
		return OnSubPlayBet(pData, wDataSize);
	case SUB_S_PLAY_BET_FAIL://��עʧ��
		return OnSubPlayBetFail(pData, wDataSize);
	case SUB_S_BET_CLEAR:	//�����ע
		return OnSubBetClear(pData, wDataSize);
	case SUB_S_AMDIN_COMMAND:		//����
		{
			return OnSubAdminControl(pData,wDataSize);
		}	
	}
	return false;
}

//������Ϣ
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
 //����
 if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
 m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
	switch ( cbGameStatus )
	{
	case GAME_SCENE_FREE:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusFree));
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//��Ϣ����
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//����״̬
			SetGameStatus(GAME_SCENE_FREE);
			m_GameClientView.SetGameStatus(GAME_SCENE_FREE);

			//�����Լ�λ��
			m_GameClientView.SetMineChairID(SwitchViewChairID(GetMeChairID()));

			//����ʱ��
			SetGameClock(GetMeChairID(), IDI_FREE, pStatusFree->cbTimeLeave);

			//���ñ���
			m_lCellScore = pStatusFree->lCellScore;
			m_lPlayScore = pStatusFree->lPlayScore;
			m_lAreaLimitScore = pStatusFree->lAreaLimitScore;
			m_lPlayLimitScore = pStatusFree->lPlayLimitScore;
			m_lPlayChip = 0;

			//������Ϸ��¼
			for( int i = 0; i < RECORD_COUNT_MAX; ++i )
			{
				if ( pStatusFree->nTurnTableRecord[i] != INT_MAX )
				{
					m_GameClientView.AddTurnTableRecord( CGameLogic::GetInstance()->TurnTableAnimal(pStatusFree->nTurnTableRecord[i]) );
				}
			}
			m_GameClientView.SetShowTurnTableRecord(TRUE);
			
			//�һ�����
			if ( !IsLookonMode() )
			{
				//���õ�λע
				m_nUnitNote = BET_NUMBER;
				m_nCurrentNote = BET_NUMBER;

				m_GameClientView.SetCurrentNote(m_nCurrentNote);

				//���öһ�����
				m_lHasInChip = FALSE;
				m_GameClientView.m_WindowChip.SetChipInfo(pStatusFree->lCellScore, pStatusFree->lPlayScore, 0);
				m_GameClientView.m_WindowChip.ShowWindow(true);
			}

			//�����㶯��
			m_GameClientView.m_FishDraw.SetAddFish(TRUE);
			m_GameClientView.m_FishDraw.SetBackIndex(0);

			//��������
			m_GameClientView.m_WindowTip.SetKeepOut(TRUE);
			m_GameClientView.m_WindowTip.CloseFlicker();

			//��������
			CD3DMusic::_Object()->Stop();
			if ( rand()%2 == 0 )
				CD3DMusic::_Object()->Start(TEXT("\\Sound\\����.mp3"));
			else
				CD3DMusic::_Object()->Start(TEXT("\\Sound\\����2.mp3"));
		}
		return true;
	case GAME_SCENE_BET:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusPlay));
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;

			//��Ϣ����
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//����״̬
			SetGameStatus(GAME_SCENE_BET);
			m_GameClientView.SetGameStatus(GAME_SCENE_BET);

			//�����Լ�λ��
			m_GameClientView.SetMineChairID(SwitchViewChairID(GetMeChairID()));

			//����ʱ��
			SetGameClock(GetMeChairID(), IDI_BET, pStatusPlay->cbTimeLeave);

			//���ñ���
			m_lCellScore = pStatusPlay->lCellScore;
			m_lPlayScore = pStatusPlay->lPlayScore;
			m_lAreaLimitScore = pStatusPlay->lAreaLimitScore;
			m_lPlayLimitScore = pStatusPlay->lPlayLimitScore;
			m_lPlayChip = 0;

			//������Ϸ��¼
			for( int i = 0; i < RECORD_COUNT_MAX; ++i )
			{
				if ( pStatusPlay->nTurnTableRecord[i] != INT_MAX )
				{
					m_GameClientView.AddTurnTableRecord( CGameLogic::GetInstance()->TurnTableAnimal(pStatusPlay->nTurnTableRecord[i]) );
				}
			}
			m_GameClientView.SetShowTurnTableRecord(TRUE);

			//����ȫ����ע
			m_GameClientView.SetAnimalMultiple( pStatusPlay->nAnimalMultiple );
			m_GameClientView.SetPlayAllBet( pStatusPlay->lAllBet );
			m_GameClientView.m_BetWnd.SetPlayAllBet(pStatusPlay->lAllBet);
			if ( !IsLookonMode() )
				m_GameClientView.SetPlayBet( pStatusPlay->lPlayBet );

			//�һ�����
			if ( !IsLookonMode() )
			{
				//���õ�λע
				m_nUnitNote = BET_NUMBER;
				m_nCurrentNote = BET_NUMBER;

				m_GameClientView.SetCurrentNote(m_nCurrentNote);

				//���ó���
				m_lPlayChip = pStatusPlay->lPlayChip;
				if( m_lPlayChip == 0 )
				{
					//���öһ�����
					m_lHasInChip = FALSE;
					m_GameClientView.m_WindowChip.SetChipInfo(pStatusPlay->lCellScore, pStatusPlay->lPlayScore, 0);
					m_GameClientView.m_WindowChip.ShowWindow(true);
				}
				else
				{
					//���ó���
					m_GameClientView.SetPlayChip(m_lPlayChip);
				}

				//��ť
				for ( int i = 0; i < ANIMAL_MAX; ++i )
				{
					m_GameClientView.m_btSelectBet[i].EnableWindow(true);
				}
				m_GameClientView.m_btOperationRenewal.EnableWindow(true);
				m_GameClientView.m_btOperationCancel.EnableWindow(true);
				m_GameClientView.m_btOperationSwitch.EnableWindow(true);
			}

			//�����㶯��
			m_GameClientView.m_FishDraw.SetAddFish(TRUE);
			m_GameClientView.m_FishDraw.SetBackIndex(0);

			//��������
			m_GameClientView.m_WindowTip.SetKeepOut(FALSE);
			m_GameClientView.m_WindowTip.CloseFlicker();

			//��������
			CD3DMusic::_Object()->Stop();
			if ( rand()%2 == 0 )
				CD3DMusic::_Object()->Start(TEXT("\\Sound\\����.mp3"));
			else
				CD3DMusic::_Object()->Start(TEXT("\\Sound\\����2.mp3"));
		}
		return true;
	case GAME_SCENE_END:
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_StatusEnd));
			if (wDataSize!=sizeof(CMD_S_StatusEnd)) return false;

			//��Ϣ����
			CMD_S_StatusEnd * pStatusEnd=(CMD_S_StatusEnd *)pData;

			//����״̬
			SetGameStatus(GAME_SCENE_END);
			//�������ý���Ϊ��ע״̬�� ����ʾʱ��
			m_GameClientView.SetGameStatus(GAME_SCENE_BET);

			//�����Լ�λ��
			m_GameClientView.SetMineChairID(SwitchViewChairID(GetMeChairID()));

			//����ʱ��
			SetGameClock(GetMeChairID(), IDI_END, pStatusEnd->cbTimeLeave);

			//���ñ���
			m_lCellScore = pStatusEnd->lCellScore;
			m_lPlayScore = pStatusEnd->lPlayScore;
			m_lAreaLimitScore = pStatusEnd->lAreaLimitScore;
			m_lPlayLimitScore = pStatusEnd->lPlayLimitScore;
			m_lPlayChip = 0;

			//������Ϸ��¼
			for( int i = 0; i < RECORD_COUNT_MAX; ++i )
			{
				if ( pStatusEnd->nTurnTableRecord[i] != INT_MAX )
				{
					m_GameClientView.AddTurnTableRecord( CGameLogic::GetInstance()->TurnTableAnimal(pStatusEnd->nTurnTableRecord[i]) );
				}
			}
			m_GameClientView.SetShowTurnTableRecord(TRUE);

			//����ȫ����ע
			m_GameClientView.SetAnimalMultiple( pStatusEnd->nAnimalMultiple );
			m_GameClientView.SetPlayAllBet( pStatusEnd->lAllBet );
			m_GameClientView.m_BetWnd.SetPlayAllBet(pStatusEnd->lAllBet);
			if ( !IsLookonMode() )
				m_GameClientView.SetPlayBet( pStatusEnd->lPlayBet );

			//�һ�����
			if ( !IsLookonMode() )
			{
				//���õ�λע
				m_nUnitNote = BET_NUMBER;
				m_nCurrentNote = BET_NUMBER;

				m_GameClientView.SetCurrentNote(m_nCurrentNote);

				//���ó���
				m_lPlayChip = pStatusEnd->lPlayChip;
				if( m_lPlayChip == 0 )
				{
					//���öһ�����
					m_lHasInChip = FALSE;
					m_GameClientView.m_WindowChip.SetChipInfo(pStatusEnd->lCellScore, pStatusEnd->lPlayScore, 0);
					m_GameClientView.m_WindowChip.ShowWindow(true);
				}
				else
				{
					//���ó���
					m_GameClientView.SetPlayChip(m_lPlayChip);
				}
			}

			//�����㶯��
			m_GameClientView.m_FishDraw.SetAddFish(TRUE);
			m_GameClientView.m_FishDraw.SetBackIndex(0);

			//��������
			m_GameClientView.m_WindowTip.SetKeepOut(TRUE);
			m_GameClientView.m_WindowTip.CloseFlicker();

			//��������
			CD3DMusic::_Object()->Stop();
			if ( rand()%2 == 0 )
				CD3DMusic::_Object()->Start(TEXT("\\Sound\\����.mp3"));
			else
				CD3DMusic::_Object()->Start(TEXT("\\Sound\\����2.mp3"));
		}
		return true;
	}

	return false;
}


//�����¼�
bool CGameClientEngine::OnGameOptionChange()
{
	if ( !CGlobalUnits::GetInstance()->m_bAllowSound || CGlobalUnits::GetInstance()->m_bMuteStatuts )
		CD3DMusic::_Object()->ControlVolume(-10000);
	else
		CD3DMusic::_Object()->ControlVolume(0);

	return true;
}

//��������
bool CGameClientEngine::AllowBackGroundSound(bool bAllowSound)
{
	if ( !CGlobalUnits::GetInstance()->m_bAllowSound || CGlobalUnits::GetInstance()->m_bMuteStatuts )
		CD3DMusic::_Object()->ControlVolume(-10000);
	else
		CD3DMusic::_Object()->ControlVolume(0);

	return true;
}

//����ʱ��
bool CGameClientEngine::OnSubGameFree(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameFree));
	if (wDataSize!=sizeof(CMD_S_GameFree)) return false;

	//��Ϣ����
	CMD_S_GameFree * pGameFree=(CMD_S_GameFree *)pBuffer;
	WORD wMeChair = GetMeChairID();
	if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
		m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

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
	WORD wMeChair = GetMeChairID();

	//Ԥִ����Ϣ

	//��ӵ�Ӯ��Ǯ
	m_lPlayChip += m_lPlayWin;
	m_lPlayWin = 0;
	m_GameClientView.SetPlayChip(m_lPlayChip);
	m_GameClientView.SetPlayShowWin(0);
	m_GameClientView.SetPlayShowPrizes(0);

	//���ñ���
	int nAnimalMultiple[ANIMAL_MAX] = {0};
	ZeroMemory(nAnimalMultiple, sizeof(nAnimalMultiple));
	m_GameClientView.SetAnimalMultiple(nAnimalMultiple);

	//������һ��
	for( int i = 0; i < ANIMAL_MAX; ++i )
	{
		if ( m_lPlayBet[i] != 0 )
		{
			CopyMemory(m_lPlayLastBet, m_lPlayBet, sizeof(m_lPlayBet));
			break;
		}
	}

	//�����ע���
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));
	ZeroMemory(m_lPlayAllBet, sizeof(m_lPlayAllBet));
	ZeroMemory(m_hPlayAllBet, sizeof(m_hPlayAllBet));

	//���ü�¼
	if ( m_nTurnTableRecord != INT_MAX )
	{
		m_GameClientView.AddTurnTableRecord( CGameLogic::GetInstance()->TurnTableAnimal(m_nTurnTableRecord) );
	}
	m_GameClientView.SetShowTurnTableRecord(TRUE);

	//���ý���
	m_GameClientView.EndWin();
	m_GameClientView.SetPlayBet(m_lPlayBet);
	m_GameClientView.SetPlayAllBet(m_lPlayAllBet);
	m_GameClientView.m_BetWnd.SetPlayAllBet(m_lPlayAllBet);
	m_GameClientView.EndTurnTable();
	m_GameClientView.m_WindowOver.ShowWindow(false);
	m_GameClientView.m_FishDraw.SetBackIndex(0);

	//�����㶯��
	m_GameClientView.m_FishDraw.SetAddFish(TRUE);

	//��������
	m_GameClientView.m_WindowTip.SetKeepOut(TRUE);
	m_GameClientView.m_WindowTip.CloseFlicker();

	//�����Ƿ�ر�
	static bool bClose = false;
	if ( m_lPlayChip <= 0 && m_lHasInChip && !bClose )
	{
		bClose = true;
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("���ĳ��������꣬���˳���Ϸ�������������Ϸ��"));
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
		return true;
	}

	//��������
	CD3DMusic::_Object()->Stop();
	if ( rand()%2 == 0 )
		CD3DMusic::_Object()->Start(TEXT("\\Sound\\����.mp3"));
	else
		CD3DMusic::_Object()->Start(TEXT("\\Sound\\����2.mp3"));

	//ԭʼ��Ϸ��ʼ��ע��Ϣ
	//����״̬
	SetGameStatus(GAME_SCENE_BET);
	m_GameClientView.SetGameStatus(GAME_SCENE_BET);

	//����ʱ��
	SetGameClock(wMeChair, IDI_BET, pGameStart->cbTimeLeave);

	//���ñ���
	m_GameClientView.SetAnimalMultiple(pGameStart->nAnimalMultiple);

	//�����ע���
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));
	ZeroMemory(m_lPlayAllBet, sizeof(m_lPlayAllBet));
	ZeroMemory(m_hPlayAllBet, sizeof(m_hPlayAllBet));

	//���ý���
	m_GameClientView.EndWin();
	m_GameClientView.SetPlayBet(m_lPlayBet);
	m_GameClientView.SetPlayAllBet(m_lPlayAllBet);
	m_GameClientView.m_BetWnd.SetPlayAllBet(m_lPlayAllBet);
	m_GameClientView.EndTurnTable();
	m_GameClientView.m_WindowOver.ShowWindow(false);

	if ( !IsLookonMode() )
	{
		//������ť
		for ( int i = 0; i < ANIMAL_MAX; ++i )
		{
			m_GameClientView.m_btSelectBet[i].EnableWindow(true);
		}
		m_GameClientView.m_btOperationRenewal.EnableWindow(true);
		m_GameClientView.m_btOperationCancel.EnableWindow(true);
		m_GameClientView.m_btOperationSwitch.EnableWindow(true);
	}

	//��������
	m_GameClientView.m_WindowTip.SetKeepOut(FALSE);
	m_GameClientView.m_WindowTip.CloseFlicker();
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_BIRD],55,83,false);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_BEAST],168,83,false);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_GOLD_SHARK],55,130,false);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_SLIVER_SHARK],168,130,false);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_LION],28,200,false);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_PANDA],162,200,false);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_MONKEY],28,258,false);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_RABBIT],162,258,false);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_EAGLE],28,315,false);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_PEACOCK],162,315,false);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_PIGEON],28,378,false);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_SWALLOW],162,378,false);
	if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
		m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
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
	WORD wMeChair = GetMeChairID();

	//����״̬
	SetGameStatus(GAME_SCENE_END);
	m_GameClientView.SetGameStatus(GAME_SCENE_END);

	//����ʱ��
	SetGameClock(wMeChair, IDI_END, pGameEnd->cbTimeLeave);

	//��������
	m_GameClientView.GameOver(pGameEnd->bTurnTwoTime, pGameEnd->nTurnTableTarget, pGameEnd->nPrizesMultiple, pGameEnd->lPlayWin, pGameEnd->lPlayPrizes );

	//����Ӯ��Ǯ
	m_lPlayWin = pGameEnd->lPlayWin[0] + pGameEnd->lPlayWin[1] + pGameEnd->lPlayPrizes;

	//�����¼
	m_nTurnTableRecord = pGameEnd->nTurnTableTarget[0];
	m_GameClientView.SetShowTurnTableRecord(FALSE);

	//�رհ�ť
	for ( int i = 0; i < ANIMAL_MAX; ++i )
	{
		m_GameClientView.m_btSelectBet[i].EnableWindow(false);
	}
	m_GameClientView.m_btOperationRenewal.EnableWindow(false);
	m_GameClientView.m_btOperationCancel.EnableWindow(false);
	m_GameClientView.m_btOperationSwitch.EnableWindow(false);
	m_GameClientView.m_FishDraw.SetBackIndex(2);

	//�ر��㶯��
	m_GameClientView.m_FishDraw.SetAddFish(FALSE);

	//��������
	m_GameClientView.m_WindowTip.SetKeepOut(TRUE);

	//���òʽ�
	m_GameClientView.SetPlayShowPrizes(pGameEnd->lPlayShowPrizes);
	if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
		m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);

	return true;
}

//�����ע
bool CGameClientEngine::OnSubPlayBet(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlayBet));
	if (wDataSize!=sizeof(CMD_S_PlayBet)) return false;

	//��Ϣ����
	CMD_S_PlayBet * pPlayBet=(CMD_S_PlayBet *)pBuffer;

	//���Լ��Ŵ���
	if ( GetMeChairID() != pPlayBet->wChairID || IsLookonMode() )
	{
		//���ñ���
		m_lPlayAllBet[pPlayBet->nAnimalIndex] += pPlayBet->lBetChip;

		//���ý���
		m_GameClientView.SetPlayAllBet(m_lPlayAllBet[pPlayBet->nAnimalIndex], pPlayBet->nAnimalIndex);
		m_GameClientView.m_BetWnd.SetPlayAllBet(m_lPlayAllBet[pPlayBet->nAnimalIndex], pPlayBet->nAnimalIndex);
	}
	m_hPlayAllBet[pPlayBet->nAnimalIndex] += pPlayBet->mHBet;
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_BIRD],55,83);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_BEAST],168,83);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_GOLD_SHARK],55,130);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_SLIVER_SHARK],168,130);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_LION],28,200);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_PANDA],162,200);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_MONKEY],28,258);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_RABBIT],162,258);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_EAGLE],28,315);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_PEACOCK],162,315);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_PIGEON],28,378);
	m_GameClientView.m_pClientControlDlg->SetText(m_hPlayAllBet[ANIMAL_SWALLOW],162,378);
	if(CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)&&m_GameClientView.m_hInst)
		m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
	return true;
}

//��עʧ��
bool CGameClientEngine::OnSubPlayBetFail(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PlayBetFail));
	if (wDataSize!=sizeof(CMD_S_PlayBetFail)) return false;

	//��Ϣ����
	CMD_S_PlayBetFail * pPlayBetFail = (CMD_S_PlayBetFail *)pBuffer;

	//�Լ��Ŵ���
	if ( GetMeChairID() == pPlayBetFail->wChairID && !IsLookonMode() )
	{
		//���ñ���
		m_lPlayChip += pPlayBetFail->lBetChip;
		m_lPlayBet[pPlayBetFail->nAnimalIndex] -= pPlayBetFail->lBetChip;
		m_lPlayAllBet[pPlayBetFail->nAnimalIndex] -= pPlayBetFail->lBetChip;

		//���ý���
		m_GameClientView.SetPlayChip(m_lPlayChip);
		m_GameClientView.SetPlayBet(m_lPlayBet[pPlayBetFail->nAnimalIndex], pPlayBetFail->nAnimalIndex);
		m_GameClientView.SetPlayAllBet(m_lPlayAllBet[pPlayBetFail->nAnimalIndex], pPlayBetFail->nAnimalIndex);
		m_GameClientView.m_BetWnd.SetPlayAllBet(m_lPlayAllBet[pPlayBetFail->nAnimalIndex], pPlayBetFail->nAnimalIndex);
	}

	return true;
}

//�����ע
bool CGameClientEngine::OnSubBetClear(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_BetClear));
	if (wDataSize!=sizeof(CMD_S_BetClear)) return false;

	//��Ϣ����
	CMD_S_BetClear * pBetClear=(CMD_S_BetClear *)pBuffer;

	//���Լ��Ŵ���
	if ( GetMeChairID() != pBetClear->wChairID || IsLookonMode() )
	{
		//��ձ���
		for ( int i = 0; i < ANIMAL_MAX; ++i )
		{
			if ( pBetClear->lPlayBet[i] > 0 )
			{
				m_lPlayAllBet[i] -= pBetClear->lPlayBet[i];

				//���ý���
				m_GameClientView.SetPlayAllBet(m_lPlayAllBet[i], i);
				m_GameClientView.m_BetWnd.SetPlayAllBet(m_lPlayAllBet[i], i);
			}
		}
	}
	return true;
}

//��ʱ��
void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	CGameFrameEngine::OnTimer(nIDEvent);
}

//���³���
LRESULT CGameClientEngine::OnMessageUpDateChip(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

//�һ�������Ϣ
LRESULT CGameClientEngine::OnMessageChip( WPARAM wParam, LPARAM lParam )
{
	//ת������
	LONGLONG lPlayChip = *((LONGLONG*)wParam);

	//�ж�����
	if ( lPlayChip * m_lCellScore > m_lPlayScore )
		return 0;

	//���ñ���
	m_lPlayChip = lPlayChip;

	//���ý���
	m_GameClientView.SetPlayChip(m_lPlayChip);

	//������Ϣ
	CMD_C_Chip CMDChip;
	CMDChip.lChip = m_lPlayChip;
	SendSocketData(SUB_C_EXCHANGE_CHIP, &CMDChip, sizeof(CMDChip));

	//�Ѿ��һ�����
	m_lHasInChip = TRUE;

	//���ý���
	m_GameClientView.SetFocus();

	return 0;
}

//��ע��Ϣ
LRESULT CGameClientEngine::OnMessagePlayBet(WPARAM wParam, LPARAM lParam)
{
	//�ж���Ϣ
	if ( wParam < 0 || wParam >= ANIMAL_MAX )
		return 0;

	if ( m_lPlayChip < m_nCurrentNote )
		return 0;

	if ( GetGameStatus() != GAME_SCENE_BET  )
		return 0;

	//�������
	int nAnimalIndex = (int)wParam;

	//�жϳ���
	if ( (m_lAreaLimitScore != 0 && m_lPlayAllBet[nAnimalIndex] + m_nCurrentNote > m_lAreaLimitScore)
		|| (m_lPlayLimitScore != 0 && m_lPlayBet[nAnimalIndex] + m_nCurrentNote > m_lPlayLimitScore) )
	{
		return 0;
	}

	//���ñ���
	m_lPlayChip -= m_nCurrentNote;
	m_lPlayBet[nAnimalIndex] += m_nCurrentNote;
	m_lPlayAllBet[nAnimalIndex] += m_nCurrentNote;

	//���ý���
	m_GameClientView.SetPlayChip(m_lPlayChip);
	m_GameClientView.SetPlayBet(m_lPlayBet[nAnimalIndex], nAnimalIndex);
	m_GameClientView.SetPlayAllBet(m_lPlayAllBet[nAnimalIndex], nAnimalIndex);
	m_GameClientView.m_BetWnd.SetPlayAllBet(m_lPlayAllBet[nAnimalIndex], nAnimalIndex);

	//������Ϣ
	CMD_C_PlayBet CMDPlayBet;
	CMDPlayBet.lBetChip = m_nCurrentNote;
	CMDPlayBet.nAnimalIndex = (int)wParam;
	SendSocketData(SUB_C_PLAY_BET, &CMDPlayBet, sizeof(CMDPlayBet));

	if ( CGlobalUnits::GetInstance()->m_bAllowSound && !CGlobalUnits::GetInstance()->m_bMuteStatuts )
	{
		CD3DMusic::_Object()->Start(TEXT("\\Sound\\ѹ��.mp3"));
	}

	return 0;
}

//��ѹ��Ϣ
LRESULT CGameClientEngine::OnMessageOperationRenewal(WPARAM wParam, LPARAM lParam)
{
	//��һ��
	if ( m_lPlayLastBet[0] == -1 )
		return 0;
	
	//�����
	SendSocketData(SUB_C_BET_CLEAR);
	for ( int i = 0; i < ANIMAL_MAX; ++i )
	{
		m_lPlayChip += m_lPlayBet[i];
		m_lPlayAllBet[i] -= m_lPlayBet[i];
		m_lPlayBet[i] = 0;
	}

	//�ж��Ƿ���ע
	LONGLONG lPlayChip = 0;
	for ( int i = 0; i < ANIMAL_MAX; ++i )
	{
		//���ñ���
		lPlayChip += m_lPlayLastBet[i];
	}
	if( lPlayChip > m_lPlayChip )
	{
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("���ĳ��벻�㣬�޷���ѹ������ע��"));
		return 0;
	}

	//������ע
	for ( int i = 0; i < ANIMAL_MAX; ++i )
	{
		//���ñ���
		m_lPlayChip -= m_lPlayLastBet[i];
		m_lPlayBet[i] += m_lPlayLastBet[i];
		m_lPlayAllBet[i] += m_lPlayLastBet[i];

		//������Ϣ
		CMD_C_PlayBet CMDPlayBet;
		CMDPlayBet.lBetChip = m_lPlayLastBet[i];
		CMDPlayBet.nAnimalIndex = i;
		SendSocketData(SUB_C_PLAY_BET, &CMDPlayBet, sizeof(CMDPlayBet));
	}

	//���ý���
	m_GameClientView.SetPlayChip(m_lPlayChip);
	m_GameClientView.SetPlayBet(m_lPlayBet);
	m_GameClientView.SetPlayAllBet(m_lPlayAllBet);
	m_GameClientView.m_BetWnd.SetPlayAllBet(m_lPlayAllBet);

	if ( CGlobalUnits::GetInstance()->m_bAllowSound && !CGlobalUnits::GetInstance()->m_bMuteStatuts )
	{
		CD3DMusic::_Object()->Start(TEXT("\\Sound\\ѹ��.mp3"));
	}

	return 0;
}

//ȡ����Ϣ
LRESULT CGameClientEngine::OnMessageOperationCancel(WPARAM wParam, LPARAM lParam)
{
	//������Ϣ
	SendSocketData(SUB_C_BET_CLEAR);

	//��ձ���
	for ( int i = 0; i < ANIMAL_MAX; ++i )
	{
		m_lPlayChip += m_lPlayBet[i];
		m_lPlayAllBet[i] -= m_lPlayBet[i];
		m_lPlayBet[i] = 0;

		//���ý���
		m_GameClientView.SetPlayChip(m_lPlayChip);
		m_GameClientView.SetPlayBet(m_lPlayBet[i], i);
		m_GameClientView.SetPlayAllBet(m_lPlayAllBet[i], i);
		m_GameClientView.m_BetWnd.SetPlayAllBet(m_lPlayAllBet[i], i);
	}

	return 0;
}

//�л���Ϣ
LRESULT CGameClientEngine::OnMessageOperationSwitch(WPARAM wParam, LPARAM lParam)
{
	//�л���λֵ
	int nNote[] = { BET_NUMBER, BET_NUMBER*10, BET_NUMBER*100 };
	for ( int i = 0; i < CountArray(nNote); ++i )
	{
		if ( m_nUnitNote == nNote[i] )
		{
			m_nUnitNote = nNote[(i + 1)%CountArray(nNote)];
			m_nCurrentNote = m_nUnitNote;
			break;
		}
	}

	//���ý���
	m_GameClientView.SetCurrentNote(m_nCurrentNote);

	return 0;
}
//����
bool CGameClientEngine::OnSubAdminControl(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_CommandResult));
	if(wDataSize!=sizeof(CMD_S_CommandResult)) return false;

	m_GameClientView.m_pClientControlDlg->ReqResult(pBuffer);
	return true;
}

//����
LRESULT CGameClientEngine::OnAdminControl( WPARAM wParam, LPARAM lParam )
{
	
	SendSocketData(SUB_C_AMDIN_COMMAND,(CMD_C_AdminReq*)wParam,sizeof(CMD_C_AdminReq));
	
	return true;
}



