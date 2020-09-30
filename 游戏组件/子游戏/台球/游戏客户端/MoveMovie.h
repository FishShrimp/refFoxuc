#pragma once
#include "HgeGUI.h"
//////////һ��ͼ�ƶ�(��a��b)�Ķ���///////////////////////////////////////////////////////
#define WM_MOVE_END                 (WM_USER+020)                       //����������Ϣ
class CMoveMovie
{
public:
	CMoveMovie(void);
	~CMoveMovie(void);
	void SetResource(CHgeSprite* pGraphSprite,CWnd *pParentWnd);
	//��ptStart�ƶ���ptEnd,��ʱtime
	void StartMovie(CPoint &ptStart,CPoint &ptEnd,DWORD time);
	//��ptStart�ƶ���ptEnd,��ʱtime,suspendTime:�ƶ�ǰ��ͣ��ʱ��
	void StartMovie(CPoint &ptStart,CPoint &ptEnd,DWORD time,int suspendTime);
	void DrawMovie();
	void CalcPosition(float dt);
	//����
	void Stop();
	//��������Ϣ
	void ForceStop();
	bool IsPlaying();
	//���ö�����ʼ�ƶ�ǰ��ͣ��ʱ��
	void SetBeginMoveSuspendTime(int time );
	//���ö�����ʧǰ��ͣ��ʱ��
	void SetDisapperTime(int time)
	{
		m_disapperTime = time;
	}
	//��������
	void SetNumResource(CHgeSprite *num)
	{
		m_pGraphSpriteNum = num;
	}
	void ShowNum(int num,CPoint offset);
private:
	CHgeSprite   *m_pGraphSprite;
	CHgeSprite   *m_pGraphSpriteNum;//����ͼƬ
	CPoint          m_ptStart;
	CPoint          m_ptEnd;
	CPoint          m_ptCurrent;
	DWORD           m_tickCount;
	bool            m_bIsMoving;
	CWnd          * m_pParentWnd;
	int             m_beforeMoveSuspendTime;//�����ƶ�ǰ��ͣ��ʱ��
	int             m_disapperTime;         //������ʧǰ��ͣ��ʱ��
	int             m_showNum;//��Ҫ��ʾ������
	CPoint          m_numOffset;
};
