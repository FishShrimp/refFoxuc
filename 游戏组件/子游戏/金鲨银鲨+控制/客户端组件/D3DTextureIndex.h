#pragma once

class CD3DTextureIndex : protected CD3DTexture
{
	//��Ϣ����
protected:
	int							m_nWidthNumber;				//������
	int							m_nHeightNumber;			//������

	//���캯��
public:
	CD3DTextureIndex(void);
	~CD3DTextureIndex(void);

	//ͼƬ��Ϣ
public:
	//�����ж�
	bool IsNull();
	//������
	int GetWidth() { return m_ImageSize.cx/m_nWidthNumber; }
	//����߶�
	int GetHeight() { return m_ImageSize.cy/m_nHeightNumber; }

	//���ܺ���
public:
	//��������
	bool LoadImage(CD3DDevice * pD3DDevice, HINSTANCE hInstance, LPCTSTR pszResource, DWORD dwColorKey, int nWidthNumber, int nHeightNumber );
	//��������
	bool LoadImage(CD3DDevice * pD3DDevice, HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszTypeName, int nWidthNumber, int nHeightNumber);
	////��������
	//bool LoadImage(CD3DDevice * pD3DDevice, LPCTSTR pszTextureName, BOOL bCreateCache, int nWidthNumber, int nHeightNumber);

	//�滭����
public:
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nIndex);
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nIndex);
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXSource, int nYSource, int nSourceWidth, int nSourceHeight);

	//�滭����
public:
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, int nXDest, int nYDest, BYTE cbAlpha, int nIndex);
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nDestWidth, int nDestHeight, BYTE cbAlpha, int nIndex);
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXSource, int nYSource, int nSourceWidth, int nSourceHeight, BYTE cbAlpha);

	//�滭����
public:
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nIndex);
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nIndex);
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXSource, int nYSource, int nSourceWidth, int nSourceHeight);


	//�滭����
public:
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, BYTE cbAlpha, int nIndex);
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nDestWidth, int nDestHeight, BYTE cbAlpha, int nIndex);
	//�滭ͼƬ
	bool DrawImage(CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXSource, int nYSource, int nSourceWidth, int nSourceHeight, BYTE cbAlpha);


//	//-------------------------------------------------------------------------------------
//	//��ɫ����
//public:
//	//�滭��ɫͼƬ
//	bool DrawColourFilterImage(CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nIndex);
//	//�滭��ɫͼƬ
//	bool DrawColourFilterImage(CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nIndex);
//	//�滭��ɫͼƬ
//	bool DrawColourFilterImage(CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXSource, int nYSource, int nSourceWidth, int nSourceHeight);
//
//	//�滭����
//public:
//	//�滭��ɫͼƬ
//	bool DrawColourFilterImage(CD3DDevice * pD3DDevice, int nXDest, int nYDest, BYTE cbAlpha, int nIndex);
//	//�滭��ɫͼƬ
//	bool DrawColourFilterImage(CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nDestWidth, int nDestHeight, BYTE cbAlpha, int nIndex);
//	//�滭��ɫͼƬ
//	bool DrawColourFilterImage(CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXSource, int nYSource, int nSourceWidth, int nSourceHeight, BYTE cbAlpha);
//
//	//�滭����
//public:
//	//�滭��ɫͼƬ
//	bool DrawColourFilterImage(CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nIndex);
//	//�滭��ɫͼƬ
//	bool DrawColourFilterImage(CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nIndex);
//	//�滭��ɫͼƬ
//	bool DrawColourFilterImage(CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXSource, int nYSource, int nSourceWidth, int nSourceHeight);
//
//	//�滭����
//public:
//	//�滭��ɫͼƬ
//	bool DrawColourFilterImage(CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, BYTE cbAlpha, int nIndex);
//	//�滭��ɫͼƬ
//	bool DrawColourFilterImage(CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nDestWidth, int nDestHeight, BYTE cbAlpha, int nIndex);
//	//�滭��ɫͼƬ
//	bool DrawColourFilterImage(CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXSource, int nYSource, int nSourceWidth, int nSourceHeight, BYTE cbAlpha);

};
