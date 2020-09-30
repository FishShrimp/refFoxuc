// ClientControlItemSink.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "ClientControlItemSink.h"
#include ".\clientcontrolitemsink.h"


// CClientControlItemSinkDlg �Ի���

IMPLEMENT_DYNAMIC(CClientControlItemSinkDlg, IClientControlDlg)

CClientControlItemSinkDlg::CClientControlItemSinkDlg(CWnd* pParent /*=NULL*/)
	: IClientControlDlg(CClientControlItemSinkDlg::IDD, pParent)
{
	ZeroMemory(m_cbWinArea,sizeof(m_cbWinArea));	
	m_cbExcuteTimes=0;			//ִ�д���
}

CClientControlItemSinkDlg::~CClientControlItemSinkDlg()
{
}

void CClientControlItemSinkDlg::DoDataExchange(CDataExchange* pDX)
{
	IClientControlDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientControlItemSinkDlg, IClientControlDlg)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnExcute)
	ON_BN_CLICKED(IDC_BUTTON_RESET,OnReSet)
	ON_BN_CLICKED(IDC_BUTTON_SYN,OnRefresh)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL,OnCancel)
//	ON_WM_TIMER()
END_MESSAGE_MAP()

// CClientControlItemSinkDlg ��Ϣ�������

//��ʼ��
BOOL CClientControlItemSinkDlg::OnInitDialog()
{
	IClientControlDlg::OnInitDialog();

	SetDlgItemText(IDC_STATIC_TIMES,TEXT("���ƾ���:"));
	SetDlgItemText(IDC_STATIC_AREA, TEXT("�������:"));
	SetDlgItemText(IDC_STATIC_NOTIC,TEXT("����˵��:"));
	SetDlgItemText(IDC_STATIC_TEXT,TEXT("������Ӯ���Ʊ���Ϸ�����Ʋ�������!"));
	SetDlgItemText(IDC_CHECK_LONG,TEXT("��"));
	SetDlgItemText(IDC_CHECK_HE,TEXT("��"));
	SetDlgItemText(IDC_CHECK_HU,TEXT("��"));	
	SetDlgItemText(IDC_CHECK_1,TEXT("2-6"));
	SetDlgItemText(IDC_CHECK_2,TEXT("7-11"));
	SetDlgItemText(IDC_CHECK_3,TEXT("12-13"));
	SetDlgItemText(IDC_CHECK_4,TEXT("14"));
	SetDlgItemText(IDC_CHECK_5,TEXT("15-16"));
	SetDlgItemText(IDC_CHECK_6,TEXT("17-21"));
	SetDlgItemText(IDC_CHECK_7,TEXT("22-26"));

	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(TEXT("5"));


	SetDlgItemText(IDC_BUTTON_RESET,TEXT("ȡ������"));
	SetDlgItemText(IDC_BUTTON_SYN,TEXT("��ǰ����"));
	SetDlgItemText(IDC_BUTTON_OK,TEXT("ִ��"));
	SetDlgItemText(IDC_BUTTON_CANCEL,TEXT("ȡ��"));

	return TRUE; 
}

void CClientControlItemSinkDlg::OnReSet()
{
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType=RQ_RESET_CONTROL;
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}

void CClientControlItemSinkDlg::OnRefresh()
{
	CMD_C_AdminReq adminReq;
	adminReq.cbReqType=RQ_PRINT_SYN;
	CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
}


void CClientControlItemSinkDlg::OnExcute()
{
	CMD_C_AdminReq adminReq;
	BYTE cbIndex=0;
	int nCheckID[2]={0,0};
	BYTE CheckCount=0;
	for(cbIndex=0;cbIndex<10;cbIndex++)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_LONG+cbIndex))->GetCheck())
		{
			nCheckID[CheckCount++]=cbIndex+1;
			if(CheckCount>1) break;
		}
	}

	int nSelect=(BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel();

	if(nCheckID[0]>0&&nSelect>=0)
	{
		adminReq.cbReqType=RQ_SET_WIN_AREA;
		adminReq.cbExtendData[0]=(BYTE)nCheckID[0];
		adminReq.cbExtendData[1]=(BYTE)nCheckID[1];
		adminReq.cbExtendData[2]=(BYTE)nSelect+1;
		m_cbWinArea[0]=adminReq.cbExtendData[0];
		m_cbWinArea[1]=adminReq.cbExtendData[1];
		m_cbExcuteTimes=adminReq.cbExtendData[2];

		CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
	}
	else
	{
		AfxMessageBox(TEXT("��ѡ���ܿش����Լ��ܿ�����!"));
		OnRefresh();
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


bool CClientControlItemSinkDlg::ReqResult(const void * pBuffer)
{

	const CMD_S_CommandResult*pResult=(CMD_S_CommandResult*)pBuffer;
	CString str;
	switch(pResult->cbAckType)
	{

	case ACK_SET_WIN_AREA:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str="��Ӯ���������Ѿ����ܣ�";
				/*switch(m_cbWinArea)
				{
				case 0:str.Format(TEXT("���ܿ�����"));break;
				case 1:str.Format(TEXT("�����ѽ���,ʤ������:�ж���,ִ�д���:%d"),m_cbExcuteTimes);break;
				case 2:str.Format(TEXT("�����ѽ���,ʤ������:  ��  ,ִ�д���:%d"),m_cbExcuteTimes);break;
				case 3:str.Format(TEXT("�����ѽ���,ʤ������:������,ִ�д���:%d"),m_cbExcuteTimes);break;
				case 4:str.Format(TEXT("�����ѽ���,ʤ������:  ƽ  ,ִ�д���:%d"),m_cbExcuteTimes);break;
				case 5:str.Format(TEXT("�����ѽ���,ʤ������:ͬ��ƽ,ִ�д���:%d"),m_cbExcuteTimes);break;
				case 6:str.Format(TEXT("�����ѽ���,ʤ������:ׯ����,ִ�д���:%d"),m_cbExcuteTimes);break;
				case 7:str.Format(TEXT("�����ѽ���,ʤ������:  ׯ  ,ִ�д���:%d"),m_cbExcuteTimes);break;
				case 8:str.Format(TEXT("�����ѽ���,ʤ������:ׯ����,ִ�д���:%d"),m_cbExcuteTimes);break;
				case 9:str.Format(TEXT("�����ѽ���,ʤ������:ׯ����,ִ�д���:%d"),m_cbExcuteTimes);break;
				case 10:str.Format(TEXT("�����ѽ���,ʤ������:ׯ����,ִ�д���:%d"),m_cbExcuteTimes);break;
				case 11:str.Format(TEXT("�����ѽ���,ʤ������:ׯ����,ִ�д���:%d"),m_cbExcuteTimes);break;
				default:break;
				}*/
			}
			else
			{
				str.Format(TEXT("����ʧ��!"));
				m_cbExcuteTimes=0;
				ZeroMemory(m_cbWinArea,sizeof(m_cbWinArea));
			}
			break;
		}
	case ACK_RESET_CONTROL:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str="���������ѽ���!";
				for(int nIndex=0;nIndex<10;nIndex++)
					((CButton*)GetDlgItem(IDC_CHECK_LONG+nIndex))->SetCheck(0);
				if(m_cbExcuteTimes>0)
					((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->SetCurSel(-1);
				ZeroMemory(m_cbWinArea,sizeof(m_cbWinArea));
				m_cbExcuteTimes=0;
			}
			break;
		}
	case ACK_PRINT_SYN:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str="������ͬ�������ѽ���!";
				for(int nIndex=0;nIndex<10;nIndex++)
					((CButton*)GetDlgItem(IDC_CHECK_LONG+nIndex))->SetCheck(0);

				m_cbWinArea[0]=pResult->cbExtendData[0];
				m_cbWinArea[1]=pResult->cbExtendData[1];
				m_cbExcuteTimes=pResult->cbExtendData[2];
				if(m_cbWinArea[0]>0)
					((CButton*)GetDlgItem(IDC_CHECK_LONG+m_cbWinArea[0]-1))->SetCheck(1);
				if(m_cbExcuteTimes>0&&m_cbExcuteTimes<=5)
					((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->SetCurSel(m_cbExcuteTimes-1);

				//SetTimer(10,2000,0);
			}
			else
			{
				str.Format(TEXT("����ʧ��!"));
			}
			break;
		}

	default: break;
	}

	SetWindowText(str);
	return true;
}


void CClientControlItemSinkDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	ShowWindow(SW_HIDE);
	//	CDialog::OnCancel();
}

//void CClientControlItemSinkDlg::OnTimer(UINT nIDEvent)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	KillTimer(10);
//	CMD_S_CommandResult Result;
//	Result.cbAckType=ACK_SET_WIN_AREA;
//	Result.cbResult=CR_ACCEPT;
//	ReqResult(&Result);
//	CDialog::OnTimer(nIDEvent);
//}

void __cdecl CClientControlItemSinkDlg::OnAllowControl(bool bEnable)
{
	GetDlgItem(IDC_BUTTON_OK)->EnableWindow(bEnable);
}

