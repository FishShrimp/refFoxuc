#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						114								//��Ϸ I D
#define GAME_PLAYER					100								//��Ϸ����
#define GAME_NAME					TEXT("��������")				//��Ϸ����

#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾
//״̬����
#define GS_FREE						GAME_STATUS_FREE				//�ȴ���ʼ
#define	GS_BET						GAME_STATUS_PLAY				//��ע״̬
#define	GS_BET_END					GAME_STATUS_PLAY+1				//��ע����״̬
#define	GS_HORSES					GAME_STATUS_PLAY+2				//����״̬

//��Ϸ��Ϣ
#define IDM_PLAYER_BET				(WM_USER + 1000)				//��ס��Ϣ
#define IDM_ADMIN_COMMDN			(WM_USER + 999)					//������Ϣ

#define SERVER_LEN					32								//���䳤��

//��ʷ��¼
#define MAX_SCORE_HISTORY			20								//��ʷ����

//��ƥ����
#define	HORSES_ONE					0								//1����
#define	HORSES_TWO					1								//2����
#define	HORSES_THREE				2								//3����
#define	HORSES_FOUR					3								//4����
#define	HORSES_FIVE					4								//5����
#define	HORSES_SIX					5								//6����
#define HORSES_ALL					6								//�ϼ�����

//��ƥλ��
#define	HORSES_X_POS				180									//��ʼXλ��
#define	HORSES_ONE_Y_POS			315 								//1����
#define	HORSES_TWO_Y_POS			285 								//2����
#define	HORSES_THREE_Y_POS			255 								//3����
#define	HORSES_FOUR_Y_POS			220									//4����
#define	HORSES_FIVE_Y_POS			190									//5����
#define	HORSES_SIX_Y_POS			160									//6����

//��ע��������
#define AREA_1_6					0								//1_6 ����
#define AREA_1_5					1								//1_5 ����
#define AREA_1_4					2								//1_4 ����
#define AREA_1_3					3								//1_3 ����
#define AREA_1_2					4								//1_2 ����
#define AREA_2_6					5								//2_6 ����
#define AREA_2_5					6								//2_5 ����
#define AREA_2_4					7								//2_4 ����
#define AREA_2_3					8								//2_3 ����
#define AREA_3_6					9								//3_6 ����
#define AREA_3_5					10								//3_5 ����
#define AREA_3_4					11								//3_4 ����
#define AREA_4_6					12								//4_6 ����
#define AREA_4_5					13								//4_5 ����
#define AREA_5_6					14								//5_6 ����
#define AREA_ALL					15								//�ϼ�����

//��������
#define RANKING_FIRST				0								//��һ��
#define RANKING_SECOND				1								//�ڶ���
#define RANKING_THIRD				2								//������
#define RANKING_FOURTH				3								//������
#define RANKING_FIFTH				4								//������
#define RANKING_SIXTH				5								//������
#define RANKING_NULL				6								//������

//��עʧ������
#define FAIL_TYPE_OVERTOP			1								//��������
#define FAIL_TYPE_TIME_OVER			0								//��ʱ

//�ܵ�����
#define HAND_LENGTH					170							//ͷ
#define TAIL_LENGTH					185							//β

//Ԥ�����ʱ��
#define COMPLETION_TIME				25								//25��

//�ٶȿ粽
#define STEP_SPEED					950//(COMPLETION_TIME*10)			//

//�м䱳���ظ�����
#define BACK_COUNT					25

//�����ٶ�
#define BASIC_SPEED					20

//���ٶ�
#define HIGH_SPEED					300

//���ٶ�
#define LOW_SPEED					300

//����Ƶ��
#define FREQUENCY					5

//���ٶ�
#define ACCELERATION				3

//��ƥ����
#define HORSES_NAME_LENGTH			32

//��������Ϣ
struct tagRobotInfo
{
	int nChip[4];														//���붨��
	int nAreaChance[AREA_ALL];											//������
	TCHAR szCfgFileName[MAX_PATH];										//�����ļ�

	tagRobotInfo()
	{
		int nTmpChip[4] = {1000,10000,100000,1000000};
		int nTmpAreaChance[AREA_ALL] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("HorseBattle.ini");

		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};


//��¼��Ϣ
struct tagHistoryRecord
{
	INT								nStreak;						//����
	INT								nRanking;						//����
	INT								nRiskCompensate;				//����
	INT								nHours;							//Сʱ
	INT								nMinutes;						//����
	INT								nSeconds;						//����
};

//���г���
struct CMD_S_SceneFree
{
	INT								nTimeLeave;							//ʣ��ʱ��
	INT								nStreak;							//����
	INT								nMultiple[AREA_ALL];				//������
	TCHAR							szHorsesName[HORSES_ALL][HORSES_NAME_LENGTH];	//��ƥ����
	tagHistoryRecord				GameRecords[MAX_SCORE_HISTORY];		//��Ϸ��¼
	INT								nWinCount[HORSES_ALL];				//ȫ��Ӯ�ĳ���

	LONGLONG						lAreaLimitScore;						//����������
	LONGLONG						lUserLimitScore;						//������������

	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
};

//��ע����
struct CMD_S_SceneBet
{
	INT								nTimeLeave;							//ʣ��ʱ��
	INT								nStreak;							//����
	INT								nMultiple[AREA_ALL];				//������
	TCHAR							szHorsesName[HORSES_ALL][HORSES_NAME_LENGTH];	//��ƥ����
	tagHistoryRecord				GameRecords[MAX_SCORE_HISTORY];		//��Ϸ��¼
	INT								nWinCount[HORSES_ALL];				//ȫ��Ӯ�ĳ���

	INT								nBetPlayerCount;					//��ע����
	LONGLONG						lUserMaxScore;						//�������·���
	LONGLONG						lPlayerBet[AREA_ALL];				//�����ע
	LONGLONG						lPlayerBetAll[AREA_ALL];			//������ע

	LONGLONG						lAreaLimitScore;						//����������
	LONGLONG						lUserLimitScore;						//������������

	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
};

//��ע��������
struct CMD_S_SceneBetEnd
{
	INT								nTimeLeave;							//ʣ��ʱ��
	INT								nStreak;							//����
	INT								nMultiple[AREA_ALL];				//������
	TCHAR							szHorsesName[HORSES_ALL][HORSES_NAME_LENGTH];	//��ƥ����
	tagHistoryRecord				GameRecords[MAX_SCORE_HISTORY];		//��Ϸ��¼
	INT								nWinCount[HORSES_ALL];				//ȫ��Ӯ�ĳ���

	INT								nBetPlayerCount;					//��ע����
	LONGLONG						lPlayerBet[AREA_ALL];				//�����ע
	LONGLONG						lPlayerBetAll[AREA_ALL];			//������ע

	LONGLONG						lAreaLimitScore;						//����������
	LONGLONG						lUserLimitScore;						//������������

	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
};

//������
struct CMD_S_SceneHorses
{
	INT								nTimeLeave;							//ʣ��ʱ��
	INT								nStreak;							//����
	INT								nMultiple[AREA_ALL];				//������
	TCHAR							szHorsesName[HORSES_ALL][HORSES_NAME_LENGTH];	//��ƥ����
	tagHistoryRecord				GameRecords[MAX_SCORE_HISTORY];		//��Ϸ��¼
	INT								nWinCount[HORSES_ALL];				//ȫ��Ӯ�ĳ���

	INT								nBetPlayerCount;					//��ע����
	LONGLONG						lPlayerBet[AREA_ALL];				//�����ע
	LONGLONG						lPlayerBetAll[AREA_ALL];			//������ע
	LONGLONG						lPlayerWinning;						//�����Ӯ

	LONGLONG						lAreaLimitScore;						//����������
	LONGLONG						lUserLimitScore;						//������������

	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_BET_START				100									//��ʼ��ע
#define SUB_S_BET_END				101									//��ע����
#define SUB_S_HORSES_START			102									//����ʼ
#define SUB_S_PLAYER_BET			103									//�û���ע
#define SUB_S_PLAYER_BET_FAIL		104									//��עʧ��
#define SUB_S_CONTROL_SYSTEM		105									//ϵͳ����
#define SUB_S_NAMED_HORSES			106									//��ƨ����
#define SUB_S_HORSES_END			107									//�������
#define SUB_S_MANDATOY_END			108									//ǿ�ƽ���
#define SUB_S_ADMIN_COMMDN			109									//ϵͳ����

//��ʼ��ע
struct CMD_S_BetStart
{
	INT								nTimeLeave;							//ʣ��ʱ��
	LONGLONG						lUserMaxScore;						//�������·���
	INT								nTimeBetEnd;						//��ע����ʱ��
	int								nChipRobotCount;					//�������� (��ע������)
};

//��ע����
struct CMD_S_BetEnd
{
	INT								nTimeLeave;							//ʣ��ʱ��
};


//����ʼ
struct CMD_S_HorsesStart
{
	INT								nTimeLeave;							//ʣ��ʱ��
	INT								nHorsesSpeed[HORSES_ALL][COMPLETION_TIME];	//ÿƥ���ÿ���ٶ�
	BYTE							cbHorsesRanking[RANKING_NULL];		//����

	//��ҳɼ�
	LONGLONG						lPlayerWinning;						//�����Ӯ
	LONGLONG						lPlayerReturnBet;					//��ҷ�����ע
};


//�������
struct CMD_S_HorsesEnd
{
	INT								nTimeLeave;							//ʣ��ʱ��
	tagHistoryRecord				RecordRecord;						//��ʷ��¼	
	INT								nWinCount[HORSES_ALL];				//ȫ��Ӯ�ĳ���

	//����
	INT								nMultiple[AREA_ALL];				//������

	//��ҳɼ�
	LONGLONG						lPlayerWinning[AREA_ALL];			//�����Ӯ
	LONGLONG						lPlayerBet[AREA_ALL];				//��ע
};

//�����ע
struct CMD_S_PlayerBet
{
	WORD							wChairID;							//λ��
	LONGLONG						lBetScore[AREA_ALL];				//�����ע
	INT								nBetPlayerCount;					//��ע����
	bool							bIsAndroid;							//�Ƿ������
};

//�����עʧ��
struct CMD_S_PlayerBetFail
{
	WORD							wChairID;							//λ��
	BYTE							cbFailType;							//ʧ������
	LONGLONG						lBetScore[AREA_ALL];				//�����ע
};

//ϵͳ����
struct CMD_S_ControlSystem
{

};

//��ƨ����
struct CMD_S_NamedHorses
{

};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_PLAYER_BET			1									//�û���ע
#define SUB_C_NAMED_HORSES			2									//��ƨ����
#define SUB_C_ADMIN_COMMDN			3									//ϵͳ����

//�����ע
struct CMD_C_PlayerBet
{
	LONGLONG						lBetScore[AREA_ALL];				//�����ע
};
//��ƨ����
struct CMD_C_NamedHorses
{

};


//////////////////////////////////////////////////////////////////////////
//����
//���������Ʒ���
#define	 S_CR_FAILURE				0		//ʧ��
#define  S_CR_UPDATE_SUCCES			1		//���³ɹ�
#define	 S_CR_SET_SUCCESS			2		//���óɹ�
#define  S_CR_CANCEL_SUCCESS		3		//ȡ���ɹ�
struct CMD_S_ControlReturns
{
	BYTE cbReturnsType;					//�ظ�����
	BYTE cbControlTimes;				//���ƴ���
	BYTE cbControlArea;					//��������
	BYTE bAuthoritiesExecuted;			//����ִ��
	INT  nControlMultiple[AREA_ALL];	//���Ʊ���
};


//�ͻ��˿�������
#define  C_CA_UPDATE				1		//����
#define	 C_CA_SET					2		//����
#define  C_CA_CANCELS				3		//ȡ��
struct CMD_C_ControlApplication
{
	BYTE cbControlAppType;				//��������
	BYTE cbControlTimes;				//���ƴ���
	BYTE cbControlArea;					//��������
	BYTE bAuthoritiesExecuted;			//����ִ��
	INT  nControlMultiple[AREA_ALL];	//���Ʊ���
	
};

#endif
