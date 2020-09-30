/*
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hgeSprite helper class header Ϊ����ʵ��������HGE������
*/


#ifndef HGESPRITE_H
#define HGESPRITE_H


#include "hge.h"
#include "hgerect.h"


/*
** HGE Sprite class
*/
class hgeSprite
{
public:
	hgeSprite(HTEXTURE tex, float x, float y, float w, float h);
	hgeSprite(const hgeSprite &spr);		// �������캯��
	~hgeSprite() { hge->Release(); }
	
	// ��澫�鵽��Ļ��
	void		Render(float x, float y);

	// �����ű�������ת��澫��; 
	void		RenderEx(float x, float y, float rot, float hscale=1.0f, float vscale=0.0f);

	// ����쳤����
	void		RenderStretch(float x1, float y1, float x2, float y2);

	// ��澫�鵽��Ļ�ϵ������ı���
	void		Render4V(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

	// ���ò���Ϊ��������
	void		SetTexture(HTEXTURE tex);

	// ���ò��ʵ��ض�����Ϊ��������
	void		SetTextureRect(float x, float y, float w, float h, bool adjSize = true);

	// Ϊָ���Ķ����������������ɫ�ʺ�͸����
	void		SetColor(DWORD col, int i=-1);

	// Ϊָ���Ķ�������������趨z��
	void		SetZ(float z, int i=-1);

	// ���þ���Ļ��ģʽ
	void		SetBlendMode(int blend) { quad.blend=blend; }

	// �趨�����ê��(���ţ���ת��λ�Ʋ��������ĵ�,�����ͼƬ���Ͻǵ�λ������)
	void		SetHotSpot(float x, float y) { hotX=x; hotY=y; }

	// ˮƽ��ֱ��ת����; ��bHotSpot=true;��ת���뾫���ê���������,����ֻ��ͼ�񱻷�ת��ê�㲻��.
	void		SetFlip(bool bX, bool bY, bool bHotSpot = false);

	// ���ص�ǰ����Ĳ���
	HTEXTURE	GetTexture() const { return quad.tex; }

	// ���ر���ǰ�������õ��ض���������
	void		GetTextureRect(float *x, float *y, float *w, float *h) const { *x=tx; *y=ty; *w=width; *h=height; }

	// ����ָ�����鶥�����ɫ
	DWORD		GetColor(int i=0) const { return quad.v[i].col; }

	// ����ָ�����鶥���z������
	float		GetZ(int i=0) const { return quad.v[i].z; }

	// ���ص�ǰ����Ļ��ģʽ
	int			GetBlendMode() const { return quad.blend; }

	// ���ؾ����ê��(ê�������ţ���ת��λ�Ʋ��������ĵ�)
	void		GetHotSpot(float *x, float *y) const { *x=hotX; *y=hotY; }

	// ���ص�ǰ����ķ�ת����
	void		GetFlip(bool *bX, bool *bY) const { *bX=bXFlip; *bY=bYFlip; }

	// ���ؾ���Ŀ��
	float		GetWidth() const { return width; }

	// ���ؾ���ĸ߶�
	float		GetHeight() const { return height; }

	// ���ؾ���İ�Χ�з�Χ
	hgeRect*	GetBoundingBox(float x, float y, hgeRect *rect) const { rect->Set(x-hotX, y-hotY, x-hotX+width, y-hotY+height); return rect; }

	// �������ź���ת����İ�Χ�з�Χ
	hgeRect*	GetBoundingBoxEx(float x, float y, float rot, float hscale, float vscale,  hgeRect *rect) const;

protected:
	hgeSprite();
	static HGE	*hge;		// ȫ��Ψһʵ��, ��HgeImpl::HgeCreate(version)����

	hgeQuad		quad;		// v[0]��ʾ���Ͻ����꣬v[1]��ʾ���Ͻ����꣬v[2]��ʾ���½����꣬v[3]��ʾ���½�����; ��������: 012023 ˳ʱ�����
	float		tx, ty, width, height;		// �������ʼ��ȡλ�ü����
	float		tex_width, tex_height;		// ��������Ŀ��
	float		hotX, hotY;					// �����������ĵ�, ���뾫�����Ͻǵ����λ��
	bool		bXFlip, bYFlip, bHSFlip;	// �Ƿ�ˮƽ����ֱ�������ĵ���з�ת
};


#endif
