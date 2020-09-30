#pragma once

#include "HgeGUI.h"


/*
�л��Ķ���
1.һ�����ͼƬ�ļ��л�����
2.һ��ͼ��֡��ͼƬ����
*/
class CSWichMovie
{
public:
	CSWichMovie(void);
	~CSWichMovie(void);
	//������Դ��graphSpriteָ��һ��ͼƬ��Դ��PtPos,λ�ã�num:ָ������Ĵ�С��swithTime�л���ʱ��
	void SetResource(CHgeSprite**graphSprite,int num,CPoint &ptPos,int swithTime,CWnd *pParent);
	void SetResource(CHgeSprite*graphSprite,int num,CPoint ptPos=CPoint(0,0),int switchTime=100,CWnd *pParent=NULL,int ex=2);
	//��ͼ
	void DrawMovice();
	//֡����
	void OnCalcFrame(float dt);
	//qǿ��ֹͣ
	void Stop();
	//ֹͣ���ǲ�������Ϣ
	void ForceStop();
	//��ʼ����
	void Start(CPoint pt,int switchTime,bool bLoop=false);
	void Start(int x,int y,int switchTime=200,bool bLoop=false);
	//������ʧ�ӳ�
	void SetDisappearTime(int time);
	//����ÿһ֡��������Դ��
	void SetPerExchangePic(int ex)
	{
		m_perExchangePic = ex;
	}
protected:
	CHgeSprite **m_pSprite;      //����ͼƬ
	CHgeSprite  *m_pSingleSprite;//һ��ͼƬ
	int           m_currentFrame;
	int           m_totalFrame;
	CPoint        m_ptPosition;
	int           m_swithTime;
	bool          m_bLoop;
	bool          m_bStop;
	CWnd         *m_pParent;
	int           m_disappearTime;//��ʧ�ӳ�
	int           m_perExchangePic;
};
