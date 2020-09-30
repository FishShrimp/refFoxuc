#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\��Ϸ�ͻ���\GameLogic.h"
#include "..\��Ϣ����\CMD_LiarsDice.h"
#include "DlgCustomRule.h"

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink :  public ITableFrameSink, public ITableUserAction
{
	//�������
protected:
	CGameLogic							m_GameLogic;										//��Ϸ�߼�
	ITableFrame							* m_pITableFrame;									//��ܽӿ�
	const tagGameServiceOption			* m_pGameServiceOption;								//���ò���
	tagGameServiceAttrib *				m_pGameServiceAttrib;								//��Ϸ����
	tagCustomRule *						m_pGameCustomRule;									//�Զ�����

	//���Ա���
protected:
	static const WORD					m_wPlayerCount;										//��Ϸ����
	static const BYTE					m_GameStartMode;									//��ʼģʽ

	//��Ϸ����
protected:
	BYTE								m_bDiceData[GAME_PLAYER][5];						//��������	
	LONGLONG							m_lChip;											//��ע��С
	LONGLONG							m_lMaxChip;											//�����ע
	WORD								m_wCurUser;											//��ǰ���
	WORD								m_wChipUser;										//��ע���
	WORD								m_wLoseUser;										//ʧ�����
	tagDiceYell							m_UserYell[GAME_PLAYER];							//��󺰻�
	WORD								m_wTurnCount;										//��������
	bool								m_bHaveYellOne;										//�Ƿ񺰹�1
	bool								m_bHaveThrow[GAME_PLAYER];							//�Ƿ�ҡ��
	bool								m_bCompleteOpen[GAME_PLAYER];						//��ɿ���

	//���ñ���
protected:
	int									m_nCfgCellScoreType;								//��������	(0 ��������ٽ�ҵİٷֱ� 1 ֱ��������ֵ other ��0)
	int									m_nCfgMinScoreMul;									//���ñ���	(����1������СΪ1/4 ���Ϊ1��, 20������СΪ1/80 ���Ϊ1/20)
	int									m_nCfgCellScore;									//���÷���	

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
	//�Ƿ���Ч
	virtual bool  IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��λ����
	virtual VOID RepositionSink();
	//��������
	virtual bool Initialization(IUnknownEx * pIUnknownEx);

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

	//��Ϣ�ӿ�
public:
	//��ʼģʽ
	virtual BYTE  GetGameStartMode();
	//��Ϸ״̬
	virtual bool  IsUserPlaying(WORD wChairID);

	//��Ϣ����
protected:
	//������ע
	bool OnSubChip(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//���ҡ��
	bool OnSubThrowDice(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//ҡ�����
	bool OnSubThrowFinish(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//��Һ���
	bool OnSubYellDice(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//��ҿ���
	bool OnSubOpenDice(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//��������
	bool OnSubCompleteOpen(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);

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

	//////////////////////////////////////////////////////////////////////////
	//�����¼�
public:
	//����ͬ��
	virtual bool  OnEventUserReqReady(WORD wChairID, IServerUserItem * pIServerUserItem){return true ;}
	//�������
	virtual bool  OnEventUserReqOffLine(WORD wChairID, IServerUserItem * pIServerUserItem){return true ;}
	//��������
	virtual bool  OnEventUserReqReConnect(WORD wChairID, IServerUserItem * pIServerUserItem){return true ;}
	//��������
	virtual bool  OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon){return true ;}
	//��������
	virtual bool  OnEventUserReqStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon){return true ;}

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
};

//////////////////////////////////////////////////////////////////////////

#endif
