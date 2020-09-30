#ifndef CMD_LAND_HEAD_FILE
#define CMD_LAND_HEAD_FILE
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//�����궨��
#define KIND_ID						201									//��Ϸ I D
#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("����������")					//��Ϸ����

//�������
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

//��Ϸ״̬
#define GS_WK_FREE					GAME_STATUS_FREE					//�ȴ���ʼ
#define GS_WK_SCORE					GAME_STATUS_PLAY					//�з�״̬
#define GS_WK_PLAYING				GAME_STATUS_PLAY+1					//��Ϸ����

//////////////////////////////////////////////////////////////////////////
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
	LONGLONG						lBaseScore;							//��������

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
	BYTE							bScoreInfo[GAME_PLAYER];			//�з���Ϣ
	BYTE							bCardData[GAME_PLAYER][33];			//�����˿�
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
	BYTE							bBackCard[8];						//�����˿�
	BYTE							bCardData[GAME_PLAYER][33];			//�����˿�
	BYTE							bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[33];					//�����б�
	BYTE							cbMagicCardData[33];			//����˿�
	bool							bUserTrustee[GAME_PLAYER];			//����й�
		
	//��ʷ����
	LONGLONG						lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG						lCollectScore[GAME_PLAYER];			//������Ϣ
};

//�����˿�
struct CMD_S_SendCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[25];						//�˿��б�
};

//�����˿�
struct CMD_S_SendAllCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[GAME_PLAYER][33];			//�˿��б�
	BYTE							bBackCardData[8];					//�����˿�
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
	BYTE							bLandScore;							//��������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bBackCard[8];						//�����˿�
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bCardData[33];						//�˿��б�
};

//��������
struct CMD_S_PassCard
{
	BYTE							bNewTurn;							//һ�ֿ�ʼ
	WORD				 			wPassUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONGLONG						lGameTax;							//��Ϸ˰��
	LONGLONG						lGameScore[GAME_PLAYER];			//��Ϸ����
	BYTE							bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							bCardData[108];						//�˿��б� 
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
	BYTE							bCardData[33];						//�˿��б�
};

//////////////////////////////////////////////////////////////////////////
#pragma pack()

#endif
