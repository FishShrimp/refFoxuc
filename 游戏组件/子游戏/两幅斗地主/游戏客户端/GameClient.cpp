#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientEngine.h"

#define new DEBUG_NEW

//Ӧ�ó������
CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientApp::CGameClientApp()
{
}

//��������
CGameClientApp::~CGameClientApp() 
{
}
//��������
CGameFrameEngine * CGameClientApp::GetGameFrameEngine(DWORD dwSDKVersion)
{
	//�汾���
	if (InterfaceVersionCompare(VERSION_FRAME_SDK,dwSDKVersion)==false) return NULL;

	//��������
	return new CGameClientEngine;
}

//////////////////////////////////////////////////////////////////////////
