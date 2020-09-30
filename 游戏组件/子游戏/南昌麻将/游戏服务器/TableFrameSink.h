#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//Ч������
enum enEstimatKind
{
	EstimatKind_OutCard,			//����Ч��
	EstimatKind_GangCard,			//����Ч��
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	WORD							m_wSiceCount;							//���ӵ���
	WORD							m_wSiceCount2;							//���ӵ�����
	WORD							m_wSiceCount3;							//���ӵ���3
	WORD							m_wBankerUser;							//ׯ���û�
	BYTE							m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	//�û��˿�
	bool							m_bCallCard[GAME_PLAYER];				//ϴ���־
	BYTE							m_cbBatch[GAME_PLAYER];					//ϴ�豶��
	bool							m_bTrustee[GAME_PLAYER];				//�Ƿ��й�
	bool							m_bBatchEnd[GAME_PLAYER];				//ϴ�����

protected:
	//��������
	BYTE							m_cbKingCardIndex[2];					//��������
	BYTE							m_cbKingCount[GAME_PLAYER];				//������Ŀ
	BYTE							m_cbViceKingCount[GAME_PLAYER];			//������Ŀ	
	BYTE							m_cbOutKingScore[GAME_PLAYER];			//��ط���		
	
	LONG							m_lUpDownScore[GAME_PLAYER];			//���·��÷�	
	BYTE							m_cbUpDownKingCardData;				//���·�����

	//��ʷ����
protected:
	LONG							m_lAllTurnScore[GAME_PLAYER];			//�ֵܾ÷�
	LONG							m_lLastTurnScore[GAME_PLAYER];			//�Ͼֵ÷�

protected:	
	//��ʱ����
	LONG							m_lGameScoreEx[GAME_PLAYER];			//��Ϸ�÷�

	//������Ϣ
protected:
	WORD							m_wOutCardUser;							//�����û�
	BYTE							m_cbOutCardData;						//�����˿�
	BYTE							m_cbOutCardCount;						//������Ŀ
	BYTE							m_cbDiscardCount[GAME_PLAYER];			//������Ŀ
	BYTE							m_cbDiscardCard[GAME_PLAYER][55];		//������¼
	BYTE							m_cbGangCount;							//���ƴ���
	INT								m_BankContinueCount;					//��ׯ���� ����¥
	BYTE							m_cbMustLeft;							//��8ǽ��

	//������Ϣ
protected:
	BYTE							m_cbSendCardData;						//�����˿�
	BYTE							m_cbSendCardCount;						//������Ŀ
	BYTE							m_cbLeftCardCount;						//ʣ����Ŀ
	BYTE							m_cbRepertoryCard[MAX_REPERTORY];		//����˿�

	//���б���
protected:
	WORD							m_wResumeUser;							//��ԭ�û�
	WORD							m_wCurrentUser;							//��ǰ�û�
	WORD							m_wProvideUser;							//��Ӧ�û�
	BYTE							m_cbProvideCard;						//��Ӧ�˿�

	//״̬����
protected:
	bool							m_bSendStatus;							//����״̬
	bool							m_bGangStatus;							//����״̬
	bool							m_bEnjoinChiHu[GAME_PLAYER];			//��ֹ�Ժ�
	bool							m_bEnjoinChiPeng[GAME_PLAYER];			//��ֹ����

	//�û�״̬
public:
	bool							m_bResponse[GAME_PLAYER];				//��Ӧ��־
	WORD							m_wUserAction[GAME_PLAYER];				//�û�����
	BYTE							m_cbOperateCard[GAME_PLAYER];			//�����˿�
	WORD							m_wPerformAction[GAME_PLAYER];			//ִ�ж���

	//����˿�
protected:
	BYTE							m_cbWeaveItemCount[GAME_PLAYER];		//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][4];		//����˿�

	//������Ϣ
protected:
	BYTE							m_cbChiHuCard;							//�Ժ��˿�
	tagChiHuResult					m_ChiHuResult[GAME_PLAYER];				//�Ժ����

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

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
	virtual VOID  Release() {}
	
	//�ӿڲ�ѯ
	virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool  Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual void  RepositionSink();

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

	//��Ϸ״̬
	virtual bool  IsUserPlaying(WORD wChairID);

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
	virtual bool  OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool  OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool  OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����¼�
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize);

	//�û��¼�
public:
	//�û�����
	virtual bool  OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool  OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool  OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool  OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool  OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize) { return true; }

	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData);
	//�û�����
	bool OnUserOperateCard(WORD wChairID, WORD wOperateCode, BYTE cbOperateCard);
	//ϴ��
	bool OnUserCallFinish(WORD wChairID,BYTE cbChipTimes);
	//��Ϸ��ʼ
	bool OnEventGameStart2();
	//ϴ�����
	bool OnUserBatchEnd(WORD wChairID);


	//��������
protected:
	//���Ͳ���
	bool SendOperateNotify();
	//�ɷ��˿�
	bool DispatchCardData(WORD wCurrentUser,bool bNotGang=true);
	//��Ӧ�ж�
	bool EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind);
};

//////////////////////////////////////////////////////////////////////////

#endif