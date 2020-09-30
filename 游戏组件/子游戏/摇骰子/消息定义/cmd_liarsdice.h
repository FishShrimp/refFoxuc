#ifndef CMD_LIARSDICE_HEAD_FILE
#define CMD_LIARSDICE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							13									//��Ϸ I D
#define GAME_PLAYER						2									//��Ϸ����
#define GAME_NAME						TEXT("ҡ����")					    //��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����
#define HAND_DICE_COUNT					5									//������Ŀ

#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				    //����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				    //����汾

//��Ϸ״̬
#define GS_WK_FREE				    	GAME_STATUS_FREE					//�ȴ���ʼ
#define GS_WK_SETCHIP					GAME_STATUS_PLAY+1					//�����ע
#define GS_WK_THROWDICE			    	GAME_STATUS_PLAY+2					//���ҡ��
#define GS_WK_PLAYING			    	GAME_STATUS_PLAY+3					//��Ϸ����
#define GS_WK_OPENDICE					GAME_STATUS_PLAY+4					//��ҿ���

//�ṹ�嶨��

//�û�����
struct	tagDiceYell
{
	BYTE	bDiceCount;		//������Ŀ
	BYTE	bDicePoint;		//���ӵ���
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ
#define SUB_S_START_CHIP				100									//��ʼ��ע
#define SUB_S_CHIP_RESULT				101									//��ע���
#define SUB_S_SEND_DICE					102									//������ֵ
#define SUB_S_THROW_DICE				103									//���ҡ��
#define SUB_S_YELL_DICE					104									//��Һ���
#define SUB_S_YELL_RESULT				105									//�������
#define SUB_S_OPEN_DICE					106									//��ҿ���
#define SUB_S_GAME_START				107									//��Ϸ��ʼ
#define SUB_S_GAME_END					108									//��Ϸ����
//////////////////////////////////////////////////////////////////////////

//��Ϸ״̬
struct CMD_S_StatusFree
{
	WORD							wLeftTimer;					
};


//��ʼ��ע
struct CMD_S_Chip_Start
{
	WORD			wUserID;							//����κ�
	LONGLONG		lMaxChip;							//�����
};

//��ע���
struct CMD_S_Chip_Result
{
	WORD			wUserID;							//����κ�
	LONGLONG		lChip;								//��ע���
};

//������ֵ
struct CMD_S_SendDice
{
	WORD			wCurUser;							//�û��κ�
	BYTE			bDiceData[HAND_DICE_COUNT];			//�û���ֵ
};

//���ҡ��
struct CMD_S_ThrowDice
{
	WORD			wThrowUser;							//�û��κ�
};

//��Һ���
struct CMD_S_YellDice
{
	WORD			wCurUser;							//��ǰ�û�
	WORD			wGameTurn;							//��Ϸ����
	tagDiceYell		maxYell;							//��󺰻�
};

//�������
struct CMD_S_YellDice_Result
{
	WORD			wYellUser;							//�����û�
	tagDiceYell		curYell;							//��������				
};

//��ҿ���
struct CMD_S_OpenDice
{
	WORD			wOpenUser;							//�����û�
	BYTE			bDiceData[2][HAND_DICE_COUNT];		//�û���ֵ
	BYTE			bDicePoint;							//��������
	int				nDiceOpenCount;						//������Ŀ
	int				nDiceRealCount;						//ʵ����Ŀ 
	bool			bOpenRight;							//�Ƿ񿪶�	(�����ֲ´���Ŀ)
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONGLONG		lGameTax;							//��Ϸ˰��
	LONGLONG		lGameScore[GAME_PLAYER];			//��Ϸ����
	BYTE			bEndMode;							//������ʽ
};

//��������
//��ע״̬
struct CMD_S_StatusSetChip
{
	WORD			wChipUser;							//��ע���
	LONGLONG		lMaxChip;							//�����ע
};

//ҡ��״̬
struct CMD_S_StatusThrow
{
	BYTE			bDiceData[HAND_DICE_COUNT];			//�û���ֵ
	LONGLONG		lChip;								//��ע���
	bool			bHaveThrow[GAME_PLAYER];			//�Ƿ�ҡ��
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD			wCurUser;							//��ǰ���
	BYTE			bDiceData[HAND_DICE_COUNT];			//�û���ֵ
	LONGLONG		lChip;								//��ע���
	WORD			wGameTurn;							//��Ϸ����
	tagDiceYell		userYell[GAME_PLAYER];				//�û�����
};

//����״̬
struct CMD_S_StatusOpen
{
	WORD			wOpenUser;							//�����û�
	BYTE			bDiceData[2][HAND_DICE_COUNT];		//�û���ֵ
	LONGLONG		lChip;								//��ע���
	tagDiceYell		curYell;							//��ǰ����
	int				nDiceRealCount;						//ʵ����Ŀ 
	bool			bOpenRight;							//�Ƿ񿪶�	(�����ֲ´���Ŀ)
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_THROWDICE					300									//ҡ����ʼ
#define SUB_C_THROWFINISH				301									//ҡ�����
#define SUB_C_CHIP						302									//��ע����
#define SUB_C_YELLDICE					303									//��������
#define SUB_C_OPENDICE					304									//��������
#define SUB_C_COMPLETEOPEN				305									//�������	(������Ϣ��ʾ���)

//�����ע
struct CMD_C_Chip
{
	WORD			wUserID;							//����κ�
	LONGLONG		lChip;								//��ע���
};

//��Һ���
struct CMD_C_YellDice
{
	WORD			wUserID;							//����κ�
	tagDiceYell		curYell;							//��ǰ����
};

//�������� (�˳����Ƕ�����������)
struct OpenDice_Scene
{
	WORD wOpenUser;		//�������
	BYTE bRealDiceCount;//ʵ����Ŀ
	BYTE bDicePoint;	//���ĵ���
	bool bOpenRight;	//�Ƿ���
	BYTE bODTime;		//����ʱ��
	void Init()
	{
		wOpenUser=0;
		bRealDiceCount = 0;
		bDicePoint = 0;
		bOpenRight = false;
		bODTime = 0;
	}
};

//////////////////////////////////////////////////////////////////////////

#endif
