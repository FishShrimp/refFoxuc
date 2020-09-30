#pragma once


//��Ϣ����

class CDialogPlayBet : public CDialog
{
	DECLARE_DYNAMIC(CDialogPlayBet)

public:
	CDialogPlayBet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogPlayBet();

	// �Ի�������
	enum { IDD = IDD_DIALOG_BET };

	//��Ϸ����
public:
	INT						m_nMultiple[AREA_ALL];	//������
	LONGLONG				m_lLastRound[AREA_ALL];	//��һ�ֻ���
	LONGLONG				m_lPlayerScore;			//��һ���
	LONGLONG				m_lTheNote;				//��ǰע��

	//ͼƬ
public:
	CPngImage				m_ImageBackdrop;		//����

	//�ؼ�
public:
	CEdit					m_editInput[AREA_ALL];		//�����
	CSkinButton				m_btAdd[AREA_ALL];			//���Ӱ�ť
	CSkinButton				m_btReduce[AREA_ALL];		//���ٰ�ť
	CSkinButton				m_btBet1000;				//��ע1000
	CSkinButton				m_btBet1W;					//��ע1W
	CSkinButton				m_btBet10W;					//��ע10W
	CSkinButton				m_btBet100W;				//��ע100W
	CRect					m_rcBet1000;				//1000λ��
	CRect					m_rcBet1W;					//1Wλ��
	CRect					m_rcBet10W;					//10Wλ��
	CRect					m_rcBet100W;				//100Wλ��
	CSkinButton				m_btDetermine;				//ȷ��
	CSkinButton				m_btReset;					//����
	CSkinButton				m_btRepeat;					//�ظ�
	CSkinButton				m_btClosee;					//�ر�

	//��Դ
public:
	CFont					m_InfoFont;
	CFont					m_MultipleFont;
	CBrush					m_InfoBrush;

	//�ڲ�����
private:
	//�����
	void					EditAdd(BYTE cbArea);	
	//�����
	void					EditReduce(BYTE cbArea);	
	//��������
	void					EditLimit(BYTE cbArea);

	//���ƺ���
public:
	//���û���
	void SetScore( LONGLONG lScore );
	//�����Ƿ�����ע
	void SetCanBet( BOOL bCanBet);
	//���ñ���
	void SetMultiple( INT nMultiple[AREA_ALL]);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButton1000();
	afx_msg void OnBnClickedButton1w();
	afx_msg void OnBnClickedButton10w();
	afx_msg void OnBnClickedButton100w();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonReduce16();
	afx_msg void OnBnClickedButtonAdd16();
	afx_msg void OnBnClickedButtonReduce15();
	afx_msg void OnBnClickedButtonAdd15();
	afx_msg void OnBnClickedButtonReduce14();
	afx_msg void OnBnClickedButtonAdd14();
	afx_msg void OnBnClickedButtonReduce13();
	afx_msg void OnBnClickedButtonAdd13();
	afx_msg void OnBnClickedButtonReduce12();
	afx_msg void OnBnClickedButtonAdd12();
	afx_msg void OnBnClickedButtonReduce26();
	afx_msg void OnBnClickedButtonAdd26();
	afx_msg void OnBnClickedButtonReduce25();
	afx_msg void OnBnClickedButtonAdd25();
	afx_msg void OnBnClickedButtonReduce24();
	afx_msg void OnBnClickedButtonAdd24();
	afx_msg void OnBnClickedButtonReduce23();
	afx_msg void OnBnClickedButtonAdd23();
	afx_msg void OnBnClickedButtonReduce36();
	afx_msg void OnBnClickedButtonAdd36();
	afx_msg void OnBnClickedButtonReduce35();
	afx_msg void OnBnClickedButtonAdd35();
	afx_msg void OnBnClickedButtonReduce34();
	afx_msg void OnBnClickedButtonAdd34();
	afx_msg void OnBnClickedButtonReduce46();
	afx_msg void OnBnClickedButtonAdd46();
	afx_msg void OnBnClickedButtonReduce45();
	afx_msg void OnBnClickedButtonAdd45();
	afx_msg void OnBnClickedButtonReduce56();
	afx_msg void OnBnClickedButtonAdd56();
	afx_msg void OnBnClickedButtonDetermine();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonRepeat();
	afx_msg void OnBnClickedButtonCloseBet();
	afx_msg void OnEnChangeEdit16();
	afx_msg void OnEnChangeEdit15();
	afx_msg void OnEnChangeEdit14();
	afx_msg void OnEnChangeEdit13();
	afx_msg void OnEnChangeEdit12();
	afx_msg void OnEnChangeEdit26();
	afx_msg void OnEnChangeEdit25();
	afx_msg void OnEnChangeEdit24();
	afx_msg void OnEnChangeEdit23();
	afx_msg void OnEnChangeEdit36();
	afx_msg void OnEnChangeEdit35();
	afx_msg void OnEnChangeEdit34();
	afx_msg void OnEnChangeEdit46();
	afx_msg void OnEnChangeEdit45();
	afx_msg void OnEnChangeEdit56();
};
