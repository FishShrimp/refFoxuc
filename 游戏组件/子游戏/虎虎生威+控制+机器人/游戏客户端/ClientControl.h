#pragma once
#define IDM_ADMIN_COMMDN			WM_USER+2007
#define IDM_GET_ACCOUNT				WM_USER+2008
//��Ϸ���ƻ���
class IClientControlDlg : public CDialog 
{
public:
	IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent){}
	virtual ~IClientControlDlg(void){}

public:
	//���¿���
	virtual void __cdecl UpdateControl(CMD_S_ControlReturns* pControlReturns) = NULL;
	virtual void __cdecl ResetUserBet() = NULL;
	virtual void __cdecl SetUserBetScore(BYTE cbArea,LONGLONG lScore) = NULL;
	virtual void __cdecl SetUserGameScore(BYTE cbArea,LONGLONG lScore) = NULL;
	virtual void __cdecl SetUserNickName(CString strNickName) = NULL;
	virtual void __cdecl ResetUserNickName() = NULL;
};
