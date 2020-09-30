#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��


#define KIND_ID							11									//��Ϸ I D
#define GAME_PLAYER						5									//��Ϸ����
#define GAME_NAME						TEXT("����")						//��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����
#define MAX_COUNT						4									//�����Ŀ

//����ԭ��						
#define GER_NO_PLAYER						0x10							//û�����

//��Ϸ״̬						
#define GS_TK_FREE						GAME_STATUS_FREE					//�ȴ���ʼ
#define GS_TK_INVEST					GAME_STATUS_PLAY					//�±�Ǯע
#define GS_TK_SCORE						GAME_STATUS_PLAY+1					//��ע״̬
#define GS_TK_OPEN_CARD					GAME_STATUS_PLAY+2				    //��Ϸ����

#define VERSION_SERVER					PROCESS_VERSION(6,0,3)			    //����汾
#define VERSION_CLIENT					PROCESS_VERSION(6,0,3)			    //����汾

//����ԭ��						
#define END_REASON_EXIT					1									//ǿ�˽���
#define END_REASON_PASS					2									//���ƽ���
#define END_REASON_NORMAL				3									//��������
#define END_REASON_GIVEUP				4									//��������

//////////////////////////////////////////////////////////////////////////

#define JETTON_COUNT					8								//�������
#define LAYER_HEIGHT					4								//����߶�
#define JETTON_SPECE					3								//������
#define CONTROL_COUNT					4								//�ؼ�����

//////////////////////////////////////////////////////////////////////////

//����������ṹ
#define SUB_S_GAME_START				101									//��Ϸ��ʼ
#define SUB_S_USER_INVEST				107									//�û��±�
#define SUB_S_ADD_SCORE					102									//��ע���
#define SUB_S_SEND_CARD					103									//������Ϣ
#define SUB_S_GAME_END					104									//��Ϸ����
#define SUB_S_OPEN_START				115									//��ʼ����
#define SUB_S_OPEN_CARD					105									//�û�����
#define SUB_S_GIVE_UP					106									//�û�����


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

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONGLONG							lCellScore;							//��������
};

//��Ϸ״̬
struct CMD_S_StatusInvest
{
	WORD								wBankerUser;						//ׯ���û�

	BYTE								cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	bool								bInvestFinish[GAME_PLAYER];			//��ɱ�־

	LONGLONG							lCellScore;							//��������
	LONGLONG							lUserScore[GAME_PLAYER];			//�û�����
};

//��Ϸ״̬
struct CMD_S_StatusScore
{
	WORD								wCurrentUser;						//��ǰ�û�
	WORD								wBankerUser;						//ׯ���û�

	BYTE								cbHandCard[2];						//�û��˿�
	BYTE								cbMingCard[GAME_PLAYER][2];			//�û��˿�
	BYTE								cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬

	LONGLONG							lCellScore;							//��������
	LONGLONG							lUserScore[GAME_PLAYER];			//�û�����
	LONGLONG							lTotalScore[GAME_PLAYER];			//��ע��Ŀ
	LONGLONG							lTurnMaxScore;						//�����ע
	LONGLONG							lTurnMinScore;						//��С��ע
};

//��Ϸ״̬
struct CMD_S_StatusOpenCard
{
	WORD								wBankerUser;						//ׯ���û�

	BYTE								cbOpenFinish[GAME_PLAYER];			//��ɱ�־
	BYTE								cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬

	BYTE								cbHandCard[2];						//�û��˿�
	BYTE								cbMingCard[GAME_PLAYER][2];			//�û��˿�

	LONGLONG							lCellScore;							//��������
	LONGLONG							lUserScore[GAME_PLAYER];			//�û�����
	LONGLONG							lTotalScore[GAME_PLAYER];			//��ע��Ŀ
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD								wBankerUser;						//ׯ���û�
	LONGLONG							lUserScore[GAME_PLAYER];			//�û�����
};

//�û��±�
struct CMD_S_UserInvest
{
	WORD								wChairID;							//�û����
	LONGLONG							lScore;								//�û�����	
};

//�û�����
struct CMD_S_GiveUp
{
	WORD								wGameStatus;						//��Ϸ״̬
	WORD								wGiveUpUser;						//�����û�
	LONGLONG							lLostScore;							//������	
};

//�û���ע
struct CMD_S_AddScore
{
	WORD								wAddScoreUser;						//��ע�û�
	WORD								wCurrentUser;						//��ǰ�û�
	LONGLONG							lAddScoreCount;						//��ע��Ŀ
	LONGLONG							lTurnMaxScore;						//�����ע
	LONGLONG							lTurnMinScore;						//��С��ע
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONGLONG							lGameTax[GAME_PLAYER];				//��Ϸ˰��
	LONGLONG							lGameScore[GAME_PLAYER];			//��Ϸ�÷�
	BYTE								cbCardData[GAME_PLAYER][MAX_COUNT];	//�û��˿�
	BYTE								cbEndReason;						//����ԭ��
};

//�������ݰ�
struct CMD_S_SendCard
{
	BYTE								cbCardCount;						//�˿���Ŀ
	WORD								wCurrentUser;						//��ǰ�û�
	BYTE								cbHandCard[2];						//�û��˿�
	BYTE								cbMingCard[GAME_PLAYER][2];			//�û��˿�
	//bool								bStartOpen;							//��ʼ����
};

//�û�̯��
struct CMD_S_Open_Card
{
	WORD								wChairID;							//̯���û�
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_USER_INVEST				1									//�û��±�
#define SUB_C_ADD_SCORE					2									//�û���ע
#define SUB_C_OPEN_CARD					3									//�û�̯��
#define SUB_C_GIVE_UP					4									//�û�����

//�û���ׯ
struct CMD_C_UserInvest
{
	LONGLONG							lInvestGold;						//�±���Ŀ
};

//�û���ע
struct CMD_C_AddScore
{
	LONGLONG							lScore;								//��ע��Ŀ
};

//�û�����
struct CMD_C_OpenCard
{
	BYTE								cbCardData[MAX_COUNT];				//�û��˿�
};

//////////////////////////////////////////////////////////////////////////

#endif
