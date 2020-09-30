#pragma once


// CDialogStatistics �Ի���

class CDialogStatistics : public CDialog
{
	DECLARE_DYNAMIC(CDialogStatistics)

public:
	CDialogStatistics(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogStatistics();

// �Ի�������
	enum { IDD = IDD_DIALOG_TONGJI };

	//��Ϸ����
protected:
	INT					m_nWinCount[HORSES_ALL];			//ȫ��Ӯ�ĳ���

	//��Դ����
protected:
	CFont				m_InfoFont;							//����
	CBitImage			m_ImageLine;						//��
	CBitImage			m_ImageBackdrop;					//����
	CSkinButton			m_btClose;							//�ر�
	CSkinButton			m_btDetermine;						//ȷ��


	//���ƺ���
public:
	//���û���
	void SetWinCount( INT nWinCount[HORSES_ALL] );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonTjClose();
	afx_msg void OnBnClickedButtonTjClose2();
};
