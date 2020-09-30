#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x0400		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0400		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ�� Windows 98 ����߰汾���ض����ܡ�
#define _WIN32_WINDOWS 0x0410 //Ϊ Windows Me �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0400	//Ϊ IE 5.0 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ��������������ȫ���Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ���ĺͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����

#include <afxdtctl.h>		// Internet Explorer 4 �����ؼ��� MFC ֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows �����ؼ��� MFC ֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

//////////////////////////////////////////////////////////////////////////
//��������
#define INVALID_ITEM				0xFFFF								//��Ч����
//��ඨ��
#define DEF_X_DISTANCE				17									//Ĭ�ϼ��
#define DEF_Y_DISTANCE				17									//Ĭ�ϼ��
#define DEF_SHOOT_DISTANCE			20									//Ĭ�ϼ��

//��ඨ��
#define DEF_X_DISTANCE_SMALL		17									//Ĭ�ϼ��
#define DEF_Y_DISTANCE_SMALL		17									//Ĭ�ϼ��
#define DEF_SHOOT_DISTANCE_SMALL	20									//Ĭ�ϼ��

//��Ϣ����
#define	IDM_LEFT_HIT_CARD			(WM_USER+500)						//����˿�
#define	IDM_RIGHT_HIT_CARD			(WM_USER+501)						//�һ��˿�

#define MAX_CARD_COUNT				27									//�˿���Ŀ
#define SPACE_CARD_DATA				255									//����˿�

//��ֵ����
#define	CARD_MASK_COLOR				0xF0								//��ɫ����
#define	CARD_MASK_VALUE				0x0F								//��ֵ����

//X ���з�ʽ
enum enXCollocateMode 
{ 
	enXLeft,						//�����
	enXCenter,						//�ж���
	enXRight,						//�Ҷ���
};

//Y ���з�ʽ
enum enYCollocateMode 
{ 
	enYTop,							//�϶���
	enYCenter,						//�ж���
	enYBottom,						//�¶���
};
//�˿˽ṹ
struct tagCardItem
{
	bool							bShoot;								//�����־
	BYTE							cbCardData;							//�˿�����
};

//���뻷��
#include "..\..\..\SDKCondition.h"

#ifdef SDK_CONDITION

//////////////////////////////////////////////////////////////////////////////////
//��������
#include "..\��Ϣ����\CMD_Game.h"
#include "..\..\..\������\Include\GameFrameHead.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../������/Lib/Ansi/WHImage.lib")
//#pragma comment (lib,"../../../������/Lib/Ansi/D3DEngine.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/GameFrame.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/SkinControl.lib")
#else
#pragma comment (lib,"../../../������/Lib/Unicode/WHImage.lib")
//#pragma comment (lib,"../../../������/Lib/Unicode/D3DEngine.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/GameFrame.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/SkinControl.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../������/Lib/Ansi/WHImageD.lib")
//#pragma comment (lib,"../../../������/Lib/Ansi/D3DEngineD.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/GameFrameD.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/SkinControlD.lib")
#else
#pragma comment (lib,"../../../������/Lib/Unicode/WHImageD.lib")
//#pragma comment (lib,"../../../������/Lib/Unicode/D3DEngineD.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/GameFrameD.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/SkinControlD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////////////
//ϵͳ����

#include "..\��Ϣ����\CMD_Game.h"
#include "..\..\..\..\ϵͳģ��\�ͻ������\��Ϸ���\GameFrameHead.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Ansi/WHImage.lib")
//#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Ansi/D3DEngine.lib")
#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Ansi/GameFrame.lib")
#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Ansi/SkinControl.lib")
#else
#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Unicode/WHImage.lib")
//#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Unicode/D3DEngine.lib")
#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Unicode/GameFrame.lib")
#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Unicode/SkinControl.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Ansi/WHImageD.lib")
//#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Ansi/D3DEngineD.lib")
#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Ansi/GameFrameD.lib")
#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Ansi/SkinControlD.lib")
#else
#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Unicode/WHImageD.lib")
//#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Unicode/D3DEngineD.lib")
#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Unicode/GameFrameD.lib")
#pragma comment (lib,"../../../ϵͳģ��/���ӿ�/Unicode/SkinControlD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////
