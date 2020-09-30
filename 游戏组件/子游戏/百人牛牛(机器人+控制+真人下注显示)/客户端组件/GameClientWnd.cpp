#include "StdAfx.h"
#include "Resource.h"
#include "GameClientWnd.h"

//////////////////////////////////////////////////////////////////////////////////

//���ư�ť
#define IDC_MIN						100									//��С��ť
#define IDC_MAX						101									//���ť
#define IDC_CLOSE					102									//�رհ�ť

//�ؼ���ʶ
#define IDC_SKIN_SPLITTER			200									//��ֿؼ�
#define IDC_GAME_CLIENT_VIEW		201									//��ͼ��ʶ

//��Ļλ��
#define BORAD_SIZE					6									//�߿��С
#define CAPTION_SIZE				32									//�����С

//�ؼ���С
#define SPLITTER_CX					0									//��ֿ��
#define CAPTION_SIZE				32									//�����С

//��Ļ����
#define LESS_SCREEN_CY				740									//��С�߶�
#define LESS_SCREEN_CX				1024								//��С���

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientWnd, CFrameWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETTINGCHANGE()

	//�Զ���Ϣ
	ON_MESSAGE(WM_SETTEXT,OnSetTextMesage)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CFrameEncircle::CFrameEncircle()
{
}

//��������
CFrameEncircle::~CFrameEncircle()
{
}

//�滭����
bool CFrameEncircle::PreDrawEncircleImage(tagEncircleImage & EncircleImage)
{
	//����ͼ��
	CBitImage ImageLogo;
	ImageLogo.LoadFromResource(AfxGetInstanceHandle(),TEXT("GAME_LOGO"));

	//�滭ͼ��
	if (ImageLogo.IsNull()==false)
	{
		//��������
		CDC * pDC=CDC::FromHandle(EncircleImage.ImageTL.GetDC());

		//�滭ͼ��
		ImageLogo.TransDrawImage(pDC,10,4,RGB(255,0,255));

		//�ͷ���Դ
		EncircleImage.ImageTL.ReleaseDC();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientWnd::CGameClientWnd()
{
	//״̬����
	m_bMaxShow=false;
	m_bShowControl=true;
	m_rcNormalSize.SetRect(0,0,0,0);

	//����ӿ�
	m_pIClientKernel=NULL;
	m_pIGameFrameView=NULL;
	m_pIGameFrameService=NULL;

	//��ܻ���
	tagEncircleResource	EncircleFrame;
	EncircleFrame.pszImageTL=MAKEINTRESOURCE(IDB_FRAME_TL);
	EncircleFrame.pszImageTM=MAKEINTRESOURCE(IDB_FRAME_TM);
	EncircleFrame.pszImageTR=MAKEINTRESOURCE(IDB_FRAME_TR);
	EncircleFrame.pszImageML=MAKEINTRESOURCE(IDB_FRAME_ML);
	EncircleFrame.pszImageMR=MAKEINTRESOURCE(IDB_FRAME_MR);
	EncircleFrame.pszImageBL=MAKEINTRESOURCE(IDB_FRAME_BL);
	EncircleFrame.pszImageBM=MAKEINTRESOURCE(IDB_FRAME_BM);
	EncircleFrame.pszImageBR=MAKEINTRESOURCE(IDB_FRAME_BR);
	m_FrameEncircle.InitEncircleResource(EncircleFrame,GetModuleHandle(GAME_FRAME_DLL_NAME));

	//ע�����
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	pGlobalUnits->RegisterGlobalModule(MODULE_GAME_FRAME_WND,QUERY_ME_INTERFACE(IUnknownEx));

	return;
}

//��������
CGameClientWnd::~CGameClientWnd()
{
}

//�ӿڲ�ѯ
VOID * CGameClientWnd::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameFrameWnd,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameFrameWnd,Guid,dwQueryVer);
	return NULL;
}

//��Ϣ����
BOOL CGameClientWnd::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//�����
BOOL CGameClientWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//���ܰ�ť
	switch (nCommandID)
	{
	case IDC_MIN:				//��С��ť
		{
			ShowWindow(SW_MINIMIZE);
			return TRUE;
		}
	case IDC_MAX:				//���ť
		{
			//���ڿ���
			if (m_bMaxShow==true)
			{
				RestoreWindow();
			}
			else
			{
				MaxSizeWindow();
			}

			//���½���
			RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW);

			return TRUE;
		}
	case IDC_CLOSE:				//�رհ�ť
		{
			PostMessage(WM_CLOSE,0,0);
			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//��ԭ����
bool CGameClientWnd::RestoreWindow()
{
	//״̬�ж�
	if (m_bMaxShow==true)
	{
		//���ñ���
		m_bMaxShow=false;

		//���ð�ť
		m_btMax.SetButtonImage(IDB_BT_MAX,GetModuleHandle(GAME_FRAME_DLL_NAME),false,false);

		//�ƶ�����
		LockWindowUpdate();
		SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//��󴰿�
bool CGameClientWnd::MaxSizeWindow()
{
	//״̬�ж�
	if (m_bMaxShow==false)
	{
		//���ñ���
		m_bMaxShow=true;

		//Ĭ��λ��
		GetWindowRect(&m_rcNormalSize);

		//���ð�ť
		m_btMax.SetButtonImage(IDB_BT_RESORE,GetModuleHandle(GAME_FRAME_DLL_NAME),false,false);

		//��ȡλ��
		CRect rcArce;
		SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

		//�ƶ�����
		LockWindowUpdate();
		SetWindowPos(NULL,rcArce.left-2,rcArce.top-2,rcArce.Width()+4,rcArce.Height()+4,SWP_NOZORDER);
		UnlockWindowUpdate();
	}

	return true;
}

//�����ؼ�
VOID CGameClientWnd::RectifyControl(INT nWidth, INT nHeight)
{
	//״̬�ж�
	if ((nWidth==0)||(nHeight==0)) return;

	//�ƶ�׼��
	HDWP hDwp=BeginDeferWindowPos(32);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	//������Ϣ
	tagEncircleInfo EncircleInfoFrame;
	m_FrameEncircle.GetEncircleInfo(EncircleInfoFrame);

	//��������
	CRect rcSplitter;
	rcSplitter.top=EncircleInfoFrame.nTBorder;
	rcSplitter.bottom=nHeight-EncircleInfoFrame.nBBorder;

	//��ֿؼ�
	if (m_bShowControl==true)
	{
		rcSplitter.left=nWidth*75L/100L;
		rcSplitter.right=rcSplitter.left+SPLITTER_CX;
	}
	else
	{
		rcSplitter.left=nWidth-EncircleInfoFrame.nRBorder-SPLITTER_CX;
		rcSplitter.right=nWidth-EncircleInfoFrame.nRBorder;
	}

	//��ѯ��Ϸ
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
	IGameFrameView * pIGameFrameView=(IGameFrameView *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_VIEW,IID_IGameFrameView,VER_IGameFrameView);

	//��Ϸ��ͼ
	if (pIGameFrameView!=NULL)
	{
		HWND hWndView=pIGameFrameView->GetGameViewHwnd();
		DeferWindowPos(hDwp,hWndView,NULL,EncircleInfoFrame.nLBorder,EncircleInfoFrame.nTBorder,rcSplitter.left-EncircleInfoFrame.nLBorder,
			nHeight-EncircleInfoFrame.nTBorder-EncircleInfoFrame.nBBorder,uFlags);
	}

	//���ư�ť
	DeferWindowPos(hDwp,m_btMin,NULL,nWidth-87,4,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btMax,NULL,nWidth-59,4,0,0,uFlags|SWP_NOSIZE);
	DeferWindowPos(hDwp,m_btClose,NULL,nWidth-31,4,0,0,uFlags|SWP_NOSIZE);

	//�ƶ��ؼ�
	DeferWindowPos(hDwp,m_SkinSplitter,NULL,rcSplitter.left,rcSplitter.top,rcSplitter.Width(),rcSplitter.Height(),uFlags);
	DeferWindowPos(hDwp,m_GameFrameControl,NULL,rcSplitter.right,EncircleInfoFrame.nTBorder,nWidth-rcSplitter.right-EncircleInfoFrame.nRBorder,
		nHeight-EncircleInfoFrame.nBBorder-EncircleInfoFrame.nTBorder,uFlags);

	//�ƶ�����
	EndDeferWindowPos(hDwp);

	return;
}

//�滭����
BOOL CGameClientWnd::OnEraseBkgnd(CDC * pDC)
{
	//��ȡ����
	CString strTitle;
	GetWindowText(strTitle);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���λ��
	tagEncircleInfo FrameEncircleInfo;
	m_FrameEncircle.GetEncircleInfo(FrameEncircleInfo);

	//�滭���
	m_FrameEncircle.DrawEncircleFrame(pDC,rcClient);

	//�滭����
	if (strTitle.IsEmpty()==false)
	{
		//��������
		CRect rcTitle;
		rcTitle.SetRect(35,8,rcClient.Width()-150,FrameEncircleInfo.nTBorder-8);

		//��������
		INT nXMove[8]={1,1,1,0,-1,-1,-1,0};
		INT nYMove[8]={-1,0,1,1,1,0,-1,-1};

		//���û���
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(CSkinDialog::m_SkinAttribute.m_crTitleFrame);
		pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

		//�滭�߿�
		for (INT i=0;i<CountArray(nXMove);i++)
		{
			//����λ��
			CRect rcTitleFrame;
			rcTitleFrame.top=rcTitle.top+nYMove[i];
			rcTitleFrame.left=rcTitle.left+nXMove[i];
			rcTitleFrame.right=rcTitle.right+nXMove[i];
			rcTitleFrame.bottom=rcTitle.bottom+nYMove[i];

			//�滭�ַ�
			pDC->DrawText(strTitle,&rcTitleFrame,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		}

		//�滭����
		pDC->SetTextColor(CSkinDialog::m_SkinAttribute.m_crTitleText);
		pDC->DrawText(strTitle,&rcTitle,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}


	return TRUE;
}

//��������
UINT CGameClientWnd::OnNcHitTest(CPoint Point)
{
	//��ť����
	if (m_bMaxShow==false)
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//ת��λ��
		CPoint ClientPoint=Point;
		ScreenToClient(&ClientPoint);

		//����λ��
		if (ClientPoint.x<=BORAD_SIZE)
		{
			if (ClientPoint.y<=CAPTION_SIZE) return HTTOPLEFT;
			if (ClientPoint.y>=(rcClient.Height()-BORAD_SIZE)) return HTBOTTOMLEFT;
			return HTLEFT;
		}

		//����λ��
		if (ClientPoint.x>=(rcClient.Width()-BORAD_SIZE))
		{
			if (ClientPoint.y<=CAPTION_SIZE) return HTTOPRIGHT;
			if (ClientPoint.y>=(rcClient.Height()-BORAD_SIZE)) return HTBOTTOMRIGHT;
			return HTRIGHT;
		}

		//����λ��
		if (ClientPoint.y<=BORAD_SIZE) return HTTOP;
		if (ClientPoint.y>=(rcClient.Height()-BORAD_SIZE)) return HTBOTTOM;
	}

	return __super::OnNcHitTest(Point);
}

//λ����Ϣ
VOID CGameClientWnd::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//λ����Ϣ
VOID CGameClientWnd::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);

	//����λ��
	lpMMI->ptMinTrackSize.x=LESS_SCREEN_CX;
	lpMMI->ptMinTrackSize.y=LESS_SCREEN_CY;

	return;
}

//������Ϣ
INT CGameClientWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ô���
	ModifyStyle(WS_CAPTION,0,0);
	ModifyStyleEx(WS_BORDER|WS_EX_CLIENTEDGE|WS_EX_WINDOWEDGE,0,0);

	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//��ѯ�ӿ�
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);
	m_pIGameFrameView=(IGameFrameView *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_VIEW,IID_IGameFrameView,VER_IGameFrameView);
	m_pIGameFrameService=(IGameFrameService *)pGlobalUnits->QueryGlobalModule(MODULE_GAME_FRAME_SERVICE,IID_IGameFrameService,VER_IGameFrameService);

	//������ť
	CRect rcCreate(0,0,0,0);
	m_btMin.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MIN);
	m_btMax.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_MAX);
	m_btClose.Create(NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_CLOSE);

	//���ð�ť
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_btMin.SetButtonImage(IDB_BT_MIN,hInstance,false,false);
	m_btMax.SetButtonImage(IDB_BT_MAX,hInstance,false,false);
	m_btClose.SetButtonImage(IDB_BT_CLOSE,TEXT("BT_CLOSE"),hInstance,false,false);

	//���ƴ���
	AfxSetResourceHandle(hInstance);
	m_GameFrameControl.Create(IDD_GAME_CONTROL,this);
	m_SkinSplitter.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_SKIN_SPLITTER);

	//��ʾ�ؼ�
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btMin,TEXT("��С��"));
	m_ToolTipCtrl.AddTool(&m_btMax,TEXT("���"));
	m_ToolTipCtrl.AddTool(&m_btClose,TEXT("�ر���Ϸ"));

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//��ȡλ��
	CSize SizeRestrict;
	CWHRegKey InfoKeyItem;
	if (InfoKeyItem.OpenRegKey(REG_INTERFACE_INFO,false)==true)
	{
		SizeRestrict.cx=InfoKeyItem.GetValue(TEXT("ScreenWidth"),rcArce.Width()/2);
		SizeRestrict.cy=InfoKeyItem.GetValue(TEXT("ScreenHeight"),rcArce.Height()/2);
	}

	//λ�õ���
	SizeRestrict.cx=__max(LESS_SCREEN_CX,SizeRestrict.cx);
	SizeRestrict.cy=__max(LESS_SCREEN_CY,SizeRestrict.cy);
	SizeRestrict.cx=__min(rcArce.Width(),SizeRestrict.cx);
	SizeRestrict.cy=__min(rcArce.Height(),SizeRestrict.cy);

	//�ƶ�����
	m_rcNormalSize.top=(rcArce.Height()-SizeRestrict.cy)/2;
	m_rcNormalSize.left=(rcArce.Width()-SizeRestrict.cx)/2;
	m_rcNormalSize.right=m_rcNormalSize.left+SizeRestrict.cx;
	m_rcNormalSize.bottom=m_rcNormalSize.top+SizeRestrict.cy;
	SetWindowPos(NULL,m_rcNormalSize.left,m_rcNormalSize.top,m_rcNormalSize.Width(),m_rcNormalSize.Height(),SWP_NOZORDER);

	//������ͼ
	ASSERT(m_pIGameFrameView!=NULL);
	if (m_pIGameFrameView!=NULL) m_pIGameFrameView->CreateGameViewWnd(this,IDC_GAME_CLIENT_VIEW);

	//��ʾ����
	MaxSizeWindow();

	return 0L;
}

//�����Ϣ
VOID CGameClientWnd::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//ģ�����
	if ((IsZoomed()==FALSE)&&(Point.y<=CAPTION_SIZE))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}

	return;
}

//�����Ϣ
VOID CGameClientWnd::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDblClk(nFlags,Point);

	//״̬�ж�
	if (Point.y>CAPTION_SIZE) return;

	//���ڿ���
	if (m_bMaxShow==true)
	{
		RestoreWindow();
	}
	else
	{
		MaxSizeWindow();
	}
	
	return;
}

//���øı�
VOID CGameClientWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__super::OnSettingChange(uFlags,lpszSection);

	//��ȡ��С
	CRect rcClient,rcWindow;
	GetClientRect(&rcClient);
	GetWindowRect(&rcWindow);
	ClientToScreen(&rcClient);

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,0);

	//����λ��
	rcArce.top-=(rcClient.top-rcWindow.top);
	rcArce.left-=(rcClient.left-rcWindow.left);
	rcArce.right+=(rcWindow.right-rcClient.right);
	rcArce.bottom+=(rcWindow.bottom-rcClient.bottom);

	//�ƶ�����
	SetWindowPos(NULL,rcArce.left,rcArce.top,rcArce.Width(),rcArce.Height(),SWP_NOZORDER|SWP_NOCOPYBITS);

	return;
}

//������Ϣ
LRESULT	CGameClientWnd::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	//Ĭ�ϵ���
	LRESULT lResult=DefWindowProc(WM_SETTEXT,wParam,lParam);

	//���±���
	Invalidate(TRUE);

	return lResult;
}

//////////////////////////////////////////////////////////////////////////////////
