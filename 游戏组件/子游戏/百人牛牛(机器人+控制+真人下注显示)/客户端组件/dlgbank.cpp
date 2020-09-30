// DlgBank.cpp : implementation file
//

#include "stdafx.h"
#include "DlgBank.h"
#include "resource.h"
#include ".\dlgbank.h"
#include "GameLogic.h"

///////////////////////////////////////////////////////////////
#define IDC_OK				500
#define IDC_CANCEL			501
#define	IDC_FRESH			502

//��ʱ��I D
#define IDI_CHARMVALUE_UPDATE_VIEW		1								//���½���
#define TIME_CHARMVALUE_UPDATE_VIEW		200								//���½���


void MakeString(CString &strNum,LONGLONG lNumber)
{
	CString strTempNum;
	strTempNum.Format(_T("%I64d"), (lNumber>0?lNumber:-lNumber));

	int nLength = strTempNum.GetLength();
	for (int i = 0; i < int((nLength-1)/3); i++)
		strTempNum.Insert(nLength - 3*(i+1), _T(","));
	strNum.Format(_T("%s%s"), (lNumber<0?_T("-"):_T("")), strTempNum);
}

void MakeStringToNum(CString str,LONGLONG &Num)
{
	CString strNumber = str;
	strNumber.Remove(',');
	strNumber.Trim();
	myscanf(strNumber.GetBuffer(),strNumber.GetLength(),_TEXT("%I64d"),&Num);

	return;
}

IMPLEMENT_DYNAMIC(CDlgBank, CSkinDialog)
CDlgBank::CDlgBank(CWnd* pParent /*=NULL*/)
: CSkinDialog(IDD_BANK_STORAGE, pParent)
{
	m_lInCount=0;
	m_lGameGold=0;
	m_bCanDraw=true;
	//	m_bDrawAllGold=false;
	//	m_bStorageAllGold=false;
	m_lStorageGold=0;
	m_pMeUserData=NULL;
	m_pIClientKernel=NULL;
	m_bBankStorage=true;
	m_ImageFrame.LoadImage(AfxGetInstanceHandle(),TEXT("BANK_FRAME"));	
	m_ImageNumber.LoadImage(AfxGetInstanceHandle(),TEXT("BANKNUM"));
}

CDlgBank::~CDlgBank()
{
}

void CDlgBank::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	//DDX_Control(pDX, IDOK, m_btOK);
	//DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_USER_PASSWORD, m_Edit);
	DDX_Control(pDX, IDC_STATIC_PASS, m_Static);
}


BEGIN_MESSAGE_MAP(CDlgBank, CSkinDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()	
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_OK,OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL,OnCancelEvent)
	ON_EN_CHANGE(IDC_IN_COUNT, OnEnChangeInCount)
END_MESSAGE_MAP()


void CDlgBank::OnEnRadioStore()
{
	SetBankerActionType(true);
}

void CDlgBank::OnEnRadioGet()
{

	SetBankerActionType(false);
}

void CDlgBank::AllowStorage(bool bAllow)
{
	m_blCanStore=bAllow;
	ShowItem();
}

void CDlgBank::AllowDraw(bool bAllow)
{
	m_bCanDraw=bAllow;
	ShowItem();
}

//�滭����
VOID CDlgBank::DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos)
{
	//ת������
	TCHAR szControl[128]=TEXT("");
	CString cs;

	MakeString(cs,lScore);
	_sntprintf(szControl,sizeof(szControl),TEXT("%s"),cs);

	//��������
	INT nXDrawPos=nXPos;
	INT nScoreLength=lstrlen(szControl);

	//�滭�ж�
	if (nScoreLength>0L)
	{
		////������Դ
		//CPngImage ImageNumber;
		//ImageNumber.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("BANKNUM"));

		//��ȡ��С
		CSize SizeNumber;
		SizeNumber.SetSize(m_ImageNumber.GetWidth()/12L,m_ImageNumber.GetHeight());

		//�滭����
		for (INT i=0;i<nScoreLength;i++)
		{
			//�滭����
			if (szControl[i]==TEXT(','))
			{
				m_ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*10L,0L);
			}

			//�滭���
			if (szControl[i]==TEXT('.'))
			{
				m_ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*11L,0L);
			}

			//�滭����
			if (szControl[i]>=TEXT('0')&&szControl[i]<=TEXT('9'))
			{
				m_ImageNumber.DrawImage(pDC,nXDrawPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*(szControl[i]-TEXT('0')),0L);
			}

			//����λ��
			nXDrawPos+=SizeNumber.cx;
		}
	}

	return;
}

//��ȡ��ť
void CDlgBank::OnBnClickedOk()
{

	CString strinCount;
	GetDlgItemText(IDC_IN_COUNT,strinCount);
	MakeStringToNum(strinCount,m_lInCount);

	if(((CButton*)GetDlgItem(IDC_CHECK_ALL))->GetCheck()==1)
	{
		if(!m_bBankStorage)
			m_lInCount=m_pMeUserData->GetUserInsure();
		else
			m_lInCount=m_pMeUserData->GetUserInsure();
	}

	//������֤
	if (false==m_bBankStorage && (m_lInCount <= 0 || m_lInCount > m_lStorageGold ))
	{
		CString strMessage;
		if(m_lStorageGold>0)strMessage.Format(TEXT("���������Ϸ��ֵ������1��%I64d֮��"), m_lStorageGold);
		else strMessage.Format(TEXT("��Ĵ洢��Ϸ����ĿΪ0,���ܽ�����ȡ����!"));
		MessageBox(strMessage,TEXT("��ܰ��ʾ"));

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		GetDlgItem(IDC_USER_PASSWORD)->SetWindowText(_TEXT(""));
		return;
	}


	//������֤
	if (true==m_bBankStorage && (m_lInCount <= 0 || m_lInCount > m_lGameGold ))
	{
		CString strMessage;
		if(m_lGameGold>0)strMessage.Format(TEXT("���������Ϸ��ֵ������1��%I64d֮��"),m_lGameGold );
		else strMessage.Format(TEXT("��ĵ�ǰ��Ϸ����ĿΪ0,���ܽ��д洢����!"));
		MessageBox(strMessage,TEXT("��ܰ��ʾ"));

		GetDlgItem(IDC_IN_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(0,-1);
		GetDlgItem(IDC_USER_PASSWORD)->SetWindowText(_TEXT(""));
		return;
	}
	//������֤
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");
	GetDlgItemText(IDC_USER_PASSWORD,szPassword,CountArray(szPassword));

	if (szPassword[0]==0&&m_blUsingPassWord)
	{
		MessageBox(TEXT("���벻��Ϊ�գ��������������룡"),TEXT("��ܰ��ʾ"));
		GetDlgItem(IDC_USER_PASSWORD)->SetWindowText(_TEXT(""));
		GetDlgItem(IDC_USER_PASSWORD)->SetFocus();
		return;
	}

	if(m_blUsingPassWord)
	{
		TCHAR szTempPassword[LEN_PASSWORD]=TEXT("");
		CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
		CWHEncrypt::MD5Encrypt(szTempPassword,szPassword);
	}

	if (false==m_bBankStorage)
	{
		//��������
		CMD_GR_C_TakeScoreRequest TakeScoreRequest;
		ZeroMemory(&TakeScoreRequest,sizeof(TakeScoreRequest));
		TakeScoreRequest.cbActivityGame=TRUE;
		TakeScoreRequest.lTakeScore=m_lInCount;
		CopyMemory(TakeScoreRequest.szInsurePass, szPassword, sizeof(TakeScoreRequest.szInsurePass));

		//��������
		m_pIClientKernel->SendSocketData(MDM_GR_INSURE,SUB_GR_TAKE_SCORE_REQUEST,&TakeScoreRequest,sizeof(TakeScoreRequest));
	}
	else
	{
		//״̬�ж�
		if (US_PLAYING==m_pMeUserData->GetUserStatus())
		{
			MessageBox(TEXT("�������Ϸ���ٴ�"),TEXT("��ܰ��ʾ"));
			return;	
		}
		//��������
		CMD_GR_C_SaveScoreRequest SaveScoreRequest;
		SaveScoreRequest.cbActivityGame=TRUE;
		SaveScoreRequest.lSaveScore=m_lInCount;

		//��������
		m_pIClientKernel->SendSocketData(MDM_GR_INSURE,SUB_GR_SAVE_SCORE_REQUEST,&SaveScoreRequest,sizeof(SaveScoreRequest));
	}

	SetDlgItemText(IDC_USER_PASSWORD,TEXT(""));
	return;
}

//��ʼ������
BOOL CDlgBank::OnInitDialog()
{
	CSkinDialog::OnInitDialog();
	UpdateData(FALSE);
	SetWindowText(TEXT("����"));
	SetBankerActionType(true);
	//��ʱ����
	SetTimer(IDI_CHARMVALUE_UPDATE_VIEW,TIME_CHARMVALUE_UPDATE_VIEW,NULL);
	return TRUE;
}

//�滭����
VOID CDlgBank::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{	
	m_ImageFrame.DrawImage(pDC,30,55);
	DrawNumberString(pDC,m_lGameGold,200,78);
	DrawNumberString(pDC,m_lStorageGold,200,104);
	CRect rcScore(120,80,200,95);
	pDC->DrawText(TEXT("��ǰ��Ϸ�ң�"),rcScore,DT_END_ELLIPSIS | DT_CENTER | DT_WORDBREAK);
	CRect rcBank(120,105,200,120);
	pDC->DrawText(TEXT("������Ϸ�ң�"),rcBank,DT_END_ELLIPSIS | DT_CENTER | DT_WORDBREAK);
	CRect rect(0,315,nWidth,nHeight);
	pDC->DrawText(TEXT("��ܰ��ʾ��������Ϸ���������ѣ�ȡ�����۳�1%��������"),rect,DT_END_ELLIPSIS | DT_CENTER | DT_WORDBREAK);
}

//���½���
void CDlgBank::UpdateView()
{
	CString strinCount;
	GetDlgItemText(IDC_IN_COUNT,strinCount);
	LONGLONG OutNum = 0;
	MakeStringToNum(strinCount,OutNum);

	if(m_OrInCount!= OutNum)
	{
		m_OrInCount = OutNum;
		if(OutNum!=0)
		{
			MakeString(strinCount,OutNum);
			SetDlgItemText(IDC_IN_COUNT,strinCount);
			((CEdit*)GetDlgItem(IDC_IN_COUNT))->SetSel(strinCount.GetLength(),strinCount.GetLength(),TRUE); 
		}
		//������֤
		if ((OutNum < 0 || OutNum > m_lStorageGold ))
			return;	
	}
	UpdateData(TRUE);

	if(m_pMeUserData==NULL)return;

	//������Ϣ
	m_lGameGold=((m_pMeUserData->GetUserScore()<0)?0:m_pMeUserData->GetUserScore());
	m_lStorageGold=m_pMeUserData->GetUserInsure();

	//���½���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

	CString strlGameGold;

	MakeString(strlGameGold,m_lGameGold);
	SetDlgItemText(IDC_GAME_GOLD,strlGameGold);

	CString strlStorageGold;
	MakeString(strlStorageGold,m_lStorageGold);
	SetDlgItemText(IDC_STORAGE_GOLD,strlStorageGold);

	if(((CButton*)GetDlgItem(IDC_CHECK_ALL))->GetCheck()==1)
	{
		if(!m_bBankStorage)
			SetDlgItemText(IDC_IN_COUNT,strlStorageGold);
		else
			SetDlgItemText(IDC_IN_COUNT,strlGameGold);
	}

	UpdateData(FALSE);
}

//�����Ϣ
void CDlgBank::OnLButtonDown(UINT nFlags, CPoint point)
{
	CSkinDialog::OnLButtonDown(nFlags, point);
}

//�����Ϣ
void CDlgBank::OnMouseMove(UINT nFlags, CPoint point)
{
	CSkinDialog::OnMouseMove(nFlags, point);
}

//������Ϣ
BOOL CDlgBank::PreTranslateMessage(MSG* pMsg)
{
	//������ĸ
	if(GetFocus()==GetDlgItem(IDC_IN_COUNT)&& pMsg->message==WM_CHAR)
	{
		BYTE bMesValue = (BYTE)(pMsg->wParam);
		BYTE bTemp = bMesValue-'0';
		if((bTemp<0 || bTemp>9) && bMesValue!=VK_BACK) return TRUE;
	}
	return __super::PreTranslateMessage(pMsg);
}

//ʱ����Ϣ
void CDlgBank::OnTimer(UINT nIDEvent)
{
	//ʱ����Ϣ
	switch(nIDEvent)
	{
	case IDI_CHARMVALUE_UPDATE_VIEW:		//���½���
		{
			//���½���
			UpdateView();
			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//������Ϣ
void CDlgBank::OnEnChangeInCount()
{
	CString strInCount;
	CString strStorageGold;
	CString strGameGold;
	GetDlgItem(IDC_IN_COUNT)->GetWindowText(strInCount);
	MakeStringToNum(strInCount, m_lInCount);
	MakeString(strStorageGold, m_lStorageGold);
	MakeString(strGameGold, m_lGameGold);

	//������֤
	if (false==m_bBankStorage && (m_lInCount < 0 || m_lInCount > m_lStorageGold || strInCount.GetLength() > strStorageGold.GetLength()) )
	{
		m_lInCount = m_lStorageGold;
		MakeString(strInCount,m_lInCount);
		GetDlgItem(IDC_IN_COUNT)->SetWindowText(strInCount);
	}

	//������֤
	if (true==m_bBankStorage && (m_lInCount < 0 || m_lInCount > m_lGameGold || strInCount.GetLength() > strGameGold.GetLength()) )
	{
		m_lInCount = m_lGameGold;
		MakeString(strInCount,m_lInCount);
		GetDlgItem(IDC_IN_COUNT)->SetWindowText(strInCount);
	}

	return ;
}

//�Ƿ�ѡ��
bool CDlgBank::IsButtonSelected(UINT uButtonID)
{
	//return ((CButton *)GetDlgItem(uButtonID))->GetCheck()==BST_CHECKED;
	return true;
}

int CDlgBank::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
	CRect rect(0, 0, 0, 0);	
	m_btOK.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rect,this,IDC_OK);	
	m_btCancel.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rect,this,IDC_CANCEL);
	m_btFresh.Create(NULL,WS_CHILD|WS_VISIBLE|WS_DISABLED,rect,this,IDC_FRESH);
	m_btOK.SetWindowPos(NULL,110,240,94,32,SWP_NOSIZE);
	m_btCancel.SetWindowPos(NULL,285,240,94,32,SWP_NOSIZE);
	m_btFresh.SetWindowPos(NULL,390,66,64,64,SWP_NOSIZE);
	return 0;
}

//ѡ�а�ť
void CDlgBank::SetButtonSelected(UINT uButtonID, bool bSelected)
{
	CButton * pButton=(CButton *)GetDlgItem(uButtonID);
	if (bSelected) pButton->SetCheck(BST_CHECKED);
	else pButton->SetCheck(BST_UNCHECKED);
	return;
}

//������Ϣ
void CDlgBank::SetSendInfo(IClientKernel *pClientKernel,IClientUserItem const*pMeUserData)
{
	ASSERT(pClientKernel!=NULL);

	//������Ϣ
	m_pIClientKernel = pClientKernel;
	m_pMeUserData=const_cast<IClientUserItem *>(pMeUserData);

	return;
}
void CDlgBank::SetPostPoint(CPoint Point)
{
	CRect Rect;
	GetWindowRect(&Rect);
	SetWindowPos(NULL,Point.x,Point.y-Rect.bottom+Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top,SWP_NOZORDER);
}
//������Ϣ
void CDlgBank::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	ASSERT(m_pIClientKernel!=NULL);

	//������Ϣ
	m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);

	return ;
}

void CDlgBank::ShowItem()
{
	HINSTANCE hInstance = AfxGetInstanceHandle();	
	m_btFresh.SetButtonImage(IDB_BT_FRESH,hInstance,false,false);
	m_btFresh.EnableWindow(TRUE);
	if(m_bBankStorage)
	{
		GetDlgItem(IDC_IN_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_USER_PASSWORD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PASS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_USER_PASSWORD)->EnableWindow(false);
		m_btOK.SetButtonImage(IDB_SAVEGOLD, hInstance, false,false);
		m_btCancel.SetButtonImage(IDB_CLOSE,hInstance,false,false);
		m_btOK.EnableWindow(m_blCanStore);
		m_btCancel.EnableWindow(TRUE);
	}

	else
	{
		GetDlgItem(IDC_IN_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_USER_PASSWORD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PASS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_USER_PASSWORD)->EnableWindow(TRUE);
		m_btOK.SetButtonImage(IDB_GETGOLD, hInstance, false,false);
		m_btCancel.SetButtonImage(IDB_CLOSE,hInstance,false,false);
		m_btOK.EnableWindow(m_bCanDraw);
		m_btCancel.EnableWindow(TRUE);
	}

}
//��������
void CDlgBank::SetBankerActionType(bool bStorage) 
{
	//���ñ���
	m_bBankStorage=bStorage;
	//���ñ���
	if (m_bBankStorage)
	{
		//		m_bDrawAllGold=((CButton*)(GetDlgItem(IDC_CHECK_ALL)))->GetCheck()==1?true:false;
		//	((CButton*)(GetDlgItem(IDC_CHECK_ALL)))->SetCheck(m_bStorageAllGold?1:0);
		SetDlgItemText(IDC_IN_COUNT,TEXT(""));
		SetDlgItemText(IDC_USER_PASSWORD,TEXT(""));
		//GetDlgItem(IDOK)->SetWindowText(TEXT("���"));
		GetDlgItem(IDC_CHECK_ALL)->SetWindowText(TEXT("�������н��"));
		m_blUsingPassWord = false;
	}
	else
	{
		//	m_bStorageAllGold=((CButton*)(GetDlgItem(IDC_CHECK_ALL)))->GetCheck()==1?true:false;
		//	((CButton*)(GetDlgItem(IDC_CHECK_ALL)))->SetCheck(m_bDrawAllGold?1:0);
		SetDlgItemText(IDC_IN_COUNT,TEXT(""));
		SetDlgItemText(IDC_USER_PASSWORD,TEXT(""));
		//GetDlgItem(IDOK)->SetWindowText(TEXT("ȡ��"));
		GetDlgItem(IDC_CHECK_ALL)->SetWindowText(TEXT("ȡ�����н��"));
		m_blUsingPassWord = true;
	}
	ShowItem();
}

void CDlgBank::OnCancelEvent()
{
	DestroyWindow();
}

void CDlgBank::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	DestroyWindow();
}
void CDlgBank::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	DestroyWindow();	
}
