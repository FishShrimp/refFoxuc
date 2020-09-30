#pragma once

//���ݻ��㻭ͼ.. ��ʾ����ͼƬ����
struct tagBASE
{
	CPoint		ptBase;		//����
	int			nWidth;		//��
	int			nHeight;	//��
};

class CMyPngImage : public CPngImage
{
public:
	CMyPngImage(void){}
	~CMyPngImage(void){}

	//�滭
public:
	//�滭ͼ��
	bool DrawImage(CDC * pDC, tagBASE* pBase, INT nXPos, INT nYPos);
	//�滭ͼ��
	bool DrawImage(CDC * pDC, tagBASE* pBase, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT YSrc);
	//�滭ͼ��
	bool DrawImage(CDC * pDC, tagBASE* pBase, int nHeight, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT YSrc, INT nSrcWidth, INT nSrcHeight);

};

class CMySkinImage : public CBitImage
{
public:
	CMySkinImage(void){}
	~CMySkinImage(void){}

	//��ͼ����
public:
	//�滭ͼ��
	BOOL BitBlt( CDC * pDestDC,  tagBASE* pBase, INT nXPos, INT nYPos );
	//͸���滭
	bool AlphaDrawImage(CDC * pDestDC, tagBASE* pBase, INT nXPos, INT nYPos, COLORREF crTransColor);
	//͸���滭
	bool AlphaDrawImage(CDC * pDestDC, tagBASE* pBase, INT nXPos, INT nYPos, INT cxDest, INT cyDest, INT xSrc, INT ySrc, COLORREF crTransColor);

};

class CMyD3DTexture : public CD3DTexture
{
public:
	CMyD3DTexture(void){}
	~CMyD3DTexture(void){}

	//�滭����
public:
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, tagBASE* pBase, INT nXDest, INT nYDest);
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, tagBASE* pBase, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource);
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, tagBASE* pBase, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, INT nSourceWidth, INT nSourceHeight);

	//�滭����
public:
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, tagBASE* pBase, INT nXDest, INT nYDest, BYTE cbAlpha);
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, tagBASE* pBase, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, BYTE cbAlpha);
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, tagBASE* pBase, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, INT nSourceWidth, INT nSourceHeight, BYTE cbAlpha);

};
