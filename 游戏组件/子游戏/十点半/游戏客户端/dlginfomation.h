#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////


//��Ϸ����
class CDlgInfomation : public CSkinDialog
{
	//�ؼ�����
public:
	CSkinButton						m_btOK;								//ȷ����ť
	CSkinButton						m_btCancel;							//ȡ����ť

	//��������
public:
	//���캯��
	CDlgInfomation();
	//��������
	virtual ~CDlgInfomation();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
