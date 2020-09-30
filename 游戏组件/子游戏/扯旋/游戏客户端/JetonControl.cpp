#include "StdAfx.h"
#include "Resource.h"
#include "JetonControl.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//���캯��
CJettonControl::CJettonControl()
{
	//���ñ���
	m_lCellJetton=0l;
	m_lUserJetton=0l;
	m_lUserHaveCount=0l;
	m_lUserAddJetton=0l;
	m_lMinJetton=0l;
	m_cbFirstIndex=0;
	m_cbMaxLayerCount=10;
	m_UserJettonlPost.SetPoint(0,0);	
	m_AddJettonPost.SetPoint(0,0);	;
	ZeroMemory(m_cbChangeStatus,sizeof(m_cbChangeStatus));
	ZeroMemory(m_AddLayerInfo,sizeof(m_AddLayerInfo));
	ZeroMemory(m_UserLayerInfo,sizeof(m_UserLayerInfo));
	m_lScoreIndex[0]=10000000;
	m_lScoreIndex[1]=1000000;
	m_lScoreIndex[2]=100000;
	m_lScoreIndex[3]=10000;
	m_lScoreIndex[4]=1000;
	m_lScoreIndex[5]=100;
	m_lScoreIndex[6]=10;
	m_lScoreIndex[7]=1;

	//������Դ
	m_ImageJettonBack.LoadFromResource(AfxGetInstanceHandle(),IDB_JETTON_BACK);
	m_ImageBigJetton.LoadFromResource(AfxGetInstanceHandle(),IDB_BIG_JETTON);
	m_ImageSmaJetton.LoadFromResource(AfxGetInstanceHandle(),IDB_SMALL_JETTON);

	//�����С
	//CImageHandle ImageBigJetton(&m_ImageBigJetton);
	//CImageHandle ImageSmaJetton(&m_ImageSmaJetton);
	//CImageHandle ImageJettonBack(&m_ImageJettonBack);
	m_BigJettonSize.SetSize(m_ImageBigJetton.GetWidth()/JETTON_COUNT,m_ImageBigJetton.GetHeight());	
	m_SmaJettonSize.SetSize(m_ImageSmaJetton.GetWidth()/JETTON_COUNT,m_ImageSmaJetton.GetHeight());	 
	m_JettonBackSize.SetSize(m_ImageJettonBack.GetWidth()/2,m_ImageJettonBack.GetHeight());	 

	m_pbtDecrease=NULL;
	m_pbtClearJetton=NULL;
	m_pbtAdd=NULL;

	return;
}

//��������
CJettonControl::~CJettonControl()
{
	m_pbtDecrease= NULL;
	m_pbtClearJetton=NULL;
	m_pbtAdd=NULL;
}

//��������
void CJettonControl::SetUserHaveCount(LONGLONG lHaveCound)
{
	m_lUserHaveCount = lHaveCound;

	return ;
}

//���ð�ť
void CJettonControl::SetButton(CSkinButton *btDecrease,CSkinButton *pbtClearJetton,CSkinButton *pbtAdd)
{
	m_pbtDecrease=btDecrease;
	m_pbtClearJetton=pbtClearJetton;
	m_pbtAdd=pbtAdd;

	return ;
}

//���¿ؼ�
void CJettonControl::UpdataButton()
{
	//����ť
	BOOL bEnable=(m_lUserAddJetton>m_lMinJetton)?TRUE:FALSE;
	m_pbtClearJetton->EnableWindow(bEnable);

	//��ע��ť
	m_pbtAdd->EnableWindow(bEnable);

	//��ע��ť
	for(BYTE i=0;i<CONTROL_COUNT;i++)
	{
		//LONGLONG lScore = m_lCellJetton*(LONGLONG)pow(10,(CONTROL_COUNT-1-i));
		bEnable=(m_AddLayerInfo[i].cbLayerCount>0 && (m_lUserAddJetton>m_lMinJetton))?TRUE:FALSE;
		m_pbtDecrease[i].EnableWindow(bEnable);
	}

	return ;
}

//�滭λ��
void CJettonControl::SetControlPost(CPoint ControlPost,CPoint AddPost)
{
	m_UserJettonlPost=ControlPost;	
	m_AddJettonPost=AddPost;

	return ;
}

//��Ԫ����
void CJettonControl::SetJettonLayer(BYTE cbJettonLayer)
{
	m_cbMaxLayerCount = cbJettonLayer;

	return ;
}

//���õ�Ԫ
void CJettonControl::SetCellJetton(LONGLONG lCellJetton)
{
	m_lCellJetton = lCellJetton;

	for (BYTE i=0;i<CountArray(m_lScoreIndex);i++)
	{
		if(m_lScoreIndex[i]==m_lCellJetton)break;
	}

	if(i<CONTROL_COUNT-1)i=CONTROL_COUNT-1;
	m_cbFirstIndex=i-(CONTROL_COUNT-1);

	return ;
}

//��С����
void CJettonControl::SetMinJetton(LONGLONG lMinJetton)
{
	m_lMinJetton = lMinJetton;
	m_lUserAddJetton = m_lMinJetton;
	ZeroMemory(m_AddLayerInfo,sizeof(m_AddLayerInfo));
	CountJettonInfo(m_lUserAddJetton,m_AddLayerInfo);

	//���¿ؼ�
	UpdataButton();

	return ;
}

//���ó���
void CJettonControl::SetUserJetton(LONGLONG lUserJetton)
{
	m_lUserJetton = lUserJetton;
	ZeroMemory(m_UserLayerInfo,sizeof(m_UserLayerInfo));
	CountJettonInfo(lUserJetton,m_UserLayerInfo);

	//���¿ؼ�
	UpdataButton();

	return ;
}

//���ó���
void CJettonControl::SetAddJetton(LONGLONG lAddJetton)
{
	m_lUserAddJetton = lAddJetton;	
	ZeroMemory(m_AddLayerInfo,sizeof(m_AddLayerInfo));
	CountJettonInfo(lAddJetton,m_AddLayerInfo);

	//���¿ؼ�
	UpdataButton();

	return ;
}

//�������
void CJettonControl::CountJettonInfo(LONGLONG lScore,JettonLayerInfo LayerInfo[CONTROL_COUNT],BYTE cbChangeIndex)
{
	if(lScore<m_lCellJetton)return;

	//������Ŀ
	LONGLONG lScoreCount=lScore;
	ZeroMemory(LayerInfo,sizeof(JettonLayerInfo)*CONTROL_COUNT);
	for (BYTE i=m_cbFirstIndex;i<m_cbFirstIndex+CONTROL_COUNT;i++)
	{
		//������Ŀ
		LONGLONG lCellCount=lScoreCount/m_lScoreIndex[i];

		//�������
		if (lCellCount==0L) continue;

		//������Ϣ
		LayerInfo[i-m_cbFirstIndex].cbDrawIndex=JETTON_COUNT-i-1;
		LayerInfo[i-m_cbFirstIndex].cbLayerCount=(BYTE)lCellCount;

		//������Ŀ
		lScoreCount-=lCellCount*m_lScoreIndex[i];
	}

	//���˻滭
	if(cbChangeIndex==CONTROL_COUNT)return;

	//������Ŀ
	for(LONGLONG i=0;i<CONTROL_COUNT-1;i++)
	{
		if(i==cbChangeIndex)break;
		if(LayerInfo[i].cbLayerCount>0 && LayerInfo[i+1].cbLayerCount==0)
		{
			LayerInfo[i].cbLayerCount-=1;
			BYTE cbIndex=JETTON_COUNT-1-LayerInfo[i].cbDrawIndex;
			LayerInfo[i+1].cbLayerCount=BYTE(m_lScoreIndex[cbIndex]/m_lScoreIndex[cbIndex+1]);
			LayerInfo[i+1].cbDrawIndex=JETTON_COUNT-(cbIndex+1)-1;
		}
	}

	return ;
}

//����ж�
bool CJettonControl::EstimateHitJetton(CPoint Point)
{
	if(m_lUserJetton+m_lUserAddJetton==0)return false;

	//�жϼ�ע
	for(INT i=CONTROL_COUNT-1;i>=0;i--)
	{
		//����λ��
		INT nIndex = CONTROL_COUNT-i-1;
		INT nXSrc = m_UserJettonlPost.x+(m_BigJettonSize.cx+JETTON_SPECE)*nIndex;
		INT nYSrc = m_UserJettonlPost.y+m_BigJettonSize.cy;
		INT nXDest =nXSrc+m_BigJettonSize.cx;
		INT nYDest =0;
		if(m_UserLayerInfo[i].cbLayerCount>0)
		{
			nYDest=m_UserJettonlPost.y-(m_UserLayerInfo[i].cbLayerCount-1)*LAYER_HEIGHT;
		}
		else nYDest=nYSrc-m_JettonBackSize.cy;

		//��Чλ��
		if(Point.x>=nXSrc && Point.x<=nXDest && Point.y<=nYSrc && Point.y>=nYDest)
		{
			//��עλ��
			if(m_UserLayerInfo[i].cbLayerCount>0)
			{
				nIndex = JETTON_COUNT-1-m_UserLayerInfo[i].cbDrawIndex;

				//ת������
				m_UserLayerInfo[i].cbLayerCount--;
				if(m_UserLayerInfo[i].cbLayerCount==0 && m_cbChangeStatus[i]==FALSE)
				{
					m_cbChangeStatus[i]=TRUE;
				}

				//�������
				m_lUserJetton-=m_lScoreIndex[nIndex];
				m_lUserAddJetton+=m_lScoreIndex[nIndex];
				//if(m_AddLayerInfo[i].cbLayerCount==m_cbMaxLayerCount)
				{
					CountJettonInfo(m_lUserAddJetton,m_AddLayerInfo,CONTROL_COUNT);
				}
				//else 
				//{
				//	m_AddLayerInfo[i].cbLayerCount++;
				//	m_AddLayerInfo[i].cbDrawIndex=m_UserLayerInfo[i].cbDrawIndex;
				//}

				//���¿ؼ�
				UpdataButton();
				return true;
			}
			else
			{
				//ָ��ת��
				for(LONGLONG j=i-1;j>=0;j--)
				{
					if(m_UserLayerInfo[j].cbLayerCount==0)continue;
					CountJettonInfo(m_lUserJetton,m_UserLayerInfo,(BYTE)i);

					//���¿ؼ�
					UpdataButton();
					return true;
				}

				//�������
				LONGLONG lScore = m_lCellJetton*(LONGLONG)pow(10,(CONTROL_COUNT-1-i));
				if(m_lUserJetton>=lScore)
				{
					CountJettonInfo(m_lUserJetton,m_UserLayerInfo,CONTROL_COUNT);

					//���¿ؼ�
					UpdataButton();
					return true;
				}

			}
		}
	}

	//�жϼ�ע
	for(INT i=CONTROL_COUNT-1;i>=0;i--)
	{
		//����λ��
		INT nIndex = CONTROL_COUNT-i-1;
		INT nXSrc = m_AddJettonPost.x+(m_SmaJettonSize.cx+JETTON_SPECE)*nIndex;
		INT nYSrc = m_AddJettonPost.y+m_SmaJettonSize.cy;
		INT nXDest =nXSrc+m_SmaJettonSize.cx;
		INT nYDest =m_AddJettonPost.y-(m_AddLayerInfo[i].cbLayerCount-1)*LAYER_HEIGHT;

		//��Чλ��
		if(Point.x>=nXSrc && Point.x<=nXDest && Point.y<=nYSrc && Point.y>=nYDest)
		{
			return DecreaseJetton(WORD(i));
		}
	}

	return false;
}

//�ƶ��ж�
bool CJettonControl::EstimateMove(CPoint Point)
{
	if(m_lUserJetton+m_lUserAddJetton==0)return false;

	//�ж�λ��
	for(INT i=CONTROL_COUNT-1;i>=0;i--)
	{
		//����λ��
		INT nIndex = CONTROL_COUNT-i-1;
		INT nXSrc = m_UserJettonlPost.x+(m_BigJettonSize.cx+JETTON_SPECE)*nIndex;
		INT nYSrc = m_UserJettonlPost.y+m_BigJettonSize.cy;
		INT nXDest =nXSrc+m_BigJettonSize.cx;
		INT nYDest =m_UserJettonlPost.y-(m_UserLayerInfo[i].cbLayerCount-1)*LAYER_HEIGHT;

		//��Чλ��
		if(Point.x>=nXSrc && Point.x<=nXDest && Point.y<=nYSrc && Point.y>=nYDest)
		{
			if(m_UserLayerInfo[i].cbLayerCount==0 && m_cbChangeStatus[i]==FALSE)
			{
				m_cbChangeStatus[i]=TRUE;
				return true;
			}
		}
		else if(m_cbChangeStatus[i]==TRUE)
		{
			m_cbChangeStatus[i]=FALSE;
			return true;
		}
	}

	return false;
}

//���ٳ���
bool CJettonControl::DecreaseJetton(WORD wControlID)
{
	ASSERT(wControlID<CONTROL_COUNT);
	if(wControlID>=CONTROL_COUNT)return false;

	if(m_AddLayerInfo[wControlID].cbLayerCount>0)
	{
		INT nIndex = JETTON_COUNT-1-m_AddLayerInfo[wControlID].cbDrawIndex;

		//���ٳ���
		if(m_lUserAddJetton-m_lScoreIndex[nIndex]>=m_lMinJetton)
		{
			m_AddLayerInfo[wControlID].cbLayerCount--;

			m_lUserJetton+=m_lScoreIndex[nIndex];
			m_lUserAddJetton-=m_lScoreIndex[nIndex];
			if(m_UserLayerInfo[wControlID].cbLayerCount==m_cbMaxLayerCount)
			{
				CountJettonInfo(m_lUserJetton,m_UserLayerInfo,CONTROL_COUNT);
			}
			else 
			{
				m_UserLayerInfo[wControlID].cbLayerCount++;
				m_UserLayerInfo[wControlID].cbDrawIndex=m_AddLayerInfo[wControlID].cbDrawIndex;
			}
		}
		//���ٳ���
		else
		{
			LONGLONG lTemp=m_lUserAddJetton-m_lMinJetton;
			m_lUserJetton+=lTemp;
			m_lUserAddJetton=m_lMinJetton;
			ZeroMemory(m_AddLayerInfo,sizeof(m_AddLayerInfo));
			ZeroMemory(m_UserLayerInfo,sizeof(m_UserLayerInfo));
			CountJettonInfo(m_lUserJetton,m_UserLayerInfo);
			CountJettonInfo(m_lUserAddJetton,m_AddLayerInfo);
		}

		//���¿ؼ�
		UpdataButton();

		return true;
	}

	return false;
}

//�����ע
void CJettonControl::ClearAddJetton()
{
	//�����ע
	m_lUserJetton = m_lUserJetton+m_lUserAddJetton-m_lMinJetton;
	m_lUserAddJetton = m_lMinJetton;
	ZeroMemory(m_AddLayerInfo,sizeof(m_AddLayerInfo));
	ZeroMemory(m_UserLayerInfo,sizeof(m_UserLayerInfo));
	CountJettonInfo(m_lUserJetton,m_UserLayerInfo);
	CountJettonInfo(m_lUserAddJetton,m_AddLayerInfo);

	//���¿ؼ�
	UpdataButton();

	return;
}

//��������
void CJettonControl::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
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

//�滭����
void CJettonControl::DrawJettonControl(CDC * pDC)
{
	//�滭�ж�
	if (m_lUserJetton==0L && m_lUserAddJetton==0L) return;

	//������Դ
	//CImageHandle ImageBigJetton(&m_ImageBigJetton);
	//CImageHandle ImageSmaJetton(&m_ImageSmaJetton);
	//CImageHandle ImageJettonBack(&m_ImageJettonBack);

	//���뱳��
	for(BYTE i=0;i<CONTROL_COUNT;i++)
	{
		if(m_UserLayerInfo[i].cbLayerCount>0)continue;

		LONGLONG lScore = m_lCellJetton*(LONGLONG)pow(10,(CONTROL_COUNT-1-i));
		if(m_lUserJetton>=lScore)
		{
			//�滭λ��
			INT nIndex = CONTROL_COUNT-i-1;
			INT nXPost=m_UserJettonlPost.x+(m_JettonBackSize.cx+1)*nIndex;
			INT nYPost=m_UserJettonlPost.y-m_JettonBackSize.cy/2+6;

			//�滭����
			m_ImageJettonBack.TransDrawImage(pDC,nXPost,nYPost,m_JettonBackSize.cx,
				m_JettonBackSize.cy,m_JettonBackSize.cx*m_cbChangeStatus[i],0,RGB(255,0,255));
		}
	}

	//�滭����
	for(BYTE k=0;k<2;k++)
	{
		//������Ϣ
		LONGLONG lScore=(k==0)?(m_lUserJetton):(m_lUserAddJetton);
		CSize *pJettonSize =(k==0)?(&m_BigJettonSize):(&m_SmaJettonSize);
		CPoint *pDrawPoint =(k==0)?(&m_UserJettonlPost):(&m_AddJettonPost);
		CBitImage *pSkinImage =(k==0)?(&m_ImageBigJetton):(&m_ImageSmaJetton);
		JettonLayerInfo LayerInfo[CONTROL_COUNT];
		if(k==0)CopyMemory(LayerInfo,m_UserLayerInfo,sizeof(LayerInfo));
		else CopyMemory(LayerInfo,m_AddLayerInfo,sizeof(LayerInfo));

		//�滭����
		for(INT i=CONTROL_COUNT-1;i>=0;i--)
		{
			if (LayerInfo[i].cbLayerCount==0L) continue;

			//�׻�λ��
			BYTE cbScoreIndex=LayerInfo[i].cbDrawIndex;
			INT nIndex = CONTROL_COUNT-i-1;
			INT nXPost=pDrawPoint->x+(pJettonSize->cx+JETTON_SPECE)*nIndex;
			INT nYPost=pDrawPoint->y;

			//�滭����
			LONGLONG lLayerCount=__min(LayerInfo[i].cbLayerCount,m_cbMaxLayerCount);
			for (INT j=0;j<lLayerCount;j++)
			{
				pSkinImage->TransDrawImage(pDC,nXPost,nYPost-LAYER_HEIGHT*j,pJettonSize->cx,
					pJettonSize->cy,cbScoreIndex*pJettonSize->cx,0,RGB(255,0,255));
			}
		}

		//if(k==1 && lScore-m_lMinJetton<=0)continue;

		//��������
		CFont ViewFont;
		ViewFont.CreateFont(-13,0,0,0,400,0,0,0,134,3,2,1,1,TEXT("����"));
		CFont *pOldFont=pDC->SelectObject(&ViewFont);
		pDC->SetTextAlign(TA_LEFT);
		int iBkMode = pDC->SetBkMode( TRANSPARENT );

		//�滭��Ŀ
		TCHAR szBuffer[64]=TEXT("");
		if(k==0)
		{
			INT nXPost=pDrawPoint->x+155;
			INT nYPost =pDrawPoint->y+5;
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("����:%I64d"),lScore);
			DrawTextString(pDC,szBuffer,RGB(255,255,0),RGB(0,0,0),nXPost,nYPost);
		}
		else
		{
			INT nXPost=pDrawPoint->x+5;
			INT nYPost=pDrawPoint->y+35;
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("������:%I64d,����:%I64d"),m_lMinJetton,lScore-m_lMinJetton);
			DrawTextString(pDC,szBuffer,RGB(255,255,0),RGB(0,0,0),nXPost,nYPost);
		}

		//�ͷ���Դ
		pDC->SetBkMode(iBkMode);
		pDC->SelectObject(pOldFont);
		ViewFont.DeleteObject();	

		//�滭����
		for(INT i=CONTROL_COUNT-1;i>=0;i--)
		{
			//�滭����
			if(LayerInfo[i].cbLayerCount>m_cbMaxLayerCount)
			{
				//��������
				CFont ViewFont;
				ViewFont.CreateFont(-13,0,0,0,400,0,0,0,134,3,2,1,1,TEXT("����"));
				CFont *pOldFont=pDC->SelectObject(&ViewFont);
				pDC->SetTextAlign(TA_LEFT);

				//�滭��Ŀ
				TCHAR szBuffer[64]=TEXT("");
				INT nIndex = CONTROL_COUNT-i-1;
				INT nXPost=pDrawPoint->x+(pJettonSize->cx+JETTON_SPECE)*nIndex;
				INT nYPost=pDrawPoint->y;
				_sntprintf(szBuffer,sizeof(szBuffer),TEXT("��%d"),LayerInfo[i].cbLayerCount);
				DrawTextString(pDC,szBuffer,RGB(255,255,0),RGB(0,0,0),nXPost,nYPost);

				//�ͷ���Դ
				pDC->SelectObject(pOldFont);
				ViewFont.DeleteObject();	
			}
		}
	}

	//����ע��
	if(m_lUserHaveCount>0)
	{
		//��������
		CFont ViewFont;
		ViewFont.CreateFont(-13,0,0,0,400,0,0,0,134,3,2,1,1,TEXT("����"));
		CFont *pOldFont=pDC->SelectObject(&ViewFont);
		pDC->SetTextAlign(TA_LEFT);
		int iBkMode = pDC->SetBkMode( TRANSPARENT );

		//�滭��Ŀ
		TCHAR szBuffer[64]=TEXT("");
		INT nXPost=m_UserJettonlPost.x+155;
		INT nYPost =m_UserJettonlPost.y-10;
		_sntprintf(szBuffer,sizeof(szBuffer),TEXT("����:%I64d"),m_lUserHaveCount);
		DrawTextString(pDC,szBuffer,RGB(255,255,0),RGB(0,0,0),nXPost,nYPost);


		//�ͷ���Դ
		pDC->SetBkMode(iBkMode);
		pDC->SelectObject(pOldFont);
		ViewFont.DeleteObject();	
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
