// physics.h: interface for the physics class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(PHYSICS_H)
#define PHYSICS_H

#pragma once

#include "3dmath.h"
#include ".\ballrender.h"


CONST INT NUMWALL=14;
CONST INT NUMPING=12;
CONST INT NUMHOLE=6;
CONST int NUMINFO=95;


#define BALL_NUM 16            //��
class physics  
{	
public:
	physics();
	virtual ~physics();
	struct collision
	{
		Vector3 Pn; //����
		float Pt;  //Ħ������
		Vector3 tangent_vel; //Ħ���ٶ�
		float tangent_speed;  //Ħ���ٶȴ�С
		float  c ;//��������(Ca+Cb)
		float Vn; //����ٶ�
		float Vn2; //ײ���������ٶ�
		Vector3 _N;  //��ײ��ֱ����
		Vector3  Ra;  //ײ���㵽��������(����a)
		Vector3  Va; //ײ������ٶ�(����a)
		float  Ca;  //��������(����a)
		Vector3  Rb;  //ײ���㵽��������(����b)
		Vector3  Vb; //ײ����ײ������ٶ�(����B)
		float  Cb;  //��������(����B)
	};
	struct cline
	{
		Vector3  starpnt ;
		Vector3  endpnt ;
		Vector3  Nor    ;
		float d 	;
	};
	struct cPing
	{
		Vector3  pnt ;
		float R ;
	};

	void Allcollision(CBallRender*p_BALL);
	void collision_hole(CBallRender*p_BALL); 

	BOOL process_collision(CBallRender* Boxa,CBallRender* Boxb,Vector3 collpos ,Vector3 colN ,float Extra ) ;
	BOOL process_collision_wall(CBallRender* Boxa,Vector3 collpos ,Vector3 colN ,float Extra ) ;

	void collision_wall(CBallRender*p_BALL);
	void SetOffset(int x,int y)
	{
		m_offsetx = x;
		m_offsety = y;
	}

	POINT	m_ptClientMouse;	
	POINT	m_ptClientMid;
	cline   WALL[NUMWALL];
	cPing   Ping[NUMPING];
	cPing   Hole[NUMHOLE]; //�Ŷ�

	float  Elasticity_BallToBall ;     //������֮��ķ���ϵ��
	float  Elasticity_BallToWall;     //ǽ����ķ���ϵ��

	float  Per_Friction_BallToBall ;	 //����� ˲��ײ����Ħ��ϵ�� 
	float  Per_Friction_BallToWall ;    //ǽ���� ˲��ײ����Ħ��ϵ�� 
public:
//	HEFFECT  snd[4];
	//hgeParticleSystem    *par;//һ����������
	void     Init(void *pMainWnd);
    bool     Exit();
	void	 InitOneOver(){m_nOverNum =0;m_wFistHitBall =17;m_bHitWithWall=false;};

public:
	BYTE     m_wOverBall[BALL_NUM];                //��˴�����򣨰�����˳��
	int      m_nOverNum;                           //������  
	BYTE     m_wFistHitBall;                       //��һ����������
	bool     m_bHitWithWall;                       //�Ƿ�������ǽ
	void    *m_pMainWnd;
	int      m_offsetx;
	int      m_offsety;
	const static float HoleInfo[NUMINFO];

};

#endif 