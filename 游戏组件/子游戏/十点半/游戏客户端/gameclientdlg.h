#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientEngine : public CGameFrameEngine
{
	//��Ϸ����
protected:
	LONGLONG						m_lCellScore;						//��Ԫ����
	LONGLONG						m_lTableScore[GAME_PLAYER];			//�����
	bool							m_bAddScore;						//�Ѽӱ�
	bool							m_bGameStart;						//��Ϸ��ʼ

	//��ұ���
	WORD							m_wCurrentUser;						//��ǰ���
	WORD							m_wBankerUser;						//ׯ��
	BYTE							m_byUserStatus[GAME_PLAYER];		//���״̬

	//��������
	TCHAR							m_szAccounts[GAME_PLAYER][32];		//�������

	BYTE							m_bySendCardCount;							//������Ŀ
	WORD							m_wStartChairId;							//������ʼλ��
	BYTE							m_bySendCardData[GAME_PLAYER][MAX_COUNT];	//��������

	CMD_S_GameStart					m_GameStart;						//��Ϸ��ʼ��Ϣ
	CMD_S_GameEnd					m_GameEnd;							//��Ϸ������Ϣ

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	
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
	//��Ϸ����
	virtual void OnGameOptionSet();

	//ʱ���¼�
public:
	//ʱ��ɾ��
	virtual bool OnEventGameClockKill(WORD wChairID);
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);

	//��Ϸ�¼�
public:
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pData, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard( const void *pData, WORD wDataSize );
	//��Ҽӱ�
	bool OnSubUserAddScore( const void *pData, WORD wDataSize );
	//���ͣ��
	bool OnSubUserStopCard( const void *pData, WORD wDataSize );
	//�������
	bool OnSubUserLeft( const void *pData, WORD wDataSize );
	//��Ϸ��ʼ
	bool OnSubGamePlay( const void *pData, WORD wDataSize );

	//��Ϣӳ��
protected:
	//��ʼ��Ϣ
	LRESULT OnStart( WPARAM wParam, LPARAM lParam );
	//��ע��Ϣ
	LRESULT OnAddScore( WPARAM wParam, LPARAM lParam );
	//��ע��Ϣ
	LRESULT OnGiveCard( WPARAM wParam, LPARAM lParam );
	//������Ϣ
	LRESULT OnStopCard( WPARAM wParam, LPARAM lParam );
	//��ע��Ϣ
	LRESULT OnUserScore( WPARAM wParam, LPARAM lParam );

	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	//��������
protected:
	//���ؿ���
	VOID HideScoreControl();
	//���¿���
	VOID UpdateScoreControl();
	//�������
	LRESULT OnSendCardFinish(WPARAM wParam, LPARAM lParam);
	//�����ƶ����
	LRESULT OnMoveJetFinish(WPARAM wParam, LPARAM lParam);
	//���Ų�������
	VOID PlayActionSound( WORD wChairId, BYTE byAction );
	//ִ�н���
	VOID PerformGameConclude();
	//ִ�п�ʼ
	VOID PerformGameStart();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
