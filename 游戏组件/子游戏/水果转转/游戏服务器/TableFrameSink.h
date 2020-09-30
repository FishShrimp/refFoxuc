#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"

//ʤ�ʿ����˺���Ϣ
struct ControlAccount	
{
	DWORD dwUserID;
	int	  nOdds;
};

//��ʼģʽ
enum enStartMode
{
	enStartMode_FullReady,				//���˿�ʼ
	enStartMode_AllReady,				//����׼��
	enStartMode_Symmetry,				//�Գƿ�ʼ
	enStartMode_TimeControl,			//ʱ�����
};

//��������
enum enScoreKind
{
	enScoreKind_Win,					//ʤ
	enScoreKind_Lost,					//��
	enScoreKind_Draw,					//��
	enScoreKind_Flee,					//��
	enScoreKind_Service,				//����
	enScoreKind_Present,				//����
};
//#define START_MODE_ALL_READY		0x00								//����׼��
//#define START_MODE_FULL_READY		0x01								//���˿�ʼ
//#define START_MODE_PAIR_READY		0x02								//��Կ�ʼ
//#define START_MODE_TIME_CONTROL		0x10								//ʱ�����
//#define START_MODE_MASTER_CONTROL	0x11								//�������

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//����ע��
protected:
	__int64								m_iTotalJettonScore[JETTON_AREA_COUNT];				//��������ע�ܶ�����
	__int64								m_iUserAreaScore[GAME_PLAYER][JETTON_AREA_COUNT];	//���������������ע������
	__int64								m_iAreaScoreLimit[JETTON_AREA_COUNT];				//������ǰ���·�
	__int64								m_iSysLimitScore;									//ϵͳ��ע����ϵͳ��ׯʱ����ע�޶�
	static const	int					m_iOddsArray[JETTON_AREA_COUNT];					//����������
	const	int							m_iMaxAreaScore;									//���������������·�
	bool								m_bControlWin;										//�Ƿ����ʤ�ʣ�������
	int									m_nMaxBankerTimes;									//һ��ׯ�������ڿ�������پ֣�������
	int									m_nHasWinTimes;										//ĳׯ����ׯ��������Ӯ�˶��ٴ�

	CWHArray<ControlAccount>		m_CtrlAccountArray;									//�˺ſ�������


	//���Ʊ���
protected:
	__int64							m_iUserLimitScore;						//������ע
	__int64							m_iApplyBankerCondition;				//��ׯ����
	TCHAR							m_szConfigFileName[MAX_PATH];			//�����ļ�

	//��ҳɼ�
protected:
	__int64							m_iUserWinScore[GAME_PLAYER];			//��ҳɼ�
	__int64							m_iUserReturnScore[GAME_PLAYER];		//������ע
	__int64							m_iUserRevenue[GAME_PLAYER];			//���˰��

	//�˿���Ϣ
protected:
	BYTE							m_iGoalAnimal;							//������Ϸ�Ὺ���Ķ���
	__int64							m_iGameTimes;							//��ǰ����Ϸ���������ĵڼ���

	//״̬����
protected:
	DWORD							m_dwJettonTime;							//��עʱ��

	//ׯ����Ϣ
protected:
	CWHArray<WORD>			m_ApplyUserArray;						//������ׯ����б�
	WORD							m_wCurrentBanker;						//��ǰׯ��
	WORD							m_wBankerTime;							//��ׯ����
	__int64							m_iBankerWinScore;						//�ۼƳɼ�
	__int64							m_iBankerCurGameScore;					//��ǰ�ɼ�
	bool							m_bEnableSysBanker;						//ϵͳ��ׯ��������

	//��¼����
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//��Ϸ��¼
	int								m_nRecordFirst;							//��ʼ��¼
	int								m_nRecordLast;							//����¼
	CFile							m_FileRecord;							//��¼���
	DWORD							m_dwRecordCount;						//��¼��Ŀ

	//�������
protected:
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
//	ITableFrameControl				* m_pITableFrameControl;				//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
	static const enStartMode		m_GameStartMode;						//��ʼģʽ

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
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID)	{return true;}
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore)	{ }

	//�̳нӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem) { return 0;}
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem) {return 0;}

	//����ӿ�
public:
	//��λ�ӿ�
	virtual VOID RepositionSink();
	//���ýӿ�
	virtual bool Initialization(IUnknownEx * pIUnknownEx);

	//��Ϣ�ӿ�
public:
	//��ʼģʽ
	virtual enStartMode __cdecl GetGameStartMode();
	//�ϳº�2��
	virtual bool __cdecl GetGameBGode(WORD nChair){return true;};
	//��Ϸ״̬
	virtual bool __cdecl IsUserPlaying(WORD wChairID);

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//xxx
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize) {return true;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return true;}


	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM dwBindParameter);
	//��Ϸ��Ϣ����
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�����¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) ;
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem){ return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize){ return true; }


	//��Ϸ�¼�
protected:
	//��ע�¼�
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, __int64 iJettonScore);
	//����ׯ��
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//ȡ������
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);

	//���ͺ���
private:
	//�����˿�
	bool DispatchTableCard();

	//���淢��
	void NormalDispatchCard();

	//���Ʒ���
	void CtrlDispatchCard(int nCtrlWinOdds = 100 );

	//����ׯ��
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//����ׯ��
	bool ChangeBanker(bool bCancelCurrentBanker);
	//���ͼ�¼
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//������Ϣ
	void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);

	//ÿ����Ϸ��ʼʱ�̣������������·�ֵ
	bool __cdecl CalculateAreaInitLimite();


	//��Ϸͳ��
private:
	//����÷�
    __int64 CalculateScore();

	//ʤ�ʼ��㣬ϵͳ��ׯ������ʱ�á�����ֵΪ��������ʤ�����򣬷�Χ��0-7
	int	CalculateWinOdds();

	//����ĳ���������Ӯ�����������Ӯ�Ľ����
	bool GetOneAreaWinOdds(int nAreaIndex, __int64 & iResult);
};

//////////////////////////////////////////////////////////////////////////

#endif
