#ifndef CMD_LAND_HEAD_FILE
#define CMD_LAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//��Ŀ����

//��Ϸ��ʱ��
#define IDI_GAME_START					11								//��Ϸ��ʼ
#define IDI_SET_BASEBALL     			17								//����ĸ��
#define IDI_HIT_BALL                    18                              //����
#define IDI_WAIT_SENDRESULT             19                              //�ȴ����ͻ�������ʱ��

 #define	TIME_GAME_START			(30*1000)							//��Ϸ��ʼ
 #define	TIME_SET_BASEBALL		(10*1000)							//����ĸ��
 #define TIME_HIT_BALL          (20*1000)							//����
 #define TIME_WAIT_SENDRESULT  (40*1000)                            //�ȴ����ͻ�������ʱ��

//�����궨��
#define KIND_ID						188							    	//��Ϸ I D
#define GAME_PLAYER					2									//��Ϸ����
#define GAME_NAME					TEXT("��ʽ8��")						//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)					//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)					//����汾
#define GAME_GENRE					(GAME_GENRE_GOLD|GAME_GENRE_MATCH)		//��Ϸ����
#define BALL_NUM                    16

//��Ϸ״̬
#define GS_WK_FREE					GAME_STATUS_FREE								//�ȴ���ʼ
#define GS_WK_SET_SETBASEBALL       GAME_STATUS_FREE+1                           //����ĸ��
#define GS_WK_WAIT_HITBALL          GAME_STATUS_FREE+2                           //�ȴ�����
#define GS_WK_PLAYING				GAME_STATUS_PLAY+4						//��Ϸ����
#define MAX_DOUBLE					200									//��Ϸ����
//////////////////////////////////////////////////////////////////////////
//����������ṹ


#define SUB_S_GAME_START			104									//��Ϸ��ʼ
#define SUB_S_GAME_END				107									//��Ϸ����
#define SUB_S_GAME_CONFIG			115									//����
#define SUB_S_HITBALL               116                                 //����
#define SUB_S_SETBASEBALL           117                                 //����ĸ��
#define SUB_S_END_SETBASEBALL       118                                 //����ĸ�����
#define SUB_S_END_SETBASEBALL_TIMEOUT  119                              //����ĸ���������ʱ
#define SUB_S_CLUBPOS               120                                 //���
#define SUB_S_HIT_RESULT            121                                 //������
#define SUB_S_HIT_TIMEOUT           122                                 //����ʱ
#define SUB_S_GIVEUP                123                                 //����
#define SUB_S_STARTHITBALL          124                                 //����
 
 
struct CMD_S_HIT_TIMEOUT
{
	WORD currentUser;
	int  timeOutCount;
};
 
//��Ϸ״̬
struct CMD_S_StatusFree
{
	WORD							wServerType;						//����������
};
struct PT
{
	float x;
	float y;
};
//��Ϸ������
struct CMD_S_StatusGamePlay
{
	BYTE      currentUser;		      //��ǰ�����	
	PT		  ptBall[BALL_NUM];       //ÿ�����λ��
	BOOL	  ballState[BALL_NUM];    //���Ƿ�ɼ�
	BYTE      inHoleBall[2][8];       //��������
	BYTE      maxComob[2];               //���������
	BYTE      userBallType[2];        //��С��
	BYTE      gameStatus;             //��ǰ��״̬
	__int64   baseScore;              //�ͷ�
}; 
//��Ϸ��ʼ
struct CMD_S_GameStart
{
	__int64                        nBaseScore;
	BYTE                           currentUser;
	BYTE                           roundCount;
};
//����ĸ��
struct CMD_S_SET_BASEBALL
{
	float mousex;
	float mousey;
};
//���ý���
struct CMD_S_END_SET_BASEBALL
{
	float ballPosx;
	float ballPosy;
};
struct CMD_S_END_SET_BASEBALL_TIMEOUT
{
	float ballPosx;
	float ballPosy;
	int   timeOutCount;//��ʱ����
};
//���
struct CMD_S_BALLCLUB
{
	PT ptMousePos;			//���λ��,������ӵ�λ��
	PT ptBaseBallPos;		//����λ��
	float  m_hitForce;          //�������
};
//����
struct CMD_S_HitBall
{
	float                           fHitForce;               //����
	float                           fMousePosx;              //�����
	float                           fMousePosy;              //�����
	float                           fHitPosx;                //�����
	float                           fHitPosy;                //�����
	float                           fbaseBallPosx;           //����
	float                           fbaseBallPosy;           //����
	float                           fAngle;                  //�Ƕ�
	WORD                            wHitUser;                //�����û�
	int                             nCurHitNum;                //��ǰ�ڼ���
};

//������
struct CMD_S_HitResult
{
	bool    bHitWithWall;           //�Ƿ�����ǽ
	BYTE    inHoleNum;				//������
	BYTE    ballInHole[BALL_NUM];	//������
	BYTE    firstBallNum;           //��һ����������
	PT      ptBall[BALL_NUM];       //ÿ�����״̬
};
enum HIT_RESULT
{
	openFail=1,                     //����ʧ��
	firtIn=2,                       //�״ν��� 
	noInHole=3,						//û�н���
	rightInHole=4,					//��������
	wrongInHole=5,                  //����
	gameEnd=6,                      //��Ϸ����
};

//����ͷ��˵���Ϣ
struct CMD_S_CLIENT_HITRESLUT
{
	BYTE    inHoleNum;				//������
	BYTE    ballInHole[BALL_NUM];	//������
	BYTE    firstBallNum;           //��һ����������
	PT      ptBall[BALL_NUM];       //ÿ�����λ��
	BOOL    ballState[BALL_NUM];    //���״̬
	BYTE    currentUser;            //��һ����������
	BYTE    hitResult;              //����Ľ��
	BYTE    comobNum;               //������
	BYTE    totalComobNum[2]  ;     //ÿ���˵�������
	BYTE    userBallType[2];        // 1���Ǵ���0����С��
};
//��Ϸ����
struct CMD_S_GameEnd
{
	__int64							nGameScore[GAME_PLAYER];						//��Ϸ����
	__int64							nBaseScore;										//������
	BYTE                            comobNum[GAME_PLAYER];                          //������
	BYTE                            endReason; //1:�������㣬2�����䣬3������,4��ʱ,5���ݴ�����Ϸ��ɢ //����ԭ��

};

//////////////////////////////////////////////////////////////////////////

#endif