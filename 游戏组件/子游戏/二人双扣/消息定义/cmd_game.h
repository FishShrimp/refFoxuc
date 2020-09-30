#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//������
//��Ϸ����
#ifdef VIDEO_GAME
#define KIND_ID						208										//��Ϸ I D
#define GAME_NAME					TEXT("��ƴ˫��"	)						//��Ϸ����
#define GAME_PLAYER					2									//��Ϸ����
#else
#define KIND_ID						208								//��Ϸ I D
#define GAME_NAME					TEXT("��ƴ˫��"	)						//��Ϸ����
#define GAME_PLAYER					2									//��Ϸ����
#endif

#define VERSION_SERVER			    	PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				    PROCESS_VERSION(6,0,3)				//����汾

//////////////////////////////////////////////////////////////////////////
//��Ŀ����

#define MAX_COUNT					27									//�����Ŀ
#define FULL_COUNT					108									//ȫ����Ŀ
#define TYPE_COUNT					11									//������Ŀ
#define CARD_TYPE_COUNT				15									//��ֵ����
#define BOMB_TYPE_COUNT				9									//ը������
//////////////////////////////////////////////////////////////////////////

//��ֵ����
#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����


//////////////////////////////////////////////////////////////////////////
//״̬����

//��Ϸ״̬
#define GS_SK_FREE					GAME_STATUS_FREE								//�ȴ���ʼ
#define GS_SK_PLAYING				GAME_STATUS_PLAY							//��Ϸ״̬


//�ṹ�嶨��
//�����ṹ
struct tagAnalyseResult
{
	//ԭʼ����
	BYTE  m_cbCardCount;
	//���ͷ���
	struct tagCardStyle
	{
		BYTE  m_cbCard;
		BYTE  m_cbCount;
	}m_CardStyle[CARD_TYPE_COUNT];          //ֽ�ƹ���

	int  m_nUnEqualCard;					//��������
	int  m_nJockerCount;					//��������

	//�������
	BYTE  m_cbCardType;						//ȡ������

	BYTE  m_cbMaxIndex;						//��ʼ����
	BYTE  m_cbMinIndex;						//��ֹ����
			#define  NORINDEX_BEGIN 0		//��������ʼ����
			#define  NORINDEX_END	12		//��������ֹ����
	int   m_nMaxCount;						//���������

	//ը����Ϣ
	BYTE  m_cbBombGrade;					//ը���ȼ�
};
//////////////////////////////////////////////////////////////////////////
//�����

#define SUB_S_GAME_START			100									//��Ϸ����
#define SUB_S_OUT_CARD				101									//�û�����
#define SUB_S_PASS_CARD				102									//�û�����
#define SUB_S_GAME_END				103									//��Ϸ����
#define SUB_S_TRUSTEE				104									//�û��й�
#define SUB_S_PLAYVOICE             106                                 //��������

struct CMD_S_PlayVoice
{
	WORD  wChairId;					//�������
	BYTE  type;						//��������
	BYTE  VoiceId;					//�������
};

//����״̬
struct CMD_S_StatusFree
{
	//ׯ���û�
	WORD							wBankerUser;						//ׯ���û�
	//��ʷ����
	LONGLONG                        lAllTurnScore;						//�ֵܾ÷�
	LONGLONG                        lLastTurnScore;						//�Ͼֵ÷�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONGLONG						lCellScore[3];						//��Ԫ����
	WORD							wCurrentUser;						//��ǰ���

	//��ʷ����
	LONGLONG                        lAllTurnScore[GAME_PLAYER];			//�ֵܾ÷�
	LONGLONG                        lLastTurnScore[GAME_PLAYER];		//�Ͼֵ÷�

	//ׯ���û�
	WORD							wBankerUser;						//ׯ���û�

	//��Ӧ��־
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�

	//������Ϣ
	WORD							wTurnWiner;							   //ʤ�����
	BYTE							cbTurnCardCount;					   //������Ŀ
	BYTE							cbTurnCardData[MAX_COUNT];			   //��������
	BYTE							cbOutCardCount[GAME_PLAYER];		   //������Ŀ
	BYTE							cbOutCardData[GAME_PLAYER][MAX_COUNT]; //�����б�
	tagAnalyseResult				LastOutCard;						   //������

	//�˿���Ϣ
	BYTE							cbHandCardData[MAX_COUNT];			//�����˿�
	BYTE							cbHandCardCount[GAME_PLAYER];		//�˿���Ŀ
};

//�����˿�
struct CMD_S_GameStart
{
	WORD				 			wCurrentUser;						//��ǰ���
	LONGLONG						lCellScore[3];						//��Ԫ����
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�
	BYTE							cbStartRandCard;					//����˿�
	WORD							wGetRandCardID;						//�����û�
	BYTE							cbGetCardPosition;					//����λ��	
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�
};

//��������
struct CMD_S_PassCard
{
	BYTE							cbTurnOver;							//һ�ֽ���
	WORD				 			wCurrentUser;						//��ǰ���
	WORD				 			wPassCardUser;						//�������
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//��Ϸ�ɼ�
	LONGLONG						lGameScore[GAME_PLAYER];			//��Ϸ����
	LONGLONG						lAllTurnScore[GAME_PLAYER];			//�ֵܾ÷�
	LONGLONG						lLastTurnScore[GAME_PLAYER];		//�Ͼֵ÷�	
	LONGLONG						lGameTax;							//��Ϸ˰��
	//�÷���ϸ
	LONGLONG						lBombScore[GAME_PLAYER];										//ը���÷�
	BYTE							cbBombList[GAME_PLAYER][BOMB_TYPE_COUNT];		//ը������

	//�˿���Ϣ
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//�˿��б�
};
//�û��й�
struct CMD_S_Trustee
{
	bool							bTrustee;							//�Ƿ��й�
	WORD							wChairID;							//�й��û�
};
//////////////////////////////////////////////////////////////////////////
//�����

#define SUB_C_OUT_CARD				1									//�û�����
#define SUB_C_PASS_CARD				2									//�û�����
#define SUB_C_TRUSTEE				5									//�û��й�
#define SUB_C_PLAYVOICE             6									//��������
#define SUB_C_REQ_HUNTERS			7									//���볭��

struct CMD_C_PlayVoice
{
	BYTE  type;
	BYTE  VoiceId;
};

//�û�����
struct CMD_C_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	BYTE							cbCardData[MAX_COUNT];				//�˿�����
};
//�û��й�
struct CMD_C_Trustee
{
	bool							bTrustee;							//�Ƿ��й�	
};

//////////////////////////////////////////////////////////////////////////

#endif