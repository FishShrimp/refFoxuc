#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��
#define MAX_COUNT 4
//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//�˿�����
#define D2H_VALUE0					0									//��Ч����
#define D2H_D2H						199									//��������
#define D2H_DOBLUE_Q				198									//�������
#define D2H_DOBLUE_2				197									//�ض�����
#define D2H_DOBLUE_8				196									//�ʶ�����
#define D2H_DOBLUE_4				195									//�Ͷ�����
#define D2H_DOBLUE_46A				194									//�ж�����
#define D2H_DOBLUE_67AJ				193									//�¶�����
#define D2H_DOBLUE_9875				192									//��������
#define D2H_TH						191									//�������
#define D2H_TG						190									//�������
#define D2H_DG						189									//�ظ�����

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
private:
	static BYTE						m_cbCardListData[32];				//�˿˶���

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);

	//���ܺ���
public:
	//���⻻��
	void ChangeSpeCard( BYTE cbCardData[], BYTE cbCardCount);
	//������ֵ
	BYTE GetSpeCardValue(BYTE cbCardData[], BYTE cbCardCount);
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�Ա��˿�
	BYTE CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
