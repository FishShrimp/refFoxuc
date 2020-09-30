#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//������

//��Ϸ����
#define KIND_ID						307									//��Ϸ I D
#define GAME_NAME					TEXT("�����齫")					//��Ϸ����

//�������
#define GAME_PLAYER					4									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

//////////////////////////////////////////////////////////////////////////////////
//״̬����

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_CHIP				(GAME_STATUS_PLAY+1)				//���״̬
#define GAME_SCENE_PLAY				(GAME_STATUS_PLAY+2)				//��Ϸ����
//////////////////////////////////////////////////////////////////////////////////

//��������
#define MAX_WEAVE					5									//������
#define MAX_INDEX					42									//�������
#define MAX_COUNT					17									//�����Ŀ
#define MAX_REPERTORY				136									//�����
#define MAX_HUA_CARD				8									//���Ƹ���

//�˿˶���
#define HEAP_FULL_COUNT				34									//����ȫ��

#define MAX_RIGHT_COUNT				3									//���ȨλDWORD����			

//////////////////////////////////////////////////////////////////////////

//�������
struct CMD_WeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbCardData[4];						//�������
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				101									//��������
#define SUB_S_SEND_CARD				102									//�����˿�
#define SUB_S_OPERATE_NOTIFY		104									//������ʾ
#define SUB_S_OPERATE_RESULT		105									//��������
#define SUB_S_GAME_END				106									//��Ϸ����
#define SUB_S_TRUSTEE				107									//�û��й�
#define SUB_S_LISTEN				103									//�û�����
#define SUB_S_REPLACE_CARD			108									//�û�����
#define SUB_S_CHIP					109									//�����Ϣ
#define SUB_S_CHIP_RESULT			110									//��׽��

//����״̬
struct CMD_S_StatusFree
{
	SCORE						lCellScore;							//�������
	WORD							wBankerUser;						//ׯ���û�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�
};

//���״̬
struct CMD_S_StatusChip
{
	SCORE						lCellScore;							//�������
	WORD							wBankerUser;						//ׯ���û�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�
	BYTE							cbLianZhuangCount;					//��ׯ����
	BYTE							bMaiDi[GAME_PLAYER];				//���״̬
	bool							bHaveMaiDi[GAME_PLAYER];			//�Ƿ����
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	SCORE						lCellScore;									//��Ԫ����
	WORD							wBankerUser;								//ׯ���û�
	WORD							wCurrentUser;								//��ǰ�û�
	WORD							wReplaceUser;								//�����滻�û�
	BYTE							cbQuanFeng;									//Ȧ��
	BYTE							cbHuaCardCount[GAME_PLAYER];				//������Ŀ
	BYTE							cbHuaCardData[GAME_PLAYER][8];				//��������
	BYTE							cbHuaCardInfo[GAME_PLAYER][8];				//������Ϣ

	//״̬����
	BYTE							cbActionCard;								//�����˿�
	BYTE							cbActionMask;								//��������
	BYTE							cbHearStatus[GAME_PLAYER];					//����״̬
	BYTE							cbLeftCardCount;							//ʣ����Ŀ
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�
	bool							bTuoPai;									//�Ƿ�����

	//������Ϣ
	WORD							wOutCardUser;								//�����û�
	BYTE							cbOutCardData;								//�����˿�
	BYTE							cbDiscardCount[GAME_PLAYER];				//������Ŀ
	BYTE							cbDiscardCard[GAME_PLAYER][60];				//������¼

	//���Ƽ�¼
	BYTE							cbFengCardData[8];							//���Ƽ�¼
	BYTE							cbFengCardCount;							//���Ƽ�¼

	//�˿�����
	BYTE							cbCardCount;								//�˿���Ŀ
	BYTE							cbCardData[MAX_COUNT];						//�˿��б�
	BYTE							cbSendCardData;								//�����˿�

	//����˿�
	BYTE							cbWeaveCount[GAME_PLAYER];					//�����Ŀ
	CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//����˿�

	//������Ϣ
	WORD							wHeapHead;									//����ͷ��
	WORD							wHeapTail;									//����β��
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//������Ϣ

	//�������
	BYTE							bMagicIndex;								//������ֵ
	WORD							wMagicPos;									//����λ
	BYTE							cbHeapMagic;								//�����λ	(��β������
};

//�����Ϣ
struct CMD_S_Chip
{
	WORD							wBankerUser;								//ׯ��λ��
	BYTE							cbLianZhuangCount;							//��ׯ����
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	LONG							lSiceCount;									//���ӵ���
	WORD							wBankerUser;								//ׯ���û�
	WORD							wCurrentUser;								//��ǰ�û�
	WORD							wReplaceUser;								//�����û�
	BYTE							cbUserAction;								//�û�����
	BYTE							cbCardData[MAX_COUNT];						//�˿��б�
	BYTE							cbQuanFeng;									//Ȧ��
	WORD							wHeapHead;									//������ͷ
	WORD							wHeapTail;									//������β
	BYTE							cbHeapCardInfo[GAME_PLAYER][2];				//������Ϣ
	BYTE							bMagicIndex;								//������ֵ						
	WORD							wMagicPos;									//����λ
	BYTE							cbHeapMagic;								//�����λ	(��β������
	BYTE							cbLianZhuangCount;							//��ׯ����
};

//��������
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
};

//�����˿�
struct CMD_S_SendCard
{
	BYTE							cbCardData;							//�˿�����
	BYTE							cbActionMask;						//��������
	WORD							wCurrentUser;						//��ǰ�û�
	WORD							wReplaceUser;						//�����û�
	WORD							wSendCardUser;						//�����û�
	bool							bTail;								//ĩβ����
};

//������ʾ
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	BYTE							cbActionMask;						//��������
	BYTE							cbActionCard;						//�����˿�
};

//��������
struct CMD_S_OperateResult
{
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard[3];					//�����˿�
	bool							bTuoPai;							//�Ƿ�����
};

//��Ϸ����
struct CMD_S_GameEnd
{
	SCORE						lGameTax;							//��Ϸ˰��
	//������Ϣ
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbProvideCard;						//��Ӧ�˿�
	DWORD							dwChiHuKind[GAME_PLAYER];			//��������
	DWORD							dwChiHuRight[MAX_RIGHT_COUNT];		//��������
	BYTE							cbHuaCardCount;						//���Ƹ���
	BYTE							cbFanCount;							//�ܷ���

	//������Ϣ
	SCORE						lGameScore[GAME_PLAYER];			//��Ϸ����

	//�˿���Ϣ
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//�˿�����
};

//�û��й�
struct CMD_S_Trustee
{
	bool							bTrustee;							//�Ƿ��й�
	WORD							wChairID;							//�й��û�
};

//�û�����
struct CMD_S_Listen
{
	WORD							wChairId;							//�����û�
};

//��������
struct CMD_S_ReplaceCard
{
	WORD							wReplaceUser;						//�����û�
	BYTE							cbReplaceCard;						//�����˿�
};

//��׽��
struct CMD_S_Chip_Result
{
	bool							bTotal;								//�Ƿ�ȫ��
	WORD							wMaiDiUser;							//����û�
	BYTE							bMaidi[GAME_PLAYER];				//�������
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_OPERATE_CARD			3									//�����˿�
#define SUB_C_TRUSTEE				4									//�û��й�
#define SUB_C_LISTEN				2									//�û�����
#define SUB_C_REPLACE_CARD			5									//�û�����
#define SUB_C_CHIP					6									//�û����

//��������
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//�˿�����
};

//��������
struct CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard[3];					//�����˿�
};

//�û�����
struct CMD_C_Listen
{
	BYTE							cbListen;							//�����û�
};

//�û��й�
struct CMD_C_Trustee
{
	bool							bTrustee;							//�Ƿ��й�	
};

//��������
struct CMD_C_ReplaceCard
{
	BYTE							cbCardData;							//�˿�����
};

//�����Ϣ
struct CMD_C_Chip
{
	BYTE							bMaiDi;								//�������
};

//////////////////////////////////////////////////////////////////////////

#endif