#pragma once


#include <math.h>
#include <list>


//////////////////////////////////////////////////////////////////////////
#define TY_BTSHOW		0x0001					// ��ʾ
#define TY_BTHIDE		0x0002					// ����
#define TY_BTENABLE		0x0004					// ����
#define TY_BTUNENABLE	0x0008					// ������

// ������
class CGameClientView;
//////////////////////////////////////////////////////////////////////////
// ͼƬ��
class CHgeImage
{
public:
	CHgeImage(void);
	virtual ~CHgeImage(void);

	// �ڲ�����
private:
	HGE*								m_pHge;
	hgeSprite*							m_pImage;

	// ͼƬ����
private:
	float								m_fWidth;
	float								m_fHeight;
	float								m_fItemWidth;
	float								m_fItemHeight;

	// ��������
public:
	// ��ȡ����
	float GetWidth(void) { return m_fWidth; }
	// ��ȡ����
	float GetHeight(void) { return m_fHeight; }
	// ��ȡ����
	float GetItemWidth(void) { return m_fItemWidth; }
	// ��ȡ����
	float GetItemHeight(void) { return m_fItemHeight; }

	// ���ܺ���
public:
	// װ��ͼƬ
	bool LoadImage(const char *filename, int nxNum = 1, int nyNum = 1, DWORD size = 0, bool bMipmap = false);
	// ж��ͼƬ
	void DestroyImage(void);
	// ����ͼƬ
	void DrawImage(float x, float y);
	// ����ͼƬ
	void DrawImage(float x, float y, float rot, float hscale = 1.0f, float vscale = 0.0f);
};

//////////////////////////////////////////////////////////////////////////
const int BUTTON_COUNT = 4;

// ��ť�� 
class CHgeButton
{
public:
	CHgeButton();
	virtual ~CHgeButton();

	// ״̬
public:
	enum { BT_Normal = 0, BT_Move, BT_Down, BT_UnEable };

	// ����״̬
private:
	bool							m_bLoad;
	bool							m_bVisible;
	bool							m_bEnable;
	BYTE							m_cbState;

	// ��ʾ״̬
private:
	HGE*							m_pHge;
	hgeSprite*						m_pBtSprite[BUTTON_COUNT];

	// �������
private:
	CRect							m_ptBase;

	// ��������
public:
	// ���ÿɼ�
	void SetVisible(bool b) { m_bVisible = b; }
	// ���ÿ���
	void SetEnable(bool b) { m_bEnable = b; }
	// ��ȡ�ɼ�
	bool GetVisible(void) { return m_bVisible; }
	// ��ȡ����
	bool GetEnable(void) { return m_bEnable; }
	// ����״̬
	void SetState(BYTE cb) { m_cbState = cb; }
	// ��ȡ״̬
	BYTE GetState(void) { return m_cbState; }
	// ���ð�ťλ��
	void SetPos(CPoint pt);
	// ��ȡ��ťλ��
	CPoint GetPos(void);
	// ��ȡ��ť��С
	CSize GetSize(void);
	// �ڰ�ť��
	bool GetInBt(CPoint pt) { return m_ptBase.PtInRect(pt); }
	// ���ܺ���
public:
	// װ�ذ�ť��Դ
	bool LoadImage(const char *filename, DWORD size = 0, bool bMipmap = false);
	// ���ư�ť
	void DrawButton(void);
	// ɾ����ť
	void DestroyButton(void);
};

//////////////////////////////////////////////////////////////////////////
struct tagBtItem
{
	CHgeButton* pBt;
	BYTE		cbID;
};

typedef CWHArray<tagBtItem> HgeBtArray;

// ��ť����
class CHgeButtonMgr
{
public:
	CHgeButtonMgr(CGameClientView* pFrameView);
	virtual ~CHgeButtonMgr();

	// ��ť����
private:
	CGameClientView*				m_pIGameFrameView;
	HgeBtArray						m_BtArray;

	// ���״̬
private:
	bool							m_bMLDown;
	bool							m_bMLUP;
	CPoint							m_ptMouse;

	// ���ܺ���
public:
	// ���Ӱ�ť
	void AddButton(BYTE cbID, const char *filename, DWORD size = 0, bool bMipmap = false);
	// ɾ����ť
	void DelButton(BYTE cbID);
	// ɾ����ť
	void DelButton(void);
	// ���°�ť
	void UpdateButton(void);
	// ���ư�ť
	void DrawButton(void);
	// ����λ��
	void SetButtonPt(BYTE cbID, CPoint pt);
	// �������λ��
	void SetMMove(CPoint pt);
	// ����������
	void SetMLDown(CPoint pt);
	// ����������
	void SetMLUp(CPoint pt);
	// ��ȡ��ť����
	bool IsHaveButton(BYTE cbID);
	// ������ʾ
	void SetButtonShow(BYTE cbID, WORD wType);
	// ��ȡ��С
	void GetButtonSize(BYTE cbID, CSize& size);
};

//////////////////////////////////////////////////////////////////////////
// ������
class CHgeSprite : public hgeSprite
{
	// ����
public:
	CHgeSprite(HTEXTURE tex, float x, float y, float w, float h, int nItem = 1);

	// ͼƬ����
private:
	float								m_fWidth;
	float								m_fHeight;
	float								m_fItemWidth;
	float								m_fItemHeight;

	// ��������
public:
	// ��ȡ����
	float GetWidth(void) { return m_fWidth; }
	// ��ȡ����
	float GetHeight(void) { return m_fHeight; }
	// ��ȡ����
	float GetItemWidth(void) { return m_fItemWidth; }
	// ��ȡ����
	float GetItemHeight(void) { return m_fItemHeight; }
};

//////////////////////////////////////////////////////////////////////////
struct tagSdItem
{
	HEFFECT hSound;
	WORD	wSoundID;
};

typedef CWHArray<tagSdItem> HgeSdArray;

// ������
class CHgeSound
{
	// ����
public:
	CHgeSound(CGameClientView* pFrameView);
	virtual~CHgeSound();

	// �ڲ�����
protected:
	CGameClientView*				m_pIGameFrameView;
	HgeSdArray						m_HgeSdArray;
	HGE*							m_pHge;

	// ���ܺ���
public:
	// ��������
	void AddSound(WORD wID, const char* fileName);
	// ɾ������
	void DelSound(WORD wID);
	// ɾ������
	void DelSound();
	// ��������
	void PlayGameSound(WORD wID, bool bLoop = false);
	// �ж���������
	bool IsHaveSound(WORD wID);

	// ��������
private:
	// ��ȡ����λ��
	int GetSoundIndex(WORD wID);
};

//////////////////////////////////////////////////////////////////////////
// ��������ṹ
typedef struct tagEngineFontGlyph
{
	float		x;
	float		y;
	float		w;
	float		h;
	HTEXTURE	t;
	wchar_t		c;
}TENGINEFONTGLYPH;
typedef CWHArray<HTEXTURE> HgeTextureArray;

// ������
class CHgeFont
{
public:
	//CHgeFont(const char* lpsFontName, int nFaceSize, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bUnderline = FALSE, BOOL bAntialias = TRUE);

	CHgeFont(
		const char* lpsFontName = "������",
		int nFaceSize=12,
		BOOL bBold = FALSE,
		BOOL bItalic = FALSE,
		BOOL bAntialias = FALSE);
	~CHgeFont(void);

public:
	// ��Ⱦ�ı�
	virtual void	Print( float x, float y, const char *format, ... );
	virtual void	PrintCenter( float x, float y, const char *format, ... );
	virtual void	Render(float x, float y, const wchar_t* text );

	// �������ȡ��ɫ
	virtual void	SetColor( DWORD dwColor, int i = -1 );
	virtual DWORD	GetColor( int i = 0 );

	// ��ȡ�ı����
	virtual SIZE	GetTextSize( const wchar_t* text );

	// �����ּ��
	virtual void	SetKerningWidth( float kerning );
	virtual void	SetKerningHeight( float kerning );

	// ��ȡ�ּ��
	virtual float	GetKerningWidth();
	virtual float	GetKerningHeight();

	// �����С
	virtual float	GetFontSize();

public:
	// �����ַ���ȡ����
	void		 	GetGlyphByCharacter( wchar_t c );
	float			GetWidthFromCharacter(wchar_t c);
	void			CacheCharacter(wchar_t c);
	//char wchar_tת������
	bool			MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize);

private:
	typedef struct tagEngineFontGlyph
	{
		HTEXTURE	t;
		float		w;
		float		h;
		float		x;
		float		y;
		float		c;
		wchar_t		wcIndex;
	}TENGINEFONTGLYPH;

	typedef std::list<TENGINEFONTGLYPH *> GlyphList;
	typedef GlyphList::iterator GlyphListIterator;
	GlyphList			m_GlyphList;

	UINT				m_nAntialias;//�����
	LONG				m_nAscent;//����
	DWORD				m_dwFontColor;
	float				m_nFontSize;
	float				m_nKerningWidth;
	float				m_nKerningHeight;

	HGE*				m_pHGE;
	hgeSprite*			m_pSprite;

	// GDI�豸
	HDC					m_hMemDC;
	HFONT				m_hFont;
};

//////////////////////////////////////////////////////////////////////////
// �ı���
class CHgeText : public hgeGUIObject
{
public:
	CHgeText(int id, float x, float y, float w, float h, CHgeFont *fnt);
	virtual ~CHgeText();

	// �麯����ʵ��
protected:
	CHgeText(const CHgeText &go);
	CHgeText&	operator= (const CHgeText &go);
	
	// ���庯��
private:
	CHgeFont*					m_pHgeFont;
	float						m_tx;
	float						m_ty;
	int							m_nAlign;
	char						m_text[256];

	// ���ܺ���
public:
	void			SetMode(int _align);
	void			SetPos(int x, int y);
	void			SetText(const char *_text);
	void			printf(const char *format, ...);
	void			printfEx(const char *format, ...);
	virtual void	Render();	
};


//////////////////////////////////////////////////////////////////////////
// װ����
class CHgeLoad
{
public:
	CHgeLoad(void);
	virtual ~CHgeLoad(void);

	// HGE
private:
	HGE*							m_pHge;

	// ���ܺ���
public:
	// װ�ؾ���
	CHgeSprite* LoadHgeSprite(const char* fileName, int nItem = 1);
};