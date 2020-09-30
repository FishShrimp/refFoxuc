#pragma once
#include "SHButton.h"


#define PYOE_LEFT                     0x00000000
#define PYOE_CENTER                   0x00000001
#define PYOE_RIGHT                    0x00000002
#define PYOE_TOP                      0x00000000
#define PYOE_VCENTER                  0x00000004
#define PYOE_BOTTOM                   0x00000008

class CPlayOperate;

//�������
class CEditOperate : public CEdit
{

public:
	LONGLONG					m_lMaxScore;		//�����ע
	LONGLONG					m_lMinScore;		//��С��ע
	CPlayOperate*				m_pPlayOperate;		//��Ҳ�����

protected:
	CBrush						m_Brush;			//������ɫ

public:
	CEditOperate();
	~CEditOperate();

	// ������ɫ
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// ������ɫ
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	// �ж��ַ�
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	// �����Ҽ�
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	// ���̵��
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	DECLARE_MESSAGE_MAP()
};

// ��Ҳ���
class CPlayOperate
{
	// ��������
protected:
	bool							m_bShow;				// ��ʾ��ʾ
	bool							m_bMouseDown;			// �����
	LONG							m_lCellScore;			// ��Ϸ��ע
	int								m_lDragSite;			// �϶�λ��

	// ���Զ���
protected:
	CPoint							m_ptPos;				// λ����Ϣ
	CSize							m_sizeBakc;				// ��С��Ϣ
	CRect							m_rectDrag;				// �϶�����
	CSize							m_sizeDrag;				// �϶�ͼƬ��С

	// ��Դ����
public:
	CPngImage						m_ImageBack;			// ����ͼƬ
	CPngImage						m_ImageDrag;			// �϶�ͼƬ
	CSkinButton						m_btOneScore;			// 1����ť
	CSkinButton						m_btTwoScore;			// 2����ť
	CSkinButton						m_btThreeScore;			// 3����ť	
	CSkinButton						m_btFourScore;			// 4����ť
	CSkinButton						m_btAddScore;			// ��ע��ť
	CSkinButton						m_btFollow;				// ��ע��ť
	CSkinButton						m_btGiveUp;				// ������ť
	CSkinButton						m_btShowHand;			// �����ť
	CSHButton						m_btADD;				// �����ť
	CSHButton						m_btSUB;				// �����ť

	CFont							m_fontScoreInput;		// ��������
	CEditOperate					m_edieScoreInput;		// ��������

public:
	CPlayOperate(void);
	~CPlayOperate(void);

	// ���Ժ���
public:
	// ��ʼ��
	void Initialization(CWnd* pParent);
	// ��ť��Ϣ
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	// �滭����
public:
	// �滭����
	void DrawPlayOperate(CDC * pDC, INT nWidth, INT nHeight);

	// ���ú���
public:
	// ����λ��
	void SetOperatePos( int nPosX, int nPosY, UINT nFlags);
	// ������Ϣ
	void SetCellScore( LONG lCellScore);
	// ������ע��Χ��
	void SetBetCount( LONGLONG lMaxBetCount, LONGLONG lMinBetCount );

	// ��ȡ����
public:
	// ��ȡ��������
	LONGLONG GetBetCount();
	// ��ȡλ��
	CRect GetOperateRect();
	// ��ȡ��ʾ״̬
	bool	 IsShow() { return m_bShow; }
	// ��ȡ��ע״̬
	bool	 IsAddShow() { return m_btAddScore.IsWindowEnabled()&&m_btAddScore.IsWindowVisible(); }


	// ���ܺ���
public:
	// ��ʾ����
	void ShowOperate( bool bShow );
	// ��Ϣ����
	BOOL PreTranslateMessage(MSG * pMsg);
	// ���û���
	void SetEditScore();
	// ���û���
	void SetDragScore();
	// ���û���
	void ScoreADD();
	// ���û���
	void ScoreSUB();
	// ��Ӷ���
	CString AddComma( LONGLONG lScore );
	// ��Ӷ���
	CString AddComma( CString strScore );
	// ɾ������
	LONGLONG DeleteComma( CString strScore );
};
