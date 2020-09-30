#include "StdAfx.h"
#include ".\PoolPhysics.h"
#include "GameClientView.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define BACK_WIDTH  800
#define BACK_HEIGHT  600
#define TABLE_WIDTH  800
#define TABLE_HEIGHT  436
#define BAR_WIDTH  48


const float physics::HoleInfo[NUMINFO]=
	{
		12.5,				//line
		28, 5, 69, 40,		
		78, 48, 380, 48,
		440, 48, 733, 48,
		743, 40, 773, 2,
		769, 70, 798, 52,
		763, 81, 763, 371,
		768, 377, 796, 403,
		740, 405, 775, 440,
		441, 402, 730, 402,
		84, 402, 378, 402,
		47, 433, 70, 405,
		2, 418, 38, 380,
		50, 82, 50, 370,
		0, 0, 0, 0,
		12.5,				// angle
		82, 413,				
		377, 413,
		442, 413,
		732, 413,
		777, 368,
		777, 79,
		734, 34,
		440, 34,
		380, 34,
		80, 34,
		37, 78,
		37, 367,
		19,				// hole
		52, 51,			
		408, 33,
		760, 50,
		761, 397,
		408, 412,
		53, 397
};

physics::physics()
{	
	//Elasticity_BallToBall = 0.8f;  //������֮��ķ���ϵ��
	Elasticity_BallToBall = 0.9f;  //������֮��ķ���ϵ��

	Elasticity_BallToWall = 0.65f;  //ǽ����ķ���ϵ��

	//Per_Friction_BallToBall = 0.4f;	 //����� ˲��ײ����Ħ��ϵ�� 
	Per_Friction_BallToBall = 0.10f;	 //����� ˲��ײ����Ħ��ϵ��
	//Per_Friction_BallToWall = 0.5f;    //ǽ���� ˲��ײ����Ħ��ϵ��
	Per_Friction_BallToWall = 0.5f;    //ǽ���� ˲��ײ����Ħ��ϵ��
	m_offsetx = 0;
	m_offsety = 0;	

	ZeroMemory(m_wOverBall, sizeof(m_wOverBall));
	m_nOverNum = 0;
	m_pMainWnd = NULL;
	m_bHitWithWall = false;
	m_wFistHitBall = 0;
	m_ptClientMouse.x = 0;
	m_ptClientMouse.y = 0;
	m_ptClientMid.x = 0;
	m_ptClientMid.y = 0;

}

physics::~physics()
{

}


void physics:: Allcollision(CBallRender*p_BALL)
{	
	Vector3 pos1 , pos2	, Vtra ;
	float	Extra ;
	int i,j;

	//������֮�����ײ
	for (i=0;i<16-1;i++)
	{	
		if (p_BALL[i].BALL_bEnable==false)continue;
		pos1 = p_BALL[i].Ball_Pos;

		for (j=(i+1);j<16;j++)
		{
			if (p_BALL[j].BALL_bEnable==false)continue;
			if ((p_BALL[i].BALL_State==4)&&(p_BALL[j].BALL_State==4))continue;

			pos2 = (*(p_BALL+j)).Ball_Pos;
			Vtra = (pos1- pos2)	;

			Extra = Vtra.mag() - (banjin + banjin);
			if (Extra < 0)
			{
				if(process_collision ((p_BALL+i), (p_BALL+j), (pos1+ pos2)/2, NormaliseVector(Vtra), -Extra))
				{
					if(m_pMainWnd)
					{				
						((CGameClientView*)m_pMainWnd)->PlayGameSound(IDS_COLL);
					}
				}
				if (i==0 && m_wFistHitBall ==17)
				{
					m_wFistHitBall = p_BALL[j].BallNo;
				}
			}
		}
	}
	collision_wall(p_BALL);
	collision_hole(p_BALL);
}

//������Ƿ����
void physics::collision_hole(CBallRender*p_BALL)
{
	Vector3 pos1,  Vtra ;
	float	Extra ;
	int i,j;

	for (i=0;i<16;i++)
	{ 
		if (p_BALL[i].BALL_bEnable==false)continue;
		pos1 = p_BALL[i].Ball_Pos;
		for (j=0;j<NUMHOLE;j++)
		{ 
			Vtra = (pos1-Hole[j].pnt) ;
			//�����Ľ��붴��,ʩ��һ��ʹ�����п�£
			Extra = Vtra.mag()- Hole[j].R ;
			if (Extra < 0)
				p_BALL[i].BALL_V-=(NormaliseVector(Vtra)/1000);
			//����ȫ���붴��
			Extra = Vtra.mag() + banjin - Hole[j].R ;
			if (Extra < 0) 
			{
				//p_BALL[i].binhole=true;
				p_BALL[i].BALL_bEnable=false;
				// p_BALL[i].Pos=Vector3(0);
				p_BALL[i].BALL_V=Vector3(0);
				p_BALL[i].BALL_Vang=Vector3(0);
				p_BALL[i].BALL_State=4;
				//����
				if(m_pMainWnd&&j>=0&&j<=5)
				{
					((CGameClientView*)m_pMainWnd)->m_liziMovie[j].Start(Hole[j].pnt.X-75+m_offsetx,Hole[j].pnt.Y-80+m_offsety,50,false);
					((CGameClientView*)m_pMainWnd)->PlayGameSound(IDS_HOLE);

				}
				//if (p_BALL[i].BallNo) //�������ҲҪ����
				{
					m_wOverBall[m_nOverNum++]=p_BALL[i].BallNo;
					if(m_pMainWnd&&p_BALL[i].BallNo>0)
						((CGameClientView*)m_pMainWnd)->SetHitBallInHole(p_BALL[i].BallNo);
				}
			}
		}
	}

	//��ֹԽ��
	for (i=0;i<16;i++)
	{
		if (p_BALL[i].BALL_bEnable==false)continue;
		pos1 = (*(p_BALL+i)).Ball_Pos;

		if ((pos1.Y<48)||(pos1.Y>400)||(pos1.X<45)||(pos1.X>TABLE_WIDTH-45))
		{
			//p_BALL[i].binhole=true;
			p_BALL[i].BALL_bEnable=false;
			p_BALL[i].BALL_V=Vector3(0);
			p_BALL[i].BALL_Vang=Vector3(0);
			p_BALL[i].BALL_State=4;
			int j=0;
			for (j=0;j<NUMHOLE;j++)
			{ 
				Vtra = (pos1-Hole[j].pnt) ;
				//�����Ľ��붴��,ʩ��һ��ʹ�����п�£				 
				Extra = Vtra.mag()  - Hole[j].R ;
				if (Extra < 0) 
				{
					break;
				}
			}
			ASSERT(j>=0&&j<=5);
			//����
			if(m_pMainWnd&&j>=0&&j<=5)
			{
				((CGameClientView*)m_pMainWnd)->m_liziMovie[j].Start(Hole[j].pnt.X-75+m_offsetx,Hole[j].pnt.Y-80+m_offsety,50,false);
					((CGameClientView*)m_pMainWnd)->PlayGameSound(IDS_HOLE);
			}
			//f (p_BALL[i].BallNo)
			{
				m_wOverBall[m_nOverNum++]=p_BALL[i].BallNo;
				if(m_pMainWnd&&p_BALL[i].BallNo>0)
						((CGameClientView*)m_pMainWnd)->SetHitBallInHole(p_BALL[i].BallNo);
			}
		}
	}

}

void physics::collision_wall(CBallRender*p_BALL)
{
	Vector3 pos1 ,  Vtra ;
	float	Extra ;
	int i,j;

	Vector3 CZ ; //��ֱ��
	float k ; //��ֱ�� 

	//��ǽ�����ײ
	for (i=0;i<16;i++)
	{ 
		if (p_BALL[i].BALL_State==4)continue;
		for (j=0;j<NUMWALL;j++)
		{ 

			pos1 = (*(p_BALL+i)).Ball_Pos;
			if(pos1.X<45)
			{
				int a = 0;
				int b = a++;
			}
			Extra = dot(pos1, WALL[j].Nor) - WALL[j].d ;
			if (abs(Extra) < banjin) 
			{
				CZ = (pos1+ (WALL[j].Nor* -Extra));

				if (WALL[j].starpnt.X != WALL[j].endpnt.X) 
					k = (CZ.X - WALL[j].endpnt.X) / (WALL[j].starpnt.X - WALL[j].endpnt.X);
				else
					k = (CZ.Y - WALL[j].endpnt.Y) / (WALL[j].starpnt.Y - WALL[j].endpnt.Y) ;


				if ((k > 0) && (k < 1))
				{
					if (Extra > 0) 
						process_collision_wall( (p_BALL+i), (pos1 -(WALL[j].Nor*banjin)), WALL[j].Nor, banjin - Extra);
					else
						process_collision_wall( (p_BALL+i), (pos1 +(WALL[j].Nor*banjin)), -WALL[j].Nor, banjin + Extra);

				} //if k > 0 And k < 1 {

			}

		}
	}	

	//��Բ�߽ǵ���ײ
	for (i=0;i<16;i++)
	{ 	 
		if (p_BALL[i].BALL_State==4)continue;
		for (j=0;j<NUMPING;j++)
		{ 
			pos1 = (*(p_BALL+i)).Ball_Pos;
			Vtra = (pos1- Ping[j].pnt) ;
			Extra = Vtra.mag() - (banjin + Ping[j].R) ;
			if (Extra < 0) 
			{
				Vtra = NormaliseVector(Vtra);
				process_collision_wall ((p_BALL+i), (pos1+ (Vtra* -banjin)), Vtra, -Extra )	;
			}
		}

	}

}


//ǽ�����ײ��
BOOL physics::process_collision_wall(CBallRender* Boxa,Vector3 collpos ,Vector3 colN ,float Extra )
{

	m_bHitWithWall  = true;
	TRACE("-----------------------------------------��ǽ��-----------------------------------------");
	//�̴�����
	Boxa->Ball_Pos+=(colN*Extra);

	collision cl;
	Vector3 posa, Vanga ,  Va ;

	posa = Boxa->Ball_Pos ; 
	Vanga = Boxa->BALL_Vang ;
	Va = Boxa->BALL_V;

	//struct collision
	//{
	//	Vector3 Pn; //����
	//	float Pt;  //Ħ������
	//	Vector3 tangent_vel; //Ħ���ٶ�
	//	float tangent_speed;  //Ħ���ٶȴ�С
	//	float  c ;//��������(Ca+Cb)
	//	float Vn; //����ٶ�
	//	float Vn2; //ײ���������ٶ�
	//	Vector3 _N;  //��ײ��ֱ����
	//	Vector3  Ra;  //ײ���㵽��������(����a)
	//	Vector3  Va; //ײ������ٶ�(����a)
	//	float  Ca;  //��������(����a)
	//	Vector3  Rb;  //ײ���㵽��������(����b)
	//	Vector3  Vb; //ײ����ײ������ٶ�(����B)
	//	float  Cb;  //��������(����B)
	//};

	cl._N = colN;//��ײ��ֱ����
	cl.Ra = (collpos- posa); //ײ���㵽��������(����a)
	cl.Va = (cross(Vanga, cl.Ra)+Va);  //v'= v +�ء�R//ײ������ٶ�(����a)
	cl.Vn = dot(cl._N, cl.Va) ; //��Դ�ֱ�ٶ�
	cl.Vn2 = -Elasticity_BallToWall * cl.Vn; //��ֱ����ٶ�(��ײ��) Elasticity_BallToWall�Ƿ���ϵ��
	if (cl.Vn >= 0 ) return FALSE;
	cl.Ca = 1.0f / M ;

	cl.Pn=cl._N * ((cl.Vn2 - cl.Vn) / cl.Ca );

	//Ӧ��������
	Boxa->apply_impulse(cl.Pn, collpos);  


	//��ʼĦ�������ļ���
	collision clnew; //�Ѿ��ı����ײ

	clnew._N = cl._N ;
	clnew.Ra = cl.Ra;
	clnew.Va = (cross(Vanga, clnew.Ra)+Va);  // v'= v +�ء�R

	clnew.tangent_vel = (clnew.Va- (clnew._N* dot(clnew.Va, clnew._N)));  //tangent_vel=Va+Vb-((Va+Vb)��N)*N

	clnew.tangent_speed = clnew.tangent_vel.mag();
	if (clnew.tangent_speed > 0)
	{
		Vector3 T ;
		T= -clnew.tangent_vel / clnew.tangent_speed ;
		//Ca = 1/m+((Ra��T)/Iz��Ra)T
		clnew.c = 1.f / M + dot(T, cross((cross(clnew.Ra, T) / Boxa->Iz), clnew.Ra)) ;

		if (clnew.c > 0 )
		{

			float Ptt;  //��ʱĦ������
			Ptt = clnew.tangent_speed / clnew.c;

			if (Ptt < Per_Friction_BallToWall * cl.Pn.mag())  //����Ħ���ж� Per_Friction_BallToWall��Ħ��ϵ��

				clnew.Pt = Ptt; //��Ħ��
			else
				clnew.Pt = Per_Friction_BallToWall * cl.Pn.mag(); //��Ħ��

			Boxa->apply_impulse (T* clnew.Pt, collpos);  //Ӧ��Ħ������

		} //end if (clnew.c > 0 )

	}  //end if (clnew.tangent_speed > 0)

	//hge->Effect_Play(snd[1]);
	//PlayWav("ballHITBar.wav");//ײǽ
	return TRUE ;	
}

//��ʼ�������ļ���
BOOL physics::process_collision(CBallRender* Boxa,CBallRender* Boxb,Vector3 collpos ,Vector3 colN ,float Extra )
{
	//�̴�����
	Boxa->Ball_Pos +=(colN*Extra * 0.5);
	Boxb->Ball_Pos +=(colN*-Extra * 0.5);

	collision cl;
	Vector3 posa, posb ,Vanga , Vangb, Va , Vb;

	posa = Boxa->Ball_Pos ; 	posb = Boxb->Ball_Pos; 
	Vanga = Boxa->BALL_Vang; Vangb =Boxb->BALL_Vang ;
	Va = Boxa->BALL_V; Vb = Boxb->BALL_V;

	cl._N = colN;
	cl.Ra = (collpos- posa);
	cl.Rb = (collpos- posb);
	cl.Va = (cross(Vanga, cl.Ra)+ Va);  //v'= v +�ء�R
	cl.Vb = (cross(Vangb, cl.Rb)+ Vb);
	cl.Vn = dot(cl._N, cl.Va) - dot(cl._N, cl.Vb); //��Դ�ֱ�ٶ�
	cl.Vn2 = -Elasticity_BallToBall* cl.Vn; //��ֱ����ٶ�(��ײ��) Elasticity_BallToBall�Ƿ���ϵ��
	if (cl.Vn >= 0 ) return FALSE;
	cl.Ca = 1.0f / M ;
	cl.Cb = 1.0f / M;
	cl.Pn=cl._N * ((cl.Vn2 - cl.Vn) / (cl.Ca + cl.Cb));

	//Ӧ��������
	Boxa->apply_impulse(cl.Pn, collpos);  
	Boxb->apply_impulse (-cl.Pn, collpos);

	//��ʼĦ�������ļ���
	collision clnew; //�Ѿ��ı����ײ
	Vector3 Vr_All ;

	clnew._N = cl._N ;
	clnew.Ra = cl.Ra; clnew.Rb = cl.Rb;
	clnew.Va = (cross(Vanga, clnew.Ra)+Va);  // v'= v +�ء�R
	clnew.Vb = (cross(Vangb, clnew.Rb)+Vb);
	Vr_All = (clnew.Va- clnew.Vb);
	clnew.tangent_vel = (Vr_All- (clnew._N* dot(Vr_All, clnew._N)));  //tangent_vel=Va+Vb-((Va+Vb)��N)*N

	clnew.tangent_speed = clnew.tangent_vel.mag();
	if (clnew.tangent_speed > 0)
	{
		Vector3 T ;
		T= -clnew.tangent_vel / clnew.tangent_speed ;
		//Ca = 1/m+((Ra��T)/Iz��Ra)T
		clnew.Ca = 1 / M + dot(T, cross((cross(clnew.Ra, T) / Boxa->Iz), clnew.Ra)) ;
		clnew.Cb = 1 / M + dot(T, cross((cross(clnew.Rb, T) / Boxb->Iz), clnew.Ra)) ;
		clnew.c = clnew.Ca + clnew.Cb ;

		if (clnew.c > 0 )
		{
			const float friction = Per_Friction_BallToBall; //��Ħ��ϵ��
			float Ptt;  //��ʱĦ������
			Ptt = clnew.tangent_speed / clnew.c;

			if (Ptt < friction * cl.Pn.mag())  //����Ħ���ж�
				clnew.Pt = Ptt; //��Ħ��
			else
				clnew.Pt = friction * cl.Pn.mag(); //��Ħ��

			Boxa->apply_impulse (T* clnew.Pt, collpos);  //Ӧ��Ħ������
			Boxb->apply_impulse (-T* clnew.Pt, collpos);

		} //end if (clnew.c > 0 )

	}  //end if (clnew.tangent_speed > 0)


	return TRUE ;	
}

void  physics::Init(void *pMainWnd)
{
	m_pMainWnd = pMainWnd;

 	int i = 0, nIndex = 1;
	int offsetx = 10;
	for (i=0;i<NUMWALL;i++)
	{
		float bx,by,ex,ey;
		bx = HoleInfo[nIndex++];
		by = HoleInfo[nIndex++];
		ex = HoleInfo[nIndex++];
		ey = HoleInfo[nIndex++];
		cline& tWALL= WALL[i];
		tWALL.starpnt=Vector3(bx,by);
		tWALL.endpnt=Vector3(ex,ey);
		tWALL.Nor=RotateZ(NormaliseVector(tWALL.starpnt-tWALL.endpnt), PI_DIV2);
		tWALL.d = dot(tWALL.starpnt, tWALL.Nor) ;
	}	

	//Բת�Ǵ�
	//  10      9  8   	    7
	// 11      				 6
	// 
	//	
	//
	// 12			         5
	//  1       2  3        4	

//	float r =hge->Ini_GetFloat("Angle","R",18.f);
	float r =   HoleInfo[nIndex++];
	for (i=0;i<NUMPING;i++)
	{
		float px,py;
		px = HoleInfo[nIndex++];
		py = HoleInfo[nIndex++];
		Ping[i].pnt=Vector3(px,py);
		Ping[i].R=r;
	}	

	///////////////////////////////////////////////////////
	//6���Ŷ�  
	//r =hge->Ini_GetFloat("Hole","R",22.f);
	r = HoleInfo[nIndex++];
	for (i=0;i<NUMHOLE;i++)
	{
		float px,py;
		px = HoleInfo[nIndex++];
		py = HoleInfo[nIndex++];
		Hole[i].pnt=Vector3(px,py);
		Hole[i].R=r;
	}

	ASSERT(nIndex == NUMINFO);
}
bool  physics::Exit()
{
	/*for(int i=0;i<4;i++)
	    hge->Effect_Free(snd[i]);*/
	return true;
}
