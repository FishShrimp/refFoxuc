#include "StdAfx.h"
#include "Resource.h"
#include "GoldView.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
bool							CGoldView::m_bInit=false;				//��ʼ��־
CBitImage						CGoldView::m_ImageGold;					//��������

//////////////////////////////////////////////////////////////////////////

//���캯��
CGoldView::CGoldView()
{
	m_lGold=0L;
	m_lMaxLayer=10;
	m_lCellScore=10L;
	m_cbFirstIndex=3;
	m_bBeelinePut=false;
	m_lScoreIndex[0]=10000000;
	m_lScoreIndex[1]=1000000;
	m_lScoreIndex[2]=100000;
	m_lScoreIndex[3]=10000;
	m_lScoreIndex[4]=1000;
	m_lScoreIndex[5]=100;
	m_lScoreIndex[6]=10;
	m_lScoreIndex[7]=1;
	memset(m_lDrawOrder,0,sizeof(m_lDrawOrder));
	memset(m_ptDrawJetton,0,sizeof(m_ptDrawJetton));
	memset(m_JettonHeapInfo,0,sizeof(m_JettonHeapInfo));
	if (m_bInit==false)
	{
		m_bInit=true;
		m_ImageGold.LoadFromResource(GetModuleHandle(NULL),IDB_SMALL_JETTON);
	}
	HINSTANCE hInstance=AfxGetInstanceHandle();

	//�����С
	//CImageHandle ImageHandle(&m_ImageGold);
	m_JettonSize.SetSize(m_ImageGold.GetWidth()/CountArray(m_lScoreIndex),m_ImageGold.GetHeight());

	return;
}

//��������
CGoldView::~CGoldView()
{
}

//���ðڷ�
void CGoldView::SetBeelinePut(bool bBeelinePut)
{
	if (bBeelinePut!=m_bBeelinePut)
	{
		m_bBeelinePut=bBeelinePut;
		RectifyGoldLayer();
	}
	return ;
}

//���ó���
void CGoldView::SetGold(LONGLONG lGold)
{
	if (m_lGold!=lGold)
	{
		m_lGold=lGold;
		RectifyGoldLayer();
	}
	return;
}

//���õ�Ԫ
void CGoldView::SetCellJetton(LONGLONG lCellJetton)
{
	m_lCellScore = lCellJetton;

	for (BYTE i=0;i<CountArray(m_lScoreIndex);i++)
	{
		if(m_lScoreIndex[i]==m_lCellScore)break;
	}

	if(i<CONTROL_COUNT-1)i=CONTROL_COUNT-1;
	m_cbFirstIndex=i-(CONTROL_COUNT-1);

	return ;
}

//���ò���
void CGoldView::SetMaxGoldLayer(LONGLONG lMaxLayer)
{
	if (m_lMaxLayer!=lMaxLayer)
	{
		m_lMaxLayer=lMaxLayer; 
	}
	return;
}

//�滭����
void CGoldView::DrawGoldView(CDC * pDC, int nXPos, int nYPos, bool bCount,bool bCenter)
{
	if(GetGold()==0)return;

	//����λͼ
	//CImageHandle ImageHandle(&m_ImageGold);

	//λ����Ϣ
	int nYPosDraw=nYPos;
	int nXPosDraw=nXPos;

	//�滭����
	INT nCount=0;
	for (INT i=0;i<CountArray(m_JettonHeapInfo);i++)
	{
		LONGLONG lIndex = (!m_bBeelinePut)?m_lDrawOrder[i]:i;
		LONGLONG lLayerCount = __min(m_JettonHeapInfo[lIndex].cbLayerCount,m_lMaxLayer);
		for (INT j=0;j<lLayerCount;j++)
		{
			//�滭λ��
			if(j==0)
			{
				nYPosDraw=nYPos+m_ptDrawJetton[nCount].y;
				nXPosDraw=nXPos+m_ptDrawJetton[nCount++].x;
			}

			//�滭����
			INT nIndex = m_JettonHeapInfo[lIndex].cbDrawIndex;
			m_ImageGold.TransDrawImage(pDC,nXPosDraw,nYPosDraw-JETTON_SPECE*j,
				m_JettonSize.cx,m_JettonSize.cy,nIndex*m_JettonSize.cx,0,RGB(255,0,255));
		}
		if(!bCenter && nCount==CONTROL_COUNT/*+1*/)break;
	}

	//�滭����
	if((m_lGold>=1L)&&(bCount))
	{
		//��������
		CFont ViewFont;
		INT nFontSize = (bCenter)?(-15):(-13);
		ViewFont.CreateFont(nFontSize,0,0,0,400,0,0,0,134,3,2,1,1,TEXT("����"));
		CFont *pOldFont=pDC->SelectObject(&ViewFont);
		pDC->SetTextAlign(TA_CENTER);
       int iBkMode = pDC->SetBkMode( TRANSPARENT );

	   //���λ��
	   if(!m_bBeelinePut)
	   {
		   if(nCount<3)nYPos+=32;
		   else nYPos+=45;
	   }
	   else nYPos+=30;

	   //�����Ϣ
	   TCHAR szBuffer[64]=TEXT(""),szCountBuffer[64]=TEXT("");
	   GetGlodString(m_lGold,szCountBuffer);
	   if(bCenter)_sntprintf(szBuffer,sizeof(szBuffer),TEXT("����:%s"),szCountBuffer);
	   else _sntprintf(szBuffer,sizeof(szBuffer),TEXT("%s"),szCountBuffer);
	   DrawTextString(pDC,szBuffer,RGB(255,255,0),RGB(0,0,0),nXPos+16,nYPos);

		//�ͷ���Դ
	   pDC->SetBkMode(iBkMode);
	   pDC->SelectObject(pOldFont);
	   ViewFont.DeleteObject();	
	}

	return;
}

//���������
void CGoldView::RectifyGoldLayer()
{
	if(GetGold()==0)
	{
		memset(m_JettonHeapInfo,0,sizeof(m_JettonHeapInfo));
		return;
	}

	//��������
	LONGLONG lGold=m_lGold;
	memset(m_JettonHeapInfo,0,sizeof(m_JettonHeapInfo));

	//���������
	BYTE cbDrawCount=0;
	struct tagOrderInfo
	{
		BYTE cbLayerCount;
		LONGLONG lIndex;
	}OrderInfo[JETTON_COUNT];
	ZeroMemory(OrderInfo,sizeof(OrderInfo));
	for (BYTE i=0;i<JETTON_COUNT;i++)
	{
		OrderInfo[i].lIndex=i;
		if (lGold>=m_lScoreIndex[i])
		{
			cbDrawCount++;
			m_JettonHeapInfo[i].cbLayerCount=BYTE(lGold/m_lScoreIndex[i]);
			m_JettonHeapInfo[i].cbDrawIndex=JETTON_COUNT-i-1;
			lGold-=m_JettonHeapInfo[i].cbLayerCount*m_lScoreIndex[i];
			OrderInfo[i].cbLayerCount = m_JettonHeapInfo[i].cbLayerCount;
		}
	}

	//ֱ�߰ڷ�
	if(m_bBeelinePut)
	{
		//����λ��
		switch(cbDrawCount)
		{
		case 1:
			{
				m_ptDrawJetton[0].x=0;
				m_ptDrawJetton[0].y=0;
				return;
			}
		case 2:
			{
				m_ptDrawJetton[0].x=-m_JettonSize.cx/2-1;
				m_ptDrawJetton[0].y=0;
				m_ptDrawJetton[1].x=m_JettonSize.cx/2+1;
				m_ptDrawJetton[1].y=0;
				return;
			}
		case 3:
			{
				m_ptDrawJetton[0].x=0;
				m_ptDrawJetton[0].y=0;
				m_ptDrawJetton[1].x=-m_JettonSize.cx-1;
				m_ptDrawJetton[1].y=0;
				m_ptDrawJetton[2].x=m_JettonSize.cx+1;
				m_ptDrawJetton[2].y=0;
				return;
			}
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			{
				m_ptDrawJetton[0].x=-m_JettonSize.cx/2-1;
				m_ptDrawJetton[0].y=0;
				m_ptDrawJetton[1].x=m_JettonSize.cx/2+1;
				m_ptDrawJetton[1].y=0;
				m_ptDrawJetton[2].x=-m_JettonSize.cx*3/2-2;
				m_ptDrawJetton[2].y=0;
				m_ptDrawJetton[3].x=m_JettonSize.cx*3/2+3;
				m_ptDrawJetton[3].y=0;
				m_ptDrawJetton[4].x=m_JettonSize.cx*5/2+4;
				m_ptDrawJetton[4].y=0;
				return;
			}
		}
		return;
	}

	//�Ӷൽ��
	for (LONGLONG i=0;i<JETTON_COUNT;i++)
	{
		for (LONGLONG j=i+1;j<JETTON_COUNT;j++)
		{
			if (OrderInfo[i].cbLayerCount<OrderInfo[j].cbLayerCount)
			{
				LONGLONG lTemp = OrderInfo[i].cbLayerCount;
				OrderInfo[i].cbLayerCount = OrderInfo[j].cbLayerCount;
				OrderInfo[j].cbLayerCount = (BYTE)lTemp;

				lTemp = OrderInfo[i].lIndex;
				OrderInfo[i].lIndex = OrderInfo[j].lIndex;
				OrderInfo[j].lIndex = lTemp;
			}
		}
	}

	//�滭˳��
	for (LONGLONG i=0;i<JETTON_COUNT;i++)m_lDrawOrder[i]=OrderInfo[i].lIndex;

	//����λ��
	switch(cbDrawCount)
	{
	case 1:
		{
			m_ptDrawJetton[0].x=0;
			m_ptDrawJetton[0].y=0;
			return;
		}
	case 2:
		{
			m_ptDrawJetton[0].x=-m_JettonSize.cx/2-2;
			m_ptDrawJetton[0].y=0;
			m_ptDrawJetton[1].x=m_JettonSize.cx/2+2;
			m_ptDrawJetton[1].y=0;
			return;
		}
	case 3:
		{
			m_ptDrawJetton[0].x=-m_JettonSize.cx/2-1;
			m_ptDrawJetton[0].y=-m_JettonSize.cy/2;
			m_ptDrawJetton[1].x=m_JettonSize.cx/2+1;
			m_ptDrawJetton[1].y=-m_JettonSize.cy/2;
			m_ptDrawJetton[2].x=0;
			m_ptDrawJetton[2].y=m_JettonSize.cy/2+1;
			return;
		}
	case 4:
		{
			m_ptDrawJetton[0].x=-m_JettonSize.cx/2-1;
			m_ptDrawJetton[0].y=-m_JettonSize.cy/2-1;
			m_ptDrawJetton[1].x=m_JettonSize.cx/2+1;
			m_ptDrawJetton[1].y=-m_JettonSize.cy/2-1;
			m_ptDrawJetton[2].x=-m_JettonSize.cx/2-1;
			m_ptDrawJetton[2].y=m_JettonSize.cy/2+1;
			m_ptDrawJetton[3].x=m_JettonSize.cx/2+1;
			m_ptDrawJetton[3].y=m_JettonSize.cy/2+1;
			return;
		}
	case 5:
	case 6:
		{
			m_ptDrawJetton[0].x=-m_JettonSize.cx/2-1;
			m_ptDrawJetton[0].y=-m_JettonSize.cy/2-1;
			m_ptDrawJetton[1].x=m_JettonSize.cx/2+1;
			m_ptDrawJetton[1].y=-m_JettonSize.cy/2-1;
			m_ptDrawJetton[2].x=-m_JettonSize.cx/2-1;
			m_ptDrawJetton[2].y=m_JettonSize.cy/2+1;
			m_ptDrawJetton[3].x=m_JettonSize.cx/2+1;
			m_ptDrawJetton[3].y=m_JettonSize.cy/2+1;
			m_ptDrawJetton[4].x=-m_JettonSize.cx*3/2-2;
			m_ptDrawJetton[4].y=0;
			m_ptDrawJetton[5].x=m_JettonSize.cx*3/2+2;
			m_ptDrawJetton[5].y=0;
			return;
		}	
	case 7:
	case 8:
		{
			m_ptDrawJetton[0].x=-m_JettonSize.cx/2-1;
			m_ptDrawJetton[0].y=-m_JettonSize.cy/2-1;
			m_ptDrawJetton[4].x=-m_JettonSize.cx*3/2-2;
			m_ptDrawJetton[4].y=-m_JettonSize.cy/2-1;
			m_ptDrawJetton[1].x=m_JettonSize.cx/2+1;
			m_ptDrawJetton[1].y=-m_JettonSize.cy/2-1;
			m_ptDrawJetton[5].x=m_JettonSize.cx*3/2+2;
			m_ptDrawJetton[5].y=-m_JettonSize.cy/2-1;
			m_ptDrawJetton[2].x=-m_JettonSize.cx/2-1;
			m_ptDrawJetton[2].y=m_JettonSize.cy/2+1;
			m_ptDrawJetton[6].x=-m_JettonSize.cx*3/2-2;
			m_ptDrawJetton[6].y=m_JettonSize.cy/2+1;
			m_ptDrawJetton[3].x=m_JettonSize.cx/2+1;
			m_ptDrawJetton[3].y=m_JettonSize.cy/2+1;
			m_ptDrawJetton[7].x=+m_JettonSize.cx*3/2+2;
			m_ptDrawJetton[7].y=m_JettonSize.cy/2+1;
			return;
		}	
	}

	return;
}

//���Ա��ַ�
LPCTSTR CGoldView::GetGlodString(LONGLONG lGold, TCHAR szString[])
{
	LONGLONG lTemp=(LONGLONG)lGold; 
	if(lGold<0L) return szString;

	//����С����
	DWORD dwCharBit=0L;

	//ת���ַ�
	lTemp = (LONGLONG)lGold; 
	DWORD dwNumBit=0L;
	do
	{
		dwNumBit++;
		szString[dwCharBit++]=(TCHAR)(lTemp%10+TEXT('0'));
		if (dwNumBit%3==0) szString[dwCharBit++]=TEXT(',');
		lTemp/=10;
	} while (lTemp>0L);

	//�����ַ�
	if (szString[dwCharBit-1]==TEXT(',')) szString[dwCharBit-1]=0;
	szString[dwCharBit]=0;

	//βͷ����
	_tcsrev(szString);

	return szString;
}

//��������
void CGoldView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//////////////////////////////////////////////////////////////////////////
