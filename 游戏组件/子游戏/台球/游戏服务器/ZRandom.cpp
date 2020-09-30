// ZRandom.cpp
// �����
#include "Stdafx.h"
#include "ZRandom.h"

DWORD ZRandom::MCoef_32[2]	=	{0xE7BD2160,0xDA3A2A9C};	// ����Сm����ϵ��

// dwMset: �����������ʹ�õ�Сm����
// dwKey: ���������
// nNumber: ������������г���(��DWORDΪ��λ)
// pdwRandom: ָ��������洢�ռ��ָ��
// return value: �������֮���������������״̬
DWORD ZRandom::Random(DWORD dwMset,DWORD dwKey,int nNumber,DWORD *pdwRandom)
{
	__asm
	{
		CLD
		MOV EDI,pdwRandom;
		MOV ECX,nNumber;
		MOV ESI,dwMset;
		MOV EAX,dwKey;

		ALIGN 4
LOOP_CIRCLE1:
		PUSH ECX;
		MOV ECX,32;
LOOP_CIRCLE2:
		MOV EBX,EAX;
		SHL EAX,1;
		AND EBX,ESI; //select the bit for xor
		MOV EDX,EBX;
		BSWAP EBX;
		XOR BX,DX;
		XOR BH,BL; // because P only judge one byte
		// so must XOR to judge the p of whole word
		JP NEXT; //jp equals the xor

		INC EAX;
NEXT: 
		DEC ecx;
		JNZ LOOP_CIRCLE2;

		POP ECX;
		STOSD;
		DEC ECX;
		JNZ LOOP_CIRCLE1;
		MOV dwKey,EAX;
	}
	return dwKey;
}

// ��������ΪnLength�ֽڵ������
// Buffer: ָ���������������ָ��
// nLength: �������������
void ZRandom::RandomGenerator(void *pBuffer,int nLength)
{
	DWORD A;
	BYTE B[4];

	__asm
	{
		_emit 0x0f;
		_emit 0x31;
		MOV A,EAX;
	}

	A = Random(MCoef_32[0],A,nLength/4,(DWORD *)pBuffer);

	if(nLength%4 != 0)
	{
		Random(MCoef_32[0],A,1,(DWORD *)B);
		memcpy((DWORD *)pBuffer+(nLength/4)*4,B,nLength%4);
	}
}

DWORD ZRandom::Random(DWORD max)
{
	DWORD a;
	DWORD ret;

	_asm
	{
		_emit 0x0f;
		_emit 0x31;
		mov a,EAX;
	}

	Random(MCoef_32[0],a,1,&ret);

	return ret%max;
}


////////////////////////////////////////////////////////////////////////
//	ZRandom_List
ZRandom_List::ZRandom_List() 
{ 
	m_pBuff = NULL; 
	m_dwSize = 0;
}

HRESULT ZRandom_List::Init(DWORD size)
{
	//ZASSERT(size > 0);

	Release();

	m_pBuff = new DWORD[size];
	if(m_pBuff == NULL)
		return E_FAIL;

	m_dwSize = size;

	return S_OK;
}

void ZRandom_List::Release()
{
	if(m_pBuff)
	{
		delete[] m_pBuff;

		m_pBuff = NULL;
	}

	m_dwSize = 0;
}

void ZRandom_List::Random(DWORD nums,DWORD max)
{
	//ZASSERT(nums <= max && max <= m_dwSize);

	DWORD i,j,temp;

	for(i = 0; i < max; i++)
		m_pBuff[i] = i;

	for(i = 0; i < nums; i++)
	{
		j = ZRandom::Random(max-i) + i;

		temp = m_pBuff[i];
		m_pBuff[i] = m_pBuff[j];
		m_pBuff[j] = temp;
	}
}