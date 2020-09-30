#pragma once

#include "Stdafx.h"
 
#include "movemovie.h" 
#include "SWichMovie.h"
#include "BallRender.h"
#include "PoolPhysics.h"
#include "InterfaceView.h"

//#define DEBUG_GAME
//////////////////////////////////////////////////////////////////////////
//��Ϣ����
#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_SETBASERULE				(WM_USER+110)						//���û�����
#define IDI_SUSPEND_SEND                2000                                //��ͣ����ʱ��

//��ťID 
#define IDC_START						1									//��ʼ��ť
#define IDC_RESTART     				2									//��ʼ��ť 
#define IDC_SOUND						3									//����
#define IDC_GIVEPU                      4									//����
#define IDC_REBEGIN                     5

// ����ID
#define IDS_BACK						1
#define IDS_CLOCK						2
#define IDS_ENTER						3
#define IDS_COMBO						4
#define IDS_BEG							5
#define IDS_WIN							6
#define IDS_FAILURE						7
#define IDS_HIT							8
#define IDS_COIN						9
#define IDS_GOOD						10
#define IDS_COLL						11
#define IDS_HOLE						12

class CGameClientDlg;
enum BASE_BALL_STATE
{
	nothing= 0,//��Ч
	waitSet = 1,//�ȴ�����
	waitHit = 2,//�ȴ�����
	startHit = 3 ,//��ʼ����
	afterHit = 4,//��������
	allState = 5,//ȫ����ֹ
};

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI, public ITQView
{

public:
	physics                        m_physics;                          //����ϵͳ
	CBallRender					   m_balls[BALL_NUM];				   // ��
	CBallRender					   m_visibleBalls[BALL_NUM];		   // û�д��ȥ����
	CPoint                         m_ptMousePos;                       //����λ��
	CPoint                         m_ptInBallDesk;                     //����������ϵ�λ��
	CPoint                         m_ptHitMark;                        //�����˷�����ĵ�
	CPoint                         m_ptCenterHitMark;                  //�����˷���������ĵ�
	float                          m_ClubAnge;                         //���ӵĽǶ�
	int                            m_offsetx;						   //�������Ӧ���ڵ�ƫ����
	int                            m_offsety;
	float                          m_hitForce;                         // �����
	HCURSOR                        m_cursor;                           //���
	__int64						   m_nBaseScore;					   //��������
	DWORD						   m_dwLastClickLButtonTime;		   //���������ʱ��
	int                            m_offsetDistance;                   //����ʱ����˵����˾���
	BYTE                           m_comobNum[GAME_PLAYER];            //���������
	BYTE                           m_baseBallState;                    // �����״̬
	bool                           m_bFirstInBall;                     //�Ƿ��ǵ�һ������
	bool                           m_bHit;                             //�Ƿ������
	__int64                        m_endScore[GAME_PLAYER];
	BYTE						   m_userBallType[GAME_PLAYER];        //1���Ǵ���0����С��
	BYTE                           m_roundCount;                       //����
	bool                           m_bSingleMode ;                     //�Ƿ��ǵ���ģʽ
	float                          m_angle;                            //�Ƕ�
public:
	CGameClientDlg					*m_pParent;							//

	
	BOOL							m_IsGameEnd;						//�Ƿ���Ϸ����	
 	//״̬����
	long							m_lWidth;							//�����
	long							m_lHeight;							//�����
	CList<CString,CString&>         m_systemInfoList;                   //�����Ϣ



	//λ����Ϣ
public:
	 
	CPoint                          m_ptUserInfo[GAME_PLAYER];          //�����Ϣ��
	CPoint                          m_ptSeat[GAME_PLAYER];              //��λ��λ��
	CPoint                          m_ptBaseScore;                      //�׷�
	PT                              m_ptBallClub;                       //�Է��ƶ����
	bool                            m_bMoveBallClub;                    //�Ƿ��ƶ����

	// ͼƬ����
public:
	CHgeSprite					*m_pSpriteBack;						//��Ϸ����
	CHgeSprite					*m_pSpriteLogo;						//��Ϸ����	
	CHgeSprite					*m_pSpriteResult;					//�����
	CHgeSprite                    *m_pSpriteClub;                     //���
	CHgeSprite                    *m_pSpriteSightBall;                //��׼��	
	CHgeSprite                    *m_pSpriteHoleBall;					//������
	CHgeSprite                    *m_pSpriteMovieSingle;              //��������Ļ�
	CHgeSprite                    *m_pSpriteInfo[2];                  //�����Ϣ 
	CHgeSprite                    *m_pSpriteCenterInfo;               //�м�Ŀ� 
	CHgeSprite                    *m_pSpriteHitMark;                  //ʮ��
	CHgeSprite                    *m_pSpriteHitBigBall;               //��׼�򣬴���
	CHgeSprite                    *m_pSpriteHitForce_left;            //������
	CHgeSprite                    *m_pSpriteHitForce_right;           //������
	CHgeSprite                    *m_pHitAngle_left;					//�Ƕ�
	CHgeSprite                    *m_pHitAnge_right;                  //�Ƕ�
	CHgeText					  *m_pTextForce;                      //����
	CHgeSprite                    *m_pSpriteCoinObverse[72];          //Ӳ�ҵ�����
	CHgeSprite                    *m_pSpriteCoinReverse[72];          //Ӳ�ҵķ���
	CHgeSprite                    *m_pSpriteObverse;                  //���濪��
	CHgeSprite                    *m_pSpriteReverse;                  //���濪��
	CHgeSprite                    *m_pSpriteHand;                     //�֡�
	CHgeSprite                    *m_pSpriteFailOpen;                 //����ʧ�ܣ���������
	CHgeSprite                    *m_pSpriteContinue;                 //���˻��ᣬ��������
	CHgeSprite                    *m_pSpriteChange;                   //��ͨ���򣬽�������
	CHgeSprite                    *m_pSpriteSelect;                   //ѡ����ɫ����������
	CHgeSprite                    *m_pSpriteSelect_hua;               //ѡ����ɫ����������
	CHgeSprite                    *m_pSpriteFailHit;                  //���򷸹棬��������
	CHgeSprite                    *m_pSpriteWin;                      //Ӯ
	CHgeSprite                    *m_pSpriteLose;                     //��
	CHgeSprite                    *m_pSpriteGameStart;                //��ֿ�ʼ
	CHgeSprite                    *m_pSpriteNum;		                //����
	CHgeSprite                    *m_pSpriteBallNumBk;                //��ʾ�����ĵ�ͼ 
	CHgeSprite                    *m_pSpriteComob;                    //������
	CHgeSprite                    *m_pSpriteTimeBar[5];               //ʱ����
	CHgeSprite                    *m_pSpriteTimeNum;                  //ʱ������ 
	CHgeSprite                    *m_pSpriteDirection[2];             // ��ͷ


	//�ı���
protected:
	CHgeText						*m_pTextUserInfo[GAME_PLAYER];		 //�����Ϣ
	CHgeText						*m_pTextUserScore[GAME_PLAYER];		 //�������
	CHgeText						*m_pTextUserCombom[GAME_PLAYER];	 //������Ŀ
	CHgeText						*m_pTextUserBaseScore[GAME_PLAYER];  //�׷�
	CHgeText						*m_pTextBallNum;                     //���
	CHgeText						*m_pTextEndResult[12];               //������ϵ��ı����  
	CHgeText						*m_pTextSystemInfo[12];              //��Ϸ��Ϣ

	//����
public:	
	CSWichMovie						m_liziMovie[NUMHOLE];               //���Ӷ���,ÿ������һ������
	CSWichMovie                     m_coinMovieObverse;                 //ͶӲ�ҵ�����
	CSWichMovie                     m_coinMovieReverse;                 //Ͷ�Ҷ�������
	CSWichMovie                     m_directionMovie;                   //����
	CMoveMovie                      m_movieObverse;                     //����
	CMoveMovie                      m_movieReverse;                     //����
	CMoveMovie                      m_movieHitResult;                   //������
	CMoveMovie                      m_movieGameStart;                   //��ֿ�ʼ
	CMoveMovie                      m_movieComob;                       //����

	//�˿˿ؼ�
public:
	CPoint							m_ptTimer[GAME_PLAYER];
	CPoint                          m_ptReady[GAME_PLAYER];


//////////////////////////////////////////////////////////////////////////
	// HGE ����
public:
	CHgeSound*						m_pHgeSound;						// ��������
	CHgeLoad						m_HgeLoad;							// ��Դװ��
	static HGE*						m_pHge;								// HGEָ��
	static CHgeButtonMgr*			m_pBtMgr;							// ��ť����	

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();


	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID  Release() {  delete this; return ; }
	//�ӿڲ�ѯ
	virtual void *  QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	// ��ͼ�ӿ�
public:
	// ��ȡHGEָ��
	virtual HGE* __cdecl GetHge(void) { return m_pHge; }
	// ���Ӱ�ť
	virtual void __cdecl AddButton(BYTE cbID, const char *filename, DWORD size = 0, bool bMipmap = false);
	// ɾ����ť
	virtual void __cdecl DelButton(BYTE cbID, bool bAll = false);
	// ���ð�ť
	virtual void __cdecl SetButtonPos(BYTE cbID, CPoint pt);
	// ������ʾ
	virtual void __cdecl SetButtonShow(BYTE cbID, WORD wType);
	// ��ȡ��С
	virtual void __cdecl GetButtonSize(BYTE cbID, CSize& size);
	// ��ť�¼�
	virtual void __cdecl OnEventButton(WORD wButtonID, BYTE cbButtonType);
	// ���Ӱ�ť
	virtual void __cdecl AddSound(WORD wID, const char* fileName);
	// ɾ������
	virtual void __cdecl DelSound(WORD wID, bool bAll = false);
	// ��������
	virtual void __cdecl PlayGameSound(WORD wID, bool bLoop = false);

	//�̳к���
public:
	virtual void ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight) {}
	//WIN7֧��
	virtual bool RealizeWIN7() { return true; }

	//�������
	virtual VOID InvalidGameView(INT nXPos, INT nYPos, INT nWidth, INT nHeight){}

public:
	// ���ƺ���
	virtual bool RenderViewEx(void);
	// ���ƺ���
	virtual bool FrameViewEx(void);
	// HGE��ʼ����
	virtual void OnInitHge(void);

	// ��̬����
protected:
	// ��̬���ƽӿ�
	static bool RenderView(void);
	// ��̬���ƽӿ�
	static bool FrameView(void);

	// ���к���
public:
	// ��ʼ��HGE
	void InitHge(HWND hWnd);	
	// ����HGE
	void RunHge(float fDelTime = 0);
	// װ����Դ
	bool LoadResource(void); 

public:	
	//�����
	void  DrawBallClub();
	//���ð�ťλ��
	void RectifyButton(); 


	//��ͼ����
private:
	//������
	void DrawBack( CRect *pRect );
 	//�����
	void DrawUser( CRect *pRect );
	//�������
	void DrawResultFrame( CRect *pRect );
	//����ʾ��Ϣ
	void DrawFront( CRect *pRect );
	//����Ϸ��Ϣ
	void DrawGameInfo( CRect *pRect );
	//��ʱ��
	void DrawTime( int nTime, CPoint point );
	void DrawTableWaitTime();

	//����
protected:
 
	//���ַ�����ӿո�
	void FillStringWithSpace( TCHAR str[], int nLen );
	//�õ���˵ĳ���
	int  GetBallClubLength();
	//����ַ���
	int SplitString(CString &str,int splitLen);

 
	//------------------------------------------------------------------------------//

	//���ú���
public:
	void SetGameEnd();
	//���ܺ���
public:
	
	//��������
	void SetBaseScore(__int64 nBaseScore);
	//���÷���
	__int64 GetBaseScore() { return m_nBaseScore; }
	//����UI
	void ResetUI(); 
	void ResetBall();
	//��ʼ����������ʾÿ����û�����ȥ����leftLitterNum�������С���
	void InitVisibleBall(bool leftLitterNum,bool visible=true);
	//������Ϸ��ʼ
	void SetGameStart();
	// ����
	void HitBall(float fHitPx,float fHitPy,float mousex,float mousey,float fHitForce);
	//������ļ��
	bool BallStateCheck(float dt);
	//���ý���
	void SetHitBallInHole(int ballNum);
	//void �Է��ƶ����
	void SetBallClubMove( PT pt);
	
	//��ť�¼�
protected:
	//��ʼ��ť
	void OnStart();
	//����
	void OnSound();
public:
	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//���ٴ���
	afx_msg void OnDestroy( );
	//ʱ�亯��
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// ��ť����
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// ��ť����
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	// ��ť����
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	// ��С
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//������������Ϣ֪ͨ
	afx_msg LRESULT OnMovieEnd(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
