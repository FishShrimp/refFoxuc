#ifndef GAME_OPTION_HEAD_FILE
#define GAME_OPTION_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//ע�����

#define REG_GAME_CLIENT_OPTION				TEXT("GameOption")				//��Ϸ��Ϣ

//////////////////////////////////////////////////////////////////////////


//��Ϸѡ��
class CGameOption
{
	//���Ŷ���
public:
	BOOL 							m_bMusicVoice;								//��������
	BOOL 							m_bSoundVoice;								//��Ч����
	INT								m_nCurrentVolumeMusic;						//���ִ�С
	INT								m_nCurrentVolumeSound;						//��Ч��С

	UINT							m_nStartKey;								//��ʼ��ťֵ
	UINT							m_nLineCountUpKey;							//��������ֵ
	UINT							m_nLineCountDownKey;						//��������ֵ
	UINT							m_nLineNumberUpKey;							//��ֵ����ֵ
	UINT							m_nLineNumberDownKey;						//��ֵ����ֵ

	//���Ա���
protected:
	TCHAR							m_szRegKeyName[16];							//ע������

	//��������
public:
	//���캯��
	CGameOption();
	//��������
	virtual ~CGameOption();

	//���ܺ���
public:
	//���ز���
	VOID LoadParameter();
	//�������
	VOID SaveParameter();
	//Ĭ�ϲ���
	VOID DefaultParameter();

	//���ú���
public:
	//���ò���
	bool InitParameter(LPCTSTR pszKeyName);
};


//////////////////////////////////////////////////////////////////////////

//����˵��
extern CGameOption				g_GameOption;						//ȫ������

//////////////////////////////////////////////////////////////////////////

#endif