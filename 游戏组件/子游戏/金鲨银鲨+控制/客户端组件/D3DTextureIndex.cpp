#include "StdAfx.h"
#include ".\d3dtextureindex.h"

CD3DTextureIndex::CD3DTextureIndex(void)
{
	m_nWidthNumber = 0;
	m_nHeightNumber = 0;
}

CD3DTextureIndex::~CD3DTextureIndex(void)
{

}

//�����ж�
bool CD3DTextureIndex::IsNull()
{
	return CD3DTexture::IsNull();
}

//��������
bool CD3DTextureIndex::LoadImage( CD3DDevice * pD3DDevice, HINSTANCE hInstance, LPCTSTR pszResource, DWORD dwColorKey, int nWidthNumber, int nHeightNumber )
{
	//�����ж�
	ASSERT(nWidthNumber > 0 && nHeightNumber>0);

	//���ñ���
	m_nWidthNumber = nWidthNumber;
	m_nHeightNumber = nHeightNumber;

	//����ͼƬ
	return CD3DTexture::LoadImage( pD3DDevice, hInstance, pszResource, dwColorKey );
}

//��������
bool CD3DTextureIndex::LoadImage( CD3DDevice * pD3DDevice, HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszTypeName, int nWidthNumber, int nHeightNumber )
{
	//�����ж�
	ASSERT(nWidthNumber > 0 && nHeightNumber>0);

	//���ñ���
	m_nWidthNumber = nWidthNumber;
	m_nHeightNumber = nHeightNumber;

	//����ͼƬ
	return CD3DTexture::LoadImage( pD3DDevice, hInstance, pszResource, pszTypeName );
}

////��������
//bool CD3DTextureIndex::LoadImage( CD3DDevice * pD3DDevice, LPCTSTR pszTextureName, BOOL bCreateCache, int nWidthNumber, int nHeightNumber )
//{
//	//�����ж�
//	ASSERT(nWidthNumber > 0 && nHeightNumber>0);
//
//	//���ñ���
//	m_nWidthNumber = nWidthNumber;
//	m_nHeightNumber = nHeightNumber;
//
//	//����ͼƬ
//	return CD3DTexture::LoadImage( pD3DDevice, pszTextureName, bCreateCache );
//}

//�滭ͼƬ
bool CD3DTextureIndex::DrawImage( CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nIndex )
{
	//�������� ��ȫͼ
	if ( nIndex >= m_nWidthNumber * m_nHeightNumber )
		return CD3DTexture::DrawImage( pD3DDevice, nXDest, nYDest );

	//���ñ���
	int nXSource = (nIndex%m_nWidthNumber) * GetWidth();
	int nYSource = (nIndex/m_nWidthNumber) * GetHeight();
	int nSourceWidth = GetWidth();
	int nSourceHeight = GetHeight();

	//��ͼ
	return CD3DTexture::DrawImage( pD3DDevice, nXDest, nYDest, nSourceWidth, nSourceHeight, nXSource, nYSource );
}

//�滭ͼƬ
bool CD3DTextureIndex::DrawImage(CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nIndex)
{
	//�������� ��ȫͼ
	if ( nIndex >= m_nWidthNumber * m_nHeightNumber )
		return CD3DTexture::DrawImage( pD3DDevice, nXDest, nYDest );

	//���ñ���
	int nXSource = (nIndex%m_nWidthNumber) * GetWidth();
	int nYSource = (nIndex/m_nWidthNumber) * GetHeight();
	int nSourceWidth = GetWidth();
	int nSourceHeight = GetHeight();

	//��ͼ
	return CD3DTexture::DrawImage( pD3DDevice, nXDest, nYDest, nDestWidth, nDestHeight, nXSource, nYSource, nSourceWidth, nSourceHeight );
}

//�滭ͼƬ
bool CD3DTextureIndex::DrawImage( CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXSource, int nYSource, int nSourceWidth, int nSourceHeight )
{
	//��ͼ
	return CD3DTexture::DrawImage( pD3DDevice, nXDest, nYDest, nDestWidth, nDestHeight, nXSource, nYSource, nSourceWidth, nSourceHeight );
}

//�滭ͼƬ
bool CD3DTextureIndex::DrawImage( CD3DDevice * pD3DDevice, int nXDest, int nYDest, BYTE cbAlpha, int nIndex )
{
	//�������� ��ȫͼ
	if ( nIndex >= m_nWidthNumber * m_nHeightNumber )
		return CD3DTexture::DrawImage( pD3DDevice, nXDest, nYDest, cbAlpha );

	//���ñ���
	int nXSource = (nIndex%m_nWidthNumber) * GetWidth();
	int nYSource = (nIndex/m_nWidthNumber) * GetHeight();
	int nSourceWidth = GetWidth();
	int nSourceHeight = GetHeight();

	//��ͼ
	return CD3DTexture::DrawImage( pD3DDevice, nXDest, nYDest, nSourceWidth, nSourceHeight, nXSource, nYSource, cbAlpha );
}

//�滭ͼƬ
bool CD3DTextureIndex::DrawImage(CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nDestWidth, int nDestHeight, BYTE cbAlpha, int nIndex)
{
	//�������� ��ȫͼ
	if ( nIndex >= m_nWidthNumber * m_nHeightNumber )
		return CD3DTexture::DrawImage( pD3DDevice, nXDest, nYDest, cbAlpha );

	//���ñ���
	int nXSource = (nIndex%m_nWidthNumber) * GetWidth();
	int nYSource = (nIndex/m_nWidthNumber) * GetHeight();
	int nSourceWidth = GetWidth();
	int nSourceHeight = GetHeight();

	//��ͼ
	return CD3DTexture::DrawImage( pD3DDevice, nXDest, nYDest, nDestWidth, nDestHeight, nXSource, nYSource, nSourceWidth, nSourceHeight, cbAlpha );
}

//�滭ͼƬ
bool CD3DTextureIndex::DrawImage( CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXSource, int nYSource, int nSourceWidth, int nSourceHeight, BYTE cbAlpha )
{
	//��ͼ
	return CD3DTexture::DrawImage( pD3DDevice, nXDest, nYDest, nDestWidth, nDestHeight, nXSource, nYSource, nSourceWidth, nSourceHeight, cbAlpha );
}

//�滭ͼƬ
bool CD3DTextureIndex::DrawImage( CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nIndex )
{
	//�������� ��ȫͼ
	if ( nIndex >= m_nWidthNumber * m_nHeightNumber )
		return CD3DTexture::DrawImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest );

	//���ñ���
	int nXSource = (nIndex%m_nWidthNumber) * GetWidth();
	int nYSource = (nIndex/m_nWidthNumber) * GetHeight();
	int nSourceWidth = GetWidth();
	int nSourceHeight = GetHeight();

	//��ͼ
	return CD3DTexture::DrawImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest, nSourceWidth, nSourceHeight, nXSource, nYSource );
}

//�滭ͼƬ
bool CD3DTextureIndex::DrawImage(CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nIndex)
{
	//�������� ��ȫͼ
	if ( nIndex >= m_nWidthNumber * m_nHeightNumber )
		return CD3DTexture::DrawImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest );

	//���ñ���
	int nXSource = (nIndex%m_nWidthNumber) * GetWidth();
	int nYSource = (nIndex/m_nWidthNumber) * GetHeight();
	int nSourceWidth = GetWidth();
	int nSourceHeight = GetHeight();

	//��ͼ
	return CD3DTexture::DrawImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest, nDestWidth, nDestHeight, nXSource, nYSource, nSourceWidth, nSourceHeight );
}

//�滭ͼƬ
bool CD3DTextureIndex::DrawImage( CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXSource, int nYSource, int nSourceWidth, int nSourceHeight )
{
	//��ͼ
	return CD3DTexture::DrawImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest, nDestWidth, nDestHeight, nXSource, nYSource, nSourceWidth, nSourceHeight );
}

//�滭ͼƬ
bool CD3DTextureIndex::DrawImage( CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, BYTE cbAlpha, int nIndex )
{
	//�������� ��ȫͼ
	if ( nIndex >= m_nWidthNumber * m_nHeightNumber )
		return CD3DTexture::DrawImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest, cbAlpha );

	//���ñ���
	int nXSource = (nIndex%m_nWidthNumber) * GetWidth();
	int nYSource = (nIndex/m_nWidthNumber) * GetHeight();
	int nSourceWidth = GetWidth();
	int nSourceHeight = GetHeight();

	//��ͼ
	return CD3DTexture::DrawImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest, nSourceWidth, nSourceHeight, nXSource, nYSource, cbAlpha );

}

//�滭ͼƬ
bool CD3DTextureIndex::DrawImage(CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nDestWidth, int nDestHeight, BYTE cbAlpha, int nIndex)
{
	//�������� ��ȫͼ
	if ( nIndex >= m_nWidthNumber * m_nHeightNumber )
		return CD3DTexture::DrawImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest, cbAlpha );

	//���ñ���
	int nXSource = (nIndex%m_nWidthNumber) * GetWidth();
	int nYSource = (nIndex/m_nWidthNumber) * GetHeight();
	int nSourceWidth = GetWidth();
	int nSourceHeight = GetHeight();

	//��ͼ
	return CD3DTexture::DrawImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest, nDestWidth, nDestHeight, nXSource, nYSource, nSourceWidth, nSourceHeight, cbAlpha );

}

//�滭ͼƬ
bool CD3DTextureIndex::DrawImage( CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXSource, int nYSource, int nSourceWidth, int nSourceHeight, BYTE cbAlpha )
{
	//��ͼ
	return CD3DTexture::DrawImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest, nDestWidth, nDestHeight, nXSource, nYSource, nSourceWidth, nSourceHeight, cbAlpha );
}

////-------------------------------------------------------------------------------------
////��ɫ����
////
////�滭��ɫͼƬ
//bool CD3DTextureIndex::DrawColourFilterImage( CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nIndex )
//{
//	//�������� ��ȫͼ
//	if ( nIndex >= m_nWidthNumber * m_nHeightNumber )
//		return CD3DTexture::DrawColourFilterImage( pD3DDevice, nXDest, nYDest,nIndex );
//
//	//���ñ���
//	int nXSource = (nIndex%m_nWidthNumber) * GetWidth();
//	int nYSource = (nIndex/m_nWidthNumber) * GetHeight();
//	int nSourceWidth = GetWidth();
//	int nSourceHeight = GetHeight();
//
//	//��ͼ
//	return CD3DTexture::DrawColourFilterImage( pD3DDevice, nXDest, nYDest, nSourceWidth, nSourceHeight, nXSource, nYSource,nIndex );
//}
//
////�滭��ɫͼƬ
//bool CD3DTextureIndex::DrawColourFilterImage(CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nIndex)
//{
//	//�������� ��ȫͼ
//	if ( nIndex >= m_nWidthNumber * m_nHeightNumber )
//		return CD3DTexture::DrawColourFilterImage( pD3DDevice, nXDest, nYDest,nIndex );
//
//	//���ñ���
//	int nXSource = (nIndex%m_nWidthNumber) * GetWidth();
//	int nYSource = (nIndex/m_nWidthNumber) * GetHeight();
//	int nSourceWidth = GetWidth();
//	int nSourceHeight = GetHeight();
//
//	//��ͼ
//	return CD3DTexture::DrawColourFilterImage( pD3DDevice, nXDest, nYDest, nDestWidth, nDestHeight, nXSource, nYSource, nSourceWidth, nSourceHeight );
//}
//
////�滭��ɫͼƬ
//bool CD3DTextureIndex::DrawColourFilterImage( CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXSource, int nYSource, int nSourceWidth, int nSourceHeight )
//{
//	//��ͼ
//	return CD3DTexture::DrawColourFilterImage( pD3DDevice, nXDest, nYDest, nDestWidth, nDestHeight, nXSource, nYSource, nSourceWidth, nSourceHeight );
//}
//
////�滭��ɫͼƬ
//bool CD3DTextureIndex::DrawColourFilterImage( CD3DDevice * pD3DDevice, int nXDest, int nYDest, BYTE cbAlpha, int nIndex )
//{
//	//�������� ��ȫͼ
//	if ( nIndex >= m_nWidthNumber * m_nHeightNumber )
//		return CD3DTexture::DrawColourFilterImage( pD3DDevice, nXDest, nYDest, cbAlpha );
//
//	//���ñ���
//	int nXSource = (nIndex%m_nWidthNumber) * GetWidth();
//	int nYSource = (nIndex/m_nWidthNumber) * GetHeight();
//	int nSourceWidth = GetWidth();
//	int nSourceHeight = GetHeight();
//
//	//��ͼ
//	return CD3DTexture::DrawColourFilterImage( pD3DDevice, nXDest, nYDest, nSourceWidth, nSourceHeight, nXSource, nYSource, cbAlpha );
//}
//
////�滭��ɫͼƬ
//bool CD3DTextureIndex::DrawColourFilterImage(CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nDestWidth, int nDestHeight, BYTE cbAlpha, int nIndex)
//{
//	//�������� ��ȫͼ
//	if ( nIndex >= m_nWidthNumber * m_nHeightNumber )
//		return CD3DTexture::DrawColourFilterImage( pD3DDevice, nXDest, nYDest, cbAlpha );
//
//	//���ñ���
//	int nXSource = (nIndex%m_nWidthNumber) * GetWidth();
//	int nYSource = (nIndex/m_nWidthNumber) * GetHeight();
//	int nSourceWidth = GetWidth();
//	int nSourceHeight = GetHeight();
//
//	//��ͼ
//	return CD3DTexture::DrawColourFilterImage( pD3DDevice, nXDest, nYDest, nDestWidth, nDestHeight, nXSource, nYSource, nSourceWidth, nSourceHeight, cbAlpha );
//
//}
//
////�滭��ɫͼƬ
//bool CD3DTextureIndex::DrawColourFilterImage( CD3DDevice * pD3DDevice, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXSource, int nYSource, int nSourceWidth, int nSourceHeight, BYTE cbAlpha )
//{
//	//��ͼ
//	return CD3DTexture::DrawColourFilterImage( pD3DDevice, nXDest, nYDest, nDestWidth, nDestHeight, nXSource, nYSource, nSourceWidth, nSourceHeight, cbAlpha );
//}
//
////�滭��ɫͼƬ
//bool CD3DTextureIndex::DrawColourFilterImage( CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nIndex )
//{
//	//�������� ��ȫͼ
//	if ( nIndex >= m_nWidthNumber * m_nHeightNumber )
//		return CD3DTexture::DrawColourFilterImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest,nIndex);
//
//	//���ñ���
//	int nXSource = (nIndex%m_nWidthNumber) * GetWidth();
//	int nYSource = (nIndex/m_nWidthNumber) * GetHeight();
//	int nSourceWidth = GetWidth();
//	int nSourceHeight = GetHeight();
//
//	//��ͼ
//	return CD3DTexture::DrawColourFilterImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest, nSourceWidth, nSourceHeight, nXSource, nYSource );
//
//}
//
////�滭��ɫͼƬ
//bool CD3DTextureIndex::DrawColourFilterImage(CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nIndex)
//{
//	//�������� ��ȫͼ
//	if ( nIndex >= m_nWidthNumber * m_nHeightNumber )
//		return CD3DTexture::DrawColourFilterImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest,nIndex);
//
//	//���ñ���
//	int nXSource = (nIndex%m_nWidthNumber) * GetWidth();
//	int nYSource = (nIndex/m_nWidthNumber) * GetHeight();
//	int nSourceWidth = GetWidth();
//	int nSourceHeight = GetHeight();
//
//	//��ͼ
//	return CD3DTexture::DrawColourFilterImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest, nDestWidth, nDestHeight, nXSource, nYSource, nSourceWidth, nSourceHeight );
//
//}
//
//
////�滭��ɫͼƬ
//bool CD3DTextureIndex::DrawColourFilterImage( CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXSource, int nYSource, int nSourceWidth, int nSourceHeight )
//{
//	//��ͼ
//	return CD3DTexture::DrawColourFilterImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest, nDestWidth, nDestHeight, nXSource, nYSource, nSourceWidth, nSourceHeight );
//}
//
////�滭��ɫͼƬ
//bool CD3DTextureIndex::DrawColourFilterImage( CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, BYTE cbAlpha, int nIndex )
//{
//	//�������� ��ȫͼ
//	if ( nIndex >= m_nWidthNumber * m_nHeightNumber )
//		return CD3DTexture::DrawColourFilterImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest, cbAlpha );
//
//	//���ñ���
//	int nXSource = (nIndex%m_nWidthNumber) * GetWidth();
//	int nYSource = (nIndex/m_nWidthNumber) * GetHeight();
//	int nSourceWidth = GetWidth();
//	int nSourceHeight = GetHeight();
//
//	//��ͼ
//	return CD3DTexture::DrawColourFilterImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest, nSourceWidth, nSourceHeight, nXSource, nYSource, cbAlpha,nIndex );
//
//}
//
////�滭��ɫͼƬ
//bool CD3DTextureIndex::DrawColourFilterImage(CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nDestWidth, int nDestHeight, BYTE cbAlpha, int nIndex)
//{
//	//�������� ��ȫͼ
//	if ( nIndex >= m_nWidthNumber * m_nHeightNumber )
//		return CD3DTexture::DrawColourFilterImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest, cbAlpha );
//
//	//���ñ���
//	int nXSource = (nIndex%m_nWidthNumber) * GetWidth();
//	int nYSource = (nIndex/m_nWidthNumber) * GetHeight();
//	int nSourceWidth = GetWidth();
//	int nSourceHeight = GetHeight();
//
//	//��ͼ
//	return CD3DTexture::DrawColourFilterImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest, nDestWidth, nDestHeight, nXSource, nYSource, nSourceWidth, nSourceHeight, cbAlpha );
//
//}
//
////�滭��ɫͼƬ
//bool CD3DTextureIndex::DrawColourFilterImage( CD3DDevice * pD3DDevice, CPoint ptRotationOffset, FLOAT fRadian, CHAR chDirection, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXSource, int nYSource, int nSourceWidth, int nSourceHeight, BYTE cbAlpha )
//{
//	//��ͼ
//	return CD3DTexture::DrawColourFilterImage( pD3DDevice, ptRotationOffset, fRadian, chDirection, nXDest, nYDest, nDestWidth, nDestHeight, nXSource, nYSource, nSourceWidth, nSourceHeight, cbAlpha );
//}