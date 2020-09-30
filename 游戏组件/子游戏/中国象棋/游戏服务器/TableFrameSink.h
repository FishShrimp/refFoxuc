#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\��Ϣ����\CMD_CChess.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
extern void TraceMessage(LPCTSTR pszMessage);

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//�������
protected:
	bool							m_bGameRule;						//�����־
	WORD							m_wRuleRoundID;						//�����ʶ
	tagGameRuleInfo					m_GameRuleInfo;						//��Ϸ����
	BYTE							cccccccc1[255];

	//�û�״̬
protected:
	WORD							m_wBankerUser;						//ׯ�����
	WORD							m_wCurrentUser;						//��ǰ�û�
	WORD							m_wLeaveDrawTime[GAME_PLAYER];		//ʣ���ʱ
	BYTE							cccccccc2[255];

	//����״̬
protected:
	BYTE							m_cbConcludeReason;					//����ԭ��
	BYTE							m_cbPeaceCount[GAME_PLAYER];		//��ʹ���
	BYTE							cccccccc3[255];
	BYTE							m_cbRegretCount[GAME_PLAYER];		//�������
	BYTE							cccccccc4[255];
	BYTE							m_cbRequestStatus[GAME_PLAYER];		//����״̬
	BYTE							cccccccc5[255];

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;					//��ܽӿ�
	tagGameServiceOption *			m_pGameServiceOption;				//��Ϸ����
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//��Ϸ����
	//CGlobalUnits *                  m_pGlobalUnits;                     //ȫ�ֵ�Ԫ

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;						//��Ϸ����
	static const BYTE				m_GameStartMode;					//��ʼģʽ

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() { }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(const IID & Guid, DWORD dwQueryVer);


	//����ӿ�
public:
	//��ʼ��
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();

	//��Ϣ�ӿ�
public:
	
	//��Ϸ״̬
	virtual bool  IsUserPlaying(WORD wChairID);

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
	//��ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����¼�
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize);


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
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) ;
	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){};

	//��Ϸ��Ϣ
protected:
	//�ƶ�����
	bool OnUserMoveChess(WORD wChairID, BYTE cbXSourcePos, BYTE cbYSourcePos, BYTE cbXTargetPos, BYTE cbYTargetPos, WORD wUserTimeCount);
	//��������
	bool OnUserPeaceRequest(WORD wChairID);
	//�����Ӧ
	bool OnUserPeaceRespond(WORD wChairID, BYTE cbApprove);
	//��������
	bool OnUserRegretRequest(WORD wChairID);
	//�����Ӧ
	bool OnUserRegretRespond(WORD wChairID, BYTE cbApprove);
	//��������
	bool OnUserConcludeRequest(WORD wChairID, BYTE cbReason);

	//��������
protected:
	//���ּ���
	bool CalculateGameScore(WORD wWinnerUser, LONGLONG lGameScore[GAME_PLAYER]);
};

//////////////////////////////////////////////////////////////////////////

#endif
