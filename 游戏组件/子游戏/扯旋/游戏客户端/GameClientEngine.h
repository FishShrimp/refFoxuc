#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//����˵��
//typedef CWHArray<CMD_Buffer,CMD_Buffer &> CSocketPacketArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientEngine: public CGameFrameEngine
{
	//��Ϸ����
protected:
	WORD							m_wViewChairID[GAME_PLAYER];			//��ͼλ��
	WORD							m_wBankerUser;							//ׯ���û�
	WORD							m_wCurrentUser;							//��ǰ�û�
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�û�����
	TCHAR							m_szAccounts[GAME_PLAYER][LEN_NICKNAME];	//�������
	LONGLONG						m_lUserScore[GAME_PLAYER];				//�û����
	LONGLONG						m_lCellScore;							//��Ԫ����
	LONGLONG						m_lUserGetScore[GAME_PLAYER];			//�û��÷�
	LONGLONG						m_lUserTax[GAME_PLAYER];				//�û�˰��

	//��ע��Ϣ
protected:
	LONGLONG						m_lTurnMaxScore;						//�����ע
	LONGLONG						m_lTurnMinScore;						//��С��ע
	LONGLONG						m_lTableScore[GAME_PLAYER];				//��ע��Ŀ
	LONGLONG						m_lTotalScore[GAME_PLAYER];				//��ע��Ŀ

	//�������
protected:
	//CSocketPacketArray				m_SocketPacketArray;					//���ݻ���

	//�ؼ�����
public:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
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
	//��Ϸ����
	virtual void OnGameOptionSet();

	//ʱ���¼�
public:
	//ʱ��ɾ��
	virtual bool OnEventGameClockKill(WORD wChairID);
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID);

	//�Թ�״̬
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize);
	//��Ϸ����
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pBuffer, WORD wDataSize);
	//��������
	virtual bool AllowBackGroundSound(bool bAllowSound){return true;}

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(VOID * pBuffer, WORD wDataSize);
	//�û��±�
	bool OnSubUserInvest(VOID * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard(VOID * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubUserGiveUp(VOID * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubAddScore(VOID * pBuffer, WORD wDataSize);
	//��ʼ����
	bool OnSubStartOpen(VOID * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOpenCard(VOID * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(VOID * pBuffer, WORD wDataSize);

	//��������
public:
	//��ȡ����
	void GetHeapTailType(WORD wChairID,TCHAR szHeapString[],TCHAR szTailString[]);
	//��ȡ����
	void GetCardType(BYTE cbCardData[],BYTE cbCardCount,TCHAR szResult[]);
	//��ȡ����
	void GetViewUserName(WORD wViewChairID,TCHAR szNametring[]);
	//�Ӵ�λ��
	WORD GetViewChairID(WORD wChairID);
	//���¿���
	void UpdateScoreControl();
	//���¿���
	void UpdateInvestControl();

	//��Ϣӳ��
protected:
	//��ʼ��ť
	LRESULT	OnStart(WPARAM wParam, LPARAM lParam);
	//��ע��ť 
	LRESULT	OnAddScore(WPARAM wParam, LPARAM lParam);
	//����ť 
	LRESULT OnClearScore(WPARAM wParam, LPARAM lParam);
	//��ע����
	LRESULT OnDecreaseJetton(WPARAM wParam, LPARAM lParam);
	//���ư�ť
	LRESULT	OnOpenCard(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnSendCardFinish(WPARAM wParam, LPARAM lParam);
	//�������
	LRESULT OnOpenCardFinish(WPARAM wParam, LPARAM lParam);

public:
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
