// DialogControl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "resource.h"
#include "DialogControl.h"
#include ".\dialogcontrol.h"


// CDialogControl �Ի���

IMPLEMENT_DYNAMIC(CDialogControl, CDialog)
CDialogControl::CDialogControl(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogControl::IDD, pParent)
{
	memset(m_lAllUserBet,0,sizeof(m_lAllUserBet));
}

CDialogControl::~CDialogControl()
{
}

void CDialogControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_C_1_6, m_editInput[AREA_1_6]);
	DDX_Control(pDX, IDC_EDIT_C_1_5, m_editInput[AREA_1_5]);
	DDX_Control(pDX, IDC_EDIT_C_1_4, m_editInput[AREA_1_4]);
	DDX_Control(pDX, IDC_EDIT_C_1_3, m_editInput[AREA_1_3]);
	DDX_Control(pDX, IDC_EDIT_C_1_2, m_editInput[AREA_1_2]);
	DDX_Control(pDX, IDC_EDIT_C_2_6, m_editInput[AREA_2_6]);
	DDX_Control(pDX, IDC_EDIT_C_2_5, m_editInput[AREA_2_5]);
	DDX_Control(pDX, IDC_EDIT_C_2_4, m_editInput[AREA_2_4]);
	DDX_Control(pDX, IDC_EDIT_C_2_3, m_editInput[AREA_2_3]);
	DDX_Control(pDX, IDC_EDIT_C_3_6, m_editInput[AREA_3_6]);
	DDX_Control(pDX, IDC_EDIT_C_3_5, m_editInput[AREA_3_5]);
	DDX_Control(pDX, IDC_EDIT_C_3_4, m_editInput[AREA_3_4]);
	DDX_Control(pDX, IDC_EDIT_C_4_6, m_editInput[AREA_4_6]);
	DDX_Control(pDX, IDC_EDIT_C_4_5, m_editInput[AREA_4_5]);
	DDX_Control(pDX, IDC_EDIT_C_5_6, m_editInput[AREA_5_6]);

	DDX_Control(pDX, IDC_RADIO_1_6, m_radioArea[AREA_1_6]);
	DDX_Control(pDX, IDC_RADIO_1_5, m_radioArea[AREA_1_5]);
	DDX_Control(pDX, IDC_RADIO_1_4, m_radioArea[AREA_1_4]);
	DDX_Control(pDX, IDC_RADIO_1_3, m_radioArea[AREA_1_3]);
	DDX_Control(pDX, IDC_RADIO_1_2, m_radioArea[AREA_1_2]);
	DDX_Control(pDX, IDC_RADIO_2_6, m_radioArea[AREA_2_6]);
	DDX_Control(pDX, IDC_RADIO_2_5, m_radioArea[AREA_2_5]);
	DDX_Control(pDX, IDC_RADIO_2_4, m_radioArea[AREA_2_4]);
	DDX_Control(pDX, IDC_RADIO_2_3, m_radioArea[AREA_2_3]);
	DDX_Control(pDX, IDC_RADIO_3_6, m_radioArea[AREA_3_6]);
	DDX_Control(pDX, IDC_RADIO_3_5, m_radioArea[AREA_3_5]);
	DDX_Control(pDX, IDC_RADIO_3_4, m_radioArea[AREA_3_4]);
	DDX_Control(pDX, IDC_RADIO_4_6, m_radioArea[AREA_4_6]);
	DDX_Control(pDX, IDC_RADIO_4_5, m_radioArea[AREA_4_5]);
	DDX_Control(pDX, IDC_RADIO_5_6, m_radioArea[AREA_5_6]);
}


BEGIN_MESSAGE_MAP(CDialogControl, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_SYN, OnBnClickedButtonSyn)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO_NWE, OnBnClickedRadioNwe)
	ON_BN_CLICKED(IDC_RADIO_NEXT, OnBnClickedRadioNext)
END_MESSAGE_MAP()


// CDialogControl ��Ϣ�������
//��ʼ��
BOOL CDialogControl::OnInitDialog()
{
	CDialog::OnInitDialog();

	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("5"));

	SetDlgItemText(IDC_STATIC_TIMES,TEXT("���ƾ�����"));
	SetDlgItemText(IDC_STATIC_AREA, TEXT("������ƣ�"));
	SetDlgItemText(IDC_STATIC_NOTIC,TEXT("����˵����"));
	SetDlgItemText(IDC_STATIC_TEXT,TEXT("������Ӯ���Ʊ���Ϸ�����Ʋ������ȣ�"));
	
	SetDlgItemText(IDC_STATIC_ALL_BET,TEXT("��ע����"));
	SetDlgItemText(IDC_ST_AREA1,TEXT("1-6"));
	SetDlgItemText(IDC_ST_AREA2,TEXT("1-5"));
	SetDlgItemText(IDC_ST_AREA3,TEXT("1-4"));
	SetDlgItemText(IDC_ST_AREA4,TEXT("1-3"));
	SetDlgItemText(IDC_ST_AREA5,TEXT("1-2"));

	SetDlgItemText(IDC_ST_AREA6,TEXT("2-6"));
	SetDlgItemText(IDC_ST_AREA7,TEXT("2-5"));
	SetDlgItemText(IDC_ST_AREA8,TEXT("2-4"));
	SetDlgItemText(IDC_ST_AREA9,TEXT("2-3"));

	SetDlgItemText(IDC_ST_AREA10,TEXT("3-6"));
	SetDlgItemText(IDC_ST_AREA11,TEXT("3-5"));
	SetDlgItemText(IDC_ST_AREA12,TEXT("3-4"));

	SetDlgItemText(IDC_ST_AREA13,TEXT("4-6"));
	SetDlgItemText(IDC_ST_AREA14,TEXT("4-5"));

	SetDlgItemText(IDC_ST_AREA15,TEXT("5-6"));

	SetDlgItemText(IDC_RADIO_1_6,TEXT("1-6"));
	SetDlgItemText(IDC_RADIO_1_5,TEXT("1-5"));
	SetDlgItemText(IDC_RADIO_1_4,TEXT("1-4"));
	SetDlgItemText(IDC_RADIO_1_3,TEXT("1-3"));
	SetDlgItemText(IDC_RADIO_1_2,TEXT("1-2"));

	SetDlgItemText(IDC_RADIO_2_6,TEXT("2-6"));
	SetDlgItemText(IDC_RADIO_2_5,TEXT("2-5"));
	SetDlgItemText(IDC_RADIO_2_4,TEXT("2-4"));
	SetDlgItemText(IDC_RADIO_2_3,TEXT("2-3"));

	SetDlgItemText(IDC_RADIO_3_6,TEXT("3-6"));
	SetDlgItemText(IDC_RADIO_3_5,TEXT("3-5"));
	SetDlgItemText(IDC_RADIO_3_4,TEXT("3-4"));

	SetDlgItemText(IDC_RADIO_4_6,TEXT("4-6"));
	SetDlgItemText(IDC_RADIO_4_5,TEXT("4-5"));

	SetDlgItemText(IDC_RADIO_5_6,TEXT("5-6"));

	SetDlgItemText(IDC_RADIO_NWE,TEXT("����ִ��(�޷����ñ���)"));
	SetDlgItemText(IDC_RADIO_NEXT,TEXT("�¾�ִ��"));

	SetDlgItemText(IDC_BUTTON_RESET,TEXT("ȡ������"));
	SetDlgItemText(IDC_BUTTON_SYN,TEXT("������Ϣ"));
	SetDlgItemText(IDC_BUTTON_OK,TEXT("ִ��"));
	SetDlgItemText(IDC_BUTTON_CANCEL,TEXT("ȡ��"));

	((CButton*)GetDlgItem(IDC_RADIO_NWE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_NEXT))->SetCheck(TRUE);
	


	return TRUE;  
}


//������ɫ
HBRUSH CDialogControl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID()==IDC_EDIT_INFO) 
	{  
		pDC->SetTextColor(RGB(255,10,10)); 
	} 

	return hbr;
}

//ȡ������
void CDialogControl::OnBnClickedButtonReset()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication, sizeof(ControlApplication));
	ControlApplication.cbControlAppType = C_CA_CANCELS;
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
}

//���ֿ���
void CDialogControl::OnBnClickedButtonSyn()
{
	CMD_C_ControlApplication ControlApplication;
	ZeroMemory(&ControlApplication, sizeof(ControlApplication));
	ControlApplication.cbControlAppType = C_CA_UPDATE;
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
}

//��������
void CDialogControl::OnBnClickedButtonOk()
{
	//�������
	CMD_C_ControlApplication ControlApplication;

	//��ʼ����
	ControlApplication.cbControlTimes = 0;
	ControlApplication.cbControlAppType = C_CA_SET;
	ControlApplication.cbControlArea = 255;
	for ( int i = 0; i < AREA_ALL; ++i)
		ControlApplication.nControlMultiple[i] = -1;

	//���ִ��ʱ��
	if ( ((CButton*)GetDlgItem(IDC_RADIO_NWE))->GetCheck() )
	{
		ControlApplication.bAuthoritiesExecuted = TRUE;
	}
	else if( ((CButton*)GetDlgItem(IDC_RADIO_NEXT))->GetCheck() )
	{
		ControlApplication.bAuthoritiesExecuted = FALSE;
	}
	
	//��ÿ�������
	bool bControlArea = false;
	for (int i = 0; i < AREA_ALL; ++i )
	{
		if ( m_radioArea[i].GetCheck() )
		{
			ControlApplication.cbControlArea = i;
			bControlArea = true;
			break;
		}
	}

	//��ÿ��Ʊ���
	bool bControlMultiple = false;
	for ( int i = 0 ; i < AREA_ALL; ++i)
	{
		TCHAR szCount[64] = {0};
		m_editInput[i].GetWindowText(szCount, sizeof(szCount));
		int nTemp = _tstoi(szCount);

		if ( nTemp > 0 )
		{
			bControlMultiple = true;
			ControlApplication.nControlMultiple[i] = nTemp;
		}
	}

	//���ʱ��
	int nSelectTimes = ((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel();

	if (  ( bControlMultiple || bControlArea ) && nSelectTimes >= 0 && nSelectTimes != 0xffffffff )
	{
		ControlApplication.cbControlAppType = C_CA_SET;
		ControlApplication.cbControlTimes = static_cast<BYTE>(nSelectTimes + 1);
		CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&ControlApplication,0);
	}
	else
	{
		if ( nSelectTimes == 0xffffffff )
		{
			SetDlgItemText(IDC_EDIT_INFO,TEXT("��ѡ���ܿش�����"));
		}
		else if ( !bControlMultiple && !bControlArea )
		{
			SetDlgItemText(IDC_EDIT_INFO,TEXT("��ѡ������������Ʊ��ʣ�"));
		}
	}
}

//ȡ���ر�
void CDialogControl::OnBnClickedButtonCancel()
{
	ShowWindow(SW_HIDE);
}

//���¿���
void CDialogControl::UpdateControl( CMD_S_ControlReturns* pControlReturns )
{
	switch(pControlReturns->cbReturnsType)
	{
	case S_CR_FAILURE:
		{
			for (int i = 0; i < AREA_ALL; ++i )
			{
				m_radioArea[i].SetCheck(FALSE);
				m_editInput[i].SetWindowText(TEXT(""));
			}
			SetDlgItemText(IDC_EDIT_INFO,TEXT("����ʧ�ܣ�"));
			break;
		}
	case S_CR_UPDATE_SUCCES:
		{
			TCHAR zsText[512] = TEXT("");
			TCHAR zsTextTemp[512] = TEXT("");
			PrintingInfo(zsTextTemp,512,pControlReturns->cbControlArea,pControlReturns->nControlMultiple,pControlReturns->cbControlTimes);
			_sntprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ���\r\n %s"),zsTextTemp);
			SetDlgItemText(IDC_EDIT_INFO,zsText);
			break;
		}
	case S_CR_SET_SUCCESS:
		{
			TCHAR zsText[512] = TEXT("");
			TCHAR zsTextTemp[512] = TEXT("");
			PrintingInfo(zsTextTemp,512,pControlReturns->cbControlArea,pControlReturns->nControlMultiple,pControlReturns->cbControlTimes);
			if( pControlReturns->bAuthoritiesExecuted )
			{
				_sntprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ�����ʼִ�У�\r\n %s"),zsTextTemp);
			}
			else
			{
				_sntprintf(zsText,CountArray(zsText),TEXT("�������ݳɹ�����һ�ֿ�ʼִ�У�\r\n %s"),zsTextTemp);
			}
			SetDlgItemText(IDC_EDIT_INFO,zsText);
			break;
		}
	case S_CR_CANCEL_SUCCESS:
		{
			for (int i = 0; i < AREA_ALL; ++i )
			{
				m_radioArea[i].SetCheck(FALSE);
				m_editInput[i].SetWindowText(TEXT(""));
			}
			SetDlgItemText(IDC_EDIT_INFO,TEXT("ȡ�����óɹ���"));
			break;
		}
	}
}

//��Ϣ
void CDialogControl::PrintingInfo( TCHAR* pText, WORD cbCount, BYTE cbArea, int nMultiple[AREA_ALL], BYTE cbTimes )
{
	TCHAR szDesc[64] = TEXT("");

	//��ȡ���������Լ�����
	_sntprintf(pText,cbCount,TEXT("��������"));
	if ( cbArea >= AREA_1_6 && cbArea < AREA_ALL )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("%s��"), ObtainArea(cbArea) );
		//lstrcat( pText,szDesc );
		_tcscat( pText,szDesc );

		if ( nMultiple[cbArea] > 0 )
		{
			_sntprintf( szDesc,CountArray(szDesc),TEXT("ʤ��������%d��"), nMultiple[cbArea] );
			_tcscat( pText,szDesc );
		}
		else
		{
			_sntprintf( szDesc,CountArray(szDesc),TEXT("ʤ��������������Ĭ��ֵ��") );
			_tcscat( pText,szDesc );
		}

		_sntprintf(szDesc,CountArray(szDesc),TEXT("ִ�д�����%d��\r\n "), cbTimes);
		_tcscat( pText,szDesc );
	}
	else 
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("���ޡ�\r\n ") );
		_tcscat( pText,szDesc );
	}

	//��������
	int nMultipleCount = 0;
	for ( int i = 0; i < AREA_ALL; ++i)
	{
		if ( nMultiple[i] > 0)
		{
			if ( nMultipleCount == 0 )
			{
				_sntprintf( szDesc,CountArray(szDesc),TEXT("���Ʊ�����") );
				_tcscat( pText,szDesc );
			}

			_sntprintf( szDesc,CountArray(szDesc),TEXT("��%s��%d����") , ObtainArea(i), nMultiple[i]);
			_tcscat( pText,szDesc );
			nMultipleCount++;
		}
	}
	if ( nMultipleCount == 0 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("���Ʊ��������ޣ�������Ĭ��ֵ������\r\n") );
		_tcscat( pText,szDesc );
	}
	else if ( nMultipleCount > 0 && nMultipleCount < AREA_ALL )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("����δ����������������Ĭ��ֵ����\r\n") );
		_tcscat( pText,szDesc );
	}
}

void CDialogControl::ResetUserBet()
{
	CString strPrint;
	memset(m_lAllUserBet,0,sizeof(m_lAllUserBet));
	SetDlgItemText(IDC_ST_AREA1,TEXT("1-6"));
	SetDlgItemText(IDC_ST_AREA2,TEXT("1-5"));
	SetDlgItemText(IDC_ST_AREA3,TEXT("1-4"));
	SetDlgItemText(IDC_ST_AREA4,TEXT("1-3"));
	SetDlgItemText(IDC_ST_AREA5,TEXT("1-2"));

	SetDlgItemText(IDC_ST_AREA6,TEXT("2-6"));
	SetDlgItemText(IDC_ST_AREA7,TEXT("2-5"));
	SetDlgItemText(IDC_ST_AREA8,TEXT("2-4"));
	SetDlgItemText(IDC_ST_AREA9,TEXT("2-3"));

	SetDlgItemText(IDC_ST_AREA10,TEXT("3-6"));
	SetDlgItemText(IDC_ST_AREA11,TEXT("3-5"));
	SetDlgItemText(IDC_ST_AREA12,TEXT("3-4"));

	SetDlgItemText(IDC_ST_AREA13,TEXT("4-6"));
	SetDlgItemText(IDC_ST_AREA14,TEXT("4-5"));

	SetDlgItemText(IDC_ST_AREA15,TEXT("5-6"));
}

void CDialogControl::SetUserBetScore(LONGLONG lBetScore[AREA_ALL])
{
	CString strPrint[AREA_ALL];
	for( int i = 0 ; i < AREA_ALL ; ++i )
	{
		m_lAllUserBet[i] += lBetScore[i];

		switch(i)//IDC_ST_AREA1
		{
			SetDlgItemText(IDC_ST_AREA1,TEXT("1-6"));
			SetDlgItemText(IDC_ST_AREA2,TEXT("1-5"));
			SetDlgItemText(IDC_ST_AREA3,TEXT("1-4"));
			SetDlgItemText(IDC_ST_AREA4,TEXT("1-3"));
			SetDlgItemText(IDC_ST_AREA5,TEXT("1-2"));

			SetDlgItemText(IDC_ST_AREA6,TEXT("2-6"));
			SetDlgItemText(IDC_ST_AREA7,TEXT("2-5"));
			SetDlgItemText(IDC_ST_AREA8,TEXT("2-4"));
			SetDlgItemText(IDC_ST_AREA9,TEXT("2-3"));

			SetDlgItemText(IDC_ST_AREA10,TEXT("3-6"));
			SetDlgItemText(IDC_ST_AREA11,TEXT("3-5"));
			SetDlgItemText(IDC_ST_AREA12,TEXT("3-4"));

			SetDlgItemText(IDC_ST_AREA13,TEXT("4-6"));
			SetDlgItemText(IDC_ST_AREA14,TEXT("4-5"));

			SetDlgItemText(IDC_ST_AREA15,TEXT("5-6"));
		case 0: strPrint[i].Format(TEXT("1-6:%I64d"),m_lAllUserBet[i]);break;
		case 1: strPrint[i].Format(TEXT("1-5:%I64d"),m_lAllUserBet[i]);break;
		case 2: strPrint[i].Format(TEXT("1-4:%I64d"),m_lAllUserBet[i]);break;
		case 3: strPrint[i].Format(TEXT("1-3:%I64d"),m_lAllUserBet[i]);break;
		case 4: strPrint[i].Format(TEXT("1-2:%I64d"),m_lAllUserBet[i]);break;
	
		case 5: strPrint[i].Format(TEXT("2-6:%I64d"),m_lAllUserBet[i]);break;
		case 6: strPrint[i].Format(TEXT("2-5:%I64d"),m_lAllUserBet[i]);break;
		case 7: strPrint[i].Format(TEXT("2-4:%I64d"),m_lAllUserBet[i]);break;
		case 8: strPrint[i].Format(TEXT("2-3:%I64d"),m_lAllUserBet[i]);break;

		case 9: strPrint[i].Format(TEXT("3-6:%I64d"),m_lAllUserBet[i]);break;
		case 10: strPrint[i].Format(TEXT("3-5:%I64d"),m_lAllUserBet[i]);break;
		case 11: strPrint[i].Format(TEXT("3-4:%I64d"),m_lAllUserBet[i]);break;
		case 12: strPrint[i].Format(TEXT("4-6:%I64d"),m_lAllUserBet[i]);break;
		case 13: strPrint[i].Format(TEXT("4-5:%I64d"),m_lAllUserBet[i]);break;
		case 14: strPrint[i].Format(TEXT("5-6:%I64d"),m_lAllUserBet[i]);break;
		default:false;
		}
		if(GetSafeHwnd())
			SetDlgItemText(IDC_ST_AREA1+i,strPrint[i]);
	}
	
	//CString strPrint;
	//switch(cbArea)
	//{
	//case 0: strPrint.Format(TEXT("����:%I64d"),m_lAllUserBet[cbArea]);break;
	//case 1: strPrint.Format(TEXT("����:%I64d"),m_lAllUserBet[cbArea]);break;
	//case 2: strPrint.Format(TEXT("����:%I64d"),m_lAllUserBet[cbArea]);break;
	//case 3: strPrint.Format(TEXT("�Ͻ�:%I64d"),m_lAllUserBet[cbArea]);break;
	//case 4: strPrint.Format(TEXT("����:%I64d"),m_lAllUserBet[cbArea]);break;
	//case 5: strPrint.Format(TEXT("�½�:%I64d"),m_lAllUserBet[cbArea]);break;
	//default:false;
	//}
	//if(GetSafeHwnd())
	//	SetDlgItemText(IDC_ST_AREA1+cbArea,strPrint);
}


CString CDialogControl::ObtainArea( BYTE cbArea )
{
	CString str;
	if ( cbArea < AREA_1_6 && cbArea >= AREA_ALL )
	{
		ASSERT(FALSE);
		return str;
	}

	if ( cbArea == AREA_1_6 )
		str = TEXT("1-6");
	else if ( cbArea == AREA_1_5 )
		str = TEXT("1-5");
	else if ( cbArea == AREA_1_4 )
		str = TEXT("1-4");
	else if ( cbArea == AREA_1_3 )
		str = TEXT("1-3");
	else if ( cbArea == AREA_1_2 )
		str = TEXT("1-2");
	else if ( cbArea == AREA_2_6 )
		str = TEXT("2-6");
	else if ( cbArea == AREA_2_5 )
		str = TEXT("2-5");
	else if ( cbArea == AREA_2_4 )
		str = TEXT("2-4");
	else if ( cbArea == AREA_2_3 )
		str = TEXT("2-3");
	else if ( cbArea == AREA_2_6 )
		str = TEXT("3-6");
	else if ( cbArea == AREA_3_5 )
		str = TEXT("3-5");
	else if ( cbArea == AREA_3_4 )
		str = TEXT("3-4");
	else if ( cbArea == AREA_4_6 )
		str = TEXT("4-6");
	else if ( cbArea == AREA_4_5 )
		str = TEXT("4-5");
	else if ( cbArea == AREA_5_6 )
		str = TEXT("5-6");

	return str;
}

//������Ч��ť
void CDialogControl::OnBnClickedRadioNwe()
{
	if(((CButton*)GetDlgItem(IDC_RADIO_NWE))->GetCheck())
	{
		for ( int i = 0; i < AREA_ALL; ++i )
		{
			m_editInput[i].SetWindowText(TEXT(""));
			m_editInput[i].EnableWindow(FALSE);
		}
	}
}

//�¾���Ч��ť
void CDialogControl::OnBnClickedRadioNext()
{
	if(((CButton*)GetDlgItem(IDC_RADIO_NEXT))->GetCheck())
	{
		for ( int i = 0; i < AREA_ALL; ++i )
		{
			m_editInput[i].EnableWindow(TRUE);
		}
	}
}
