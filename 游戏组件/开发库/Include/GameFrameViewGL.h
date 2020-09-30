#ifndef GAME_FRAME_VIEW_GL_HEAD_FILE
#define GAME_FRAME_VIEW_GL_HEAD_FILE

#pragma once

#include "GameFrameHead.h"
#include "GameFrameView.h"

//////////////////////////////////////////////////////////////////////////////////

//��ͼ���
class GAME_FRAME_CLASS CGameFrameViewGL : public CGameFrameView
{
	//״̬����
protected:
	bool							m_bInitGL;							//������־
	HANDLE 							m_hRenderThread;					//��Ⱦ�߳�

	//��������
protected:
	CRect							m_RectDirtySurface;					//�ػ�����

	//֡��ͳ��
protected:
	DWORD							m_dwDrawBenchmark;					//��׼���
	DWORD							m_dwDrawLastCount;					//�滭����
	DWORD							m_dwDrawCurrentCount;				//�滭����

	//�������
public:
	CGLFont							m_GLFont;
	CGLDevice						m_GLDevice;

	//��������
public:
	//���캯��
	CGameFrameViewGL();
	//��������
	virtual ~CGameFrameViewGL();

	//���غ���
public:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�������
	virtual VOID InvalidGameView(INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//���溯��
private:
	//��������
	virtual VOID CartoonMovie()=NULL;
	//���ý���
	virtual VOID InitGameView(CGLDevice* pGLDevice, INT nWidth, INT nHeight)=NULL;
	//�滭����
	virtual VOID DrawGameView(CGLDevice* pGLDevice, INT nWidth, INT nHeight)=NULL;

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//Ĭ�ϻص�
	virtual LRESULT DefWindowProc(UINT nMessage, WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//��Ⱦ�߳�
	VOID StartRenderThread();

	//�������
public:
	//�������
	VOID DrawTextString(CGLDevice* pGLDevice, LPCTSTR pszString, CRect rcDraw,UINT nFormat, COLORREF GLColor);
	//�������
	VOID DrawTextString(CGLDevice* pGLDevice, LPCTSTR pszString, INT nXPos, INT nYPos, COLORREF GLColor);
	//�滭�ַ�
	VOID DrawTextString(CGLDevice* pGLDevice, LPCTSTR pszString, CRect rcDraw, UINT nFormat, COLORREF crText, COLORREF crFrame);
	//�滭�ַ�
	VOID DrawTextString(CGLDevice* pGLDevice, LPCTSTR pszString, INT nXPos, INT nYPos,  COLORREF crText, COLORREF crFrame);

	//��Ϣӳ��
protected:
	//�滭����
	VOID OnPaint();
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);

	//�Զ���Ϣ
protected:
	//������Ϣ
	LRESULT OnMessageGLStatus(WPARAM wParam, LPARAM lParam);
	//��Ⱦ��Ϣ
	LRESULT OnMessageGLRender(WPARAM wParam, LPARAM lParam);

	//��̬����
private:
	//��Ⱦ�߳�
	static VOID GLRenderThread(LPVOID pThreadData);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
};

//////////////////////////////////////////////////////////////////////////////////

#endif