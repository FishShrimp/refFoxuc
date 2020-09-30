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
	
	m_lStorage=0;
	m_lDeduct=0;
}

CClientControlItemSinkDlg::~CClientControlItemSinkDlg()
{
}

void CClientControlItemSinkDlg::DoDataExchange(CDataExchange* pDX)
{
	IClientControlDlg::DoDataExchange(pDX);
}

//������
bool  CClientControlItemSinkDlg::ReqResult(const void * pBuffer)
{
	const CMD_S_CommandResult*pResult=(CMD_S_CommandResult*)pBuffer;
	CString str;
	switch(pResult->cbAckType)
	{
	case ACK_SET_WIN_AREA:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str=_T("��Ӯ���������Ѿ����ܣ�");
				SetTimer(1,1000,0);
			}
			else
			{
				str.Format(_T("����ʧ��!"));
				m_cbExcuteTimes=0;
				m_cbControlStyle=0;
				ZeroMemory(m_bWinArea,sizeof(m_bWinArea));
				SetTimer(1,1000,0);
			}
			break;
		}
	case ACK_RESET_CONTROL:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				m_cbControlStyle=0;
				m_cbExcuteTimes=0;
				ZeroMemory(m_bWinArea,sizeof(m_bWinArea));
				ReSetAdminWnd();
				str="���������ѽ���!";
				SetTimer(1,1000,0);
			}
			break;
		}
	case ACK_PRINT_SYN:
		{
			if(pResult->cbResult==CR_ACCEPT)
			{  
				str="������ͬ�������ѽ���!";
				
				tagAdminReq*pAdminReq=(tagAdminReq*)pResult->cbExtendData;
				m_cbExcuteTimes=pAdminReq->m_cbExcuteTimes;
				m_cbControlStyle=pAdminReq->m_cbControlStyle;
				memcpy(m_bWinArea,pAdminReq->m_bWinArea,sizeof(m_bWinArea));
				ReSetAdminWnd();
				SetTimer(1,1000,0);
			}
			else
			{
				str.Format(_T("����ʧ��!"));
			}
			break;
		}

	default: break;
	}

	SetWindowText(str);
	return true;
}

void CClientControlItemSinkDlg::ReSetAdminWnd()
{
	if(m_cbControlStyle!=CS_BANKER_WIN&&m_cbControlStyle!=CS_BANKER_LOSE)
	{
		((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_CT_BANKER))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_WIN))->SetCheck(m_cbControlStyle==CS_BANKER_WIN?1:0);
		((CButton*)GetDlgItem(IDC_RADIO_LOSE))->SetCheck(m_cbControlStyle==CS_BANKER_LOSE?1:0);
	}

	if(m_cbControlStyle==CS_BET_AREA)
	{
		((CButton*)GetDlgItem(IDC_RADIO_CT_BANKER))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->SetCheck(1);

		BYTE cbIndex=0;
		for(;cbIndex<CONTROL_AREA;cbIndex++)
			((CButton*)GetDlgItem(IDC_CHECK_TIAN+cbIndex))->SetCheck(m_bWinArea[cbIndex]?1:0);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->SetCurSel(m_cbExcuteTimes>0?(m_cbExcuteTimes-1):-1);
	OnRadioClick();
}

BEGIN_MESSAGE_MAP(CClientControlItemSinkDlg, IClientControlDlg)
	ON_BN_CLICKED(IDC_BT_RESET,OnReSet)
	ON_BN_CLICKED(IDC_BT_CANCEL,OnCancel)
	ON_BN_CLICKED(IDC_BT_EXCUTE,OnExcute)
	ON_BN_CLICKED(IDC_BT_CURSET,OnRefresh)
	ON_BN_CLICKED(IDC_RADIO_CT_AREA,OnRadioClick)
	ON_BN_CLICKED(IDC_RADIO_CT_BANKER,OnRadioClick)
	ON_BN_CLICKED(IDC_BTN_UPDATE_STORAGE, OnBnClickedBtnUpdateStorage)
END_MESSAGE_MAP()

// CClientControlItemSinkDlg ��Ϣ�������

void CClientControlItemSinkDlg::PrintCurStatus()
{
	CString str;
	switch(m_cbControlStyle)
	{
	case CS_BET_AREA:
		{
			str="ʤ������:";
			BYTE cbIndex=0;
			bool bFlags=false;
			for(cbIndex=0;cbIndex<AREA_COUNT;cbIndex++)
			{
				if(m_bWinArea[cbIndex])
				{
					bFlags=true;
					switch(cbIndex)
					{
						case 0:str+="�� ";break;
						case 1:str+="�� ";break;
						case 2:str+="�� ";break;
						case 3:str+="�� ";break;
						default: break;
					}
				}
			}
			if(!bFlags)
			{
				str+="ׯ��ͨɱ";
			}
			str.AppendFormat(_T(",ִ�д���:%d"),m_cbExcuteTimes);
			break;
		}
	case CS_BANKER_WIN:
		{
			str.Format(_T("ׯ���ܿ�,�ܿط�ʽ:Ӯ,ִ�д���:%d"),m_cbExcuteTimes);break;
		}
	case CS_BANKER_LOSE:
		{
			str.Format(_T("ׯ���ܿ�,�ܿط�ʽ:��,ִ�д���:%d"),m_cbExcuteTimes);break;
		}
	default: str=_T("�ǿ���״̬");break;
	}	

	SetWindowText(str);
}

void CClientControlItemSinkDlg::OnRadioClick()
{
	if(((CButton*)GetDlgItem(IDC_RADIO_CT_BANKER))->GetCheck()==1)
	{
		GetDlgItem(IDC_RADIO_LOSE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_WIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_TIAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_DI)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_XUAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_HUANG)->EnableWindow(FALSE);
	}
	else
	{
		if(((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->GetCheck())
		{
			GetDlgItem(IDC_RADIO_LOSE)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_WIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_TIAN)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_DI)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_XUAN)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_HUANG)->EnableWindow(TRUE);
		}
	}
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
	BYTE cbIndex=0;
	bool bFlags=false;
	m_cbControlStyle=0;
	m_cbExcuteTimes=0;
	ZeroMemory(m_bWinArea,sizeof(m_bWinArea));

	//����ׯ��
	if(((CButton*)GetDlgItem(IDC_RADIO_CT_BANKER))->GetCheck())
	{
		if(((CButton*)GetDlgItem(IDC_RADIO_WIN))->GetCheck())
		{
			m_cbControlStyle=CS_BANKER_WIN;
			bFlags=true;
		}
		else
		{
			if(((CButton*)GetDlgItem(IDC_RADIO_LOSE))->GetCheck())
			{
				m_cbControlStyle=CS_BANKER_LOSE;
				bFlags=true;
			}
		}
	}
	else //��������
	{
		if(((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->GetCheck())
		{
			m_cbControlStyle=CS_BET_AREA;
			for(cbIndex=0;cbIndex<CONTROL_AREA;cbIndex++)
				m_bWinArea[cbIndex]=((CButton*)GetDlgItem(IDC_CHECK_TIAN+cbIndex))->GetCheck()?true:false;
			bFlags=true;
		}
	}

	m_cbExcuteTimes=(BYTE)((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->GetCurSel()+1;
	//��ȡִ�д���
	if(m_cbExcuteTimes<=0)
	{
		bFlags=false;
	}
	

	if(bFlags) //������Ч
	{
		CMD_C_AdminReq adminReq;
		adminReq.cbReqType=RQ_SET_WIN_AREA;
		tagAdminReq*pAdminReq=(tagAdminReq*)adminReq.cbExtendData;
		pAdminReq->m_cbExcuteTimes=m_cbExcuteTimes;
		pAdminReq->m_cbControlStyle=m_cbControlStyle;
		memcpy(pAdminReq->m_bWinArea,m_bWinArea,sizeof(m_bWinArea));
		CGameFrameEngine::GetInstance()->SendMessage(IDM_ADMIN_COMMDN,(WPARAM)&adminReq,0);
	}
	else
	{
		AfxMessageBox(_T("��ѡ���ܿش����Լ��ܿط�ʽ!"));
		OnRefresh();
	}
}

void CClientControlItemSinkDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::OnCancel();
}

//��ʼ��
BOOL CClientControlItemSinkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetDlgItemText(IDC_BT_RESET,_T("ȡ������"));	
	SetDlgItemText(IDC_BT_CURSET,_T("��ǰ����"));
	SetDlgItemText(IDC_BT_EXCUTE,_T("ִ��"));	
	SetDlgItemText(IDC_BT_CANCEL,_T("ȡ��"));	
	SetDlgItemText(IDC_RADIO_WIN,_T("ׯ��Ӯ"));	
	SetDlgItemText(IDC_RADIO_LOSE,_T("ׯ����"));			 
	SetDlgItemText(IDC_RADIO_CT_BANKER,_T("ׯ�ҿ���"));	
	SetDlgItemText(IDC_RADIO_CT_AREA,_T("�������"));	
	SetDlgItemText(IDC_CHECK_TIAN,_T("��"));	
	SetDlgItemText(IDC_CHECK_DI,_T("��"));	
	SetDlgItemText(IDC_CHECK_XUAN,_T("��"));
	SetDlgItemText(IDC_CHECK_HUANG,_T("��"));
	SetDlgItemText(IDC_STATIC_TIMES	,_T("���ƴ���"));	
	SetDlgItemText(IDC_STATIC_CHOICE,_T("����ѡ��"));	
	SetDlgItemText(IDC_STATIC_NOTICE,_T("˵��"));
	SetDlgItemText(IDC_STATIC_NOTICE1,_T("1.������Ӯ���Ʊ���Ϸ�����Ʋ������ȡ�"));	
	SetDlgItemText(IDC_STATIC_NOTICE2,_T("2.������ƣ�ѡ��Ϊʤ��δѡ��Ϊ�䡣"));	

	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(_T("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(_T("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(_T("3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(_T("4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_TIMES))->AddString(_T("5"));

	((CButton*)GetDlgItem(IDC_RADIO_CT_AREA))->SetCheck(1);
	OnRadioClick();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void  CClientControlItemSinkDlg::OnAllowControl(bool bEnable)
{
	GetDlgItem(IDC_BT_EXCUTE)->EnableWindow(bEnable);
}

void CClientControlItemSinkDlg::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillTimer(1);
	PrintCurStatus();
	CDialog::OnTimer(nIDEvent);
}

//���¿��
void CClientControlItemSinkDlg::OnBnClickedBtnUpdateStorage()
{
	//��ȡ�ַ�
	CString strStorage=TEXT(""),strDeduct=TEXT("");
	GetDlgItemText(IDC_EDIT_STORAGE,strStorage);
	//ȥ���ո�
	strStorage.TrimLeft();
	strStorage.TrimRight();

	GetDlgItemText(IDC_EDIT_DEDUCT,strDeduct);
	//ȥ���ո�
	strDeduct.TrimLeft();
	strDeduct.TrimRight();
	LONGLONG lStorage=0L,lDeduct=0L;
	lStorage = StrToInt(strStorage);
	lDeduct = StrToInt(strDeduct);

	CMD_C_UpdateStorage adminReq;
	ZeroMemory(&adminReq,sizeof(adminReq));
	adminReq.cbReqType=RQ_SET_STORAGE;
	adminReq.lStorage=lStorage;
	adminReq.lStorageDeduct=lDeduct;


	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	if (pGameFrameEngine!=NULL) pGameFrameEngine->SendMessage(IDM_UPDATE_STORAGE,(WPARAM)&adminReq,0);
}

void CClientControlItemSinkDlg::RequestUpdateStorage()
{
	CMD_C_UpdateStorage adminReq;
	ZeroMemory(&adminReq,sizeof(adminReq));
	adminReq.cbReqType=RQ_REFRESH_STORAGE;

	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	if (pGameFrameEngine!=NULL) 
	{		
		pGameFrameEngine->SendMessage(IDM_UPDATE_STORAGE,(WPARAM)&adminReq,0);
		//AfxMessageBox(_T("IDM_UPDATE_STORAGE  ����"));
	}

}


//���¿��
bool __cdecl CClientControlItemSinkDlg::UpdateStorage(const void * pBuffer)
{

	const CMD_S_UpdateStorage*pResult=(CMD_S_UpdateStorage*)pBuffer;

	//��ȡ�ַ�
	CString strStorage=TEXT(""),strDeduct=TEXT("");
	m_lStorage=pResult->lStorage;
	m_lDeduct=pResult->lStorageDeduct;

	//��ȡ�ַ�
	strStorage.Format(TEXT("%I64d"),m_lStorage);
	strDeduct.Format(TEXT("%I64d"),m_lDeduct);

	SetDlgItemText(IDC_EDIT_STORAGE,strStorage);
	SetDlgItemText(IDC_EDIT_DEDUCT,strDeduct);

	SetWindowText(TEXT("�����³ɹ���"));

	return true;
}

//���¿ؼ�
void __cdecl CClientControlItemSinkDlg::UpdateControl()
{

	RequestUpdateStorage();

}
