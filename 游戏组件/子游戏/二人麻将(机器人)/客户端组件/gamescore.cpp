#include "Stdafx.h"
#include "GameScore.h"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////

//��ť��ʶ
#define IDC_CLOSE_SCORE				100									//�رճɼ�

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameScore, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameScore::CGameScore() : CDialog(IDD_GAME_SCORE)
{
	//��ʼ������
	ZeroMemory( &m_ScoreInfo,sizeof(m_ScoreInfo) );
	m_cbWeaveCount = 0;
	m_ChiHuRight.SetEmpty();

	HINSTANCE hInst = AfxGetInstanceHandle();
	m_ImageBack.LoadFromResource(hInst ,IDB_GAME_SCORE);
	m_ImageFangPao.LoadImage( hInst,TEXT("FANG_PAO") );
	m_ImageZiMo.LoadImage( hInst,TEXT("ZI_MO") );
}

//��������
CGameScore::~CGameScore()
{
}

//�ؼ���
void CGameScore::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btCloseScore);
}

//��ʼ������
BOOL CGameScore::OnInitDialog()
{
	__super::OnInitDialog();

	//ɾ�����ڱ���ͻ�ȡ����
	ModifyStyle(WS_CAPTION,0,0);
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	

	//��������
	CRgn RgnControl;
	m_ImageBack.CreateImageRegion(RgnControl,RGB(255,0,255));

	//���ô���
	if (RgnControl.GetSafeHandle()!=NULL)
	{
		//�ƶ�����
		SetWindowRgn(RgnControl,TRUE);
		SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE);
	}

	m_btCloseScore.SetButtonImage(IDB_BT_SCORE_CLOSE,AfxGetInstanceHandle(),false,false);

	//������ť
	CRect rcClient;
	GetClientRect(&rcClient);
	CRect rcBtClose;
	m_btCloseScore.GetWindowRect(&rcBtClose);
	m_btCloseScore.SetWindowPos(NULL,(rcClient.Width()-rcBtClose.Width())/2,rcClient.Height()-rcBtClose.Height()-10,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//��������
	//LOGFONT lg;
	//ZeroMemory( &lg,sizeof(lg) );
	//lg.lfCharSet = GB2312_CHARSET;
	//_tcscpy( lg.lfFaceName,"����" );
	//lg.lfHeight = -20;
	//if( !m_InfoFont.CreateFontIndirect( &lg ) )
	//	m_InfoFont.Attach(CSkinResourceManager::GetDefaultFont());

	//����͸��
	ModifyStyleEx(0,0x00080000);
	HINSTANCE hInst = LoadLibrary(TEXT("User32.DLL")); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL;
		//ȡ��SetLayeredWindowAttributes��������ָ�� 
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(fun)fun(this->GetSafeHwnd(),0,220,2); 
		FreeLibrary(hInst); 
	}

	return TRUE;
}

//��λ����
void CGameScore::RestorationData()
{
	//���ñ���
	m_cbWeaveCount=0;
	ZeroMemory(&m_ScoreInfo,sizeof(m_ScoreInfo));

	//���ش���
	if (m_hWnd!=NULL) ShowWindow(SW_HIDE);

	return;
}

//���û���
void CGameScore::SetScoreInfo(const tagScoreInfo & ScoreInfo, const tagWeaveInfo & WeaveInfo, const CChiHuRight &ChiHuRight)
{
	//���ñ���
	m_ScoreInfo=ScoreInfo;
	m_cbWeaveCount=WeaveInfo.cbWeaveCount;
	m_ChiHuRight = ChiHuRight;

	//��ϱ���
	for (BYTE i=0;i<m_cbWeaveCount;i++)
	{
		bool bPublicWeave=(WeaveInfo.cbPublicWeave[i]==TRUE);
		m_WeaveCard[i].SetCardData(WeaveInfo.cbCardData[i],WeaveInfo.cbCardCount[i]);
		m_WeaveCard[i].SetDisplayItem(true);
	}

	//��ʾ����
	ShowWindow(SW_SHOW);

	return;
}

//�رհ�ť
void CGameScore::OnOK()
{
	//���ش���
	RestorationData();

	__super::OnOK();
}

//�ػ�����
void CGameScore::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CDC DCBuffer;
	CBitmap ImageBuffer;
	DCBuffer.CreateCompatibleDC(&dc);
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���� DC
	DCBuffer.SetBkMode(TRANSPARENT);
	DCBuffer.SelectObject(&ImageBuffer);
	//DCBuffer.SelectObject(m_InfoFont);
	DCBuffer.SetTextColor(RGB(250,250,250));

	//�滭����

	if (m_ImageBack.IsNull()==false) m_ImageBack.BitBlt(DCBuffer,0,0);

	//λ�ñ���
	int nCardSpace=5;
	int nItemWidth=g_CardResource.m_ImageTableBottom.GetViewWidth();
	int nTotalWidth=m_cbWeaveCount*(nItemWidth*3+nCardSpace)+nItemWidth*m_ScoreInfo.cbCardCount+nCardSpace;

	//����λ��
	int nYCardPos=140-g_CardResource.m_ImageTableBottom.GetViewHeight();
	int nXCardPos=(rcClient.Width()-nTotalWidth)/2;

	//�滭���
	for (BYTE i=0;i<m_cbWeaveCount;i++)
	{
		//�滭�˿�
		m_WeaveCard[i].DrawCardControl(&DCBuffer,nXCardPos,nYCardPos,false);

		//����λ��
		nXCardPos+=(nCardSpace+nItemWidth*3);
	}

	//�滭�˿�
	for (BYTE i=0;i<m_ScoreInfo.cbCardCount;i++)
	{
		//�滭�˿�
		g_CardResource.m_ImageTableBottom.DrawCardItem(&DCBuffer,m_ScoreInfo.cbCardData[i],nXCardPos,nYCardPos,false);

		//����λ��
		nXCardPos+=nItemWidth;
		if ((i+2)==m_ScoreInfo.cbCardCount) nXCardPos+=nCardSpace;
	}

	TCHAR szBuffer[32] = TEXT("");
	//������Ϣ
	if( m_ScoreInfo.wProvideUser != INVALID_CHAIR )
	{
		CRect rcDraw;
		//����
		if( m_ScoreInfo.wProvideUser != m_ScoreInfo.wChiHuUser )
		{
			rcDraw.SetRect(15,148,190,175);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_RIGHT|DT_BOTTOM );
			m_ImageFangPao.DrawImage( &DCBuffer,205,150 );
			rcDraw.SetRect(295,148,482,175);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wChiHuUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wChiHuUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_LEFT|DT_BOTTOM );
		}
		//����
		else
		{
			rcDraw.SetRect(15,148,190,175);
			DCBuffer.DrawText( m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser],lstrlen(m_ScoreInfo.szUserName[m_ScoreInfo.wProvideUser]),
				&rcDraw,DT_SINGLELINE|DT_END_ELLIPSIS|DT_RIGHT|DT_BOTTOM );
			m_ImageZiMo.DrawImage( &DCBuffer,215,150 );
		}
	}

	//�б���Ϣ
	CString szUserInfo;
	CSkinButton btDraw;
	DCBuffer.SetTextAlign(TA_LEFT);
	INT nY=200,nCard=190,nMagic=270,nGang=345,nScore=415;
	DCBuffer.TextOut(66,nY,TEXT("���"));
	//btDraw.SetCapture(&DCBuffer,TEXT("���"),RGB(0,255,255),RGB(0,0,0),66,nY);
	//btDraw.DrawTextString(&DCBuffer,TEXT("����"),RGB(0,255,255),RGB(0,0,0),nCard,nY);
	//btDraw.DrawTextString(&DCBuffer,TEXT("����"),RGB(0,255,255),RGB(0,0,0),nMagic,nY);
	//btDraw.DrawTextString(&DCBuffer,TEXT("����"),RGB(0,255,255),RGB(0,0,0),nGang,nY);
	//btDraw.DrawTextString(&DCBuffer,TEXT("�ϼ�"),RGB(0,255,255),RGB(0,0,0),nScore,nY);
	DCBuffer.TextOut(nCard,nY,TEXT("����"));
	DCBuffer.TextOut(nMagic,nY,TEXT("����"));
	DCBuffer.TextOut(nGang,nY,TEXT("����"));
	DCBuffer.TextOut(nScore,nY,TEXT("�ϼ�"));
	//���Ƶ÷�
	LONGLONG lScore[GAME_PLAYER]={};
	for( WORD j = 0; j < GAME_PLAYER; j++ )
	{
		if(j==m_ScoreInfo.wChiHuUser)
		{
			lScore[j]+=((m_ScoreInfo.wProvideUser != m_ScoreInfo.wChiHuUser)?1:2);
			lScore[(j+1)%GAME_PLAYER]-=((m_ScoreInfo.wProvideUser != m_ScoreInfo.wChiHuUser)?1:2);
		}
		if(j==m_ScoreInfo.wExitUser)
		{
			lScore[j]-=3;
			lScore[(j+1)%GAME_PLAYER]+=3;
		}
	}

	//ǿ���û�
	if(INVALID_CHAIR!=m_ScoreInfo.wExitUser)
	{
		DCBuffer.SetTextAlign(TA_CENTER);
		szUserInfo.Format(TEXT("[%s]ǿ�ˣ��۳�%I64d��"),m_ScoreInfo.szUserName[m_ScoreInfo.wExitUser],3*m_ScoreInfo.lCellScore);
		//btDraw.DrawTextString(&DCBuffer,szUserInfo,RGB(255,255,255),RGB(0,0,0),250,110);
			DCBuffer.TextOut(250,110,szUserInfo);
	}

	//�÷���Ϣ

	for( WORD j = 0; j < GAME_PLAYER; j++ )
	{
		WORD i=(j+m_ScoreInfo.wMeChairID)%GAME_PLAYER;
		INT nY1=nY+32+j*30;
		INT nX = 80;
		INT nAdd=18;
		DCBuffer.SetTextColor(((i==m_ScoreInfo.wMeChairID)? RGB(252,255,0):RGB(252,255,0) ));
				DCBuffer.SetTextAlign(TA_LEFT);
		//���
		szUserInfo=m_ScoreInfo.szUserName[i];
		//btDraw.DrawTextString(&DCBuffer,szUserInfo,RGB(255,255,255),RGB(0,0,0),nX,nY1);
		CRect rc(20,nY1,160,nY1+16);
		DCBuffer.DrawText(szUserInfo,&rc,DT_LEFT|DT_END_ELLIPSIS);
			DCBuffer.SetTextAlign(TA_CENTER);
			//DCBuffer.TextOut(nX,nY1,szUserInfo);
		//����
		nX = nCard+nAdd;
		_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%I64d"),m_ScoreInfo.lMingGang[i]*m_ScoreInfo.lCellScore );
		//btDraw.DrawTextString(&DCBuffer,szBuffer,RGB(255,255,255),RGB(0,0,0),nX,nY1);
			DCBuffer.TextOut(nX,nY1,szBuffer);
		//����
		nX = nMagic+nAdd;
		_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%I64d"),m_ScoreInfo.lLangGang[i]*m_ScoreInfo.lCellScore );
		//btDraw.DrawTextString(&DCBuffer,szBuffer,RGB(255,255,255),RGB(0,0,0),nX,nY1);
			DCBuffer.TextOut(nX,nY1,szBuffer);
		//����
		nX = nGang+nAdd;
		_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%I64d"),lScore[i]*m_ScoreInfo.lCellScore );
		//btDraw.DrawTextString(&DCBuffer,szBuffer,RGB(255,255,255),RGB(0,0,0),nX,nY1);
			DCBuffer.TextOut(nX,nY1,szBuffer);
		//����
		nX = nScore+nAdd;
		_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%I64d��"),m_ScoreInfo.lGameScore[i] );
		//btDraw.DrawTextString(&DCBuffer,szBuffer,RGB(255,255,255),RGB(0,0,0),nX,nY1);
			DCBuffer.TextOut(nX,nY1,szBuffer);
	}

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&DCBuffer,0,0,SRCCOPY);

	//������Դ
	DCBuffer.DeleteDC();
	ImageBuffer.DeleteObject();

	return;
}

//�����Ϣ
void CGameScore::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//��������
BOOL CGameScore::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
