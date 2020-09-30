#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	bool							m_bShowHand;							//�����־
	WORD							m_wOperaCount;							//��������
	WORD							m_wCurrentUser;							//��ǰ�û�
	WORD							m_wLastMostUser;						//�ϴη�������û�
	bool							m_bGameEnd;								//�Ƿ����

	//�û�״̬
protected:
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	SCORE						m_lTableScore[GAME_PLAYER*2];			//��ע��Ŀ
	SCORE						m_lUserMaxScore[GAME_PLAYER];			//�����ע
	SCORE						m_lLostScore[GAME_PLAYER];				//��ҿ۷�

	//�˿˱���
protected:
	BYTE							m_cbSendCardCount;						//������Ŀ
	BYTE							m_cbCardCount[GAME_PLAYER];				//�˿���Ŀ
	BYTE							m_cbHandCardData[GAME_PLAYER][5];		//�����˿�

	//��ע��Ϣ
protected:
	SCORE						m_lMaxScore;							//
	SCORE						m_lCellScore;							//��Ԫ��ע
	SCORE						m_lTurnMaxScore;						//�����ע
	SCORE						m_lTurnLessScore;						//��С��ע
	SCORE						m_lShowHandScore;						//

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
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
	virtual VOID Release() { }
	//�Ƿ���Ч
	virtual bool IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

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
	virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool  OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter);
	//��Ϸ��Ϣ����
	virtual bool  OnGameMessage(WORD wSubCmdID,VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool  OnFrameMessage(WORD wSubCmdID,VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
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

	//�����¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool  OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool  OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser){ return true; }
	//�û�����
	virtual bool  OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser){ return true; };
	//�û�ͬ��
	virtual bool  OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }

	//��Ϸ�¼�
protected:
	//�����¼�
	bool OnUserGiveUp(WORD wChairID,bool bLeave=false);
	//��ע�¼�
	bool OnUserAddScore(WORD wChairID, SCORE lScore, bool bGiveUp);

	//��Ϸ����
protected:
	//�ƶ�ʤ��
	WORD EstimateWinner(BYTE cbStartPos, BYTE cbConcludePos);
};

//////////////////////////////////////////////////////////////////////////

#endif
