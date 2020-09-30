#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define ST_ORDER					0									//��С����
#define ST_COUNT					1									//��Ŀ����
#define ST_VALUE					2									//��ֵ����

//��ֵ����
#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_THREE					3									//��������
#define CT_SINGLE_LINK				4									//��������
#define CT_DOUBLE_LINK				5									//���ö���
#define CT_THREE_LINK				6									//���ö���
#define CT_BOMB						7									//ը������
#define CT_BOMB_3W					8									//3��ը��
#define CT_BOMB_TW					9									//����ը��
#define CT_BOMB_LINK				10									//��ը����

/////////////////////////////////////




//���ƽ��
struct tagOutCardResult
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbResultCard[MAX_COUNT];			//����˿�
};

//////////////////////////////////////////////////////////////////////////



//��Ϸ�߼���
class CGameLogic
{
	//��������
public:
	static const BYTE				m_cbCardData[FULL_COUNT];							//�˿�����
    static const BYTE				m_cbCardGrade[TYPE_COUNT];							//���͵ȼ�
	static const BYTE				m_cbTransTable[BOMB_TYPE_COUNT][BOMB_TYPE_COUNT];	//ԾǨ�б�
	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���Ժ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&MASK_COLOR; }
	//���ƺ���
public:
	//�����˿�
	VOID SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	VOID RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//�����˿ˣ�����ǣ�
	BYTE RandCardList(BYTE cbCardList[],BYTE cbBufferCount,BYTE cbMainCard,WORD cbOutPut[]);
	//ɾ���˿�
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);

	//�߼�����
public:
	//ը��ת��
	bool TransBombGrade(BYTE cbBombInfo[]);
	//�����߷���
	BYTE GetMaxBombGrade(BYTE *cbBomblist);
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�߼�ת����
	BYTE TransForLogicValue(BYTE cbCardData);
	//�߼�ת����
	BYTE GetCardLogicValueEx(BYTE cbCardData);
	//�Ա��˿�
	bool CompareCard(tagAnalyseResult& PerOutCard,tagAnalyseResult& CurOutCard);
	//��������
	bool SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult);

	//�ڲ�����
public:
	//ը������
	bool IsBombCard(tagAnalyseResult& AnalyseResult);
	//����Ŀ������
	bool FindValidCard(tagAnalyseResult& CurOutCard,tagAnalyseResult& cbPerOutCard);
	//��������
	bool FillCardCount(tagAnalyseResult&AnalyseResultEx,int nMinCount);
	//�������
	bool FillCardSeque(tagAnalyseResult&AnalyseResult,int cbMinCount,int cbMinLink,bool bAllowCycle);
	//�����˿�
	VOID AnalysebCardData(const BYTE cbCardData[],int cbCardCount,tagAnalyseResult& AnalyseResult);
	//��������
	bool FindLinkCard(tagAnalyseResult& AnalyseResult,BYTE cbMinCardCount,BYTE cbMinLinkLong,bool bLockCount,bool bAllowCycle);
};

//////////////////////////////////////////////////////////////////////////

#endif