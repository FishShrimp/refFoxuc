#pragma once
#include "Resource.h"
#include "../��Ϸ�ͻ���/ClientControl.h"

// CAdminControlDlg �Ի���


class AFX_EXT_CLASS CAdminControlDlg :public IClientControlDlg
{
	DECLARE_DYNAMIC(CAdminControlDlg)

public:
	CAdminControlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdminControlDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_SYSTEM };
protected:		
	LONGLONG  m_lAllUserBet[AREA_COUNT];				//�û���ע
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	//������ע
	virtual void __cdecl ResetUserBet();
	//�����ע
	virtual void __cdecl SetUserBetScore(BYTE cbArea,LONGLONG lScore);
	//�����ע
	virtual void __cdecl SetUserGameScore(BYTE cbArea,LONGLONG lScore);
	//���¿���
	virtual void __cdecl UpdateControl(CMD_S_ControlReturns* pControlReturns);
	//���1
	virtual void __cdecl SetUserNickName(CString strNickName);
	virtual void __cdecl ResetUserNickName();

protected:
	//��Ϣ
	void PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes);

public:
	//��ʼ��
	virtual BOOL OnInitDialog();
	//������ɫ
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//ȡ������
	afx_msg void OnBnClickedButtonReset();
	//���ֿ���
	afx_msg void OnBnClickedButtonSyn();
	//��������
	afx_msg void OnBnClickedButtonOk();
	//ȡ���ر�
	afx_msg void OnBnClickedButtonCancel();

	afx_msg void OnBnClickedBtCheak();
	afx_msg void OnBnClickedBtGet();
};
