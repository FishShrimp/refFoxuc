#ifndef CMD_HK_FIVE_CARD_HEAD_FILE
#define CMD_HK_FIVE_CARD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							1000								//��Ϸ I D
#define GAME_PLAYER						5									//��Ϸ����
#define GAME_NAME						TEXT("��ť����")					//��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����

#define MAX_COUNT						5									//�����Ŀ
#define VERSION_SERVER					PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT					PROCESS_VERSION(6,0,3)				//����汾
//����ԭ��
#define GER_NO_PLAYER					0x10								//û�����

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START				100									//��Ϸ��ʼ
#define SUB_S_ADD_SCORE					101									//��ע���
#define SUB_S_GIVE_UP					102									//������ע
#define SUB_S_SEND_CARD					103									//������Ϣ
#define SUB_S_GAME_END					104									//��Ϸ����

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONGLONG							lCellScore;							//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��־����
	BYTE								bShowHand;							//�����־

	//��ע��Ϣ
	LONGLONG							lCellScore;							//��Ԫ��ע
	LONGLONG							lTurnMaxScore;						//�����ע
	LONGLONG							lTurnLessScore;						//��С��ע
	LONGLONG							lShowHandScore;						//

	//״̬��Ϣ
	WORD				 				wCurrentUser;						//��ǰ���
	BYTE								cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	LONGLONG							lTableScore[GAME_PLAYER*2];			//��ע��Ŀ

	//�˿���Ϣ
	BYTE								cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE								cbHandCardData[GAME_PLAYER][5];		//�����˿�
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	//��ע��Ϣ
	LONGLONG							lCellScore;							//��Ԫ��ע
	LONGLONG							lTurnMaxScore;						//�����ע
	LONGLONG							lTurnLessScore;						//��С��ע
	LONGLONG							lShowHandScore;						//

	//�û���Ϣ
	WORD				 				wCurrentUser;						//��ǰ���

	//�˿�����
	BYTE								cbObscureCard;						//�����˿�
	BYTE								cbCardData[GAME_PLAYER];			//�û��˿�
};

//�û���ע
struct CMD_S_AddScore
{
	WORD								wCurrentUser;						//��ǰ�û�
	WORD								wAddScoreUser;						//��ע�û�
	LONGLONG							lAddScoreCount;						//��ע��Ŀ
	LONGLONG							lTurnLessScore;						//���ټ�ע
	bool								bShowHand;							//�Ƿ����
};

//�û�����
struct CMD_S_GiveUp
{
	WORD								wGiveUpUser;						//�����û�
	LONGLONG							lTurnMaxScore;						//
};

//�������ݰ�
struct CMD_S_SendCard
{
	WORD								wCurrentUser;						//��ǰ�û�
	WORD								wLastMostUser;						//�ϴ�����û�
	LONGLONG							lTurnMaxScore;						//�����ע
	LONGLONG							lTurnLessScore;						//��С��ע
	BYTE								cbSendCardCount;					//������Ŀ
	BYTE								cbCardData[GAME_PLAYER][2];			//�û��˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONGLONG							lGameTax[GAME_PLAYER];				//��Ϸ˰��
	LONGLONG							lGameScore[GAME_PLAYER];			//��Ϸ�÷�
	BYTE								cbCardData[GAME_PLAYER];			//�û��˿�
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_ADD_SCORE					1									//�û���ע
#define SUB_C_GIVE_UP					2									//������ע

//�û���ע
struct CMD_C_AddScore
{
	LONGLONG							lScore;								//��ע��Ŀ
};

//////////////////////////////////////////////////////////////////////////

#endif
