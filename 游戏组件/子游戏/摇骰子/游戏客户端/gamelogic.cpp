#include "StdAfx.h"
#include "GameLogic.h"

#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}

/****************************************************
*��������IsSpecialType 
*���ܣ�  �ж�һ�������Ƿ�����������,��������Ϊ���ӵ�
		 ʱ�򱣴汪�����ӵ���ֵ					  JJ
*������	 bDiceData			��������(5)			IN
		 bDiceCount			������Ŀ(5)			IN
		 bHaveYellOne		�Ƿ񺰹�1			IN
		 pSpType			������Ϣ			OUT
*����ֵ	 true	��������
		 false  ��ͨ����
****************************************************/
bool CGameLogic::IsSpecialType(BYTE bDiceData[], BYTE bDiceCount, bool bHaveYellOne, tagSpecialType *pSpType)
{
	//����У��
	ASSERT(bDiceData!=NULL&&bDiceCount==5&&pSpType!=NULL);
	if (NULL == bDiceData || 5 != bDiceCount || NULL == pSpType)
		return false;

	ZeroMemory(pSpType, sizeof(tagSpecialType));

	//��������
	BYTE bTmpArr[6] = {0};
	BYTE bBaoziPoint = 0;
	
	//�ж�˳��
	for (WORD i = 0; i < bDiceCount; i++)
	{
		ASSERT(bDiceData[i]>=1&&bDiceData[i]<=6);

		if (bTmpArr[bDiceData[i]-1] != 0)
			break;
		
		bTmpArr[bDiceData[i]-1]++;

		if (i == bDiceCount - 1)
		{
			pSpType->bType = DICE_TYPE_STRAIGHT;
			return true;
		}
	}
	
	//�жϱ���
	for (WORD i = 0; i < bDiceCount; i++)
	{
		ASSERT(bDiceData[i]>=1&&bDiceData[i]<=6);

		if (!bHaveYellOne && bDiceData[i] == 1 && i != bDiceCount - 1)
			continue;
		
		if (bBaoziPoint == 0)
			bBaoziPoint = bDiceData[i];
		else if (bBaoziPoint != bDiceData[i])
		{
			if (bDiceData[i] != 1 || bHaveYellOne)
				break;
		}

		if (i == bDiceCount - 1)
		{
			pSpType->bType = DICE_TYPE_BAOZI;
			pSpType->bBaiziPoint = bBaoziPoint;
			return true;
		}
	}

	pSpType->bType = DICE_TYPE_NORMAL;
	return false;
}

/****************************************************
*��������RandDice 
*���ܣ�  �����������ֵ							JJ
*������	 bDiceData			��������(5)			OUT
		 bDiceCount			������Ŀ(5)			IN
*����ֵ	 ��
****************************************************/
void CGameLogic::RandDice(BYTE bDiceData[], BYTE bDiceCount)
{
	//����У��
	ASSERT(bDiceData!=NULL);
	if (NULL == bDiceData)	return;

	ZeroMemory(bDiceData, sizeof(BYTE)*bDiceCount);
	srand(GetTickCount());
	
	for (WORD i = 0; i < bDiceCount; i++)
		bDiceData[i] = rand()%6 + 1;
}

/****************************************************
*��������CalcDiceRange 
*���ܣ�  ���ݵ�ǰ��󺰻�����ĳ����Ŀ�ɺ�����С����	JJ
*������	 pDiceYell			��󺰻���Ϣ		IN
		 bDiceCount			Ҫ������Ŀ			IN
		 bFirstYell			�Ƿ���Ϸ��һ�κ���	IN
		 pbMinPoint			��С����			OUT
		 pbMaxPoint			������			OUT
*����ֵ	 0		 �е�ɺ�
		 1		 �޵�ɺ� (С�ڵ��ں�����Ŀ)
		 2		 �޵�ɺ� (������󺰻�12��6)
		 other	 ����
****************************************************/
LRESULT CGameLogic::CalcDiceRange(tagDiceYell  *pDiceYell, BYTE bDiceCount, bool bFirstYell, BYTE *pbMinPoint, BYTE *pbMaxPoint)
{
	//����У��
	ASSERT(pDiceYell!=NULL&&bDiceCount>=2&&bDiceCount<=12&&pbMinPoint!=NULL&&pbMaxPoint!=NULL);
	ASSERT(pDiceYell->bDiceCount>=0&&pDiceYell->bDiceCount<=12&&pDiceYell->bDicePoint>=0&&pDiceYell->bDicePoint<=6);
	if (pDiceYell == NULL || bDiceCount < 2 || bDiceCount > 12 || pbMinPoint == NULL || pbMaxPoint == NULL)
		return 3;
	
	//���ֺ���
	if (bFirstYell)
	{
		if (2 == bDiceCount)
		{
			*pbMinPoint = 1;
			*pbMaxPoint = 1;
		}
		else if (bDiceCount > 2 && bDiceCount < 13)
		{
			*pbMinPoint = 1;
			*pbMaxPoint = 6;
		}
		else	
			return 1;
	}
	else
	{
		//�ﵽ���
		if (pDiceYell->bDiceCount == 12 && pDiceYell->bDicePoint == 6)
			return 2;
			
		if (bDiceCount < pDiceYell->bDiceCount)
			return 1;
		else if (bDiceCount == pDiceYell->bDiceCount)
		{
			if (pDiceYell->bDicePoint == 6)
				return 1;
			else
			{
				*pbMinPoint = pDiceYell->bDicePoint + 1;
				*pbMaxPoint = 6;
			}
		}
		else
		{
			*pbMinPoint = 1;
			*pbMaxPoint = 6;
		}
	}

	return 0;
}

/****************************************************
*��������CalcDiceCount 
*���ܣ�  ����ĳ���������Ŀ	JJ
*������	 bDiceData			��������(5)			IN
		 bDiceCount			����������Ŀ(5)		IN
		 bDicePoint			Ҫ����������(1-6)	IN
		 bHaveYellOne		�Ƿ񺰹�1			IN
*����ֵ	 ������������Ŀ(0-12)
****************************************************/
int CGameLogic::CalcDiceCount(BYTE *bDiceData, BYTE bDiceCount, BYTE bDicePoint, bool bHaveYellOne/* = false*/)
{
	//����У��
	ASSERT(bDiceData!=NULL&&bDiceCount==5&&bDicePoint>=1&&bDicePoint<=6);
	if (bDiceData == NULL || bDiceCount !=5 || bDicePoint < 1 || bDiceCount > 6)
		return false;
	
	//��������
	tagSpecialType tst = {0};

	//��������
	if (IsSpecialType(bDiceData, bDiceCount, bHaveYellOne, &tst))
	{
		if (tst.bType == DICE_TYPE_STRAIGHT)
			return 0;
		else if (tst.bType == DICE_TYPE_BAOZI)
		{
			if ( tst.bBaiziPoint == bDicePoint || (!bHaveYellOne && tst.bBaiziPoint==1) )
				return 6;
		}
		else
		{
			//MyMsgBox(_T("CGameLogic::CalcDice [(%d %d %d %d %d) %d  %d]"), bDiceData[0], bDiceData[1], bDiceData[2], 
			//					bDiceData[3], bDiceData[4], bDicePoint, tst.bType);
			return 0;
		}
	}

	BYTE bCount = 0;
	for (int i = 0; i < bDiceCount; i++)
	{
		if (bDiceData[i] == bDicePoint)
		{
			bCount++;
			continue;
		}
		if (bDiceData[i] == 1 && !bHaveYellOne)
			bCount++;
	}
	return bCount;
}

/****************************************************
*��������CalcLimitBigYell 
*���ܣ�  ����ȴ��뺰������һ���ֵ����Ϊ12*6�򷵻�false JJ
*������	 pDiceYell	����ָ��				INOUT
*����ֵ	 true �����ֵ
		 false ����ֵ���ߴ���ֵ�ﵽ12*6
****************************************************/
bool CGameLogic::CalcLimitBigYell(tagDiceYell *pDiceYell)
{
	//������֤
	ASSERT(pDiceYell != NULL);
	if (pDiceYell == NULL)	return false;
	
	if	(pDiceYell->bDiceCount == 0 && pDiceYell->bDicePoint == 0)
	{
		pDiceYell->bDiceCount = 2;
		pDiceYell->bDicePoint = 1;
		return true;
	}

	ASSERT(pDiceYell->bDiceCount>=0&&pDiceYell->bDiceCount<=12&&pDiceYell->bDicePoint>=0&&pDiceYell->bDicePoint<=6);
	if (pDiceYell->bDiceCount<2 || pDiceYell->bDiceCount>12 || pDiceYell->bDicePoint<1 || pDiceYell->bDicePoint>6)
		return false;

	if (pDiceYell->bDiceCount == 12 && pDiceYell->bDicePoint == 6)
		return false;

	pDiceYell->bDiceCount = (pDiceYell->bDicePoint==6?pDiceYell->bDiceCount+1:pDiceYell->bDiceCount);
	pDiceYell->bDicePoint = (pDiceYell->bDicePoint==6?1:pDiceYell->bDicePoint+1);

	return true;
}
//////////////////////////////////////////////////////////////////////////
