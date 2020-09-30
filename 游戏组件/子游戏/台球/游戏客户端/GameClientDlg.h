#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

 
//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameEngine
{
	//��Ϸ����
public:
	WORD							m_wCurrentUser;						//��ǰ��� 
	BYTE                            m_bOutTimeTimes;                    //��ʱ����
	WORD                            m_serverType;                       //����������
	
	bool							m_bInited;
  
	//�ؼ�����
protected: 
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();

	//����̳�
private:
	//��ʼ����
	virtual bool OnInitGameEngine();
	//���ÿ��
	virtual bool OnResetGameEngine();
	//��Ϸ����
	virtual void OnGameOptionSet();
	//ʱ��ɾ��
	virtual bool OnEventGameClockKill(WORD wChairID);
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);
	//�Թ�״̬
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);
	//�����ӿ�
public:
	//��������
	virtual bool AllowBackGroundSound(bool bAllowSound){ return true; }
	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart( void * pBuffer, WORD wDataSize);
	// ��Ϸ����
	bool OnSubGameEnd( void *pBuffer,WORD wDataSize); 
	//����ĸ��
	bool OnSubSetBaseBall( void * pBuffer, WORD wDataSize);
	//���ý���
	bool OnSubEndSetBaseBall( void * pBuffer, WORD wDataSize);
	//�������
	bool OnSubBallClubPos( void * pBuffer, WORD wDataSize);
	//����
	bool OnSubHitBall( void * pBuffer, WORD wDataSize);
	//������
	bool OnSubHitResult( void *pBuffer,WORD wDataSize);
 
	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
