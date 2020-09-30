// AdminControlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "resource.h"
#include "AdminControlDlg.h"
#include <set>
#include <algorithm>
#include ".\admincontroldlg.h"
using namespace std;


// CAdminControlDlg �Ի���

IMPLEMENT_DYNAMIC(CAdminControlDlg, CDialog)
CAdminControlDlg::CAdminControlDlg(CWnd* pParent /*=NULL*/)
: CDialog(CAdminControlDlg::IDD, pParent)
{
}

CAdminControlDlg::~CAdminControlDlg()
{
}

void CAdminControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAdminControlDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_SYN, OnBnClickedButtonSyn)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_CHECK_LONG, OnBnClickedCheckLong)
	ON_BN_CLICKED(IDC_CHECK_PING, OnBnClickedCheckPing)
	ON_BN_CLICKED(IDC_CHECK_HU, OnBnClickedCheckHu)
	ON_BN_CLICKED(IDC_CHECK_2_13, OnBnClickedCheck213)
	ON_BN_CLICKED(IDC_CHECK_14, OnBnClickedCheck14)
	ON_BN_CLICKED(IDC_CHECK_15_26, OnBnClickedCheck1526)
	ON_BN_CLICKED(IDC_CHECK_2_6, OnBnClickedCheck26)
	ON_BN_CLICKED(IDC_CHECK_7_11, OnBnClickedCheck711)
	ON_BN_CLICKED(IDC_CHECK_12_16, OnBnClickedCheck1216)
	ON_BN_CLICKED(IDC_CHECK_17_21, OnBnClickedCheck1721)
	ON_BN_CLICKED(IDC_CHECK_22_26, OnBnClickedCheck2226)
END_MESSAGE_MAP()

// CAdminControlDlg ��Ϣ�������

BOOL CAdminControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//������Ϣ
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("5"));
	SetDlgItemText(IDC_STATIC_TIMES,TEXT("���ƾ�����"));
	SetDlgItemText(IDC_STATIC_AREA, TEXT("������ƣ�"));
	SetDlgItemText(IDC_STATIC_NOTIC,TEXT("����˵����"));
	SetDlgItemText(IDC_STATIC_TEXT,TEXT("������Ӯ���Ʊ���Ϸ�����Ʋ������ȣ�"));
	SetDlgItemText(IDC_CHECK_LONG,TEXT("��"));
	SetDlgItemText(IDC_CHECK_PING,TEXT("��"));
	SetDlgItemText(IDC_CHECK_HU,TEXT("��"));
	SetDlgItemText(IDC_CHECK_2_13,TEXT("2-13"));
	SetDlgItemText(IDC_CHECK_14,TEXT("14"));
	SetDlgItemText(IDC_CHECK_15_26,TEXT("15-26"));
	SetDlgItemText(IDC_CHECK_2_6,TEXT("2-6"));
	SetDlgItemText(IDC_CHECK_7_11,TEXT("7-11"));
	SetDlgItemText(IDC_CHECK_12_16,TEXT("12-16"));
	SetDlgItemText(IDC_CHECK_17_21,TEXT("17-21"));
	SetDlgItemText(IDC_CHECK_22_26,TEXT("22-26"));
	SetDlgItemText(IDC_BUTTON_RESET,TEXT("ȡ������"));
	SetDlgItemText(IDC_BUTTON_SYN,TEXT("����"));
	SetDlgItemText(IDC_BUTTON_OK,TEXT("ִ��"));
	SetDlgItemText(IDC_BUTTON_CANCEL,TEXT("ȡ��"));

	return TRUE; 

}

//�ؼ���ɫ
HBRUSH CAdminControlDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID()==IDC_STATIC_INFO) 
	{  
		pDC->SetTextColor(RGB(255,10,10)); 
	} 
	return hbr;
}


//ȡ������
void CAdminControlDlg::OnBnClickedButtonReset()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication, sizeof(ControlApplication));
	ControlApplication.cbControlAppType = C_CA_CANCELS;
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
}

//���ֿ���
void CAdminControlDlg::OnBnClickedButtonSyn()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication, sizeof(ControlApplication));
	ControlApplication.cbControlAppType = C_CA_UPDATE;
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
}

//��������
void CAdminControlDlg::OnBnClickedButtonOk()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication, sizeof(ControlApplication));

	//���ܿ���
	if ( ((CButton*)GetDlgItem(IDC_CHECK_LONG))->GetCheck() )
	{
		ControlApplication.cbControlArea[AREA_LONG] = TRUE;
	}
	else if ( ((CButton*)GetDlgItem(IDC_CHECK_PING))->GetCheck() )
	{
		ControlApplication.cbControlArea[AREA_PING] = TRUE;
	}
	else if ( ((CButton*)GetDlgItem(IDC_CHECK_HU))->GetCheck() )
	{
		ControlApplication.cbControlArea[AREA_HU] = TRUE;
	}

	if ( ((CButton*)GetDlgItem(IDC_CHECK_2_13))->GetCheck() )
	{
		ControlApplication.cbControlArea[AREA_2_13] = TRUE;
	}
	else if ( ((CButton*)GetDlgItem(IDC_CHECK_14))->GetCheck() )
	{
		ControlApplication.cbControlArea[AREA_14] = TRUE;
	}
	else if ( ((CButton*)GetDlgItem(IDC_CHECK_15_26))->GetCheck() )
	{
		ControlApplication.cbControlArea[AREA_15_26] = TRUE;
	}

	if ( ((CButton*)GetDlgItem(IDC_CHECK_2_6))->GetCheck() )
	{
		ControlApplication.cbControlArea[AREA_2_6] = TRUE;
	}
	else if ( ((CButton*)GetDlgItem(IDC_CHECK_7_11))->GetCheck() )
	{
		ControlApplication.cbControlArea[AREA_7_11] = TRUE;
	}
	else if ( ((CButton*)GetDlgItem(IDC_CHECK_12_16))->GetCheck() )
	{
		ControlApplication.cbControlArea[AREA_12_16] = TRUE;
	}
	else if ( ((CButton*)GetDlgItem(IDC_CHECK_17_21))->GetCheck() )
	{
		ControlApplication.cbControlArea[AREA_17_21] = TRUE;
	}
	else if ( ((CButton*)GetDlgItem(IDC_CHECK_22_26))->GetCheck() )
	{
		ControlApplication.cbControlArea[AREA_22_26] = TRUE;
	}


	int nSelectTimes = ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel();

	//�Ƿ��п���
	bool bNoControl = false;
	for ( int i = 0; i < AREA_ALL; ++i)
	{
		if ( ControlApplication.cbControlArea[i] == TRUE )
		{
			bNoControl = true;
			break;
		}
	}

	if ( bNoControl && nSelectTimes >= 0 && nSelectTimes != 0xffffffff)
	{
		if ( !JudgeFeasibility(ControlApplication.cbControlArea) )
		{
			SetDlgItemText(IDC_STATIC_INFO,TEXT("��ѡ������޺��ʺϼ������ܻ���ɷ�����̱����������ѡ��"));
			return;
		}
		ControlApplication.cbControlAppType = C_CA_SET;
		ControlApplication.cbControlTimes = static_cast<BYTE>(nSelectTimes + 1);
		CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
	}
	else
	{
		if ( !bNoControl && nSelectTimes == 0xffffffff )
		{
			SetDlgItemText(IDC_STATIC_INFO,TEXT("��ѡ���ܿش����Լ��ܿ�����"));
		}
		else if ( !bNoControl )
		{
			SetDlgItemText(IDC_STATIC_INFO,TEXT("��ѡ���ܿ�����"));
		}
		else if ( nSelectTimes == 0xffffffff )
		{
			SetDlgItemText(IDC_STATIC_INFO,TEXT("��ѡ���ܿش�����"));
		}
	}
}

//ȡ���ر�
void CAdminControlDlg::OnBnClickedButtonCancel()
{
	ShowWindow(SW_HIDE);
}

//���¿���
void CAdminControlDlg::UpdateControl( CMD_S_ControlReturns* pControlReturns )
{
	switch(pControlReturns->cbReturnsType)
	{
	case S_CR_FAILURE:
		{
			SetDlgItemText(IDC_STATIC_INFO,TEXT("����ʧ�ܣ�"));
			break;
		}
	case S_CR_UPDATE_SUCCES:
		{
			TCHAR zsText[256] = TEXT("");
			TCHAR zsTextTemp[256] = TEXT("");
			PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
			_sntprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
			SetDlgItemText(IDC_STATIC_INFO,zsText);
			break;
		}
	case S_CR_SET_SUCCESS:
		{
			TCHAR zsText[256] = TEXT("");
			TCHAR zsTextTemp[256] = TEXT("");
			PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
			_sntprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
			SetDlgItemText(IDC_STATIC_INFO,zsText);
			break;
		}
	case S_CR_CANCEL_SUCCESS:
		{
			SetDlgItemText(IDC_STATIC_INFO,TEXT("ȡ�����óɹ���"));
			break;
		}
	}
}

//��Ϣ
void CAdminControlDlg::PrintingInfo( TCHAR* pText, WORD cbCount, BYTE cbArea[AREA_ALL], BYTE cbTimes )
{
	bool bNoControl = false;
	for ( int i = 0; i < AREA_ALL; ++i)
	{
		if ( cbArea[i] == TRUE )
		{
			bNoControl = true;
			break;
		}
	}
	if ( !bNoControl )
	{
		_sntprintf(pText,cbCount,TEXT("��ʱ�޿��ơ�"));
		return;
	}

	TCHAR szDesc[32] = TEXT("");
	_sntprintf(pText,cbCount,TEXT("ʤ������"));

	if ( cbArea[AREA_LONG] == TRUE )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("��������") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea[AREA_PING] == TRUE )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("��ƽ����") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea[AREA_HU] == TRUE )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("��������") );
		lstrcat( pText,szDesc );
	}

	if ( cbArea[AREA_2_13] == TRUE )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("��2-13����") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea[AREA_14] == TRUE )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("��14����") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea[AREA_15_26] == TRUE )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("��15-16����") );
		lstrcat( pText,szDesc );
	}


	if ( cbArea[AREA_2_6] == TRUE )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("��2-6����") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea[AREA_7_11] == TRUE )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("��7-11����") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea[AREA_12_16] == TRUE )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("��12-16����") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea[AREA_17_21] == TRUE )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("��17-21����") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea[AREA_22_26] == TRUE )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("��22-26����") );
		lstrcat( pText,szDesc );
	}


	_sntprintf(szDesc,CountArray(szDesc),TEXT("ִ�д�����%d��"), cbTimes);
	lstrcat( pText,szDesc );
}

//�жϿ�����
bool CAdminControlDlg::JudgeFeasibility( BYTE cbArea[AREA_ALL] )
{
	// 1 - 26
	// ��3������. �������������ж�. �����ж���ֻ��2������
	set<BYTE>  setStudyOne;
	set<BYTE>  setStudyTwo;

	if ( cbArea[AREA_2_13] == TRUE )
	{
		BYTE cbTemp[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
		for (int i = 0; i < CountArray(cbTemp); ++i)
			setStudyOne.insert(cbTemp[i]);
	}
	else if ( cbArea[AREA_14] == TRUE )
	{
		BYTE cbTemp[] = {14};
		for (int i = 0; i < CountArray(cbTemp); ++i)
			setStudyOne.insert(cbTemp[i]);
	}
	else if ( cbArea[AREA_15_26] == TRUE )
	{
		BYTE cbTemp[] = {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};
		for (int i = 0; i < CountArray(cbTemp); ++i)
			setStudyOne.insert(cbTemp[i]);
	}
	else
	{
		BYTE cbTemp[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};
		for (int i = 0; i < CountArray(cbTemp); ++i)
			setStudyOne.insert(cbTemp[i]);
	}

	if ( cbArea[AREA_2_6] == TRUE )
	{
		BYTE cbTemp[] = {2, 3, 4, 5, 6};
		for (int i = 0; i < CountArray(cbTemp); ++i)
			setStudyTwo.insert(cbTemp[i]);
	}
	else if ( cbArea[AREA_7_11] == TRUE )
	{
		BYTE cbTemp[] = {7, 8, 9, 10, 11};
		for (int i = 0; i < CountArray(cbTemp); ++i)
			setStudyTwo.insert(cbTemp[i]);
	}
	else if ( cbArea[AREA_12_16] == TRUE )
	{
		BYTE cbTemp[] = {12, 13, 14, 15, 16};
		for (int i = 0; i < CountArray(cbTemp); ++i)
			setStudyTwo.insert(cbTemp[i]);
	}
	else if ( cbArea[AREA_17_21] == TRUE )
	{
		BYTE cbTemp[] = {17, 18, 19, 20, 21};
		for (int i = 0; i < CountArray(cbTemp); ++i)
			setStudyTwo.insert(cbTemp[i]);
	}
	else if ( cbArea[AREA_22_26] == TRUE )
	{
		BYTE cbTemp[] = {22, 23, 24, 25, 26};
		for (int i = 0; i < CountArray(cbTemp); ++i)
			setStudyTwo.insert(cbTemp[i]);
	}
	else
	{
		BYTE cbTemp[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};
		for (int i = 0; i < CountArray(cbTemp); ++i)
			setStudyTwo.insert(cbTemp[i]);
	}


	if ( setStudyOne.size() > 26 || setStudyTwo.size() > 26 )
	{
		ASSERT(FALSE);
		return false;
	}

	set<int> setIntersectionTempOne;
	set<int>::iterator itOne = setIntersectionTempOne.begin();
	set_intersection(setStudyOne.begin(), setStudyOne.end(), setStudyTwo.begin(), setStudyTwo.end(), inserter(setIntersectionTempOne,itOne));

	if ( setIntersectionTempOne.size() > 0 )
	{
		if ( cbArea[AREA_LONG] == TRUE || cbArea[AREA_HU] == TRUE )
		{
			for ( set<int>::iterator itr = setIntersectionTempOne.begin(); itr != setIntersectionTempOne.end(); itr++)
			{
				if ( (*itr) > 2 )
				{
					return true;
				}
			}
			return false;
		}
		else if ( cbArea[AREA_PING] == TRUE )
		{
			for ( set<int>::iterator itr = setIntersectionTempOne.begin(); itr != setIntersectionTempOne.end(); itr++)
			{
				if ( (*itr) % 2 == 0 )
				{
					return true;
				}
			}
			return false;
		}
		return true;
	}
	return false;
}


void CAdminControlDlg::OnBnClickedCheckLong()
{
	((CButton*)GetDlgItem(IDC_CHECK_PING))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_HU))->SetCheck(FALSE);
}

void CAdminControlDlg::OnBnClickedCheckPing()
{
	((CButton*)GetDlgItem(IDC_CHECK_LONG))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_HU))->SetCheck(FALSE);
}

void CAdminControlDlg::OnBnClickedCheckHu()
{
	((CButton*)GetDlgItem(IDC_CHECK_PING))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_LONG))->SetCheck(FALSE);
}

void CAdminControlDlg::OnBnClickedCheck213()
{
	((CButton*)GetDlgItem(IDC_CHECK_14))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_15_26))->SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_CHECK_17_21))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_22_26))->SetCheck(FALSE);
}

void CAdminControlDlg::OnBnClickedCheck14()
{
	((CButton*)GetDlgItem(IDC_CHECK_2_13))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_15_26))->SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_CHECK_2_6))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_7_11))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_17_21))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_22_26))->SetCheck(FALSE);
}

void CAdminControlDlg::OnBnClickedCheck1526()
{
	((CButton*)GetDlgItem(IDC_CHECK_2_13))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_14))->SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_CHECK_2_6))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_7_11))->SetCheck(FALSE);
}

void CAdminControlDlg::OnBnClickedCheck26()
{
	((CButton*)GetDlgItem(IDC_CHECK_14))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_15_26))->SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_CHECK_7_11))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_12_16))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_17_21))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_22_26))->SetCheck(FALSE);
}

void CAdminControlDlg::OnBnClickedCheck711()
{
	((CButton*)GetDlgItem(IDC_CHECK_14))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_15_26))->SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_CHECK_2_6))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_12_16))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_17_21))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_22_26))->SetCheck(FALSE);
}

void CAdminControlDlg::OnBnClickedCheck1216()
{
	((CButton*)GetDlgItem(IDC_CHECK_2_6))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_7_11))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_17_21))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_22_26))->SetCheck(FALSE);
}

void CAdminControlDlg::OnBnClickedCheck1721()
{
	((CButton*)GetDlgItem(IDC_CHECK_2_13))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_14))->SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_CHECK_2_6))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_7_11))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_12_16))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_22_26))->SetCheck(FALSE);
}

void CAdminControlDlg::OnBnClickedCheck2226()
{
	((CButton*)GetDlgItem(IDC_CHECK_2_13))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_14))->SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_CHECK_2_6))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_7_11))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_12_16))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_17_21))->SetCheck(FALSE);
}
