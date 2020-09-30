#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, ITableUserAction
{
	//��Ϸ����
protected:
	WORD							m_wCurrentUser;						//��ǰ���	
	__int64							m_nTatolScore[GAME_PLAYER];			//�ܵ���Ӯ����
	__int64							m_nTurnScore[GAME_PLAYER];			//��һ����Ӯ
	__int64                         m_baseScore;                        //��ע
 	BYTE							m_cbPlayOutTimeNum[GAME_PLAYER];	//��Ϸ��ʱ����,��ʱ5������
	BYTE                            m_ball[GAME_PLAYER][8];              //ÿ���˴��ȥ����
	BYTE                            m_ballNum[GAME_PLAYER];
	BYTE						    m_userBallType[GAME_PLAYER];         //1���Ǵ���0����С��
	bool                            m_bFirstBall;                        //�Ƿ��ǵ�һ������
	bool                            m_bWrongHit[GAME_PLAYER];            //�Ƿ���򷸹�
	bool                            m_bHit;
	PT                              m_ptBallPos[BALL_NUM];               //ÿ�����λ��
	BYTE                            m_comob[GAME_PLAYER];                //����
	BYTE                            m_currentComob;                      //��ǰ��������
	BYTE                            m_roundCount;                        //�ڼ���
	bool                            m_bGameStart;
	bool                            m_bSendHitResult[GAME_PLAYER];       //�Ƿ��յ�������
	CMD_S_HitResult                 m_hitResult;                         //������
	CMD_S_HitBall                   m_hitBall;                           //��������
	BYTE                            m_endReason;                         //����ԭ��

	//�������
protected:
	ITableFrame						* m_pITableFrame;					//��ܽӿ�
	tagGameServiceOption *			m_pGameServiceOption;					//��Ϸ����
	tagGameServiceAttrib *			m_pGameServiceAttrib;					//��Ϸ����

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
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
	virtual VOID * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();


	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool  OnEventGameStart();
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

	
	//�û������¼�
public:
	//�û�����
	virtual bool  OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool  OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool  OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool  OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }
	
	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//��ѯ�����
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}

	//�¼��ӿ�
public:
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore) { }
	//��Ϸ�¼�
protected:	
	//����ʧ�ܣ����¿���
	void OnFaultBegin();
	//�����
	bool OnUserHitBallResult(const void * pDataBuffer);
	
};

//////////////////////////////////////////////////////////////////////////

#endif