#include "StdAfx.h"
#include "ScoreView.h"
#include ".\scoreview.h"

//////////////////////////////////////////////////////////////////////////

#define IDC_ENSURE						200					//ȷ�ϰ�ť

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_BN_CLICKED(IDC_ENSURE, OnEnsureButton)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView(void)
{
	//��������
	m_lGameTax=0L;
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//������Դ
	m_ImageBack.LoadFromResource(AfxGetInstanceHandle(),IDB_GAME_END);
}

//��������
CScoreView::~CScoreView(void)
{
}

//��ʼ������
BOOL CScoreView::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	m_lGameTax=0L;
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//������ť
	CRect CreateRect(0,0,0,0);
	m_btEnsure.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_ENSURE);
	m_btEnsure.SetButtonImage(IDB_BT_OK, AfxGetInstanceHandle(), false,false);
	m_btEnsure.MoveWindow(145, 208, 97, 35);
	m_btEnsure.EnableWindow(TRUE);
	m_btEnsure.ShowWindow(SW_SHOW);

	//���ý���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//�ƶ�����
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	//������Դ������
	m_pSkinResourceManager= (CSkinResourceManager *)CSkinResourceManager::GetInstance();
	ASSERT(m_pSkinResourceManager!=NULL);

	return TRUE;
}

//�ػ�����
void CScoreView::OnPaint() 
{
	CPaintDC dc(this); 

	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255,0,0));
	dc.SelectObject(m_pSkinResourceManager->GetDefaultFont());

	//�滭����
	m_ImageBack.BitBlt(dc,0,0);

	//��ʾ����
	CRect rcDraw;
	CString strScore ;
	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		//if(_tcscpy(m_szUserName[i],_T("")))
		{
			dc.SetTextColor( RGB(215, 198, 108) );

			//�û�����
			rcDraw.left=37;
			rcDraw.right=197;
			rcDraw.top=i*40+87;
			rcDraw.bottom=rcDraw.top+20;
			dc.DrawText(m_szUserName[i],lstrlen(m_szUserName[i]),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);

			//������ɫ
			if ( m_lGameScore[i] > 0 )
				dc.SetTextColor( RGB(215, 198, 108) ) ;
			else 
				dc.SetTextColor( RGB( 250, 48, 0 ) ) ;

			//�û�����
			rcDraw.left=213;
			rcDraw.right=351;
			strScore.Format(_T("%I64d") , m_lGameScore[i]) ;
			dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
		}
	}

	return;
}

//�滭����
BOOL CScoreView::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//�����Ϣ
void CScoreView::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	return;
}

//���û���
void CScoreView::ResetScore()
{
	//��������
	m_lGameTax=0L;
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//�滭��ͼ
	Invalidate(NULL);
	
	return;
}

//����˰��
void CScoreView::SetGameTax(LONGLONG lGameTax)
{
	//���ñ���
	m_lGameTax=lGameTax;
	Invalidate(FALSE);

	return;
}

//���û���
void CScoreView::SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONGLONG lScore)
{
	//���ñ���
	if (wChairID<CountArray(m_lGameScore))
	{
		m_lGameScore[wChairID]=lScore;
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));
		Invalidate(FALSE);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

void CScoreView::OnKillFocus(CWnd* pNewWnd)
{
	//���ش���
	ShowWindow(SW_HIDE) ;

	CDialog::OnKillFocus(pNewWnd);
}
