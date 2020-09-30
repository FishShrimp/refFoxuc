#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientEngine : public CGameFrameEngine
{
	//�û�����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�

	//��������
protected:
	WORD							m_wHeapHand;						//����ͷ��
	WORD							m_wHeapTail;						//����β��
	BYTE							m_cbHeapCardInfo[4][2];				//������Ϣ

	//������ر���
	WORD							m_wKingHeapPos;						//���ƶ�λ
	WORD							m_wKingPos;							//����λ��
	BYTE							m_cbKingCardIndex[2];				//��������

	//�йܱ���
	bool							m_bStustee;							//�йܱ�־
	WORD							m_wTimeOutCount;					//��ʱ����

	//�˿˱���
protected:
	BYTE							m_cbLeftCardCount;					//ʣ����Ŀ
	BYTE							m_cbCardIndex[MAX_INDEX];			//�����˿�
	BYTE							m_cbCardData[MAX_COUNT];			//�Լ����е��˿�

	//����˿�
protected:
	BYTE							m_cbWeaveCount[GAME_PLAYER];		//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][4];	//����˿�

	//������Ϣ
protected:
	WORD							m_wOutCardUser;						//�����û�
	BYTE							m_cbOutCardData;					//�����˿�
	BYTE							m_cbDiscardCount[GAME_PLAYER];		//������Ŀ
	BYTE							m_cbDiscardCard[GAME_PLAYER][55];	//������¼
	bool                            m_bStart;                           //ɸ�Ӷ����ǲ��ǽ���
	WORD							m_wUserAction;						//��Ӧ����

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
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID);

	//�Թ�״̬
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pBuffer, WORD wDataSize);
	//��Ϸ����
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pBuffer, WORD wDataSize);
	//��������
	virtual bool AllowBackGroundSound(bool bAllowSound){return true;}

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(VOID * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(VOID * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard(VOID * pBuffer, WORD wDataSize);
	//������ʾ
	bool OnSubOperateNotify(VOID * pBuffer, WORD wDataSize);
	//�������
	bool OnSubOperateResult(VOID * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(VOID * pBuffer, WORD wDataSize);
	//��Ϸϴ��
	bool OnSubGameChip(VOID *pBuffer,WORD wDataSize);
	//�û��й�
	bool OnSubTrustee(VOID * pBuffer,WORD wDataSize);
	//ϴ����
	bool OnSubBatchResult(VOID *pBuffer,WORD wDataSize);

	//��������
protected:
	//������Ϣ
	void SetHandCardData(BYTE cbCardData[], WORD wCardCount, BYTE cbCurrentCard);
	//����֮��ǰ��
	void SetHandCurrentCard(BYTE cbCurrentCard);
	//��������
	void PlayCardSound(WORD wChairID,BYTE cbCardData);
	//��������
	void PlayActionSound(WORD wChairID,BYTE cbAction);
	//�۳��˿�
	void DeductionTableCard(bool bHeadCard);
	//��ʾ����
	bool ShowOperateControl(WORD wUserAction, BYTE cbActionCard);
	//����У��
	bool VerdictOutCard(BYTE cbPreCardData);
	//��Ϣӳ��
protected:
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//���Ʋ���
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//�˿˲���
	LRESULT OnCardOperate(WPARAM wParam, LPARAM lParam);
	//ɸ�Ӷ������
	LRESULT OnGameStart(WPARAM wParam, LPARAM lParam);
	//ɸ�Ӷ������
	LRESULT OnSiceTwo(WPARAM wParam, LPARAM lParam);
	//��׷���
	LRESULT OnBatch(WPARAM wParam,LPARAM lParam);
	//�Ϲܿ���
	LRESULT OnStusteeControl(WPARAM wParam, LPARAM lParam);
	//ϴ�����
	LRESULT OnBatchEnd(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
