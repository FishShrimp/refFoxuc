#pragma once


// CDialogRecord �Ի���

class CDialogRecord : public CDialog
{
	DECLARE_DYNAMIC(CDialogRecord)

public:
	CDialogRecord(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogRecord();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOG };

	//��Ϸ��¼
public:
	CWHArray<tagHistoryRecord> m_GameRecords;			//��Ϸ��¼

	//��Ϸ����
protected:
	LONGLONG			m_lPlayerScore;						//��һ���
	LONGLONG			m_lAllBet;							//����Ͷע
	LONGLONG			m_lBetMumber;						//Ͷע����

	//��Դ����
protected:
	CSize				m_szTotalSize;						//�ܴ�С
	CFont				m_InfoFont;							//����
	CBitImage			m_ImageBackdropHand;				//����ͷ
	CBitImage			m_ImageBackdropTail;				//����β
	CBitImage			m_ImageBackdrop;					//������
	CSkinButton			m_btOk;								//OK


	//�ڲ�����
private:
	//�������
	CString IdentifyAreas(BYTE cbArea);

	//���ƺ���
public:
	//���û���
	void SetScore( LONGLONG lScore, LONGLONG lAllBet, LONGLONG lBetMumber );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonLogok();
};
#pragma once


