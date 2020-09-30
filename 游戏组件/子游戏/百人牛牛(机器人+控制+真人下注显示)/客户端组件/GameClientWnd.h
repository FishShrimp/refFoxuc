#ifndef GAME_FRAME_WND_HEAD_FILE
#define GAME_FRAME_WND_HEAD_FILE

#pragma once

#include "GameFrameHead.h"
#include "GameFrameControl.h"

//////////////////////////////////////////////////////////////////////////////////

//�����Դ
class CFrameEncircle : public CSkinEncircle
{
	//��������
public:
	//���캯��
	CFrameEncircle();
	//��������
	virtual ~CFrameEncircle();

	//���غ���
protected:
	//�滭����
	virtual bool PreDrawEncircleImage(tagEncircleImage & EncircleImage);
};

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class GAME_FRAME_CLASS CGameClientWnd : public CFrameWnd, public IGameFrameWnd
{
	//״̬����
protected:
	bool							m_bMaxShow;							//����־
	bool							m_bShowControl;						//��ʾ��־
	CRect							m_rcNormalSize;						//����λ��

	//����ӿ�
protected:
	IClientKernel *					m_pIClientKernel;					//�ں˽ӿ�
	IGameFrameView *				m_pIGameFrameView;					//��ͼ�ӿ�
	IGameFrameService *				m_pIGameFrameService;				//��ܷ���

	//�ؼ�����
public:
	CSkinButton						m_btMin;							//��С��ť
	CSkinButton						m_btMax;							//���ť
	CSkinButton						m_btClose;							//�رհ�ť

	//���ܿؼ�
public:
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CSkinSplitter					m_SkinSplitter;						//��ֿؼ�
	CGameFrameControl				m_GameFrameControl;					//���ƿ��

	//��Դ����
protected:
	CFrameEncircle					m_FrameEncircle;					//�����Դ

	//��������
public:
	//���캯��
	CGameClientWnd();
	//��������
	virtual ~CGameClientWnd();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���溯��
protected:
	//��ԭ����
	bool RestoreWindow();
	//��󴰿�
	bool MaxSizeWindow();

	//�������
protected:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);

	//��Ϣ����
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//��������
	UINT OnNcHitTest(CPoint Point);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//λ����Ϣ
	VOID OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);
	//�ı���Ϣ
	VOID OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	//�Զ���Ϣ
protected:
	//������Ϣ
	LRESULT	OnSetTextMesage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif