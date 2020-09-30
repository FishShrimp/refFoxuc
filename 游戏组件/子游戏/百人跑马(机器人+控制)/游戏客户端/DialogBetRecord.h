#pragma once

//��Ϣ�ṹ
struct BetRecordInfo
{
	INT			nStreak;			//����
	INT			nRanking;			//����
	LONGLONG	lBet;				//��ע
	LONGLONG	lWin;				//��Ӯ
	INT			nHours;				//Сʱ
	INT			nMinutes;			//����
};

//�б������
class CColorHeaderCtrl : public CHeaderCtrl
{
public:
	CColorHeaderCtrl(){}
	~CColorHeaderCtrl(){}

public:
	//�������ػ�
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()
};

//�б�
class CMyListCtrl : public CListCtrl
{
public:
	CMyListCtrl(){}
	~CMyListCtrl(){}


	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

// CDialogBetRecord �Ի���
class CDialogBetRecord : public CDialog
{
	DECLARE_DYNAMIC(CDialogBetRecord)

public:
	CDialogBetRecord(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogBetRecord();

// �Ի�������
	enum { IDD = IDD_DIALOG_BET_LOG };

	//����
public:
	//�б�
	CMyListCtrl m_listBetRecord;
	//������
	CColorHeaderCtrl m_ColorHeader;

	//��Դ
public:
	CFont					m_InfoFont;
	CBitImage				m_ImageBackdrop;		//����
	CSkinButton				m_btClosee;				//�ر�

	//�ڲ�����
private:
	//�������
	CString IdentifyAreas(BYTE cbArea);

	//���ƺ���
public:
	//�����Ϣ
	void		AddInfo(BetRecordInfo* pInfo);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonBrOk();
};
