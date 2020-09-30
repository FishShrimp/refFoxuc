#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						109									//��Ϸ I D
#define GAME_PLAYER					100									//��Ϸ����
#define GAME_NAME					TEXT("С��")					    //��Ϸ����

//״̬����
#define	GS_PLACE_JETTON				GAME_STATUS_PLAY					//��ע״̬
#define	GS_GAME_END					GAME_STATUS_PLAY+1					//����״̬
#define	GS_MOVECARD_END				GAME_STATUS_PLAY+2					//����״̬

#define VERSION_SERVER			    PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

//��������
#define ID_SHUN_MEN					1									//˳��
#define ID_JIAO_L					2									//��߽�
#define ID_QIAO						3									//��
#define ID_DUI_MEN					4									//����
#define ID_DAO_MEN					5									//����
#define ID_JIAO_R					6									//�ұ߽�
#define MAX_INDEX					3									//�������

//�������
#define BANKER_INDEX				0									//ׯ������
#define SHUN_MEN_INDEX				1									//˳������
#define DUI_MEN_INDEX				2									//��������
#define DAO_MEN_INDEX				3									//��������

#define AREA_COUNT					6									//������Ŀ
#define AREA_ARRY_COUNT				7									//������Ŀ

#define MAX_CARD					2
#define MAX_CARDGROUP				4

#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#endif

//���ʶ���
#define RATE_TWO_PAIR				12									//��������

#define SERVER_LEN					32									//���䳤��

#define CONTROL_AREA				3									//�ܿ�����

//����������Ϣ
struct tagControlInfo
{
	BYTE cbControlArea[MAX_INDEX];			//��������
};

struct tagAdminReq
{
	BYTE							m_cbExcuteTimes;					//ִ�д���
	BYTE							m_cbControlStyle;					//���Ʒ�ʽ
#define		CS_BANKER_LOSE    1
#define		CS_BANKER_WIN	  2
#define		CS_BET_AREA		  3
	bool							m_bWinArea[3];						//Ӯ������
};

struct CMD_C_CheakAccount  //
{
	TCHAR							dwUserAccount[32];					//ȡ�����
};

struct CMD_C_AdminReq
{
	BYTE cbReqType;
#define RQ_SET_WIN_AREA	1
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

//��������Ϣ
struct tagRobotInfo
{
	int nChip[7];														//���붨��
	int nAreaChance[AREA_COUNT];										//������
	TCHAR szCfgFileName[MAX_PATH];										//�����ļ�
	int	nMaxTime;														//�������

	tagRobotInfo()
	{
		int nTmpChip[7] = {1000, 10000, 100000, 500000, 1000000, 5000000, 10000000};
		int nTmpAreaChance[AREA_COUNT] = {1, 0, 0, 1, 1, 0};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("NineXiaoBattleConfig.ini");

		nMaxTime = 1;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};

//��¼��Ϣ
struct tagServerGameRecord
{
	bool							bWinShunMen;						//˳��ʤ��
	bool							bWinDuiMen;							//����ʤ��
	bool							bWinDaoMen;							//����ʤ��
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

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
#define SUB_S_CANCEL_CAN			109									//ȡ������
#define SUB_S_ANDROA_AREA			110									//Ӯ����
#define SUB_S_AMDIN_COMMAND_RESULT  111									//����Ա����
#define SUB_S_TO_BANKER_MAX         112									//�ﵽׯ������

#define SUB_S_AMDIN_COMMAND			110									//����Ա����

#define SUB_S_SEND_ACCOUNT			113									//�����˺�
#define SUB_S_ADMIN_CHEAK			114									//��ѯ�˺�

//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	WORD							wPlaceUser;							//��ע���
	BYTE							lJettonArea;						//��ע����
	LONGLONG						lPlaceScore;						//��ǰ��ע
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

//�Ƿ�ɹ�ȡ������
struct CMD_S_bCanCancelBanker
{
	WORD							wChariID;
	BOOL							blCancel;
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//��ׯ���
	LONGLONG						lBankerScore;						//ׯ�ҽ��
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

	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����ע
	LONGLONG						lAllJettonScore[AREA_ARRY_COUNT];		//ȫ����ע

	//�����ע
	LONGLONG						lUserJettonScore[AREA_ARRY_COUNT];		//������ע

	//��һ���
	LONGLONG						lUserMaxScore;						//�����ע							

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������

	//�˿���Ϣ
	BYTE							cbTableCardArray[4][2];				//�����˿�

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

	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
};

//��Ϸ����
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ��λ��
	LONGLONG						lBankerScore;						//ׯ�ҽ��
	LONGLONG						lUserMaxScore;						//�ҵĽ��
	BYTE							cbTimeLeave;						//ʣ��ʱ��	
	bool							bContiueCard;						//��������
	int								nChipRobotCount;					//�������� (��ע������)
};

//�û���ע
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//�û�λ��
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
	BYTE							cbAndroidUser;						//������ʶ
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�˿���Ϣ
	BYTE							cbTableCardArray[4][2];				//�����˿�
	BYTE							cbLeftCardCount;					//�˿���Ŀ

	BYTE							bcFirstCard;
 
	//ׯ����Ϣ
	LONGLONG						lBankerScore;						//ׯ�ҳɼ�
	LONGLONG						lBankerTotallScore;					//ׯ�ҳɼ�
	INT								nBankerTime;						//��ׯ����

	//��ҳɼ�
	LONGLONG						lUserScore;							//��ҳɼ�
	LONGLONG						lUserReturnScore;					//���ػ���

	//ȫ����Ϣ
	LONGLONG						lRevenue;							//��Ϸ˰��
};

struct CMD_C_SendAccount
{
	DWORD							m_UserID[GAME_PLAYER];					//���ID
	DWORD							m_UserCount;							//�������
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_CANCEL_BANKER			3									//ȡ������
#define SUB_C_CONTINUE_CARD			4									//��������
#define SUB_C_MANAGE_CONTROL		5									//ȡ������
#define SUB_C_AMDIN_COMMAND			6									//����Ա����
#define SUB_C_GET_ACCOUNT			7									//��ȡ�ǳ�
#define SUB_C_CHEAK_ACCOUNT			8									//��ȡ�ǳ�

//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE						cbJettonArea;						//��������
	LONGLONG					lJettonScore;						//��ע��Ŀ
};


//////////////////////////////////////////////////////////////////////////

//Ӯ������
struct CMD_S_AndroidArea
{
	bool  bWinShunMen;
	bool  bWinDuiMen;
	bool  bWinDaoMen;
};

struct CMD_S_UserJettonScore
{
	//�����ע
	LONGLONG						lUserJettonScore[AREA_COUNT+1];		//������ע
};

#define ADMIN_COMMAND_TYPE_NONECONTROL           0                      //û�п���
#define ADMIN_COMMAND_TYPE_BANKCONTROL           1                      //ׯ�ҿ���
#define ADMIN_COMMAND_TYPE_AREACONTROL           2                      //�������
struct CMD_C_AdminControl
{
	BYTE                        bCommandType;                           //��������
	BYTE                        bWinArea;                               //Ӯ����
	BYTE                        bBankWin;                               //�Ƿ�ׯ��Ӯ
};

#endif
//
