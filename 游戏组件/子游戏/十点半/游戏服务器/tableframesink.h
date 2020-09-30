#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "DlgCustomRule.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	LONGLONG						m_lCellScore;						//��ע
	WORD							m_wCurrentUser;						//��ǰ���
	WORD							m_wBankerUser;						//ׯ��
	LONGLONG						m_lGameScore[GAME_PLAYER];			//�÷�

	//״̬����
	BYTE							m_byUserStatus[GAME_PLAYER];		//���״̬
	bool							m_bAddScore[GAME_PLAYER];			//�Ƿ��Ѽӱ�
	
	//��ע����
	LONGLONG						m_lTableScore[GAME_PLAYER];			//��ǰ����ע

	//�˿˱���
	BYTE							m_cbRepertoryCard[FULL_COUNT];		//����˿�
	BYTE							m_bySendCardCount;					//������Ŀ
	BYTE							m_byCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							m_byHandCardData[GAME_PLAYER][MAX_COUNT];	//�����˿�

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;					//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;				//���ò���
	tagGameServiceAttrib *			m_pGameServiceAttrib;					//��Ϸ����
	tagCustomRule *					m_pGameCustomRule;						//�Զ�����

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;						//��Ϸ����
	static const BYTE		m_GameStartMode;					//��ʼģʽ

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual VOID RepositionSink();
	//��������
	virtual bool Initialization(IUnknownEx * pIUnknownEx);

	//��Ϣ�ӿ�
public:
	//��ʼģʽ
	virtual BYTE  GetGameStartMode();
	//��Ϸ״̬
	virtual bool  IsUserPlaying(WORD wChairID);

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem);
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){};


	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//����ӿ�
public:
	//��Ϸ��Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }
	
	//��Ϸ�¼�
protected:
	//��Ҽӱ�
	bool OnSubUserAddScore( WORD wChariId );
	//���ͣ��
	bool OnSubUserGiveUp( WORD wChairId );
	//���Ҫ��
	bool OnSubUserGiveCard( WORD wChairId );
	//�����ע
	bool OnSubUserScore(WORD wChairId,LONGLONG lScore);

	//��������
protected:
	//��ʼ����
	VOID SendGamePlay();
};

//////////////////////////////////////////////////////////////////////////

#endif