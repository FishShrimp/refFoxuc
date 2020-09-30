#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��Ϸ����
protected:
	LONGLONG						m_lRobotScoreRange[2];				//���Χ
	LONGLONG						m_lRobotBankGetScore;				//�������
	LONGLONG						m_lRobotBankGetScoreBanker;			//������� (ׯ��)
	int								m_nRobotBankStorageMul;				//����

	INT								m_nStreak;								//����
	INT								m_nDay;									//����
	DWORD							m_dwGameTime;							//��Ϸʱ��
	INT								m_nFreeTime;							//����ʱ��
	INT								m_nBetTime;								//��עʱ��
	INT								m_nBetEndTime;							//��ע����ʱ��
	INT								m_nHorsesTime;							//����ʱ��
	INT								m_nHorsesEndTime;						//�������ʱ��

	//���Ʊ���
protected:
	INT								m_nCLMultiple[AREA_ALL];				//������
	BYTE							m_cbCLTimes;							//���ƴ���
	BYTE							m_cbCLArea;								//��������
	bool							m_bControl;								//�Ƿ����

	//��Ϸ���
protected:
	BYTE							m_cbGameResults;						//��������(2��)

	//���
protected:
	LONGLONG						m_StorageStart;							//��������ÿ���ӵĿ����ֵ����ȡʧ�ܰ� 0 ����
	int								m_nStorageNowNode;						//��ǰ����
	int								m_nStorageIntervalTime;					//���������ʱ��
	int								m_nStorageCount;						//�����Ŀ
	LONGLONG						m_StorageArray[30];			//��������ÿ���ӵĿ����ֵ����ȡʧ�ܰ� 0 ����
	INT								m_StorageDeduct;						//ÿ����Ϸ������۳��Ŀ���������ȡʧ�ܰ� 1.00 ����

	//����
protected:
	INT								m_nBetPlayerCount;						//��ע����
	LONGLONG						m_lPlayerBet[GAME_PLAYER][AREA_ALL];	//�����ע
	LONGLONG						m_lPlayerBetWin[GAME_PLAYER][AREA_ALL];	//���������Ӯ
	LONGLONG						m_lPlayerBetAll[AREA_ALL];				//������ע
	LONGLONG						m_lPlayerWinning[GAME_PLAYER];			//�����Ӯ
	LONGLONG						m_lPlayerReturnBet[GAME_PLAYER];		//��ҷ�����ע
	LONGLONG						m_lPlayerRevenue[GAME_PLAYER];			//���˰��

	//������
protected:
	BOOL							m_bMultipleControl;						//��������
	INT								m_nMultiple[AREA_ALL];					//������

	//��ƥ����
protected:
	INT								m_nHorsesSpeed[HORSES_ALL][COMPLETION_TIME];	//ÿƥ���ÿ���ٶ�
	BYTE							m_cbHorsesRanking[RANKING_NULL];					//��ƥ����
	TCHAR							m_szHorsesName[HORSES_ALL][HORSES_NAME_LENGTH];	//��ƥ����

	//���Ʊ���
protected:
	LONGLONG						m_lAreaLimitScore;						//����������
	LONGLONG						m_lUserLimitScore;						//������������

	//������Ϣ
protected:
	TCHAR							m_szConfigFileName[MAX_PATH];			//�����ļ�
	TCHAR							m_szGameRoomName[SERVER_LEN];			//��������

	//��Ϸ��¼
protected:
	CWHArray<tagHistoryRecord> m_GameRecords;							//��Ϸ��¼
	INT								m_nWinCount[HORSES_ALL];				//ȫ��Ӯ�ĳ���

	//�����˿���
protected:
	int								m_nMaxChipRobot;						//�����Ŀ (��ע������)
	int								m_nChipRobotCount;						//����ͳ�� (��ע������)
	LONGLONG						m_lRobotAreaLimit;						//��������������
	LONGLONG						m_lRobotAreaScore[AREA_ALL];			//������������ע


	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	tagGameServiceOption			* m_pGameServiceOption;					//���ò���
	tagGameServiceAttrib *			m_pGameServiceAttrib;					//��Ϸ����

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
	//��ȡ����
	LONGLONG GetPrivateProfileInt64(LPCTSTR lpAppName, LPCTSTR lpKeyName, LONGLONG lDefault, LPCTSTR lpFileName);

	//����ӿ�
public:
	//��λ�ӿ�
	virtual VOID RepositionSink();
	//���ýӿ�
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
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
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID);	

	//��Ϸ�¼�
protected:
	//��ע�¼�
	bool OnSubPlayBet(const void * pBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//����
protected:
	bool OnAdminControl(const void * pBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//����
protected:
	//�������
	void HorsesProcess();
	//����ʼ
	void HorsesStart();
	//�������
	bool HorsesEnd();
	//�����
	bool FinalResults();

	//����
protected:
	//��Ҫ����
	bool NeedControl();
	//�������
	bool MeetControl();
	//��ɿ���
	bool CompleteControl();
	//��������
	void MultipleControl();
	//�����ñ���
	void RandomMultiples();
	//��������
	void ChaosArray(INT nArray[], INT nCount);
	//��������
	void ChaosArray(INT nArrayOne[], INT nCountOne, INT nArrayTwo[], INT nCountTwo);
	//����
protected:
	//�������
	bool CalculateScore();
	//�������·�
	LONGLONG GetPlayersMaxBet( WORD wChiarID, BYTE cbArea = AREA_ALL );


};

//////////////////////////////////////////////////////////////////////////

#endif
