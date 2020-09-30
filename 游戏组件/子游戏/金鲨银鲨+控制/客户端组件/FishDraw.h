#pragma once
#include "ObjectMove.h"
#include "D3DTextureIndex.h"

#define FISH_MAX					6
class CFishDraw
{
	//��Ϣ����
public:
	int											m_nBackIndex;				//��������
	CPoint										m_ptBullet[4];				//�����ӵ�λ��
	CPoint										m_ptCannon[4];				//��̨λ��
	int											m_nCannonAlpha;				//��̨����

	//�����
public:
	CWHArray<tgaFishDraw*>						m_ArrayFishDraw;
	CLapseCount									m_LapseCount;				//���ż���
	BOOL										m_bAddFish;					//�����

	CObjectMove*								m_pBulletMove[4];			//�ӵ��ƶ�
	CObjectMove*								m_pNetMove[4];				//�����ƶ�
	CLapseCount									m_BulletCount;				//���ż���

	//��Դ
public:
	CD3DTexture							m_ImgaeBack[7];				//����
	CD3DTexture							m_ImgaeWater[12];			//ˮ��
	CD3DTextureIndex							m_ImgaeFish[FISH_MAX];		//��

	CD3DTextureIndex							m_ImgaeGun;					//�ܹ�
	CD3DTextureIndex							m_ImgaeCannon;				//����

	CD3DTexture							m_ImgaeBullet;				//�ӵ�
	CD3DTextureIndex							m_ImgaeNet;					//��

	//λ��
public:
	CRect										m_rectFishPond;
	
	//���캯��
public:
	CFishDraw(void);
	~CFishDraw(void);


	//���ú���
public:
	//�Ƿ������
	void SetAddFish( BOOL bAddFish ) { m_bAddFish = bAddFish; }
	//�������
	VOID SetFishPond( CRect	rectFishPond );
	//���ñ�������
	VOID SetBackIndex( int nBackIndex ) { m_nBackIndex = nBackIndex; }


	//��ȡ����
	int GetBackIndex() { return m_nBackIndex; }

	//ϵͳ�¼�
public:
	//����
	VOID OnFishMovie();
	//����
	VOID OnFishCreate(CD3DDevice * pD3DDevice);

	//���غ���
public:
	//�滭
	VOID OnFishDraw(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight);

	//����
public:
	//��·��
	BYTE FishPath( CPoint* pFishMove );
};
