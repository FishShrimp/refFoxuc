#pragma once

//��Ϸ���ƻ���
class IClientControlDlg : public CDialog 
{
public:
	IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent){}
	virtual ~IClientControlDlg(void){}

public:
	//���¿���
	virtual void  OnAllowControl(bool bEnable) = NULL;
	//������
	virtual bool  ReqResult(const void * pBuffer) = NULL;
	virtual void __cdecl UpdateControl() = NULL;
	
	//���¿��
	virtual bool __cdecl UpdateStorage(const void * pBuffer) = NULL;
};
