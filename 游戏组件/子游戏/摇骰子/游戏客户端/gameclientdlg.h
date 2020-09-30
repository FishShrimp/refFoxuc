#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientEngine : public CGameFrameEngine
{
	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	//���ñ���
protected:
	BYTE							m_bSetChipTime;						//��עʱ��
	BYTE							m_bThrowDiceTime;					//ҡ��ʱ��
	BYTE							m_bYellDiceTime;					//����ʱ��
	BYTE							m_bOpenInterval;					//����ʱ�� (�������������״̬֮��ļ��)

	//��Ϸ����
protected:
	BYTE							m_bDiceData[GAME_PLAYER][5];		//��������
	WORD							m_wTurnCount;						//��������	
	LONGLONG						m_lChip;							//��ע��С
	LONGLONG						m_lMaxChip;							//�����ע
	WORD							m_wCurUser;							//��ǰ���
	tagDiceYell						m_UserYell[GAME_PLAYER];			//��󺰻�
	UINT_PTR						m_nTimer;							//��ʱ����
	
	//��������
public:
	//���캯��
	CGameClientEngine();
	//��������
	virtual ~CGameClientEngine();

	//��Ϣ����
protected:
	//��ʼ��ע
	bool OnSubStartChip(const void * pBuffer, WORD wDataSize);
	//��ע���
	bool OnSubChipResult(const void * pBuffer, WORD wDataSize);
	//������ֵ
	bool OnSubSendDice(const void * pBuffer, WORD wDataSize);
	//���ҡ��
	bool OnSubThrowDice(const void * pBuffer, WORD wDataSize);
	//��Һ���
	bool OnSubYellDice(const void * pBuffer, WORD wDataSize);
	//�������
	bool OnSubYellResult(const void * pBuffer, WORD wDataSize);
	//��ҿ���
	bool OnSubOpenDice(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

    //��������
protected:
	//���ú���
	void SetYellPanel(BYTE bDiceCount);
	//��������
	void PlaySoundBySex(BYTE bDiceCount, BYTE bDicePoint, bool bBoy = true);

	//����̳�
private:
	//��ʼ����
	virtual bool OnInitGameEngine();
	//���ÿ��
	virtual bool OnResetGameEngine();
	//��Ϸ����
	virtual void OnGameOptionSet();

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

	//��������
	virtual bool AllowBackGroundSound(bool bAllowSound){return true;}

	//��Ϣ����
protected:
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//ѹע��Ϣ
	LRESULT OnSetChip(WPARAM wParam, LPARAM lParam);
	//ҡ����Ϣ
	LRESULT OnThrowDice(WPARAM wParam, LPARAM lParam);
	//ҡ������

	//������Ϣ
	LRESULT OnLookDice(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnScroll(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnDiceNum(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnYellOk(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnOpenDice(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
