#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����
//��������
const BYTE cbIndexCount=5;

//�˿�����
const BYTE	CGameLogic::m_cbCardData[FULL_COUNT]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
		0x4E,0x4F,
};

const BYTE	CGameLogic::m_cbGoodcardData[GOOD_CARD_COUTN]=
{
	0x01,0x02,
		0x11,0x12,
		0x21,0x22,
		0x31,0x32,
		0x4E,0x4F,
		0x07,0x08,0x09,
		0x17,0x18,0x19,
		0x27,0x28,0x29,
		0x37,0x38,0x39,
		0x0A,0x0B,0x0C,0x0D,
		0x1A,0x1B,0x1C,0x1D,
		0x2A,0x2B,0x2C,0x2D,
		0x3A,0x3B,0x3C,0x3D
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
	//AI����
	m_lBankerOutCardCount = 0 ;
	m_bCanOutSingleKing=false;
}

//��������
CGameLogic::~CGameLogic()
{
#ifdef _CHECK_DUG
	file.Close() ;
#endif
}

//��ȡ����
BYTE CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount)
{
	//������
	switch (cbCardCount)
	{
	case 0:	//����
		{
			return CT_ERROR;
		}
	case 1: //����
		{
			if(GetCardColor(cbCardData[0])==0x40) 
			{
				if(m_bCanOutSingleKing==true) return CT_SINGLE;
				return CT_ERROR;
			}
			return CT_SINGLE;
		}
	case 2:	//���ƻ��
		{
			BYTE bValue1=GetCardValue(cbCardData[0]);
			BYTE bValue2=GetCardValue(cbCardData[1]);

			if(bValue1==0x0F && bValue2==0x0E)	return CT_BOMB_KING;

			if (GetCardLogicValue(cbCardData[0])==GetCardLogicValue(cbCardData[1])) return CT_DOUBLE;

			if(bValue1==0x0E || bValue1==0x0F) return CT_DOUBLE;

			return CT_ERROR;
		}
	case 3://ը��
		{
			BYTE bValue1=GetCardValue(cbCardData[0]);
			BYTE bValue2=GetCardValue(cbCardData[1]);
			BYTE bValue3=GetCardValue(cbCardData[2]);

			//Ӳը��
			if (GetCardLogicValue(cbCardData[0])==GetCardLogicValue(cbCardData[2])) return CT_BOMB_3A;	

			//��ը��
			if(bValue1==0x0E || bValue1==0x0F )
			{
				if(bValue2==0x0E || bValue2==0x0F) return CT_BOMB_3;
				if(bValue2==bValue3) return CT_BOMB_3;
			}
			break;
		}
	case 4://�ⵯ
		{
			BYTE bValue1=GetCardValue(cbCardData[0]);
			BYTE bValue2=GetCardValue(cbCardData[1]);
			BYTE bValue3=GetCardValue(cbCardData[2]);
			BYTE bValue4=GetCardValue(cbCardData[3]);

			//Ӳ�ⵯ
			if (GetCardLogicValue(cbCardData[0])==GetCardLogicValue(cbCardData[3])) return CT_BOMB_4A;	

			//���ⵯ
			if(bValue1==0x0E || bValue1==0x0F)
			{
				if( bValue2==0x0E || bValue2==0x0F)
				{
					if(bValue3==bValue4) return CT_BOMB_4;
				}
				if(bValue2==bValue4) return CT_BOMB_4;
			}
			break;
		}
	}

	//�����˿�
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

	//ը���ж�
	if ( 4 < cbCardCount && cbCardCount <= 8 )
	{
		BYTE bColor1=GetCardColor(cbCardData[0]);
		BYTE bColor2=GetCardColor(cbCardData[1]);
		//�����ж�
		if ((AnalyseResult.cbFourCount==1)&&(cbCardCount==5) && bColor1==0x40) return CT_BOMB_KING;
		if ((AnalyseResult.cbFourCount==1)&&(cbCardCount==6) && bColor1==0x40 && bColor2==0x40) return CT_BOMB_KING;
	}

	//��������
	if (AnalyseResult.cbDoubleCount>=2)
	{
		//��������
		BYTE cbCardData=AnalyseResult.cbDoubleCardData[0];
		BYTE cbFirstLogicValue=GetCardLogicValue(cbCardData);

		//�������
		if (cbFirstLogicValue>=15) return CT_ERROR;

		//�����ж�
		for (BYTE i=1;i<AnalyseResult.cbDoubleCount;i++)
		{
			BYTE cbCardData=AnalyseResult.cbDoubleCardData[i*2];
			if (cbFirstLogicValue!=(GetCardLogicValue(cbCardData)+i)) return CT_ERROR;
		}

		//�����ж�
		if ((AnalyseResult.cbDoubleCount*2)==cbCardCount) return CT_DOUBLE_LINE;

		return CT_ERROR;
	}

	//�����ж�
	if ((AnalyseResult.cbSignedCount>=3)&&(AnalyseResult.cbSignedCount==cbCardCount))
	{
		//��������
		BYTE cbCardData=AnalyseResult.cbSignedCardData[0];
		BYTE cbFirstLogicValue=GetCardLogicValue(cbCardData);

		//�������
		if (cbFirstLogicValue>=15) return CT_ERROR;

		//�����ж�
		for (BYTE i=1;i<AnalyseResult.cbSignedCount;i++)
		{
			BYTE cbCardData=AnalyseResult.cbSignedCardData[i];
			if (cbFirstLogicValue!=(GetCardLogicValue(cbCardData)+i)) return CT_ERROR;
		}

		return CT_SINGLE_LINE;
	}

	return CT_ERROR;
}

//�����˿�
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//��Ŀ����
	if (cbCardCount==0) return;

	//ת����ֵ
	BYTE cbSortValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardLogicValue(cbCardData[i]);	

	//�������
	bool bSorted=true;
	BYTE cbThreeCount,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			if ((cbSortValue[i]<cbSortValue[i+1])||
				((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//����λ��
				cbThreeCount=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbThreeCount;
				cbThreeCount=cbSortValue[i];
				cbSortValue[i]=cbSortValue[i+1];
				cbSortValue[i+1]=cbThreeCount;
				bSorted=false;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	//��Ŀ����
	if (cbSortType==ST_COUNT)
	{
		//�����˿�
		BYTE cbIndex=0;
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

		//��������
		CopyMemory(&cbCardData[cbIndex],AnalyseResult.cbFourCardData,sizeof(BYTE)*AnalyseResult.cbFourCount*4);
		cbIndex+=AnalyseResult.cbFourCount*4;

		//��������
		CopyMemory(&cbCardData[cbIndex],AnalyseResult.cbThreeCardData,sizeof(BYTE)*AnalyseResult.cbThreeCount*3);
		cbIndex+=AnalyseResult.cbThreeCount*3;

		//��������
		CopyMemory(&cbCardData[cbIndex],AnalyseResult.cbDoubleCardData,sizeof(BYTE)*AnalyseResult.cbDoubleCount*2);
		cbIndex+=AnalyseResult.cbDoubleCount*2;

		//��������
		CopyMemory(&cbCardData[cbIndex],AnalyseResult.cbSignedCardData,sizeof(BYTE)*AnalyseResult.cbSignedCount);
		cbIndex+=AnalyseResult.cbSignedCount;
	}

	return;
}
//�õ�����
void CGameLogic::GetGoodCardData(BYTE cbGoodCardData[NORMAL_COUNT])
{
	//����׼��
	BYTE cbCardData[CountArray(m_cbGoodcardData)];
	BYTE cbCardBuffer[CountArray(m_cbGoodcardData)];
	CopyMemory(cbCardData,m_cbGoodcardData,sizeof(m_cbGoodcardData));

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	BYTE cbBufferCount=CountArray(m_cbGoodcardData);
	do
	{
		cbPosition=rand()%(cbBufferCount-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	//���ƺ���
	CopyMemory(cbGoodCardData, cbCardBuffer, NORMAL_COUNT) ;
}

//ɾ������
bool CGameLogic::RemoveGoodCardData(BYTE cbGoodcardData[NORMAL_COUNT], BYTE cbGoodCardCount, BYTE cbCardData[FULL_COUNT], BYTE cbCardCount) 
{
	//��������
	ASSERT(cbGoodCardCount<=cbCardCount);
	if(cbGoodCardCount>cbCardCount)
		return false ;

	//�������
	BYTE cbDeleteCount=0,cbTempCardData[FULL_COUNT];
	if (cbCardCount>CountArray(cbTempCardData)) return false;
	CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//�����˿�
	for (BYTE i=0;i<cbGoodCardCount;i++)
	{
		for (BYTE j=0;j<cbCardCount;j++)
		{
			if (cbGoodcardData[i]==cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j]=0;
				break;
			}
		}
	}
	ASSERT(cbDeleteCount==cbGoodCardCount) ;
	if (cbDeleteCount!=cbGoodCardCount) return false;

	//�����˿�
	BYTE cbCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) cbCardData[cbCardPos++]=cbTempCardData[i];
	}

	return true;
}

//�����˿�
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//����׼��
	BYTE cbCardData[CountArray(m_cbCardData)];
	CopyMemory(cbCardData,m_cbCardData,sizeof(m_cbCardData));

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbBufferCount-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	return;
}
//�����ֲ�
VOID CGameLogic::AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing)
{
	//���ñ���
	ZeroMemory(&Distributing,sizeof(Distributing));

	//���ñ���
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbCardData[i]==0) continue;

		//��ȡ����
		BYTE cbCardColor=GetCardColor(cbCardData[i]);
		BYTE cbCardValue=GetCardValue(cbCardData[i]);

		//�ֲ���Ϣ
		Distributing.cbCardCount++;
		Distributing.cbDistributing[cbCardValue-1][cbIndexCount]++;
		Distributing.cbDistributing[cbCardValue-1][cbCardColor>>4]++;
	}

	return;
}
//�����˿�
BYTE CGameLogic::MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex)
{
	return (cbColorIndex<<4)|(cbValueIndex+1);
}

//ɾ���˿�
bool CGameLogic::RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	ASSERT(cbRemoveCount<=cbCardCount);
	if(cbRemoveCount>cbCardCount)
		return false ;

	//�������
	BYTE cbDeleteCount=0,cbTempCardData[MAX_COUNT];
	if (cbCardCount>CountArray(cbTempCardData)) return false;
	CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//�����˿�
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		for (BYTE j=0;j<cbCardCount;j++)
		{
			if (cbRemoveCard[i]==cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j]=0;
				break;
			}
		}
	}
	if (cbDeleteCount!=cbRemoveCount) return false;

	//�����˿�
	BYTE cbCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) cbCardData[cbCardPos++]=cbTempCardData[i];
	}

	return true;
}

//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	//��ȡ����
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//��Ч�ж�
	if ((cbCardData==0x4E)||(cbCardData==0x4F)) return true;
	if ((cbCardColor<=0x30)&&(cbCardValue>=0x01)&&(cbCardValue<=0x0D)) return true;

	return false;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	if(cbCardData==0) return 0;
	//�˿�����
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);
	ASSERT(cbCardValue>0 && cbCardValue<=(MASK_VALUE&0x4f)) ;

	//ת����ֵ
	if (cbCardColor==0x40) return cbCardValue+2;
	return (cbCardValue<=2)?(cbCardValue+13):cbCardValue;
}

//�Ա��˿�
bool CGameLogic::CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{
	//��ȡ����
	BYTE cbNextType=GetCardType(cbNextCard,cbNextCount);
	BYTE cbFirstType=GetCardType(cbFirstCard,cbFirstCount);

	//�����ж�
	if (cbNextType==CT_ERROR) return false;

	//ը���ж�
	if ((cbFirstType<CT_BOMB_3)&&(cbNextType>=CT_BOMB_3)) return true;
	if ((cbFirstType>=CT_BOMB_3)&&(cbNextType<CT_BOMB_3)) return false;

	//ը���Ƚ�
	if((cbFirstType>=CT_BOMB_3)&&(cbNextType>=CT_BOMB_3))
	{
		if(cbFirstType!=cbNextType) return cbNextType>cbFirstType;
		if(cbNextCount!=cbFirstCount) return cbNextCount>cbFirstCount;
	}

	if(cbFirstCount!=cbNextCount) return false;

	//��ʼ�Ա�
	switch (cbNextType)
	{
	case CT_SINGLE://����
		{
			//��ȡ��ֵ
			BYTE cbNextLogicValue=GetCardLogicValue(cbNextCard[0]);
			BYTE cbFirstLogicValue=GetCardLogicValue(cbFirstCard[0]);

			//�Ա��˿�
			return (cbNextLogicValue==cbFirstLogicValue+1|| ((cbNextLogicValue>cbFirstLogicValue)&&cbNextLogicValue==15));
		}
	case CT_DOUBLE://����
		{
			//��ȡ��ֵ
			BYTE cbNextLogicValue=GetCardLogicValue(cbNextCard[1]);
			BYTE cbFirstLogicValue=GetCardLogicValue(cbFirstCard[1]);

			bool bFirstKing=(GetCardColor(cbFirstCard[0])==0x40?true:false);
			bool bNextKing=(GetCardColor(cbNextCard[0])==0x40?true:false);

			//ͬ���ʱ��Ƚ���
			if(cbNextLogicValue==cbFirstLogicValue)
			{
				//��������
				if(bNextKing==true && bFirstKing==true) return false;
				//�ȳ�������
				if(bFirstKing==true && bNextKing==false) return true;
				//���������
				if(bFirstKing==false && bNextKing==true) return false;
				//����������
				if(bFirstKing==false && bNextKing==false) return false;
			}

			//�Ա��˿�
			return (cbNextLogicValue==cbFirstLogicValue+1|| ((cbNextLogicValue>cbFirstLogicValue)&&cbNextLogicValue==15));
		}
	case CT_SINGLE_LINE:
	case CT_DOUBLE_LINE:
		{
			//��ȡ��ֵ
			BYTE cbNextLogicValue=GetCardLogicValue(cbNextCard[cbNextCount-1]);
			BYTE cbFirstLogicValue=GetCardLogicValue(cbFirstCard[cbFirstCount-1]);

			//�Ա��˿�
			return cbNextLogicValue==cbFirstLogicValue+1;
		}
	case CT_BOMB_3:
	case CT_BOMB_3A:
	case CT_BOMB_4:
	case CT_BOMB_4A:
		{
			//��ȡ��ֵ
			BYTE cbNextLogicValue=GetCardLogicValue(cbNextCard[cbNextCount-1]);
			BYTE cbFirstLogicValue=GetCardLogicValue(cbFirstCard[cbFirstCount-1]);

			//�Ա��˿�
			return cbNextLogicValue>cbFirstLogicValue;
		}
	default:
		return false;

	}

	return false;
}

//�����˿�
bool CGameLogic::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
	//���ý��
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	//�˿˷���
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//��������
		BYTE cbSameCount=1;
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);
		if(cbLogicValue<=0) 
			return false;

		//����ͬ��
		for (BYTE j=i+1;j<cbCardCount;j++)
		{
			//��ȡ�˿�
			if (GetCardLogicValue(cbCardData[j])!=cbLogicValue) break;

			//���ñ���
			cbSameCount++;
		}

		//���ý��
		switch (cbSameCount)
		{
		case 1:		//����
			{
				BYTE cbIndex=AnalyseResult.cbSignedCount++;
				AnalyseResult.cbSignedCardData[cbIndex*cbSameCount]=cbCardData[i];
				break;
			}
		case 2:		//����
			{
				BYTE cbIndex=AnalyseResult.cbDoubleCount++;
				AnalyseResult.cbDoubleCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbDoubleCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				break;
			}
		case 3:		//����
			{
				BYTE cbIndex=AnalyseResult.cbThreeCount++;
				AnalyseResult.cbThreeCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbThreeCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbThreeCardData[cbIndex*cbSameCount+2]=cbCardData[i+2];
				break;
			}
		case 4:		//����
			{
				BYTE cbIndex=AnalyseResult.cbFourCount++;
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount+2]=cbCardData[i+2];
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount+3]=cbCardData[i+3];
				break;
			}
		case 5:		//����
			{
				BYTE cbIndex=AnalyseResult.cbFiveCount++;
				AnalyseResult.cbFiveCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbFiveCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbFiveCardData[cbIndex*cbSameCount+2]=cbCardData[i+2];
				AnalyseResult.cbFiveCardData[cbIndex*cbSameCount+3]=cbCardData[i+3];
				AnalyseResult.cbFiveCardData[cbIndex*cbSameCount+4]=cbCardData[i+4];
				break;
			}
		case 6:		//����
			{
				BYTE cbIndex=AnalyseResult.cbSixCount++;
				AnalyseResult.cbSixCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbSixCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbSixCardData[cbIndex*cbSameCount+2]=cbCardData[i+2];
				AnalyseResult.cbSixCardData[cbIndex*cbSameCount+3]=cbCardData[i+3];
				AnalyseResult.cbSixCardData[cbIndex*cbSameCount+4]=cbCardData[i+4];
				AnalyseResult.cbSixCardData[cbIndex*cbSameCount+5]=cbCardData[i+5];
				break;
			}
		case 7:		//����
			{
				BYTE cbIndex=AnalyseResult.cbSevenCount++;
				AnalyseResult.cbSevenCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbSevenCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbSevenCardData[cbIndex*cbSameCount+2]=cbCardData[i+2];
				AnalyseResult.cbSevenCardData[cbIndex*cbSameCount+3]=cbCardData[i+3];
				AnalyseResult.cbSevenCardData[cbIndex*cbSameCount+4]=cbCardData[i+4];
				AnalyseResult.cbSevenCardData[cbIndex*cbSameCount+5]=cbCardData[i+5];
				AnalyseResult.cbSevenCardData[cbIndex*cbSameCount+6]=cbCardData[i+6];
				break;
			}
		case 8:		//����
			{
				BYTE cbIndex=AnalyseResult.cbEightCount++;
				AnalyseResult.cbEightCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbEightCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbEightCardData[cbIndex*cbSameCount+2]=cbCardData[i+2];
				AnalyseResult.cbEightCardData[cbIndex*cbSameCount+3]=cbCardData[i+3];
				AnalyseResult.cbEightCardData[cbIndex*cbSameCount+4]=cbCardData[i+4];
				AnalyseResult.cbEightCardData[cbIndex*cbSameCount+5]=cbCardData[i+5];
				AnalyseResult.cbEightCardData[cbIndex*cbSameCount+6]=cbCardData[i+6];
				AnalyseResult.cbEightCardData[cbIndex*cbSameCount+7]=cbCardData[i+7];
				break;
			}
		}

		//��������
		i+=cbSameCount-1;
	}

	return true;
}
//����˿�
BYTE CGameLogic::GetRandomCard(void)
{
	size_t cbIndex = rand()%(sizeof(m_cbCardData)) ;
	return m_cbCardData[cbIndex] ;
}
//����˿�
bool CGameLogic::MagicCardData(BYTE cbCardData[], BYTE cbCardCount, BYTE cbResultCard[MAX_COUNT], BYTE cbTurnCardData[MAX_COUNT],BYTE cbTurnCardCount)
{
	//����Ƿ���Ҫ����//ֻ��Ҫ��˳��
	BYTE bTurnType=CT_ERROR;
	if(cbTurnCardCount>0)
	{
		bTurnType=GetCardType(cbTurnCardData,cbTurnCardCount);
		if(bTurnType!=CT_DOUBLE_LINE && bTurnType!=CT_SINGLE_LINE)
		{
			CopyMemory(cbResultCard,cbCardData,cbCardCount*sizeof(BYTE));
			return false;		
		}
	}

	//��������
	BYTE cbNormalCard[MAX_COUNT];
	BYTE cbMagicCardCount=0,cbNormalCardCount=0;

	//���׼��
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//�˿�����
		BYTE cbCardColor=GetCardColor(cbCardData[i]);

		//���ͳ��
		if (cbCardColor==0x40) cbMagicCardCount++;
		else cbNormalCard[cbNormalCardCount++]=cbCardData[i];
	}

	//ʧ���ж�
	if ((cbMagicCardCount==0)||(cbNormalCardCount==0))
	{
		CopyMemory(cbResultCard,cbCardData,cbCardCount*sizeof(BYTE));
		return false;
	}

	//�Լ�����
	if(bTurnType==CT_ERROR)
	{
		BYTE bTempCard[MAX_COUNT];
		ZeroMemory(bTempCard,sizeof(bTempCard));

		//1���ı���
		if(cbMagicCardCount==1)
		{
			for(BYTE i=14;i>0;i--) 
			{
				if(i==14) bTempCard[0]=1;
				else bTempCard[0]=i;
				CopyMemory(&bTempCard[1],cbNormalCard,sizeof(BYTE)*cbNormalCardCount);
				SortCardList(bTempCard,cbCardCount,ST_ORDER);
				if(GetCardType(bTempCard,cbCardCount)!=CT_ERROR)
				{
					CopyMemory(cbResultCard,bTempCard,sizeof(BYTE)*cbCardCount);
					return true;
				}
			}
		}
		//2���ı���
		if(cbMagicCardCount==2)
		{
			for(BYTE i=14;i>0;i--) for(BYTE j=14;j>0;j--)
			{
				if(i==14) bTempCard[0]=1;
				else bTempCard[0]=i;
				if(j==14) bTempCard[1]=1;
				else bTempCard[1]=j;
				CopyMemory(&bTempCard[2],cbNormalCard,sizeof(BYTE)*cbNormalCardCount);
				SortCardList(bTempCard,cbCardCount,ST_ORDER);
				if(GetCardType(bTempCard,cbCardCount)!=CT_ERROR)
				{
					CopyMemory(cbResultCard,bTempCard,sizeof(BYTE)*cbCardCount);
					return true;
				}
			}
		}
	}

	//��ȡ��׼��ֵ
	BYTE cbBaseValue=GetCardLogicValue(cbTurnCardData[0]);
	//A���ϵ�˳����󣬷���
	if(cbBaseValue==14)
	{
		CopyMemory(cbResultCard,cbCardData,cbCardCount*sizeof(BYTE));
		return false;		
	}

	//�����ֲ�
	tagDistributing Distributing;
	AnalysebDistributing(cbNormalCard,cbNormalCardCount,Distributing);
	//�ƶ�A����
	CopyMemory(Distributing.cbDistributing[13],Distributing.cbDistributing[0],sizeof(Distributing.cbDistributing[13]));

	//�������˳
	if(bTurnType==CT_SINGLE_LINE)
	{
		BYTE bNeedCount=0;
		//�����Ҫ��ĸ���
		for(BYTE i=cbBaseValue;i>(cbBaseValue-cbTurnCardCount);i--)
		{
			if(Distributing.cbDistributing[i][5]==0) bNeedCount++;
		}
		//������Ļ����ж�
		if(bNeedCount!=cbMagicCardCount)
		{
			CopyMemory(cbResultCard,cbCardData,cbCardCount*sizeof(BYTE));
			return false;
		}
		//������
		CopyMemory(cbResultCard,cbNormalCard,sizeof(BYTE)*cbNormalCardCount);
		BYTE bTempCount=cbNormalCardCount;

		//���ӱ���
		for(BYTE i=cbBaseValue;i>(cbBaseValue-cbTurnCardCount);i--)
		{
			if(Distributing.cbDistributing[i][5]==0)
			{
				if(i==13) cbResultCard[bTempCount++]=1;
				else cbResultCard[bTempCount++]= i+1;
			}
		}
		SortCardList(cbResultCard,cbTurnCardCount,ST_ORDER);
		return true;
	}

	//��������
	if(bTurnType==CT_DOUBLE_LINE)
	{
		BYTE bTempCard[MAX_COUNT];
		ZeroMemory(bTempCard,sizeof(bTempCard));

		//1���ı���
		if(cbMagicCardCount==1)
		{
			for(BYTE i=14;i>0;i--) 
			{
				if(i==14) bTempCard[0]=1;
				else bTempCard[0]=i;
				CopyMemory(&bTempCard[1],cbNormalCard,sizeof(BYTE)*cbNormalCardCount);
				SortCardList(bTempCard,cbCardCount,ST_ORDER);
				if(CompareCard(cbTurnCardData,bTempCard,cbTurnCardCount,cbCardCount)==true)
				{
					CopyMemory(cbResultCard,bTempCard,sizeof(BYTE)*cbCardCount);
					return true;
				}
			}
		}
		//2���ı���
		if(cbMagicCardCount==2)
		{
			for(BYTE i=14;i>0;i--) for(BYTE j=14;j>0;j--)
			{
				if(i==14) bTempCard[0]=1;
				else bTempCard[0]=i;
				if(j==14) bTempCard[1]=1;
				else bTempCard[1]=j;
				CopyMemory(&bTempCard[2],cbNormalCard,sizeof(BYTE)*cbNormalCardCount);
				SortCardList(bTempCard,cbCardCount,ST_ORDER);
				if(CompareCard(cbTurnCardData,bTempCard,cbTurnCardCount,cbCardCount)==true)
				{
					CopyMemory(cbResultCard,bTempCard,sizeof(BYTE)*cbCardCount);
					return true;
				}
			}
		}
	}

	//�����˿�
	CopyMemory(cbResultCard,cbCardData,cbCardCount*sizeof(BYTE));

	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��������
bool CGameLogic::SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult)
{
	//�׳���
	if(cbTurnCardCount==0)
	{
		OutCardResult.cbCardCount=1;
		OutCardResult.cbResultCard[0]=cbHandCardData[cbHandCardCount-1];

		return true;
	}

	//�������
	BYTE bTurnType=GetCardType(cbTurnCardData,cbTurnCardCount);
	BYTE bTurnLogicValue=GetCardLogicValue(cbTurnCardData[0]);

	switch(bTurnType)
	{
	case CT_SINGLE:
		{
			//Ѱ�Ҵ�һ�����
			for(BYTE i=0;i<cbHandCardCount;i++)
			{
				if(GetCardLogicValue(cbHandCardData[i])==bTurnLogicValue+1 && GetCardColor(cbHandCardData[i])!=0x40 )
				{
					OutCardResult.cbCardCount=1;
					OutCardResult.cbResultCard[0]=cbHandCardData[i];
					return true;
				}
			}
			//Ѱ��2
			for(BYTE i=0;i<cbHandCardCount;i++)
			{
				if(GetCardLogicValue(cbHandCardData[i])==15 && bTurnLogicValue!=15)
				{
					OutCardResult.cbCardCount=1;
					OutCardResult.cbResultCard[0]=cbHandCardData[i];
					return true;
				}
			}
			break;
		}
	case CT_DOUBLE:
		{
			bTurnLogicValue=GetCardLogicValue(cbTurnCardData[1]);
			//ȡ������
			BYTE bOutIndex=GetCardValue(cbTurnCardData[1])-1;
			//����2
			if(bOutIndex==1) break;

			//��������
			tagDistributing Distribute;
			AnalysebDistributing(cbHandCardData,cbHandCardCount,Distribute);
			//�ƶ�A
			CopyMemory(Distribute.cbDistributing[13],Distribute.cbDistributing[0],sizeof(Distribute.cbDistributing[0]));

			//����
			bool bKingDouble=false;
			if(GetCardColor(cbTurnCardData[0])==0x40) bKingDouble=true;
			if(bKingDouble)
			{
				//��������
				if(Distribute.cbDistributing[bOutIndex+1][5]>=2 && (bOutIndex)!=14)
				{
					OutCardResult.cbCardCount=0;
					for(BYTE j=0;j<4;j++)
					{
						if(Distribute.cbDistributing[bOutIndex][j]>0)
						{			
							OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=MakeCardData(bOutIndex,j);
							if(OutCardResult.cbCardCount==2) return true;
						}					
					}
				}
				//������2
				if(Distribute.cbDistributing[1][5]>=2)
				{
					OutCardResult.cbCardCount=0;
					for(BYTE j=0;j<4;j++)
					{
						if(Distribute.cbDistributing[1][j]>0)
						{			
							OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=MakeCardData(1,j);
							if(OutCardResult.cbCardCount==2) return true;
						}					
					}
				}	
			}

			//��������
			if(Distribute.cbDistributing[bOutIndex+1][5]>=2 && (bOutIndex+1)!=14)
			{
				OutCardResult.cbCardCount=0;
				for(BYTE j=0;j<4;j++)
				{
					if(Distribute.cbDistributing[bOutIndex+1][j]>0)
					{			
						OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=MakeCardData(bOutIndex+1,j);
						if(OutCardResult.cbCardCount==2) return true;
					}					
				}
			}
			//������2
			if(Distribute.cbDistributing[1][5]>=2)
			{
				OutCardResult.cbCardCount=0;
				for(BYTE j=0;j<4;j++)
				{
					if(Distribute.cbDistributing[1][j]>0)
					{			
						OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=MakeCardData(1,j);
						if(OutCardResult.cbCardCount==2) return true;
					}					
				}
			}
			//��������
			if( (GetCardColor(cbHandCardData[0])==0x40) && Distribute.cbDistributing[bOutIndex+1][5]==1 && bOutIndex!=13)
			{

				for(BYTE j=0;j<4;j++)
				{
					if(Distribute.cbDistributing[bOutIndex+1][j]>0)
					{			
						if((bOutIndex+1)==13) OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=MakeCardData(0,j);
						else OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=MakeCardData(bOutIndex+1,j);
						break;
					}					
				}
				//���ӱ���
				OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbHandCardData[0];
				if(CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount)==true) return true;
			}
			//�������ƶ�2
			if( (GetCardColor(cbHandCardData[0])==0x40) && Distribute.cbDistributing[1][5]==1)
			{
				if(Distribute.cbDistributing[14][5]>0)
				{
					OutCardResult.cbCardCount=0;
					for(BYTE j=0;j<4;j++)
					{
						if(Distribute.cbDistributing[1][j]>0)
						{			
							OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=MakeCardData(1,j);
						}					
					}

					//���ӱ���
					OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbHandCardData[0];
					if(CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount)==true) return true;
				}
			}
			break;
		}
	case CT_SINGLE_LINE:
	case CT_DOUBLE_LINE:
		{
			//ȡ�õ�ǰ������
			BYTE bIndex=GetCardValue(cbTurnCardData[0])-1;
			//A�ⶥ
			if(bIndex==0) break;

			//��������
			tagDistributing Distribute;
			AnalysebDistributing(cbHandCardData,cbHandCardCount,Distribute);
			//�ƶ�A
			CopyMemory(Distribute.cbDistributing[13],Distribute.cbDistributing[0],sizeof(Distribute.cbDistributing[0]));
			//�����
			ZeroMemory(Distribute.cbDistributing[14],sizeof(Distribute.cbDistributing[14]));

			//��������
			BYTE bLinkCount=0;
			BYTE bCellCount=1;
			if(bTurnType==CT_SINGLE_LINE) bLinkCount=cbTurnCardCount;
			if(bTurnType==CT_DOUBLE_LINE) {bLinkCount=cbTurnCardCount/2;bCellCount=2;}

			//��ʼλ��
			BYTE bStartIndex=bIndex-bLinkCount+2;
			if(bStartIndex<3) break;

			//�ж��Ƿ��������
			bool bHaveLink=true;
			for(BYTE i=bStartIndex;i<bStartIndex+bLinkCount;i++)
			{
				if(Distribute.cbDistributing[i][5]<bCellCount) 
				{
					bHaveLink=false;
					break;
				}
			}
			//��������
			if(bHaveLink==true)
			{
				OutCardResult.cbCardCount=0;
				for(BYTE i=bStartIndex;i<bStartIndex+bLinkCount;i++)
				{
					BYTE bGetCellCount=0;
					for(BYTE j=0;j<4;j++)
					{
						if(Distribute.cbDistributing[i][j]>0)
						{			
							OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=MakeCardData(i,j);
							bGetCellCount++;
							if(bGetCellCount==bCellCount) break;
						}					
					}
				}
				if(OutCardResult.cbCardCount==cbTurnCardCount) return true;
			}
			break;
		}
	default:
		break;

	}

	//����3������ը��
	if(bTurnType<=CT_BOMB_3)
	{
		//ȡ�õ�ǰ������
		BYTE bIndex=GetCardValue(cbTurnCardData[0])-1;

		//��������
		tagDistributing Distribute;
		AnalysebDistributing(cbHandCardData,cbHandCardCount,Distribute);
		//�ƶ�A
		CopyMemory(Distribute.cbDistributing[13],Distribute.cbDistributing[0],sizeof(Distribute.cbDistributing[0]));
		//�����
		BYTE bKingCount=0;
		if(GetCardColor(cbHandCardData[0])==0x40) bKingCount=1;
		if(GetCardColor(cbHandCardData[1])==0x40) bKingCount=2;
		ZeroMemory(Distribute.cbDistributing[14],sizeof(Distribute.cbDistributing[14]));

		if(bKingCount>0)
		{
			for(BYTE i=0;i<14;i++)
			{
				if(Distribute.cbDistributing[i][5]==2)
				{
					if(i>bIndex || bTurnType<CT_BOMB_3)
					{
						for(BYTE j=0;j<4;j++)
						{
							for (BYTE k=0;k<Distribute.cbDistributing[i][j];k++)
							{
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=MakeCardData(i,j);
							}					
						}
						OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbHandCardData[0];
						if(CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount)) return true;	
					}
				}
			}	
		}
	}

	//����3����Ӳը��
	if(bTurnType<=CT_BOMB_3A)
	{
		//ȡ�õ�ǰ������
		BYTE bIndex=GetCardValue(cbTurnCardData[0])-1;

		//��������
		tagDistributing Distribute;
		AnalysebDistributing(cbHandCardData,cbHandCardCount,Distribute);
		//�ƶ�A
		CopyMemory(Distribute.cbDistributing[13],Distribute.cbDistributing[0],sizeof(Distribute.cbDistributing[0]));
		//�����
		ZeroMemory(Distribute.cbDistributing[14],sizeof(Distribute.cbDistributing[14]));

		for(BYTE i=0;i<14;i++)
		{
			if(Distribute.cbDistributing[i][5]==3)
			{
				if(i>bIndex || bTurnType<CT_BOMB_3)
				{
					for(BYTE j=0;j<4;j++)
					{
						for (BYTE k=0;k<Distribute.cbDistributing[i][j];k++)
						{
							OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=MakeCardData(i,j);
						}					
					}
					if(CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount)) return true;
				}
			}
		}		
	}

	//����4������ը��
	if(bTurnType<=CT_BOMB_4A)
	{
		//ȡ�õ�ǰ������
		BYTE bIndex=GetCardValue(cbTurnCardData[0])-1;

		//��������
		tagDistributing Distribute;
		AnalysebDistributing(cbHandCardData,cbHandCardCount,Distribute);
		//�ƶ�A
		CopyMemory(Distribute.cbDistributing[13],Distribute.cbDistributing[0],sizeof(Distribute.cbDistributing[0]));
		//�����
		BYTE bKingCount=0;
		if(GetCardColor(cbHandCardData[0])==0x40) bKingCount=1;
		if(GetCardColor(cbHandCardData[1])==0x40) bKingCount=2;
		ZeroMemory(Distribute.cbDistributing[14],sizeof(Distribute.cbDistributing[14]));

		if(bKingCount==1)
		{
			for(BYTE i=0;i<14;i++)
			{
				if(Distribute.cbDistributing[i][5]==3)
				{
					if(i>bIndex || bTurnType<CT_BOMB_4)
					{
						for(BYTE j=0;j<4;j++)
						{
							for (BYTE k=0;k<Distribute.cbDistributing[i][j];k++)
							{
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=MakeCardData(i,j);
							}					
						}
						OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbHandCardData[0];
						if(CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount)) return true;
					}
				}
			}
		}
	}

	//����4����Ӳը��
	if(bTurnType<=CT_BOMB_4A)
	{
		//ȡ�õ�ǰ������
		BYTE bIndex=GetCardValue(cbTurnCardData[0])-1;

		//��������
		tagDistributing Distribute;
		AnalysebDistributing(cbHandCardData,cbHandCardCount,Distribute);
		//�ƶ�A
		CopyMemory(Distribute.cbDistributing[13],Distribute.cbDistributing[0],sizeof(Distribute.cbDistributing[0]));
		//�����
		ZeroMemory(Distribute.cbDistributing[14],sizeof(Distribute.cbDistributing[14]));

		for(BYTE i=0;i<14;i++)
		{
			if(Distribute.cbDistributing[i][5]>=4)
			{
				if(i>bIndex || bTurnType<CT_BOMB_4)
				{
					for(BYTE j=0;j<4;j++)
					{
						for (BYTE k=0;k<Distribute.cbDistributing[i][j];k++)
						{
							OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=MakeCardData(i,j);
						}					
					}
					if(CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount)) return true;
				}
			}
		}
	}

	//����˫��ը��
	if(cbHandCardCount>=2 &&  cbHandCardData[0]==0x4F && cbHandCardData[1]==0x4E)
	{
		if(bTurnType<CT_BOMB_KING)
		{
			OutCardResult.cbCardCount=2;
			OutCardResult.cbResultCard[0]=cbHandCardData[0];
			OutCardResult.cbResultCard[1]=cbHandCardData[1];
			if(CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount)) return true;
		}
	}	

	//����˫��ը�� 5����
	if(bTurnType==CT_BOMB_KING) 
	{
		//��������
		tagDistributing Distribute;
		AnalysebDistributing(cbHandCardData,cbHandCardCount,Distribute);
		//�ƶ�A
		CopyMemory(Distribute.cbDistributing[13],Distribute.cbDistributing[0],sizeof(Distribute.cbDistributing[0]));
		//�����
		BYTE bKingCount=0;
		if(GetCardColor(cbHandCardData[0])==0x40) bKingCount=1;
		if(GetCardColor(cbHandCardData[1])==0x40) bKingCount=2;
		ZeroMemory(Distribute.cbDistributing[14],sizeof(Distribute.cbDistributing[14]));

		if(bKingCount>=1)
		{
			for(BYTE i=0;i<14;i++)
			{
				if(Distribute.cbDistributing[i][5]==4)
				{
					if(bTurnType<CT_BOMB_KING)
					{
						for(BYTE j=0;j<4;j++)
						{
							for (BYTE k=0;k<Distribute.cbDistributing[i][j];k++)
							{
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=MakeCardData(i,j);
							}					
						}
						OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbHandCardData[0];
						if(CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount)) return true;
					}
				}
			}
		}
	}

	//����˫��ը�� 6����
	if(bTurnType==CT_BOMB_KING) 
	{
		//��������
		tagDistributing Distribute;
		AnalysebDistributing(cbHandCardData,cbHandCardCount,Distribute);
		//�ƶ�A
		CopyMemory(Distribute.cbDistributing[13],Distribute.cbDistributing[0],sizeof(Distribute.cbDistributing[0]));
		//�����
		BYTE bKingCount=0;
		if(GetCardColor(cbHandCardData[0])==0x40) bKingCount=1;
		if(GetCardColor(cbHandCardData[1])==0x40) bKingCount=2;
		ZeroMemory(Distribute.cbDistributing[14],sizeof(Distribute.cbDistributing[14]));

		if(bKingCount==2)
		{
			for(BYTE i=0;i<14;i++)
			{
				if(Distribute.cbDistributing[i][5]==4)
				{
					if( bTurnType<CT_BOMB_KING)
					{
						for(BYTE j=0;j<4;j++)
						{
							for (BYTE k=0;k<Distribute.cbDistributing[i][j];k++)
							{
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=MakeCardData(i,j);
							}					
						}
						OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbHandCardData[0];
						OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=cbHandCardData[1];
						if(CompareCard(cbTurnCardData,OutCardResult.cbResultCard,cbTurnCardCount,OutCardResult.cbCardCount)) return true;
					}
				}
			}
		}
	}

	return false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGameLogic::SetCanOutSingleKing(bool bCanOut)
{
	m_bCanOutSingleKing=bCanOut;
	return;
}
