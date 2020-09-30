#ifndef GAME_CLIENT_ENGINE_HEAD_FILE
#define GAME_CLIENT_ENGINE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//����˵��
//typedef CWHArray<CMD_Buffer,CMD_Buffer &> CSocketPacketArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientEngine : public CGameFrameEngine
{
	//��ע��Ϣ
protected:
	LONGLONG						m_lCellScore;							//��Ԫ��ע
	LONGLONG						m_lTurnMaxScore;						//�����ע
	LONGLONG						m_lTurnLessScore;						//��С��ע
	LONGLONG						m_lTableScore[GAME_PLAYER*2];			//��ע��Ŀ
	LONGLONG						m_lShowHandScore;						//������߷�
	LONGLONG						m_lUserScore[GAME_PLAYER];				//

	//״̬����
protected:
	bool							m_bShowHand;								//�����־
	WORD							m_wCurrentUser;								//��ǰ�û�
	BYTE							m_cbPlayStatus[GAME_PLAYER];				//��Ϸ״̬
	TCHAR							m_szAccounts[GAME_PLAYER][LEN_ACCOUNTS];	//�������

	//�ؼ�����
public:
	CGameClientView					m_GameClientView;						//��Ϸ��ͼ
	
	//��������
public:
	//���캯��
	CGameClientEngine();
	//��������
	virtual ~CGameClientEngine();

	//����̳�
private:
	//��ʼ����
	virtual bool OnInitGameEngine();
	//���ÿ��
	virtual bool OnResetGameEngine();
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID);
	//ɾ����ʱ��
	virtual bool OnEventGameClockKill(WORD wChairID);
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubAddScore(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

public:
	//��������
	virtual bool AllowBackGroundSound(bool bAllowSound){return true;}

	//��������
protected:
	//���ؿ���
	void HideScoreControl();
	//���¿���
	void UpdateScoreControl();

	//��Ϣӳ��
protected:
	//��ʼ��ť
	LRESULT	OnStart(WPARAM wParam, LPARAM lParam);
	//������ť
	LRESULT OnGiveUp(WPARAM wParam, LPARAM lParam);
	//��ע��ť
	LRESULT OnFollow(WPARAM wParam, LPARAM lParam);
	//��ע��ť 
	LRESULT	OnAddScore(WPARAM wParam, LPARAM lParam);
	//�����ť
	LRESULT OnShowHand(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnSendCardFinish(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

#endif
//////////////////////////////////////////////////////////////////////////
