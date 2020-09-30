#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "afxtempl.h"
#include "..\��Ϣ����\CMD_LiarsDice.h"
//////////////////////////////////////////////////////////////////////////	

//��������
#define		DICE_TYPE_NORMAL		0x00		//��ͨ����
#define		DICE_TYPE_STRAIGHT		0x01		//˳������
#define		DICE_TYPE_BAOZI			0x02		//��������
#define		DICE_TYPE_ERROR			0x03		//��������

//��������
struct	tagSpecialType
{
	BYTE	bType;			//����
	BYTE	bBaiziPoint;	//����������Ϊ���ӵ�ʱ��, ��ֵΪ���Ӵ���ĵ���
};

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////	
//���Ժ���
static void MyMsgBox(LPCSTR lpszFormat, ...)
{
//	va_list args;
//	int		nBuf;
//	TCHAR	szBuffer[512];
//
//	va_start(args, lpszFormat);
//	nBuf = _vsntprintf(szBuffer, sizeof(szBuffer)-sizeof(TCHAR), lpszFormat, args);
//#ifdef _DEBUG
//	AfxMessageBox(szBuffer);
//#else
//	TCHAR	szFormat[512];
//	_stprintf(szFormat, _T("[JJ]: %s"), szBuffer);
//	OutputDebugString(szFormat);
//#endif
//
//	va_end(args);
}

static void MyDebug(LPCSTR lpszFormat, ...)
{
	//va_list args;
	//int		nBuf;
	//TCHAR	szBuffer[512];
	//TCHAR	szFormat[512];

	//va_start(args, lpszFormat);
	//nBuf = _vsntprintf(szBuffer, sizeof(szBuffer)-sizeof(TCHAR), lpszFormat, args);
	//_stprintf(szFormat, _T("[JJ]: %s"), szBuffer);
	//OutputDebugString(szFormat);

	//va_end(args);
}
//////////////////////////////////////////////////////////////////////////


//��Ϸ�߼���
class CGameLogic
{
	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

public:
	//��������
	bool IsSpecialType(BYTE bDiceData[], BYTE bDiceCount, bool bHaveYellOne, tagSpecialType *pSpType);

	//�����ֵ
	void RandDice(BYTE bDiceData[], BYTE bDiceCount);

	//�ɺ�����
	LRESULT CalcDiceRange(tagDiceYell  *pDiceYell, BYTE bDiceCount, bool bFirstYell, BYTE *pbMinPoint, BYTE *pbMaxPoint);

	//������Ŀ
	int CalcDiceCount(BYTE bDiceData[], BYTE bDiceCount, BYTE bDicePoint, bool bHaveYellOne = false);

	//��С��ֵ
	bool CalcLimitBigYell(tagDiceYell *pDiceYell);
};

//////////////////////////////////////////////////////////////////////////

#endif
