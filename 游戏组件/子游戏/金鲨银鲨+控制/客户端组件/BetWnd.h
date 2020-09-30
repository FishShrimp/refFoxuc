#pragma once

class CBetWnd : public CWnd
{

	//������Ϣ
public:
	LONGLONG						m_lPlayAllBet[ANIMAL_MAX];			//���������ע
	CPngImage						m_ImageBack;						//����
	CPngImage						m_ImageNumber;						//����

	//���캯��
public:
	CBetWnd(void);
	~CBetWnd(void);

	//���ú���
public:
	//�������������ע
	void SetPlayAllBet( LONGLONG lPlayAllBet[ANIMAL_MAX] );
	//�������������ע
	void SetPlayAllBet( LONGLONG lPlayAllBet, int nAnimalIndex );

	//ϵͳ����
public:
	//�滭
	afx_msg void OnPaint();
	//�滭
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

	// ���溯��
private:
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* pImageNumber, TCHAR* szImageNum, TCHAR* szOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);	

};
