#pragma once

#include "OpenGL.h"
#include "OpenglEngineHead.h"

//��Դ��Ϣ
struct tagGLResourceInfo
{
	DWORD		dwSize;								//��Դ��С
	LPVOID		pcbBuffer;							//��Դ�ڴ�
};


//GL TGA������
class OPENGL_ENGINE_CLASS CGLTexture
{
	//ͼƬ��Ϣ
protected:
	GLuint							m_nWidth;										
	GLuint							m_nHeight;										
	GLuint							m_nTextureID;											

	//��ת��Ϣ
protected:
	FLOAT							m_fAngle;									//��ת����
	FLOAT							m_fRotationX;								//��ת����
	FLOAT							m_fRotationY;								//��ת����

	//�Գ���Ϣ
protected:
	BOOL							m_bSymmetrical;								//�Գ���Ϣ

	//��������
private:
	GLboolean						m_bLoadSuccess;								// �����Ƿ�ɹ�

public:
	CGLTexture(void);
	~CGLTexture(void);

	//ͼƬ��Ϣ
public:
	//�����ж�
	GLboolean IsNull();
	//������
	INT GetWidth() { return m_nWidth; }
	//����߶�
	INT GetHeight() { return m_nHeight; }
	//����ID
	INT GetTextureID() { return m_nTextureID; }

	//�������
public:
	//��������
	GLboolean Destory();

	//���ƺ���
public:
	//������ת
	GLvoid SetRotation(FLOAT fRotationX, FLOAT fRotationY, FLOAT fAngle);
	//���öԳ�
	GLvoid SetSymmetrical(BOOL bSymmetrical);

	//���ܺ���
public:
	//��������
	GLboolean LoadImage( CGLDevice* pOpenGL, HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszTypeName, DWORD wImageType);

	//�滭����
public:
	//�滭ͼƬ
	GLboolean DrawImage(CGLDevice* pOpenGL, INT nXDest, INT nYDest);
	//�滭ͼƬ
	GLboolean DrawImage(CGLDevice* pOpenGL, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource);
	//�滭ͼƬ
	GLboolean DrawImage(CGLDevice* pOpenGL, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, INT nSourceWidth, INT nSourceHeight);

public:
	//�滭ͼƬ
	GLboolean DrawImage(CGLDevice* pOpenGL, INT nXDest, INT nYDest, COLORREF GLColor);
	//�滭ͼƬ
	GLboolean DrawImage(CGLDevice* pOpenGL, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, COLORREF GLColor);
	//�滭ͼƬ
	GLboolean DrawImage(CGLDevice* pOpenGL, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, INT nSourceWidth, INT nSourceHeight, COLORREF GLColor);


	//�滭����
public:
	//�滭ͼƬ
	GLboolean DrawImage(CGLDevice* pOpenGL, INT nXDest, INT nYDest, BYTE cbAlpha);
	//�滭ͼƬ
	GLboolean DrawImage(CGLDevice* pOpenGL, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, BYTE cbAlpha);
	//�滭ͼƬ
	GLboolean DrawImage(CGLDevice* pOpenGL, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, INT nSourceWidth, INT nSourceHeight, BYTE cbAlpha);

	//�滭����
public:
	//�滭ͼƬ
	GLboolean DrawColourFilterImage(CGLDevice* pOpenGL, INT nXDest, INT nYDest);
	//�滭ͼƬ
	GLboolean DrawColourFilterImage(CGLDevice* pOpenGL, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource);
	//�滭ͼƬ
	GLboolean DrawColourFilterImage(CGLDevice* pOpenGL, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, INT nSourceWidth, INT nSourceHeight);

	//�滭����
public:
	//�滭ͼƬ
	GLboolean DrawColourFilterImage(CGLDevice* pOpenGL, INT nXDest, INT nYDest, BYTE cbAlpha);
	//�滭ͼƬ
	GLboolean DrawColourFilterImage(CGLDevice* pOpenGL, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, BYTE cbAlpha);
	//�滭ͼƬ
	GLboolean DrawColourFilterImage(CGLDevice* pOpenGL, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, INT nSourceWidth, INT nSourceHeight, BYTE cbAlpha);


	//�ڲ�����
protected:
	//������ͼ
	GLvoid	  TextureMap(GLfloat fImageBeginX, GLfloat fImageBeginY, GLfloat fImageEndX, GLfloat fImageEndY, INT nDestWidth, INT nDestHeight);
	//��ȡ��Դ
	GLboolean GetResourceInfo(HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszTypeName, tagGLResourceInfo & ResourceInfo);
	

};



