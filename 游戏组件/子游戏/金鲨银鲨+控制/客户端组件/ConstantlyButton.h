#pragma once

class CConstantlyButton : public CVirtualButton
{

public:
	BOOL								m_bSend;				//������Ϣ
	CLapseCount							m_WaitingTime;			//�ȴ���ʱ
	CLapseCount							m_SendTime;				//���ͼ�ʱ

public:
	CConstantlyButton(void);
	~CConstantlyButton(void);

	//ϵͳ�¼�
protected:
	//������Ϣ
	virtual VOID OnWindowMovie();

	//���غ���
protected:
	//����¼�
	virtual VOID OnEventMouse(UINT uMessage, UINT nFlags, INT nXMousePos, INT nYMousePos);

	//��ȡ��Դ
public:
	CD3DTexture* GetD3DTexture() { return &m_D3DTextureButton; }
};
