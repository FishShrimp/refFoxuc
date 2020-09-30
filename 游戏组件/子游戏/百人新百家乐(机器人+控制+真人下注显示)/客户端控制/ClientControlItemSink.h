#pragma once
#include "../��Ϸ�ͻ���/ClientControl.h"


// CClientControlItemSinkDlg �Ի���

class AFX_EXT_CLASS CClientControlItemSinkDlg : public IClientControlDlg
{
	DECLARE_DYNAMIC(CClientControlItemSinkDlg)

protected:
	BYTE m_cbWinArea;				//Ӯ������
	BYTE m_cbExcuteTimes;			//ִ�д���
	LONGLONG m_lStorage;
	LONGLONG m_lDeduct;

public:
	CClientControlItemSinkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClientControlItemSinkDlg();

// �Ի�������
	enum { IDD = IDD_CLIENT_CONTROL_EX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	//������
	virtual bool __cdecl ReqResult(const void * pBuffer);
	//���¿��
	virtual bool __cdecl UpdateStorage(const void * pBuffer);
	//���¿ؼ�
	virtual void __cdecl UpdateControl();

	virtual BOOL OnInitDialog();
	afx_msg void  OnReSet();
	afx_msg void  OnRefresh();
	afx_msg void  OnExcute();

protected:
	virtual void OnCancel();

public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedBtnUpdateStorage();
	void RequestUpdateStorage();
};
