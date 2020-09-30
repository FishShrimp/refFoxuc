#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "HistoryRecord.h"
//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	WORD							m_wBombTime;						//ը������
	BYTE							m_bLandScore;						//��������
	WORD							m_wFirstUser;						//�׽��û�
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ���
	BYTE							m_bScoreInfo[GAME_PLAYER];			//�з���Ϣ

	//������Ϣ
protected:
	WORD							m_wTurnWiner;						//ʤ�����
	BYTE							m_bTurnCardCount;					//������Ŀ
	BYTE							m_bTurnCardData[33];				//�����б�
	bool							m_bUserTrustee[GAME_PLAYER];		//����й�

	//�˿���Ϣ
protected:
	BYTE							m_bBackCard[8];						//��Ϸ����
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							m_bOutCardCount[GAME_PLAYER];		//���ƴ���
	BYTE							m_bHandCardData[GAME_PLAYER][33];	//�����˿�
	BYTE							m_cbMagicCardData[MAX_COUNT];		//����˿�

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CHistoryScore					m_HistoryScore;						//��ʷ�ɼ�
	
	//����ӿ�
protected:
	ITableFrame	*					m_pITableFrame;						//��ܽӿ�
	tagGameServiceOption *          m_pGameServiceOption;				//���ò���
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//��Ϸ����


	//���Ա���
protected:
	static const WORD				m_wPlayerCount;						//��Ϸ����


	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID  RepositionSink();

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){};
	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool  OnEventGameStart();
	//��Ϸ����
	virtual bool  OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool  OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool  OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter);
	//��Ϸ��Ϣ����
	virtual bool  OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool  OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}
	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}
	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//��Ϸ�¼�
protected:
	//�з��¼�
	bool OnUserLandScore(WORD wChairID, BYTE bLandScore);
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount);
	//�û�����
	bool OnUserPassCard(WORD wChairID);
};

//////////////////////////////////////////////////////////////////////////

#endif
