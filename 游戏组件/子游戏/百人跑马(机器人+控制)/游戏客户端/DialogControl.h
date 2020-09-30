#pragma once


// CDialogControl �Ի���

class CDialogControl : public CDialog
{
	DECLARE_DYNAMIC(CDialogControl)

public:
		LONGLONG  m_lAllUserBet[AREA_ALL];				//�û���ע
	CDialogControl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogControl();

	// �Ի�������
	enum { IDD = IDD_DIALOG_SYSTEM };

	//�ؼ�
public:
	CEdit					m_editInput[AREA_ALL];		//�����
	CButton					m_radioArea[AREA_ALL];		//��������

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	//���¿���
	void UpdateControl(CMD_S_ControlReturns* pControlReturns);

protected:
	//��Ϣ
	void PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, int nMultiple[AREA_ALL], BYTE cbTimes);
	//��ȡ����
	CString	ObtainArea( BYTE cbArea );


	DECLARE_MESSAGE_MAP()
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
	//������Ч��ť
	afx_msg void OnBnClickedRadioNwe();
	//�¾���Ч��ť
	afx_msg void OnBnClickedRadioNext();
	//������ע
	void ResetUserBet();
	//�����ע
	void SetUserBetScore(LONGLONG lBetScore[AREA_ALL]);
};
