#include "StdAfx.h"
#include "Tableframesink.h"
#include "GameServerManager.h"
//#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//ȫ�ֱ���
static CGameServiceManager			g_GameServiceManager;				//�������

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameServiceManager::CGameServiceManager(void)
{
	//�ں�����
	m_GameServiceAttrib.wKindID=KIND_ID;
	m_GameServiceAttrib.wChairCount=GAME_PLAYER;
	m_GameServiceAttrib.wSupporType=GAME_GENRE;

	//���ܱ�־
	m_GameServiceAttrib.cbDynamicJoin=TRUE;
	m_GameServiceAttrib.cbAndroidUser=FALSE;
	m_GameServiceAttrib.cbOffLineTrustee=FALSE;

	//��������
	m_GameServiceAttrib.dwServerVersion=VERSION_SERVER;
	m_GameServiceAttrib.dwClientVersion=VERSION_CLIENT;
	lstrcpyn(m_GameServiceAttrib.szGameName,GAME_NAME,CountArray(m_GameServiceAttrib.szGameName));

	lstrcpyn(m_GameServiceAttrib.szDataBaseName,szTreasureDB,CountArray(m_GameServiceAttrib.szDataBaseName));
	lstrcpyn(m_GameServiceAttrib.szClientEXEName,TEXT("HKFiveCardAN.exe"),CountArray(m_GameServiceAttrib.szClientEXEName));
	lstrcpyn(m_GameServiceAttrib.szServerDLLName,TEXT("HKFiveCardANServer.DLL"),CountArray(m_GameServiceAttrib.szServerDLLName));

	return;
}

//��������
CGameServiceManager::~CGameServiceManager(void)
{
}

//�ӿڲ�ѯ
VOID *  CGameServiceManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameServiceManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager,Guid,dwQueryVer);
	return NULL;
}

//������Ϸ��
void *  CGameServiceManager::CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer)
{
	//��������
	CTableFrameSink * pTableFrameSink=NULL;
	try
	{
		pTableFrameSink=new CTableFrameSink();
		if (pTableFrameSink==NULL) throw TEXT("����ʧ��");
		void * pObject=pTableFrameSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pTableFrameSink);

	return NULL;
}

//��ȡ����
bool  CGameServiceManager::GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)
{
	GameServiceAttrib=m_GameServiceAttrib;
	return true;
}

//�����޸�
bool  CGameServiceManager::RectifyParameter(tagGameServiceOption &GameServiceOption)
{
	ASSERT(&GameServiceOption);
	if(&GameServiceOption == NULL) return false;
	//���ֵ���
	GameServiceOption.lCellScore=__max(1L,GameServiceOption.lCellScore);

	//��������
	GameServiceOption.lMinTableScore=__max(GameServiceOption.lCellScore*10L,GameServiceOption.lMinTableScore);

	//��������
	if (GameServiceOption.lRestrictScore!=0L)
	{
		GameServiceOption.lRestrictScore=__max(GameServiceOption.lRestrictScore,GameServiceOption.lMinTableScore);
	}

	//��������
	if(GameServiceOption.lMaxEnterScore<=GameServiceOption.lMinTableScore)
		GameServiceOption.lMaxEnterScore=0L;
	else if(GameServiceOption.lRestrictScore>0)
		GameServiceOption.lMaxEnterScore=__min(GameServiceOption.lMaxEnterScore,GameServiceOption.lRestrictScore);

	return true;
}

//��������
VOID * CGameServiceManager::CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)
{
	////��������
	//CAndroidUserItemSink * pAndroidUserItemSink=NULL;

	//try
	//{
	//	//��������
	//	pAndroidUserItemSink=new CAndroidUserItemSink();
	//	if (pAndroidUserItemSink==NULL) throw TEXT("����ʧ��");

	//	//��ѯ�ӿ�
	//	void * pObject=pAndroidUserItemSink->QueryInterface(Guid,dwQueryVer);
	//	if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");

	//	return pObject;
	//}
	//catch (...) {}

	////ɾ������
	//SafeDelete(pAndroidUserItemSink);

	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * CreateGameServiceManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	return g_GameServiceManager.QueryInterface(Guid,dwInterfaceVer);
}

//////////////////////////////////////////////////////////////////////////
