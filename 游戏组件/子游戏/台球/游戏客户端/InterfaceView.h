#pragma once

#ifdef _UNICODE
#define VER_ITQView INTERFACE_VERSION(1,1)
static const GUID IID_ITQView={0x12f43bfc,0xf886,0x42f5,0x0099,0xf2,0x40,0xa8,0xd8,0xf1,0x3b,0xd8};
#else
#define VER_ITQView INTERFACE_VERSION(1,1)
static const GUID IID_ITQView={0xf6f9222d,0x6e8b,0x4834,0x009d,0xa5,0x31,0x7d,0x27,0xce,0x1e,0x33};
#endif


//��Ϸ��ͼ�ӿ�
interface ITQView : public IUnknownEx
{
public:
	// ��ȡHGEָ��
	virtual HGE* __cdecl GetHge(void)=NULL;

public:
	// ���Ӱ�ť
	virtual void __cdecl AddButton(BYTE cbID, const char *filename, DWORD size = 0, bool bMipmap = false)=NULL;
	// ɾ����ť
	virtual void __cdecl DelButton(BYTE cbID, bool bAll = false)=NULL;
	// ���ð�ť
	virtual void __cdecl SetButtonPos(BYTE cbID, CPoint pt)=NULL;
	// ������ʾ
	virtual void __cdecl SetButtonShow(BYTE cbID, WORD wType)=NULL;
	// ��ȡ��С
	virtual void __cdecl GetButtonSize(BYTE cbID, CSize& size)=NULL;
	// ��ť�¼�
	virtual void __cdecl OnEventButton(WORD wButtonID, BYTE cbButtonType)=NULL;

public:
	// ���Ӱ�ť
	virtual void __cdecl AddSound(WORD wID, const char* fileName)=NULL;
	// ɾ������
	virtual void __cdecl DelSound(WORD wID, bool bAll = false)=NULL;
	// ��������
	virtual void __cdecl PlayGameSound(WORD wID, bool bLoop = false)=NULL;
};