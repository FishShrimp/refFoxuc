#pragma once

#include "3dmath.h"
#include "InterfaceView.h"
#include "HgeGUI.h"

//#define banjin 12.5 //�뾶
//#define BALLDiameter  25 //ֱ��

#define BACK_WIDTH  800//�������ڵĿ��
#define BACK_HEIGHT  600//�������ڵĸ߶�

#define TABLE_WIDTH  812//ָ������������ȣ������߿�
#define TABLE_HEIGHT 447//�����������߶ȣ������߿�

#define BAR_WIDTH  48  //��̨�Ŀ��
#define MIN_FORCE  3.2 //��С������

#define banjin 12.5 //�뾶
#define BALLDiameter  25 //ֱ��
#define BALLTEXW  banjin*2+1 //������ͼ��� 1����Ϊ�������

class CBallRender
{
//#define  BALL_R   16
//#define  BALL_2R  32
#define  M   1.f //���Ծ�

public:
	CBallRender(void);
	~CBallRender(void);
public:
	int             BallNo; 	
	int             offsetx;
	int             offsety;
public:	
	HTEXTURE		 tex;                 //������ľ��
	Vector3 Ball_Pos;//���λ��
	Matrix3  BALL_Orientation;//ÿ����ķ���
	Vector3 BALL_V ;    //�ٶ�
	Vector3 BALL_Vang ; //���ٶ�
	int BALL_State; //״̬
	bool BALL_bEnable;  //	�Ƿ���Ч
	bool BALL_bIsRedraw; //�Ƿ����
	bool BALL_bIsInit; //�Ƿ��ʼ��
	bool binhole;
	float Iz ; //���Ծ�
	float SENSTA_FRICTION;  //����Ħ����С��ֵⷧ
	float SENROU_FRICTION;  //����Ħ����С��ֵⷧ	
	DWORD originColor;
	int  banJin;
	int  zhiJin;
	
	//////////////////////////////////////////////////////////////////////////
	ITQView*					m_pIGameFrameView;
	void SetView(IUnknownEx* pIUnknownEx);
	HGE * GetHge(void) { if (m_pIGameFrameView) return m_pIGameFrameView->GetHge(); return NULL; }
	//////////////////////////////////////////////////////////////////////////
	float ranged_random(float v1,float v2)
	{
		return v1 + (v2-v1)*((float)rand())/((float)RAND_MAX);
	}
	//ȡ��ĳͼƬ���β�ֵ������
	void GetFitColor(float X, float Y ,DWORD *indate,int mPitch,DWORD *outdate) //��������
	{
		FLOAT IncX,IncY;
		FLOAT K1 , K2, K3 , K4;
		int T1,T2;

		IncX = X - (int)X;
		IncY = Y - (int)Y;
		T1=(int)X;
		T2=(int)Y;
		K1 = (1 - IncX) * (1 - IncY);
		K2 = IncX * (1 - IncY);
		K3 = (1 - IncX) * IncY;
		K4 = IncX * IncY ;


		BYTE *p=(BYTE *)indate+T1*4+T2*mPitch ;
		BYTE poutdate[4];

		//��ֵ
		poutdate[0] =(int)(K1*(*p)+ K2**(p+4)+ K3**(p+mPitch)+ K4**(p+4+mPitch));
		poutdate[1] =(int)(K1**(p+1)+ K2**(p+5)+ K3**(p+mPitch+1)+ K4**(p+5+mPitch));
		poutdate[2] =(int)(K1**(p+2)+ K2**(p+6)+ K3**(p+mPitch+2)+ K4**(p+6+mPitch));
		poutdate[3] =(int)(K1**(p+3)+ K2**(p+7)+ K3**(p+mPitch+3)+ K4**(p+7+mPitch));

		*outdate =ARGB(poutdate[3],poutdate[2],poutdate[1],poutdate[0]);
	}

		 //	����ݵ��APL��� C1����,C2�������, NUMMIX APLϵ��(0-9)
    __inline void  MIX_APLC (BYTE *C1,BYTE *C2,SHORT NUMMIX  )
	{
	*C2	=(NUMMIX*(int)*C1+(9-NUMMIX)*(int)*C2)/9;
	*(C2+1)	=(NUMMIX*(INT)*(C1+1)+(9-NUMMIX)*(INT)*(C2+1))/9;
	*(C2+2)	=(NUMMIX*(INT)*(C1+2)+(9-NUMMIX)*(INT)*(C2+2))/9;
	} 
	//һ���������뵥ɫ�ĳ˷����
	__inline void  MIX_multself1 (BYTE *indate,int COL )
	{
		*indate= (COL**indate) >>8;
		*(indate+1)= (COL**(indate+1)) >>8;
		*(indate+2)= (COL**(indate+2)) >>8;
	}

	
__inline Vector3 CALEPOS(FLOAT X, FLOAT Y)
{
	FLOAT lng = banJin*banJin - (X* X + Y*Y);
	return  Vector3(X, Y, -sqrtf(lng)) ;
}

//3X3��Ե����ݵ�
	void  CALEFJC	(FLOAT Xf, FLOAT Yf ,INT Ni ,INT Nj);
	void ApplyForce(Vector3 force,Vector3 colpos );//Ӧ����
	inline void apply_impulse(Vector3 impulse ,Vector3 colpos);//Ħ����
	void  Moveball();
	void  UpdateBallTex();
	
public:	
	void   Init(int _ballNo,Vector3 _ballPos);
	void   Reset(int _ballNo,Vector3 _ballPos,bool _enable);
	void   Move();
	void   Render();
	void   RenderEx();
	void   SetOffSet(int x,int y) {offsetx = x;offsety = y;}
	bool   MouseMoveIn(float x,float y);
	//�Ƿ��Ǿ�ֹ��
	bool   IsStop();
	void   Clear()
	{
		ZeroMemory(&BALL_V,sizeof(BALL_V));
		ZeroMemory(&BALL_Vang,sizeof(BALL_Vang));
		BALL_State = 4;		 
	}
};
