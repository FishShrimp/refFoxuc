#pragma once
#include "../��Ϸ�ͻ���/ClientControl.h"


// CClientControlItemSinkDlg �Ի���

class AFX_EXT_CLASS CClientControlItemSinkDlg : public IClientControlDlg
{
	DECLARE_DYNAMIC(CClientControlItemSinkDlg)

protected:
	bool m_bWinArea[CONTROL_AREA];
	BYTE m_cbControlStyle;
	BYTE m_cbExcuteTimes;

public:
	CClientControlItemSinkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClientControlItemSinkDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADMIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	//���¿���
	virtual void __cdecl OnAllowControl(bool bEnable);
	//������
	virtual bool __cdecl ReqResult(const void * pBuffer);

public:
	//���ý���
	void ReSetAdminWnd();

public:
	afx_msg void  OnReSet();
	afx_msg void  OnRefresh();
	afx_msg void  OnExcute();
	afx_msg void  OnRadioClick();
	afx_msg void OnBnClickedBtnUpdateStorage();
	void RequestUpdateStorage();

	bool __cdecl UpdateStorage(const void * pBuffer);
	void __cdecl UpdateControl();
protected:
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
};
