#include "StdAfx.h"
#include ".\constantlybutton.h"

CConstantlyButton::CConstantlyButton(void)
{
	m_bSend = FALSE;
}

CConstantlyButton::~CConstantlyButton(void)
{
}

//������Ϣ
VOID CConstantlyButton::OnWindowMovie()
{
	if ( m_bMouseMove && m_bMouseDown )
	{
		if ( m_WaitingTime.GetLapseCount(500) > 0 )
		{
			m_bSend = TRUE;
		}
		if ( m_bSend && m_SendTime.GetLapseCount(30) > 0 )
		{
			//��ť�¼�
			if (m_bEnable==true)
			{
				if (m_pParentWindow==NULL)
				{
					//��ȡ�豸
					ASSERT(m_pVirtualEngine!=NULL);
					CD3DDevice * pD3DDevice=m_pVirtualEngine->GetD3DDevice();

					//��ȡ����
					ASSERT(pD3DDevice!=NULL);
					HWND hWndDevice=pD3DDevice->GetWndDevice();

					//������Ϣ
					ASSERT(hWndDevice!=NULL);
					if (hWndDevice!=NULL) SendMessage(hWndDevice,WM_COMMAND,m_uWindowID,(LPARAM)hWndDevice);
				}
				else
				{
					ASSERT(FALSE);
				}
			}
		}
	}
	else
	{
		m_bSend = FALSE;
	}
	CVirtualButton::OnWindowMovie();
}


//����¼�
VOID CConstantlyButton::OnEventMouse( UINT uMessage, UINT nFlags, INT nXMousePos, INT nYMousePos )
{
	switch (uMessage)
	{
	case WM_LBUTTONDOWN:	//������Ϣ
	case WM_LBUTTONDBLCLK:	//˫����Ϣ
		{
			m_WaitingTime.Initialization();
			m_SendTime.Initialization();
			break;
		}
	}

	CVirtualButton::OnEventMouse( uMessage, nFlags, nXMousePos, nYMousePos );
}

