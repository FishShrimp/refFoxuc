#include "Stdafx.h"
#include "DlgInfomation.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgInfomation, CSkinDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgInfomation::CDlgInfomation() : CSkinDialog(IDD_INFOMATION)
{
}

//��������
CDlgInfomation::~CDlgInfomation()
{
}

//�ؼ���
void CDlgInfomation::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
}

//��ʼ������
BOOL CDlgInfomation::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("��Ϸ֪ͨ"));


	return TRUE;
}

//ȷ����Ϣ
void CDlgInfomation::OnOK()
{
	__super::OnOK();
}

//////////////////////////////////////////////////////////////////////////
