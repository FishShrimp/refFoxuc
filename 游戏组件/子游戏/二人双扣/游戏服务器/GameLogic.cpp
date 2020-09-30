#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

//��������
const BYTE cbIndexCount=5;
/*
//�˿�����
const BYTE	CGameLogic::m_cbCardData[FULL_COUNT]=
{
	     0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,			//���� A - K
		 0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,			//÷�� A - K
		 0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,			//���� A - K
	0x31,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
};
*/

//�˿�����

//����λ��ʾ���ͼ���,����λ��ʾ�õȼ������ڲ��Ƿ��пɱ��ԣ��ַ���ȱ�ʾ�пɱ���
const BYTE  CGameLogic::m_cbCardGrade[11]={0x00,0x11,0x12,0x13,0x14,0x15,0x16,0x20,0x20,0x20,0x20};

const BYTE	CGameLogic::m_cbCardData[FULL_COUNT]=
{
	0x01,0x11,0x21,0x31,
	0x02,0x12,0x22,0x32,			   
	0x03,0x13,0x23,0x33,		  
	0x04,0x14,0x24,0x34,		  
	0x05,0x15,0x25,0x35,	   
	0x06,0x16,0x26,0x36,
	0x07,0x17,0x27,0x37,
	0x08,0x18,0x28,0x38,
	0x09,0x19,0x29,0x39,
	0x0A,0x1A,0x2A,0x3A,
	0x0B,0x1B,0x2B,0x3B,
	0x0C,0x1C,0x2C,0x3C,
	0x0D,0x1D,0x2D,0x3D,0x4E,0x4F, //��С��    
	0x01,0x11,0x21,0x31,
	0x02,0x12,0x22,0x32,			   
	0x03,0x13,0x23,0x33,		  
	0x04,0x14,0x24,0x34,		  
	0x05,0x15,0x25,0x35,	   
	0x06,0x16,0x26,0x36,
	0x07,0x17,0x27,0x37,
	0x08,0x18,0x28,0x38,
	0x09,0x19,0x29,0x39,
	0x0A,0x1A,0x2A,0x3A,
	0x0B,0x1B,0x2B,0x3B,
	0x0C,0x1C,0x2C,0x3C,
	0x0D,0x1D,0x2D,0x3D,0x4E,0x4F //��С��                                                            
}; 

//ը��Ȩֵת���� ��8λ��ʾ����ը��,��8λ��ʾ������ը������
const BYTE  CGameLogic::m_cbTransTable[9][9]=
{
	0x00,0x03,0x04,0x35,0x46,0x57,0x60,0x00,0x00,
	0x00,0x00,0x02,0x03,0x34,0x45,0x50,0x00,0x00,
	0x00,0x00,0x00,0x02,0x03,0x34,0x40,0x50,0x00,
	0x00,0x00,0x00,0x00,0x02,0x03,0x30,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x30,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x30,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}

//�����˿�
VOID CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//��Ŀ����
	if (cbCardCount==0) return;

	//ת����ֵ
	BYTE cbSortValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) 
	{
		cbSortValue[i]=GetCardLogicValue(cbCardData[i]);
	}

	//�������
	bool bSorted=true;
	BYTE cbSwitchData=0,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			if ((cbSortValue[i]<cbSortValue[i+1])||
				((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//���ñ�־
				bSorted=false;

				//�˿�����
				cbSwitchData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbSwitchData;

				//����Ȩλ
				cbSwitchData=cbSortValue[i];
				cbSortValue[i]=cbSortValue[i+1];
				cbSortValue[i+1]=cbSwitchData;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	return;
}

BYTE CGameLogic::RandCardList(BYTE cbCardList[],BYTE cbBufferCount,BYTE cbMainCard,WORD cbOutPut[])
{
	//����׼��
	BYTE cbCardData[sizeof(BYTE)*FULL_COUNT];
	CopyMemory(cbCardData,cbCardList,sizeof(cbCardData));

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	BYTE cbOutCount=0;
	do
	{
		cbPosition=rand()%(cbBufferCount-cbRandCount);
		//��¼���
		if(cbCardData[cbPosition]==cbMainCard)
			cbOutPut[cbOutCount++]=cbRandCount;

		cbCardList[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
	} while (cbRandCount<cbBufferCount);
	return cbOutCount;
}


//�����˿�
VOID CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	BYTE cbPosition=0;
	BYTE cbRandCount=0;
	BYTE cbTempRandCount=0;
	//����׼��
	BYTE cbCardData[CountArray(m_cbCardData)];
	CopyMemory(cbCardData,m_cbCardData,sizeof(m_cbCardData));

	do
	{
		cbTempRandCount=0;
		cbPosition=rand()%(cbBufferCount-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
		cbTempRandCount++;
		while ((cbPosition<cbBufferCount-cbRandCount) && (cbTempRandCount<4))
		{
			cbPosition++;
			cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
			cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
			cbTempRandCount++;
		}

	} while (cbRandCount<cbBufferCount);


	//�������
	BYTE cbTemp=0;
	BYTE cbArray[4]={0,1,2,3};
	memcpy(cbCardData,cbCardBuffer,sizeof(cbCardData));
	for(int nIndex=0;nIndex<4;nIndex++)
	{
		cbTemp=rand()%(4-nIndex);
		memcpy(cbCardBuffer+nIndex*MAX_COUNT,cbCardData+cbArray[cbTemp]*MAX_COUNT,sizeof(BYTE)*MAX_COUNT);
		cbArray[cbTemp]=cbArray[3-nIndex];
	}
	return;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	ASSERT(cbRemoveCount<=cbCardCount);

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


BYTE CGameLogic::TransForLogicValue(BYTE cbCardData)
{
	if(cbCardData>=14)
		cbCardData-=13;

	//�����ɫ
	switch(rand()%4)
	{
		case 0:cbCardData=cbCardData|0x00;break;
		case 1:cbCardData=cbCardData|0x10;break;
		case 2:cbCardData=cbCardData|0x20;break;
		case 3:cbCardData=cbCardData|0x30;break;
	}
	return cbCardData;
}

BYTE CGameLogic::GetMaxBombGrade(BYTE *cbBomblist)
{
	BYTE cbMaxBombCrade=0;
	BYTE cbIndex=0;
	for(cbIndex=0;cbIndex<BOMB_TYPE_COUNT;cbIndex++)
	{
		if(cbBomblist[cbIndex]>0)
		{
			cbMaxBombCrade=cbIndex;
		}
	}
	return cbMaxBombCrade;
}

bool CGameLogic::TransBombGrade(BYTE cbBombList[])
{
	int nVIndex=0;
	int nHIndex=0;
	for(nHIndex=0;nHIndex<BOMB_TYPE_COUNT;nHIndex++)
	{
		for(nVIndex=BOMB_TYPE_COUNT-1;nVIndex>=nHIndex;nVIndex--)
		{
			BYTE cbTemp=m_cbTransTable[nHIndex][nVIndex];
			if(cbTemp>0&&(cbTemp&0x0F)>0)
			{
				if(cbBombList[nHIndex]>=(cbTemp&0x0F))
				{
					cbBombList[nHIndex]-=(cbTemp&0x0F);
					cbBombList[nVIndex]+=1;
				}
			}
		}
	}
	return true;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//�˿�����
	BYTE cbReturn=0;
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	if(cbCardColor==0x40)
		cbReturn=cbCardValue+2; 
	else
		cbReturn=(cbCardValue<=2)?(cbCardValue+13):cbCardValue;
	//ת����ֵ
	return cbReturn;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValueEx(BYTE cbCardData)
{
	//�˿�����
	BYTE cbReturn=0;
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	if(cbCardColor==0x40)
		cbReturn=cbCardValue+2; 
	else
	{
		cbReturn=(cbCardValue<=1)?(cbCardValue+13):cbCardValue;
	}
	//ת����ֵ
	return cbReturn;
}

//�Ա��˿�
bool CGameLogic::CompareCard(tagAnalyseResult& CurOutCard,tagAnalyseResult& PerOutCard)
{	
	//���Ҷ�Ӧ��Ч����
	bool bReturn=false;
	if(FindValidCard(CurOutCard,PerOutCard))
	{
		BYTE  cbPerType=PerOutCard.m_cbCardType;
		BYTE  cbCurType=CurOutCard.m_cbCardType;
		BYTE  cbPerMinCard=GetCardLogicValue(PerOutCard.m_CardStyle[PerOutCard.m_cbMinIndex].m_cbCard);
		BYTE  cbCurMinCard=GetCardLogicValue(CurOutCard.m_CardStyle[CurOutCard.m_cbMinIndex].m_cbCard);

		if((m_cbCardGrade[cbPerType]&0xF0)<(m_cbCardGrade[cbCurType]&0xF0))
		{
			bReturn=true;
		}
		else
		{
			if((m_cbCardGrade[cbPerType]&0xF0)==(m_cbCardGrade[cbCurType]&0xF0))
			{
				switch(cbPerType)
				{
					case CT_SINGLE:				//��������
					case CT_DOUBLE:				//��������
					case CT_THREE:				//��������
					case CT_SINGLE_LINK:		//��������
					case CT_DOUBLE_LINK:		//���ö���
					case CT_THREE_LINK:			//��������
						{
							//��Ŀ�ж�
							if (PerOutCard.m_cbCardCount==CurOutCard.m_cbCardCount)
								bReturn=(cbCurMinCard>cbPerMinCard)?true:false;
							break;
						}
					case CT_BOMB:
					case CT_BOMB_3W:
					case CT_BOMB_TW:
					case CT_BOMB_LINK:
						{
							
							if(CurOutCard.m_cbBombGrade!=PerOutCard.m_cbBombGrade)
							{
								bReturn=(CurOutCard.m_cbBombGrade>PerOutCard.m_cbBombGrade)?true:false;
							}
							else
							{
								//Ȩֵ��������жϵ����������
								if(CurOutCard.m_cbBombGrade==PerOutCard.m_cbBombGrade)
								{
									if(CurOutCard.m_CardStyle[CurOutCard.m_cbMinIndex].m_cbCount!=PerOutCard.m_CardStyle[PerOutCard.m_cbMinIndex].m_cbCount)
										bReturn=(CurOutCard.m_CardStyle[CurOutCard.m_cbMinIndex].m_cbCount>PerOutCard.m_CardStyle[PerOutCard.m_cbMinIndex].m_cbCount)?true:false;
									else
									{
										if(cbPerType==CT_BOMB_LINK)
											bReturn=(GetCardLogicValueEx(CurOutCard.m_CardStyle[CurOutCard.m_cbMaxIndex].m_cbCard)>GetCardLogicValueEx(PerOutCard.m_CardStyle[PerOutCard.m_cbMaxIndex].m_cbCard))?true:false;
										else
											bReturn=(GetCardLogicValue(CurOutCard.m_CardStyle[CurOutCard.m_cbMaxIndex].m_cbCard)>GetCardLogicValue(PerOutCard.m_CardStyle[PerOutCard.m_cbMaxIndex].m_cbCard))?true:false;
									}
								}

							}
							break;
						}
				}
			}
		}
	}
	
	return bReturn;
}

//��������
bool CGameLogic::SearchOutCard(const BYTE cbHandCardData[], BYTE cbHandCardCount, const BYTE cbTurnCardData[], BYTE cbTurnCardCount, tagOutCardResult & OutCardResult)
{
	//���ý��
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));
	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=cbHandCardCount;
	CopyMemory(cbCardData,cbHandCardData,sizeof(BYTE)*cbHandCardCount);
	//�����˿�
	SortCardList(cbCardData,cbCardCount);
	//��ȡ��ֵ
	BYTE cbLogicValue=GetCardLogicValue(cbCardData[cbCardCount-1]);

	//�����ж�
	BYTE cbSameCount=1;
	for (BYTE i=1;i<cbCardCount;i++)
	{
		if (GetCardLogicValue(cbCardData[cbCardCount-i-1])==cbLogicValue) cbSameCount++;
		else break;
	}
	//��ɴ���
	if (cbSameCount>1)
	{
		OutCardResult.cbCardCount=cbSameCount;
		for (BYTE j=0;j<cbSameCount;j++) OutCardResult.cbResultCard[j]=cbCardData[cbCardCount-1-j];
		return true;
	}
	else
	{
		if(cbSameCount==1)
		{
			//���ƴ���
			OutCardResult.cbCardCount=1;
			OutCardResult.cbResultCard[0]=cbCardData[cbCardCount-1];
		}
		else  return false;
	}
	return true;
}

bool CGameLogic::FindValidCard(tagAnalyseResult & CurOutCard,tagAnalyseResult&PerOutCard)
{
		tagAnalyseResult analyseCard;
		memcpy(&analyseCard,&CurOutCard,sizeof(tagAnalyseResult));
		
		BYTE cbPerCardType=PerOutCard.m_cbCardType;
		bool bReturn=false;
		for(BYTE cbIndex=cbPerCardType;cbIndex<11;cbIndex++)
		{
			//�Ƿ��ǲο����ͻ��߸߽�����
			if(cbIndex==cbPerCardType||(m_cbCardGrade[cbPerCardType]&0xF0)<(m_cbCardGrade[cbIndex]&0xF0))
			{
				memcpy(&CurOutCard,&analyseCard,sizeof(tagAnalyseResult));
				switch (cbIndex)
				{
					case CT_SINGLE:
						{
							if(CurOutCard.m_cbCardCount==1)
							{
								bReturn=true;
								CurOutCard.m_cbCardType=cbIndex;								
							}
							break;
						}
					case CT_DOUBLE:
						{
							if(CurOutCard.m_cbCardCount==2)
							{
								if(CurOutCard.m_cbCardCount==CurOutCard.m_nJockerCount)
								{
									bReturn=true;
									CurOutCard.m_cbCardType=cbIndex;
								}
								else
								{
									if(CurOutCard.m_CardStyle[CurOutCard.m_cbMinIndex].m_cbCount+CurOutCard.m_nJockerCount==2)
									{
										bReturn=true;
										CurOutCard.m_cbCardType=cbIndex;
									}
								}
							}
							break;
						}
					case CT_THREE:
						{
							if(CurOutCard.m_cbCardCount==3&&CurOutCard.m_CardStyle[CurOutCard.m_cbMinIndex].m_cbCount+CurOutCard.m_nJockerCount==3&&CurOutCard.m_nJockerCount<=2)
							{
								bReturn=true;
								CurOutCard.m_cbCardType=cbIndex;
							}
							break;
						}
					case CT_SINGLE_LINK:
						{
							if(bReturn=FindLinkCard(CurOutCard,1,5,true,false))
								CurOutCard.m_cbCardType=cbIndex;
							break;
						}
					case CT_DOUBLE_LINK:
						{
							if(bReturn=FindLinkCard(CurOutCard,2,3,true,false))
								CurOutCard.m_cbCardType=cbIndex;
							break;
						}
					case CT_THREE_LINK:
						{
							if(bReturn=FindLinkCard(CurOutCard,3,3,true,false))
								CurOutCard.m_cbCardType=cbIndex;
							break;
						}

					case CT_BOMB:
					case CT_BOMB_3W:
					case CT_BOMB_TW:
					case CT_BOMB_LINK: 
						{
							bReturn=IsBombCard(CurOutCard);
							break;
						}
				}
				if(bReturn) break;
			}
		}
					
	return bReturn;
}


#define  LEFT_BORDER  0
#define  RIGHT_BORDER 12
#define  ELEMT_COUNT  13

bool CGameLogic::FillCardCount(tagAnalyseResult&AnalyseResultEx,int nMinCount)
{
	//�������
	int nDifference=0;
	int nTempIndex=0;
	bool bFlags=true;

	tagAnalyseResult AnalyseResult;
	memcpy(&AnalyseResult,&AnalyseResultEx,sizeof(tagAnalyseResult));

	for(int nIndex=AnalyseResult.m_cbMinIndex;nIndex<=AnalyseResult.m_cbMaxIndex;nIndex++)
	{
		nTempIndex=nIndex%ELEMT_COUNT;
		nDifference=nMinCount-(int)AnalyseResult.m_CardStyle[nTempIndex].m_cbCount;
		if(nDifference<=AnalyseResult.m_nJockerCount)
		{
			if(nDifference>0)
			{
				if(AnalyseResult.m_CardStyle[nTempIndex].m_cbCount==0)
					AnalyseResult.m_nUnEqualCard++;

				AnalyseResult.m_CardStyle[nTempIndex].m_cbCount+=nDifference;
				AnalyseResult.m_nJockerCount-=nDifference;

				if(AnalyseResult.m_CardStyle[nTempIndex].m_cbCard==0)
					AnalyseResult.m_CardStyle[nTempIndex].m_cbCard=TransForLogicValue((AnalyseResult.m_cbMinIndex+1+nIndex)%ELEMT_COUNT);
			}
		}
		else
		{
			return false;
		}
	}
	AnalyseResult.m_nMaxCount=nMinCount;
	memcpy(&AnalyseResultEx,&AnalyseResult,sizeof(tagAnalyseResult));
	return true;
}




bool CGameLogic::FillCardSeque(tagAnalyseResult&AnalyseResult,int cbMinCount,int cbMinLink,bool bAllowCycle)
{
	//��������������������ͬ�����������Ƶ������򷵻�
	if(AnalyseResult.m_nUnEqualCard*cbMinCount>AnalyseResult.m_cbCardCount) return false;
	//��������
	BYTE cbMaxLong=AnalyseResult.m_nUnEqualCard+AnalyseResult.m_nJockerCount/cbMinCount;
	//���������󳤶�С����С���󳤶�
	if(cbMaxLong<cbMinLink) return false;

	//�����ѭ���޷���λ�ɹ�
	if((AnalyseResult.m_cbMinIndex+cbMaxLong)<AnalyseResult.m_cbMaxIndex)
	{
		if(bAllowCycle)	//�Ƿ�������ѭ����λ
		{
			//�ҵ���ʼ��
			int nCoverCount=AnalyseResult.m_nUnEqualCard;
			BYTE cbStartIndex=(AnalyseResult.m_cbMinIndex+ELEMT_COUNT-cbMaxLong)%ELEMT_COUNT;
			for(BYTE cbIndex=cbStartIndex;cbIndex<cbStartIndex+ELEMT_COUNT;cbIndex++)
			{
				if(AnalyseResult.m_CardStyle[cbIndex%ELEMT_COUNT].m_cbCount>0)
				{
					//��������
					if(nCoverCount==AnalyseResult.m_nUnEqualCard)
						AnalyseResult.m_cbMinIndex=cbIndex;
					if((--nCoverCount)==0)
					{
						AnalyseResult.m_cbMaxIndex=cbIndex;
						break;
					}	
				}
			}
		}
	}

	//������ֹ��
	BYTE cbCurLinkLong=AnalyseResult.m_cbMaxIndex-AnalyseResult.m_cbMinIndex+1;
	//��������
	if(cbCurLinkLong<cbMinLink)
	{
		for(BYTE cbIndex=0;cbIndex<cbMinLink-cbCurLinkLong;cbIndex++)
		{
			//���Ҳ���
			if(AnalyseResult.m_cbMaxIndex+1<RIGHT_BORDER)
				AnalyseResult.m_cbMaxIndex++;
			else //���󲹳�
				AnalyseResult.m_cbMinIndex=(((AnalyseResult.m_cbMinIndex+ELEMT_COUNT)-1)%ELEMT_COUNT);
		}
	}
	//����λ
	return FillCardCount(AnalyseResult,cbMinCount);
}

//�鿴�Ƿ����� �����ڼ����ӣ�ͬ���ţ����öԣ���ͬ����,�������Ƶ�ը����˳�ӵ�����
bool CGameLogic::FindLinkCard(tagAnalyseResult & AnalyseResult,BYTE cbMinCardCount,BYTE cbMinLinkLong,bool bLockCount,bool bAllowCycle)
{
	//�����ͬ����Ϊ��
	if(AnalyseResult.m_nUnEqualCard==0) return false;

	if(bLockCount)
	{
		if(AnalyseResult.m_nMaxCount>cbMinCardCount) return false;
	}

	//�Ƿ�������̳��Ⱥ�����������Ҫ��
	if(!FillCardSeque(AnalyseResult,cbMinCardCount,cbMinLinkLong,bAllowCycle))
		return false;

	//�����������ҷ��ֻ��б��ƣ�������ص�����ϵĶ��׻��β������������
	if(AnalyseResult.m_nJockerCount>0)
	{
		//������������
		if(!bLockCount)	FillCardCount(AnalyseResult,AnalyseResult.m_nMaxCount+1);
		//���ӳ���
		if(AnalyseResult.m_nJockerCount>0)
		{
			BYTE cbAddLong=AnalyseResult.m_nJockerCount/AnalyseResult.m_nMaxCount;
			if(AnalyseResult.m_nJockerCount%(AnalyseResult.m_nMaxCount)==0)
			{
				return FillCardSeque(AnalyseResult,AnalyseResult.m_nMaxCount,AnalyseResult.m_nUnEqualCard+cbAddLong,bAllowCycle);
			}
			else
			{
				if(bLockCount) return false;
			}
		}
	}

	AnalyseResult.m_cbMaxIndex=AnalyseResult.m_cbMaxIndex%ELEMT_COUNT;
	AnalyseResult.m_cbMinIndex=AnalyseResult.m_cbMinIndex%ELEMT_COUNT;
	return true;
}


bool  CGameLogic::IsBombCard(tagAnalyseResult & AnalyseResult)
{
	switch (AnalyseResult.m_cbCardCount)
	{
	case 0:	//����
	case 1: //����
	case 2:	//����	
		{
			break;
		}
	case 3:	//����
		{
			if(AnalyseResult.m_nJockerCount==AnalyseResult.m_cbCardCount)
			{
				AnalyseResult.m_cbCardType=CT_BOMB_3W;
				AnalyseResult.m_cbBombGrade=6;
				AnalyseResult.m_nUnEqualCard=1;
				AnalyseResult.m_cbMinIndex=0;
				AnalyseResult.m_cbMaxIndex=0;
				AnalyseResult.m_CardStyle[0].m_cbCard=0x4E;
				AnalyseResult.m_CardStyle[0].m_cbCount=3;
				return true;
			}
			break;
		}
	default:
		{
			//����ը
			if(AnalyseResult.m_nJockerCount==AnalyseResult.m_cbCardCount)
			{
				AnalyseResult.m_cbCardType=CT_BOMB_TW;
				AnalyseResult.m_cbBombGrade=7;
				AnalyseResult.m_nUnEqualCard=1;
				AnalyseResult.m_cbMinIndex=0;
				AnalyseResult.m_cbMaxIndex=0;
				AnalyseResult.m_CardStyle[0].m_cbCard=0x4E;
				AnalyseResult.m_CardStyle[0].m_cbCount=4;
				return true;
			}
			//��ը
			if(AnalyseResult.m_nUnEqualCard==1)
			{
				AnalyseResult.m_cbCardType=CT_BOMB;
				AnalyseResult.m_cbBombGrade=AnalyseResult.m_cbCardCount;
				AnalyseResult.m_CardStyle[AnalyseResult.m_cbMinIndex].m_cbCount+=AnalyseResult.m_nJockerCount;
				return true;
			}
			//��ը
			if(FindLinkCard(AnalyseResult,4,3,false,true))
			{
				AnalyseResult.m_cbCardType=CT_BOMB_LINK;
				BYTE cbTemp;
				//��ȡ��ը������
				for(int i=0;i<BOMB_TYPE_COUNT;i++)
				{
					cbTemp=m_cbTransTable[AnalyseResult.m_nMaxCount-4][i];
					if((cbTemp&0xF0)>0&&AnalyseResult.m_nUnEqualCard==((cbTemp&0xF0)>>4))
					{
						AnalyseResult.m_cbBombGrade=i+4;
						break;
					}
				}
				if(i==9) return false;
				return true;
			}
			break;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
VOID CGameLogic::AnalysebCardData(const BYTE cbCardData[], int cbCardCount, tagAnalyseResult & analyseResultEx)
{
	//���ý��
	ZeroMemory(&analyseResultEx,sizeof(analyseResultEx));
	int nIndex=0;
	analyseResultEx.m_cbMinIndex=255;
	analyseResultEx.m_cbCardCount=cbCardCount;
	for(nIndex=0;nIndex<cbCardCount;nIndex++)
	{
		analyseResultEx.m_CardStyle[GetCardLogicValue(cbCardData[nIndex])-3].m_cbCard=cbCardData[nIndex];
		analyseResultEx.m_CardStyle[GetCardLogicValue(cbCardData[nIndex])-3].m_cbCount++;
		analyseResultEx.m_cbMinIndex=min(GetCardLogicValue(cbCardData[nIndex])-3,analyseResultEx.m_cbMinIndex);
			
		if(cbCardData[nIndex]&0x40)
			analyseResultEx.m_nJockerCount++;
		else
			analyseResultEx.m_cbMaxIndex=max(GetCardLogicValue(cbCardData[nIndex])-3,analyseResultEx.m_cbMaxIndex);
	}

	//���ֻ����
	if(analyseResultEx.m_nJockerCount==analyseResultEx.m_cbCardCount)
	{
		if(analyseResultEx.m_CardStyle[14].m_cbCount>0)
			analyseResultEx.m_cbMaxIndex=14;
		else
			analyseResultEx.m_cbMaxIndex=13;
	}
	else
	{
		//�Ƶ�����
		for(nIndex=analyseResultEx.m_cbMinIndex;nIndex<=analyseResultEx.m_cbMaxIndex;nIndex++)
		{
			if(analyseResultEx.m_CardStyle[nIndex].m_cbCount>0)
				analyseResultEx.m_nUnEqualCard++;
			analyseResultEx.m_nMaxCount=max(analyseResultEx.m_nMaxCount,analyseResultEx.m_CardStyle[nIndex].m_cbCount);
		}
	}
	return;
}