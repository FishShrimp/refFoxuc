#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"
#include "ServerControl.h"
#include "HistoryScore.h"

//////////////////////////////////////////////////////////////////////////////////
//��ʷ��¼
#define MAX_SCORE_HISTORY			16									//��ʷ����
//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//����ע��
protected:
	LONGLONG							m_lAreaInAllScore[AREA_ALL];			//ÿ��������ܷ�				


	//������ע
protected:
	LONGLONG							m_lUserInAllScore[GAME_PLAYER][AREA_ALL];//ÿ�����ÿ���������ע

	//���Ʊ���
protected:
	bool								m_bRefreshCfg;							//ÿ��ˢ��
	LONGLONG							m_lAreaLimitScore;						//��������
	LONGLONG							m_lUserLimitScore;						//��������
	LONGLONG							m_lApplyBankerCondition;				//��������
	TCHAR								m_szConfigFileName[MAX_PATH];			//�����ļ�
	TCHAR								m_szGameRoomName[LEN_SERVER];			//��������
	LONGLONG							m_StorageStart;							//��������ÿ���ӵĿ����ֵ����ȡʧ�ܰ� 0 ����
	LONGLONG							m_StorageDeduct;						//ÿ����Ϸ������۳��Ŀ���������ȡʧ�ܰ� 1.00 ����


	//��ҳɼ�
protected:
	LONGLONG							m_lUserWinScore[GAME_PLAYER];			//��ҳɼ�
	LONGLONG							m_lUserReturnScore[GAME_PLAYER];		//������ע
	LONGLONG							m_lUserRevenue[GAME_PLAYER];			//���˰��

	//�˿���Ϣ
protected:
	BYTE							m_cbCardCount[2];						//�˿���Ŀ
	BYTE							m_cbTableCardArray[2][3];				//�����˿�

	//״̬����
protected:
	DWORD							m_dwJettonTime;							//��ʼʱ��

	//ׯ����Ϣ
protected:
	CWHArray<WORD>			m_ApplyUserArray;						//�������
	WORD							m_wCurrentBanker;						//��ǰׯ��
	WORD							m_wBankerTime;							//��ׯ����
	LONGLONG						m_lBankerScore;							//ׯ�ҽ��
	LONGLONG						m_lBankerWinScore;						//�ۼƳɼ�
	LONGLONG						m_lBankerCurGameScore;					//��ǰ�ɼ�
	bool							m_bEnableSysBanker;						//ϵͳ��ׯ

	//�������
protected:
	HINSTANCE						m_hInst;
	IServerControl*					m_pServerContro;


	//�����˿���
protected:
	int								m_nMaxChipRobot;						//�����Ŀ (��ע������)
	int								m_nChipRobotCount;						//����ͳ�� (��ע������)
	LONGLONG						m_lRobotAreaLimit;						//��������������
	LONGLONG						m_lRobotBetCount;						//��������ע����
	LONGLONG						m_lRobotAreaScore[AREA_ALL];			//������������ע
	int								m_nRobotListMaxCount;					//�������


	//ׯ������
protected:
	//��ׯ�������ã���ׯ�������趨�ľ���֮��(m_lBankerMAX)��
	//�������ֵ��������������ׯ���б�����������ҽ��ʱ��
	//�����ټ���ׯm_lBankerAdd�֣���ׯ���������á�
	LONGLONG						m_lBankerMAX;							//���ׯ����
	LONGLONG						m_lBankerAdd;							//ׯ��������

	//��ҳ���m_lBankerScoreMAX֮�󣬾�����������ҵĽ��ֵ�������Ľ��ֵ����Ҳ�����ټ�ׯm_lBankerScoreAdd�֡�
	LONGLONG						m_lBankerScoreMAX;						//ׯ��Ǯ
	LONGLONG						m_lBankerScoreAdd;						//ׯ��Ǯ��ʱ,��ׯ������

	//���ׯ����
	LONGLONG						m_lPlayerBankerMAX;						//������ׯ����

	//��ׯ
	bool							m_bExchangeBanker;						//����ׯ��

	//ʱ������
protected:						
	BYTE							m_cbFreeTime;							//����ʱ��
	BYTE							m_cbBetTime;							//��עʱ��
	BYTE							m_cbEndTime;							//����ʱ��

	//��¼����
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//��Ϸ��¼
	int								m_nRecordFirst;							//��ʼ��¼
	int								m_nRecordLast;							//����¼
	CFile							m_FileRecord;							//��¼���
	DWORD							m_dwRecordCount;						//��¼��Ŀ

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	tagGameServiceOption			* m_pGameServiceOption;					//���ò���
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
	virtual VOID Release();
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��λ�ӿ�
	virtual VOID RepositionSink();
	//���ýӿ�
	virtual bool Initialization(IUnknownEx * pIUnknownEx);

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
	//��Ϸ��Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }
	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}
	//�۳������
	virtual bool QueryBuckleServiceCharge(WORD wChairID);
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){};
	//��Ϸ�¼�
protected:
	//��ע�¼�
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore);
	//����ׯ��
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//ȡ������
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);

	//��������
private:
	//�����˿�
	bool DispatchTableCard();
	//����ׯ��
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//����ׯ��
	bool ChangeBanker(bool bCancelCurrentBanker);
	//���ͼ�¼
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//������Ϣ
	void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);

	//��ע����
private:
	//�����ע
	LONGLONG GetMaxPlayerScore( BYTE cbJettonArea, WORD wChairID );

	//��Ϸͳ��
private:
	//��Ϸ��������
	LONGLONG GameOver();
	//����÷�
	bool CalculateScore(OUT LONGLONG& lBankerWinScore, OUT BYTE& cbResult, OUT BYTE& cbAndValues);
	//�ƶ�Ӯ��
	void DeduceWinner(OUT INT nAreaWin[AREA_ALL], OUT BYTE& cbResult, OUT BYTE& cbAndValues);
	//��ȡ����
	void ReadConfigInformation(bool bReadFresh);
};

//////////////////////////////////////////////////////////////////////////////////

#endif