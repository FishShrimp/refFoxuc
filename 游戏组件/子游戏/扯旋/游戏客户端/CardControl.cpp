#include "StdAfx.h"
#include "Resource.h"
#include "GameOption.h"
#include "CardControl.h"
#include ".\cardcontrol.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
bool					CCardControl::m_bLoad=false;					//��ʼ��־
CSize					CCardControl::m_CardSize;						//�˿˴�С
CBitImage				CCardControl::m_ImageCard;						//�˿�ͼƬ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCardControl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardControl::CCardControl()
{
	//���ñ���
	m_bFlash=false;	
	m_bDisable=false;	
	m_bPositively=false;
	m_bDisplayHead=false;
	m_dwCardHSpace=DEFAULT_PELS;

	//״̬����
	m_bCaptureMouse=false;

	//λ�ñ���
	m_XCollocateMode=enXCenter;
	m_YCollocateMode=enYCenter;

	//��Դ����
	if (m_bLoad==false)
	{
		//������Դ
		m_bLoad=true;
		m_ImageCard.LoadFromResource(AfxGetInstanceHandle(),IDB_CARD);

		//��ȡ��С
		//CImageHandle CardImageHandle(&m_ImageCard);
		m_CardSize.cx=m_ImageCard.GetWidth()/13;
		m_CardSize.cy=m_ImageCard.GetHeight()/5;
	}
	m_ImageMark.LoadFromResource(AfxGetInstanceHandle(),IDB_CARD_MASK);

	return;
}

//��������
CCardControl::~CCardControl()
{
}

//������Ϣ
int CCardControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ñ���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return 0;
}

//�ػ�����
void CCardControl::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//����λͼ
	//CImageHandle HandleCard(&m_ImageCard);
	//CImageHandle HandleMark(&m_ImageMark);

	//����λͼ
	CBitmap BufferImage;
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���� DC
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(&dc);
	BufferDC.SelectObject(&BufferImage);

	//�滭�˿�
	DWORD dwXImagePos,dwYImagePos;
	DWORD dwXScreenPos,dwYScreenPos;
	for (INT_PTR i=0;i<m_CardDataArray.GetCount();i++)
	{
		//��������
		BYTE cbCardData=m_CardDataArray[i];
		bool bDrawCard=((i!=0 && i!=1)||(m_bDisplayHead==true));
		if(cbCardData==0xff)continue;

		//ת������
		if(cbCardData==0x4e)cbCardData=0x42;

		//ͼƬλ��
		if(m_bDisable)
		{
			dwXImagePos=m_CardSize.cx*3;
			dwYImagePos=m_CardSize.cy*4;
		}
		else if ((bDrawCard==true)&&(cbCardData!=0))
		{
			dwXImagePos=((cbCardData&CARD_MASK_VALUE)-1)*m_CardSize.cx;
			dwYImagePos=((cbCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
		}
		else
		{
			dwXImagePos=m_CardSize.cx*2;
			dwYImagePos=m_CardSize.cy*4;
		}

		//��Ļλ��
		dwYScreenPos=0;
		dwXScreenPos=m_dwCardHSpace*i;

		//�滭�˿�
		m_ImageCard.TransDrawImage(&BufferDC,dwXScreenPos,dwYScreenPos,m_CardSize.cx,m_CardSize.cy,dwXImagePos,dwYImagePos,RGB(255,0,255));

		//�滭��˸
		if(m_bFlash && i>=MAX_COUNT/2)
		{
			m_ImageMark.TransDrawImage(&BufferDC,dwXScreenPos+1,dwYScreenPos+1,RGB(255,0,255));
		}
	}

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

//�����Ϣ
void CCardControl::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	return;
}

//�����Ϣ
void CCardControl::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	return;
}

//����ƶ�
void CCardControl::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//״̬�ж�
	if ((m_bPositively==false)||(m_bDisplayHead==true)) return;
	INT_PTR cbCardCount=m_CardDataArray.GetCount();
	if (cbCardCount==0)return;

	//�жϷ�Χ
	BYTE cbReturnValue=false;
	if((Point.x>(int)m_dwCardHSpace*2)&&(cbCardCount>2))cbReturnValue=true;
	if(!cbReturnValue && Point.x>(int)m_dwCardHSpace+m_CardSize.cx)cbReturnValue=true;
	if(cbReturnValue)
	{
		//״̬�ж�
		if (m_bCaptureMouse==false) return;

		//�ͷŲ���
		m_bCaptureMouse=false;

		//ˢ�½���
		CRect rcInvalid(0,0,(m_CardDataArray.GetCount()==2)?((int)m_dwCardHSpace+m_CardSize.cx):((int)m_dwCardHSpace*2),m_CardSize.cy);
		InvalidateRect(rcInvalid,FALSE);

		return;
	}

	if(m_bCaptureMouse)return;

	//�������
	m_bCaptureMouse=true;

	//�滭�˿�
	INT nXPost=0,nYPost=0;
	for(BYTE i=0;i<m_CardDataArray.GetCount();i++)
	{
		if(i==2)break;
		//��������
		BYTE cbCardData=m_CardDataArray[i];
		int nDrawWidth=(m_CardDataArray.GetCount()==1+i)?m_CardSize.cx:m_dwCardHSpace;

		//ת������
		if(cbCardData==0x4e)cbCardData=0x42;

		//����λ��
		DWORD dwXImagePos=((cbCardData&CARD_MASK_VALUE)-1)*m_CardSize.cx;
		DWORD dwYImagePos=((cbCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;

		//�滭�˿�
		CClientDC ClientDC(this);
		//CImageHandle HandleCard(&m_ImageCard);
		m_ImageCard.TransDrawImage(&ClientDC,nXPost,0,nDrawWidth,m_CardSize.cy,dwXImagePos,dwYImagePos,RGB(255,0,255));
		nXPost += nDrawWidth;
	}

	return;
}

//�����Ϣ
BOOL CCardControl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//if (m_bHitPositively)
	//{
		////��ȡ���
		//CPoint MousePoint;
		//GetCursorPos(&MousePoint);
		//ScreenToClient(&MousePoint);

		////λ���ж�
		//for(INT i=0;i<m_CardDataArray.GetCount();i++)
		//{
		//	INT nX=i*(INT)m_dwCardHSpace;
		//	INT nWidth=(m_CardDataArray.GetCount()-1==i)?m_CardSize.cx:m_dwCardHSpace;
		//	if(MousePoint.x>=nX && MousePoint.x<=nX+nWidth && m_CardDataArray[i]!=0xff)
		//	{
		//		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
		//		return TRUE;
		//	}
		//}

		////λ���ж�
		//if (MousePoint.x<(int)m_dwCardHSpace*(m_CardDataArray.GetCount()-1)+m_CardSize.cx)
		//{
		//	SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
		//	return TRUE;
		//}
	//}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//��������
void CCardControl::AnalyseMingCard()
{
	//״̬�ж�
	if (m_bCaptureMouse==false) return;

	//�ͷŲ���
	ReleaseCapture();
	m_bCaptureMouse=false;

	//ˢ�½���
	CRect rcInvalid(0,0,(m_CardDataArray.GetCount()==2)?((int)m_dwCardHSpace+m_CardSize.cx):((int)m_dwCardHSpace*2),m_CardSize.cy);
	InvalidateRect(rcInvalid,FALSE);

	return;
}

//��ȡ��Ŀ
DWORD CCardControl::GetCardCount()
{
	return (WORD)m_CardDataArray.GetCount();
}

//��ȡ��Ŀ
DWORD CCardControl::GetUnDisableCardCount()
{
	DWORD dwCount=0;
	for(WORD i=0;i<m_CardDataArray.GetCount();i++)
	{
		if(m_CardDataArray.GetAt(i)!=0)dwCount++;
	}
	return dwCount;
}

//��ʾ�˿�
void CCardControl::SetDisplayHead(bool bDisplayHead)
{
	//״̬�ж�
	if (m_bDisplayHead==bDisplayHead) return;

	//���ñ���
	m_bDisplayHead=bDisplayHead;

	//�ػ��ؼ�
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//������˸
void CCardControl::SetCardFlash(bool bFlash)
{
	//���ñ���
	m_bFlash=bFlash;

	//�ػ��ؼ�
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//������Ч
void CCardControl::SetDisableCardBack(bool bDisable)
{
	//���ñ���
	m_bDisable=bDisable;

	//�ػ��ؼ�
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//���ü��
void CCardControl::SetCardSpace(DWORD dwCardSpace)
{
	//���ñ���
	m_dwCardHSpace=dwCardSpace;

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//������Ӧ
void CCardControl::SetPositively(bool bPositively)
{
	//���ñ���
	m_bPositively=bPositively;

	return;
}

//�����˿�
DWORD CCardControl::GetCardData(BYTE cbCardData[], DWORD dwMaxCount)
{
	//�����˿�
	DWORD dwCopyCount=__min((DWORD)m_CardDataArray.GetCount(),dwMaxCount);
	CopyMemory(cbCardData,m_CardDataArray.GetData(),sizeof(BYTE)*dwCopyCount);

	return dwCopyCount;
}

//�����˿�
BYTE CCardControl::GetCardData(BYTE cbIndex)
{
	if(cbIndex>m_CardDataArray.GetCount()-1)return 0;

	return m_CardDataArray.GetAt(cbIndex);
}

//�����˿�
DWORD CCardControl::SetCardData(const BYTE cbCardData[], DWORD dwCardCount)
{
	//�����˿�
	m_CardDataArray.SetSize(dwCardCount);
	CopyMemory(m_CardDataArray.GetData(),cbCardData,sizeof(BYTE)*dwCardCount);

	//����λ��
	RectifyControl();

	return dwCardCount;
}

//�����˿�
DWORD CCardControl::SetNullityCardData(BYTE cbIndex,BYTE cbCardData)
{
	if(cbIndex>m_CardDataArray.GetCount()-1)return 0;

	m_CardDataArray[cbIndex]=cbCardData;

	//����λ��
	RectifyControl();

	return 1;
}

//��ȡ�˿�
DWORD CCardControl::GetNullityCardData(BYTE cbCardData)
{
	DWORD dwCount = 0;
	for(INT i=0;i<m_CardDataArray.GetCount();i++)
	{
		if(m_CardDataArray[i]==cbCardData)dwCount++;
	}

	return dwCount;
}

//��׼λ��
void CCardControl::SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//��׼λ��
void CCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//��ȡλ��
CPoint CCardControl::GetCardPos(bool bMaxCount)
{
	//��������
	DWORD dwCardCount=((bMaxCount)?(MAX_COUNT):((DWORD)m_CardDataArray.GetCount()));

	//�����С
	CSize ControlSize;
	ControlSize.cy=m_CardSize.cy;
	ControlSize.cx=(dwCardCount>0)?(m_CardSize.cx+(dwCardCount-1)*m_dwCardHSpace):0;

	//����λ��
	int nXPos=0;
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ nXPos=m_BenchmarkPos.x; break; }
	case enXCenter: { nXPos=m_BenchmarkPos.x-ControlSize.cx/2; break; }
	case enXRight:	{ nXPos=m_BenchmarkPos.x-ControlSize.cx; break; }
	}

	//����λ��
	int nYPos=0;
	switch (m_YCollocateMode)
	{
	case enYTop:	{ nYPos=m_BenchmarkPos.y; break; }
	case enYCenter: { nYPos=m_BenchmarkPos.y-ControlSize.cy/2; break; }
	case enYBottom: { nYPos=m_BenchmarkPos.y-ControlSize.cy; break; }
	}

	CPoint cpTemp;
	cpTemp.x=nXPos;
	cpTemp.y=nYPos;

	return cpTemp;
}

//�����ؼ�
void CCardControl::RectifyControl()
{
	//��������
	DWORD dwCardCount=(DWORD)m_CardDataArray.GetCount();

	//�����С
	CSize ControlSize;
	ControlSize.cy=m_CardSize.cy;
	ControlSize.cx=(dwCardCount>0)?(m_CardSize.cx+(dwCardCount-1)*m_dwCardHSpace):0;

	//����λ��
	int nXPos=0;
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ nXPos=m_BenchmarkPos.x; break; }
	case enXCenter: { nXPos=m_BenchmarkPos.x-ControlSize.cx/2; break; }
	case enXRight:	{ nXPos=m_BenchmarkPos.x-ControlSize.cx; break; }
	}

	//����λ��
	int nYPos=0;
	switch (m_YCollocateMode)
	{
	case enYTop:	{ nYPos=m_BenchmarkPos.y; break; }
	case enYCenter: { nYPos=m_BenchmarkPos.y-ControlSize.cy/2; break; }
	case enYBottom: { nYPos=m_BenchmarkPos.y-ControlSize.cy; break; }
	}

	//�ƶ�λ��
	SetWindowPos(NULL,nXPos,nYPos,ControlSize.cx,ControlSize.cy,SWP_NOZORDER);

	//��������
	CRgn CardRegion,SignedRegion;
	CardRegion.CreateRectRgn(0,0,0,0);
	for (DWORD i=0;i<dwCardCount;i++)
	{
		SignedRegion.CreateRoundRectRgn(i*m_dwCardHSpace,0,i*m_dwCardHSpace+m_CardSize.cx+1,m_CardSize.cy+1,2,2);
		CardRegion.CombineRgn(&CardRegion,&SignedRegion,RGN_OR);
		SignedRegion.DeleteObject();
	}

	//��������
	SetWindowRgn(CardRegion,TRUE);
	m_CardRegion.DeleteObject();
	m_CardRegion.Attach(CardRegion.Detach());

	//�ػ�����
	Invalidate(FALSE);

	return;
}

//////////////////////////////////////////////////////////////////////////

