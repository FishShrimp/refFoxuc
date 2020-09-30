#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#ifndef _UNICODE

#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#else

#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#endif

#define UR_GAME_CONTROL					0x20000000L				//��Ϸ�������

//////////////////////////////////////////////////////////////////////////////////
//������

//��Ϸ����
#define KIND_ID						107									//��Ϸ I D
#define GAME_NAME					TEXT("����������")					//��Ϸ����

//�������
#define GAME_PLAYER					MAX_CHAIR									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

//////////////////////////////////////////////////////////////////////////////////
//״̬����

#define GAME_SCENE_PLACE_JETTON				GAME_STATUS_PLAY					//�ȴ���ʼ
#define GAME_SCENE_GAME_END					GAME_STATUS_PLAY+1					//��Ϸ����
//////////////////////////////////////////////////////////////////////////

//������multiple
#define MULTIPLE_KONG				0								//�� ����
#define MULTIPLE_LONG				2								//�� ����
#define MULTIPLE_PING				8								//ƽ ����
#define MULTIPLE_HU					2								//�� ����
#define MULTIPLE_2_13				2								//2  - 13 ����
#define MULTIPLE_14					8								//14 ����
#define MULTIPLE_15_26				2								//15 - 26 ����
#define MULTIPLE_2_6				8								//2  - 6 ����
#define MULTIPLE_7_11				4								//7  - 11 ����
#define MULTIPLE_12_16				3								//12 - 16 ����
#define MULTIPLE_17_21				4								//17 - 21 ����
#define MULTIPLE_22_26				8								//22 - 26 ����

//��������area
#define AREA_KONG					0								//�� ����
#define AREA_LONG					1								//�� ����
#define AREA_PING					2								//ƽ ����
#define AREA_HU						3								//�� ����
#define AREA_2_13					4								//2  - 13 ����
#define AREA_14						5								//14 ����
#define AREA_15_26					6								//15 - 26 ����
#define AREA_2_6					7								//2  - 6 ����
#define AREA_7_11					8								//7  - 11 ����
#define AREA_12_16					9								//12 - 16 ����
#define AREA_17_21					10								//17 - 21 ����
#define AREA_22_26					11								//22 - 26 ����
#define AREA_ALL					12								//�ϼ�����

//������
#define RESULT_WIN					1								//Ӯ
#define RESULT_LOSE					-1								//��

//��¼��Ϣ
struct tagServerGameRecord
{
	BYTE							cbResult;							//�� ƽ ��
	BYTE							cbAndValues;						//�� ֵ
	BYTE							cbLong;								//����
	BYTE							cbHu;								//����
};


//��������Ϣ
struct tagRobotInfo
{
	int nChip[6];														//���붨��
	int nAreaChance[AREA_ALL];											//������
	TCHAR szCfgFileName[MAX_PATH];										//�����ļ�
	int	nMaxTime;														//�������

	tagRobotInfo()
	{
		int nTmpChip[6] = {100,1000,10000,100000,1000000,5000000};
		int nTmpAreaChance[AREA_ALL] = {11, 1, 11, 11, 4, 11, 8, 8, 8, 8, 8};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("LongHuDouBattle.ini");

		nMaxTime = 1;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
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
//////////////////////////////////////////////////////////////////////////////////
//�����-������

#define SUB_S_GAME_FREE				99									//��Ϸ����
#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			101									//�û���ע
#define SUB_S_GAME_END				102									//��Ϸ����
#define SUB_S_APPLY_BANKER			103									//����ׯ��
#define SUB_S_CHANGE_BANKER			104									//�л�ׯ��
#define SUB_S_CHANGE_USER_SCORE		105									//���»���
#define SUB_S_SEND_RECORD			106									//��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL		107									//��עʧ��
#define SUB_S_CANCEL_BANKER			108									//ȡ������
#define SUB_S_AMDIN_COMMAND			109									//ϵͳ����


//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	WORD							wPlaceUser;							//��ע���
	BYTE							lJettonArea;						//��ע����
	LONGLONG							lPlaceScore;						//��ǰ��ע
};

//���»���
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//���Ӻ���
	DOUBLE							lScore;								//��һ���

	//ׯ����Ϣ
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	DOUBLE							lCurrentBankerScore;				//ׯ�ҷ���
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//�������
};

//ȡ������
struct CMD_S_CancelBanker
{
	TCHAR							szCancelUser[32];					//ȡ�����
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//��ׯ���
	LONGLONG							lBankerScore;						//ׯ�ҽ��
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�����Ϣ
	LONGLONG						lUserMaxScore;							//��ҽ��

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG						lBankerWinScore;					//ׯ�ҳɼ�
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������

	TCHAR							szGameRoomName[32];			//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����ע
	LONGLONG						lAreaInAllScore[AREA_ALL];			//ÿ��������ܷ�				

	//������ע
	LONGLONG						lUserInAllScore[AREA_ALL];//ÿ�����ÿ���������ע

	//��һ���
	LONGLONG						lUserMaxScore;						//�����ע							

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������

	//�˿���Ϣ
	BYTE							cbCardCount[2];						//�˿���Ŀ
	BYTE							cbTableCardArray[2][3];				//�����˿�

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG						lBankerWinScore;					//ׯ��Ӯ��
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG						lEndBankerScore;					//ׯ�ҳɼ�
	LONGLONG						lEndUserScore;						//��ҳɼ�
	LONGLONG						lEndUserReturnScore;				//���ػ���
	LONGLONG						lEndRevenue;						//��Ϸ˰��

	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	BYTE							cbGameStatus;						//��Ϸ״̬

	TCHAR							szGameRoomName[32];					//��������
};

//��Ϸ����
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	INT64                             nListUserCount;						//�б�����
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ��λ��
	LONGLONG							lBankerScore;						//ׯ�ҽ��
	LONGLONG							lUserMaxScore;						//�ҵĽ��
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	int								nChipRobotCount;					//�������� (��ע������)
};

//�û���ע
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//�û�λ��
	BYTE							cbJettonArea;						//��������
	LONGLONG							lJettonScore;						//��ע��Ŀ
	BYTE							cbAndroid;							//������
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�˿���Ϣ
	BYTE							cbCardCount[2];						//�˿���Ŀ
	BYTE							cbTableCardArray[2][3];				//�����˿�

	//ׯ����Ϣ
	LONGLONG							lBankerScore;						//ׯ�ҳɼ�
	LONGLONG							lBankerTotallScore;					//ׯ�ҳɼ�
	INT								nBankerTime;						//��ׯ����

	//��ҳɼ�
	LONGLONG							lUserScore;							//��ҳɼ�
	LONGLONG							lUserReturnScore;					//���ػ���

	//ȫ����Ϣ
	LONGLONG							lRevenue;							//��Ϸ˰��
};

//////////////////////////////////////////////////////////////////////////////////
//�����-�ͻ���

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_CANCEL_BANKER			3									//ȡ������
#define SUB_C_AMDIN_COMMAND			4									//ϵͳ����

struct CMD_C_AdminReq
{
	BYTE cbReqType;
#define RQ_SET_WIN_AREA	1
#define RQ_RESET_CONTROL	2
#define RQ_PRINT_SYN		3
	BYTE cbExtendData[20];			//��������
};

//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define IDM_ADMIN_COMMDN WM_USER+1000

//����������Ϣ
struct tagControlInfo
{
	INT  nAreaWin[AREA_ALL];		//��������
};

//���������Ʒ���
#define	 S_CR_FAILURE				0		//ʧ��
#define  S_CR_UPDATE_SUCCES			1		//���³ɹ�
#define	 S_CR_SET_SUCCESS			2		//���óɹ�
#define  S_CR_CANCEL_SUCCESS		3		//ȡ���ɹ�
struct CMD_S_ControlReturns
{
	BYTE cbReturnsType;				//�ظ�����
	BYTE cbControlArea[AREA_ALL];	//��������
	BYTE cbControlTimes;			//���ƴ���
};


//�ͻ��˿�������
#define  C_CA_UPDATE				1		//����
#define	 C_CA_SET					2		//����
#define  C_CA_CANCELS				3		//ȡ��
struct CMD_C_ControlApplication
{
	BYTE cbControlAppType;			//��������
	BYTE cbControlArea[AREA_ALL];	//��������
	BYTE cbControlTimes;			//���ƴ���
};

//////////////////////////////////////////////////////////////////////////////////

#endif