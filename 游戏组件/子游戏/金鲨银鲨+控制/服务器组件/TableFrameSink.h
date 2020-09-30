#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"
#include "HistoryScore.h"
#include "ServerControl.h"

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//��ע��
protected:
	LONG							m_lCellScore;							//��λ����
	LONGLONG						m_lAllBet[ANIMAL_MAX];					//����ע
	LONGLONG						m_lPlayBet[GAME_PLAYER][ANIMAL_MAX];	//�����ע
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	LONGLONG						m_HPlayBet[ANIMAL_MAX];                 //��Ա�����עͳ��
	LONGLONG						m_FPlayBet[ANIMAL_MAX];					//�ǻ�Ա�����עͳ��
	LONGLONG						m_APlayBet[ANIMAL_MAX];					//��������עͳ��
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//��ҷ���
protected:
	DWORD							m_dwDrawStartTime[GAME_PLAYER];			//��ҿ�ʼʱ��
	LONGLONG						m_lPlayOriginalScore[GAME_PLAYER];		//���ԭʼ����
	LONGLONG						m_lPlayScore[GAME_PLAYER];				//��ҷ���
	LONGLONG						m_lPlayChip[GAME_PLAYER];				//��ҳ���

	//����
protected:
	LONGLONG						m_lPlayWinAnimal[GAME_PLAYER][ANIMAL_MAX];	//�����Ӯ������
	LONGLONG						m_lPlayWinChip[GAME_PLAYER];			//��ҳɼ����ܣ�
	LONGLONG						m_lPlayWinScore[GAME_PLAYER][2];		//��ҵ÷֣����׶Σ�
	LONGLONG						m_lPlayWinPrizes[GAME_PLAYER];			//��Ҳʽ�

	//ת��·��
protected:
	BOOL							m_bTurnTwoTime;							//ת2��
	int								m_nTurnTableTarget[2];					//ת��Ŀ��
	CWHArray<int>					m_ArrayTurnTableRecord;					//ת�̼�¼

	//������Ϣ
protected:
	int								m_nLastMultipleIndex;					//��һ�ε�����
	int								m_nAnimalMultiple[ANIMAL_MAX];			//����

	//���ɼ���
protected:
	int								m_nGenerationChance[ANIMAL_MAX];		//���ɼ���

	//�������
protected:
	HINSTANCE						m_hInst;
	IServerControl*					m_pServerContro;

	//״̬����
protected:
	DWORD							m_dwBetTime;							//��ʼʱ��

	//ʱ������
protected:
	BYTE							m_cbFreeTime;							//����ʱ��
	BYTE							m_cbBetTime;							//��עʱ��
	BYTE							m_cbEndTime;							//����ʱ��

	//���ñ���
protected:
	LONGLONG						m_lAreaLimitScore;						//��������
	LONGLONG						m_lPlayLimitScore;						//�������
	static LONGLONG					m_lStorageStart;						//��������ÿ���ӵĿ����ֵ����ȡʧ�ܰ� 0 ����
	static LONGLONG					m_lStorageDeduct;						//ÿ����Ϸ������۳��Ŀ���������ȡʧ�ܰ� 1.00 ����

	//����ӿ�
protected:
	ITableFrame	*					m_pITableFrame;							//��ܽӿ�
	tagGameServiceOption*			m_pGameServiceOption;					//���ò���

	TCHAR								szConfigFileName[MAX_PATH];			//�����ļ�
	TCHAR								m_szGameRoomName[LEN_SERVER];			//��������

	//�����˿���
protected:
	int								m_nMaxChipRobot;						//�����Ŀ (��ע������)
	int								m_nChipRobotCount;						//����ͳ�� (��ע������)
	LONGLONG						m_lRobotAreaLimit;						//��������������
	LONGLONG						m_lRobotBetCount;						//��������ע����
	LONGLONG						m_lRobotAreaScore[ANIMAL_MAX];			//������������ע
	int								m_nRobotListMaxCount;					//�������
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
	//��������
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();


	BYTE	GetProbility();


	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem) { return 0; }
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem) { return 0; }
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID) { return false; }

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

	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore) {}

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//��Ϣ����
public:
	//����һ���Ϣ
	bool OnSubChip(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�û���ע��Ϣ
	bool OnSubPlayBet(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����ע��Ϣ
	bool OnSubBetClear(VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);


	//���ܺ���
public:
	//����ת��
	bool CreateTableTable();
	//��Ϸ��������
	int GameOver();
	//����÷�
	LONGLONG CalculateScore( bool bFirst, int nWinAnimal, int& nPrizesMultiple );
	//����д��
	void GameOverWriteScore();
};

//////////////////////////////////////////////////////////////////////////////////

#endif