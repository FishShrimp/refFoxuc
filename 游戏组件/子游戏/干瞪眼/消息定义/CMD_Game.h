#ifndef CMD_PAIJIU_HEAD_FILE
#define CMD_PAIJIU_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��
#define KIND_ID							226									//��Ϸ I D
#define GAME_NAME						TEXT("�ɵ���")						//��Ϸ����
#define GAME_PLAYER						6									//��Ϸ����
#define MAX_COUNT						27									//�����Ŀ

#define VERSION_SERVER			    	PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				    PROCESS_VERSION(6,0,3)				//����汾

#define HAND_CARD_COUNT					2									//������Ŀ

//��Ϸ״̬
#define GS_WK_FREE				    	GAME_STATUS_FREE					//����״̬
#define GS_WK_SCORE						GAME_STATUS_PLAY					//��Ϸ״̬
#define GS_WK_PLAYING					GAME_STATUS_PLAY+1					//�ȴ�״̬
/////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_SEND_CARD				100									//��������
#define SUB_S_LAND_SCORE			101									//�з�����
#define SUB_S_GAME_START			102									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				103									//�û�����
#define SUB_S_PASS_CARD				104									//��������
#define SUB_S_GAME_END				105									//��Ϸ����

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONGLONG							lBaseScore;							//��������

	//��ʷ����
	LONGLONG						lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG						lCollectScore[GAME_PLAYER];			//������Ϣ
};

//��Ϸ״̬
struct CMD_S_StatusScore
{
	BYTE							bLandScore;							//��������
	LONGLONG						lBaseScore;							//��������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bScoreInfo[3];						//�з���Ϣ
	BYTE							bCardData[3][20];					//�����˿�
	bool							bUserTrustee[GAME_PLAYER];			//����й�

	//��ʷ����
	LONGLONG						lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG						lCollectScore[GAME_PLAYER];			//������Ϣ
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD							wLandUser;							//�������
	WORD							wBombTime;							//ը������
	LONGLONG						lBaseScore;							//��������
	BYTE							bLandScore;							//��������
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bBackCard[3];						//�����˿�
	BYTE							bCardData[GAME_PLAYER][MAX_COUNT];	//�����˿�
	BYTE							bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[MAX_COUNT];			//�����б�
	BYTE							bMagicData[MAX_COUNT];
	bool							bUserTrustee[GAME_PLAYER];			//����й�
	BYTE							bLeftCardCount;

	//��ʷ����
	LONGLONG						lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG						lCollectScore[GAME_PLAYER];			//������Ϣ
};

//�����˿�
struct CMD_S_SendCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[17];						//�˿��б�
};

//�����˿�
struct CMD_S_SendAllCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[GAME_PLAYER][MAX_COUNT];			//�˿��б�
	BYTE							bBackCardData[3];					//�����˿�
};

//�û��з�
struct CMD_S_LandScore
{
	WORD							bLandUser;							//�з����
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bLandScore;							//�ϴνз�
	BYTE							bCurrentScore;						//��ǰ�з�
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD				 			wLandUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[MAX_COUNT];
	BYTE							bCardCount;
	BYTE							bLeftCardCount;
	BYTE							bUserCardCount[GAME_PLAYER];
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bUserCardCount[GAME_PLAYER];
	BYTE							bMagicData[MAX_COUNT];
	BYTE							bCardData[MAX_COUNT];				//�˿��б�
};

//��������
struct CMD_S_PassCard
{
	BYTE							bNewTurn;							//һ�ֿ�ʼ
	bool							bCanOutSingleKing;
	WORD				 			wPassUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bUserCardCount[GAME_PLAYER];
	BYTE							bCardData[GAME_PLAYER][MAX_COUNT];
	BYTE							bLeftCardCount;
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONGLONG							lGameTax;							//��Ϸ˰��
	LONGLONG							lGameScore[GAME_PLAYER];			//��Ϸ����
	BYTE							bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							bCardData[162];						//�˿��б� 
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_LAND_SCORE			1									//�û��з�
#define SUB_C_OUT_CART				2									//�û�����
#define SUB_C_PASS_CARD				3									//��������
#define SUB_C_TRUSTEE				4									//�й���Ϣ

//�йܽṹ
struct CMD_C_UserTrustee {
	WORD							wUserChairID;						//�������
	bool							bTrustee;							//�йܱ�ʶ
};

//�û��з�
struct CMD_C_LandScore
{
	BYTE							bLandScore;							//��������
};

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[MAX_COUNT];						//�˿��б�
};

//////////////////////////////////////////////////////////////////////////

#endif
