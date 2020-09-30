#ifndef GAME_CLIENT_ENGINE_HEAD_FILE
#define GAME_CLIENT_ENGINE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CGameClientEngine : public CGameFrameEngine
{
	//��Ϸ����
protected:
	BOOL							m_lHasInChip;						//�Ѿ�������
	LONG							m_lCellScore;						//�׷�
	LONGLONG						m_lPlayScore;						//��һ���
	LONGLONG						m_lPlayChip;						//��ҳ���
	int								m_nTurnTableRecord;					//��Ϸ��¼

	//��ע����
protected:
	int								m_nCurrentNote;						//��ǰע
	int								m_nUnitNote;						//��λע
	LONGLONG						m_lPlayBet[ANIMAL_MAX];				//�����ע
	LONGLONG						m_lPlayAllBet[ANIMAL_MAX];			//���������ע
	LONGLONG						m_lPlayLastBet[ANIMAL_MAX];			//�ϴ�ѹס
	LONGLONG                        m_hPlayAllBet[ANIMAL_MAX];           //��Ա�����ע
	LONGLONG						m_lPlayWin;							//���Ӯ��Ǯ

	//���ñ���
protected:
	LONGLONG						m_lAreaLimitScore;					//��������
	LONGLONG						m_lPlayLimitScore;					//�������

	//�������
protected:
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

	//��Ϸ�¼�
public:
	//�Թ���Ϣ
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//�����¼�
	virtual bool OnGameOptionChange();
	//��������
	virtual bool AllowBackGroundSound(bool bAllowSound);

	//��Ϣ����
protected:
	//����ʱ��
	bool OnSubGameFree(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//�����ע
	bool OnSubPlayBet(const void * pBuffer, WORD wDataSize);
	//��עʧ��
	bool OnSubPlayBetFail(const void * pBuffer, WORD wDataSize);
	//�����ע
	bool OnSubBetClear(const void * pBuffer, WORD wDataSize);
	//����
	bool OnSubAdminControl(const void * pBuffer, WORD wDataSize);

	//ϵͳ��Ϣ
public:
	//��ʱ��
	afx_msg void OnTimer(UINT nIDEvent);

	//��Ϣӳ��
protected:
	//���³���
	LRESULT OnMessageUpDateChip(WPARAM wParam, LPARAM lParam);
	//�һ�������Ϣ
	LRESULT OnMessageChip(WPARAM wParam, LPARAM lParam);
	//��ע��Ϣ
	LRESULT OnMessagePlayBet(WPARAM wParam, LPARAM lParam);
	//��ѹ��Ϣ
	LRESULT OnMessageOperationRenewal(WPARAM wParam, LPARAM lParam);
	//ȡ����Ϣ
	LRESULT OnMessageOperationCancel(WPARAM wParam, LPARAM lParam);
	//�л���Ϣ
	LRESULT OnMessageOperationSwitch(WPARAM wParam, LPARAM lParam);
	//����
	LRESULT  OnAdminControl(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////////////

#endif