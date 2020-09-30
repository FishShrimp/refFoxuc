#include "StdAfx.h"
#include "Resource.h"
#include "GameOption.h"

#define new DEBUG_NEW

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameOption::CGameOption()
{
	//���Ա���
	ZeroMemory(m_szRegKeyName,sizeof(m_szRegKeyName));

	//���ز���
	DefaultParameter();

	return;
}

//��������
CGameOption::~CGameOption()
{
}

//���ز���
VOID CGameOption::LoadParameter()
{
	//��������
	CWHRegKey RegOptionItem;

	//���ñ���
	if (RegOptionItem.OpenRegKey(m_szRegKeyName,REG_GAME_CLIENT_OPTION,false)==true)
	{
		m_bMusicVoice=RegOptionItem.GetValue(TEXT("bMusicVoice"),m_bMusicVoice);
		m_bSoundVoice=RegOptionItem.GetValue(TEXT("bSoundVoice"),m_bSoundVoice);
		m_nCurrentVolumeMusic=RegOptionItem.GetValue(TEXT("nVolumeMusic"),m_nCurrentVolumeMusic);
		m_nCurrentVolumeSound=RegOptionItem.GetValue(TEXT("nVolumeSound"),m_nCurrentVolumeSound);

		m_nStartKey=RegOptionItem.GetValue(TEXT("nStartKey"),m_nStartKey);
		m_nLineCountUpKey=RegOptionItem.GetValue(TEXT("nLineCountUpKey"),m_nLineCountUpKey);
		m_nLineCountDownKey=RegOptionItem.GetValue(TEXT("nLineCountDownKey"),m_nLineCountDownKey);
		m_nLineNumberUpKey=RegOptionItem.GetValue(TEXT("nLineNumberUpKey"),m_nLineNumberUpKey);
		m_nLineNumberDownKey=RegOptionItem.GetValue(TEXT("nLineNumberDownKey"),m_nLineNumberDownKey);
	}

	return;
}

//�������
VOID CGameOption::SaveParameter()
{
	//��������
	CWHRegKey RegOptionItem;

	//���ñ���
	if (RegOptionItem.OpenRegKey(m_szRegKeyName,REG_GAME_CLIENT_OPTION,true)==true)
	{
		RegOptionItem.WriteValue(TEXT("bMusicVoice"),m_bMusicVoice);
		RegOptionItem.WriteValue(TEXT("bSoundVoice"),m_bSoundVoice);
		RegOptionItem.WriteValue(TEXT("nVolumeMusic"),m_nCurrentVolumeMusic);
		RegOptionItem.WriteValue(TEXT("nVolumeSound"),m_nCurrentVolumeSound);

		RegOptionItem.WriteValue(TEXT("nStartKey"),m_nStartKey);
		RegOptionItem.WriteValue(TEXT("nLineCountUpKey"),m_nLineCountUpKey);
		RegOptionItem.WriteValue(TEXT("nLineCountDownKey"),m_nLineCountDownKey);
		RegOptionItem.WriteValue(TEXT("nLineNumberUpKey"),m_nLineNumberUpKey);
		RegOptionItem.WriteValue(TEXT("nLineNumberDownKey"),m_nLineNumberDownKey);
	}

	return;
}

//Ĭ�ϲ���
VOID CGameOption::DefaultParameter()
{
	m_bMusicVoice = TRUE;
	m_bSoundVoice = TRUE;
	m_nCurrentVolumeMusic = -1000;
	m_nCurrentVolumeSound = 0;

	m_nStartKey = VK_RETURN;
	m_nLineCountUpKey = VK_UP;
	m_nLineCountDownKey = VK_DOWN;
	m_nLineNumberUpKey = VK_RIGHT;
	m_nLineNumberDownKey = VK_LEFT;

	return;
}

//���ò���
bool CGameOption::InitParameter(LPCTSTR pszKeyName)
{
	//���ñ���
	lstrcpyn( m_szRegKeyName,pszKeyName,CountArray(m_szRegKeyName) );

	LoadParameter();

	return true;
}

//////////////////////////////////////////////////////////////////////////

//ȫ������
CGameOption g_GameOption;

//////////////////////////////////////////////////////////////////////////
