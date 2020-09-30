#include "StdAfx.h"
#include "GameClient.h"
#include "GameClientView.h"
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//������ʶ
#define IDI_HIDE_CARD					100								//�����˿�
#define IDI_SEND_CARD					101								//���Ʊ�ʶ
#define IDI_USER_ACTION					102								//������ʶ
#define IDI_MOVE_CHIP					103								//�����ƶ�

//������Ŀ
#define SEND_CARD_COUNT					5								//������Ŀ

//λ�ö���
#define CARD_EXCURSION_X				283								//�˿�ƫ��
#define CARD_EXCURSION_Y				177								//�˿�ƫ��

//���ƶ���
#define SEND_STEP_COUNT					8								//����
#define SPEED_SEND_CARD					10								//�����ٶ�

//���붨��
#define CHIP_COUNT						14								//��������
#define PILING_HEIGHT					6								//�ѻ��߶�


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)

	//ϵͳ��Ϣ
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()

	//��ť��Ϣ
	ON_BN_CLICKED(IDC_START, OnBnClickedStart)
	ON_BN_CLICKED(IDC_CONTROL_FOLLOW, OnBnClickedFollow)
	ON_BN_CLICKED(IDC_CONTROL_GIVEUP, OnBnClickedGiveUp)
	ON_BN_CLICKED(IDC_CONTROL_SHOWHAND, OnBnClickedShowHand)

	ON_BN_CLICKED(IDC_ADD_SCORE, OnBnClickedAddScore)
	ON_BN_CLICKED(IDC_ADD_SCORE_ONE, OnBnClickedAddScoreOne)
	ON_BN_CLICKED(IDC_ADD_SCORE_TWO, OnBnClickedAddScoreTwo)
	ON_BN_CLICKED(IDC_ADD_SCORE_THREE, OnBnClickedAddScoreThree)
	ON_BN_CLICKED(IDC_ADD_SCORE_FOUR, OnBnClickedAddScoreFour)

	ON_BN_CLICKED(IDC_CONTROL_ADD, OnBnClickedADD)
	ON_BN_CLICKED(IDC_CONTROL_SUB, OnBnClickedSUB)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//״̬����
	m_bFirstChange = TRUE;
	m_lCellScore=0L;
	m_lDrawMaxScore=0L;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));
	ZeroMemory(m_lUserAddAllScore,sizeof(m_lUserAddAllScore));
	ZeroMemory(m_lUserSaveScore,sizeof(m_lUserSaveScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lOverScore,sizeof(m_lOverScore));
	ZeroMemory(m_lOverSaveScore,sizeof(m_lOverSaveScore));
	

	//��������
	m_SendCardPos.SetPoint(0,0);
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = SEND_STEP_COUNT;
	m_nXStep = 0;
	m_nYStep = 0;

	//��������
	m_cbUserAction=0;
	m_wActionUser=INVALID_CHAIR;

	//״̬����
	m_bShowScore=false;
	ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));

	//�˿˳���
	ZeroMemory(m_lPalyBetChip,sizeof(m_lPalyBetChip));
	m_lALLBetChip = 0;

	for ( int i = 0; i < GAME_PLAYER; ++i )
	{
		m_CardControl[i].Initialize( i != 2 );
	}

	//������Դ
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_ImageReady.LoadImage(AfxGetInstanceHandle(), TEXT("READY"));

	m_ImageGameCard.LoadImage(AfxGetInstanceHandle(), TEXT("GAME_CARD"));
	m_ImageViewFill.LoadFromResource(hResInstance,IDB_VIEW_FILL);
	m_ImageViewBack.LoadImage(hResInstance,TEXT("VIEW_BACK"));
	m_ImgaeScoreTotalNumber.LoadImage(hResInstance,TEXT("SCORE_NUMBER"));
	m_ImageActionBack.LoadImage(hResInstance,TEXT("ACTION_BACK"));
	m_ImageActionFont.LoadImage(hResInstance,TEXT("ACTION_FONT"));
	m_ImageShowHand.LoadImage(hResInstance,TEXT("SHOW_HAND"));

	m_ImageUserInfoH.LoadImage(hResInstance,TEXT("USER_INFO_H"));
	m_ImageUserInfoV.LoadImage(hResInstance,TEXT("USER_INFO_V"));
	m_ImageUserPalyH.LoadImage(hResInstance,TEXT("USERPLAYING_INFO_H"));
	m_ImageUserPalyV.LoadImage(hResInstance,TEXT("USERPLAYING_INFO_V"));

	m_ImageChip.LoadImage(hResInstance,TEXT("JETTON_VIEW"));

	m_ImageCellScore.LoadImage(hResInstance,TEXT("CELL_SCORE"));
	m_ImgaeScoreTotal.LoadImage(hResInstance,TEXT("SCORE_TOTAL"));

	m_ImageCellNumber.LoadImage(hResInstance,TEXT("CELL_NUMBER"));

	m_ImgaeScoreBackR.LoadImage(hResInstance,TEXT("GAME_SCORE_R"));
	m_ImgaeScoreBackL.LoadImage(hResInstance,TEXT("GAME_SCORE_L"));
	m_ImgaeScoreBackM.LoadImage(hResInstance,TEXT("GAME_SCORE_M"));

	m_ImgaeScoreSum.LoadImage(hResInstance,TEXT("SUM_IMSUM"));
	m_ImgaeScoreWin.LoadImage(hResInstance,TEXT("WIN_NUMBER"));
	m_ImgaeScoreLose.LoadImage(hResInstance,TEXT("LOSE_NUMBER"));

	m_ImageTime.LoadImage(hResInstance,TEXT("TIME_NUMBER"));

	m_ImageCradType.LoadImage(hResInstance,TEXT("CARD_TYPE"));

	//��ȡ��С
	m_SizeGameCard.SetSize(m_ImageGameCard.GetWidth()/13,m_ImageGameCard.GetHeight()/5);

	return;
}

//��������
CGameClientView::~CGameClientView()
{
	//ɾ������
	for ( int i = 0 ; i < m_ArrayPlayChip.GetCount(); ++i )
		SafeDelete(m_ArrayPlayChip[i]);
	m_ArrayPlayChip.RemoveAll();

	for ( int i = 0 ; i < m_ArrayBetChip.GetCount(); ++i )
		SafeDelete(m_ArrayBetChip[i]);
	m_ArrayBetChip.RemoveAll();

	for ( int i = 0 ; i < m_ArrayOverChip.GetCount(); ++i )
		SafeDelete(m_ArrayOverChip[i]);
	m_ArrayOverChip.RemoveAll();
}

//��Ϣ����
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL) 
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN 
		&& m_PlayOperate.IsShow() && m_PlayOperate.IsAddShow() )
	{
		//������Ϣ
		SendEngineMessage(IDM_FOLLOW,(WPARAM)m_PlayOperate.GetBetCount(),0);
	}

	if ( m_PlayOperate.PreTranslateMessage(pMsg) )
	{
		CRect rect = m_PlayOperate.GetOperateRect();
		InvalidGameView(rect.left, rect.top, rect.Width(), rect.Height());
		return TRUE;
	}

	bool bUpdateGameView = false;
	if ( pMsg->message == WM_CHAR )
	{
		bUpdateGameView = true;

	}

	BOOL bReturn = __super::PreTranslateMessage(pMsg);

	if( bUpdateGameView )
	{
		CRect rect = m_PlayOperate.GetOperateRect();
		InvalidGameView(rect.left, rect.top, rect.Width(), rect.Height());
	}

	return bReturn;
}

//���ý���
VOID CGameClientView::ResetGameView()
{
	__super::ResetGameView();

	//ɾ��ʱ��
	KillTimer(IDI_HIDE_CARD);
	KillTimer(IDI_SEND_CARD);
	KillTimer(IDI_USER_ACTION);

	//״̬����
	m_lCellScore=0L;
	m_lDrawMaxScore=0L;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));
	ZeroMemory(m_lUserAddAllScore,sizeof(m_lUserAddAllScore));
	ZeroMemory(m_lUserSaveScore,sizeof(m_lUserSaveScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lOverScore,sizeof(m_lOverScore));
	ZeroMemory(m_lOverSaveScore,sizeof(m_lOverSaveScore));

	//��������
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = 0;
	m_nXStep = 0;
	m_nYStep = 0;
	m_SendCardItemArray.RemoveAll();

	//��������
	m_cbUserAction=0;
	m_wActionUser=INVALID_CHAIR;

	//״̬����
	m_bShowScore=false;
	ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));

	//���ư�ť
	m_btStart.ShowWindow(SW_HIDE);
	m_PlayOperate.ShowOperate(false);

	//��ֹ��ť
	m_PlayOperate.m_btFollow.EnableWindow(FALSE);
	m_PlayOperate.m_btAddScore.EnableWindow(FALSE);
	m_PlayOperate.m_btShowHand.EnableWindow(FALSE);

	//�˿˳���
	ZeroMemory(m_lPalyBetChip,sizeof(m_lPalyBetChip));
	m_lALLBetChip = 0;

	//����ؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
		m_CardControl[i].SetCardData(NULL,0);

	//�û��˿�
	m_CardControl[MYSELF_VIEW_ID].SetPositively(false);
	m_CardControl[MYSELF_VIEW_ID].SetDisplayHead(false);

	//ɾ������
	for ( int i = 0 ; i < m_ArrayPlayChip.GetCount(); ++i )
		SafeDelete(m_ArrayPlayChip[i]);
	m_ArrayPlayChip.RemoveAll();

	for ( int i = 0 ; i < m_ArrayBetChip.GetCount(); ++i )
		SafeDelete(m_ArrayBetChip[i]);
	m_ArrayBetChip.RemoveAll();

	for ( int i = 0 ; i < m_ArrayOverChip.GetCount(); ++i )
		SafeDelete(m_ArrayOverChip[i]);
	m_ArrayOverChip.RemoveAll();

	return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//׼��λ��
	m_ptReady[0].SetPoint(nWidth/2 - 220, nHeight/2 - 140);
	m_ptReady[1].SetPoint(nWidth/2 - 235, nHeight/2 + 30);
	m_ptReady[2].SetPoint(nWidth/2 - 1,   nHeight/2 + 190 + 18);
	m_ptReady[3].SetPoint(nWidth/2 + 235, nHeight/2 + 30);
	m_ptReady[4].SetPoint(nWidth/2 + 220, nHeight/2 - 140);

	// ͷ��
	m_ptUserInfo[0].SetPoint(nWidth/2 - 220, nHeight/2 - 250);
	m_ptUserInfo[1].SetPoint(nWidth/2 - 335, nHeight/2 + 30);
	m_ptUserInfo[2].SetPoint(nWidth/2,       nHeight/2 + 240 + 18);
	m_ptUserInfo[3].SetPoint(nWidth/2 + 335, nHeight/2 + 30);
	m_ptUserInfo[4].SetPoint(nWidth/2 + 220, nHeight/2 - 250);

	//ʱ��λ��
	m_ptClock[0].SetPoint(nWidth/2 - 325, nHeight/2 - 252);
	m_ptClock[1].SetPoint(nWidth/2 - 345, nHeight/2 - 67);
	m_ptClock[2].SetPoint(nWidth/2 - 105, nHeight/2 + 235 + 18);
	m_ptClock[3].SetPoint(nWidth/2 + 345, nHeight/2 - 67);
	m_ptClock[4].SetPoint(nWidth/2 + 325, nHeight/2 - 252);

	//����λ��
	m_ptUserAction[0].SetPoint(nWidth/2 - 90, nHeight/2 - 245);
	m_ptUserAction[1].SetPoint(nWidth/2 - 335, nHeight/2 - 55);
	m_ptUserAction[2].SetPoint(nWidth/2 - 130, nHeight/2 + 190 + 18);
	m_ptUserAction[3].SetPoint(nWidth/2 + 335, nHeight/2 - 55);
	m_ptUserAction[4].SetPoint(nWidth/2 + 90, nHeight/2 - 245);


	//�˿˿ؼ�
	m_CardControl[0].SetBenchmarkPos(nWidth/2 - 220, nHeight/2 - 200,	enXCenter,enYTop);
	m_CardControl[1].SetBenchmarkPos(nWidth/2 - 293, nHeight/2 + 30,	enXLeft,enYCenter);
	m_CardControl[2].SetBenchmarkPos(nWidth/2,		 nHeight/2 + 193 + 18,	enXCenter,enYBottom);
	m_CardControl[3].SetBenchmarkPos(nWidth/2 + 293, nHeight/2 + 30,	enXRight,enYCenter);
	m_CardControl[4].SetBenchmarkPos(nWidth/2 + 220, nHeight/2 - 200,	enXCenter,enYTop);

	//��ҳ���
	m_ptPalyBetChip[0].SetPoint(nWidth/2 - 122, nHeight/2 - 125);
	m_ptPalyBetChip[1].SetPoint(nWidth/2 - 120,	nHeight/2 + 30);
	m_ptPalyBetChip[2].SetPoint(nWidth/2,		nHeight/2 + 45 + 18);
	m_ptPalyBetChip[3].SetPoint(nWidth/2 + 120,	nHeight/2 + 30);
	m_ptPalyBetChip[4].SetPoint(nWidth/2 + 122, nHeight/2 - 125);

	// ��������
	m_ptOverScore[0].SetPoint(nWidth/2 - 220, nHeight/2 - 75);
	m_ptOverScore[1].SetPoint(nWidth/2 - 205, nHeight/2 + 102);
	m_ptOverScore[2].SetPoint(nWidth/2,       nHeight/2 + 290 + 18);
	m_ptOverScore[3].SetPoint(nWidth/2 + 205, nHeight/2 + 102);
	m_ptOverScore[4].SetPoint(nWidth/2 + 220, nHeight/2 - 75);

	//������ʼλ��
	m_SendCardPos.SetPoint(nWidth / 2 + 20,nHeight / 2 - 200);
	
	//��ʼ��ť
	m_btStart.SetWindowPos( NULL,nWidth/2 + 88,nHeight/2 + 150 + 22,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);

	//���ƴ���
	m_PlayOperate.SetOperatePos( nWidth/2 + 100, nHeight/2+120 + 18, PYOE_LEFT|PYOE_TOP);

	// �������
	m_rectBetChip.SetRect( nWidth / 2 - 75,nHeight / 2 - 140, nWidth / 2 + 75,nHeight / 2 - 10 + 18);

	// �任����λ��
	for ( int i = 0 ; i < m_ArrayBetChip.GetCount(); ++i )
	{
		CPoint ptOffset( (nWidth - m_sizeWin.cx)/2,(nHeight - m_sizeWin.cy)/2 ); 
		m_ArrayBetChip[i]->ptBeginPos += ptOffset;
		m_ArrayBetChip[i]->ptEndPos += ptOffset;
		m_ArrayBetChip[i]->ptCurrentPos += ptOffset;
	}
	
	// ���ڴ�С
	m_sizeWin.SetSize(nWidth, nHeight);

	if( m_bFirstChange && nWidth != 0 && nWidth != 754 && nHeight != 0 )
	{
		m_bFirstChange = FALSE;
		if ( nWidth < (int)m_ImageViewBack.GetWidth() )
		{
		}
	}
	return;
}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);
	m_ImageViewBack.DrawImage( pDC, nWidth/2 - m_ImageViewBack.GetWidth()/2, nHeight/2 - m_ImageViewBack.GetHeight()/2);

	// ��Ϣ��ʾ
	m_ImageCellScore.DrawImage( pDC, nWidth/2 - 360, 0 );
	m_ImgaeScoreTotal.DrawImage( pDC, nWidth/2 - 123, 0 );

	//��Ϣ����
	if( m_lCellScore != 0 )
		DrawNumber(pDC, &m_ImageCellNumber, TEXT("0123456789"), m_lCellScore,   nWidth/2 - 292, 7);
	if( m_lDrawMaxScore != 0 )
		DrawNumber(pDC, &m_ImageCellNumber, TEXT("0123456789"), m_lDrawMaxScore,nWidth/2 - 292, 37);

	//������ע
	LONGLONG lTableScore = m_lALLBetChip;
	for( INT i = 0; i < GAME_PLAYER; i++ )
		lTableScore += m_lPalyBetChip[i];

	//��ע��Ϣ
	if ( lTableScore > 0L )
	{
		DrawNumber(pDC, &m_ImgaeScoreTotalNumber, TEXT("0123456789"), lTableScore, nWidth/2 - 23, 32);
	}

	//�û��˿�
	for (BYTE i=0;i<GAME_PLAYER;i++) 
	{
		m_CardControl[i].DrawCardControl(pDC);
	}

	//�滭�û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IClientUserItem * pIClientUserItem=GetClientUserItem(i);
		//IClientUserItem * pIClientUserItem=GetClientUserItem(2);
		if (pIClientUserItem == NULL) continue;

		//�滭ʱ��
		WORD wTime = GetUserClock(i);
		if ( wTime > 0 && wTime <= 99 )
		{
			TCHAR szTime[128] = {0};
			if( wTime > 9)
				_sntprintf(szTime,CountArray(szTime),TEXT("%d"), wTime);
			else
				_sntprintf(szTime,CountArray(szTime),TEXT("0%d"), wTime);

			DrawNumber(pDC , &m_ImageTime, TEXT("0123456789"), szTime, m_ptClock[i].x, m_ptClock[i].y, DT_CENTER );
		}

		// �滭�����Ϣ
		CPoint ptShowPos;
		if ( i == 1 || i == 3 )
		{
			ptShowPos.SetPoint(m_ptUserInfo[i].x - m_ImageUserInfoV.GetWidth()/2, m_ptUserInfo[i].y - m_ImageUserInfoV.GetHeight()/2);
			
			// ����
			m_ImageUserInfoV.DrawImage( pDC, ptShowPos.x, ptShowPos.y);

			// ���
			if ( pIClientUserItem->GetUserStatus() == US_PLAYING && GetUserClock(i) )
			{
				m_ImageUserPalyV.DrawImage( pDC, m_ptUserInfo[i].x - m_ImageUserPalyV.GetWidth()/2, m_ptUserInfo[i].y - m_ImageUserPalyV.GetHeight()/2);
			}
			
			// ͷ��
			m_ptAvatar[i].SetPoint(ptShowPos.x + 13, ptShowPos.y + 8);
			DrawUserAvatar(pDC, m_ptAvatar[i].x, m_ptAvatar[i].y, pIClientUserItem);

			// ����
			CRect rectInfo( ptShowPos.x + 8, ptShowPos.y + 65, ptShowPos.x + 67, ptShowPos.y + 77);
			DrawTextStringEx(pDC,pIClientUserItem->GetNickName(),RGB(255,255,255),RGB(0,0,0), rectInfo, DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE|DT_LEFT);
		
			// �û��ȼ�
			rectInfo.OffsetRect( 0, 16 );
			DrawTextStringEx(pDC,GetLevelDescribe(pIClientUserItem),RGB(153,255,0),RGB(0,0,0),rectInfo, DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE|DT_LEFT);

			//�û�����
			rectInfo.OffsetRect( 0, 16 );
			DrawTextStringEx(pDC,AddComma(m_lUserScore[i]),RGB(255,255,0),RGB(0,0,0),rectInfo, DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE|DT_LEFT);
		
			// �û���ע
			if( m_lTableScore[i] > 0L )
			{
				UINT oldAlign = pDC->SetTextAlign(TA_CENTER);
				TCHAR szBuffer[128];
				_sntprintf(szBuffer,CountArray(szBuffer),TEXT("����ע��%s"),AddComma(m_lTableScore[i]));

				if ( i == 1 )
				{
					pDC->SetTextAlign(TA_LEFT);
					DrawTextStringEx(pDC, szBuffer, RGB(200,249,248), RGB(0,0,0), m_ptUserInfo[i].x - 37, m_ptUserInfo[i].y + m_ImageUserInfoV.GetHeight()/2 + 2);
				}
				else
				{
					pDC->SetTextAlign(TA_RIGHT);
					DrawTextStringEx(pDC, szBuffer, RGB(200,249,248), RGB(0,0,0), m_ptUserInfo[i].x + 37, m_ptUserInfo[i].y + m_ImageUserInfoV.GetHeight()/2 + 2);
				}
				pDC->SetTextAlign(oldAlign);
			}
		}
		else
		{
			ptShowPos.SetPoint(m_ptUserInfo[i].x - m_ImageUserInfoH.GetWidth()/2, m_ptUserInfo[i].y - m_ImageUserInfoH.GetHeight()/2);
			
			// ����
			m_ImageUserInfoH.DrawImage( pDC, ptShowPos.x, ptShowPos.y);

			// ���
			if ( pIClientUserItem->GetUserStatus() == US_PLAYING && GetUserClock(i) )
			{
				m_ImageUserPalyH.DrawImage( pDC, m_ptUserInfo[i].x - m_ImageUserPalyH.GetWidth()/2, m_ptUserInfo[i].y - m_ImageUserPalyH.GetHeight()/2);
			}

			// ͷ��
			m_ptAvatar[i].SetPoint(ptShowPos.x + 9, ptShowPos.y + 9);
			DrawUserAvatar(pDC, m_ptAvatar[i].x, m_ptAvatar[i].y, pIClientUserItem);

			// ����
			CRect rectInfo( ptShowPos.x + 64, ptShowPos.y + 11, ptShowPos.x + 162, ptShowPos.y + 23);
			DrawTextStringEx(pDC,pIClientUserItem->GetNickName(),RGB(255,255,255),RGB(0,0,0),rectInfo, DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE|DT_LEFT);
		
			// �û��ȼ�
			rectInfo.OffsetRect( 0, 16 );
			DrawTextStringEx(pDC,GetLevelDescribe(pIClientUserItem),RGB(153,255,0),RGB(0,0,0),rectInfo, DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE|DT_LEFT);

			//�û�����
			rectInfo.OffsetRect( 0, 16 );
			DrawTextStringEx(pDC,AddComma(m_lUserScore[i]),RGB(255,255,0),RGB(0,0,0),rectInfo, DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE|DT_LEFT);

			// �û���ע
			if( m_lTableScore[i] > 0L )
			{
				UINT oldAlign = pDC->SetTextAlign(TA_CENTER);
				TCHAR szBuffer[128];
				_sntprintf(szBuffer,CountArray(szBuffer),TEXT("����ע��%s"),AddComma(m_lTableScore[i]));

				if ( i == 2 )
					DrawTextStringEx(pDC, szBuffer, RGB(200,249,248), RGB(0,0,0), m_ptUserInfo[i].x, m_ptUserInfo[i].y - m_ImageUserInfoH.GetHeight()/2 - 12);
				else
					DrawTextStringEx(pDC, szBuffer, RGB(200,249,248), RGB(0,0,0), m_ptUserInfo[i].x, m_ptUserInfo[i].y + m_ImageUserInfoH.GetHeight()/2 + 2);

				pDC->SetTextAlign(oldAlign);
			}
		}
	}

	// �ƶ�����
	for ( int i = 0 ; i < m_ArrayBetChip.GetCount(); ++i )
	{
		// ��ȡ����
		INT nChipHeight = m_ImageChip.GetHeight();
		INT nChipWidth  = m_ImageChip.GetWidth() / CHIP_COUNT;

		// �滭����
		m_ImageChip.DrawImage( pDC, (INT)m_ArrayBetChip[i]->ptCurrentPos.x - nChipWidth/2, (INT)m_ArrayBetChip[i]->ptCurrentPos.y - nChipHeight / 2,
			nChipWidth, nChipHeight, 
			nChipWidth * m_ArrayBetChip[i]->wChipIndex, 0 );
	}

	// ���ճ���
	for ( int i = 0 ; i < m_ArrayOverChip.GetCount(); ++i )
	{
		// ��ȡ����
		INT nChipHeight = m_ImageChip.GetHeight();
		INT nChipWidth  = m_ImageChip.GetWidth() / CHIP_COUNT;

		// �滭����
		if ( !m_ArrayOverChip[i]->bShow )
			continue;

		m_ImageChip.DrawImage( pDC, (INT)m_ArrayOverChip[i]->ptCurrentPos.x - nChipWidth/2, (INT)m_ArrayOverChip[i]->ptCurrentPos.y - nChipHeight / 2,
			nChipWidth, nChipHeight, 
			nChipWidth * m_ArrayOverChip[i]->wChipIndex, 0 );
	}

	//�û���ǰ����
	int nChipCount[GAME_PLAYER] = {0};
	for ( int i = 0 ; i < m_ArrayPlayChip.GetCount(); ++i )
	{
		// ��ȡ����
		INT nChipHeight = m_ImageChip.GetHeight();
		INT nChipWidth  = m_ImageChip.GetWidth() / CHIP_COUNT;

		if( m_ArrayPlayChip[i]->wChipChairID < GAME_PLAYER && nChipCount[m_ArrayPlayChip[i]->wChipChairID] <= PILING_HEIGHT )
			nChipCount[m_ArrayPlayChip[i]->wChipChairID]++;

		if( nChipCount[m_ArrayPlayChip[i]->wChipChairID] >= PILING_HEIGHT )
			continue;

		// �滭����
		m_ImageChip.DrawImage( pDC, (INT)m_ArrayPlayChip[i]->ptCurrentPos.x - nChipWidth/2, (INT)m_ArrayPlayChip[i]->ptCurrentPos.y - nChipHeight / 2,
			nChipWidth, nChipHeight, 
			nChipWidth * m_ArrayPlayChip[i]->wChipIndex, 0 );
	}

	//�û���ǰֹͣ����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IClientUserItem * pIClientUserItem=GetClientUserItem(i);
		if (pIClientUserItem == NULL) continue;

		LONGLONG lPalyBetChip = m_lPalyBetChip[i];
		LONGLONG lScoreArray[] = { 1L, 5L, 10L, 50L, 100L, 500L, 1000L, 5000L, 10000L, 50000L, 100000L, 500000L, 1000000L, 5000000L };
		UINT	 nChipCount    = 0;
		while ( lPalyBetChip > 0  && nChipCount < PILING_HEIGHT )
		{
			for( int j = CountArray(lScoreArray) - 1; j >=0; j-- )
			{
				if ( lPalyBetChip >= lScoreArray[j] )
				{
					// ��ȡ����
					INT nChipHeight = m_ImageChip.GetHeight();
					INT nChipWidth  = m_ImageChip.GetWidth() / CHIP_COUNT;
					INT nXPos = m_ptPalyBetChip[i].x - nChipWidth/2;
					INT nYPos = m_ptPalyBetChip[i].y - nChipHeight/2 - nChipCount * 5;
					m_ImageChip.DrawImage(pDC, nXPos, nYPos, nChipWidth, nChipHeight, nChipWidth * j, 0);
					lPalyBetChip -= lScoreArray[j];
					nChipCount++;
					break;
				}
			}
		}


		//��������
		if( m_lPalyBetChip[i] > 0L )
		{
			UINT oldAlign = pDC->SetTextAlign(TA_CENTER);
			DrawTextStringEx(pDC, AddComma(m_lPalyBetChip[i]), RGB(200,249,248), RGB(0,0,0), m_ptPalyBetChip[i].x,m_ptPalyBetChip[i].y + 15);
			pDC->SetTextAlign(oldAlign);
		}
	}

	//�滭�����˿�
	if ( m_SendCardItemArray.GetCount() > 0 )
	{
		//��������
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

		//��ȡ��С
		int nItemWidth=m_SizeGameCard.cx;
		int nItemHeight=m_SizeGameCard.cy;

		//�滭�˿�
		m_ImageGameCard.DrawImage(pDC,m_SendCardCurPos.x,m_SendCardCurPos.y,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4);
	}

	//�û�����
	if ( m_wActionUser != INVALID_CHAIR && m_lOverScore[m_wActionUser] == 0 )
	{
		if ( m_cbUserAction != AC_SHOW_HAND )
		{
			//����λ��
			INT nXPos=m_ptUserAction[m_wActionUser].x-m_ImageActionBack.GetWidth()/10;
			INT nYPos=m_ptUserAction[m_wActionUser].y-m_ImageActionBack.GetHeight()/2;

			//�滭����
			m_ImageActionBack.DrawImage(pDC,nXPos ,nYPos, 
				m_ImageActionBack.GetWidth() / 5,	m_ImageActionBack.GetHeight(),
				m_wActionUser * m_ImageActionBack.GetWidth() / 5, 0);

			//�滭����
			m_ImageActionFont.DrawImage(pDC,nXPos + 12, nYPos + 7,
				m_ImageActionFont.GetWidth() / 5,	m_ImageActionFont.GetHeight(),
				m_cbUserAction * m_ImageActionFont.GetWidth() / 5, 0);
		}
		else
		{
			//�������
			m_ImageShowHand.DrawImage(pDC,(nWidth-m_ImageShowHand.GetWidth())/2,(nHeight-m_ImageShowHand.GetHeight())/2);
		}
	}
	
	// ������Ϣ
	for ( int i = 0; i < GAME_PLAYER; ++i )
	{
		DrawOverScore(pDC, m_lOverScore[i], m_ptOverScore[i].x, m_ptOverScore[i].y);

		//��ȡ�û�
		IClientUserItem * pIClientUserItem = GetClientUserItem(i);

		if ( m_lOverScore[i] != 0 && m_CardControl[i].GetCardCount() == MAX_CARD_COUNT && m_CardControl[i].GetDisplayHead()
			&& ( pIClientUserItem == NULL || pIClientUserItem->GetUserStatus() != US_READY))
		{
			BYTE cbCrad[MAX_CARD_COUNT] = {0};
			m_CardControl[i].GetCardData( cbCrad, MAX_CARD_COUNT );

			//����λ��
			CPoint ptCenterPoint;
			m_CardControl[i].GetCenterPoint(ptCenterPoint);
			INT nXPos = ptCenterPoint.x - m_ImageCradType.GetWidth()/18;
			INT nYPos = ptCenterPoint.y - m_ImageCradType.GetHeight()/2;

			//�滭����
			m_GameLogic.SortCardList( cbCrad, MAX_CARD_COUNT);
			m_ImageCradType.DrawImage(pDC,nXPos, nYPos ,
				m_ImageCradType.GetWidth() / 9,	m_ImageCradType.GetHeight(),
				(m_GameLogic.GetCardGenre( cbCrad, (BYTE)m_CardControl[i].GetCardCount() ) - 1) * m_ImageCradType.GetWidth() / 9, 0);
		}
	}
	
	//׼����־
	for( int i = 0; i < GAME_PLAYER; ++i )
	{
		//��ȡ�û�
		IClientUserItem * pIClientUserItem=GetClientUserItem(i);
		if (pIClientUserItem == NULL) continue;

		if (pIClientUserItem->GetUserStatus() == US_READY)
			m_ImageReady.DrawImage(pDC, m_ptReady[i].x - m_ImageReady.GetWidth()/2, m_ptReady[i].y - m_ImageReady.GetHeight()/2);
	}

	// ��������
	m_PlayOperate.DrawPlayOperate(pDC,nWidth,nHeight);

	return;
}

//�Ƿ���
bool CGameClientView::IsDispatchStatus()
{
	return m_SendCardItemArray.GetCount()>0;
}

//ֹͣ����
VOID CGameClientView::ConcludeDispatch()
{
	//����ж�
	if (m_SendCardItemArray.GetCount()==0) return;

	//��ɶ���
	for (INT_PTR i=0;i<m_SendCardItemArray.GetCount();i++)
	{
		//��ȡ����
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[i];

		//��ȡ�˿�
		BYTE cbCardData[MAX_COUNT];
		WORD wChairID=pSendCardItem->wChairID;
		BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

		//�����˿�
		if (cbCardCount<MAX_COUNT)
		{
			cbCardData[cbCardCount++]=pSendCardItem->cbCardData;
			m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
		}
	}

	//ɾ������
	KillTimer(IDI_SEND_CARD);
	m_SendCardItemArray.RemoveAll();

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//�����˿�
VOID CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;

	//���ñ���
	tagSendCardItem SendCardItem;
	SendCardItem.wChairID=wChairID;
	SendCardItem.cbCardData=cbCardData;
	m_SendCardItemArray.Add(SendCardItem);

	//��������
	if (m_SendCardItemArray.GetCount()==1) 
	{
		//���ö�ʱ��
		SetTimer(IDI_SEND_CARD,SPEED_SEND_CARD,NULL);

		//
		m_nStepCount = SEND_STEP_COUNT;
		m_SendCardCurPos = m_SendCardPos;
		m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
		m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

		InvalidGameView(0,0,0,0);

		//��������
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		pGlobalUnits->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
	}

	return;
}

//��Ϸ��ע
bool CGameClientView::SetCellScore(LONG lCellScore)
{
	m_PlayOperate.SetCellScore(lCellScore);
	if (m_lCellScore!=lCellScore)
	{
		//���ñ���
		m_lCellScore=lCellScore;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	return true;
}

//�����ע
bool CGameClientView::SetDrawMaxScore(LONGLONG lDrawMaxScore)
{
	if (m_lDrawMaxScore!=lDrawMaxScore)
	{
		//���ñ���
		m_lDrawMaxScore=lDrawMaxScore;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	return true;
}

//���û���
bool CGameClientView::SetUserScore(WORD wChairID, LONGLONG lUserScore)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//���û���
	if (m_lUserScore[wChairID]!=lUserScore)
	{
		m_lUserScore[wChairID] = lUserScore;
		m_lUserSaveScore[wChairID] = lUserScore;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	return true;
}

//���û���
bool CGameClientView::SetUserSaveScore(WORD wChairID, LONGLONG lUserScore)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//���û���
	if (m_lUserSaveScore[wChairID] != lUserScore)
	{
		m_lUserAddScore[wChairID] = 0;
		m_lUserAddAllScore[wChairID] = lUserScore - m_lUserScore[wChairID];
		m_lUserSaveScore[wChairID] = lUserScore;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	return true;
}

//���ý�������
bool CGameClientView::SetOverScore(WORD wChairID, LONGLONG lUserScore)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//���û���
	if (m_lOverScore[wChairID] != lUserScore)
	{
		m_lOverScore[wChairID] = 0;
		m_lOverSaveScore[wChairID] = lUserScore;
	}

	return true;
}

//������ע
void CGameClientView::SetUserTableScore(WORD wChairID, LONGLONG lTableScore)
{
	//��������
	if (wChairID!=INVALID_CHAIR) m_lTableScore[wChairID]=lTableScore;
	else ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//���ö���
bool CGameClientView::SetUserAction(WORD wActionUser, BYTE cbUserAction)
{
	//���ö���
	if ((m_wActionUser!=wActionUser)||(m_cbUserAction!=cbUserAction))
	{
		//���ñ���
		m_wActionUser=wActionUser;
		m_cbUserAction=cbUserAction;

		//���½���
		InvalidGameView(0,0,0,0);

		//����ʱ��
		if (m_wActionUser!=INVALID_CHAIR) SetTimer(IDI_USER_ACTION,3000,NULL);
	}

	return true;
}

//���û���
bool CGameClientView::SetHistoryScore(WORD wChairID,tagHistoryScore * pHistoryScore)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//���ñ���
	m_pHistoryScore[wChairID]=pHistoryScore;

	//���½���
	InvalidGameView(0,0,0,0);

	return true;
}

// �滭����
void CGameClientView::DrawNumber( CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum,INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),TEXT("%I64d"),lOutNum);
	DrawNumber(pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos, uFormat);
}

// �滭����
void CGameClientView::DrawNumber(CDC * pDC , CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat /*= DT_LEFT*/)
{
	// ������Դ
	INT nNumberHeight=ImageNumber->GetHeight();
	INT nNumberWidth=ImageNumber->GetWidth()/lstrlen(szImageNum);

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
				ImageNumber->DrawImage(pDC, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
				nXPos += nNumberWidth;
				break;
			}
		}
	}
}


// �滭��������
void CGameClientView::DrawOverScore( CDC * pDC, LONGLONG lOutNum, INT nXPos, INT nYPos )
{
	if( lOutNum == 0 )
		return;

	CPngImage* ImageNumber = NULL;
	TCHAR szOutNum[128] = {0};
	TCHAR szImageNum[128] = { TEXT("0123456789") };
	_sntprintf(szOutNum,CountArray(szOutNum),TEXT("%I64d"),lOutNum);

	if ( lOutNum > 0 )
		ImageNumber = &m_ImgaeScoreWin;
	else
		ImageNumber = &m_ImgaeScoreLose;

	INT nNumberHeight = ImageNumber->GetHeight();
	INT nNumberWidth  = ImageNumber->GetWidth() / 10;

	nXPos -= ((INT)(((DOUBLE)(lstrlen(szOutNum)) / 2.0) * nNumberWidth) + m_ImgaeScoreSum.GetWidth() / 4);
	nYPos -= ImageNumber->GetHeight() / 2;

	int nBackX = nXPos + 35;
	int nBackY = nYPos + ImageNumber->GetHeight() / 2 - m_ImgaeScoreBackL.GetHeight() / 2 - 1;
	int nBackWidth = (int)lstrlen(szOutNum) * nNumberWidth + m_ImgaeScoreSum.GetWidth() / 2 - 35;

	if ( lOutNum < 0 )
		nBackWidth -= nNumberWidth;

	m_ImgaeScoreBackL.DrawImage( pDC, nXPos - 15, nBackY);
	for( int i = 0; i < nBackWidth; i += m_ImgaeScoreBackM.GetWidth() )
	{
		m_ImgaeScoreBackM.DrawImage( pDC, nBackX, nBackY);
		nBackX += m_ImgaeScoreBackM.GetWidth();
	}
	m_ImgaeScoreBackR.DrawImage( pDC, nBackX, nBackY);


	if ( lOutNum > 0)
		m_ImgaeScoreSum.DrawImage( pDC, nXPos, nYPos, m_ImgaeScoreSum.GetWidth()/2, m_ImgaeScoreSum.GetHeight(), 0, 0);
	else
		m_ImgaeScoreSum.DrawImage( pDC, nXPos, nYPos, m_ImgaeScoreSum.GetWidth()/2, m_ImgaeScoreSum.GetHeight(), m_ImgaeScoreSum.GetWidth()/2, 0);

	nXPos += m_ImgaeScoreSum.GetWidth()/2;

	for ( INT i = 0; i < lstrlen(szOutNum); ++i )
	{
		for ( INT j = 0; j < lstrlen(szImageNum); ++j )
		{
			if ( szOutNum[i] == szImageNum[j] && szOutNum[i] != '\0' )
			{
				ImageNumber->DrawImage(pDC, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
				nXPos += nNumberWidth;
				break;
			}
		}
	}
}


//��ʼ��ť
VOID CGameClientView::OnBnClickedStart()
{
	//������Ϣ
	SendEngineMessage(IDM_START,0,0);

	return;
}

//������ť
VOID CGameClientView::OnBnClickedGiveUp()
{
	//������Ϣ
	SendEngineMessage(IDM_GIVE_UP,0,0);

	return;
}

//��ע��ť
VOID CGameClientView::OnBnClickedFollow()
{
	//������Ϣ
	SendEngineMessage(IDM_FOLLOW,0,0);

	return;
}

//��ע��ť
VOID CGameClientView::OnBnClickedAddScore()
{
	//������Ϣ
	SendEngineMessage(IDM_FOLLOW,(WPARAM)m_PlayOperate.GetBetCount(),0);

	return;
}

//��ע��ť
VOID CGameClientView::OnBnClickedAddScoreOne()
{
	//������Ϣ
	SendEngineMessage(IDM_FOLLOW,m_lCellScore,0);
}

//��ע��ť
VOID CGameClientView::OnBnClickedAddScoreTwo()
{
	//������Ϣ
	SendEngineMessage(IDM_FOLLOW,m_lCellScore*2,0);
}

//��ע��ť
VOID CGameClientView::OnBnClickedAddScoreThree()
{
	//������Ϣ
	SendEngineMessage(IDM_FOLLOW,m_lCellScore*3,0);
}

//��ע��ť
VOID CGameClientView::OnBnClickedAddScoreFour()
{
	//������Ϣ
	SendEngineMessage(IDM_FOLLOW,m_lCellScore*4,0);
}

//�����ť
VOID CGameClientView::OnBnClickedShowHand()
{
	//������Ϣ
	SendEngineMessage(IDM_SHOW_HAND,0,0);

	return;
}

//���ְ�ť
VOID CGameClientView::OnBnClickedADD()
{
	m_PlayOperate.ScoreADD();

	CRect rect = m_PlayOperate.GetOperateRect();
	InvalidGameView(rect.left, rect.top, rect.Width(), rect.Height());
}

//���ְ�ť
VOID CGameClientView::OnBnClickedSUB()
{
	m_PlayOperate.ScoreSUB();

	CRect rect = m_PlayOperate.GetOperateRect();
	InvalidGameView(rect.left, rect.top, rect.Width(), rect.Height());
}

//ʱ����Ϣ
VOID CGameClientView::OnTimer(UINT nIDEvent)
{
	//��Ϣ����
	switch (nIDEvent)
	{
	case IDI_HIDE_CARD:		//�����˿�
		{
			//ɾ��ʱ��
			KillTimer(IDI_HIDE_CARD);

			//�����ж�
			IClientUserItem * pIClientUserItem=GetClientUserItem(MYSELF_VIEW_ID);
			if( pIClientUserItem == NULL )
				return;

			if ( m_lOverScore[MYSELF_VIEW_ID] == 0 && m_CardControl[MYSELF_VIEW_ID].GetDisplayHead() && m_CardControl[MYSELF_VIEW_ID].GetPositively() && pIClientUserItem->GetUserStatus() == US_PLAYING )
			{
				//���ÿؼ�
				m_CardControl[MYSELF_VIEW_ID].SetDisplayHead(false);

				//���½���
				InvalidGameView(0,0,0,0);
			}

			return;
		}
	case IDI_SEND_CARD:		//���ƶ���
		{
			//����λ��
			m_SendCardCurPos.x += m_nXStep;
			m_SendCardCurPos.y += m_nYStep;

			//���½���
			InvalidGameView(0,0,0,0);

			//ֹͣ�ж�
			if ( --m_nStepCount == 0 )
			{
				//��ȡ�˿�
				BYTE cbCardData[MAX_COUNT];
				WORD wChairID=m_SendCardItemArray[0].wChairID;
				BYTE cbCardCount=(BYTE)m_CardControl[wChairID].GetCardData(cbCardData,CountArray(cbCardData));

				//�����˿�
				if (cbCardCount<MAX_COUNT)
				{
					cbCardData[cbCardCount++]=m_SendCardItemArray[0].cbCardData;
					m_CardControl[wChairID].SetCardData(cbCardData,cbCardCount);
				}

				//ɾ������
				m_SendCardItemArray.RemoveAt(0);

				//��������
				if (m_SendCardItemArray.GetCount()>0)
				{
					//
					wChairID = m_SendCardItemArray[0].wChairID;
					m_nStepCount = SEND_STEP_COUNT;
					m_SendCardCurPos = m_SendCardPos;
					m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
					m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

					//��������
					CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					pGlobalUnits->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));

					return;
				}
				else 
				{
					//��ɴ���
					KillTimer(IDI_SEND_CARD);
					//������Ϣ
					SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

					return;
				}
			}

			return;
		}
	case IDI_USER_ACTION:	//�û�����
		{
			//���ñ���
			m_cbUserAction=0;
			m_wActionUser=INVALID_CHAIR;

			//���½���
			InvalidGameView(0,0,0,0);

			return;
		}
	case IDI_MOVE_CHIP:
		{
			BOOL bUpdateGameView = FALSE;
			for ( int i = 0 ; i < m_ArrayBetChip.GetCount(); ++i )
			{
				if( m_ArrayBetChip[i]->Move() )
				{
					bUpdateGameView = TRUE;
				}
			}

			int nIndex = 0;
			while ( nIndex < m_ArrayPlayChip.GetCount())
			{
				if( m_ArrayPlayChip[nIndex]->Move() )
				{
					bUpdateGameView = TRUE;
					nIndex++;
				}
				else
				{
					bUpdateGameView = TRUE;

					// ��������
					LONGLONG lScoreArray[] = { 1L, 5L, 10L, 50L, 100L, 500L, 1000L, 5000L, 10000L, 50000L, 100000L, 500000L, 1000000L, 5000000L };

					// ��ӷ���
					m_lPalyBetChip[m_ArrayPlayChip[nIndex]->wChipChairID] += lScoreArray[m_ArrayPlayChip[nIndex]->wChipIndex];

					// ɾ������
					SafeDelete(m_ArrayPlayChip[nIndex]);
					m_ArrayPlayChip.RemoveAt(nIndex);

				}
			}

			// ��ӻ���
			if ( m_ArrayOverChip.GetCount() )
			{
				// ����λ��
				WORD   wChairID = m_ArrayOverChip[0]->wChipChairID;
				CPoint ptBenchmarkPos = m_ptAvatar[wChairID];
				ptBenchmarkPos.Offset( 25, 25 );

				// ��ӻ���
				if ( m_ArrayBetChip.GetCount() )
				{
					INT_PTR nMaxIndex = m_ArrayBetChip.GetCount()/5;
					if ( nMaxIndex <= 0 )
						nMaxIndex = 1;

					while ( nMaxIndex > 0 )
					{
						stuMoveChip* pMoveChip = new stuMoveChip(m_ArrayBetChip[0]->wChipIndex, wChairID, 10, m_ArrayBetChip[0]->ptCurrentPos, ptBenchmarkPos, FALSE);
						m_ArrayOverChip.Add(pMoveChip);
						SafeDelete(m_ArrayBetChip[0]);
						m_ArrayBetChip.RemoveAt(0);
						nMaxIndex--;
					}

				}

				// ��ӻ���
				if ( m_ArrayPlayChip.GetCount() )
				{
					INT_PTR nMaxIndex = m_ArrayPlayChip.GetCount()/5;
					if ( nMaxIndex <= 0 )
						nMaxIndex = 1;

					while ( nMaxIndex > 0 )
					{
						stuMoveChip* pMoveChip = new stuMoveChip(m_ArrayPlayChip[0]->wChipIndex, wChairID, 10, m_ArrayPlayChip[0]->ptCurrentPos, ptBenchmarkPos, FALSE);
						m_ArrayOverChip.Add(pMoveChip);
						SafeDelete(m_ArrayPlayChip[0]);
						m_ArrayPlayChip.RemoveAt(0);
						nMaxIndex--;
					}
				}
				

				// ��ӻ���
				LONGLONG lScoreArray[] = { 1L, 5L, 10L, 50L, 100L, 500L, 1000L, 5000L, 10000L, 50000L, 100000L, 500000L, 1000000L, 5000000L };
			
				for ( int nSite = 0; nSite < GAME_PLAYER; ++nSite )
				{
					int nChipCount = 0;
					while ( m_lPalyBetChip[nSite] > 0 )
					{
						for( int j = CountArray(lScoreArray) - 1; j >=0; j-- )
						{
							if ( m_lPalyBetChip[nSite] >= lScoreArray[j] )
							{	
								CPoint ptBegin(m_ptPalyBetChip[nSite]);
								CPoint ptEnd(ptBenchmarkPos);
								ptEnd.y -= nChipCount * 5;
								ptBegin.y -= nChipCount * 5;
								stuMoveChip* pMoveChip = new stuMoveChip(j, wChairID, 10, ptBegin, ptEnd, FALSE);
								if ( nChipCount >= PILING_HEIGHT )
									pMoveChip->SetShow(FALSE);
								
								m_ArrayOverChip.Add(pMoveChip);
								m_lPalyBetChip[nSite] -= lScoreArray[j];
								nChipCount++;
								break;
							}
						}
					}
				}
			}

			// �ƶ����ն���
			nIndex = 0;
			while ( nIndex < m_ArrayOverChip.GetCount())
			{
				if( m_ArrayOverChip[nIndex]->Move() )
				{
					bUpdateGameView = TRUE;
					nIndex++;
				}
				else
				{
					// ��������
					WORD wChipChairID = m_ArrayOverChip[nIndex]->wChipChairID;
					LONGLONG lScoreArray[] = { 1L, 5L, 10L, 50L, 100L, 500L, 1000L, 5000L, 10000L, 50000L, 100000L, 500000L, 1000000L, 5000000L };

					// ��ӷ���
					m_lUserScore[wChipChairID] += lScoreArray[m_ArrayOverChip[nIndex]->wChipIndex];
					m_lUserAddScore[wChipChairID] += lScoreArray[m_ArrayOverChip[nIndex]->wChipIndex];

					if ( m_lUserAddScore[wChipChairID] > m_lUserAddAllScore[wChipChairID] )
						m_lUserAddScore[wChipChairID] = m_lUserAddAllScore[wChipChairID];


					for ( int i = 0; i < GAME_PLAYER; ++i )
					{
						if ( m_lUserAddAllScore[i] == 0 || m_lOverSaveScore[i] == 0 )
							continue;

						m_lOverScore[i] = (LONGLONG)( m_lOverSaveScore[i] * m_lUserAddScore[wChipChairID] / m_lUserAddAllScore[wChipChairID]);
					}

					// ɾ������
					SafeDelete(m_ArrayOverChip[nIndex]);
					m_ArrayOverChip.RemoveAt(nIndex);

					bUpdateGameView = TRUE;

					if ( m_ArrayOverChip.GetCount() == 0 )
					{
						memcpy(m_lUserScore, m_lUserSaveScore, sizeof(m_lUserScore));
						memcpy(m_lOverScore, m_lOverSaveScore, sizeof(m_lOverScore));
					}
				}
			}


			//���½���
			if ( bUpdateGameView )
			{
				InvalidGameView( 0,0,0,0 );
			}
			
			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//��������
INT CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_PlayOperate.Initialization(this);

	//��Դ����
	HINSTANCE hResInstance=AfxGetInstanceHandle();

	//���ư�ť
	m_btStart.Create(NULL,WS_CHILD,rcCreate,this,IDC_START);

	//���ư�ť
	m_btStart.SetButtonImage(IDB_BT_START,hResInstance,false,false);

	//������ʾ
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btStart,TEXT("��ʼ��Ϸ"));
	m_ToolTipCtrl.AddTool(&m_PlayOperate.m_btFollow,TEXT("��ע"));
	m_ToolTipCtrl.AddTool(&m_PlayOperate.m_btGiveUp,TEXT("����"));
	m_ToolTipCtrl.AddTool(&m_PlayOperate.m_btAddScore,TEXT("��ע"));
	m_ToolTipCtrl.AddTool(&m_PlayOperate.m_btShowHand,TEXT("���"));

	m_DFontEx.CreateFont(this, TEXT("����"), 12, 400 );

	// ���������ƶ�
	SetTimer(IDI_MOVE_CHIP, 30, NULL);

	return 0;
}

//�����Ϣ
VOID CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//for ( int i = 0 ; i < GAME_PLAYER; ++i )
	//{
	//	BYTE cbCrad[5] = { 0x09,0x19,0x29,0x0C,0x0A };
	//	m_CardControl[i].SetCardData( cbCrad, 5);
	//}
	//
	//InvalidGameView(0,0,0,0);

	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�˿˵��
	if (m_CardControl[MYSELF_VIEW_ID].OnEventLeftMouseDown(MousePoint)==true)
	{
		//���½���
		InvalidGameView(0,0,0,0);

		//����ʱ��
		if (m_CardControl[MYSELF_VIEW_ID].GetDisplayHead()==true)
		{
			SetTimer(IDI_HIDE_CARD,2000,NULL);
		}
		else KillTimer(IDI_HIDE_CARD);
	}

	return;
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�˿˲���
	if (m_CardControl[MYSELF_VIEW_ID].OnEventSetCursor(MousePoint)==true) return TRUE;

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//�滭�ַ�
bool CGameClientView::DrawTextStringEx(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, INT nDrawFormat)
{
	//��������
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�����ʽ
	UINT nTextAlign=pDC->SetTextAlign(TA_TOP|TA_LEFT);

	//�滭�߿�
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		//����λ��
		CRect rcFrame;
		rcFrame.top=rcRect.top+nYExcursion[i];
		rcFrame.left=rcRect.left+nXExcursion[i];
		rcFrame.right=rcRect.right+nXExcursion[i];
		rcFrame.bottom=rcRect.bottom+nYExcursion[i];

		//�滭�ַ�
		m_DFontEx.DrawText(pDC,pszString,&rcFrame,crFrame,nDrawFormat);
	}

	//�滭�ַ�
	m_DFontEx.DrawText(pDC,pszString,&rcRect,crText,nDrawFormat);

	//��ԭ��ʽ
	pDC->SetTextAlign(nTextAlign);

	return true;
}

// �滭�ַ�
bool CGameClientView::DrawTextStringEx( CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, INT nXPos, INT nYPos )
{
	//��������
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�����ʽ
	UINT nTextAlign=pDC->GetTextAlign();
	INT nDrawFormat=0;

	if ( nTextAlign&TA_CENTER )
		nDrawFormat |= DT_CENTER;
	else if( nTextAlign&TA_RIGHT )
		nDrawFormat |= DT_RIGHT;
	else
		nDrawFormat |= DT_LEFT;

	if( nTextAlign&TA_BOTTOM )
		nDrawFormat |= DT_BOTTOM;
	else 
		nDrawFormat |= DT_TOP;

	//�滭�߿�
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		//����λ��
		int nYPosT=nYPos+nYExcursion[i];
		int nXPosT=nXPos+nXExcursion[i];

		//�滭�ַ�
		m_DFontEx.DrawText(pDC,pszString,nXPosT,nYPosT,crFrame,nDrawFormat);
	}

	//�滭�ַ�
	m_DFontEx.DrawText(pDC,pszString,nXPos,nYPos,crText,nDrawFormat);

	return true;
}

// ���������ע
void CGameClientView::AddDeskChip( LONGLONG lScore, WORD wChairID )
{

	// ��������
	LONGLONG lScoreArray[] = { 1L, 5L, 10L, 50L, 100L, 500L, 1000L, 5000L, 10000L, 50000L, 100000L, 500000L, 1000000L, 5000000L };
	UINT	 nChipCount    = 0;

	// ɾ����ע
	if( wChairID <= GAME_PLAYER )
	{
		int nIndex = 0;
		while( nIndex < m_ArrayPlayChip.GetCount() )
		{	
			if( m_ArrayPlayChip[nIndex]->wChipChairID == wChairID )
			{
				WORD	wChipIndex; 
				UINT	nAllTime; 
				DPOINT	ptBeginPos; 
				DPOINT	ptEndPos;

				wChipIndex = m_ArrayPlayChip[nIndex]->wChipIndex;
				nAllTime = 8;
				CPoint ptBenchmarkPos = m_ptPalyBetChip[wChairID];
				ptBeginPos.SetPoint( m_ArrayPlayChip[nIndex]->ptCurrentPos.x, m_ArrayPlayChip[nIndex]->ptCurrentPos.y);
				ptEndPos.SetPoint( m_rectBetChip.left + rand()%m_rectBetChip.Width(), m_rectBetChip.top + rand()%m_rectBetChip.Height());
				stuMoveChip* pMoveChip = new stuMoveChip(wChipIndex, wChairID, nAllTime, ptBeginPos, ptEndPos, TRUE);
				m_ArrayBetChip.Add(pMoveChip);
				lScore -= lScoreArray[m_ArrayPlayChip[nIndex]->wChipIndex];
				SafeDelete(m_ArrayPlayChip[nIndex]);
				m_ArrayPlayChip.RemoveAt(nIndex);
				continue;
			}
			nIndex++;
		}
	}

	// ��ȡ����
	nChipCount    = 0;
	while ( lScore > 0 )
	{
		for( int i = CountArray(lScoreArray) - 1; i >=0; i-- )
		{
			if ( lScore >= lScoreArray[i] )
			{
				WORD	wChipIndex; 
				UINT	nAllTime; 
				DPOINT	ptBeginPos; 
				DPOINT	ptEndPos;

				if ( wChairID >= GAME_PLAYER )
				{
					wChipIndex = i;
					nAllTime = 0;
					ptBeginPos.SetPoint( m_rectBetChip.left + rand()%m_rectBetChip.Width(), m_rectBetChip.top + rand()%m_rectBetChip.Height());
					ptEndPos = ptBeginPos;
				}
				else
				{
					wChipIndex = i;
					nAllTime = 8;
					CPoint ptBenchmarkPos = m_ptPalyBetChip[wChairID];
					ptBeginPos.SetPoint( ptBenchmarkPos.x, ptBenchmarkPos.y - nChipCount * 5);
					ptEndPos.SetPoint( m_rectBetChip.left + rand()%m_rectBetChip.Width(), m_rectBetChip.top + rand()%m_rectBetChip.Height());
				}
				stuMoveChip* pMoveChip = new stuMoveChip(wChipIndex, wChairID, nAllTime, ptBeginPos, ptEndPos, TRUE);
				m_ArrayBetChip.Add(pMoveChip);
				lScore -= lScoreArray[i];
				if ( nChipCount < PILING_HEIGHT )
					nChipCount++;

				break;
			}
		}
	}
}


// ��������ע
void CGameClientView::AddPlayChip( LONGLONG lScore, WORD wChairID )
{
	if( wChairID >= GAME_PLAYER )
		return;

	// ��������
	LONGLONG lScoreArray[] = { 1L, 5L, 10L, 50L, 100L, 500L, 1000L, 5000L, 10000L, 50000L, 100000L, 500000L, 1000000L, 5000000L };

	// ��ȡ����
	while ( lScore > 0 )
	{
		for( int i = CountArray(lScoreArray) - 1; i >=0; i-- )
		{
			if ( lScore >= lScoreArray[i] )
			{
				WORD	wChipIndex; 
				UINT	nAllTime; 
				DPOINT	ptBeginPos; 
				DPOINT	ptEndPos;

				wChipIndex = i;
				nAllTime = 10;
				CPoint ptBenchmarkPos = m_ptAvatar[wChairID];
				ptBenchmarkPos.Offset( 25, 25 );
				ptBeginPos.SetPoint( ptBenchmarkPos.x, ptBenchmarkPos.y );
				ptEndPos.SetPoint( m_rectBetChip.left + rand()%m_rectBetChip.Width(), m_rectBetChip.top + rand()%m_rectBetChip.Height());

				stuMoveChip* pMoveChip = new stuMoveChip(wChipIndex, wChairID, nAllTime, ptBeginPos, ptEndPos, TRUE);
				m_ArrayBetChip.Add(pMoveChip);
				lScore -= lScoreArray[i];
				
				break;
			}
		}
	}
	
}

// �����Ҽ�ע
void CGameClientView::AddPlayBetChip( LONGLONG lScore, WORD wChairID )
{
	if( wChairID >= GAME_PLAYER )
		return;

	if ( lScore == 0 )
	{
		m_lPalyBetChip[wChairID] = lScore;
		return;
	}

	lScore -= m_lPalyBetChip[wChairID];

	// ��������
	LONGLONG lScoreArray[] = { 1L, 5L, 10L, 50L, 100L, 500L, 1000L, 5000L, 10000L, 50000L, 100000L, 500000L, 1000000L, 5000000L };
	UINT	 nChipCount    = 0;

	// ��ȡ����
	while ( lScore > 0 )
	{
		for( int i = CountArray(lScoreArray) - 1; i >=0; i-- )
		{
			if ( lScore >= lScoreArray[i] )
			{
				WORD	wChipIndex; 
				UINT	nAllTime; 
				DPOINT	ptBeginPos; 
				DPOINT	ptEndPos;

				wChipIndex = i;
				nAllTime = 15;
				CPoint ptBenchmarkPos = m_ptAvatar[wChairID];
				ptBenchmarkPos.Offset( 25, 25 );
				ptBeginPos.SetPoint( ptBenchmarkPos.x, ptBenchmarkPos.y - nChipCount * 5);
				ptEndPos.SetPoint( m_ptPalyBetChip[wChairID].x, m_ptPalyBetChip[wChairID].y - nChipCount * 5);

				stuMoveChip* pMoveChip = new stuMoveChip(wChipIndex, wChairID, nAllTime, ptBeginPos, ptEndPos, FALSE);
				m_ArrayPlayChip.Add(pMoveChip);
				lScore -= lScoreArray[i];

				if ( nChipCount < PILING_HEIGHT )
					nChipCount++;
				break;
			}
		}
	}
}

// �������
void CGameClientView::ChioRecovery( WORD wChairID )
{
	// ����λ��
	CPoint ptBenchmarkPos = m_ptAvatar[wChairID];
	ptBenchmarkPos.Offset( 25, 25 );

	// ��ӻ���
	if ( m_ArrayBetChip.GetCount() )
	{
		stuMoveChip* pMoveChip = new stuMoveChip(m_ArrayBetChip[0]->wChipIndex, wChairID, 10, m_ArrayBetChip[0]->ptCurrentPos, ptBenchmarkPos, FALSE);
		m_ArrayOverChip.Add(pMoveChip);
		SafeDelete(m_ArrayBetChip[0]);
		m_ArrayBetChip.RemoveAt(0);
	}
}

// ��Ϣ����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	return CGameFrameView::OnCommand(wParam, lParam);
}

// ��Ӷ���
CString CGameClientView::AddComma( LONGLONG lScore )
{
	CString strScore;
	CString strReturn;
	strScore.Format(TEXT("%I64d"), lScore);

	int nStrCount = 0;
	for( int i = strScore.GetLength() - 1; i >= 0; )
	{
		if( (nStrCount%4) == 0 && nStrCount != 0 )
		{
			strReturn.Insert(0, ',');
			nStrCount = 0;
		}
		else
		{
			strReturn.Insert(0, strScore.GetAt(i));
			nStrCount++;
			i--;
		}
	}
	return strReturn;
}
	
//////////////////////////////////////////////////////////////////////////


