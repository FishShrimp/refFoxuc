#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//��Ϸ����
protected:
	WORD							m_wBankerUser;							//ׯ���û�
	bool							m_bMingZhu[GAME_PLAYER];
	BYTE							m_cbPlayStatus[GAME_PLAYER];			//��Ϸ���
	BYTE							m_cbPlayerCount;
	LONGLONG						m_lCurrentTimes;						//��ǰ����
	LONGLONG						m_lTableScore[GAME_PLAYER];				//�����ע
	LONGLONG						m_lMaxScore;							//�ⶥ��Ŀ
	LONGLONG						m_lMaxCellScore;						//��Ԫ����
	LONGLONG						m_lCellScore;							//��Ԫ��ע
	LONGLONG						m_lUserMaxScore;						//������

	//�����˿�
protected:
	BYTE							m_cbHandCardData[MAX_COUNT];		//�����˿�
	BYTE							m_cbAllUserCardData[GAME_PLAYER][MAX_COUNT];//�����˿�
	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	IAndroidUserItem *				m_pIAndroidUserItem;				//�û��ӿ�

	//��������
public:
	//���캯��
	CAndroidUserItemSink();
	//��������
	virtual ~CAndroidUserItemSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//��ʼ�ӿ�
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool RepositionSink();

	//��Ϸ�¼�
public:
	//ʱ����Ϣ
	virtual bool OnEventTimer(UINT nTimerID);
	//��Ϸ��Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize);

	//�û��¼�
public:
	//�û�����
	virtual VOID OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û��뿪
	virtual VOID OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�����
	virtual VOID OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�״̬
	virtual VOID OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û���λ
	virtual VOID OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//��Ϣ����
protected:
	//�����¼�
	bool OnUserGiveUp(WORD wChairID,bool bExit=false);
	//�����¼�
	bool OnUserLookCard(WORD wChairID);
	//�����¼�
	bool OnUserCompareCard(WORD wFirstChairID,WORD wNextChairID);
	//�����¼�
	bool OnUserOpenCard(WORD wUserID);
	//��ע�¼�
	bool OnUserAddScore(WORD wChairID, LONGLONG lScore, bool bGiveUp, bool bCompareUser);
	
	//������Ϣ
	bool OnSubGameStart(VOID * pData, WORD wDataSize);
	bool OnSubAddScore(VOID * pData, WORD wDataSize);
	bool OnSubCompareCard(VOID * pData, WORD wDataSize);
	bool OnSubLookCard(VOID * pData, WORD wDataSize);

	bool OnSubGiveUp(VOID * pData, WORD wDataSize);
	bool OnSubPlayerExit(VOID * pData, WORD wDataSize);
	bool OnSubGameEnd(VOID * pData,WORD wDataSize);
	void UpdataControl(WORD ChairID);

};

//////////////////////////////////////////////////////////////////////////

#endif