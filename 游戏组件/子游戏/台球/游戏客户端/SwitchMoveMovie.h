#pragma once
#include "swichmovie.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////����һ������ͼƬ�л����ƶ����ŵĶ���
class CSwitchMoveMovie :
	public CSWichMovie
{
public:
	CSwitchMoveMovie(void);
	~CSwitchMoveMovie(void);
	//��ʼ����
	void Start(CPoint ptStart,CPoint ptEnd,DWORD switchTime);
	void OnCalcFrame(float dt);
private:
	CPoint m_ptStart;
	CPoint m_ptEnd;
	DWORD  m_totalTime;//��ʱ��
	DWORD  m_playingTime;//����ʱ��
};
