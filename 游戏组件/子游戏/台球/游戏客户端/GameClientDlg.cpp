#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////
//�궨��


//////////////////////////////////////////////////////////////////////////
 
#define CARD_SPACE 16
BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameEngine)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
#define WAIT_TIME     20
#define WM_NEXT_SEND_CARD           150//xiaһ�����ƿ�ʼ

//���캯��
CGameClientDlg::CGameClientDlg() 
{
	//��Ϸ����
	m_bOutTimeTimes = 0;
	m_serverType = 0;
	
	m_bInited = false;
	m_bInited = true;
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
//	SetWindowText(TEXT("��������"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	srand(time(NULL)); 
	
	//���ÿؼ�
	m_wCurrentUser = GetMeChairID();	
	m_GameClientView.m_pParent = this;
	CRect rc;
	m_GameClientView.GetClientRect(rc);
	m_GameClientView.RectifyControl(rc.Width(),rc.Height());
	

	m_bInited = true;
	
	//m_pIStringMessage->InsertCustomString(TEXT("רҵ�������ο���,��Ϸ��ֲ,��ϵQQ 649662208"),RGB(255,0,255));
	return true;
} 
//���ÿ��
bool CGameClientDlg::OnResetGameEngine()
{
	//ɾ����ʱ
	KillGameClock(0);
	m_bOutTimeTimes = 0;

	return true;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound= true;//IsEnableSound();
	//��������
	if (GameOption.DoModal()==IDOK)
	{

		//���ÿؼ�
		//EnableSound(GameOption.m_bEnableSound);
	}
	if(GameOption.m_bEnableSound)
	{		
//		PlayGameBackSound("TaiQiu\\sound\\common\\bkmusic\\Background.mp3",true);
	 }
}
//ʱ��ɾ��
bool CGameClientDlg::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//ʱ����Ϣ
bool CGameClientDlg::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{ 
    switch(nTimerID)
	{
	case IDI_SET_BASEBALL:
	case IDI_HIT_BALL:
		{
			if(nElapse<=5&&nElapse>0)
			{
				m_GameClientView.PlayGameSound(IDS_CLOCK);
				TRACE("nTimerId=%d,nElapse=%d",nTimerID,nElapse);
			}
		}
		break;		 
	}
	return true;
}

//�Թ�״̬
bool CGameClientDlg::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
     return true;
}


//������Ϣ
bool CGameClientDlg::OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_CONFIG:		//��Ϸ����
		{	
			return true;
		}
	case SUB_S_SETBASEBALL:
		{
			return OnSubSetBaseBall(pBuffer,wDataSize);
		}
	case SUB_S_END_SETBASEBALL:
		{
			return OnSubEndSetBaseBall(pBuffer,wDataSize);
		}
	case SUB_S_END_SETBASEBALL_TIMEOUT:
		{
			m_GameClientView.m_coinMovieObverse.ForceStop();
			m_GameClientView.m_coinMovieReverse.ForceStop();
			m_GameClientView.m_directionMovie.ForceStop();
			m_GameClientView.m_movieGameStart.ForceStop();
			CMD_S_END_SET_BASEBALL_TIMEOUT *setBaseBall = (CMD_S_END_SET_BASEBALL_TIMEOUT*)pBuffer;
			m_GameClientView.m_balls[0].Ball_Pos.X = setBaseBall->ballPosx;
			m_GameClientView.m_balls[0].Ball_Pos.Y = setBaseBall->ballPosy;
			m_GameClientView.m_balls[0].BALL_bEnable = true;
		 
			m_GameClientView.m_baseBallState = waitHit;
			SetGameStatus(GS_WK_WAIT_HITBALL);
			KillGameClock(IDI_SET_BASEBALL);
			SetGameClock(m_wCurrentUser,IDI_HIT_BALL,TIME_HIT_BALL/1000);
			//��ʾ��ʱ��Ϣ
		//	AfxMessageBox("timeOut");
			TCHAR szBuffer[1024];
			 tagUserInfo * pUserData = m_GameClientView.GetClientUserItem(SwitchViewChairID(m_wCurrentUser))->GetUserInfo();
			_sntprintf(szBuffer,1024,TEXT("%s "),pUserData->szNickName);
			CString str;
			str.Format(TEXT("%s��ʱ%d��,��ʱ5������"),szBuffer,setBaseBall->timeOutCount);
			m_GameClientView.m_systemInfoList.AddTail(str);
			return true;
		}
	case SUB_S_HIT_TIMEOUT:
		{	
			
			CMD_S_HIT_TIMEOUT *hitTimeOut = (CMD_S_HIT_TIMEOUT*)pBuffer;
			m_wCurrentUser = hitTimeOut->currentUser;
			SetGameStatus(GS_WK_WAIT_HITBALL);
			KillGameClock(IDI_HIT_BALL);
			SetGameClock(m_wCurrentUser,IDI_HIT_BALL,TIME_HIT_BALL/1000);
			TCHAR szBuffer[1024];
			 tagUserInfo * pUserData=m_GameClientView.GetClientUserItem(SwitchViewChairID((m_wCurrentUser+1)%GAME_PLAYER))->GetUserInfo();
			_sntprintf(szBuffer,1024,TEXT("%s "),pUserData->szNickName);
			CString str;
			str.Format(TEXT("%s��ʱ%d��,��ʱ5������"),szBuffer,hitTimeOut->timeOutCount);
			m_GameClientView.m_systemInfoList.AddTail(str);
			return true;
		
		}
	case SUB_S_CLUBPOS:
		{
			return OnSubBallClubPos(pBuffer,wDataSize);
		}
	case SUB_S_HITBALL:
		{
			return OnSubHitBall(pBuffer,wDataSize);
		}
	case SUB_S_HIT_RESULT:
		{
			return OnSubHitResult(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	 
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}	 
	}

	return false;
}

//��Ϸ����
bool CGameClientDlg::OnEventSceneMessage(BYTE cbGameStation, bool bLookonUser, VOID * pBuffer, WORD wDataSize)
{ 
	switch (cbGameStation)
	{		
	case GS_WK_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			if(IsLookonMode()==false)
			{
				m_GameClientView.SetButtonShow(IDC_SOUND, TY_BTSHOW|TY_BTENABLE);
			}
			SetGameStatus(GAME_STATUS_FREE);
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			OnResetGameEngine();
			m_GameClientView.ResetGameView();	
			m_serverType = pStatusFree->wServerType;
			if(pStatusFree->wServerType == GAME_GENRE_MATCH)
			{ 
				return OnStart(0,0);
			}
			else if (IsLookonMode()==false)
			{
				//���ÿؼ�
				m_wCurrentUser = GetMeChairID();
				m_GameClientView.SetButtonShow(IDC_START, TY_BTSHOW|TY_BTENABLE);
			}
			m_GameClientView.PlayGameSound(IDS_ENTER);
			return true;
		} 
		return true;
	case GS_WK_SET_SETBASEBALL:		 
	case GS_WK_WAIT_HITBALL:
		{
			if(this->IsLookonMode()==false)
			{
				m_GameClientView.SetButtonShow(IDC_GIVEPU, TY_BTSHOW|TY_BTENABLE);
				m_GameClientView.SetButtonShow(IDC_REBEGIN, TY_BTHIDE|TY_BTUNENABLE);
				m_GameClientView.SetButtonShow(IDC_SOUND, TY_BTSHOW|TY_BTENABLE);
			}
			else
			{
				m_GameClientView.SetButtonShow(IDC_GIVEPU, TY_BTSHOW|TY_BTUNENABLE);
				m_GameClientView.SetButtonShow(IDC_REBEGIN, TY_BTHIDE|TY_BTUNENABLE);
				m_GameClientView.SetButtonShow(IDC_SOUND, TY_BTSHOW|TY_BTUNENABLE);
			}
				

			m_GameClientView.m_bSingleMode = false;
			if (wDataSize!=sizeof(CMD_S_StatusGamePlay)) return false;
			CMD_S_StatusGamePlay * pStatusPlay=(CMD_S_StatusGamePlay *)pBuffer;
			m_GameClientView.SetBaseScore(pStatusPlay->baseScore);
			m_wCurrentUser = pStatusPlay->currentUser;
			SetGameStatus(pStatusPlay->gameStatus);
			if(pStatusPlay->gameStatus==GS_WK_SET_SETBASEBALL)
			{
				m_GameClientView.m_baseBallState = waitSet;
				SetGameClock(m_wCurrentUser,IDI_SET_BASEBALL,TIME_SET_BASEBALL/1000);
			}
			else if(pStatusPlay->gameStatus == GS_WK_WAIT_HITBALL)
			{
				m_GameClientView.m_baseBallState = waitHit;
				SetGameClock(m_wCurrentUser,IDI_HIT_BALL,TIME_HIT_BALL/1000);
			}
			else
			{
				ASSERT(0);
			}
			for(int i=0;i<BALL_NUM;i++)
			{
				m_GameClientView.m_balls[i].BALL_bEnable = pStatusPlay->ballState[i];
				m_GameClientView.m_balls[i].Ball_Pos.X = pStatusPlay->ptBall[i].x;
				m_GameClientView.m_balls[i].Ball_Pos.Y = pStatusPlay->ptBall[i].y;
				m_GameClientView.m_balls[i].BALL_State = 4;
			}
			for(int i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_comobNum[i] = pStatusPlay->maxComob[i];
			}
			//���ý���
			if(pStatusPlay->inHoleBall[0][7]+pStatusPlay->inHoleBall[1][7]>0)
			{
				m_GameClientView.m_bFirstInBall = false;
			}
			else
			{
				m_GameClientView.m_bFirstInBall = true;
			}
			memcpy(m_GameClientView.m_userBallType,pStatusPlay->userBallType,sizeof(m_GameClientView.m_userBallType));
			if(m_GameClientView.m_bFirstInBall==false)
			{
				if(pStatusPlay->userBallType[GetMeChairID()]==1)
				{//������Ǵ���
					if(GetMeChairID()==1)
					{//�ұ�
						m_GameClientView.InitVisibleBall(true,true);
					}
					else
					{
						m_GameClientView.InitVisibleBall(false,true);
					}
				}
				else
				{//����С��
					if(GetMeChairID()==1)
					{
						m_GameClientView.InitVisibleBall(false,true);
					}
					else
					{
						m_GameClientView.InitVisibleBall(true,true);
					}
				}
			}
			for(int i=0;i<GAME_PLAYER;i++)
			{
				for(int j=0;j<pStatusPlay->inHoleBall[i][7];j++)
				{
					m_GameClientView.SetHitBallInHole(pStatusPlay->inHoleBall[i][j]);
				}
			}
		}
	}

	return true;
}

 //����ĸ��
bool CGameClientDlg::OnSubSetBaseBall( void * pBuffer, WORD wDataSize)
{
	if(GetMeChairID()==m_wCurrentUser&&!this->IsLookonMode())
	{//�����ǶԷ�����������Ϣ
		ASSERT(0);
		return true;
	}
	if(m_GameClientView.m_baseBallState!=waitSet)
	{
		ASSERT(0);
		return true;
	}
	if(GetGameStatus()!=GS_WK_SET_SETBASEBALL)
	{
		ASSERT(0);
		return true;
	}
	CMD_S_SET_BASEBALL *setBaseBall = (CMD_S_SET_BASEBALL*)pBuffer;
	m_GameClientView.m_ptInBallDesk.x = setBaseBall->mousex+m_GameClientView.m_offsetx;
	m_GameClientView.m_ptInBallDesk.y = setBaseBall->mousey+m_GameClientView.m_offsety;
	return true;
}
bool CGameClientDlg::OnSubEndSetBaseBall( void * pBuffer, WORD wDataSize)
{	 
	if(GetGameStatus()!=GS_WK_SET_SETBASEBALL)
	{
		ASSERT(0);
		return true;
	}
	m_GameClientView.m_coinMovieObverse.ForceStop();
	m_GameClientView.m_coinMovieReverse.ForceStop();
	m_GameClientView.m_directionMovie.ForceStop();
	m_GameClientView.m_movieGameStart.ForceStop();
	CMD_S_END_SET_BASEBALL *setBaseBall = (CMD_S_END_SET_BASEBALL*)pBuffer;
	m_GameClientView.m_balls[0].Ball_Pos.X = setBaseBall->ballPosx;
	m_GameClientView.m_balls[0].Ball_Pos.Y = setBaseBall->ballPosy;
	m_GameClientView.m_balls[0].BALL_bEnable = true;
 
	m_GameClientView.m_baseBallState = waitHit;
	SetGameStatus(GS_WK_WAIT_HITBALL);
	KillGameClock(IDI_SET_BASEBALL);
	SetGameClock(m_wCurrentUser,IDI_HIT_BALL,TIME_HIT_BALL/1000);
	
	TCHAR szBuffer[1024];
	 tagUserInfo * pUserData=m_GameClientView.GetClientUserItem(SwitchViewChairID(m_wCurrentUser))->GetUserInfo();	
	_sntprintf(szBuffer,1024,TEXT("%s "),pUserData->szNickName);
	CString str;
	str.Format(TEXT("%s��ʼ����"),szBuffer);
	m_GameClientView.m_systemInfoList.AddTail(str);
	return true;
}
//�������
bool CGameClientDlg::OnSubBallClubPos( void * pBuffer, WORD wDataSize)
{
	if(GetMeChairID()==m_wCurrentUser&&!this->IsLookonMode())
	{//�����ǶԷ�����������Ϣ
//		ASSERT(0);
		return true;
	}
	if(m_GameClientView.m_baseBallState!=waitHit)
	{
		m_GameClientView.m_baseBallState = waitHit;
		//ASSERT(0);
	//	return false;
	}
	if(GetGameStatus()!=GS_WK_WAIT_HITBALL)
	{
		ASSERT(0);
		return true;
	}
	WORD temp = GetGameStatus();
	CMD_S_BALLCLUB *ballClub = (CMD_S_BALLCLUB*)pBuffer;
	/*m_GameClientView.m_ptInBallDesk.x = ballClub->ptMousePos.x+m_GameClientView.m_offsetx;
	m_GameClientView.m_ptInBallDesk.y = ballClub->ptMousePos.y+m_GameClientView.m_offsety;*/
	m_GameClientView.SetBallClubMove(ballClub->ptMousePos);
	m_GameClientView.m_balls[0].Ball_Pos.X = ballClub->ptBaseBallPos.x;
	m_GameClientView.m_balls[0].Ball_Pos.Y = ballClub->ptBaseBallPos.y;
	m_GameClientView.m_hitForce = ballClub->m_hitForce;
	return true;
	
}
bool CGameClientDlg::OnSubHitBall( void * pBuffer, WORD wDataSize)
{
	if(wDataSize!=sizeof(CMD_S_HitBall))
	{
		ASSERT(0);
		return false;
	}
	//if(GetMeChairID()==m_wCurrentUser)
	//{//�����ǶԷ�����������Ϣ
	//	ASSERT(0);
	//	return false;
	//}
	if(m_GameClientView.m_baseBallState!=waitHit)
	{
		m_GameClientView.m_baseBallState = waitHit;
		/*ASSERT(0);
		return false;*/
	}
	if(GetGameStatus()!=GS_WK_WAIT_HITBALL)
	{
		ASSERT(0);
		return false;
	}
	CMD_S_HitBall *hitBall = (CMD_S_HitBall*)pBuffer;
	m_GameClientView.m_balls[0].Ball_Pos.X = hitBall->fbaseBallPosx;
	m_GameClientView.m_balls[0].Ball_Pos.Y = hitBall->fbaseBallPosy;
	m_GameClientView.m_angle = hitBall->fAngle;
	m_GameClientView.HitBall(hitBall->fHitPosx,hitBall->fHitPosy,hitBall->fMousePosx+m_GameClientView.m_offsetx,hitBall->fMousePosy+m_GameClientView.m_offsety,hitBall->fHitForce);
	m_GameClientView.m_angle = 0;
	m_GameClientView.m_baseBallState = afterHit;
	m_GameClientView.m_bHit = true;
	m_GameClientView.m_bMoveBallClub = false;
	KillGameClock(IDI_HIT_BALL);	 
	while(::ShowCursor(true)<0);
	return true;
}
//������
bool CGameClientDlg::OnSubHitResult( void *pBuffer,WORD wDataSize)
{
	if(wDataSize!=sizeof(CMD_S_CLIENT_HITRESLUT))
	{
		ASSERT(0);
		return false;
	}
	bool needAjustState = false;
	//if(GetMeChairID()==m_wCurrentUser)
	//{//�����ǶԷ�����������Ϣ
	//	ASSERT(0);
	//	return false;
	//}

	CMD_S_CLIENT_HITRESLUT * clientHitResult = (CMD_S_CLIENT_HITRESLUT*) pBuffer;
	//memcpy(m_GameClientView.m_comobNum,clientHitResult->totalComobNum,sizeof(BYTE)*2);
	for(int i=0;i<2;i++)
	{
		m_GameClientView.m_comobNum[SwitchViewChairID(i)] = clientHitResult->totalComobNum[i]; 
	}

	m_GameClientView.m_ptHitMark = m_GameClientView.m_ptCenterHitMark;
	//���Ƿ�ȫ����ֹ��
	if(m_GameClientView.BallStateCheck(0)==false)
	{
		//ASSERT(0);
		TRACE("��û��ȫ��ֹͣ");
		needAjustState = true;
	}
	else
	{//�������
		if(m_GameClientView.m_physics.m_nOverNum!=clientHitResult->inHoleNum)
		{
			ASSERT(0);
			needAjustState = true;
		}
		if(m_GameClientView.m_physics.m_wFistHitBall!=clientHitResult->firstBallNum)
		{
			ASSERT(0);
			needAjustState = true;
		}
		for(int i=0;i<BALL_NUM;i++)
		{
			if(m_GameClientView.m_balls[i].BALL_bEnable!=clientHitResult->ballState[i])
			{
			//	ASSERT(0);
				needAjustState = true;
				break;
			}
			if(m_GameClientView.m_balls[i].BALL_bEnable)
			{
				if(m_GameClientView.m_balls[i].Ball_Pos.X!=clientHitResult->ptBall[i].x||m_GameClientView.m_balls[i].Ball_Pos.Y!=clientHitResult->ptBall[i].y)
				{
					TRACE("λ�ò�һ��:�Լ�:x=%f,y=%f,�Է�:x=%f,y=%f",m_GameClientView.m_balls[i].Ball_Pos.X,m_GameClientView.m_balls[i].Ball_Pos.Y,clientHitResult->ptBall[i].x,clientHitResult->ptBall[i].y);
					//ASSERT(0);
					needAjustState = true;
					break;
				}
			}
		}
	}
	if(m_GameClientView.m_baseBallState!=allState)
	{//������״̬���ԡ�
		//ASSERT(0);
		needAjustState = true;
		TRACE("��������");
	}
	if(needAjustState)
	{//���²������״̬
		for(int i=0;i<BALL_NUM;i++)
		{
			m_GameClientView.m_balls[i].Clear();
		}
		for(int i=0;i<BALL_NUM;i++)
		{
			m_GameClientView.m_balls[i].BALL_bEnable = clientHitResult->ballState[i];
			m_GameClientView.m_balls[i].Ball_Pos.X = clientHitResult->ptBall[i].x;
			m_GameClientView.m_balls[i].Ball_Pos.Y = clientHitResult->ptBall[i].y;
		}
		CString str;
		str.Format(TEXT("��Ϣ�������������⣬��Ҫͬ������"));
		//m_GameClientView.m_systemInfoList.AddTail(str);
	}

	memcpy(m_GameClientView.m_userBallType,clientHitResult->userBallType,sizeof(m_GameClientView.m_userBallType));
	if(m_GameClientView.m_userBallType[0]!=100&&m_GameClientView.m_userBallType[1]!=100)
	{
		if(clientHitResult->userBallType[GetMeChairID()]==1)
		{//������Ǵ���
			if(GetMeChairID()==1)
			{//�ұ�
				m_GameClientView.InitVisibleBall(true,true);
			}
			else
			{
				m_GameClientView.InitVisibleBall(false,true);
			}
		}
		else
		{//����С��
			if(GetMeChairID()==1)
			{
				m_GameClientView.InitVisibleBall(false,true);
			}
			else
			{
				m_GameClientView.InitVisibleBall(true,true);
			}
		}
		//���õ�����ʾû�н�������
		for(int i=1;i<BALL_NUM;i++)
		{
			if(i==8)
				continue;
			if(m_GameClientView.m_balls[i].BALL_bEnable==false)
			{
				m_GameClientView.m_visibleBalls[i].BALL_bEnable = false;
			}
			else
			{
				m_GameClientView.m_visibleBalls[i].BALL_bEnable = true;
			}
		}
	}

	
	//
	DWORD tempCurrentUser = m_wCurrentUser;
	m_wCurrentUser = (WORD)clientHitResult->currentUser;
	if(m_GameClientView.m_balls[0].BALL_bEnable==false)
	{
		m_GameClientView.m_baseBallState = waitSet;//�ȴ�����ĸ��
		SetGameStatus(GS_WK_SET_SETBASEBALL);
		SetGameClock(m_wCurrentUser,IDI_SET_BASEBALL,TIME_SET_BASEBALL/1000);
	}
	else
	{
		m_GameClientView.m_baseBallState = waitHit;
		SetGameStatus(GS_WK_WAIT_HITBALL);
		SetGameClock(m_wCurrentUser,IDI_HIT_BALL,TIME_HIT_BALL/1000);
	}
	//����
	int x = m_GameClientView.m_offsetx + ( m_GameClientView.m_pSpriteLogo->GetWidth() -  m_GameClientView.m_pSpriteFailOpen->GetWidth())/2;
	int y =  m_GameClientView.m_offsety + ( m_GameClientView.m_pSpriteLogo->GetHeight() -  m_GameClientView.m_pSpriteFailOpen->GetHeight())/2;	
	TCHAR szBuffer[1024];
	 tagUserInfo * pUserData=m_GameClientView.GetClientUserItem(SwitchViewChairID(tempCurrentUser))->GetUserInfo();
	_sntprintf(szBuffer,1024,_T("%s "),pUserData->szNickName);
	CString name = CString(szBuffer);
	if(clientHitResult->hitResult == openFail)
	{//����ʧ��
		m_GameClientView.ResetBall();
		m_GameClientView.InitVisibleBall(false,false);
		m_GameClientView.m_movieHitResult.SetResource(m_GameClientView.m_pSpriteFailOpen,this);
		m_GameClientView.m_movieHitResult.StartMovie(CPoint(x,y),CPoint(x,y-150),2000,500);
		ASSERT(m_wCurrentUser!=tempCurrentUser);
		CString str;
		str.Format(_T("����ʧ�ܣ�������ң����¿���"));
		m_GameClientView.m_systemInfoList.AddTail(str);		 
	}
	else if (clientHitResult->hitResult==firtIn)
	{//�״ν���ѡ����ɫ
		if(clientHitResult->ballInHole[0]<8)
		{
			m_GameClientView.m_movieHitResult.SetResource(m_GameClientView.m_pSpriteSelect,this);
			m_GameClientView.m_movieHitResult.StartMovie(CPoint(x,y),CPoint(x,y-150),2000,500);
		}
		else
		{
			m_GameClientView.m_movieHitResult.SetResource(m_GameClientView.m_pSpriteSelect_hua,this);
			m_GameClientView.m_movieHitResult.StartMovie(CPoint(x,y),CPoint(x,y-150),2000,500);
		}
		CString str;
		str.Format(_T("�״ν���ѡ����ɫ"));
		m_GameClientView.m_systemInfoList.AddTail(str);
	}
	else if(clientHitResult->hitResult==noInHole)
	{//��ͨ����
		if(tempCurrentUser == m_wCurrentUser)
		{//���˻���
			m_GameClientView.m_movieHitResult.SetResource(m_GameClientView.m_pSpriteContinue,this);
			m_GameClientView.m_movieHitResult.StartMovie(CPoint(x,y),CPoint(x,y-150),2000,500);
			CString str;
			str.Format(_T("%s����,��������"),GetTableUserItem(m_wCurrentUser)->GetNickName());
			m_GameClientView.m_systemInfoList.AddTail(str);
		}
		else
		{//�������
			m_GameClientView.m_movieHitResult.SetResource(m_GameClientView.m_pSpriteChange,this);
			m_GameClientView.m_movieHitResult.StartMovie(CPoint(x,y),CPoint(x,y-150),2000,500);
			CString str;
			str.Format(_T("%s��ͨ����,�������"),GetTableUserItem(m_wCurrentUser)->GetNickName());
			m_GameClientView.m_systemInfoList.AddTail(str);
			ASSERT(m_wCurrentUser!=tempCurrentUser);
		}
	}
	else if(clientHitResult->hitResult==rightInHole)
	{//���ˣ���������
		//m_GameClientView.m_movieHitResult.SetResource(m_GameClientView.m_pSpriteChange,this);
		//m_GameClientView.m_movieHitResult.StartMovie(CPoint(x,y),CPoint(x,y-150),2000,500);
		//������
		CString str;
		str.Format(_T("%s��ȷ����,��������"),szBuffer);
		m_GameClientView.m_systemInfoList.AddTail(str);
		if(clientHitResult->comobNum>0)
		{			
			int x = m_GameClientView.m_offsetx + (m_GameClientView.m_pSpriteLogo->GetWidth()-m_GameClientView.m_pSpriteComob->GetWidth())/2;
			int y =  m_GameClientView.m_offsetx + (m_GameClientView.m_pSpriteLogo->GetHeight()-m_GameClientView.m_pSpriteComob->GetHeight())/2;
			CPoint ptStart(x,y);
			CPoint ptEnd(x,y-150);
			if(clientHitResult->comobNum>=10)
				m_GameClientView.m_movieComob.ShowNum(clientHitResult->comobNum,CPoint(-120,0));
			else
				m_GameClientView.m_movieComob.ShowNum(clientHitResult->comobNum,CPoint(-60,0));
			m_GameClientView.m_movieComob.StartMovie(ptStart,ptEnd,2000,800);
			m_GameClientView.PlayGameSound(IDS_COMBO);
		}
	}
	else if(clientHitResult->hitResult==wrongInHole)
	{//���򷸹棬�������
		m_GameClientView.m_movieHitResult.SetResource(m_GameClientView.m_pSpriteFailHit,this);
		m_GameClientView.m_movieHitResult.StartMovie(CPoint(x,y),CPoint(x,y-150),2000,500);
		ASSERT(m_wCurrentUser!=tempCurrentUser);
		CString str;
		str.Format(_T("%s���򷸹�,�������"),GetTableUserItem(tempCurrentUser)->GetNickName());
		m_GameClientView.m_systemInfoList.AddTail(str);
	}
	else if(clientHitResult->hitResult==gameEnd)
	{
	/*	CString str;
		str.Format("��Ϸ����...");
		m_GameClientView.m_systemInfoList.AddTail(str);*/
	}
	if(m_wCurrentUser == GetMeChairID())
	{
		ActiveGameFrame();
		if(this->IsLookonMode()==false)
		{
			CRect rc ;
			rc.left = m_GameClientView.m_offsetx + BAR_WIDTH;
			rc.top =  m_GameClientView.m_offsety + BAR_WIDTH-1;
			rc.right = rc.left +  m_GameClientView.m_pSpriteLogo->GetWidth() - BAR_WIDTH * 2;
			rc.bottom = rc.top +  m_GameClientView.m_pSpriteLogo->GetHeight() - BAR_WIDTH * 2;
			if(rc.PtInRect(m_GameClientView.m_ptMousePos))
				while(::ShowCursor(false)>=0);
			else
				while(::ShowCursor(true)<0);
		}
	}
	return true;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart( void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	m_wCurrentUser = pGameStart->currentUser;
	m_GameClientView.m_roundCount = pGameStart->roundCount;
	m_GameClientView.SetBaseScore(pGameStart->nBaseScore);
	m_GameClientView.SetGameStart();
	SetGameStatus(GS_WK_SET_SETBASEBALL);	
	if(m_wCurrentUser == GetMeChairID())
	{
		this->ActiveGameFrame();
	}
	m_GameClientView.PlayGameSound(IDS_BEG);
	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd( void * pBuffer, WORD wDataSize)
{
	if (wDataSize!=sizeof(CMD_S_GameEnd))
		return false;
	KillGameClock(IDI_SET_BASEBALL);
	KillGameClock(IDI_HIT_BALL);
	CMD_S_GameEnd *pGameEnd = (CMD_S_GameEnd*)pBuffer;
	m_GameClientView.m_IsGameEnd = true;
	m_GameClientView.m_bSingleMode = true;
	for(int i=0;i<2;i++)
	{
		m_GameClientView.m_endScore[SwitchViewChairID(i)] = pGameEnd->nGameScore[i];
		m_GameClientView.m_comobNum[SwitchViewChairID(i)] = pGameEnd->comobNum[i];
	}
//	m_GameClientView.SetTableWaitTime(TIME_GAME_START/1000); 
	m_GameClientView.KillTimer(IDI_SUSPEND_SEND);
	SetGameStatus(GAME_STATUS_FREE);
	m_pIStringMessage->InsertCustomString(TEXT("��Ϸ����"),RGB(220,0,0));
	TCHAR str[100];
	BYTE loseUser = 0;
	if(pGameEnd->nGameScore[GetMeChairID()]>0)
	{
		loseUser = (GetMeChairID()+1)%2;
		 tagUserInfo *pUserData = this->GetTableUserItem(GetMeChairID())->GetUserInfo();
		_stprintf(str,TEXT("%s Ӯ���÷֣�%I64d"),pUserData->szNickName, pGameEnd->nGameScore[GetMeChairID()]);
		m_pIStringMessage->InsertCustomString(str,RGB(0,200,0))		;
		
		m_GameClientView.PlayGameSound(IDS_WIN);
	}
	else
	{
		loseUser = GetMeChairID();
		 tagUserInfo *pUserData = this->GetTableUserItem((GetMeChairID()+1)%2)->GetUserInfo();
		_stprintf(str,TEXT("%s Ӯ���÷֣�%I64d"),pUserData->szNickName,pGameEnd->nGameScore[(GetMeChairID()+1)%2]);
		m_pIStringMessage->InsertCustomString(str,RGB(0,200,0))		;
		m_GameClientView.PlayGameSound(IDS_FAILURE);
	}
	//1:�������㣬2�����䣬3������,4��ʱ,5���ݴ�����Ϸ��ɢ //����ԭ��
	CString szTemp;
	if(pGameEnd->endReason==1)
	{
		szTemp.Format(TEXT("��Ϸ����"));
	}
	else if(pGameEnd->endReason==2)
	{
		 tagUserInfo *pUserData = GetTableUserItem(loseUser)->GetUserInfo();
		szTemp.Format(TEXT("%s ���� ��Ϸ����"),pUserData->szNickName);
	}
	else if(pGameEnd->endReason==3)
	{
		 tagUserInfo *pUserData = GetTableUserItem(loseUser)->GetUserInfo();
		szTemp.Format(TEXT("%s ���� ��Ϸ����"),pUserData->szNickName);
	}
	else if(pGameEnd->endReason==4)
	{
		 tagUserInfo *pUserData = GetTableUserItem(loseUser)->GetUserInfo();
		szTemp.Format(TEXT("%s ��ʱ5�� ��Ϸ����"),pUserData->szNickName);
	}
	else if(pGameEnd->endReason==5)
	{
		szTemp.Format(TEXT("�����쳣 ��Ϸ��ɢ"));
	}
	else 
	{
		szTemp.Format(TEXT("��Ϸ����"));
	}
	m_GameClientView.m_systemInfoList.AddTail(szTemp);

	if(pGameEnd->nGameScore[GetMeChairID()]>0)
	{
		 tagUserInfo *pUserData = this->GetTableUserItem(GetMeChairID())->GetUserInfo();
		_stprintf(str,TEXT("%s Ӯ���÷֣�%I64d"),pUserData->szNickName,pGameEnd->nGameScore[GetMeChairID()]);
		CString szTemp = str;
		m_GameClientView.m_systemInfoList.AddTail(szTemp);
	}
	else
	{
		 tagUserInfo *pUserData = this->GetTableUserItem((GetMeChairID()+1)%2)->GetUserInfo();
		_stprintf(str,TEXT("%s Ӯ���÷֣�%I64d"), pUserData->szNickName,pGameEnd->nGameScore[(GetMeChairID()+1)%2]);		
		CString szTemp = str;
		m_GameClientView.m_systemInfoList.AddTail(szTemp);
	}
	
	m_GameClientView.m_bMoveBallClub = false;
	m_GameClientView.SetButtonShow(IDC_GIVEPU, TY_BTUNENABLE);
	m_GameClientView.m_movieGameStart.Stop();
	m_GameClientView.m_movieHitResult.Stop();
	m_GameClientView.m_movieObverse.Stop();
	m_GameClientView.m_movieReverse.Stop();
	//m_GameClientView.m_coinMovieObverse.SetDisappearTime(0);
	//m_GameClientView.m_coinMovieObverse.Stop();
	//m_GameClientView.m_coinMovieReverse.SetDisappearTime(0);
	//m_GameClientView.m_coinMovieReverse.Stop();
	m_GameClientView.m_baseBallState = waitSet;
	while(::ShowCursor(true)<0);
	return true;
}
 
 
//��ʱ����Ϣ
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);	
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//���ñ���		
	OnResetGameEngine();

	//������Ϣ
	SendUserReady(NULL,0);
	return 0;
}


//////////////////////////////////////////////////////////////////////////

