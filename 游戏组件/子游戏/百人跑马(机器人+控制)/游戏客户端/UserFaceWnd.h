#pragma once

class CUserFaceWnd : public CWnd
{
public:
	CUserFaceWnd(void);
	~CUserFaceWnd(void);

protected:
	//�滭ָ��
	CGameFrameViewD3D*		m_pFrameView;
	//ͷ��ָ��
	const IClientUserItem*	m_pUserData;

public:
	//���û滭ָ��
	void SetFrameView(CGameFrameViewD3D* pFrameView);
	//����ͷ��ָ��
	void SetUserData(const IClientUserItem* pUserData);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
