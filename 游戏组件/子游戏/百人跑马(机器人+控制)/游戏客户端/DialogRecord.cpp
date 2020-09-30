// DialogRecord.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "resource.h"
#include "DialogRecord.h"


// CDialogRecord �Ի���

IMPLEMENT_DYNAMIC(CDialogRecord, CDialog)
CDialogRecord::CDialogRecord(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogRecord::IDD, pParent)
{
	m_InfoFont.CreateFont(12,0,0,0,400,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
	m_lPlayerScore = 0l;
	m_lAllBet = 0l;
	m_lBetMumber = 0l;
}

CDialogRecord::~CDialogRecord()
{
	m_InfoFont.DeleteObject();
}

void CDialogRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_LOGOK, m_btOk);
}


BEGIN_MESSAGE_MAP(CDialogRecord, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_LOGOK, OnBnClickedButtonLogok)
END_MESSAGE_MAP()


// CDialogRecord ��Ϣ�������

BOOL CDialogRecord::OnInitDialog()
{
	CDialog::OnInitDialog();

	HINSTANCE hInstance = AfxGetInstanceHandle();

	m_ImageBackdrop.LoadFromResource( hInstance,IDB_HISTORYSCORE_M );
	m_ImageBackdropHand.LoadFromResource( hInstance,IDB_HISTORYSCORE_T );
	m_ImageBackdropTail.LoadFromResource( hInstance,IDB_HISTORYSCORE_B );
	m_btOk.SetButtonImage( IDB_BT_CLOSE_HISTORYSCORE,  hInstance , false, false);

	ModifyStyle(0, WS_CLIPCHILDREN, 0);

	m_szTotalSize.SetSize(m_ImageBackdrop.GetWidth(), m_ImageBackdrop.GetHeight() * MAX_SCORE_HISTORY + m_ImageBackdropHand.GetHeight() + m_ImageBackdropTail.GetHeight());
	SetWindowPos(NULL, 0, 0, m_szTotalSize.cx, m_szTotalSize.cy, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOMOVE);

	m_btOk.MoveWindow(m_szTotalSize.cx/2 - 11, m_szTotalSize.cy - 25, 23, 20);


	return TRUE; 
}

BOOL CDialogRecord::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CDialogRecord::OnPaint()
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
	CBitmap* oldBitmap = DCBuffer.SelectObject(&ImageBuffer);
	CFont* oldFont = DCBuffer.SelectObject(&m_InfoFont);
	UINT nTextAlign = DCBuffer.SetTextAlign(TA_TOP|TA_LEFT);
	COLORREF oldColor = DCBuffer.SetTextColor(RGB(250,250,255));

	//����
	m_ImageBackdropHand.BitBlt(DCBuffer.GetSafeHdc(), 0, 0);
	int nBcakY = m_ImageBackdropHand.GetHeight();
	for ( int i = 0; i < MAX_SCORE_HISTORY; ++i)
	{
		m_ImageBackdrop.BitBlt(DCBuffer.GetSafeHdc(), 0, nBcakY);
		nBcakY += m_ImageBackdrop.GetHeight();
	}
	m_ImageBackdropTail.BitBlt(DCBuffer.GetSafeHdc(), 0, nBcakY);

	//д��¼
	int nRecordsY = m_ImageBackdropHand.GetHeight();
	TCHAR szInfo[64] = TEXT("");
	CRect rect;
	for ( int i = 0; i < m_GameRecords.GetCount(); ++i)
	{
		rect.SetRect( 0, nRecordsY, 28, nRecordsY + 15);
		_sntprintf(szInfo, sizeof(szInfo), TEXT("%d"), m_GameRecords[i].nStreak);
		DCBuffer.DrawText(szInfo, rect , DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER );

		rect.SetRect( 28, nRecordsY, 57, nRecordsY + 15);
		_sntprintf(szInfo, sizeof(szInfo), TEXT("%s"), IdentifyAreas(m_GameRecords[i].nRanking));
		DCBuffer.DrawText(szInfo, rect , DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER );

		rect.SetRect( 57, nRecordsY, 86, nRecordsY + 15);
		_sntprintf(szInfo, sizeof(szInfo), TEXT("%d"), m_GameRecords[i].nRiskCompensate);
		DCBuffer.DrawText(szInfo, rect , DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER );
		
		rect.SetRect( 86, nRecordsY, 151, nRecordsY + 15);
		_sntprintf(szInfo, sizeof(szInfo), TEXT("%s%d:%s%d:%s%d"), 
			m_GameRecords[i].nHours >= 10 ? TEXT("") : TEXT("0"), m_GameRecords[i].nHours, 
			m_GameRecords[i].nMinutes >= 10 ? TEXT("") : TEXT("0"), m_GameRecords[i].nMinutes, 
			m_GameRecords[i].nSeconds >= 10 ? TEXT("") : TEXT("0"), m_GameRecords[i].nSeconds);
		DCBuffer.DrawText(szInfo, rect , DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER );
		nRecordsY += 15;
	}
	
	nRecordsY = m_szTotalSize.cy - 85;
	rect.SetRect( 5, nRecordsY, 146, nRecordsY + 15);
	_sntprintf(szInfo, sizeof(szInfo), TEXT("������Ͷע��%I64d"), m_lAllBet);
	DCBuffer.DrawText(szInfo, rect , DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_LEFT );

	nRecordsY += 15;
	rect.SetRect( 5, nRecordsY, 146, nRecordsY + 15);
	_sntprintf(szInfo, sizeof(szInfo), TEXT("����Ͷע������%I64d"), m_lBetMumber);
	DCBuffer.DrawText(szInfo, rect , DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_LEFT );

	nRecordsY += 20;
	rect.SetRect( 5, nRecordsY, 146, nRecordsY + 15);
	_sntprintf(szInfo, sizeof(szInfo), TEXT("�������Ϊ��%I64d"), m_lPlayerScore);
	DCBuffer.DrawText(szInfo, rect , DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_LEFT );

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&DCBuffer,0,0,SRCCOPY);

	//������Դ
	DCBuffer.SetTextColor(oldColor);
	DCBuffer.SetTextAlign(nTextAlign);
	DCBuffer.SelectObject(oldBitmap);
	DCBuffer.SelectObject(oldFont);
	DCBuffer.DeleteDC();
	ImageBuffer.DeleteObject();
}

void CDialogRecord::OnBnClickedButtonLogok()
{
	ShowWindow(SW_HIDE);
}

//���û���
void CDialogRecord::SetScore( LONGLONG lScore, LONGLONG lAllBet, LONGLONG lBetMumber )
{
	if( m_lPlayerScore != lScore || m_lAllBet != lAllBet || m_lBetMumber != lBetMumber )
	{
		m_lPlayerScore = lScore;
		m_lAllBet = lAllBet;
		m_lBetMumber = lBetMumber;

		if(GetSafeHwnd() && IsWindowVisible())
		{
			Invalidate(FALSE);
		}
	}
}

//�������
CString CDialogRecord::IdentifyAreas( BYTE cbArea )
{
	CString str = TEXT("");
	if ( cbArea == AREA_1_6 )
	{
		str = TEXT("1-6");
	}
	else if( cbArea == AREA_1_5 )
	{
		str = TEXT("1-5");
	}
	else if( cbArea == AREA_1_4 )
	{
		str = TEXT("1-4");
	}
	else if( cbArea == AREA_1_3 )
	{
		str = TEXT("1-3");
	}
	else if( cbArea == AREA_1_2 )
	{
		str = TEXT("1-2");
	}
	else if( cbArea == AREA_2_6 )
	{
		str = TEXT("2-6");
	}
	else if( cbArea == AREA_2_5 )
	{
		str = TEXT("2-5");
	}
	else if( cbArea == AREA_2_4 )
	{
		str = TEXT("2-4");
	}
	else if( cbArea == AREA_2_3 )
	{
		str = TEXT("2-3");
	}
	else if( cbArea == AREA_3_6 )
	{
		str = TEXT("3-6");
	}
	else if( cbArea == AREA_3_5 )
	{
		str = TEXT("3-5");
	}
	else if( cbArea == AREA_3_4 )
	{
		str = TEXT("3-4");
	}
	else if( cbArea == AREA_4_6 )
	{
		str = TEXT("4-6");
	}
	else if( cbArea == AREA_4_5 )
	{
		str = TEXT("4-5");
	}
	else if( cbArea == AREA_5_6 )
	{
		str = TEXT("5-6");
	}
	else 
	{
		ASSERT(FALSE);
	}
	return str;
}