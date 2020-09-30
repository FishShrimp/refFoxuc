#pragma once


// CAdminControlDlg �Ի���
#define IDM_ADMIN_COMMDN WM_USER+1000
class CAdminControlDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdminControlDlg)

public:
	CAdminControlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdminControlDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_SYSTEM };

public:
	//���¿���
	void UpdateControl(CMD_S_ControlReturns* pControlReturns);

protected:
	//��Ϣ
	void PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea[AREA_ALL], BYTE cbTimes);
	//�жϿ�����
	bool JudgeFeasibility(BYTE cbArea[AREA_ALL]);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//��ʼ��
	virtual BOOL OnInitDialog();
	//�ؼ���ɫ
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//ȡ������
	afx_msg void OnBnClickedButtonReset();
	//���ֿ���
	afx_msg void OnBnClickedButtonSyn();
	//��������
	afx_msg void OnBnClickedButtonOk();
	//ȡ���ر�
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedCheckLong();
	afx_msg void OnBnClickedCheckPing();
	afx_msg void OnBnClickedCheckHu();
	afx_msg void OnBnClickedCheck213();
	afx_msg void OnBnClickedCheck14();
	afx_msg void OnBnClickedCheck1526();
	afx_msg void OnBnClickedCheck26();
	afx_msg void OnBnClickedCheck711();
	afx_msg void OnBnClickedCheck1216();
	afx_msg void OnBnClickedCheck1721();
	afx_msg void OnBnClickedCheck2226();
};
