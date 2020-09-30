#ifndef CMD_RUNFAST_HEAD_FILE
#define CMD_RUNFAST_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						15									//��Ϸ I D
#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("ʮ���")						//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_GOLD)					//��Ϸ����

#define MAX_COUNT					9									//�����Ŀ
#define FULL_COUNT					54									//

#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

//���Ͷ���
#define CT_ERROR					0									//��������
#define CT_WU_XIAO					11									//��С
#define CT_REN_WU_XIAO				12									//����С
#define CT_TIAN_WANG				13									//����
#define CT_LIU_XIAO					14									//��С
#define CT_QI_XIAO					15									//��С
#define CT_BA_XIAO					16									//��С
#define CT_JIU_XIAO					17									//��С
#define CT_DA_TIAN_WANG				18									//������

//��Ϸ״̬
#define GS_TH_FREE					GAME_STATUS_FREE								//����״̬
#define GS_TH_SCORE					GAME_STATUS_PLAY							//��ע״̬
#define GS_TH_PLAY					(GAME_STATUS_PLAY+1)						//��Ϸ״̬

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			101									//��Ϸ��ʼ
#define SUB_S_GAME_END				102									//��Ϸ����
#define SUB_S_ADD_SCORE				103									//��Ҳ�������
#define SUB_S_SEND_CARD				104									//��������
#define SUB_S_GIVE_UP				106									//ͣ������
#define SUB_S_USER_LEFT				105									//�������
#define SUB_S_GAME_PLAY				107									//��Ϸ��ʼ

//����״̬
struct CMD_S_StatusFree
{
	//��Ԫ����
	LONGLONG						lCellScore;							//��Ԫ����
};

//��ע״̬
struct CMD_S_StatusScore
{
	//��Ԫ����
	LONGLONG						lCellScore;							//��Ԫ����

	//״̬����
	WORD							wBankerUser;						//ׯ��
	BYTE							cbHadScore;							//����ע
	bool							bUserStatus[GAME_PLAYER];			//�û�״̬
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ԫ����
	LONGLONG						lCellScore;							//��Ԫ����

	//״̬����
	WORD							wCurrentUser;						//��ǰ���
	WORD							wBankerUser;						//ׯ��
	BYTE							byUserStatus[GAME_PLAYER];			//�û�״̬

	//��ע����
	LONGLONG						lTableScore[GAME_PLAYER];			//��ע

	//�˿˱���
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbHandCardData[GAME_PLAYER][MAX_COUNT];		//�����˿�
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	//��Ԫ����
	LONGLONG						lCellScore;							//��Ԫ����

	//״̬����
	WORD							wBankerUser;						//ׯ��
};

//��Ϸ��ʼ
struct CMD_S_GamePlay
{
	//״̬����
	WORD							wCurrentUser;						//��ǰ���

	//�˿˱���
	BYTE							byCardData[GAME_PLAYER];			//����

	//��ע����
	LONGLONG						lTableScore[GAME_PLAYER];			//��ע
};

//��Ҳ���
struct CMD_S_AddScore
{
	WORD							wAddScoreUser;						//�������
	WORD							wCurrentUser;						//��ǰ���
	BYTE							cbCardData;							//�����
};

//�����˿�
struct CMD_S_SendCard
{
	//��Ϸ��Ϣ
	WORD							wCurrentUser;						//��ǰ���
	WORD							wSendCardUser;						//�������
	BYTE							cbCardData;							//�û��˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE							byCardData[GAME_PLAYER];			//��������
	LONGLONG						lGameScore[GAME_PLAYER];			//��ҵ÷�
};

//ͣ��
struct CMD_S_GiveUp
{
	WORD							wGiveUpUser;						//ͣ�����
	WORD							wCurrentUser;						//��ǰ���
};

//�������
struct CMD_S_UserLeft
{
	WORD							wLeftUser;							//�������
	WORD							wCurrentUser;						//��ǰ���
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_ADD_SCORE			1									//�ӱ�
#define SUB_C_GIVE_UP			2									//ͣ��
#define SUB_C_GIVE_CARD			3									//Ҫ��
#define SUB_C_SCORE				4									//��ע

//////////////////////////////////////////////////////////////////////////

//��ע
struct CMD_C_Score
{
	LONGLONG						lScore;								//��ע��
};

//////////////////////////////////////////////////////////////////////////

#endif