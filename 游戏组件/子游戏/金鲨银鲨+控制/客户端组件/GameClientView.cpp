#include "StdAfx.h"
#include "GameLogic.h"
#include "GameClient.h"
#include "GameClientView.h"
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////////////

//�滭��ʶ
#define DRN_LEFT								0							//���	
#define DRN_RIGHT								1							//�ұ�
#define DRN_TOP									2							//�ϱ�
#define DRN_BOTTOM								3							//�±�

//��ע��ť
#define IDC_ANIMAL_LION							100							//ʨ��
#define IDC_ANIMAL_PANDA						101							//��è
#define IDC_ANIMAL_MONKEY						102							//����
#define IDC_ANIMAL_RABBIT						103							//����
#define IDC_ANIMAL_EAGLE						104							//��ӥ
#define IDC_ANIMAL_PEACOCK						105							//��ȸ
#define IDC_ANIMAL_PIGEON						106							//����
#define IDC_ANIMAL_SWALLOW						107							//����
#define IDC_ANIMAL_SLIVER_SHARK					108							//����
#define IDC_ANIMAL_GOLD_SHARK					109							//����
#define IDC_ANIMAL_BIRD							110							//����
#define IDC_ANIMAL_BEAST						111							//����
#define IDC_ADMIN								212							//��ť��ʶ

#define IDC_OPERATION_RENEWAL					121							//��ѹ
#define IDC_OPERATION_CANCEL					122							//ȡ��
#define IDC_OPERATION_SWITCH					123							//�л�

//��ʱ��
#define IDI_TIME_SECOND							151							//�ڶ�Ȧ��ʱ��
//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewD3D)
	
	ON_BN_CLICKED(IDC_ANIMAL_LION, OnBnClickedAnimalLion)
	ON_BN_CLICKED(IDC_ANIMAL_PANDA, OnBnClickedAnimalPanda)
	ON_BN_CLICKED(IDC_ANIMAL_MONKEY, OnBnClickedAnimalMonkey)
	ON_BN_CLICKED(IDC_ANIMAL_RABBIT, OnBnClickedAnimalRabbit)
	ON_BN_CLICKED(IDC_ANIMAL_EAGLE, OnBnClickedAnimalEagle)
	ON_BN_CLICKED(IDC_ANIMAL_PEACOCK, OnBnClickedAnimalPeacock)
	ON_BN_CLICKED(IDC_ANIMAL_PIGEON, OnBnClickedAnimalPigeon)
	ON_BN_CLICKED(IDC_ANIMAL_SWALLOW, OnBnClickedAnimalSwallow)
	ON_BN_CLICKED(IDC_ANIMAL_SLIVER_SHARK, OnBnClickedAnimalSliverShark)
	ON_BN_CLICKED(IDC_ANIMAL_GOLD_SHARK, OnBnClickedAnimalGoldShark)
	ON_BN_CLICKED(IDC_ANIMAL_BIRD, OnBnClickedAnimalBird)
	ON_BN_CLICKED(IDC_ANIMAL_BEAST, OnBnClickedAnimalBeast)

	ON_BN_CLICKED(IDC_OPERATION_RENEWAL, OnBnClickedOperationRenewal)
	ON_BN_CLICKED(IDC_OPERATION_CANCEL, OnBnClickedOperationCancel)
	ON_BN_CLICKED(IDC_OPERATION_SWITCH, OnBnClickedOperationSwitch)
	
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_SETCURSOR()

	ON_WM_KILLFOCUS()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��Ϸ����
	m_nCurrentNote = 0;
	ZeroMemory(m_nAnimalMultiple, sizeof(m_nAnimalMultiple));
	m_cbGameStatus = GAME_SCENE_FREE;

	//��Ϸ��¼
	m_ArrayTurnTableRecord.RemoveAll();
	m_bShowTurnTableRecord = FALSE;

	//��ұ���
	m_wMineChairID = INVALID_CHAIR;
	m_lPlayChip = 0;
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));
	ZeroMemory(m_lPlayAllBet, sizeof(m_lPlayAllBet));
	ZeroMemory(m_bKeyDown, sizeof(m_bKeyDown));
	ZeroMemory(m_bKeySend, sizeof(m_bKeySend));

	//�÷ֱ���
	ZeroMemory(m_lPlayWin, sizeof(m_lPlayWin));
	m_lPlayPrizes = 0;
	m_lPlayShowWin = 0;
	m_lPlayShowPrizes = 0;
	m_PlayPrizesLapse.Initialization();

	//ת�̶���
	m_bTurnTwoTime = FALSE;
	ZeroMemory(m_nTurnTarget, sizeof(m_nTurnTarget));
	m_nPrizesMultiple = 0;

	m_nBrightCount = 0;
	m_nTurnTableCurrent = 0;
	m_nTurnTableTarget = 0;
	m_nTurnTableCount = 0;
	m_nTurnTableDelay = 0;
	m_nTurnCount = 0;
	m_nTurnTransit = 0;
	m_nTurnTableLaps = 0;
	m_bTurnTable = FALSE;

	//Ӯ���� 
	m_nWinShow = FALSE;
	m_nWinIndex = INT_MAX;
	m_nWinFrame = 0;

	m_nPrizesShow = FALSE;
	m_nGoldSilverShow = FALSE;

	//���綯��
	m_nLaserShow = FALSE;
	m_nLaserDown = FALSE;
	m_nLaserY = 0;

	//��������
	for ( int i = 0; i < TURAN_TABLE_MAX; ++i )
	{
		m_AnimalsDraw[i].bBright = false;
		m_AnimalsDraw[i].bflicker = false;
		m_AnimalsDraw[i].rectDraw.SetRect(0, 0, 0, 0);
	}

	m_BetWnd.SetPlayAllBet(m_lPlayAllBet);
	m_pClientControlDlg = NULL;
}

//��������
CGameClientView::~CGameClientView()
{
	if( m_pClientControlDlg )
	{
		delete m_pClientControlDlg;
		m_pClientControlDlg = NULL;
	}

	if( m_hInst )
	{
		FreeLibrary(m_hInst);
		m_hInst = NULL;
	}
	CD3DMusic::Delete();
}

//���ý���
VOID CGameClientView::ResetGameView()
{
	//��Ϸ����
	m_nCurrentNote = 0;
	ZeroMemory(m_nAnimalMultiple, sizeof(m_nAnimalMultiple));
	m_cbGameStatus = GAME_SCENE_FREE;

	//��Ϸ��¼
	m_ArrayTurnTableRecord.RemoveAll();
	m_bShowTurnTableRecord = FALSE;

	//��ұ���
	m_wMineChairID = INVALID_CHAIR;
	m_lPlayChip = 0;
	ZeroMemory(m_lPlayBet, sizeof(m_lPlayBet));
	ZeroMemory(m_lPlayAllBet, sizeof(m_lPlayAllBet));
	ZeroMemory(m_bKeyDown, sizeof(m_bKeyDown));
	ZeroMemory(m_bKeySend, sizeof(m_bKeySend));

	//�÷ֱ���
	ZeroMemory(m_lPlayWin, sizeof(m_lPlayWin));
	m_lPlayPrizes = 0;
	m_lPlayShowWin = 0;
	m_lPlayShowPrizes = 0;

	//ת�̶���
	m_bTurnTwoTime = FALSE;
	ZeroMemory(m_nTurnTarget, sizeof(m_nTurnTarget));
	m_nPrizesMultiple = 0;

	m_nBrightCount = 0;
	m_nTurnTableCurrent = 0;
	m_nTurnTableTarget = 0;
	m_nTurnTableCount = 0;
	m_nTurnTableDelay = 0;
	m_nTurnCount = 0;
	m_nTurnTransit = 0;
	m_nTurnTableLaps = 0;
	m_bTurnTable = FALSE;

	//Ӯ���� 
	m_nWinShow = FALSE;
	m_nWinIndex = INT_MAX;
	m_nWinFrame = 0;

	m_nPrizesShow = FALSE;
	m_nGoldSilverShow = FALSE;

	//���綯��
	m_nLaserShow = FALSE;
	m_nLaserDown = FALSE;
	m_nLaserY = 0;

	//��������
	for ( int i = 0; i < TURAN_TABLE_MAX; ++i )
	{
		m_AnimalsDraw[i].bBright = false;
		m_AnimalsDraw[i].bflicker = false;
		m_AnimalsDraw[i].rectDraw.SetRect(0, 0, 0, 0);
	}

	m_BetWnd.SetPlayAllBet(m_lPlayAllBet);
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//���ⰴť
	if ( !m_bInitD3D )
		return;

	//��׼λ��
	m_ptBenchmark.SetPoint( nWidth/2 - m_ImageMainBack.GetWidth()/2, nHeight/2 - m_ImageMainBack.GetHeight()/2 );
	m_sizeStage.SetSize( m_ImageMainBack.GetWidth(), m_ImageMainBack.GetHeight() );

	m_WindowTip.SetBenchmark(m_ptBenchmark);
	m_WindowTip.SetStage(m_sizeStage);

	//����λ��
	CPoint ptBegin;
	CPoint ptOffSet;
	ptBegin.SetPoint(m_ptBenchmark.x + 1, m_ptBenchmark.y + 1);
	ptOffSet.SetPoint(0, 0);
	for ( int i = 0; i <= 8; i++ )
	{
		m_AnimalsDraw[i].rectDraw.SetRect( ptBegin.x + ptOffSet.x, ptBegin.y + ptOffSet.y, ptBegin.x + ptOffSet.x + 82, ptBegin.y + ptOffSet.y + 70);
		ptOffSet.x += 84;
	}

	ptBegin.SetPoint(m_ptBenchmark.x + 673, m_ptBenchmark.y + 73);
	ptOffSet.SetPoint(0, 0);
	for ( int i = 9; i <= 14; i++ )
	{
		m_AnimalsDraw[i].rectDraw.SetRect( ptBegin.x + ptOffSet.x, ptBegin.y + ptOffSet.y, ptBegin.x + ptOffSet.x + 82, ptBegin.y + ptOffSet.y + 70);
		ptOffSet.y += 72;
	}

	ptBegin.SetPoint(m_ptBenchmark.x + 589, m_ptBenchmark.y + 433);
	ptOffSet.SetPoint(0, 0);
	for ( int i = 15; i <= 22; i++ )
	{
		m_AnimalsDraw[i].rectDraw.SetRect( ptBegin.x + ptOffSet.x, ptBegin.y + ptOffSet.y, ptBegin.x + ptOffSet.x + 82, ptBegin.y + ptOffSet.y + 70);
		ptOffSet.x -= 84;
	}

	ptBegin.SetPoint(m_ptBenchmark.x + 1, m_ptBenchmark.y + 361);
	ptOffSet.SetPoint(0, 0);
	for ( int i = 23; i <= 27; i++ )
	{
		m_AnimalsDraw[i].rectDraw.SetRect( ptBegin.x + ptOffSet.x, ptBegin.y + ptOffSet.y, ptBegin.x + ptOffSet.x + 82, ptBegin.y + ptOffSet.y + 70);
		ptOffSet.y -= 72;
	}

	//��ע��ť
	m_btSelectBet[ANIMAL_LION].SetWindowPos(m_ptBenchmark.x + 390, m_ptBenchmark.y + 648, 0, 0, SWP_NOSIZE);
	m_btSelectBet[ANIMAL_PANDA].SetWindowPos(m_ptBenchmark.x + 480, m_ptBenchmark.y + 648, 0, 0, SWP_NOSIZE);
	m_btSelectBet[ANIMAL_MONKEY].SetWindowPos(m_ptBenchmark.x + 570, m_ptBenchmark.y + 648, 0, 0, SWP_NOSIZE);
	m_btSelectBet[ANIMAL_RABBIT].SetWindowPos(m_ptBenchmark.x + 660, m_ptBenchmark.y + 648, 0, 0, SWP_NOSIZE);
	m_btSelectBet[ANIMAL_EAGLE].SetWindowPos(m_ptBenchmark.x + 300, m_ptBenchmark.y + 648, 0, 0, SWP_NOSIZE);
	m_btSelectBet[ANIMAL_PEACOCK].SetWindowPos(m_ptBenchmark.x + 208, m_ptBenchmark.y + 648, 0, 0, SWP_NOSIZE);
	m_btSelectBet[ANIMAL_PIGEON].SetWindowPos(m_ptBenchmark.x + 115, m_ptBenchmark.y + 648 , 0, 0, SWP_NOSIZE);
	m_btSelectBet[ANIMAL_SWALLOW].SetWindowPos(m_ptBenchmark.x + 20, m_ptBenchmark.y + 648, 0, 0, SWP_NOSIZE);
	m_btSelectBet[ANIMAL_SLIVER_SHARK].SetWindowPos(m_ptBenchmark.x + 393, m_ptBenchmark.y + 534, 0, 0, SWP_NOSIZE);
	m_btSelectBet[ANIMAL_GOLD_SHARK].SetWindowPos(m_ptBenchmark.x + 301, m_ptBenchmark.y + 534, 0, 0, SWP_NOSIZE);
	m_btSelectBet[ANIMAL_BIRD].SetWindowPos(m_ptBenchmark.x + 209, m_ptBenchmark.y + 534, 0, 0, SWP_NOSIZE);
	m_btSelectBet[ANIMAL_BEAST].SetWindowPos(m_ptBenchmark.x + 483, m_ptBenchmark.y + 534, 0, 0, SWP_NOSIZE);

	m_btOperationRenewal.SetWindowPos(m_ptBenchmark.x + 573, m_ptBenchmark.y + 543, 0, 0, SWP_NOSIZE);
	m_btOperationCancel.SetWindowPos(m_ptBenchmark.x + 573 + 55, m_ptBenchmark.y + 543, 0, 0, SWP_NOSIZE);
	m_btOperationSwitch.SetWindowPos(m_ptBenchmark.x + 573 + (55)*2, m_ptBenchmark.y + 543, 0, 0, SWP_NOSIZE);

	//���ﱶ��λ��
	ZeroMemory(m_ptAnimalMultiple, sizeof(m_ptAnimalMultiple));
	m_ptAnimalMultiple[ANIMAL_LION].SetPoint( m_ptBenchmark.x + 452, m_ptBenchmark.y + 597 );
	m_ptAnimalMultiple[ANIMAL_PANDA].SetPoint( m_ptBenchmark.x + 543, m_ptBenchmark.y + 597 );
	m_ptAnimalMultiple[ANIMAL_MONKEY].SetPoint( m_ptBenchmark.x + 634, m_ptBenchmark.y + 597 );
	m_ptAnimalMultiple[ANIMAL_RABBIT].SetPoint( m_ptBenchmark.x + 727, m_ptBenchmark.y + 597 );
	m_ptAnimalMultiple[ANIMAL_EAGLE].SetPoint( m_ptBenchmark.x + 360, m_ptBenchmark.y + 597 );
	m_ptAnimalMultiple[ANIMAL_PEACOCK].SetPoint( m_ptBenchmark.x + 270, m_ptBenchmark.y + 597 );
	m_ptAnimalMultiple[ANIMAL_PIGEON].SetPoint( m_ptBenchmark.x + 178, m_ptBenchmark.y + 597 );
	m_ptAnimalMultiple[ANIMAL_SWALLOW].SetPoint( m_ptBenchmark.x + 84, m_ptBenchmark.y + 597 );

	m_ptPlayBet[ANIMAL_LION].SetPoint( m_ptBenchmark.x + 462, m_ptBenchmark.y + 621 );
	m_ptPlayBet[ANIMAL_PANDA].SetPoint( m_ptBenchmark.x + 553, m_ptBenchmark.y + 621 );
	m_ptPlayBet[ANIMAL_MONKEY].SetPoint( m_ptBenchmark.x + 644, m_ptBenchmark.y + 621 );
	m_ptPlayBet[ANIMAL_RABBIT].SetPoint( m_ptBenchmark.x + 736, m_ptBenchmark.y + 621 );
	m_ptPlayBet[ANIMAL_EAGLE].SetPoint( m_ptBenchmark.x + 370, m_ptBenchmark.y + 621 );
	m_ptPlayBet[ANIMAL_PEACOCK].SetPoint( m_ptBenchmark.x + 280, m_ptBenchmark.y + 621 );
	m_ptPlayBet[ANIMAL_PIGEON].SetPoint( m_ptBenchmark.x + 188, m_ptBenchmark.y + 621 );
	m_ptPlayBet[ANIMAL_SWALLOW].SetPoint( m_ptBenchmark.x + 94, m_ptBenchmark.y + 621 );

	m_ptPlayBet[ANIMAL_SLIVER_SHARK].SetPoint( m_ptBenchmark.x + 460, m_ptBenchmark.y + 511 );
	m_ptPlayBet[ANIMAL_GOLD_SHARK].SetPoint( m_ptBenchmark.x + 368, m_ptBenchmark.y + 511 );
	m_ptPlayBet[ANIMAL_BIRD].SetPoint( m_ptBenchmark.x + 276, m_ptBenchmark.y + 511 );
	m_ptPlayBet[ANIMAL_BEAST].SetPoint( m_ptBenchmark.x + 549, m_ptBenchmark.y + 511 );

	//����
	m_WindowChip.SetWindowPos(nWidth/2 - 477/2, nHeight/2 - 277/2, 0, 0, SWP_NOSIZE);
	m_WindowOver.SetWindowPos(nWidth/2 - 254/2, m_ptBenchmark.y + 255 - 126/2, 0, 0, SWP_NOSIZE);

	//��
	CRect rect;
	rect.SetRect( m_ptBenchmark.x , m_ptBenchmark.y, m_ptBenchmark.x + 756, m_ptBenchmark.y + 506 );
	m_FishDraw.SetFishPond(rect);

	//���ö�������λ��
	CPoint ptAnimalSite[ANIMAL_MAX];
	for ( int i = 0; i < ANIMAL_MAX; ++i )
	{
		m_btSelectBet[i].GetWindowRect(rect);
		ptAnimalSite[i].SetPoint(rect.left, rect.top);
	}
	m_WindowTip.SetAnimalSite(ptAnimalSite);

	return;
}

//��������
VOID CGameClientView::CartoonMovie()
{
	//�㶯��
	m_FishDraw.OnFishMovie();

	//�ʽ�
	if ( m_cbGameStatus != GAME_SCENE_END )
	{
		if ( m_PlayPrizesLapse.GetLapseCount(150) > 0 )
		{
			m_lPlayShowPrizes = rand()%70000;
		}
	}

	//������Ϣ
	for ( int i = 0; i < ANIMAL_MAX ; ++i )
	{
		if ( m_bKeyDown[i] )
		{
			if ( m_KeyLapse[i].GetLapseCount(500) > 0)
			{
				m_bKeySend[i] = TRUE;
			}
		}
		else
		{
			m_bKeySend[i] = FALSE;
		}

		if ( m_bKeySend[i] && m_KeyLapse[i].GetLapseCount(30) > 0 )
		{
			SendEngineMessage(IDM_PLAY_BET, i, 0);
		}
	}


	//ת�̶���
	if ( m_bTurnTable && m_TurnTableLapse.GetLapseCount(15) > 0 )
	{
		//��ʱ ʱ�����
		if ( m_nTurnTableDelay > 0 )
		{
			m_nTurnTableDelay--;
		}

		//������һλ
		if( m_nTurnTableDelay == 0 )
		{
			//��λ��
			m_nTurnTableCurrent = (m_nTurnTableCurrent + 1)%TURAN_TABLE_MAX;

			//�ƶ�����
			m_nTurnCount++;
			if ( m_nTurnTableCurrent == m_nTurnTableTarget && m_nTurnCount == m_nTurnTransit )
			{
				//CString str;
				//str.Format(TEXT(" %d. %d. %d "), m_nTurnCount, m_nTurnTransit, m_nTurnCount - m_nTurnTransit );
				//AfxMessageBox(str);

				m_bTurnTable = FALSE;

				//������
				m_nBrightCount = 1;

				//���ñ���
				int nTargetAnimal = CGameLogic::GetInstance()->TurnTableAnimal(m_nTurnTableTarget);
				int nTargetAnimalType = CGameLogic::GetInstance()->TurnTableAnimalType(m_nTurnTableTarget);

				//����Ӯ�Ķ���
				if ( nTargetAnimal == ANIMAL_GOLD_SHARK || nTargetAnimal == ANIMAL_SLIVER_SHARK )
				{
					BeginWin(nTargetAnimal);
				}
				else
				{
					BeginWin(nTargetAnimal);
				}
				if ( nTargetAnimalType == ANIMAL_TYPE_BEAST )
					m_FishDraw.SetBackIndex(3);
				else if ( nTargetAnimalType == ANIMAL_TYPE_BIRD )
					m_FishDraw.SetBackIndex(4);
				else if ( nTargetAnimalType == ANIMAL_TYPE_SLIVER )
					m_FishDraw.SetBackIndex(5);
				else if ( nTargetAnimalType == ANIMAL_TYPE_GOLD )
					m_FishDraw.SetBackIndex(6);

				//������˸
				m_WindowTip.SetAnimalFlicker( nTargetAnimal );
				if ( nTargetAnimalType == ANIMAL_TYPE_BEAST )
					m_WindowTip.SetAnimalFlicker( ANIMAL_BEAST );
				if ( nTargetAnimalType == ANIMAL_TYPE_BIRD )
					m_WindowTip.SetAnimalFlicker( ANIMAL_BIRD );

				//��˸��ǰ
				m_AnimalsDraw[m_nTurnTableCurrent].bflicker = true;

				//ת�ڶ�Ȧ
				if ( m_bTurnTwoTime && m_nTurnTableLaps == 0 )
				{
					m_lPlayShowWin += m_lPlayWin[0];
					m_lPlayShowWin += m_lPlayPrizes;
					m_lPlayShowPrizes = m_lPlayPrizes;
				}
				//��һȦֱ�ӽ���
				else if ( m_nTurnTableLaps == 0 )
				{
					m_lPlayShowWin += m_lPlayWin[0];
				}
				//�ڶ�Ȧ���
				else
				{
					m_lPlayShowWin += m_lPlayWin[1];
				}
			}
		}

		//��һȦ�ٶ�
		if ( m_nTurnTableLaps == 0 )
		{
			//ǰ100���Ǽ���
			if ( m_nTurnCount < (m_nTurnTransit - TURAN_TABLE_MAX) )
			{
				//��ʱ
				if ( m_nTurnTableDelay == 0 )
				{
					//���٣���ʱʱ�����
					if ( m_nTurnTableCount > 1 )
						m_nTurnTableCount--;

					//�¼���ʱ
					m_nTurnTableDelay = m_nTurnTableCount;

					//������
					if ( m_nBrightCount < 6 && m_nTurnTableLaps == 0 )
						m_nBrightCount++;
				}
			}
			//��ת��
			else if( m_nTurnCount == (m_nTurnTransit - TURAN_TABLE_MAX) )
			{
				m_nTurnTableCount = 1;
				m_nTurnTableDelay = 1;
			}
			//����
			else
			{
				if ( m_nTurnTableDelay == 0 )
				{
					//���٣���ʱʱ������
					if ( ( m_nTurnTransit - m_nTurnCount )%4 == 0 )
					{
						m_nTurnTableCount++;

						//������
						if ( m_nBrightCount > 1 )
							m_nBrightCount--;
					}

					if ( m_nTurnTransit - m_nTurnCount < TURAN_TABLE_MAX - 15 )
					{
						m_nTurnTableCount++;
					}

					//�¼���ʱ
					m_nTurnTableDelay = m_nTurnTableCount;
				}
			}
		}
		//�ڶ�Ȧ�ٶ�
		else
		{
			if ( m_nTurnTableDelay == 0 )
			{
				m_nTurnTableDelay = 1;
			}
		}
	}

	//Ӯ����
	if ( m_nWinShow )
	{
		if ( m_nWinFrameTime.GetLapseCount(200) > 0 )
		{
			m_nWinFrame = (m_nWinFrame + 1)%5;
		}

		//3����ʱ
		if ( m_nWinContinueTime.GetLapseCount(4000) > 0 )
		{
			if ( m_nWinIndex == ANIMAL_GOLD_SHARK )
			{
				//����ǽ��裬 �����ɲ���ʾ
				BeginWin( ANIMAL_MAX );
				m_nPrizesShow = TRUE;
				m_nPrizesContinueTime.Initialization();
			}
			else if ( m_nWinIndex == ANIMAL_SLIVER_SHARK )
			{
				//�����㣬������һ��
				EndWin();
				BeginTurnTable(false, m_nTurnTarget[1]);
			}
		}
	}

	//�ɲ���ʾ
	if ( m_nPrizesShow )
	{
		//3����ʱ
		if ( m_nPrizesContinueTime.GetLapseCount(9000) > 0 )
		{
			EndWin();
			BeginLaser();
			m_nPrizesShow = FALSE;
		}
	}

	//��������
	if ( m_nGoldSilverShow )
	{
		//3����ʱ
		if ( m_nGoldSilverContinueTime.GetLapseCount(12000) > 0 )
		{
			EndGoldSilver();

			BeginTurnTable(true, m_nTurnTarget[0]);
		}
	}

	//����ת�̶���
	for ( int i = 0; i < TURAN_TABLE_MAX; ++i )
	{
		m_AnimalsDraw[i].bBright = false;
	}

	int nTurnTableIndex = m_nTurnTableCurrent;
	for ( int i = m_nBrightCount; i > 0; i-- )
	{
		m_AnimalsDraw[nTurnTableIndex].bBright = true;
		nTurnTableIndex = (nTurnTableIndex + TURAN_TABLE_MAX - 1)%TURAN_TABLE_MAX;
	}

	//���⶯��
	if ( m_nLaserShow )
	{
		if( m_nLaserDown )
		{
			m_nLaserY += 10;
			if ( m_nLaserY > m_ptBenchmark.y + 504 )
			{
				m_nLaserDown = FALSE;
			}
		}
		else
		{
			m_nLaserY -= 10;
			if ( m_nLaserY < m_ptBenchmark.y - 504 )
			{
				EndLaser();
				BeginTurnTable(false, m_nTurnTarget[1]);
			}
		}
	}


	return;
}

//�����豸
VOID CGameClientView::InitGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)
{
	//��������
	CRect rcCreate(0,0,0,0);
	HINSTANCE hResInstance=AfxGetInstanceHandle();

	m_ImageKeepOut.LoadImage(pD3DDevice, hResInstance, TEXT("KEEP_OUT"), TEXT("MAIN"));
	m_ImageMainBack.LoadImage(pD3DDevice, hResInstance, TEXT("MAIN_BACK"), TEXT("MAIN"));
	m_ImageAnimals.LoadImage(pD3DDevice, hResInstance, TEXT("MAIN_ANIMALS"), TEXT("MAIN"), 5, 2);
	m_ImageAnimalsBack.LoadImage(pD3DDevice, hResInstance, TEXT("MAIN_ANIMALS_BACK"), TEXT("MAIN"), 5, 1);
	m_ImageAnimalFlicker[0].LoadImage(pD3DDevice, hResInstance, TEXT("FLICKER_RED"), TEXT("MAIN"), 2, 1);
	m_ImageAnimalFlicker[1].LoadImage(pD3DDevice, hResInstance, TEXT("FLICKER_GREEN"), TEXT("MAIN"), 2, 1);
	m_ImageAnimalFlicker[2].LoadImage(pD3DDevice, hResInstance, TEXT("FLICKER_YELLOW"), TEXT("MAIN"), 2, 1);
	m_ImageNumberGreen.LoadImage(pD3DDevice, hResInstance, TEXT("NUMBER_GREEN"), TEXT("MAIN"));
	m_ImageNumberRed.LoadImage(pD3DDevice, hResInstance, TEXT("NUMBER_RED"), TEXT("MAIN"));
	m_ImageNumberYellow.LoadImage(pD3DDevice, hResInstance, TEXT("NUMBER_YELLOW"), TEXT("MAIN"));
	m_ImageNumberBlue.LoadImage(pD3DDevice, hResInstance, TEXT("NUMBER_BLUE"), TEXT("MAIN"));
	m_ImageTimeBack.LoadImage(pD3DDevice, hResInstance, TEXT("TIME_BACK"), TEXT("MAIN"));
	m_ImageTimeNumber.LoadImage(pD3DDevice, hResInstance, TEXT("TIME_NUMBER"), TEXT("MAIN"));
	m_ImageAnimalRecord.LoadImage(pD3DDevice, hResInstance, TEXT("HISTORY_ANIMAL"), TEXT("MAIN"), 10, 1 );

	m_ImageAnimalsWin[ANIMAL_LION].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_LION"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWin[ANIMAL_PANDA].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_PANDA"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWin[ANIMAL_MONKEY].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_MONKEY"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWin[ANIMAL_RABBIT].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_RABBIT"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWin[ANIMAL_EAGLE].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_EAGLE"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWin[ANIMAL_PEACOCK].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_PEACOCK"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWin[ANIMAL_PIGEON].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_PIGEON"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWin[ANIMAL_SWALLOW].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_SWALLOW"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWin[ANIMAL_SLIVER_SHARK].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_SLIVER_SHARK"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWin[ANIMAL_GOLD_SHARK].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_GOLD_SHARK"), TEXT("WIN"), 5, 1);
	m_ImageWinPrizes.LoadImage(pD3DDevice, hResInstance, TEXT("WIN_PRIZES"), TEXT("WIN"), 5, 1);
	m_ImageWinBright.LoadImage(pD3DDevice, hResInstance, TEXT("WIN_BRIGHT"), TEXT("WIN"), 5, 1);
	m_ImagePrizesNumber.LoadImage(pD3DDevice, hResInstance, TEXT("PRIZES_NUMBER"), TEXT("WIN") );
	
	m_ImageAnimalsWinS[ANIMAL_LION].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_LION_S"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWinS[ANIMAL_PANDA].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_PANDA_S"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWinS[ANIMAL_MONKEY].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_MONKEY_S"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWinS[ANIMAL_RABBIT].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_RABBIT_S"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWinS[ANIMAL_EAGLE].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_EAGLE_S"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWinS[ANIMAL_PEACOCK].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_PEACOCK_S"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWinS[ANIMAL_PIGEON].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_PIGEON_S"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWinS[ANIMAL_SWALLOW].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_SWALLOW_S"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWinS[ANIMAL_SLIVER_SHARK].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_SLIVER_SHARK_S"), TEXT("WIN"), 5, 1);
	m_ImageAnimalsWinS[ANIMAL_GOLD_SHARK].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_GOLD_SHARK_S"), TEXT("WIN"), 5, 1);

	m_ImageLaser[0].LoadImage(pD3DDevice, hResInstance, TEXT("LASER_I"), TEXT("MAIN") );
	m_ImageLaser[1].LoadImage(pD3DDevice, hResInstance, TEXT("LASER_II"), TEXT("MAIN") );

	//��ť����
	m_btSelectBet[ANIMAL_LION].ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_ANIMAL_LION,&m_VirtualEngine,NULL);
	m_btSelectBet[ANIMAL_PANDA].ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_ANIMAL_PANDA,&m_VirtualEngine,NULL);
	m_btSelectBet[ANIMAL_MONKEY].ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_ANIMAL_MONKEY,&m_VirtualEngine,NULL);
	m_btSelectBet[ANIMAL_RABBIT].ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_ANIMAL_RABBIT,&m_VirtualEngine,NULL);
	m_btSelectBet[ANIMAL_EAGLE].ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_ANIMAL_EAGLE,&m_VirtualEngine,NULL);
	m_btSelectBet[ANIMAL_PEACOCK].ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_ANIMAL_PEACOCK,&m_VirtualEngine,NULL);
	m_btSelectBet[ANIMAL_PIGEON].ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_ANIMAL_PIGEON,&m_VirtualEngine,NULL);
	m_btSelectBet[ANIMAL_SWALLOW].ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_ANIMAL_SWALLOW,&m_VirtualEngine,NULL);
	m_btSelectBet[ANIMAL_SLIVER_SHARK].ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_ANIMAL_SLIVER_SHARK,&m_VirtualEngine,NULL);
	m_btSelectBet[ANIMAL_GOLD_SHARK].ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_ANIMAL_GOLD_SHARK,&m_VirtualEngine,NULL);
	m_btSelectBet[ANIMAL_BIRD].ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_ANIMAL_BIRD,&m_VirtualEngine,NULL);
	m_btSelectBet[ANIMAL_BEAST].ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_ANIMAL_BEAST,&m_VirtualEngine,NULL);

	m_btOperationRenewal.ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_OPERATION_RENEWAL,&m_VirtualEngine,NULL);
	m_btOperationCancel.ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_OPERATION_CANCEL,&m_VirtualEngine,NULL);
	m_btOperationSwitch.ActiveWindow(rcCreate,WS_VISIBLE|WS_DISABLED,IDC_OPERATION_SWITCH,&m_VirtualEngine,NULL);

	m_btSelectBet[ANIMAL_LION].SetButtonImage(pD3DDevice,TEXT("BT_ANIMAL_LION"),TEXT("BTPNG"),hResInstance);
	m_btSelectBet[ANIMAL_PANDA].SetButtonImage(pD3DDevice,TEXT("BT_ANIMAL_PANDA"),TEXT("BTPNG"),hResInstance);
	m_btSelectBet[ANIMAL_MONKEY].SetButtonImage(pD3DDevice,TEXT("BT_ANIMAL_MONKEY"),TEXT("BTPNG"),hResInstance);
	m_btSelectBet[ANIMAL_RABBIT].SetButtonImage(pD3DDevice,TEXT("BT_ANIMAL_RABBIT"),TEXT("BTPNG"),hResInstance);
	m_btSelectBet[ANIMAL_EAGLE].SetButtonImage(pD3DDevice,TEXT("BT_ANIMAL_EAGLE"),TEXT("BTPNG"),hResInstance);
	m_btSelectBet[ANIMAL_PEACOCK].SetButtonImage(pD3DDevice,TEXT("BT_ANIMAL_PEACOCK"),TEXT("BTPNG"),hResInstance);
	m_btSelectBet[ANIMAL_PIGEON].SetButtonImage(pD3DDevice,TEXT("BT_ANIMAL_PIGEON"),TEXT("BTPNG"),hResInstance);
	m_btSelectBet[ANIMAL_SWALLOW].SetButtonImage(pD3DDevice,TEXT("BT_ANIMAL_SWALLOW"),TEXT("BTPNG"),hResInstance);
	m_btSelectBet[ANIMAL_SLIVER_SHARK].SetButtonImage(pD3DDevice,TEXT("BT_ANIMAL_SLIVER_SHARK"),TEXT("BTPNG"),hResInstance);
	m_btSelectBet[ANIMAL_GOLD_SHARK].SetButtonImage(pD3DDevice,TEXT("BT_ANIMAL_GOLD_SHARK"),TEXT("BTPNG"),hResInstance);
	m_btSelectBet[ANIMAL_BIRD].SetButtonImage(pD3DDevice,TEXT("BT_ANIMAL_BIRD"),TEXT("BTPNG"),hResInstance);
	m_btSelectBet[ANIMAL_BEAST].SetButtonImage(pD3DDevice,TEXT("BT_ANIMAL_BEAST"),TEXT("BTPNG"),hResInstance);

	m_btOperationRenewal.SetButtonImage(pD3DDevice,TEXT("BT_OPERATION_RENEWAL"),TEXT("BTPNG"),hResInstance);
	m_btOperationCancel.SetButtonImage(pD3DDevice,TEXT("BT_OPERATION_CANCEL"),TEXT("BTPNG"),hResInstance);
	m_btOperationSwitch.SetButtonImage(pD3DDevice,TEXT("BT_OPERATION_SWITCH"),TEXT("BTPNG"),hResInstance);

	m_ImageSharkWin[0].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_SHARK_0"), TEXT("WIN"), 5, 1);
	m_ImageSharkWin[1].LoadImage(pD3DDevice, hResInstance, TEXT("WIN_SHARK_1"), TEXT("WIN"), 5, 1);

	//������
	m_FishDraw.OnFishCreate(pD3DDevice);

	//��������
	m_WindowChip.ActiveWindow(rcCreate, 0, 1001, &m_VirtualEngine, NULL);
	m_WindowOver.ActiveWindow(rcCreate, 0, 1002, &m_VirtualEngine, NULL);

	//���㴰�ڣ���Զ��󴴽�
	m_WindowTip.ActiveWindow(rcCreate, 0, 999, &m_VirtualEngine, NULL);
	m_WindowTip.ShowWindow(true);
	//���ư�ť
	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);
	//����
	m_hInst = NULL;
	m_pClientControlDlg = NULL;
	m_hInst = LoadLibrary(TEXT("SharkBattleClientControl.dll"));
	if ( m_hInst )
	{
		typedef void * (*CREATE)(CWnd* pParentWnd); 
		CREATE ClientControl = (CREATE)GetProcAddress(m_hInst,"CreateClientControl"); 
		if ( ClientControl )
		{
			m_pClientControlDlg = static_cast<IClientControlDlg*>(ClientControl(this));
			m_pClientControlDlg->ShowWindow( SW_HIDE );
		}
	}
	//���ö���ͼƬ
	CD3DTexture* pImageAnimal[ANIMAL_MAX] = 
	{
		m_btSelectBet[ANIMAL_LION].GetD3DTexture(),m_btSelectBet[ANIMAL_PANDA].GetD3DTexture(),m_btSelectBet[ANIMAL_MONKEY].GetD3DTexture(),
		m_btSelectBet[ANIMAL_RABBIT].GetD3DTexture(),m_btSelectBet[ANIMAL_EAGLE].GetD3DTexture(),m_btSelectBet[ANIMAL_PEACOCK].GetD3DTexture(),
		m_btSelectBet[ANIMAL_PIGEON].GetD3DTexture(),m_btSelectBet[ANIMAL_SWALLOW].GetD3DTexture(),m_btSelectBet[ANIMAL_SLIVER_SHARK].GetD3DTexture(),
		m_btSelectBet[ANIMAL_GOLD_SHARK].GetD3DTexture(),m_btSelectBet[ANIMAL_BIRD].GetD3DTexture(),m_btSelectBet[ANIMAL_BEAST].GetD3DTexture()
	};
	m_WindowTip.SetImageAnimal(pImageAnimal);

	//��������
	m_BetWnd.Create(TEXT(""), TEXT(""), WS_VISIBLE|WS_CHILD, CRect(0,0,0,0), this, 1023);

	//�������
	//IGameFrameControl * pIGameFrameControl=(IGameFrameControl *)CGlobalUnits::GetInstance()->QueryGlobalModule(MODULE_GAME_FRAME_CONTROL,IID_IGameFrameControl,VER_IGameFrameControl);
	//if(pIGameFrameControl!=NULL) pIGameFrameControl->SetCustomPanel(&m_BetWnd);

	//������Ⱦ
	StartRenderThread();

	return;
}

//�滭����
VOID CGameClientView::DrawGameView(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)
{
	//�滭��
	m_FishDraw.OnFishDraw(pD3DDevice, nWidth, nHeight);

	//ת��
	static int nMainBackAlpha = 0;
	if ( m_FishDraw.GetBackIndex() != 0 )
	{
		nMainBackAlpha += 20;
		if ( nMainBackAlpha > 255 )
			nMainBackAlpha = 255;
		
		m_ImageMainBack.DrawImage(pD3DDevice, m_ptBenchmark.x, m_ptBenchmark.y, 756, 504, 0, 0, (BYTE)nMainBackAlpha );

		//ת�̶���
		for ( int i = 0; i < TURAN_TABLE_MAX; ++i )
		{
			//����
			if ( m_AnimalsDraw[i].bflicker )
			{
				int nAnimalType = CGameLogic::GetInstance()->TurnTableAnimalType(i);
				if( nAnimalType == ANIMAL_TYPE_SLIVER || nAnimalType == ANIMAL_TYPE_GOLD )
					nAnimalType = 2;
				else if ( nAnimalType == ANIMAL_TYPE_BEAST )
					nAnimalType = 0;
				else if ( nAnimalType == ANIMAL_TYPE_BIRD )
					nAnimalType = 1;

				m_ImageAnimalFlicker[nAnimalType].DrawImage(pD3DDevice, m_AnimalsDraw[i].rectDraw.left + 1, m_AnimalsDraw[i].rectDraw.top + 2, (GetTickCount()/150)%2);

				//С����
				int nWinSIndex = CGameLogic::GetInstance()->TurnTableAnimal(i);
				CPoint ptAnimal( m_AnimalsDraw[i].rectDraw.left + m_AnimalsDraw[i].rectDraw.Width()/2 - m_ImageAnimalsWinS[nWinSIndex].GetWidth()/2, m_AnimalsDraw[i].rectDraw.top + m_AnimalsDraw[i].rectDraw.Height()/2 - m_ImageAnimalsWinS[nWinSIndex].GetHeight()/2);
				m_ImageAnimalsWinS[nWinSIndex].DrawImage(pD3DDevice, ptAnimal.x, ptAnimal.y, (GetTickCount()/120)%5);
			}

			else
			{
				//�����л�
				if ( m_AnimalsDraw[i].bBright )
				{
					int nAnimalType = CGameLogic::GetInstance()->TurnTableAnimalType(i);
					if( nAnimalType == ANIMAL_TYPE_GOLD )
						nAnimalType = 2;
					else if ( nAnimalType == ANIMAL_TYPE_BEAST )
						nAnimalType = 0;
					else if ( nAnimalType == ANIMAL_TYPE_BIRD )
						nAnimalType = 1;

					if ( i == 0 || i == 22 )
						nAnimalType = 1;
					else if (i == 14 || i == 8 )
						nAnimalType = 0;

					m_ImageAnimalFlicker[nAnimalType].DrawImage(pD3DDevice, m_AnimalsDraw[i].rectDraw.left + 1, m_AnimalsDraw[i].rectDraw.top + 2, (GetTickCount()/200)%2);
				}

				CSize  sizeAnimal( (LONG)(m_ImageAnimals.GetWidth() ), (LONG)(m_ImageAnimals.GetHeight()) );
				CPoint ptAnimal( m_AnimalsDraw[i].rectDraw.left + m_AnimalsDraw[i].rectDraw.Width()/2 - sizeAnimal.cx/2, m_AnimalsDraw[i].rectDraw.top + m_AnimalsDraw[i].rectDraw.Height()/2 - sizeAnimal.cy/2);
				m_ImageAnimals.DrawImage(pD3DDevice, ptAnimal.x, ptAnimal.y, sizeAnimal.cx, sizeAnimal.cy, CGameLogic::GetInstance()->TurnTableAnimal(i));
			}
		}
	}
	else
	{
		nMainBackAlpha -= 20;
		if ( nMainBackAlpha < 0 )
			nMainBackAlpha = 0;
	}

	//�滭����
	if ( m_nLaserShow )
	{
		m_ImageLaser[ (GetTickCount()/120)%2 ].DrawImage(pD3DDevice, m_ptBenchmark.x + 2, m_nLaserY);
	}

	//�ڵ���
	m_ImageKeepOut.DrawImage(pD3DDevice, 0, 0, m_ptBenchmark.x, nHeight, 0, 0);
	m_ImageKeepOut.DrawImage(pD3DDevice, m_ptBenchmark.x, 0, m_ImageMainBack.GetWidth(), m_ptBenchmark.y, 0, 0);
	m_ImageKeepOut.DrawImage(pD3DDevice, m_ptBenchmark.x + m_ImageMainBack.GetWidth(), 0, nWidth - (m_ptBenchmark.x + m_ImageMainBack.GetWidth()), nHeight, 0, 0);
	m_ImageKeepOut.DrawImage(pD3DDevice, m_ptBenchmark.x, m_ptBenchmark.y + m_ImageMainBack.GetHeight(), m_ImageMainBack.GetWidth(), nHeight - (m_ptBenchmark.y + m_ImageMainBack.GetHeight()), 0, 0);

	//�ײ�����
	m_ImageMainBack.DrawImage(pD3DDevice, m_ptBenchmark.x, m_ptBenchmark.y + 504, 756, 198, 0, 504 );

	//�滭������Ϣ
	DrawPlayInfo(pD3DDevice, nWidth, nHeight);

	//��Ϸ��¼
	for ( int i = 0, nIndex = (int)m_ArrayTurnTableRecord.GetCount() - 1; i < m_ArrayTurnTableRecord.GetCount() && m_bShowTurnTableRecord && nIndex >= 0; ++i, nIndex-- )
	{
		m_ImageAnimalRecord.DrawImage(pD3DDevice, m_ptBenchmark.x + 130 + (i%10) * 50, m_ptBenchmark.y + 402 + (i/10) * 50, BYTE(200), m_ArrayTurnTableRecord[nIndex]);
	
		m_ImageAnimalRecord.DrawImage(pD3DDevice,  CPoint(m_ImageAnimalRecord.GetWidth()/2, m_ImageAnimalRecord.GetHeight()/2), D3DX_PI, 'z', 
			m_ptBenchmark.x + 130 + 450 - (i%10) * 50, m_ptBenchmark.y + 50 - (i/10) * 50, BYTE(200), m_ArrayTurnTableRecord[nIndex]);
		
		m_ImageAnimalRecord.DrawImage(pD3DDevice,  CPoint(m_ImageAnimalRecord.GetWidth()/2, m_ImageAnimalRecord.GetHeight()/2), D3DX_PI / 2, 'z', 
			m_ptBenchmark.x + 656 + (i/10) * 50, m_ptBenchmark.y + 450 - (i%10) * 50, BYTE(200), m_ArrayTurnTableRecord[nIndex]);

		m_ImageAnimalRecord.DrawImage(pD3DDevice,  CPoint(m_ImageAnimalRecord.GetWidth()/2, m_ImageAnimalRecord.GetHeight()/2), -D3DX_PI / 2, 'z', 
			m_ptBenchmark.x + 50 - (i/10) * 50, m_ptBenchmark.y + (i%10) * 50, BYTE(200), m_ArrayTurnTableRecord[nIndex]);
	}

	//�滭ʱ��
	if ( m_wMineChairID != INVALID_CHAIR && m_cbGameStatus != GAME_SCENE_END && m_cbGameStatus != GAME_SCENE_FREE && !m_nWinShow )
	{
		TCHAR strNumber[128];
		WORD wClock = GetUserClock(m_wMineChairID);
		if( wClock > 99 )
			_sntprintf(strNumber, CountArray(strNumber), TEXT("99"));
		else if ( wClock < 10 )
			_sntprintf(strNumber, CountArray(strNumber), TEXT("0%d"), wClock);
		else
			_sntprintf(strNumber, CountArray(strNumber), TEXT("%d"), wClock);

		m_ImageTimeBack.DrawImage(pD3DDevice, m_ptBenchmark.x + 378 - m_ImageTimeBack.GetWidth()/2, m_ptBenchmark.y + 255 - m_ImageTimeBack.GetHeight()/2);
		DrawNumber(pD3DDevice, &m_ImageTimeNumber, TEXT("0123456789"), strNumber, m_ptBenchmark.x + 378, m_ptBenchmark.y + 241, DT_CENTER);
	}

	//�滭Ӯ����
	if ( m_nWinShow && m_FishDraw.GetBackIndex() != 0 )
	{
		CD3DTextureIndex* pImageWin = NULL;
		int	nMultiple = 0;
		if ( m_nWinIndex <= ANIMAL_GOLD_SHARK && m_nWinIndex >= 0 )
		{
			pImageWin = &m_ImageAnimalsWin[m_nWinIndex];
			nMultiple = m_nAnimalMultiple[m_nWinIndex];
		}
		else if( m_nWinIndex == ANIMAL_MAX )
		{
			pImageWin = &m_ImageWinPrizes;
			nMultiple = m_nPrizesMultiple;
		}
		
		if ( pImageWin != NULL )
		{
			//����
			if ( m_nWinIndex == ANIMAL_MAX || m_nWinIndex == ANIMAL_SLIVER_SHARK || m_nWinIndex == ANIMAL_GOLD_SHARK )
			{
				m_ImageWinBright.DrawImage(pD3DDevice,  m_ptBenchmark.x + 378 - m_ImageWinBright.GetWidth()/2, m_ptBenchmark.y + 145 - m_ImageWinBright.GetHeight()/2, m_nWinFrame);
				m_ImageWinBright.DrawImage(pD3DDevice,  m_ptBenchmark.x + 378 - m_ImageWinBright.GetWidth()/2, m_ptBenchmark.y + 360 - m_ImageWinBright.GetHeight()/2, m_nWinFrame);
				m_ImageWinBright.DrawImage(pD3DDevice,  m_ptBenchmark.x + 168 - m_ImageWinBright.GetWidth()/2, m_ptBenchmark.y + 250 - m_ImageWinBright.GetHeight()/2, m_nWinFrame);
				m_ImageWinBright.DrawImage(pD3DDevice,  m_ptBenchmark.x + 589 - m_ImageWinBright.GetWidth()/2, m_ptBenchmark.y + 250 - m_ImageWinBright.GetHeight()/2, m_nWinFrame);
			}

			pImageWin->DrawImage(pD3DDevice, CPoint(pImageWin->GetWidth()/2, pImageWin->GetHeight()/2), D3DX_PI, 'z', 
				m_ptBenchmark.x + 378 - pImageWin->GetWidth()/2, m_ptBenchmark.y + 145 - pImageWin->GetHeight()/2, m_nWinFrame);
			pImageWin->DrawImage(pD3DDevice, CPoint(pImageWin->GetWidth()/2, pImageWin->GetHeight()/2), 0.f, 'z', 
				m_ptBenchmark.x + 378 - pImageWin->GetWidth()/2, m_ptBenchmark.y + 360 - pImageWin->GetHeight()/2, m_nWinFrame);
			pImageWin->DrawImage(pD3DDevice, CPoint(pImageWin->GetWidth()/2, pImageWin->GetHeight()/2), -D3DX_PI / 2, 'z', 
				m_ptBenchmark.x + 168 - pImageWin->GetWidth()/2, m_ptBenchmark.y + 250 - pImageWin->GetHeight()/2, m_nWinFrame);
			pImageWin->DrawImage(pD3DDevice, CPoint(pImageWin->GetWidth()/2, pImageWin->GetHeight()/2), D3DX_PI / 2, 'z', 
				m_ptBenchmark.x + 589 - pImageWin->GetWidth()/2, m_ptBenchmark.y + 250 - pImageWin->GetHeight()/2, m_nWinFrame);

			TCHAR strNumber[128];
			_sntprintf(strNumber, CountArray(strNumber), TEXT("*%d"), nMultiple);
			DrawRotateNumber(pD3DDevice, &m_ImagePrizesNumber, TEXT("0123456789*"), strNumber,  m_ptBenchmark.x + 378 - 32, m_ptBenchmark.y + 145 - 40, DRN_TOP);
			DrawRotateNumber(pD3DDevice, &m_ImagePrizesNumber, TEXT("0123456789*"), strNumber,  m_ptBenchmark.x + 378, m_ptBenchmark.y + 360 + 40, DRN_BOTTOM);
			DrawRotateNumber(pD3DDevice, &m_ImagePrizesNumber, TEXT("0123456789*"), strNumber,  m_ptBenchmark.x + 168 - 40, m_ptBenchmark.y + 250, DRN_LEFT);
			DrawRotateNumber(pD3DDevice, &m_ImagePrizesNumber, TEXT("0123456789*"), strNumber,  m_ptBenchmark.x + 589 + 40, m_ptBenchmark.y + 250 - 32, DRN_RIGHT);		
		}
	}

	if ( m_nGoldSilverShow && m_FishDraw.GetBackIndex() != 0 )
	{
		int nWinFrame = (GetTickCount()/200)%5;
		CSize sizeWinBright(m_ImageWinBright.GetWidth() * 3 / 2, m_ImageWinBright.GetHeight() * 3 / 2);
		m_ImageWinBright.DrawImage(pD3DDevice,  m_ptBenchmark.x + 189 - sizeWinBright.cx/2, m_ptBenchmark.y + 250 - sizeWinBright.cy/2, sizeWinBright.cx, sizeWinBright.cy, nWinFrame);
		m_ImageWinBright.DrawImage(pD3DDevice,  m_ptBenchmark.x + 378 - sizeWinBright.cx/2, m_ptBenchmark.y + 250 - sizeWinBright.cy/2, sizeWinBright.cx, sizeWinBright.cy, nWinFrame);
		m_ImageWinBright.DrawImage(pD3DDevice,  m_ptBenchmark.x + 567 - sizeWinBright.cx/2, m_ptBenchmark.y + 250 - sizeWinBright.cy/2, sizeWinBright.cx, sizeWinBright.cy, nWinFrame);
		
		m_ImageSharkWin[0].DrawImage(pD3DDevice,  m_ptBenchmark.x + 189 - m_ImageSharkWin[0].GetWidth()/2, m_ptBenchmark.y + 250 - m_ImageSharkWin[0].GetHeight()/2, nWinFrame);
		m_ImageSharkWin[1].DrawImage(pD3DDevice,  m_ptBenchmark.x + 378 - m_ImageSharkWin[1].GetWidth()/2, m_ptBenchmark.y + 250 - m_ImageSharkWin[1].GetHeight()/2, nWinFrame);
		m_ImageSharkWin[0].DrawImage(pD3DDevice,  m_ptBenchmark.x + 567 - m_ImageSharkWin[0].GetWidth()/2, m_ptBenchmark.y + 250 - m_ImageSharkWin[0].GetHeight()/2, nWinFrame);
	}

	return;
}

//�滭������Ϣ
VOID CGameClientView::DrawPlayInfo(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)
{
	//�滭���ﱶ��
	TCHAR strNumber[128];
	for ( int i = 0; i <= ANIMAL_SWALLOW; ++i )
	{
		_sntprintf(strNumber, CountArray(strNumber), TEXT("%d"), m_nAnimalMultiple[i]);
		DrawNumber(pD3DDevice, &m_ImageNumberGreen, TEXT("-0123456789"), strNumber, m_ptAnimalMultiple[i].x, m_ptAnimalMultiple[i].y, DT_RIGHT);
	}

	//��ע���
	_sntprintf(strNumber, CountArray(strNumber), TEXT("%d"), m_nCurrentNote);
	DrawNumber(pD3DDevice, &m_ImageNumberRed, TEXT("-0123456789"), strNumber, m_ptBenchmark.x + 735, m_ptBenchmark.y + 510, DT_RIGHT);

	//������ע
	for ( int i = 0; i < ANIMAL_MAX; ++i )
	{
		_sntprintf(strNumber, CountArray(strNumber), TEXT("%I64d"), m_lPlayBet[i]);
		DrawNumber(pD3DDevice, &m_ImageNumberRed, TEXT("-0123456789"), strNumber, m_ptPlayBet[i].x, m_ptPlayBet[i].y, DT_RIGHT);
	}

	//���˳���
	_sntprintf(strNumber, CountArray(strNumber), TEXT("%I64d"), m_lPlayChip);
	DrawNumber(pD3DDevice, &m_ImageNumberYellow, TEXT("-0123456789"), strNumber, m_ptBenchmark.x + 186, m_ptBenchmark.y + 509, DT_RIGHT);

	//�ʽ�λ��
	_sntprintf(strNumber, CountArray(strNumber), TEXT("%I64d"), m_lPlayShowPrizes);
	DrawNumber(pD3DDevice, &m_ImageNumberRed, TEXT("-0123456789"), strNumber, m_ptBenchmark.x + 186, m_ptBenchmark.y + 537, DT_RIGHT);

	//�÷�λ��
	_sntprintf(strNumber, CountArray(strNumber), TEXT("%I64d"), m_lPlayShowWin);
	DrawNumber(pD3DDevice, &m_ImageNumberRed, TEXT("-0123456789"), strNumber, m_ptBenchmark.x + 186, m_ptBenchmark.y + 565, DT_RIGHT);
}

// �滭����
void CGameClientView::DrawNumber(CD3DDevice * pD3DDevice , CD3DTexture* pImageNumber, TCHAR* szImageNum, TCHAR* szOutNum, INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/, BYTE cbAlpha /*= 255*/)
{
	// ������Դ
	INT nNumberHeight=pImageNumber->GetHeight();
	INT nNumberWidth=pImageNumber->GetWidth()/lstrlen(szImageNum);

	if ( uFormat == DT_CENTER )
	{
		nXPos -= (INT)(((DOUBLE)(lstrlen(szOutNum)) / 2.0) * nNumberWidth);
	}
	else if ( uFormat == DT_RIGHT )
	{
		nXPos -= lstrlen(szOutNum) * nNumberWidth;
	}

	for ( INT i = 0; i < lstrlen(szOutNum); ++i )
	{
		for ( INT j = 0; j < lstrlen(szImageNum); ++j )
		{
			if ( szOutNum[i] == szImageNum[j] && szOutNum[i] != '\0' )
			{
				pImageNumber->DrawImage(pD3DDevice, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight, cbAlpha);
				nXPos += nNumberWidth;
				break;
			}
		}
	}
}

// �滭����
void CGameClientView::DrawRotateNumber(CD3DDevice * pD3DDevice , CD3DTexture* pImageNumber, TCHAR* szImageNum, TCHAR* szOutNum, INT nXPos, INT nYPos, UINT uFormat)
{
	// ������Դ
	INT nNumberHeight=pImageNumber->GetHeight();
	INT nNumberWidth=pImageNumber->GetWidth()/lstrlen(szImageNum);

	// ���ֳ���
	int nImageCount = lstrlen(szImageNum);
	int nOutCount = lstrlen(szOutNum);

	// ƫ��
	CPoint ptOffSet(0,0);

	// ��ת
	CPoint ptRotate(pImageNumber->GetWidth()/nImageCount/2, nNumberHeight/2);

	float fAngle = 0;
	if ( uFormat == DRN_LEFT )
	{
		fAngle = -D3DX_PI/2;

		nXPos -= nNumberHeight/2;
		nYPos -= (INT)(((DOUBLE)(nOutCount) / 2.0) * nNumberWidth);

		ptOffSet.SetPoint(0, nNumberWidth);
	}
	else if ( uFormat == DRN_RIGHT )
	{
		fAngle = D3DX_PI/2;

		nXPos -= nNumberHeight/2;
		nYPos += (INT)(((DOUBLE)(nOutCount) / 2.0) * nNumberWidth);

		ptOffSet.SetPoint(0, -nNumberWidth);
	}
	else if ( uFormat == DRN_BOTTOM )
	{
		fAngle = 0;

		nXPos -= (INT)(((DOUBLE)(nOutCount) / 2.0) * nNumberWidth);
		nYPos -= nNumberHeight/2;

		ptOffSet.SetPoint(nNumberWidth, 0);
	}
	else if ( uFormat == DRN_TOP )
	{
		fAngle = D3DX_PI;

		nXPos += (INT)(((DOUBLE)(nOutCount) / 2.0) * nNumberWidth);
		nYPos -= nNumberHeight/2;

		ptOffSet.SetPoint(-nNumberWidth, 0);
	}


	for ( INT i = 0; i < nOutCount; ++i )
	{
		for ( INT j = 0; j < nImageCount; ++j )
		{
			if ( szOutNum[i] == szImageNum[j] && szOutNum[i] != '\0' )
			{
				pImageNumber->DrawImage(pD3DDevice, ptRotate, fAngle, 'z', nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
				nXPos += ptOffSet.x;
				nYPos += ptOffSet.y;
				break;
			}
		}
	}

	
	pImageNumber->DrawImage(pD3DDevice, CPoint(pImageNumber->GetWidth()/nImageCount/2, pImageNumber->GetHeight()/nImageCount/2), fAngle, 'z', 
		m_ptBenchmark.x + 378 - pImageNumber->GetWidth()/2, m_ptBenchmark.y + 145 - pImageNumber->GetHeight()/2, m_nWinFrame);
}

//�滭�ַ�
VOID CGameClientView::DrawTextString(CD3DDevice* pD3DDevice, CD3DFont * pD3DFont, LPCTSTR pszString, CRect rcDraw, UINT nFormat, COLORREF crText, COLORREF crFrame)
{
	//��������
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		//����λ��
		CRect rcFrame;
		rcFrame.top=rcDraw.top+nYExcursion[i];
		rcFrame.left=rcDraw.left+nXExcursion[i];
		rcFrame.right=rcDraw.right+nXExcursion[i];
		rcFrame.bottom=rcDraw.bottom+nYExcursion[i];

		//�滭�ַ�
		pD3DFont->DrawText(pD3DDevice, pszString,&rcFrame,nFormat,crFrame);
	}

	//�滭�ַ�
	pD3DFont->DrawText(pD3DDevice, pszString,&rcDraw, nFormat, crText);

	return;
}

//��Ӽ�¼
void CGameClientView::AddTurnTableRecord( int nAnimalIndex )
{
	//��Ӽ�¼
	m_ArrayTurnTableRecord.Add( nAnimalIndex );
	if( m_ArrayTurnTableRecord.GetCount() > RECORD_COUNT_MAX )
	{
		m_ArrayTurnTableRecord.RemoveAt(0);
	}
}

//��Ϸ����
void CGameClientView::GameOver( BOOL bTurnTwoTime, int nTurnTableTarget[2], int nPrizesMultiple, LONGLONG lPlayWin[2], LONGLONG lPlayPrizes )
{
	//���ñ���
	m_bTurnTwoTime = bTurnTwoTime;
	CopyMemory(m_nTurnTarget, nTurnTableTarget, sizeof(m_nTurnTarget));
	m_nPrizesMultiple = nPrizesMultiple;
	
	m_lPlayShowWin = 0;
	m_lPlayShowPrizes = 0;
	m_lPlayPrizes = lPlayPrizes;
	CopyMemory(m_lPlayWin, lPlayWin, sizeof(m_lPlayWin));

	//����� ���㡣��ʼ����
	int nTargetAnimal = CGameLogic::GetInstance()->TurnTableAnimal(m_nTurnTarget[0]);
	if ( nTargetAnimal == ANIMAL_GOLD_SHARK || nTargetAnimal == ANIMAL_SLIVER_SHARK )
	{
		BeginGoldSilver();
	}
	else
	{
		//��������
		BeginTurnTable(true, m_nTurnTarget[0]);
	}
}

//��ʼתȦ
void CGameClientView::BeginTurnTable( bool bFirst, int nTurnTableTarget  )
{
	//ת�̶���
	if ( bFirst )
	{
		//��һ�δ�ͷת��ת��Ȧ
		m_nTurnTableLaps = 0;
		//m_nTurnTableCurrent = 0;
		m_nTurnTransit = TURAN_TABLE_MAX * 7;
	
		if( nTurnTableTarget > m_nTurnTableCurrent )
		{
			m_nTurnTransit += ( nTurnTableTarget - m_nTurnTableCurrent );
		}
		else
		{
			m_nTurnTransit += ( TURAN_TABLE_MAX - m_nTurnTableCurrent ) + nTurnTableTarget;
		}
		
		//��������
		for ( int i = 0; i < TURAN_TABLE_MAX; ++i )
		{
			m_AnimalsDraw[i].bBright = false;
			m_AnimalsDraw[i].bflicker = false;
		}

		//��������
		CD3DMusic::_Object()->Stop();
		CD3DMusic::_Object()->Start(TEXT("\\Sound\\ת��.mp3"));
	}
	else
	{
		//�ڶ���ֻת��Ȧ
		m_nTurnTableLaps = 1;
		m_nTurnTransit = TURAN_TABLE_MAX;
		if( nTurnTableTarget > m_nTurnTableCurrent )
			m_nTurnTransit += ( nTurnTableTarget - m_nTurnTableCurrent );
		else
			m_nTurnTransit += ( TURAN_TABLE_MAX - m_nTurnTableCurrent ) + nTurnTableTarget;
	}
	
	m_nBrightCount = 1;
	m_nTurnTableTarget = nTurnTableTarget;
	m_nTurnTableCount = 10;
	m_nTurnTableDelay = m_nTurnTableCount;
	m_nTurnCount = 0;
	m_bTurnTable = TRUE;
	m_TurnTableLapse.Initialization();
}

//����תȦ
void CGameClientView::EndTurnTable()
{
	m_nBrightCount = 0;
	//m_nTurnTableCurrent = 0;
	m_nTurnTableTarget = 0;
	m_nTurnTableCount = 10;
	m_nTurnTableDelay = 0;
	m_nTurnCount = 0;
	m_nTurnTransit = 0;
	m_nTurnTableLaps = 0;
	m_bTurnTable = 0;
	m_TurnTableLapse.Initialization();

	//��������
	for ( int i = 0; i < TURAN_TABLE_MAX; ++i )
	{
		m_AnimalsDraw[i].bBright = false;
		m_AnimalsDraw[i].bflicker = false;
	}
}

//��ʾӮ
void CGameClientView::BeginWin( int nAnimalIndex )
{
	if ( !((nAnimalIndex <= ANIMAL_GOLD_SHARK && nAnimalIndex >= 0) || (nAnimalIndex == ANIMAL_MAX)) )
		return;

	m_nWinShow = TRUE;
	m_nWinIndex = nAnimalIndex;
	m_nWinFrame = 0;
	m_nWinFrameTime.Initialization();
	m_nWinContinueTime.Initialization();

	switch(nAnimalIndex)
	{
	case ANIMAL_LION:
		{
			CD3DMusic::_Object()->Start(TEXT("\\Sound\\ʨ��.mp3"));
		}
		break;
	case ANIMAL_PANDA:
		{
			CD3DMusic::_Object()->Start(TEXT("\\Sound\\��è.mp3"));
		}
		break;
	case ANIMAL_MONKEY:
		{
			CD3DMusic::_Object()->Start(TEXT("\\Sound\\����.mp3"));
		}
		break;
	case ANIMAL_RABBIT:
		{
			CD3DMusic::_Object()->Start(TEXT("\\Sound\\����.mp3"));
		}
		break;
	case ANIMAL_EAGLE:
		{
			CD3DMusic::_Object()->Start(TEXT("\\Sound\\��ӥ.mp3"));
		}
		break;
	case ANIMAL_PEACOCK:
		{
			CD3DMusic::_Object()->Start(TEXT("\\Sound\\��ȸ.mp3"));
		}
		break;
	case ANIMAL_PIGEON:
		{
			CD3DMusic::_Object()->Start(TEXT("\\Sound\\����.mp3"));
		}
		break;
	case ANIMAL_SWALLOW:
		{
			CD3DMusic::_Object()->Start(TEXT("\\Sound\\����.mp3"));
		}
		break;
	case ANIMAL_SLIVER_SHARK:
		{
			CD3DMusic::_Object()->Start(TEXT("\\Sound\\����.mp3"));
		}
		break;
	case ANIMAL_GOLD_SHARK:
		{
			CD3DMusic::_Object()->Start(TEXT("\\Sound\\����.mp3"));
		}
		break;
	}
}

//����Ӯ
void CGameClientView::EndWin()
{
	m_nWinShow = FALSE;
	m_nWinIndex = INT_MAX;
}

//��ʾ����
void CGameClientView::BeginLaser()
{
	m_nLaserShow = TRUE;
	m_nLaserDown = TRUE;
	m_nLaserY = m_ptBenchmark.y - 504;

	CD3DMusic::_Object()->Stop();
	CD3DMusic::_Object()->Start(TEXT("\\Sound\\����.mp3"));
}

//��������
void CGameClientView::EndLaser()
{
	m_nLaserShow = FALSE;
	m_nLaserDown = FALSE;
	m_nLaserY = 0;
}

//��ʾ������
void CGameClientView::BeginGoldSilver()
{
	m_nGoldSilverShow = TRUE;
	m_nGoldSilverContinueTime.Initialization();
}

//����������
void CGameClientView::EndGoldSilver()
{
	m_nGoldSilverShow = FALSE;
}

// ʨ�Ӱ�ť
VOID CGameClientView::OnBnClickedAnimalLion()
{
	SendEngineMessage(IDM_PLAY_BET, ANIMAL_LION, 0);
}

// ��è��ť
VOID CGameClientView::OnBnClickedAnimalPanda()
{
	SendEngineMessage(IDM_PLAY_BET, ANIMAL_PANDA, 0);
}

// ���Ӱ�ť
VOID CGameClientView::OnBnClickedAnimalMonkey()
{
	SendEngineMessage(IDM_PLAY_BET, ANIMAL_MONKEY, 0);
}

// ���Ӱ�ť
VOID CGameClientView::OnBnClickedAnimalRabbit()
{
	SendEngineMessage(IDM_PLAY_BET, ANIMAL_RABBIT, 0);
}

// ��ӥ��ť
VOID CGameClientView::OnBnClickedAnimalEagle()
{
	SendEngineMessage(IDM_PLAY_BET, ANIMAL_EAGLE, 0);
}

// ��ȸ��ť
VOID CGameClientView::OnBnClickedAnimalPeacock()
{
	SendEngineMessage(IDM_PLAY_BET, ANIMAL_PEACOCK, 0);
}

// ���Ӱ�ť
VOID CGameClientView::OnBnClickedAnimalPigeon()
{
	SendEngineMessage(IDM_PLAY_BET, ANIMAL_PIGEON, 0);
}

// ���Ӱ�ť
VOID CGameClientView::OnBnClickedAnimalSwallow()
{
	SendEngineMessage(IDM_PLAY_BET, ANIMAL_SWALLOW, 0);
}

// ���谴ť
VOID CGameClientView::OnBnClickedAnimalSliverShark()
{
	SendEngineMessage(IDM_PLAY_BET, ANIMAL_SLIVER_SHARK, 0);
}

// ���谴ť
VOID CGameClientView::OnBnClickedAnimalGoldShark()
{
	SendEngineMessage(IDM_PLAY_BET, ANIMAL_GOLD_SHARK, 0);
}

// ���ݰ�ť
VOID CGameClientView::OnBnClickedAnimalBird()
{
	SendEngineMessage(IDM_PLAY_BET, ANIMAL_BIRD, 0);
}

// ���ް�ť
VOID CGameClientView::OnBnClickedAnimalBeast()
{
	SendEngineMessage(IDM_PLAY_BET, ANIMAL_BEAST, 0);
}

// ��ѹ��ť
VOID CGameClientView::OnBnClickedOperationRenewal()
{	
	SendEngineMessage(IDM_OPERATION_RENEWAL, 0, 0);
}

// ȡ����ť
VOID CGameClientView::OnBnClickedOperationCancel()
{
	SendEngineMessage(IDM_OPERATION_CANCEL, 0, 0);
}

// �л���ť
VOID CGameClientView::OnBnClickedOperationSwitch()
{
	SendEngineMessage(IDM_OPERATION_SWITCH, 0, 0);
}

//������
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CGameFrameViewD3D::OnLButtonDown(nFlags, point);
}

//������
void CGameClientView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CGameFrameViewD3D::OnRButtonDown(nFlags, point);
}

//��Ϣ����
BOOL CGameClientView::PreTranslateMessage(MSG* pMsg)
{
	return CGameFrameViewD3D::PreTranslateMessage(pMsg);
}

//��ʱ��
void CGameClientView::OnTimer(UINT nIDEvent)
{
	CGameFrameViewD3D::OnTimer(nIDEvent);
}

//���̰���
void CGameClientView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'Q':
		{
			m_bKeyDown[ANIMAL_SWALLOW] = TRUE;
			m_KeyLapse[ANIMAL_SWALLOW].Initialization();
		}
		break;
	case 'W':
		{
			m_bKeyDown[ANIMAL_PIGEON] = TRUE;
			m_KeyLapse[ANIMAL_PIGEON].Initialization();
		}
		break;
	case 'E':
		{
			m_bKeyDown[ANIMAL_PEACOCK] = TRUE;
			m_KeyLapse[ANIMAL_PEACOCK].Initialization();
		}
		break;
	case 'R':
		{
			m_bKeyDown[ANIMAL_EAGLE] = TRUE;
			m_KeyLapse[ANIMAL_EAGLE].Initialization();
		}
		break;
	case 'T':
		{
			m_bKeyDown[ANIMAL_LION] = TRUE;
			m_KeyLapse[ANIMAL_LION].Initialization();
		}
		break;
	case 'Y':
		{
			m_bKeyDown[ANIMAL_PANDA] = TRUE;
			m_KeyLapse[ANIMAL_PANDA].Initialization();
		}
		break;
	case 'U':
		{
			m_bKeyDown[ANIMAL_MONKEY] = TRUE;
			m_KeyLapse[ANIMAL_MONKEY].Initialization();
		}
		break;
	case 'I':
		{
			m_bKeyDown[ANIMAL_RABBIT] = TRUE;
			m_KeyLapse[ANIMAL_RABBIT].Initialization();
		}
		break;
	case 0x31:
		{
			m_bKeyDown[ANIMAL_BIRD] = TRUE;
			m_KeyLapse[ANIMAL_BIRD].Initialization();
		}
		break;
	case 0x32:
		{
			m_bKeyDown[ANIMAL_BEAST] = TRUE;
			m_KeyLapse[ANIMAL_BEAST].Initialization();
		}
		break;
	case 0x33:
		{
			m_bKeyDown[ANIMAL_GOLD_SHARK] = TRUE;
			m_KeyLapse[ANIMAL_GOLD_SHARK].Initialization();
		}
		break;
	case 0x34:
		{
			m_bKeyDown[ANIMAL_SLIVER_SHARK] = TRUE;
			m_KeyLapse[ANIMAL_SLIVER_SHARK].Initialization();
		}
		break;
	}
	CGameFrameViewD3D::OnKeyDown(nChar, nRepCnt, nFlags);
}

//��������
void CGameClientView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'Q':
		{
			m_bKeyDown[ANIMAL_SWALLOW] = FALSE;
			SendEngineMessage(IDM_PLAY_BET, ANIMAL_SWALLOW, 0);
		}
		break;
	case 'W':
		{
			m_bKeyDown[ANIMAL_PIGEON] = FALSE;
			SendEngineMessage(IDM_PLAY_BET, ANIMAL_PIGEON, 0);
		}
		break;
	case 'E':
		{
			m_bKeyDown[ANIMAL_PEACOCK] = FALSE;
			SendEngineMessage(IDM_PLAY_BET, ANIMAL_PEACOCK, 0);
		}
		break;
	case 'R':
		{
			m_bKeyDown[ANIMAL_EAGLE] = FALSE;
			SendEngineMessage(IDM_PLAY_BET, ANIMAL_EAGLE, 0);
		}
		break;
	case 'T':
		{
			m_bKeyDown[ANIMAL_LION] = FALSE;
			SendEngineMessage(IDM_PLAY_BET, ANIMAL_LION, 0);
		}
		break;
	case 'Y':
		{
			m_bKeyDown[ANIMAL_PANDA] = FALSE;
			SendEngineMessage(IDM_PLAY_BET, ANIMAL_PANDA, 0);
		}
		break;
	case 'U':
		{
			m_bKeyDown[ANIMAL_MONKEY] = FALSE;
			SendEngineMessage(IDM_PLAY_BET, ANIMAL_MONKEY, 0);
		}
		break;
	case 'I':
		{
			m_bKeyDown[ANIMAL_RABBIT] = FALSE;
			SendEngineMessage(IDM_PLAY_BET,ANIMAL_RABBIT , 0);
		}
		break;
	case 0x31:
		{
			m_bKeyDown[ANIMAL_BIRD] = FALSE;
			SendEngineMessage(IDM_PLAY_BET, ANIMAL_BIRD, 0);
		}
		break;
	case 0x32:
		{
			m_bKeyDown[ANIMAL_BEAST] = FALSE;
			SendEngineMessage(IDM_PLAY_BET, ANIMAL_BEAST, 0);
		}
		break;
	case 0x33:
		{
			m_bKeyDown[ANIMAL_GOLD_SHARK] = FALSE;
			SendEngineMessage(IDM_PLAY_BET, ANIMAL_GOLD_SHARK, 0);
		}
		break;
	case 0x34:
		{
			m_bKeyDown[ANIMAL_SLIVER_SHARK] = FALSE;
			SendEngineMessage(IDM_PLAY_BET, ANIMAL_SLIVER_SHARK, 0);
		}
		break;
	}

	CGameFrameViewD3D::OnKeyUp(nChar, nRepCnt, nFlags);
}


//ʧȥ����
void CGameClientView::OnKillFocus(CWnd* pNewWnd)
{
	CGameFrameViewD3D::OnKillFocus(pNewWnd);
	ZeroMemory(m_bKeyDown, sizeof(m_bKeyDown));
	ZeroMemory(m_bKeySend, sizeof(m_bKeySend));
}
//////////////////////////////////////////////////////////////////////////////////
//����Ա����
void CGameClientView::OpenAdminWnd()
{
	//��Ȩ��
	if(m_pClientControlDlg != NULL )
	//if (m_pClientControlDlg != NULL && (CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight)))

	{
		if(!m_pClientControlDlg->IsWindowVisible()) 
			m_pClientControlDlg->ShowWindow(SW_SHOW);
		else 
			m_pClientControlDlg->ShowWindow(SW_HIDE);
	}
}

//�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_ADMIN:
		OpenAdminWnd();
		break;
	}
	return __super::OnCommand(wParam, lParam);
}
//�������
void CGameClientView::AllowControl(BYTE cbStatus)
{
	if(m_pClientControlDlg != NULL && m_pClientControlDlg->m_hWnd!=NULL && m_hInst)
	{
		bool bEnable=false;
		switch(cbStatus)
		{
		case GAME_STATUS_FREE: bEnable=true; break;
		case GAME_SCENE_END:bEnable=false;break;
		case GAME_SCENE_BET:bEnable=true;break;
		default:bEnable=false;break;
		}
		m_pClientControlDlg->OnAllowControl(bEnable);
	}
}



