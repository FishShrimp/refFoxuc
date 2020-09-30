#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//������

//��Ϸ����
#define KIND_ID						127									//��Ϸ I D
#define GAME_NAME					TEXT("��������")					//��Ϸ����

//�������
#define GAME_PLAYER					MAX_CHAIR							//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

//////////////////////////////////////////////////////////////////////////////////
//״̬����

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_BET				GAME_STATUS_PLAY					//��ע״̬
#define GAME_SCENE_END				(GAME_STATUS_PLAY+1)				//����״̬

//////////////////////////////////////////////////////////////////////////////////
//�����
#define GEM_FIRST							4							//��һ����ֵ
#define GEM_SECOND							5							//�ڶ�����ֵ
#define GEM_THIRD							6							//��������ֵ
#define GEM_MAX								7							//�����ֵ

//////////////////////////////////////////////////////////////////////////////////
//ת������
#define TURAN_TABLE_MAX						28							//ת������
#define TURAN_TABLE_JUMP					30							//ת����ת

//��������
#define ANIMAL_LION							0							//ʨ��
#define ANIMAL_PANDA						1							//��è
#define ANIMAL_MONKEY						2							//����
#define ANIMAL_RABBIT						3							//����
#define ANIMAL_EAGLE						4							//��ӥ
#define ANIMAL_PEACOCK						5							//��ȸ
#define ANIMAL_PIGEON						6							//����
#define ANIMAL_SWALLOW						7							//����
#define ANIMAL_SLIVER_SHARK					8							//����
#define ANIMAL_GOLD_SHARK					9							//����
#define ANIMAL_BIRD							10							//����
#define ANIMAL_BEAST						11							//����
#define ANIMAL_MAX							12

//������Ϣ
#define ANIMAL_TYPE_NULL					0							//��
#define ANIMAL_TYPE_BEAST					1							//����
#define ANIMAL_TYPE_BIRD					2							//����
#define ANIMAL_TYPE_GOLD					3							//�ƽ�
#define ANIMAL_TYPE_SLIVER					4							//����


//��Ϸ��¼
#define RECORD_COUNT_MAX					20							//�������

#define BULLET_MAX_COUNT                    6                           //�һ����������Ŀ

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
#define IDM_UPDATE_CHIP						(WM_USER+2000)				//���³�����Ϣ
#define IDM_EXCHANGE_CHIP					(WM_USER+200)				//�һ�������Ϣ

//��ע��ť
#define IDM_PLAY_BET						(WM_USER+201)				//ʨ��

//���ư�ť
#define IDM_OPERATION_RENEWAL				(WM_USER+213)				//��ѹ
#define IDM_OPERATION_CANCEL				(WM_USER+214)				//ȡ��
#define IDM_OPERATION_SWITCH				(WM_USER+215)				//�л�
#define IDM_ADMIN_COMMDN WM_USER+1000

//////////////////////////////////////////////////////////////////////////////////
//����
#define	MAX_VOLUME (3000)
#define SUB_C_AMDIN_COMMAND			4									//ϵͳ����
//////////////////////////////////////////////////////////////////////////////////
//�����-������
#define SUB_S_AMDIN_COMMAND			109									//ϵͳ����


//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct CMD_C_AdminReq
{
	BYTE cbReqType;
#define RQ_SET_WIN_AREA  	1
#define RQ_RESET_CONTROL	2
#define RQ_PRINT_SYN		3
	BYTE cbExtendData[20];			//��������
};

//����ظ�
struct CMD_S_CommandResult
{
	BYTE cbAckType;					//�ظ�����
#define ACK_SET_WIN_AREA  1
#define ACK_PRINT_SYN     2
#define ACK_RESET_CONTROL 3
	BYTE cbResult;
#define CR_ACCEPT  2			//����
#define CR_REFUSAL 3			//�ܾ�
	BYTE cbExtendData[20];			//��������
};
//����������Ϣ
struct tagControlInfo
{
	INT  nAreaWin[ANIMAL_MAX];		//��������
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��Ϸ״̬
struct CMD_S_StatusFree
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	LONG							lCellScore;							//�׷�
	LONGLONG						lPlayScore;							//��ҷ���
	LONGLONG						lStorageStart;						//��棨�ʳأ�

	LONGLONG						lAreaLimitScore;					//��������
	LONGLONG						lPlayLimitScore;					//�������
	
	int								nTurnTableRecord[RECORD_COUNT_MAX];	//��Ϸ��¼
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	LONG							lCellScore;							//�׷�
	LONGLONG						lPlayScore;							//��ҷ���
	LONGLONG						lPlayChip;							//��ҳ���
	LONGLONG						lStorageStart;						//��棨�ʳأ�		

	LONGLONG						lAreaLimitScore;					//��������
	LONGLONG						lPlayLimitScore;					//�������

	int								nAnimalMultiple[ANIMAL_MAX];		//���ﱶ��
	LONGLONG						lAllBet[ANIMAL_MAX];				//����ע
	LONGLONG						lPlayBet[ANIMAL_MAX];				//�����ע

	int								nTurnTableRecord[RECORD_COUNT_MAX];	//��Ϸ��¼
};

//��Ϸ״̬
struct CMD_S_StatusEnd
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	LONG							lCellScore;							//�׷�
	LONGLONG						lPlayScore;							//��ҷ���
	LONGLONG						lPlayChip;							//��ҳ���
	LONGLONG						lStorageStart;						//��棨�ʳأ�

	LONGLONG						lAreaLimitScore;					//��������
	LONGLONG						lPlayLimitScore;					//�������

	int								nAnimalMultiple[ANIMAL_MAX];		//���ﱶ��
	LONGLONG						lAllBet[ANIMAL_MAX];				//����ע
	LONGLONG						lPlayBet[ANIMAL_MAX];				//�����ע

	int								nTurnTableRecord[RECORD_COUNT_MAX];	//��Ϸ��¼
};


//////////////////////////////////////////////////////////////////////////////////
//����������ṹ
enum
{
	SUB_S_GAME_FREE = 101,						//��Ϸ����
	SUB_S_GAME_START,							//��Ϸ��ʼ
	SUB_S_GAME_END,								//��Ϸ����
	SUB_S_PLAY_BET,								//�û���ע
	SUB_S_PLAY_BET_FAIL,						//�û���עʧ��
	SUB_S_BET_CLEAR,							//�����ע
};

//��Ϸ����
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	int								nAnimalMultiple[ANIMAL_MAX];		//���ﱶ��
	LONGLONG						lStorageStart;						//��棨�ʳأ�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	BOOL							bTurnTwoTime;						//ת2��
	int								nTurnTableTarget[2];				//ת��Ŀ��
	int								nPrizesMultiple;					//�ʽ�	
	LONGLONG						lPlayWin[2];						//�����Ӯ
	LONGLONG						lPlayPrizes;						//��Ҳʽ�

	LONGLONG						lPlayShowPrizes;					//��ʾ�ʽ�
};

//�û���ע
struct CMD_S_PlayBet
{
	WORD							wChairID;							//���λ��
	LONGLONG                        mHBet;								//��Ա�����ע
	int								nAnimalIndex;						//��ע����
	LONGLONG						lBetChip;							//��������
};

//�û���עʧ��
struct CMD_S_PlayBetFail
{
	WORD							wChairID;							//���λ��
	int								nAnimalIndex;						//��ע����
	LONGLONG						lBetChip;							//��������
};

//�����ע
struct CMD_S_BetClear
{
	WORD							wChairID;							//���λ��
	LONGLONG						lPlayBet[ANIMAL_MAX];				//����������
};
//////////////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
enum
{
	SUB_C_EXCHANGE_CHIP = 101,						//�һ�����
	SUB_C_PLAY_BET,									//��ע��Ϣ
	SUB_C_BET_CLEAR,								//�����ע
};

//�һ�����
struct CMD_C_Chip
{
	LONGLONG						lChip;								//��������
};

//��ע��Ϣ
struct CMD_C_PlayBet
{
	int								nAnimalIndex;						//��ע����
	LONGLONG						lBetChip;							//��������
};

//�����ע
struct CMD_C_BetClear
{
};
//��������Ϣ
struct tagRobotInfo
{
	int nChip[3];														//���붨��
	int nAreaChance[ANIMAL_MAX];										//������
	TCHAR szCfgFileName[MAX_PATH];										//�����ļ�
	int	nMaxTime;														//�������

	tagRobotInfo()
	{
		int nTmpChip[3] = {10,100,1000};
		int nTmpAreaChance[ANIMAL_MAX] = {11, 1, 11, 11, 4, 11, 8, 8, 8, 8, 8};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("SharkBattleConfig.ini");

		nMaxTime = 1;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};

#endif