#pragma once

#include "OpenGL.h"
#include "GLTexture.h"
#include "GLLapseCount.h"
#include "GLVirtualWindow.h"
#include "OpenglEngineHead.h"

class OPENGL_ENGINE_CLASS CGLVirtualButton : public CGLVirtualWindow
{
	//״̬����
protected:
	bool							m_bMouseDown;						//���±�־
	bool							m_bMouseMove;						//������־
	bool							m_bButtonFocus;						//�����ʾ				
	CRect							m_rcButtonRect;						//��ť����

	//��Դ����
protected:
	LPCTSTR							m_pszTypeName;						//��Դ����
	LPCTSTR							m_pszResource;						//��Դ��Ϣ
	HINSTANCE						m_hResInstance;						//��Դ���

	//��������
protected:
	WORD							m_wImageIndex;						//��������
	CGLLapseCount					m_ImageLapseCount;					//���ż���

	//��Դ����
protected:
	CGLTexture						m_GLTextureButton;					//��ť����

public:
	CGLVirtualButton(void);
	~CGLVirtualButton(void);


	//ϵͳ�¼�
protected:
	//������Ϣ
	virtual VOID OnWindowMovie();
	//������Ϣ
	virtual VOID OnWindowCreate(CGLDevice * pGLDevice);
	//������Ϣ
	virtual VOID OnWindowDestory();

	//���غ���
protected:
	//����¼�
	virtual VOID OnEventMouse(UINT uMessage, UINT nFlags, INT nXMousePos, INT nYMousePos);
	//��ť�¼�
	virtual VOID OnEventButton(UINT uButtonID, UINT uMessage, INT nXMousePos, INT nYMousePos);
	//�滭����
	virtual VOID OnEventDrawWindow(CGLDevice * pGLDevice, INT nXOriginPos, INT nYOriginPos);

	//���ܺ���
public:
	//���ý���
	VOID SetButtonFocus(bool bButtonFocus) { m_bButtonFocus = bButtonFocus; }
	//��������
	VOID SetButtonRect(CRect rcButtonRect);
	//����λͼ
	VOID SetButtonImage(CGLDevice * pGLDevice, HINSTANCE hResInstance, LPCTSTR pszResource, LPCTSTR pszTypeName,  DWORD wImageType);

	//�ڲ�����
protected:
	//�����ؼ�
	VOID RectifyControl(CGLDevice * pGLDevice);
};
