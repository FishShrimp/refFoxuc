#pragma once
#include "../�ͻ������/ClientControl.h"


// CClientControlItemSinkDlg �Ի���

class AFX_EXT_CLASS CClientControlItemSinkDlg : public IClientControlDlg
{
	DECLARE_DYNAMIC(CClientControlItemSinkDlg)

protected:
	BYTE m_cbWinArea[2];				//Ӯ������
	BYTE m_cbExcuteTimes;				//ִ�д���

public:
	CClientControlItemSinkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClientControlItemSinkDlg();

// �Ի�������
	enum { IDD = IDD_CLIENT_CONTROL_EX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	//���¿���
	virtual void __cdecl OnAllowControl(bool bEnable);
	//������
	virtual bool __cdecl ReqResult(const void * pBuffer);

	virtual void __cdecl SetText(SCORE m_Score,int x,int y,bool bXor);

	virtual BOOL  OnInitDialog();
	afx_msg void  OnReSet();
	afx_msg void  OnRefresh();
	afx_msg void  OnExcute();
	afx_msg void  OnSexClicked(UINT nCmdID);

protected:
	virtual void OnCancel();

public:
//	afx_msg void OnTimer(UINT nIDEvent);
	
};
