#include "StdAfx.h"
#include "Resource.h"
#include "ApplyUserList.h"
#include ".\applyuserlist.h"

//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CMyListCtrl::OnEraseBkgnd(CDC* pDC)
{
	CClientDC dc(this);
	CRect clientRect;
	GetClientRect(&clientRect);

	CDC bitmapDC;
	bitmapDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_APPLY_USER_BACK);
	CBitmap* pOldBitmap = bitmapDC.SelectObject(&bitmap);
	pDC->StretchBlt(clientRect.left,clientRect.top,clientRect.right-clientRect.left,clientRect.bottom-clientRect.top,&bitmapDC,0,0,clientRect.right-clientRect.left,clientRect.bottom-clientRect.top,SRCCOPY);
	bitmapDC.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
	pOldBitmap = NULL;
	return TRUE;
}


BEGIN_MESSAGE_MAP(CApplyUser, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CApplyUser::CApplyUser() : CDialog(IDD_DLG_GAME_RECORD)
{
}

//��������
CApplyUser::~CApplyUser()
{
}

//�ؼ���
void CApplyUser::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RECORD_LIST, m_AppyUserList);
}

//��ʼ������
BOOL CApplyUser::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	TCHAR szBuffer[_MAX_PATH];     
	CString   sPath;   
	::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH);
	sPath.Format(_T("res://%s/#2/#193"),szBuffer);

	LVBKIMAGE   bki;   
	bki.ulFlags   =   LVBKIF_STYLE_TILE   |   LVBKIF_SOURCE_URL   ;   
	bki.pszImage   =   sPath.GetBuffer(sPath.GetLength());   
	bki.cchImageMax   =   sPath.GetLength();   
	BOOL bSuccess =m_AppyUserList.SetBkImage(&bki);  
	m_AppyUserList.SetTextBkColor(-1);
	m_AppyUserList.SetTextColor(RGB(232, 245, 3));

	//�����б�
	m_AppyUserList.InsertColumn(0,TEXT("�������"),LVCFMT_LEFT,150);
	m_AppyUserList.InsertColumn(1,TEXT("��Ϸ��"),LVCFMT_LEFT,100);

	//tagApplyUser ApplyUser;
	//ApplyUser.strUserName = "kdjfdksdsdsdkjkjkjkjkjkjkfjdkjfj";
	//ApplyUser.lUserScore = 1000000;

	//InserUser(ApplyUser);
	//InserUser(ApplyUser);
	//InserUser(ApplyUser);
	//InserUser(ApplyUser);
	//InserUser(ApplyUser);
	//InserUser(ApplyUser);
	//InserUser(ApplyUser);
	//InserUser(ApplyUser);
	//InserUser(ApplyUser);
	//InserUser(ApplyUser);
	//InserUser(ApplyUser);
	//InserUser(ApplyUser);
	//InserUser(ApplyUser);
	//InserUser(ApplyUser);
	//InserUser(ApplyUser);
	//InserUser(ApplyUser);

	return FALSE;
}

//�����б�
void CApplyUser::InserUser(tagApplyUser & ApplyUser)
{
	//��������
	WORD wListIndex=0;
	WORD wColumnCount=1;
	TCHAR szBuffer[128]=TEXT("");

	//��Ϸ���
	INT nItemIndex=m_AppyUserList.InsertItem(m_AppyUserList.GetItemCount(),ApplyUser.strUserName);

	//��ҽ��
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%I64d"),ApplyUser.lUserScore);
	m_AppyUserList.SetItem(nItemIndex,wColumnCount++,LVIF_TEXT,szBuffer,0,0,0,0);

	return;
}

//λ�ñ仯
void CApplyUser::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//�б�ؼ�
	if (m_AppyUserList.m_hWnd!=NULL)
	{
		m_AppyUserList.MoveWindow(0,0,cx,cy);
	}

	return;
}

//ɾ���б�
void CApplyUser::DeleteUser(tagApplyUser & ApplyUser)
{
	//�������
	LVFINDINFO lvFindInfo;
	ZeroMemory( &lvFindInfo, sizeof( lvFindInfo ) );
	lvFindInfo.flags = LVFI_STRING;
	lvFindInfo.psz = (LPCTSTR)ApplyUser.strUserName;

	//��������
	int nItem = m_AppyUserList.FindItem( &lvFindInfo );

	//ɾ������
	if ( nItem != -1 ) m_AppyUserList.DeleteItem( nItem );
}

//�����б�
void CApplyUser::UpdateUser( tagApplyUser & ApplyUser )
{
	//�������
	LVFINDINFO lvFindInfo;
	ZeroMemory( &lvFindInfo, sizeof( lvFindInfo ) );
	lvFindInfo.flags = LVFI_STRING;
	lvFindInfo.psz = (LPCTSTR)ApplyUser.strUserName;

	//��������
	int nItem = m_AppyUserList.FindItem( &lvFindInfo );

	//ɾ������
	if ( nItem != -1 ) 
	{
		TCHAR szBuffer[128]=TEXT("");

		//��ҽ��
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("%I64d"),ApplyUser.lUserScore);
		m_AppyUserList.SetItem(nItem,1,LVIF_TEXT,szBuffer,0,0,0,0);
	}
}

//����б�
void CApplyUser::ClearAll()
{
	m_AppyUserList.DeleteAllItems();
}

//�������
bool CApplyUser::FindUser(LPCTSTR lpszUserName)
{
	//�������
	LVFINDINFO lvFindInfo;
	ZeroMemory( &lvFindInfo, sizeof( lvFindInfo ) );
	lvFindInfo.flags=LVFI_STRING;
	lvFindInfo.psz=lpszUserName;

	//��������
	int nItem=m_AppyUserList.FindItem(&lvFindInfo);

	//���ؽ��
	return -1==nItem ? false : true;
}
//////////////////////////////////////////////////////////////////////////

