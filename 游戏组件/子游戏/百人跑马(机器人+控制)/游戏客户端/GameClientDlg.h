#pragma once

#include "Stdafx.h"
#include "GameClientView.h"
#include "DirectSound.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CGameClientEngine : public CGameFrameEngine
{
	//��Ԫ����
	friend class CGameClientView;


	//��Ϸ����
protected:
	INT								m_nMultiple[AREA_ALL];				//������

	//���Ʊ���
protected:
	LONGLONG						m_lAreaLimitScore;					//����������
	LONGLONG						m_lUserLimitScore;					//������������

	//����
protected:
	LONGLONG						m_lPlayerBet[AREA_ALL];				//�����ע
	LONGLONG						m_lPlayerBetAll[AREA_ALL];			//������ע

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	//��������
public:
	//���캯��
	CGameClientEngine();
	//��������
	virtual ~CGameClientEngine();

	//ϵͳ�¼�
public:
	//��������
	virtual bool OnInitGameEngine();
	//���ú���
	virtual bool OnResetGameEngine();

	//ʱ���¼�
public:
	//ʱ��ɾ��
	virtual bool OnEventGameClockKill(WORD wChairID);
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);
	//��������
	virtual bool AllowBackGroundSound(bool bAllowSound);

	//��Ϸ�¼�
public:
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);

	//�ڲ�����
private:
	//���¿ؼ�
	void UpdateControls();
	//�Լ�����·�
	LONGLONG GetMeMaxBet( BYTE cbArea = AREA_ALL );

	//��Ϣӳ��
protected:
	//��ע��Ϣ
	LRESULT OnPlayerBet(WPARAM wParam, LPARAM lParam);
	//����
	LRESULT OnAdminControl(WPARAM wParam, LPARAM lParam);

	//��Ϣӳ��
public:
	//��ע��ʼ
	bool OnSubBetStart(const void * pBuffer, WORD wDataSize);
	//��ע����
	bool OnSubBetEnd(const void * pBuffer, WORD wDataSize);
	//����ʼ
	bool OnSubHorsesStart(const void * pBuffer, WORD wDataSize);
	//�������
	bool OnSubHorsesEnd(const void * pBuffer, WORD wDataSize);
	//�����ע
	bool OnSubPlayerBet(const void * pBuffer, WORD wDataSize);
	//��עʧ��
	bool OnSubPlayerBetFail(const void * pBuffer, WORD wDataSize);
	//ǿ�ƽ���
	bool OnSubMandatoryEnd(const void * pBuffer, WORD wDataSize);
	//����
	bool OnSubAdminControl(const void * pBuffer, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
